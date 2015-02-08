#include "CppUnitTest.h"
#include <cstdlib>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <unit_tests_common.h>
#include <pipe_channel.h>
#include <common\xor_shift.h>
#include <common\on_exit.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace unit_tests_common;

namespace common_lib_tests_project
{
	const std::size_t test_message_size = 100;
	const unsigned int number_of_iterations = 1000;

	std::wstring get_channel_random_name()
	{
		const auto counter = GetTickCount64();
		return std::to_wstring(utils::xor_shift_next(counter));
	}

	void fill_test_buffer(pipe_channel::buffer_t & buffer)
	{
		unit_tests_common::generate_random_values<pipe_channel::byte_t>(std::back_inserter(buffer), test_message_size);
	}

	pipe_channel::buffer_t process_message_no_call(const pipe_channel::byte_t *, const pipe_channel::byte_t *)
	{
		Assert::Fail();
		return pipe_channel::buffer_t();
	}

	pipe_channel::buffer_t process_message_empty_response(const pipe_channel::byte_t *, const pipe_channel::byte_t *)
	{
		return pipe_channel::buffer_t();
	}

	pipe_channel::buffer_t process_message_oversized_response(const pipe_channel::byte_t *, const pipe_channel::byte_t *)
	{
		return pipe_channel::buffer_t(pipe_channel::get_max_message_size() + 1);
	}
		
	pipe_channel::buffer_t process_message_exception(const pipe_channel::byte_t *, const pipe_channel::byte_t *)
	{
		throw std::runtime_error("Test");
	}
	
	pipe_channel::buffer_t process_message(const pipe_channel::byte_t * first, const pipe_channel::byte_t * last)
	{
		const auto distance = std::distance(first, last);
		Assert::IsTrue(distance >= 0);

		pipe_channel::buffer_t out_buffer;
		std::copy(first, last, std::back_inserter(out_buffer));

		return out_buffer;
	}

