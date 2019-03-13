#pragma once

#include <string>
#include <vector>

#include <error_lib/application_exception.h>

namespace win32
{
	enum class win32_errc;

	const std::error_category& get_win32_error_category() noexcept;

	inline std::error_code make_error_code(win32_errc e) noexcept
	{
		return std::error_code(static_cast<int>(e), get_win32_error_category());
	}

	std::error_code get_last_error_code();

	std::string make_what(std::wstring&& func, std::wstring&& wwhat, std::wstring_view arg_view);

	namespace exception
	{
		::common::application_exception by_last_error();
		::common::application_exception by_last_error(std::string  func, std::wstring_view arg_view = {});
		::common::application_exception by_last_error(std::wstring func, std::wstring_view arg_view = {});

		::common::application_exception by_error(int raw_code_vlue);
		::common::application_exception by_error(int raw_code_vlue, std::string  func, std::wstring_view arg_view = {});
		::common::application_exception by_error(int raw_code_vlue, std::wstring func, std::wstring_view arg_view = {});

		::common::application_exception by_error(std::error_code code);
		::common::application_exception by_error(std::error_code code, std::string  func, std::wstring_view arg_view = {});
		::common::application_exception by_error(std::error_code code, std::wstring func, std::wstring_view arg_view = {});
	};

} // namespace win32


namespace common
{
	template<> struct is_app_error_code_enum < win32::win32_errc > : std::true_type {};
}


namespace std
{
	template<> struct is_error_code_enum < win32::win32_errc > : std::true_type {};
}
