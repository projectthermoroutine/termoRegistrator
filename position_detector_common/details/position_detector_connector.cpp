#include <common\handle_holder.h>
#include <loglib\log.h>
#include <common\sync_helpers.h>
#include <atomic>

#include "position_detector_connector.h"
#include <sstream>

namespace position_detector
{

position_detector_connector_exception::position_detector_connector_exception(HRESULT error_code, const std::string & message) :
	std::runtime_error(message), _error_code(error_code)
{
	std::ostringstream ss;
	ss << message << " Error: " << std::hex << std::showbase << error_code;
	_message = ss.str();
}

const char * position_detector_connector_exception::what() const
{
	return _message.c_str();
}

HRESULT position_detector_connector_exception::get_error_code() const
{
	return _error_code;
}

position_detector_connector::position_detector_connector(connector_api_ptr_t api) :
_api(api),
_buffer(std::make_unique<get_message_struct>()),
_close_requested(false)
{
	LOG_STACK();
}

position_detector_connector::~position_detector_connector()
{
	LOG_STACK();
	close();
}

void  position_detector_connector::process_incoming_message(
	const stop_requested_func_t& stop_requested,
	const HANDLE stop_event,
	const message_processing_func_t& message_process_func)
{

	LOG_TRACE() << "Getting message.";
	const packet_size_t packet_size = get_message(_buffer.get(), sizeof(get_message_struct), stop_requested, stop_event);
	if (!packet_size)
	{
		return;
	}

	LOG_TRACE() << "Packet size: " << packet_size;

	message_process_func(_buffer->data, packet_size);
}


void position_detector_connector::process_incoming_message(
	const HANDLE stop_event,
	const message_processing_func_t& message_process_func
)
{
	LOG_STACK();

	if (stop_event == 0)
		throw std::invalid_argument("The passed argument stop_event can't be equal to 0");
	if (stop_event == INVALID_HANDLE_VALUE)
		throw std::invalid_argument("The passed argument stop_event can't be equal to INVALID_HANDLE_VALUE");

	LOG_TRACE() << "Getting message.";
	const packet_size_t packet_size = get_message(_buffer.get(), sizeof(get_message_struct), stop_event);
	if (!packet_size)
	{
		return;
	}

	LOG_TRACE() << "Packet size: " << packet_size;

	message_process_func(_buffer->data, packet_size);


}

unsigned int position_detector_connector::get_message(
	void * const buffer,
	const uint32_t buffer_size,
	const stop_requested_func_t& stop_requested,
	const HANDLE stop_event
	)
{
	LOG_STACK();

	SecureZeroMemory(buffer, buffer_size);
	{
		for (;;)
		{
			if (stop_requested() || _close_requested)
			{
				LOG_TRACE() << "Stop was requested.";
				return 0;
			}

			auto result = _api->get_message(buffer, buffer_size, stop_event);
			if (result > 0){
				return result;
			}
		}
	}
	return 0;
}


unsigned int position_detector_connector::get_message(
	void * const buffer,
	const uint32_t buffer_size,
	const HANDLE stop_event)
{
	LOG_STACK();

	SecureZeroMemory(buffer, buffer_size);
	{
		for (;;)
		{
			if (sync_helpers::is_event_set(stop_event) || _close_requested)
			{
				LOG_TRACE() << "Stop was requested.";
				return 0;
			}

			auto result = _api->get_message(buffer, buffer_size, stop_event);
			if (result > 0){
				return result;
			}
		}
	}
	return 0;
}

void position_detector_connector::close()
{
	_close_requested = true;
	_api->close();
}

} // namespace position_detector


