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

#include "loglib/details/cpplogger.h"


namespace cpplogger 
{

	namespace details
	{

		class thread_handle
		{

		public:

			thread_handle();
			thread_handle(HANDLE handle);
			thread_handle(thread_handle&& other);
			~thread_handle();

			thread_handle& operator=(HANDLE handle);

			thread_handle& operator=(thread_handle&& other);

			thread_handle(const thread_handle& other) = delete;
			thread_handle& operator=(const thread_handle& other) = delete;

			void swap(thread_handle& other);

			void break_work();

		public: // Методы для вызова из рабочего потока для проверки на необходимость продолжения работы и уведомления о завершении

			bool wait_request_on_stopped(DWORD time_to_wait);

			bool is_stop_event_set();

		private:

            HANDLE m_stop_watching_event;
			HANDLE m_thread_handle;
		};

		class internal_logger_exception final : public std::exception
		{
		public:
            internal_logger_exception(const std::wstring &message)
				: std::exception(string_utils::convert_wchar_to_utf8(message).c_str())
			{
			}
		};

		class logger_interface
		{
		public:
			virtual ~logger_interface() = default;

            virtual void initialize(const std::wstring &config_path,
                const std::wstring &config_file_name,
                const std::wstring &log_path,
                const std::wstring &log_file_prefix,
                bool watch_settings_changes = true,
                const logger::use_developer_log_changed_f& = {}) = 0;

			virtual void deinitialize() = 0;

			virtual void reload_settings_from_config_file() = 0;
            
            virtual logger::settings get_current_logger_settings() const = 0;
            virtual std::wstring get_full_config_path() const = 0;
            virtual std::wstring get_log_path() const = 0;
            virtual std::wstring get_log_prefix() const = 0;
            virtual bool is_watch_settings_changes() const = 0;
            virtual void log_message(const cpplogger::message& msg) = 0;
    	};

		/*Logger class declaration*/
		class Logger final : public logger_interface
		{

		public:
			Logger();
			~Logger();

			Logger(const Logger &) = delete;
			Logger &operator = (const Logger &) = delete;

            void initialize(const std::wstring &config_path,
                const std::wstring &config_file_name,
                const std::wstring &log_path,
                const std::wstring &log_file_prefix,
                bool watch_settings_changes,
                const logger::use_developer_log_changed_f & use_developer_log_changed) override;

			virtual void deinitialize() override;

			void reload_settings_from_config_file() override;

            logger::settings get_current_logger_settings() const override;
            std::wstring get_full_config_path() const override;
            std::wstring get_log_path() const override;
            std::wstring get_log_prefix() const override;
            bool is_watch_settings_changes() const override;
            void log_message(const message& msg) override;

		private:

			static DWORD WINAPI watch_config_change_win32(_In_ LPVOID lpParameter);
			void watch_config_change();

            logger::settings current_logger_settings_;
            std::wstring full_config_path_;
            std::wstring _config_path;
            std::wstring _config_file_name;
			FILETIME _last_config_write_time;
            logger::use_developer_log_changed_f _use_developer_log_changed;

			mutable sync_helpers::rw_lock _config_lock;
			bool _watch_config_changes;

			thread_handle _config_watch_thread_handle;

			std::unique_ptr<filestream> _filestream;
		};
	}

}