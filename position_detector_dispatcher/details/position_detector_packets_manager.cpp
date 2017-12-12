#include <Windows.h>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <exception>
#include <loglib\log.h>
#include <memory>
#include <vector>
#include <map>
#include <mutex>

#include "position_detector_packets_manager.h"
#include <position_detector_common\position_detector_packet.h>
#include <position_detector_common\details\position_detector_packet_details.h>


namespace position_detector
{
	using namespace events;
	using namespace synchronization;

	static const std::uint8_t default_counter_size = 5;
	namespace proxy
	{

		void send_data_to_clients(const clients_context_list_t& clients, const BYTE * data, unsigned int data_size)
		{
			for (const auto& client : clients)
				client->send_data(data, data_size);
		}

		packets_manager::packets_manager() :
			_client_packets_manager(default_counter_size)
			, _pd_state(pd_state::none_active)
		{
			LOG_STACK();

			_client_packets_manager.set_track_state_change_notify([this](bool active)
																		{
																			this->_pd_state = active ? packets_manager::pd_state::active : packets_manager::pd_state::none_active;
																		}
			);

			_packets_stream.dispatch_event_packet_func = std::bind(&position_detector::packets_manager::add_packet<event_packet_ptr_t>, &_client_packets_manager, std::placeholders::_1);
			_packets_stream.dispatch_synchro_packet_func = std::bind(&position_detector::packets_manager::add_packet<sync_packet_ptr_t>, &_client_packets_manager, std::placeholders::_1);

		}

		void packets_manager::set_counter_size(std::uint8_t counter_size)
		{
			_client_packets_manager.set_counter_size(counter_size);
		}

		void packets_manager::dispatch_sync_packet(const BYTE * data, unsigned int data_size)
		{
			_packets_stream.dispatch_synchro_packet(data, data_size);
			send_to_clients_sync_packet(data, data_size);
		}
		void packets_manager::dispatch_event_packet(const BYTE * data, unsigned int data_size)
		{
			{
				std::lock_guard<std::mutex> guard(_pd_state_mtx);
				_packets_stream.dispatch_event_packet(data, data_size);
			}

			send_to_clients_event_packet(data, data_size);
		}

		void packets_manager::send_to_clients_sync_packet(const BYTE * data, unsigned int data_size)
		{
			LOG_STACK();
			std::lock_guard<std::mutex> guard(_clients_synchro_packets_mtx);
			send_data_to_clients(_clients_sync_packets, data, data_size);
		}
		void packets_manager::send_to_clients_event_packet(const BYTE * data, unsigned int data_size)
		{
			LOG_STACK();
			std::lock_guard<std::mutex> guard(_clients_event_packets_mtx);
			send_data_to_clients(_clients_event_packets, data, data_size);
		}

		void notify_client_pd_state_active(const client_context_ptr_t& client_context, const position_detector::packets_manager& packets_manager)
		{
			const auto events = packets_manager.get_actual_pd_event_packets();
			for (const auto& event : events)
				client_context->send_data(event->event_raw_data.data(), (unsigned int)event->event_raw_data.size());
		}

		void packets_manager::add_client(const client_context_ptr_t& client_context, packet_type packet_type)
		{
			LOG_STACK();
			if (packet_type == packet_type::synchronization_packet){
				std::lock_guard<std::mutex> guard(_clients_synchro_packets_mtx);
				_clients_sync_packets.emplace_back(client_context);
			}

			if (packet_type == packet_type::event_packet)
			{
				std::lock_guard<std::mutex> guard(_pd_state_mtx);

				if (_pd_state == pd_state::active)
				{
					notify_client_pd_state_active(client_context, _client_packets_manager);
				}

				std::lock_guard<std::mutex> clients_guard(_clients_event_packets_mtx);
				_clients_event_packets.emplace_back(client_context);

			}
		}

		bool remove_client_impl(std::mutex& mtx, clients_context_list_t& container, uint32_t id)
		{
			LOG_STACK();

			std::lock_guard<std::mutex> guard(mtx);

			const auto prev_size = container.size();
			container.erase(std::remove_if(container.begin(), container.end(), [id](const client_context_ptr_t& context){return context->ID() == id; }),
				container.end());
			return prev_size != container.size();

		}

		bool packets_manager::remove_client(uint32_t id, packet_type packet_type)
		{
			LOG_STACK();

			switch (packet_type)
			{
			case packet_type::synchronization_packet:
				return remove_client_impl(_clients_synchro_packets_mtx, _clients_sync_packets, id);
			case packet_type::event_packet:
				return remove_client_impl(_clients_event_packets_mtx, _clients_event_packets, id);
			};

			return false;
		}
	}//namespace proxy
}//namespace position_detector
