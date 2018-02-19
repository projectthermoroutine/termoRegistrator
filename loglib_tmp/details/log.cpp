#include "log.h"
#include <future>
#include <fstream>
#include <memory>
#include <mutex>
#include <atomic>
#include <list>
#include <utility>
#include <sstream>
#include <chrono>
#include <codecvt>
#include <map>

#define NOGDI
#include <Windows.h>

#include <common\sync_helpers.h>
#include <common\date_helpers.h>

#include "cpplogger.h"
#include "log_details.h"


#if defined(_MSC_VER) && _MSC_VER < 1900
#	define thread_local __declspec(thread)
#endif 


namespace
{
	thread_local std::size_t scope_logger_level_from_this_thread = 0;

	std::uint64_t get_max_messages_size();

	using thread_id_t = decltype(GetCurrentThreadId());
	using process_id_t = decltype(GetCurrentProcessId());
	auto const skipped_message = std::wstring{ L"Message was skipped" };

	class thread_log_history final
	{
	public:

		thread_log_history() : _messages_size(0), _last_insert_time() {}
		~thread_log_history() { clear(); }

		void insert_message(const cpplogger::message& msg)
		{
			if (msg.text.empty())
			{
				return;
			}
			
			auto const new_message_mem_size = calc_message_memory_size(msg);
			auto const max_messages_size = static_cast<std::size_t>(get_max_messages_size());

			const auto insertion_func = [&](const cpplogger::message& msg, std::size_t message_mem_size)
			{
				auto const time = std::chrono::system_clock::now();
				_messages.push_back(msg);
				_messages_size += message_mem_size;
				_last_insert_time = std::chrono::steady_clock::now();
			};
			
			const auto cleanup_history_if_needed = [&](std::size_t input_message_mem_size)
			{
				while ((_messages_size + input_message_mem_size > max_messages_size) && (!_messages.empty()))
				{
					auto const reclaimed_mem_size = calc_message_memory_size(_messages.front());
					_messages.pop_front();
					_messages_size -= reclaimed_mem_size;
				}
			};

			std::lock_guard <decltype(_mx)> lock(_mx);

			if (new_message_mem_size <= max_messages_size)
			{
				cleanup_history_if_needed(new_message_mem_size);
				insertion_func(msg, new_message_mem_size);
			}
			else
			{
				cpplogger::message skipped_msg;
				skipped_msg.text = skipped_message;
				skipped_msg.time = msg.time;
				skipped_msg.thread_id = msg.thread_id;
				skipped_msg.log_level = msg.log_level;
				skipped_msg.scope_level = msg.scope_level;

				auto const skipped_message_mem_size = calc_message_memory_size(skipped_msg);
				cleanup_history_if_needed(skipped_message_mem_size);
				insertion_func(skipped_msg, skipped_message_mem_size);
			}
		}

		void for_each_message(const std::function<void(const cpplogger::message&)>& process_func) const
		{
			for (auto const & message : _messages)
			{
				process_func(message);
			}
		}

		void clear()
		{
			std::lock_guard <decltype(_mx)> lock(_mx);
			_messages.clear();
			_messages_size = 0;
		}

		bool empty() const
		{
			std::lock_guard <decltype(_mx)> lock(_mx);
			return _messages.empty();
		}

		std::wstring get_last_insert_message(logger::level* sev_ptr) const
		{
			std::lock_guard <decltype(_mx)> lock(_mx);

			if (_messages.empty())
				return std::wstring();

			const cpplogger::message& message(_messages.back());

			if (sev_ptr)
				*sev_ptr = message.log_level;

			return message.text;
		}

		std::chrono::steady_clock::time_point get_last_insert_time() const
		{
			std::lock_guard <decltype(_mx)> lock(_mx);
			return _last_insert_time;
		}

		std::size_t get_messages_size() const
		{
			std::lock_guard <decltype(_mx)> lock(_mx);
			return _messages_size;
		}

		std::size_t get_messages_count() const
		{
			std::lock_guard <decltype(_mx)> lock(_mx);
			return _messages.size();
		}

		thread_log_history(const thread_log_history &) = delete;
		thread_log_history & operator = (const thread_log_history &) = delete;

