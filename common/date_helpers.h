#pragma once

#include <string>
#include <chrono>

namespace date_helpers
{
	inline std::string time_to_str(time_t time, const char * format_str)
	{
		if (format_str == nullptr)
			throw std::invalid_argument("The passed argument format_str can't be equal to nullptr");

		const unsigned int str_buf_size = 1024;
		char str_buf[str_buf_size];
		str_buf[0] = 0;

		tm timeinfo;
		if (localtime_s(&timeinfo, &time) == 0)
		{
			strftime(str_buf, sizeof(str_buf), format_str, &timeinfo);
		}

		return str_buf;
	}
}