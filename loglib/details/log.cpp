#include <log.h>
#include <fstream>
#include <mutex>
#include <list>
#include <utility>
#include <sstream>
#include <chrono>
#include <random>


#define NOGDI
#include <Windows.h>

#include <log4cplus\logger.h>
#include <log4cplus\loglevel.h>
#include <log4cplus\configurator.h>

#include <common\path_helpers.h>
#include <common\sync_helpers.h>
#include <common\date_helpers.h>
#include <common\fs_helpers.h>



namespace
{
	unsigned int get_max_messages_size();
	void cleanup_stale_history();

	log4cplus::LogLevel severity_to_log_level(logger::severity sev)
	{
		using namespace logger;

		switch (sev)
		{
		case severity::trace: return log4cplus::TRACE_LOG_LEVEL;
		case severity::debug: return log4cplus::DEBUG_LOG_LEVEL;
		case severity::info: return log4cplus::INFO_LOG_LEVEL;
		case severity::warn: return log4cplus::WARN_LOG_LEVEL;
		case severity::error: return log4cplus::ERROR_LOG_LEVEL;
		case severity::fatal: return log4cplus::FATAL_LOG_LEVEL;
		}

		throw std::invalid_argument("The passed argument sev is invalid");
	}

	using thread_id_t = decltype(GetCurrentThreadId());
	using process_id_t = decltype(GetCurrentProcessId());

	class thread_log_history final
	{
	public:
		struct message_info
		{
			logger::severity sev;
			std::chrono::system_clock::time_point time;
			std::wstring message;
		};

		thread_log_history() : _messages_size(0) {}

		void insert_message(logger::severity sev, const std::wstring& message)
		{
			if (message.empty())
			{
				return;
			}

			auto const time = std::chrono::system_clock::now();

			auto const new_message_mem_size = calc_message_memory_size(message);
			auto const max_messages_size = get_max_messages_size();
			while (_messages_size + new_message_mem_size > static_cast<std::size_t>(max_messages_size))
			{
				auto reclaimed_mem_size = calc_message_memory_size(_messages.front().message);
				_messages.pop_front();
				_messages_size -= reclaimed_mem_size;
			}

			_messages.push_back({ sev, time, message });
			_messages_size += new_message_mem_size;

			_last_insert_time = std::chrono::steady_clock::now();
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
			_messages.clear();
			_messages_size = 0;
		}

		bool empty() const
		{
			return _messages.empty();
		}

