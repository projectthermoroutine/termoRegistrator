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

#include "details\shared_memory_channel.h"

#include "position_detector_packets_manager.h"


namespace position_detector
{
	void send_data_to_clients(const clients_context_list_t& clients, const BYTE * data, unsigned int data_size)
	{
		std::for_each(clients.begin(), clients.end(), [data, data_size](const client_context_ptr_t context){context->send_data(data, data_size); });
	}

	packets_manager::packets_manager() : _is_clients_sync_packets_busy(0)
	{
	}

	void packets_manager::send_to_clients_sync_packet(const BYTE * data, unsigned int data_size)
	{
		auto prev_value = _InterlockedCompareExchange8(&_is_clients_sync_packets_busy, 1, 0);
		if (prev_value != 0)
		{
			_clients_synchro_packets_mtx.lock();
			while (_InterlockedCompareExchange8(&_is_clients_sync_packets_busy, 1, 0) != 0);
		}
		send_data_to_clients(_clients_sync_packets,data,data_size);

		_InterlockedAnd8(&_is_clients_sync_packets_busy, 0);
		if (prev_value != 0)
		{
			_clients_synchro_packets_mtx.unlock();
		}


	}
	void packets_manager::send_to_clients_event_packet(const BYTE * data, unsigned int data_size)
	{
		std::lock_guard<std::mutex> guard(_clients_event_packets_mtx);
		send_data_to_clients(_clients_event_packets, data, data_size);
	}

	void packets_manager::add_client(const client_context_ptr_t& client_context, packet_type packet_type)
	{
		if (packet_type == packet_type::synchronization_packet){
			std::lock_guard<std::mutex> guard(_clients_synchro_packets_mtx);
			_clients_sync_packets.emplace_back(client_context);
		}

		if (packet_type == packet_type::event_packet){
			std::lock_guard<std::mutex> guard(_clients_event_packets_mtx);
			_clients_event_packets.emplace_back(client_context);
		}
	}
	
	bool packets_manager::remove_client(uint32_t id, packet_type packet_type)
	{
		bool res = false;
		if (packet_type == packet_type::synchronization_packet){
			auto prev_value = _InterlockedCompareExchange8(&_is_clients_sync_packets_busy, 1, 0);
			if (prev_value != 0)
			{
				_clients_synchro_packets_mtx.lock();
				while (_InterlockedCompareExchange8(&_is_clients_sync_packets_busy, 1, 0) != 0);
			}
			auto context = _clients_sync_packets.begin();
			for (; context != _clients_sync_packets.end(); context++)
			{
				if (context->get()->ID() == id)
					break;
			}
			if (context != _clients_sync_packets.end()){
				_clients_sync_packets.erase(context);
				res = true;
			}

			_InterlockedAnd8(&_is_clients_sync_packets_busy, 0);
			if (prev_value != 0)
			{
				_clients_synchro_packets_mtx.unlock();
			}
		}

		if (packet_type == packet_type::event_packet){
			std::lock_guard<std::mutex> guard(_clients_event_packets_mtx);
			auto context = _clients_event_packets.begin();
			for (; context != _clients_event_packets.end(); context++)
			{
				if (context->get()->ID() == id)
					break;
			}
			if (context != _clients_event_packets.end()){
				_clients_event_packets.erase(context);
				res = true;
			}
		}

		return res;
	}

}//namespace position_detector
