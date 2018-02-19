#pragma once

#include <string>

namespace product_names
{

	using file_name_t = std::wstring;
	using string_t = std::wstring;

	string_t get_product_registry_name();
	string_t get_product_registry_path_and_name();
	string_t get_company_registry_name();
	string_t get_on_time_update_last_start_time_key_name();
	string_t get_last_service_start_time_key_name();
	string_t get_on_time_scan_job_last_start_time_key_name();
}