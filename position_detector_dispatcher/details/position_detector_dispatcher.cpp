#include "position_detector_dispatcher.h"
#include "position_detector_connector_api.h"
#include <position_detector_common\position_detector_connector.h>
#include <memory>
namespace position_detector
{

	connector_ptr_t create_connector(const std::vector<std::string>& settings)
	{
		return std::make_shared<position_detector_connector>(std::make_shared<details::device_connector_api>(settings));
	}
	position_synchronizer_dispatcher::position_synchronizer_dispatcher(process_packets_factory process_packets_factory, position_detector_dispatcher::active_state_callback_func_t active_state_callback_func) :
		position_detector_dispatcher(process_packets_factory, create_connector_func_t(create_connector), active_state_callback_func)
	{
		
	}

	scoped_WSA::scoped_WSA()
	{
		LOG_STACK();
		LOG_TRACE() << "Starting Windows Sockets ";
		WSADATA wsaData;
		auto result = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (result != NO_ERROR) {
			LOG_DEBUG() << "Could not start up Windows Sockets: error: " << std::hex << std::showbase << result;
			std::ostringstream ss;
			ss << "Could not start up Windows Sockets: error: " << std::hex << std::showbase << result;
			throw std::runtime_error(ss.str());
		}
	}

	scoped_WSA::~scoped_WSA()
	{
		LOG_STACK();
		WSACleanup();
	}


} // namespace position_detector