#pragma once

#include <common\sync_helpers.h>
#include <common\handle_holder.h>
#include <thread>

#include <algorithm>
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <utility>
#include <cstdlib>
#include <functional>

#include <position_detector_common\position_detector_connector.h>

namespace client_pd_dispatcher_test_project
{
	using namespace unit_tests_common;
	using namespace position_detector;

	const unsigned int count_messages = 1000;

	//template<typename TMessage>
	//using data_gen_func_t = TMessage(*)();

	template<typename TMessage>
	using data_gen_func_t =  std::function<TMessage()>;


	class test_udp_server
	{

	public:
		test_udp_server(const char* ip, unsigned short port) :
			_port(port), _ip(ip)
		{
			{
				socket_handle_holder socket(::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP));
				if (!socket) {
					const auto wsa_result = WSAGetLastError();
					throw position_detector_connector_exception(wsa_result, "Could not create Windows Socket.");
				}

				_socket.swap(socket);

			}

			handle_holder tmp_closing(sync_helpers::create_basic_event_object(true));
			_closing_requested.swap(tmp_closing);
		}
		template<typename TMessage>
		void start_server(typename data_gen_func_t<TMessage> data_gen_func, unsigned int messages_count,size_t delay = 0)
		{
			sockaddr_in receiver_addr;
			receiver_addr.sin_family = AF_INET;
			receiver_addr.sin_addr.s_addr = inet_addr(_ip.c_str());
			receiver_addr.sin_port = htons(_port);

			auto socket = _socket.get();
			const int BufLen = sizeof(TMessage);
			TMessage message;
			char *SendBuf = reinterpret_cast<char*>(&message);
			unsigned int count_messages_sended = 0;

			auto _interval = std::chrono::microseconds(delay);
			auto deadline = std::chrono::steady_clock::now();


			while (!sync_helpers::is_event_set(_closing_requested))
			{
				message = data_gen_func();
				auto result = sendto(socket,
					SendBuf, BufLen, 0, (SOCKADDR *)& receiver_addr, sizeof (receiver_addr));
				if (result == SOCKET_ERROR) {
					const auto wsa_result = WSAGetLastError();
					throw position_detector_connector_exception(wsa_result, "Could not send datagram.");
				}
				count_messages_sended++;
				std::this_thread::sleep_until(deadline += _interval);
				if (count_messages_sended == messages_count)
					break;
			}
		}

		void stop_server()
		{
			sync_helpers::set_event(_closing_requested);
		}

	private:
		unsigned short _port;
		std::string _ip;
		socket_handle_holder _socket;
		handle_holder _closing_requested;

	};


	class scoped_WSA final
	{
	public:
		scoped_WSA()
		{
			LOG_STACK();
			LOG_TRACE() << "Starting Windows Sockets ";
			WSADATA wsaData;
			auto result = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (result != NO_ERROR) {
				LOG_DEBUG() << "Could not start up Windows Sockets: error: " << std::hex << std::showbase << result;
				std::ostringstream ss;
				ss << "Could not start up Windows Sockets: error: " << std::hex << std::showbase << result;
				throw std::runtime_error(ss.str());
			}
		}
		~scoped_WSA()
		{
			LOG_STACK();
			WSACleanup();
		}

		scoped_WSA(const scoped_WSA &) = delete;
		scoped_WSA & operator = (const scoped_WSA &) = delete;
	};

}
