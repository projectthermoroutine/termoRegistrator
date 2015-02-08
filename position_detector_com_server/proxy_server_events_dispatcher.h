#pragma once

#include <Windows.h>
#include <memory>
#include <vector>
#include <map>
#include <mutex>
#include <position_detector_dispatcher\details\shared_memory_channel.h>
#include "..\midl\PD_COMServer_i.h"

namespace proxy_server_pd_ns
{
	class events_manager;
	typedef std::unique_ptr<events_manager> events_manager_ptr_t;
	typedef std::shared_ptr<position_detector::shared_memory_channel> events_stream_ptr_t;
	typedef std::vector<events_stream_ptr_t> events_streams_list_t;

	typedef errorSource event_type;

	class events_manager
	{
	public:

		void send_error(errorSource err_src, const std::string & err);
		void send_event(event_type ev_type, const std::string & event);
		void add_client(const events_stream_ptr_t& events_stream);
		void remove_client(uint32_t id);

	private:

		events_streams_list_t _streams;
		std::mutex _streams_mtx;
	};

}//namespace position_detector

