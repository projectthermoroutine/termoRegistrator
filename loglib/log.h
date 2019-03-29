#pragma once

#include <chrono>
#include <deque>
#include <locale>
#include <cstdint>
#include <sstream>
#include <functional>
#include <string_view>

#include <loglib/types.h>

namespace logger
{
	void __cdecl log_message(level, const std::wstring &);

	bool initialize(
		const std::wstring &settings_path,
		const std::wstring &settings_file_name,
		const std::wstring &log_path,
		const std::wstring &log_file_prefix,
		bool watch_config_changes,
		const use_developer_log_changed_f & use_developer_log_changed);

	void deinitialize() noexcept;

	void set_settings(logger::settings&& settings);
	void reset_settings_to_default();
	logger::settings get_settings();

	std::wstring get_log_path();
	std::wstring get_log_name_prefix();

	std::string except_what_to_string(std::exception_ptr except, std::string* category = nullptr);
	std::wstring except_what_to_wstring(std::exception_ptr except, std::wstring* category = nullptr);

	std::deque<std::string> chain_of_except_what_to_strings(std::exception_ptr except, std::deque<std::string>* categories = nullptr);
	std::deque<std::wstring> chain_of_except_what_to_wstrings(std::exception_ptr except, std::deque<std::wstring>* categories = nullptr);

	std::wstring log_exception(std::exception_ptr except, logger::level level, const std::wstring & head_message);
	std::wstring log_current_exception(logger::level level, const std::wstring & head_message);
	void try_catchs_to_log(const std::function<void()>& func, logger::level level, const std::wstring & head_message);

	struct instance_t;
	const instance_t& logger_instance();
	[[nodiscard]] bool set_logger_instance(const instance_t&) noexcept;

	class scope_logger final
	{
	public:

		static std::wstring make_str_elapsed_time(const std::chrono::steady_clock::time_point& time);
		static std::wstring make_str_delta_time(const std::chrono::microseconds& delta);

	public:

		scope_logger(std::wstring_view func_name, std::wstring_view file_name, int line_number);
		~scope_logger();

		scope_logger(const scope_logger&) = delete;
		scope_logger& operator=(const scope_logger&) = delete;

	private:

		const std::wstring_view m_func_name;
		std::chrono::steady_clock::time_point m_start;
	};

	class scope_logger_ex
	{
	public:

		scope_logger_ex(std::wstring&& scope_name, std::wstring_view file_name, int line_number);
		scope_logger_ex(const std::wstring& scope_name, std::wstring_view file_name, int line_number);
		scope_logger_ex(const std::string& scope_name, std::wstring_view file_name, int line_number);
		~scope_logger_ex();

		scope_logger_ex(scope_logger_ex&&);

		scope_logger_ex(const scope_logger_ex&) = delete;
		scope_logger_ex& operator=(const scope_logger_ex&) = delete;

	private:

		std::wstring m_scope_name;
		std::chrono::steady_clock::time_point m_start;
	};

	class log_stream
	{
	public:

		log_stream(level sev);
		~log_stream();

		log_stream & operator<<(char ref);
		log_stream & operator<<(const char* ref);
		log_stream & operator<<(const std::string& ref);
		log_stream & operator<<(std::string_view str_view);

		template <typename T>
		inline log_stream & operator<<(const T& ref)
		{
			_ss << ref;
			return *this;
		}

		inline std::wstring str() const
		{
			return _ss.str();
		}

		log_stream(const log_stream &) = delete;
		log_stream operator = (const log_stream &) = delete;
	private:
		void flush();

		level _lvl;
		std::wostringstream _ss;
	};

} // namespace logger


using namespace std::string_view_literals;

#define ADD_SUFFIX(x,suffix) x ## suffix
#define ADD_SV_SUFFIX(x) ADD_SUFFIX(x,sv)
#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define WIDE_SV(x) ADD_SV_SUFFIX(WIDE1(x))

#define LOG(sev) ::logger::log_stream(sev)

#define LOG_TRACE() LOG(::logger::level::trace)
#define LOG_DEBUG() LOG(::logger::level::debug)
#define LOG_INFO() LOG(::logger::level::info)
#define LOG_WARN() LOG(::logger::level::warn)
#define LOG_ERROR() LOG(::logger::level::error)
#define LOG_FATAL() LOG(::logger::level::fatal)

#define __LOG_STACK_JOIN2(param1, param2) param1 ## param2
#define __LOG_STACK_JOIN(param1, param2) __LOG_STACK_JOIN2(param1, param2)
#define LOG_STACK() const ::logger::scope_logger __LOG_STACK_JOIN(logger_visiter_function_on_line_, __LINE__)(WIDE_SV(""__FUNCTION__""), WIDE_SV(__FILE__), __LINE__)
#define LOG_STACK_EX(scope_name) const ::logger::scope_logger_ex __LOG_STACK_JOIN(logger_visiter_function_on_line_, __LINE__)((scope_name), WIDE_SV(__FILE__), __LINE__)
