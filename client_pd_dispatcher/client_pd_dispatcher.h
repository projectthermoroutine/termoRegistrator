#pragma once

#include <memory>
#include "position_detector_packets_manager.h"
#include <common\thread_exception.h>
namespace position_detector
{
	typedef struct _connection_address
	{
		std::string ip;
		std::string i_ip;
		unsigned short port;
	}connection_address;

	using pd_proxy_errors_callback_func_t = std::function<void(const std::string&)>;
	class client_pd_dispatcher final
	{
	public:
		client_pd_dispatcher(const packets_manager_ptr_t & packets_manager, pd_proxy_errors_callback_func_t pd_proxy_errors_callback);

		~client_pd_dispatcher();
		client_pd_dispatcher(const client_pd_dispatcher &) = delete;
		client_pd_dispatcher & operator = (const client_pd_dispatcher &) = delete;

		void run_processing_loop(const connection_address& pd_address, const connection_address& pd_events_address, const thread_exception_handler_ptr& exc_queue);
		void stop_processing_loop();

	private:
		struct Impl;
		std::unique_ptr<Impl> _p_impl;

	};

} // namespace position_detector
