#pragma once
#define WAIT_THREAD_TERMINATING_ON


struct deleter_bstr
{
	inline void operator()(BSTR ptr) noexcept { if (ptr) ::SysFreeString(ptr); }
};

using bstr_holder = std::unique_ptr<OLECHAR, deleter_bstr>;
