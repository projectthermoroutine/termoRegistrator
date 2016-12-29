
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <condition_variable>

#include <loglib/log.h>

#include "thread_exception.h"


	class thread_exception_queue final
	{
	public:
		thread_exception_queue();
		~thread_exception_queue();

		void raise_exception(const std::exception_ptr &);
		bool wait_and_get(std::exception_ptr &ex);
		void interrupt();

	private:
		std::string get_exception_info(const std::exception_ptr &ex);

		std::queue<std::exception_ptr> _exception_queue;
		std::condition_variable _cv;
		std::mutex _queue_mtx;
		bool _interruption_point;
	};

	thread_exception_queue::thread_exception_queue()
		: _interruption_point(false)
	{
	}

	thread_exception_queue::~thread_exception_queue()
	{
	}

	std::string thread_exception_queue::get_exception_info(const std::exception_ptr &ex)
	{
		try
		{
			std::rethrow_exception(ex);
		}
		catch (const std::exception &e)
		{
			return std::string(e.what());
		}
		catch (...)
		{
			return std::string("Unknown exception");
		}

		return std::string("Unknown exception");
	}

	void thread_exception_queue::raise_exception(const std::exception_ptr &ex)
	{
		std::lock_guard<decltype(_queue_mtx)> lock(_queue_mtx);
		if (_interruption_point)
		{
			LOG_WARN() << L"Interrupt was requested. Exception can't process. Exception info: " << get_exception_info(ex).c_str();
		}
		else
		{
			_exception_queue.push(ex);
			_cv.notify_one();
		}
	}

	bool thread_exception_queue::wait_and_get(std::exception_ptr &ex)
	{
		std::unique_lock<decltype(_queue_mtx)> lock(_queue_mtx);
		_cv.wait(lock, [&]
		{
			return _interruption_point || !_exception_queue.empty();
		});

		if (_interruption_point)
		{
			if (!_exception_queue.empty())
			{
				std::string dump_exceptions_message("Unprocessed exceptions:");

				while (!_exception_queue.empty())
				{
					const auto cur_exception = _exception_queue.front();
					_exception_queue.pop();
					dump_exceptions_message.append("\n - " + get_exception_info(cur_exception));
				}

				LOG_WARN() << dump_exceptions_message.c_str();
			}

			return false;
		}

		ex = std::move(_exception_queue.front());
		_exception_queue.pop();

		return true;
	}

	void thread_exception_queue::interrupt()
	{
		std::lock_guard<decltype(_queue_mtx)> lock(_queue_mtx);
		_interruption_point = true;
		_cv.notify_one();
	}

	class thread_exception_handler::impl final
	{
	public:
		impl(const exception_callback_t &exception_callback);
		~impl();

		void raise_exception(const std::exception_ptr &e);

		impl(const impl &) = delete;
		impl & operator = (const impl &) = delete;

	private:
		void processing_loop();

		thread_exception_queue _exception_queue;
		exception_callback_t _exception_callback;
		std::thread _processing_loop_thread;
	};

	thread_exception_handler::impl::impl(const exception_callback_t &exception_callback) :
		_exception_queue(),
		_exception_callback(exception_callback),
		_processing_loop_thread(&thread_exception_handler::impl::processing_loop, this)
	{
	}

	thread_exception_handler::impl::~impl()
	{
		_exception_queue.interrupt();

		if (_processing_loop_thread.joinable())
		{
			_processing_loop_thread.join();
		}
	}

	void thread_exception_handler::impl::raise_exception(const std::exception_ptr &e)
	{
		_exception_queue.raise_exception(e);
	}

	void thread_exception_handler::impl::processing_loop()
	{
		LOG_STACK();

		std::exception_ptr ptr_result;
		while (_exception_queue.wait_and_get(ptr_result))
		{
			try
			{
				_exception_callback(ptr_result);
			}
			catch (const std::exception &e)
			{
				LOG_DEBUG() << "Exception in thread exception handler callback. Info: " << e.what();
			}
			catch (...)
			{
				LOG_DEBUG() << "Unknown exception in thread exception handler callback.";
			}
		}
	}

	thread_exception_handler::thread_exception_handler(const exception_callback_t &exception_callback)
		: _pimpl(std::make_unique<impl>(exception_callback))
	{ }

	thread_exception_handler::~thread_exception_handler()
	{ }

	void thread_exception_handler::raise_exception(const std::exception_ptr &e)
	{
		_pimpl->raise_exception(e);
	}