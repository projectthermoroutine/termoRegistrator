#include <pipe_channel.h>
#include "pipe_channel_details.h"

#include <array>
#include <string>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <atomic>
#include <mutex>
#include <limits>
#include <condition_variable>
#include <cstdint>
#include <vector>
#include <windows.h>
#include <handle_holder.h>
#include <sync_helpers.h>
#include <on_exit.h>
#include <loglib\log.h>

namespace pipe_channel
{
	namespace details
	{
		pipe_connection_holder::pipe_connection_holder(handle_holder & pipe_handle) : _pipe_handle(pipe_handle)
		{
			LOG_STACK();

			if (!_pipe_handle)
				throw std::invalid_argument("pipe_handle is invalid");
		}

		pipe_connection_holder::~pipe_connection_holder()
		{
			LOG_STACK();

			if (!_pipe_handle)
				throw std::invalid_argument("pipe_handle is invalid");

			if (_pipe_handle)
			{
				LOG_TRACE() << "Flushing pipe's buffers.";
				if (!FlushFileBuffers(_pipe_handle.get()))
				{
					const auto last_error = GetLastError();
					LOG_DEBUG() << "FlushFileBuffers failed: " << std::hex << std::showbase << last_error;
				}

				LOG_TRACE() << "Disconnecting pipe.";
				if (!DisconnectNamedPipe(_pipe_handle.get()))
				{
					const auto last_error = GetLastError();
					LOG_DEBUG() << "DisconnectNamedPipe failed: " << std::hex << std::showbase << last_error;
				}
			}
		}

		buffer_t get_message_buffer(const byte_t * buffer, std::size_t buffer_size)
		{
			LOG_STACK();

			LOG_TRACE() << "Getting message buffer for a message with size: " << buffer_size;

			buffer_t in_raw_buffer(sizeof(details::message_size_t));
			*(reinterpret_cast<details::message_size_t*>(in_raw_buffer.data())) = static_cast<details::message_size_t>(buffer_size);

			if (buffer_size != 0)
			{
				if (buffer == nullptr)
					throw std::invalid_argument("buffer is nullptr");

				std::copy_n(buffer, buffer_size, std::back_inserter(in_raw_buffer));
			}

			return in_raw_buffer;
		}

		void read_file(const handle_holder & handle, byte_t * buffer, std::size_t buffer_size)
		{
			LOG_STACK();

			if (buffer == nullptr)
				throw std::invalid_argument("buffer is nullptr");

			if (buffer_size == 0) return;
			if (buffer_size > std::numeric_limits<DWORD>::max())
			{
				throw std::invalid_argument("Buffer size is too big.");
			}

			DWORD bytes_read = 0;
			if (!ReadFile(handle.get(), buffer, static_cast<DWORD>(buffer_size), &bytes_read, NULL))
			{
				auto const last_error = GetLastError();
				if (last_error != ERROR_MORE_DATA)
				{
					LOG_DEBUG() << "ReadFile failed for the pipe: " << std::hex << std::showbase << last_error;
					throw pipe_error(last_error);
				}
			}

			if (bytes_read != buffer_size)
			{
				throw std::runtime_error("The buffer was read partially.");
			}
		}

		void write_file(const handle_holder & handle, const byte_t * buffer, std::size_t buffer_size)
		{
			LOG_STACK();

			if (buffer == nullptr)
				throw std::invalid_argument("buffer is nullptr");

			if (buffer_size == 0) return;
			if (buffer_size > std::numeric_limits<DWORD>::max())
			{
				throw std::invalid_argument("Buffer size is too big.");
			}

			DWORD bytes_written = 0;
			if (!WriteFile(handle.get(), buffer, static_cast<DWORD>(buffer_size), &bytes_written, NULL))
			{
				auto const last_error = GetLastError();
				LOG_DEBUG() << "WriteFile failed for the pipe: " << std::hex << std::showbase << last_error;
				throw pipe_error(last_error);
			}

			if (bytes_written != buffer_size)
			{
				throw std::logic_error("The buffer was written partially.");
			}
		}

