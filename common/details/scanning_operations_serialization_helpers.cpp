/////////////////////////////////////
// (C) 2014 ООО "Код Безопасности"
// Проект: SNES-AV eset
// Автор: Зайцев Роман
// Создан: 29.09.2014
// Краткое описание: сериализация в строку и обратно enum-ов из scanning_operations
/////////////////////////////////////

#include <common\scanning_operations_serialization_helpers.h>
#include <algorithm>

namespace details
{
	static const std::pair<scanning_operations::clean_mode, const wchar_t*> clean_mode_to_text_table[] =
	{
		{ scanning_operations::clean_mode::none,		L"none" },
		{ scanning_operations::clean_mode::standard,	L"standard" },
		{ scanning_operations::clean_mode::strict,		L"strict" },
		{ scanning_operations::clean_mode::rigorous,	L"rigorous" },
		{ scanning_operations::clean_mode::remove,		L"remove" }
	};
}

std::wistream & operator >> (std::wistream & stream, scanning_operations::clean_mode & val)
{
	std::wstring str;
	stream >> str;

	const auto find_result = std::find_if(std::cbegin(details::clean_mode_to_text_table), std::cend(details::clean_mode_to_text_table), [&](const std::pair<scanning_operations::clean_mode, const wchar_t*>& item)
	{
		return item.second == str;
	});

	if (find_result == std::cend(details::clean_mode_to_text_table))
		throw std::invalid_argument("Invalid value of clean_mode.");
	else
		val = find_result->first;

	return stream;
}

std::wostream & operator << (std::wostream & stream, const scanning_operations::clean_mode & val)
{
	const auto find_result = std::find_if(std::cbegin(details::clean_mode_to_text_table), std::cend(details::clean_mode_to_text_table), [&](const std::pair<scanning_operations::clean_mode, const wchar_t*>& item)
	{
		return item.first == val;
	});

	if (find_result == std::cend(details::clean_mode_to_text_table))
		throw std::invalid_argument("Invalid value of clean_mode.");

	stream << find_result->second;

	return stream;
}