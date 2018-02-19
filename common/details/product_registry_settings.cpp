#include <string>
#include <cassert>
#include <loglib/log.h>

#include <common/product_registry_settings.h>
#include <common/product_names.h>
#include <common/path_helpers.h>
#include <common/registry_helpers.h>
#include <common/sync_helpers.h>
#include <common/log_and_throw.h>

using namespace registry_helpers;

namespace product_registry_settings
{

	std::time_t get_on_time_update_last_start_time()
	{
		LOG_STACK();

		return get_time_t_from_registry(HKEY_LOCAL_MACHINE, product_names::get_product_registry_path_and_name(), product_names::get_on_time_update_last_start_time_key_name());
	}

	void set_on_time_update_last_start_time(std::time_t value)
	{
		LOG_STACK();

		return set_time_t_to_registry(HKEY_LOCAL_MACHINE, product_names::get_product_registry_path_and_name(), product_names::get_on_time_update_last_start_time_key_name(), value);
	}

	std::time_t get_service_start_time()
	{
		LOG_STACK();

		return get_time_t_from_registry(HKEY_LOCAL_MACHINE, product_names::get_product_registry_path_and_name(), product_names::get_last_service_start_time_key_name());
	}

	void set_service_start_time(std::time_t value)
	{
		LOG_STACK();

		return set_time_t_to_registry(HKEY_LOCAL_MACHINE, product_names::get_product_registry_path_and_name(), product_names::get_last_service_start_time_key_name(), value);
	}

	std::time_t get_on_time_scanner_last_start_time()
	{
		LOG_STACK();

		return get_time_t_from_registry(HKEY_LOCAL_MACHINE, product_names::get_product_registry_path_and_name(), product_names::get_on_time_scan_job_last_start_time_key_name());
	}

	void set_on_time_scanner_last_start_time(std::time_t value)
	{
		LOG_STACK();

		return set_time_t_to_registry(HKEY_LOCAL_MACHINE, product_names::get_product_registry_path_and_name(), product_names::get_on_time_scan_job_last_start_time_key_name(), value);
	}

}