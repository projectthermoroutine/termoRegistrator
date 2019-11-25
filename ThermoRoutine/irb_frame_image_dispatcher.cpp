#include <Windows.h>
#include <memory>
#include <functional>
#include "irb_frame_image_dispatcher.h"
#include "irb_frame_helper.h"
#include "irb_frame_visiter.h"
#include <common\sync_helpers.h>
#include <ppl.h>

#define RASTER_FROM_TEMP_VALS_ON

namespace irb_frame_image_dispatcher
{
	namespace 
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

		void calc_calibration_interval(
			const irb_frame_helper::IRBFrame& frame,
			const tv_helper::TVpalette &palette,
			const temperature_span_t &manual__calibration_interval,
			IMAGE_CALIBRATION_TYPE calibration_type,
			temperature_span_t & T_span,
			float & scale, 
			int & offset
		) noexcept
		{
			using namespace irb_frame_helper;

			offset = 0;

			temperature_span_t real_span;

			switch (calibration_type)
			{
			case irb_frame_image_dispatcher::IMAGE_CALIBRATION_TYPE::NONE:
			{
				T_span.first = frame.header.calibration.tmin - g_Kelvin_Celsius_Delta /** (frame.correction_T_enabled() ? frame.correction_T_params().factor : 1)*/;
				T_span.second = frame.header.calibration.tmax - g_Kelvin_Celsius_Delta /** (frame.correction_T_enabled() ? frame.correction_T_params().factor : 1)*/;
				scale = (float)palette.numI / (T_span.second - T_span.first);
				return;
			}
			case irb_frame_image_dispatcher::IMAGE_CALIBRATION_TYPE::MIN_MAX:
			{
				T_span.first = frame.minT();
				T_span.second = frame.maxT();

				scale = 0.0f;
				if (T_span.first != T_span.second)
					scale = (float)palette.numI / (T_span.second - T_span.first);
				return;
			}
			case irb_frame_image_dispatcher::IMAGE_CALIBRATION_TYPE::AVERAGE:
			{
				auto high_temp_delta = frame.maxT() - frame.avgT();
				auto low_temp_delta = frame.avgT() - frame.minT();
				auto delta_delta = low_temp_delta / high_temp_delta;
				auto min_delta = high_temp_delta;
				if (high_temp_delta > low_temp_delta) {
					delta_delta = high_temp_delta / low_temp_delta;
					min_delta = low_temp_delta;
				}
				if (delta_delta > 4 * min_delta)
				{
					if (high_temp_delta > low_temp_delta)
					{
						T_span.first = frame.minT();
						T_span.second = frame.avgT() + low_temp_delta;
					}
					else
					{
						T_span.first = frame.avgT() - high_temp_delta;
						T_span.second = frame.maxT();
					}

					real_span = T_span;
					break;
				}

				T_span.first = frame.minT();
				T_span.second = frame.maxT();
				real_span = T_span;
				break;
			}
			case irb_frame_image_dispatcher::IMAGE_CALIBRATION_TYPE::MANUAL:
			{
				T_span = manual__calibration_interval;

				if (T_span.first != T_span.second)
					scale = (float)palette.numI / (T_span.second - T_span.first);

				return;
			}
			};//switch (_calibration_type)

			scale = 0.0f;
			auto span = real_span.second - real_span.first;
			if (span == 0)
				return;
			auto coeff = (float)palette.numI / span;
			static const float max_coeff = 8.0f;
			if (coeff > max_coeff)
			{
				scale = max_coeff;
				offset = (int)((frame.avgT() - T_span.first)*(coeff - max_coeff));
			}
			else
			{
				scale = coeff;
			}
		}
	}//namespace


	image_dispatcher::image_dispatcher():
		_calibration_type(IMAGE_CALIBRATION_TYPE::MIN_MAX),
		_width(0),
		_height(0), 
		_last_frame(nullptr),
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

	bool image_dispatcher::get_calibration_interval(irb_frame_helper::IRBFrame& frame, temperature_span_t & temperature_span, float & scale, int & offset) noexcept
	{
		bool result{false};

		offset = 0;
		if (_last_frame != &frame ||
			!frame.T_measured() || 
			(_check_bad_pixels && has_bad_pixels(frame.header.calibration.camera_sn))
			)
		{
			result = true;
			if (_check_bad_pixels &&
				has_bad_pixels(frame.header.calibration.camera_sn))
			{
				frame.ExtremumExcludePixels(_temp_vals.get(), *_bad_pixels_mask);
			}
			else
			{
#ifdef USE_PPL
				frame.Extremum_parallel(_temp_vals.get());
#else
				frame.Extremum(_temp_vals.get());
#endif
			}

			_last_frame = &frame;
		}

		calc_calibration_interval(frame, _palette, _calibration_interval, _calibration_type, temperature_span, scale, offset);
	
		return result;
	}

