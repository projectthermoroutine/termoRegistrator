#include <codecvt>
#include <cwctype>
#include <algorithm>
#include <locale>

#include <error_lib/error_codes.h>
#include <error_lib/application_exception.h>

#include <common/locale.hpp>
#include <common/string_utils.h>
#include <common/log_and_throw.h>
#include <Windows.h>

namespace string_utils
{
	namespace
	{
		template <typename T>
		T trim_helper(const T& str)
		{
			const auto is_space = [](int ch) { return std::isspace(ch, std::locale::classic()); };
			const auto start = std::find_if_not(str.begin(), str.end(), is_space);
			if (start == str.end())
				return T();
			const auto end = std::find_if_not(str.crbegin(), T::const_reverse_iterator(start), is_space);
			return T(&*start, (&*end + 1) - (&*start));
		}

		template <typename T>
		T trim_begin_helper(const T& str)
		{
			const auto is_space = [](int ch) { return std::isspace(ch, std::locale::classic()); };
			const auto start = std::find_if_not(str.begin(), str.end(), is_space);
			if (start == str.end())
				return T();
			return T(&*start);
		}
	}

	bool ends_with_case_insensitive(std::wstring_view src, std::wstring_view expected_end)
	{
		if (src.size() < expected_end.size())
			return false;

		src.remove_prefix(src.size() - expected_end.size());

		return compare_case_insensitive(src.data(), expected_end.size(), expected_end.data(), expected_end.size());
	}

	bool starts_with_case_insensitive(std::wstring_view src, std::wstring_view expected_start)
	{
		if (src.size() < expected_start.size())
			return false;

		src.remove_suffix(src.size() - expected_start.size());

		return compare_case_insensitive(src.data(), expected_start.size(), expected_start.data(), expected_start.size());
	}

	bool compare_case_insensitive(std::wstring_view string1, std::wstring_view string2)
	{
		return compare_case_insensitive(string1.data(), string1.length(), string2.data(), string2.length());
	}

	bool compare_case_insensitive(const wchar_t* str1_ptr, std::size_t str1_size, const wchar_t* str2_ptr, std::size_t str2_size)
	{
		if (str1_size != str2_size)
			return false;

		while (str1_size > 0 && (*str1_ptr == *str2_ptr || std::towlower(*str1_ptr) == std::towlower(*str2_ptr)))
		{
			str1_size -= 1;
			str1_ptr += 1;
			str2_ptr += 1;
		}

		return (0 == str1_size);
	}

	std::wstring convert_utf8_to_wchar(std::string_view str_view)
	{
		return common::wstring_convert<wchar_t>().from_bytes(str_view);
	}

	std::string convert_wchar_to_utf8(std::wstring_view str_view)
	{
		return common::wstring_convert<wchar_t>().to_bytes(str_view);
	}


	std::string trim(const std::string &s)
	{
		return trim_helper(s);
	}

	std::wstring trim(const std::wstring &s)
	{
		return trim_helper(s);
	}

	std::string trim_begin(const std::string &s)
	{
		return trim_begin_helper(s);
	}

	std::wstring trim_begin(const std::wstring &s)
	{
		return trim_begin_helper(s);
	}


	//-----------------------------------------------------------------------------
	// Converts an ANSI/MBCS string to Unicode UTF-16.
	// Wraps MultiByteToWideChar() using modern C++ and STL.
	// Throws a StringConversionException on error.
	//-----------------------------------------------------------------------------
	std::wstring ConvertToUTF16(const std::string & source, const unsigned int codePage)
	{
		// Fail if an invalid input character is encountered
		static const DWORD conversionFlags = MB_ERR_INVALID_CHARS;

		// Require size for destination string
		const int utf16Length = ::MultiByteToWideChar(
			codePage,           // code page for the conversion
			conversionFlags,    // flags
			source.c_str(),     // source string
			(int)source.length(),    // length (in chars) of source string
			NULL,               // unused - no conversion done in this step
			0                   // request size of destination buffer, in wchar_t's
		);
		if (utf16Length == 0)
		{
			const DWORD error = ::GetLastError();
			throw StringConversionException(
				"MultiByteToWideChar() failed: Can't get length of destination UTF-16 string.",
				error);
		}

		// Allocate room for destination string
		std::wstring utf16Text;
		utf16Text.resize(utf16Length);

		// Convert to Unicode UTF-16
		if (!::MultiByteToWideChar(
			codePage,           // code page for conversion
			0,                  // validation was done in previous call
			source.c_str(),     // source string
			(int)source.length(),    // length (in chars) of source string
			&utf16Text[0],      // destination buffer
			(int)utf16Text.length()  // size of destination buffer, in wchar_t's
		))
		{
			const DWORD error = ::GetLastError();
			throw StringConversionException(
				"MultiByteToWideChar() failed: Can't convert to UTF-16 string.",
				error);
		}

		return utf16Text;
	}
}//namespace string_utils