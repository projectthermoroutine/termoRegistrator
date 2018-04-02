#pragma once
#include <Windows.h>
#include "irb_frame_helper.h"
#include "ThermoRoutine_i.h"
#include <comutil.h>


template<typename TFrame>
inline
void
fill_frame_info(
irb_frame_info& frame_info,
const TFrame & frame
)
{
	frame_info.image_info.width = frame.header.geometry.imgWidth;
	frame_info.image_info.height = frame.header.geometry.imgHeight;

	//frame_info.image_info.firstValidX = frame.header.geometry.firstValidX;
	//frame_info.image_info.lastValidX = frame.header.geometry.lastValidX;
	//frame_info.image_info.firstValidY = frame.header.geometry.firstValidY;
	//frame_info.image_info.lastValidY = frame.header.geometry.lastValidY;

	frame_info.measure.tmin = frame.minT();
	frame_info.measure.tavr = frame.avgT();
	frame_info.measure.tmax = frame.maxT();

	frame_info.measure.object_tmin = frame.minT();
	frame_info.measure.object_tmax = frame.maxT();

	frame_info.measure.calibration_min = frame.header.calibration.tmin - 273.15f;
	frame_info.measure.calibration_max = frame.header.calibration.tmax - 273.15f;

	fill_frame_position_info(frame_info.coordinate, frame);

	frame_info.timestamp = frame.get_frame_time_in_sec();

	frame_info.max_T_point.x = frame.max_T_point().first;
	frame_info.max_T_point.y = frame.max_T_point().second;

	frame_info.min_T_point.x = frame.min_T_point().first;
	frame_info.min_T_point.y = frame.min_T_point().second;

}

template<typename TFrame>
inline
void
fill_frame_position_info(
frame_coordinate& frame_coordinate,
const TFrame & frame
)
{
	const auto & frame_coords = frame.coords;
	frame_coordinate.counter = frame_coords.counter;
	frame_coordinate.coordinate = frame_coords.coordinate;
	frame_coordinate.picket = frame_coords.picket;
	frame_coordinate.offset = frame_coords.offset;
	frame_coordinate.path = ::SysAllocString(frame_coords.path.c_str());
	//frame_coordinate.path_name = nullptr;
	frame_coordinate.path_name = ::SysAllocString(frame_coords.path_name.c_str());
	frame_coordinate.line = ::SysAllocString(frame_coords.line.c_str());
	frame_coordinate.direction = frame_coords.direction;
	frame_coordinate.camera_offset = frame_coords.camera_offset;
	frame_coordinate.path_type = static_cast<decltype(frame_coordinate.path_type)>(frame_coords.path_type);
}



//typedef struct _area_temperature_measure
//{
//	float min;
//	float max;
//	float avr;
//}area_temperature_measure;
//
//typedef struct _area_temperature_measure_result
//{
//	int area_id;
//	bool result_is_valid;
//	area_temperature_measure measure;
//}area_temperature_measure_result;
