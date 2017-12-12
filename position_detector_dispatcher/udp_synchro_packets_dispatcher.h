#pragma once

#include <functional>
#include <memory>
#include <position_detector_common\position_detector_dispatcher_common.h>
#include <position_detector_common\position_detector_packet.h>
#include "position_detector_packets_manager.h"
#include "position_detector_dispatcher.h"

namespace position_detector
{
	class udp_proxy_pd_dispatcher final
	{
	public:
		udp_proxy_pd_dispatcher(proxy::packets_manager & packets_manager, position_detector_dispatcher::active_state_callback_func_t active_state_callback_func)
		{
			process_packets_factory factory;
			factory.create_process_synchro_packet_func = create_process_packet_func_t([&]()->message_processing_func_t
																							{
																								return std::bind(&proxy::packets_manager::dispatch_sync_packet, &packets_manager, std::placeholders::_1, std::placeholders::_2); 
																							}
			);
			factory.create_process_event_packet_func = create_process_packet_func_t([&]()->message_processing_func_t
																							{
																								return std::bind(&proxy::packets_manager::dispatch_event_packet, &packets_manager, std::placeholders::_1, std::placeholders::_2);
																							}
			);
			packets_dispatcher = new position_synchronizer_dispatcher(factory, active_state_callback_func);
		}

		~udp_proxy_pd_dispatcher()
		{
			delete packets_dispatcher;
		}
		udp_proxy_pd_dispatcher(const udp_proxy_pd_dispatcher &) = delete;
		udp_proxy_pd_dispatcher & operator = (const udp_proxy_pd_dispatcher &) = delete;


		inline void run_processing_loop(position_detector_dispatcher::settings_func_t settings_func, const thread_exception_handler_ptr& exc_queue) { packets_dispatcher->run_processing_loop(settings_func, exc_queue); }
		inline void stop_processing_loop() { packets_dispatcher->stop_processing_loop(); }

	private:

		position_synchronizer_dispatcher *packets_dispatcher;
		scoped_WSA _WSA;


	};

} // namespace position_detector
