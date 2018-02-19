#pragma once

#include <string>
#include <cstdint>

//#include <position_detector_dispatcher\proxy_server_pd.h>

namespace position_detector
{
	struct connection_address
	{
		std::string ip;
		std::string i_ip;
		unsigned short port;
	};

	namespace settings
	{
		struct settings_t
		{
			connection_address pd_address;
			connection_address pd_events_address;

			std::uint8_t counter_size;

			explicit operator bool() const { return counter_size > 0; }
			bool operator==(const settings_t& other) const 
			{ 
				return counter_size == other.counter_size &&
					pd_address.ip == other.pd_address.ip &&
					pd_address.i_ip == other.pd_address.i_ip &&
					pd_address.port == other.pd_address.port &&
					pd_events_address.ip == other.pd_events_address.ip &&
					pd_events_address.i_ip == other.pd_events_address.i_ip &&
					pd_events_address.port == other.pd_events_address.port;
			}
		};

		settings_t read_settings(const std::wstring& file_path);
		void write_settings(const std::wstring& file_path, const settings_t& settings);

	}
}//namespace position_detector

