#pragma once
#include <Windows.h>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <map>
#include <common\handle_holder.h>
#include "AreaBase.h"

#include "metro_map.h"
#include "tv_helper.h"

#include "irb_frame_helper.h"
#include "irb_file_helper.h"

#include <common\sync_helpers.h>
#include <list>
#include "irb_frame_image_dispatcher.h"

//#include "structures.h"

#ifdef _WINGDI_
#undef _WINGDI_
#endif
#include <atltypes.h>
#include <atltime.h>
#include "ThermoRoutine_i.h"



// -----------------------------------------------------------
// -----------------------------------------------------------
//  ласс дл€ работы с файлами, записанными тепловизором
// -----------------------------------------------------------
// -----------------------------------------------------------

#define TIME_POPRAVKA  3600*4  // это вычитаетс€ из времени тепловизора

// услови€ фильтрации (битовые флаги)
#define  FILTER_TIME0      0x01   // начальное врем€
#define  FILTER_TIME1      0x02   // конечное врем€
#define  FILTER_METKA      0x04   // метка
#define  FILTER_TEMPFULL   0x08   // max отклонение температуры от среднего по кадру
#define  FILTER_TEMPREGION 0x10   // max отклонение температуры от среднего по области
#define  FILTER_PICKMIN    0x20   // минимальный пикет
#define  FILTER_PICKMAX    0x40   // максимальный пикет
         
// услови€ поиска с учетом фильтрации
enum FILTER_SEARCH_TYPE{
	FILTER_NO,      // без учета фильтрации
	FILTER_PLUS,    // поиск вперед
	FILTER_MINUS,   // поиск назад
	FILTER_NEAREST // поиск ближайшей
};

// услови€ фильтрации

struct FILTER 
{
private:
	static const int max_count_items = 2;
public:
	float time[max_count_items];
	float T[max_count_items];
	int pick[max_count_items];
	BYTE metka;
	DWORD flags;
	std::unique_ptr<BYTE[]> filter;
	uint32_t num;     // число элементов в filter

public:
	FILTER() :metka(0), flags(0), num(0)
	{
		for (int i = 0; i < max_count_items; i++) {
			time[i] = 0;T[i] = 0;pick[i] = 0;
		}
	}

	FILTER(const FILTER &f)
	{
		copy(f);
		if (f.num > 0){
			Open(f.num);
			memcpy(filter.get(), f.filter.get(), num);
		}
	}

	FILTER(FILTER &&f)
	{
		copy(f);
		filter.swap(f.filter);
	}
	FILTER & operator = (const FILTER & f)
	{
		copy(f);
		if (f.num > 0){
			Open(f.num);
			memcpy(filter.get(), f.filter.get(), num);
		}
		return *this;
	}
	FILTER & operator = (FILTER && f)
	{
		copy(f);
		filter.swap(f.filter);
		return *this;
	}

	void Reset()
	{
		num = 0;
		filter.reset();
	}

	void copy_params(const FILTER & f)
	{
		auto save_num = num;
		copy(f);
		num = save_num;
	}
private:
	void copy(const FILTER & f)
	{
		for (int i = 0; i < max_count_items; i++) {
			time[i] = f.time[i];
			T[i] = f.T[i];
			pick[i] = f.pick[i];
		}
		metka = f.metka;
		flags = f.flags;
		num = f.num;
	}
public:

	void Open(DWORD n)
	{
		if (n == 0)
			throw std::invalid_argument("zero were passed as argument n to the method Open FILTER object");
		num = n;
		filter = std::make_unique<BYTE[]>(num);
		for (DWORD i = 0; i < num; i++)
			filter[i] = 1;
	}

	DWORD CalcNum()
	{  // сколько кадров удовлетвор€ет услови€м фильтрации
		if (!filter) return 0;
		DWORD i = 0, n = 0;
		for (; i < num; i++) if (filter[i]) n++;
		return n;
	}

	inline uint32_t size() const { return num; }
};

COLORREF Color(float val, const tv_helper::TVpalette& pallete, const tv_helper::TVdiap& diap);  // каким цветом рисуем это значение

using namespace irb_file_helper;
using namespace irb_frame_helper;

using irb_file_t = std::shared_ptr<IRBFile>;
using irb_files_list_t = std::vector<irb_file_t>;
using irb_frame_ptr_t = std::unique_ptr<IRBFrame>;
using irb_frame_shared_ptr_t = std::shared_ptr<IRBFrame>;

//using area_ptr_t = std::unique_ptr<AreaBase>;
using area_ptr_t = std::shared_ptr<AreaBase>;

