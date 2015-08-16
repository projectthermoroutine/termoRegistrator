#pragma once

#include <Windows.h>

#include <algorithm>
#include <memory>
#include <array>
#include <cstdint>

#include <common\handle_holder.h>
#include <common\sync_helpers.h>
#include <loglib\log.h>
#include <common\shared_memory_channel.h>

#include <position_detector_common\position_detector_connector.h>

namespace position_detector
{
	namespace details
	{
		using namespace channels;
#define SLEEPINTERVAL 1000
		class shared_memory_connector_api :public  shared_memory_channel,public connector_api
		{
		public:
			shared_memory_connector_api(const std::vector<std::string>& settings)try:
				shared_memory_channel(settings)
			{
				LOG_STACK();
			}
			catch (const shared_memory_channel_exception& exc)
			{
				throw position_detector_connector_exception(exc.get_error_code(), exc.what());
			}

			void close() {}

			virtual int connector_api::get_message(void * const buffer, const uint32_t buffer_size, const HANDLE stop_event)
			{
				return shared_memory_channel::get_message(buffer,buffer_size,stop_event);
			}
			shared_memory_connector_api(const shared_memory_connector_api &) = delete;
			shared_memory_connector_api & operator = (const shared_memory_connector_api &) = delete;

		};

	} // namespace details
} // namespace position_detector