#pragma once
#include <Windows.h>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <exception>
#include <loglib\log.h>

template <typename handle_t, handle_t default_value = handle_t(), handle_t invalid_value = static_cast<handle_t>(INVALID_HANDLE_VALUE)>
class generic_handle_holder
{
public:

	generic_handle_holder()
		: _handle(default_value)
		, _close_func()
	{}

	generic_handle_holder(handle_t handle, void*close_func(handle_t))
		: generic_handle_holder(handle, std::bind(close_handle, std::placeholders::_1))
	{}

	generic_handle_holder(handle_t handle, std::function<void(handle_t)>&& close_func)
		: _handle(handle)
		, _close_func(std::move(close_func))
	{}

	generic_handle_holder(generic_handle_holder && other)
		: _handle(default_value)
		, _close_func()
	{
		this->swap(other);
	}

	~generic_handle_holder()
	{
		destroy(_handle, _close_func);
	}

	handle_t get()
	{
		return _handle;
	}

	const handle_t get() const
	{
		return _handle;
	}

	explicit operator bool() const
	{
		return !(_handle == invalid_value || _handle == default_value);
	}

	bool operator==(const generic_handle_holder& t) const
	{
		return _handle == t._handle;
	}

	bool operator!=(const generic_handle_holder& t) const
	{
		return !this->operator==(t);
	}

	generic_handle_holder & operator= (generic_handle_holder && other)
	{
		generic_handle_holder tmp;
		tmp.swap(other);
		tmp.swap(*this);

		return *this;
	}

	void swap(generic_handle_holder & x)
	{
		std::swap(x._handle, _handle);
		std::swap(x._close_func, _close_func);
	}

	inline void reset(handle_t new_handle = default_value)
	{
		std::swap(_handle, new_handle);
		destroy(new_handle, _close_func);
	}

	generic_handle_holder(const generic_handle_holder &) = delete;
	generic_handle_holder & operator = (const generic_handle_holder &) = delete;
	bool operator<(const generic_handle_holder&) = delete;
	bool operator>(const generic_handle_holder&) = delete;
	bool operator<=(const generic_handle_holder&) = delete;
	bool operator>=(const generic_handle_holder&) = delete;

private:

	inline static void destroy(handle_t& handle, const std::function<void(handle_t)>& close_func)
	{
		if (handle != default_value)
		{
			if (handle != invalid_value && close_func)
				close_func(handle);

			handle = default_value;
		}
	}

private:

	handle_t _handle;
	std::function<void(handle_t)> _close_func;
};

class handle_holder final : public generic_handle_holder<HANDLE>
{
public:
	handle_holder() : generic_handle_holder() {}
	handle_holder(HANDLE handle) : generic_handle_holder(handle, &close_handle) {}

	handle_holder(handle_holder && x) : generic_handle_holder(std::move(x)) {}

	handle_holder & operator = (handle_holder && x)
	{
		generic_handle_holder::operator=(std::move(x));
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

