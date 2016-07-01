#include <Windows.h>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <exception>
#include <loglib\log.h>
#include <memory>
#include <vector>
#include <map>

#include <position_detector_common\position_detector_packet.h>
#include "client_pd_dispatcher.h"

#include <position_detector_common\position_detector_dispatcher_common.h>
#include "position_detector_packets_manager.h"
#include "position_detector_dispatcher.h"
#include "details\packet_stream.h"
#include "server_proxy_pd_connector.h"


namespace position_detector
{
	using namespace events;
	using namespace synchronization;



	struct client_pd_dispatcher::Impl
	{
		std::unique_ptr<position_synchronizer_dispatcher> packets_dispatcher;
		packets_stream packets_ostream;
		std::unique_ptr<server_proxy_pd_connector> server_connector;
		pd_proxy_errors_callback_func_t pd_proxy_errors_callback;
		bool _is_active;

		void notify_dispatch_error(const std::string& msg)
		{
			pd_proxy_errors_callback(msg);
		}
	};


	void _active_state_callback_func(bool)
	{}

	client_pd_dispatcher::client_pd_dispatcher(const packets_manager_ptr_t &packets_manager, pd_proxy_errors_callback_func_t pd_proxy_errors_callback)
	{
		decltype(_p_impl) impl = std::make_unique<client_pd_dispatcher::Impl>();
		impl->_is_active = false;
		impl->pd_proxy_errors_callback = pd_proxy_errors_callback;
		try{
			impl->server_connector = std::make_unique<server_proxy_pd_connector>
				(std::bind(&client_pd_dispatcher::Impl::notify_dispatch_error, impl.get(), std::placeholders::_1),
				std::bind(&client_pd_dispatcher::Impl::notify_dispatch_error, impl.get(), std::placeholders::_1),
				std::bind(&client_pd_dispatcher::Impl::notify_dispatch_error, impl.get(), std::placeholders::_1)
				);
		}
		catch (const position_detector::server_proxy_pd_connector_exception& exc)
		{
			LOG_ERROR() << exc.what();
			throw;
		}

		impl->packets_ostream.dispatch_event_packet_func = std::bind(&packets_manager::add_packet<event_packet_ptr_t>, packets_manager.get(), std::placeholders::_1);
		impl->packets_ostream.dispatch_synchro_packet_func = std::bind(&packets_manager::add_packet<sync_packet_ptr_t>, packets_manager.get(), std::placeholders::_1);

		process_packets_factory factory;
		factory.create_process_synchro_packet_func = create_process_packet_func_t([&impl]()->message_processing_func_t{return std::bind(&packets_stream::dispatch_synchro_packet, &impl->packets_ostream, std::placeholders::_1, std::placeholders::_2); });
		factory.create_process_event_packet_func = create_process_packet_func_t([&impl]()->message_processing_func_t{return std::bind(&packets_stream::dispatch_event_packet, &impl->packets_ostream, std::placeholders::_1, std::placeholders::_2); });
		impl->packets_dispatcher = std::make_unique<position_synchronizer_dispatcher>(factory, _active_state_callback_func);
		_p_impl.swap(impl);
	}

	client_pd_dispatcher::~client_pd_dispatcher()
	{
		if (_p_impl->_is_active)
			_p_impl->packets_dispatcher->stop_processing_loop();
	}

	void client_pd_dispatcher::run_processing_loop(const connection_address& pd_address, const connection_address& pd_events_address,const exception_queue_ptr_t& exc_queue)
	{

		std::vector<std::string> server_settings{ "pd_ip", pd_address.ip, "pd_i_ip", pd_address.i_ip, "pd_port", std::to_string(pd_address.port),
												"pd_events_ip", pd_events_address.ip, "pd_i_events_ip", pd_events_address.i_ip, "pd_events_port", std::to_string(pd_events_address.port)
												};

		_p_impl->server_connector->setConfig(server_settings);

		auto client_settings = _p_impl->server_connector->getConfig();

		auto settings_func = [client_settings](const std::string &key)->std::vector<std::string>
		{
			std::vector<std::string> result;
			if (key == "Syncronizer device")
			{
				result.push_back(client_settings[0].share_memory_name);
				result.push_back(std::to_string(client_settings[0].share_memory_size));
				result.push_back(client_settings[0].read_event_name);
				result.push_back(std::to_string(client_settings[0].id));
			}
			if (key == "Syncronizer events device")
			{
				result.push_back(client_settings[1].share_memory_name);
				result.push_back(std::to_string(client_settings[1].share_memory_size));
				result.push_back(client_settings[1].read_event_name);
				result.push_back(std::to_string(client_settings[1].id));
			}

			return result;
		};

		_p_impl->_is_active = true;
		_p_impl->packets_dispatcher->run_processing_loop(settings_func, exc_queue);
	}
	void client_pd_dispatcher::stop_processing_loop() 
	{ 
		if (_p_impl->_is_active){
			_p_impl->_is_active = false;
			_p_impl->packets_dispatcher->stop_processing_loop();
		}
	}

}//namespace position_detector
