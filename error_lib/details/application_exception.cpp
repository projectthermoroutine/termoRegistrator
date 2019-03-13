#include <windows.h>
#include <assert.h>

#include <error_lib\application_exception.h>
#include <error_lib\error_codes.h>

#include <common\string_utils.h>
#include <common\std_string_view_extended_operators.hpp>

//#include <message_localization_lib\message.h>



namespace common
{
	using namespace std::literals;

	namespace
	{
		struct category_t : std::error_category
		{
			category_t() noexcept : std::error_category() {}

			virtual const char* name() const noexcept override
			{
				return "application";
			}

			virtual std::string message(int code_value) const override
			{
				std::string static_message(to_static_message(static_cast<result_code>(code_value)));
				return !static_message.empty() ? std::move(static_message) : ("Internal error code: "sv + std::to_string(code_value));
			}
		};

	} // namespace anonymous


	const std::error_category& application_error_category() noexcept
	{
		static const category_t category;
		return category;
	}

	application_exception application_exception::make_with_what_force(const std::error_code& code, std::string what_force)
	{
		return application_exception(code, std::move(what_force));
	}

	application_exception::application_exception(std::error_code code, std::string what_force)
		: std::runtime_error(what_force)
		, m_code(code)
		, m_insert_strings()
		, m_localized_wwhat(nullptr)
	{}

	application_exception::application_exception(localized_wwhat_t localized_wwhat, std::error_code code, std::vector<std::wstring> insert_strings)
		: std::runtime_error(code.message())
		, m_code(code)
		, m_insert_strings(std::move(insert_strings))
		, m_localized_wwhat(localized_wwhat)
	{}

	//application_exception::application_exception(localized_wwhat_t localized_wwhat, std::error_code code, std::vector<std::wstring> insert_strings)
	//	: std::runtime_error(string_utils::convert_wchar_to_utf8(localization::format_string(code.message(), insert_strings)))
	//	, m_code(code)
	//	, m_insert_strings(std::move(insert_strings))
	//	, m_localized_wwhat(localized_wwhat)
	//{}

	const std::error_code& application_exception::code() const noexcept
	{
		return m_code;
	}

	std::wstring application_exception::wwhat() const
	{
		return string_utils::convert_utf8_to_wchar(this->what());
	}

	std::wstring application_exception::localized_wwhat() const
	{
		if (m_localized_wwhat)
		{
			try
			{
				return m_localized_wwhat(m_code.value(), m_insert_strings);
			}
			catch (...)
			{
				assert(false);
			}
		}

		return this->wwhat();
	}

}