#pragma once

#include <system_error>

#include <error_lib/application_exception.h>

namespace win32
{

	enum class com_errc;

	const std::error_category& get_com_error_category() noexcept;

	inline std::error_code make_error_code(com_errc e) noexcept
	{
		return{ static_cast<int>(e), get_com_error_category() };
	}
}


namespace common
{
	template<> struct is_app_error_code_enum < win32::com_errc > : std::true_type {};
}


namespace std
{
	template<> struct is_error_code_enum < win32::com_errc > : std::true_type {};
}
