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

	std::wstring widen(const std::string& narrow, const std::locale& locale = std::locale(""))
	{
		if (narrow.empty())
			return std::wstring();

		typedef std::string::traits_type::state_type state_type;
		typedef std::codecvt<wchar_t, char, state_type> CVT;

		const CVT& cvt = std::use_facet<CVT>(locale);
		std::wstring wide(narrow.size(), '\0');
		state_type state = state_type();
		const char* from_beg = &narrow[0];
		const char* from_end = from_beg + narrow.size();
		const char* from_nxt;
		wchar_t* to_beg = &wide[0];
		wchar_t* to_end = to_beg + wide.size();
		wchar_t* to_nxt;

		std::wstring::size_type sz = 0;
		std::codecvt_base::result r;
		do
		{
			r = cvt.in(state, from_beg, from_end, from_nxt,
				to_beg, to_end, to_nxt);
			switch (r)
			{
			case std::codecvt_base::error:
				return wide;

			case std::codecvt_base::partial:
				sz += to_nxt - to_beg;
				wide.resize(2 * wide.size());
				to_beg = &wide[sz];
				to_end = &wide[0] + wide.size();
				break;

			case std::codecvt_base::noconv:
				wide.resize(sz + (from_end - from_beg));
				std::memcpy(&wide[sz], from_beg, (std::size_t)(from_end - from_beg));
				r = std::codecvt_base::ok;
				break;

			case std::codecvt_base::ok:
				sz += to_nxt - to_beg;
				wide.resize(sz);
				break;
			}
		} while (r != std::codecvt_base::ok);

		return wide;
	}

}