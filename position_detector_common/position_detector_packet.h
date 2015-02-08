#pragma once

#include <Windows.h>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <exception>
#include <loglib\log.h>
#include <memory>


namespace position_detector
{
	class deserialization_error : public std::runtime_error
	{
	public:
		deserialization_error(const char* error) : runtime_error(error) {}
	};


	template<typename TMessage>
	TMessage parce_packet_from_message(const BYTE * message, unsigned int message_size);

	namespace synchronization
	{
		const unsigned int min_synchro_paket_size = 16;
		struct synchro_packet_t;
		using sync_packet_ptr_t = std::shared_ptr<synchro_packet_t>;

		typedef unsigned long long timestamp_t;

	}//namespace synchronization
	namespace events
	{
		class event_packet;
		using event_packet_ptr_t = std::shared_ptr<event_packet>;

	}//namespace events

}//namespace position_detector

