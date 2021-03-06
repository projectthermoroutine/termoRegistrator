#pragma once
#include <Windows.h>
#include <vector>
#include <memory>
#include "irb_frame_helper.h"
#include <common\handle_holder.h>
#include <fstream>
#include <queue>
#include <list>
#include <map>
#include <common\sync_helpers.h>
#include <future>
#include "irb_file_helper.h"

namespace irb_frame_delegates
{

	using irb_frame_shared_ptr_t = std::shared_ptr<irb_frame_helper::IRBFrame>;

	const auto default_frames_per_file = 1200UL;
	//using namespace irb_frame_helper;

	using irb_frame_list_t = std::deque<irb_frame_shared_ptr_t>;
	using frame_id_t = uint32_t;
	using irb_frames_map_t = std::map<irb_frame_helper::frame_id_t, irb_frame_shared_ptr_t>;
	class irb_frames_writer;
	//using writer_ptr_t = std::unique_ptr<irb_frames_writer>;
	using writer_ptr_t = std::shared_ptr<irb_frames_writer>;

	using prepare_frame_func_t = std::function<bool(const irb_frame_shared_ptr_t&)>;

	class irb_frames_cache final
	{

	public:
		irb_frames_cache(uint16_t max_frames_in_cache, prepare_frame_func_t prepare_frame_func, uint16_t prepaire_delay_frames_number = 25);
	public:
		~irb_frames_cache();

		bool process_frame(const irb_frame_shared_ptr_t& frame);
		bool process_frame_non_cache(const irb_frame_shared_ptr_t& frame);

		writer_ptr_t set_writer(const writer_ptr_t &writer);
		void save_frames(bool wait = false);
		void save_frames_to_tmp_stream();


		void set_max_frames_in_cache(uint16_t max_frames_in_cache) { _max_frames_in_cache = max_frames_in_cache; }
		void set_max_frames_for_writer(uint16_t max_frames);
		void reset()
		{
			_InterlockedCompareExchange8((char*)(&_busy), 1, 0);
			//while (_InterlockedCompareExchange8((char*)(&_state), 1, 0) != 0);
			_lock.lock();

			if (_prepaired_cache.size() < _max_frames_in_cache){
				_lock.unlock();
				save_frames();
				_lock.lock();
			}

			_prepaired_cache.clear(); _not_prepaired_cache.clear(); 

			_lock.unlock();
			//_InterlockedAnd8((char*)(&_state), 0);

			_InterlockedAnd8((char*)(&_busy), 0);

		}

		//irb_frame_shared_ptr_t get_frame_by_id(irb_frame_helper::frame_id_t frame_id);


	private:
		uint16_t _max_frames_in_cache;
		irb_frames_map_t _prepaired_cache;
		irb_frame_list_t _not_prepaired_cache;

		uint16_t _max_frames_for_writer;
		writer_ptr_t _writer;
		prepare_frame_func_t _prepare_frame_func;

		uint16_t _prepaire_delay_frames_number;

		uint16_t _file_counter;
		uint32_t _frame_counter;

		std::mutex _lock;
		std::mutex _lock_writer;

		sync_helpers::rw_lock _lock_frames_map;

		volatile byte _state;
		volatile byte _busy;

		struct queue_item
		{
			queue_item() :save_event(0), flag(false){}
			queue_item(irb_frames_map_t&& frames_, const writer_ptr_t &writer_, HANDLE event, bool flag_) :frames(frames_), writer(writer_), save_event(event), flag(flag_){}
			irb_frames_map_t frames;
			writer_ptr_t writer;
			HANDLE save_event;
			bool flag;
		};

		std::queue<queue_item> _queue;
		std::mutex _queue_mtx;
		handle_holder _queue_semaphore;
		std::thread _flush_frames_thread;

		volatile bool _b_stop_requested;
	private:
		void start_flush_thread();
		void stop_flush_thread();
		void flush_loop();
		void flush_frames(const irb_frames_map_t & frames, const writer_ptr_t &writer, bool tmp_stream);
		void _save_frames(irb_frames_map_t && frames, const writer_ptr_t &writer, bool wait, bool tmp_stream);

	};


	using camera_offset_t = int32_t;
	using new_irb_file_func_t = std::function<void(const std::wstring&)>;
	class irb_frames_writer final
	{
	public:
		irb_frames_writer(camera_offset_t camera_offset,
						const std::wstring & dir,
						const std::wstring & name_pattern,
						new_irb_file_func_t new_irb_file_func,
						uint32_t max_frames_per_file = 0
						);
		~irb_frames_writer();

		void flush_frames(const irb_frames_map_t& frames, uint16_t file_counter);
		void flush_frames_to_tmp_file(const irb_frames_map_t& frames, uint16_t file_counter);
		void set_max_frames_per_file(uint16_t max_frames_per_file) { _InterlockedExchange(&_max_frames_per_file, max_frames_per_file); }

	private:
		std::wstring _dir;
		std::wstring _name_pattern;
		uint16_t _cur_file_index;
		uint32_t _last_frame_index;
		std::ofstream _file;
		camera_offset_t _camera_offset;

		std::unique_ptr<irb_file_helper::IRBFile> _irb_file;
		uint32_t _max_frames_per_file;
		new_irb_file_func_t _new_irb_file_func;
	};




}