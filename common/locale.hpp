

#pragma once


#include <string>
#include <string_view>


namespace common::platform_details
{

	struct wstring_convert
	{
		static std::size_t to_bytes(const wchar_t* wstr, std::size_t wlength, char*    bstr = nullptr, std::size_t blength = 0) noexcept;
		static std::size_t from_bytes(const char*  bstr, std::size_t blength, wchar_t* wstr = nullptr, std::size_t wlength = 0) noexcept;
	};
	struct u16string_convert
	{
		static std::size_t to_bytes(const char16_t* wstr, std::size_t wlength, char*     bstr = nullptr, std::size_t blength = 0) noexcept;
		static std::size_t from_bytes(const char*   bstr, std::size_t blength, char16_t* wstr = nullptr, std::size_t wlength = 0) noexcept;
	};
	struct u32string_convert
	{
		static std::size_t to_bytes(const char32_t* wstr, std::size_t wlength, char*     bstr = nullptr, std::size_t blength = 0) noexcept;
		static std::size_t from_bytes(const char*   bstr, std::size_t blength, char32_t* wstr = nullptr, std::size_t wlength = 0) noexcept;
	};


} // namespace common::platform_details


namespace common::details
{

	template<class _Wide>
	inline std::size_t wconvert_to_bytes(const _Wide* wstr, std::size_t wlength, char* bstr, std::size_t blength) noexcept
	{
		if constexpr (std::is_same_v<_Wide, wchar_t>)
			return platform_details::wstring_convert::to_bytes(wstr, wlength, bstr, blength);
		else
			if constexpr (std::is_same_v<_Wide, char16_t>)
				return platform_details::u16string_convert::to_bytes(wstr, wlength, bstr, blength);
			else
				if constexpr (std::is_same_v<_Wide, char32_t>)
					return platform_details::u32string_convert::to_bytes(wstr, wlength, bstr, blength);
				else
					static_assert(false);
	}

	template<class _Wide>
	inline std::size_t wconvert_from_bytes(const char* bstr, std::size_t blength, _Wide* wstr, std::size_t wlength) noexcept
	{
		if constexpr (std::is_same_v<_Wide, wchar_t>)
			return platform_details::wstring_convert::from_bytes(bstr, blength, wstr, wlength);
		else
			if constexpr (std::is_same_v<_Wide, char16_t>)
				return platform_details::u16string_convert::from_bytes(bstr, blength, wstr, wlength);
			else
				if constexpr (std::is_same_v<_Wide, char32_t>)
					return platform_details::u32string_convert::from_bytes(bstr, blength, wstr, wlength);
				else
					static_assert(false);
	}


} // namespace details


namespace common
{

	template <
		class _Wide = wchar_t,
		bool  _NoExcept = false
	>
		class wstring_convert
	{
	public:

		using byte_traits = std::char_traits<char>;
		using wide_traits = std::char_traits<_Wide>;

		using byte_alloc = std::allocator<char>;
		using wide_alloc = std::allocator<_Wide>;

		using byte_string = std::basic_string<char, byte_traits, byte_alloc>;
		using wide_string = std::basic_string<_Wide, wide_traits, wide_alloc>;

	public:

		wide_string from_bytes(char bchar) const noexcept(_NoExcept)
		{
			return from_bytes(&bchar, 1);
		}

		wide_string from_bytes(const char* bstr) const noexcept(_NoExcept)
		{
			const std::size_t size{ bstr == nullptr ? 0 : byte_traits::length(bstr) };

			return (size == 0 ? wide_string() : from_bytes(bstr, size));
		}

		wide_string from_bytes(const byte_string& bstr) const noexcept(_NoExcept)
		{
			return (bstr.empty() ? wide_string() : from_bytes(bstr.data(), bstr.size()));
		}

		wide_string from_bytes(const char* bstr, std::size_t blength) const noexcept(_NoExcept)
		{
			const std::size_t wlength{ details::wconvert_from_bytes<_Wide>(bstr, blength, nullptr, 0) };
			if (wlength == 0)
				return wide_string();

			wide_string result;

			try
			{
				result.resize(wlength);
			}
			catch (...)
			{
				if constexpr (_NoExcept)
					return wide_string();
				else
					throw;
			}

			if (details::wconvert_from_bytes<_Wide>(bstr, blength, result.data(), result.size()) == wlength)
				return result;

			if constexpr (_NoExcept)
				return wide_string();
			else
				throw std::runtime_error("common::details::wconvert_from_bytes is failed");
		}

	public:

		byte_string to_bytes(_Wide wchar) const noexcept(_NoExcept)
		{
			return to_bytes(&wchar, 1);
		}

		byte_string to_bytes(const _Wide* wstr) const noexcept(_NoExcept)
		{
			const std::size_t size{ wstr == nullptr ? 0 : wide_traits::length(wstr) };

			return (0 == size ? byte_string() : to_bytes(wstr, size));
		}

		byte_string to_bytes(const wide_string& wstr) const noexcept(_NoExcept)
		{
			return (wstr.empty() ? byte_string() : to_bytes(wstr.data(), wstr.size()));
		}

		byte_string to_bytes(const _Wide* wstr, std::size_t wlength) const noexcept(_NoExcept)
		{
			const std::size_t blength{ details::wconvert_to_bytes(wstr, wlength, nullptr, 0) };
			if (blength == 0)
				return byte_string();

			byte_string result;

			try
			{
				result.resize(blength);
			}
			catch (...)
			{
				if constexpr (_NoExcept)
					return byte_string();
				else
					throw;
			}

			if (details::wconvert_to_bytes(wstr, wlength, result.data(), result.size()) == blength)
				return result;

			if constexpr (_NoExcept)
				return byte_string();
			else
				throw std::runtime_error("common::details::wconvert_to_bytes is failed");
		}

	public:

		using byte_string_view = std::basic_string_view<char, byte_traits>;
		using wide_string_view = std::basic_string_view<_Wide, wide_traits>;

		wide_string from_bytes(byte_string_view bstr_view) const noexcept(_NoExcept)
		{
			return (bstr_view.empty() ? wide_string() : from_bytes(bstr_view.data(), bstr_view.size()));
		}

		byte_string to_bytes(wide_string_view wstr_view) const noexcept(_NoExcept)
		{
			return (wstr_view.empty() ? byte_string() : to_bytes(wstr_view.data(), wstr_view.size()));
		}
	};


	template <class _Wide = wchar_t>
	using wstring_convert_noexcept = wstring_convert<_Wide, true>;


} // namespace common
