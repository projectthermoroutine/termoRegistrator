#include "packets_manager_helpers.h"

namespace position_detector
{
	using namespace events;
	using namespace synchronization;

	namespace packets_manager_helpers
	{

		path_info_ptr_t retrieve_path_info(const position_item_t & position_item)
		{
			path_info_ptr_t path_info_ = std::make_shared<path_info>();

			path_info_->line = position_item.direction_item.code;
			path_info_->railway = position_item.railway_item.code;
			path_info_->path = position_item.way_direction_item.name;
			path_info_->path_name = position_item.way_direction_item.naturalId;
			if (position_item.way_direction_item.kind != L"Main"){
				path_info_->path_type = PATH_TYPE::Other;
				if (position_item.way_direction_item.kind != L"Station")
					path_info_->path_type = PATH_TYPE::Station;
			}

			return path_info_;
		}

		path_info_ptr_t retrieve_path_info(const StartCommandEvent_packet & event)
		{
			return retrieve_path_info(event.track_settings.user_start_item);
		}

		path_info_ptr_t retrieve_path_info(const PassportChangedEvent_packet & event)
		{
			return retrieve_path_info(event.change_passport_point_direction.start_item);
		}




	}
}