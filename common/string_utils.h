#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <cstdint>
#include <iterator>
#include <sstream>
#include <numeric>

#include <common/std_string_view_extended_operators.hpp>


namespace string_utils
{
	template<class _StrOrView>
	using str_t = std::basic_string<typename _StrOrView::value_type, typename _StrOrView::traits_type, std::allocator<typename _StrOrView::value_type>>;

	template<class _StrOrView>
	using view_t = std::basic_string_view<typename _StrOrView::value_type, typename _StrOrView::traits_type>;

	template<class _StrOrView>
	using char_t = typename _StrOrView::value_type;


	bool compare_case_insensitive(std::wstring_view string1, std::wstring_view string2);
	bool compare_case_insensitive(const wchar_t* str1_ptr, std::size_t str1_size, const wchar_t* str2_ptr, std::size_t str2_size);

	std::wstring convert_utf8_to_wchar(std::string_view str_view);
	std::string convert_wchar_to_utf8(std::wstring_view str_view);

	std::string trim(const std::string &s);
	std::wstring trim(const std::wstring &s);
	std::string trim_begin(const std::string &s);
	std::wstring trim_begin(const std::wstring &s);

	template<class _Elem>
	constexpr std::basic_string_view<_Elem, std::char_traits<_Elem>> spaces_chars() noexcept
	{
		using _StrView = std::basic_string_view<_Elem, std::char_traits<_Elem>>;
		static _Elem chars[] =
		{
			_StrView::traits_type::to_char_type(' '),
			_StrView::traits_type::to_char_type('\f'),
			_StrView::traits_type::to_char_type('\n'),
			_StrView::traits_type::to_char_type('\r'),
			_StrView::traits_type::to_char_type('\t'),
			_StrView::traits_type::to_char_type('\v'),
		};
		return _StrView(chars, std::size(chars));
	}

	template<class _Elem>
	std::basic_string_view<_Elem, std::char_traits<_Elem>>
		trim_begin(std::basic_string_view<_Elem, std::char_traits<_Elem>> sv)
	{
		using _StrView = std::basic_string_view<_Elem, std::char_traits<_Elem>>;
		const typename _StrView::size_type pos(sv.find_first_not_of(spaces_chars<_Elem>()));
		return (pos == sv.npos) ? sv.substr(0, 0) : _StrView(sv.data() + pos, sv.length() - pos);
	}

	template<class _Elem>
	std::basic_string_view<_Elem, std::char_traits<_Elem>>
		trim_end(std::basic_string_view<_Elem, std::char_traits<_Elem>> sv)
	{
		using _StrView = std::basic_string_view<_Elem, std::char_traits<_Elem>>;
		const typename _StrView::size_type pos(sv.find_last_not_of(spaces_chars<_Elem>()));
		return (pos == sv.npos) ? sv.substr(0, 0) : _StrView(sv.data(), pos + 1);
	}

	template<class _Elem>
	std::basic_string_view<_Elem, std::char_traits<_Elem>>
		trim(std::basic_string_view<_Elem, std::char_traits<_Elem>> sv)
	{
		return trim_begin(trim_end(sv));
	}


	constexpr bool ends_with(std::string_view src, std::string_view expected_end)
	{
		return (src.size() >= expected_end.size() && src.compare(src.size() - expected_end.size(), std::wstring_view::npos, expected_end) == 0);
	}
	constexpr bool ends_with(std::wstring_view src, std::wstring_view expected_end)
	{
		return (src.size() >= expected_end.size() && src.compare(src.size() - expected_end.size(), std::wstring_view::npos, expected_end) == 0);
	}

	constexpr bool starts_with(std::string_view src, std::string_view expected_start)
	{
		return (src.size() >= expected_start.size() && src.compare(0, expected_start.size(), expected_start) == 0);
	}
	constexpr bool starts_with(std::wstring_view src, std::wstring_view expected_start)
	{
		return (src.size() >= expected_start.size() && src.compare(0, expected_start.size(), expected_start) == 0);
	}

	template<class _StringView>
	inline constexpr bool ends_with_ex(std::wstring_view src, _StringView expected_end)
	{
		return ends_with(src, expected_end);
	}
	template<class _StringView, class... _StringsView>
	inline constexpr bool ends_with_ex(std::wstring_view src, _StringView expected_end, _StringsView... expecteds_end)
	{
		return ends_with(src, expected_end) || ends_with_ex(src, expecteds_end...);
	}

