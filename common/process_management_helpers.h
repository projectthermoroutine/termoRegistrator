#pragma once

#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

#include <common/handle_holder.h>

namespace process_management
{
	using process_id_t = decltype(GetCurrentProcessId());
	using session_id_t = int;

	HANDLE get_handle_opened_process(const process_id_t, handle_holder& cache_handle);

	std::wstring process_image_nt_path(const process_id_t, handle_holder* cache_handle_ptr = nullptr);
    std::wstring get_module_nt_path_from_base_address(process_id_t process_id, LPVOID module_address, handle_holder* cache_handle_ptr = nullptr, bool* ok = nullptr);

	std::wstring get_current_module_path();
	std::wstring get_module_file_name(HINSTANCE instance);
}