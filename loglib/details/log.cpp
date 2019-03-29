#include "log.h"
#include <future>
#include <fstream>
#include <memory>
#include <mutex>
#include <atomic>
#include <list>
#include <utility>
#include <sstream>
#include <cassert>
#include <chrono>
#include <codecvt>
#include <map>

#define NOGDI
#include <Windows.h>

#include <common\locale.hpp>
#include <common\sync_helpers.h>
#include <common\date_helpers.h>
#include <error_lib\error_codes.h>
#include <error_lib\application_exception.h>
#include <common\log_and_throw.h>
#include <common\unhandled_exception.h>

#include "cpplogger.h"
#include "log_details.h"


#if defined(_MSC_VER) && _MSC_VER < 1900
#	define thread_local __declspec(thread)
#endif 


namespace cpplogger
{
	struct instance_t;
	const instance_t& logger_instance();
	bool set_logger_instance(instance_t*) noexcept;
}

namespace
{
//	thread_local std::size_t scope_logger_level_from_this_thread = 0;

	std::uint64_t get_max_messages_size();

	using thread_id_t = decltype(GetCurrentThreadId());
	using process_id_t = decltype(GetCurrentProcessId());
	auto const skipped_message = std::wstring{ L"Message was skipped" };

	class thread_log_history final
	{
	public:

		thread_log_history() : _messages_size(0), _last_insert_time(), _scope_level(0) {}
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

		thread_log_history(thread_log_history && src) : _messages(std::move(src._messages)), _messages_size(src._messages_size), _scope_level(src._scope_level)
		{}

		thread_log_history & operator = (thread_log_history && src)
		{
			_messages = std::move(src._messages);
			_messages_size = src._messages_size;
			_scope_level = src._scope_level;
			return *this;
		}

		std::size_t scope_level() const { return _scope_level; }
		void inc_scope_level() { ++_scope_level; }
		void dec_scope_level() { --_scope_level; }

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
		std::size_t _scope_level;
	};


	void dump_history(
		process_id_t process_id,
		thread_id_t thread_id,
		const thread_log_history & history,
		logger::level sev,
		thread_id_t causer);

	__declspec(noinline) static bool instance_exists()
	{
		return true;
	}

	class instance
	{
	private:

		template <void* _Address, std::size_t _Size>
		static bool set_dummy_on(const std::uint8_t asm_code[_Size])
		{
			void* const address(_Address);
			std::size_t const size(_Size);

			DWORD protect(PAGE_READWRITE);
			if (0 == ::VirtualProtect(address, size, protect, &protect))
				return false;

			std::memcpy(address, asm_code, size);
			::VirtualProtect(address, size, protect, &protect);

			return true;
		}

	public:
		instance() 
			: m_extern_instance_ptr(nullptr)
			
		{}

		~instance()
		{
			const std::uint8_t _asm_return[] = { 0x32, 0xC0, 0xC3, 0x90 };
			set_dummy_on<(void*)::instance_exists, sizeof(_asm_return)>(_asm_return);
		}

	public:

		inline void reset() 
		{
			clear_history_log();
		}

		void reset_extern_instance() noexcept
		{
			m_extern_instance_ptr = nullptr;
		}

		bool set_extern_instance(instance* extr_instance) noexcept
		{
			if (m_extern_instance_ptr || !extr_instance || extr_instance == this)
				return false;

			m_extern_instance_ptr = extr_instance;

			try
			{
				clear_history_log_itself();
			}
			catch (...)
			{
				// WTF
			}

			return true;
		}

	public:

		std::size_t get_scoped_logger_level_for_current_thread()
		{
			if (m_extern_instance_ptr) 
				return m_extern_instance_ptr->get_scoped_logger_level_for_current_thread();

			auto const thread_id = GetCurrentThreadId();
			std::lock_guard<decltype(thread_history_mx)> lock(thread_history_mx);
			auto hist_iter = thread_histories.find(thread_id);
			if (hist_iter == thread_histories.end())
				return 0;

			return hist_iter->second.scope_level();
		}

