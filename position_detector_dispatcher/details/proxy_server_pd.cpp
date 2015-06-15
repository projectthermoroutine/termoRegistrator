#include <common\handle_holder.h>
#include <common\sync_helpers.h>
#include <loglib\log.h>
#include <common\xor_shift.h>

#include "client_context.h"
#include "proxy_server_pd.h"

#include "details\shared_memory_channel.h"

#include "position_detector_packets_manager.h"
#include "udp_synchro_packets_dispatcher.h"


#include "position_detector_dispatcher.h"
#include "position_detector_connector_api.h"
#include <position_detector_common\position_detector_connector.h>
#include <position_detector_common\position_detector_dispatcher_common.h>
#include <memory>
namespace position_detector
{
	proxy_server_pd::proxy_server_pd(position_detector_dispatcher::active_state_callback_func_t active_state_callback_func) : _state(state::TurnOff)
	{
		LOG_STACK()
		packets_manager_ptr_t packets_manager(std::make_shared<packets_manager>());
		pd_dispatcher_ptr_t pd_dispatcher(std::make_shared<udp_proxy_pd_dispatcher>(packets_manager, active_state_callback_func));

		_packets_manager.swap(packets_manager);
		_pd_dispatcher.swap(pd_dispatcher);
	}

	proxy_server_pd::~proxy_server_pd()
	{
		LOG_STACK()
		_pd_dispatcher->stop_processing_loop();
	}

	void proxy_server_pd::start(const connection_address& pd_address, const connection_address& pd_events_address, const exception_queue_ptr_t& exc_queue)
	{
		LOG_STACK()
		if (_state == state::TurnOn)
			return;

		_pd_dispatcher->run_processing_loop([&pd_address, &pd_events_address](const std::string &key)->std::vector<std::string>
		{
			std::vector<std::string> result;
			if (key == "Syncronizer device")
			{
				result.push_back(pd_address.ip);
				result.push_back(pd_address.i_ip);
				result.push_back(std::to_string(pd_address.port));
			}
			if (key == "Syncronizer events device")
			{
				result.push_back(pd_events_address.ip);
				result.push_back(pd_events_address.i_ip);
				result.push_back(std::to_string(pd_events_address.port));
			}

			return result;
		}, exc_queue);

		_state = state::TurnOn;
	}
	void proxy_server_pd::stop()
	{
		LOG_STACK()
		_state = state::TurnOff;
		_pd_dispatcher->stop_processing_loop();
	}

	void proxy_server_pd::add_client(const client_context_ptr_t& client_context, packet_type packet_type)
	{
		_packets_manager->add_client(client_context, packet_type);
	}

	bool proxy_server_pd::remove_client(uint32_t id, packet_type packet_type)
	{
		return _packets_manager->remove_client(id, packet_type);
	}

	client_context_ptr_t proxy_server_pd::create_client_context(uint32_t id,packet_type packet_type)
	{
		return std::make_shared<client_context>(id,packet_type);
	}

} // namespace position_detector