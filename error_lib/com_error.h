/////////////////////////////////////
// (C) 2014 ��� "��� ������������"
// ������: SNES-AV eset
// �����: ������ �����
// ������: 18.03.2015
// ������� ��������: ��������� ������ � �������� COM
/////////////////////////////////////
#pragma once

#include <system_error>

namespace win32
{

	enum class com_errc;

	const std::error_category& get_com_error_category();

	inline std::error_code make_error_code(com_errc e)
	{
		return{ static_cast<int>(e), get_com_error_category() };
	}

}

namespace std
{
	template<> struct is_error_code_enum < win32::com_errc >: public true_type{};
}
