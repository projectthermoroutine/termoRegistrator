#include "log.h"
#include <fstream>
#include <mutex>
#include <atomic>
#include <list>
#include <utility>
#include <sstream>
#include <chrono>
#include <map>

#define NOGDI
#include <Windows.h>

#include <common\sync_helpers.h>
#include <common\date_helpers.h>

#include "cpplogger.h"

namespace
{
	std::uint64_t get_max_messages_size();
	void cleanup_stale_history();

	cpplogger::log_level severity_to_log_level(logger::severity sev)
	{
		using namespace logger;

		switch (sev)
		{
		case severity::trace: return cpplogger::log_level::trace;
		case severity::debug: return cpplogger::log_level::debug;
		case severity::info: return cpplogger::log_level::info;
		case severity::warn: return cpplogger::log_level::warn;
		case severity::error: return cpplogger::log_level::error;
		case severity::fatal: return cpplogger::log_level::fatal;
		}

		throw std::invalid_argument("The passed argument sev is invalid");
	}

	using thread_id_t = decltype(GetCurrentThreadId());
	using process_id_t = decltype(GetCurrentProcessId());
	auto const skipped_message = std::wstring{ L"Message was skipped" };

	class thread_log_history final
	{
	public:
		struct message_info
		{
			logger::severity sev;
			std::chrono::system_clock::time_point time;
			std::wstring message;
		};

		thread_log_history() : _messages_size(0), _last_insert_time() {}
		~thread_log_history() { clear(); }

		void insert_message(logger::severity sev, const std::wstring& message)
		{
			if (message.empty())
			{
				return;
			}

			auto const new_message_mem_size = calc_message_memory_size(message);
			auto const max_messages_size = static_cast<std::size_t>(get_max_messages_size());

			const auto insertion_func = [&](const std::wstring &message, std::size_t message_mem_size)
			{
				auto const time = std::chrono::system_clock::now();
				_messages.push_back({ sev, time, message });
				_messages_size += message_mem_size;
				_last_insert_time = std::chrono::steady_clock::now();
			};

			const auto cleanup_history_if_needed = [&](std::size_t input_message_mem_size)
			{
				while ((_messages_size + input_message_mem_size > max_messages_size) && (!_messages.empty()))
				{
					auto const reclaimed_mem_size = calc_message_memory_size(_messages.front().message);
					_messages.pop_front();
					_messages_size -= reclaimed_mem_size;
				}
			};

			std::lock_guard <decltype(_mx)> lock(_mx);

			if (new_message_mem_size <= max_messages_size)
			{
				cleanup_history_if_needed(new_message_mem_size);
				insertion_func(message, new_message_mem_size);
			}
			else
			{
				auto const skipped_message_mem_size = calc_message_memory_size(skipped_message);
				cleanup_history_if_needed(skipped_message_mem_size);
				insertion_func(skipped_message, skipped_message_mem_size);
			}
		}

		void for_each_message(const std::function<void(const message_info &)>& process_func) const
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
		static std::size_t calc_message_memory_size(const std::wstring & message)
		{
			//  V119 More than one sizeof() operator is used in one expression. log.cpp 126
			return message.size() * sizeof(std::wstring::value_type) + sizeof(message_info); //-V119
		}

		mutable std::mutex _mx;
		std::list<message_info> _messages;
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
		logger::severity sev,
		thread_id_t causer);

	void write_to_issue_history_log(logger::severity sev, const std::wstring & message)
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

			const bool dump_required = sev == severity::error || sev == severity::fatal;
			if (!dump_required)
			{
				lock.unlock();
			}

			hist_iter->second.insert_message(sev, message);

