#pragma once
#include <Windows.h>
#include <vector>
#include <memory>
#include "irb_frame_helper.h"


namespace irb_frame_spec_info
{

	class irb_frame_spec_info
	{
	public:
		irb_frame_spec_info(const irb_frame_helper::IRBFrame & frame);
	private:
		irb_frame_helper::FrameCoord coords;
		double frame_time;


		friend std::ostream & operator<<(std::ostream & out, const irb_frame_spec_info &frame_spec_info);
	};

	std::ostream & operator<<(std::ostream & out, const irb_frame_spec_info &frame_spec_info);

}