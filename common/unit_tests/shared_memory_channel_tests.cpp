#include "CppUnitTest.h"
#include <unit_tests_common.h>
#include <common\shared_memory_channel.h>
#include <common\sync_helpers.h>
#include <common\path_helpers.h>
#include <common\security.h>

#include <loglib/details/cpplogger.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace unit_tests_common;

namespace common_lib_tests_project
{
	using namespace channels;


	typedef ULONG32 counter_t;
	typedef ULONG64 timestamp_t;
	typedef ULONG32 speed_t;

	struct test_packet_t
	{
		counter_t counter;
		timestamp_t timestamp;
		speed_t speed;
	};


	using sh_m_ptr = std::unique_ptr<shared_memory_channel>;

	shared_memory_channel * create_shared_memory_channel(uint32_t id)
	{
		LOG_STACK();
		std::wstring name_suffix = L"_test_sh_m_n";
		unsigned int memory_size = sizeof(test_packet_t);
		std::wstring shared_memory_name;
		sync_helpers::create_random_name(shared_memory_name, true);
		shared_memory_name += name_suffix;

		return new shared_memory_channel(id, shared_memory_name, memory_size, ::channels::perfomance_tag);
	}


	shared_memory_channel * open_shared_memory_channel(const shared_memory_channel & src_sh_m)
	{
		LOG_STACK();

		std::vector<std::string> settings{ string_utils::convert_wchar_to_utf8(src_sh_m.shared_memory_name()), std::to_string(src_sh_m.shared_memory_size()), string_utils::convert_wchar_to_utf8(src_sh_m.event_name()) };
		return new shared_memory_channel(settings);
	}

	TEST_CLASS(shared_memory_channel_test)
	{
	public:

		TEST_CLASS_INITIALIZE(shared_memory_channel_test_initialize)
		{
			checked_execute([]
			{
				{
					const auto& module_path = security::get_current_module_path();
					const auto& module_dir = path_helpers::get_directory_path(module_path);

					const auto logger_config_name = std::wstring(L"test_config.xml");

					logger::settings settings;
					settings.level = logger::level::trace;
					settings.max_backup_index = 2;
					settings.max_buffer_size = 2 * 1024 * 1024;
					settings.max_file_size = settings.max_buffer_size;
					settings.use_developer_log = true;

					cpplogger::write_logger_settings(path_helpers::concatenate_paths(module_dir, logger_config_name), settings);

					logger::initialize(module_dir,
						logger_config_name,
						module_dir,
						path_helpers::get_base_name(security::get_current_module_path()), false, {});
				}

			});
		}

		TEST_METHOD(sh_m_stress_test)
		{
			checked_execute([] 
			{

				constexpr std::uint32_t id = 1;
				sh_m_ptr src_sh_m{ create_shared_memory_channel(id) };

				Assert::IsTrue(src_sh_m.operator bool());

				sh_m_ptr dest_sh_m{ open_shared_memory_channel(*src_sh_m) };

				Assert::IsTrue(dest_sh_m.operator bool());

				handle_holder stop_event = sync_helpers::create_basic_manual_reset_event();
				bool stop_flag{ false };

				std::thread gen_thread([&]
				{
					test_packet_t test_packet{};

					for (int step = 0; step < 1000000; ++step)
					{
						test_packet.counter = step + 1;

						sync_helpers::is_event_set(stop_event);
						src_sh_m->send_data(reinterpret_cast<BYTE *>(&test_packet), sizeof(test_packet_t));
					}

					stop_flag = true;
					sync_helpers::set_event(stop_event);

				});


				test_packet_t dst_test_packet{};

				counter_t prev_counter{};

				while (!sync_helpers::is_event_set(stop_event))
				//while (!stop_flag)
				{
					auto res = dest_sh_m->get_message(reinterpret_cast<BYTE *>(&dst_test_packet), sizeof(test_packet_t), stop_event.get());

					if (res) 
					{
						Assert::IsTrue(dst_test_packet.counter > prev_counter);
						prev_counter = dst_test_packet.counter;
						LOG_DEBUG() << L"Counter: " << prev_counter;
					}
					else
						Assert::IsTrue(sync_helpers::is_event_set(stop_event));

					sync_helpers::is_event_set(stop_event);

				}


				if (gen_thread.joinable())
					gen_thread.join();

			});
		}
	};
}