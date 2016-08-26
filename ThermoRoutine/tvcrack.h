#pragma once
#include <vector>
#include <memory>
#include <list>

#include "irb_frame_helper.h"
#include "irb_file_helper.h"

#include "irb_frame_filter.h"
#include "irb_frame_manager.h"

#include <common\on_exit.h>

using namespace irb_file_helper;
using namespace irb_frame_helper;

using irb_file_t = std::shared_ptr<IRBFile>;
using irb_files_list_t = std::vector<irb_file_t>;
using irb_frame_ptr_t = std::unique_ptr<IRBFrame>;
using irb_frame_shared_ptr_t = std::shared_ptr<IRBFrame>;

void init_key_spans_and_keys_data(
	const irb_files_list_t& irb_frames_streams_list,
	std::vector<irb_frames_key_span_t> & span_key_list,
	std::vector<irb_frames_key_list_t> & key_list
	);


using irb_frames_indexes_span_t = std::pair<uint32_t, uint32_t>;

std::vector<irb_frames_indexes_span_t> init_indexes_spans(
	irb_files_list_t& irb_frames_streams_list
	);

template<int cache_limit = 1000>
class CTVcrack final
{
public:
private:
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

	//using irb_frames_cache_t = std::list<cache_irb_frames_item>;


	using irb_frames_cache_t = std::map<uint32_t, ::irb_frame_shared_ptr_t>;



	template<typename T, T defaultValue>
	class filter_table : public std::vector < T >
	{
	public:
		void resize(size_type _Newsize)
		{
			std::vector < T >::resize(_Newsize, defaultValue);
		}

		size_type operator()(size_type Pos, FILTER_SEARCH_TYPE Cond)
		{
			if (Pos > size())
				return size_type(-1);

			if (at(Pos) == defaultValue) return Pos;
			if (Cond == FILTER_SEARCH_TYPE::FILTER_NO) return Pos;
			if (Cond == FILTER_SEARCH_TYPE::FILTER_NEAREST)
			{
				size_type plus = 0xffffffff, minus = 0xffffffff, flag = 0, plusindex = -1, minusindex;
				for (size_type i = Pos + 1; i < size(); ++i)
					if (at(Pos) == defaultValue)
					{
						plus = i - Pos;
						plusindex = i;
						++flag;
						break;
					}
				if (Pos)
				{
					for (size_type i = Pos - 1; i != std::numeric_limits<size_type>::max(); --i)
						if (at(Pos) == defaultValue)
						{
							minus = Pos - i;
							minusindex = i;
							++flag;
							break;
						}
				}
				if (!flag)
					return  size_type(-1);
				if (minus < plus)
					return minusindex;
				return plusindex;
			}
			if (Cond == FILTER_SEARCH_TYPE::FILTER_PLUS)
			{
				for (size_type i = Pos + 1; i < size(); ++i) if (at(Pos) == defaultValue) return i;
			}
			else
				if (Cond == FILTER_SEARCH_TYPE::FILTER_MINUS)
				{
					if (!Pos)
						return size_type(-1);
					for (size_type i = Pos - 1; i != std::numeric_limits<size_type>::max(); i--)
						if (at(Pos) == defaultValue) return i;
				}
			return size_type(-1);
		}

	};


public:
	CTVcrack() :_number_all_frames(0), _max_number_cached_frames(cache_limit)
	{}
	~CTVcrack() 
	{
		Close();
	}
private:
	filter_table<bool,true> _filter_table;
public:
	void mark_frame_in_filter(uint32_t frame_index,bool state)
	{
		if (frame_index >= _number_all_frames)
			return;
		_filter_table[frame_index] = state;
	}

public:
	void reset(bool save_filter = false)
	{
		this->files.clear();
		m_curframe.reset();
		_number_all_frames = 0;
		_cur_frame_index = 0;

		clear_cache();
		_map_frames_key_spans_to_file_index.clear();
		_lists_frames_key.clear();
		_map_frames_indexes_spans_to_file_index.clear();
		if (!save_filter)
			_filter_table.clear();
	}

	void set_irb_files(irb_files_list_t& files, bool try_inherit_filter = false)
	{
		reset(try_inherit_filter);
		this->files.swap(files);
		files.clear();

		_map_frames_indexes_spans_to_file_index = init_indexes_spans(this->files);

		_number_all_frames = count_frames_in_files();

		if (!try_inherit_filter){
			_filter_table.resize(_number_all_frames);
		}
	}

	void write_camera_offset(int32_t offset)
	{
		for each (auto & file in files)
		{
			write_camera_offset_to_file(*file, offset);
		}
	}

	::irb_frame_shared_ptr_t get_frame_by_index(uint32_t index);
private:
	//::irb_frame_shared_ptr_t read_frame_by_id(uint32_t id);
	::irb_frame_shared_ptr_t read_frame_by_index(uint32_t index);
	int get_irb_file_index_by_frame_index(uint32_t index);
	int get_irb_file_index_by_frame_id(uint32_t id);
	int get_irb_file_index_by_frame_coordinate(coordinate_t coordinate);
	int get_irb_file_index_by_frame_time(double time);

public:
	frame_id_t get_irb_frame_id_by_index(uint32_t index);

