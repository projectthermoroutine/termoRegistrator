#include "CppUnitTest.h"
#include <unit_tests_common.h>

#include <stdexcept>
#include <string>
#include <memory>
#include <security.h>
#include <sync_helpers.h>
#include <handle_holder.h>
#include <pipe_channel.h>
#include <on_exit.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace unit_tests_common;

namespace common_lib_tests_project
{
	TEST_CLASS(security_tests)
	{
	public:
		TEST_METHOD(security_attributes_test_event)
		{
			checked_execute([] {
				std::wstring event_name;
				handle_holder event_handle;
				const auto event_creation_func = [&event_name, &event_handle](const SECURITY_ATTRIBUTES & sec_attr) -> void
				{
					event_handle = sync_helpers::create_random_name_event(event_name, &sec_attr);
				};

				security::process_sec_attributes_for_internal_ipc(event_creation_func, EVENT_MODIFY_STATE | SYNCHRONIZE);

				sync_helpers::set_event(event_handle);
				Assert::IsTrue(sync_helpers::is_event_set(event_handle));
				sync_helpers::reset_event(event_handle);
				Assert::IsFalse(sync_helpers::is_event_set(event_handle));
			});
		}

		TEST_METHOD(security_attributes_test_pipe)
		{
			checked_execute([] {

				pipe_channel::process_message_func_t pipe_message_func = [](const pipe_channel::byte_t *, const pipe_channel::byte_t *) -> pipe_channel::buffer_t
				{
					return pipe_channel::buffer_t();
				};

				auto ready_event = sync_helpers::create_basic_event_object(true);
				std::shared_ptr<pipe_channel::server_channel> p_server_channel;
				const std::wstring pipe_name = std::to_wstring(GetTickCount64());
				const auto pipe_creation_func = [&p_server_channel, pipe_name, pipe_message_func, &ready_event](const SECURITY_ATTRIBUTES & sec_attr) -> void
				{
					p_server_channel = std::make_shared<pipe_channel::server_channel>(pipe_name);
					sync_helpers::set_event(ready_event);
					p_server_channel->run_server_loop(pipe_message_func, &sec_attr);
				};

				std::thread test_thread;
				utils::on_exit exit_guard([&test_thread, &p_server_channel]
				{
					if (p_server_channel) p_server_channel->stop_server_loop();
					if (test_thread.joinable()) test_thread.join();
				});

				test_thread = std::thread([pipe_creation_func] { security::process_sec_attributes_for_internal_ipc(pipe_creation_func, GENERIC_READ | GENERIC_WRITE); });
				sync_helpers::wait(ready_event);
				p_server_channel->wait_for_pipe_initialization();
			});
		}
	};
}