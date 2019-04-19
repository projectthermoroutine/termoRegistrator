#include <Windows.h>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <exception>
#include <loglib\log.h>
#include <memory>
#include <vector>
#include <map>
#include <mutex>


#include "proxy_server_events_dispatcher.h"


namespace proxy_server_pd_ns
{
	void send_data_to_clients(const events_streams_list_t& clients, const BYTE * data, unsigned int data_size)
	{
		LOG_STACK();
		std::for_each(clients.begin(), clients.end(), [data, data_size](const events_stream_ptr_t& stream){stream->send_data(data, data_size); });
	}

	void events_manager::send_event(event_type ev_type, const std::string & event)
	{
		LOG_STACK();
		send_error(ev_type, event);
	}
	void events_manager::send_error(errorSource err_src, const std::string & err)
	{
		LOG_STACK();
		unsigned int buffer_size = 0;
		{
			std::lock_guard<std::mutex> guard(_streams_mtx);
			if (_streams.empty())
				return;
			buffer_size = _streams[0]->shared_memory_size();
		}

		std::unique_ptr<BYTE[]> buffer(new BYTE[buffer_size]);
		std::fill(buffer.get(), buffer.get() + buffer_size, (BYTE)0);

		errorInfo * p_error_info = reinterpret_cast<errorInfo *>(buffer.get());
		p_error_info->err_source = err_src;
		auto data_size = std::min(err.size(), buffer_size - sizeof(p_error_info->err_source) - 1);
		std::memcpy(&p_error_info->err_str, err.c_str(), data_size);
		data_size += sizeof(p_error_info->err_source);

		std::lock_guard<std::mutex> guard(_streams_mtx);
		send_data_to_clients(_streams, buffer.get(), (unsigned int)data_size);
	}

	void events_manager::add_client(const events_stream_ptr_t& events_stream)
	{
		std::lock_guard<std::mutex> guard(_streams_mtx);
		_streams.emplace_back(events_stream);
	}
	void events_manager::remove_client(uint32_t id)
	{
		std::lock_guard<std::mutex> guard(_streams_mtx);
		for (auto context = _streams.begin(); context != _streams.end(); ++context)
		{
			if (context->get()->ID() == id){
				_streams.erase(context);
				break;
			}
		}
	}

	
}//namespace position_detector
