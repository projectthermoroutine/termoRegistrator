#pragma once
#include <string>
#include <management_operations_lib\on_access_scan_management.h>
#include <common\rpc_light_exceptions.h>

namespace management
{
	namespace on_access_scan_management
	{
		std::wstring to_wstring(scanner_state_t);

		EXCEPTION_LIST();
	}
}