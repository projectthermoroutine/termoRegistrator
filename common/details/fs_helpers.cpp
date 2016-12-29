#include <windows.h>
#include <winternl.h>
#pragma warning(push)
#pragma warning(disable:4005)
#include <ntstatus.h>
#pragma warning(pop)

#include <memory>
#include <algorithm>
#include <system_error>
#include <vector>

#include "fs_helpers.h"

#include <loglib\log.h>
#include <common\path_helpers.h>
#include <common\handle_holder.h>

namespace
{

	

	class find_file_handle_holder final : public generic_handle_holder<HANDLE, INVALID_HANDLE_VALUE>
	{
	public:
//		explicit find_file_handle_holder(HANDLE h) : generic_handle_holder(h, (close_func_t)&::FindClose) {}
		explicit find_file_handle_holder(HANDLE h) : generic_handle_holder(h, [](const HANDLE& handle) { ::FindClose(handle); }) {}
	};

	bool dir_to_skip(const fs_helpers::string_t & path)
	{
		return path == L"." || path == L"..";
	}

	HMODULE load_library(const wchar_t * library_name)
	{
		auto library = LoadLibraryW(library_name);
		if (library == 0)
		{
			auto last_error = GetLastError();
			throw std::system_error(last_error, std::system_category());
		}

		return library;
	}
	
	void * get_proc_address(HMODULE library, const char * proc_name)
	{
		if (library == 0)
			throw std::invalid_argument("The passed argument library can't be equal to nullptr");

		if (proc_name == nullptr)
			throw std::invalid_argument("The passed argument proc_name can't be equal to nullptr");

		auto proc = GetProcAddress(library, proc_name);
		if (proc == 0)
		{
			auto last_error = GetLastError();
			throw std::system_error(last_error, std::system_category());
		}

		return proc;
	}

	class for_all_streams final
	{
	public:
		for_all_streams() : _p_find_first_stream(0), _p_find_next_stream(0)
		{
			LOG_STACK();

			init_functions();
		}

		void operator () (
			const fs_helpers::string_t & object_name,
			const fs_helpers::stream_enumeration_func_t & func)
		{
			LOG_STACK();

			fs_helpers::scoped_thread_wow64_redirect_disable redirect_disable;

			WIN32_FIND_STREAM_DATA find_stream_data;
			auto find_handle = find_first_stream(object_name.c_str(), FindStreamInfoStandard, &find_stream_data, 0);
			if (find_handle == INVALID_HANDLE_VALUE)
			{
				auto last_error = GetLastError();
				if (last_error == ERROR_HANDLE_EOF)
				{
					return;
				}
				else
				{
					LOG_DEBUG() << "Find first stream failed with error: " << std::hex << std::showbase << last_error;
					return;
				}
			}

			find_file_handle_holder find_handle_holder(find_handle);
			for (;;)
			{
				if (find_next_stream(find_handle_holder.get(), &find_stream_data) == 0)
				{
					auto last_error = GetLastError();
					if (last_error == ERROR_HANDLE_EOF)
					{
						break;
					}
					else
					{
						LOG_DEBUG() << "Find next stream failed with error: " << std::hex << std::showbase << last_error;
						return;
					}
				}

				const auto full_stream_name(object_name + find_stream_data.cStreamName);
				func(full_stream_name, static_cast<fs_helpers::file_size_t>(find_stream_data.StreamSize.QuadPart));
			}
		}
	private:
		void init_functions()
		{
			auto library = load_library(L"kernel32.dll");
			auto proc_find_first = get_proc_address(library, "FindFirstStreamW");
			auto proc_find_next = get_proc_address(library, "FindNextStreamW");

			_p_find_first_stream = reinterpret_cast<decltype(_p_find_first_stream)>(proc_find_first);
			_p_find_next_stream = reinterpret_cast<decltype(_p_find_next_stream)>(proc_find_next);
		}
		
		inline HANDLE find_first_stream(
			__in       LPCWSTR lpFileName,
			__in       STREAM_INFO_LEVELS InfoLevel,
			__out      LPVOID lpFindStreamData,
			__reserved DWORD dwFlags)
		{
			if (_p_find_first_stream == nullptr)
				throw std::invalid_argument("The passed argument _p_find_first_stream can't be equal to nullptr");

			return _p_find_first_stream(lpFileName, InfoLevel, lpFindStreamData, dwFlags);
		}

		inline BOOL find_next_stream(
			__in  HANDLE hFindStream,
			__out LPVOID lpFindStreamData)
		{
			if (_p_find_next_stream == nullptr)
				throw std::invalid_argument("The passed argument _p_find_next_stream can't be equal to nullptr");

			return _p_find_next_stream(hFindStream, lpFindStreamData);
		}

		HANDLE(WINAPI *_p_find_first_stream)(
		__in       LPCWSTR lpFileName,
		__in       STREAM_INFO_LEVELS InfoLevel,
		__out      LPVOID lpFindStreamData,
		__reserved DWORD dwFlags
		);

		BOOL (APIENTRY *_p_find_next_stream)(
		__in  HANDLE hFindStream,
		__out LPVOID lpFindStreamData
		);
	};

	const wchar_t stream_name_delim = L':';
}

namespace fs_helpers
{

	stream_data_size_t get_stream_data_size(std::istream & stream)
	{
		stream.seekg(0, std::ios::beg);
		auto begin = stream.tellg();
		stream.seekg(0, std::ios::end);
		auto end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		return end - begin;
	}

	scoped_thread_wow64_redirect_disable::scoped_thread_wow64_redirect_disable() : old_value(nullptr), _disabled(false)
	{
		LOG_STACK();
		if (!Wow64DisableWow64FsRedirection(&old_value))
		{
			auto const last_error = GetLastError();
			if (last_error == ERROR_INVALID_FUNCTION) return;
			LOG_DEBUG() << "Wow64DisableWow64FsRedirection failed with error: " << std::hex << std::showbase << last_error;
			throw std::runtime_error("Unable to disable Windows-on-Windows redirection.");
		}
		_disabled = true;
	}

	scoped_thread_wow64_redirect_disable::~scoped_thread_wow64_redirect_disable()
	{
		LOG_STACK();
		if (!_disabled) return;
		if (!Wow64RevertWow64FsRedirection(old_value))
		{
			auto const last_error = GetLastError();
			LOG_DEBUG() << "Wow64RevertWow64FsRedirection failed with error: " << std::hex << std::showbase << last_error;
		}
	}

}
