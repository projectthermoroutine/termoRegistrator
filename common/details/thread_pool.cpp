#include "thread_pool.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <queue>
#include <algorithm>

#include <loglib\log.h>
#include <common\sync_helpers.h>

namespace thread_pool
{
	namespace details
	{

		template<typename T>
		class atomic_auto_counter final
		{
		public:
			atomic_auto_counter(std::atomic<T> & a) : _a(a)
			{
				++_a;
			}
			~atomic_auto_counter()
			{
				--_a;
			}

			atomic_auto_counter(const atomic_auto_counter &) = delete;
			atomic_auto_counter & operator =(const atomic_auto_counter &) = delete;
		private:
			std::atomic<T> & _a;
		};

		class thread_pool_impl final
		{
		public:
			using work_item_t = std::function<void()>;

			thread_pool_impl(
				unsigned int max_num_of_threads,
				const std::chrono::duration<uint64_t> thread_keep_alive);

			~thread_pool_impl();

			void queue_work_item(work_item_t work_item);
			void wait_for_completion();

			thread_pool_impl(const thread_pool_impl &) = delete;
			thread_pool_impl & operator = (const thread_pool_impl &) = delete;
		private:
			static void work_thread(thread_pool_impl * p_thread_pool);

			void do_work();
			bool wait_for_new_item(std::unique_lock<std::mutex> & lock);
			void wait_for_threads_stopping();

			const unsigned int _max_num_of_threads;
			const std::chrono::duration<uint64_t> _thread_keep_alive;

			std::mutex _threads_mutex;
			std::deque<std::thread> _threads;

			std::mutex _work_items_mutex;
			std::condition_variable _processing_event;
			std::queue<work_item_t> _work_items;

			std::atomic<bool> _terminate;
			std::atomic<bool> _completion_state;
			std::atomic<unsigned int> _num_busy_threads;
			sync_helpers::rw_lock _completion_state_lock;
		};

		thread_pool_impl::thread_pool_impl(
			unsigned int max_num_of_threads,
			const std::chrono::duration<uint64_t> thread_keep_alive) :
			_max_num_of_threads(max_num_of_threads),
			_thread_keep_alive(thread_keep_alive),
			_terminate(false),
			_completion_state(false),
			_num_busy_threads(0)
		{
			LOG_STACK();

			LOG_TRACE() << "Maximum number of threads: " << max_num_of_threads;
			LOG_TRACE() << "Time to keep threads alive (in ms): " << std::chrono::duration_cast<std::chrono::milliseconds>(_thread_keep_alive).count();

			if (_max_num_of_threads == 0)
				throw std::invalid_argument("The passed max_num_of_threads is 0");
		}

		thread_pool_impl::~thread_pool_impl()
		{
			LOG_STACK();

			_terminate.store(true);

			wait_for_threads_stopping();
		}

		void thread_pool_impl::queue_work_item(work_item_t work_item)
		{
			LOG_STACK();

			if (_terminate.load())
			{
				LOG_TRACE() << "Could not queue the item because termination was requested before.";
				throw std::logic_error("thread_pool::queue_work_item was called after termination is requested.");
			}

			sync_helpers::rw_lock_guard_shared lock_guard(_completion_state_lock);

			if (_completion_state.load())
			{
				LOG_TRACE() << "Could not queue the item because the thread pool was waiting for completion of the threads.";
				throw std::logic_error("thread_pool::queue_work_item was called after completion is requested.");
			}

			{
				std::unique_lock<std::mutex> lock(_work_items_mutex);
				_work_items.push(work_item);
			}

			auto num_of_busy_threads = _num_busy_threads.load();
			if (num_of_busy_threads < _max_num_of_threads)
			{
				std::unique_lock<std::mutex> lock(_threads_mutex);				
				auto num_of_threads = _threads.size();
				LOG_TRACE() << "Number of threads in pool: " << num_of_threads;
				if (num_of_threads == _max_num_of_threads)
				{
					LOG_TRACE() << "Trying to clean stale threads.";

					// clean stale threads
					auto const new_end = std::remove_if(
						_threads.begin(),
						_threads.end(),
						[](const std::thread & th) { return !th.joinable(); });

					_threads.erase(new_end, _threads.end());
					num_of_threads = _threads.size();

					LOG_TRACE() << "Number of threads in the pool after cleaning: " << num_of_threads;
				}
				num_of_busy_threads = _num_busy_threads.load(); // load again under the lock to mitigate race conditions
				if (num_of_threads < _max_num_of_threads && num_of_busy_threads == num_of_threads)
				{
					LOG_TRACE() << "Adding a new thread into the pool.";
					_threads.emplace_back(&thread_pool_impl::work_thread, this);
				}
			}

			_processing_event.notify_one();
		}

