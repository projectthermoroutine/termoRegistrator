
#pragma once

#include <string>
#include <string_view>


inline namespace std_ext
{
	template<
		class _Elem,
		class _Traits = std::char_traits<_Elem>,
		class _Alloc = std::allocator<_Elem>
	>
	inline std::basic_string<_Elem, _Traits, _Alloc> operator+(std::basic_string<_Elem, _Traits, _Alloc>&& _Left, std::basic_string_view<_Elem, _Traits> _Right)
	{
		return std::move(_Left.append(_Right));
	}

	template<
		class _Elem,
		class _Traits = std::char_traits<_Elem>,
		class _Alloc = std::allocator<_Elem>
	>
	inline std::basic_string<_Elem, _Traits, _Alloc> operator+(const std::basic_string<_Elem, _Traits, _Alloc>& _Left, std::basic_string_view<_Elem, _Traits> _Right)
	{
		std::basic_string<_Elem, _Traits, _Alloc> result;
		result.reserve(_Left.length() + _Right.length());
		return std::move(result.append(_Left).append(_Right));
	}

	template<
		class _Elem,
		class _Traits = std::char_traits<_Elem>,
		class _Alloc = std::allocator<_Elem>
	>
	inline std::basic_string<_Elem, _Traits, _Alloc> operator+(std::basic_string_view<_Elem, _Traits> _Left, std::basic_string<_Elem, _Traits, _Alloc>&& _Right)
	{
		return _Right.insert(0, _Left), std::move(_Right);
	}

	template<
		class _Elem,
		class _Traits = std::char_traits<_Elem>,
		class _Alloc = std::allocator<_Elem>
	>
	inline std::basic_string<_Elem, _Traits, _Alloc> operator+(std::basic_string_view<_Elem, _Traits> _Left, const std::basic_string<_Elem, _Traits, _Alloc>& _Right)
	{
		std::basic_string<_Elem, _Traits, _Alloc> result;
		result.reserve(_Left.length() + _Right.length());
		return std::move(result.append(_Left).append(_Right));
	}

	template<
		class _Elem,
		class _Traits = std::char_traits<_Elem>,
		class _Alloc = std::allocator<_Elem>
	>
	inline std::basic_string<_Elem, _Traits, _Alloc> operator+(std::basic_string_view<_Elem, _Traits> _Left, std::basic_string_view<_Elem, _Traits> _Right)
	{
		std::basic_string<_Elem, _Traits, _Alloc> result;
		result.reserve(_Left.length() + _Right.length());
		return std::move(result.append(_Left).append(_Right));
	}

	template<
		class _Elem,
		class _Traits = std::char_traits<_Elem>,
		class _Alloc = std::allocator<_Elem>
	>
	inline std::basic_string<_Elem, _Traits, _Alloc> operator+(std::basic_string_view<_Elem, _Traits> _Left, const _Elem* _Right)
	{
		return _Left + std::basic_string_view<_Elem, _Traits>(_Right);
	}

	template<
		class _Elem,
		class _Traits = std::char_traits<_Elem>,
		class _Alloc = std::allocator<_Elem>
	>
	inline std::basic_string<_Elem, _Traits, _Alloc> operator+(const _Elem* _Left, std::basic_string_view<_Elem, _Traits> _Right)
	{
		return std::basic_string_view<_Elem, _Traits>(_Left) + _Right;
	}

	template<
		class _Elem,
		class _Traits = std::char_traits<_Elem>,
		class _Alloc = std::allocator<_Elem>
	>
	inline std::basic_string<_Elem, _Traits, _Alloc> operator+(std::basic_string_view<_Elem, _Traits> _Left, _Elem _Right)
	{
		std::basic_string<_Elem, _Traits, _Alloc> result;
		result.reserve(_Left.length() + 1);
		return std::move(result.append(_Left).append(1, _Right));
	}

	template<
		class _Elem,
		class _Traits = std::char_traits<_Elem>,
		class _Alloc = std::allocator<_Elem>
	>
	inline std::basic_string<_Elem, _Traits, _Alloc> operator+(_Elem _Left, std::basic_string_view<_Elem, _Traits> _Right)
	{
		std::basic_string<_Elem, _Traits, _Alloc> result;
		result.reserve(_Right.length() + 1);
		return std::move(result.append(1, _Left).append(_Right));
	}

} // inline namespace std_ext
