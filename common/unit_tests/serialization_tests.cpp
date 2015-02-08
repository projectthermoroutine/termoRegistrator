#include "CppUnitTest.h"
#include <unit_tests_common.h>

#include <serialization.h>
#include <limits>

#include <vector>
#include <deque>
#include <list>
#include <tuple>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace unit_tests_common;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			inline std::wstring ToString(std::int64_t value) { return std::to_wstring(value); }
			inline std::wstring ToString(std::uint64_t value) { return std::to_wstring(value); }
			inline std::wstring ToString(std::int16_t value) { return std::to_wstring(value); }
			inline std::wstring ToString(std::uint16_t value) { return std::to_wstring(value); }
		}
	}
}

namespace common_lib_tests_project
{
	enum class enum_class_uint8 : std::uint8_t { element_0 = 0, element_1, element_2 };
	enum class enum_class_uint16 : std::uint16_t { element_0 = 0, element_1, element_2 };
	enum class enum_class_uint32 : std::uint32_t { element_0 = 0, element_1, element_2 };
	enum class enum_class_uint64 : std::uint64_t { element_0 = 0, element_1, element_2 };
	enum class enum_class_int8 : std::int8_t { element_0 = 0, element_1, element_2 };
	enum class enum_class_int16 : std::int16_t { element_0 = 0, element_1, element_2 };
	enum class enum_class_int32 : std::int32_t { element_0 = 0, element_1, element_2 };
	enum class enum_class_int64 : std::int64_t { element_0 = 0, element_1, element_2 };

	enum c_style_enum { element_0 = 0, element_1, element_2 };

	template <typename value_t>
	void test_binary_operations_for_value(value_t value)
	{
		std::uint8_t buffer[sizeof(value_t)];

		auto buffer_iter = &buffer[0];
		auto const written = serialization::details::insert_binary_data(buffer_iter, value);
		Assert::IsTrue(written != 0);
		Assert::IsTrue(buffer_iter == (buffer + written));

		buffer_iter = &buffer[0];
		value_t actual_value;
		auto const read = serialization::details::get_binary_data(buffer_iter, actual_value);
		Assert::IsTrue(written == read);
		Assert::IsTrue(buffer_iter == (buffer + read));

		Assert::IsTrue(value == actual_value);
	}

	template <typename value_t>
	void test_binary_operations()
	{
		std::vector<value_t> test_values;
		unit_tests_common::generate_test_numbers_range(
			std::numeric_limits<value_t>::min(),
			std::numeric_limits<value_t>::max(),
			std::back_inserter(test_values));

		for (auto const value : test_values)
		{
			test_binary_operations_for_value(value);
		}
	}

	template <>
	void test_binary_operations<bool>()
	{
		test_binary_operations_for_value(false);
		test_binary_operations_for_value(true);
	}
		
	template <typename value_t>
	void test_containers_serialization()
	{
		std::vector<value_t> vector;
		std::deque<value_t> deque;
		std::list<value_t> list;

		const std::size_t vector_size = 128 * 1024;
		const std::size_t deque_size = 128 * 1024;
		const std::size_t list_size = 64 * 1024;

		generate_random_values<value_t>(std::back_inserter(vector), vector_size);
		generate_random_values<value_t>(std::back_inserter(deque), deque_size);
		generate_random_values<value_t>(std::back_inserter(list), list_size);

		serialization::binary_stream stream;
		stream << vector << deque << list;

		auto const buffer_size = stream.buffer_size();
		Assert::IsTrue(buffer_size != 0);

		std::vector<value_t> d_vector;
		std::deque<value_t> d_deque;
		std::list<value_t> d_list;

		stream >> d_vector >> d_deque >> d_list;

		Assert::IsTrue(stream.buffer_size() == buffer_size);

		Assert::IsTrue(vector.size() == d_vector.size());
		Assert::IsTrue(deque.size() == d_deque.size());
		Assert::IsTrue(list.size() == d_list.size());

		Assert::IsTrue(std::equal(vector.begin(), vector.end(), d_vector.begin()));
		Assert::IsTrue(std::equal(deque.begin(), deque.end(), d_deque.begin()));
		Assert::IsTrue(std::equal(list.begin(), list.end(), d_list.begin()));
	}

