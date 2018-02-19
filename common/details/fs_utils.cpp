#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <Winternl.h>
#include <winioctl.h>

#include <ktmw32.h>

#pragma warning (push)
#pragma warning (disable:4324)	//  warning C4324: '_CLS_SCAN_CONTEXT' : structure was padded due to __declspec(align())
#pragma warning (disable:4201)	//  warning C4201: nonstandard extension used : nameless struct/union
#include <txfw32.h>
#pragma warning (pop)


#include "fs_utils.h"
#include <loglib\log.h>

#include <common\handle_holder.h>
//#include <common\sha256.h>
#include <common\fs_helpers.h>
#include <common\path_helpers.h>
#include <common\log_and_throw.h>
#include <common\on_exit.h>

#include <limits>
#include <random>
#include <memory>

#include <stdio.h>

#include <error_lib\win32_error.h>

namespace fs_utils
{
//	using namespace ::common;

	bool file_exists(const string_t& file_name)
	{
		LOG_STACK();

		fs_helpers::scoped_thread_wow64_redirect_disable wow64_redirect_disable;

		if (file_name.empty())
			throw std::logic_error("passed argument file name can't be empty");

		WIN32_FIND_DATA find_file_data;
		generic_handle_holder<HANDLE, INVALID_HANDLE_VALUE> handle(::FindFirstFileW(file_name.c_str(), &find_file_data), [](const HANDLE& handle){ ::FindClose(handle); });

		LOG_TRACE() << "File " << file_name << (handle ? " exists" : " doesn't exist");

		return static_cast<bool>(handle);
	}

	//bool file_exists(const string_t& file_name)
	//{
	//	LOG_STACK();

	//	fs_helpers::scoped_thread_wow64_redirect_disable wow64_redirect_disable;

	//	if (file_name.empty())
	//		throw std::logic_error("passed argument file name can't be empty");

	//	try
	//	{
	//		//const auto & long_file_path = path_helpers::get_long_path_name(file_name);

	//		const DWORD attributes = GetFileAttributesW(file_name.c_str());
	//		if (attributes == INVALID_FILE_ATTRIBUTES)
	//			return false;

	//		return true;
	//	}
	//	catch (win32::exception &)
	//	{
	//		return false;
	//	}
	//}


	file_size_t get_file_size(const string_t& file_name)
	{
		LOG_STACK();

		if (file_name.empty())
			throw std::logic_error("passed argument file name can't be empty");

		fs_helpers::scoped_thread_wow64_redirect_disable wow64_redirect_disable;

		WIN32_FIND_DATA find_file_data;
		const HANDLE handle(::FindFirstFileW(file_name.c_str(), &find_file_data));
		if (handle == nullptr || handle == INVALID_HANDLE_VALUE)
			LOG_AND_THROW(win32::exception::by_last_error("FindFirstFileW", file_name));

		::FindClose(handle);

		return ((static_cast<file_size_t>(find_file_data.nFileSizeHigh) << 32) | static_cast<file_size_t>(find_file_data.nFileSizeLow));
	}



	FILETIME get_file_last_write_time(const string_t & path)
	{
		LOG_STACK();

		if (path.empty())
		{
			LOG_AND_THROW(std::invalid_argument("Empty path was passed."));
		}

		handle_holder file_handle = CreateFileW(
			path.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			HANDLE());

		if (!file_handle)
		{
			auto last_error = GetLastError();
			LOG_DEBUG() << "CreateFile failed for path: " << path << " Error: "<< std::hex << std::showbase << last_error;
			throw std::runtime_error("CreateFile failed");
		}

		FILETIME last_write_time{};
		if (!GetFileTime(file_handle.get(), NULL, NULL, &last_write_time))
		{
			auto last_error = GetLastError();
			LOG_DEBUG() << "GetFileTime failed for path: " << path << " Error: " << std::hex << std::showbase << last_error;
			throw std::runtime_error("GetFileTime failed");
		}

		return last_write_time;
	}
}