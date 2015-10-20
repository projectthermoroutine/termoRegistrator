#pragma once

#include <vector>
#include <mutex>
#include <string>
#include <memory>
#include <atomic>
#include <thread>
#include <queue>
#include <common/string_utils.h>
#include <common/sync_helpers.h>
#include <loglib/log_file_writer.h>

#include "cpplogger.h"

namespace cpplogger 
{

	namespace details
	{
		struct log_file_info
		{
			wstring_t full_path;
			std::size_t backup_index;
		};

		class internal_logger_exception final : public std::exception
		{
		public:
			internal_logger_exception(const wstring_t &message)
				: std::exception(string_utils::convert_wchar_to_utf8(message).c_str())
			{
			}
		};

		class logger_interface
		{
		public:
			virtual ~logger_interface();

			virtual void initialize(const wstring_t &config,
				const wstring_t &log_path,
				const wstring_t &log_file_prefix,
				bool watch_config_changes,
				const notify_developers_logging_enabled_func_t & notify_developers_logging_enabled_func) = 0;

			virtual void reload_settings_from_config(const wstring_t & config) = 0;
			virtual void reload_settings_from_config() = 0;

			virtual logger_settings get_current_logger_settings() const = 0;
			virtual wstring_t get_config() const = 0;
			virtual wstring_t get_log_path() const = 0;
			virtual wstring_t get_log_prefix() const = 0;
			virtual void log_message(const log_level &level, const wstring_t &message) = 0;
		};

		/*Logger class declaration*/
		class Logger final : public logger_interface
		{

		public:
			Logger();
			~Logger();

			Logger(const Logger &) = delete;
			Logger &operator = (const Logger &) = delete;

			void initialize(const wstring_t &config,
				const wstring_t &log_path,
				const wstring_t &log_file_prefix,
				bool watch_config_changes = true,
				const logger::notify_developers_logging_enabled_func_t & notify_developers_logging_enabled_func = logger::notify_developers_logging_enabled_func_t()) override;

			void reload_settings_from_config(const wstring_t & config) override;
			void reload_settings_from_config() override;

			logger_settings get_current_logger_settings() const override;
			wstring_t get_config() const override;
			wstring_t get_log_path() const override;
			wstring_t get_log_prefix() const override;
			void log_message(const log_level &level, const wstring_t &message) override;

		private:
			/*logging methods*/
			wstring_t create_file_path(std::size_t backup_index);
			void create_log_file();
			void cleanup_stale_log();
			void rolling_files();
			void synchronize_log_file_directory();
			void write_message_to_file(const wstring_t &message);
			void clear_log_file_data();
			bool log_file_exist(std::size_t backup_index) const;

			void watch_config_change();

			logger_settings current_logger_settings_;
			wstring_t log_path_;
			wstring_t log_file_prefix_;
			std::queue<wstring_t> cached_exception_log_messages_;
			wstring_t _config;
			std::thread _config_watch_thread;
			handle_holder _stop_watching_event;
			FILETIME _last_config_write_time;
			logger::notify_developers_logging_enabled_func_t _notify_developers_logging_enabled_func;

			std::vector <log_file_info> log_files_;
			mutable std::recursive_mutex log_write_mutex_;
			mutable sync_helpers::rw_lock _config_lock;
			std::atomic <bool> flag_internal_error_;
			std::atomic <bool> flag_io_error_;
			logger::log_file_writer log_writer_;
			bool _watch_config_changes;
		};
		
		std::shared_ptr<logger_interface> get_logger_instance();
	}

}