	template <typename value_t, typename deserialization_value_t>
	void test_cross_deserialization_func_impl()
	{
		serialization::binary_stream stream;
		value_t value = unit_tests_common::get_random<value_t>();
		stream << value;

		deserialization_value_t d_value;
		Assert::ExpectException<serialization::deserialization_type_error>([&stream, &d_value]() { stream >> d_value; });
	}

	template <typename value_t>
	void test_correct_deserialization_func_impl()
	{
		serialization::binary_stream stream;
		value_t value = unit_tests_common::get_random<value_t>();
		stream << value;

		value_t d_value;
		stream >> d_value;

		Assert::IsTrue(value == d_value);

		serialization::binary_stream stream2;
		std::copy(stream.cbegin(), stream.cend(), stream2.insert_iter());

		value_t d_value2;
		stream2 >> d_value2;

		Assert::IsTrue(value == d_value2);
	}

	template <typename value_t>
	void test_partial_buffer_impl(value_t value)
	{
		serialization::binary_stream stream;
		stream << value;

		serialization::binary_stream stream2;
		std::copy(stream.cbegin(), stream.cend() - 1, stream2.insert_iter());

		value_t d_value2;
		Assert::ExpectException<serialization::deserialization_partial_buffer_error>([&stream2, &d_value2]() { stream2 >> d_value2; });
	}

	template <typename value_t, typename deserialization_value_t>
	struct test_cross_deserialization_impl;

	template <typename value_t, typename deserialization_value_t>
	struct test_cross_deserialization_impl
	{
		test_cross_deserialization_impl()
		{
			test_cross_deserialization_func_impl<value_t, deserialization_value_t>();
		}
	};

	template <typename value_t>
	struct test_cross_deserialization_impl<value_t, value_t>
	{
		test_cross_deserialization_impl()
		{
			test_correct_deserialization_func_impl<value_t>();
		}
	};

	template <typename value_t>
	struct test_cross_deserialization_impl<wchar_t, value_t> : 
		std::conditional<
			std::is_same<typename serialization::details::nearest_fixed_type<wchar_t>::type, typename serialization::details::nearest_fixed_type<value_t>::type>::value,
			test_cross_deserialization_impl<typename serialization::details::nearest_fixed_type<wchar_t>::type, typename serialization::details::nearest_fixed_type<wchar_t>::type>,
			test_cross_deserialization_impl<typename serialization::details::nearest_fixed_type<wchar_t>::type, value_t> >::type
	{
	};

	template <typename value_t>
	struct test_cross_deserialization_impl<value_t, wchar_t> : test_cross_deserialization_impl<wchar_t, value_t>
	{
	};
		
	template <>
	struct test_cross_deserialization_impl<wchar_t, wchar_t>
	{
		test_cross_deserialization_impl()
		{
			test_correct_deserialization_func_impl<wchar_t>();
		}
	};

	template <typename value_t>
	void test_cross_deserialization_container()
	{
		test_cross_deserialization_func_impl<value_t, std::vector<value_t> >();
		test_cross_deserialization_func_impl<std::vector<value_t>, value_t >();
		test_correct_deserialization_func_impl<std::vector<value_t> >();
	}

	template <typename value_t>
	void test_cross_deserialization()
	{
		test_cross_deserialization_impl<value_t, std::uint8_t>();
		test_cross_deserialization_impl<value_t, std::uint16_t>();
		test_cross_deserialization_impl<value_t, std::uint32_t>();
		test_cross_deserialization_impl<value_t, std::uint64_t>();

		test_cross_deserialization_impl<value_t, std::int8_t>();
		test_cross_deserialization_impl<value_t, std::int16_t>();
		test_cross_deserialization_impl<value_t, std::int32_t>();
		test_cross_deserialization_impl<value_t, std::int64_t>();

		test_cross_deserialization_impl<value_t, bool>();
		test_cross_deserialization_impl<value_t, wchar_t>();

		test_cross_deserialization_container<value_t>();
	}

	template <typename value_t>
	void test_partial_buffer()
	{
		test_partial_buffer_impl(unit_tests_common::get_random<value_t>());

		std::vector<value_t> vector;
		test_partial_buffer_impl(vector);

		std::generate_n(std::back_inserter(vector), 10, []() { return unit_tests_common::get_random<value_t>(); });
		test_partial_buffer_impl(vector);
	}

	template <typename T>
	void init_random(T & x)
	{
		x = unit_tests_common::get_random<decltype(x)>();
	}