		void thread_pool_impl::wait_for_completion()
		{
			LOG_STACK();

			if (_terminate.load())
			{
				LOG_TRACE() << "Skip waiting for completion because termination was requested before.";
				throw std::logic_error("thread_pool::wait_for_completion was called after termination is requested.");
			}

			sync_helpers::rw_lock_guard_exclusive lock_guard(_completion_state_lock);

			_completion_state.store(true);

			wait_for_threads_stopping();

			if (!_terminate.load())
			{
				_completion_state.store(false);
				
				if (!_work_items.empty())
					throw std::logic_error("_work_items are not empty");

				if (_num_busy_threads.load() != 0)
					throw std::logic_error("_num_busy_threads is not equal to zero");			
			}
		}

		void thread_pool_impl::work_thread(thread_pool_impl * p_thread_pool)
		{
			LOG_STACK();

			if (p_thread_pool == nullptr)
				throw std::invalid_argument("The passed p_thread_pool is nullptr");

			p_thread_pool->do_work();
		}

		void thread_pool_impl::do_work()
		{
			LOG_STACK();

			while (!_terminate.load())
			{
				std::unique_lock<std::mutex> lock(_work_items_mutex);
				if (!wait_for_new_item(lock))
				{
					break;
				}

				auto work_item = _work_items.front();
				_work_items.pop();

				{
					atomic_auto_counter<unsigned int> counter(_num_busy_threads);
					lock.unlock();
					work_item();
				}
			}

			{
				// remove themself from thread list
				auto const thread_id = std::this_thread::get_id();
				LOG_TRACE() << "Removing thread with id " << thread_id << " from the thread pool.";
				std::unique_lock<std::mutex> lock(_threads_mutex);
				auto const iter = std::find_if(_threads.begin(), _threads.end(), [thread_id](const std::thread & th) { return th.get_id() == thread_id; });
				if (iter != _threads.end())
				{
					iter->detach();
					_threads.erase(iter);
					LOG_TRACE() << "Number of threads in pool: " << _threads.size();
				}
				else
				{
					LOG_TRACE() << "Could not find thread with id: " << thread_id;
				}
			}
		}

		bool thread_pool_impl::wait_for_new_item(std::unique_lock<std::mutex> & lock)
		{
			LOG_STACK();

			while (_work_items.empty())
			{
				if (_terminate.load() || _completion_state.load())
				{
					LOG_TRACE() << "Waiting was interrupted by request for termination or completion.";
					return false;
				}

				if (_processing_event.wait_for(lock, _thread_keep_alive) == std::cv_status::timeout)
				{
					LOG_TRACE() << "Waiting was interrupted due inactivity of the thread: " << std::this_thread::get_id();
					return false;
				}
			}

			return true;
		}
	
		void thread_pool_impl::wait_for_threads_stopping()
		{
			LOG_STACK();

			decltype(_threads) threads;
			{
				std::unique_lock<std::mutex> lock(_threads_mutex);
				for (auto & thread : _threads)
				{
					if (std::this_thread::get_id() == thread.get_id())
					{
						LOG_DEBUG() << "wait_for_threads_stopping was called from a thread pool's thread.";
						throw std::logic_error("Cannot wait for completion of thread pool's thread from itself.");
					}
				}
				threads.swap(_threads);
			}

			_processing_event.notify_all();

			LOG_TRACE() << "Joining the threads.";
			for (auto & thread : threads)
			{
				if (thread.joinable())
				{
					thread.join();
				}
			}
			LOG_TRACE() << "All threads are completed.";
		}

		inline thread_pool_impl * get_thread_pool_impl(void * p)
		{
			if (p == nullptr)
				throw std::invalid_argument("thread_pool_impl is nullptr");

			return static_cast<thread_pool_impl *>(p);
		}
	} // namespace details

	thread_pool::thread_pool(
		unsigned int max_num_of_threads,
		const std::chrono::duration<uint64_t> thread_keep_alive) : _p_impl(new details::thread_pool_impl(max_num_of_threads, thread_keep_alive))
	{
	}

	thread_pool::~thread_pool()
	{
		auto const p_impl = _p_impl;
		_p_impl = 0;
		delete details::get_thread_pool_impl(p_impl);
	}

	void thread_pool::queue_work_item(work_item_t work_item)
	{
		details::get_thread_pool_impl(_p_impl)->queue_work_item(work_item);
	}

	void thread_pool::wait_for_completion()
	{
		details::get_thread_pool_impl(_p_impl)->wait_for_completion();
	}
}