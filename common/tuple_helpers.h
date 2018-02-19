#pragma once

#include <cstddef>
#include <tuple>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <type_traits>

namespace tuple_helpers
{
#define DECLARE_TUPLE_GET(tuple_type, index_type) \
template <index_type index> \
auto get(tuple_type & tuple) -> decltype(std::get<static_cast<std::size_t>(index)>(tuple)) \
{ \
	return std::get<static_cast<std::size_t>(index)>(tuple); \
} \
template <index_type index> \
auto get(const tuple_type & tuple) -> decltype(std::get<static_cast<std::size_t>(index)>(tuple)) \
{ \
	return std::get<static_cast<std::size_t>(index)>(tuple); \
}

	template <std::size_t> struct depth_hint { };

	template <std::size_t ...s> struct seq { typedef seq<s...> type; };
	template <std::size_t max, std::size_t N = 0, std::size_t...s> struct make_seq :
		std::conditional<(N < max), make_seq<max, N + 1, max - N - 1, s...>, seq<s...> >::type{};

	namespace details
	{
		template <typename tuple_t, typename func_t>
		inline void for_each_in_tuple_impl(tuple_t & , func_t & , depth_hint<0>) { }

		template <typename tuple_t, typename func_t, size_t N>
		inline void for_each_in_tuple_impl(tuple_t & tuple, func_t & func, depth_hint<N>)
		{
			for_each_in_tuple_impl(tuple, func, depth_hint<N - 1>());
			func(std::get<N - 1>(tuple));
		}

		template <typename tuple_t, typename func_t>
		inline void for_each_in_tuple_impl(tuple_t & tuple, func_t & func)
		{
			for_each_in_tuple_impl(tuple, func, depth_hint<std::tuple_size<tuple_t>::value>());
		}

		template <typename tuple_t, typename func_t>
		inline void for_each_tuple_index_impl(func_t &, depth_hint<0>) { }

		template <typename tuple_t, typename func_t, size_t N>
		inline void for_each_tuple_index_impl(func_t & func, depth_hint<N>)
		{
			for_each_tuple_index_impl<tuple_t>(func, depth_hint<N - 1>());
			func(depth_hint<N - 1>());
		}

		template <typename tuple_t, typename func_t>
		inline void for_each_tuple_index_impl(func_t & func)
		{
			for_each_tuple_index_impl<tuple_t>(func, depth_hint<std::tuple_size<tuple_t>::value>());
		}

		template<typename, typename>
		struct append_to_type_seq { };

		template<typename T, typename... Ts>
		struct append_to_type_seq<T, std::tuple<Ts...>>
		{
			using type = std::tuple<Ts..., T>;
		};

		template<typename... Ts>
		struct revert_type_seq
		{
			using type = std::tuple<>;
		};

		template<typename T, typename... Ts>
		struct revert_type_seq<T, Ts...>
		{
			using type = typename append_to_type_seq<
				T,
				typename revert_type_seq<Ts...>::type
			>::type;
		};

		template <std::size_t... Is>
		struct index_list { };

		template <std::size_t MIN, std::size_t N, std::size_t... Is>
		struct range_builder;

		template <std::size_t MIN, std::size_t... Is>
		struct range_builder<MIN, MIN, Is...>
		{
			typedef index_list<Is...> type;
		};

		template <std::size_t MIN, std::size_t N, std::size_t... Is>
		struct range_builder : public range_builder<MIN, N - 1, N - 1, Is...>
		{ };

		template<std::size_t MIN, std::size_t MAX>
		using index_range = typename range_builder<MIN, MAX>::type;

		template<typename... Args, std::size_t... Is>
		typename revert_type_seq<Args...>::type
		revert_tuple(std::tuple<Args...> t, index_list<Is...>)
		{
			using reverted_tuple = typename revert_type_seq<Args...>::type;

			// Forwarding machinery that handles both lvalues and rvalues...
			auto rt = std::forward_as_tuple(
				std::forward<
				typename std::conditional<
				std::is_lvalue_reference<
				typename std::tuple_element<Is, reverted_tuple>::type
				>::value,
				typename std::tuple_element<Is, reverted_tuple>::type,
				typename std::remove_reference<
				typename std::tuple_element<Is, reverted_tuple>::type
				>::type
				>::type
				>(std::get<sizeof...(Args) - Is - 1>(t))...
				);

			return rt;
		}
	}

	template<typename func_t, typename ...types>
	void for_each_in_tuple(std::tuple<types...> & tpl, func_t & func)
	{
		details::for_each_in_tuple_impl(tpl, func);
	}

	template<typename func_t, typename ...types>
	void for_each_in_tuple(const std::tuple<types...> & tpl, func_t & func)
	{
		details::for_each_in_tuple_impl(const_cast<typename std::remove_cv<decltype(tpl)>::type>(tpl), func);
	}

	template<typename tuple_t, typename func_t>
	void for_each_tuple_index(func_t & func)
	{
		details::for_each_tuple_index_impl<tuple_t>(func);
	}

	template<typename... types>
	typename details::revert_type_seq<types...>::type
	revert_tuple(std::tuple<types...> t)
	{
		return details::revert_tuple(t, details::index_range<0, sizeof...(types)>());
	}

	template<typename T>
	std::wstring format_argument(const T& argument)
	{
		std::wostringstream stream;
		stream << argument;
		return stream.str();
	}

	template<typename tuple_t>
	void tuple_to_strings(const tuple_t& t, std::vector<std::wstring>& args_as_strings, seq<0>)
	{
		args_as_strings.push_back(format_argument(std::get<std::tuple_size<tuple_t>::value - 0 - 1>(t)));
	}

	template<typename tuple_t, std::size_t N>
	void tuple_to_strings(const tuple_t& t, std::vector<std::wstring>& args_as_strings, seq<N>)
	{
		args_as_strings.push_back(format_argument(std::get<std::tuple_size<tuple_t>::value - N - 1>(t)));
		tuple_to_strings(t, args_as_strings, tuple_helpers::seq<N - 1>());
	}

	template <typename T, template <typename...> class Template>
	struct is_specialization_of : std::false_type {};

	template <template <typename...> class Template, typename... Args>
	struct is_specialization_of<Template<Args...>, Template> : std::true_type{};

	template<typename T>
	struct is_tuple
	{
		static const bool value = is_specialization_of<typename std::decay<T>::type, std::tuple>::value;
	};

	template<typename T>
	struct is_array
	{
		static const bool value = false;
	};

	template<template <typename, std::size_t> class Template, typename item_t, std::size_t item_count>
	struct is_array<Template<item_t, item_count>>
	{
		static const bool value = std::is_base_of<std::array<item_t, item_count>, Template<item_t, item_count>>::value;
	};

} // namespace tuple_helpers