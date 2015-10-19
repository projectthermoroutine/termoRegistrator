#pragma once

#include <string>
#include <vector>
#include <exception>
#include <common\string_utils.h>

namespace zip
{
	using wstring_t = std::wstring;
	using string_t = std::string;
	using compression_level_t = int;

	class compression_exception final : public std::exception
	{
	public:
		compression_exception(const wstring_t &message)
			: std::exception(string_utils::convert_wchar_to_utf8(message).c_str())
		{
		}
	};

	void create_archive(const std::vector <wstring_t> & file_names,
		const wstring_t &zip_out_file_path,
		const wstring_t &zip_out_file_name,
		compression_level_t compression_level);
}