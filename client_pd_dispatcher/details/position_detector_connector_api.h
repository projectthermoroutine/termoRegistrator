#pragma once

#include <Windows.h>

#include <algorithm>
#include <memory>
#include <array>
#include <cstdint>

#include <common\handle_holder.h>
#include <common\sync_helpers.h>
#include <loglib\log.h>

#include <position_detector_common\position_detector_connector.h>

namespace position_detector
{
	namespace details
	{
#define SLEEPINTERVAL 1000
		class shared_memory_connector_api : public connector_api
		{
			const unsigned int min_shared_memory_size_b = 16;
		public:
			shared_memory_connector_api(const std::vector<std::string>& settings) :
				_shared_buffer(nullptr)
			{
				LOG_STACK();

				if (settings.size() < 3)
					throw std::invalid_argument("Invalid size argument were passed.");

				std::string shared_memory_name = settings[0];
				std::string shared_memory_size_str = settings[1];
				std::string read_event_name = settings[2];


				if (shared_memory_name.empty())
					throw std::invalid_argument("The passed argument shared memory name can't be empty");
				if (shared_memory_size_str.empty())
					throw std::invalid_argument("The passed argument shared memory size can't be empty");
				if (read_event_name.empty())
					throw std::invalid_argument("The passed argument read event name can't be empty");


				_shared_memory_size = std::stoul(shared_memory_size_str);

				if (_shared_memory_size < min_shared_memory_size_b)
					throw std::invalid_argument("The passed argument shared memory size can't be less 16");

				{
					//LOG_TRACE() << "Opening read event";
					handle_holder read_event = sync_helpers::open_event_for_sync(read_event_name.c_str());

					//LOG_TRACE() << "Opening shared memory";
					handle_holder h_shared_memory(::OpenFileMappingA(FILE_MAP_READ, FALSE, shared_memory_name.c_str()));
					if (!h_shared_memory) {

						const auto result = ::GetLastError();
						LOG_DEBUG() << "Could not open Shared memory. Error: " << std::hex << std::showbase << result;
						throw position_detector_connector_exception(result, "Could not open Shared memory");
					}

					//LOG_TRACE() << "Maping shared memory";

					_shared_buffer = ::MapViewOfFile(h_shared_memory.get(), FILE_MAP_READ, 0, 0, _shared_memory_size);
					if (_shared_buffer == nullptr) {

						const auto result = ::GetLastError();
						LOG_DEBUG() << "Could not map shared memory. Error: " << std::hex << std::showbase << result;
						throw position_detector_connector_exception(result, "Could not map shared memory.");
					}

					_h_shared_memory.swap(h_shared_memory);
					_read_event.swap(read_event);

					//LOG_TRACE() << "Shared memory opened and maped successfully.";
				}

			}

			void close() {}

			~shared_memory_connector_api()
			{
				if (_shared_buffer != nullptr)
					UnmapViewOfFile(_shared_buffer);
			}

			int get_message(get_message_struct * const buffer, const packet_size_t buffer_size)
			{
				LOG_STACK();

				auto result = sync_helpers::wait(_read_event, SLEEPINTERVAL);// wait_message(1);
				if (result){
					auto count_data = std::min(_shared_memory_size, buffer_size);
					std::memcpy(buffer, _shared_buffer, count_data);
					return count_data;
				}

				return 0;
			}

			shared_memory_connector_api(const shared_memory_connector_api &) = delete;
			shared_memory_connector_api & operator = (const shared_memory_connector_api &) = delete;

		private:
			void * _shared_buffer;
			handle_holder _h_shared_memory;
			handle_holder _read_event;
			unsigned int _shared_memory_size;

		};

	} // namespace details
} // namespace position_detector