	inline const ::irb_frame_shared_ptr_t & get_current_frame() const { return m_curframe; }

	inline uint32_t get_current_frame_index() const { return _cur_frame_index; }

	inline int number_irb_files() const { return (int)files.size(); };

	void clear_cache() { _cached_irb_frames.clear(); }

private:
	irb_files_list_t files;
	std::vector<irb_frames_key_list_t> _lists_frames_key;
	std::vector<irb_frames_key_span_t> _map_frames_key_spans_to_file_index;
	
	std::vector<irb_frames_indexes_span_t> _map_frames_indexes_spans_to_file_index;
	
	uint32_t _number_all_frames;

	irb_file_t _opened_file;

	::irb_frame_shared_ptr_t m_curframe;
	irb_frames_cache_t _cached_irb_frames;
	uint16_t _max_number_cached_frames;
	uint32_t _cur_frame_index;

private:

	double  m_time[2];          // врем€ начала и окончани€ записи
	coordinate_t _last_coordinate;      // сколько всего км проехали

public:
	BOOL Open()
	{
		if (_number_all_frames == 0)
			return false;

		m_time[0] = m_time[1] = 0;

		if (_filter_table.size() != _number_all_frames)
			_filter_table.resize(_number_all_frames);

		Go_to_frame_by_index(0, FILTER_SEARCH_TYPE::FILTER_NO);
		if (!m_curframe)
			return false;

		m_time[0] = m_curframe->get_frame_time_in_sec();

		auto last_frame = get_frame_by_index(_number_all_frames-1);

		if (!last_frame)
			return false;
		m_time[1] = last_frame->get_frame_time_in_sec();
		_last_coordinate = m_curframe->coords.coordinate;

		return true;
	}

	void Close()
	{
		_filter_table.clear();
	}
	inline bool IsOpen() const { return !files.empty(); }

	uint32_t number_frames_in_files()
	{
		return _number_all_frames;
	}


	DWORD count_frames_in_files()
	{
		DWORD num = 0;
		for (auto & file : files)
		{
			file->open();
			num += file->count_frames();
			file->close();
		}
		return num;
	}

	DWORD NumFilter()
	{
		unsigned long result = 0;
		for (auto marked : _filter_table) 
			if (marked) 
				++result;
		return result;
	} 

	inline double GetStartTime() const
	{
		if (!IsOpen()) return 0;
		return m_time[0];
	}

	inline double GetLastTime() const
	{
		if (!IsOpen()) return 0;
		return m_time[1];
	}

//	DWORD Go_to_frame_by_id(DWORD N, FILTER_SEARCH_TYPE filter);       // переместитьс€ на кадр є N
	DWORD Go_to_frame_by_index(DWORD N, FILTER_SEARCH_TYPE filter);       // переместитьс€ на кадр є N
	LONG Find(DWORD N, FILTER_SEARCH_TYPE filter)
	{
		return _filter_table(N,filter);
	}

	bool FlushFrame(uint32_t frame_index, const ::irb_frame_shared_ptr_t &frame);

	template<int max_number_frames = 1000>
	BOOL SaveFilter(int & count_frames, const std::wstring &dirname, const std::wstring &prefix)
	{
		std::wstring fullname = dirname + L"\\" + prefix + L"_filtered_0000";

		std::vector<::irb_frame_shared_ptr_t> frames_for_write;

		int counter = 0;
		for (uint32_t i = 0; i <= _number_all_frames; ++i)
		{
			count_frames = i + 1;
			if (counter == max_number_frames)
			{
				try{
					irb_frame_manager::save_frames(frames_for_write, fullname);
				}
				catch (const irb_file_helper::irb_file_exception&)
				{
					return false;
				}

				wchar_t buf[5];
				swprintf_s(buf, L"%04d", (int)(i / max_number_frames));
				fullname = dirname + L"\\" + prefix + L"_filtered_" + buf;

				counter = 0;
			}

			if (_filter_table[i])
			{
				auto frame = get_frame_by_index(i);
				if (!frame)
					continue;

				++counter;
				frames_for_write.emplace_back(frame);
			}
		}

		return irb_frame_manager::save_frames(frames_for_write, fullname);
	}


	// ‘»Ћ№“–ј÷»я

	BOOL IsFilterYes(DWORD index, const irb_frame_filter::FILTER &frame_filter)
	{
		auto frame = get_frame_by_index(index);
		if (!frame)
			return false;
		return irb_frame_filter::frame_filtered<::irb_frame_shared_ptr_t>(frame_filter, frame);
	}

