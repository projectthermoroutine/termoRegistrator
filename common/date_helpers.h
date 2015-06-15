#pragma once

#include <string>
#include <chrono>
#include <common\log_and_throw.h>
namespace date_helpers
{
	
	namespace details
	{

		inline std::string time_to_str_helper(time_t time, const char * format_str, errno_t(convert_time)(std::tm*, const time_t*))
		{
			if(format_str == nullptr)
				LOG_AND_THROW(std::invalid_argument("Argument format_str can't be null."));

			const unsigned int str_buf_size = 1024;
			char str_buf[str_buf_size];
			str_buf[0] = 0;

			tm timeinfo;
			if(convert_time(&timeinfo, &time) == 0)
			{
				strftime(str_buf, sizeof(str_buf), format_str, &timeinfo);
			}

			return str_buf;
		}

	}

	inline std::string local_time_to_str(time_t time, const char * format_str)
	{
		return details::time_to_str_helper(time, format_str, &localtime_s);
	}

	inline std::string gm_time_to_str(time_t time, const char * format_str)
	{
		return details::time_to_str_helper(time, format_str, &gmtime_s);
	}
}