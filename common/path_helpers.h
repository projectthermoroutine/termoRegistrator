#pragma once

#include <string>

namespace path_helpers
{
	std::wstring concatenate_paths(const std::wstring & x, const std::wstring & y);
	void extract_file_path_and_extension(const std::wstring& file_path, std::wstring& file_path_wo_extension, std::wstring& extension);
}