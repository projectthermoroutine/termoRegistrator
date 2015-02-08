#include "irb_frames_cache.h"
#include "defines.h"
namespace irb_frames_cache
{

	irb_frames_cache::irb_frames_cache() :_b_stop_requested(false),
		_queue_semaphore(sync_helpers::create_basic_semaphore_object(0)),
		_max_cache_size(10),
		_cache_size(0),
		_last_requested_id(0)
	{
	}


	irb_frames_cache::~irb_frames_cache()
	{
		_b_stop_requested = true;
		sync_helpers::release_semaphore(_queue_semaphore);
		if (_cache_loop_thread.joinable())
		{
			_cache_loop_thread.join();
		}
	}

	void irb_frames_cache::push_frame(const irb_frame_shared_ptr_t & frame)
	{
		_queue_mtx.lock();
		_queue.push(frame);
		_queue_mtx.unlock();

		sync_helpers::release_semaphore(_queue_semaphore);

	}

	void irb_frames_cache::cache_loop(new_irb_frame_process_func_t new_irb_frame_process_func)
	{

		_cache_size = 0;
		_last_requested_id = 0;
		_cache.clear();
		for (;;)
		{
			if (!sync_helpers::wait(_queue_semaphore))
			{
				break;
			}
			if (_queue.empty())
			{
				if (_b_stop_requested)
					break;
				continue;
			}
			if (_b_stop_requested)
				break;

			_queue_mtx.lock();
			auto data = _queue.front();
			_queue.pop();
			_queue_mtx.unlock();

			auto frame_id = data->id;
			_cache_lock.lock(true);
			_cache_size++;
			_cache.emplace_back(data);
			if (_cache_size >= _max_cache_size)
			{
				_cache_size--;
				_cache.pop_front();
			}
			_cache_lock.unlock(true);
			if (new_irb_frame_process_func)
				new_irb_frame_process_func(frame_id);
		}

		_cache_lock.lock(true);
		_cache_size = 0;
		_last_requested_id = 0;
		_cache.clear();
		_cache_lock.unlock(true);


	}

	irb_frame_shared_ptr_t irb_frames_cache::get_frame_by_id(irb_frame_helper::frame_id_t frame_id)
	{
		sync_helpers::rw_lock_guard_shared guard(_cache_lock); 
		for (const auto & frame : _cache)
		{
			if (frame && frame->id == frame_id)
				return frame;
		}

		return irb_frame_shared_ptr_t();
	}

	irb_frame_shared_ptr_t irb_frames_cache::get_next_frame()
	{
		sync_helpers::rw_lock_guard_shared guard(_cache_lock);
		if (_cache_size == 0)
			return irb_frame_shared_ptr_t();
		auto & frame = _cache.back();
		if (frame->id == _last_requested_id){
			return irb_frame_shared_ptr_t();
		}
		_last_requested_id = frame->id;
		return frame;
	}


	void irb_frames_cache::start_cache(new_irb_frame_process_func_t new_irb_frame_process_func)
	{
		if (_cache_loop_thread.joinable())
		{
			LOG_DEBUG() << "Looks like run_processing_loop was called twice.";
			throw std::logic_error("The processing loop must not be running at this point.");
		}
		_b_stop_requested = false;
		std::thread processing_loop_thread(
			[this, new_irb_frame_process_func]()
		{ this->cache_loop(new_irb_frame_process_func); }
		);

		_cache_loop_thread.swap(processing_loop_thread);

	}

	void irb_frames_cache::stop_cache()
	{
		_b_stop_requested = true;
		sync_helpers::release_semaphore(_queue_semaphore);
		if (_cache_loop_thread.joinable())
		{
#ifdef WAIT_THREAD_TERMINATING_ON
			_cache_loop_thread.join();
#else
			_cache_loop_thread.detach();
#endif
		}

	}

}