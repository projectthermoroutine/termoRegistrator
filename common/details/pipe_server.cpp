#include <thread>

#include <pipe_server.h>
#include <pipe_channel.h>
#include <rpc_light.h>

#include <loglib\log.h>

namespace rpc_light
{
	namespace details
	{
		class pipe_server_impl final
		{
		public:
			pipe_server_impl(
				const std::wstring & pipe_name,
				rpc_light::function_call_dispatcher && function_call_dispatcher, 
				const SECURITY_ATTRIBUTES * sec_attr) :
				_pipe_server_channel(pipe_name),
				_function_call_dispatcher(std::move(function_call_dispatcher))
			{
				LOG_STACK();

				_pipe_channel_loop = std::thread([this, &sec_attr]()
				{
					LOG_STACK();
					try
					{
						this->_pipe_server_channel.run_server_loop(
							[this](const pipe_channel::byte_t * first, const pipe_channel::byte_t * last) { return this->process_management_message(first, last); },
							sec_attr
						);
					}
					catch (const std::exception & exc)
					{
						LOG_ERROR() << "An exception was caught inside pipe server loop: " << exc.what();
						throw;
					}
					catch (...)
					{
						LOG_ERROR() << "An unknown exception was caught inside pipe server loop.";
						throw;
					}
				});

				if (!_pipe_server_channel.wait_for_pipe_initialization())
				{
					LOG_DEBUG() << "Could not initialize server's pipe.";
					throw std::runtime_error("Server pipe initialization is failed.");
				}
			}

			~pipe_server_impl()
			{
				LOG_STACK();

				_pipe_server_channel.stop_server_loop();

				if (_pipe_channel_loop.joinable())
				{
					_pipe_channel_loop.join();
				}
			}
		private:
			pipe_channel::buffer_t process_management_message(const pipe_channel::byte_t * first, const pipe_channel::byte_t * last)
			{
				LOG_STACK();

				if (first == nullptr || last == nullptr)
					throw std::invalid_argument("The passed message pointers are invalid.");

				serialization::binary_stream out_stream;

				{
					serialization::binary_stream in_stream;
					std::copy(first, last, in_stream.insert_iter()); // TODO : eliminate redundant copy operation

					rpc_light::execute_function(
						_function_call_dispatcher,
						in_stream,
						out_stream,
						[this](std::string name, rpc_light::function_args_num_t num, serialization::binary_stream & in, serialization::binary_stream & out) {this->_function_call_dispatcher.call_function(name, num, in, out); }
					);
				}

				pipe_channel::buffer_t out_buffer;
				std::copy(out_stream.cbegin(), out_stream.cend(), std::back_inserter(out_buffer));

				return out_buffer;
			}

			pipe_channel::server_channel _pipe_server_channel;
			std::thread _pipe_channel_loop;
			rpc_light::function_call_dispatcher _function_call_dispatcher;
		};

		pipe_server_impl * cast_to_pipe_server_impl(void * p)
		{
			if (p == nullptr)
				throw std::invalid_argument("pipe_server_impl is nullptr");

			return static_cast<pipe_server_impl*>(p);
		}
	}

	pipe_server::pipe_server(
		const std::wstring & pipe_name,
		rpc_light::function_call_dispatcher && function_call_dispatcher,
		const SECURITY_ATTRIBUTES * sec_attr) :
		_p_impl(new details::pipe_server_impl(pipe_name, std::move(function_call_dispatcher), sec_attr))
	{
	}

	pipe_server::~pipe_server()
	{
		auto p_impl = details::cast_to_pipe_server_impl(_p_impl);
		_p_impl = nullptr;
		delete p_impl;
	}
}