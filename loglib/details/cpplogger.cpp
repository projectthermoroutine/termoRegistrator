#include <Windows.h>
#include <common/path_helpers.h>
#include <common/date_helpers.h>
#include <common/sync_helpers.h>
#include <common/on_exit.h>
#include <common/fs_utils.h>
#include <sstream>
#include <cstdio>
#include <pugixml.hpp>
#include <set>
#include <regex>
#include <map>
#include <unordered_map>
#include <array>
#include <mutex>
#include <future>
#include <memory>
#include <cassert>

#include <error_lib/win32_error.h>
#include <error_lib/error_codes.h>
#include <error_lib/application_exception.h>
#include <common/log_and_throw.h>

#include "loglib/details/zipper.h"
#include "loglib/details/cpplogger_details.h"


namespace cpplogger
{
	namespace
	{

        class instance
        {
            class fake_impl;

        public:

			~instance()
			{
				//                                   ret  noop  noop  noop  noop
				const std::uint8_t _asm_return[] = {0xC3, 0x90, 0x90, 0x90, 0x90};
				set_dummy_on<reinterpret_cast<void*>(::logger::log_message), sizeof(_asm_return)>(_asm_return);
			}

            using pointer = std::shared_ptr < details::logger_interface > ;

            template< class impl_logger_t, class... init_args_t> inline
            std::enable_if_t<std::is_base_of<details::logger_interface, impl_logger_t>::value, bool>
            set(init_args_t&&... init_args);

            inline void reset();

            inline pointer get();

        private:

            inline pointer get_lk_read();

            inline pointer get_lk_read_write();

			template <void* _Address, std::size_t _Size>
			static bool set_dummy_on(const std::uint8_t asm_code[_Size])
			{
				void* const address(_Address);
				std::size_t const size(_Size);

				DWORD protect(PAGE_READWRITE);
				if (0 == ::VirtualProtect(address, size, protect, &protect))
					return false;

				__try 
				{
					__try 
					{
						std::memcpy(address, asm_code, size);
					}
					__finally 
					{
						::VirtualProtect(address, size, protect, &protect);
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER) {}


				return true;
			}

		private:

            sync_helpers::rw_lock m_init_access;
            sync_helpers::rw_lock m_access;
            pointer m_logger;
            bool m_real_impl;
        };
        

        instance g_instance;

        const logger::level log_level_default(logger::level::trace);

        auto const use_developer_log_key = "use_developer_log";
        auto const level_key = "level";
        auto const max_backup_index_key = "max_backup_index";
        auto const max_file_size_key = "max_file_size";
        auto const max_buffer_size_key = "max_buffer_size";
        auto const config_file_root_tag = "log_settings";
        auto const developer_log_tag = "developer_log";
        auto const history_log_tag = "history_log";

        auto const max_config_file_size = 1024 * 1024;

        const logger::settings default_logger_settings([]
        {
            logger::settings settings;
            settings.use_developer_log = false;
            settings.level = log_level_default;
            settings.max_buffer_size = 1048576;
            settings.max_file_size = 1048576;
            settings.max_backup_index = 4;

            return settings;
        }());

        /*help methods*/
		static void output_debug_print(const std::wstring &message)
		{
			OutputDebugStringW((message + L'\n').c_str());
		}

        static std::uint64_t get_file_size(const std::wstring &full_path)
		{
			WIN32_FILE_ATTRIBUTE_DATA fad;

			if (!GetFileAttributesExW(full_path.c_str(), GetFileExInfoStandard, &fad))
			{
				const auto last_error = GetLastError();
				output_debug_print(L"GetFileAttributesExW failed with error:" + std::to_wstring(last_error));
				return 0;
			}

			LARGE_INTEGER file_size;
			file_size.LowPart = fad.nFileSizeLow;
			file_size.HighPart = fad.nFileSizeHigh;

			if (file_size.QuadPart < 0)
			{
				output_debug_print(L"cpplogger get_file_size: Negative file size exception.");
				return 0;
			}

			return file_size.QuadPart;

		}

