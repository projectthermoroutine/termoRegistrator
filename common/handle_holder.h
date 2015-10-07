#pragma once
#include <Windows.h>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <exception>
#include <loglib\log.h>

template <typename handle_t, handle_t invalid_handle>
class generic_handle_holder
{
public:
	typedef void (*close_func_t)(handle_t);
	generic_handle_holder(close_func_t close_func) : _handle(0), _close_func2(close_func) {}
	generic_handle_holder(handle_t handle, close_func_t close_func) : _handle(handle), _close_func2(close_func) {}
	~generic_handle_holder()
	{
		if (this->operator bool())
		{
			if (_close_func2!=nullptr)
			{
				_close_func2(_handle);
			}
			_handle = 0;
		}
	}

	handle_t get()
	{
		return _handle;
	}

	const handle_t get() const
	{
		return _handle;
	}

	handle_t release()
	{
		auto handle = _handle;
		_handle = 0;
		return handle;
	}

	explicit operator bool() const
	{
		return !(_handle == invalid_handle || _handle == 0);
	}

	bool operator==(const generic_handle_holder& t) const
	{
		return _handle == t._handle;
	}

	bool operator!=(const generic_handle_holder& t) const
	{
		return !this->operator==(t);
	}

	void swap(generic_handle_holder & x)
	{
		std::swap(x._handle, _handle);
		std::swap(x._close_func2, _close_func2);
	}

	generic_handle_holder(generic_handle_holder && x) : _handle(0)
	{
		this->swap(x);
	}

	generic_handle_holder & operator = (generic_handle_holder && x)
	{
		this->swap(x);
		return *this;
	}

	generic_handle_holder(const generic_handle_holder &) = delete;
	generic_handle_holder & operator = (const generic_handle_holder &) = delete;
	bool operator<(const generic_handle_holder&) = delete;
	bool operator>(const generic_handle_holder&) = delete;
	bool operator<=(const generic_handle_holder&) = delete;
	bool operator>=(const generic_handle_holder&) = delete;

private:
	handle_t _handle;
	close_func_t _close_func2;

};

class handle_holder final : public generic_handle_holder<HANDLE, INVALID_HANDLE_VALUE>
{
public:
	handle_holder() : generic_handle_holder(&close_handle) {}
	handle_holder(HANDLE handle) : generic_handle_holder(handle, &close_handle) {}

	handle_holder(handle_holder && x) : generic_handle_holder<HANDLE, INVALID_HANDLE_VALUE>(std::move(x)) {}

	handle_holder & operator = (handle_holder && x)
	{
		generic_handle_holder<HANDLE, INVALID_HANDLE_VALUE>::operator=(std::move(x));
		return *this;
	}

	handle_holder(const handle_holder &) = delete;
	handle_holder & operator = (const handle_holder &) = delete;

private:
	static void close_handle(HANDLE handle)
	{
		auto result = ::CloseHandle(handle);
		if (!result)
		{
			LOG_FATAL() << "Failed to close handle. Terminating.";
			std::terminate();
		}
	}
};

