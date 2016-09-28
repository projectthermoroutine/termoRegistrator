#pragma once

#include <memory>
#include <string>
#include <functional>

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
	using counter_t = uint32_t;
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
	struct _tag_path_info // информация о пути
	{
		_tag_path_info() : direction(0), path_type(PATH_TYPE::Main){}
		railway_t railway;			// код жд 
		line_t line;			// код линии 
		path_t path;			// код пути 
		PATH_TYPE path_type;		// тип пути
		line_path_t line_path;
		uint8_t direction;		// направление
		std::wstring path_name;

		bool operator==(const _tag_path_info& other)
		{
			return railway == other.railway &&
				path == other.path &&
				path_type == other.path_type &&
				line == other.line;
		}
	};


	typedef struct _tag_path_info path_info;

	using path_info_ptr_t = std::shared_ptr<path_info>;


	typedef struct _tag_track_point_info // информация о пути
	{
		_tag_track_point_info() :counter(0), coordinate(0), timestamp(0), speed(0), direction(false), valid(false){}
		counter_t counter;
		counter_size_t counter_size;
		coordinate_t coordinate; // координата от начала пути в мм 
		picket_t picket; 
		offset_t offset;

		timestamp_t timestamp;
		speed_t speed;
		direction_t direction;
		path_info_ptr_t _path_info;
		bool valid;
	} track_point_info;

#pragma pack(pop)

	enum class CoordType{
		METRO = 100,
		RAILWAY = 1000
	};

	class icoordinate_calculator;
	using passport_changed_process_func_t = std::function<void(counter_t, const icoordinate_calculator&)>;
	using coordinate_corrected_process_func_t = std::function<void(counter_t, const icoordinate_calculator&)>;
	class packets_manager
	{
		static const unsigned int default_container_limit = 1000000;
	public:
		packets_manager(uint8_t counter_size, coordinate_t device_offset = 0, DEVICE_LAYOUT device_lyaout = DEVICE_LAYOUT::AHEAD, unsigned int container_limit = default_container_limit, CoordType coord_type = CoordType::METRO);
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


	private:

		struct Impl;
		std::unique_ptr<Impl> _p_impl;
	};

	class icoordinate_calculator 
	{
	public:
		virtual void calculate(counter_t counter, int32_t direction, track_point_info& info) = 0;
	};



}//namespace position_detector