	void FilterFrames(irb_frame_filter::FILTER& filter)
	{
		for (unsigned int i = 0; i < _number_all_frames; ++i)
		{
			_filter_table[i] = false;
			if (IsFilterYes(i, filter))
				_filter_table[i] = true;
		}
	}
};


// устанавливает метку дл€ кадра; ' '==сн€ть метку
template<int cache_limit> bool CTVcrack<cache_limit>::FlushFrame(uint32_t frame_index, const ::irb_frame_shared_ptr_t &frame)
{
	if (!frame)
		return false;

	auto file_index = get_irb_file_index_by_frame_index(frame_index);
	if (file_index < 0)
		return false;

	const auto & irb_frame_indexes_interval = _map_frames_indexes_spans_to_file_index[file_index];

	auto & irb_file = files[file_index];
	if (!irb_file)
		return false;
	utils::on_exit file_guard([&]{irb_file->close();});
	if (_opened_file != irb_file)
	{
		irb_file->open();
	}
	else
		file_guard.cancel();

	irb_file->write_frame_by_index(frame_index - irb_frame_indexes_interval.first, *frame.get());
	return true;

}
template<typename TKey, class TContainer, typename TContainerItem>
int get_irb_file_index_by_key_item(TKey item, TContainer container, TKey(*get_key_item_id_func)(const TContainerItem&))
{
	int index = -1;

	for (int i = 0; i < (int)container.size(); ++i)
	{
		auto & span = container[i];
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



uint64_t get_key_item_id(const irb_frame_key& key);
uint64_t get_key_item_coordinate(const irb_frame_key& key);
double get_key_item_time(const irb_frame_key& key);

uint32_t get_key_item_index(const uint32_t & index);

template<int cache_limit>
int CTVcrack<cache_limit>::get_irb_file_index_by_frame_index(uint32_t index)
{
	return get_irb_file_index_by_key_item(index, _map_frames_indexes_spans_to_file_index, get_key_item_index);
}


template<int cache_limit>
int CTVcrack<cache_limit>::get_irb_file_index_by_frame_id(uint32_t id)
{
	return get_irb_file_index_by_key_item((uint64_t)id, _map_frames_key_spans_to_file_index, get_key_item_id);
}

template<int cache_limit>
int CTVcrack<cache_limit>::get_irb_file_index_by_frame_coordinate(coordinate_t coordinate)
{
	return get_irb_file_index_by_key_item(coordinate, _map_frames_key_spans_to_file_index, get_key_item_coordinate);
}
template<int cache_limit>
int CTVcrack<cache_limit>::get_irb_file_index_by_frame_time(double time)
{
	return get_irb_file_index_by_key_item(time, _map_frames_key_spans_to_file_index, get_key_item_time);
}

template<int cache_limit>
::irb_frame_shared_ptr_t CTVcrack<cache_limit>::read_frame_by_index(uint32_t index)
{
	auto file_index = get_irb_file_index_by_frame_index(index);
	if (file_index == -1)
		return ::irb_frame_shared_ptr_t();

	auto & irb_file = files[file_index];
	if (!irb_file)
		return ::irb_frame_shared_ptr_t();

	if (_opened_file != irb_file)
	{
		if (_opened_file)
			_opened_file->close();
		_opened_file = irb_file;
		_opened_file->open();
	}

	const auto & irb_frame_indexes_interval = _map_frames_indexes_spans_to_file_index[file_index];

	::irb_frame_shared_ptr_t frame(irb_file->read_frame_by_index(index - irb_frame_indexes_interval.first));
	if (frame)
	{
//		_cached_irb_frames.emplace_back(cache_irb_frames_item{ frame->id, frame->coords.coordinate, frame->header.presentation.imgTime, frame });
		_cached_irb_frames.insert({ index, frame });
		if (_max_number_cached_frames <= _cached_irb_frames.size())
			_cached_irb_frames.clear();//_cached_irb_frames.pop_front();
	}
	return frame;
}

template<int cache_limit>
::irb_frame_shared_ptr_t CTVcrack<cache_limit>::get_frame_by_index(uint32_t index)
{
	if (index >= _number_all_frames)
		return ::irb_frame_shared_ptr_t();
	if (_cur_frame_index == index && m_curframe)
		return m_curframe;

	auto result = _cached_irb_frames.find(index);
	if (result != _cached_irb_frames.end()) {
		return result->second;
	}

	return read_frame_by_index(index);
}

template<int cache_limit>
DWORD CTVcrack<cache_limit>::Go_to_frame_by_index(DWORD N, FILTER_SEARCH_TYPE filter)
{
	if (_cur_frame_index == N && m_curframe)
		return N;

	if (N >= _number_all_frames)
		return N;

	auto index = Find(N, filter);
	if (index == -1)
		return N;

	m_curframe = std::make_shared<IRBFrame>(*get_frame_by_index(index));

	_cur_frame_index = index;

	return index;
}

template<int cache_limit>
frame_id_t CTVcrack<cache_limit>::get_irb_frame_id_by_index(uint32_t index)
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