        void for_each_fs_object(const std::wstring& path, const std::function<void(const std::wstring &)>& func)
		{
#ifndef _WIN64
            // make guard WOW64 FS redirect
            void* old_value_wow64_redirect(nullptr);
            const std::unique_ptr<void, std::function<void(void*)>> wow64_redirect_guard(
            [&]()
            {
                return reinterpret_cast<void*>(::Wow64DisableWow64FsRedirection(&old_value_wow64_redirect));
            }(),
            [&](void* is_ok_wow64_redirect)
            {
                if (FALSE != reinterpret_cast<BOOL>(is_ok_wow64_redirect)) ::Wow64RevertWow64FsRedirection(old_value_wow64_redirect);
            });
#endif // NOT _WIN64

            std::wstring log_message_prefix(L"FindFirstFile failed with error: ");

			WIN32_FIND_DATAW find_data;
            const HANDLE find_handle(::FindFirstFileW(path_helpers::concatenate_paths(path, L"*").c_str(), &find_data));
            if (find_handle != nullptr && find_handle != INVALID_HANDLE_VALUE)
            {
                // make guard valid handle
                const std::unique_ptr<void, std::function<void(void*)>> find_handle_guard(reinterpret_cast<void*>(find_handle), [=](void*) { ::FindClose(find_handle); });

                // skipped sub-dirs
                const std::set<std::wstring> all_dirs_to_skip{L".", L".."};

                log_message_prefix = L"FindNextFile failed with error: ";

                do
                {
                    if (std::end(all_dirs_to_skip) == all_dirs_to_skip.find(find_data.cFileName))
                    {
                        func(path_helpers::concatenate_paths(path, find_data.cFileName));
                    }
                }
                while (FALSE != FindNextFileW(find_handle, &find_data));
            }

            const DWORD last_error(::GetLastError());
            if (last_error != ERROR_FILE_NOT_FOUND && last_error != ERROR_NO_MORE_FILES)
            {
                assert(last_error != ERROR_SUCCESS);

                std::wostringstream error;
                error << log_message_prefix << std::hex << std::showbase << last_error;
                output_debug_print(error.str());
            }
		}

        std::tuple <std::wstring, std::wstring, std::wstring, std::wstring> split_path(std::wstring file_full_path)
		{
			const auto long_path_prefix = std::wstring{ L"\\\\?\\" };
			const bool is_long_path = file_full_path.compare(0, long_path_prefix.size(), long_path_prefix) == 0;

			if (is_long_path)
				file_full_path.erase(std::begin(file_full_path), std::begin(file_full_path) + long_path_prefix.size());

			const auto buffer_size = std::size_t{ 32768 };

			auto drive_name_buf = std::unique_ptr < wchar_t[] > { new wchar_t[static_cast<std::size_t>(buffer_size)] };
			ZeroMemory(drive_name_buf.get(), buffer_size*sizeof(drive_name_buf[0]));
			auto dir_name_buf = std::unique_ptr < wchar_t[] > { new wchar_t[static_cast<std::size_t>(buffer_size)] };
			ZeroMemory(dir_name_buf.get(), buffer_size*sizeof(dir_name_buf[0]));
			auto file_name_buf = std::unique_ptr < wchar_t[] > { new wchar_t[static_cast<std::size_t>(buffer_size)] };
			ZeroMemory(file_name_buf.get(), buffer_size*sizeof(file_name_buf[0]));
			auto extension_buf = std::unique_ptr < wchar_t[] > { new wchar_t[static_cast<std::size_t>(buffer_size)] };
			ZeroMemory(extension_buf.get(), buffer_size*sizeof(extension_buf[0]));

			auto const split_result = _wsplitpath_s(
				file_full_path.c_str(),
				drive_name_buf.get(),
				buffer_size,
				dir_name_buf.get(),
				buffer_size,
				file_name_buf.get(),
				buffer_size,
				extension_buf.get(),
				buffer_size);

			if (split_result != 0)
				throw std::runtime_error("Could not split path: " + string_utils::convert_wchar_to_utf8(file_full_path));

			auto drive_name = std::wstring{ drive_name_buf.get() };
			auto dir_name = std::wstring{ dir_name_buf.get() };
			auto file_name = std::wstring{ file_name_buf.get() };
			auto file_ext = std::wstring{ extension_buf.get() };

			if (drive_name.empty() && dir_name.empty() && file_name.empty() && file_ext.empty())
				throw std::runtime_error("Could not split path: " + string_utils::convert_wchar_to_utf8(file_full_path) + ";result is empty");

			if (is_long_path && !drive_name.empty())
				drive_name = long_path_prefix + drive_name;

			return std::make_tuple(std::move(drive_name), std::move(dir_name), std::move(file_name), std::move(file_ext));
		}

		static bool remove_log_file(const std::wstring &filename)
		{
			int result = _wremove(filename.c_str());

			return result != 0 ? false : true;
		}

        bool file_exists(const std::wstring &file_full_path)
		{
			if (file_full_path.empty())
				return false;

			DWORD attributes = GetFileAttributesW(file_full_path.c_str());

			return (attributes != INVALID_FILE_ATTRIBUTES) && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
		}

