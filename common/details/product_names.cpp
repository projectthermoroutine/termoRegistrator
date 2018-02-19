#include <common/product_names.h>
#include <common/product_registry_settings.h>
#include <common/log_and_throw.h>
#include <memory>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define PRODUCT_SUITE_NAME_W L"ThermoRegistrator"


#define PRODUCT_FULL_NAME_COMPONENT_W	L"ThermoRegistrator"
#define PRODUCT_NAME_COMPONENT_W L"ThermoRegistrator"

#define PRODUCT_REG_COMPANY_NAME_W	L"Tvema"
#define PRODUCT_REG_PRODUCT_NAME_PART_W	PRODUCT_REG_COMPANY_NAME_W L"\\" PRODUCT_FULL_NAME_COMPONENT_W

#define PRODUCT_REG_ON_TIME_UPDATE_LAST_START_TIME_KEY_NAME_W L"OnTimeUpdateLastStartTime"
#define PRODUCT_REG_ON_TIME_JOB_LAST_START_TIME_KEY_NAME_W L"OnTimeJobLastStartTime"
#define PRODUCT_REG_LAST_SERVICE_START_TIME_KEY_NAME_W L"LastStartTime"


namespace product_names
{
	bool is_wow64_process()
	{
		BOOL result = TRUE;
		const auto call_result = IsWow64Process(GetCurrentProcess(), &result);
		if (call_result == FALSE)
			LOG_AND_THROW(std::runtime_error("IsWow64Process failed"));

		return result != FALSE;
	}


	string_t get_installdir_registry_name()
	{
		return L"";
	}

	string_t get_product_registry_name()
	{
		return PRODUCT_REG_PRODUCT_NAME_PART_W;
	}

	string_t get_company_registry_name()
	{
		return PRODUCT_REG_COMPANY_NAME_W;
	}

	string_t get_product_registry_path_and_name()
	{
#ifndef _WIN64
		return (is_wow64_process() ? L"SOFTWARE\\Wow6432Node\\" : L"SOFTWARE\\") + get_company_registry_name() + L"\\" + get_product_registry_name();
#else
		return L"SOFTWARE\\Wow6432Node\\" + get_company_registry_name() + L"\\" + get_product_registry_name();
#endif // _WIN64
	}


	string_t get_on_time_update_last_start_time_key_name()
	{
		return PRODUCT_REG_ON_TIME_UPDATE_LAST_START_TIME_KEY_NAME_W;
	}

	string_t get_last_service_start_time_key_name()
	{
		return PRODUCT_REG_LAST_SERVICE_START_TIME_KEY_NAME_W;
	}

	string_t get_on_time_scan_job_last_start_time_key_name()
	{
		return PRODUCT_REG_ON_TIME_JOB_LAST_START_TIME_KEY_NAME_W;
	}
}