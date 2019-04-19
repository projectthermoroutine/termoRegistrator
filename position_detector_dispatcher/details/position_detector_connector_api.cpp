
#define NOMINMAX

#include<common\log_and_throw.h>
#include "position_detector_connector_api.h"
#include <loglib\log.h>

#include <error_lib\win32_error.h>
#include <error_lib\win32_error_codes.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <vector>

namespace position_detector
{
	using namespace std::literals;

	namespace connector 
	{
		std::error_code get_last_error_code();
		std::error_code get_wsa_last_error_code();

		namespace exception
		{
			::common::application_exception by_last_error();
			::common::application_exception by_last_error(std::string  func, std::wstring_view arg_view = {});
			::common::application_exception by_last_error(std::wstring func, std::wstring_view arg_view = {});

			::common::application_exception by_wsa_last_error();
			::common::application_exception by_wsa_last_error(std::string  func, std::wstring_view arg_view = {});
			::common::application_exception by_wsa_last_error(std::wstring func, std::wstring_view arg_view = {});

		}//namespace exception
	}//namespace connector

	namespace details
	{

		int                  
			join_source_group(SOCKET sd, uint32_t grpaddr,
			uint32_t iaddr)
		{
				struct ip_mreq imr;
				memset(&imr, 0, sizeof(imr));

				imr.imr_multiaddr.s_addr = grpaddr;
				imr.imr_interface.s_addr = iaddr;
				return setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&imr, sizeof(imr));
		}

		int
			leave_source_group(int sd, uint32_t grpaddr,
			uint32_t srcaddr, uint32_t iaddr)
		{
				struct ip_mreq_source imr;

				imr.imr_multiaddr.s_addr = grpaddr;
				imr.imr_sourceaddr.s_addr = srcaddr;
				imr.imr_interface.s_addr = iaddr;
				return setsockopt(sd, IPPROTO_IP, IP_DROP_SOURCE_MEMBERSHIP, (char *)&imr, sizeof(imr));
			}


		unsigned long
			inet_addr_safed(const std::string & ip)
		{
			IN_ADDR i_addr;

			auto res = inet_pton(AF_INET, ip.c_str(), &i_addr);
			if (res != 1)
			{
				if (res == SOCKET_ERROR)
				{
					LOG_AND_THROW(connector::exception::by_wsa_last_error(L"inet_pton", string_utils::convert_utf8_to_wchar(ip))) << L"Could not get network address for ip: "sv << ip;
				}
				if (res == 0)
				{
					LOG_AND_THROW(connector::exception::by_error(static_cast<int>(win32::win32_errc::error_invalid_data), "inet_pton", string_utils::convert_utf8_to_wchar(ip))) << ip << L" is not a valid IPv4 dotted-decimal string"sv;
				}
			}

			return i_addr.S_un.S_addr;

		}


		const int  device_connector_api::_data_buf_size = 4112;

#	pragma warning(push)
#	pragma warning(disable: 4127) // conditional expression is constant

		device_connector_api::device_connector_api(const std::vector<std::string>& settings):
			_data_buf(std::make_unique<BYTE[]>(_data_buf_size))
		{
			LOG_STACK();

			std::string ip = settings[0];
			std::string i_ip = settings[1];
			std::string str_port = settings[2];
			_ip4_address = string_utils::convert_utf8_to_wchar(ip);
			_i_ip4_address = string_utils::convert_utf8_to_wchar(i_ip);

			if (ip.empty())
				throw std::invalid_argument("The passed argument ip4 address can't be empty");
			if (i_ip.empty())
				throw std::invalid_argument("The passed argument interface ip4 address can't be empty");
			if (str_port.empty())
				throw std::invalid_argument("The passed argument port can't be empty");

			_port = (unsigned short)std::stoul(str_port);

			if (_port < 1000)
				throw std::invalid_argument("The passed argument port can't be less 1000");


			_log_arg = L"i_ip: " + _i_ip4_address + L", ip: " + _ip4_address + L", port: " + std::to_wstring(_port);


			WSA_event_handle_holder wsa_event(::CreateEventW(nullptr,FALSE,FALSE,nullptr));
			if (!wsa_event) {
				LOG_AND_THROW(connector::exception::by_last_error("CreateEvent")) << "Could not create WSA Event object";
			}

			_wsa_event.swap(wsa_event);

			ZeroMemory((PVOID)&_Overlapped, sizeof(WSAOVERLAPPED));
			_Overlapped.hEvent = _wsa_event.get();

			{

				LOG_TRACE() << "Creating UDP socket";
				socket_handle_holder socket(::WSASocketW(AF_INET, SOCK_DGRAM, IPPROTO_UDP, 0, 0, WSA_FLAG_MULTIPOINT_C_LEAF | WSA_FLAG_MULTIPOINT_D_LEAF | WSA_FLAG_OVERLAPPED));

				if (!socket) {
					LOG_AND_THROW(connector::exception::by_wsa_last_error("WSASocket")) << L"Could not create Windows Socket"sv;
				}

				auto i_addr = inet_addr_safed(i_ip);

				int i = 1;
				if (setsockopt(socket.get(), SOL_SOCKET, SO_REUSEADDR, (char *)&i, sizeof(i)) == SOCKET_ERROR)
				{
					LOG_AND_THROW(connector::exception::by_wsa_last_error("setsockopt")) << L"Could not setting broadcast socket"sv;
				}

				sockaddr_in service;
				memset(&service, 0, sizeof(service));
				service.sin_family = AF_INET;
				service.sin_addr.s_addr = i_addr;
				service.sin_port = htons(_port);

				LOG_TRACE() << L"Binding UDP socket: port: "sv << _port;

				const auto result = bind(socket.get(), (SOCKADDR *)&service, sizeof (service));
				if (result == SOCKET_ERROR) {

					LOG_AND_THROW(connector::exception::by_wsa_last_error("bind", _log_arg)) << L"Could not bind socket"sv;
				}

				//if (is_multicast)
				{
					if (join_source_group((int)socket.get(), inet_addr_safed(ip), i_addr) == SOCKET_ERROR)
					{
						LOG_AND_THROW(connector::exception::by_wsa_last_error("join_source_group", _log_arg)) << L"Could not join_source_group"sv;
					}
				}

				_socket.swap(socket);

				LOG_TRACE() << L"Socket created and binded successfully. ["sv << _log_arg << L']';
			}

			_SenderAddrSize = sizeof(sockaddr_in);

			_DataBuf.len = _data_buf_size;
			_DataBuf.buf = reinterpret_cast<decltype(_DataBuf.buf)>(_data_buf.get());

			FD_ZERO(&_fds);
			auto socket = _socket.get();
			FD_SET(socket, &_fds);
			_timeout.tv_sec = wait_delay_sec;
			_timeout.tv_usec = 0;


		}
#	pragma warning(pop)

