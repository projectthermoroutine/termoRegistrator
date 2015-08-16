#include "irb_frames_cache.h"
#include "irb_grab_frames_dispatcher.h"
#include <client_pd_dispatcher\position_detector_packets_manager.h>

#include "defines.h"

bool
retrieve_frame_position(
const irb_grab_frames_dispatcher::irb_frame_shared_ptr_t& frame,
const position_detector::packets_manager_ptr_t& _coordinates_manager
)
{
	position_detector::track_point_info _point_info;
#ifdef TIMESTAMP_SYNCH_PACKET_ON
	time_t frame_time = (time_t)frame->get_frame_time_in_msec();
	auto res = _coordinates_manager->get_point_info_by_time(frame_time, point_info);
#else
	auto res = _coordinates_manager->get_last_point_info(_point_info);
#endif
	if (res)
	{
		frame->coords.coordinate = _point_info.coordinate;
		frame->coords.path = _point_info._path_info->path;
		frame->coords.line = _point_info._path_info->line;
	}
	return res;
}


namespace irb_frames_cache
{
}