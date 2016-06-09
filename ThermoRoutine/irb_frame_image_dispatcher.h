#pragma once
#include <Windows.h>
#include <vector>
#include <memory>
#include "irb_frame_helper.h"
#include <common\handle_holder.h>
#include <fstream>
#include <queue>
#include <list>
#include <map>
#include <common\sync_helpers.h>
#include "AreaBase.h"

#include "tv_helper.h"

#ifdef _WINGDI_
#undef _WINGDI_
#endif
#include "ThermoRoutine_i.h"



namespace irb_frame_image_dispatcher
{

	using irb_frame_shared_ptr_t = std::shared_ptr<irb_frame_helper::IRBFrame>;

	//using namespace irb_frame_helper;

	using irb_frame_raster_t = DWORD;
	using irb_frame_raster_ptr_t = irb_frame_raster_t*;
	using area_ptr_t = std::shared_ptr<AreaBase>;

	using temperature_span_t = std::pair<float, float>;

	COLORREF Color(float val, const tv_helper::TVpalette& pallete, const temperature_span_t& span);

	//typedef struct _area_temperature_measure
	//{
	//	float min;
	//	float max;
	//	float avr;
	//}area_temperature_measure;

	enum class IMAGE_CALIBRATION_TYPE
	{
		NONE,
		MIN_MAX,
		AVERAGE,
		MANUAL
	};


	using bad_pixels_mask_ptr_t = std::unique_ptr<irb_frame_helper::bad_pixels_mask>;

	class image_dispatcher final
	{

	public:
		image_dispatcher();

		image_dispatcher(const image_dispatcher& other)
		{
			if (this == &other)
				return;
			_check_bad_pixels = other._check_bad_pixels;
			_areas_dispatcher = other._areas_dispatcher;
			_temperature_span = other._temperature_span;
			_calibration_interval = other._calibration_interval;
			_palette = other._palette;
			allocate_temp_vals(other._width, other._height);
		}

		image_dispatcher & operator = (const image_dispatcher &other)
		{
			if (this != &other){
				_check_bad_pixels = other._check_bad_pixels;
				_areas_dispatcher = other._areas_dispatcher;
				_temperature_span = other._temperature_span;
				_calibration_interval = other._calibration_interval;
				_palette = other._palette;
				allocate_temp_vals(other._width, other._height);
			}
			return (*this);
		}

	public:
		~image_dispatcher();

		bool get_formated_frame_raster(const irb_frame_shared_ptr_t & frame, irb_frame_raster_ptr_t raster, temperature_span_t & calibration_interval);
		void calculate_average_temperature(const irb_frame_shared_ptr_t & frame);

		void set_calibration_type(IMAGE_CALIBRATION_TYPE type) { _calibration_type = type; }

	private:
		void calculate_frame_temperature_span(const irb_frame_shared_ptr_t & frame, temperature_span_t & temperature_span);
		void get_calibration_interval(irb_frame_helper::IRBFrame& frame, temperature_span_t & temperature_span, float & scale, int & offset);
		void allocate_temp_vals(uint16_t width, uint16_t height);
		inline bool has_bad_pixels(const char camera_sn[15]) const { return _bad_pixels_camera_sn.compare(0, _bad_pixels_camera_sn.size(), camera_sn) == 0; }

	private:
		temperature_span_t _temperature_span;
		temperature_span_t _calibration_interval;
		tv_helper::TVpalette _palette;    // палитра

		std::unique_ptr<float[]> _temp_vals;
		irb_frame_helper::frame_id_t _last_frame_id;
		uint16_t _width;
		uint16_t _height;

		IMAGE_CALIBRATION_TYPE _calibration_type;
		bool _check_bad_pixels;

	private:

		bad_pixels_mask_ptr_t _bad_pixels_mask;
		std::string _bad_pixels_camera_sn;
		areas_dispatcher _areas_dispatcher;

	public:
		void set_bad_pixels_mask(bad_pixels_mask_ptr_t & mask, const std::string& camera_sn)
		{
			_bad_pixels_mask.swap(mask);
			_bad_pixels_camera_sn = camera_sn;
			_check_bad_pixels = _bad_pixels_mask.operator bool();
		}

		const areas_dispatcher& areas_dispatcher() const { return _areas_dispatcher; }


		bool get_area_temperature_measure(int area_id, area_temperature_measure &measure)
		{
			return _areas_dispatcher.get_area_temperature_measure(area_id, measure);
		}

		void set_areas_mask_size(uint16_t width, uint16_t height){ _areas_dispatcher.set_areas_mask_size(width, height); }
		void clear_areas() { _areas_dispatcher.clear_areas(); }

		bool AddAreaRect(const AreaRect &area) { return _areas_dispatcher.AddAreaRect(area); }
		bool AddAreaEllips(const AreaEllips &area){ return _areas_dispatcher.AddAreaEllips(area); }
		void DelArea(SHORT id){ _areas_dispatcher.DelArea(id); }

		void ChangeRectArea(SHORT id, const AreaRect &area) { _areas_dispatcher.ChangeRectArea(id,area); }
		void ChangeEllipsArea(SHORT id, const AreaEllips &area){ _areas_dispatcher.ChangeEllipsArea(id,area); }


		bool set_palette(const std::wstring & palette_file_name) { return _palette.Load(palette_file_name); }
		void set_default_palette() { _palette.LoadDefault(); }
		const void * get_palette_image() { return _palette.image; }
		const tv_helper::TVpalette& get_palette() { return _palette; }
		int get_palette_size() const { return _palette.numI; }
		void set_calibration_interval(float min, float max) { _calibration_interval.first = min; _calibration_interval.second = max; }
	};


	template<class TFrame>
	void retrieve_areas_T(
		const TFrame& frame,
		float * temp_vals,
		areas_dispatcher& areas
		)
	{

		areas.lock();
		if (areas.Empty()){
			areas.unlock();
			return;
		}

		short areas_width, areas_height;

		areas.get_areas_mask_size(areas_width, areas_height);


		if (areas_width < frame->header.geometry.imgWidth || areas_height < frame->header.geometry.imgHeight)
		{
			areas.unlock();
			return;
		}

		std::unique_ptr<float[]> T_vals_ptr;
		if (temp_vals == nullptr){
			T_vals_ptr = std::make_unique<float[]>(frame->header.geometry.imgWidth*frame->header.geometry.imgHeight);
			temp_vals = T_vals_ptr.get();
		}

		frame->Extremum(temp_vals);

		areas.set_default_areas();
		auto & areas_mask = areas.get_areas_mask();
		mask_item_t *cur_area_mask_item = areas_mask.mask.data();

		int firstY = frame->header.geometry.firstValidY;
		int lastY = frame->header.geometry.lastValidY;
		int firstX = frame->header.geometry.firstValidX;
		int lastX = frame->header.geometry.lastValidX;

		float * pixel_temp;
		for (int y = firstY; y <= lastY; y++)
		{
			pixel_temp = &temp_vals[frame->header.geometry.imgWidth*y + firstX];
			cur_area_mask_item = &areas_mask.mask[frame->header.geometry.imgWidth*y + firstX];
			for (int x = firstX; x <= lastX; x++, pixel_temp++/*cur_pixel++*/)
			{
				if (IS_AREA_MASK_ITEM_SET(cur_area_mask_item))
				{
					auto area = areas_mask.get_key(cur_area_mask_item);
					if (area != nullptr)
					{
						area->SetTemp(*pixel_temp);
					}
				}

				cur_area_mask_item++;
			}
		}

		areas.unlock();
	}
}