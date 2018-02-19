/////////////////////////////////////
// (C) 2014 ООО "Код Безопасности"
// Проект: SNES-AV eset
// Автор: Зайцев Роман
// Создан: 26.01.2015
// Краткое описание: интерфейс работы с результатами и условиями ошибок
/////////////////////////////////////
#pragma once

#include <system_error>
#include <cstdint>
#include <codecvt>
#include <vector>


namespace serialization
{
	class binary_stream;

} // namespace serialization


namespace common
{

	enum class result_code: std::uint32_t;

	const std::error_category& application_error_category();

	inline std::error_code make_error_code(result_code e)
	{
		return{static_cast<int>(e), application_error_category()};
	}

	inline std::error_condition make_error_condition(result_code e)
	{
		return{static_cast<int>(e), application_error_category()};
	}

	class application_exception: public std::system_error
	{
	public:

		using build_error_code_f = bool(*)(int code_value, const std::string& category_name, std::error_code& error_code);

		void serialize(serialization::binary_stream& out, const std::string& name) const;
		static application_exception deserialize(serialization::binary_stream& in, build_error_code_f build_error_code);

		static application_exception make_with_what_force(const std::error_code& code, const std::string& what_force);

		template <class... Args_As_WStrings>
		static inline application_exception make(result_code _Result_code, Args_As_WStrings&&... args_as_strings)
		{
			return application_exception(::common::make_error_code(_Result_code), std::vector<std::wstring>{std::forward<Args_As_WStrings>(args_as_strings)...});
		}

		template <::common::result_code _Result_code, class... Args_As_WStrings>
		static inline application_exception make(Args_As_WStrings&&... args_as_strings)
		{
			return make(_Result_code, std::forward<Args_As_WStrings>(args_as_strings)...);
		}

	public:

		application_exception(const std::error_code& code);
		application_exception(const std::error_code& code, std::vector<std::wstring>&& args_as_strings);

		virtual const char * what() const override;
		virtual std::wstring what_string() const;

	private:

		struct later_what_init_tag {};
		application_exception(const std::error_code& code, later_what_init_tag);

	protected:

		std::string m_what;
	};
}

namespace std
{
	template<> struct is_error_code_enum<common::result_code>: public true_type{};
	template<> struct is_error_condition_enum<common::result_code>: public true_type{};
}