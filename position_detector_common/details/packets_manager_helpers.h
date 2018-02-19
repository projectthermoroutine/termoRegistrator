#pragma once

#include <position_detector_common\position_detector_packet.h>
#include <position_detector_common\details\position_detector_packet_details.h>
#include <position_detector_common\position_detector_packets_manager.h>

namespace position_detector
{
	using namespace events;
	using namespace synchronization;

	namespace packets_manager_helpers
	{

		path_info_ptr_t retrieve_path_info(const StartCommandEvent_packet & event);
		path_info_ptr_t retrieve_path_info(const PassportChangedEvent_packet & event);

	}
}