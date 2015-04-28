#include <Windows.h>

#include <vector>
#include <memory>
#include <algorithm>

#include <common\sync_helpers.h>

#include "metro_map.h"
#include "movie_transit.h"

#include "tvcrack.h"

#include "irb_frame_helper.h"
#include "irb_file_helper.h"

#include "irb_frame_spec_info.h"


#ifdef _WINGDI_
#undef _WINGDI_
#endif

namespace movie_transit_ns
{
	using namespace metro_map;
	struct movie_transit::Impl
	{
		metro_map_ptr_t metro_map;
		CTVcrack TVcrack;
	};


	movie_transit::movie_transit(const metro_map_ptr_t & metro_map) :_p_impl(new Impl)
	{
		_p_impl->metro_map = metro_map;
	}
	movie_transit::~movie_transit() = default;


	void movie_transit::clear_cache()
	{
		_p_impl->TVcrack.clear_cache();
	}


	::irb_frame_shared_ptr_t movie_transit::get_frame_by_index(uint32_t index)
	{
		return _p_impl->TVcrack.get_frame_by_index(index);
	}

	::irb_frame_shared_ptr_t movie_transit::get_frame_by_id(DWORD num)
	{
		return _p_impl->TVcrack.get_frame_by_id(num);
	}
	::irb_frame_shared_ptr_t movie_transit::get_frame_by_coordinate(coordinate_t coordinate)
	{
		return _p_impl->TVcrack.get_frame_by_coordinate(coordinate);
	}
	::irb_frame_shared_ptr_t movie_transit::get_frame_by_time(double time)
	{
		return _p_impl->TVcrack.get_frame_by_time(time);
	}

	::irb_frame_shared_ptr_t movie_transit::read_frame_by_id(DWORD num)
	{
		return _p_impl->TVcrack.read_frame_by_id(num);
	}
	int movie_transit::get_irb_file_index_by_frame_id(DWORD num)
	{
		return _p_impl->TVcrack.get_irb_file_index_by_frame_id(num);
	}

	bool movie_transit::SaveFrames(const std::vector<::irb_frame_shared_ptr_t> & frames, const std::string & fname)
	{
		return _p_impl->TVcrack.SaveFrames(frames, fname);
	}

	bool movie_transit::save_frame(uint32_t index, uint32_t picket, uint32_t offset, const std::string & fname)
	{
		irb_frame_spec_info::irb_frame_position_info position_info{ picket, offset };
		return _p_impl->TVcrack.save_frame(index, position_info, fname);
	}

	DWORD movie_transit::get_last_irb_frame_id()
	{
		return _p_impl->TVcrack.get_last_irb_frame_id();
	}
	DWORD movie_transit::get_first_irb_frame_id()
	{
		return _p_impl->TVcrack.get_first_irb_frame_id();
	}
	DWORD movie_transit::get_irb_frame_id_by_index(DWORD index)
	{
		return _p_impl->TVcrack.get_irb_frame_id_by_index(index);
	}
	LONG movie_transit::get_irb_frame_index_by_id(DWORD id)
	{
		return _p_impl->TVcrack.get_irb_frame_index_by_id(id);
	}




	void movie_transit::reset(bool save_filter)
	{
		_p_impl->TVcrack.reset(save_filter);
	}

	void movie_transit::add_irb_files(irb_files_list_t& files_list, bool try_inherit_filter)
	{
		_p_impl->TVcrack.set_irb_files(files_list, try_inherit_filter);
	}

	int movie_transit::number_irb_files()
	{
		return _p_impl->TVcrack.number_irb_files();
	}


	int movie_transit::number_frames_in_files()
	{
		return _p_impl->TVcrack.count_frames_in_files();
	}

	 const IRBFrame * movie_transit::current_irb_frame()
	{
		 return _p_impl->TVcrack.get_current_frame().get();
	}


	 bool movie_transit::set_palette(const char * pallete_file_name)
	 {
		 return _p_impl->TVcrack.set_palette(pallete_file_name);
	 }


	 void movie_transit::set_default_palette()
	 {
		 _p_impl->TVcrack.set_default_palette();
	 }


	 bool movie_transit::get_area_temperature_measure(int area_id, area_temperature_measure &measure)
	 {
		 return _p_impl->TVcrack.get_area_temperature_measure(area_id, measure);
	 }

	 void movie_transit::remove_all_areas()
	 {
		 _p_impl->TVcrack.clear_areas();;
	 }

	 void movie_transit::set_auto_palette_mode(bool mode)
	 {
		 _p_impl->TVcrack.m_fAutoPal = mode;
	 }
	 int movie_transit::get_palette_size()
	 {
		 return _p_impl->TVcrack.get_palette_size();
	 }
	 uint32_t movie_transit::get_palette_colors_number()
	 {
		 return _p_impl->TVcrack.get_palette_colors_number();
	 }

	 const void * movie_transit::get_palette_image()
	 {
		 return _p_impl->TVcrack.get_palette_image();
	 }

	 const TVpalette& movie_transit::get_palette()
	 {
		 return _p_impl->TVcrack.get_palette();
	 }

	 void movie_transit::set_palette_calibration_mode(calibration_mode mode)
	 {
		 _p_impl->TVcrack.set_palette_calibration_mode(mode);
	 }
	 void movie_transit::set_palette_calibration(float min, float max)
	 {
		 _p_impl->TVcrack.set_palette_calibration(min,max);
	 }



	 coordinate_t  movie_transit::ReadKm(DWORD frameNum)
	 {
		 return _p_impl->TVcrack.ReadKm(frameNum);
	 }