		void inc_scope_level() 
		{ 
			if (m_extern_instance_ptr)
				return m_extern_instance_ptr->inc_scope_level();

			auto const thread_id = GetCurrentThreadId();
			std::lock_guard<decltype(thread_history_mx)> lock(thread_history_mx);
			auto hist_iter = thread_histories.find(thread_id);
			if (hist_iter != thread_histories.end())
				hist_iter->second.inc_scope_level();
		}
		void dec_scope_level() 
		{
			if (m_extern_instance_ptr)
				return m_extern_instance_ptr->dec_scope_level();

			auto const thread_id = GetCurrentThreadId();
			std::lock_guard<decltype(thread_history_mx)> lock(thread_history_mx);
			auto hist_iter = thread_histories.find(thread_id);
			if (hist_iter != thread_histories.end())
				hist_iter->second.dec_scope_level();
		}

		void cleanup_stale_history()
		{
			if (m_extern_instance_ptr) {
				m_extern_instance_ptr->cleanup_stale_history();
				return;
			}

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
			if (m_extern_instance_ptr)
				m_extern_instance_ptr->clear_history_log();
			else
				clear_history_log_itself();
		}

		void write_to_issue_history_log(const cpplogger::message& msg)
		{
			using namespace logger;

			if (m_extern_instance_ptr) {
				m_extern_instance_ptr->write_to_issue_history_log(msg);
				return;
			}

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

	public:
		std::size_t get_thread_history_log_count_messages(thread_id_t id)
		{
			if (!m_extern_instance_ptr)
			{
				const auto find_iter(thread_histories.find(id));
				return find_iter != thread_histories.end() ? find_iter->second.get_messages_count() : std::size_t(0);
			}

			return m_extern_instance_ptr->get_thread_history_log_count_messages(id);
		}

		std::wstring get_last_insert_message_in_history(thread_id_t id, logger::level* sev_ptr)
		{
			if (m_extern_instance_ptr)
				return m_extern_instance_ptr->get_last_insert_message_in_history(id, sev_ptr);


			std::lock_guard<std::mutex> lk(thread_history_mx);

			const auto it(thread_histories.find(id));
			if (it == std::end(thread_histories))
				return std::wstring();

			return it->second.get_last_insert_message(sev_ptr);
		}

private:
		void clear_history_log_itself()
		{
			std::lock_guard <decltype(thread_history_mx)> lock(thread_history_mx);

			for (std::pair<const thread_id_t, thread_log_history>& pair : thread_histories)
				pair.second.clear();
		}

	private:
		std::mutex thread_history_mx;
		std::map<thread_id_t, thread_log_history> thread_histories;
		std::chrono::steady_clock::time_point last_history_clean = std::chrono::steady_clock::now();
		const std::chrono::steady_clock::duration clean_period = std::chrono::minutes(1);

		instance * m_extern_instance_ptr;
	};

	instance g_instance;

}//namespace anonymous

namespace logger
{
	struct instance_t
	{
		instance& instance_impl_ref;
		cpplogger::instance_t * logger_instance_ptr;
	};

	instance_t g_instance_wrapper = { g_instance, nullptr };

	const instance_t& logger_instance()
	{
		g_instance_wrapper.logger_instance_ptr = const_cast<cpplogger::instance_t *>(&(cpplogger::logger_instance()));
		return g_instance_wrapper;
	}

	bool set_logger_instance(const instance_t& instance) noexcept
	{
		if (!g_instance.set_extern_instance(&instance.instance_impl_ref))
			return false;

		if (!cpplogger::set_logger_instance(instance.logger_instance_ptr))
		{
			g_instance.reset_extern_instance();
			return false;
		}

		return true;
	}

}//namespace logger

namespace
{
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
}//namespace anonymous

namespace logger
{
	using namespace std::string_view_literals;

