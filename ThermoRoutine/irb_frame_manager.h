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

	bool save_frames(const std::vector<irb_frame_shared_ptr_t> & frames, const std::string & fname);
	bool save_frame(const irb_frame_shared_ptr_t& frame, const irb_frame_spec_info::irb_frame_position_info & frame_position_info, const std::string & fname);

}