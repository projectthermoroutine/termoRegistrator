#include "tvcrack.h"

#include <Windows.h>
#ifdef _WINGDI_
#undef _WINGDI_
#endif

#include <atltypes.h>
#include <atltime.h>

#include <vector>
#include <memory>
#include <thread>
#include <algorithm>

#include <common\handle_holder.h>
#include <common\on_exit.h>
#include <common\sync_helpers.h>

#include "metro_map.h"

#include "irb_frame_helper.h"
#include "irb_file_helper.h"

// относительное время в виде строки
time_t Msec2String(time_t msec, char *str, int znak)
{
	if (!str)
		return msec;

	int ms = (int)(msec % 1000);
	int sec = (int)(msec / 1000);
	int min = (sec / 60) % 60;
	int hour = sec / 3600;
	sec = sec % 60;
	sprintf_s(str,20, "%d:%02d:%02d", hour, min, sec);

	if (znak > 0)
	{
		char format[20], s[20];
		sprintf_s(format, "%%.%df", znak);
		sprintf_s(s, format, 0.001*ms);
		if (atof(s) < 1)
		{
			strcat_s(str,20, &s[1]);
		}
		else
		{
			if (hour < 2)
			{
				CTime t0(2000, 1, 1, hour, min, sec);
				CTime t = CTime(t0.GetTime() + 1);
				sprintf_s(str,20, "%d:%02d:%02d.0", t.GetHour(), t.GetMinute(), t.GetSecond());
			}
		}
	}
	return msec;
}

CTVcrack::CTVcrack()
{
	m_scaleTop = 0.0;
	m_scaleBottom = 0.0;
	m_needToCalcDiap = true;

	_number_all_frames = 0;
	_max_number_cached_frames = 1200;

	m_fAutoPal = false;
}
CTVcrack::~CTVcrack()
{
	Close();
}

void init_key_spans_and_keys_data(
	const irb_files_list_t irb_frames_streams_list,
	std::vector<irb_frames_key_span_t> & span_key_list,
	std::vector<irb_frames_key_list_t> & key_list
	)
{
	span_key_list.clear();
	key_list.clear();

	for (auto & stream : irb_frames_streams_list)
	{
		span_key_list.emplace_back(stream->get_frames_key_span());
		key_list.emplace_back(stream->get_frames_key_list());
	}

	std::sort(key_list.begin(), key_list.end(),
		[](const irb_frames_key_list_t& el1, const irb_frames_key_list_t& el2)
	{
		if (el1.size() == 0)
			return false;
		if (el2.size() == 0)
			return true;
		if (el1.back().id < el2.front().id)
			return true;
		return false;
	});
}


void CTVcrack::reset(bool save_filter)
{
	this->files.clear();
	m_curframe.reset();
	_number_all_frames = 0;
	clear_cache();
	_map_frames_key_spans_to_file_index.clear();
	_lists_frames_key.clear();
	if (!save_filter)
		m_filter.Reset();

}

void CTVcrack::set_irb_files(irb_files_list_t& files, bool try_inherit_filter)
{
	reset(try_inherit_filter);
	this->files = std::move(files);

	init_key_spans_and_keys_data(this->files, _map_frames_key_spans_to_file_index, _lists_frames_key);
	_number_all_frames = count_frames_in_files();

	if (!try_inherit_filter || m_filter.size() != _number_all_frames){
		m_filter.Open(_number_all_frames);
	}
}

DWORD CTVcrack::get_irb_frame_id_by_index(uint32_t index)
{
	if (_number_all_frames <= index)
		return 0;
	DWORD result_id = 0;
	DWORD count_frames = 0;
	for (auto & list_ids : _lists_frames_key)
	{	
		if (index >= count_frames + list_ids.size()){
			count_frames += (DWORD)list_ids.size();
			continue;
		}
		result_id = list_ids[index - count_frames].id;
		break;
	}

	return result_id;
}

LONG CTVcrack::get_irb_frame_index_by_id(uint32_t id)
{
	LONG result_id = -1;
	for (auto & list_ids : _lists_frames_key)
	{
		if (id > list_ids.back().id)
			continue;
		auto & res = std::find_if(list_ids.cbegin(), list_ids.cend(), [id](const irb_frame_key& key)->bool{return key.id == id; });
		if (res == list_ids.cend())
			return -1;

		result_id = (LONG)std::distance(list_ids.cbegin(), res);
		break;
	}

	return result_id;
}