using namespace metro_map;
using namespace tv_helper;

// -----------------------------------------------------------
// основной класс дл€ работы с файлом
// -----------------------------------------------------------
class CTVcrack final
{
	typedef struct _cache_irb_frames_item
	{
		_cache_irb_frames_item(uint32_t id, coordinate_t coordinate, double time, const ::irb_frame_shared_ptr_t & p_frame)
		{
			key.id = id;
			key.coordinate = coordinate;
			key.time = time;
			frame = p_frame;
		}
		irb_frame_key key;
		::irb_frame_shared_ptr_t frame;
	} cache_irb_frames_item;

	using irb_frames_cache_t = std::list<cache_irb_frames_item>;

public:
	CTVcrack();
	~CTVcrack();
private:
	std::mutex _lock_mtx;
	FILTER m_filter;          // услови€ и результаты фильтрации 
public:
	uint32_t get_filter_flags() const { return m_filter.flags; }
	void mark_frame_in_filter(uint32_t frame_index,bool state);
	void get_filter_params(FILTER &f) const { f.copy_params(m_filter); }
public:
	void reset(bool save_filter = false);
	void set_irb_files(irb_files_list_t& files, bool try_inherit_filter = false);

	::irb_frame_shared_ptr_t get_frame_by_index(uint32_t index);

	::irb_frame_shared_ptr_t get_frame_by_id(uint32_t id);
	::irb_frame_shared_ptr_t get_frame_by_coordinate(coordinate_t coordinate);
	::irb_frame_shared_ptr_t get_frame_by_time(double time);

	::irb_frame_shared_ptr_t read_frame_by_id(uint32_t id);
	int get_irb_file_index_by_frame_id(uint32_t id);
	int get_irb_file_index_by_frame_coordinate(coordinate_t coordinate);
	int get_irb_file_index_by_frame_time(double time);


	bool SaveFrames(const std::vector<::irb_frame_shared_ptr_t> & frames, const std::string & fname);

	DWORD get_last_irb_frame_id();
	DWORD get_first_irb_frame_id();
	DWORD get_irb_frame_id_by_index(uint32_t id);
	LONG get_irb_frame_index_by_id(uint32_t index);

	inline const ::irb_frame_shared_ptr_t & get_current_frame() const { return m_curframe; }

	inline int number_irb_files() const { return (int)files.size(); };

	void clear_cache() { _cached_irb_frames.clear(); }
private:
	VARIANT varres;

	float m_scaleTop;
	float m_scaleBottom;
	bool m_needToCalcDiap;

private:
	irb_files_list_t files; // указатели IRBFile - файлы
	std::vector<irb_frames_key_list_t> _lists_frames_key;
	std::vector<irb_frames_key_span_t> _map_frames_key_spans_to_file_index;
	uint32_t _number_all_frames;


	::irb_frame_shared_ptr_t m_curframe;
	irb_frames_cache_t _cached_irb_frames;
	uint16_t _max_number_cached_frames;
	DWORD _cur_frame_index;
	DWORD _cur_frame_id;

	sync_helpers::rw_lock _lock_rw;

	irb_frame_image_dispatcher::image_dispatcher _image_dispatcher;

private:
	template<typename TKey>
	int get_irb_file_index_by_key_item(TKey item, TKey(*get_key_item_id_func)(const irb_frame_key&));

public:

	CTime  m_time[2];          // врем€ начала и окончани€ записи
	coordinate_t _last_coordinate;      // сколько всего км проехали
	time_t m_msec0;             // миллисекунды 1-го кадра
	time_t m_sec0;			  // секунды 1-го кадра

public:
	bool set_palette(const char * pallete_file_name) { return _image_dispatcher.set_palette(pallete_file_name); }
	void set_default_palette() { _image_dispatcher.set_default_palette(); }
	uint32_t get_palette_colors_number() { return _image_dispatcher.get_palette().numI; }
	const void * get_palette_image() { return _image_dispatcher.get_palette_image(); }
	const TVpalette& get_palette(){ return _image_dispatcher.get_palette(); }
	int get_palette_size() { return _image_dispatcher.get_palette_size(); }
	void set_palette_calibration_mode(calibration_mode mode) { _image_dispatcher.set_calibration_type(static_cast<irb_frame_image_dispatcher::IMAGE_CALIBRATION_TYPE>(mode)); }
	void set_palette_calibration(float min, float max) { _image_dispatcher.set_calibration_interval(min,max); }


public:
	TVcalibr m_cal;     // калибровка

public:
	coordinate_t ReadKm(DWORD frameNum);  // прочитать километраж
	coordinate_t GetKm();  // возвращает км дл€ текущего файла
	time_t ReadTime(DWORD N);  // прочитать врем€
	BOOL m_fAutoPal;           // признак разрешени€ автоподстройки палитры
public:
	BOOL Open();
	void Close();            // закрыть файл 
	inline bool IsOpen() const { return !files.empty(); }