	namespace details
	{
		namespace sc_ass
		{
			static std::atomic<bool> gs_flag = true;

			bool _set_is_use_crt_wassert(bool value)
			{
				return gs_flag.exchange(value);
			}

			bool _is_use_crt_wassert()
			{
				return gs_flag.load();
			}

			[[noreturn]] void _wassert(wchar_t const* _Message, wchar_t const* _File, unsigned _Line)
			{
				std::wostringstream ss;
				ss << std::endl << L"================================================================================";
				ss << std::endl << L"Security Code assertion failed!";
				ss << std::endl << L"--------------------------------------------------------------------------------";
				ss << std::endl;
				ss << std::endl << L"File: " << _File;
				ss << std::endl << L"Line: " << _Line;
				ss << std::endl;
				ss << std::endl << L"Expression: " << _Message;
				ss << std::endl;
				ss << std::endl << L"================================================================================";
				::logger::log_message(::logger::level::fatal, ss.str());

				unhandled_exception_handler::not_create_zip_at_terminate();
				std::terminate();
			}

			void _impossible_logical_branch(wchar_t const* _Message, wchar_t const* _File, unsigned _Line)
			{
				std::wostringstream ss;
				ss << std::endl << L"================================================================================";
				ss << std::endl << L"Security Code assertion failed!";
				ss << std::endl << L"--------------------------------------------------------------------------------";
				ss << std::endl;
				ss << std::endl << L"File: " << _File;
				ss << std::endl << L"Line: " << _Line;
				ss << std::endl;
				ss << std::endl << L"Impossible logical branch: " << _Message;
				ss << std::endl;
				ss << std::endl << L"================================================================================";
				::logger::log_message(::logger::level::error, ss.str());
			}

		} // namespace sc_ass

		std::size_t get_thread_history_log_count_messages(thread_id_t id)
		{
			return g_instance.get_thread_history_log_count_messages(id);
		}

		std::wstring get_last_insert_message_in_history(thread_id_t id, logger::level* sev_ptr)
		{
			return g_instance.get_last_insert_message_in_history(id, sev_ptr);
		}

		void fill_what_category(std::exception_ptr except, std::string& what, std::string* category, bool rethrow_is_nested)
		{
			assert(except);

			try
			{
				std::rethrow_exception(except);
			}
			catch (const std::future_error& exc)
			{
				what.assign(exc.what());

				if (category)
					category->assign(exc.code().category().name());

				if (rethrow_is_nested)
					std::rethrow_if_nested(exc);
			}
			catch (const std::system_error& exc)
			{
				what.assign(exc.what());
				
				if (category)
					category->assign(exc.code().category().name());

				if (rethrow_is_nested)
					std::rethrow_if_nested(exc);
			}
			catch (const ::common::application_exception& exc)
			{
				what.assign(common::wstring_convert<wchar_t>().to_bytes(exc.localized_wwhat()));

				if (category)
					category->assign(exc.code().category().name());

				if (rethrow_is_nested)
					std::rethrow_if_nested(exc);
			}
			catch (const std::exception& exc)
			{
				what.assign(exc.what());
				
				if (category)
					category->clear();

				if (rethrow_is_nested)
					std::rethrow_if_nested(exc);
			}
			catch (...)
			{
				what.assign("<unknown>");
				
				if (category)
					category->clear();
			}
		}

	} // namespace details

	void __cdecl log_message(level lvl, const std::wstring & text)
	{
		if (!instance_exists())
			return;

		cpplogger::message message(cpplogger::message::make(lvl, g_instance.get_scoped_logger_level_for_current_thread(), std::wstring(text)));

		if (!instance_exists())
			return;

		g_instance.write_to_issue_history_log(message);

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

		g_instance.clear_history_log();

#ifdef _DEBUG
		if (cpplogger::get_current_logger_settings().use_developer_log)
			OutputDebugStringW(L"Developers log is initialized\n");
#endif // _DEBUG		

		return true;
	}