DWORD CTVcrack::get_last_irb_frame_id()
{
	DWORD result_id = 0;
	for (auto & span : _map_frames_key_spans_to_file_index)
	{
		if (result_id < span.second.id){
			result_id = span.second.id;
		}
	}

	return result_id;
}
DWORD CTVcrack::get_first_irb_frame_id()
{
	DWORD result_id = (DWORD)-1;
	for (auto & span : _map_frames_key_spans_to_file_index)
	{
		if (result_id > span.first.id){
			result_id = span.first.id;
		}
	}

	return result_id;
}

BOOL CTVcrack::Open()
{
	//_number_all_frames = count_frames_in_files();
	if (_number_all_frames == 0)
		return false;

	m_time[0] = m_time[1] = 0;

	auto first_frame_id = get_first_irb_frame_id();
	auto last_frame_id = get_last_irb_frame_id();

	if (m_filter.size() != _number_all_frames)
		m_filter.Open(_number_all_frames);
	
	Go_to_frame_by_id(first_frame_id, FILTER_NO);
	if (!m_curframe)
		return false;


	_image_dispatcher.set_areas_mask_size(m_curframe->header.geometry.imgWidth, m_curframe->header.geometry.imgHeight);

	m_msec0 = (time_t)m_curframe->get_frame_time_in_msec();

	m_time[0] = convert_irb_frame_time_in_sec(m_curframe->get_frame_time_in_sec());

	auto last_frame = get_frame_by_id(last_frame_id);

	if (!last_frame)
		return false;
	m_time[1] = convert_irb_frame_time_in_sec(last_frame->get_frame_time_in_sec());
	_last_coordinate = m_curframe->coords.coordinate;

	return true;
}

// закрыть фильтр 
void CTVcrack::Close() {

	m_filter.Reset();
}

// -------------------------------------------------------------
// общая информация о файле
// -------------------------------------------------------------
// сколько кадров в файле
DWORD CTVcrack::count_frames_in_files()
{
	DWORD num = 0;
	for (auto & file : files)
	{
		num += file->count_frames();
	}
	return num;
}
// -------------------------------------------------------------
// найти кадр с учетом фильтрации
// -------------------------------------------------------------
LONG CTVcrack::Find(DWORD N, int filter)
{
	if (m_filter.filter[N]) return N;
	if (filter == FILTER_NO) return N;
	if (filter == FILTER_NEAREST)
	{
		DWORD plus = 0xffffffff, minus = 0xffffffff, flag = 0, plusindex, minusindex;
		for (DWORD i = N + 1; i < _number_all_frames; i++)
		if (m_filter.filter[i])
		{
			plus = i - N;
			plusindex = i;
			flag++;
			break;
		}
		if (N)
		{
			for (DWORD i = N - 1; i >= 0; i--)
			if (m_filter.filter[i])
			{
				minus = N - i;
				minusindex = i;
				flag++;
				break;
			}
		}
		if (!flag)
			return  -1;
		if (minus < plus)
			return minusindex;
		return plusindex;
	}
	if (filter == FILTER_PLUS)
	{
		for (DWORD i = N + 1; i < _number_all_frames; i++) if (m_filter.filter[i]) return i;
	}
	else
	if (filter == FILTER_MINUS)
	{
		if (!N)
			return -1;
		for (DWORD i = N - 1; i >= 0; i--)
		if (m_filter.filter[i]) return i;
	}
	return -1;
}

// -------------------------------------------------------------
// установить новую текущую запись.
// return новый текйщий №
// -------------------------------------------------------------
// переместиться на кадр № N

DWORD CTVcrack::Go_to_frame_by_index(DWORD N, int filter)
{
	if (_cur_frame_index == N)
		return N;

	if (N >= _number_all_frames)
		return N;

	auto index = Find(N, filter);
	if (index == -1)
		return N;

	auto frame_id = get_irb_frame_id_by_index(index);
	if (frame_id == 0)
		return _cur_frame_index;

	_cur_frame_index = index;
	_cur_frame_id = frame_id;

	m_curframe = std::make_shared<IRBFrame>(*get_frame_by_id(frame_id));

	return index;
}


DWORD CTVcrack::Go_to_frame_by_id(DWORD N, int filter)
{
	auto frame_index = get_irb_file_index_by_frame_id(N);
	if (frame_index == -1)
		return N;

	frame_index = Find(frame_index, filter);
	if (frame_index == -1)
		return N;

	int counter = 0;
	auto frame_id = get_irb_frame_id_by_index(frame_index);
	if (frame_id == 0)
		return m_curframe->id;

	_cur_frame_index = frame_index;
	_cur_frame_id = frame_id;
	m_curframe = std::make_shared<IRBFrame>(*get_frame_by_id(frame_id));

	return m_curframe->id;
}

