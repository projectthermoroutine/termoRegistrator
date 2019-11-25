#pragma once

#include <memory>
#include <vector>
#include <common\handle_holder.h>

namespace channels
{
	using namespace std::literals;

	struct perfomance_tag_t
	{
		explicit perfomance_tag_t() = default;
	};

	inline constexpr perfomance_tag_t perfomance_tag{};

	class shared_memory_channel
	{
	public:
		enum class mode_t : std::uint8_t { cycle_buffer = 0, always_newest = 1 };
	
	public:
#pragma pack(push,8)
		struct header_t
		{
			const mode_t		mode;
			const std::uint8_t  count;
			const std::uint32_t block_size;

			std::uint32_t ticks;
			std::uint8_t last_readed;
		};

		struct block_header_t
		{
			volatile long state;
		};
#pragma pack(pop)

	public:
		shared_memory_channel(uint32_t id, const std::wstring& name, unsigned int size)
			: shared_memory_channel(id, name, size, mode_t::cycle_buffer)
		{}

		shared_memory_channel(uint32_t id, const std::wstring& name, unsigned int size, perfomance_tag_t)
			:shared_memory_channel(id, name, size, mode_t::always_newest)
		{}

		shared_memory_channel(uint32_t id, const std::wstring& name, unsigned int size, mode_t mode);
		shared_memory_channel(const std::vector<std::string>& settings);

		~shared_memory_channel();

		shared_memory_channel(const shared_memory_channel &) = delete;
		shared_memory_channel & operator = (const shared_memory_channel &) = delete;


		int get_message(void * const buffer, const uint32_t buffer_size, const HANDLE stop_event);
		void send_data(const BYTE * data, unsigned int data_size);

		inline std::wstring event_name() const	{ return _read_event_name;	}
		inline std::wstring shared_memory_name() const	{ return _shared_memory_name; }
		inline unsigned int shared_memory_size() const	{ return _shared_memory_size; }
		inline uint32_t ID() const { return _id; }

	private:
		uint32_t _id;
		void * _shared_buffer;
		handle_holder _h_shared_memory;
		handle_holder _read_event;
		unsigned int _shared_memory_size;
		std::wstring _read_event_name;
		std::wstring _shared_memory_name;


		const std::size_t _block_size;
		const std::size_t _block_data_size;
		header_t * _header;
		block_header_t * _blocks_begin;
		block_header_t * _blocks_end;
		const std::uint8_t _blocks_count;
	};

}//namespace channels

