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
		_b_stop_requested(false),
		_max_frames_for_writer(0)
	{
		start_flush_thread();
	}
	irb_frames_cache::~irb_frames_cache()
	{
		_InterlockedCompareExchange8((char*)(&_busy), 1, 0);
		save_frames(true);
		stop_flush_thread();
		_queue_mtx.lock();
		while (!_queue.empty())
		{
			auto data = _queue.front();
			_queue.pop();
			if (data.save_event != INVALID_HANDLE_VALUE && data.save_event != 0)
				sync_helpers::set_event(data.save_event);
		} 
		_queue_mtx.unlock();
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

		if (_busy == 1)
			return true;

		_lock.lock();

		frame->id = ++_frame_counter;

		auto res = _prepare_frame_func(frame);
		if (res)
		{
			_prepaired_cache.emplace(frame->id, frame);

			auto cache_size = _prepaired_cache.size();
			if (cache_size == _max_frames_in_cache)
			{
				if (_writer){
					_lock.unlock();
					save_frames();
					_lock.lock();
				}
				else
				{
					_prepaired_cache.clear();
				}
			}
		}

		_lock.unlock();
		return true;
	}

	void irb_frames_cache::save_frames(bool wait)
	{
		std::lock_guard<std::mutex> guard(_lock_writer);
		if (_writer){
			_lock.lock();
			irb_frames_map_t flush_cache(std::move(_prepaired_cache));
			_lock.unlock();

			if (flush_cache.empty())
				return;

			handle_holder save_event(INVALID_HANDLE_VALUE);
			if (wait){
				save_event.swap(sync_helpers::create_basic_event_object(false));
			}

			if (_b_stop_requested)
				return;

			_queue_mtx.lock();
			_queue.push({ std::move(flush_cache), _writer, save_event.get() });
			_queue_mtx.unlock();
			
			sync_helpers::release_semaphore(_queue_semaphore);

			if (wait){
				sync_helpers::wait(save_event);
			}

			_file_counter++;
		}
	}

	void irb_frames_cache::flush_frames(const irb_frames_map_t & frames, const writer_ptr_t &writer)
	{
		//std::lock_guard<std::mutex> guard(_lock_writer);
		if (writer){
			try{

				writer->flush_frames(frames, _file_counter);
			}
			catch (const irb_file_helper::irb_file_exception& exc)
			{
				auto error = exc.what();
				return;
			}
		}
	}

	void irb_frames_cache::set_max_frames_for_writer(uint16_t max_frames)
	{ 
		_max_frames_for_writer = max_frames;
		std::lock_guard<std::mutex> guard(_lock_writer);
		if (_writer){
			_writer->set_max_frames_per_file(max_frames);
		}
	}

	void irb_frames_cache::set_writer(const writer_ptr_t &writer)
	{
		save_frames(true);

		std::lock_guard<std::mutex> guard(_lock_writer);
		_writer = writer;
		if (_max_frames_for_writer > 0 && _writer){
			_writer->set_max_frames_per_file(_max_frames_for_writer);
		}

		_file_counter = 0;
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

			_queue_mtx.lock();
			auto data = _queue.front();
			_queue.pop();
			_queue_mtx.unlock();

			flush_frames(data.frames,data.writer);
			if (data.save_event != INVALID_HANDLE_VALUE && data.save_event != 0)
				sync_helpers::set_event(data.save_event);

			if (_b_stop_requested)
				break;

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

	irb_frames_writer::irb_frames_writer(
		camera_offset_t camera_offset, 
		const std::wstring & dir, const std::wstring & name_pattern, 
		new_irb_file_func_t new_irb_file_func,
		uint32_t max_frames_per_file
		) :
		_camera_offset(camera_offset),
		_dir(dir),
		_name_pattern(name_pattern),
		_cur_file_index(0),
		_last_frame_index(1),
		_new_irb_file_func(new_irb_file_func),
		_max_frames_per_file(max_frames_per_file)
	{
	}
	irb_frames_writer::~irb_frames_writer(){}


	irb_file_helper::stream_ptr_t
		create_irb_file(
		const std::wstring & dir, const std::wstring & name_pattern,
		camera_offset_t camera_offset,
		uint16_t file_counter,
		uint32_t max_frames_per_file,
		std::wstring & file_name
		)
	{
		wchar_t buf[5];
		swprintf_s(buf, L"%04d", (int)file_counter);
		file_name = dir + name_pattern + buf + L".irb";
		return irb_file_helper::create_irb_file(file_name, camera_offset, irb_file_helper::irb_file_version::patched, max_frames_per_file);
	}

	void irb_frames_writer::flush_frames(const irb_frames_map_t& frames, uint16_t file_counter)
	{
		if (frames.empty() || _max_frames_per_file == 0)
			return;

		if (!_irb_file){

			try{
				std::wstring file_name;
				auto irb_stream = create_irb_file(_dir, _name_pattern, _camera_offset, _cur_file_index++, _max_frames_per_file, file_name);
				_irb_file.swap(std::make_unique<irb_file_helper::IRBFile>(irb_stream));
				if (_new_irb_file_func)
					_new_irb_file_func(file_name);

			}
			catch (const irb_file_helper::irb_file_exception&)
			{
				return;
			}
		}

		auto max_frames_in_file = _irb_file->max_number_frames();
		auto current_frames_in_file = _irb_file->count_frames();
		auto number_frames = (uint32_t)frames.size();
		auto current_frames_number = number_frames;

		if (number_frames > max_frames_in_file - current_frames_in_file){
			current_frames_number = max_frames_in_file - current_frames_in_file;
		}

		std::vector<irb_frame_shared_ptr_t> list;
		for (auto & frame : frames)
		{
			frame.second->id = _last_frame_index++;
			list.emplace_back(frame.second);
		}

		int start_index = 0;
		do 
		{
			number_frames -= current_frames_number;

			std::vector<irb_frame_shared_ptr_t> current_list(list.begin() + start_index, list.begin() + start_index + current_frames_number);
			start_index += current_frames_number;

			try
			{
				_irb_file->append_frames(current_list);
			}
			catch (const irb_file_helper::irb_file_exception&)
			{
				return;
			}

			auto max_frames_per_file = _max_frames_per_file;
			if (_irb_file->count_frames() == _irb_file->max_number_frames())
			{

				if (number_frames > 0)
				{
					try{
						std::wstring file_name;
						auto irb_stream = create_irb_file(_dir, _name_pattern, _camera_offset, _cur_file_index++, max_frames_per_file, file_name);
						_irb_file.swap(std::make_unique<irb_file_helper::IRBFile>(irb_stream));
						if (_new_irb_file_func)
							_new_irb_file_func(file_name);

					}
					catch (const irb_file_helper::irb_file_exception&)
					{
						return;
					}
				}
				else
					_irb_file.release();

			}

			if (max_frames_per_file == 0)
				return;

			if (number_frames < max_frames_per_file)
				current_frames_number = number_frames;
			else
				current_frames_number = max_frames_per_file;



		} while (number_frames > 0);

	}

}