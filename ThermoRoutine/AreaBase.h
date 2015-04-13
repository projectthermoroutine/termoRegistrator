#pragma once
#include <vector>
#include <common\sync_helpers.h>
#include "ThermoRoutine_i.h"

using point_coordinate = std::pair<int16_t, int16_t>;

class AreaBase
{
public:
	int id;
	float m_min;
	float m_max;
	float m_avr;
	float m_summary;
	int pixelsCounter;
	bool is_valid;
public:
	AreaBase(void) :is_valid(false)
	{
		reset();
	}

	void reset()
	{
		m_min = 1000;
		m_max = -1000;
		m_avr = 0;
		m_summary = 0;
		pixelsCounter = 0;
	}
	bool operator==(const AreaBase& area){ return id == area.id; }

	virtual bool IsInTheArea(int x, int y) = 0;
	virtual void SetTemp(float temp) final
	{
		is_valid = true;
		if (temp < m_min)
			m_min = temp;
		if (temp > m_max)
			m_max = temp;
		m_summary += temp;
		pixelsCounter++;

		if (pixelsCounter > 0)
			m_avr = m_summary / pixelsCounter;

	}
	virtual void SetTemp(int x, int y, float temp) final
	{
		if (!IsInTheArea(x, y))
			return;
		SetTemp(temp);
	}


	virtual std::vector<point_coordinate> get_area_sprite(int max_width = 0,int max_height = 0) = 0;


};

class AreaEllips :
	public AreaBase
{
private:
	int _x0;
	int _y0;
	int _a;
	int _b;
	float _e;

	int _a2;
	int _b2;

public:
	AreaEllips(int x,int y,int a,int b) : AreaBase()
	{
		_x0 = x;
		_y0 = y;
		_a = a/2;
		_b = b/2;
		_a2 = _a*_a;
		_b2 = _b*_b;
		_e = std::sqrtf(1.0f - _b2 / _a2);
	}
	bool IsInTheArea(int x, int y) override
	{
		int dX = _x0 + _a;
		int dY = _y0 + _b;
		return ((x - dX) ^ 2) / _a2 + ((y - dY) ^ 2) / _b2 <= 1;
	}

	std::vector<point_coordinate> get_area_sprite(int max_width, int max_height) override
	{
		std::vector<point_coordinate> points;

		if (max_width > 0 && max_width < _a * 2)
			return points;

		if (max_height > 0 && max_height < _b * 2)
			return points;

		for (int16_t y = _y0 - _b; y < _y0 + _b; y++)
		{
			int x1 = (int)std::sqrt(_a2*(1 - y / _b2));
			for (int16_t x = _x0 - x1; x <= _x0 + x1; x++)
			{
				points.emplace_back(std::make_pair(x, y));
			}
		}

		return points;
	}


};
class AreaPoly :
	public AreaBase
{
public:
	AreaPoly(void) : AreaBase()
	{
	}
	bool IsInTheArea(int x, int y) override
	{
		return false;
	}
};
class AreaRect :
	public AreaBase
{
public:
	int m_x;
	int m_y;
	int m_width;
	int m_height;
public:
	AreaRect(void) : AreaBase()
	{
		m_x = 0;
		m_y = 0;
		m_width = 0;
		m_height = 0;
	}

	bool IsInTheArea(int x, int y) override
	{
		return (x > m_x) && (x < (m_x + m_width)) && (y > m_y) && (y < (m_y + m_height));
	}


	std::vector<point_coordinate> get_area_sprite(int max_width, int max_height) override
	{
		std::vector<point_coordinate> points;

		int width = m_width;
		int height = m_height;
		if (max_width > 0 && max_width < m_width)
			width = max_width;

		if (max_height > 0 && max_height < m_height)
			height = max_height;

		for (int16_t y = m_y; y < m_y + height; y++)
		for (int16_t x = m_x; x < m_x + width; x++)
		{
			points.emplace_back(std::make_pair(x,y));
		}

		return points;
	}

};


#define GET_AREA_MASK_ITEM_KEY(_item) *(_item)
#define SET_AREA_MASK_ITEM_KEY(_item,_key) *(_item) = _key;
#define IS_AREA_MASK_ITEM_SET(_item) (*(_item) > 0)


using mask_key_t = AreaBase*;
using mask_item_t = AreaBase*;
class areas_mask
{
public:
	areas_mask() :width(0), height(0){}

