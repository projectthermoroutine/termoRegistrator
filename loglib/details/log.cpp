#include <Windows.h>
#include "log.h"

namespace logger
{

	void log_message(severity, const std::string & )
	{

		//wprintf(L"%ws\n", message.c_str());
	}


	std::string to_elapsed_str(const std::chrono::steady_clock::time_point & start, const std::chrono::steady_clock::time_point & end)
	{
		std::ostringstream ss;
		auto elapsed_microsec = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		if (elapsed_microsec < 1000)
		{
			ss << elapsed_microsec << " microsec";
		}
		else
		{
			auto elapsed_ms = elapsed_microsec / 1000;
			if (elapsed_ms < 1000)
			{
				ss << elapsed_ms << " ms";
			}
			else
			{
				auto elapsed_sec = elapsed_ms / 1000;
				ss << elapsed_sec << " sec";
			}
		}

		return ss.str();
	}

}