	void test_pipe_call_for_size(const std::wstring & channel_name, std::size_t size)
	{
		std::vector<pipe_channel::byte_t> in_buffer;
		unit_tests_common::generate_random_values<pipe_channel::byte_t>(std::back_inserter(in_buffer), size);
		for (;;)
		{
			try
			{
				const auto out_buffer = pipe_channel::call_named_pipe(channel_name, in_buffer.data(), in_buffer.size());
				Assert::IsTrue(in_buffer == out_buffer);
				break;
			}
			catch (const pipe_channel::pipe_error & exc)
			{
				const auto error_code = exc.get_error_code();
				if (error_code == ERROR_PIPE_BUSY)
				{
					for (;;)
					{
						try
						{
							pipe_channel::wait_for_named_pipe(channel_name);
							break;
						}
						catch (const pipe_channel::pipe_error & exc)
						{
							if (exc.get_error_code() == ERROR_SEM_TIMEOUT)
							{
								std::this_thread::sleep_for(std::chrono::seconds(1));
							}
							else
							{
								throw;
							}
						}
					}
				}
				else if (error_code == ERROR_PIPE_NOT_CONNECTED)
				{
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
				else
				{
					throw;
				}
			}
		}
	}

	void test_call_after_fail(pipe_channel::process_message_func_t func)
	{
		const std::wstring channel_name(get_channel_random_name());

		std::vector<pipe_channel::byte_t> in_buffer;
		fill_test_buffer(in_buffer);

		pipe_channel::server_channel server_channel(channel_name);

		{
			std::thread loop_thread([&server_channel, &func]() { server_channel.run_server_loop(func); });
			Assert::IsTrue(loop_thread.joinable());

			utils::on_exit exit_guard([&server_channel, &loop_thread]
			{
				server_channel.stop_server_loop();
				loop_thread.join();
			});

			Assert::IsTrue(server_channel.wait_for_pipe_initialization());

			try
			{
				pipe_channel::call_named_pipe(channel_name, in_buffer.data(), in_buffer.size());
				Assert::Fail();
			}
			catch (const pipe_channel::pipe_error & exc)
			{
				Assert::IsTrue(exc.get_error_code() == ERROR_PIPE_NOT_CONNECTED);
			}
			catch (...)
			{
				Assert::Fail();
			}
		}

		{
			std::thread loop_thread([&server_channel]() { server_channel.run_server_loop(process_message); });
			Assert::IsTrue(loop_thread.joinable());

			utils::on_exit exit_guard([&server_channel, &loop_thread]
			{
				server_channel.stop_server_loop();
				loop_thread.join();
			});

			Assert::IsTrue(server_channel.wait_for_pipe_initialization());

			const auto out_buffer = pipe_channel::call_named_pipe(channel_name, in_buffer.data(), in_buffer.size());
			Assert::IsTrue(in_buffer == out_buffer);
		}
	}

	pipe_channel::buffer_t create_channel_and_call(const pipe_channel::buffer_t & in_buffer, pipe_channel::process_message_func_t func)
	{
		const std::wstring channel_name(get_channel_random_name());
		pipe_channel::server_channel server_channel(channel_name);
		std::thread loop_thread([&server_channel, &func]() { server_channel.run_server_loop(func); });

		utils::on_exit exit_guard([&server_channel, &loop_thread]
		{
			server_channel.stop_server_loop();
			loop_thread.join();
		});

		Assert::IsTrue(server_channel.wait_for_pipe_initialization());

		return pipe_channel::call_named_pipe(channel_name, in_buffer.data(), in_buffer.size());
	}

	TEST_CLASS(pipe_channel_test)
	{
	public:
		TEST_METHOD(server_channel_create_test)
		{
			checked_execute([] {
				for (unsigned int n = 0; n != number_of_iterations; ++n)
				{
					const std::wstring channel_name(get_channel_random_name());
					pipe_channel::server_channel server_channel(channel_name);
				}
			});
		}

		TEST_METHOD(server_channel_run_twice)
		{
			checked_execute([] {
				const std::wstring channel_name(get_channel_random_name());
				pipe_channel::server_channel server_channel(channel_name);
				for (unsigned int n = 0; n != number_of_iterations; ++n)
				{
					std::thread loop_thread([&server_channel]() { server_channel.run_server_loop(process_message_no_call); });
					Assert::IsTrue(loop_thread.joinable());
					utils::on_exit exit_guard([&server_channel, &loop_thread]
					{
						server_channel.stop_server_loop();
						loop_thread.join();
					});

					Assert::IsTrue(server_channel.wait_for_pipe_initialization());
					Assert::ExpectException<std::logic_error>([&server_channel] {server_channel.run_server_loop(process_message_no_call); });
				}
			});
		}

		TEST_METHOD(server_channel_run_stop_loop_test)
		{
			checked_execute([] {
				const std::wstring channel_name(get_channel_random_name());
				pipe_channel::server_channel server_channel(channel_name);
	
				for (unsigned int n = 0; n != number_of_iterations; ++n)
				{
					std::thread loop_thread([&server_channel]() { server_channel.run_server_loop(process_message_no_call); });
					Assert::IsTrue(loop_thread.joinable());
					server_channel.stop_server_loop();
					loop_thread.join();
				}
			});
		}

		TEST_METHOD(server_channel_run_wait_init_stop_loop_test)
		{
			checked_execute([] {
				for (unsigned int n = 0; n != number_of_iterations; ++n)
				{
					const std::wstring channel_name(get_channel_random_name());
					pipe_channel::server_channel server_channel(channel_name);
					std::thread loop_thread([&server_channel]() { server_channel.run_server_loop(process_message_no_call); });
					Assert::IsTrue(loop_thread.joinable());
					utils::on_exit exit_guard([&server_channel, &loop_thread]
					{
						server_channel.stop_server_loop();
						loop_thread.join();
					});
					Assert::IsTrue(server_channel.wait_for_pipe_initialization());
				}
			});
		}

		TEST_METHOD(server_channel_run_stop_twice)
		{
			checked_execute([] {
				const std::wstring channel_name(get_channel_random_name());
				pipe_channel::server_channel server_channel(channel_name);
				for (unsigned int n = 0; n != number_of_iterations; ++n)
				{
					std::thread loop_thread([&server_channel]() { server_channel.run_server_loop(process_message_no_call); });
					Assert::IsTrue(loop_thread.joinable());
					utils::on_exit exit_guard([&server_channel, &loop_thread]
					{
						server_channel.stop_server_loop();
						loop_thread.join();
					});
					server_channel.stop_server_loop();
				}
			});
		}

		TEST_METHOD(server_channel_run_wait_stop_twice)
		{
			checked_execute([] {
				for (unsigned int n = 0; n != number_of_iterations; ++n)
				{
					const std::wstring channel_name(get_channel_random_name());
					pipe_channel::server_channel server_channel(channel_name);
					std::thread loop_thread([&server_channel]() { server_channel.run_server_loop(process_message_no_call); });
					Assert::IsTrue(loop_thread.joinable());
					utils::on_exit exit_guard([&server_channel, &loop_thread]
					{
						server_channel.stop_server_loop();
						loop_thread.join();
					});
					Assert::IsTrue(server_channel.wait_for_pipe_initialization());
					server_channel.stop_server_loop();
				}
			});
		}

		TEST_METHOD(server_channel_call_test)
		{
			checked_execute([] {
				const std::wstring channel_name(get_channel_random_name());
				pipe_channel::server_channel server_channel(channel_name);
				std::thread loop_thread([&server_channel]() { server_channel.run_server_loop(process_message); });
				
				Assert::IsTrue(loop_thread.joinable());
				utils::on_exit exit_guard([&server_channel, &loop_thread]
				{
					server_channel.stop_server_loop();
					loop_thread.join();
				});

				std::vector<std::size_t> buffer_sizes;
				unit_tests_common::generate_test_numbers_range(
					1u,
					pipe_channel::get_max_message_size(),
					std::back_inserter(buffer_sizes));

				Assert::IsTrue(server_channel.wait_for_pipe_initialization());

				for (const auto size : buffer_sizes)
				{
					test_pipe_call_for_size(channel_name, size);
				}
			});
		}

		TEST_METHOD(server_channel_call_multi_client_test)
		{
			checked_execute([] {
				const std::wstring channel_name(get_channel_random_name());
				pipe_channel::server_channel server_channel(channel_name);
				std::thread loop_thread([&server_channel]() { server_channel.run_server_loop(process_message); });
				Assert::IsTrue(loop_thread.joinable());

				std::vector<std::thread> threads;

				utils::on_exit exit_guard([&server_channel, &loop_thread, &threads]
				{
					for (auto & thread : threads)
					{
						if (thread.joinable())
						{
							thread.join();
						}
					}

					server_channel.stop_server_loop();
					if (loop_thread.joinable())
					{
						loop_thread.join();
					}
				});

				const auto max_message_size = pipe_channel::get_max_message_size() / 8;
				const auto num_of_testing_threads = std::thread::hardware_concurrency() * 8;
				
				Assert::IsTrue(server_channel.wait_for_pipe_initialization());
				
				std::mutex fail_mutex;
				std::atomic<bool> failed(false);
				std::string what;

				for (std::size_t n = 0; n != num_of_testing_threads; ++n)
				{
					threads.emplace_back([&channel_name, &max_message_size, &failed, &what, &fail_mutex] {
						try
						{
							if (!failed.load())
							{
								test_pipe_call_for_size(channel_name, max_message_size);
							}
						}
						catch (const std::exception & exc)
						{
							std::lock_guard<std::mutex> lock(fail_mutex);
							bool expected = false;
							if (failed.compare_exchange_strong(expected, true))
							{
								what = exc.what();
							}
						}
						catch (...)
						{
							std::lock_guard<std::mutex> lock(fail_mutex);
							bool expected = false;
							if (failed.compare_exchange_strong(expected, true))
							{
								what = "Unknown exception";
							}
						}
					});
				}

				for (auto & thread : threads)
				{
					if (thread.joinable())
					{
						thread.join();
					}
				}

				Assert::IsFalse(failed.load(), convert_string(what).c_str());
			});
		}

		TEST_METHOD(server_channel_empty_response_test)
		{
			checked_execute([] {
				std::vector<pipe_channel::byte_t> in_buffer;
				fill_test_buffer(in_buffer);
				const auto out_buffer = create_channel_and_call(in_buffer, process_message_empty_response);
				Assert::IsTrue(out_buffer.empty());
			});
		}

		TEST_METHOD(server_channel_oversized_response_test)
		{
			checked_execute([] {
				pipe_channel::buffer_t in_buffer;
				fill_test_buffer(in_buffer);
				try
				{
					const auto out_buffer = create_channel_and_call(in_buffer, process_message_oversized_response);
					Assert::Fail();
				}
				catch (const pipe_channel::pipe_error & exc)
				{
					Assert::IsTrue(exc.get_error_code() == ERROR_PIPE_NOT_CONNECTED);
				}
				catch (...)
				{
					Assert::Fail();
				}
			});
		}

		TEST_METHOD(server_channel_call_after_oversized_message_test)
		{
			checked_execute([] {
				test_call_after_fail(process_message_oversized_response);
			});
		}

		TEST_METHOD(server_channel_call_after_exception_test)
		{
			checked_execute([] {
				test_call_after_fail(process_message_exception);
			});
		}
	};
}