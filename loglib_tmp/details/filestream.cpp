
#include <mutex>
#include <cassert>
#include <sstream>
#include <codecvt>
#include <cwctype>
#include <algorithm>

#include <windows.h>

#include <common/string_utils.h>

#include "details/filestream.h"


namespace
{

	class rolling_exception final : public std::exception
	{
	public:
		rolling_exception(const std::wstring& message)
			: std::exception(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(message).c_str())
		{ }
	};
	
	void for_each_fs_objects(const std::wstring& path, const std::function<void(const std::wstring& file_name_abs)>& func)
	{
		std::wstring tmp_path(path);
		if (!tmp_path.empty() && tmp_path.back() != L'\\' && tmp_path.back() != L'/')
			tmp_path.append(L"\\");

		WIN32_FIND_DATAW find_data;
		HANDLE handle(::FindFirstFileW(std::wstring(tmp_path).append(L"*").c_str(), &find_data));

		if (INVALID_HANDLE_VALUE == handle)
		{
			if (ERROR_FILE_NOT_FOUND == ::GetLastError())
				return;
				
			throw rolling_exception(L"FindFirstFile failed");
		}

		auto goto_next_item([](HANDLE find_handle, LPWIN32_FIND_DATAW find_data)
		{
			if (0 == ::FindNextFileW(find_handle, find_data))
			{
				if (ERROR_NO_MORE_FILES != ::GetLastError())
					throw rolling_exception(L"FindNextFile failed");

				return false;
			}

			return true;
		});

		try
		{
			do
			{
				const std::wstring object_name(find_data.cFileName);
				if (object_name == L"." || object_name == L"..")
					continue;

				func(std::wstring(tmp_path).append(object_name));

			} while (goto_next_item(handle, &find_data));

			::FindClose(handle);
		}
		catch (...)
		{
			::FindClose(handle);
			throw;
		}
	}

} // namespace


namespace cpplogger
{

	namespace details
	{

		filestream::filestream(const std::wstring& path, const std::wstring& file_prefix)
			: m_path(path)
			, m_file_prefix(file_prefix)
			, m_files_info()
			, m_writer()
			, m_no_recorded_messages()
            , m_number_irretrievably_lost_messages(0)
			, m_settings()
			, m_settings_changed(false)
#ifdef LOG_FILESTREAM_THREAD_USING
            , m_break_work(false)
            , m_messages()
            , m_finished_handle(::CreateEventW(NULL, FALSE, FALSE, NULL))
            , m_thread_handle(::CreateThread(nullptr, 64 * 1024, &filestream::run_win32, this, (STACK_SIZE_PARAM_IS_A_RESERVATION), nullptr))
#endif // LOG_FILESTREAM_THREAD_USING
		{ }

		filestream::~filestream()
		{
#ifdef LOG_FILESTREAM_THREAD_USING
            this->break_work();

            if (WAIT_OBJECT_0 != ::WaitForSingleObject(m_finished_handle, INFINITE))
            {
                OutputDebugStringA("Error waiting finished work logger thread");
            }
            ::CloseHandle(m_finished_handle);
            ::CloseHandle(m_thread_handle);
#endif // LOG_FILESTREAM_THREAD_USING
		}

		std::wstring filestream::path() const
		{
			return m_path;
		}

		std::wstring filestream::file_prefix() const
		{
			return m_file_prefix;
		}

        void filestream::set_settings(logger::settings&& settings)
		{
#ifdef LOG_FILESTREAM_THREAD_USING
            std::lock_guard<decltype(m_messages.access)> guard(m_messages.access);
#endif // LOG_FILESTREAM_THREAD_USING

			m_settings = std::move(settings);
		}

        void filestream::push_message(cpplogger::message&& msg)
        {
#ifdef LOG_FILESTREAM_THREAD_USING
            std::lock_guard<decltype(m_messages.access)> guard(m_messages.access);

            m_messages.list.emplace_back(std::move(msg));

            m_messages.condition.notify_one();
#else
            std::lock_guard<std::mutex> lk(m_writer_access);

            write(msg, m_settings);
#endif // LOG_FILESTREAM_THREAD_USING
        }

        void filestream::push_message(cpplogger::message&& msg, logger::settings&& settings)
        {
#ifdef LOG_FILESTREAM_THREAD_USING
            std::lock_guard<decltype(m_messages.access)> guard(m_messages.access);

            m_messages.list.emplace_back(std::move(msg));

            m_settings = std::move(settings);

            m_messages.condition.notify_one();
#else
            std::lock_guard<std::mutex> lk(m_writer_access);

            m_settings = std::move(settings);

            write(msg, m_settings);
#endif // LOG_FILESTREAM_THREAD_USING
        }

#ifdef LOG_FILESTREAM_THREAD_USING
        void filestream::break_work()
		{
			std::lock_guard<decltype(m_messages.access)> guard(m_messages.access);
		
			if (m_break_work)
				return;

			m_break_work = true;

			m_messages.condition.notify_one();
		}

