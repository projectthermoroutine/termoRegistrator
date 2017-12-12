#pragma once

#include <Windows.h>
#include <memory>
#include <vector>
#include <map>
#include <mutex>
#include "client_context.h"

#include <position_detector_common\position_detector_packets_manager.h>
#include <position_detector_common\details\packet_stream.h>

namespace position_detector
{
	namespace proxy
	{
		class packets_manager;
		typedef std::shared_ptr<packets_manager> packets_manager_ptr_t;

		class packets_manager
		{
		public:
			enum pd_state { none_active, active, };

			packets_manager();

			void dispatch_sync_packet(const BYTE * data, unsigned int data_size);
			void dispatch_event_packet(const BYTE * data, unsigned int data_size);

			void add_client(const client_context_ptr_t& client_context, packet_type packet_type = packet_type::synchronization_packet);
			bool remove_client(uint32_t id, packet_type packet_type = packet_type::synchronization_packet);

			void set_counter_size(std::uint8_t counter_size);

		private:
			void send_to_clients_sync_packet(const BYTE * data, unsigned int data_size);
			void send_to_clients_event_packet(const BYTE * data, unsigned int data_size);

		private:
			clients_context_list_t _clients_sync_packets;
			clients_context_list_t _clients_event_packets;
			std::mutex _clients_event_packets_mtx;
			std::mutex _clients_synchro_packets_mtx;

		private:

			pd_state _pd_state;
			std::mutex _pd_state_mtx;

		private:

			position_detector::packets_manager _client_packets_manager;
			position_detector::packets_stream  _packets_stream;

		};
	}//namespace proxy
}//namespace position_detector