		thread_log_history(thread_log_history && src) : _messages(std::move(src._messages)), _messages_size(src._messages_size)
		{
		}

		thread_log_history & operator = (thread_log_history && src)
		{
			_messages = std::move(src._messages);
			_messages_size = src._messages_size;
			return *this;
		}
	private:
		static std::size_t calc_message_memory_size(const cpplogger::message& msg)
		{
			//  V119 More than one sizeof() operator is used in one expression. log.cpp 126
			return msg.text.size() * sizeof(std::wstring::value_type) + sizeof(cpplogger::message); //-V119
		}

		mutable std::mutex _mx;
		std::list<cpplogger::message> _messages;
		std::size_t _messages_size;
		std::chrono::steady_clock::time_point _last_insert_time;
	};

	std::mutex thread_history_mx;
	std::map<thread_id_t, thread_log_history> thread_histories;
	std::chrono::steady_clock::time_point last_history_clean = std::chrono::steady_clock::now();
	const std::chrono::steady_clock::duration clean_period = std::chrono::minutes(1);

	void dump_history(
		process_id_t process_id,
		thread_id_t thread_id,
		const thread_log_history & history,
		logger::level sev,
		thread_id_t causer);

	void cleanup_stale_history()
	{
		auto now = std::chrono::steady_clock::now();
		if (now >= last_history_clean + clean_period)
		{
			std::lock_guard <decltype(thread_history_mx)> lock(thread_history_mx);
			for (auto iter = thread_histories.begin(); iter != thread_histories.end();)
			{
				if (iter->second.get_last_insert_time() + clean_period <= now && !iter->second.empty())
				{
					auto const handle = OpenThread(SYNCHRONIZE, FALSE, iter->first);
					if (handle == NULL)
					{
						iter = thread_histories.erase(iter);
						continue;
					}
					CloseHandle(handle);
				}
				++iter;
			}

			last_history_clean = now;
		}
	}

	void clear_history_log()
	{
		std::lock_guard <decltype(thread_history_mx)> lock(thread_history_mx);

		for (std::pair<const thread_id_t, thread_log_history>& pair : thread_histories)
			pair.second.clear();
	}

	void write_to_issue_history_log(const cpplogger::message& msg)
	{
		using namespace logger;

		auto const thread_id = GetCurrentThreadId();
		{
			std::unique_lock<std::mutex> lock(thread_history_mx);
			auto hist_iter = thread_histories.find(thread_id);
			if (hist_iter == thread_histories.end())
			{
				hist_iter = thread_histories.emplace(thread_id, thread_log_history()).first;
			}

			const bool dump_required = msg.log_level == level::error || msg.log_level == level::fatal;
			if (!dump_required)
			{
				lock.unlock();
			}

			hist_iter->second.insert_message(msg);

			if (dump_required)
			{
				auto const process_id = GetCurrentProcessId();
				for (auto const & thread_history_pair : thread_histories)
				{
					dump_history(
						process_id,
						thread_history_pair.first,
						thread_history_pair.second,
						msg.log_level,
						thread_id);
				}
				
				for (auto & thread_history_pair : thread_histories)
				{
					thread_history_pair.second.clear();
				}

				cpplogger::check_and_compress_history_logs(cpplogger::get_log_path(),
					cpplogger::get_log_prefix(),
					cpplogger::get_current_logger_settings());
			}
			else
			{
				cleanup_stale_history();
			}
		}
	}

	void dump_history(
		process_id_t process_id,
		thread_id_t thread_id,
		const thread_log_history & history,
		logger::level lvl,
		thread_id_t causer)
	{
		if (history.empty())
		{
			return;
		}

		time_t current_time;
		time(&current_time);

		std::wostringstream ss;
		auto const & log_path = logger::get_log_path();
		if (!log_path.empty())
		{
			ss << log_path << '\\';
		}
		ss << logger::get_log_name_prefix();
		ss << date_helpers::local_time_to_str(current_time, "%Y-%m-%d_%H-%M-%S").c_str() << '_' << process_id << '_' << causer << '_';
		if (causer == thread_id)
		{
			ss << log_level_to_string(lvl);
		}
		else
		{
			ss << thread_id;
		}
		ss << ".log";

		auto const file_name = ss.str();

		std::ofstream log_file(file_name, std::ios_base::out | std::ios_base::app);
		if (!log_file.is_open())
		{
			return;
		}

		const bool not_print_thread_id(false);

		history.for_each_message([&](const cpplogger::message& message)
		{
			log_file << string_utils::convert_wchar_to_utf8(message.text_with_apply_format(not_print_thread_id)) << std::endl;
		});
	}