bool CTVcrack::get_formated_current_frame_raster(
	irb_frame_image_dispatcher::irb_frame_raster_ptr_t raster,
	irb_frame_image_dispatcher::temperature_span_t & calibration_interval
	)
{
	if (!m_curframe)
		return false;

	return get_formated_frame_raster(m_curframe, raster, calibration_interval);

}
bool CTVcrack::get_formated_frame_raster_by_index(DWORD N,
	irb_frame_image_dispatcher::irb_frame_raster_ptr_t raster,
	irb_frame_image_dispatcher::temperature_span_t & calibration_interval
	)
{
	if (_cur_frame_index == N)
		return get_formated_current_frame_raster(raster, calibration_interval);

	auto frame_id = get_irb_frame_id_by_index(N);
	if (frame_id == 0)
		return false;

	auto frame = get_frame_by_id(frame_id);
	if (!frame)
		return false;

	return get_formated_frame_raster(frame, raster, calibration_interval);

}
bool CTVcrack::get_formated_frame_raster(const ::irb_frame_shared_ptr_t & frame,
	irb_frame_image_dispatcher::irb_frame_raster_ptr_t raster,
	irb_frame_image_dispatcher::temperature_span_t & calibration_interval
	)
{
	return _image_dispatcher.get_formated_frame_raster(frame, raster, calibration_interval);
}

// прочитать время
time_t CTVcrack::ReadTime(DWORD N) 
{

	auto frame_id = get_irb_frame_id_by_index(N);
	if (frame_id == 0)
		return 0;
	auto frame = get_frame_by_id(frame_id);

	if (!frame)
		return 0;

	auto res = convert_irb_frame_time_in_sec(frame->get_frame_time_in_sec()) - TIME_POPRAVKA;

	return res;
}

time_t CTVcrack::Msec(char *str, int znak)
{
	return Msec2String((time_t)(m_curframe->get_frame_time_in_msec()) - m_msec0, str, znak);
}

uint64_t get_key_item_id(const irb_frame_key& key)
{
	return (uint64_t)key.id;
}


uint64_t get_key_item_coordinate(const irb_frame_key& key)
{
	return key.coordinate;
}

uint64_t get_key_item_time(const irb_frame_key& key)
{
	return key.coordinate;
}


int CTVcrack::get_irb_file_index_by_key_item(uint64_t item, uint64_t (*get_key_item_id_func)(const irb_frame_key&))
{
	int index = -1;

	for (int i = 0; i < (int)_map_frames_key_spans_to_file_index.size(); i++)
	{
		auto & span = _map_frames_key_spans_to_file_index[i];
		auto begin_value = get_key_item_id_func(span.first);
		auto end_value = get_key_item_id_func(span.second);
		if (begin_value <= item && end_value >= item)
		{
			index = i;
			break;
		}
	}
	return index;
}

int CTVcrack::get_irb_file_index_by_frame_id(uint32_t id)
{
	return get_irb_file_index_by_key_item((uint64_t)id, get_key_item_id);
}

int CTVcrack::get_irb_file_index_by_frame_coordinate(coordinate_t coordinate)
{
	return get_irb_file_index_by_key_item(coordinate, get_key_item_coordinate);
}
int CTVcrack::get_irb_file_index_by_frame_time(time_t time)
{
	return get_irb_file_index_by_key_item(time, get_key_item_time);
}

::irb_frame_shared_ptr_t CTVcrack::read_frame_by_id(uint32_t id)
{
	auto file_index = get_irb_file_index_by_frame_id(id);
	if (file_index == -1)
		return ::irb_frame_shared_ptr_t();

	auto & irb_file = files[file_index];

	::irb_frame_shared_ptr_t frame(irb_file->read_frame(id).release());
	if (frame)
	{
		cache_irb_frames_item item(frame->id, frame->coords.coordinate, frame->coords.coordinate, frame);
		_cached_irb_frames.emplace_back(item);
		if (_max_number_cached_frames <= _cached_irb_frames.size())
			_cached_irb_frames.pop_front();
	}
	return frame;
}


