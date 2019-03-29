#pragma once

#include <cstdint>
#include <string>
#include <functional>

#include <loglib/types.h>

#include "loglib/details/filestream.h"


namespace cpplogger
{
    struct prefix_function
    {
        static const std::size_t lenght;
        static const std::wstring entry;
        static const std::wstring entry_exception;
        static const std::wstring exit;
        static const std::wstring exit_exception;
    };


    bool initialize(const std::wstring &config_path,
        const std::wstring &config_file_name,
        const std::wstring &log_path,
        const std::wstring &log_file_prefix,
		bool watch_config_changes,
        const logger::use_developer_log_changed_f & use_developer_log_changed);


	void free_logger_instance() noexcept;

	/* Read/write logger settings*/
    logger::settings read_logger_settings(const std::wstring &full_config_path);
	logger::settings get_default_logger_settings();
	logger::settings get_current_logger_settings();
    void reset_settings(logger::settings&& settings);
    void write_logger_settings(const std::wstring &full_file_path, const logger::settings &settings);
	void reload_settings_from_config_file();

    void validate_logger_settings(logger::settings &settings, const std::wstring &full_config_path);
	
    void check_and_compress_history_logs(const std::wstring &log_path,
        const std::wstring &log_file_prefix,
		const logger::settings &settings);

    std::wstring get_config_value_by_log_level(logger::level sev);
    logger::level get_log_level_by_config_value(const std::wstring &val);

    void log_message(const cpplogger::message& msg);
    std::wstring get_log_path();
    std::wstring get_log_prefix();
    std::wstring get_full_config_path();

} // namespace cpplogger