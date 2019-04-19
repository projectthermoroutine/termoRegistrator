#include <common\handle_holder.h>
#include <common\sync_helpers.h>
#include <loglib\log.h>
#include <common\xor_shift.h>

#include "client_context.h"

//#include "details\shared_memory_channel.h"
#include <common\shared_memory_channel.h>

#include <memory>
namespace position_detector
{

#define SYNC_PACKET_SIZE 16
#define EVENT_PACKET_SIZE 4096 - 16*sizeof(long)
	shared_memory_channel * create_shared_memory_channel(uint32_t id,packet_type packet_type = packet_type::synchronization_packet)
	{
		LOG_STACK();
		std::wstring name_suffix = L"_sync_sh_m_n";
		unsigned int memory_size = SYNC_PACKET_SIZE;
		if (packet_type == packet_type::event_packet)
		{
			memory_size = EVENT_PACKET_SIZE;
			name_suffix = L"_events_sh_m_n";
		}
		std::wstring shared_memory_name;
		sync_helpers::create_random_name(shared_memory_name, true);
		shared_memory_name += name_suffix;

		shared_memory_channel *p_channel = new shared_memory_channel(id, shared_memory_name, memory_size);
		
		return p_channel;
	}

	client_context::client_context(uint32_t id, packet_type packet_type)
	{
		LOG_STACK();
		_channel = create_shared_memory_channel(id,packet_type);
	}

	client_context::~client_context()
	{
		LOG_STACK();
		delete _channel;
	}

	std::wstring client_context::get_shared_memory_name() const
	{
		return _channel->shared_memory_name();
	}
	unsigned int client_context::get_shared_memory_size() const
	{
		return _channel->shared_memory_size();
	}
	std::wstring client_context::get_event_name() const
	{
		return _channel->event_name();
	}

	void client_context::send_data(const BYTE * data, unsigned int data_size)
	{
		_channel->send_data(data, data_size);
	}

	bool client_context::operator==(const client_context& other) const
	{
		return _channel->event_name() == other._channel->event_name() &&
			_channel->shared_memory_name() == other._channel->shared_memory_name();
	}
	uint32_t client_context::ID() const
	{
		return _channel->ID();
	}

} // namespace position_detector