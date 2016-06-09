#pragma once
#include <vector>
#include <memory>
#include <tuple>
#include "irb_frame_helper.h"

namespace pixels_mask_helper
{

	using bad_pixels_mask_ptr = std::unique_ptr<irb_frame_helper::bad_pixels_mask>;
	using camera_settings_t = std::string;
	std::tuple<camera_settings_t, std::vector<bad_pixels_mask_ptr> > create_bad_pixels_mask(const std::wstring& camera_bad_pixels_settings);
}