	template<class _StringView>
	inline constexpr bool starts_with_ex(std::wstring_view src, _StringView expected_start)
	{
		return starts_with(src, expected_start);
	}
	template<class _StringView, class... _StringsView>
	inline constexpr bool starts_with_ex(std::wstring_view src, _StringView expected_start, _StringsView... expecteds_start)
	{
		return starts_with(src, expected_start) || starts_with_ex(src, expecteds_start...);
	}


	bool ends_with_case_insensitive(std::wstring_view src, std::wstring_view expected_end);
	bool starts_with_case_insensitive(std::wstring_view src, std::wstring_view expected_start);

	template<class _StringView>
	inline bool ends_with_case_insensitive_ex(std::wstring_view src, _StringView expected_end)
	{
		return ends_with_case_insensitive(src, expected_end);
	}
	template<class _StringView, class... _StringsView>
	inline bool ends_with_case_insensitive_ex(std::wstring_view src, _StringView expected_end, _StringsView... expecteds_end)
	{
		return ends_with_case_insensitive(src, expected_end) || ends_with_case_insensitive_ex(src, expecteds_end...);
	}

	template<class _StringView>
	inline bool starts_with_case_insensitive_ex(std::wstring_view src, _StringView expected_start)
	{
		return starts_with_case_insensitive(src, expected_start);
	}
	template<class _StringView, class... _StringsView>
	inline bool starts_with_case_insensitive_ex(std::wstring_view src, _StringView expected_start, _StringsView... expecteds_start)
	{
		return starts_with_case_insensitive(src, expected_start) || starts_with_case_insensitive_ex(src, expecteds_start...);
	}


	namespace details
	{
		template<class _Elem>
		constexpr std::basic_string_view<_Elem, std::char_traits<_Elem>> hex_chars{};
		template<>
		constexpr std::basic_string_view<char, std::char_traits<char>> hex_chars<char>{ "0123456789ABCDEF" };
		template<>
		constexpr std::basic_string_view<wchar_t, std::char_traits<wchar_t>> hex_chars<wchar_t>{ L"0123456789ABCDEF" };

		enum write_order_t
		{
			direct,
			reverse
		};

		enum byte_order_t
		{
			little_endian,
			big_endian
		};
		inline byte_order_t byte_order() noexcept
		{
			const std::uint16_t value{ 0x00FF };
			return (*reinterpret_cast<const std::uint8_t*>(&value) == 0x00) ? byte_order_t::big_endian : byte_order_t::little_endian;
		}

		template<write_order_t _WriteOrder, class _String, class _Byte>
		_String to_xstring(const _Byte* bytes_ptr, std::size_t bytes_count)
		{
			static_assert(sizeof(_Byte) == sizeof(std::uint8_t));
			static_assert(std::is_pod_v<_Byte> == std::is_pod_v<std::uint8_t>);

			_String result(2 * bytes_count, L'\0');

			typename _String::pointer dest_ptr{ result.data() };

			if constexpr (_WriteOrder == write_order_t::reverse)
			{
				bytes_ptr += bytes_count - 1;
			}

			for (; bytes_count > 0; bytes_count -= 1)
			{
				const std::uint8_t byte{ static_cast<std::uint8_t>(*bytes_ptr) };
				(*dest_ptr++) = hex_chars<char_t<_String>>[byte >> 4];
				(*dest_ptr++) = hex_chars<char_t<_String>>[byte & 0x0F];

				if constexpr (_WriteOrder == write_order_t::direct)
				{
					bytes_ptr += 1;
				}
				else
				{
					static_assert(_WriteOrder == write_order_t::reverse);
					bytes_ptr -= 1;
				}
			}

			return result;
		}

	} // namespace details
	template<class _Byte = std::byte>
	inline std::string to_string(const _Byte* bytes_ptr, std::size_t bytes_count)
	{
		return details::to_xstring<details::write_order_t::direct, std::string>(bytes_ptr, bytes_count);
	}
	template<class _Byte = std::byte, template<class item_t, class allocator_t = std::allocator<item_t>> class container_t>
	inline std::string to_string(const container_t<_Byte>& bytes)
	{
		using std::data, std::size;
		return details::to_xstring<details::write_order_t::direct, std::string>(data(bytes), size(bytes));
	}
	template <class _Type>
	inline std::string to_hex_string(_Type value)
	{
		using namespace std::string_view_literals;

		if (details::byte_order_t::little_endian == details::byte_order())
			return "0x"sv + details::to_xstring<details::write_order_t::reverse, std::string, std::byte>(reinterpret_cast<std::byte*>(&value), sizeof(value));
		else
			return "0x"sv + details::to_xstring<details::write_order_t::direct, std::string, std::byte>(reinterpret_cast<std::byte*>(&value), sizeof(value));
	}
	template<class _Byte = std::byte>
	inline std::wstring to_wstring(const _Byte* bytes_ptr, std::size_t bytes_count)
	{
		return details::to_xstring<details::write_order_t::direct, std::wstring>(bytes_ptr, bytes_count);
	}
	template<class _Byte = std::byte, template<class item_t, class allocator_t = std::allocator<item_t>> class container_t>
	inline std::wstring to_wstring(const container_t<_Byte>& bytes)
	{
		using std::data, std::size;
		return details::to_xstring<details::write_order_t::direct, std::wstring>(data(bytes), size(bytes));
	}
	template <class _Type>
	inline std::wstring to_hex_wstring(_Type value)
	{
		using namespace std::string_view_literals;

		if (details::byte_order_t::little_endian == details::byte_order())
			return L"0x"sv + details::to_xstring<details::write_order_t::reverse, std::wstring, std::byte>(reinterpret_cast<std::byte*>(&value), sizeof(value));
		else
			return L"0x"sv + details::to_xstring<details::write_order_t::direct, std::wstring, std::byte>(reinterpret_cast<std::byte*>(&value), sizeof(value));
	}

