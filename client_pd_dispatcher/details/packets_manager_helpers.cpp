#include "packets_manager_helpers.h"

namespace position_detector
{
	using namespace events;
	using namespace synchronization;

	namespace packets_manager_helpers
	{

		path_info_ptr_t retrieve_path_info(const StartCommandEvent_packet & event)
		{
			path_info_ptr_t path_info_ = std::make_shared<path_info>();

			path_info_->path = event.track_settings.user_start_item.way_direction_item.direction_code;
			path_info_->line = event.track_settings.user_start_item.railway_item.code;
			path_info_->path_name = event.track_settings.user_start_item.way_direction_item.name;
			return path_info_;
		}

		path_info_ptr_t retrieve_path_info(const PassportChangedEvent_packet & event)
		{
			path_info_ptr_t path_info_ = std::make_shared<path_info>();

			path_info_->path = event.change_passport_point_direction.start_item.way_direction_item.direction_code;
			path_info_->line = event.change_passport_point_direction.start_item.railway_item.code;
			path_info_->path_name = event.change_passport_point_direction.start_item.way_direction_item.name;
			return path_info_;
		}




	}
}