	// обща€ информаци€ о файле
	DWORD count_frames_in_files();             // сколько кадров в файлах
	DWORD NumFilter() { return m_filter.CalcNum(); }  // число кадров с учетом фильрации
	inline time_t Msec0() { return m_msec0; }
	inline time_t Msec() { return m_curframe->Msec() - m_msec0; }
	time_t Msec(char *str, int znak = 3);

	inline CTime GetTime() const
	{
		if (!IsOpen()) 
			return CTime::GetCurrentTime();
		return CTime(convert_irb_frame_time_in_sec(m_curframe->get_frame_time_in_sec()) - TIME_POPRAVKA);
	}
	inline CTime GetStartTime() const
	{
		if (!IsOpen()) return CTime::GetCurrentTime();
		return CTime(m_time[0].GetTime() - TIME_POPRAVKA);
	}

	inline CTime GetLastTime() const
	{
		if (!IsOpen()) return CTime::GetCurrentTime();
		return CTime(m_time[1].GetTime() - TIME_POPRAVKA);
	}

	bool get_formated_frame_raster_by_index(DWORD N,
		irb_frame_image_dispatcher::irb_frame_raster_ptr_t raster,
		irb_frame_image_dispatcher::temperature_span_t & calibration_interval
		);
	bool get_formated_current_frame_raster(
		irb_frame_image_dispatcher::irb_frame_raster_ptr_t raster,
		irb_frame_image_dispatcher::temperature_span_t & calibration_interval
		);
	bool get_formated_frame_raster(const ::irb_frame_shared_ptr_t & frame,
		irb_frame_image_dispatcher::irb_frame_raster_ptr_t raster,
		irb_frame_image_dispatcher::temperature_span_t & calibration_interval
		);

	DWORD Go_to_frame_by_id(DWORD N, int filter);       // переместитьс€ на кадр є N
	DWORD Go_to_frame_by_index(DWORD N, int filter);       // переместитьс€ на кадр є N
	LONG Find(DWORD N, int filter);    // найти кадр с учетом фильтрации

	BOOL GetMetka(char *metka = NULL);  // возвращает метку дл€ текущего кадра
	BOOL SetMetka(char metka);  // устанавливает метку дл€ кадра; ' '==сн€ть метку
	BOOL SetMetka(DWORD N, char metka);
	int FindMetka(int startindex, int dir);  // поиск метки
	inline int NextMetka()
	{
		return FindMetka(_cur_frame_index + 1, 1);
	}

	inline int PrevMetka()
	{
		if (!_cur_frame_index)
			return -1;
		return FindMetka(_cur_frame_index - 1, -1);
	}
	BOOL SaveCurr(const std::string & fname);    // сохранить текущий кадр
	BOOL SaveFrame(IRBFrame *frame, const std::string & fname);
	BOOL SaveFilter(int & p, const std::string &fname, const std::string &filePrefix);  // сохранить с учетом фильтрации

	// ‘»Ћ№“–ј÷»я
	//   BOOL WriteFilter(DWORD index, BOOL flag);  // запись результата фильтра
	BOOL IsFilterYes(const IRBFrame *frame);
	BOOL IsFilterYes();
	BOOL IsFilterYes(DWORD index);
	void SetFilter(const FILTER &f);  // установить фильтр

	void CurFrameTemperaturesCompute(void);
	void FilterFrames(FILTER& filter);

public:

	bool get_area_temperature_measure(int area_id,area_temperature_measure &measure)
	{
		return _image_dispatcher.get_area_temperature_measure(area_id, measure);
	}
	void clear_areas() { _image_dispatcher.clear_areas(); }

	void AddAreaRect(const AreaRect &area) { return _image_dispatcher.AddAreaRect(area); }
	void AddAreaEllips(const AreaEllips &area){ return _image_dispatcher.AddAreaEllips(area); }
	void DelArea(SHORT id){ _image_dispatcher.DelArea(id); }

	void ChangeRectArea(SHORT id, const AreaRect &area){ _image_dispatcher.ChangeRectArea(id, area); }
	void ChangeEllipsArea(SHORT id, const AreaEllips &area){ _image_dispatcher.ChangeEllipsArea(id, area); }


};