		void write_empty_response(const handle_holder & handle)
		{
			LOG_STACK();

			DWORD bytes_written = 0;
			if (!WriteFile(handle.get(), NULL, 0, &bytes_written, NULL))
			{
				auto const last_error = GetLastError();
				LOG_DEBUG() << "WriteFile failed for the pipe: " << std::hex << std::showbase << last_error;
				throw pipe_error(last_error);
			}

			if (bytes_written != 0)
			{
				throw std::logic_error("Unexpected amount of data was written.");
			}
		}

		void process_message(handle_holder & handle, process_message_func_t func)
		{
			LOG_STACK();

			details::message_size_t message_size = 0;
			details::read_file(
				handle,
				reinterpret_cast<byte_t*>(&message_size),
				sizeof(message_size));

			if (message_size > details::max_message_size)
			{
				throw invalid_message("The message is larger than maximum allowed size.");
			}

			DWORD total_bytes_available = 0;
			if (!PeekNamedPipe(handle.get(), NULL, 0, NULL, &total_bytes_available, NULL))
			{
				auto const last_error = GetLastError();
				LOG_DEBUG() << "PeekNamedPipe failed for the pipe: " << std::hex << std::showbase << last_error;
				throw pipe_error(last_error);
			}

			total_bytes_available += sizeof(message_size);

			if ((message_size + sizeof(message_size)) > total_bytes_available)
			{
				throw invalid_message("Message size is larger than number of bytes available in the buffer.");
			}

			if (message_size == 0)
			{
				LOG_TRACE() << "Empty message.";
				write_empty_response(handle);
				return;
			}

			std::unique_ptr<byte_t[]> buffer(new byte_t[message_size]);
			details::read_file(handle, buffer.get(), message_size);

			buffer_t response_buffer(func(buffer.get(), buffer.get() + message_size));

			if (response_buffer.size() > get_max_message_size())
			{
				throw pipe_response_too_large_error();
			}

			buffer_t out_raw_buffer(details::get_message_buffer(response_buffer.data(), response_buffer.size()));

			details::write_file(
				handle,
				out_raw_buffer.data(),
				out_raw_buffer.size());
		}

		std::wstring get_pipe_full_name(const std::wstring & name)
		{
			if (name.empty())
				throw std::invalid_argument("pipe full name is empty");

			return std::wstring(L"\\\\.\\pipe\\") + name;
		}

		class server_channel_impl final
		{
		public:
			server_channel_impl(const std::wstring & name) :
				_name(name),
				_server_loop_running(false),
				_stop_event(sync_helpers::create_basic_event_object(true)),
				_stop_requested(false),
				_run_failed(false)
			{
				LOG_STACK();

				if (name.empty())
					throw std::invalid_argument("name is empty");
			}

			~server_channel_impl()
			{
				LOG_STACK();

				stop_server_loop();
			}