	std::uint64_t get_max_messages_size()
	{
		return cpplogger::get_current_logger_settings().max_buffer_size;
	}
}

namespace logger
{

	namespace details
	{

		std::size_t get_thread_history_log_count_messages(thread_id_t id)
		{
			const auto find_iter(thread_histories.find(id));
			return find_iter != thread_histories.end() ? find_iter->second.get_messages_count() : std::size_t(0);
		}

		std::wstring get_last_insert_message_in_history(thread_id_t id, logger::level* sev_ptr)
		{
			std::lock_guard<std::mutex> lk(thread_history_mx);

			const auto it(thread_histories.find(id));
			if (it == std::end(thread_histories))
				return std::wstring();

			return it->second.get_last_insert_message(sev_ptr);
		}

	} // namespace details

	void __cdecl log_message(level lvl, const std::wstring & text)
	{
		cpplogger::message message(cpplogger::message::make(lvl, scope_logger_level_from_this_thread, std::wstring(text)));

		write_to_issue_history_log(message);

		if (cpplogger::get_current_logger_settings().use_developer_log)
		{
			cpplogger::log_message(message);
		}
#ifdef _DEBUG
		else
		{
			OutputDebugStringW((message.text_with_apply_format() + L'\n').c_str());
		}
#endif // _DEBUG		
	}

	bool initialize(const std::wstring &settings_path,
		const std::wstring &settings_file_name,
		const std::wstring &log_path,
		const std::wstring &log_file_prefix,
		bool watch_config_changes,
		const use_developer_log_changed_f & use_developer_log_changed)
	{
		if (cpplogger::initialize(settings_path, settings_file_name, log_path, log_file_prefix, watch_config_changes, use_developer_log_changed))
			return false;

		clear_history_log();

#ifdef _DEBUG
		if (cpplogger::get_current_logger_settings().use_developer_log)
			OutputDebugStringW(L"Developers log is initialized\n");
#endif // _DEBUG		

		return true;
	}

	void deinitialize()
	{
		cpplogger::free_logger_instance();
	}

	void set_settings(settings&& settings)
	{
		cpplogger::reset_settings(std::move(settings));
	}

	void reset_settings_to_default()
	{
		cpplogger::reset_settings(cpplogger::get_default_logger_settings());
	}

	logger::settings get_settings()
	{
		return cpplogger::get_current_logger_settings();
	}

	std::wstring get_log_path()
	{
		return cpplogger::get_log_path();
	}

	std::wstring get_log_name_prefix()
	{
		return cpplogger::get_log_prefix();
	}


	std::wstring scope_logger::make_str_elapsed_time(const std::chrono::steady_clock::time_point& time)
	{
		auto elapsed(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - time).count());
		if (elapsed < 1000)
			return std::to_wstring(elapsed).append(L" microsec");

		elapsed /= 1000;
		if (elapsed < 1000)
			return std::to_wstring(elapsed).append(L" ms");

		elapsed /= 1000;
		if (elapsed < 60)
			return std::to_wstring(elapsed).append(L" sec");

		auto elapsed_minutes(elapsed / 60);
		elapsed %= 60;

