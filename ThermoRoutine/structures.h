#pragma once
#include <Windows.h>
#include "irb_frame_helper.h"
#include "ThermoRoutine_i.h"
#include <comutil.h>

inline
void
fill_frame_info(
irb_frame_info& frame_info,
const std::shared_ptr<irb_frame_helper::IRBFrame> & frame
)
{
	frame_info.image_info.width = frame->header.geometry.imgWidth;
	frame_info.image_info.height = frame->header.geometry.imgHeight;

	//frame_info.image_info.firstValidX = frame->header.geometry.firstValidX;
	//frame_info.image_info.lastValidX = frame->header.geometry.lastValidX;
	//frame_info.image_info.firstValidY = frame->header.geometry.firstValidY;
	//frame_info.image_info.lastValidY = frame->header.geometry.lastValidY;

	frame_info.measure.tmin = frame->min_temperature;
	frame_info.measure.tavr = frame->avr_temperature;
	frame_info.measure.tmax = frame->max_temperature;

	frame_info.measure.object_tmin = frame->spec.IRBmin;
	frame_info.measure.object_tmax = frame->spec.IRBmax;

	frame_info.measure.calibration_min = frame->header.calibration.tmin - 273.15f;
	frame_info.measure.calibration_max = frame->header.calibration.tmax - 273.15f;

	const auto & frame_coords = frame->coords;
	frame_info.coordinate.coordinate = frame_coords.coordinate;
	auto bstr_path = _com_util::ConvertStringToBSTR(frame_coords.path.c_str());
	frame_info.coordinate.path = bstr_path;
	auto bstr_line = _com_util::ConvertStringToBSTR(frame_coords.line.c_str());
	frame_info.coordinate.line = bstr_line;
	frame_info.coordinate.direction = frame_coords.direction;
	frame_info.coordinate.camera_offset = frame_coords.camera_offset;

	frame_info.timestamp = frame->get_frame_time_in_sec();
}

inline
void
fill_frame_info(
irb_frame_info& frame_info,
const irb_frame_helper::IRBFrame & frame
)
{
	frame_info.image_info.width = frame.header.geometry.imgWidth;
	frame_info.image_info.height = frame.header.geometry.imgHeight;

	//frame_info.image_info.firstValidX = frame.header.geometry.firstValidX;
	//frame_info.image_info.lastValidX = frame.header.geometry.lastValidX;
	//frame_info.image_info.firstValidY = frame.header.geometry.firstValidY;
	//frame_info.image_info.lastValidY = frame.header.geometry.lastValidY;

	frame_info.measure.tmin = frame.min_temperature;
	frame_info.measure.tavr = frame.avr_temperature;
	frame_info.measure.tmax = frame.max_temperature;

	frame_info.measure.object_tmin = frame.spec.IRBmin;
	frame_info.measure.object_tmax = frame.spec.IRBmax;

	frame_info.measure.calibration_min = frame.header.calibration.tmin - 273.15f;
	frame_info.measure.calibration_max = frame.header.calibration.tmax - 273.15f;

	const auto & frame_coords = frame.coords;
	frame_info.coordinate.coordinate = frame_coords.coordinate;
	auto bstr_path = _com_util::ConvertStringToBSTR(frame_coords.path.c_str());
	frame_info.coordinate.path = bstr_path;
	auto bstr_line = _com_util::ConvertStringToBSTR(frame_coords.line.c_str());
	frame_info.coordinate.line = bstr_line;
	frame_info.coordinate.direction = frame_coords.direction;
	frame_info.coordinate.camera_offset = frame_coords.camera_offset;

	frame_info.timestamp = frame.get_frame_time_in_sec();
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
