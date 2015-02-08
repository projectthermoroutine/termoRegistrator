#include "CppUnitTest.h"
#include <unit_tests_common.h>

#include <iterator>
#include <rpc_light.h>

#include <vector>
#include <deque>
#include <list>
#include <tuple>
#include <stdexcept>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace unit_tests_common;

namespace common_lib_tests_project
{
	const std::uint8_t magic_uint8_t = 0x12u;
	const std::uint16_t magic_uint16_t = 0x2345u;
	const std::uint32_t magic_uint32_t = 0x34567890ul;
	const std::uint64_t magic_uint64_t = 0x4567890abcdef123ull;

	const std::int8_t magic_int8_t = -12;
	const std::int16_t magic_int16_t = -1234;
	const std::int32_t magic_int32_t = -12345678l;
	const std::int64_t magic_int64_t = -12345678901234ll;

	const auto magic2_uint32_t = magic_uint32_t * 2;
	const auto magic3_uint32_t = magic_uint32_t * 3;

	void f_void_void(void) {}

	void f_void_uint8_t(std::uint8_t value) { Assert::IsTrue(value == magic_uint8_t); }
	void f_void_uint16_t(std::uint16_t value) { Assert::IsTrue(value == magic_uint16_t); }
	void f_void_uint32_t(std::uint32_t value) { Assert::IsTrue(value == magic_uint32_t); }
	void f_void_uint64_t(std::uint64_t value) { Assert::IsTrue(value == magic_uint64_t); }

	void f_void_int8_t(std::int8_t value) { Assert::IsTrue(value == magic_int8_t); }
	void f_void_int16_t(std::int16_t value) { Assert::IsTrue(value == magic_int16_t); }
	void f_void_int32_t(std::int32_t value) { Assert::IsTrue(value == magic_int32_t); }
	void f_void_int64_t(std::int64_t value) { Assert::IsTrue(value == magic_int64_t); }

	std::uint8_t f_uint8_t_2_uint8_t(std::uint8_t a, std::uint8_t b) { return a + b; }
	std::uint16_t f_uint16_t_2_uint16_t(std::uint16_t a, std::uint16_t b) { return a + b; }
	std::uint32_t f_uint32_t_2_uint32_t(std::uint32_t a, std::uint32_t b) { return a + b; }
	std::uint64_t f_uint64_t_2_uint64_t(std::uint64_t a, std::uint64_t b) { return a + b; }

	template <typename container_t>
	container_t f_reverse_container(container_t x)
	{
		std::reverse(x.begin(), x.end());
		return x;
	}

	std::vector<std::uint32_t> f_vector_uint32_t_3_uint32_t(std::uint32_t a, std::uint32_t b, std::uint32_t c)
	{
		std::vector<std::uint32_t> result;
		result.push_back(a);
		result.push_back(b);
		result.push_back(c);
		return result;
	}

	void check_sample_vector_uint32_t(std::vector<std::uint32_t> vec)
	{
		Assert::IsTrue(vec.size() == 3);
		Assert::IsTrue(vec[0] == magic_uint32_t);
		Assert::IsTrue(vec[1] == magic2_uint32_t);
		Assert::IsTrue(vec[2] == magic3_uint32_t);
	}

	void f_void_vector_uint32(std::vector<std::uint32_t> vec)
	{
		check_sample_vector_uint32_t(vec);
	}

	void f_void_vector_uint32_vector_uint32(std::vector<std::uint32_t> vec1, std::vector<std::uint32_t> vec2)
	{
		check_sample_vector_uint32_t(vec1);
		std::reverse(vec2.begin(), vec2.end());
		check_sample_vector_uint32_t(vec2);
	}

	template <typename ...types>
	std::uint64_t f_uint64_t_tuple(std::tuple<types...> tuples)
	{
		std::uint64_t total = 0;
		tuple_helpers::for_each_in_tuple(tuples, [&total](std::uint64_t x) { total += x; });
		return total;
	}

	const std::size_t test_container_size = 100;

	template <typename container_t>
	container_t make_test_container()
	{
		container_t result;
		container_t::value_type n = 0;
		std::generate_n(std::back_inserter(result), test_container_size, [&n]() { return n++; });
		return result;
	}

	template <typename TContainer>
	void test_container_impl()
	{
		rpc_light::function_call_dispatcher dispatcher;
		test_channel channel(dispatcher);

		TContainer x = make_test_container<TContainer>();
		Assert::IsTrue(!x.empty());
		REGISTER_FUNCTION_ENTRY(common_lib_tests_project::f_reverse_container<TContainer>, dispatcher);
		auto reverted_x = RPC_LIGHT_INVOKE(channel, common_lib_tests_project, f_reverse_container<TContainer>, x);
		Assert::IsTrue(!reverted_x.empty());
		Assert::IsTrue(reverted_x.size() == x.size());

		Assert::IsTrue(std::equal(x.cbegin(), x.cend(), reverted_x.crbegin()));
	}

