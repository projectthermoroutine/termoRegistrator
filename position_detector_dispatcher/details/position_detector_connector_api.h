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

			int get_message(void * const buffer, const uint32_t buffer_size, const HANDLE stop_event);

			virtual void close() {}

			device_connector_api(const device_connector_api &) = delete;
			device_connector_api & operator = (const device_connector_api &) = delete;
		private:

			int wait_message();
			int recievefrom_message(
				void * const buffer,
				const uint32_t buffer_size
				);
			int recievefrom_message_async(
				void * const buffer,
				const uint32_t buffer_size,
				const HANDLE stop_event
				);
		private:
			unsigned short _port;
			std::wstring _ip4_address;
			std::wstring _i_ip4_address;
			socket_handle_holder _socket;

			fd_set _fds;
			timeval _timeout;
			sockaddr_in _SenderAddr;
			int _SenderAddrSize;


			WSADATA _wsaData;
			WSABUF _DataBuf;
			WSAOVERLAPPED _Overlapped;

			WSA_event_handle_holder _wsa_event;
			std::unique_ptr<BYTE[]> _data_buf;
			const static int  _data_buf_size;

			std::wstring _log_arg;
		};

	} // namespace details
} // namespace position_detector