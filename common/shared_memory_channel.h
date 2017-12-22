#pragma once

#ifdef max
#undef max
#endif //max

#ifdef min
#undef min
#endif //min

#ifndef NOMINMAX
#define NOMINMAX
#endif //NOMINMAX

#include <Windows.h>

#include <algorithm>
#include <stdexcept>
#include <exception>
#include <loglib\log.h>
#include <memory>
#include <vector>
#include <common\handle_holder.h>
#include <common\sync_helpers.h>
#include <common\log_and_throw.h>

#include <common/security.h>

#include <error_lib\application_exception.h>
#include <error_lib\error_codes.h>
#include <error_lib\win32_error.h>
#include <error_lib\win32_error_codes.h>

namespace channels
{
	class shared_memory_channel
	{
		class control_block final
		{
		public:
			control_block() :_busy(0), _has_data(0){}

			inline bool try_lock()
			{
				return InterlockedCompareExchange(&_busy, 1, 0) == 0 ? true : false;
			}
			inline void unlock()
			{
				InterlockedAnd(&_busy, 0);
			}
			inline bool has_data()
			{
				return InterlockedCompareExchange(&_has_data, 0, 1) == 1 ? true : false;
			}
			inline long set_data()		{
				InterlockedCompareExchange(&_has_data, 1, 0);
				return 1;
			}

		private:
			long _busy;
			long _has_data;

		};

		const unsigned int min_shared_memory_size_b = 16 + sizeof(control_block);

	public:

		shared_memory_channel(uint32_t id, const std::wstring& name, unsigned int size) 
			:_id(id), 
			ctrl_block(nullptr),
			_data_buffer(nullptr),
			_data_buffer_size(0)
		{
			LOG_STACK();

			if (name.empty())
				LOG_AND_THROW(::common::application_exception(::common::result_code::passed_argument_name_of_shared_mem_could_not_be_empty)) << L"The passed argument name could not be empty.";

			if (size == 0)
				LOG_AND_THROW(::common::application_exception(::common::result_code::passed_argument_size_of_shared_mem_could_not_be_zero)) << L"The passed argument size could not equal zero.";

			std::wstring read_event_name;
			handle_holder read_event;
			handle_holder h_shared_memory;

			const auto shared_memory_event_init_func = [&](const SECURITY_ATTRIBUTES &sec_attr)
			{
				LOG_TRACE() << "Creating read event object";
				read_event = sync_helpers::create_random_name_event(read_event_name, false, false, true, &sec_attr);
			};



			const auto shared_memory_init_func = [&](const SECURITY_ATTRIBUTES &sec_attr)
			{
				LOG_TRACE() << "Creating shared memory. [Name: " << name << ", size: " << size << "]";
				h_shared_memory = handle_holder{ ::CreateFileMappingW(INVALID_HANDLE_VALUE, const_cast<LPSECURITY_ATTRIBUTES>(&sec_attr), PAGE_READWRITE, 0, size, name.c_str()) };

				if (!h_shared_memory)
				{
					LOG_AND_THROW(win32::exception::by_last_error("CreateFileMappingW", name)) << L"Failed to create shared memory object with name: " << name;
				}
			};

			_shared_memory_size = size + sizeof(control_block);
			{

				security::process_sec_attributes_for_shared_memory_event(shared_memory_event_init_func, SYNCHRONIZE | EVENT_MODIFY_STATE);
				security::process_sec_attributes_for_shared_memory(shared_memory_init_func);

				LOG_TRACE() << "Mapping shared memory";

				_shared_buffer = ::MapViewOfFile(h_shared_memory.get(), FILE_MAP_WRITE, 0, 0, _shared_memory_size);
				if (_shared_buffer == nullptr) {
					LOG_AND_THROW(win32::exception::by_last_error("MapViewOfFile", name)) << L"Could not map shared memory with name: " << name;
				}

				_h_shared_memory.swap(h_shared_memory);
				_read_event.swap(read_event);
				_read_event_name = read_event_name;
				_shared_memory_name = name;
				ctrl_block = reinterpret_cast<decltype(ctrl_block)>(_shared_buffer);
				_data_buffer = reinterpret_cast<void *>(reinterpret_cast<char *>(_shared_buffer)+sizeof(control_block));
				_data_buffer_size = size;

				SecureZeroMemory(ctrl_block, sizeof(control_block));
				//ctrl_block->set_data();
				LOG_TRACE() << "Shared memory created and mapped successfully.";
			}

		}
		shared_memory_channel(const std::vector<std::string>& settings) :
			_id(0),
			ctrl_block(nullptr),
			_data_buffer(nullptr),
			_shared_buffer(nullptr),
			_data_buffer_size(0)
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
				LOG_TRACE() << "Opening read event with name: " << read_event_name;
				handle_holder read_event = sync_helpers::open_event_for_sync(read_event_name.c_str());

