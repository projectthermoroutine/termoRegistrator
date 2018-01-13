#include <Windows.h>

#include <vector>
#include <memory>
#include <algorithm>

#include "movie_transit.h"

#include "tvcrack.h"

#include "irb_frame_helper.h"
//#include "irb_file_helper.h"

//#include "irb_frame_spec_info.h"
#include "irb_frame_manager.h"
#include "irb_frame_filter.h"


#ifdef _WINGDI_
#undef _WINGDI_
#endif

namespace movie_transit_ns
{

	movie_transit_exception::movie_transit_exception(HRESULT error_code, const std::string & message) :
		std::runtime_error(message), _error_code(error_code)
	{
		std::ostringstream ss;
		ss << message << " Error: " << std::hex << std::showbase << error_code;
		_message = ss.str();
	}

	const char * movie_transit_exception::what() const
	{
		return _message.c_str();
	}

	HRESULT movie_transit_exception::get_error_code() const
	{
		return _error_code;
	}



	struct movie_transit::Impl
	{
		CTVcrack<> TVcrack;
		irb_frame_image_dispatcher::image_dispatcher _image_dispatcher;
		irb_frame_filter::FILTER _frame_filter;
	};


	movie_transit::movie_transit() :_p_impl(std::make_unique<Impl>())
	{
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

	std::vector<char> movie_transit::get_frame_raw_data_by_index(uint32_t index)
	{
		auto frame = _p_impl->TVcrack.get_frame_by_index(index);
		if (!frame)
			return std::vector<char>();
		return irb_frame_helper::get_frame_raw_data(*frame);
	}

	bool movie_transit::SaveFrames(const std::vector<::irb_frame_shared_ptr_t> & frames, const std::wstring & fname, uint16_t frames_per_file)
	{
		return irb_frame_manager::save_frames(frames, fname, frames_per_file);
	}
	
	bool movie_transit::SaveFrames(const std::vector<uint32_t> & frames_indexes, const std::wstring & fname, uint16_t frames_per_file)
	{
		return irb_frame_manager::save_frames(frames_indexes, 
											std::bind(&CTVcrack<>::get_frame_by_index, &_p_impl->TVcrack,std::placeholders::_1),
											fname, 
											frames_per_file
											);
	}

	bool movie_transit::save_frame(uint32_t index,const std::string & device_name, int32_t picket, int32_t offset, const std::wstring & fname)
	{
		irb_frame_spec_info::irb_frame_position_info position_info{ picket, offset };

		auto const frame = _p_impl->TVcrack.get_frame_by_index(index);
		return irb_frame_manager::save_frame(frame, device_name, position_info, fname);
	}

	bool movie_transit::save_frame(uint32_t index, const std::wstring & fname)
	{
		auto const frame = _p_impl->TVcrack.get_frame_by_index(index);
		return irb_frame_manager::save_frame(frame, fname);
	}

	frame_id_t movie_transit::get_irb_frame_id_by_index(DWORD index)
	{
		return _p_impl->TVcrack.get_irb_frame_id_by_index(index);
	}


	void movie_transit::reset(bool save_filter)
	{
		_p_impl->TVcrack.reset(save_filter);
	}

	void movie_transit::add_irb_files(irb_files_list_t& files_list, bool try_inherit_filter)
	{
		_p_impl->TVcrack.set_irb_files(files_list, try_inherit_filter);
	}

	void movie_transit::write_camera_offset(int32_t offset)
	{
		_p_impl->TVcrack.write_camera_offset(offset);
	}

	int movie_transit::number_irb_files()
	{
		return _p_impl->TVcrack.number_irb_files();
	}


	int movie_transit::number_frames_in_files()
	{
		return _p_impl->TVcrack.number_frames_in_files();
	}

	::irb_frame_shared_ptr_t movie_transit::current_irb_frame()
	{
		 return _p_impl->TVcrack.get_current_frame();
	}


	 bool movie_transit::set_palette(const std::wstring & pallete_file_name)
	 {
		 return _p_impl->_image_dispatcher.set_palette(pallete_file_name);
	 }


	 void movie_transit::set_default_palette()
	 {
		 _p_impl->_image_dispatcher.set_default_palette();
	 }


	 bool movie_transit::get_area_temperature_measure(int area_id, area_temperature_measure &measure)
	 {
		 return _p_impl->_image_dispatcher.get_area_temperature_measure(area_id, measure);
	 }

	 void movie_transit::remove_all_areas()
	 {
		 _p_impl->_image_dispatcher.clear_areas();;
	 }

	 int movie_transit::get_palette_size()
	 {
		 return _p_impl->_image_dispatcher.get_palette_size();
	 }
	 uint32_t movie_transit::get_palette_colors_number()
	 {
		 return _p_impl->_image_dispatcher.get_palette().numI;
	 }

	 const void * movie_transit::get_palette_image()
	 {
		 return _p_impl->_image_dispatcher.get_palette_image();
	 }

	 const tv_helper::TVpalette& movie_transit::get_palette()
	 {
		 return _p_impl->_image_dispatcher.get_palette();
	 }

	 void movie_transit::set_palette_calibration_mode(calibration_mode mode)
	 {
		 _p_impl->_image_dispatcher.set_calibration_type(static_cast<irb_frame_image_dispatcher::IMAGE_CALIBRATION_TYPE>(mode));
	 }
	 void movie_transit::set_palette_calibration(float min, float max)
	 {
		 _p_impl->_image_dispatcher.set_calibration_interval(min,max);
	 }


	 void movie_transit::mark_frame_in_filter(uint32_t frame_index, bool state)
	 {
		 _p_impl->TVcrack.mark_frame_in_filter(frame_index, state);
	 }
	 void movie_transit::get_filter_params(irb_frame_filter::FILTER &f)
	 {
		 f = _p_impl->_frame_filter;
	 }
	 uint32_t movie_transit::get_filter_flags()
	 {
		 return _p_impl->_frame_filter.flags;
	 }
	 void movie_transit::SetFilter(irb_frame_filter::FILTER &f)
	 {
		 _p_impl->_frame_filter = std::move(f);
		 _p_impl->_frame_filter.areas = std::bind(&irb_frame_image_dispatcher::image_dispatcher::areas_dispatcher, &_p_impl->_image_dispatcher);
	 }

	 bool  movie_transit::get_formated_frame_raster_by_index(DWORD N,
		 irb_frame_image_dispatcher::irb_frame_raster_ptr_t raster,
		 irb_frame_image_dispatcher::temperature_span_t & calibration_interval)
	 {
		 auto const frame = _p_impl->TVcrack.get_frame_by_index(N);
#ifdef USE_PPL
		 return _p_impl->_image_dispatcher.get_formated_frame_raster_parallel(frame, raster, calibration_interval);
#else
		 return _p_impl->_image_dispatcher.get_formated_frame_raster(frame, raster, calibration_interval);
#endif

	 }

	 bool  movie_transit::get_formated_current_frame_raster(
		 irb_frame_image_dispatcher::irb_frame_raster_ptr_t raster,
		 irb_frame_image_dispatcher::temperature_span_t & calibration_interval
		 )
	 {
		 auto const frame = _p_impl->TVcrack.get_current_frame();
#ifdef USE_PPL
		 return _p_impl->_image_dispatcher.get_formated_frame_raster_parallel(frame, raster, calibration_interval);
#else
		 return _p_impl->_image_dispatcher.get_formated_frame_raster(frame, raster, calibration_interval);
#endif
	 }

	 bool  movie_transit::get_formated_frame_raster(const ::irb_frame_shared_ptr_t & frame,
		 irb_frame_image_dispatcher::irb_frame_raster_ptr_t raster,
		 irb_frame_image_dispatcher::temperature_span_t & calibration_interval)
	 {
#ifdef USE_PPL
		 return _p_impl->_image_dispatcher.get_formated_frame_raster_parallel(frame, raster, calibration_interval);
#else
		 return _p_impl->_image_dispatcher.get_formated_frame_raster(frame, raster, calibration_interval);
#endif
	 }


	BOOL movie_transit::Open()
	{
		auto result = _p_impl->TVcrack.Open();

		if (!result)
			return FALSE;

		auto const cur_frame = _p_impl->TVcrack.get_current_frame();
		uint16_t width = 1024;
		uint16_t height = 768;
		if (cur_frame){
			width = cur_frame->header.geometry.imgWidth;
			height = cur_frame->header.geometry.imgHeight;

		}

		_p_impl->_image_dispatcher.set_areas_mask_size(width, height);

		return result;

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
	double movie_transit::GetStartTime()
	{
		return _p_impl->TVcrack.GetStartTime();
	}
	double movie_transit::GetLastTime()
	{
		return _p_impl->TVcrack.GetLastTime();
	}

	DWORD movie_transit::Go_to_frame_by_index(DWORD N, FILTER_SEARCH_TYPE filter)
	{
		LOG_STACK();
		return _p_impl->TVcrack.Go_to_frame_by_index(N, filter);
	}

	DWORD movie_transit::Find(DWORD N, FILTER_SEARCH_TYPE filter)
	{
		return _p_impl->TVcrack.Find(N, filter);
	}


	BOOL movie_transit::GetMetka()
	{
		auto frame = _p_impl->TVcrack.get_current_frame();
		if (frame)
			return frame->marked();
		return false;

	}
	BOOL movie_transit::SetMetka(char metka)
	{
		auto frame = _p_impl->TVcrack.get_current_frame();
		if (!frame)
			return false;

		bool state = true;
		if (metka == ' ')
			state = false;
		frame->set_marked(state);

		return _p_impl->TVcrack.FlushFrame(_p_impl->TVcrack.get_current_frame_index(), frame);
	}
	BOOL movie_transit::SetMetka(DWORD N, char metka)
	{
		auto frame = _p_impl->TVcrack.get_frame_by_index(N);
		if (!frame)
			return false;

		bool state = true;
		if (metka == ' ')
			state = false;
		frame->set_marked(state);

		return _p_impl->TVcrack.FlushFrame(N,frame);
	}
	int movie_transit::FindMetka(int startindex, int dir)
	{
		auto number_all_frames = _p_impl->TVcrack.number_frames_in_files();
		for (int i = startindex;; i += dir)
		{
			if (i < 0 || i >= (int)number_all_frames)
				return -1;
			auto frame = _p_impl->TVcrack.get_frame_by_index(i);
			if (frame)
				return frame->id;
		}
		__assume(false);
	}
	int movie_transit::NextMetka()
	{
		return FindMetka(_p_impl->TVcrack.get_current_frame_index() + 1, 1);
	}

	int movie_transit::PrevMetka()
	{
		return FindMetka(_p_impl->TVcrack.get_current_frame_index() - 1, -1);
	}


	BOOL movie_transit::SaveCurr(wchar_t *fname)
	{
		irb_frame_spec_info::irb_frame_position_info position_info{ 0, 0 };

		auto const frame = _p_impl->TVcrack.get_current_frame();
		return irb_frame_manager::save_frame(frame,"", position_info, fname);
	}
	BOOL movie_transit::SaveFilter(int & p, wchar_t *fname, wchar_t *filePrefix)
	{
		return _p_impl->TVcrack.SaveFilter(p, fname, filePrefix);
	}

	BOOL movie_transit::IsFilterYes(const ::irb_frame_shared_ptr_t &frame, const irb_frame_filter::FILTER &frame_filter)
	{
		return irb_frame_filter::frame_filtered<::irb_frame_shared_ptr_t>(frame_filter, frame);
	}

	//bool movie_transit::IsFilterYes()
	//{
	//	return _p_impl->TVcrack.IsFilterYes();
	//}

	BOOL movie_transit::IsFilterYes(DWORD index)
	{
		auto frame = _p_impl->TVcrack.get_frame_by_index(index);
		if (!frame)
			return false;
		return IsFilterYes(frame, _p_impl->_frame_filter);
	}
	void movie_transit::CurFrameTemperaturesCompute(void)
	{
		if (!_p_impl->TVcrack.IsOpen())
			return;

		auto const frame = _p_impl->TVcrack.get_current_frame();
		if (frame){
			_p_impl->_image_dispatcher.calculate_average_temperature(frame);
		}

	}
	void movie_transit::FilterFrames(irb_frame_filter::FILTER& filter)
	{
		filter.areas = std::bind(&irb_frame_image_dispatcher::image_dispatcher::areas_dispatcher, &_p_impl->_image_dispatcher);
		_p_impl->TVcrack.FilterFrames(filter);
	}

	bool movie_transit::AddAreaRect(const AreaRect& area)
	{
		return _p_impl->_image_dispatcher.AddAreaRect(area);
	}
	bool movie_transit::AddAreaEllips(const AreaEllips& area)
	{
		return _p_impl->_image_dispatcher.AddAreaEllips(area);
	}
	void movie_transit::DelArea(SHORT id)
	{
		_p_impl->_image_dispatcher.DelArea(id);
	}

	void movie_transit::ChangeRectArea(SHORT id, const AreaRect &area)
	{
		_p_impl->_image_dispatcher.ChangeRectArea(id,area);
	}
	void movie_transit::ChangeEllipsArea(SHORT id, const AreaEllips &area)
	{
		_p_impl->_image_dispatcher.ChangeEllipsArea(id,area);
	}

	const areas_dispatcher& movie_transit::areas_dispatcher() const
	{
		return _p_impl->_image_dispatcher.areas_dispatcher();
	}

	irb_frame_image_dispatcher::image_dispatcher& movie_transit::image_dispatcher() const
	{
		return _p_impl->_image_dispatcher;
	}
	
}