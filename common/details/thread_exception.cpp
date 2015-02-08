#include <thread_exception.h>
#include <common\thread_exception.h>

#include <thread>
#include <memory>
#include <atomic>
#include <mutex>
#include <windows.h>
#include <handle_holder.h>
#include <sync_helpers.h>
#include <loglib\log.h>
#include <future>


exception_queue::exception_queue():
_is_need_create_promise(false),
_is_promise_created(true),
exception(std::make_unique<std::promise<void>>())
{
}
std::future<void> exception_queue::get_future()
{
	std::lock_guard<std::mutex> guard(lock);
	if (!_is_need_create_promise){
		_is_promise_created = true;
		return exception->get_future();
	}

	_is_need_create_promise = false;
	_is_promise_created = true;
	exception = std::make_unique<std::promise<void>>();
	return exception->get_future();
}
void exception_queue::release()
{
	std::lock_guard<std::mutex> guard(lock);
	if (_is_need_create_promise)
		return;
	_is_need_create_promise = true;
//	if (_is_promise_created)
		exception->set_value();
}
void exception_queue::raise_exception()
{ 
	std::lock_guard<std::mutex> guard(lock);
	if (_is_need_create_promise)
		return;
	_is_need_create_promise = true; 
	exception->set_exception(std::current_exception()); 
}

struct thread_exception_handler::Impl
{
	exception_process_func_t _exception_process_func;
	std::thread _listener_thread;
	exception_queue_ptr_t _exc_queue;
	std::future<void> _result;
	std::atomic<bool> is_processing;
};


thread_exception_handler::thread_exception_handler(const exception_queue_ptr_t& exc_queue, exception_process_func_t _exception_process_func)
{
	decltype(_p_impl) impl = std::make_unique<thread_exception_handler::Impl>();
	impl->_exception_process_func = _exception_process_func;
	impl->_exc_queue = exc_queue;
	impl->is_processing = false;

	_p_impl.swap(impl);

}
thread_exception_handler::~thread_exception_handler()
{
	stop_processing();
}
void thread_exception_handler::start_processing()
	{
		LOG_STACK();
		if (_p_impl->is_processing.load()){
			return;
		}
		if (_p_impl->_listener_thread.joinable())
		{
			return;
			//LOG_DEBUG() << "Looks like run_processing_loop was called twice.";
			//throw std::logic_error("The processing loop must not be running at this point.");
		}

		std::thread processing_loop_thread(
			[this]()
		{ this->processing_loop(); }
		);

		LOG_TRACE() << "Thread was created.";

		_p_impl->_listener_thread.swap(processing_loop_thread);
	}
void thread_exception_handler::stop_processing()
	{
		LOG_STACK();

		LOG_TRACE() << "Requesting stopping.";
		if (_p_impl->is_processing.load()){
			_p_impl->_exc_queue->release();
		}
		else
		{
			if (_p_impl->_listener_thread.joinable())
			{
				_p_impl->_listener_thread.detach();
			}
			return;
		}
		LOG_TRACE() << "Stop flag was set.";
		if (_p_impl->_listener_thread.joinable())
		{
			LOG_TRACE() << "Waiting for completion of processing loop thread.";
			_p_impl->_listener_thread.join();
			LOG_TRACE() << "Processing loop thread finished.";
		}
	}

	void thread_exception_handler::processing_loop()
	{
		LOG_STACK();
		_p_impl->is_processing = true;
		try
		{
			LOG_TRACE() << "Running processing loop.";
			for (;;)
			{
				_p_impl->_result = _p_impl->_exc_queue->get_future();
				try{
					_p_impl->_result.get();
				}
				catch (...)
				{
					_p_impl->_exception_process_func(std::current_exception());
					continue;
				}
				LOG_TRACE() << "Stop was requested.";
				break;
			}
		}
		catch (...)
		{
			_p_impl->is_processing = false;
			_p_impl->_exception_process_func(std::current_exception());
		}

		_p_impl->is_processing = false;

	}
