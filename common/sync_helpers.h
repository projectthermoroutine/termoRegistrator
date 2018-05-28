#pragma once

#ifdef max
#undef max
#endif //max

#ifdef min
#undef min
#endif //min

#ifndef NOMINMAX
#define NOMINMAX
#endif //NOMINMAX

#include <common\handle_holder.h>
#include <common\on_exit.h>
#include <limits>
#include <Windows.h>

namespace sync_helpers
{
	struct wait_result
	{
		bool event_raised = false;
		std::size_t event_index = std::numeric_limits<std::size_t>::max();
	};

	void create_random_name(std::wstring & gen_name, bool is_global = true);
	handle_holder create_basic_semaphore_object(int32_t initial_count, int32_t max_count = MAXLONG);
	handle_holder create_basic_event_object(bool manual_reset);
	handle_holder create_basic_manual_reset_event();
	handle_holder create_random_name_event(std::wstring & event_name, bool isManualReset = false, bool initState = false, bool is_global = false, const SECURITY_ATTRIBUTES * p_sec_attr = nullptr);
	handle_holder open_event_for_sync(const std::wstring & event_name);
	handle_holder open_event_for_sync(const std::string & event_name);
	handle_holder open_event_object(const std::wstring & event_name, DWORD desired_access = GENERIC_ALL);
	handle_holder open_event_object(const std::string & event_name, DWORD desired_access = GENERIC_ALL);
	handle_holder open_process_for_sync(DWORD pid);
	void set_event(const handle_holder & event);
	void set_event(HANDLE event);
	void reset_event(const handle_holder & event);
	void release_semaphore(const handle_holder & semaphore, int32_t release_count = 1);
	bool wait(const handle_holder & object, DWORD time_to_wait = INFINITE);
	bool wait(const HANDLE handle, DWORD time_to_wait = INFINITE);
	wait_result wait_any(const HANDLE * first, std::size_t num_of_elements, DWORD time_to_wait = INFINITE);
	bool is_event_set(const handle_holder & event);
	bool is_event_set(const HANDLE handle);

	class scoped_thread_priority final
	{
	public:
		explicit scoped_thread_priority(int priority);
		~scoped_thread_priority();

		scoped_thread_priority(const scoped_thread_priority &) = delete;
		scoped_thread_priority & operator = (const scoped_thread_priority &) = delete;
	private:
		int _prev_priority;
	};

	class scoped_thread_background_priority final
	{
	public:
		scoped_thread_background_priority();
		~scoped_thread_background_priority();

		scoped_thread_background_priority(const scoped_thread_background_priority &) = delete;
		scoped_thread_background_priority & operator = (const scoped_thread_background_priority &) = delete;
	};

	class rw_lock final
	{
	public:
		rw_lock();

		void lock(bool exclusive);
		void unlock(bool exclusive);

		rw_lock(const rw_lock &) = delete;
		rw_lock & operator = (const rw_lock &) = delete;
	private:
		SRWLOCK _lock;
	};

	template <bool exclusive>
	class rw_lock_guard final
	{
	public:
		explicit rw_lock_guard(rw_lock & lock) : _lock(lock)
		{
			_lock.lock(exclusive);
		}

		~rw_lock_guard()
		{
			_lock.unlock(exclusive);
		}

		rw_lock_guard(const rw_lock_guard &) = delete;
		rw_lock_guard & operator = (const rw_lock_guard &) = delete;
	private:
		rw_lock & _lock;
	};

	using rw_lock_guard_shared = rw_lock_guard<false>;
	using rw_lock_guard_exclusive = rw_lock_guard<true>;

	class once_flag final
	{
	public:
		once_flag();
		~once_flag();

		once_flag(const once_flag &) = delete;
		once_flag & operator = (const once_flag &) = delete;

		template<class func_t, class... args_t>
		friend void call_once(once_flag & flag, func_t && func, args_t && ... args);
	private:
		CRITICAL_SECTION sec;
		volatile bool completed;
		volatile LONG enter_num;
	};

	template<class func_t, class... args_t>
	inline void call_once(once_flag & flag, func_t && func, args_t && ... args)
	{
		if (flag.completed) return;

		const auto leave_func = [&flag] { --flag.enter_num;  LeaveCriticalSection(&flag.sec); };
		utils::on_exit exit_guard(leave_func);

		EnterCriticalSection(&flag.sec);
		++flag.enter_num;

		if (flag.enter_num > 1)
		{
			// prevent recursion
			return;
		}

		if (flag.completed) return;
		func(args...);
		MemoryBarrier();
		flag.completed = true;
	}
} // namespace sync_helpers