#include "position_detector_dispatcher.h"
#include "position_detector_connector_api.h"
#include <position_detector_common\position_detector_connector.h>
#include <memory>
namespace position_detector
{

	connector_ptr_t create_connector(const std::vector<std::string>& settings)
	{
		return std::make_shared<position_detector_connector>(std::make_shared<details::shared_memory_connector_api>(settings));
	}
	position_synchronizer_dispatcher::position_synchronizer_dispatcher(process_packets_factory process_packets_factory, position_detector_dispatcher::active_state_callback_func_t active_state_callback_func) :
		position_detector_dispatcher(process_packets_factory, create_connector_func_t(create_connector), active_state_callback_func)
	{
	}

} // namespace position_detector