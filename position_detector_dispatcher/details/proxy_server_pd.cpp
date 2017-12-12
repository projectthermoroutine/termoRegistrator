#include <common\handle_holder.h>
#include <common\sync_helpers.h>
#include <loglib\log.h>
#include <common\xor_shift.h>

#include "client_context.h"
#include "proxy_server_pd.h"

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
		, _packets_manager()
		, _pd_dispatcher(_packets_manager, active_state_callback_func)
	{
		LOG_STACK();
	}

	proxy_server_pd::~proxy_server_pd()
	{
		LOG_STACK();
		_state = state::TurnOff;
		_pd_dispatcher.stop_processing_loop();
	}

	void proxy_server_pd::start(const settings::settings_t& pd_settings, const thread_exception_handler_ptr& exc_queue)
	{
		LOG_STACK();
		if (_state == state::TurnOn)
			return;

		_packets_manager.set_counter_size(pd_settings.counter_size);

		_pd_dispatcher.run_processing_loop([&](const std::string &key)->std::vector<std::string>
		{
			std::vector<std::string> result;
			if (key == "Syncronizer device")
			{
				result.push_back(pd_settings.pd_address.ip);
				result.push_back(pd_settings.pd_address.i_ip);
				result.push_back(std::to_string(pd_settings.pd_address.port));
			}
			if (key == "Syncronizer events device")
			{
				result.push_back(pd_settings.pd_events_address.ip);
				result.push_back(pd_settings.pd_events_address.i_ip);
				result.push_back(std::to_string(pd_settings.pd_events_address.port));
			}

			return result;
		}, exc_queue);

		_state = state::TurnOn;
	}
	void proxy_server_pd::stop()
	{
		LOG_STACK();
		_state = state::TurnOff;
		_pd_dispatcher.stop_processing_loop();
	}

	void proxy_server_pd::add_client(const client_context_ptr_t& client_context, packet_type packet_type)
	{
		_packets_manager.add_client(client_context, packet_type);
	}

	bool proxy_server_pd::remove_client(uint32_t id, packet_type packet_type)
	{
		return _packets_manager.remove_client(id, packet_type);
	}

	client_context_ptr_t proxy_server_pd::create_client_context(uint32_t id,packet_type packet_type)
	{
		return std::make_shared<client_context>(id,packet_type);
	}

} // namespace position_detector