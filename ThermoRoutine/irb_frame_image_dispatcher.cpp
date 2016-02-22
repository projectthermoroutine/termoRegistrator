#include <Windows.h>
#include <memory>
#include <functional>
#include "irb_frame_image_dispatcher.h"
#include "irb_frame_helper.h"
#include <common\sync_helpers.h>

#define RASTER_FROM_TEMP_VALS_ON

namespace irb_frame_image_dispatcher
{
	// каким цветом рисуем это значение
	COLORREF Color(float val, const tv_helper::TVpalette& pallete, const temperature_span_t& span)
	{
		double x = pallete.numI*(val - span.first) / (span.second - span.first);
		if (x < 0) x = 0;
		int r = (int)(x + 0.5);
		if (r >= pallete.numI) r = pallete.numI - 1;
		BYTE R = GetBValue(pallete.image[r]);
		BYTE B = GetRValue(pallete.image[r]);
		BYTE G = GetGValue(pallete.image[r]);

		return RGB(R, G, B);
	}


	image_dispatcher::image_dispatcher():
		_calibration_type(IMAGE_CALIBRATION_TYPE::MIN_MAX),
		_width(0),
		_height(0), 
		_last_frame_id(0),
		_check_bad_pixels(false)
	{
		allocate_temp_vals(1024,768);
		_calibration_interval.first = 0.0f;
		_calibration_interval.second = 50.f;
	}

	image_dispatcher::~image_dispatcher() = default;

	void image_dispatcher::allocate_temp_vals(uint16_t width, uint16_t height)
	{
		if (_width != width || _height != height)
		{
			_width = width; _height = height;
			_temp_vals = std::make_unique<float[]>(_width*_height);
		}
	}


	// расчет диапазона для текущего кадра     
	void image_dispatcher::calculate_frame_temperature_span(const irb_frame_shared_ptr_t & frame, temperature_span_t &temperature_span)
	{
		frame->Extremum();
		temperature_span.first = frame->min_temperature;
		temperature_span.second = frame->max_temperature;
	}

	void image_dispatcher::get_calibration_interval(irb_frame_helper::IRBFrame& frame, temperature_span_t & temperature_span, float & scale, int & offset)
	{
		offset = 0;
		if (_last_frame_id != frame.id || !frame.is_temperature_span_calculated(_temp_vals.get()))
		{
			if (_check_bad_pixels && 
				has_bad_pixels(frame.header.calibration.camera_sn) &&
				!frame.is_bad_pixels_processed())
				frame.ExtremumExcludePixels(_temp_vals.get(),*_bad_pixels_mask);
			else
				frame.Extremum(_temp_vals.get());
			_last_frame_id = frame.id;
		}

		temperature_span_t real_span;

		switch (_calibration_type)
		{
		case irb_frame_image_dispatcher::IMAGE_CALIBRATION_TYPE::NONE:
			temperature_span.first = frame.header.calibration.tmin - 273.15f;
			temperature_span.second = frame.header.calibration.tmax - 273.15f;
			scale = (float)_palette.numI / (temperature_span.second - temperature_span.first);
			return;
			//break;
		case irb_frame_image_dispatcher::IMAGE_CALIBRATION_TYPE::MIN_MAX:

			temperature_span.first = frame.minT();
			temperature_span.second = frame.maxT();

			//real_span.first = frame->min_temperature;
			//real_span.second = frame->max_temperature;
			scale = 0.0f;
			if (temperature_span.first == temperature_span.second)
				return;
			scale = (float)_palette.numI / (temperature_span.second - temperature_span.first);
			return;

			//break;
		case irb_frame_image_dispatcher::IMAGE_CALIBRATION_TYPE::AVERAGE:
		{
																			auto high_temp_delta = frame.maxT() - frame.avgT();
																			auto low_temp_delta = frame.avgT() - frame.minT();
																			auto delta_delta = low_temp_delta - high_temp_delta;
																			auto min_delta = high_temp_delta;
																			if (high_temp_delta > low_temp_delta){
																				delta_delta = high_temp_delta - low_temp_delta;
																				min_delta = low_temp_delta;
																			}
																			if (delta_delta > 2 * min_delta)
																			{
																				if (high_temp_delta > low_temp_delta)
																				{
																					temperature_span.first = frame.minT();
																					temperature_span.second = frame.avgT() + low_temp_delta;
																				}
																				else
																				{
																					temperature_span.first = frame.avgT() - high_temp_delta;
																					temperature_span.second = frame.maxT();
																				}

																				real_span = temperature_span;
																				break;
																			}

																			temperature_span.first = frame.minT();
																			temperature_span.second = frame.maxT();
																			real_span = temperature_span;
																			break;
		}
		case irb_frame_image_dispatcher::IMAGE_CALIBRATION_TYPE::MANUAL:

			temperature_span = _calibration_interval;
//			real_span = temperature_span;

			if (temperature_span.first == temperature_span.second)
				return;
			scale = (float)_palette.numI / (temperature_span.second - temperature_span.first);

			return;
			break;
		default:
			break;
		}

		scale = 0.0f;
		auto span = real_span.second - real_span.first;
		if (span == 0)
			return;
		auto coeff = (float)_palette.numI / span;
		static const float max_coeff = 8.0f;
		if (coeff > max_coeff)
		{
			scale = max_coeff;
			offset = (int)((frame.avgT() - temperature_span.first)*(coeff - max_coeff));
		}
		else
		{
			scale = coeff;
		}
	
	}