	class random_initializer final
	{
	public:
		random_initializer() {}

		template <typename T>
		inline void operator () (T & val)
		{
			val = unit_tests_common::get_random<T>();
		}

		random_initializer(const random_initializer &) = delete;
		random_initializer & operator = (const random_initializer &) = delete;
	};

	template <typename ...types>
	void test_tuple()
	{
		serialization::binary_stream stream;

		random_initializer init;
		std::tuple<types...> x;
		tuple_helpers::for_each_in_tuple(x, init);

		stream << x;
		std::tuple<types...> y;
		stream >> y;
		Assert::IsTrue(x == y);
	}

	template <typename T, std::size_t N>
	void test_enum(const T (&values)[N])
	{
		serialization::binary_stream stream;
		
		std::vector<T> reference_enums;
		std::vector<std::tuple<T>> reference_enums_in_tuple;

		for (const auto value : values)
		{
			stream << value;
			reference_enums.push_back(value);
			reference_enums_in_tuple.push_back(std::make_tuple(value));
		}

		stream << reference_enums;
		stream << reference_enums_in_tuple;

		for (const auto expected_value : values)
		{
			T value;
			stream >> value;
			Assert::IsTrue(value == expected_value);
		}

		std::vector<T> enums;
		std::vector<std::tuple<T>> enums_in_tuple;

		stream >> enums;
		stream >> enums_in_tuple;

		Assert::IsTrue(enums == reference_enums);
		Assert::IsTrue(enums_in_tuple == reference_enums_in_tuple);
	}

