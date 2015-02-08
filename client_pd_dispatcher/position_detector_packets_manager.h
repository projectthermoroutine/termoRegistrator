#pragma once

#include <memory>

//#define TIMESTAMP_SYNCH_PACKET_ON

namespace position_detector
{
	class packets_manager;
	using packets_manager_ptr_t = std::shared_ptr<packets_manager>;

	using coordinate_t = uint64_t;
	using line_t = uint32_t;
	using path_t = uint32_t;
	using span_t = uint32_t;
	using picket_t = uint32_t;
	using timestamp_t = uint64_t;
	using speed_t = uint32_t;
	using direction_t = bool;


	typedef struct _movment_info
	{
		_movment_info() :coordinate(0), timestamp(0), speed(0), direction(false) {}

		timestamp_t timestamp;
		speed_t speed;
		direction_t direction;
		coordinate_t coordinate; // координата от начала пути в мм 

	}movment_info;


	typedef struct _path_info // информация о пути
	{
		_path_info() : path(0), span(0),line(0){}
		path_t path;			// код пути 
		line_t line;			// код линии 
		span_t span;			// код перегона
		std::string path_name;	 // перегон
	} path_info;

	using path_info_ptr_t = std::shared_ptr<path_info>;


	typedef struct _track_point_info // информация о пути
	{
		movment_info _movment_info;
		path_info_ptr_t _path_info;
	} track_point_info;



	class packets_manager
	{
		static const unsigned int default_container_limit = 1000000;
	public:
		packets_manager(unsigned int container_limit = default_container_limit);
		~packets_manager();

		template<typename TMessage> void add_packet(const TMessage &);
#ifdef TIMESTAMP_SYNCH_PACKET_ON
		bool get_point_info_by_time(const time_t &time, track_point_info& info);
#else
		bool get_last_point_info(track_point_info& info);
#endif
	private:

		struct Impl;
		std::unique_ptr<Impl> _p_impl;
	};

}//namespace position_detector

