#include "path_helpers.h"
#include <loglib\log.h>
#include <common\log_and_throw.h>
#include <memory>

#include <windows.h>

namespace path_helpers
{
	using drive_name_t = std::wstring;
	using directory_name_t = std::wstring;
	using file_name_t = std::wstring;
	using file_ext_t = std::wstring;

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

	namespace details
	{
		const wchar_t long_path_prefix[] = L"\\\\?\\";
		const wchar_t unc_long_path_prefix[] = L"\\\\?\\UNC";

		const std::size_t long_path_prefix_size = sizeof(long_path_prefix) / sizeof(long_path_prefix[0]) - 1;
		const std::size_t unc_long_path_prefix_size = sizeof(unc_long_path_prefix) / sizeof(unc_long_path_prefix[0]) - 1;
	}

	bool is_long_path(const std::wstring & path)
	{
		if (path.length() < details::long_path_prefix_size)
			return false;

		const auto & buffer = path.data();
		return buffer[0] == '\\' && buffer[1] == '\\' && buffer[2] == '?' && buffer[3] == '\\';
	}

	std::tuple<drive_name_t, directory_name_t, file_name_t, file_ext_t> split_path(std::wstring path)
	{
		LOG_STACK();

		if (path.empty())
			LOG_AND_THROW(std::runtime_error("Could not split path: ")) << "Could not split path. Path is empty.";

		const auto long_path = is_long_path(path);
		if (long_path)
			path.erase(std::begin(path), std::begin(path) + details::long_path_prefix_size);

		const auto buffer_size = std::size_t{ 32768 };

		auto drive_name_buf = std::unique_ptr<wchar_t[]>{ new wchar_t[static_cast<std::size_t>(buffer_size)] };
		ZeroMemory(drive_name_buf.get(), buffer_size*sizeof(drive_name_buf[0]));
		auto dir_name_buf = std::unique_ptr<wchar_t[]>{ new wchar_t[static_cast<std::size_t>(buffer_size)] };
		ZeroMemory(dir_name_buf.get(), buffer_size*sizeof(dir_name_buf[0]));
		auto file_name_buf = std::unique_ptr<wchar_t[]>{ new wchar_t[static_cast<std::size_t>(buffer_size)] };
		ZeroMemory(file_name_buf.get(), buffer_size*sizeof(file_name_buf[0]));
		auto extension_buf = std::unique_ptr<wchar_t[]>{ new wchar_t[static_cast<std::size_t>(buffer_size)] };
		ZeroMemory(extension_buf.get(), buffer_size*sizeof(extension_buf[0]));

		auto const split_result = _wsplitpath_s(
			path.c_str(),
			drive_name_buf.get(),
			buffer_size,
			dir_name_buf.get(),
			buffer_size,
			file_name_buf.get(),
			buffer_size,
			extension_buf.get(),
			buffer_size);

		if (split_result != 0)
			LOG_AND_THROW(std::runtime_error("Could not split path: ")) << "Could not split path: " << path;

		auto drive_name = std::wstring{ drive_name_buf.get() };
		auto dir_name = std::wstring{ dir_name_buf.get() };
		auto file_name = std::wstring{ file_name_buf.get() };
		auto file_ext = std::wstring{ extension_buf.get() };

		if (drive_name.empty() && dir_name.empty() && file_name.empty() && file_ext.empty())
			LOG_AND_THROW(std::runtime_error("Could not split path: ")) << "Could not split path: " << path << "; result is empty";

		if (long_path && !drive_name.empty())
			drive_name = details::long_path_prefix + drive_name;

		return std::make_tuple(std::move(drive_name), std::move(dir_name), std::move(file_name), std::move(file_ext));
	}

	std::wstring get_base_name(const std::wstring & path)
	{
		LOG_STACK();

		auto file_name = file_name_t{};
		std::tie(std::ignore, std::ignore, file_name, std::ignore) = split_path(path);

		return file_name;
	}


	std::wstring get_file_name(const std::wstring & path_and_file_name)
	{
		LOG_STACK();

		auto file_name = file_name_t{};
		auto file_ext = file_ext_t{};
		std::tie(std::ignore, std::ignore, file_name, file_ext) = split_path(path_and_file_name);

		return file_name + file_ext;
	}

	std::wstring get_directory_path(const std::wstring& path_and_file_name)
	{
		LOG_STACK();

		auto directory_name = directory_name_t{};
		auto drive_name = drive_name_t{};
		std::tie(drive_name, directory_name, std::ignore, std::ignore) = split_path(path_and_file_name);

		if (drive_name.empty() && directory_name.empty())
			LOG_AND_THROW(std::runtime_error("Could not split path: ")) << "Could not split path: " << path_and_file_name << "; result is empty";

		if (!drive_name.empty())
			return concatenate_paths(drive_name, directory_name);
		else
			return directory_name;
	}

}