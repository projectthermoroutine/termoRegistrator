
#include "irb_frame_spec_info.h"
#include "irb_frame_helper.h"

#include <algorithm>
#include <common\fs_helpers.h>
#include <fstream>
#include <common/string_utils.h>

namespace irb_frame_spec_info
{

#define CONVERT_TO_UTF8(_utf16_str)  string_utils::convert_wchar_to_utf8((_utf16_str))

#define MAX_NAME_LENGTH_CB  64
#define MAX_ITEM_LENGTH_CB  255
#define MAX_SMALL_ITEM_LENGTH_CB  55

	typedef uint64_t distance_t;
	typedef double data_time_t;
	typedef uint32_t length_t;


#pragma pack(push,1)
	typedef struct _SpecInfo // ���������� � ����
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
		
		struct CorrectionT
		{
			uint8_t enabled;
			irb_frame_helper::correction_T_params_t params;
		};

		CorrectionT correction_T;

	} SpecInfo;
#pragma pack(pop)

	std::ostream & operator<<(std::ostream & out, const irb_frame_spec_info &frame_spec_info)
	{
		SpecInfo info = { 0 };
		auto device_name = CONVERT_TO_UTF8(frame_spec_info._device_name);
		auto path = CONVERT_TO_UTF8(frame_spec_info.coords.path);
		auto line = CONVERT_TO_UTF8(frame_spec_info.coords.line);

		info.data_time = frame_spec_info.frame_time;
		info.line_length = static_cast<length_t>(line.size());
		info.path_length = static_cast<length_t>(path.size());
		info.device_length = static_cast<length_t>(device_name.size());

		const float offset_m = (float)frame_spec_info.coords.offset / 1000;
		std::string str_coordinate = std::to_string(frame_spec_info.coords.picket) + " �� " + std::to_string(offset_m) + " �";

		info.coordinate_length = static_cast<length_t>(str_coordinate.size());

		strncpy_s(reinterpret_cast<char*>(&info.path), MAX_ITEM_LENGTH_CB, path.c_str(), _TRUNCATE);
		strncpy_s(reinterpret_cast<char*>(&info.line), MAX_ITEM_LENGTH_CB, line.c_str(), _TRUNCATE);
		strncpy_s(reinterpret_cast<char*>(&info.device), MAX_ITEM_LENGTH_CB, device_name.c_str(), _TRUNCATE);
		strncpy_s(reinterpret_cast<char*>(&info.coordinate), MAX_SMALL_ITEM_LENGTH_CB, str_coordinate.c_str(), _TRUNCATE);

		if (frame_spec_info._frame.correction_T_enabled())
		{
			info.correction_T.enabled = 1;
			info.correction_T.params.factor = frame_spec_info._frame.correction_T_params().factor;
			info.correction_T.params.offset = frame_spec_info._frame.correction_T_params().offset;
		}

		out.write(reinterpret_cast<const char*>(&info), sizeof(SpecInfo));
		return out;
	}

	irb_frame_spec_info::irb_frame_spec_info(const irb_frame_helper::IRBFrame & frame,
		const irb_frame_position_info & frame_position_info,
		const std::wstring & device_name) 
		: _frame(frame)
		, coords(frame.coords)
		, frame_time(frame.header.presentation.imgTime)
		, _frame_position_info(frame_position_info)
		, _device_name(device_name)
	{
	}



}