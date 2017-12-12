#pragma once

#include <Windows.h>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <exception>
#include <memory>
#include <vector>
#include <map>
#include <common\handle_holder.h>
#include <common\sync_helpers.h>
#include <loglib\log.h>

#include "client_context.h"
#include "position_detector_packets_manager.h"

#include <position_detector_dispatcher\pd_settings.h>
#include <position_detector_dispatcher\udp_synchro_packets_dispatcher.h>
#include <position_detector_common\position_detector_dispatcher_common.h>

namespace position_detector
{
	class proxy_server_pd_exception : public std::runtime_error
	{
	public:
		proxy_server_pd_exception(HRESULT error_code, const std::string & message) :
			std::runtime_error(message), _error_code(error_code)
		{
				std::ostringstream ss;
				ss << message << " Error: " << std::hex << std::showbase << error_code;
				_message = ss.str();
			}
		const char * what() const override
		{
			return _message.c_str();
		}

		HRESULT get_error_code() const
		{
			return _error_code;
		}

	private:
		std::string _message;
		HRESULT _error_code;
	};

	//class udp_proxy_pd_dispatcher;
	//typedef std::shared_ptr<udp_proxy_pd_dispatcher> pd_dispatcher_ptr_t;

	class proxy_server_pd final
	{
	public:
		enum class state {
			TurnOff,
			TurnOn
		};
	public:

		proxy_server_pd(position_detector_dispatcher::active_state_callback_func_t active_state_callback_func);
		~proxy_server_pd();

		void add_client(const client_context_ptr_t& client_context, packet_type packet_type = packet_type::synchronization_packet);
		bool remove_client(uint32_t id, packet_type packet_type = packet_type::synchronization_packet);

		client_context_ptr_t create_client_context(uint32_t id, packet_type packet_type = packet_type::synchronization_packet);

		void start(const settings::settings_t& pd_settings, const thread_exception_handler_ptr& exc_queue);
		void stop();

		inline state State() const { return _state; }

	private:

		proxy::packets_manager _packets_manager;
		udp_proxy_pd_dispatcher _pd_dispatcher;

		state _state;
	};

}