#pragma once

#include <cstdint>
#include <string>
#include <stdexcept>
#include <vector>
#include <functional>
#include <windows.h>
#include <serialization.h>

namespace pipe_channel
{
	using error_t = decltype(GetLastError());
	using byte_t = std::uint8_t;
	using buffer_t = std::vector<byte_t>;
	using process_message_func_t = std::function<buffer_t(const byte_t *, const byte_t *)>;

	class pipe_error : public std::runtime_error
	{
	public:
		pipe_error(error_t error);

		inline error_t get_error_code() const
		{
			return _error;
		}
	private:
		std::string pipe_error::get_error_message(error_t error);
		error_t _error;
	};

	class pipe_response_too_large_error : public std::runtime_error
	{
	public:
		pipe_response_too_large_error();
	};

	class invalid_message : public std::runtime_error
	{
	public:
		invalid_message(const char * message);
	};

	std::size_t get_max_message_size();
	
	buffer_t call_named_pipe(
		const std::wstring & name,
		const byte_t * in_buffer,
		std::size_t in_buffer_size);

	void wait_for_named_pipe(const std::wstring & name);

	class server_channel final
	{
	public:
		server_channel(const std::wstring & name);
		~server_channel();

		void run_server_loop(process_message_func_t func, const SECURITY_ATTRIBUTES * sec_attr = nullptr);
		void stop_server_loop();

		bool wait_for_pipe_initialization();

		server_channel(const server_channel &) = delete;
		server_channel & operator = (const server_channel &) = delete;
	private:
		void * _p_impl;
	};

	class client_channel final
	{
	public:
		client_channel(const std::wstring & channel_name);
		~client_channel();

		void send(
			serialization::binary_stream::const_iterator begin,
			serialization::binary_stream::const_iterator end,
			serialization::binary_stream::insert_iterator out_iter);

		bool check_channel();

		client_channel(const client_channel &) = delete;
		client_channel & operator = (const client_channel &) = delete;
	private:
		void * _p_impl;
	};

}