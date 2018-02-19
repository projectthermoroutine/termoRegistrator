#include <windows.h>

#include <error_lib/application_exception.h>
#include <error_lib/error_codes.h>

#include <common/string_utils.h>

//#include <resources/event_messages.h>

//#include <message_localization_lib/message.h>


namespace
{
    class category_t final: public std::error_category
    {
    public:
        virtual const char* name() const _NOEXCEPT override
        {
            return "application";
        }

        virtual std::string message(int error_val) const override
        {
            return std::string("internal error code: ") + std::to_string(error_val) + " [category: " + name() + "]";
        }

        virtual bool equivalent(const std::error_code& code, int condition) const _NOEXCEPT override
        {
			switch (static_cast<common::result_code>(condition))
			{
			case common::result_code::success: return !code;
			default: return false;
			}
		}
    };
}


namespace common
{

	std::string to_string(result_code code, const std::vector<std::wstring>& /*args_as_strings*/)
	{
		std::string result = "Code: " + std::to_string(static_cast<std::uint16_t>(code));

		return result;
	}

    application_exception application_exception::make_with_what_force(const std::error_code& code, const std::string& what_force)
    {
        application_exception except(code, later_what_init_tag());
        except.m_what = what_force;
        return except;
    }

    application_exception::application_exception(const std::error_code& code)
        : application_exception(code, std::vector<std::wstring>())
    {}

    application_exception::application_exception(const std::error_code& code, later_what_init_tag)
        : std::system_error(code)
        , m_what(/* later init */)
    {}

    application_exception::application_exception(const std::error_code& code, std::vector<std::wstring>&& args_as_strings)
        : std::system_error(code)
        , m_what(code.category() == application_error_category() ? common::to_string(static_cast<common::result_code>(code.value()), args_as_strings) : std::string())
    {}

    const char * application_exception::what() const
    {
        return (m_what.empty() ? std::system_error::what() : m_what.c_str());
    }

    std::wstring application_exception::what_string() const
    {
        return string_utils::convert_utf8_to_wchar(application_exception::what());
    }

    const std::error_category& application_error_category()
    {
        static category_t category;
        return category;
    }
}