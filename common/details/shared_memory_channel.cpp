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

#include "shared_memory_channel.h"

//#define SPINLOCK
//#define ACQUIRE_RELEASE

namespace channels
{
	using namespace std::string_view_literals;

	namespace
	{
		constexpr std::uint8_t default_blocks_count_for_mode(shared_memory_channel::mode_t mode)
		{
			switch (mode)
			{
			case shared_memory_channel::mode_t::cycle_buffer:   return 20;
			case shared_memory_channel::mode_t::always_newest:  return 2;
			}
			__assume(false);
		}


		inline constexpr std::wstring_view to_wstring_view(shared_memory_channel::mode_t mode)
		{
			switch (mode)
			{
			case shared_memory_channel::mode_t::cycle_buffer:   return L"cycle buffer"sv;
			case shared_memory_channel::mode_t::always_newest:  return L"always newest data"sv;
			default:											return L"<unknown>"sv;
			}
			__assume(false);
		}
		inline std::wstring to_wstring(shared_memory_channel::mode_t mode)
		{
			return std::wstring(to_wstring_view(mode));
		}


		static const long block_free = 0;
		static const long block_has_data = 1;
		static const long block_busy = 2;

		const unsigned int min_shared_memory_size_b = 16 + sizeof(shared_memory_channel::header_t) + sizeof(shared_memory_channel::block_header_t);

	}

	shared_memory_channel::shared_memory_channel(uint32_t id, const std::wstring& name, unsigned int size, mode_t mode)
		: _id(id)
		, _header(nullptr)
		, _blocks_begin(nullptr)
		, _blocks_end(nullptr)
		, _block_size(static_cast<std::size_t>(size) + sizeof(block_header_t))
		, _block_data_size(size)
		, _blocks_count(default_blocks_count_for_mode(mode))
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
			LOG_TRACE() << "Creating shared memory. [Name: " << name << ", size: " << _shared_memory_size << "]";
			h_shared_memory = handle_holder{ ::CreateFileMappingW(INVALID_HANDLE_VALUE, const_cast<LPSECURITY_ATTRIBUTES>(&sec_attr), PAGE_READWRITE, 0, _shared_memory_size, name.c_str()) };