	template <typename T>
	void test_containers()
	{
		test_container_impl<std::vector<T>>();
		test_container_impl<std::deque<T>>();
		test_container_impl<std::list<T>>();
	}

	template <typename... types>
	void test_tuple_uint(std::tuple<types...> && tuple)
	{
		rpc_light::function_call_dispatcher dispatcher;
		test_channel channel(dispatcher);

		REGISTER_FUNCTION_ENTRY(common_lib_tests_project::f_uint64_t_tuple<types...>, dispatcher);
		auto sum_actual = RPC_LIGHT_INVOKE(channel, common_lib_tests_project, f_uint64_t_tuple<types...>, tuple);
		
		std::uint64_t sum_expected = 0;
		tuple_helpers::for_each_in_tuple(tuple, [&sum_expected](std::uint64_t x) { sum_expected += x; });

		Assert::IsTrue(sum_actual == sum_expected);
	}

	template <typename exception_t>
	struct thrower
	{
		inline thrower() { throw exception_t();  }
	};

	template <typename exception_t>
	struct non_class_thrower : thrower<exception_t> {};

	template <typename exception_t>
	struct thrower_with_message
	{
		inline thrower_with_message() { throw exception_t("Test message"); }
	};

	template <>
	struct thrower_with_message<std::system_error>
	{
		inline thrower_with_message() { throw std::system_error(std::error_code()); }
	};

	class my_exception {};

	template <typename throw_exception_t>
	void f_void_exception()
	{
		std::conditional<std::is_class<throw_exception_t>::value,
			std::conditional<std::is_default_constructible<throw_exception_t>::value, thrower<throw_exception_t>, thrower_with_message<throw_exception_t> >::type,
			non_class_thrower<throw_exception_t>>::type();
	}

	template <typename throw_exception_t, typename catch_exception_t>
	void test_exception()
	{
		rpc_light::function_call_dispatcher dispatcher;
		test_channel channel(dispatcher);

		REGISTER_FUNCTION_ENTRY(common_lib_tests_project::f_void_exception<throw_exception_t>, dispatcher);
		Assert::ExpectException<const catch_exception_t&>([&] { RPC_LIGHT_INVOKE(channel, common_lib_tests_project, f_void_exception<throw_exception_t>); });
	}

	namespace a
	{
		EXCEPTION_LIST();
		const std::uint64_t magic_uint64_t = 5ull;
		inline std::uint64_t f_uint64_t_uint64_t(std::uint64_t x) { return x * a::magic_uint64_t; }
	}

	namespace b
	{
		EXCEPTION_LIST();
		const std::uint64_t magic_uint64_t = 7ull;
		inline std::uint64_t f_uint64_t_uint64_t(std::uint64_t x) { return x * b::magic_uint64_t; }
	}

	namespace c
	{
		EXCEPTION_LIST();
		const std::uint64_t magic_uint64_t = 11ull;
		inline std::uint64_t f_uint64_t_uint64_t(std::uint64_t x) { return x * c::magic_uint64_t; }
	}

	class user_exception: public std::exception
	{
	public:
		user_exception(int code, const std::string& what): std::exception(), code_(code), what_(what) {}

		virtual const char* what() const override
		{
			return what_.c_str();
		}

		int code() const
		{
			return code_;
		}
	private:

		int code_;
		std::string what_;
	};

	void throw_user_exception()
	{
		throw user_exception(10, "Test 10");
	};

	EXCEPTION_LIST(user_exception);

	class test_channel final
	{
	public:
		test_channel(rpc_light::function_call_dispatcher & dispatcher) : _dispatcher(dispatcher) {}

		template <typename iter_in_t, typename iter_out_t>
		void send(iter_in_t in_begin, iter_in_t in_end, iter_out_t out)
		{
			serialization::binary_stream in_stream;
			std::copy(in_begin, in_end, in_stream.insert_iter());

			serialization::binary_stream out_stream;
			rpc_light::execute_function(
				_dispatcher,
				in_stream,
				out_stream,
				[this](std::string name, rpc_light::function_args_num_t num, serialization::binary_stream & in, serialization::binary_stream & out) {this->_dispatcher.call_function(name, num, in, out);}
			);

			std::copy(out_stream.cbegin(), out_stream.cend(), out);
		}
		
		test_channel(const test_channel &) = delete;
		test_channel & operator = (const test_channel &) = delete;
	private:
		rpc_light::function_call_dispatcher & _dispatcher;
	};

