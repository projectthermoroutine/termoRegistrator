#pragma once

#include <common\sync_helpers.h>

#include <chrono>
#include <thread>

#include <algorithm>
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <utility>
#include <cstdlib>
#include <functional>

#include <position_detector_common\position_detector_packet.h>
#include <position_detector_common\details\position_detector_packet_details.h>

namespace client_pd_dispatcher_test_project
{
	using namespace position_detector;
	typedef ULONG32 counter_t;
	typedef ULONG64 timestamp_t;
	typedef ULONG32 speed_t;

#pragma pack (push)
#pragma pack (1)

#define DIRECTION_MASK 0x80000000UL
	struct test_synchro_packet_t
	{
		counter_t counter;
		timestamp_t timestamp;
		speed_t speed;
	};

#pragma pack (pop)

	using namespace position_detector::events;
	using namespace position_detector::synchronization;

	bool compare_synchro_packets(const test_synchro_packet_t &p1, const synchro_packet_t &p2);

	using check_event_packet_func_t = std::function<bool(const position_detector::events::event_packet_ptr_t &)>;

	using compare_event_packet_func_t = bool(*)(const event_packet *, const event_packet *);

	bool compare_data_time_item(const datetime & dt1, const datetime & dt2);
	bool compare_event_packets(const event_packet *p1, const event_packet *p2);
}