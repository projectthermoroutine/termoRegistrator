

#pragma once


#include <locale>
#include <string>

#if _HAS_CXX17
#	include <string_view>
#endif // _HAS_CXX17


namespace common { namespace platform_details
{
	struct wstring_convert
	{
		static std::size_t to_bytes(const wchar_t* wstr, std::size_t wlength, char*    bstr = nullptr, std::size_t blength = 0);
		static std::size_t from_bytes(const char*  bstr, std::size_t blength, wchar_t* wstr = nullptr, std::size_t wlength = 0);
	};
	struct u16string_convert
	{
		static std::size_t to_bytes(const char16_t* wstr, std::size_t wlength, char*     bstr = nullptr, std::size_t blength = 0);
		static std::size_t from_bytes(const char*   bstr, std::size_t blength, char16_t* wstr = nullptr, std::size_t wlength = 0);
	};
	struct u32string_convert
	{
		static std::size_t to_bytes(const char32_t* wstr, std::size_t wlength, char*     bstr = nullptr, std::size_t blength = 0);
		static std::size_t from_bytes(const char*   bstr, std::size_t blength, char32_t* wstr = nullptr, std::size_t wlength = 0);
	};

}} // namespace common::platform_details


namespace common
{
	template <
		class _Wide = wchar_t,
		class _WAlloc = std::allocator<_Wide>,
		class _BAlloc = std::allocator<char>
	>
	class wstring_convert
	{
	public:

		using byte_traits = std::char_traits<char>;
		using wide_traits = std::char_traits<_Wide>;

		using byte_string = std::basic_string<char, byte_traits, _BAlloc>;
		using wide_string = std::basic_string<_Wide, wide_traits, _WAlloc>;

		using byte_size_t = typename byte_string::size_type;
		using wide_size_t = typename wide_string::size_type;

	public:

		wide_string from_bytes(char bchar)
		{
			return from_bytes(&bchar, 1);
		}

		wide_string from_bytes(const char* bptr)
		{
			const byte_size_t size{ bptr == nullptr ? 0 : byte_traits::length(bptr) };

			return (size == 0 ? wide_string() : from_bytes(bptr, size));
		}

		wide_string from_bytes(const byte_string& bstr)
		{
			return (bstr.empty() ? wide_string() : from_bytes(bstr.c_str(), bstr.size()));
		}
		
		wide_string from_bytes(const typename byte_string::value_type* bptr, byte_size_t blength)
		{
			wide_string result{};

			bool is_ok{ false };

			if constexpr (std::is_same_v<_Wide, wchar_t>)
			{
				result.resize(platform_details::wstring_convert::from_bytes(bptr, blength));
				is_ok = (platform_details::wstring_convert::from_bytes(bptr, blength, result.data(), result.size()) == result.size());
			}
			else
				if constexpr (std::is_same_v<_Wide, char16_t>)
				{
					result.resize(platform_details::u16string_convert::from_bytes(bptr, blength));
					is_ok = (platform_details::u16string_convert::from_bytes(bptr, blength, result.data(), result.size()) == result.size());
				}
				else
					if constexpr (std::is_same_v<_Wide, char32_t>)
					{
						result.resize(platform_details::u32string_convert::from_bytes(bptr, blength));
						is_ok = (platform_details::u32string_convert::from_bytes(bptr, blength, result.data(), result.size()) == result.size());
					}
					else
					{
						static_assert(false);
					}

			if (!is_ok)
				throw std::runtime_error("platform_details::xstring_convert::from_bytes is failed");

			return result;
		}

	public:

		byte_string to_bytes(_Wide wchar)
		{
			return to_bytes(&wchar, 1);
		}

		byte_string to_bytes(const _Wide* wptr)
		{
			const wide_size_t size{ wptr == nullptr ? 0 : wide_traits::length(wptr) };

			return (0 == size ? byte_string() : to_bytes(wptr, size));
		}

		byte_string to_bytes(const wide_string& wstr)
		{
			return (wstr.empty() ? byte_string() : to_bytes(wstr.c_str(), wstr.size()));
		}

		byte_string to_bytes(const typename wide_string::value_type* wptr, wide_size_t wlength)
		{
			byte_string result{};

			bool is_ok{ false };

			if constexpr (std::is_same_v<_Wide, wchar_t>)
			{
				result.resize(platform_details::wstring_convert::to_bytes(wptr, wlength));
				is_ok = (platform_details::wstring_convert::to_bytes(wptr, wlength, result.data(), result.size()) == result.size());
			}
			else
			if constexpr (std::is_same_v<_Wide, char16_t>)
			{
				result.resize(platform_details::u16string_convert::to_bytes(wptr, wlength));
				is_ok = (platform_details::u16string_convert::to_bytes(wptr, wlength, result.data(), result.size()) == result.size());
			}
			else
			if constexpr (std::is_same_v<_Wide, char32_t>)
			{
				result.resize(platform_details::u32string_convert::to_bytes(wptr, wlength));
				is_ok = (platform_details::u32string_convert::to_bytes(wptr, wlength, result.data(), result.size()) == result.size());
			}
			else
			{
				static_assert(false);
			}

			if (!is_ok)
				throw std::runtime_error("platform_details::xstring_convert::to_bytes is failed");

			return result;
		}
	
#if _HAS_CXX17
	public:

