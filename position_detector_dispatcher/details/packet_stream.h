#pragma once

#include <Windows.h>

#include <algorithm>
#include <memory>
#include <array>
#include <cstdint>

#include <common\handle_holder.h>
#include <common\sync_helpers.h>
#include <loglib\log.h>

namespace position_detector
{
	using dispatch_packet_func_t = std::function<void(const BYTE * , unsigned int )>;

	typedef dispatch_packet_func_t dispatch_event_packet_func_t, dispatch_synchro_packet_func_t;

	class packets_stream
	{
	public:
		dispatch_event_packet_func_t dispatch_event_packet_func;
		dispatch_synchro_packet_func_t dispatch_synchro_packet_func;

		void dispatch_synchro_packet(const BYTE * message, unsigned int message_size)
		{
			LOG_STACK();

			if (message == nullptr)
				throw std::invalid_argument("The passed argument message can't be equal to nullptr");

			try{

				dispatch_synchro_packet_func(message, message_size);

			}
			catch (const std::runtime_error & exc)
			{
				LOG_WARN() << exc.what();
				return;
			}
		}
		void dispatch_event_packet(const BYTE * message, unsigned int message_size)
		{
			LOG_STACK();

			if (message == nullptr)
				throw std::invalid_argument("The passed argument message can't be equal to nullptr");

			try{


				dispatch_event_packet_func(message, message_size);

			}
			catch (const std::runtime_error & exc)
			{
				LOG_WARN() << exc.what();
				return;
			}
		}
	};
}