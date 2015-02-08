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

	using irb_frame_shared_ptr_t = std::shared_ptr<irb_frame_helper::IRBFrame>;
	using new_irb_frame_process_func_t = std::function<void(irb_frame_helper::frame_id_t)>;

	class irb_frames_cache
	{
	public:
		irb_frames_cache();
		~irb_frames_cache();

		void start_cache(new_irb_frame_process_func_t new_irb_frame_process_func);
		void stop_cache();

		void push_frame(const irb_frame_shared_ptr_t & frame);
		irb_frame_shared_ptr_t get_frame_by_id(irb_frame_helper::frame_id_t frame_id);
		irb_frame_shared_ptr_t get_next_frame();

	private:
		void cache_loop(new_irb_frame_process_func_t new_irb_frame_process_func);
	private:
		volatile bool _b_stop_requested;

		std::queue<irb_frame_shared_ptr_t> _queue;
		std::mutex _queue_mtx;
		handle_holder _queue_semaphore;
		std::thread _cache_loop_thread;


		std::list<irb_frame_shared_ptr_t> _cache;
		uint8_t _cache_size;
		irb_frame_helper::frame_id_t _last_requested_id;
		sync_helpers::rw_lock _cache_lock;

		uint8_t _max_cache_size;
	};

}