		FILETIME get_file_last_write_time(const std::wstring & path)
		{
			if (path.empty())
				throw std::runtime_error("Invalid argument path were passed to the get_file_last_write_time function.");

			HANDLE file_handle = CreateFileW(
				path.c_str(),
				GENERIC_READ,
				FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				HANDLE());

			if (file_handle == INVALID_HANDLE_VALUE)
			{
				const auto last_error = GetLastError();
				throw std::runtime_error("CreateFileW failed with error: " + std::to_string(last_error));
			}

			FILETIME last_write_time{};
			if (!GetFileTime(file_handle, NULL, NULL, &last_write_time))
			{
				CloseHandle(file_handle);
				const auto last_error = GetLastError();
				throw std::runtime_error("GetFileTime failed for path: " + string_utils::convert_wchar_to_utf8(path) + ". Error: " + std::to_string(last_error));
			}

			CloseHandle(file_handle);
			return last_write_time;
		}


        SYSTEMTIME get_file_system_datetime(const std::wstring &file_full_path)
		{
			WIN32_FILE_ATTRIBUTE_DATA fad;
			SYSTEMTIME sys_time;

			if (!GetFileAttributesExW(file_full_path.c_str(), GetFileExInfoStandard, &fad))
			{
				const auto last_error = GetLastError();
				output_debug_print(L"GetFileAttributesExW failed with error:" + std::to_wstring(last_error));
				throw std::runtime_error("GetFileAttributesExW failed in cpplogger::get_file_system_datetime.");
			}

			if (!FileTimeToSystemTime(&fad.ftLastWriteTime, &sys_time))
			{
				const auto last_error = GetLastError();
				output_debug_print(L"FileTimeToSystemTime failed with error:" + std::to_wstring(last_error));
				throw std::runtime_error("FileTimeToSystemTime failed in cpplogger::get_file_system_datetime.");
			}

			return sys_time;
		}
	
    } // namespace anonymous

    namespace details
    {
        const DWORD config_watch_period = 3000; // 3 sec

        thread_handle::thread_handle()
            : m_stop_watching_event(nullptr)
            , m_thread_handle(nullptr)
        {}

        thread_handle::thread_handle(HANDLE handle)
            : m_stop_watching_event(handle == nullptr || handle == INVALID_HANDLE_VALUE ? nullptr : ::CreateEventW(NULL, FALSE, FALSE, NULL))
            , m_thread_handle(handle)
        {}

        thread_handle::thread_handle(thread_handle&& other)
            : m_stop_watching_event(nullptr)
            , m_thread_handle(nullptr)
        {
            this->swap(other);
        }

        thread_handle::~thread_handle()
        {
            if (m_thread_handle == nullptr || m_thread_handle == INVALID_HANDLE_VALUE)
                return;

            assert(m_stop_watching_event);

            this->break_work();

            ::WaitForSingleObject(m_thread_handle, INFINITE);
            ::CloseHandle(m_thread_handle);
        }

        thread_handle& thread_handle::operator=(HANDLE handle)
        {
            thread_handle tmp(handle);
            tmp.swap(*this);

            return *this;
        }

        thread_handle& thread_handle::operator=(thread_handle&& other)
        {
            thread_handle tmp;
            tmp.swap(other);
            tmp.swap(*this);

            return *this;
        }

        void thread_handle::swap(thread_handle& other)
        {
            std::swap(m_stop_watching_event, other.m_stop_watching_event);
            std::swap(m_thread_handle, other.m_thread_handle);
        }

        void thread_handle::break_work()
        {
            if (m_stop_watching_event == nullptr)
                return;

            if (FALSE == ::SetEvent(m_stop_watching_event))
            {
                ::OutputDebugStringA("Error set event stop watching");
            }

            ::SwitchToThread();
        }

        bool thread_handle::wait_request_on_stopped(DWORD time_to_wait)
        {
            if (m_thread_handle == nullptr || m_thread_handle == INVALID_HANDLE_VALUE)
                throw common::application_exception(common::result_code::passed_argument_event_cant_be_invalid_handle_value);

            assert(m_stop_watching_event);

            switch (::WaitForSingleObject(m_stop_watching_event, time_to_wait))
            {
            case WAIT_OBJECT_0:
                return true;
            case WAIT_TIMEOUT:
            case WAIT_ABANDONED:
                return false;
            }

            throw win32::exception::by_last_error("WaitForSingleObject failed");
        }

        bool thread_handle::is_stop_event_set()
        {
            return this->wait_request_on_stopped(0);
        }


        Logger::Logger()
            : current_logger_settings_(default_logger_settings)
            , _watch_config_changes(false)
            , _last_config_write_time()
            , _config_watch_thread_handle()
        {}

        Logger::~Logger()
        {}