	template<
		class _String,
		template<class item_t, class allocator_t = std::allocator<item_t>> class container_t
	>
		inline _String join(const container_t<_String>& items, view_t<_String> delimiter, bool skip_empty_items = false)
	{
		_String result;

		if (!items.empty())
		{
			result = items.front();

			const std::size_t items_size(items.size());

			if (items_size > 1)
			{
				using size_type = typename _String::size_type;

				const size_type delimiter_length(delimiter.length());

				auto it(std::begin(items));
				const auto it_end(std::end(items));

				result.reserve(
					std::accumulate(
						it,
						it_end,
						size_type((items_size - 1) * delimiter_length),
						[](size_type length, const _String& item)
				{
					return length + item.length();
				}));

				while (++it != it_end)
				{
					if (skip_empty_items && it->empty())
						continue;

					if (delimiter_length > 0)
						result += delimiter;

					result += *it;
				}

				if (skip_empty_items && (delimiter_length > 0) && items.front().empty())
					result.erase(0, delimiter_length);
			}
		}

		return result;
	}
	template<
		class _String,
		template<class item_t, class compare_t = std::less<item_t>, class allocator_t = std::allocator<item_t>> class container_t
	>
		inline _String join(const container_t<_String>& items, view_t<_String> delimiter, bool skip_empty_items = false)
	{
		std::vector<_String> tmp_items(items.size());
		std::copy(std::begin(items), std::end(items), std::begin(tmp_items));
		return string_utils::join(tmp_items, delimiter, skip_empty_items);
	}
	template<
		std::size_t _Index,
		class _String,
		template<class key_t, class item_t, class compare_t = std::less<key_t>, class allocator_t = std::allocator<std::pair<const key_t, item_t>>> class container_t
	>
		inline _String join(const container_t<_String, _String>& items, view_t<_String> delimiter, bool skip_empty_items = false)
	{
		std::vector<_String> tmp_items(items.size());
		std::transform(std::begin(items), std::end(items), std::begin(tmp_items), [](const std::pair<_String, _String>& item) { return std::get<_Index>(item); });
		return string_utils::join(tmp_items, delimiter, skip_empty_items);
	}
	template<class _String>
	inline _String join(const std::initializer_list<_String>& items, view_t<_String> delimiter, bool skip_empty_items = false)
	{
		return string_utils::join(std::vector<_String>{items}, delimiter, skip_empty_items);
	}