	TEST_CLASS(serialization_test)
	{
	public:
		TEST_METHOD(convert_to_fixed_type)
		{
			using namespace serialization::details;

			static_assert(std::is_same<nearest_fixed_type<std::uint8_t>::type, std::uint8_t>::value, "Nearest type mismatch for std::uint8_t");
			static_assert(std::is_same<nearest_fixed_type<std::uint16_t>::type, std::uint16_t>::value, "Nearest type mismatch for std::uint16_t");
			static_assert(std::is_same<nearest_fixed_type<std::uint32_t>::type, std::uint32_t>::value, "Nearest type mismatch for std::uint32_t");
			static_assert(std::is_same<nearest_fixed_type<std::uint64_t>::type, std::uint64_t>::value, "Nearest type mismatch for std::uint64_t");

			static_assert(std::is_same<nearest_fixed_type<std::int8_t>::type, std::int8_t>::value, "Nearest type mismatch for std::int8_t");
			static_assert(std::is_same<nearest_fixed_type<std::int16_t>::type, std::int16_t>::value, "Nearest type mismatch for std::int16_t");
			static_assert(std::is_same<nearest_fixed_type<std::int32_t>::type, std::int32_t>::value, "Nearest type mismatch for std::int32_t");
			static_assert(std::is_same<nearest_fixed_type<std::int64_t>::type, std::int64_t>::value, "Nearest type mismatch for std::int64_t");
		}

		TEST_METHOD(serialize_deserialize_binary_operations_test)
		{
			checked_execute([] {
				test_binary_operations<std::uint8_t>();
				test_binary_operations<std::uint16_t>();
				test_binary_operations<std::uint32_t>();
				test_binary_operations<std::uint64_t>();

				test_binary_operations<std::int8_t>();
				test_binary_operations<std::int16_t>();
				test_binary_operations<std::int32_t>();
				test_binary_operations<std::int64_t>();

				test_binary_operations<bool>();
				test_binary_operations<wchar_t>();
			});
		}

		TEST_METHOD(serialize_type_info_test)
		{
			checked_execute([] {
				using namespace serialization::details;

				Assert::IsTrue(get_type_info<std::uint8_t, false>() == 1);
				Assert::IsTrue(get_type_info<std::uint16_t, false>() == 2);
				Assert::IsTrue(get_type_info<std::uint32_t, false>() == 4);
				Assert::IsTrue(get_type_info<std::uint64_t, false>() == 8);
				Assert::IsTrue(get_type_info<std::int8_t, false>() == (1 | type_info_signed));
				Assert::IsTrue(get_type_info<std::int16_t, false>() == (2 | type_info_signed));
				Assert::IsTrue(get_type_info<std::int32_t, false>() == (4 | type_info_signed));
				Assert::IsTrue(get_type_info<std::int64_t, false>() == (8 | type_info_signed));
				Assert::IsTrue(get_type_info<bool, false>() == type_info_bool);				

				Assert::IsTrue(get_type_info<std::string, false>() == type_info_complex);
				Assert::IsTrue(get_type_info<std::wstring, false>() == type_info_complex);
				Assert::IsTrue(get_type_info<std::vector<std::uint8_t>, false>() == type_info_complex);
				Assert::IsTrue(get_type_info<std::deque<std::uint8_t>, false>() == type_info_complex);
				Assert::IsTrue(get_type_info<std::list<std::uint8_t>, false>() == type_info_complex);
				Assert::IsTrue(get_type_info<std::tuple<>, false>() == type_info_complex);
				Assert::IsTrue(get_type_info<std::tuple<bool>, false>() == type_info_complex);
				Assert::IsTrue(get_type_info<std::tuple<std::uint8_t>, false>() == type_info_complex);
				Assert::IsTrue(get_type_info<std::tuple<std::uint8_t, std::uint16_t>, false>() == type_info_complex);

				Assert::IsTrue(get_type_info<std::uint8_t, true>() == (1 | type_info_array));
				Assert::IsTrue(get_type_info<std::uint16_t, true>() == (2 | type_info_array));
				Assert::IsTrue(get_type_info<std::uint32_t, true>() == (4 | type_info_array));
				Assert::IsTrue(get_type_info<std::uint64_t, true>() == (8 | type_info_array));
				Assert::IsTrue(get_type_info<std::int8_t, true>() == (1 | type_info_signed | type_info_array));
				Assert::IsTrue(get_type_info<std::int16_t, true>() == (2 | type_info_signed | type_info_array));
				Assert::IsTrue(get_type_info<std::int32_t, true>() == (4 | type_info_signed | type_info_array));
				Assert::IsTrue(get_type_info<std::int64_t, true>() == (8 | type_info_signed | type_info_array));
				Assert::IsTrue(get_type_info<bool, true>() == (type_info_bool | type_info_array));

				Assert::IsTrue(get_type_info<std::string, true>() == (type_info_complex | type_info_array));
				Assert::IsTrue(get_type_info<std::wstring, true>() == (type_info_complex | type_info_array));
				Assert::IsTrue(get_type_info<std::vector<std::uint8_t>, true>() == (type_info_complex | type_info_array));
				Assert::IsTrue(get_type_info<std::deque<std::uint8_t>, true>() == (type_info_complex | type_info_array));
				Assert::IsTrue(get_type_info<std::list<std::uint8_t>, true>() == (type_info_complex | type_info_array));
				Assert::IsTrue(get_type_info<std::tuple<>, true>() == (type_info_complex | type_info_array));
				Assert::IsTrue(get_type_info<std::tuple<bool>, true>() == (type_info_complex | type_info_array));
				Assert::IsTrue(get_type_info<std::tuple<std::uint8_t>, true>() == (type_info_complex | type_info_array));
				Assert::IsTrue(get_type_info<std::tuple<std::uint8_t, std::uint16_t>, true>() == (type_info_complex | type_info_array));

				Assert::IsTrue(get_type_info<enum_class_uint8, false>() == 1);
				Assert::IsTrue(get_type_info<enum_class_uint16, false>() == 2);
				Assert::IsTrue(get_type_info<enum_class_uint32, false>() == 4);
				Assert::IsTrue(get_type_info<enum_class_uint64, false>() == 8);
				Assert::IsTrue(get_type_info<enum_class_int8, false>() == (1 | type_info_signed));
				Assert::IsTrue(get_type_info<enum_class_int16, false>() == (2 | type_info_signed));
				Assert::IsTrue(get_type_info<enum_class_int32, false>() == (4 | type_info_signed));
				Assert::IsTrue(get_type_info<enum_class_int64, false>() == (8 | type_info_signed));

				Assert::IsTrue(
					get_type_info<c_style_enum, false>() == get_type_info<int, false>() || 
					get_type_info<c_style_enum, false>() == get_type_info<unsigned int, false>());
			});
		}

		TEST_METHOD(serialize_deserialize_basic_types_test)
		{
			checked_execute([] {
				
				const int8_t i8_p = std::numeric_limits<int8_t>::max();
				const int16_t i16_p = std::numeric_limits<int16_t>::max();
				const int32_t i32_p = std::numeric_limits<int32_t>::max();
				const int64_t i64_p = std::numeric_limits<int64_t>::max();
				
				const int8_t i8_n = std::numeric_limits<int8_t>::min();
				const int16_t i16_n = std::numeric_limits<int16_t>::min();
				const int32_t i32_n = std::numeric_limits<int32_t>::min();
				const int64_t i64_n = std::numeric_limits<int64_t>::min();

				const uint8_t ui8 = std::numeric_limits<uint8_t>::max();
				const uint16_t ui16 = std::numeric_limits<uint16_t>::max();
				const uint32_t ui32 = std::numeric_limits<uint32_t>::max();
				const uint64_t ui64 = std::numeric_limits<uint64_t>::max();

				serialization::binary_stream stream;
				stream << i8_p << i16_p << i32_p << i64_p << i8_n << i16_n << i32_n << i64_n << ui8 << ui16 << ui32 << ui64;

				auto const buffer_size = stream.buffer_size();
				Assert::IsTrue(buffer_size != 0);

				int8_t d_i8_p;
				int16_t d_i16_p;
				int32_t d_i32_p;
				int64_t d_i64_p;

				int8_t d_i8_n;
				int16_t d_i16_n;
				int32_t d_i32_n;
				int64_t d_i64_n;

				uint8_t d_ui8;
				uint16_t d_ui16;
				uint32_t d_ui32;
				uint64_t d_ui64;

				stream >> d_i8_p >> d_i16_p >> d_i32_p >> d_i64_p >> d_i8_n >> d_i16_n >> d_i32_n >> d_i64_n >> d_ui8 >> d_ui16 >> d_ui32 >> d_ui64;

				Assert::IsTrue(stream.buffer_size() == buffer_size);

				Assert::AreEqual(i8_n, d_i8_n);
				Assert::AreEqual(i16_n, d_i16_n);
				Assert::AreEqual(i32_n, d_i32_n);
				Assert::AreEqual(i64_n, d_i64_n);

				Assert::AreEqual(i8_p, d_i8_p);
				Assert::AreEqual(i16_p, d_i16_p);
				Assert::AreEqual(i32_p, d_i32_p);
				Assert::AreEqual(i64_p, d_i64_p);

				Assert::AreEqual(ui8, d_ui8);
				Assert::AreEqual(ui16, d_ui16);
				Assert::AreEqual(ui32, d_ui32);
				Assert::AreEqual(ui64, d_ui64);
			});
		}

		TEST_METHOD(serialize_deserialize_strings_test)
		{
			checked_execute([] {
				
				const unsigned int number_of_strings = 15;
				
				std::string str("test_string");
				std::wstring wstr(L"test_wstring");

				std::vector<std::string> strings;
				std::vector<std::wstring> wstrings;

				for (unsigned int n = 0; n != number_of_strings; ++n)
				{
					strings.push_back(str);
					wstrings.push_back(wstr);

					str += str + std::to_string(n);
					wstr += wstr + std::to_wstring(n);
				}

				serialization::binary_stream stream;
				for (unsigned int n = 0; n != number_of_strings; ++n)
				{
					stream << strings[n] << wstrings[n];
				}

				auto const buffer_size = stream.buffer_size();
				Assert::IsTrue(buffer_size != 0);

				for (unsigned int n = 0; n != number_of_strings; ++n)
				{
					std::string str;
					std::wstring wstr;

					stream >> str >> wstr;
					
					Assert::AreEqual(str, strings[n]);
					Assert::AreEqual(wstr, wstrings[n]);
				}

				Assert::IsTrue(stream.buffer_size() == buffer_size);
			});
		}

		TEST_METHOD(serialize_deserialize_containers_test)
		{
			checked_execute([] {
				test_containers_serialization<std::uint8_t>();
				test_containers_serialization<std::uint16_t>();
				test_containers_serialization<std::uint32_t>();
				test_containers_serialization<std::uint64_t>();

				test_containers_serialization<std::int8_t>();
				test_containers_serialization<std::int16_t>();
				test_containers_serialization<std::int32_t>();
				test_containers_serialization<std::int64_t>();

				test_containers_serialization<bool>();
				test_containers_serialization<wchar_t>();
			});
		}

		TEST_METHOD(cross_type_deserialization_test)
		{
			checked_execute([] {
				test_cross_deserialization<std::uint8_t>();
				test_cross_deserialization<std::uint16_t>();
				test_cross_deserialization<std::uint32_t>();
				test_cross_deserialization<std::uint64_t>();

				test_cross_deserialization<std::int8_t>();
				test_cross_deserialization<std::int16_t>();
				test_cross_deserialization<std::int32_t>();
				test_cross_deserialization<std::int64_t>();

				test_cross_deserialization<bool>();
				test_cross_deserialization<wchar_t>();
			});
		}

		TEST_METHOD(partial_deserialization_test)
		{
			checked_execute([] {
				test_partial_buffer<std::uint8_t>();
				test_partial_buffer<std::uint16_t>();
				test_partial_buffer<std::uint32_t>();
				test_partial_buffer<std::uint64_t>();

				test_partial_buffer<std::int8_t>();
				test_partial_buffer<std::int16_t>();
				test_partial_buffer<std::int32_t>();
				test_partial_buffer<std::int64_t>();

				test_partial_buffer<bool>();
				test_partial_buffer<wchar_t>();
			});
		}

		TEST_METHOD(tuple_serialization_deserialization_test)
		{
			checked_execute([] {
				test_tuple<>();

				test_tuple<std::uint8_t>();
				test_tuple<std::uint8_t, std::uint16_t>();
				test_tuple<std::uint8_t, std::uint16_t, std::uint32_t>();
				test_tuple<std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t>();

				test_tuple<std::int8_t>();
				test_tuple<std::int8_t, std::int16_t>();
				test_tuple<std::int8_t, std::int16_t, std::int32_t>();
				test_tuple<std::int8_t, std::int16_t, std::int32_t, std::int64_t>();

				test_tuple<std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t, std::int8_t, std::int16_t, std::int32_t, std::int64_t>();

				test_tuple< std::vector<std::uint8_t> >();
				test_tuple< std::vector<std::uint16_t> >();
				test_tuple< std::vector<std::uint32_t> >();
				test_tuple< std::vector<std::uint64_t> >();

				test_tuple< std::vector<std::int8_t> >();
				test_tuple< std::vector<std::int16_t> >();
				test_tuple< std::vector<std::int32_t> >();
				test_tuple< std::vector<std::int64_t> >();

				test_tuple< std::tuple< std::vector<std::int64_t> > >();
				test_tuple< std::tuple< std::vector<std::int64_t>, std::vector<std::int32_t> > >();

				test_tuple< std::vector< std::tuple<> > >();
				test_tuple< std::vector< std::tuple< std::int64_t > > >();

				test_tuple< std::vector< std::tuple< std::vector< std::tuple< std::int64_t, std::int32_t > >, std::vector< std::tuple< std::int32_t, std::int16_t > > > > >();
			});
		}

		TEST_METHOD(enums_serialization_deserialization_test)
		{
			checked_execute([] {
				enum_class_uint8 ui8[] = { enum_class_uint8::element_0, enum_class_uint8::element_1, enum_class_uint8::element_2 };
				enum_class_uint16 ui16[] = { enum_class_uint16::element_0, enum_class_uint16::element_1, enum_class_uint16::element_2 };
				enum_class_uint32 ui32[] = { enum_class_uint32::element_0, enum_class_uint32::element_1, enum_class_uint32::element_2 };
				enum_class_uint64 ui64[] = { enum_class_uint64::element_0, enum_class_uint64::element_1, enum_class_uint64::element_2 };
				enum_class_int8 i8[] = { enum_class_int8::element_0, enum_class_int8::element_1, enum_class_int8::element_2 };
				enum_class_int16 i16[] = { enum_class_int16::element_0, enum_class_int16::element_1, enum_class_int16::element_2 };
				enum_class_int32 i32[] = { enum_class_int32::element_0, enum_class_int32::element_1, enum_class_int32::element_2 };
				enum_class_int64 i64[] = { enum_class_int64::element_0, enum_class_int64::element_1, enum_class_int64::element_2 };
				c_style_enum c[] = { element_0, element_1, element_2 };

				test_enum(ui8);
				test_enum(ui16);
				test_enum(ui32);
				test_enum(ui64);
				test_enum(i8);
				test_enum(i16);
				test_enum(i32);
				test_enum(i64);
				test_enum(c);
			});
		}
	};
}