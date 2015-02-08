#include "CppUnitTest.h"

#include <loglib\log.h>
#include <climits>
#include <thread>
#include <vector>

#include <common\unit_tests_common.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace
{
	void run_logging_in_cycle()
	{
		for (int n = 0; n < 1000; ++n)
		{
			LOG_DEBUG() << n;
		}

		LOG_ERROR() << "Flush all messages.";
	}

	void init_logger()
	{
		const std::wstring work_dir(L".");
		const std::wstring main_config_file_name(L"log.conf");
		const std::wstring dev_log_config_file_name(L"log4cplus.properties");
		const std::wstring log_file_name(L"test_log");

		logger::initialize(
			work_dir,
			main_config_file_name,
			dev_log_config_file_name,
			work_dir,
			log_file_name);
	}
}

namespace loglib_test_project
{		
	using namespace unit_tests_common;

	TEST_CLASS(basic_test)
	{
	public:		
		TEST_METHOD(uninitialized_logging)
		{
			checked_execute([]
			{
				LOG_STACK();
				LOG_TRACE() << "Trace";
				LOG_DEBUG() << "Debug";
				LOG_INFO() << "Info";
				LOG_ERROR() << "Error";
				LOG_FATAL() << "Fatal";
			});
		}

		TEST_METHOD(empty_func_logging)
		{
			checked_execute([]
			{
				init_logger();
				LOG_STACK();
			});
		}

		TEST_METHOD(test_logging)
		{
			checked_execute([]
			{
				init_logger();
				LOG_STACK();

				LOG_TRACE() << "Trace";
				LOG_DEBUG() << "Debug";
				LOG_INFO() << "Info";
				LOG_ERROR() << "Error";
				LOG_FATAL() << "Fatal";

				logger::severity severities[] = {
					logger::severity::trace,
					logger::severity::debug,
					logger::severity::info,
					logger::severity::warn,
					logger::severity::error,
					logger::severity::fatal
				};

				for (auto sev : severities)
				{
					LOG(sev) << "Test for severity: " << static_cast<int>(sev);
				}
			});
		}

		TEST_METHOD(test_logging_in_cycle)
		{
			checked_execute([]
			{
				init_logger();
				LOG_STACK();
				run_logging_in_cycle();
			});
		}

		TEST_METHOD(test_logging_for_types)
		{
			checked_execute([]
			{
				init_logger();
				LOG_STACK();

				LOG_DEBUG() << 'a';
				LOG_DEBUG() << L'a';
				LOG_DEBUG() << "abc";
				LOG_DEBUG() << L"abc";
				LOG_DEBUG() << 10;
				LOG_DEBUG() << -10;
				LOG_DEBUG() << 10U;
				LOG_DEBUG() << 10L;
				LOG_DEBUG() << -10L;
				LOG_DEBUG() << 10UL;
				LOG_DEBUG() << true;
				LOG_DEBUG() << false;
				LOG_DEBUG() << 10.10;
				LOG_DEBUG() << -10.10;
				LOG_DEBUG() << LLONG_MIN;
				LOG_DEBUG() << LLONG_MAX;
				LOG_DEBUG() << ULLONG_MAX;
			});
		}

		TEST_METHOD(test_logging_mth)
		{
			checked_execute([]
			{
				init_logger();
				LOG_STACK();

				const unsigned int number_of_threads = 100;
				std::vector<std::thread> threads;
				for (unsigned int n = 0; n < number_of_threads; ++n)
				{
					threads.push_back(std::thread([]
					{
						LOG_STACK();
						init_logger();
						run_logging_in_cycle();
					}));
				}

				for (auto & th : threads)
				{
					th.join();
				}
			});
		}
	};
}