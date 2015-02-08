#pragma once

#include <tuple>
#include <utility>
#include <cstdint>
#include <stdexcept>
#include <functional>
#include <map>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <memory>
#include <common/sync_helpers.h>

#include <tuple_helpers.h>
#include <serialization.h>

#include <common/rpc_light_exceptions.h>

namespace rpc_light
{
	class unexpected_error : public std::runtime_error
	{
	public:
		unexpected_error() : runtime_error("An unexpected exception was thrown on the server side.") {}
	};

	using function_args_num_t = std::uint32_t;

	namespace details
	{
		enum class response_type_t : std::uint8_t
		{
			response_success = 0,
			deserialization_error = 1,
			unexpected_error = 2,
			exception = 3
		};

		template<typename F>
		struct function_traits;

		// function pointer
		template<class R, class... Args>
		struct function_traits<R(*)(Args...)> : public function_traits<R(Args...)> {};

		template<typename R, typename... Args>
		struct function_traits<R(Args...)>
		{
			using return_type = R;
			using args_tuple = std::tuple<Args...>;
		};

		template <typename...  ArgTypes>
		void serialize_function_call(serialization::binary_stream & out, const char * str_func_name, ArgTypes &&... args)
		{
			std::string func_name(str_func_name);
			if (func_name.empty())
			{
				throw std::invalid_argument("Function name is empty.");
			}

			out << func_name;

			function_args_num_t number_of_args = static_cast<function_args_num_t>(sizeof...(ArgTypes));
			out << number_of_args;

			out << std::make_tuple(args...);
		}

		//  TODO: auto ->
		template<std::size_t I = 0, typename FuncT, typename... Tp>
		inline typename std::enable_if<I == sizeof...(Tp), void>::type
		exception_deserialization_helper(std::tuple<Tp...> &, FuncT)
		{

		}

		template<std::size_t I = 0, typename FuncT, typename... Tp>
		inline typename std::enable_if < I < sizeof...(Tp), void>::type
			exception_deserialization_helper(std::tuple<Tp...>& t, FuncT f)
		{
			f(std::get<I>(t));
			exception_deserialization_helper<I + 1, FuncT, Tp...>(t, f);
		}

		inline void process_std_exception(serialization::binary_stream & stream, deserialize_exception_helper_t deserialize_exception_func)
		{
			std::string name;
			stream >> name;

			if (!deserialize_exception_func(name, stream))
			{
				if (!rpc_light::deserialize_exception_helper(name, stream))
				{
					throw std::logic_error("Exception deserializer " + name + " was not found");
				}
			}
		}

		inline void process_response_header(serialization::binary_stream & stream, deserialize_exception_helper_t deserialize_exception_func)
		{
			std::underlying_type<response_type_t>::type response_type;
			stream >> response_type;
			switch (static_cast<response_type_t>(response_type))
			{
			case response_type_t::response_success: return;
			case response_type_t::deserialization_error: throw deserialization_error();
			case response_type_t::unexpected_error: throw unexpected_error();
			case response_type_t::exception: process_std_exception(stream, deserialize_exception_func); break;
			default: throw response_type_error();
			}
		}

		template <typename TRetVal>
		typename std::enable_if<!(std::is_void<TRetVal>::value), TRetVal>::type
		get_result_from_response(serialization::binary_stream & stream)
		{
			TRetVal result;
			stream >> result;
			return result;
		}

		template <typename TRetVal>
		typename std::enable_if<std::is_void<TRetVal>::value, void>::type
		get_result_from_response(serialization::binary_stream & ) {}

		inline void write_success_response(serialization::binary_stream & out)
		{
			out << static_cast<std::underlying_type<response_type_t>::type>(response_type_t::response_success);
		}

		inline void write_error_response(serialization::binary_stream & out, response_type_t error_type)
		{
			out << static_cast<std::underlying_type<response_type_t>::type>(error_type);
		}

