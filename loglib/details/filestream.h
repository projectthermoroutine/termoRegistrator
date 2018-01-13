
#pragma once


//#ifndef LOG_FILESTREAM_THREAD_USING
//#   define LOG_FILESTREAM_THREAD_USING
//#endif // LOG_FILESTREAM_THREAD_USING


#include <list>
#include <mutex>
#include <string>
#include <thread>
#include <condition_variable>

#include <common\sync_helpers.h>

#include <loglib\log_file_writer.h>


namespace cpplogger
{
    struct message
    {
        logger::level log_level;
        std::size_t scope_level;
        std::wstring text;

        unsigned long thread_id;
        std::chrono::system_clock::time_point time;

        static message make(std::wstring&& text);
        static message make(const std::wstring& text);
        static message make(logger::level log_level, std::wstring&& text);
        static message make(logger::level log_level, const std::wstring& text);
        static message make(logger::level log_level, std::size_t scope_level, std::wstring&& text);
        std::wstring text_with_apply_format(bool print_thread_id = true, bool apply_tabs_body = true) const;
    };


	namespace details
	{

		/** Класс по работе с файлом лога в отдельном потоке */
		class filestream
		{
		public:
			enum class flush_mode
			{
				after_roll = 0,
				per_message,
			};

		private:

			enum class wait_result
			{
				success = 0,
				break_work,
			};

			struct file_info
			{
				std::wstring full_path;
				std::size_t backup_index;
			};

		public: // Методы для вызова из сторонних потоков

			filestream(const std::wstring& path, const std::wstring& file_prefix);
			~filestream();

			filestream(filestream&&) = delete;
			filestream(const filestream&) = delete;

			filestream& operator=(filestream&&) = delete;
			filestream& operator=(const filestream&) = delete;

			std::wstring path() const;

			std::wstring file_prefix() const;

			void set_settings(logger::settings&& settings);
			void push_message(cpplogger::message&& msg);
            void push_message(cpplogger::message&& msg, logger::settings&& settings);

		public:
			void set_flush_mode(flush_mode);
			filestream& operator<<(flush_mode);

#ifdef LOG_FILESTREAM_THREAD_USING
            void break_work();
#else       
            inline void break_work() {}
#endif // LOG_FILESTREAM_THREAD_USING

        private:

#ifdef LOG_FILESTREAM_THREAD_USING
            wait_result wait_and_take_messages(std::list<cpplogger::message>& msgs, logger::settings& settings);

            static DWORD WINAPI run_win32(_In_ LPVOID lpParameter);
			void run();
#endif // LOG_FILESTREAM_THREAD_USING

            void write(const cpplogger::message& msg, const logger::settings& settings);

			void create_new_file_and_open_to_write();

			std::wstring create_file_path(std::size_t backup_index) const;

			bool file_exist(std::size_t backup_index) const;

			void clear_file_data();

            void rolling_files(const logger::settings& settings);

			void cleanup_stale_log();

			bool remove_file(const std::wstring& filename) const;

			bool rename_file(const std::wstring& old_filename, const std::wstring& new_filename) const;

			std::size_t get_number_from_end(std::wstring& str, std::size_t default_number) const;

            void synchronize_log_file_directory(const logger::settings& settings);

            bool write_message_to_file(const std::wstring& text, bool* need_apply_rolling);

		private:

			const std::wstring m_path;
			const std::wstring m_file_prefix;

			std::list<file_info> m_files_info;

			logger::log_file_writer m_writer;
			flush_mode m_flush_mode;

            std::list<cpplogger::message> m_no_recorded_messages;
            std::size_t m_number_irretrievably_lost_messages;

		private:

            logger::settings m_settings;
            bool m_settings_changed;
#ifdef LOG_FILESTREAM_THREAD_USING
            bool m_break_work;

			struct
			{
                sync_helpers::mutex_crit_section access;
                sync_helpers::condition_variable condition;

                std::list<cpplogger::message> list;

			} m_messages;

            HANDLE m_finished_handle;
            HANDLE m_thread_handle;
#else
            std::mutex m_writer_access;

#endif // LOG_FILESTREAM_THREAD_USING
        };
	} // namespace details

} // namespace cpplogger