#pragma once

#include <sstream>
#include <chrono>

namespace logger
{
	enum class severity { trace = 0, debug = 1, info = 2, warn = 3, error = 4, fatal = 5 };
	std::string severity_to_str(logger::severity sev);
	logger::severity str_to_severity(const std::string& str);

	void log_message(severity, const std::wstring &);

	void initialize(
		const std::wstring & log_config,
		bool  developers_log,
		uint64_t max_log_buufer_size,
		const std::wstring & logs_path,
		const std::wstring & log_file_name);

	void threadCleanup();

	std::wstring get_log_path();
	std::wstring get_log_name_prefix();

	std::wstring to_elapsed_str(const std::chrono::steady_clock::time_point & start, const std::chrono::steady_clock::time_point & end);

	struct logging_settings_t
	{
		bool enable_developers_log;
		unsigned int max_log_buffer_size;
		std::wstring log_path;
		std::wstring log_name_prefix;
	};

	logging_settings_t current_logging_settings();
	void write_logging_settings(std::ostream & output_stream, const logging_settings_t& settings);

	bool developers_log_enabled();

	template <typename func_name_t, typename file_name_t, typename line_num_t>
	class scope_logger final
	{
	public:
		scope_logger(const func_name_t & func_name, const file_name_t & file_name, const line_num_t & line_num) :
			_start(std::chrono::steady_clock::now()), _func_name(func_name)
		{
			std::wostringstream ss;
			ss << "ENTER function: " << func_name << ", file: " << file_name << ", line: " << line_num;
			log_message(severity::debug, ss.str());
		}

		~scope_logger()
		{
			std::wostringstream ss;
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
		log_stream(severity sev): _severity(sev) {}
		~log_stream();

		template <typename T>
		log_stream & operator << (const T & ref)
		{
			_ss << ref;
			return *this;
		}

		log_stream(const log_stream &) = delete;
		log_stream operator = (const log_stream &) = delete;
	private:
		void flush();

		severity _severity;
		std::wostringstream _ss;
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
