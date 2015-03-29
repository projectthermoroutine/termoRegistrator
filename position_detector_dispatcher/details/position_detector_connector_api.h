#pragma once

#include <Windows.h>

#include <algorithm>
#include <memory>
#include <array>
#include <cstdint>

#include <common\handle_holder.h>
#include <common\sync_helpers.h>
#include <loglib\log.h>

#include <position_detector_common\position_detector_connector.h>

namespace position_detector
{
	namespace details
	{

		class device_connector_api : public connector_api
		{
			static const long wait_delay_sec = 4;
		public:
#	pragma warning(push)
#	pragma warning(disable: 4127) // conditional expression is constant

			device_connector_api(const std::vector<std::string>& settings)
			{
				LOG_STACK();

				std::string ip = settings[0];
				std::string str_port = settings[1];
				_ip4_address = ip;

				if (ip.empty())
					throw std::invalid_argument("The passed argument ip4 address can't be empty");
				if (str_port.empty())
					throw std::invalid_argument("The passed argument port can't be empty");

				_port = (unsigned short)std::stoul(str_port);

				if (_port < 1000)
					throw std::invalid_argument("The passed argument port can't be less 1000");

				{

					LOG_TRACE() << "Creating UDP socket";
					socket_handle_holder socket(::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP));
					if (!socket) {

						const auto wsa_result = WSAGetLastError();
						LOG_DEBUG() << "Could not create Windows Socket. Error: " << std::hex << std::showbase << wsa_result;
						throw position_detector_connector_exception(wsa_result, "Could not create Windows Socket.");
					}

					ULONG s_address = INADDR_ANY;

					if (_ip4_address != "0.0.0.0" && _ip4_address != "224.5.6.1")
					{
						s_address = inet_addr(_ip4_address.c_str());
					}

					sockaddr_in service;
					service.sin_family = AF_INET;
					service.sin_addr.s_addr = s_address;
					service.sin_port = htons(_port);

					LOG_TRACE() << "Binding UDP socket: port: " << _port;

					const auto result = bind(socket.get(), (SOCKADDR *)&service, sizeof (service));
					if (result == SOCKET_ERROR) {

						const auto wsa_result = WSAGetLastError();
						LOG_DEBUG() << "Could not bind socket. Error: " << std::hex << std::showbase << wsa_result;
						throw position_detector_connector_exception(wsa_result, "Could not bind socket.");
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

			int get_message(get_message_struct * const buffer, const packet_size_t buffer_size)
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

			virtual void close() {}


			device_connector_api(const device_connector_api &) = delete;
			device_connector_api & operator = (const device_connector_api &) = delete;
		private:

#	pragma warning(push)
#	pragma warning(disable: 4127) // conditional expression is constant
			int wait_message()
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
			int recievefrom_message(
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

		private:
			unsigned short _port;
			std::string _ip4_address;
			socket_handle_holder _socket;

			fd_set _fds;
			timeval _timeout;
			sockaddr_in _SenderAddr;
			int _SenderAddrSize;
		};

	} // namespace details
} // namespace position_detector