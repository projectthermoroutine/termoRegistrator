#include "irb_frame_manager.h"

namespace irb_frame_manager
{
	using namespace irb_frame_helper;
	using namespace irb_file_helper;


	bool save_frames(const std::vector<irb_frame_shared_ptr_t> & frames, const std::string & fname)
	{
		if (frames.size() == 0)
		{
			return false;
		}
		try{
			auto file_stream = create_irb_file(fname, irb_file_version::original, (uint32_t)frames.size());
			IRBFile f(file_stream);
			f.append_frames(frames);
		}
		catch (const irb_file_helper::irb_file_exception&)
		{
			return false;
		}

		return true;

	}
	bool save_frame(const irb_frame_shared_ptr_t& frame, const irb_frame_spec_info::irb_frame_position_info & frame_position_info, const std::string & fname)
	{
		if (!frame || fname.empty())
			return false;

		try{
			auto file_stream = create_irb_file(fname, frame->coords.camera_offset, irb_file_version::original, 2);
			IRBFile f(file_stream);

			irb_block_info_t frame_block_info = { 1, 0, 101, 1 };
			f.write_block_data(frame_block_info, *frame);

			irb_block_info_t spec_block_info = { 9, 9, 101, 2 };
			irb_frame_spec_info::irb_frame_spec_info spec(*frame, frame_position_info);

			f.write_block_data(spec_block_info, spec);
		}
		catch (const irb_file_helper::irb_file_exception&)
		{
			return false;
		}

		return true;
	}
}
