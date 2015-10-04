/////////////////////////////////////
// (C) 2014 ООО "Код Безопасности"
// Проект: SNES-AV eset
// Автор: Зайцев Роман
// Создан: 16.01.2015
// Краткое описание: интерфейс функций работы со строками
/////////////////////////////////////
#pragma once

#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include <exception>

namespace string_utils
{
    std::wstring convert_utf8_to_wchar(const std::string& str);
    std::string convert_wchar_to_utf8(const std::wstring& str);

	std::string trim(const std::string &s);
	std::wstring trim(const std::wstring &s);

	template <typename T>
	std::string to_hex(T value)
	{
		std::stringstream ss;
		ss << std::hex << std::showbase << value;
		return ss.str();
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

}