        void Logger::initialize(const std::wstring &config_path,
            const std::wstring &config_file_name,
            const std::wstring &log_path,
            const std::wstring &log_file_prefix,
            bool watch_config_changes,
            const logger::use_developer_log_changed_f & use_developer_log_changed)
        {
            const std::wstring tmp_full_config_path = path_helpers::concatenate_paths(config_path, config_file_name);

            logger::settings current_logger_settings(default_logger_settings);
            decltype(_last_config_write_time) last_config_write_time{};

            try
            {
                current_logger_settings = read_logger_settings(tmp_full_config_path);
                last_config_write_time = fs_utils::get_file_last_write_time(tmp_full_config_path);
            }
            catch (const std::exception & exc)
            {
                output_debug_print(L"Could not read logger settings. Error: " + string_utils::convert_utf8_to_wchar(exc.what()));
            }

            if (current_logger_settings != default_logger_settings)
            {
                validate_logger_settings(current_logger_settings, tmp_full_config_path);
            }

            check_and_compress_history_logs(log_path, log_file_prefix, current_logger_settings);

            std::promise<Logger*> promise_this_init;
            std::unique_ptr<std::future<Logger*>> future_this_init(std::make_unique<std::future<Logger*>>(promise_this_init.get_future()));

            thread_handle config_watch_thread_handle(watch_config_changes
                ? ::CreateThread(nullptr, 64 * 1024, &Logger::watch_config_change_win32, future_this_init.get(), (STACK_SIZE_PARAM_IS_A_RESERVATION), nullptr)
                : nullptr);

            future_this_init.release();

            std::unique_ptr<details::filestream> filestream(std::make_unique<details::filestream>(log_path, log_file_prefix));

            bool use_developer_log = false;
            {
                sync_helpers::rw_lock_guard_exclusive lock(_config_lock);
                _config_watch_thread_handle = std::move(config_watch_thread_handle);
                _filestream = std::move(filestream);
                _config_path = config_path;
                _config_file_name = config_file_name;
                _watch_config_changes = watch_config_changes;
                current_logger_settings_ = std::move(current_logger_settings);
                full_config_path_ = std::move(tmp_full_config_path);
                _last_config_write_time = last_config_write_time;
                _use_developer_log_changed = use_developer_log_changed;
                use_developer_log = current_logger_settings_.use_developer_log;
            }

            {
                if (_use_developer_log_changed && use_developer_log)
                {
                    _use_developer_log_changed(true);
                }
            }

            promise_this_init.set_value(this);
        }

        void Logger::deinitialize()
        {
            _config_watch_thread_handle.break_work();

            if (_filestream)
                _filestream->break_work();
        }

        void Logger::reload_settings_from_config_file()
        {
            std::wstring full_config_path;
            logger::settings new_logger_settings;
            logger::settings current_logger_settings;

            {
                sync_helpers::rw_lock_guard_shared lock(_config_lock);
                current_logger_settings = current_logger_settings_;
                full_config_path = full_config_path_;
            }

            try
            {
                new_logger_settings = read_logger_settings(full_config_path);
            }
            catch (const details::internal_logger_exception & exc)
            {
                output_debug_print(L"Could not read logger settings. Error: " + string_utils::convert_utf8_to_wchar(exc.what()));
                return;
            }

            if (new_logger_settings == current_logger_settings)
            {
                output_debug_print(L"Settings read from configuration file does not differ from the current settings.");
                return;
            }

            if (new_logger_settings != default_logger_settings)
            {
                validate_logger_settings(new_logger_settings, full_config_path);
            }

            bool developers_logs_flag_changed = false;
            bool use_developer_log = false;
            logger::use_developer_log_changed_f use_developer_log_changed;

            {
                sync_helpers::rw_lock_guard_exclusive lock(_config_lock);
                developers_logs_flag_changed = new_logger_settings.use_developer_log != current_logger_settings_.use_developer_log;
                full_config_path_ = full_config_path;
                current_logger_settings_ = std::move(new_logger_settings);
                use_developer_log = current_logger_settings_.use_developer_log;
                use_developer_log_changed = _use_developer_log_changed;
            }

            {
                if (developers_logs_flag_changed && use_developer_log_changed)
                {
					if (_filestream) {
						*_filestream << (use_developer_log ? filestream::flush_mode::after_roll : filestream::flush_mode::per_message);
					}
                    use_developer_log_changed(use_developer_log);
                }
            }
        }

        logger::settings Logger::get_current_logger_settings() const
        {
            sync_helpers::rw_lock_guard_shared lock(_config_lock);
            return current_logger_settings_;
        }

        std::wstring Logger::get_full_config_path() const
        {
            sync_helpers::rw_lock_guard_shared lock(_config_lock);
            return full_config_path_;
        }

        std::wstring Logger::get_log_path() const
        {
            return _filestream ? _filestream->path() : std::wstring();
        }

