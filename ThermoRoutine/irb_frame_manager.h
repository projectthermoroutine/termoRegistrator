#pragma once
#include <Windows.h>
#include <vector>
#include <memory>
#include <mutex>
#include <common\handle_holder.h>

#include "tv_helper.h"

#include "irb_frame_helper.h"
#include "irb_file_helper.h"

#include <common\sync_helpers.h>
#include <list>
#include "irb_frame_image_dispatcher.h"
#include "irb_frame_spec_info.h"

namespace irb_frame_manager
{
	using irb_file_t = std::shared_ptr < irb_file_helper::IRBFile >;
	using irb_files_list_t = std::vector < irb_file_t > ;
	using irb_frame_ptr_t = std::unique_ptr < irb_frame_helper::IRBFrame >;
	using irb_frame_shared_ptr_t = std::shared_ptr < irb_frame_helper::IRBFrame >;

	template<typename TFrameKey>
	using get_frame_func_t = std::function<irb_frame_shared_ptr_t(const TFrameKey&)>;

	class frames_block_saver
	{
	public:
		frames_block_saver(int max_frames_in_memory = 1200);

	public:
		bool operator()(const std::vector<uint32_t> & frames_indexes,
			get_frame_func_t<uint32_t>  get_frame_by_index,
			const std::string & fileNamePattern,
			uint16_t frames_per_file
			);

	private:
		int _max_frames_in_memory;
	};

	template<typename TSaveStrategy = frames_block_saver>
	bool save_frames(const std::vector<uint32_t> & frames_indexes, get_frame_func_t<uint32_t>  get_frame_by_index, const std::string & fname, uint16_t frames_per_file = 0)
	{
		if (frames_indexes.empty() || !get_frame_by_index)
		{
			return false;
		}
		TSaveStrategy save_strategy(1200);
		return save_strategy(frames_indexes, get_frame_by_index, fname, frames_per_file);
	}

	bool save_frames(const std::vector<irb_frame_shared_ptr_t> & frames, const std::string & fname, uint16_t frames_per_file = 0);
	bool save_frame(const irb_frame_shared_ptr_t& frame, const std::string & device_name, const irb_frame_spec_info::irb_frame_position_info & frame_position_info, const std::string & fname);

}