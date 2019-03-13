#include <algorithm>

#include <windows.h>

#include <common/string_utils.h>
#include <common/std_string_view_extended_operators.hpp>

#include <position_detector_connector.h>
#include <error_lib/win32_error.h>
#include <error_lib/win32_error_codes.h>

namespace position_detector::connector
{
	using namespace std::literals;

	class connector_error_category final : public std::error_category
	{
	public:

		connector_error_category() noexcept : std::error_category() {}

		virtual const char* name() const noexcept override
		{
			return "position detector connector";
		}

		virtual std::string message(int code_value) const override
		{
			std::string static_message(::win32::to_static_message(static_cast<::win32::win32_errc>(code_value)));
			return !static_message.empty() ? std::move(static_message) : ("Internal position detector connector-error: "sv + std::to_string(code_value));
		}
	};

	const std::error_category& get_connector_error_category() noexcept
	{
		static const connector_error_category category;
		return category;
	}

} // namespace position_detector::connector


namespace position_detector::connector::exception
{
	::common::application_exception by_error(int raw_code_vlue)
	{
		return by_error(make_error_code(raw_code_vlue), L""s, L""sv);
	}

	::common::application_exception by_error(int raw_code_vlue, std::string func, std::wstring_view arg_view)
	{
		return by_error(make_error_code(raw_code_vlue), string_utils::convert_utf8_to_wchar(func), arg_view);
	}

	::common::application_exception by_error(int raw_code_vlue, std::wstring func, std::wstring_view arg_view)
	{
		return by_error(make_error_code(raw_code_vlue), std::move(func), arg_view);
	}

	::common::application_exception by_error(std::error_code code)
	{
		return by_error(code, L""s, L""sv);
	}

	::common::application_exception by_error(std::error_code code, std::string func, std::wstring_view arg_view)
	{
		return by_error(code, string_utils::convert_utf8_to_wchar(func), arg_view);
	}

	::common::application_exception by_error(std::error_code code, std::wstring func, std::wstring_view arg_view)
	{
		return ::common::application_exception::make_with_what_force(code, ::win32::make_what(std::move(func), to_wstring(static_cast<::win32::win32_errc>(code.value())), arg_view));
	}

} // namespace win32::exception
