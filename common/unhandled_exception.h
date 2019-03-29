#pragma once

#include <string>
#include <functional>

namespace unhandled_exception_handler
{
	using dump_message_func_t = std::function<void(const std::wstring &)>;

	void not_create_zip_at_terminate();

	void initialize(const std::wstring & path_for_dumps, dump_message_func_t dump_message_func);
}