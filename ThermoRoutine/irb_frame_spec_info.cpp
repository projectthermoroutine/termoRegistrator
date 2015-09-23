
#include "irb_frame_spec_info.h"
#include "irb_frame_helper.h"

#include <algorithm>
#include <common\fs_helpers.h>
#include <fstream>


namespace irb_frame_spec_info
{

#define MAX_NAME_LENGTH_CB  64
#define MAX_ITEM_LENGTH_CB  255
#define MAX_SMALL_ITEM_LENGTH_CB  55

	typedef uint64_t distance_t;
	typedef double data_time_t;
	typedef uint32_t length_t;


#pragma pack(push,1)
	typedef struct _SpecInfo // информация о пути
	{
		distance_t distance;
		data_time_t data_time;
		length_t line_length;
		length_t path_length;
		length_t device_length;
		length_t coordinate_length;
		char	 line[MAX_ITEM_LENGTH_CB];
		char	 path[MAX_ITEM_LENGTH_CB];
		char	 device[MAX_ITEM_LENGTH_CB];
		char	 coordinate[MAX_SMALL_ITEM_LENGTH_CB];
	}SpecInfo;
#pragma pack(pop)

	std::ostream & operator<<(std::ostream & out, const irb_frame_spec_info &frame_spec_info)
	{
		SpecInfo info = { 0 };
		info.data_time = frame_spec_info.frame_time;
		info.line_length = frame_spec_info.coords.line.size();
		info.path_length = frame_spec_info.coords.path.size();
		info.device_length = frame_spec_info._device_name.size();

		std::string str_coordinate = std::to_string(frame_spec_info.coords.picket) + " пк " + std::to_string(frame_spec_info.coords.offset / 1000) + " м";

		info.coordinate_length = str_coordinate.size();

		strncpy_s(reinterpret_cast<char*>(&info.path), MAX_ITEM_LENGTH_CB, frame_spec_info.coords.path.c_str(), _TRUNCATE);
		strncpy_s(reinterpret_cast<char*>(&info.line), MAX_ITEM_LENGTH_CB, frame_spec_info.coords.line.c_str(), _TRUNCATE);
		strncpy_s(reinterpret_cast<char*>(&info.device), MAX_ITEM_LENGTH_CB, frame_spec_info._device_name.c_str(), _TRUNCATE);
		strncpy_s(reinterpret_cast<char*>(&info.coordinate), MAX_SMALL_ITEM_LENGTH_CB, str_coordinate.c_str(), _TRUNCATE);

		out.write(reinterpret_cast<const char*>(&info), sizeof(SpecInfo));
		return out;
	}

	irb_frame_spec_info::irb_frame_spec_info(const irb_frame_helper::IRBFrame & frame,
		const irb_frame_position_info & frame_position_info,
		const std::string & device_name) :
		coords(frame.coords), 
		frame_time(frame.header.presentation.imgTime),
		_frame_position_info(frame_position_info),
		_device_name(device_name)
	{
	}



}