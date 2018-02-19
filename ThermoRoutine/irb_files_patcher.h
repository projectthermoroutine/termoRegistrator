#pragma once
#include <Windows.h>
#include <vector>
#include <memory>
#include "irb_frame_delegates.h"
#include <position_detector_common\position_detector_packets_manager.h>

namespace irb_files_patcher
{
	class irb_files_patcher final
	{
	public:
		irb_files_patcher();
		~irb_files_patcher();

	public:

		void new_irb_file(const std::wstring & filename);
		void irb_file_changed(irb_frame_delegates::irb_frames_infos_t&& info, const std::wstring & filename);

		void recalc_coordinate(position_detector::counter32_t, position_detector::counter32_t, position_detector::coordinate_calculator_ptr_t);


	private:
		struct Impl;
		std::unique_ptr<Impl> _p_impl;

	};

}