	void deinitialize() noexcept
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
		return make_str_delta_time(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - time));
	}

	std::wstring scope_logger::make_str_delta_time(const std::chrono::microseconds& delta)
	{
		auto raw_delta(delta.count());
		if (raw_delta < 1000)
			return std::to_wstring(raw_delta).append(L" microsec");

		raw_delta /= 1000;
		if (raw_delta < 1000)
			return std::to_wstring(raw_delta).append(L" ms");

		raw_delta /= 1000;
		if (raw_delta < 60)
			return std::to_wstring(raw_delta).append(L" sec");

		auto elapsed_minutes(raw_delta / 60);
		raw_delta %= 60;

		return std::to_wstring(elapsed_minutes).append(L" min, ").append(std::to_wstring(raw_delta).append(L" sec"));
	}

	scope_logger::scope_logger(std::wstring_view func_name, std::wstring_view file_name, int line_number)
		: m_func_name(func_name)
		, m_start()
	{
		std::wostringstream ss;
		ss << (0 != std::uncaught_exceptions() ? cpplogger::prefix_function::entry_exception : cpplogger::prefix_function::entry) << ' ';
		ss << func_name << L", file: "sv << file_name << L", line: "sv << line_number;

		log_message(level::debug, ss.str());

		if (instance_exists())
			g_instance.inc_scope_level();

		m_start = std::chrono::steady_clock::now();
	}

	scope_logger::~scope_logger()
	{
		const std::wstring elapsed_time(make_str_elapsed_time(m_start));

		std::wostringstream ss;
		ss << (0 != std::uncaught_exceptions() ? cpplogger::prefix_function::exit_exception : cpplogger::prefix_function::exit) << ' ';
		ss << m_func_name << L", elapsed: "sv << elapsed_time;

		if (instance_exists())
			g_instance.dec_scope_level();

		log_message(level::debug, ss.str());
	}


	
	scope_logger_ex::scope_logger_ex(std::wstring&& scope_name, std::wstring_view file_name, int line_number)
		: m_scope_name(std::move(scope_name))
		, m_start()
	{
		std::wostringstream ss;
		ss << (0 != std::uncaught_exceptions() ? cpplogger::prefix_function::entry_exception : cpplogger::prefix_function::entry) << ' ';
		ss << m_scope_name << L", file: "sv << file_name << L", line: "sv << line_number;

		log_message(level::debug, ss.str());

		if (instance_exists())
			g_instance.inc_scope_level();

		m_start = std::chrono::steady_clock::now();
	}

	scope_logger_ex::scope_logger_ex(const std::wstring& scope_name, std::wstring_view file_name, int line_number)
		: scope_logger_ex(std::wstring(scope_name), file_name, line_number)
	{}

	scope_logger_ex::scope_logger_ex(const std::string& scope_name, std::wstring_view file_name, int line_number)
		: scope_logger_ex(common::wstring_convert<wchar_t>().from_bytes(scope_name), file_name, line_number)
	{

	}

	scope_logger_ex::scope_logger_ex(scope_logger_ex&& other)
		: m_scope_name(std::move(other.m_scope_name))
		, m_start(std::move(other.m_start))
	{
	}

	scope_logger_ex::~scope_logger_ex()
	{
		if (!m_scope_name.empty())
		{
			const std::wstring elapsed_time(scope_logger::make_str_elapsed_time(m_start));

			std::wostringstream ss;
			ss << (0 != std::uncaught_exceptions() ? cpplogger::prefix_function::exit_exception : cpplogger::prefix_function::exit) << ' ';
			ss << m_scope_name << ", elapsed: " << elapsed_time;

			if (instance_exists())
				g_instance.dec_scope_level();

			log_message(level::debug, ss.str());
		}
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
		_ss << common::wstring_convert<wchar_t>().from_bytes(ref);
		return *this;
	}

	log_stream & log_stream::operator<<(const char* ref)
	{
		_ss << (ref ? common::wstring_convert<wchar_t>().from_bytes(ref) : common::wstring_convert<wchar_t>().from_bytes('\0'));
		return *this;
	}

	log_stream & log_stream::operator<<(const std::string& ref)
	{
		_ss << common::wstring_convert<wchar_t>().from_bytes(ref);
		return *this;
	}

	log_stream & log_stream::operator<<(std::string_view str_view)
	{
		_ss << common::wstring_convert<wchar_t>().from_bytes(str_view.data(), str_view.size());
		return *this;
	}

	void log_stream::flush()
	{
		const std::wstring str(_ss.str());
		if (!str.empty())
			log_message(_lvl, str);
	}

	std::string except_what_to_string(std::exception_ptr except, std::string* category)
	{
		std::string result;
		details::fill_what_category(except, result, category, false/*not_rethrow_is_nested*/);
		return result;
	}

	std::wstring except_what_to_wstring(std::exception_ptr except, std::wstring* category)
	{
		std::string result;

		if (category)
		{
			std::string tmp_category;
			details::fill_what_category(except, result, &tmp_category, false/*not_rethrow_is_nested*/);
			*category = string_utils::convert_utf8_to_wchar(tmp_category);
		}
		else
		{
			details::fill_what_category(except, result, nullptr, false/*not_rethrow_is_nested*/);
		}

		return string_utils::convert_utf8_to_wchar(result);
	}

	std::deque<std::string> chain_of_except_what_to_strings(std::exception_ptr except, std::deque<std::string>* categories)
	{
		std::deque<std::string> result;

		std::string what;
		std::string category;

		while (except)
		{
			try
			{
				details::fill_what_category(except, what, (categories ? &category : nullptr), true/*rethrow_is_nested*/);
				except = nullptr;
			}
			catch (...)
			{
				except = std::current_exception();
			}

			result.push_back(std::move(what));

			if (categories)
				categories->push_back(std::move(category));
		}

		return result;
	}

	std::deque<std::wstring> chain_of_except_what_to_wstrings(std::exception_ptr except, std::deque<std::wstring>* categories)
	{
		std::deque<std::string> categories_a;
		std::deque<std::string> result_a{ chain_of_except_what_to_strings(except, (categories ? &categories_a : nullptr)) };

		std::deque<std::wstring> result;
		result.resize(result_a.size());

		std::transform(
			std::begin(result_a),
			std::end(result_a),
			std::begin(result),
			[](const std::string& message) { return string_utils::convert_utf8_to_wchar(message); });

		if (categories)
		{
			categories->resize(categories_a.size());
			std::transform(
				std::begin(categories_a),
				std::end(categories_a),
				std::begin(*categories),
				[](const std::string& category) { return string_utils::convert_utf8_to_wchar(category); });
		}

		return result;
	}

	std::wstring log_exception(std::exception_ptr except, logger::level level, const std::wstring & head_message)
	{
		std::deque<std::wstring> categories;
		std::deque<std::wstring> errors{ chain_of_except_what_to_wstrings(except, &categories) };

		assert(!errors.empty());
		assert(errors.size() == categories.size());

		std::wostringstream ss;

		if (1 == errors.size())
		{
			ss << L" [error: " << errors.front() << L']';
			if (!categories.front().empty())
				ss << L" [category: " << categories.front() << L']';
		}
		else
		{
			std::wstring tab;
			auto it_text{ std::begin(errors) };
			auto it_category{ std::begin(categories) };
			const auto it_text_end{ std::end(errors) };
			for (; it_text != it_text_end; ++it_text, ++it_category, tab += L"  ")
			{
				assert(it_category != std::end(categories));

				ss << L"\n" << tab << L" [error: " << *it_text << L']';
				if (!it_category->empty())
					ss << L" [category: " << (*it_category) << L']';
			}
		}

		LOG(level) << head_message << ss.str();

		return std::move(errors.front());
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