		std::chrono::steady_clock::time_point get_last_insert_time() const
		{
			return _last_insert_time;
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
		}
	private:
		static std::size_t calc_message_memory_size(const std::wstring & message)
		{
			//  V119 More than one sizeof() operator is used in one expression. log.cpp 126
			return message.size() * sizeof(std::wstring::value_type) + sizeof(message_info); //-V119
		}

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
			}
		}

		cleanup_stale_history();
	}

	void cleanup_stale_history()
	{
		auto const now = std::chrono::steady_clock::now();
		if (now >= last_history_clean + clean_period)
		{
			auto const local_clean_period = clean_period; // to avoid lambda limitations

			std::unique_lock<std::mutex> lock(thread_history_mx);
			for (auto iter = thread_histories.begin(); iter != thread_histories.end(); )
			{
				if (iter->second.get_last_insert_time() + clean_period >= now && !iter->second.empty())
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

		std::wofstream log_file(file_name, std::ios_base::out | std::ios_base::app);
		if (!log_file.is_open())
		{
			return;
		}

		history.for_each_message(
			[&log_file](const thread_log_history::message_info & message_info)
		{
			log_file << date_helpers::local_time_to_str(std::chrono::system_clock::to_time_t(message_info.time), "%Y/%m/%d %H:%M:%S").c_str();
			log_file << ' ' << logger::severity_to_str(message_info.sev).c_str() << ' ' << message_info.message << std::endl;
		}
		);
	}

	sync_helpers::once_flag init_once;

	const unsigned int default_max_log_buffer_size = 1024 * 1024; // 1 MB
	const unsigned int max_log_buffer_size_limit = 16 * 1024 * 1024; // 16 MB

	logger::logging_settings_t logging_settings{};
	bool history_logger_initialized = false;	

	void init_history_log(
		std::istream & input_stream,
		const std::wstring & logs_path,
		const std::wstring & log_name_prefix)
	{
		UNREFERENCED_PARAMETER(input_stream);
		UNREFERENCED_PARAMETER(logs_path);
		UNREFERENCED_PARAMETER(log_name_prefix);
		history_logger_initialized = true;
	}

	void do_init(
		const std::wstring & log_config,
		bool  developers_log,
		uint64_t max_log_buufer_size, 
		const std::wstring & logs_path,
		const std::wstring & log_name)
	{
		static const wchar_t log_file_path_property_name[] = L"log4cplus.appender.ROLLING.File";


		OutputDebugStringW(L"Initializing history log.\n");

		logging_settings = { developers_log, (unsigned int)max_log_buufer_size, logs_path, log_name };
		history_logger_initialized = true;

		if (logging_settings.enable_developers_log)
		{
			OutputDebugStringW(L"Initializing developers log.\n");

			std::wstringstream log_config_stream;
			log_config_stream << log_config;
			
			log4cplus::Logger::shutdown();
			log4cplus::helpers::Properties properties(log_config_stream);

			properties.setProperty(log_file_path_property_name, path_helpers::concatenate_paths(logs_path, log_name) + L".log");
			
			log4cplus::PropertyConfigurator configurator(properties);

			
			log4cplus::initialize();

			configurator.configure();
		}

		OutputDebugStringW(L"Leaving do_init.\n");
	}

	unsigned int get_max_messages_size()
	{
		return logging_settings.max_log_buffer_size;
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

	auto const developers_log_key = "developers_log";
	auto const max_log_buffer_size_key = "max_log_buffer_size";

	void write_logging_settings(std::ostream & output_stream, const logging_settings_t& settings)
	{
		//  TODO: simple realization without Poco
		output_stream << "{\r\n";
		output_stream << "\t\"" << developers_log_key << "\" : " << std::boolalpha << settings.enable_developers_log << ",\r\n";
		output_stream << "\t\"" << max_log_buffer_size_key << "\" : " << settings.max_log_buffer_size << "\r\n";
		output_stream << "}";
	}

	logger::logging_settings_t current_logging_settings()
	{
		return logging_settings;
	}

	void log_message(severity sev, const std::wstring & message)
	{
		if (history_logger_initialized)
		{
			write_to_issue_history_log(sev, message);
		}

		if (logging_settings.enable_developers_log)
		{
			log4cplus::Logger::getRoot().log(severity_to_log_level(sev), message);
		}

#ifdef _DEBUG		
		if (!logging_settings.enable_developers_log)
		{
			time_t current_time;
			time(&current_time);

			std::wostringstream line;
			line << date_helpers::local_time_to_str(current_time, "%Y/%m/%d %H:%M:%S").c_str();
			line << ' ' << GetCurrentThreadId();
			line << ' ' << severity_to_str(sev).c_str() << ' ' << message << std::endl;

			OutputDebugStringW(line.str().c_str());
		}
#endif // _DEBUG		
	}

	void initialize(
		const std::wstring & log_config,
		bool  developers_log,
		uint64_t max_log_buufer_size, 
		const std::wstring & logs_path,
		const std::wstring & log_name
		)
	{
		sync_helpers::call_once(
			init_once,
			do_init,
			log_config,
			developers_log,
			max_log_buufer_size,
			logs_path,
			log_name);
	}

	void threadCleanup()
	{
		if (history_logger_initialized){
			history_logger_initialized = false;
			logging_settings.enable_developers_log = false;


			//log4cplus::Logger::shutdown();
			//log4cplus::threadCleanup();
			//std::unique_lock<std::mutex> lock(thread_history_mx);
			//thread_histories.clear();
		}
	}

	std::wstring get_log_path()
	{
		return logging_settings.log_path;
	}

	std::wstring get_log_name_prefix()
	{
		return logging_settings.log_name_prefix;
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

	bool developers_log_enabled()
	{
		return logging_settings.enable_developers_log;
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