		using byte_string_view = std::basic_string_view<char, std::char_traits<char>>;
		using wide_string_view = std::basic_string_view<_Wide, std::char_traits<_Wide>>;

		wide_string from_bytes(byte_string_view bstr_view)
		{
			return (bstr_view.empty() ? wide_string() : from_bytes(bstr_view.data(), bstr_view.size()));
		}

		byte_string to_bytes(wide_string_view wstr_view)
		{
			return (wstr_view.empty() ? byte_string() : to_bytes(wstr_view.data(), wstr_view.size()));
		}

#endif // _HAS_CXX17
};


	//template <>
	//class wstring_convert<wchar_t>
	//{
	//public:
	//
	//	using _Elem = wchar_t;
	//	using _WAlloc = std::allocator<_Elem>;
	//	using _BAlloc = std::allocator<char>;
	//
	//	using byte_traits = std::char_traits<char>;
	//	using wide_traits = std::char_traits<_Elem>;
	//
	//	using byte_string = std::basic_string<char, byte_traits, _BAlloc>;
	//	using wide_string = std::basic_string<_Elem, wide_traits, _WAlloc>;
	//
	//	using byte_size_t = typename byte_string::size_type;
	//	using wide_size_t = typename wide_string::size_type;
	//
	//	using byte_string_view = std::basic_string_view<char, std::char_traits<char>>;
	//	using wide_string_view = std::basic_string_view<_Elem, std::char_traits<_Elem>>;
	//
	//public:
	//
	//	wide_string from_bytes(char bchar)
	//	{
	//		return from_bytes_impl(&bchar, 1);
	//	}
	//
	//	wide_string from_bytes(const char* bptr)
	//	{
	//		const byte_size_t size{ bptr == nullptr ? 0 : byte_traits::length(bptr) };
	//
	//		return (size == 0 ? wide_string() : from_bytes_impl(bptr, size));
	//	}
	//
	//	wide_string from_bytes(const byte_string& bstr)
	//	{
	//		return (bstr.empty() ? wide_string() : from_bytes_impl(bstr.c_str(), bstr.size()));
	//	}
	//
	//	wide_string from_bytes(byte_string_view bstr_view)
	//	{
	//		return (bstr_view.empty() ? wide_string() : from_bytes_impl(bstr_view.data(), bstr_view.size()));
	//	}
	//
	//public:
	//
	//	byte_string to_bytes(_Elem wchar)
	//	{
	//		return to_bytes_impl(&wchar, 1);
	//	}
	//
	//	byte_string to_bytes(const _Elem* wptr)
	//	{
	//		const wide_size_t size{ wptr == nullptr ? 0 : wide_traits::length(wptr) };
	//
	//		return (0 == size ? byte_string() : to_bytes_impl(wptr, size));
	//	}
	//
	//	byte_string to_bytes(const wide_string& wstr)
	//	{
	//		return (wstr.empty() ? byte_string() : to_bytes_impl(wstr.c_str(), wstr.size()));
	//	}
	//
	//	byte_string to_bytes(wide_string_view wstr_view)
	//	{
	//		return (wstr_view.empty() ? byte_string() : to_bytes_impl(wstr_view.data(), wstr_view.size()));
	//	}
	//
	//private:
	//
	//	wide_string from_bytes_impl(const typename byte_string::value_type* bptr, byte_size_t bsize)
	//	{
	//		const wide_size_t wsize{ static_cast<wide_size_t>(::MultiByteToWideChar(CP_UTF8, 0, bptr, static_cast<int>(bsize), nullptr, 0)) };
	//
	//		wide_string result(wsize, L'\0');
	//		if (wsize != static_cast<wide_size_t>(::MultiByteToWideChar(CP_UTF8, 0, bptr, static_cast<int>(bsize), result.data(), static_cast<int>(result.size()))))
	//			throw std::runtime_error("MultiByteToWideChar is failed");
	//
	//		return result;
	//	}
	//
	//	byte_string to_bytes_impl(const typename wide_string::value_type* wptr, wide_size_t wsize)
	//	{
	//		const byte_size_t bsize{ static_cast<byte_size_t>(::WideCharToMultiByte(CP_UTF8, 0, wptr, static_cast<int>(wsize), nullptr, 0, nullptr, nullptr)) };
	//
	//		byte_string result(bsize, '\0');
	//		if (bsize != static_cast<byte_size_t>(::WideCharToMultiByte(CP_UTF8, 0, wptr, static_cast<int>(wsize), result.data(), static_cast<int>(result.size()), nullptr, nullptr)))
	//			throw std::runtime_error("WideCharToMultiByte is failed");
	//
	//		return result;
	//	}
	//};

} // namespace common
