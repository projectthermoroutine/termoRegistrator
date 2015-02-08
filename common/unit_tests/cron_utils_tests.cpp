/////////////////////////////////////
// (C) 2014 ООО "Код Безопасности"
// Проект: SNES-AV eset
// Автор: Зайцев Роман
// Создан: 02.10.2014
// Краткое описание: Юнит-тесты на модуль cron_utils
/////////////////////////////////////

#include "CppUnitTest.h"
#include <unit_tests_common.h>
#include <common\cron_utils.h>
#include <random>
#include <ctime>
#include <common\tuple_helpers.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace unit_tests_common;

namespace common_lib_tests_project
{
	using range_type = std::tuple < int, int, int > ;
	enum range_indices: std::size_t { min_index, max_index, step_index };
	DECLARE_TUPLE_GET(range_type, range_indices);

	TEST_CLASS(cron_utils_test)
	{
	public:

		static void expression_format_test_helper(const wchar_t* expression)
		{
			std::tm begin_time = { 0, 0, 0, 1, 0, 114, 1, 0 };
			std::tm end_time = { 0, 0, 1, 1, 0, 114, 1, 0 };

			cron_utils::check_event_has_occurred(expression, { std::mktime(&begin_time), std::mktime(&end_time) });
		}

		static std::time_t make_time(int sec, int min, int hour, int mday, int mon, int year, int wday, int yday)
		{
			std::tm tm;

			tm.tm_sec = sec;
			tm.tm_min = min;
			tm.tm_hour = hour;
			tm.tm_mday = mday;
			tm.tm_mon = mon;
			tm.tm_year = year-1900;
			tm.tm_wday = wday;
			tm.tm_yday = yday;
			tm.tm_isdst = 0;
			return std::mktime(&tm);
		}

		TEST_METHOD(test_invalid_expressions)
		{
			checked_execute([] {

				//  invalid format
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L" * * \r*    * *\n *  "); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L" * "); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"*//2 * * * *"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"1,,2/1 * * * *"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"1--5/2 * * * *"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"/2 * * * *"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"-2/2 * * * *"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L",1/2 * * * *"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"1/ * * * *"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"1- * * * *"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"1, * * * *"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"1*3/2 * * * *"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"1,3,*/2 * * * *"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L",* * * * *"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"a * * * *"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"* 0x1 * * *"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"-1/2 * * * *"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"1-15/0 * * * *"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"* - * * *"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"1,2,3,4/2 * * * *"); }, L"Invalid expressions must be filtered");

				//  invalid range
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"60 * * * *"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"58 24 * * *"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"58 23 0 * *"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"58 23 32 * *"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"58 23 1 0 *"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"58 23 1 13 *"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"58 23 31 12 7"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"1-4,5- 23 31 12 7"); }, L"Invalid expressions must be filtered");
				Assert::ExpectException<const std::invalid_argument&>([]{ expression_format_test_helper(L"1-4,-8 23 31 12 7"); }, L"Invalid expressions must be filtered");
			});
		}

		static range_type generate_range(const std::uniform_int_distribution<int>& dist, std::random_device& rd)
		{
			int min_value, max_value, step_value;

			min_value = dist(rd);
			step_value = dist(rd);
			do 
			{
				max_value = dist(rd);
			} while (max_value < min_value);

			do 
			{
				step_value = dist(rd);
			} while (step_value == 0);

			return std::make_tuple(min_value, max_value, step_value);
		}

		TEST_METHOD(test_valid_expressions)
		{
			checked_execute([] {
				//  from CRON examples
				expression_format_test_helper(L"5 0 * * *");					//run five minutes after midnight, every day
				expression_format_test_helper(L"15 14 1 * *");					//run at 2:15pm on the first of every month
				expression_format_test_helper(L"23 0-23/2 * * *");				//run 23 minutes after midn, 2am, 4am ..., everyday
				expression_format_test_helper(L" 0 22 * * 1-5");				//run at 10 pm on weekdays
				expression_format_test_helper(L"5 4 * * 0 ");					//run at 5 after 4 every sunday
				expression_format_test_helper(L"1-10,13-15 1-9/3 5,6,7 * 1");
				expression_format_test_helper(L"*/2 1-9/3 5,6,7 * 1");
				expression_format_test_helper(L"1,1,1,1,1,1 1-9,2-10,3-11,1-15 5,6,7 * 1");
			});
		}

		TEST_METHOD(test_random_valid_expressions)
		{
			checked_execute([] {
				std::random_device rd;

				std::uniform_int_distribution<int> minutes_dist(0, 59);
				std::uniform_int_distribution<int> hours_dist(0, 23);
				std::uniform_int_distribution<int> day_of_month_dist(1, 31);
				std::uniform_int_distribution<int> month_dist(1, 12);
				std::uniform_int_distribution<int> day_of_week_dist(0, 6);
				std::uniform_int_distribution<int> bool_dist(0u, 1u);
				auto random_bool = [&] { return bool_dist(rd) == 0; };

				const auto test_iterations = 1000;

				for (int i = 0; i < test_iterations; ++i)
				{
					const auto minutes = generate_range(minutes_dist, rd);
					const auto hours = generate_range(hours_dist, rd);
					const auto day_of_month = generate_range(day_of_month_dist, rd);
					const auto month = generate_range(month_dist, rd);
					const auto day_of_week = generate_range(day_of_week_dist, rd);

					const auto item = [&random_bool](const range_type& range) -> std::wstring
					{
						const bool use_star = random_bool();
						const bool use_commas = random_bool();
						const bool use_step = random_bool();
						const auto first = get<range_indices::min_index>(range);
						const auto last = get<range_indices::max_index>(range);
						const auto step = get<range_indices::step_index>(range);

						std::wstring result;

						if (!use_star)
						{
							if (use_commas)
							{
								for (int i = first; i <= last; i += step)
								{
									if (i == first)
										result += std::to_wstring(i);
									else
										result += L"," + std::to_wstring(i);
								}
							}
							else
								result += std::to_wstring(first) + L"-" + std::to_wstring(last);
						}
						else
							result += L"*";

						if ((!use_commas || use_star) && use_step)
							result += L"/" + std::to_wstring(step);

						return result;
					};

					std::wstring expression = item(minutes) + L" " + item(hours) + L" " + item(day_of_month) + L" " + item(month) + L" " + item(day_of_week);
					expression_format_test_helper(expression.c_str());
				}
			});
		}

		TEST_METHOD(test_valid_events)
		{
			checked_execute([]
			{
				{
					const auto begin_time = make_time(15, 8, 10, 3, 9, 2014, 5, 275);
					const auto end_time = make_time(45, 6, 11, 3, 9, 2014, 5, 275);
					
					Assert::AreEqual(false, cron_utils::check_event_has_occurred(L"7 * * * *", { begin_time, end_time }));	//  just one minute was selected
					Assert::AreEqual(false, cron_utils::check_event_has_occurred(L"6 9 * * *", { begin_time, end_time }));
					Assert::AreEqual(false, cron_utils::check_event_has_occurred(L"6 12 * * *", { begin_time, end_time }));
					Assert::AreEqual(false, cron_utils::check_event_has_occurred(L"0 0-9,12-23 * * *", { begin_time, end_time }));
					Assert::AreEqual(true, cron_utils::check_event_has_occurred(L"6 * * * *", { begin_time, end_time }));
					Assert::AreEqual(true, cron_utils::check_event_has_occurred(L"8 * * * *", { begin_time, end_time }));
					Assert::AreEqual(true, cron_utils::check_event_has_occurred(L"59 * * * *", { begin_time, end_time }));
					Assert::AreEqual(true, cron_utils::check_event_has_occurred(L"0 * * * *", { begin_time, end_time }));
				}

				{
					const auto begin_time = make_time(15, 8, 10, 3, 9, 2014, 5, 275);
					const auto end_time = make_time(45, 20, 11, 3, 9, 2014, 5, 275);

					Assert::AreEqual(true, cron_utils::check_event_has_occurred(L"9,10 * * * *", { begin_time, end_time }));
					Assert::AreEqual(true, cron_utils::check_event_has_occurred(L"7 * * * *", { begin_time, end_time }));
					Assert::AreEqual(false, cron_utils::check_event_has_occurred(L"* 1-9 * * *", { begin_time, end_time }));
				}
				
				{
					const auto begin_time = make_time(33, 22, 11, 28, 1, 2014, 5, 58);
					const auto end_time = make_time(44, 33, 12, 01, 2, 2014, 6, 59);

					Assert::AreEqual(false, cron_utils::check_event_has_occurred(L"* * 30 * *", { begin_time, end_time }));
				}

				Assert::AreEqual(true, cron_utils::check_event_has_occurred(L"* * 30 * *", { make_time(33, 22, 11, 28, 1, 2014, 5, 58), make_time(44, 33, 12, 31, 2, 2014, 1, 89) }));
				Assert::AreEqual(true, cron_utils::check_event_has_occurred(L"* * 31 * *", { make_time(33, 22, 11, 28, 1, 2014, 5, 58), make_time(44, 33, 12, 31, 2, 2014, 1, 89) }));
				Assert::AreEqual(true, cron_utils::check_event_has_occurred(L"* * 01 * *", { make_time(33, 22, 11, 28, 1, 2014, 5, 58), make_time(44, 33, 12, 31, 2, 2014, 1, 89) }));
				Assert::AreEqual(false, cron_utils::check_event_has_occurred(L"* * 31 * *", { make_time(33, 22, 11, 28, 1, 2014, 5, 58), make_time(44, 33, 12, 30, 2, 2014, 1, 88) }));
				Assert::AreEqual(true, cron_utils::check_event_has_occurred(L"* * * * 0", { make_time(33, 22, 11, 28, 1, 2014, 5, 58), make_time(44, 33, 12, 30, 2, 2014, 1, 88) }));
				Assert::AreEqual(true, cron_utils::check_event_has_occurred(L"* * * * 1", { make_time(33, 22, 11, 28, 1, 2014, 5, 58), make_time(44, 33, 12, 30, 2, 2014, 1, 88) }));
				Assert::AreEqual(false, cron_utils::check_event_has_occurred(L"* * * * 1", { make_time(33, 22, 11, 28, 1, 2014, 5, 58), make_time(44, 33, 12, 1, 2, 2014, 1, 59) }));
				Assert::AreEqual(true, cron_utils::check_event_has_occurred(L"* * * * 6", { make_time(33, 22, 11, 28, 1, 2014, 5, 58), make_time(44, 33, 12, 1, 2, 2014, 1, 59) }));
				Assert::AreEqual(true, cron_utils::check_event_has_occurred(L"* * * * 5", { make_time(33, 22, 11, 28, 1, 2014, 5, 58), make_time(44, 33, 12, 1, 2, 2014, 1, 59) }));
				Assert::AreEqual(false, cron_utils::check_event_has_occurred(L"* * * * 4", { make_time(33, 22, 11, 28, 1, 2014, 5, 58), make_time(44, 33, 12, 1, 2, 2014, 1, 59) }));

				Assert::AreEqual(false, cron_utils::check_event_has_occurred(L"* * * * 1", { make_time(33, 22, 11, 31, 11, 2013, 2, 365), make_time(44, 33, 12, 1, 0, 2014, 3, 0) }));
				Assert::AreEqual(true, cron_utils::check_event_has_occurred(L"* * * * 2", { make_time(33, 22, 11, 31, 11, 2013, 2, 365), make_time(44, 33, 12, 1, 0, 2014, 3, 0) }));
				Assert::AreEqual(false, cron_utils::check_event_has_occurred(L"* * * * 4", { make_time(33, 22, 11, 31, 11, 2013, 2, 365), make_time(44, 33, 12, 1, 0, 2014, 3, 0) }));
			});
		}
	};
}