	template<
		class _String,
		template<class item_t, class allocator_t = std::allocator<item_t>> class container_t
	>
		_String join(const container_t<_String>& items, char_t<_String> const * delimiter = nullptr, bool skip_empty_items = false)
	{
		return string_utils::join(items, (delimiter ? view_t<_String>(delimiter) : view_t<_String>()), skip_empty_items);
	}
	template<
		class _String,
		template<class item_t, class compare_t = std::less<item_t>, class allocator_t = std::allocator<item_t>> class container_t
	>
		inline _String join(const container_t<_String>& items, char_t<_String> const * delimiter = nullptr, bool skip_empty_items = false)
	{
		return string_utils::join(items, (delimiter ? view_t<_String>(delimiter) : view_t<_String>()), skip_empty_items);
	}
	template<
		std::size_t _Index,
		class _String,
		template<class key_t, class item_t, class compare_t = std::less<key_t>, class allocator_t = std::allocator<std::pair<const key_t, item_t>>> class container_t
	>
		inline _String join(const container_t<_String, _String>& items, char_t<_String> const * delimiter = nullptr, bool skip_empty_items = false)
	{
		return string_utils::join<_Index>(items, (delimiter ? view_t<_String>(delimiter) : view_t<_String>()), skip_empty_items);
	}
	template<class _String>
	inline _String join(const std::initializer_list<_String>& items, char_t<_String> const * delimiter = nullptr, bool skip_empty_items = false)
	{
		return string_utils::join(items, (delimiter ? view_t<_String>(delimiter) : view_t<_String>()), skip_empty_items);
	}

	template<
		class _StringView,
		template<class item_t, class allocator_t = std::allocator<item_t>> class container_t
	>
		str_t<_StringView> join_view(const container_t<_StringView>& items, _StringView delimiter = {}, bool skip_empty_items = false)
	{
		str_t<_StringView> result;

		if (!items.empty())
		{
			result = items.front();

			const std::size_t items_size(items.size());

			if (items_size > 1)
			{
				using size_type = typename _StringView::size_type;

				const size_type delimiter_length(delimiter.length());

				auto it(std::begin(items));
				const auto it_end(std::end(items));

				result.reserve(
					std::accumulate(
						it,
						it_end,
						size_type((items_size - 1) * delimiter_length),
						[](size_type length, _StringView item)
				{
					return length + item.length();
				}));

				while (++it != it_end)
				{
					if (skip_empty_items && it->empty())
						continue;

					if (delimiter_length > 0)
						result += delimiter;

					result += *it;
				}

				if (skip_empty_items && delimiter_length > 0 && items.front().empty())
					result.erase(0, delimiter_length);
			}
		}

		return result;
	}
	template<
		class _StringView,
		template<class item_t, class compare_t = std::less<item_t>, class allocator_t = std::allocator<item_t>> class container_t
	>
		inline str_t<_StringView> join_view(const container_t<_StringView>& items, _StringView delimiter = {}, bool skip_empty_items = false)
	{
		std::vector<_StringView> tmp_items(items.size());
		std::copy(std::begin(items), std::end(items), std::begin(tmp_items));
		return string_utils::join_view<_StringView>(tmp_items, delimiter, skip_empty_items);
	}
	template<
		std::size_t _Index,
		class _StringView,
		template<class key_t, class item_t, class compare_t = std::less<key_t>, class allocator_t = std::allocator<std::pair<const key_t, item_t>>> class container_t
	>
		inline str_t<_StringView> join_view(const container_t<_StringView, _StringView>& items, _StringView delimiter = {}, bool skip_empty_items = false)
	{
		std::vector<_StringView> tmp_items(items.size());
		std::transform(std::begin(items), std::end(items), std::begin(tmp_items), [](const std::pair<_StringView, _StringView>& item) { return std::get<_Index>(item); });
		return string_utils::join_view(tmp_items, delimiter, skip_empty_items);
	}
	template<class _StringView>
	inline str_t<_StringView> join_view(const std::initializer_list<_StringView>& items, _StringView delimiter = {}, bool skip_empty_items = false)
	{
		return string_utils::join_view<_StringView>(std::vector<_StringView>{items}, delimiter, skip_empty_items);
	}