        std::wstring Logger::get_log_prefix() const
        {
            return _filestream ? _filestream->file_prefix() : std::wstring();
        }

        bool Logger::is_watch_settings_changes() const
        {
            return _watch_config_changes;
        }

        void Logger::log_message(const cpplogger::message& msg)
        {
            logger::settings logger_settings(get_current_logger_settings());
            if (logger_settings.level > msg.log_level)
                return;

            if (_filestream)
                _filestream->push_message(cpplogger::message(msg), std::move(logger_settings));
        }

        DWORD WINAPI Logger::watch_config_change_win32(_In_ LPVOID lpParameter)
        {
            std::unique_ptr<std::future<Logger*>> future_this_init(static_cast<std::future<Logger*>*>(lpParameter));

            Logger* this_logger(nullptr);

            try
            {
                this_logger = future_this_init->get();
            }
            catch (const std::future_error&)
            {
                // Конструирование объекта было прервано
                return 1;
            }

            try
            {
                this_logger->watch_config_change();
                return 0;
            }
            catch (const std::exception & exc)
            {
                output_debug_print(string_utils::convert_utf8_to_wchar(exc.what()));
                return 2;
            }
            catch (...)
            {
                output_debug_print(L"An unknown exception was caught.");
                return 3;
            }
        }

        void Logger::watch_config_change()
        {
            if (_config_watch_thread_handle.is_stop_event_set())
                return;

            for (;;)
            {
                decltype(full_config_path_) full_config_path;
                decltype(_last_config_write_time) last_config_write_time;

                {
                    sync_helpers::rw_lock_guard_shared lock(_config_lock);
                    last_config_write_time = _last_config_write_time;
                    full_config_path = full_config_path_;
                }

                if (!full_config_path.empty() && file_exists(full_config_path))
                {
                    try
                    {
                        const auto last_write_time = get_file_last_write_time(full_config_path);

                        if (CompareFileTime(&last_write_time, &last_config_write_time) != 0)
                        {
                            reload_settings_from_config_file();

                            {
                                sync_helpers::rw_lock_guard_exclusive lock(_config_lock);
                                _last_config_write_time = last_write_time;
                            }
                        }
                    }
                    catch (const std::exception & exc)
                    {
                        output_debug_print(string_utils::convert_utf8_to_wchar(exc.what()));
                    }
                }

                if (_config_watch_thread_handle.wait_request_on_stopped(config_watch_period))
                    break;
            }
        }

    } // namespace details


    const std::size_t prefix_function::lenght(4);
    const std::wstring prefix_function::entry(L"--->");
    const std::wstring prefix_function::entry_exception(L"exc>");
    const std::wstring prefix_function::exit(L"<---");
    const std::wstring prefix_function::exit_exception(L"<exc");

    message message::make(std::wstring&& text)
    {
        return make(logger::level::trace, 0, std::move(text));
    }
    
    message message::make(const std::wstring& text)
    {
        return make(logger::level::trace, 0, std::wstring(text));
    }

    message message::make(logger::level log_level, std::wstring&& text)
    {
        return make(log_level, 0, std::move(text));
    }

    message message::make(logger::level log_level, const std::wstring& text)
    {
        return make(log_level, 0, std::wstring(text));
    }

    message message::make(logger::level log_level, std::size_t scope_level, std::wstring&& text)
    {
        cpplogger::message message;

        message.log_level = log_level;
        message.scope_level = scope_level;
        message.text = std::move(text);

        message.thread_id = ::GetCurrentThreadId();
        message.time = std::chrono::system_clock::now();

        return message;
    }

    std::wstring message::text_with_apply_format(bool print_thread_id, bool apply_tabs_body) const
    {
        const cpplogger::message& message(*this);

        std::wostringstream lines;
        lines << date_helpers::local_time_to_str(std::chrono::system_clock::to_time_t(message.time), "%Y/%m/%d %H:%M:%S").c_str();

        if (print_thread_id)
            lines << ' ' << message.thread_id;

        lines << ' ' << logger::log_level_to_string(message.log_level);
        lines << std::wstring(prefix_function::lenght * message.scope_level + 1, L' ');

        std::wstring msg_text(message.text);

        if (apply_tabs_body)
        {
            std::wstring::size_type index(msg_text.find_first_of(L'\n'));
            if (index != std::wstring::npos)
            {
                const std::size_t prefix_line_length(lines.str().length());
                std::wstring prefix_line(prefix_line_length, L' ');

                for (; index != std::wstring::npos; index = msg_text.find_first_of(L'\n', index + prefix_line_length))
                {
                    index += 1;
					msg_text.insert(index, prefix_line);
                }
            }
        }

        lines << msg_text;

        return lines.str();
    }

