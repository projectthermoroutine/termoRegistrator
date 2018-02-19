#pragma once

#include <Windows.h>

#include <algorithm>
#include <memory>
#include <array>
#include <cstdint>

#include <common\handle_holder.h>
#include <common\sync_helpers.h>
#include <loglib\log.h>

#include <position_detector_common\position_detector_packet.h>

namespace position_detector
{
	template<typename TMessage>
	using dispatch_packet_func_t = std::function<void(const TMessage &)>;

	class packets_stream
	{
	public:
		dispatch_packet_func_t<events::event_packet_ptr_t> dispatch_event_packet_func;
		dispatch_packet_func_t<synchronization::sync_packet_ptr_t> dispatch_synchro_packet_func;

		void dispatch_synchro_packet(const BYTE * message, unsigned int message_size)
		{
			if (message == nullptr)
				throw std::invalid_argument("The passed argument message can't be equal to nullptr");

			synchronization::sync_packet_ptr_t packet;
			try{

				packet =
					parce_packet_from_message<synchronization::sync_packet_ptr_t>(
					message, message_size);

				dispatch_synchro_packet_func(packet);

			}
			catch (const deserialization_error & exc)
			{
				LOG_WARN() << exc.what();
				return;
			}
		}
		void dispatch_event_packet(const BYTE * message, unsigned int message_size)
		{
			if (message == nullptr)
				throw std::invalid_argument("The passed argument message can't be equal to nullptr");

			events::event_packet_ptr_t packet;
			try{

				packet =
					parce_packet_from_message<events::event_packet_ptr_t>(
					message, message_size);

				dispatch_event_packet_func(packet);

			}
			catch (const deserialization_error & exc)
			{
				LOG_WARN() << exc.what();
				return;
			}
		}
	};
}