	 coordinate_t movie_transit::GetKm()
	 {
		 return _p_impl->TVcrack.GetKm();
	 }


	 void movie_transit::mark_frame_in_filter(uint32_t frame_index, bool state)
	 {
		 _p_impl->TVcrack.mark_frame_in_filter(frame_index, state);
	 }
	 void movie_transit::get_filter_params(FILTER &f)
	 {
		 _p_impl->TVcrack.get_filter_params(f);
	 }
	 uint32_t movie_transit::get_filter_flags()
	 {
		 return _p_impl->TVcrack.get_filter_flags();
	 }
	 void movie_transit::SetFilter(const FILTER &f)
	 {
		 _p_impl->TVcrack.SetFilter(f);
	 }

	 bool  movie_transit::get_formated_frame_raster_by_index(DWORD N,
		 irb_frame_image_dispatcher::irb_frame_raster_ptr_t raster,
		 irb_frame_image_dispatcher::temperature_span_t & calibration_interval)
	 {
		 return _p_impl->TVcrack.get_formated_frame_raster_by_index(N, raster, calibration_interval);
	 }

	 bool  movie_transit::get_formated_current_frame_raster(
		 irb_frame_image_dispatcher::irb_frame_raster_ptr_t raster,
		 irb_frame_image_dispatcher::temperature_span_t & calibration_interval
		 )
	 {
		 return _p_impl->TVcrack.get_formated_current_frame_raster(raster, calibration_interval);
	 }
	 bool  movie_transit::get_formated_frame_raster(const ::irb_frame_shared_ptr_t & frame,
		 irb_frame_image_dispatcher::irb_frame_raster_ptr_t raster,
		 irb_frame_image_dispatcher::temperature_span_t & calibration_interval)
	 {
		 return _p_impl->TVcrack.get_formated_frame_raster(frame, raster, calibration_interval);
	 }







	BOOL movie_transit::Open()
	{
		return _p_impl->TVcrack.Open();
	}
	void movie_transit::Close()
	{
		_p_impl->TVcrack.Close();
	}
	BOOL movie_transit::IsOpen()
	{
		return _p_impl->TVcrack.IsOpen();
	}
	DWORD movie_transit::NumFilter()
	{
		return _p_impl->TVcrack.NumFilter();
	}
	CTime movie_transit::GetTime()
	{
		return _p_impl->TVcrack.GetTime();
	}
	CTime movie_transit::GetStartTime()
	{
		return _p_impl->TVcrack.GetStartTime();
	}
	CTime movie_transit::GetLastTime()
	{
		return _p_impl->TVcrack.GetLastTime();
	}

	DWORD movie_transit::Go_to_frame_by_id(DWORD N, int filter)
	{
		return _p_impl->TVcrack.Go_to_frame_by_id(N, filter);
	}
	DWORD movie_transit::Go_to_frame_by_index(DWORD N, int filter)
	{
		return _p_impl->TVcrack.Go_to_frame_by_index(N, filter);
	}

	DWORD movie_transit::Find(DWORD N, int filter)
	{
		return _p_impl->TVcrack.Find(N, filter);
	}


	BOOL movie_transit::GetMetka(char *metka )
	{
		return _p_impl->TVcrack.GetMetka(metka);
	}
	BOOL movie_transit::SetMetka(char metka)
	{
		return _p_impl->TVcrack.SetMetka(metka);
	}
	BOOL movie_transit::SetMetka(DWORD N, char metka)
	{
		return _p_impl->TVcrack.SetMetka(N, metka);
	}
	int movie_transit::FindMetka(int startindex, int dir)
	{
		return _p_impl->TVcrack.FindMetka(startindex, dir);
	}
	int movie_transit::NextMetka()
	{
		return _p_impl->TVcrack.NextMetka();
	}

	int movie_transit::PrevMetka()
	{
		return _p_impl->TVcrack.PrevMetka();
	}


	BOOL movie_transit::SaveCurr(char *fname)
	{
		return _p_impl->TVcrack.SaveCurr(fname);
	}
	BOOL movie_transit::SaveFilter(int & p, char *fname, char *filePrefix )
	{
		return _p_impl->TVcrack.SaveFilter(p, fname, filePrefix);
	}

	BOOL movie_transit::IsFilterYes(IRBFrame *frame)
	{
		return _p_impl->TVcrack.IsFilterYes(frame);
	}
	BOOL movie_transit::IsFilterYes()
	{
		return _p_impl->TVcrack.IsFilterYes();
	}
	BOOL movie_transit::IsFilterYes(DWORD index)
	{
		return _p_impl->TVcrack.IsFilterYes(index);
	}
	void movie_transit::CurFrameTemperaturesCompute(void)
	{
		_p_impl->TVcrack.CurFrameTemperaturesCompute();
	}
	void movie_transit::FilterFrames(FILTER& filter)
	{
		_p_impl->TVcrack.FilterFrames(filter);
	}

	bool movie_transit::AddAreaRect(const AreaRect& area)
	{
		return _p_impl->TVcrack.AddAreaRect(area);
	}
	bool movie_transit::AddAreaEllips(const AreaEllips& area)
	{
		return _p_impl->TVcrack.AddAreaEllips(area);
	}
	void movie_transit::DelArea(SHORT id)
	{
		_p_impl->TVcrack.DelArea(id);
	}

	void movie_transit::ChangeRectArea(SHORT id, const AreaRect &area)
	{
		_p_impl->TVcrack.ChangeRectArea(id,area);
	}
	void movie_transit::ChangeEllipsArea(SHORT id, const AreaEllips &area)
	{
		_p_impl->TVcrack.ChangeEllipsArea(id,area);
	}

}