/////////////////////////////////////
// (C) 2014 ��� "��� ������������"
// ������: SNES-AV eset
// �����: ������ �����
// ������: 16.01.2015
// ������� ��������: ��������� ������� ������ �� ��������
/////////////////////////////////////
#pragma once

#include <string>
#include <vector>
#include <iterator>
#include <sstream>

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
}