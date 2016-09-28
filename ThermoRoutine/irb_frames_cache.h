#pragma once
#include <Windows.h>
#include <vector>
#include <memory>
#include "irb_frame_helper.h"
#include <common\handle_holder.h>
#include <fstream>
#include <queue>
#include <list>
#include <mutex>
#include <thread>
#include <common\sync_helpers.h>

namespace irb_frames_cache
{

#define WAIT_THREAD_TERMINATING_ON

	using irb_frame_shared_ptr_t = std::shared_ptr<irb_frame_helper::IRBFrame>;
	using new_irb_frame_process_func_t = std::function<void(irb_frame_helper::frame_id_t)>;

	template<int queue_size = 1>
	class irb_frames_cache
	{
	public:
		explicit irb_frames_cache(uint8_t cache_size = 10);
		~irb_frames_cache();

		void start_cache(new_irb_frame_process_func_t new_irb_frame_process_func);
		void stop_cache();

		void push_frame(const irb_frame_shared_ptr_t & frame);
		irb_frame_shared_ptr_t get_frame_by_id(irb_frame_helper::frame_id_t frame_id);
		irb_frame_shared_ptr_t get_next_frame();

		void set_cache_size(uint8_t cache_size) { _max_cache_size = cache_size; }
	private:
		void cache_loop(new_irb_frame_process_func_t new_irb_frame_process_func);
	private:
		volatile bool _b_stop_requested;

		std::queue<irb_frame_shared_ptr_t> _queue;
		std::mutex _queue_mtx;
		handle_holder _queue_semaphore;
		std::thread _cache_loop_thread;


		std::list<irb_frame_shared_ptr_t> _cache;
		std::vector<irb_frame_shared_ptr_t> _cache2;

		std::mutex _lock;
		irb_frame_shared_ptr_t _last_frame;
		int _cache_last_index;


		uint8_t _cache_size;
		irb_frame_helper::frame_id_t _last_requested_id;
		std::mutex _cache_lock;

		uint8_t _max_cache_size;
		//sync_helpers::critical_section _queue_mtx;

		new_irb_frame_process_func_t _new_irb_frame_process_func;
		bool _has_new_frame;
		bool _notify;
	};


	template<int queue_size>
	irb_frames_cache<queue_size>::irb_frames_cache(uint8_t cache_size) :
		_b_stop_requested(false),
		_has_new_frame(false),
		_notify(false),
		//_queue_semaphore(sync_helpers::create_basic_semaphore_object(0)),
		_max_cache_size(cache_size),
		_cache_size(0),
		_last_requested_id(0),
		_cache_last_index(0)
	{
		_cache2.resize(10);
	}

	template<int queue_size>
	irb_frames_cache<queue_size>::~irb_frames_cache()
	{
		//_b_stop_requested = true;
		//sync_helpers::release_semaphore(_queue_semaphore);
		//if (_cache_loop_thread.joinable())
		//{
		//	_cache_loop_thread.join();
		//}
	}

	template<int queue_size>
	void irb_frames_cache<queue_size>::push_frame(const irb_frame_shared_ptr_t & frame)
	{
		LOG_STACK();

		std::unique_lock<decltype(_lock)> guard(_lock);
		_cache2[_cache_last_index++] = _last_frame;
		if (_cache_last_index == 10)
			_cache_last_index = 0;
		_last_frame = frame;
		auto frame_id = _last_frame->id;
		_has_new_frame = true;
		guard.unlock();
		if (_notify){
			_new_irb_frame_process_func(frame_id);
		}
		//if (queue_size == 1)
		//{
		//	_queue_mtx.lock();
		//	_queue.push(frame);
		//	_queue_mtx.unlock();

		//	sync_helpers::release_semaphore(_queue_semaphore);
		//}
		//else
		//{
		//	bool release_semaphore = false;
		//	_queue_mtx.lock();
		//	_queue.push(frame);

		//	if (_queue.size() == queue_size)
		//		release_semaphore = true;

		//	_queue_mtx.unlock();
		//	if (release_semaphore)
		//		sync_helpers::release_semaphore(_queue_semaphore, queue_size);
		//}

	}
	template<int queue_size>
	void irb_frames_cache<queue_size>::cache_loop(new_irb_frame_process_func_t new_irb_frame_process_func)
	{

		std::vector<irb_frame_shared_ptr_t> frames_from_queue;

		frames_from_queue.reserve(queue_size + 1);
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
			while (!_queue.empty()){
				frames_from_queue.emplace_back(_queue.front());
				_queue.pop();
			}
			_queue_mtx.unlock();

			for(auto && frame : frames_from_queue){

				auto frame_id = frame->id;
				_cache_lock.lock();
				_cache_size++;
				_cache.emplace_back(frame);
				if (_cache_size >= _max_cache_size)
				{
					_cache_size--;
					_cache.pop_front();
				}
				_cache_lock.unlock();
				if (new_irb_frame_process_func)
					new_irb_frame_process_func(frame_id);

			}

			frames_from_queue.clear();
		}

