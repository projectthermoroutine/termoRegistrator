#include <common/process_management_helpers.h>
#include <common/log_and_throw.h>
#include <common/on_exit.h>

#include <vector>
#include <memory>

#include <TlHelp32.h>
#include <Psapi.h>

namespace process_management
{

	HANDLE get_handle_opened_process(process_id_t process_id, handle_holder& cache_handle)
	{
        if (!cache_handle)
        {
            const HANDLE handle(::OpenProcess((PROCESS_QUERY_INFORMATION | PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ), FALSE, process_id));
            if (handle == nullptr || handle == INVALID_HANDLE_VALUE)
                LOG_AND_THROW(std::runtime_error("OpenProcess failed"));

            cache_handle.reset(handle);
        }

		return cache_handle.get();
	}

	std::wstring process_image_nt_path(const process_id_t process_id, handle_holder* cache_handle_ptr)
	{
		LOG_STACK();

		std::vector<wchar_t> buffer_name(UNICODE_STRING_MAX_CHARS, L'\0');
		DWORD in_out_size(static_cast<DWORD>(buffer_name.size()));

        handle_holder tmp_cache_handle;
        if (!cache_handle_ptr) cache_handle_ptr = &tmp_cache_handle;

        if (0 == QueryFullProcessImageNameW(get_handle_opened_process(process_id, *cache_handle_ptr), PROCESS_NAME_NATIVE, buffer_name.data(), &in_out_size))
        {
            LOG_DEBUG() << "Failed to open process pid: " << process_id;
			LOG_AND_THROW(std::runtime_error("QueryFullProcessImageName failed"));
        }

        return std::wstring(buffer_name.data());
	}

    std::wstring get_module_nt_path_from_base_address(const process_id_t process_id, LPVOID module_address, handle_holder* cache_handle_ptr, bool* ok)
	{
		LOG_STACK();

		std::vector<wchar_t> buffer_name(UNICODE_STRING_MAX_CHARS, L'\0');

        handle_holder tmp_cache_handle;
        if (!cache_handle_ptr) cache_handle_ptr = &tmp_cache_handle;

        if (0 == GetMappedFileNameW(get_handle_opened_process(process_id, *cache_handle_ptr), reinterpret_cast<BYTE*>(module_address), buffer_name.data(), static_cast<DWORD>(buffer_name.size())))
        {
            if (ok) *ok = false;
            return std::wstring();
        }

        if (ok) *ok = true;
		return std::wstring(buffer_name.data());
	}


	std::wstring get_module_file_name(HINSTANCE instance)
	{
		LOG_STACK();

		const auto buffer_size = DWORD{ UNICODE_STRING_MAX_CHARS };
		auto path_buffer = std::unique_ptr<wchar_t[]> { new wchar_t[static_cast<std::size_t>(buffer_size)] };

		const auto get_module_file_name_result = GetModuleFileNameW(instance, path_buffer.get(), buffer_size);
		if (get_module_file_name_result == 0){
			auto last_error = GetLastError();
			std::ostringstream ss;
			ss << "GetModuleFileNameW failed." << " Error: " << std::hex << std::showbase << last_error;
			LOG_DEBUG() << ss.str().c_str();
			throw std::runtime_error(ss.str());
		}

		return{ path_buffer.get() };
	}

	std::wstring get_current_module_path()
	{
		auto module = HMODULE{};

		if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
			GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			(LPCWSTR)&get_current_module_path,
			&module))
		{
			auto last_error = GetLastError();
			std::ostringstream ss;
			ss << "GetModuleHandleExW failed." << " Error: " << std::hex << std::showbase << last_error;
			LOG_DEBUG() << ss.str().c_str();
			throw std::runtime_error(ss.str());
		}

		return get_module_file_name(module);
	}


}