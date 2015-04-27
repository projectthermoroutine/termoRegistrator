
#include "irb_frame_spec_info.h"
#include "irb_frame_helper.h"

#include <algorithm>
#include <common\fs_helpers.h>
#include <fstream>


namespace irb_frame_spec_info
{

#define MAX_NAME_LENGTH_CB  64

#pragma pack(push,1)
	typedef struct _FrameCoordPresentation // ���������� � ����
	{
		uint64_t coordinate; // �������� ���������� 
		char path[MAX_NAME_LENGTH_CB];		// ����
		char line[MAX_NAME_LENGTH_CB];
		double time;

	}FrameCoordPresentation;
#pragma pack(pop)

	std::ostream & operator<<(std::ostream & out, const irb_frame_spec_info &frame_spec_info)
	{
		FrameCoordPresentation info;
		info.coordinate = frame_spec_info.coords.coordinate;
		info.time = frame_spec_info.frame_time;
		info.path[0] = (char)0;
		info.line[0] = (char)0;
		strncpy_s(reinterpret_cast<char*>(&info.path), MAX_NAME_LENGTH_CB, frame_spec_info.coords.path.c_str(), _TRUNCATE);
		strncpy_s(reinterpret_cast<char*>(&info.line), MAX_NAME_LENGTH_CB, frame_spec_info.coords.line.c_str(), _TRUNCATE);

		out.write(reinterpret_cast<const char*>(&info), sizeof(FrameCoordPresentation));
		return out;
	}

	irb_frame_spec_info::irb_frame_spec_info(const irb_frame_helper::IRBFrame & frame) :
		coords(frame.coords), 
		frame_time(frame.header.presentation.imgTime)
	{
	}



}