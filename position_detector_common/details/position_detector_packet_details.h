#pragma once

#include <Windows.h>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <exception>
#include <loglib\log.h>
#include <memory>
#include <map>
#include <vector>


namespace position_detector
{

	namespace synchronization
	{
		typedef uint32_t counter_t;
		typedef uint32_t speed_t;
		typedef bool direction_t;

#pragma pack (push)
#pragma pack (1)

#define DIRECTION_MASK 0x80000000UL
		struct synchro_packet_t
		{
			counter_t counter;
			timestamp_t timestamp;
			speed_t speed;
			direction_t direction;

			synchro_packet_t(const synchro_packet_t&& packet) { std::memcpy(this, &packet, FIELD_OFFSET(synchro_packet_t, direction)); }
			void retrieve_direction(){ direction = speed & DIRECTION_MASK ? true : false; speed &= (~DIRECTION_MASK); }

		};

#pragma pack (pop)

	}//namespace synchronization
	namespace events
	{
		typedef uint64_t EVENT_ID;

		enum class event_type
		{
			EvUnknownEvent,
			EvStartCommandEvent,
			EvCoordinateCorrected,
			EvPassportChangedEvent,
			EvReverseEvent,
			EvStopCommandEvent

		};


		struct datetime
		{
			unsigned int year;
			unsigned int month;
			unsigned int day;
			unsigned int hour;
			unsigned int minute;
			unsigned int second;
			unsigned int millisecond;
			short tzd; //as minutes.
			unsigned long long timestamp;

			void clear()
			{
				year = 0;
				month = 0;
				day = 0;
				hour = 0;
				minute = 0;
				second = 0;
				millisecond = 0;
				tzd = 0;
				timestamp = 0;
			}
		};

		using event_raw_data_t = std::vector<std::uint8_t>;

		class event_info;
		class event_packet
		{
		public:
			event_packet() :id(0), type(event_type::EvUnknownEvent){}
			virtual ~event_packet() = default;

			virtual bool get_info(event_info * event_info) = 0;
		public:

			std::wstring event_name;
			event_type type;
			EVENT_ID id;
			unsigned int counter;
			std::string guid;
			std::wstring source;
			std::string dataTime;
			datetime data_time;

		public:
			event_raw_data_t event_raw_data;
		};


		typedef struct
		{
			std::wstring key;
			std::wstring code;
			std::wstring name;
		} event_item_t;

		typedef event_item_t railway_item_t;
		typedef event_item_t direction_item_t;

		struct coordinate_item_t
		{
			int32_t km;
			int32_t m;
			int32_t mm;
		};

		struct way_direction_item_t
		{
			std::wstring id;
			std::wstring naturalId;
			std::wstring name;
			std::wstring direction_name;
			std::wstring direction_code;
			std::wstring station_name;
			std::wstring station_code;
			std::wstring kind;
		};

		using km_t = int;
		using km_length_t = int;

		using nonstandard_kms_map_t = std::map < km_t, km_length_t >;
		using nonstandard_kms_t = nonstandard_kms_map_t;

		typedef struct
		{
			nonstandard_kms_t positive_kms;
			nonstandard_kms_t negative_kms;
		}nonstandard_kms_item_t;

		typedef struct
		{
			railway_item_t railway_item;
			direction_item_t direction_item;
			coordinate_item_t coordinate_item;
			way_direction_item_t way_direction_item;
		} position_item_t;

		typedef position_item_t user_start_item_t, user_end_item_t;

		struct track_settings_item_t
		{
			std::string orientation;
			std::string movement_direction;
			std::wstring name;
			user_start_item_t user_start_item;
			user_end_item_t user_end_item;
			nonstandard_kms_item_t kms;
		};

		class StartCommandEvent_packet;
		class CoordinateCorrected_packet;
		class PassportChangedEvent_packet;
		class ReverseEvent_packet;
		class StopCommandEvent_packet;
		class event_info
		{
			friend class StartCommandEvent_packet;
			friend class CoordinateCorrected_packet;
			friend class PassportChangedEvent_packet;
			friend class ReverseEvent_packet;
			friend class StopCommandEvent_packet;

		protected:
			virtual bool get_info(const StartCommandEvent_packet& packet) = 0;
			virtual bool get_info(const CoordinateCorrected_packet& packet) = 0;
			virtual bool get_info(const PassportChangedEvent_packet& packet) = 0;
			virtual bool get_info(const ReverseEvent_packet& packet) = 0;
			virtual bool get_info(const StopCommandEvent_packet& packet) = 0;

		public:
			virtual ~event_info(){}
		};



		class StartCommandEvent_packet : public event_packet
		{
		public:
			StartCommandEvent_packet() :event_packet(){ event_name = L"StartCommandEvent"; type = event_type::EvStartCommandEvent; }
			~StartCommandEvent_packet()
			{
			}
			virtual bool get_info(event_info * event_info) { return event_info->get_info(*this); }

		public:
			track_settings_item_t track_settings;
		};


		struct correct_direction_item_t
		{
			unsigned int counter;
			std::string direction;
			coordinate_item_t coordinate_item;
			std::string type;
		};

		class CoordinateCorrected_packet final : public event_packet
		{
		public:
			CoordinateCorrected_packet() :event_packet(){ event_name = L"CoordinateCorrectedEvent"; type = event_type::EvCoordinateCorrected; }
			~CoordinateCorrected_packet()
			{
			}
			virtual bool get_info(event_info * event_info) { return event_info->get_info(*this); }

		public:
			correct_direction_item_t correct_direction;
		};


		struct change_passport_point_direction_item_t
		{
			position_item_t start_item;
			nonstandard_kms_item_t kms;
		};
		class PassportChangedEvent_packet final : public event_packet
		{
		public:
			PassportChangedEvent_packet() :event_packet(){ event_name = L"PassportChangedEvent"; type = event_type::EvPassportChangedEvent; }
			~PassportChangedEvent_packet()
			{
			}
			virtual bool get_info(event_info * event_info) { return event_info->get_info(*this); }

		public:
			std::string direction;
			change_passport_point_direction_item_t change_passport_point_direction;
		};

		class ReverseEvent_packet final : public event_packet
		{
		public:
			ReverseEvent_packet() :event_packet(){ event_name = L"ReverseEvent"; type = event_type::EvReverseEvent; }
			~ReverseEvent_packet()
			{
			}
			virtual bool get_info(event_info * event_info) { return event_info->get_info(*this); }

		public:
			bool is_reverse;
			std::string orientation;
			bool is_auto;
		};
		class StopCommandEvent_packet final : public event_packet
		{
		public:
			StopCommandEvent_packet() :event_packet(){ event_name = L"StopCommandEvent"; type = event_type::EvStopCommandEvent; }
			~StopCommandEvent_packet()
			{
			}
			virtual bool get_info(event_info * event_info) { return event_info->get_info(*this); }

		public:
			unsigned int timeout;
		};


	}//namespace events

}//namespace position_detector

namespace logger
{
	std::wostream& operator << (std::wostream& out, const position_detector::events::CoordinateCorrected_packet & data);
	std::wostream & operator << (std::wostream & out, const ::position_detector::events::StartCommandEvent_packet & data);
	std::wostream & operator << (std::wostream & out, const ::position_detector::events::StopCommandEvent_packet & data);
	std::wostream & operator << (std::wostream & out, const ::position_detector::events::ReverseEvent_packet & data);
	std::wostream & operator << (std::wostream & out, const ::position_detector::events::PassportChangedEvent_packet & data);


}

namespace std
{
	std::wostream & operator << (std::wostream & out, const ::position_detector::events::event_packet * data);
}