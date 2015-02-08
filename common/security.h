#pragma once

#include <windows.h>
#include <functional>

namespace security
{
	using func_uses_sec_attr_t = std::function<void(const SECURITY_ATTRIBUTES &)>;	
	void process_sec_attributes_for_internal_ipc(func_uses_sec_attr_t func_uses_sec_attr, DWORD access_permissions);
	void process_sec_attributes_for_internal_pipe(func_uses_sec_attr_t func_uses_sec_attr);
	void process_sec_attributes_for_internal_event_sync(func_uses_sec_attr_t func_uses_sec_attr);
}