#define PROCESS_POINT_TEMPERATURE_NO_INLINE(_point_T, _x, _y) \
{\
	float temp_for_index = _point_T - corrected_Celsium_offset;\
	if (temp_for_index > calibration_interval.second)\
		temp_for_index = calibration_interval.second;\
	else if (temp_for_index < calibration_interval.first)\
		temp_for_index = calibration_interval.first;\
\
	int pallete_color_index = (int)(pallete_color_coefficient * (temp_for_index - calibration_interval.first)) + index_offset;\
\
	if (is_areas_exists)\
	{\
		if (IS_AREA_MASK_ITEM_SET(cur_area_mask_item))\
		{\
			auto area = GET_AREA_MASK_ITEM_KEY(cur_area_mask_item);/*areas_mask.get_key(cur_area_mask_item);*/\
			if (area != nullptr)\
			{\
				area->SetTemp( _point_T, _x, _y);\
			}\
		}\
	}\
\
	if (pallete_color_index > _palette.numI - 1)\
		pallete_color_index = _palette.numI - 1;\
\
	if (pallete_color_index < 0)\
		pallete_color_index = 0;\
\
	raster[offset] = _palette.image[pallete_color_index];\
}


#define PROCESS_POINT_TEMPERATURE(_point_T, _x, _y) \
{\
	float temp_for_index = _point_T - corrected_Celsium_offset;\
	if (temp_for_index > calibration_interval.second)\
		temp_for_index = calibration_interval.second;\
	else if (temp_for_index < calibration_interval.first)\
		temp_for_index = calibration_interval.first;\
\
	int pallete_color_index = (int)(pallete_color_coefficient * (temp_for_index - calibration_interval.first)) + index_offset;\
\
	if (is_areas_exists)\
	{\
		if (IS_AREA_MASK_ITEM_SET(cur_area_mask_item))\
		{\
			auto area = GET_AREA_MASK_ITEM_KEY(cur_area_mask_item);/*areas_mask.get_key(cur_area_mask_item);*/\
			if (area != nullptr)\
			{\
				if(area->proxy)\
					SET_PROXY_AREA_T_POINT(area, _point_T, _x, _y)\
				else{\
					AreaBase & _area_ref = *area;\
					SET_AREA_T_POINT(_area_ref, _point_T, _x, _y)\
				}\
			}\
		}\
	}\
\
	if (pallete_color_index > _palette.numI - 1)\
		pallete_color_index = _palette.numI - 1;\
\
	if (pallete_color_index < 0)\
		pallete_color_index = 0;\
\
	raster[offset] = _palette.image[pallete_color_index];\
}

	bool image_dispatcher::get_formated_frame_raster_parallel(
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
		const auto T_measured = get_calibration_interval(*frame, calibration_interval, pallete_color_coefficient, index_offset);

		const float corrected_Celsium_offset = frame->corrected_Celsium_offset();

		std::lock_guard<decltype(_areas_dispatcher)> areas_lock(_areas_dispatcher);

		auto is_areas_exists = !_areas_dispatcher.Empty();
		_areas_dispatcher.set_default_areas();
		auto & areas_mask = _areas_dispatcher.get_areas_mask();

		const int firstY = frame->header.geometry.firstValidY;
		const int lastY = frame->header.geometry.lastValidY;
		const int firstX = frame->header.geometry.firstValidX;
		const int lastX = frame->header.geometry.lastValidX;

		const WORD imgWidth = frame->header.geometry.imgWidth;

		if (T_measured)
		{
			concurrency::parallel_for(firstY, lastY + 1, [&, firstX, lastX, imgWidth](int y)
			{
				int offset = imgWidth * (y - firstY);

				float *pixel_temp = &_temp_vals[imgWidth*y + firstX];
				mask_item_t *cur_area_mask_item = &areas_mask.mask[imgWidth*y + firstX];
				for (int x = firstX; x <= lastX; ++x, ++pixel_temp/*cur_pixel++*/)
				{
					float curTemp = *pixel_temp;

					PROCESS_POINT_TEMPERATURE(curTemp, x, y);

					++cur_area_mask_item;
					++offset;
				}
			});

		}
		else
		{
			const auto process_func = [&](int x, int y, float point_T)
			{
				int offset = imgWidth * (y - firstY) + (x - firstX);
				mask_item_t *cur_area_mask_item = &areas_mask.mask[imgWidth*y + x];

				PROCESS_POINT_TEMPERATURE(point_T, x, y);
			};

			frame->foreach_T_value_parallel(process_func);
		}

		_areas_dispatcher.flush_measures();

		return true;
	}

	bool image_dispatcher::get_formated_frame_raster(
		const irb_frame_shared_ptr_t & frame, 
		irb_frame_raster_ptr_t raster, 
		temperature_span_t & calibration_interval
		) noexcept
	{
		LOG_STACK();
		if (raster == nullptr || !frame)
			return false;


		float pallete_color_coefficient = 0;
		int index_offset;
		const auto T_measured = get_calibration_interval(*frame, calibration_interval, pallete_color_coefficient, index_offset);

		const float corrected_Celsium_offset = frame->corrected_Celsium_offset();

		const int firstY = frame->header.geometry.firstValidY;
		const int lastY = frame->header.geometry.lastValidY;
		const int firstX = frame->header.geometry.firstValidX;
		const int lastX = frame->header.geometry.lastValidX;

		const WORD imgWidth = frame->header.geometry.imgWidth;

		int offset = 0;

		_areas_dispatcher.lock();
	
		auto is_areas_exists = !_areas_dispatcher.Empty();
		_areas_dispatcher.set_default_areas();
		auto & areas_mask = _areas_dispatcher.get_areas_mask();
		mask_item_t *cur_area_mask_item = areas_mask.mask.data();


		if (T_measured) {

			float * pixel_temp;

			for (int y = firstY; y <= lastY; ++y)
			{
				pixel_temp = &_temp_vals[frame->header.geometry.imgWidth*y + firstX];
				cur_area_mask_item = &areas_mask.mask[frame->header.geometry.imgWidth*y + firstX];
				for (int x = firstX; x <= lastX; ++x, ++pixel_temp/*cur_pixel++*/)
				{
					float curTemp = *pixel_temp;

					PROCESS_POINT_TEMPERATURE(curTemp, x, y);

					++cur_area_mask_item;
					++offset;
				}
			}
		}
		else
		{
			const auto process_func = [&](int x, int y, float point_T)
			{
				int offset = imgWidth * (y - firstY) + (x - firstX);
				mask_item_t *cur_area_mask_item = &areas_mask.mask[imgWidth*y + x];

				PROCESS_POINT_TEMPERATURE(point_T, x, y);
			};

			frame->foreach_T_value(process_func);

		}
		
		_areas_dispatcher.unlock();

		return true;
	}

	bool image_dispatcher::get_formated_frame_raster_fast(
		const irb_frame_shared_ptr_t & frame,
		irb_frame_raster_ptr_t raster,
		temperature_span_t & calibration_interval
	) noexcept
	{
		LOG_STACK();
		if (raster == nullptr || !frame)
			return false;

		float pallete_color_coefficient = 0;
		int index_offset;

		if (!frame->T_measured() && _last_frame != frame.get())
		{
			return get_formated_frame_raster(frame, raster, calibration_interval);
		}

		_last_frame = frame.get();

		calc_calibration_interval(*frame, _palette, _calibration_interval, _calibration_type, calibration_interval, pallete_color_coefficient, index_offset);

		const float corrected_Celsium_offset = frame->corrected_Celsium_offset();

		int offset = 0;

		float * curr_T_value = _temp_vals.get();

		_areas_dispatcher.lock();

		auto is_areas_exists = !_areas_dispatcher.Empty();
		_areas_dispatcher.set_default_areas();
		auto & areas_mask = _areas_dispatcher.get_areas_mask();
		const auto area_mask_ptr = areas_mask.mask.data();
		mask_item_t *cur_area_mask_item = area_mask_ptr;

		irb_frame_helper::IRBFrame & frame_ref = *frame;
		//const WORD imgWidth = frame_ref.header.geometry.imgWidth;

		float point_T = 0.0f;
		int x, y;
		FOREACH_FRAME_T(frame_ref, point_T, x, y, 
			*curr_T_value = point_T;
			//cur_area_mask_item = &areas_mask.mask[imgWidth*y + x];
			PROCESS_POINT_TEMPERATURE(point_T, x, y);
			++cur_area_mask_item;
			++curr_T_value;
			++offset;
			);
		_areas_dispatcher.unlock();

		return true;
	}
}