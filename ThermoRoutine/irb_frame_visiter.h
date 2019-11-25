#pragma once
#include "irb_frame_helper.h"
#include <common\string_utils.h>
#include <ppl.h>
#include <mutex>

#include <loglib\log.h>

namespace irb_frame_helper
{

	class frame_visiter final
	{
	public:
		frame_visiter(IRBFrame & frame) noexcept
			: _frame(frame)
			, min_T(frame.min_temperature)
			, max_T(frame.max_temperature)
			, avr_T(frame.avr_temperature)
			, max_T_point(frame._max_temperature_point)
			, min_T_point(frame._min_temperature_point)
			, max_T_pixel(frame._max_temperature_pixel)
			, min_T_pixel(frame._min_temperature_pixel)
			, T_measured(frame._T_measured)
		{}

	public:
		float & min_T;
		float & max_T;
		float & avr_T;
		pixel_point_t & max_T_point;
		pixel_point_t & min_T_point;

		irb_pixel_t & max_T_pixel;
		irb_pixel_t & min_T_pixel;

		bool & T_measured;

	private:
		IRBFrame & _frame;

	};

#define RETRIEVE_PIXEL_TEMPERATURE(_T_vals, _value, _pixel, _corr_T_enabled, _correction_T_params) \
	\
		byte hiByte = _pixel >> 8; \
		byte loByte = _pixel & 0xFF; \
		float Temp1 = _T_vals[hiByte];\
		float Temp2 = _T_vals[hiByte + 1];\
		_value = (Temp1 + ((Temp2 - Temp1)*(float)loByte / 256)); \
		if (_corr_T_enabled)\
			_value = _value * _correction_T_params.factor + _correction_T_params.offset;\
	


#define FOREACH_FRAME_T(_frame, _point_T, _x, _y, _action)\
	{\
		using namespace irb_frame_helper;\
		frame_visiter visiter(_frame);\
		visiter.T_measured = false;\
		/*visiter.max_T = -500.0f;*/\
		/*visiter.min_T = 500.0f;*/\
		/*visiter.avr_T = 0.0f;*/\
		float max_T = -500.0f;\
		float min_T = 500.0f;\
		pixel_point_t max_T_point{}, min_T_point{};\
		irb_pixel_t max_T_pixel{}, min_T_pixel{};\
		const int firstY = _frame.header.geometry.firstValidY;\
		const int lastY =  _frame.header.geometry.lastValidY;\
		const int firstX = _frame.header.geometry.firstValidX;\
		const int lastX =  _frame.header.geometry.lastValidX;\
		irb_pixel_t *cur_pixel;\
		/*double avg_T = 0.0f;*/\
		const bool corr_T_enabled = _frame.correction_T_enabled();\
		const correction_T_params_t & corr_T_params = _frame.correction_T_params();\
		const FLOAT * frame_T_vals = &_frame.header.calibration.tempvals[0];\
		/*const auto frame_pixels = _frame.pixels.get();*/\
		cur_pixel =  &_frame.pixels[_frame.header.geometry.imgWidth*firstY];\
		for (_y = firstY; _y <= lastY; ++_y)\
		{\
			cur_pixel += firstX;\
			for (_x = firstX; _x <= lastX; ++_x, ++cur_pixel)\
			{\
				irb_pixel_t pixel = *cur_pixel;\
				RETRIEVE_PIXEL_TEMPERATURE(frame_T_vals, _point_T, pixel, corr_T_enabled, corr_T_params);\
				_action; /*(x, y, point_temp);*/\
				/*avg_T += _point_T;*/\
				if (max_T < _point_T)\
				{\
					max_T_point.first = (std::uint16_t)_x;\
					max_T_point.second = (std::uint16_t)_y;\
					max_T = _point_T;\
					max_T_pixel = pixel;\
				}\
				if (min_T > _point_T)\
				{\
					min_T_point.first = (std::uint16_t)_x;\
					min_T_point.second = (std::uint16_t)_y;\
					min_T = _point_T;\
					min_T_pixel = pixel;\
				}\
			}\
		}\
		/*visiter.avr_T = (float)(avg_T / ((lastX - firstX + 1)*(lastY - firstY + 1)));*/\
		visiter.max_T_point = max_T_point;\
		visiter.min_T_point = min_T_point;\
		visiter.max_T_pixel = max_T_pixel;\
		visiter.min_T_pixel = min_T_pixel;\
		visiter.T_measured = true;\
	}

}//namespace irb_frame_helper