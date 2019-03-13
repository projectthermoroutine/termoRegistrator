#include <algorithm>

#include <windows.h>

#include <common/string_utils.h>
#include <common/std_string_view_extended_operators.hpp>

#include <position_detector_common/position_detector_connector.h>
#include <error_lib/application_exception.h>

#include <WinSock2.h>

namespace position_detector::connector
{
	using namespace std::literals;

	std::error_code get_last_error_code()
	{
		return make_error_code(::GetLastError());
	}

	std::error_code get_wsa_last_error_code()
	{
		return make_error_code(::WSAGetLastError());
	}

} // namespace position_detector::connector


namespace position_detector::connector::exception
{
	::common::application_exception by_last_error()
	{
		return by_error(get_last_error_code(), L""s, L""sv);
	}

	::common::application_exception by_last_error(std::string func, std::wstring_view arg_view)
	{
		return by_error(get_last_error_code(), string_utils::convert_utf8_to_wchar(func), arg_view);
	}

	::common::application_exception by_last_error(std::wstring func, std::wstring_view arg_view)
	{
		return by_error(get_last_error_code(), std::move(func), arg_view);
	}

	::common::application_exception by_wsa_last_error()
	{
		return by_error(get_last_error_code(), L""s, L""sv);
	}

	::common::application_exception by_wsa_last_error(std::string func, std::wstring_view arg_view)
	{
		return by_error(get_last_error_code(), string_utils::convert_utf8_to_wchar(func), arg_view);
	}

	::common::application_exception by_wsa_last_error(std::wstring func, std::wstring_view arg_view)
	{
		return by_error(get_last_error_code(), std::move(func), arg_view);
	}

} // namespace win32::exception