			if (dump_required)
			{
				auto const process_id = GetCurrentProcessId();
				for (auto const & thread_history_pair : thread_histories)
				{
					dump_history(
						process_id,
						thread_history_pair.first,
						thread_history_pair.second,
						sev,
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

	void cleanup_stale_history()
	{
		auto now = std::chrono::steady_clock::now();
		if (now >= last_history_clean + clean_period)
		{
			std::lock_guard <decltype(thread_history_mx)> lock(thread_history_mx);
			now = std::chrono::steady_clock::now();
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

			last_history_clean = std::chrono::steady_clock::now();
		}
	}

	void dump_history(
		process_id_t process_id,
		thread_id_t thread_id,
		const thread_log_history & history,
		logger::severity sev,
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
			ss << logger::severity_to_str(sev).c_str();
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

		history.for_each_message(
			[&log_file](const thread_log_history::message_info & message_info)
		{
			log_file << date_helpers::local_time_to_str(std::chrono::system_clock::to_time_t(message_info.time), "%Y/%m/%d %H:%M:%S").c_str();
			log_file << ' ' << logger::severity_to_str(message_info.sev).c_str() << ' ' << string_utils::convert_wchar_to_utf8(message_info.message) << '\n';
		}
		);
	}

	sync_helpers::once_flag init_once;
	std::atomic<bool> history_logger_initialized = false;

	sync_helpers::once_flag lazy_initializer_was_set;
	using lazy_initializer_func_t = std::function < void(void) > ;
	lazy_initializer_func_t lazy_initializer_func;
	std::mutex lazy_initializer_func_mtx;
	volatile LONG lazy_initialization_required = 0; // works as a flag for lazy initialization

	void do_init(const std::wstring &config,
		const std::wstring &log_path,
		const std::wstring &log_file_prefix,
		bool watch_config_changes,
		const logger::notify_developers_logging_enabled_func_t & notify_developers_logging_enabled_func)
	{
		history_logger_initialized = true;

		OutputDebugStringW(L"History log is initialized\n");

		cpplogger::initialize(config, log_path, log_file_prefix, watch_config_changes, notify_developers_logging_enabled_func);

		if (cpplogger::get_current_logger_settings().use_developer_log)
		{
			OutputDebugStringW(L"Developers log is initialized\n");
		}
	}
				 
	std::uint64_t get_max_messages_size()
	{
		return cpplogger::get_current_logger_settings().max_buffer_size;
	}

	void check_lazy_initialization_required()
	{		
		if (InterlockedCompareExchange(&lazy_initialization_required, 0, 1) == 0)
		{
			return;
		}

		decltype(lazy_initializer_func) lazy_initializer;

		{
			std::lock_guard<decltype(lazy_initializer_func_mtx)> lock(lazy_initializer_func_mtx);
			std::swap(lazy_initializer, lazy_initializer_func);
		}

		if (lazy_initializer)
		{
			lazy_initializer();
		}
	}
}

namespace logger
{
	struct severity_and_string_t
	{
		logger::severity severity;
		const char* string;
	};

	severity_and_string_t severity_to_string_table[] =
	{
		{ severity::trace, "TRACE" },
		{ severity::debug, "DEBUG" },
		{ severity::info, "INFO" },
		{ severity::warn, "WARN" },
		{ severity::error, "ERROR" },
		{ severity::fatal, "FATAL" }
	};

	std::string severity_to_str(logger::severity sev)
	{
		for (const auto& item : severity_to_string_table)
		{
			if (item.severity == sev)
				return item.string;
		}

		throw std::invalid_argument("The passed argument severity is invalid");
	}

	logger::severity str_to_severity(const std::string& str)
	{
		for (const auto& item : severity_to_string_table)
		{
			if (item.string == str)
				return item.severity;
		}

		throw std::invalid_argument("The passed argument severity is invalid");
	}

	std::wstring format_log_message(severity sev, const std::wstring &message)
	{
		time_t current_time;
		time(&current_time);

		std::wostringstream line;
		line << date_helpers::local_time_to_str(current_time, "%Y/%m/%d %H:%M:%S").c_str();
		line << ' ' << GetCurrentThreadId();
		line << ' ' << severity_to_str(sev).c_str() << ' ' << message;

		return line.str();
	}

	void log_message(severity sev, const std::wstring & message)
	{
		check_lazy_initialization_required();

		if (history_logger_initialized)
		{
			write_to_issue_history_log(sev, message);
		}

		if (cpplogger::get_current_logger_settings().use_developer_log)
		{
			std::wstring format_message = format_log_message(sev,message);
			cpplogger::log_message(severity_to_log_level(sev), format_message);
		}

#ifdef _DEBUG		
		if (!cpplogger::get_current_logger_settings().use_developer_log)
		{
			std::wstring format_message = format_log_message(sev, message);
			OutputDebugStringW((format_message+L'\n').c_str());
		}
#endif // _DEBUG		
	}

	void initialize(const std::wstring &config,
					 const std::wstring &log_path,
					 const std::wstring &log_file_prefix,
					 bool watch_config_changes,
					 const notify_developers_logging_enabled_func_t & notify_developers_logging_enabled_func)
	{
		sync_helpers::call_once(
			init_once,
			do_init,
			config,
			log_path,
			log_file_prefix,
			watch_config_changes,
			notify_developers_logging_enabled_func);
	}

	void set_lazy_initialization(
		const std::wstring &config,
		const std::wstring &log_path,
		const std::wstring &log_file_prefix,
		bool watch_config_changes,
		const notify_developers_logging_enabled_func_t & notify_developers_logging_enabled_func)
	{
		auto lazy_initializer = [config, log_path, log_file_prefix, watch_config_changes, notify_developers_logging_enabled_func]()
		{
			initialize(config, log_path, log_file_prefix, watch_config_changes, notify_developers_logging_enabled_func);
		};

		sync_helpers::call_once(
			lazy_initializer_was_set,
			[&lazy_initializer]
			{
				{
					std::lock_guard<decltype(lazy_initializer_func_mtx)> lock(lazy_initializer_func_mtx);
					lazy_initializer_func = std::move(lazy_initializer);
				}
				lazy_initialization_required = true;
			});
	}

	void deinitialize()
	{
		history_logger_initialized = false;
		cpplogger::free_logger_instance();
	}

	void reload_logging_settings()
	{
		check_lazy_initialization_required();
		cpplogger::reload_settings_from_config();
	}

	std::wstring get_log_path()
	{
		check_lazy_initialization_required();
		return cpplogger::get_log_path();
	}

	std::wstring get_log_name_prefix()
	{
		check_lazy_initialization_required();
		return cpplogger::get_log_prefix();
	}

	std::wstring to_elapsed_str(const std::chrono::steady_clock::time_point & start, const std::chrono::steady_clock::time_point & end)
	{
		std::wostringstream ss;
		const auto elapsed_microsec = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		if (elapsed_microsec < 1000)
		{
			ss << elapsed_microsec << " microsec";
		}
		else
		{
			const auto elapsed_ms = elapsed_microsec / 1000;
			if (elapsed_ms < 1000)
			{
				ss << elapsed_ms << " ms";
			}
			else
			{
				const auto elapsed_sec = elapsed_ms / 1000;
				ss << elapsed_sec << " sec";
			}
		}

		return ss.str();
	}

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

	void log_stream::flush()
	{
		if (_ss.str().empty())
		{
			return;
		}

		log_message(_severity, _ss.str());
	}

}