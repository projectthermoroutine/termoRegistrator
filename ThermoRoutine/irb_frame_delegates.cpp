#include <Windows.h>
#include <vector>
#include <memory>
#include "irb_frame_helper.h"
#include <common\handle_holder.h>
#include <fstream>
#include <queue>
#include <list>
#include <thread>
#include <mutex>
#include "irb_frame_delegates.h"
#include "irb_file_helper.h"

#include <future>

#include "defines.h"

namespace irb_frame_delegates
{
	using namespace irb_frame_helper;

	irb_frames_cache::irb_frames_cache(uint16_t max_frames_in_cache, prepare_frame_func_t prepare_frame_func, uint16_t prepaire_delay_frames_number) :
		_max_frames_in_cache(max_frames_in_cache),
		_prepare_frame_func(prepare_frame_func),
		_prepaire_delay_frames_number(prepaire_delay_frames_number),
		_file_counter(0),
		_frame_counter(0),
		_state(0),
		_busy(0),
		_queue_semaphore(sync_helpers::create_basic_semaphore_object(0)),
		_b_stop_requested(false)
	{
		start_flush_thread();
	}
	irb_frames_cache::~irb_frames_cache()
	{
		stop_flush_thread();
	}

	bool irb_frames_cache::process_frame(const irb_frame_shared_ptr_t& frame)
	{

		if (_busy == 1)
			return true;

		auto prev_value = _InterlockedCompareExchange8((char*)(&_state), 1, 0);
		if (prev_value != 0){
			_lock.lock();
		}

		_not_prepaired_cache.push_back(frame);
		frame->id = ++_frame_counter;
		auto not_prepaired_cache_size = _not_prepaired_cache.size();
		if (not_prepaired_cache_size > _max_frames_in_cache){
			_not_prepaired_cache.pop_front();
		}

		if (not_prepaired_cache_size >= _prepaire_delay_frames_number){
		
			auto &prepaire_frame = _not_prepaired_cache.front();
			auto res = _prepare_frame_func(prepaire_frame);
			if (res)
			{
				_prepaired_cache.emplace(prepaire_frame->id, prepaire_frame);
				_not_prepaired_cache.pop_front();

				auto cache_size = _prepaired_cache.size();
				if (cache_size == _max_frames_in_cache)
				{
					if (_writer){
						save_frames();
					}
					else
					{
						_prepaired_cache.clear();
					}

				}
			}
		}

		if (prev_value != 0){
			_lock.unlock();
		}
		else{
			_InterlockedAnd8((char*)(&_state), 0);
		}


		return true;
	}

	bool irb_frames_cache::process_frame_non_cache(const irb_frame_shared_ptr_t& frame)
	{
		auto prev_value = _InterlockedCompareExchange8((char*)(&_state), 1, 0);
		if (prev_value != 0){
			_lock.lock();
		}

		frame->id = ++_frame_counter;

		auto res = _prepare_frame_func(frame);
		if (res)
		{
			_prepaired_cache.emplace(frame->id, frame);

			auto cache_size = _prepaired_cache.size();
			if (cache_size == _max_frames_in_cache)
			{
				if (_writer){
					save_frames();
				}
				else
				{
					_prepaired_cache.clear();
				}
			}
		}

		if (prev_value != 0){
			_lock.unlock();
		}
		else{
			_InterlockedAnd8((char*)(&_state), 0);
		}


		return true;
	}

	void irb_frames_cache::save_frames()
	{
		if (_writer){
			irb_frames_map_t flush_cache(std::move(_prepaired_cache));

			_queue_mtx.lock();
			_queue.push(flush_cache);
			_queue_mtx.unlock();

			sync_helpers::release_semaphore(_queue_semaphore);

			_file_counter++;
		}
	}

	void irb_frames_cache::flush_frames(const irb_frames_map_t & frames)
	{
		std::lock_guard<std::mutex> guard(_lock_writer);
		if (this->_writer){
			try{

				this->_writer->flush_frames(frames, _file_counter);
			}
			catch (const irb_file_helper::irb_file_exception& exc)
			{
				auto error = exc.what();
				return;
			}
		}
	}



	void irb_frames_cache::set_writer(writer_ptr_t &writer)
	{
		save_frames();

		std::lock_guard<std::mutex> guard(_lock_writer);
		_writer.swap(writer);
		_file_counter = 0;
		_frame_counter = 0;

	}


	void irb_frames_cache::flush_loop()
	{
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


			flush_frames(data);

		}
	}


	void irb_frames_cache::start_flush_thread()
	{
		if (_flush_frames_thread.joinable())
		{
			LOG_DEBUG() << "Looks like run_processing_loop was called twice.";
			throw std::logic_error("The processing loop must not be running at this point.");
		}
		_b_stop_requested = false;
		std::thread processing_loop_thread(
			[this]()
		{ this->flush_loop(); }
		);

		_flush_frames_thread.swap(processing_loop_thread);

	}

	void irb_frames_cache::stop_flush_thread()
	{
		_b_stop_requested = true;
		sync_helpers::release_semaphore(_queue_semaphore);
		if (_flush_frames_thread.joinable())
		{
#ifdef WAIT_THREAD_TERMINATING_ON
			_flush_frames_thread.join();
#else
			_flush_frames_thread.detach();
#endif
		}

	}


	irb_frames_writer::irb_frames_writer(const std::string & dir, const std::string & name_pattern, new_irb_file_func_t new_irb_file_func) :
		_dir(dir),
		_name_pattern(name_pattern),
		_cur_file_index(0),
		_cur_frames_in_file(0),
		_new_irb_file_func(new_irb_file_func)
	{
	}
	irb_frames_writer::~irb_frames_writer(){}

	void irb_frames_writer::flush_frames(const irb_frames_map_t& frames, uint16_t file_counter)
	{
		if (frames.empty())
			return;
		std::vector<irb_frame_shared_ptr_t> list;
		for (auto & frame : frames)
		{
			list.emplace_back(frame.second);
		}

		char buf[5];
		sprintf_s(buf, "%04d", (int)(file_counter));
		std::string fullname = _dir + _name_pattern + buf + ".irb";

		try{
			auto file = irb_file_helper::create_irb_file(fullname, irb_file_helper::irb_file_version::original, (uint32_t)list.size());

			irb_file_helper::IRBFile irb_file(file);
			irb_file.append_frames(list);
		}
		catch (const irb_file_helper::irb_file_exception&)
		{
			return;
		}

		if (_new_irb_file_func)
		{
			_new_irb_file_func(fullname);
		}
	}

}