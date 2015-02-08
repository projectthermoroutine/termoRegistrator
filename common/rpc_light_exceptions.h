/////////////////////////////////////
// (C) 2014 ООО "Код Безопасности"
// Проект: SNES-AV eset
// Автор: Зайцев Роман
// Создан: 22.10.2014
// Краткое описание: модуль протаскивания исключений унаследованных от std::exception через трубу
/////////////////////////////////////
#pragma once

#include <array>
#include <string>
#include <functional>
#include <common/serialization.h>
#include <loglib/log.h>

namespace rpc_light
{
	class deserialization_error: public std::exception {};
	class serialization_error: public std::exception {};
	class response_type_error: public std::exception {};

	template<typename T>
	inline void deserialize_exception(serialization::binary_stream& /*in*/)
	{
		static_assert(std::is_base_of<std::exception, T>::value, "Class "__FUNCSIG__" must be derived from std::exception");
		LOG_DEBUG() << "Exception deserialization failed";
		throw deserialization_error();
	}

#define DESERIALIZE_EXCEPTION_WITHOUT_PARAMETERS(exception_type) \
	template<> \
	inline void deserialize_exception<exception_type>(serialization::binary_stream& /*in*/) \
	{ throw exception_type(); }

#define DESERIALIZE_EXCEPTION_WITH_STRING(exception_type) \
	template<> \
	inline void deserialize_exception<exception_type>(serialization::binary_stream& in) \
	{ std::string what; in >> what; throw exception_type(what); }

	DESERIALIZE_EXCEPTION_WITHOUT_PARAMETERS(std::bad_alloc);
	DESERIALIZE_EXCEPTION_WITHOUT_PARAMETERS(std::bad_cast);
	DESERIALIZE_EXCEPTION_WITHOUT_PARAMETERS(std::bad_exception);
	DESERIALIZE_EXCEPTION_WITHOUT_PARAMETERS(std::bad_function_call);
	DESERIALIZE_EXCEPTION_WITHOUT_PARAMETERS(std::bad_typeid);
	DESERIALIZE_EXCEPTION_WITHOUT_PARAMETERS(std::exception);
	DESERIALIZE_EXCEPTION_WITH_STRING(std::runtime_error);
	DESERIALIZE_EXCEPTION_WITH_STRING(std::invalid_argument);
	DESERIALIZE_EXCEPTION_WITH_STRING(std::logic_error);
	DESERIALIZE_EXCEPTION_WITH_STRING(std::length_error);
	DESERIALIZE_EXCEPTION_WITH_STRING(std::out_of_range);
	DESERIALIZE_EXCEPTION_WITH_STRING(std::overflow_error);
	DESERIALIZE_EXCEPTION_WITH_STRING(std::range_error);
	DESERIALIZE_EXCEPTION_WITH_STRING(std::underflow_error);

	template<typename exception_t>
	inline void serialize_exception(serialization::binary_stream& /*out*/, const std::string& /*name*/, const exception_t& /*e*/)
	{
		static_assert(std::is_base_of<std::exception, exception_t>::value, "Class "__FUNCSIG__" must be derived from std::exception");
		throw serialization_error();
	}

#define SERIALIZE_EXCEPTION_WITHOUT_PARAMETERS(exception_type) \
	template<> inline void serialize_exception<exception_type>(serialization::binary_stream& out, const std::string& name, const exception_type& /*e*/) \
	{ out << name; }

#define SERIALIZE_EXCEPTION_WITH_STRING(exception_type) \
	template<> inline void serialize_exception<exception_type>(serialization::binary_stream& out, const std::string& name, const exception_type& e) \
	{ out << name; out << std::string(e.what()); }

	SERIALIZE_EXCEPTION_WITHOUT_PARAMETERS(std::bad_alloc);
	SERIALIZE_EXCEPTION_WITHOUT_PARAMETERS(std::bad_cast);
	SERIALIZE_EXCEPTION_WITHOUT_PARAMETERS(std::bad_exception);
	SERIALIZE_EXCEPTION_WITHOUT_PARAMETERS(std::bad_function_call);
	SERIALIZE_EXCEPTION_WITHOUT_PARAMETERS(std::bad_typeid);
	SERIALIZE_EXCEPTION_WITHOUT_PARAMETERS(std::exception);
	SERIALIZE_EXCEPTION_WITH_STRING(std::runtime_error);
	SERIALIZE_EXCEPTION_WITH_STRING(std::invalid_argument);
	SERIALIZE_EXCEPTION_WITH_STRING(std::logic_error);
	SERIALIZE_EXCEPTION_WITH_STRING(std::length_error);
	SERIALIZE_EXCEPTION_WITH_STRING(std::out_of_range);
	SERIALIZE_EXCEPTION_WITH_STRING(std::overflow_error);
	SERIALIZE_EXCEPTION_WITH_STRING(std::range_error);
	SERIALIZE_EXCEPTION_WITH_STRING(std::underflow_error);

	template<typename... T>
	class throw_exception
	{
	public:
		template<typename T>
		static void make_exception(serialization::binary_stream& in)
		{
			return deserialize_exception<T>(in);
		}

		void operator()(int index, serialization::binary_stream& in)
		{
#if 0
			//  MSVC 2013 U3 compiler bug workaround
			using make_exception_func = void(*)(serialization::binary_stream&);
			static std::array<make_exception_func, sizeof...(T)> factories = { &make_exception<T>... };
			return factories.at(index)(in);
#else
			static void(*factories[])(serialization::binary_stream&) = { &make_exception<T>... };
			if (index < 0 || index >= sizeof...(T))
				throw std::range_error("Range check error");
			return factories[index](in);
#endif // 0
		}
	};

	template<>
	class throw_exception<>
	{
	public:
		void operator()(int /*index*/, serialization::binary_stream& /*in*/) {}
	};

	using deserialize_exception_helper_t = bool(*)(std::string name, serialization::binary_stream&);

#define EXCEPTION_LIST(...) \
	inline bool deserialize_exception_helper(std::string name, serialization::binary_stream& in) \
		{ \
			static const std::string exception_names = #__VA_ARGS__",";	\
			name += ","; \
			\
			const auto pos = exception_names.find(name); \
			if (pos == exception_names.npos) \
				return false; \
			else \
			{ \
				const auto exception_index = std::count(std::cbegin(exception_names), std::cbegin(exception_names) + pos, ','); \
				rpc_light::throw_exception< __VA_ARGS__ >()(static_cast<int>(exception_index), in); \
				return true; \
			} \
		}

	EXCEPTION_LIST(std::bad_alloc, std::bad_cast, std::bad_exception, std::bad_function_call, std::bad_typeid, std::runtime_error, std::invalid_argument, std::logic_error, std::length_error, std::out_of_range, std::overflow_error, std::range_error, std::underflow_error, std::exception);
}