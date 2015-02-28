#pragma once

#include <memory>

//#define TIMESTAMP_SYNCH_PACKET_ON

namespace position_detector
{
	class packets_manager;
	using packets_manager_ptr_t = std::shared_ptr<packets_manager>;

	using coordinate_t = int64_t;
	using line_t = uint32_t;
	using path_t = uint32_t;
	using span_t = uint32_t;
	using picket_t = uint32_t;
	using timestamp_t = uint64_t;
	using speed_t = uint32_t;
	using direction_t = bool;
	using counter_t = uint32_t;


	typedef struct _movment_info
	{
		_movment_info() :counter(0),coordinate(0), timestamp(0), speed(0), direction(false) {}
		counter_t counter;
		timestamp_t timestamp;
		speed_t speed;
		direction_t direction;
		coordinate_t coordinate; // ���������� �� ������ ���� � �� 

	}movment_info;


	typedef struct _path_info // ���������� � ����
	{
		_path_info() : path(0), span(0),line(0){}
		path_t path;			// ��� ���� 
		line_t line;			// ��� ����� 
		span_t span;			// ��� ��������
		std::string path_name;	 // �������
	} path_info;

	using path_info_ptr_t = std::shared_ptr<path_info>;


	typedef struct _track_point_info // ���������� � ����
	{
		movment_info _movment_info;
		path_info_ptr_t _path_info;
	} track_point_info;



	class packets_manager
	{
		friend class event_parser;
		static const unsigned int default_container_limit = 1000000;
	public:
		packets_manager(unsigned int container_limit = default_container_limit);
		~packets_manager();

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

