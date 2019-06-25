#pragma once
#include <Windows.h>
#include <vector>
#include <memory>
#include "irb_frame_helper.h"

namespace irb_frame_spec_info
{

	struct irb_frame_position_info
	{
		int32_t picket;
		int32_t offset;
	};


	class irb_frame_spec_info
	{
	public:
		irb_frame_spec_info(const irb_frame_helper::IRBFrame & frame, 
							const irb_frame_position_info & frame_position_info, 
							const std::wstring & device_name
							);
	private:
		irb_frame_helper::FrameCoord coords;
		double frame_time;
		irb_frame_position_info _frame_position_info;
		std::wstring _device_name;


		friend std::ostream & operator<<(std::ostream & out, const irb_frame_spec_info &frame_spec_info);
	};

	std::ostream & operator<<(std::ostream & out, const irb_frame_spec_info &frame_spec_info);

}