::irb_frame_shared_ptr_t CTVcrack::get_frame_by_coordinate(coordinate_t coordinate)
{
	auto & result = std::find_if(_cached_irb_frames.cbegin(), _cached_irb_frames.cend(),
		[coordinate](const cache_irb_frames_item& item)->bool
	{
		return item.key == coordinate;
	});

	if (result != _cached_irb_frames.cend())
	{
		return result->frame;
	}

	auto file_index = get_irb_file_index_by_frame_coordinate(coordinate);
	if (file_index == -1)
		return ::irb_frame_shared_ptr_t();

	::irb_frame_shared_ptr_t frame(files[file_index]->read_frame_by_coordinate(coordinate).release());
	return frame;
}
::irb_frame_shared_ptr_t CTVcrack::get_frame_by_time(time_t time)
{
	auto & result = std::find_if(_cached_irb_frames.cbegin(), _cached_irb_frames.cend(),
		[time](const cache_irb_frames_item& item)->bool
	{
		return item.key == time;
	});

	if (result != _cached_irb_frames.cend())
	{
		return result->frame;
	}
	auto file_index = get_irb_file_index_by_frame_time(time);
	if (file_index == -1)
		return ::irb_frame_shared_ptr_t();

	::irb_frame_shared_ptr_t frame(files[file_index]->read_frame_by_time(time).release());
	return frame;
}


::irb_frame_shared_ptr_t CTVcrack::get_frame_by_id(uint32_t id)
{
	auto & result = std::find_if(_cached_irb_frames.cbegin(), _cached_irb_frames.cend(), 
		[id](const cache_irb_frames_item& item)->bool
	{
		return item.key == id;
	});

	if (result != _cached_irb_frames.cend())
	{
		return result->frame;
	}

	return read_frame_by_id(id);
}

::irb_frame_shared_ptr_t CTVcrack::get_frame_by_index(uint32_t index)
{
	if (index >= _number_all_frames)
		return ::irb_frame_shared_ptr_t();
	if (_cur_frame_index == index)
		return m_curframe;

	auto frame_id = get_irb_frame_id_by_index(index);
	if (frame_id == 0)
		return ::irb_frame_shared_ptr_t();

	return get_frame_by_id(frame_id);
}

// прочитать километраж
coordinate_t CTVcrack::ReadKm(DWORD frameNum)
{
	auto frame = get_frame_by_id(frameNum);
	if (!frame) return 0;
	return frame->coords.coordinate;
}

// возвращает мм для текущего файла
coordinate_t CTVcrack::GetKm()
{
	return m_curframe->coords.coordinate;
}
BOOL CTVcrack::SaveFrame(IRBFrame *frame, const std::string & fname)
{
	if (frame == nullptr)
	{
		return false;
	}
	try{
		auto file_stream = create_irb_file(fname, irb_file_version::original, 1);
		IRBFile f(file_stream);
		f.write_frame(1, *frame);
	}
	catch (const irb_file_helper::irb_file_exception&)
	{
		return false;
	}

	return true;
}

// сохранить текущий кадр
BOOL CTVcrack::SaveCurr(const std::string & fname)
{
	return SaveFrame(m_curframe.get(), fname);
}

bool CTVcrack::SaveFrames(const std::vector<::irb_frame_shared_ptr_t> & frames, const std::string & fname)
{
	if (frames.size() == 0)
	{
		return false;
	}
	try{
		auto file_stream = create_irb_file(fname, irb_file_version::original, (uint32_t)frames.size());
		IRBFile f(file_stream);
		f.append_frames(frames);
	}
	catch (const irb_file_helper::irb_file_exception&)
	{
		return false;
	}

	return true;
}

// сохранить с учетом фильтрации
BOOL CTVcrack::SaveFilter(int & p, const std::string &dirname, const std::string &prefix)
{
	std::string fullname = dirname + "\\" + prefix + "_filtered_0000.irb";

	std::vector<::irb_frame_shared_ptr_t> frames_for_write;

	int all_counter = 0;
	int counter = 0;
	for (uint32_t i = 0; i <= _number_all_frames; i++)
	{
		p = i + 1;
		if (counter == 1000)
		{
			SaveFrames(frames_for_write, fullname);

			char buf[5];
			sprintf_s(buf, "%04d", (int)(i / 1000));
			fullname = dirname + "\\" + prefix + "_filtered_" + buf + ".irb";

			counter = 0;
		}

		if (m_filter.filter[i])
		{
			auto frame_id = get_irb_frame_id_by_index(i);
			if (frame_id == 0)
				continue;

			auto frame = get_frame_by_id(frame_id);
			if (!frame)
				continue;

			all_counter++;
			counter++;
			frames_for_write.emplace_back(frame);
		}
	}

	return SaveFrames(frames_for_write, fullname);
}

