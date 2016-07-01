#include "validate_helpers.h"
#include <ctime>
#include <chrono>
#include <common\process_management_helpers.h>
#include <common\fs_utils.h>
#include <common/log_and_throw.h>

namespace validate_helpers
{

	namespace details
	{
		std::time_t FILETIME_to_time_t(const FILETIME& file_time)
		{
			std::time_t result;

			SYSTEMTIME system_time;

			std::tm tmp = { 0 };

			if (FileTimeToSystemTime(&file_time, &system_time) == FALSE)
				LOG_AND_THROW(std::runtime_error("failed_to_convert_filetime_to_time_t"));

			tmp.tm_mday = system_time.wDay;
			tmp.tm_mon = system_time.wMonth - 1;
			tmp.tm_year = system_time.wYear - 1900;

			tmp.tm_sec = system_time.wSecond;
			tmp.tm_min = system_time.wMinute;
			tmp.tm_hour = system_time.wHour;

			result = std::mktime(&tmp);

			if (result == -1)
				LOG_AND_THROW(std::invalid_argument("Can't convert FILETIME to time_t")) << L"Low part: " << std::hex << std::showbase << file_time.dwLowDateTime << L"High part: " << std::hex << std::showbase << file_time.dwHighDateTime;

			return result;
		}
	}

	const std::uint32_t trial_days = 8;
	bool trial_expired()
	{
		using namespace std;
		using namespace std::chrono;
		using days_t = duration < int, ratio_multiply<hours::period, ratio<24> >::type >;

		const auto & process_image_path = process_management::get_current_module_path();
		
		const auto & file_time = fs_utils::get_file_last_write_time(process_image_path);

		const auto & time = details::FILETIME_to_time_t(file_time);

		const auto & file_time_point = system_clock::from_time_t(time);

		const auto & diff_time_point = file_time_point - system_clock::now();

		const auto days_elapsed = std::chrono::duration_cast<days_t>(diff_time_point).count();

		return days_elapsed >= trial_days;
	}
}