/////////////////////////////////////
// (C) 2014 ООО "Код Безопасности"
// Проект: SNES-AV eset
// Автор: Зайцев Роман
// Создан: 16.01.2015
// Краткое описание: реализация функций работы со строками
/////////////////////////////////////

#include <common/string_utils.h>
#include <common/log_and_throw.h>
#include <codecvt>
#include <algorithm>
#include <locale>

namespace string_utils
{

	namespace
	{

		template <typename T>
		T trim_helper(const T& str)
		{
			const auto is_space = [](int ch) { return std::isspace(ch, std::locale::classic()); };
			const auto start = std::find_if_not(str.begin(), str.end(), is_space);
			if(start == str.end())
				return T();
			const auto end = std::find_if_not(str.crbegin(), T::const_reverse_iterator(start), is_space);
			return T(&*start, (&*end + 1) - (&*start));
		}

	}

    std::wstring convert_utf8_to_wchar(const std::string& str)
    {
        return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(str);
    }

    std::string convert_wchar_to_utf8(const std::wstring& str)
    {
        return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(str);
    }

	
	std::string trim(const std::string &s)
	{
		return trim_helper(s);
	}

	std::wstring trim(const std::wstring &s)
	{
		return trim_helper(s);
	}

}