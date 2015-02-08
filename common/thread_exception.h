#pragma once
#include <memory>
#include <future>
#include <mutex>


class thread_exception_handler;
using thread_exception_handler_ptr = std::unique_ptr<thread_exception_handler>;


class exception_queue final
{
	using exception_ptr_t = std::unique_ptr<std::promise<void>>;
public:
	exception_queue();
public:
	std::future<void> get_future();
	void release();
	void raise_exception();
private:
	exception_ptr_t  exception;
	std::mutex lock;
	bool _is_need_create_promise;
	bool _is_promise_created;
};

using exception_queue_ptr_t = std::shared_ptr<exception_queue>;
using exception_process_func_t = std::function<void(const std::exception_ptr&)>;
class thread_exception_handler final
{
public:
	thread_exception_handler(const exception_queue_ptr_t& exc_queue, exception_process_func_t _exception_process_func);
	~thread_exception_handler();
	void start_processing();
	void stop_processing();
private:
	void processing_loop();
private:
	struct Impl;
	std::unique_ptr<Impl> _p_impl;
};