				LOG_TRACE() << "Opening shared memory with name: " << shared_memory_name;
				handle_holder h_shared_memory(::OpenFileMappingA(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, shared_memory_name.c_str()));
				if (!h_shared_memory) {
					LOG_AND_THROW(win32::exception::by_last_error("OpenFileMappingA", shared_memory_name)) << L"Could not open Shared memory with name: " << shared_memory_name.c_str();
				}

				LOG_TRACE() << "Maping shared memory for size: " << _shared_memory_size;

				_shared_buffer = ::MapViewOfFile(h_shared_memory.get(), FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, _shared_memory_size);
				if (_shared_buffer == nullptr) {
					LOG_AND_THROW(win32::exception::by_last_error("MapViewOfFile", shared_memory_name)) << L"Could not map shared memory with name: " << shared_memory_name.c_str();
				}

				_h_shared_memory.swap(h_shared_memory);
				_read_event.swap(read_event);

				LOG_TRACE() << "Shared memory opened and maped successfully.";

				ctrl_block = reinterpret_cast<decltype(ctrl_block)>(_shared_buffer);
				_data_buffer = reinterpret_cast<void *>(reinterpret_cast<char *>(_shared_buffer)+sizeof(control_block));
				_data_buffer_size = _shared_memory_size - sizeof(control_block);
				_shared_memory_name = string_utils::convert_utf8_to_wchar(shared_memory_name);
				_read_event_name = string_utils::convert_utf8_to_wchar(read_event_name);

			}

		}

		~shared_memory_channel()
		{
			if (_shared_buffer != nullptr)
				UnmapViewOfFile(_shared_buffer);
		}

		shared_memory_channel(const shared_memory_channel &) = delete;
		shared_memory_channel & operator = (const shared_memory_channel &) = delete;


		int get_message(void * const buffer, const uint32_t buffer_size, const HANDLE stop_event)
		{
			LOG_STACK();

			bool read_data = false;// ctrl_block->has_data();
			if (!read_data)
			{
				HANDLE events[2] = { _read_event.get(), stop_event };
				auto result = sync_helpers::wait_any(events, 2);
				if (result.event_raised && result.event_index == 0)
					read_data = true;

			}

			if (read_data && ctrl_block->try_lock())
			{
				auto count_data = std::min(_data_buffer_size, buffer_size);
				std::memcpy(buffer, _data_buffer, count_data);
				ctrl_block->unlock();

				//LOG_DEBUG() << "Got message from shared memory with name " << _shared_memory_name << ", size: " << count_data;

				return count_data;
			}

			return 0;
		}

		void send_data(const BYTE * data, unsigned int data_size)
		{
			if (ctrl_block->try_lock())
			{
				SecureZeroMemory(_data_buffer, _data_buffer_size);
				auto count_data = std::min(_data_buffer_size, data_size);
				std::memcpy(_data_buffer, data, count_data);
				bool set_event = false;
				if (ctrl_block->set_data() == 1)
					set_event = true;
				ctrl_block->unlock();
				if (set_event)
					sync_helpers::set_event(_read_event);

				//LOG_DEBUG() << "send message to shared memory with name " << _shared_memory_name << ", size: " << count_data;

			}
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
		void * _data_buffer;
		handle_holder _h_shared_memory;
		handle_holder _read_event;
		unsigned int _shared_memory_size;
		unsigned int _data_buffer_size;
		std::wstring _read_event_name;
		std::wstring _shared_memory_name;
		control_block * ctrl_block;

	};


}//namespace channels

