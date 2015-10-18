#pragma once

#include <common\sync_helpers.h>
#include <common\socket_holder.h>
#include <thread>

#include <algorithm>
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <utility>
#include <cstdlib>
#include <functional>
#include <thread>

#include <winsock2.h>
#include <Ws2tcpip.h>

namespace test_packets_udp_source
{
	template<typename TMessage>
	using data_gen_func_t = std::function<bool(TMessage&)>;

	unsigned long
		inet_addr_safed(const std::wstring & ip)
	{
		IN_ADDR i_addr;

		InetPton(AF_INET, ip.c_str(), &i_addr);

		return i_addr.S_un.S_addr;

	}


	class test_udp_server
	{

	public:
		test_udp_server(const std::wstring & ip, unsigned short port) :
			_port(port), _ip(ip), 
			_closing_requested(false), 
			_stoped(true),
			_owner_TID(std::this_thread::get_id())
		{
			{
				socket_handle_holder socket(::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP));
				if (!socket) {
					throw std::runtime_error("Could not create Windows Socket.");
				}

				_socket.swap(socket);
			}
		}
		template<typename TMessage>
		void start_server(typename data_gen_func_t<TMessage> data_gen_func, unsigned int messages_count,size_t delay = 0)
		{
			_server_TID = std::this_thread::get_id();
			_stoped = false;
			sockaddr_in receiver_addr;
			receiver_addr.sin_family = AF_INET;
			receiver_addr.sin_addr.s_addr = inet_addr_safed(_ip);
			receiver_addr.sin_port = htons(_port);

			auto socket = _socket.get();

			BOOLEAN param = TRUE;
			if (setsockopt(socket, SOL_SOCKET, SO_BROADCAST, (char *)&param, sizeof(param)) == SOCKET_ERROR)
			{
				_stoped = true;
				const auto wsa_result = WSAGetLastError();
				LOG_DEBUG() << "Could not setting broadcast socket. Error: " << std::hex << std::showbase << wsa_result;
				throw std::runtime_error("Could not setting broadcast socket.");
			}

			const int BufLen = sizeof(TMessage);
			TMessage message;
			char *SendBuf = reinterpret_cast<char*>(&message);
			unsigned int count_messages_sended = 0;

			auto _interval = std::chrono::milliseconds(delay);

			bool big_interval = false;
			bool small_interval = false;
			if (delay > 1000000){
				big_interval = true;
			}
			if (delay < 1){
				small_interval = true;
			}


			auto deadline = std::chrono::steady_clock::now();

			while (!_closing_requested)
			{
				RtlZeroMemory(SendBuf, BufLen);

				if (data_gen_func(message))
				{
					auto result = sendto(socket,
						SendBuf, BufLen, 0, (SOCKADDR *)& receiver_addr, sizeof(receiver_addr));
					if (result == SOCKET_ERROR) {
						const auto wsa_result = WSAGetLastError();
						throw std::runtime_error("Could not send datagram.");
					}
					count_messages_sended++;
				}
				if (big_interval)
				{
					auto small_deadline = deadline;
					while (deadline + _interval >= small_deadline)
					{
						std::this_thread::sleep_until(small_deadline += std::chrono::milliseconds(500));
						if (_closing_requested){
							_stoped = true;
							return;
						}
					}

					deadline = std::chrono::steady_clock::now();

				}
				else
					if (!small_interval)
						std::this_thread::sleep_until(deadline += _interval);

				if (messages_count > 0 && count_messages_sended == messages_count)
					break;
			}

			_stoped = true;
		}

		void stop_server()
		{
			if (!_stoped){
				_closing_requested = true;

				if (_owner_TID == _server_TID)
					return;
				while (!_stoped)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}
			}
		}

	private:
		unsigned short _port;
		std::wstring _ip;
		socket_handle_holder _socket;
		volatile bool _closing_requested;
		volatile bool _stoped;

		std::thread::id _owner_TID;
		std::thread::id _server_TID;

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
