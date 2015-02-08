#include <algorithm>
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <utility>
#include <cstdlib>
#include <functional>

#include <position_detector_packet.h>
#include "position_detector_packet_details.h"
#include "packets_helpers.h"

namespace position_detector_test_project
{
	using namespace position_detector::events;
	using namespace position_detector::synchronization;

	bool compare_synchro_packets(const test_synchro_packet_t &p1, const synchro_packet_t &p2)
	{
		bool isValid = false;
		if (p2.counter == p1.counter &&
			p2.timestamp == p1.timestamp &&
			p2.speed == (p1.speed & (~DIRECTION_MASK)))
		{
			isValid = true;
		}
		return isValid;
	}

	bool compare_event_packets(const event_packet *p1, const event_packet *p2)
	{
		bool isValid = false;
		if (p1->counter == p2->counter &&
			p1->type == p2->type &&
			p1->dataTime == p2->dataTime &&
			p1->source == p2->source &&
			p1->guid == p2->guid &&
			p1->counter == p2->counter &&
			compare_data_time_item(p1->data_time, p2->data_time)
			)
		{
			isValid = true;
		}

		return isValid;

	}

	bool compare_data_time_item(const datetime & dt1, const datetime & dt2)
	{
		return (dt1.year == dt2.year &&
			dt1.month == dt2.month &&
			dt1.day == dt2.day &&
			dt1.hour == dt2.hour &&
			dt1.minute == dt2.minute &&
			dt1.second == dt2.second &&
			dt1.millisecond == dt2.millisecond &&
			dt1.tzd == dt2.tzd
			);
	}
}