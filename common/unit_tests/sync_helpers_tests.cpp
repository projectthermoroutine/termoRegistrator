#include "CppUnitTest.h"
#include <unit_tests_common.h>
#include <sync_helpers.h>
#include <atomic>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace unit_tests_common;

namespace common_lib_tests_project
{
	TEST_CLASS(sync_helpers_test)
	{
	public:
		TEST_METHOD(call_once_test)
		{
			checked_execute([] {
				std::atomic<unsigned int> counter1 = 0;
				std::atomic<unsigned int> counter2 = 0;
				const auto once_func1 = [&counter1] { ++counter1; };
				const auto once_func2 = [&counter2] { ++counter2; };

				sync_helpers::once_flag flag0;
				sync_helpers::once_flag flag1;
				sync_helpers::once_flag flag2;

				const unsigned num_of_inits_1 = 10;
				const unsigned num_of_inits_2 = 5;

				for (unsigned int n = 0; n != num_of_inits_1; ++n)
				{
					sync_helpers::call_once(flag1, once_func1);
				}

				for (unsigned int n = 0; n != num_of_inits_2; ++n)
				{
					sync_helpers::call_once(flag2, once_func2);
				}

				Assert::IsTrue(counter1.load() == 1);
				Assert::IsTrue(counter2.load() == 1);
			});
		}

		TEST_METHOD(call_once_test_args)
		{
			checked_execute([] {
				std::atomic<unsigned int> counter = 0;

				sync_helpers::once_flag flag1;
				sync_helpers::once_flag flag2;
				sync_helpers::once_flag flag3;
				sync_helpers::once_flag flag4;

				const unsigned int magic_num = 0x1234567;

				const auto once_func1 = [&counter, magic_num](unsigned int a)
				{
					Assert::IsTrue(a == magic_num);
					++counter;
				};

				const auto once_func2 = [&counter, magic_num](unsigned int a, unsigned int b)
				{
					Assert::IsTrue(a == magic_num);
					Assert::IsTrue(b == magic_num);
					++counter;
				};

				const auto once_func3 = [&counter, magic_num](unsigned int a, unsigned int b, unsigned int c)
				{
					Assert::IsTrue(a == magic_num);
					Assert::IsTrue(b == magic_num);
					Assert::IsTrue(c == magic_num);
					++counter;
				};

				const auto once_func4 = [&counter, magic_num](unsigned int a, unsigned int b, unsigned int c, unsigned int d)
				{
					Assert::IsTrue(a == magic_num);
					Assert::IsTrue(b == magic_num);
					Assert::IsTrue(c == magic_num);
					Assert::IsTrue(d == magic_num);
					++counter;
				};

				sync_helpers::call_once(flag1, once_func1, magic_num);
				sync_helpers::call_once(flag2, once_func2, magic_num, magic_num);
				sync_helpers::call_once(flag3, once_func3, magic_num, magic_num, magic_num);
				sync_helpers::call_once(flag4, once_func4, magic_num, magic_num, magic_num, magic_num);

				Assert::IsTrue(counter.load() == 4);
			});
		}

		TEST_METHOD(call_once_exception_test)
		{
			checked_execute([] {
				sync_helpers::once_flag flag;
				const auto once_func = [] { throw std::exception(); };
				Assert::ExpectException<std::exception>([&flag, &once_func] {sync_helpers::call_once(flag, once_func); });
			});
		}

		TEST_METHOD(call_once_reenter_test)
		{
			checked_execute([] {
				std::atomic<unsigned int> counter = 0;
				sync_helpers::once_flag flag;
				const auto once_func_empty = [&counter] { ++counter; };
				const auto once_func = [&flag, &once_func_empty] { sync_helpers::call_once(flag, once_func_empty); };
				sync_helpers::call_once(flag, once_func);
				Assert::IsTrue(counter.load() == 1);
			});
		}

		TEST_METHOD(call_once_mth_test)
		{
			checked_execute([] {
				std::atomic<unsigned int> counter = 0;
				const auto once_func = [&counter] { ++counter; };

				sync_helpers::once_flag flag;
				
				const unsigned int number_of_threads = 16 * std::thread::hardware_concurrency();
				const auto test_func = [&flag, &once_func]
				{
					sync_helpers::call_once(flag, once_func);
				};
				unit_tests_common::mth_run_n(number_of_threads, test_func);
				Assert::IsTrue(counter.load() == 1);
			});
		}

		TEST_METHOD(call_once_exception_mth_test)
		{
			checked_execute([] {
				const auto once_func = [] { throw std::exception(); };

				sync_helpers::once_flag flag;

				const unsigned int number_of_threads = 16 * std::thread::hardware_concurrency();
				const auto test_func = [&flag, &once_func]
				{
					sync_helpers::call_once(flag, once_func);
				};
				Assert::ExpectException<std::exception>([number_of_threads, &test_func] { unit_tests_common::mth_run_n(number_of_threads, test_func); });
			});
		}

		TEST_METHOD(call_once_reenter_mth_test)
		{
			checked_execute([] {
				sync_helpers::once_flag flag;
				std::atomic<unsigned int> counter = 0;
				const auto once_func_empty = [&counter] { ++counter; };
				const auto once_func = [&flag, &once_func_empty] { sync_helpers::call_once(flag, once_func_empty); };
				
				const unsigned int number_of_threads = 16 * std::thread::hardware_concurrency();
				const auto test_func = [&flag, &once_func]
				{
					sync_helpers::call_once(flag, once_func);
				};
				unit_tests_common::mth_run_n(number_of_threads, test_func);
				
				Assert::IsTrue(counter.load() == 1);
			});
		}
	};
}