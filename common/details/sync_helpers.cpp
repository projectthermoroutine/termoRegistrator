#include <common\sync_helpers.h>
#include <common\handle_holder.h>
#include <common\on_exit.h>
#include <common\xor_shift.h>
#include <loglib\log.h>
#include <Windows.h>

#include <vector>

namespace sync_helpers
{
	void create_random_name(std::wstring & gen_name, bool is_global)
	{
		LOG_STACK();

		std::wstring name;

		uint64_t rnd = 0;
		{
			LARGE_INTEGER high_resolution_value{};
			if (QueryPerformanceCounter(&high_resolution_value))
			{
				rnd = high_resolution_value.QuadPart;
			}
			else
			{
				rnd = GetTickCount64();
			}
		}

		if (rnd == 0)
		{
			rnd = 1;
		}

		rnd = utils::xor_shift_next(rnd);
		auto prefix = L"Local\\";
		if (is_global)
			prefix = L"Global\\";
		name = prefix + std::to_wstring(rnd);
		gen_name = name;
	}

	handle_holder create_basic_semaphore_object(int32_t initial_count, int32_t max_count)
	{
		LOG_STACK();

		auto event = CreateSemaphoreW(NULL, initial_count, max_count, NULL);
		if (event == 0)
		{
			auto last_error = GetLastError();
			LOG_DEBUG() << "Could not create semaphore object: " << std::hex << std::showbase << last_error;
			throw std::runtime_error("Could not create semaphore object.");
		}

		return handle_holder(event);
	}


	handle_holder create_basic_event_object(bool manual_reset)
	{
		LOG_STACK();

		auto event = CreateEventW(NULL, manual_reset ? TRUE : FALSE, FALSE, NULL);
		if (event == 0)
		{
			auto last_error = GetLastError();
			LOG_DEBUG() << "Could not create event object: " << std::hex << std::showbase << last_error;
			throw std::runtime_error("Could not create event object.");
		}

		return handle_holder(event);
	}

	handle_holder create_random_name_event(std::wstring & event_name,bool isManualReset,bool initState, bool is_global, const SECURITY_ATTRIBUTES * p_sec_attr)
	{
		LOG_STACK();

		std::wstring name;

		uint64_t rnd = 0;
		{
			LARGE_INTEGER high_resolution_value{};
			if (QueryPerformanceCounter(&high_resolution_value))
			{
				rnd = high_resolution_value.QuadPart;
			}
			else
			{
				rnd = GetTickCount64();
			}
		}

		if (rnd == 0)
		{
			rnd = 1;
		}

		for (;;)
		{
			rnd = utils::xor_shift_next(rnd);
			auto prefix = L"Local\\";
			if (is_global)
				prefix = L"Global\\";
			name = prefix + std::to_wstring(rnd);

			auto const handle = CreateEventW(const_cast<LPSECURITY_ATTRIBUTES>(p_sec_attr), (BOOL)isManualReset, (BOOL)initState, name.c_str());
			if (handle == 0)
			{
				auto const last_error = GetLastError();
				if (last_error == ERROR_ALREADY_EXISTS) continue;
				LOG_DEBUG() << "Could not create an event object, error: " << std::hex << std::showbase << last_error;
				throw std::runtime_error("Could not create an event object.");
			}
			else
			{
				LOG_TRACE() << "Event object was created with name: " << name.c_str();
				event_name = name;
				return handle;
			}
		}
	}

	handle_holder open_event_object(const std::wstring & event_name, DWORD desired_access)
	{
		handle_holder event_handle(OpenEventW(desired_access, FALSE, event_name.c_str()));
		if (!event_handle)
		{
			auto const last_error = GetLastError();
			LOG_DEBUG() << "Could not open event with name: " << event_name.c_str() << " , error: " << std::hex << std::showbase << last_error;
			throw std::runtime_error("Could not get an event's handle.");
		}
		return event_handle;
	}

	handle_holder open_event_object(const std::string & event_name, DWORD desired_access)
	{
		handle_holder event_handle(OpenEventA(desired_access, FALSE, event_name.c_str()));
		if (!event_handle)
		{
			//auto const last_error = GetLastError();
			//LOG_DEBUG() << "Could not open event with name: " << event_name << " , error: " << std::hex << std::showbase << last_error;
			throw std::runtime_error("Could not get an event's handle.");
		}
		return event_handle;
	}


	handle_holder open_event_for_sync(const std::wstring & event_name)
	{
		return open_event_object(event_name, SYNCHRONIZE);
	}

	handle_holder open_event_for_sync(const std::string & event_name)
	{
		return open_event_object(event_name, SYNCHRONIZE);
	}

	handle_holder open_process_for_sync(DWORD pid)
	{
		handle_holder process_handle(OpenProcess(SYNCHRONIZE, FALSE, pid));
		if (!process_handle)
		{
			auto const last_error = GetLastError();
			LOG_DEBUG() << "Could not open process with pid: " << pid << " , error: " << std::hex << std::showbase << last_error;
			throw std::runtime_error("Could not open a process for sync.");
		}
		return process_handle;
	}

	void set_event(const handle_holder & event)
	{
		LOG_STACK();

		auto const handle = event.get();
		if (handle == 0)
			throw std::invalid_argument("The passed argument event can't be equal to 0");

		if (!SetEvent(handle))
		{
			auto const last_error = GetLastError();
			LOG_DEBUG() << "Could not trigger event: " << std::hex << std::showbase << last_error;
			throw std::runtime_error("Could not trigger event.");
		}
	}
	void set_event(HANDLE event)
	{
		LOG_STACK();

		if (event == 0)
			throw std::invalid_argument("The passed argument event can't be equal to 0");

		if (!SetEvent(event))
		{
			auto const last_error = GetLastError();
			LOG_DEBUG() << "Could not trigger event: " << std::hex << std::showbase << last_error;
			throw std::runtime_error("Could not trigger event.");
		}
	}

