#pragma once

#include <Windows.h>

#include <algorithm>
#include <memory>
#include <cstdint>
#include <vector>

namespace position_detector
{
	struct client_settings
	{
		unsigned int share_memory_size;
		std::string share_memory_name;
		std::string read_event_name;
		uint32_t id;
	} ;

	using proxy_server_pd_events_callback_func_t = std::function<void(const std::string&)>;

	class server_proxy_pd_connector final
	{

	public:
		server_proxy_pd_connector(proxy_server_pd_events_callback_func_t dispatch_error_func,
			proxy_server_pd_events_callback_func_t connection_error_func,
			proxy_server_pd_events_callback_func_t runtime_error_func);
		~server_proxy_pd_connector();
		server_proxy_pd_connector(const server_proxy_pd_connector &) = delete;
		server_proxy_pd_connector & operator = (const server_proxy_pd_connector &) = delete;

		bool setConfig(const std::vector<std::string>& config) const;
		std::vector<client_settings> getConfig() const;
	private:
		void * _p_impl;
	};


} // namespace position_detector