    bool initialize(const std::wstring &config_path,
        const std::wstring &config_file_name,
        const std::wstring &log_path,
        const std::wstring &log_file_prefix,
		bool watch_config_changes,
        const logger::use_developer_log_changed_f & use_developer_log_changed)
	{
        return g_instance.set<details::Logger>(config_path, config_file_name, log_path, log_file_prefix, watch_config_changes, use_developer_log_changed);
	}

    void log_message(const cpplogger::message& msg)
	{
        g_instance.get()->log_message(msg);
	}

    std::wstring get_full_config_path()
	{
        return g_instance.get()->get_full_config_path();
	}

    std::wstring get_log_path()
	{
        return g_instance.get()->get_log_path();
	}

    std::wstring get_log_prefix()
	{
        return g_instance.get()->get_log_prefix();
	}

    logger::settings get_current_logger_settings()
	{
        return g_instance.get()->get_current_logger_settings();
	}

    logger::settings get_default_logger_settings()
	{
		return default_logger_settings;
	}

	template <typename xml_item_type_t>
	bool check_xml_item_not_exist(const xml_item_type_t &item)
	{
		return item.empty() ? true : false;
	}

#define CHECK_XML_ITEM(item) { if(check_xml_item_not_exist(item)) return default_logger_settings; }

    logger::settings read_logger_settings(const std::wstring &full_config_path)
	{
        logger::settings tmp_settings = default_logger_settings;
		pugi::xml_document doc;

		if (get_file_size(full_config_path) > max_config_file_size)
		{
			output_debug_print(L"Very big config file: " + full_config_path + L", apply default logger settings.");
			return default_logger_settings;
		}

		pugi::xml_parse_result parse_result = doc.load_file(full_config_path.c_str());

		if (!parse_result)
		{
			output_debug_print(string_utils::convert_utf8_to_wchar(parse_result.description()) + L",status: " + std::to_wstring(parse_result.status)
				+ L",offset: " + std::to_wstring(parse_result.offset) + L", apply default logger settings.");
			return default_logger_settings;
		}

		pugi::xml_node node_log_settings = doc.child(config_file_root_tag);
		CHECK_XML_ITEM(node_log_settings);

		pugi::xml_node node_developer_log = node_log_settings.child(developer_log_tag);
		CHECK_XML_ITEM(node_developer_log);

		pugi::xml_node node_history_log = node_log_settings.child(history_log_tag);
		CHECK_XML_ITEM(node_history_log);

		CHECK_XML_ITEM(node_developer_log.attribute(use_developer_log_key));
		CHECK_XML_ITEM(node_developer_log.attribute(level_key));
		CHECK_XML_ITEM(node_developer_log.attribute(max_backup_index_key));
		CHECK_XML_ITEM(node_developer_log.attribute(max_file_size_key));
		CHECK_XML_ITEM(node_history_log.attribute(max_buffer_size_key));

		tmp_settings.use_developer_log = node_developer_log.attribute(use_developer_log_key).as_bool();
		tmp_settings.level = get_log_level_by_config_value(string_utils::convert_utf8_to_wchar(node_developer_log.attribute(level_key).value()));
		tmp_settings.max_backup_index = node_developer_log.attribute(max_backup_index_key).as_uint();
		tmp_settings.max_file_size = node_developer_log.attribute(max_file_size_key).as_ullong();

		/*Parse history log tag*/
		tmp_settings.max_buffer_size = node_history_log.attribute(max_buffer_size_key).as_ullong();

		return tmp_settings;
	}

    void reset_settings(logger::settings&& settings)
    {
        settings.validate([](const std::wstring & param_name, const std::wstring & original_value, const std::wstring &)
        {
            LOG_AND_THROW(std::invalid_argument("Invalid logging settings were passed.")) << L"Parameter '" << param_name << L"' has invalid value = " << original_value;
        });

        write_logger_settings(g_instance.get()->get_full_config_path(), settings);

        if (!g_instance.get()->is_watch_settings_changes())
            g_instance.get()->reload_settings_from_config_file();
    }

    void write_logger_settings(const std::wstring &full_file_path, const logger::settings &settings)
	{
		pugi::xml_document doc;
		pugi::xml_node log_settings = doc.append_child(config_file_root_tag);
		pugi::xml_node developer_log = log_settings.append_child(developer_log_tag);
		developer_log.append_attribute(use_developer_log_key) = settings.use_developer_log;
		developer_log.append_attribute(level_key) = string_utils::convert_wchar_to_utf8(get_config_value_by_log_level(settings.level)).c_str();
		developer_log.append_attribute(max_backup_index_key) = settings.max_backup_index;
		developer_log.append_attribute(max_file_size_key) = settings.max_file_size;

		pugi::xml_node history_log = log_settings.append_child(history_log_tag);
		history_log.append_attribute(max_buffer_size_key) = settings.max_buffer_size;

		if (!doc.save_file(full_file_path.c_str())) {
			throw details::internal_logger_exception(L"Config file save error.");
		}
	}

