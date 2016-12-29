#pragma once
#include <memory>
#include <future>
#include <mutex>


using exception_callback_t = std::function<void(const std::exception_ptr &)>;

class thread_exception_handler;
using thread_exception_handler_ptr = std::shared_ptr<thread_exception_handler>;

class thread_exception_handler final
{
public:
	thread_exception_handler(const exception_callback_t &);
	~thread_exception_handler();

	void raise_exception(const std::exception_ptr &e);

	thread_exception_handler(const thread_exception_handler &) = delete;
	thread_exception_handler & operator = (const thread_exception_handler &) = delete;

private:
	class impl;
	std::unique_ptr<impl> _pimpl;
};