		return std::to_wstring(elapsed_minutes).append(L" min, ").append(std::to_wstring(elapsed).append(L" sec"));
	}

	scope_logger::scope_logger(const char* func_name, const char* file_name, int line_number)
		: m_func_name(func_name)
		, m_start()
	{
		std::wostringstream ss;
		ss << (std::uncaught_exception() ? cpplogger::prefix_function::entry_exception : cpplogger::prefix_function::entry) << ' ';
		ss << func_name << ", file: " << file_name << ", line: " << line_number;

		log_message(level::debug, ss.str());

		scope_logger_level_from_this_thread += 1;

		m_start = std::chrono::steady_clock::now();
	}

	scope_logger::~scope_logger()
	{
		const std::wstring elapsed_time(make_str_elapsed_time(m_start));

		std::wostringstream ss;
		ss << (std::uncaught_exception() ? cpplogger::prefix_function::exit_exception : cpplogger::prefix_function::exit) << ' ';
		ss << m_func_name << ", elapsed: " << elapsed_time;

		scope_logger_level_from_this_thread -= 1;

		log_message(level::debug, ss.str());
	}


	
	scope_logger_ex::scope_logger_ex(std::wstring&& scope_name, const char* file_name, int line_number)
		: m_scope_name(std::move(scope_name))
		, m_start()
	{
		std::wostringstream ss;
		ss << (std::uncaught_exception() ? cpplogger::prefix_function::entry_exception : cpplogger::prefix_function::entry) << ' ';
		ss << m_scope_name << ", file: " << file_name << ", line: " << line_number;

		log_message(level::debug, ss.str());

		scope_logger_level_from_this_thread += 1;

		m_start = std::chrono::steady_clock::now();
	}

	scope_logger_ex::scope_logger_ex(const std::wstring& scope_name, const char* file_name, int line_number)
		: scope_logger_ex(std::wstring(scope_name), file_name, line_number)
	{}

	scope_logger_ex::scope_logger_ex(const std::string& scope_name, const char* file_name, int line_number)
		: scope_logger_ex(std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(scope_name), file_name, line_number)
	{}

	scope_logger_ex::~scope_logger_ex()
	{
		const std::wstring elapsed_time(scope_logger::make_str_elapsed_time(m_start));

		std::wostringstream ss;
		ss << (std::uncaught_exception() ? cpplogger::prefix_function::exit_exception : cpplogger::prefix_function::exit) << ' ';
		ss << m_scope_name << ", elapsed: " << elapsed_time;

		scope_logger_level_from_this_thread -= 1;

		log_message(level::debug, ss.str());
	}


	log_stream::log_stream(level sev)
		: _lvl(sev)
	{}

	log_stream::~log_stream()
	{
		try
		{
			flush();
		}
		catch (const std::exception & exc)
		{
			OutputDebugStringA(exc.what());
			std::terminate();
		}
		catch (...)
		{
			OutputDebugStringA("An unknown exception was caught");
			std::terminate();
		}
	}

	log_stream & log_stream::operator<<(char ref)
	{
		_ss << std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(ref);
		return *this;
	}

	log_stream & log_stream::operator<<(const char* ref)
	{
		_ss << (ref ? std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(ref) : std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes('\0'));
		return *this;
	}

	log_stream & log_stream::operator<<(const std::string& ref)
	{
		_ss << std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(ref);
		return *this;
	}

	void log_stream::flush()
	{
		const std::wstring str(_ss.str());
		if (!str.empty())
			log_message(_lvl, str);
	}

	std::wstring log_exception(std::exception_ptr except, logger::level level, const std::wstring & head_message)
	{
		try
		{
			std::rethrow_exception(except);
		}
		catch (const std::future_error& exc)
		{
			LOG(level)
				<< head_message
				<< L" [error: " << exc.what() << L"]"
				<< L" [category: " << exc.code().category().name() << L"]";

			return string_utils::convert_utf8_to_wchar(exc.what());
		}
		catch (const std::system_error& exc)
		{
			LOG(level)
				<< head_message
				<< L" [error: " << exc.what() << L"]"
				<< L" [category: " << exc.code().category().name() << L"]";

			return string_utils::convert_utf8_to_wchar(exc.what());
		}
		catch (const std::exception& exc)
		{
			LOG(level)
				<< head_message
				<< L" [error: " << exc.what() << L"]";

			return string_utils::convert_utf8_to_wchar(exc.what());
		}
		catch (...)
		{
			LOG(level)
				<< head_message
				<< L" [error: <unknown>]";
		}

		return std::wstring(L"<unknown>");
	}

	std::wstring log_current_exception(logger::level level, const std::wstring & head_message)
	{
		return logger::log_exception(std::current_exception(), level, head_message);
	}
	
	void try_catchs_to_log(const std::function<void()>& func, logger::level level, const std::wstring & head_message)
	{
		try
		{
			func();
		}
		catch (...)
		{
			logger::log_exception(std::current_exception(), level, head_message);
		}
	}

} // namespace logger