	void reset_event(const handle_holder & event)
	{
		LOG_STACK();

		auto const handle = event.get();

		if (handle == 0)
			throw std::invalid_argument("The passed argument event can't be equal to 0");

		if (!ResetEvent(handle))
		{
			auto const last_error = GetLastError();
			LOG_DEBUG() << "Could not reset event: " << std::hex << std::showbase << last_error;
			throw std::runtime_error("Could not set event.");
		}
	}

	void release_semaphore(const handle_holder & semaphore,int32_t release_count)
	{
		LOG_STACK();

		auto const handle = semaphore.get();

		if (handle == 0)
			throw std::invalid_argument("The passed argument semaphore can't be equal to 0");
		if (release_count == 0)
			throw std::invalid_argument("The passed argument release_count can't be equal to 0");

		if (!ReleaseSemaphore(handle, release_count,NULL))
		{
			auto const last_error = GetLastError();
			std::string _err = "Could not realese semaphore. Error: " + std::to_string(last_error);
			LOG_DEBUG() << "Could not realese semaphore: " << std::hex << std::showbase << last_error;
			throw std::runtime_error(_err.c_str());
		}
	}


	bool wait(const HANDLE handle, DWORD time_to_wait)
	{
		LOG_STACK();

		if (handle == 0)
			throw std::invalid_argument("The passed argument event cannot be equal to 0");

		if (handle == INVALID_HANDLE_VALUE)
			throw std::invalid_argument("The passed argument event cannot be equal to INVALID_HANDLE_VALUE");

		auto const wait_result = WaitForSingleObject(handle, time_to_wait);
		if (wait_result == WAIT_OBJECT_0)
		{
			return true;
		}
		else if (wait_result == WAIT_TIMEOUT || wait_result == WAIT_ABANDONED)
		{
			return false;
		}

		auto const last_error = GetLastError();
		LOG_DEBUG() << "WaitForSingleObject failed with error: " << std::hex << std::showbase << last_error;
		throw std::runtime_error("Waiting is failed.");
	}

	bool wait(const handle_holder & object, DWORD time_to_wait)
	{
		return wait(object.get(), time_to_wait);
	}

	wait_result wait_any(const HANDLE * first, std::size_t num_of_elements, DWORD time_to_wait)
	{
		LOG_STACK();

		if (first == nullptr || num_of_elements == 0)
		{
			throw std::invalid_argument("Invalid array of handles was passed.");
		}

		const auto raw_result = WaitForMultipleObjects(
			(DWORD)num_of_elements,
			first,
			FALSE,
			time_to_wait);

		if (raw_result == WAIT_FAILED)
		{
			const auto last_error = GetLastError();
			LOG_DEBUG() << "Error code: " << std::hex << std::showbase << last_error;
			throw std::runtime_error("Wait failed.");
		}

		if (raw_result >= WAIT_OBJECT_0 && raw_result < WAIT_OBJECT_0 + num_of_elements)
		{
			wait_result result;
			result.event_raised = true;
			result.event_index = static_cast<std::size_t>(raw_result - WAIT_OBJECT_0);
			return result;
		}

		LOG_DEBUG() << "Unexpected result was returned from WaitForMultipleObjects: " << raw_result;

		return wait_result();
	}

	bool is_event_set(const HANDLE handle)
	{
		LOG_STACK();
		return wait(handle, 0);
	}

	bool is_event_set(const handle_holder & event)
	{
		LOG_STACK();
		return wait(event, 0);
	}

	scoped_thread_priority::scoped_thread_priority(int priority) : _prev_priority(GetThreadPriority(GetCurrentThread()))
	{
		LOG_STACK();
		SetThreadPriority(GetCurrentThread(), priority);
	}

	scoped_thread_priority::~scoped_thread_priority()
	{
		LOG_STACK();
		SetThreadPriority(GetCurrentThread(), _prev_priority);
	}

	scoped_thread_background_priority::scoped_thread_background_priority()
	{
		LOG_STACK();
		SetThreadPriority(GetCurrentThread(), THREAD_MODE_BACKGROUND_BEGIN);
	}

	scoped_thread_background_priority::~scoped_thread_background_priority()
	{
		LOG_STACK();
		SetThreadPriority(GetCurrentThread(), THREAD_MODE_BACKGROUND_END);
	}

	rw_lock::rw_lock()
	{
		InitializeSRWLock(&_lock);
	}

	void rw_lock::lock(bool exclusive)
	{
		if (exclusive)
		{
			AcquireSRWLockExclusive(&_lock);
		}
		else
		{
			AcquireSRWLockShared(&_lock);
		}
	}

	void rw_lock::unlock(bool exclusive)
	{
		if (exclusive)
		{
			ReleaseSRWLockExclusive(&_lock);
		}
		else
		{
			ReleaseSRWLockShared(&_lock);
		}
	}

	once_flag::once_flag() : completed(false)
	{
		InitializeCriticalSection(&sec);
	}

	once_flag::~once_flag()
	{
		if (TryEnterCriticalSection(&sec))
		{
			LeaveCriticalSection(&sec);
			DeleteCriticalSection(&sec);
		}
		else
		{
			LOG_FATAL() << "Attempt to delete a critical section while it was busy.";
			std::terminate();
		}
	}
} // namespace sync_helpers