			void run_server_loop(process_message_func_t func, const SECURITY_ATTRIBUTES * sec_attr)
			{
				LOG_STACK();

				bool expected = false;
				if (!_server_loop_running.compare_exchange_strong(expected, true))
				{
					const char * already_running_message = "Processing loop has been running already for the named pipe.";
					LOG_TRACE() << already_running_message;
					throw std::logic_error(already_running_message);
				}

				_run_failed = true;

				utils::on_exit stopped_guard([this] {
					std::lock_guard<std::mutex> lock(_start_stop_mtx);
					_stop_requested = false;
					sync_helpers::reset_event(_stop_event);
					_server_loop_running = false;
					_pipe_ready_event.notify_all();
					LOG_TRACE() << "Setting stopped event.";
					_stopped_event.notify_all();
				});

				std::unique_ptr<utils::on_exit> exit_guard;

				{
					std::lock_guard<std::mutex> lock(_start_stop_mtx);

					exit_guard.reset(new utils::on_exit([this]() {
						LOG_STACK();
						std::lock_guard<std::mutex> lock(_start_stop_mtx);
						_pipe_handle = handle_holder();
					}));

					if (_stop_requested.load())
					{
						LOG_TRACE() << "Stop flag is set. Leave.";
						return;
					}

					const auto pipe_full_name = details::get_pipe_full_name(_name);

					LOG_TRACE() << "Creating named pipe: " << pipe_full_name.c_str();
					auto const pipe_handle = CreateNamedPipeW(
						pipe_full_name.c_str(),
						PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED | WRITE_DAC,
						PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
						PIPE_UNLIMITED_INSTANCES,
						details::pipe_buffer_size,
						details::pipe_buffer_size,
						details::pipe_timeout,
						const_cast<SECURITY_ATTRIBUTES*>(sec_attr));

					if (pipe_handle == INVALID_HANDLE_VALUE)
					{
						auto const last_error = GetLastError();
						LOG_DEBUG() << "CreateNamedPipe failed: " << std::hex << std::showbase << last_error;
						throw pipe_error(last_error);
					}

					_run_failed = false;

					_pipe_handle = handle_holder(pipe_handle);

					LOG_TRACE() << "Pipe is ready: " << pipe_full_name.c_str();
					_pipe_ready_event.notify_all();
				}

				LOG_TRACE() << "Run processing loop for the pipe.";

				while (!_stop_requested)
				{
					try
					{
						if (!processing_cycle_iteration(func))
						{
							LOG_TRACE() << "Stop event is set. Leave.";
							break;
						}
					}
					catch (const std::exception & exc)
					{
						LOG_ERROR() << "Exception was caught inside pipe processing loop: " << exc.what();
					}
					catch (...)
					{
						LOG_ERROR() << "Unknown exception was caught inside pipe processing loop.";
					}
				}
			}

			void stop_server_loop()
			{
				LOG_STACK();

				{
					std::unique_lock<std::mutex> lock(_start_stop_mtx);

					_stop_requested.store(true);
					sync_helpers::set_event(_stop_event);

					LOG_TRACE() << "Waiting for stopped event...";
					_stopped_event.wait(lock, [this] { return !_server_loop_running; });
				}
			}

			bool wait_for_pipe_initialization()
			{
				LOG_STACK();
				std::unique_lock<std::mutex> lock(_start_stop_mtx);
				if (_stop_requested)
				{
					LOG_TRACE() << "Stop was requested. Cannot wait for initialization of the pipe .";
					return false;
				}

				LOG_TRACE() << "Waiting for pipe ready event...";
				_pipe_ready_event.wait(lock, [this] { return _pipe_handle && _server_loop_running; });
				return !_run_failed;
			}

			server_channel_impl(const server_channel_impl &) = delete;
			server_channel_impl & operator = (const server_channel_impl &) = delete;
		private:
			bool processing_cycle_iteration(process_message_func_t func)
			{
				auto const overlapped_event = sync_helpers::create_basic_event_object(false);

				OVERLAPPED overlapped = {};
				overlapped.hEvent = overlapped_event.get();

				LOG_TRACE() << "Connecting to the pipe.";
				if (!ConnectNamedPipe(_pipe_handle.get(), &overlapped))
				{
					auto const last_error = GetLastError();
					if (last_error == ERROR_PIPE_CONNECTED)
					{
						LOG_TRACE() << "The pipe has been connected already.";
						details::pipe_connection_holder connection_holder(_pipe_handle);
						details::process_message(_pipe_handle, func);
						return true;
					}
					else if (last_error != ERROR_IO_PENDING)
					{
						LOG_DEBUG() << "ConnectNamedPipe failed: " << std::hex << std::showbase << last_error;
						throw pipe_error(last_error);
					}
				}

				details::pipe_connection_holder connection_holder(_pipe_handle);

				std::array<HANDLE, 2> wait_handles{ { overlapped_event.get(), _stop_event.get() } };

				LOG_TRACE() << "Waiting for IO completion or stop signal.";
				const auto wait_result = sync_helpers::wait_any(wait_handles.data(), wait_handles.size());
				if (!wait_result.event_raised)
				{
					return false;
				}

				if (wait_result.event_index == 0)
				{
					LOG_TRACE() << "The pipe was connected.";
					if (!HasOverlappedIoCompleted(&overlapped))
					{
						LOG_DEBUG() << "Overlapped IO has not been completed yet.";
						throw std::logic_error("Overlapped IO is expected to be completed.");
					}
					details::process_message(_pipe_handle, func);
					return true;
				}
				
				return false;
			}