        filestream::wait_result filestream::wait_and_take_messages(std::list<cpplogger::message>& msgs, logger::settings& settings)
		{
			std::unique_lock<decltype(m_messages.access)> lock(m_messages.access);

			m_messages.condition.wait(lock, [&]{ return m_break_work || !m_messages.list.empty(); });

			settings = m_settings;
            msgs = std::move(m_messages.list);

			assert(m_messages.list.empty());

			return m_break_work
				? wait_result::break_work
				: wait_result::success;
		}

        DWORD WINAPI filestream::run_win32(_In_ LPVOID lpParameter)
        {
            DWORD result(0);

            filestream* const this_filestream(static_cast<filestream*>(lpParameter));

            try
            {
                this_filestream->run();
                result = 0;
            }
            catch (...)
            {
                ::OutputDebugStringW(L"An unknown exception was caught.");
                result = 1;
            }

            if (FALSE == ::SetEvent(this_filestream->m_finished_handle))
            {
                ::OutputDebugStringA("Error set event finished work logger thread");
                result = 2;
            }

            ::SwitchToThread();

            return result;
        }

		void filestream::run()
		{
			wait_result result(wait_result::break_work);
            std::list<cpplogger::message> msgs;
            logger::settings settings;

			do
			{
                result = this->wait_and_take_messages(msgs, settings);

                for (const cpplogger::message& msg : msgs)
				{
                    this->write(msg, settings);
				}

			} while (wait_result::break_work != result);
			
			m_writer.close();
		}
#endif // LOG_FILESTREAM_THREAD_USING

        void filestream::write(const cpplogger::message& msg, const logger::settings& settings)
        {
            const std::size_t max_number_irretrievably_lost_messages(2000);

            try
            {
                bool is_ok(true);

                if (m_number_irretrievably_lost_messages > 0)
                {
                    std::wstringstream ss;
                    ss << std::endl;
                    ss << L" **********************************************" << std::endl;
                    ss << L" *** Failures in logging" << std::endl;
                    ss << L" *** Number irretrievably lost messages: " << m_number_irretrievably_lost_messages << std::endl;
                    ss << L" **********************************************" << std::endl;
                    ss << std::endl;

                    if (true == (is_ok = write_message_to_file(ss.str(), nullptr)))
                    {
                        m_number_irretrievably_lost_messages = 0;
                    }
                }

                const bool print_thread_id(true);

                const bool use_tabs(logger::level::debug >= settings.level); // Т.к. логирование посещения функций включается начиная с режима debug

                while (is_ok && !m_no_recorded_messages.empty())
                {
                    const cpplogger::message& front_msg(m_no_recorded_messages.front());

                    if (true == (is_ok = write_message_to_file(front_msg.text_with_apply_format(print_thread_id, use_tabs), nullptr)))
                    {
                        m_no_recorded_messages.pop_front();
                    }
                }

                bool need_apply_rolling(false);

                if (!is_ok || !write_message_to_file(msg.text_with_apply_format(print_thread_id, use_tabs), &need_apply_rolling))
                {
                    assert(!need_apply_rolling);

                    m_no_recorded_messages.push_back(msg);

                    if (m_no_recorded_messages.size() > max_number_irretrievably_lost_messages)
                    {
                        m_no_recorded_messages.pop_front();
                        m_number_irretrievably_lost_messages += 1;
                    }
                }

                if (need_apply_rolling)
                    rolling_files(settings);
            }
            catch (const std::bad_alloc& exc)
            {
                OutputDebugStringA(exc.what());
            }
            catch (const std::exception& exc)
            {
                std::stringstream stream;
                stream << "STL exception in worker filestream logger: " << exc.what();

                OutputDebugStringA(stream.str().c_str());
            }
            catch (...)
            {
                OutputDebugStringA("Unknown exception in worker filestream logger");
            }
        }

		void filestream::create_new_file_and_open_to_write()
		{
			file_info info;
			info.backup_index = 0;
			info.full_path = create_file_path(info.backup_index);

			if (!file_exist(info.backup_index))
				m_files_info.push_front(info);

			m_writer.open(info.full_path, logger::open_disposition::file_open_normal_mode);
		}

