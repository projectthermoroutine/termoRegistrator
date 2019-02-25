
#define NOMINMAX

#include<common\log_and_throw.h>
#include "position_detector_connector_api.h"
#include <loglib\log.h>

#include <error_lib\win32_error_codes.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <vector>

namespace position_detector
{
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
					const auto wsa_result = WSAGetLastError();
					LOG_AND_THROW(position_detector_connector_exception(win32::make_error_code(wsa_result), "inet_pton", string_utils::convert_utf8_to_wchar(ip))) << L"Could not get network address for ip: " << ip;
				}
				if (res == 0)
				{
					LOG_AND_THROW(position_detector_connector_exception(win32::win32_errc::error_invalid_data, "inet_pton", string_utils::convert_utf8_to_wchar(ip))) << ip << " is not a valid IPv4 dotted-decimal string";
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
			_ip4_address = ip;

			if (ip.empty())
				throw std::invalid_argument("The passed argument ip4 address can't be empty");
			if (i_ip.empty())
				throw std::invalid_argument("The passed argument interface ip4 address can't be empty");
			if (str_port.empty())
				throw std::invalid_argument("The passed argument port can't be empty");

			_port = (unsigned short)std::stoul(str_port);

			if (_port < 1000)
				throw std::invalid_argument("The passed argument port can't be less 1000");


			//WSA_event_handle_holder wsa_event(::WSACreateEvent());
			WSA_event_handle_holder wsa_event(::CreateEvent(nullptr,FALSE,FALSE,nullptr));
			if (!wsa_event) {
//				const auto wsa_result = WSAGetLastError();
				LOG_AND_THROW(position_detector_connector_exception(win32::get_last_error_code(), "CreateEvent")) << "Could not create WSA Event object";
			}

			_wsa_event.swap(wsa_event);

			ZeroMemory((PVOID)&_Overlapped, sizeof(WSAOVERLAPPED));
			_Overlapped.hEvent = _wsa_event.get();

			{

				LOG_TRACE() << "Creating UDP socket";
				socket_handle_holder socket(::WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, 0, 0, WSA_FLAG_MULTIPOINT_C_LEAF | WSA_FLAG_MULTIPOINT_D_LEAF | WSA_FLAG_OVERLAPPED));

				if (!socket) {

					const auto wsa_result = WSAGetLastError();
					LOG_AND_THROW(position_detector_connector_exception(win32::make_error_code(wsa_result), "WSASocket")) << "Could not create Windows Socket";
				}

				auto i_addr = inet_addr_safed(i_ip);

				int i = 1;
				if (setsockopt(socket.get(), SOL_SOCKET, SO_REUSEADDR, (char *)&i, sizeof(i)) == SOCKET_ERROR)
				{
					const auto wsa_result = WSAGetLastError();
					LOG_AND_THROW(position_detector_connector_exception(win32::make_error_code(wsa_result), "setsockopt")) << "Could not setting broadcast socket";
				}


				sockaddr_in service;
				memset(&service, 0, sizeof(service));
				service.sin_family = AF_INET;
				service.sin_addr.s_addr = i_addr;
				service.sin_port = htons(_port);

				LOG_TRACE() << "Binding UDP socket: port: " << _port;

				const auto result = bind(socket.get(), (SOCKADDR *)&service, sizeof (service));
				if (result == SOCKET_ERROR) {

					const auto wsa_result = WSAGetLastError();
					LOG_AND_THROW(position_detector_connector_exception(win32::make_error_code(wsa_result), "bind", std::to_wstring(_port))) << "Could not bind socket";
				}

				//if (is_multicast)
				{
					if (join_source_group((int)socket.get(), inet_addr_safed(_ip4_address), i_addr) == SOCKET_ERROR)
					{
						const auto wsa_result = WSAGetLastError();
						LOG_AND_THROW(position_detector_connector_exception(win32::make_error_code(wsa_result), "join_source_group", string_utils::convert_utf8_to_wchar(_ip4_address))) << "Could not join_source_group";
					}
				}

				_socket.swap(socket);

				LOG_TRACE() << "Socket created and binded successfully.";
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
				auto rc = ::WSARecvFrom(_socket.get(),
					&_DataBuf,
					1,
					&BytesRecv,
					&Flags,
					(SOCKADDR *)& _SenderAddr,
					&_SenderAddrSize, &_Overlapped, NULL);

				if (rc != 0)
				{
					const auto wsa_error = WSAGetLastError();
					if (wsa_error != WSA_IO_PENDING)
					{
						LOG_DEBUG() << "WSARecvFrom failed with error: " << std::hex << std::showbase << wsa_error;
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
				if (rc == WSA_WAIT_FAILED) {
					const auto wsa_result = WSAGetLastError();
					LOG_DEBUG() << "Unexpected result code was returned from WSAWaitForMultipleEvents: " << std::hex << std::showbase << wsa_result;
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
					LOG_DEBUG() << "Unexpected result was returned from WSAWaitForMultipleEvents: " << rc;
					return 0;
				}

				rc = WSAGetOverlappedResult(_socket.get(), &_Overlapped, &BytesRecv,
					FALSE, &Flags);
				if (rc == FALSE) {

					const auto wsa_result = WSAGetLastError();
					LOG_DEBUG() << "Unexpected result code was returned from WSARecvFrom: " << std::hex << std::showbase << wsa_result;
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
				const auto wsa_result = WSAGetLastError();
				LOG_AND_THROW(position_detector_connector_exception(win32::make_error_code(wsa_result), "select")) << "Unexpected result code was returned from select";
			}

			if (result < SOCKET_ERROR){

				LOG_AND_THROW(position_detector_connector_exception(win32::make_error_code(result), "select")) << "Unexpected result code was returned from select";
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
				const auto wsa_result = WSAGetLastError();
				LOG_AND_THROW(position_detector_connector_exception(win32::make_error_code(wsa_result), "recvfrom")) << "Unexpected result code was returned from recvfrom";
			}

			if (result < SOCKET_ERROR){

				LOG_AND_THROW(position_detector_connector_exception(win32::make_error_code(result), "recvfrom")) << "Unexpected result code was returned from recvfrom";
			}

			return result;
		}
#	pragma warning(pop)
	}
}