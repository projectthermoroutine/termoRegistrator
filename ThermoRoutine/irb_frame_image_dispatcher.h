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

	using irb_frame_raster_ptr_t = DWORD*;
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


	class image_dispatcher final
	{

	public:
		image_dispatcher();
	public:
		~image_dispatcher();

		bool get_formated_frame_raster(const irb_frame_shared_ptr_t & frame, irb_frame_raster_ptr_t raster, temperature_span_t & calibration_interval);
		void calculate_average_temperature(const irb_frame_shared_ptr_t & frame);

		void set_calibration_type(IMAGE_CALIBRATION_TYPE type) { _calibration_type = type; }

	private:
		void calculate_frame_temperature_span(const irb_frame_shared_ptr_t & frame, temperature_span_t & temperature_span);
		void get_calibration_interval(const irb_frame_shared_ptr_t & frame, temperature_span_t & temperature_span, float & scale, int & offset);
		void allocate_temp_vals(uint16_t width, uint16_t height);

	private:
		temperature_span_t _temperature_span;
		temperature_span_t _calibration_interval;
		tv_helper::TVdiap _default_diap;
		tv_helper::TVcalibr _calibration;     // калибровка
		tv_helper::TVpalette _palette;    // палитра

		std::unique_ptr<float[]> _temp_vals;
		uint16_t _width;
		uint16_t _height;

		sync_helpers::rw_lock _lock_frames_map;
		sync_helpers::rw_lock _lock_areas;


		IMAGE_CALIBRATION_TYPE _calibration_type;

	private:
		areas_dispatcher _areas_dispatcher;

	public:

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




		bool set_palette(const char * palette_file_name) { return _palette.Load(palette_file_name); }
		void set_default_palette() { _palette.LoadDefault(); }
		const void * get_palette_image() { return _palette.image; }
		const tv_helper::TVpalette& get_palette() { return _palette; }
		int get_palette_size() const { return _palette.numI; }
		void set_calibration_interval(float min, float max) { _calibration_interval.first = min; _calibration_interval.second = max; }
	};

}