		std::wstring filestream::create_file_path(std::size_t backup_index) const
		{
			std::wostringstream ss;

			if (!m_path.empty())
			{
				ss << m_path;

				if (m_path[m_path.length() - 1] != L'\\')
					ss << L'\\';
			}

			ss << m_file_prefix;
			ss << ".log";

			if (0 != backup_index)
			{
				ss << ".";
				ss << backup_index;
			}

			return ss.str();
		}

		bool filestream::file_exist(std::size_t backup_index) const
		{
			return std::find_if(std::begin(m_files_info), std::end(m_files_info), [&](const file_info& info)
			{
				return info.backup_index == backup_index;

			}) != std::end(m_files_info);
		}

		void filestream::clear_file_data()
		{
			m_writer.close();

			if (m_files_info.empty())
				return;

			try
			{
				m_writer.open(m_files_info.front().full_path, logger::open_disposition::file_open_clear_mode);
				m_writer.close();
			}
			catch (const logger::log_file_writer_exception& exc)
			{
				OutputDebugStringA(std::string("Not clear file data: ").append(exc.what()).append("\n").c_str());
			}
		}

        void filestream::rolling_files(const logger::settings& settings)
		{
            try
			{
				synchronize_log_file_directory(settings);

				if (m_files_info.back().backup_index == settings.max_backup_index)
				{
					cleanup_stale_log();
				}

				for (auto it = std::rbegin(m_files_info); it != std::rend(m_files_info); ++it)
				{
					it->backup_index += 1;

					std::wstring target_path(create_file_path(it->backup_index));

					if (!rename_file(it->full_path, target_path))
						throw rolling_exception(L"Can't rename log file from: " + it->full_path + L" to: " + target_path);

					it->full_path = target_path;
				}
			}
			catch (const rolling_exception& exc)
			{
				OutputDebugStringA(std::string("Not rolling files: ").append(exc.what()).append("\n").c_str());

				clear_file_data();
			}
		}

		void filestream::cleanup_stale_log()
		{
			assert(!m_files_info.empty());

			if (!remove_file(m_files_info.back().full_path))
				throw rolling_exception(L"Can't remove stale file: " + m_files_info.back().full_path);

			m_files_info.pop_back();
		}

		bool filestream::remove_file(const std::wstring& filename) const
		{
			return (0 == _wremove(filename.c_str()));
		}

		bool filestream::rename_file(const std::wstring& old_filename, const std::wstring& new_filename) const
		{
			return (0 == _wrename(old_filename.c_str(), new_filename.c_str()));
		}

		std::size_t filestream::get_number_from_end(std::wstring& str, std::size_t default_number) const
		{
			std::wstring str_number;

			for (auto it = std::rbegin(str), it_end = std::rend(str); it != it_end; ++it)
			{
				if (!std::iswdigit(*it))
					break;

				str_number.insert(std::begin(str_number), *it);
			}

			if (str_number.empty())
				return default_number;

			std::size_t number(default_number);

			std::wstringstream(str_number) >> number;

			return number;
		}

        void filestream::synchronize_log_file_directory(const logger::settings& settings)
		{
			std::list<file_info> new_files_info;

			const std::wstring match_pattern(m_file_prefix + L".log");

			for_each_fs_objects(m_path, [&](const std::wstring& file_name_abs)
			{
				if (std::wstring::npos == file_name_abs.find(match_pattern))
					return;
				
				file_info info;
				info.full_path = file_name_abs;
				info.backup_index = get_number_from_end(info.full_path, 0);

				new_files_info.emplace_back(std::move(info));
			});

			new_files_info.sort([](const file_info& left, const file_info& right){ return left.backup_index < right.backup_index; });

			m_files_info = std::move(new_files_info);

			while (!m_files_info.empty() && m_files_info.back().backup_index > settings.max_backup_index)
			{
				cleanup_stale_log();
			}
		}

        bool filestream::write_message_to_file(const std::wstring& message, bool* need_apply_rolling)
		{
            try
			{
				if (!m_writer.is_open())
					create_new_file_and_open_to_write();

				m_writer.write(message + L"\r\n");

                if (need_apply_rolling)
                {
                    if (m_writer.current_log_file_size() >= m_settings.max_file_size)
                    {
                        *need_apply_rolling = true;
                        m_writer.close();
                    }
                    else
                    {
                        *need_apply_rolling = false;
                    }
                }

				return true;
			}
			catch (const logger::log_file_writer_exception& exc)
			{
				OutputDebugStringA(std::string(exc.what()).append("\n").c_str());

				m_writer.close();

				return false;
			}
		}

	} // namespace details

} // namespace cpplogger