	void set_size(uint16_t width, uint16_t height)
	{
		this->width = width;
		this->height = height;
		mask.resize(width*height, 0);
	}
	void clear() { std::memset(mask.data(), 0, sizeof(mask_item_t)*mask.size()); }

	bool add_area(const std::shared_ptr<AreaBase> &area)
	{
		auto area_points = area->get_area_sprite(width,height);
		if (area_points.empty())
			return false;
		auto area_key = area.get();
		for (auto & point : area_points)
		{
			if (point.first < 0 || point.second < 0)
				continue;
			if (point.first >= width || point.second >= height)
				continue;

			auto p_item = &mask[point.second*width + point.first];
			SET_AREA_MASK_ITEM_KEY(p_item, area_key);

		}
		return true;
	}

	void delete_area(const std::vector<point_coordinate>& area_points)
	{
		for (auto & point : area_points)
		{
			if (point.first < 0 || point.second < 0)
				continue;
			if (point.first >= width || point.second >= height)
				continue;

			mask[point.second*width + point.first] = 0;
		}
	}


public:
	std::vector<mask_item_t> mask;

	uint16_t width;
	uint16_t height;

public:
	inline mask_key_t get_key(const mask_item_t* item) const { return GET_AREA_MASK_ITEM_KEY(item); }
};


class areas_dispatcher
{
	using area_ptr_t = std::shared_ptr<AreaBase>;

public:
	areas_dispatcher()
	{
	}
private:
	std::vector<area_ptr_t> _areas;
	areas_mask _areas_mask;
	sync_helpers::rw_lock _lock_areas;

private:
	const AreaBase * get_area_by_id(const std::vector<area_ptr_t>& container, int id)
	{
		auto find_index = std::find_if(container.cbegin(), container.cend(), [id](const area_ptr_t& area)->bool{return area->id == id; });
		if (find_index == container.cend())
			return nullptr;

		return find_index->get();
	}

public:
	void lock(bool exclusive) { _lock_areas.lock(exclusive); }
	void unlock(bool exclusive) { _lock_areas.unlock(exclusive); }
	areas_mask& get_areas_mask() { return _areas_mask; }

	void set_default_areas()
	{
		std::for_each(_areas.begin(), _areas.end(), [](area_ptr_t& area){area->reset(); area->is_valid = false; });
	}

public:
	bool Empty() { return _areas.empty(); }

	bool get_area_temperature_measure(int area_id, area_temperature_measure &measure)
	{
		bool res = false;
		sync_helpers::rw_lock_guard_shared guard(_lock_areas);
		auto area = get_area_by_id(_areas, area_id);
		if (area != nullptr && area->is_valid)
		{
			res = true;
			measure.min = area->m_min;
			measure.max = area->m_max;
			measure.avr = area->m_avr;
		}

		return res;
	}

	void set_areas_mask_size(short width, short height){ _areas_mask.set_size(width, height); }

	void clear_areas() { sync_helpers::rw_lock_guard_exclusive guard(_lock_areas); _areas.clear(); _areas_mask.clear(); }

private:
	template<class TArea>
	bool add_area(const TArea &area)
	{
		sync_helpers::rw_lock_guard_exclusive guard(_lock_areas);

		_areas.emplace_back(std::make_unique<TArea>(area));
		auto & _area = _areas.back();

		return _areas_mask.add_area(_area);

	}
public:
	bool AddAreaRect(const AreaRect &area)
	{
		return add_area(area);
	}

	bool AddAreaEllips(const AreaEllips &area)
	{
		return add_area(area);
	}
	void DelArea(SHORT id)
	{
		sync_helpers::rw_lock_guard_exclusive guard(_lock_areas);

		auto del_index = std::find_if(_areas.cbegin(), _areas.cend(), [id](const area_ptr_t& area)->bool{return area->id == id; });
		if (del_index != _areas.cend())
		{
			_areas.erase(del_index);
			_areas_mask.clear();
			mask_key_t i = 0;
			for (auto & area : _areas)
			{
				_areas_mask.add_area(area);
				i++;
			}
		}
	}

	void ChangeRectArea(SHORT id, const AreaRect &area)
	{
		DelArea(id);
		add_area(area);
	}
	void ChangeEllipsArea(SHORT id, const AreaEllips &area)
	{
		DelArea(id);
		add_area(area);
	}
};


