#pragma once

#include <sstream>
#include <chrono>

namespace logger
{
	enum class severity { trace, debug, info, warn, error, fatal };

	void log_message(severity, const std::string &);


	std::string to_elapsed_str(const std::chrono::steady_clock::time_point &, const std::chrono::steady_clock::time_point &);

	template <typename func_name_t, typename file_name_t, typename line_num_t>
	class scope_logger final
	{
	public:
		scope_logger(const func_name_t & func_name, const file_name_t & file_name, const line_num_t & line_num) :
			_start(std::chrono::steady_clock::now()), _func_name(func_name)
		{
			std::ostringstream ss;
			ss << "ENTER function: " << func_name << ", file: " << file_name << ", line: " << line_num;
			log_message(severity::debug, ss.str());
		}

		~scope_logger()
		{
			std::ostringstream ss;
			ss << "LEAVE function: " << _func_name << ", elapsed: " << to_elapsed_str(_start, std::chrono::steady_clock::now());
			log_message(severity::debug, ss.str());
		}

		scope_logger(const scope_logger &) = delete;
		scope_logger & operator = (const scope_logger &) = delete;
	private:
		const func_name_t & _func_name;
		const std::chrono::steady_clock::time_point _start;
	};

	class log_stream final
	{
	public:
		log_stream(severity sev) : _severity(sev) {}

		~log_stream()
		{
			flush();
		}

		template <typename T>
		log_stream & operator << (const T & /*ref*/)
		{
			//_ss << ref;
			return *this;
		}

		log_stream(const log_stream &) = delete;
		log_stream operator = (const log_stream &) = delete;
	private:
		void flush()
		{
			if (_ss.str().empty())
			{
				return;
			}

			log_message(_severity, _ss.str());
		}

		severity _severity;
		std::ostringstream _ss;
	};
}

#define LOG(sev) logger::log_stream(sev)

#define LOG_TRACE() LOG(logger::severity::trace)
#define LOG_DEBUG() LOG(logger::severity::debug)
#define LOG_INFO() LOG(logger::severity::info)
#define LOG_WARN() LOG(logger::severity::warn)
#define LOG_ERROR() LOG(logger::severity::error)
#define LOG_FATAL() LOG(logger::severity::fatal)

#define LOG_STACK() logger::scope_logger<decltype(__FUNCTION__), decltype(__FILE__), decltype(__LINE__)> logger##__FUNCTION__##__LINE__ (__FUNCTION__, __FILE__, __LINE__);
