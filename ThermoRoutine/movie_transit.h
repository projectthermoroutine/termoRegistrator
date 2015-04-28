#pragma once
#include <Windows.h>
#include <memory>
#include "metro_map.h"
#include "tvcrack.h"
#include "irb_frame_helper.h"
#include "irb_file_helper.h"

namespace movie_transit_ns
{
	class movie_transit final
	{
	public:
		movie_transit(const metro_map::metro_map_ptr_t & metro_map);
		~movie_transit();

	public:
		coordinate_t ReadKm(DWORD frameNum);  // прочитать километраж
		coordinate_t GetKm();  // возвращает км дл€ текущего файла
		time_t ReadTime(DWORD N);  // прочитать врем€

	public:

		BOOL Open();
		void Close();            // закрыть файл 
		BOOL IsOpen();
		// обща€ информаци€ о файле
		DWORD NumFilter(); // число кадров с учетом фильрации

		CTime GetTime();
		CTime GetStartTime();
		CTime GetLastTime();

		DWORD Go_to_frame_by_id(DWORD N, int filter);       // переместитьс€ на кадр є N
		DWORD Go_to_frame_by_index(DWORD N, int filter);       // переместитьс€ на кадр є N
		DWORD Find(coordinate_t km);     // найти  кадр с рассто€нием km или ближайший
		DWORD Find(DWORD N, int filter);    // найти кадр с учетом фильтрации


		// палитра
		BOOL LoadPalette(char *fname);

		BOOL GetMetka(char *metka = NULL);  // возвращает метку дл€ текущего кадра
		BOOL SetMetka(char metka);  // устанавливает метку дл€ кадра; ' '==сн€ть метку
		BOOL SetMetka(DWORD N, char metka);
		int FindMetka(int startindex, int dir);  // поиск метки
		int NextMetka();

		int PrevMetka();

		BOOL SaveCurr(char *fname);    // сохранить текущий кадр
		BOOL SaveFilter(int & p, char *fname, char *filePrefix = "");  // сохранить с учетом фильтрации

		// ‘»Ћ№“–ј÷»я
		//   BOOL WriteFilter(DWORD index, BOOL flag);  // запись результата фильтра
		BOOL IsFilterYes(IRBFrame *frame);
		BOOL IsFilterYes();
		BOOL IsFilterYes(DWORD index);
		void SetFilter(const FILTER &f);

		void CurFrameTemperaturesCompute(void);
		void FilterFrames(FILTER& filter);

		bool AddAreaRect(const AreaRect & area);
		bool AddAreaEllips(const AreaEllips &area);
		void DelArea(SHORT id);

		void ChangeRectArea(SHORT id, const AreaRect &area);
		void ChangeEllipsArea(SHORT id, const AreaEllips &area);

public:
	::irb_frame_shared_ptr_t get_frame_by_id(DWORD num);
	::irb_frame_shared_ptr_t get_frame_by_coordinate(coordinate_t coordinate);
	::irb_frame_shared_ptr_t get_frame_by_time(double time);
	::irb_frame_shared_ptr_t get_frame_by_index(uint32_t index);



	::irb_frame_shared_ptr_t read_frame_by_id(DWORD num);
	int get_irb_file_index_by_frame_id(DWORD num);

	bool save_frame(uint32_t index, uint32_t picket, uint32_t offset, const std::string & fname);
	bool SaveFrames(const std::vector<::irb_frame_shared_ptr_t> & frames, const std::string & fname);

	DWORD get_last_irb_frame_id();
	DWORD get_first_irb_frame_id();
	DWORD get_irb_frame_id_by_index(DWORD index);
	LONG get_irb_frame_index_by_id(DWORD id);


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
		int number_frames_in_files();

		const IRBFrame * current_irb_frame();


		bool set_palette(const char * pallete_file_name);
		void set_default_palette();

		bool get_area_temperature_measure(int area_id, area_temperature_measure &measure);

		void remove_all_areas();

		void set_auto_palette_mode(bool mode = true);
		int get_palette_size();
		uint32_t get_palette_colors_number();
		const void * get_palette_image();
		const TVpalette& get_palette();
		void set_palette_calibration_mode(calibration_mode mode);
		void set_palette_calibration(float min, float max);


		void mark_frame_in_filter(uint32_t frame_index, bool state);
		uint32_t get_filter_flags();
		void get_filter_params(FILTER &f);

		void clear_cache();

	private:
		struct Impl;
		std::unique_ptr<Impl> _p_impl;
	};
}
