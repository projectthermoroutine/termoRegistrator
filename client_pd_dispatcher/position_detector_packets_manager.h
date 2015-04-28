#pragma once

#include <memory>
#include <string>

//#define TIMESTAMP_SYNCH_PACKET_ON
namespace position_detector
{
	class packets_manager;
	using packets_manager_ptr_t = std::shared_ptr<packets_manager>;


	using coordinate_t = int64_t;
	using line_t = std::string;
	using railway_t = std::string;
	using path_t = std::string;
	using span_t = uint32_t;
	using picket_t = uint32_t;
	using timestamp_t = uint64_t;
	using speed_t = uint32_t;
	using direction_t = bool;
	using counter_t = uint32_t;

#pragma pack(push,8)
	struct _tag_movment_info
	{
		_tag_movment_info() :counter(0), coordinate(0), timestamp(0), speed(0), direction(false) {}
		counter_t counter;
		coordinate_t coordinate; // координата от начала пути в мм 
		timestamp_t timestamp;
		speed_t speed;
		direction_t direction;
	};

	typedef struct _tag_movment_info movment_info;

	struct _tag_path_info // информация о пути
	{
		_tag_path_info() : direction(0){}
		railway_t railway;			// код жд 
		path_t path;			// код пути 
		line_t line;			// код линии 
		uint8_t direction;			// направление
		std::string path_name;	 // перегон
	};


	typedef struct _tag_path_info path_info;

	using path_info_ptr_t = std::shared_ptr<path_info>;


	typedef struct _tag_track_point_info // информация о пути
	{
		_tag_track_point_info() :counter(0), coordinate(0), timestamp(0), speed(0), direction(false) {}
		counter_t counter;
		coordinate_t coordinate; // координата от начала пути в мм 
		timestamp_t timestamp;
		speed_t speed;
		direction_t direction;
		path_info_ptr_t _path_info;
	} track_point_info;

#pragma pack(pop)

	enum class CoordType{
		METRO = 100,
		RAILWAY = 1000
	};


	class packets_manager
	{
		friend class event_parser;
		static const unsigned int default_container_limit = 1000000;
	public:
		packets_manager(uint8_t counter_size, unsigned int container_limit = default_container_limit, CoordType coord_type = CoordType::METRO);
		~packets_manager();

		void set_counter_size(uint8_t counter_size);

		template<typename TMessage> void add_packet(const TMessage &);
#ifdef TIMESTAMP_SYNCH_PACKET_ON
		bool get_point_info_by_time(const time_t &time, track_point_info& info);
#else
		bool get_last_point_info(track_point_info& info) const;
#endif
	private:

		struct Impl;
		std::unique_ptr<Impl> _p_impl;
	};

}//namespace position_detector