    void validate_logger_settings(logger::settings &settings, const std::wstring &full_config_path)
	{
        settings.validate([&](const std::wstring & param_name, const std::wstring & original_value, const std::wstring & modified_value)
        {
            output_debug_print(L"Replace current '" + param_name + L"' for file: " + full_config_path + L" with value = " + modified_value + L". Original value was = " + original_value + L'.');
        });
	}

    std::wstring create_unique_zip_file_name(const std::wstring &log_path, const std::wstring &log_file_prefix_narrow)
	{
		time_t current_time;
		time(&current_time);
		auto zip_backup_index = 0;

		std::wostringstream ss;
		ss << log_file_prefix_narrow << date_helpers::local_time_to_str(current_time, "%Y-%m-%d_%H-%M-%S").c_str();
		const auto zip_base_name = ss.str();
		ss << ".zip";
		auto zip_file_name = ss.str();

		while (file_exists(path_helpers::concatenate_paths(log_path, zip_file_name)))
		{
			++zip_backup_index;
			const auto temp = zip_base_name + L"_(" + std::to_wstring(zip_backup_index) + L")" + L".zip";
			zip_file_name = temp;
		}

		return zip_file_name;
	}

    void check_and_compress_history_logs(const std::wstring &log_path, const std::wstring &log_file_prefix, const logger::settings &settings)
	{
        std::vector <std::wstring> history_log_files;
        std::vector <std::wstring> zip_files;
		std::uint64_t summary_zip_files_size = 0;
        std::wstring log_file_prefix_narrow = log_file_prefix;

        for_each_fs_object(log_path, [&](const std::wstring &full_file_path)
		{
			/*Don't pack zip archives*/
            if (full_file_path.find(L".zip") == std::wstring::npos)
			{
                std::wstring base_name;
                std::wstring file_ext;
				std::tie(std::ignore, std::ignore, base_name, file_ext) = split_path(full_file_path);
                std::wstring file_name = base_name + file_ext;

				auto const process_id = std::to_wstring(GetCurrentProcessId());
                auto const process_id_prefix = std::wstring(L"_") + process_id;
				auto const regex_date_pattern = L"[0-9]{4}-[0-9]{2}-[0-9]{2}";
				auto const find_pos = log_file_prefix_narrow.find(process_id_prefix);

                if (find_pos != std::wstring::npos)
				{
					log_file_prefix_narrow = log_file_prefix_narrow.substr(0, find_pos);
				}

                if (file_name.find(log_file_prefix_narrow) != std::wstring::npos && std::regex_search(file_name, std::wregex(regex_date_pattern)))
				{
					history_log_files.push_back(full_file_path);
				}
			}
			else
			{
				zip_files.push_back(full_file_path);
				summary_zip_files_size += get_file_size(full_file_path);
			}
		});

		/*process stale zip files*/
		if (!zip_files.empty() && summary_zip_files_size > settings.max_file_size)
		{
            std::sort(zip_files.begin(), zip_files.end(), [](const std::wstring& file1, const std::wstring& file2)
			{
				SYSTEMTIME st1 = get_file_system_datetime(file1);
				SYSTEMTIME st2 = get_file_system_datetime(file2);
				FILETIME ft1;
				FILETIME ft2;

				if (!SystemTimeToFileTime(&st1, &ft1) || !SystemTimeToFileTime(&st2, &ft2))
				{
					throw std::runtime_error("Exception: SystemTimeToFileTime failed.");
				}

				LONG result = CompareFileTime(&ft1, &ft2);

				return result == -1 ? true : false;
			});

			for (const auto &it : zip_files)
			{
				auto const file_size = get_file_size(it);

				if (summary_zip_files_size > settings.max_file_size)
				{
					remove_log_file(it);
					summary_zip_files_size -= file_size;
				}
				else
				{
					break;
				}
			}
		}

		/*process history log files*/
		if (history_log_files.empty())
		{
			return;
		}

		bool zipper_exception_error = true;
		const auto zip_file_name = create_unique_zip_file_name(log_path, log_file_prefix_narrow);

		try 
        {
			/*compression_level = 1 - Z_BEST_SPEED, 0 - Z_NO_COMPRESSION, 9 - Z_BEST_COMPRESSION, (-1) - Z_DEFAULT_COMPRESSION*/
			zip::create_archive(history_log_files, log_path, zip_file_name, 1);

			/*remove compressed history logs*/
			for (const auto &it : history_log_files)
			{
				//without checking, it's not critical. 
				remove_log_file(it);
			}

			zipper_exception_error = false;
		}
		catch (const std::ios_base::failure &e)
		{
			output_debug_print(string_utils::convert_utf8_to_wchar(e.what()));
		}
		catch (const zip::compression_exception &e)
		{
			output_debug_print(string_utils::convert_utf8_to_wchar(e.what()));
		}
		catch (const std::exception &e)
		{
			output_debug_print(string_utils::convert_utf8_to_wchar(e.what()));
		}

		/*remove zip archive in exception case*/
		if (zipper_exception_error)
		{
            std::wstring full_zip_path = path_helpers::concatenate_paths(log_path, zip_file_name);

			if (file_exists(full_zip_path))
			{
				//without checking, it's not critical. 
				remove_log_file(full_zip_path);
			}
		}
	}

