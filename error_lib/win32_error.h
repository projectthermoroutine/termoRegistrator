/////////////////////////////////////
// (C) 2014 ООО "Код Безопасности"
// Проект: SNES-AV eset
// Автор: Зайцев Роман
// Создан: 20.01.2015
// Краткое описание: описание ошибок WINAPI
/////////////////////////////////////
#pragma once

#include <string>

#include <error_lib/application_exception.h>

namespace win32
{

	enum class win32_errc;

    //  generic error conditions to test for
    enum class api_error
    {
        success = 0,
        file_or_path_not_found = 1
    };

    const std::error_category& get_win32_error_category();

	std::error_code get_last_error_code();

    class exception: public common::application_exception
    {
    public:

        void serialize(serialization::binary_stream& out, const std::string& name) const;
        static exception deserialize(serialization::binary_stream& in, build_error_code_f build_error_code);

		static exception by_last_error(const std::wstring& function_name, const std::wstring& arg = {});
		static exception by_last_error(const char* function_name, const std::wstring& arg = {});
		static exception by_last_error(const char* function_name, const std::string& arg);
    public:

		exception(const std::error_code& error_code);
		exception(const std::error_code& error_code, const std::string& function_name, const std::wstring& arg = {});
		exception(const std::error_code& error_code, const std::wstring& function_name, const std::wstring& arg = {});

        virtual const char * what() const override;
        virtual std::wstring what_string() const override;

    private:

		std::string make_localization_what(const std::string& function_name, const std::wstring& arg = {}) const;

    private:

        std::string m_what;
    };

    inline std::error_code make_error_code(win32_errc e)
    {
		return { static_cast<int>(e), get_win32_error_category() };
    }

    inline std::error_condition make_error_condition(api_error e)
    {
		return { static_cast<int>(e), get_win32_error_category() };
    }

	inline std::error_code make_error_code(std::uint32_t win32_error_code)
	{
		return make_error_code(static_cast<win32::win32_errc>(win32_error_code));
	}
}

namespace std
{
    template<> struct is_error_code_enum < win32::win32_errc >: public true_type{};
    template<> struct is_error_condition_enum < win32::api_error >: public true_type{};
}

inline void throw_get_last_error()
{
	throw win32::exception(win32::get_last_error_code());
}

//  Throws exception. What = message + " " + GetLastError message
inline void throw_get_last_error(const std::wstring& message)
{
    throw win32::exception(win32::get_last_error_code(), message);
}

//  Throws exception. What = message + " " + GetLastError message
inline void throw_get_last_error(const std::string& message)
{
    throw win32::exception(win32::get_last_error_code(), message);
}