	TEST_CLASS(rpc_light_test)
	{
	public:
		TEST_METHOD(rpc_light_basic_invocation_test)
		{
			checked_execute([] {
				rpc_light::function_call_dispatcher dispatcher;

				REGISTER_FUNCTION_ENTRY(common_lib_tests_project::f_void_void, dispatcher);

				REGISTER_FUNCTION_ENTRY(common_lib_tests_project::f_void_uint8_t, dispatcher);
				REGISTER_FUNCTION_ENTRY(common_lib_tests_project::f_void_uint16_t, dispatcher);
				REGISTER_FUNCTION_ENTRY(common_lib_tests_project::f_void_uint32_t, dispatcher);
				REGISTER_FUNCTION_ENTRY(common_lib_tests_project::f_void_uint64_t, dispatcher);

				REGISTER_FUNCTION_ENTRY(common_lib_tests_project::f_void_int8_t, dispatcher);
				REGISTER_FUNCTION_ENTRY(common_lib_tests_project::f_void_int16_t, dispatcher);
				REGISTER_FUNCTION_ENTRY(common_lib_tests_project::f_void_int32_t, dispatcher);
				REGISTER_FUNCTION_ENTRY(common_lib_tests_project::f_void_int64_t, dispatcher);

				REGISTER_FUNCTION_ENTRY(common_lib_tests_project::f_uint8_t_2_uint8_t, dispatcher);
				REGISTER_FUNCTION_ENTRY(common_lib_tests_project::f_uint16_t_2_uint16_t, dispatcher);
				REGISTER_FUNCTION_ENTRY(common_lib_tests_project::f_uint32_t_2_uint32_t, dispatcher);
				REGISTER_FUNCTION_ENTRY(common_lib_tests_project::f_uint64_t_2_uint64_t, dispatcher);

				test_channel channel(dispatcher);

				RPC_LIGHT_INVOKE(channel, common_lib_tests_project, f_void_void);
				
				RPC_LIGHT_INVOKE(channel, common_lib_tests_project, f_void_uint8_t, magic_uint8_t);
				RPC_LIGHT_INVOKE(channel, common_lib_tests_project, f_void_uint16_t, magic_uint16_t);
				RPC_LIGHT_INVOKE(channel, common_lib_tests_project, f_void_uint32_t, magic_uint32_t);
				RPC_LIGHT_INVOKE(channel, common_lib_tests_project, f_void_uint64_t, magic_uint64_t);

				RPC_LIGHT_INVOKE(channel, common_lib_tests_project, f_void_int8_t, magic_int8_t);
				RPC_LIGHT_INVOKE(channel, common_lib_tests_project, f_void_int16_t, magic_int16_t);
				RPC_LIGHT_INVOKE(channel, common_lib_tests_project, f_void_int32_t, magic_int32_t);
				RPC_LIGHT_INVOKE(channel, common_lib_tests_project, f_void_int64_t, magic_int64_t);

				Assert::IsTrue(RPC_LIGHT_INVOKE(channel, common_lib_tests_project, f_uint8_t_2_uint8_t, magic_uint8_t, magic_uint8_t) == 2 * magic_uint8_t);
				Assert::IsTrue(RPC_LIGHT_INVOKE(channel, common_lib_tests_project, f_uint16_t_2_uint16_t, magic_uint16_t, magic_uint16_t) == 2 * magic_uint16_t);
				Assert::IsTrue(RPC_LIGHT_INVOKE(channel, common_lib_tests_project, f_uint32_t_2_uint32_t, magic_uint32_t, magic_uint32_t) == 2 * magic_uint32_t);
				Assert::IsTrue(RPC_LIGHT_INVOKE(channel, common_lib_tests_project, f_uint64_t_2_uint64_t, magic_uint64_t, magic_uint64_t) == 2 * magic_uint64_t);
			});
		}

		TEST_METHOD(rpc_light_containers_invocation_test)
		{
			checked_execute([] {
				rpc_light::function_call_dispatcher dispatcher;

				REGISTER_FUNCTION_ENTRY(common_lib_tests_project::f_vector_uint32_t_3_uint32_t, dispatcher);
				REGISTER_FUNCTION_ENTRY(common_lib_tests_project::f_void_vector_uint32, dispatcher);
				REGISTER_FUNCTION_ENTRY(common_lib_tests_project::f_void_vector_uint32_vector_uint32, dispatcher);
				
				test_channel channel(dispatcher);

				auto result = RPC_LIGHT_INVOKE(channel, common_lib_tests_project, f_vector_uint32_t_3_uint32_t, magic_uint32_t, magic2_uint32_t, magic3_uint32_t);
				check_sample_vector_uint32_t(result);

				auto reverted = result;
				std::reverse(reverted.begin(), reverted.end());

				RPC_LIGHT_INVOKE(channel, common_lib_tests_project, f_void_vector_uint32_vector_uint32, result, reverted);

				test_containers<std::uint8_t>();
				test_containers<std::uint16_t>();
				test_containers<std::uint32_t>();
				test_containers<std::uint64_t>();

				test_containers<std::int8_t>();
				test_containers<std::int16_t>();
				test_containers<std::int32_t>();
				test_containers<std::int64_t>();

				test_containers<bool>();
				test_containers<wchar_t>();
			});
		}

		TEST_METHOD(rpc_light_tuple_invocation_test)
		{
			checked_execute([] {
				test_tuple_uint(std::tuple<>());
				test_tuple_uint(std::make_tuple(magic_uint64_t));
				test_tuple_uint(std::make_tuple(magic_uint64_t, magic_uint32_t));
				test_tuple_uint(std::make_tuple(magic_uint64_t, magic_uint32_t, magic_uint16_t, magic_uint8_t));
			});
		}

		TEST_METHOD(rpc_light_stl_exceptions)
		{
			checked_execute([] {
				test_exception<std::exception, std::exception>();
				test_exception<std::bad_alloc, std::bad_alloc>();
				test_exception<std::bad_cast, std::bad_cast>();
				test_exception<std::bad_exception, std::bad_exception>();
				test_exception<std::bad_function_call, std::bad_function_call>();
				test_exception<std::bad_typeid, std::bad_typeid>();
				test_exception<std::runtime_error, std::runtime_error>();
				test_exception<std::invalid_argument, std::invalid_argument>();
				test_exception<std::logic_error, std::logic_error>();
				test_exception<std::length_error, std::length_error>();
				test_exception<std::out_of_range, std::out_of_range>();
				test_exception<std::overflow_error, std::overflow_error>();
				test_exception<std::range_error, std::range_error>();
				test_exception<std::underflow_error, std::underflow_error>();
				test_exception<std::system_error, std::exception>();

				test_exception<int, rpc_light::unexpected_error>();
				test_exception<my_exception, rpc_light::unexpected_error>();
			});
		}

		TEST_METHOD(rpc_light_user_exceptions)
		{
			checked_execute([] {
				rpc_light::function_call_dispatcher dispatcher;
				test_channel channel(dispatcher);

				REGISTER_FUNCTION_ENTRY(common_lib_tests_project::throw_user_exception, dispatcher);
				REGISTER_EXCEPTION(user_exception, dispatcher);

				try
				{
					RPC_LIGHT_INVOKE(channel, common_lib_tests_project, throw_user_exception);
					Assert::Fail(L"Expected user_exception");
				}
				catch (const user_exception& e)
				{
					Assert::AreEqual(10, e.code());
					Assert::AreEqual("Test 10", e.what());
				}
				catch (...)
				{
					Assert::Fail(L"Unexpected exception");
				}
			});
		}

		TEST_METHOD(rpc_light_namespaces_test)
		{
			checked_execute([] {
				rpc_light::function_call_dispatcher dispatcher;

				REGISTER_FUNCTION_ENTRY(a::f_uint64_t_uint64_t, dispatcher);
				REGISTER_FUNCTION_ENTRY(b::f_uint64_t_uint64_t, dispatcher);
				REGISTER_FUNCTION_ENTRY(c::f_uint64_t_uint64_t, dispatcher);

				test_channel channel(dispatcher);

				std::uint64_t x = 123731;

				Assert::IsTrue(RPC_LIGHT_INVOKE(channel, a, f_uint64_t_uint64_t, x) == a::magic_uint64_t * x);
				Assert::IsTrue(RPC_LIGHT_INVOKE(channel, b, f_uint64_t_uint64_t, x) == b::magic_uint64_t * x);
				Assert::IsTrue(RPC_LIGHT_INVOKE(channel, c, f_uint64_t_uint64_t, x) == c::magic_uint64_t * x);
			});
		}
	};
}

namespace rpc_light
{
	template<>
	static void deserialize_exception<common_lib_tests_project::user_exception>(serialization::binary_stream& in)
	{
		int code;
		std::string what;
		in >> code;
		in >> what;
		throw common_lib_tests_project::user_exception(code, what);
	}

	template<>
	static void serialize_exception<common_lib_tests_project::user_exception>(serialization::binary_stream& out, const std::string& name, const common_lib_tests_project::user_exception& e)
	{
		out << name;
		out << e.code();
		out << std::string(e.what());
	}
}