			const std::wstring _name;
			std::mutex _start_stop_mtx;
			handle_holder _pipe_handle;
			std::atomic<bool> _server_loop_running;
			std::atomic<bool> _stop_requested;
			std::atomic<bool> _run_failed;
			handle_holder _stop_event;
			std::condition_variable _stopped_event;
			std::condition_variable _pipe_ready_event;
		};
	
		inline server_channel_impl * cast_to_server_channel_impl(void * p)
		{
			if (p == nullptr)
				throw std::invalid_argument("server_channel_impl is nullptr");

			return static_cast<server_channel_impl *>(p);
		}

		class client_channel_impl final
		{
		public:
			client_channel_impl(const std::wstring & channel_name) : _channel_name(channel_name)
			{
				LOG_STACK();
			}

			void send(
				serialization::binary_stream::const_iterator begin,
				serialization::binary_stream::const_iterator end,
				serialization::binary_stream::insert_iterator out_iter)
			{
				LOG_STACK();
				
				if (end < begin)
					throw std::invalid_argument("invalid iterators were passed");

				std::vector<byte_t> buffer;
				std::copy(begin, end, std::back_inserter(buffer));
				const auto result_buffer = call_named_pipe(_channel_name, buffer.data(), buffer.size());
				for (const auto & value : result_buffer)
				{
					*out_iter = value;
					++out_iter;
				}
			}

			client_channel_impl(const client_channel_impl &) = delete;
			client_channel_impl & operator = (const client_channel_impl &) = delete;
		private:
			const std::wstring _channel_name;
		};

		inline client_channel_impl * cast_to_client_channel_impl(void * p)
		{
			if (p == nullptr)
				throw std::invalid_argument("client_channel_impl is nullptr");

			return static_cast<client_channel_impl *>(p);
		}
	} // namespace details

	std::size_t get_max_message_size()
	{
		return details::max_message_size - sizeof(details::message_size_t);
	}

	buffer_t call_named_pipe(
		const std::wstring & name,
		const byte_t * in_buffer,
		std::size_t in_buffer_size)
	{
		LOG_STACK();

		if (details::max_message_size > std::numeric_limits<DWORD>::max())
			throw std::invalid_argument("The passed message is larger than maximum allowed size.");

		if (details::max_message_size + sizeof(details::max_message_size) > std::numeric_limits<DWORD>::max())
			throw std::invalid_argument("The passed message is larger than maximum allowed size.");

		if (name.empty())
			throw std::invalid_argument("The passed pipe name is empty.");

		if (in_buffer_size == 0) return buffer_t();

		if (in_buffer == nullptr)
			throw std::invalid_argument("The passed in_buffer is nullptr.");

		if (in_buffer_size > (details::max_message_size + sizeof(details::max_message_size)))
		{
			throw std::invalid_argument("The passed message is larger than maximum allowed size.");
		}

		buffer_t in_raw_buffer(details::get_message_buffer(in_buffer, in_buffer_size));
		buffer_t out_buffer(details::max_message_size);

		LOG_TRACE() << "Calling named pipe: '" << name.c_str() << "', data size:" << in_buffer_size;

		DWORD bytes_read = 0;
		if (!CallNamedPipeW(
			details::get_pipe_full_name(name).c_str(),
			const_cast<byte_t*>(in_raw_buffer.data()),
			static_cast<DWORD>(in_raw_buffer.size()),
			out_buffer.data(),
			static_cast<DWORD>(out_buffer.size()),
			&bytes_read,
			details::pipe_timeout))
		{
			auto const last_error = GetLastError();
			if (last_error == ERROR_MORE_DATA)
			{
				LOG_TRACE() << "More data is available in the pipe.";
				throw invalid_message("Size of the response is larger than a buffer for it.");
			}
			else
			{
				LOG_DEBUG() << "CallNamedPipe failed: " << std::hex << std::showbase << last_error;
				throw pipe_error(last_error);
			}
		}

		LOG_TRACE() << "Got response from the named pipe: '" << name.c_str() << "', response size:" << bytes_read;

		if (bytes_read > out_buffer.size())
		{
			throw invalid_message("Unexpected amount of data was read from the pipe.");
		}

		out_buffer.resize(bytes_read);

		if (bytes_read < sizeof(details::message_size_t))
		{
			throw invalid_message("Invalid response was read from the pipe.");
		}

		details::message_size_t message_size = *(reinterpret_cast<details::message_size_t*>(out_buffer.data()));
		if ((message_size + sizeof(details::message_size_t)) != bytes_read)
		{
			throw invalid_message("Size of the respone does not match size of the message.");
		}

		if (message_size == 0)
		{
			out_buffer.clear();
		}
		else
		{
			out_buffer.erase(out_buffer.begin(), out_buffer.begin() + sizeof(details::message_size_t));
		}

		out_buffer.shrink_to_fit();
		return out_buffer;
	}

