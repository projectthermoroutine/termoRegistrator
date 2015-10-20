#pragma once

#include <cstdint>
#include <string>
#include <functional>

namespace cpplogger
{
	using wstring_t = std::wstring;
	using string_t = std::string;

	enum class log_level : unsigned int
	{
		trace = 0,
		debug = 1,
		info = 2,
		warn = 3,
		error = 4,
		fatal = 5
	};

	struct logger_settings
	{
		bool use_developer_log;
		log_level level;
		std::size_t max_backup_index;
		std::uint64_t max_file_size;
		std::uint64_t max_buffer_size;

		const bool operator==(const logger_settings& op) const
		{
			return
				use_developer_log == op.use_developer_log &&
				level == op.level &&
				max_backup_index == op.max_backup_index &&
				max_file_size == op.max_file_size &&
				max_buffer_size == op.max_buffer_size;
		}

		const bool operator!=(const logger_settings& op) const
		{
			return !this->operator==(op);
		}
	};

	using notify_developers_logging_enabled_func_t = std::function < void(bool) > ;

	void initialize(const wstring_t &config,
		const wstring_t &log_path,
		const wstring_t &log_file_prefix,
		bool watch_config_changes,
		const notify_developers_logging_enabled_func_t & notify_developers_logging_enabled_func);

	void free_logger_instance();

	void reload_settings_from_config(const wstring_t & config);
	void reload_settings_from_config();

	/* Read/write logger settings*/
	logger_settings read_logger_settings(const wstring_t &config);
	logger_settings get_default_logger_settings();
	logger_settings get_current_logger_settings();
	void write_logger_settings(const wstring_t &full_file_path, const logger_settings &settings);


	using validate_report_func_t = std::function < void(const std::wstring & param_name, const std::wstring & original_value, const std::wstring & modified_value) > ;

	void validate_logger_settings(logger_settings & settings, const validate_report_func_t & validate_report_func);
	void validate_logger_settings(const logger_settings &settings, const wstring_t &config);
	
	void check_and_compress_history_logs(const wstring_t &log_path,
		const wstring_t &log_file_prefix,
		const logger_settings &settings);

	wstring_t get_config_value_by_log_level(log_level level);
	log_level get_log_level_by_config_value(const string_t &val);

	void log_message(const log_level &level, const wstring_t &message);
	wstring_t get_log_path();
	wstring_t get_log_prefix();
	wstring_t get_config();
}