		template <typename value_t>
		void write_success_response(value_t && value, serialization::binary_stream & out)
		{
			write_success_response(out);
			out << value;
		}

#pragma warning(push)
#pragma warning(disable : 4100) // 'tuple' : unreferenced formal parameter for std::tuple<>
		template <typename result_t, typename func_t, typename tuple_t, std::size_t... n>
		typename std::enable_if<std::is_void<result_t>::value, void>::type
		call_function_impl(serialization::binary_stream & out, func_t func, tuple_t tuple, tuple_helpers::seq<n...>)
		{
			func(std::get<n>(tuple)...);
			write_success_response(out);
		}

		template <typename result_t, typename func_t, typename tuple_t, std::size_t... n>
		typename std::enable_if<!(std::is_void<result_t>::value), void>::type
		call_function_impl(serialization::binary_stream & out, func_t func, tuple_t tuple, tuple_helpers::seq<n...>)
		{
			write_success_response(func(std::get<n>(tuple)...), out);
		}
	}
#pragma warning(pop)

	template <typename TRetVal, typename TFunc, typename TChannel, typename...  ArgTypes>
	TRetVal invoke(TChannel & channel, const char * func_name, deserialize_exception_helper_t deserialize_exception_func, ArgTypes &&... args)
	{
		using prototype_function_traits = details::function_traits<TFunc>;
		using arguments_type = std::tuple<typename std::decay<ArgTypes>::type ...>;

		static_assert(std::is_same<prototype_function_traits::return_type, TRetVal>::value, "Return type of the function differs from prototype.");
		static_assert(std::tuple_size<prototype_function_traits::args_tuple>::value == std::tuple_size<arguments_type>::value, "The number of passed arguments differs from number of arguments of the function prototype.");
		static_assert(std::is_same<prototype_function_traits::args_tuple, arguments_type>::value, "Type of the function arguments differs from prototype.");

		if (func_name == nullptr)
			throw std::invalid_argument("The passed argument func_name can't be equal to nullptr");

		serialization::binary_stream client_server;
		details::serialize_function_call(client_server, func_name, std::forward<ArgTypes>(args)...);

		serialization::binary_stream server_client;
		channel.send(client_server.cbegin(), client_server.cend(), server_client.insert_iter());

		details::process_response_header(server_client, deserialize_exception_func);
		return details::get_result_from_response<TRetVal>(server_client);
	}

	using dispatch_func_t = std::function<void(std::string, function_args_num_t, serialization::binary_stream &, serialization::binary_stream &)>;

	template <typename TFunc>
	void call_function(TFunc func, function_args_num_t number_of_args, serialization::binary_stream & in, serialization::binary_stream & out)
	{
		using func_traits = details::function_traits<TFunc>;
		if (std::tuple_size<func_traits::args_tuple>::value != number_of_args)
		{
			throw deserialization_error();
		}

		func_traits::args_tuple args;
		in >> args;
		details::call_function_impl<func_traits::return_type>(
			out,
			func,
			args,
			tuple_helpers::make_seq<std::tuple_size<func_traits::args_tuple>::value>());
	}

