#include "irb_frame_manager.h"
#include <streambuf>
namespace irb_frame_manager
{
	using namespace irb_frame_helper;
	using namespace irb_file_helper;

	frames_block_saver::frames_block_saver(int max_frames_in_memory) :_max_frames_in_memory(max_frames_in_memory){}

	bool frames_block_saver::operator()(const std::vector<uint32_t> & frames_indexes,
			get_frame_func_t<uint32_t>  get_frame_by_index,
			const std::wstring & fileNamePattern,
			uint16_t frames_per_file
			)
		{
			int number_frames = frames_indexes.size();

			long start_index = 0;
			long end_index = number_frames;
			if (number_frames > _max_frames_in_memory)
				end_index = _max_frames_in_memory;

			bool use_postfix = false;
			if (end_index == _max_frames_in_memory &&
				frames_per_file < _max_frames_in_memory
				)
				use_postfix = true;

			int postfix = 0;
			for (;;)
			{
				std::vector<irb_frame_shared_ptr_t> frames;
				for (long i = start_index; i < end_index; i++)
				{
					frames.emplace_back(get_frame_by_index(frames_indexes[i]));
				}
				if (frames.empty())
					return false;

				std::wstring file_name_pattern(fileNamePattern);
				if (use_postfix)
					file_name_pattern += std::to_wstring(postfix++);

				save_frames(frames, file_name_pattern, frames_per_file);

				if (end_index == number_frames - 1)
					break;

				start_index = end_index;

				if (end_index + _max_frames_in_memory > number_frames)
					end_index = number_frames;
				else
					end_index += _max_frames_in_memory;
			}
			return true;

		}

	//template<typename TSaveStrategy>
	//bool 
	//	save_frames(
	//	const std::vector<uint32_t> & frames_indexes, 
	//	get_frame_func_t<uint32_t>  get_frame_by_index, 
	//	const std::string & fileNamePattern,
	//	uint16_t frames_per_file
	//)
	//{
	//	if (frames_indexes.empty() || !get_frame_by_index)
	//	{
	//		return false;
	//	}

	//	TSaveStrategy save_strategy;

	//	return save_strategy(frames_indexes, get_frame_by_index, fileNamePattern, frames_per_file);

	//	//static int max_frames_in_memory = 1200;

	//	//int number_frames = frames_indexes.size();

	//	//long start_index = 0;
	//	//long end_index = number_frames;
	//	//if (number_frames > max_frames_in_memory)
	//	//	end_index = max_frames_in_memory;

	//	//bool use_postfix = false;
	//	//if (end_index == max_frames_in_memory && 
	//	//	frames_per_file < max_frames_in_memory
	//	//)
	//	//	use_postfix = true;

	//	//int postfix = 0;
	//	//for (;;)
	//	//{
	//	//	std::vector<irb_frame_shared_ptr_t> frames;
	//	//	for (long i = start_index; i < end_index; i++)
	//	//	{
	//	//		frames.emplace_back(get_frame_by_index(frames_indexes[i]));
	//	//	}
	//	//	if (frames.empty())
	//	//		return false;

	//	//	std::string file_name_pattern(fileNamePattern);
	//	//	if (use_postfix)
	//	//		file_name_pattern += std::to_string(postfix++);

	//	//	save_frames(frames, file_name_pattern, frames_per_file);

	//	//	if (end_index == number_frames - 1)
	//	//		break;

	//	//	start_index = end_index;

	//	//	if (end_index + max_frames_in_memory > number_frames)
	//	//		end_index = number_frames;
	//	//	else
	//	//		end_index += max_frames_in_memory;
	//	//}
	//	//return true;
	//}



	bool save_frames(const std::vector<irb_frame_shared_ptr_t> & frames, const std::wstring & fname, uint16_t frames_per_file)
	{
		if (frames.size() == 0)
		{
			return false;
		}

		if (frames_per_file == 0 || frames.size() <= frames_per_file){
			frames_per_file = frames.size();

			auto file_stream = create_irb_file(fname + L".irb", irb_file_version::patched, (uint32_t)frames_per_file);
			IRBFile f(file_stream);
			f.append_frames(frames);

		}
		else
		{
			auto first_el = frames.cbegin();
			std::vector<irb_frame_shared_ptr_t>::const_iterator last_el = first_el + frames_per_file;
			uint16_t file_index = 0;
			uint16_t last_frames_number = frames.size() - frames_per_file;

			while (first_el != frames.cend()){

				auto file_stream = create_irb_file(fname + L"_" + std::to_wstring(file_index++) + L".irb", 
													irb_file_version::patched, 
													(uint32_t)frames_per_file
													);
				IRBFile f(file_stream);

				f.append_frames({ first_el, last_el });

				first_el = last_el;
				if (last_frames_number < frames_per_file)
				{
					last_el = frames.cend();
					continue;
				}
				last_el = first_el + frames_per_file;
				last_frames_number -= frames_per_file;

			}

		}

		return true;

	}
	bool save_frame(const irb_frame_shared_ptr_t & frame, const std::wstring & fname)
	{
		auto file_stream = create_irb_file(fname, irb_file_version::patched, 1);
		IRBFile f(file_stream);
		f.append_frames({ frame });

		return true;
	}

	bool save_frame(const std::vector<char>& frame_raw_data, const std::wstring & fname)
	{
		if (frame_raw_data.empty())
			return false;

		irb_frame_shared_ptr_t frame(irb_frame_helper::create_frame_by_raw_data(frame_raw_data));
		if (!frame)
			return false;

		return save_frame(frame, fname);
	}

	bool save_frame(const std::vector<char>& frame_raw_data, const std::string & device_name, const irb_frame_spec_info::irb_frame_position_info & frame_position_info, const std::wstring & fname)
	{
		if (frame_raw_data.empty())
			return false;

		irb_frame_shared_ptr_t frame(irb_frame_helper::create_frame_by_raw_data(frame_raw_data));
		if (!frame)
			return false;

		return save_frame(frame, device_name, frame_position_info, fname);
	}

	bool save_frame(const irb_frame_shared_ptr_t& frame, const std::string & device_name, const irb_frame_spec_info::irb_frame_position_info & frame_position_info, const std::wstring & fname)
	{
		if (!frame || fname.empty())
			return false;

		try{
			auto file_stream = create_irb_file(fname, frame->coords.camera_offset, irb_file_version::original, 2);
			IRBFile f(file_stream);

			irb_block_info_t frame_block_info = { 1, 0, 101, 1 };
			f.write_block_data(frame_block_info, *frame);

			irb_block_info_t spec_block_info = { 9, 9, 101, 2 };
			irb_frame_spec_info::irb_frame_spec_info spec(*frame, frame_position_info, device_name);

			f.write_block_data(spec_block_info, spec);
		}
		catch (const irb_file_helper::irb_file_exception&)
		{
			return false;
		}

		return true;
	}
}
