#pragma once

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
#include <common\handle_holder.h>
#include <common\sync_helpers.h>
#include <loglib\log.h>

namespace position_detector
{
	class shared_memory_channel_exception : public std::runtime_error
	{
	public:
		shared_memory_channel_exception(HRESULT error_code, const std::string & message) :
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

	using namespace events;
	using namespace synchronization;

	class shared_memory_channel
	{
	public:

		shared_memory_channel(uint32_t id, const std::wstring& name, unsigned int size) :_id(id)
		{
			LOG_STACK();

			if (name.empty())
				throw std::invalid_argument("The passed argument shared memory name can't be empty");

			if (size == 0)
				throw std::invalid_argument("The passed argument shared memory size can't be zero");

			_shared_memory_size = size;
			{
				LOG_TRACE() << "Creating read event object";
				std::wstring read_event_name;
				handle_holder read_event = sync_helpers::create_random_name_event(read_event_name, false,false, true);

				LOG_TRACE() << "Creating shared memory";
				handle_holder h_shared_memory(::CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, name.c_str()));
				if (!h_shared_memory) {

					const auto result = ::GetLastError();
					LOG_DEBUG() << "Could not create Shared memory. Error: " << std::hex << std::showbase << result;
					throw shared_memory_channel_exception(result, "Could not create Shared memory");
				}

				LOG_TRACE() << "Mapping shared memory";

				_shared_buffer = ::MapViewOfFile(h_shared_memory.get(), FILE_MAP_WRITE, 0, 0, _shared_memory_size);
				if (_shared_buffer == nullptr) {

					const auto result = ::GetLastError();
					LOG_DEBUG() << "Could not map shared memory. Error: " << std::hex << std::showbase << result;
					throw shared_memory_channel_exception(result, "Could not map shared memory.");
				}

				_h_shared_memory.swap(h_shared_memory);
				_read_event.swap(read_event);
				_read_event_name = read_event_name;
				_shared_memory_name = name;
				LOG_TRACE() << "Shared memory created and mapped successfully.";
			}

		}
		~shared_memory_channel()
		{
			if (_shared_buffer != nullptr)
				UnmapViewOfFile(_shared_buffer);
		}

		void send_data(const BYTE * data,unsigned int data_size)
		{
			std::memcpy(_shared_buffer, data, data_size);
			sync_helpers::set_event(_read_event);
		}

		inline std::wstring event_name() const
		{
			return _read_event_name;
		}

		inline std::wstring shared_memory_name() const
		{
			return _shared_memory_name;
		}
		inline unsigned int shared_memory_size() const
		{
			return _shared_memory_size;
		}

		inline uint32_t ID() const { return _id; }
	private:
		uint32_t _id;
		void * _shared_buffer;
		handle_holder _h_shared_memory;
		handle_holder _read_event;
		unsigned int _shared_memory_size;
		std::wstring _read_event_name;
		std::wstring _shared_memory_name;
	};

//	using packets_manager_ptr_t = std::shared_ptr<packets_manager>;


}//namespace position_detector

