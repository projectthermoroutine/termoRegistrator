#include <Windows.h>
#include <vector>
#include <memory>
#include "irb_frame_helper.h"
#include <common\handle_holder.h>
#include <fstream>
#include <queue>
#include <list>
#include <iterator>
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
		LOG_STACK();
		start_flush_thread();
	}
	irb_frames_cache::~irb_frames_cache()
	{
		LOG_STACK();
		_InterlockedCompareExchange8((char*)(&_busy), 1, 0);
		save_frames(true);
		stop_flush_thread();
		std::lock_guard<decltype(_queue_mtx)> lock(_queue_mtx);
		while (!_queue.empty())
		{
			auto data = _queue.front();
			_queue.pop();
			if (data.save_event != INVALID_HANDLE_VALUE && data.save_event != 0)
				sync_helpers::set_event(data.save_event);
		} 
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

		std::unique_lock<decltype(_lock)> lock(_lock);

		frame->id = ++_frame_counter;

		auto res = _prepare_frame_func(frame);
		if (res)
		{
			_prepaired_cache.emplace(frame->id, frame);

			auto cache_size = _prepaired_cache.size();
			if (cache_size == _max_frames_in_cache)
			{
				_lock_writer.lock();
				if (_writer){
					_lock_writer.unlock();
					lock.unlock();
					save_frames();
					return true;
				}
				else
				{
					_lock_writer.unlock();
					_prepaired_cache.clear();
				}
			}
		}

		return true;
	}

	void irb_frames_cache::save_frames_to_tmp_stream()
	{
		LOG_STACK();
		save_frames_common(_prepaired_cache, true, true);
	}

	void irb_frames_cache::save_frames(bool wait)
	{
		LOG_STACK();
		save_frames_common(std::move(_prepaired_cache), wait, false);
	}

	template<typename TFrames>
	void irb_frames_cache::save_frames_common(TFrames&& frames, bool wait /*= false*/, bool tmp_stream /*= false*/)
	{
		LOG_STACK();
		_lock_writer.lock();
		const auto writer = _writer;
		_lock_writer.unlock();
		if (writer){
			_lock.lock();
			irb_frames_map_t flush_cache(std::forward<TFrames>(frames));
			_lock.unlock();

			_save_frames(std::move(flush_cache), writer, wait, tmp_stream);
		}
	}


	void irb_frames_cache::_save_frames(irb_frames_map_t && frames, const writer_ptr_t &writer, bool wait, bool tmp_stream)
	{
		LOG_STACK();
		if (writer){
			if (frames.empty())
				return;

			handle_holder save_event(INVALID_HANDLE_VALUE);
			if (wait){
				save_event = sync_helpers::create_basic_event_object(false);
			}

			if (_b_stop_requested)
				return;

			_queue_mtx.lock();
			_queue.push({ std::move(frames), writer, save_event.get(), tmp_stream });
			_queue_mtx.unlock();
			
			sync_helpers::release_semaphore(_queue_semaphore);

			if (wait){
				sync_helpers::wait(save_event);
			}
		}
	}

	void irb_frames_cache::flush_frames(irb_frames_map_t && frames, const writer_ptr_t &writer, bool tmp_stream)
	{
		LOG_STACK();
		if (writer){
			try{
				if (tmp_stream)
					writer->flush_frames_to_tmp_file(std::move(frames), _file_counter);
				else
					writer->flush_frames(std::move(frames), _file_counter);
			}
			catch (const irb_file_helper::irb_file_exception& /*exc*/)
			{
				//auto error = exc.what();
				return;
			}
		}
	}

	void irb_frames_cache::set_max_frames_for_writer(uint16_t max_frames)
	{ 
		LOG_STACK();
		_max_frames_for_writer = max_frames;
		std::lock_guard<std::mutex> guard(_lock_writer);
		if (_writer){
			_writer->set_max_frames_per_file(max_frames);
		}
	}

	writer_ptr_t irb_frames_cache::set_writer(const writer_ptr_t &writer)
	{
		LOG_STACK();
		save_frames(true);

		std::lock_guard<std::mutex> guard(_lock_writer);

		writer_ptr_t prev_writer = _writer;
		_writer = writer;
		if (_max_frames_for_writer > 0 && _writer){
			_writer->set_max_frames_per_file(_max_frames_for_writer);
		}

		_file_counter = 0;

		return prev_writer;
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

			flush_frames(std::move(data.frames),data.writer,data.flag);
			if (data.save_event != INVALID_HANDLE_VALUE && data.save_event != 0)
				sync_helpers::set_event(data.save_event);

			if (_b_stop_requested)
				break;

		}
	}


	void irb_frames_cache::start_flush_thread()
	{
		LOG_STACK();
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
		LOG_STACK();
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
		new_irb_file_func_t &&new_irb_file_func,
		irb_file_changed_func_t &&irb_file_changed_func,
		get_irb_frame_key_func_t &&get_irb_frame_key_func,
		uint32_t max_frames_per_file
		) :
		_camera_offset(camera_offset),
		_dir(dir),
		_name_pattern(name_pattern),
		_cur_file_index(0),
		_last_frame_index(1),
		_new_irb_file_func(std::move(new_irb_file_func)),
		_irb_file_changed_func(std::move(irb_file_changed_func)),
		_get_irb_frame_key_func(std::move(get_irb_frame_key_func)),
		_max_frames_per_file(max_frames_per_file), 
		_last_frame_index_in_current_file(0)
	{
		if (!_get_irb_frame_key_func)
			_get_irb_frame_key_func = [](const IRBFrame&){return (uint32_t)0; };
	}
	irb_frames_writer::~irb_frames_writer(){}


	std::wstring
		_create_irb_file(
		const std::wstring & name_pattern,
		camera_offset_t camera_offset,
		uint32_t max_frames_per_file,
		std::function<std::wstring(const std::wstring&)> gen_file_name_func,
		std::wstring & file_name
		)
	{
		file_name = gen_file_name_func(name_pattern);
		irb_file_helper::create_irb_file(file_name, camera_offset, irb_file_helper::irb_file_version::patched, max_frames_per_file);
		return file_name;
	}

	std::wstring generate_file_name(const std::wstring & name_pattern)
	{
		return name_pattern + L".irb";
	}
	std::wstring generate_tmp_file_name(const std::wstring & name_pattern)
	{
		return name_pattern + L".irb.tmp";
	}


	std::wstring
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
		file_name = dir + name_pattern + buf;
		return _create_irb_file(file_name, camera_offset, max_frames_per_file, generate_file_name, file_name);
	}


	std::wstring
		create_tmp_irb_file(
		const std::wstring & dir, const std::wstring & name_pattern,
		camera_offset_t camera_offset,
		uint16_t file_counter,
		uint32_t max_frames_per_file,
		std::wstring & file_name
		)
	{
		wchar_t buf[5];
		swprintf_s(buf, L"%04d", (int)file_counter);
		file_name = dir + name_pattern + buf;
		return _create_irb_file(file_name, camera_offset, max_frames_per_file, generate_tmp_file_name, file_name);
	}

	void irb_frames_writer::flush_frames_to_tmp_file(irb_frames_map_t&& frames, uint16_t /*file_counter*/)
	{

		LOG_STACK();
		std::vector<irb_frame_shared_ptr_t> list;
		for (auto & frame : frames)
		{
			frame.second->id = _last_frame_index++;
			list.emplace_back(frame.second);
		}

		try
		{
			std::wstring file_name;
			auto irb_stream = create_tmp_irb_file(_dir, _name_pattern, _camera_offset, _cur_file_index + 1, static_cast<std::uint32_t>(frames.size()), file_name);
			irb_file_helper::IRBFile irb_file(irb_stream);
			irb_file.open();
			irb_file.append_frames(list);

			if (_new_irb_file_func)
				_new_irb_file_func(file_name);

		}
		catch (const irb_file_helper::irb_file_exception&)
		{
			return;
		}
	}

	void irb_frames_writer::flush_frames(irb_frames_map_t&& frames, uint16_t /*file_counter*/)
	{
		LOG_STACK();
		if (frames.empty() || _max_frames_per_file == 0)
			return;

		auto max_frames_per_file = _max_frames_per_file;

		const auto create_new_file = [&]
		{
			try{
				std::wstring file_name;
				_irb_file_path = create_irb_file(_dir, _name_pattern, _camera_offset, _cur_file_index++, max_frames_per_file, file_name);
				_irb_file = std::make_unique<irb_file_helper::IRBFile>(_irb_file_path);
				_irb_file->open();

				if (_new_irb_file_func)
					_new_irb_file_func(file_name);

				return true;

			}
			catch (const irb_file_helper::irb_file_exception&)
			{
				return false;
			}

		};

		if (!_irb_file)
		{
			if (!create_new_file())
				return;
		}

		auto max_frames_in_file = _irb_file->max_number_frames();
		auto current_frames_in_file = _irb_file->count_frames();
		auto number_frames = (uint32_t)frames.size();
		auto current_frames_number = number_frames;

		if (number_frames > max_frames_in_file - current_frames_in_file){
			current_frames_number = max_frames_in_file - current_frames_in_file;
		}

		std::list<irb_frame_shared_ptr_t> list;
		for (auto && frame : frames)
		{
			frame.second->id = _last_frame_index++;
			list.emplace_back(std::move(frame).second);
		}

		do // while (number_frames > 0);
		{
			number_frames -= current_frames_number;

			auto last_iter = list.begin();
			std::advance(last_iter, current_frames_number);

			irb_frames_infos_t frames_infos;
			std::vector<irb_frame_shared_ptr_t> current_list;
			current_list.reserve(current_frames_number);
			for (auto iter = list.begin(); iter != last_iter;)
			{
				frames_infos.push_back({ _last_frame_index_in_current_file++, _get_irb_frame_key_func(*iter->get()) });
				current_list.emplace_back(std::move(*iter));
				iter = list.erase(iter);

			}

			try
			{
				_irb_file->append_frames(current_list);
				if (_irb_file_changed_func)
					_irb_file_changed_func(std::move(frames_infos), _irb_file_path);
			}
			catch (const irb_file_helper::irb_file_exception&)
			{
				return;
			}

			if (_irb_file->count_frames() == _irb_file->max_number_frames())
			{

				if (number_frames > 0)
				{
					if (!create_new_file())
						return;
				}
				else{
					_last_frame_index_in_current_file = 0;
					_irb_file.reset();
				}

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