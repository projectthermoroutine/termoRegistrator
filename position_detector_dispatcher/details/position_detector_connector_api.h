#pragma once

#include <memory>
#include <cstdint>

#include <common\socket_holder.h>

#include <position_detector_common\position_detector_connector.h>
namespace position_detector
{
	namespace details
	{

		class device_connector_api : public connector_api
		{
			static const long wait_delay_sec = 4;
		public:
			device_connector_api(const std::vector<std::string>& settings);

			int get_message(get_message_struct * const buffer, const packet_size_t buffer_size);

			virtual void close() {}

			device_connector_api(const device_connector_api &) = delete;
			device_connector_api & operator = (const device_connector_api &) = delete;
		private:

			int wait_message();
			int recievefrom_message(
				get_message_struct * const buffer,
				const packet_size_t buffer_size);
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