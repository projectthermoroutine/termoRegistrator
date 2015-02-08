#pragma once

#include <cstdint>
#include <handle_holder.h>

#include <pipe_channel.h>

namespace pipe_channel
{
	namespace details
	{
		using message_size_t = std::uint32_t;

		// TODO: move to configuration
		const unsigned int pipe_buffer_size = 65536; // in bytes
		const unsigned int pipe_timeout = 1000; // in ms
		const message_size_t max_message_size = 1024 * 1024; // 1 MB

		class pipe_connection_holder final
		{
		public:
			pipe_connection_holder(handle_holder & pipe_handle);
			~pipe_connection_holder();
			pipe_connection_holder(const pipe_connection_holder &) = delete;
			pipe_connection_holder & operator = (const pipe_connection_holder &) = delete;
		private:
			handle_holder & _pipe_handle;
		};

		buffer_t get_message_buffer(const byte_t * buffer, std::size_t buffer_size);
		void read_file(const handle_holder & handle, byte_t * buffer, std::size_t buffer_size);
		void write_file(const handle_holder & handle, const byte_t * buffer, std::size_t buffer_size);
		void process_message(handle_holder & handle, process_message_func_t func);
		std::wstring get_pipe_full_name(const std::wstring & name);
	} //namespace details
}