		int device_connector_api::get_message(void * const buffer, const uint32_t buffer_size, const HANDLE stop_event)
		{

			return recievefrom_message_async(buffer, buffer_size, stop_event);
		}

		int device_connector_api::recievefrom_message_async(
			void * const buffer,
			const uint32_t buffer_size,
			const HANDLE stop_event
			)
		{
			LOG_STACK();

			bool wait = true;
			DWORD BytesRecv = 0;
			DWORD Flags = 0;


			{
				int rc = ::WSARecvFrom(_socket.get(),
					&_DataBuf,
					1,
					&BytesRecv,
					&Flags,
					(SOCKADDR *)& _SenderAddr,
					&_SenderAddrSize, &_Overlapped, NULL);

				if (rc != 0)
				{
					if (WSAGetLastError() != WSA_IO_PENDING)
					{
						LOG_WARN() << connector::exception::by_wsa_last_error("WSARecvFrom", _log_arg).wwhat();
						return 0;
					}
				}
				else
					wait = false;
			}

			if (wait)
			{
				HANDLE events[2] = { _Overlapped.hEvent, stop_event };
				auto rc = WSAWaitForMultipleEvents(2, events, FALSE, INFINITE, TRUE);
				if (rc == WSA_WAIT_FAILED)
				{
					LOG_WARN() << connector::exception::by_wsa_last_error("WSAWaitForMultipleEvents", _log_arg).wwhat();
					return 0;
				}

				if (rc >= WAIT_OBJECT_0 && rc < WAIT_OBJECT_0 + 2)
				{
					auto event_index = static_cast<std::size_t>(rc - WAIT_OBJECT_0);
					if (event_index == 1){

						LOG_DEBUG() << "Stop was requested.";
						return 0;
					}
				}
				else
				{
					LOG_DEBUG() << L"Unexpected result was returned from WSAWaitForMultipleEvents: "sv << rc;
					return 0;
				}

				rc = WSAGetOverlappedResult(_socket.get(), &_Overlapped, &BytesRecv, FALSE, &Flags);
				if (rc == FALSE) 
				{
					LOG_WARN() << connector::exception::by_wsa_last_error("WSAGetOverlappedResult", _log_arg).wwhat();
					return 0;
				}

			}

			auto count_data = std::min(static_cast<packet_size_t>(_data_buf_size), buffer_size);
			count_data = std::min(static_cast<DWORD>(count_data), BytesRecv);
			std::memcpy(buffer, _data_buf.get(), count_data);

			return count_data;
		}

#	pragma warning(push)
#	pragma warning(disable: 4127) // conditional expression is constant
		int device_connector_api::wait_message()
		{

			FD_SET(_socket.get(), &_fds);

			// Return value:
			// -1: error occurred
			// 0: timed out
			// > 0: data ready to be read
			auto result = select(0, &_fds, 0, 0, &_timeout);

			if (result == SOCKET_ERROR)
			{
				LOG_AND_THROW(connector::exception::by_wsa_last_error("select")) << L"Unexpected result code was returned from select"sv;
			}

			if (result < SOCKET_ERROR){

				LOG_AND_THROW(connector::exception::by_error(result, "select")) << L"Unexpected result code was returned from select"sv;
			}

			return result;

		}
		int device_connector_api::recievefrom_message(
			void * const buffer,
			const uint32_t buffer_size)
		{
			auto result = recvfrom(_socket.get(),
				reinterpret_cast<char*>(buffer), buffer_size, 0, (SOCKADDR *)& _SenderAddr, &_SenderAddrSize);

			if (result == SOCKET_ERROR) {
				LOG_AND_THROW(connector::exception::by_wsa_last_error("recvfrom")) << "Unexpected result code was returned from recvfrom";
			}

			if (result < SOCKET_ERROR){
				LOG_AND_THROW(connector::exception::by_error(result, "recvfrom")) << "Unexpected result code was returned from recvfrom";
			}

			return result;
		}
#	pragma warning(pop)
	}
}