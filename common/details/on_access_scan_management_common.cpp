#include <on_access_scan_management_common.h>

namespace management
{
	namespace on_access_scan_management
	{
		std::wstring to_wstring(scanner_state_t state)
		{
			switch (state)
			{
			case scanner_state_t::stopped:
				return L"stopped";
			case scanner_state_t::starting:
				return L"starting";
			case scanner_state_t::running_active:
				return L"running_active";
			case scanner_state_t::running_inactive:
				return L"running_inactive";
			case scanner_state_t::stopping:
				return L"stopping";
			case scanner_state_t::failed:
				return L"failed";
			default:
				throw std::invalid_argument("Unknown value of scanner_state_t");
			}
		}
	}
}