// ----------------------------------------------------------
// Метка
// ----------------------------------------------------------
// возвращает метку для текущего кадра
BOOL CTVcrack::GetMetka(char *metka)
{
	return m_curframe->is_checked();
}
// устанавливает метку для кадра; ' '==снять метку
BOOL CTVcrack::SetMetka(char metka) {

	if (!m_curframe)
		return false;

	bool state = true;
	if (metka == ' ')
		state = false;

	m_curframe->set_check_state(state);

	auto file_index = get_irb_file_index_by_frame_id(m_curframe->id);
	if (file_index == -1)
		return false;

	files[file_index]->write_frame(m_curframe->id, *m_curframe.get());
	return true;

}
BOOL CTVcrack::SetMetka(DWORD N, char metka)
{

	if (N >= _number_all_frames) return false;
	auto frame = get_frame_by_id(N);
	if (!frame)
		return false;

	auto file_index = get_irb_file_index_by_frame_id(N);
	if (file_index == -1)
		return false;
	
	bool state = true;
	if (metka == ' ')
		state = false;
	frame->set_check_state(state);
	files[file_index]->write_frame(N, *frame.get());
	return true;

}
// поиск метки
int CTVcrack::FindMetka(int startindex, int dir)
{
	for (int i = startindex;; i += dir)
	{
		if (i < 0 || i >= (int)_number_all_frames)
			return -1;
		auto frame_id = get_irb_frame_id_by_index(i);
		if (frame_id == 0)
			return -1;
		auto frame = get_frame_by_id(frame_id);

		if (frame) return frame_id;
	}
	return -1;
}

// удовлетворяет ли кадр условиям фильтрации
BOOL CTVcrack::IsFilterYes(const IRBFrame *frame)
{
	static int oldp = 0;
	if ((m_filter.flags&FILTER_METKA) && (frame->is_checked()))
		return FALSE;
	if ((m_filter.flags&FILTER_TIME0) || (m_filter.flags&FILTER_TIME1))
	{
		CTime t(convert_irb_frame_time_in_sec(frame->get_frame_time_in_sec()) - TIME_POPRAVKA);
		int ti = t.GetMinute() + t.GetHour() * 60;
		if ((m_filter.flags&FILTER_TIME0) && ti<m_filter.time[0])
			return FALSE;
		if ((m_filter.flags&FILTER_TIME1) && ti>m_filter.time[1])
			return FALSE;
	}

	//int p = atoi((char *)frame->coords.pickNoffs);

	//if (pick[0] <= pick[1]) {
	//	if ((flags&FILTER_PICKMIN) && (p<pick[0] || !p))
	//		return FALSE;
	//	if ((flags&FILTER_PICKMAX) && (p>pick[1] || !p))
	//		return FALSE;
	//}
	//else
	//{
	//	if ((flags&FILTER_PICKMIN) && (p > pick[0] || !p))
	//		return FALSE;
	//	if ((flags&FILTER_PICKMAX) && (p < pick[1] || !p))
	//		return FALSE;
	//}

	return TRUE;
}

BOOL CTVcrack::IsFilterYes()
{

	if (!IsFilterYes(m_curframe.get()))
		return FALSE;

	float min = 0, max = 0, avr = 0;

	m_curframe->ComputeMinMaxAvr(&min, &max, &avr);

	m_curframe->min_temperature = min;
	m_curframe->avr_temperature = avr;
	m_curframe->max_temperature = max;

	if ((m_filter.flags&FILTER_TEMPFULL) && (max - avr <= m_filter.T[0]))
		return FALSE;

	if ((m_filter.flags&FILTER_TEMPREGION) && (max - avr <= m_filter.T[1]))
		return FALSE;

	return TRUE;
}

BOOL CTVcrack::IsFilterYes(DWORD index)
{
	if (!(m_filter.flags & FILTER_TEMPFULL) && !(m_filter.flags & FILTER_TEMPREGION))
	{
		auto frame = get_frame_by_index(index);
		if (!frame)
			return false;
		return IsFilterYes(frame.get());
	}

	Go_to_frame_by_index(index, FILTER_NO);
	return IsFilterYes();
}

// установить фильтр
void CTVcrack::SetFilter(const FILTER &f)
{
	m_filter.copy_params(f);
}

void CTVcrack::mark_frame_in_filter(uint32_t frame_index, bool state)
{
	if (frame_index >= _number_all_frames)
		return;

	m_filter.filter[frame_index] = state ? 1 : 0;
}
void CTVcrack::CurFrameTemperaturesCompute(void)
{
	if (!IsOpen())
		return;

	if (!m_curframe)
		return;
	_image_dispatcher.calculate_average_temperature(m_curframe);
}

void CTVcrack::FilterFrames(FILTER& filter)
{

	filter.Open(_number_all_frames);
	m_filter = std::move(filter);

	for (unsigned int i = 0; i < _number_all_frames; i++)
	{
		m_filter.filter[i] = 0;
		if (IsFilterYes(i))
		{
			m_filter.filter[i] = 1;
		}
	}
}