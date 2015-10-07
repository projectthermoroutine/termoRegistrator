#pragma once
#include <Windows.h>
#include <memory>
#include "tvcrack.h"
#include "irb_frame_helper.h"
#include "irb_file_helper.h"
#include "irb_frame_filter.h"

namespace movie_transit_ns
{
	class movie_transit_exception : public std::runtime_error
	{
	public:
		movie_transit_exception(HRESULT error_code, const std::string & message);
		const char * what() const override;
		HRESULT get_error_code() const;
	private:
		std::string _message;
		HRESULT _error_code;
	};




	class movie_transit final
	{
	public:
		movie_transit();
		~movie_transit();

	public:

		BOOL Open();
		void Close();            // закрыть файл 
		BOOL IsOpen();
		// обща€ информаци€ о файле
		DWORD NumFilter(); // число кадров с учетом фильрации

		double GetStartTime();
		double GetLastTime();

		DWORD Go_to_frame_by_index(DWORD N, FILTER_SEARCH_TYPE filter);       // переместитьс€ на кадр є N
		DWORD Find(coordinate_t km);     // найти  кадр с рассто€нием km или ближайший
		DWORD Find(DWORD N, FILTER_SEARCH_TYPE filter);    // найти кадр с учетом фильтрации


		// палитра
		BOOL LoadPalette(char *fname);

		BOOL GetMetka();  // возвращает метку дл€ текущего кадра
		BOOL SetMetka(char metka);  // устанавливает метку дл€ кадра; ' '==сн€ть метку
		BOOL SetMetka(DWORD N, char metka);
		int FindMetka(int startindex, int dir);  // поиск метки
		int NextMetka();

		int PrevMetka();

		BOOL SaveCurr(wchar_t *fname);    // сохранить текущий кадр
		BOOL SaveFilter(int & p, wchar_t *fname, wchar_t *filePrefix = L"");  // сохранить с учетом фильтрации

		// ‘»Ћ№“–ј÷»я
		//   BOOL WriteFilter(DWORD index, BOOL flag);  // запись результата фильтра
		BOOL IsFilterYes(const ::irb_frame_shared_ptr_t &frame, const irb_frame_filter::FILTER &frame_filter);
		//bool IsFilterYes();
		BOOL IsFilterYes(DWORD index);
		void SetFilter(irb_frame_filter::FILTER &f);

		void CurFrameTemperaturesCompute(void);
		void FilterFrames(irb_frame_filter::FILTER& filter);

		bool AddAreaRect(const AreaRect & area);
		bool AddAreaEllips(const AreaEllips &area);
		void DelArea(SHORT id);

		void ChangeRectArea(SHORT id, const AreaRect &area);
		void ChangeEllipsArea(SHORT id, const AreaEllips &area);

		const areas_dispatcher& areas_dispatcher() const;

public:
	::irb_frame_shared_ptr_t get_frame_by_index(uint32_t index);



	bool save_frame(uint32_t index, const std::string & device_name, uint32_t picket, uint32_t offset, const std::wstring & fname);
	bool SaveFrames(const std::vector<::irb_frame_shared_ptr_t> & frames, const std::wstring & fname, uint16_t frames_per_file);
	bool SaveFrames(const std::vector<uint32_t> & frames_indexes, const std::wstring & fname, uint16_t frames_per_file);

	frame_id_t get_irb_frame_id_by_index(DWORD index);

	bool get_formated_frame_raster_by_index(DWORD N,
		irb_frame_image_dispatcher::irb_frame_raster_ptr_t raster,
		irb_frame_image_dispatcher::temperature_span_t & calibration_interval);
	bool get_formated_current_frame_raster(
		irb_frame_image_dispatcher::irb_frame_raster_ptr_t raster,
		irb_frame_image_dispatcher::temperature_span_t & calibration_interval);
	bool get_formated_frame_raster(const ::irb_frame_shared_ptr_t & frame,
		irb_frame_image_dispatcher::irb_frame_raster_ptr_t raster,
		irb_frame_image_dispatcher::temperature_span_t & calibration_interval);

	public:
		int number_irb_files();
		void reset(bool save_filter = false);
		void add_irb_files(irb_files_list_t& files_list, bool try_inherit_filter = false);

		void write_camera_offset(int32_t offset);

		int number_frames_in_files();

		::irb_frame_shared_ptr_t current_irb_frame();


		bool set_palette(const char * pallete_file_name);
		void set_default_palette();

		bool get_area_temperature_measure(int area_id, area_temperature_measure &measure);

		void remove_all_areas();

		int get_palette_size();
		uint32_t get_palette_colors_number();
		const void * get_palette_image();
		const tv_helper::TVpalette& get_palette();
		void set_palette_calibration_mode(calibration_mode mode);
		void set_palette_calibration(float min, float max);


		void mark_frame_in_filter(uint32_t frame_index, bool state);
		uint32_t get_filter_flags();
		void get_filter_params(irb_frame_filter::FILTER &f);

		void clear_cache();

	private:
		struct Impl;
		std::unique_ptr<Impl> _p_impl;
	};
}