			if (!h_shared_memory)
			{
				LOG_AND_THROW(win32::exception::by_last_error("CreateFileMappingW", name)) << L"Failed to create shared memory object with name: " << name;
			}
		};

		_shared_memory_size = static_cast<decltype(_shared_memory_size)>(sizeof(header_t) + _block_size * _blocks_count);
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

			_header = reinterpret_cast<header_t*>(_shared_buffer);
			_blocks_begin = reinterpret_cast<block_header_t*>(_header + 1);
			_blocks_end = reinterpret_cast<block_header_t*>(reinterpret_cast<char*>(_blocks_begin) + _block_size * _blocks_count);

			ZeroMemory(_shared_buffer, _shared_memory_size);
			*const_cast<mode_t*>(&_header->mode) = mode;
			*const_cast<std::uint8_t*>(&_header->count) = _blocks_count;
			*const_cast<std::uint32_t*>(&_header->block_size) = static_cast<std::uint32_t>(_block_size);

			LOG_TRACE() << "Shared memory created and mapped successfully.";

			LOG_TRACE() << L"Shared memory info [mode: "sv << to_wstring_view(_header->mode) << L"][count: "sv << _blocks_count
				<< L", block size: "sv << _block_size
				<< L", block data size: "sv << _block_data_size << L']';

		}

	}

	shared_memory_channel::shared_memory_channel(const std::vector<std::string>& settings)
		: _id(0)
		, _shared_buffer(nullptr)
		, _header(nullptr)
		, _blocks_begin(nullptr)
		, _blocks_end(nullptr)
		, _block_size(0)
		, _block_data_size(0)
		, _blocks_count(0)
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

		_shared_memory_name = string_utils::convert_utf8_to_wchar(shared_memory_name);
		_read_event_name = string_utils::convert_utf8_to_wchar(read_event_name);

		_shared_memory_size = std::stoul(shared_memory_size_str);

		if (_shared_memory_size < min_shared_memory_size_b)
			throw std::invalid_argument("The passed argument shared memory size can't be less 16");

		{
			LOG_TRACE() << L"Opening read event with name: "sv << _read_event_name;
			handle_holder read_event;
			try
			{
				read_event = sync_helpers::open_event_for_sync(_read_event_name);
			}
			catch (const std::runtime_error&)
			{
				LOG_AND_THROW(win32::exception::by_last_error("OpenEventW", _read_event_name)) << L"Could not open the event with name: "sv << _read_event_name;
			}

			LOG_TRACE() << L"Opening shared memory with name: "sv << _shared_memory_name;
			handle_holder h_shared_memory(::OpenFileMappingW(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, _shared_memory_name.c_str()));
			if (!h_shared_memory) {
				LOG_AND_THROW(win32::exception::by_last_error("OpenFileMappingW", _shared_memory_name)) << L"Could not open the shared memory with name: "sv << _shared_memory_name;
			}

			LOG_TRACE() << "Maping shared memory for size: " << _shared_memory_size;

			_shared_buffer = ::MapViewOfFile(h_shared_memory.get(), FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, _shared_memory_size);
			if (_shared_buffer == nullptr) {
				LOG_AND_THROW(win32::exception::by_last_error("MapViewOfFile", _shared_memory_name)) << L"Could not map the shared memory with name: "sv << _shared_memory_name;
			}

			_h_shared_memory.swap(h_shared_memory);
			_read_event.swap(read_event);

			LOG_TRACE() << L"The shared memory have opened and mapped successfully."sv;


			_header = reinterpret_cast<header_t*>(_shared_buffer);
			*const_cast<std::uint8_t*>(&_blocks_count) = _header->count;
			*const_cast<std::size_t*>(&_block_size) = static_cast<std::size_t>(_header->block_size);
			*const_cast<std::size_t*>(&_block_data_size) = _block_size - sizeof(block_header_t);

			LOG_TRACE() << L"Shared memory info [mode: "sv << to_wstring_view(_header->mode) << L"][count: "sv << _blocks_count
				<< L", block size: "sv << _block_size
				<< L", block data size: "sv << _block_data_size << L']';

			_blocks_begin = reinterpret_cast<block_header_t*>(_header + 1);
			_blocks_end = reinterpret_cast<block_header_t*>(reinterpret_cast<char*>(_blocks_begin) + _block_size * _blocks_count);

		}

	}

	shared_memory_channel::~shared_memory_channel()
	{
		if (_shared_buffer != nullptr)
			UnmapViewOfFile(_shared_buffer);
	}

	int shared_memory_channel::get_message(void * const buffer, const uint32_t buffer_size, const HANDLE stop_event)
	{
		//			LOG_STACK();

		HANDLE events[2] = { _read_event.get(), stop_event };

		std::uint8_t last_readed_index = _header->mode == mode_t::always_newest ? std::uint8_t{} : _header->last_readed;
		block_header_t * actual_block = nullptr;

#ifdef SPINLOCK
		static const std::uint16_t spinlock_max{ 10000 };
		std::uint16_t spinlock_counter{ 0 };
#endif
		for (;;)
		{
			//				LOG_DEBUG() << L"last_readed_index: " << last_readed_index;

			for (auto index = last_readed_index, blocks_counter = std::uint8_t(0); blocks_counter < _blocks_count; ++blocks_counter)
			{
				block_header_t * curr_block = reinterpret_cast<block_header_t*>(reinterpret_cast<char*>(_blocks_begin) + _block_size * index);
#ifdef ACQUIRE_RELEASE
				long curr_block_state = InterlockedCompareExchangeAcquire(&curr_block->state, block_busy, block_has_data);
#else
				long curr_block_state = InterlockedCompareExchange(&curr_block->state, block_busy, block_has_data);
#endif
				if (curr_block_state == block_has_data)
				{
					if (index != 0)
						LOG_DEBUG() << L"Found block with data. Index: "sv << index;

#ifdef SPINLOCK
					//if(spinlock_counter > 0)
					//	LOG_DEBUG() << L"Found block with data. Index: " << index << L", spinlock counter: " << spinlock_counter;
#endif

					last_readed_index = index;
					actual_block = curr_block;

					if (_header->mode == mode_t::cycle_buffer) 
					{
						std::uint8_t index_tmp = index == _blocks_count - 1 ? 0 : index + 1;
						curr_block = reinterpret_cast<block_header_t*>(reinterpret_cast<char*>(_blocks_begin) + _block_size * index_tmp);
						if (curr_block->state != block_free)
							last_readed_index = index_tmp;
					}

					//						LOG_DEBUG() << L"last_readed_index: " << last_readed_index;

					break;
				}

				++index;

				if (index == _blocks_count)
					index = 0;

			}//for (auto index = last_readed_index; index < _blocks_count;)

			if (actual_block)
				break;

#ifdef SPINLOCK
			if (++spinlock_counter < spinlock_max)
				continue;

			spinlock_counter = 0;
#endif

			LOG_DEBUG() << L"Wait for a data"sv;

			if (sync_helpers::wait_any_noexcept(events, 2, INFINITE) == 1)
				return 0;
		}//for(;;)

		const auto count_data = std::min(static_cast<decltype(buffer_size)>(_block_data_size), buffer_size);
		std::memcpy(buffer, (actual_block + 1), count_data);

		_header->last_readed = last_readed_index;

#ifdef ACQUIRE_RELEASE
		InterlockedAndRelease(&actual_block->state, block_free);
#else
		InterlockedAnd(&actual_block->state, block_free);
#endif

		//LOG_DEBUG() << "Got message from shared memory with name " << _shared_memory_name << ", size: " << count_data;

		return count_data;

	}