	bool image_dispatcher::get_formated_frame_raster(
		const irb_frame_shared_ptr_t & frame, 
		irb_frame_raster_ptr_t raster, 
		temperature_span_t & calibration_interval
		)
	{

		LOG_STACK();
		if (raster == nullptr || !frame)
			return false;


		float pallete_color_coefficient = 0;
		int index_offset;
		get_calibration_interval(*frame, calibration_interval, pallete_color_coefficient, index_offset);


#ifdef RASTER_FROM_TEMP_VALS_ON
#else
		irb_frame_helper::irb_pixel_t fromw = frame->get_min_temperature_pixel();
		irb_frame_helper::irb_pixel_t tow = frame->get_max_temperature_pixel();
		auto pixels_span = tow - fromw;
		float dttDASHdw = 0.0f;
		if (pixels_span != 0){
			dttDASHdw = (calibration_interval.second - calibration_interval.first) / pixels_span;
			pallete_color_coefficient = (float)_pallete.numI / pixels_span;
		}
#endif
		int firstY = frame->header.geometry.firstValidY;
		int lastY = frame->header.geometry.lastValidY;
		int firstX = frame->header.geometry.firstValidX;
		int lastX = frame->header.geometry.lastValidX;

		int offset = 0;

		_areas_dispatcher.lock();
	
		auto is_areas_exists = !_areas_dispatcher.Empty();
		_areas_dispatcher.set_default_areas();
		auto & areas_mask = _areas_dispatcher.get_areas_mask();
		mask_item_t *cur_area_mask_item = areas_mask.mask.data();
		double avrw = 0;

		float * pixel_temp;

		for (int y = firstY; y <= lastY; y++)
		{
			pixel_temp = &_temp_vals[frame->header.geometry.imgWidth*y + firstX];
			cur_area_mask_item = &areas_mask.mask[frame->header.geometry.imgWidth*y + firstX];
			for (int x = firstX; x <= lastX; x++, pixel_temp++/*cur_pixel++*/)
			{
#ifdef RASTER_FROM_TEMP_VALS_ON
				float curTemp = *pixel_temp;

				float temp_for_index = curTemp - 273.15f;
				if (temp_for_index > calibration_interval.second)
					temp_for_index = calibration_interval.second;
				else if (temp_for_index < calibration_interval.first)
					temp_for_index = calibration_interval.first;


				int pallete_color_index = (int)(pallete_color_coefficient * (temp_for_index - calibration_interval.first)) + index_offset;

#else
				unsigned int dt = *cur_pixel - fromw;
				int pallete_color_index = (int)(pallete_color_coefficient * dt);
				float curTemp = dt * (float)dttDASHdw + temperature_span.first;
#endif
				if (is_areas_exists)
				{
					if (IS_AREA_MASK_ITEM_SET(cur_area_mask_item))
					{
						auto area = areas_mask.get_key(cur_area_mask_item);
						if (area != nullptr)
						{
							area->SetTemp(curTemp);
						}
					}
				}

				cur_area_mask_item++;

				if (pallete_color_index > _palette.numI - 1)
					pallete_color_index = _palette.numI - 1;

				if (pallete_color_index < 0)
					pallete_color_index = 0;

				raster[offset] = _palette.image[pallete_color_index];
				offset++;
			}
		}

		_areas_dispatcher.unlock();

		return true;
	}

	void  image_dispatcher::calculate_average_temperature(const irb_frame_shared_ptr_t & frame)
	{
		temperature_span_t temperature_span;
		if (!frame->is_temperature_span_calculated())
			calculate_frame_temperature_span(frame, temperature_span);
		else
		{
			temperature_span.first = frame->min_temperature;
			temperature_span.second = frame->max_temperature;
		}

		irb_frame_helper::irb_pixel_t fromw = frame->get_min_temperature_pixel();
		irb_frame_helper::irb_pixel_t tow = frame->get_max_temperature_pixel();
		double dw = (double)(tow - fromw);

		dw = ((dw > 0) ? dw : 0.01);

		unsigned int tmp = _palette.numI;

		double dttDASHdw = (double)(temperature_span.second - temperature_span.first) / dw;

		int firstY = frame->header.geometry.firstValidY;
		int lastY = frame->header.geometry.lastValidY;
		int firstX = frame->header.geometry.firstValidX;
		int lastX = frame->header.geometry.lastValidX;
		uint32_t number_valid_pixels = (lastX - firstX + 1)*(lastY - firstY + 1);

		int offset = 0;
		irb_frame_helper::irb_pixel_t *cur_pixel = nullptr;

		double avrw = 0;

		for (int y = firstY; y <= lastY; y++/*, cur_raster_line = cur_raster_line + frame->header.geometry.imgWidth*/)
		{
			cur_pixel = &frame->pixels[frame->header.geometry.imgWidth*y + firstX];
			for (int x = firstX; x <= lastX; x++, cur_pixel++)
			{
				unsigned int dt = *cur_pixel - fromw;

				int xx = (int)(((double)(tmp * dt)) / dw);

				float curTemp = dt * (float)dttDASHdw + temperature_span.first;

				avrw += curTemp;// - 273.15;
			}
		}

		if (number_valid_pixels > 0)
			frame->avr_temperature = (float)(avrw / number_valid_pixels);
	}
}