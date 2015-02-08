#pragma once

#include <string>
#include <functional>

namespace unhandled_exception_handler
{
	using dump_message_func_t = std::function<void(const std::wstring &)>;

	void initialize(const std::wstring & path_for_dumps, dump_message_func_t dump_message_func);
}