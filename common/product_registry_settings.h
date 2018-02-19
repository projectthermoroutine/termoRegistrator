#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <memory>
#include <windows.h>
#include <functional>
#include <ctime>

#include <common/registry_helpers.h>

namespace product_registry_settings
{
	using string_t = std::wstring;

	std::time_t get_on_time_update_last_start_time();
	void set_on_time_update_last_start_time(std::time_t);
	std::time_t get_on_time_scanner_last_start_time();
	void set_on_time_scanner_last_start_time(std::time_t);

	std::time_t get_last_system_shutdown_time();

	std::time_t get_service_start_time();
	void set_service_start_time(std::time_t);

}