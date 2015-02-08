#include "CppUnitTest.h"
#include <unit_tests_common.h>

#include <tuple_helpers.h>

#include <algorithm>
#include <numeric>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace unit_tests_common;

namespace details
{
	class tuple_counter final
	{
	public:
		tuple_counter() : num_of_elements(0), total_size_of_elements(0)
		{
		}

		template <typename value_t>
		void operator () (value_t & value)
		{
			total_size_of_elements += sizeof(value);
			++num_of_elements;
			elements_size.push_back(sizeof(value));
		}
		std::size_t num_of_elements;
		std::size_t total_size_of_elements;
		std::vector<std::size_t> elements_size;
	};
}

namespace common_lib_tests_project
{
	TEST_CLASS(tuple_helpers_test)
	{
	public:
		TEST_METHOD(for_each_in_tuple_test)
		{
			checked_execute([] {
				std::tuple<char, short, int, long, long long, float, double> x;
				auto const num_of_elements = std::tuple_size<decltype(x)>::value;

				std::vector<std::size_t> elements_size;
				elements_size.push_back(sizeof(std::get<0>(x)));
				elements_size.push_back(sizeof(std::get<1>(x)));
				elements_size.push_back(sizeof(std::get<2>(x)));
				elements_size.push_back(sizeof(std::get<3>(x)));
				elements_size.push_back(sizeof(std::get<4>(x)));
				elements_size.push_back(sizeof(std::get<5>(x)));
				elements_size.push_back(sizeof(std::get<6>(x)));

				details::tuple_counter counter;
				tuple_helpers::for_each_in_tuple(x, counter);

				Assert::IsTrue(counter.num_of_elements == num_of_elements);
				Assert::IsTrue(counter.total_size_of_elements == std::accumulate(elements_size.begin(), elements_size.end(), 0));
				Assert::IsTrue(counter.elements_size.size() == elements_size.size());
				Assert::IsTrue(std::equal(elements_size.begin(), elements_size.end(), counter.elements_size.begin()));
			});
		}

		TEST_METHOD(revert_tuple_test)
		{
			checked_execute([] {
				std::tuple<char, short, int, long, long long, float, double> x(10, 11, 12, 13, 14, 15.0f, 16.0f);
				auto const rx = tuple_helpers::revert_tuple(x);

				auto const a0 = std::get<0>(x);
				auto const a1 = std::get<1>(x);
				auto const a2 = std::get<2>(x);

				auto const b0 = std::get<6>(rx);
				auto const b1 = std::get<5>(rx);
				auto const b2 = std::get<4>(rx);

				static_assert(
					std::is_same<decltype(a0), decltype(b0)>::value &&
					std::is_same<decltype(a1), decltype(b1)>::value &&
					std::is_same<decltype(a2), decltype(b2)>::value,
					"Different types are detected.");

				Assert::AreEqual(a0, b0);
				Assert::AreEqual(a1, b1);
				Assert::AreEqual(a2, b2);
			});
		}
	};
}