#define RESET_ALL_HAS_DATA_BLOCKS() \
	for (auto index = 0; index < _blocks_count; ++index)\
	{\
		block_header_t * curr_block = reinterpret_cast<block_header_t*>(reinterpret_cast<char*>(_blocks_begin) + _block_size * index);\
		InterlockedCompareExchange(&curr_block->state, block_free, block_has_data);\
	}

	void shared_memory_channel::send_data(const BYTE * data, unsigned int data_size)
	{
		//			LOG_STACK();

		block_header_t * actual_block = nullptr;
		std::uint8_t count_not_readed{ 0 };
		bool force = false;

		for (;;)
		{
			for (auto index = 0; index < _blocks_count;)
			{
				block_header_t * curr_block = reinterpret_cast<block_header_t*>(reinterpret_cast<char*>(_blocks_begin) + _block_size * index);
#ifdef ACQUIRE_RELEASE
				long curr_block_state = InterlockedCompareExchangeAcquire(&curr_block->state, block_busy, block_free);
#else
				long curr_block_state = InterlockedCompareExchange(&curr_block->state, block_busy, block_free);
#endif

				if (curr_block_state == block_free)
				{
					//						LOG_DEBUG() << L"Found free block. Index: " << index;
					actual_block = curr_block;
					break;
				}

				if (curr_block_state == block_has_data)
				{
					if (_header->mode == mode_t::always_newest || (_header->mode == mode_t::cycle_buffer && force))
					{
						LOG_DEBUG() << L"Take force block. Index: "sv << index;

#ifdef ACQUIRE_RELEASE
						InterlockedCompareExchangeAcquire(&curr_block->state, block_busy, block_has_data);
#else
						if (InterlockedCompareExchange(&curr_block->state, block_busy, block_has_data) == block_has_data 
							|| InterlockedCompareExchange(&curr_block->state, block_busy, block_free) == block_free)
#endif
						{
							actual_block = curr_block;
							break;
						}
						else 
						{
							index = 0;
							continue;
						}
					}

					++count_not_readed;
				}

				++index;

				if (index == _blocks_count && (_header->mode == mode_t::always_newest ||
					(_header->mode == mode_t::cycle_buffer && count_not_readed == _blocks_count))
					)
				{
					force = true;
					index = 0;
					count_not_readed = 0;
				}
			}//for (auto index = 0; index < _blocks_count;)

			if (actual_block)
				break;

			LOG_DEBUG() << L"Not found free block. Count not readed blocks: "sv << count_not_readed;

			return;
		}//for (;;)

		if (_header->mode == mode_t::always_newest)
			RESET_ALL_HAS_DATA_BLOCKS();

		void* const block_data = (actual_block + 1);

		ZeroMemory(block_data, _block_data_size);
		auto count_data = std::min(static_cast<decltype(data_size)>(_block_data_size), data_size);
		std::memcpy(block_data, data, count_data);

#ifdef ACQUIRE_RELEASE
		InterlockedCompareExchangeRelease(&actual_block->state, block_has_data, block_busy);
#else
		InterlockedCompareExchange(&actual_block->state, block_has_data, block_busy);
#endif
		sync_helpers::set_event(_read_event);

		//LOG_DEBUG() << "send message to shared memory with name " << _shared_memory_name << ", size: " << count_data;

	}

}//namespace channels