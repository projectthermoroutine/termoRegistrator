#pragma once

#include <memory>
#include <string>
#include <functional>
#include <vector>

#include <position_detector_common\position_detector_packet.h>

//#define TIMESTAMP_SYNCH_PACKET_ON
namespace position_detector
{
	class packets_manager;
	using packets_manager_ptr_t = std::shared_ptr<packets_manager>;


	using coordinate_t = int64_t;
	using line_t = std::wstring;
	using railway_t = std::wstring;
	using path_t = std::wstring;
	using line_path_t = std::wstring;
	using span_t = uint32_t;
	using timestamp_t = uint64_t;
	using speed_t = uint32_t;
	using direction_t = bool;
	using counter_t = int64_t;
	using counter32_t = uint32_t;
	using counter_size_t = uint32_t;

	using picket_t = int32_t;
	using offset_t = int32_t;

	enum class PATH_TYPE : uint8_t {
		Main = 0,
		Station = 1,
		Other = 2
	};

	enum class DEVICE_LAYOUT : uint8_t {
		AHEAD = 0,
		BEHIND = 1
	};



#pragma pack(push,8)
	struct path_info // информация о пути
	{
		path_info() : direction(0), path_type(PATH_TYPE::Main){}
		railway_t railway;			// код жд 
		line_t line;			// код линии 
		path_t path;			// код пути 
		PATH_TYPE path_type;		// тип пути
		line_path_t line_path;
		uint8_t direction;		// направление движения (1 - на уменьшение координат; 0 - на увеличение координат)
		std::wstring path_name;
		counter32_t counter0;

		bool operator==(const path_info& other)
		{
			return path == other.path &&
				path_type == other.path_type &&
				line == other.line &&
				railway == other.railway;
		}
	};

	using path_info_ptr_t = std::shared_ptr<path_info>;

	struct track_point_info// информация о пути
	{
		track_point_info() :counter(0), coordinate(0), timestamp(0), speed(0), direction(false), valid(false){}
		position_detector::counter32_t counter;
		counter_size_t counter_size;
		coordinate_t coordinate; // координата от начала пути в мм 
		picket_t picket; 
		offset_t offset;

		timestamp_t timestamp;
		speed_t speed;
		position_detector::direction_t direction;
		path_info_ptr_t _path_info;
		bool valid;
	};

#pragma pack(pop)

	enum class CoordType{
		METRO = 100,
		RAILWAY = 1000
	};

	class icoordinate_calculator;
	//using coordinate_calculator_ptr_t = std::shared_ptr<icoordinate_calculator>;
	using coordinate_calculator_ptr_t = std::unique_ptr<icoordinate_calculator>;

	using change_coordinate_notify_t = std::function<void(position_detector::counter32_t, position_detector::counter32_t, coordinate_calculator_ptr_t)>;
	using passport_changed_process_func_t = change_coordinate_notify_t;
	using coordinate_corrected_process_func_t = change_coordinate_notify_t;

	using track_state_change_notify_func_t = std::function<void(bool)>;
	class packets_manager
	{
		static const unsigned int default_container_limit = 1000000;
	public:
		packets_manager(uint8_t counter_size, 
						coordinate_t device_offset = 0, 
						DEVICE_LAYOUT device_lyaout = DEVICE_LAYOUT::AHEAD, 
						unsigned int container_limit = default_container_limit, 
						CoordType coord_type = CoordType::METRO
					   );
		~packets_manager();

		void set_counter_size(uint8_t counter_size);
		void set_device_offset(coordinate_t device_offset);

		template<typename TMessage> void add_packet(const TMessage &);
#ifdef TIMESTAMP_SYNCH_PACKET_ON
		bool get_point_info_by_time(const time_t &time, track_point_info& info);
#else
		bool get_last_point_info(track_point_info& info) const;
#endif
		void add_passport_changed_process_func(const passport_changed_process_func_t& func);
		void add_coordinate_corrected_process_func(const coordinate_corrected_process_func_t& func);
		void remove_passport_changed_process_func(const passport_changed_process_func_t& );
		void remove_coordinate_corrected_process_func(const coordinate_corrected_process_func_t& );

		void set_track_state_change_notify(const track_state_change_notify_func_t&);

		events::event_packet_ptr_t get_start_event_packet() const;
		std::vector<events::event_packet_ptr_t> get_actual_pd_event_packets() const;

	private:

		struct Impl;
		std::unique_ptr<Impl> _p_impl;
	};

	class icoordinate_calculator 
	{
	public:
		virtual void calculate(track_point_info& info) = 0;
	};

}//namespace position_detector

