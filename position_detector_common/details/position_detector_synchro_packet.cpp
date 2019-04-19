#include "position_detector_packet.h"
#include "position_detector_packet_details.h"

#include <common\sync_helpers.h>
#include <common\handle_holder.h>
#include <common\on_exit.h>
#include <loglib\log.h>
#include <Windows.h>

#include <vector>
#include <map>

namespace position_detector
{
	using namespace std::literals;

	using namespace synchronization;
	template<>
	sync_packet_ptr_t parce_packet_from_message(const BYTE * message, unsigned int message_size)
	{
		if (message == nullptr)
			throw std::invalid_argument("The passed argument message can't be equal to nullptr");
		if (message_size < FIELD_OFFSET(synchro_packet_t, direction))
			throw std::invalid_argument("Invalid message size were passed");

		const synchro_packet_t *packet = reinterpret_cast<const synchro_packet_t*>(message);

		sync_packet_ptr_t sync_packet(new synchro_packet_t(*packet));
		sync_packet->retrieve_direction();

		LOG_TRACE() << L"Counter: "sv << packet->counter;

		return sync_packet;
	}

	
}// namespace position_detector