	namespace details
	{
		struct helper
		{
			template<template<class item_t, class allocator_t = std::allocator<item_t>> class container_t, class item_t, class... _Args>
			static inline void emplace_back(container_t<item_t>& container, _Args&&... _args)
			{
				container.emplace_back(std::forward<_Args>(_args)...);
			}
			template<template<class item_t, class compare_t = std::less<item_t>, class allocator_t = std::allocator<item_t>> class container_t, class item_t, class... _Args>
			static inline void emplace_back(container_t<item_t>& container, _Args&&... _args)
			{
				container.emplace(std::forward<_Args>(_args)...);
			}
		};
		template<class container_t, class _String>
		inline void split(container_t& container, _String&& string, view_t<_String> delimiter, bool skip_empty_items)
		{
			if (!string.empty())
			{
				using size_type = typename _String::size_type;

				const size_type delimiter_length(delimiter.length());
				if (delimiter_length == 0)
				{
					helper::emplace_back(container, std::forward<_String>(string));
				}
				else
				{
					size_type pos(0);
					size_type pos_end(0);

					for (; ((pos_end = string.find(delimiter, pos)) != _String::npos); pos = (pos_end + delimiter_length))
					{
						if (pos_end == pos)
						{
							if (!skip_empty_items)
								helper::emplace_back(container);
							continue;
						}

						helper::emplace_back(container, string.substr(pos, pos_end - pos));
					}

					helper::emplace_back(container, string.substr(pos));
				}
			}
		}
		template<class container_t, class _StringView>
		inline void split_view(container_t& container, _StringView string_view, _StringView delimiter, bool skip_empty_items)
		{
			if (!string_view.empty())
			{
				using size_type = typename _StringView::size_type;

				const size_type delimiter_length(delimiter.length());
				if (delimiter_length == 0)
				{
					helper::emplace_back(container, string_view);
				}
				else
				{
					size_type pos(0);
					size_type pos_end(0);

					for (; ((pos_end = string_view.find(delimiter, pos)) != _StringView::npos); pos = (pos_end + delimiter_length))
					{
						if (pos_end == pos)
						{
							if (!skip_empty_items)
								helper::emplace_back(container);
							continue;
						}

						helper::emplace_back(container, string_view.substr(pos, pos_end - pos));
					}

					helper::emplace_back(container, string_view.substr(pos));
				}
			}
		}
	}
	template<template<class item_t, class allocator_t = std::allocator<item_t>> class container_t, class _String>
	inline container_t<_String> split(_String string, view_t<_String> delimiter, bool skip_empty_items = false)
	{
		container_t<_String> result;
		details::split<container_t<_String>, _String>(result, std::move(string), delimiter, skip_empty_items);
		return result;
	}
	template<template<class item_t, class compare_t = std::less<item_t>, class allocator_t = std::allocator<item_t>> class container_t, class _String>
	inline container_t<_String> split(_String string, view_t<_String> delimiter, bool skip_empty_items = false)
	{
		container_t<_String> result;
		details::split<container_t<_String>, _String>(result, std::move(string), delimiter, skip_empty_items);
		return result;
	}

	template<template<class item_t, class allocator_t = std::allocator<item_t>> class container_t, class _StringView>
	inline container_t<_StringView> split_view(_StringView string_view, _StringView delimiter, bool skip_empty_items = false)
	{
		container_t<_StringView> result;
		details::split_view<container_t<_StringView>, _StringView>(result, string_view, delimiter, skip_empty_items);
		return result;
	}
	template<template<class item_t, class compare_t = std::less<item_t>, class allocator_t = std::allocator<item_t>> class container_t, class _StringView>
	inline container_t<_StringView> split_view(_StringView string_view, _StringView delimiter, bool skip_empty_items = false)
	{
		container_t<_StringView> result;
		details::split_view<container_t<_StringView>, _StringView>(result, string_view, delimiter, skip_empty_items);
		return result;
	}

	class StringConversionException
		: public std::runtime_error
	{
	public:
		// Creates exception with error message and error code.
		StringConversionException(const char* message, unsigned long error)
			: std::runtime_error(message)
			, m_error(error)
		{}

		// Creates exception with error message and error code.
		StringConversionException(const std::string& message, unsigned long error)
			: std::runtime_error(message)
			, m_error(error)
		{}

		// Windows error code.
		unsigned long Error() const
		{
			return m_error;
		}

	private:
		unsigned long m_error;
	};


	std::wstring ConvertToUTF16(const std::string & source, const unsigned int codePage);



	inline namespace static_analises
	{
		// [warning: C26449] gsl::span or std::string_view created from a temporary will be invalid when the temporary is invalidated (gsl.view)
		// Using: LOG_XXXX() << (expression ? as_view(to_wstring(10)) : L"<constexpr-text>"sv);
		inline std::string_view as_view(const std::string& str) noexcept { return std::string_view(str); }
		inline std::wstring_view as_view(const std::wstring& str) noexcept { return std::wstring_view(str); }

	} // namespace static_analises


} // namespace string_utils