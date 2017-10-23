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

	client_context_exception::client_context_exception(HRESULT error_code, const std::string & message) :
		std::runtime_error(message), _error_code(error_code)
	{
			std::ostringstream ss;
			ss << message << " Error: " << std::hex << std::showbase << error_code;
			_message = ss.str();
		}
	const char * client_context_exception::what() const
	{
		return _message.c_str();
	}

	HRESULT client_context_exception::get_error_code() const
	{
		return _error_code;
	}

#define SYNC_PACKET_SIZE 16
#define EVENT_PACKET_SIZE 4096 - 4*sizeof(long)
	shared_memory_channel * create_shared_memory_channel(uint32_t id,packet_type packet_type = packet_type::synchronization_packet)
	{
		LOG_STACK();
		unsigned int memory_size = SYNC_PACKET_SIZE;
		if (packet_type == packet_type::event_packet){
			memory_size = EVENT_PACKET_SIZE;
		}
		std::wstring shared_memory_name;
		sync_helpers::create_random_name(shared_memory_name,false);

		shared_memory_channel *p_channel = nullptr;
		try{
			p_channel = new shared_memory_channel(id,shared_memory_name, memory_size);
		}
		catch (const shared_memory_channel_exception& exc)
		{
			throw client_context_exception(exc.get_error_code(),exc.what());
		}
		
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
		LOG_STACK();
		return _channel->shared_memory_name();
	}
	unsigned int client_context::get_shared_memory_size() const
	{
		LOG_STACK();
		return _channel->shared_memory_size();
	}
	std::wstring client_context::get_event_name() const
	{
		LOG_STACK();
		return _channel->event_name();
	}

	void client_context::send_data(const BYTE * data, unsigned int data_size)
	{
		LOG_STACK();
		_channel->send_data(data, data_size);
	}

	bool client_context::operator==(const client_context& other) const
	{
		LOG_STACK();
		return _channel->event_name() == other._channel->event_name() &&
			_channel->shared_memory_name() == other._channel->shared_memory_name();
	}
	uint32_t client_context::ID() const
	{
		LOG_STACK();
		return _channel->ID();
	}

} // namespace position_detector