    std::wstring get_config_value_by_log_level(logger::level level)
	{
		if (logger::level_unknown == level)
			throw details::internal_logger_exception(L"Invalid log level value.");

        const std::wstring result(log_level_to_string(level));

        if (result == log_level_to_string(logger::level_unknown))
			throw details::internal_logger_exception(L"Invalid log level value.");

		return result;
	}

    logger::level get_log_level_by_config_value(const std::wstring& value)
	{
        std::wstring tmp_value(string_utils::trim(value));

		if (tmp_value.empty())
			throw details::internal_logger_exception(L"Empty log level config value.");
			
        const logger::level result(logger::log_level_from_string(tmp_value));

        if (logger::level_unknown == result)
			throw details::internal_logger_exception(L"Incorrect log level config value.");

		return result;
	}

	void free_logger_instance()
	{
        g_instance.reset();
	}



    namespace
    {

        class instance::fake_impl: public details::logger_interface
        {
        public:

            fake_impl()
            {}

            void initialize(const std::wstring &config_path,
                const std::wstring &config_file_name,
                const std::wstring &log_path,
                const std::wstring &log_file_prefix,
                bool,
                const logger::use_developer_log_changed_f &) override
            {
                m_config_file_name = config_file_name;
                m_config_path = config_path;
                m_log_path = log_path;
                m_log_file_prefix = log_file_prefix;
            }

            void deinitialize() override
            {}

            void reload_settings_from_config_file() override
            {
                // do nothing
            }

            logger::settings get_current_logger_settings() const override
            {
                return default_logger_settings;
            }

            std::wstring get_full_config_path() const override
            {
                return path_helpers::concatenate_paths(m_config_path, m_config_file_name);
            }

            std::wstring get_log_path() const override
            {
                return m_log_path;
            }

            std::wstring get_log_prefix() const override
            {
                return m_log_file_prefix;
            }

            bool is_watch_settings_changes() const override
            {
                return false;
            }

            void log_message(const cpplogger::message& msg) override
            {
                output_debug_print(msg.text_with_apply_format());
            }

        private:

            std::wstring m_log_path;
            std::wstring m_log_file_prefix;
            std::wstring m_config_path;
            std::wstring m_config_file_name;
        };

        template<class impl_logger_t, class... init_args_t> 
        std::enable_if_t<std::is_base_of<details::logger_interface, impl_logger_t>::value, bool>
        instance::set(init_args_t&&... init_args)
        {
            sync_helpers::rw_lock_guard_exclusive lk_init(m_init_access);

            if (m_real_impl)
                return false;

            pointer tmp(std::make_shared<impl_logger_t>());
            tmp->initialize(std::forward<init_args_t>(init_args)...);

            {
                sync_helpers::rw_lock_guard_exclusive lk_write(m_access);
                tmp.swap(m_logger);
            }

            m_real_impl = true;
            return true;
        }

        void instance::reset()
        {
            sync_helpers::rw_lock_guard_exclusive lk_init(m_init_access);

            pointer tmp;
            {
                sync_helpers::rw_lock_guard_exclusive lk_write(m_access);
                tmp.swap(m_logger);
            }

            m_real_impl = false;
        }

        instance::pointer instance::get()
        {
            if (pointer ptr = get_lk_read())
                return ptr;
            return get_lk_read_write();
        }

        instance::pointer instance::get_lk_read()
        {
            sync_helpers::rw_lock_guard_shared lk(m_access);
            return m_logger;
        }

        instance::pointer instance::get_lk_read_write()
        {
            sync_helpers::rw_lock_guard_exclusive lk(m_access);
            if (!m_logger)
                m_logger = std::make_shared<fake_impl>();

            return m_logger;
        }

    } // namespace anonymous

} // namespace cpplogger