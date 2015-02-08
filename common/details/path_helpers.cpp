#include "path_helpers.h"

namespace path_helpers
{
	std::wstring concatenate_paths(const std::wstring & x, const std::wstring & y)
	{
		const bool x_slash = !x.empty() && (x.at(x.length() - 1) == L'\\');
		const bool y_slash = !y.empty() && (y.at(0) == L'\\');

		if (!x_slash && !y_slash)
		{
			return x + L'\\' + y;
		}

		if (x_slash && y_slash)
		{
			return x.substr(0, x.length() - 1) + y;
		}

		return x + y;
	}

	void extract_file_path_and_extension(const std::wstring& file_path, std::wstring& file_path_wo_extension, std::wstring& extension)
	{
		const auto dot_pos = file_path.rfind(wchar_t('.'));
		if (dot_pos == file_path.npos)
		{
			file_path_wo_extension = file_path;
			extension.clear();
		}
		else
		{
			const auto backslash_pos = file_path.rfind(wchar_t('\\'));
			if (backslash_pos == file_path.npos || backslash_pos < dot_pos)
			{
				file_path_wo_extension = file_path.substr(0, dot_pos);
				extension = file_path.substr(dot_pos + 1);
			}
			else
			{
				file_path_wo_extension = file_path;
				extension.clear();
			}
		}
	}

}