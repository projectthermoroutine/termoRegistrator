

#include <Windows.h>

#include <common\locale.hpp>


namespace common::platform_details
{

	std::size_t wstring_convert::to_bytes(const wchar_t* wstr, std::size_t wlength, char* bstr, std::size_t blength) noexcept
	{
		return static_cast<std::size_t>(::WideCharToMultiByte(CP_UTF8, 0, wstr, static_cast<int>(wlength), bstr, static_cast<int>(blength), nullptr, nullptr));
	}
	std::size_t wstring_convert::from_bytes(const char* bstr, std::size_t blength, wchar_t* wstr, std::size_t wlength) noexcept
	{
		return static_cast<std::size_t>(::MultiByteToWideChar(CP_UTF8, 0, bstr, static_cast<int>(blength), wstr, static_cast<int>(wlength)));
	}


	//std::size_t u16string_convert::to_bytes(const char16_t* wstr, std::size_t wlength, char* bstr, std::size_t blength)
	//{
	//	(void)wstr;
	//	(void)wlength;
	//	(void)bstr;
	//	(void)blength;
	//
	//	return 0;
	//}
	//std::size_t u16string_convert::from_bytes(const char* bstr, std::size_t blength, char16_t* wstr, std::size_t wlength)
	//{
	//	(void)wstr;
	//	(void)wlength;
	//	(void)bstr;
	//	(void)blength;
	//
	//	return 0;
	//}


	//std::size_t u32string_convert::to_bytes(const char32_t* wstr, std::size_t wlength, char* bstr, std::size_t blength)
	//{
	//	(void)wstr;
	//	(void)wlength;
	//	(void)bstr;
	//	(void)blength;
	//
	//	return 0;
	//}
	//std::size_t u32string_convert::from_bytes(const char* bstr, std::size_t blength, char32_t* wstr, std::size_t wlength)
	//{
	//	(void)wstr;
	//	(void)wlength;
	//	(void)bstr;
	//	(void)blength;
	//
	//	return 0;
	//}


} // namespace common::platform_details