	using call_dispatcher_entry = std::function<void(rpc_light::function_args_num_t args_num, serialization::binary_stream & in, serialization::binary_stream & out)>;
	using exception_serialization_function = std::function<void(serialization::binary_stream&, const std::string&, const std::exception&)>;

#define REGISTER_EXCEPTION(exception_type_name, dispatcher) \
	dispatcher.register_exception<exception_type_name>(#exception_type_name)

	class function_call_dispatcher final
	{
	public:
		function_call_dispatcher()
		{
			register_standard_exceptions();
		}

		inline void register_function_entry(std::string func_name, call_dispatcher_entry entry)
		{
			if (func_name.empty())
				throw std::invalid_argument("The passed argument func_name can't be empty");

			auto const result = _entries.insert(std::make_pair(func_name, entry));
			if (!result.second)
			{
				throw std::logic_error("An entry for the specified function already exists.");
			}
		}
	
		inline void call_function(std::string func_name, rpc_light::function_args_num_t args_num, serialization::binary_stream & in, serialization::binary_stream & out) const
		{
			auto const iter = _entries.find(func_name);
			if (iter == _entries.end())
			{
				throw std::runtime_error("Could not find an entry for the passed function name.");
			}
			
			iter->second(args_num, in, out);
		}

		template<typename exception_t>
		void register_exception(const std::string& exception_class_name)
		{
			exceptions_.insert({ std::type_index(typeid(exception_t)), exception_serialization_entry{
				exception_class_name,
				[](serialization::binary_stream& stream, const std::string& exception_name, const std::exception& exception)
				{
					rpc_light::serialize_exception(stream, exception_name, static_cast<const exception_t&>(exception));
				}
			} });
		}

		function_call_dispatcher(function_call_dispatcher && x)
		{
			std::swap(_entries, x._entries);
		}

		function_call_dispatcher & operator = (function_call_dispatcher && x)
		{
			std::swap(_entries, x._entries);
			return *this;
		}

		function_call_dispatcher(const function_call_dispatcher &) = delete;
		function_call_dispatcher & operator = (const function_call_dispatcher &) = delete;
	private:
		std::map<std::string, call_dispatcher_entry> _entries;

		using exception_serialization_entry = std::pair < std::string, exception_serialization_function > ;
		std::unordered_map<std::type_index, exception_serialization_entry> exceptions_;

		friend inline void execute_function(const function_call_dispatcher& dispatcher, serialization::binary_stream & in, serialization::binary_stream & out, dispatch_func_t dispatch_func);

		void register_standard_exceptions()
		{
			REGISTER_EXCEPTION(std::bad_alloc, (*this));
			REGISTER_EXCEPTION(std::bad_cast, (*this));
			REGISTER_EXCEPTION(std::bad_exception, (*this));
			REGISTER_EXCEPTION(std::bad_function_call, (*this));
			REGISTER_EXCEPTION(std::bad_typeid, (*this));
			REGISTER_EXCEPTION(std::runtime_error, (*this));
			REGISTER_EXCEPTION(std::invalid_argument, (*this));
			REGISTER_EXCEPTION(std::logic_error, (*this));
			REGISTER_EXCEPTION(std::length_error, (*this));
			REGISTER_EXCEPTION(std::out_of_range, (*this));
			REGISTER_EXCEPTION(std::overflow_error, (*this));
			REGISTER_EXCEPTION(std::range_error, (*this));
			REGISTER_EXCEPTION(std::underflow_error, (*this));
			REGISTER_EXCEPTION(std::exception, (*this));
		}

		void serialize_exception(serialization::binary_stream & out, const std::exception& e) const
		{
			auto iter = exceptions_.find(std::type_index(typeid(e)));
			if (iter == std::end(exceptions_))
			{
				iter = exceptions_.find(std::type_index(typeid(std::exception)));
				if (iter == std::end(exceptions_))
					throw std::logic_error("std::exception was not registered");
			}
			iter->second.second(out, iter->second.first, e);
		}
	};

	inline void execute_function(const function_call_dispatcher& dispatcher, serialization::binary_stream & in, serialization::binary_stream & out, dispatch_func_t dispatch_func)
	{
		try
		{
			std::string func_name;
			in >> func_name;
			if (func_name.empty())
			{
				throw deserialization_error();
			}

			function_args_num_t number_of_args;
			in >> number_of_args;

			dispatch_func(func_name, number_of_args, in, out);
			return;
		}
		catch (const deserialization_error &)
		{
			write_error_response(out, details::response_type_t::deserialization_error);
		}
		catch (const std::exception & exc)
		{
			write_error_response(out, details::response_type_t::exception);
			dispatcher.serialize_exception(out, exc);
		}
		catch (...)
		{
			write_error_response(out, details::response_type_t::unexpected_error);
		}
	}
} // namespace rpc_light

#define RPC_LIGHT_INVOKE(channel, namespace_name, func, ...) rpc_light::invoke<decltype(namespace_name::func(__VA_ARGS__)), decltype(namespace_name::func)>(channel, #namespace_name "::" #func, &namespace_name::deserialize_exception_helper, __VA_ARGS__)

#define REGISTER_FUNCTION_ENTRY(function, dispatcher) \
	dispatcher.register_function_entry(#function, \
	[](rpc_light::function_args_num_t args_num, serialization::binary_stream & in, serialization::binary_stream & out) { rpc_light::call_function(function, args_num, in, out); })