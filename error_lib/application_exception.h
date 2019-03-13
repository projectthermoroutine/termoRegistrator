#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <type_traits>
#include <system_error>


namespace common
{

	enum class result_code : std::uint32_t;

	const std::error_category& application_error_category() noexcept;

	inline std::error_code make_error_code(result_code code) noexcept
	{
		return std::error_code(static_cast<int>(code), application_error_category());
	}


	// tests for application exception error_code enumeration
	template<class _Enum> struct is_app_error_code_enum : std::false_type {};
	template<class _Enum> inline constexpr bool is_app_error_code_enum_v = is_app_error_code_enum<_Enum>::value;


	class application_exception : public std::runtime_error
	{
		using localized_wwhat_t = std::wstring(*)(int code_value, const std::vector<std::wstring>& insert_strings);

	public:

		static application_exception make_with_what_force(const std::error_code& code, std::string what_force);

	public:

		template<class _Enum, std::enable_if_t<is_app_error_code_enum_v<_Enum>, int> = 0>
		application_exception(_Enum code)
			: application_exception(
				[](int code_value, const std::vector<std::wstring>& insert_strings) { return to_wstring(static_cast<_Enum>(code_value), insert_strings); }, // using ADL
				make_error_code(code),                                                                                                                      // using ADL
				std::vector<std::wstring>())
		{}

		template<class _Enum, class... _InsertAsWStrings, std::enable_if_t<is_app_error_code_enum_v<_Enum>, int> = 0>
		application_exception(_Enum code, _InsertAsWStrings&&... insert_strings)
			: application_exception(
				[](int code_value, const std::vector<std::wstring>& insert_strings) { return to_wstring(static_cast<_Enum>(code_value), insert_strings); }, // using ADL
				make_error_code(code),                                                                                                                      // using ADL
				std::vector<std::wstring>{ std::wstring(std::forward<_InsertAsWStrings>(insert_strings))... })
		{}

	public:

		const std::error_code& code() const noexcept;

		std::wstring wwhat() const;
		std::wstring localized_wwhat() const;

	protected:

		explicit application_exception(std::error_code code, std::string what_force);

	private:

		explicit application_exception(localized_wwhat_t localized_wwhat, std::error_code code, std::vector<std::wstring> insert_strings);

	private:

		std::error_code m_code;
		std::vector<std::wstring> m_insert_strings;
		localized_wwhat_t m_localized_wwhat;
	};

} // namespace common


namespace common
{
	template<> struct is_app_error_code_enum < common::result_code > : std::true_type {};

} // namespace common


namespace std
{
	template<> struct is_error_code_enum < common::result_code > : std::true_type {};

} // namespace std