	void wait_for_named_pipe(const std::wstring & name)
	{
		LOG_STACK();

		if (name.empty())
			throw std::invalid_argument("The passed pipe name is empty.");

		LOG_TRACE() << "Waiting for the named pipe: " << name;

		if (!WaitNamedPipeW(details::get_pipe_full_name(name).c_str(), NMPWAIT_USE_DEFAULT_WAIT))
		{
			const auto last_error = GetLastError();
			LOG_DEBUG() << "WaitNamedPipe failed: " << std::hex << std::showbase << last_error;
			throw pipe_error(last_error);
		}
	}

	server_channel::server_channel(const std::wstring & name) : _p_impl(new details::server_channel_impl(name))
	{
	}

	server_channel::~server_channel()
	{
		const auto p_impl = _p_impl;
		_p_impl = nullptr;
		if (p_impl)
		{
			delete details::cast_to_server_channel_impl(p_impl);
		}
	}

	void server_channel::run_server_loop(process_message_func_t func, const SECURITY_ATTRIBUTES * sec_attr)
	{
		details::cast_to_server_channel_impl(_p_impl)->run_server_loop(func, sec_attr);
	}

	void server_channel::stop_server_loop()
	{
		details::cast_to_server_channel_impl(_p_impl)->stop_server_loop();
	}

	bool server_channel::wait_for_pipe_initialization()
	{
		return details::cast_to_server_channel_impl(_p_impl)->wait_for_pipe_initialization();
	}

	pipe_error::pipe_error(error_t error) : std::runtime_error(get_error_message(error)), _error(error) {}

	std::string pipe_error::get_error_message(error_t error)
	{
		std::ostringstream ss;
		ss << "Pipe operation error: " << std::hex << std::showbase << error;
		return ss.str();
	}

	pipe_response_too_large_error::pipe_response_too_large_error() : std::runtime_error("The response's size is greater than the pipe's buffer.") {}

	invalid_message::invalid_message(const char * message) : std::runtime_error(message) {}

	client_channel::client_channel(const std::wstring & channel_name) :
		_p_impl(new details::client_channel_impl(channel_name))
	{
	}

	client_channel::~client_channel()
	{
		auto p_impl = details::cast_to_client_channel_impl(_p_impl);
		_p_impl = nullptr;
		delete p_impl;
	}

	void client_channel::send(
		serialization::binary_stream::const_iterator begin,
		serialization::binary_stream::const_iterator end,
		serialization::binary_stream::insert_iterator out_iter)
	{
		details::cast_to_client_channel_impl(_p_impl)->send(begin, end, out_iter);
	}

} // namespace pipe_channel