		_cache_lock.lock();
		_cache_size = 0;
		_last_requested_id = 0;
		_cache.clear();
		_cache_lock.unlock();


	}
	template<int queue_size>
	irb_frame_shared_ptr_t irb_frames_cache<queue_size>::get_frame_by_id(irb_frame_helper::frame_id_t frame_id)
	{
		std::lock_guard<decltype(_lock)> guard(_lock);
		if (_last_frame && _last_frame->id == frame_id)
			return _last_frame;

		for (const auto & frame : _cache2)
		{
			if (frame && frame->id == frame_id)
				return frame;
		}

		return{};
	}

	template<int queue_size>
	irb_frame_shared_ptr_t irb_frames_cache<queue_size>::get_next_frame()
	{
		LOG_STACK();
		std::unique_lock<decltype(_lock)> guard(_lock);
		if (!_last_frame){
			guard.unlock();
			_notify = true;
			return{};
		}
		auto frame = _last_frame;
		_has_new_frame = false;
		guard.unlock();
		if (frame->id == _last_requested_id){
			_notify = true;
			return{};
		}
		_last_requested_id = frame->id;
		_notify = false;

		return frame;

		//std::lock_guard<decltype(_cache_lock)> guard(_cache_lock);
		//if (_cache_size == 0)
		//	return irb_frame_shared_ptr_t();
		//auto & frame = _cache.back();
		//if (frame->id == _last_requested_id){
		//	return irb_frame_shared_ptr_t();
		//}
		//_last_requested_id = frame->id;
		//return frame;
	}

	template<int queue_size>
	void irb_frames_cache<queue_size>::start_cache(new_irb_frame_process_func_t new_irb_frame_process_func)
	{
		std::lock_guard<decltype(_lock)> guard(_lock);
		_last_requested_id = 0;
		_cache_last_index = 0;
		_notify = false;
		_has_new_frame = false;
		_new_irb_frame_process_func = new_irb_frame_process_func ? new_irb_frame_process_func : new_irb_frame_process_func_t([](irb_frame_helper::frame_id_t){});

		/*	if (_cache_loop_thread.joinable())
		{
		LOG_DEBUG() << "Looks like run_processing_loop was called twice.";
		throw std::logic_error("The processing loop must not be running at this point.");
		}
		_b_stop_requested = false;
		std::thread processing_loop_thread(
		[this, new_irb_frame_process_func]()
		{ this->cache_loop(new_irb_frame_process_func); }
		);

		_cache_loop_thread.swap(processing_loop_thread);*/

	}

	template<int queue_size>
	void irb_frames_cache<queue_size>::stop_cache()
	{
		std::lock_guard<decltype(_lock)> guard(_lock);
		_cache2.clear();
		_cache2.resize(10);
		_new_irb_frame_process_func = new_irb_frame_process_func_t([](irb_frame_helper::frame_id_t){});
		//		_b_stop_requested = true;
		//		sync_helpers::release_semaphore(_queue_semaphore);
		//		if (_cache_loop_thread.joinable())
		//		{
		//#ifdef WAIT_THREAD_TERMINATING_ON
		//			_cache_loop_thread.join();
		//#else
		//			_cache_loop_thread.detach();
		//#endif
		//}

	}


}