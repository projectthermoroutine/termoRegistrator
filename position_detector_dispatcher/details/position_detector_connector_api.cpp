#include "position_detector_connector_api.h"
#include <loglib\log.h>

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <vector>
namespace position_detector
{
	namespace details
	{

		int                  
			join_source_group(int sd, uint32_t grpaddr,
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
					LOG_DEBUG() << "Could not get network address. Error: " << std::hex << std::showbase << wsa_result;
					throw position_detector_connector_exception(wsa_result, "Could not get network address for '" + ip + "'");
				}
				if (res == SOCKET_ERROR)
				{
					LOG_DEBUG() << ip << " is not a valid IPv4 dotted-decimal string." << std::hex << std::showbase;
					throw position_detector_connector_exception(0, ip + " is not a valid IPv4 dotted-decimal string.");
				}
			}

			return i_addr.S_un.S_addr;

		}


#	pragma warning(push)
#	pragma warning(disable: 4127) // conditional expression is constant

		device_connector_api::device_connector_api(const std::vector<std::string>& settings)
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

			{

				LOG_TRACE() << "Creating UDP socket";
				//socket_handle_holder socket(::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP));
				socket_handle_holder socket(::WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, 0, 0, WSA_FLAG_MULTIPOINT_C_LEAF | WSA_FLAG_MULTIPOINT_D_LEAF));

				if (!socket) {

					const auto wsa_result = WSAGetLastError();
					LOG_DEBUG() << "Could not create Windows Socket. Error: " << std::hex << std::showbase << wsa_result;
					throw position_detector_connector_exception(wsa_result, "Could not create Windows Socket.");
				}

				auto i_addr = inet_addr_safed(i_ip);

				int i = 1;
				if (setsockopt(socket.get(), SOL_SOCKET, SO_REUSEADDR, (char *)&i, sizeof(i)) == SOCKET_ERROR)
				{
					const auto wsa_result = WSAGetLastError();
					LOG_DEBUG() << "Could not setting broadcast socket. Error: " << std::hex << std::showbase << wsa_result;
					throw position_detector_connector_exception(wsa_result, "Could not setting broadcast socket.");
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
					LOG_DEBUG() << "Could not bind socket. Error: " << std::hex << std::showbase << wsa_result;
					throw position_detector_connector_exception(wsa_result, "Could not bind socket.");
				}

				//if (is_multicast)
				{
					if (join_source_group(socket.get(), inet_addr_safed(_ip4_address), i_addr) == SOCKET_ERROR)
					{
						const auto wsa_result = WSAGetLastError();
						LOG_DEBUG() << "Could not join_source_group. Error: " << std::hex << std::showbase << wsa_result;
						throw position_detector_connector_exception(wsa_result, "Could not join_source_group.");
					}
				}

				_socket.swap(socket);

				LOG_TRACE() << "Socket created and binded successfully.";
			}

			FD_ZERO(&_fds);
			auto socket = _socket.get();
			FD_SET(socket, &_fds);
			_timeout.tv_sec = wait_delay_sec;
			_timeout.tv_usec = 0;
			_SenderAddrSize = sizeof (sockaddr_in);


		}
#	pragma warning(pop)

		int device_connector_api::get_message(get_message_struct * const buffer, const packet_size_t buffer_size)
		{
			auto result = wait_message();
			if (result > 0){
				result = recievefrom_message(buffer, buffer_size);
				if (result == 0){
					return 0;
				}
				return result;
			}

			return result;
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
				LOG_DEBUG() << "Unexpected result code was returned from select: " << std::hex << std::showbase << wsa_result;
				throw position_detector_connector_exception(wsa_result, "Could not get a message from the UDP port.");
			}

			if (result < SOCKET_ERROR){

				LOG_DEBUG() << "Unexpected result was returned from wait_message: " << std::hex << std::showbase << result;
				throw position_detector_connector_exception(result, "Unexpected result was returned from select function");
			}

			return result;

		}
		int device_connector_api::recievefrom_message(
			get_message_struct * const buffer,
			const packet_size_t buffer_size)
		{
			auto result = recvfrom(_socket.get(),
				reinterpret_cast<char*>(buffer), buffer_size, 0, (SOCKADDR *)& _SenderAddr, &_SenderAddrSize);

			if (result == SOCKET_ERROR) {
				const auto wsa_result = WSAGetLastError();
				LOG_DEBUG() << "Unexpected result code was returned from recvfrom: " << std::hex << std::showbase << wsa_result;
				throw position_detector_connector_exception(wsa_result, "Could not get a message from the UDP port.");
			}

			if (result < SOCKET_ERROR){

				LOG_DEBUG() << "Unexpected result was returned from recvfrom: " << std::hex << std::showbase << result;
				throw position_detector_connector_exception(result, "Unexpected result was returned from select function");
			}

			return result;
		}
#	pragma warning(pop)
	}
}