#pragma once
#include <vector>
#include <list>
#include <memory>
#include <mutex>
#include <string>
#include <common\sync_helpers.h>
#include "ThermoRoutine_i.h"

#include <loglib\log.h>

using point_coordinate = std::pair<int16_t, int16_t>;

class AreaBase
{
public:
	int id;
	float m_min;
	float m_max;
	double m_avr;
	double m_summary;
	int pixelsCounter;
	bool is_valid;
public:
	AreaBase(void) :is_valid(false)
	{
		reset();
	}

	virtual ~AreaBase(){}

	void reset()
	{
		m_min = 1000;
		m_max = -1000;
		m_avr = 0;
		m_summary = 0;
		pixelsCounter = 0;
	}

	bool operator==(const AreaBase& area){ return id == area.id; }

	virtual bool is_proxy() const { return false; }

public:
	virtual void SetTemp(float temp)
	{
		is_valid = true;
		if (temp - 273.15f < m_min)
			m_min = temp - 273.15f;
		if (temp - 273.15f > m_max)
			m_max = temp - 273.15f;
		m_summary += temp;
		pixelsCounter++;

		if (pixelsCounter > 0)
			m_avr = m_summary / pixelsCounter - 273.15f;

	}
	virtual void SetTemp(int x, int y, float temp)
	{
		if (IsInTheArea(x, y))
			SetTemp(temp);
	}

public:
	virtual bool IsInTheArea(int x, int y) const = 0;
	virtual std::vector<point_coordinate> get_area_sprite(int max_width = 0, int max_height = 0) const = 0;

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
		_a = a<2 ? 1 : a/2;
		_b = b<2 ? 1 : b/2;
		_x0 += _a;
		_y0 += _b;
		_a2 = _a*_a;
		_b2 = _b*_b;
		_e = std::sqrtf(1.0f - (float)_b2 / (float)_a2);
	}

	virtual ~AreaEllips(){}

	bool IsInTheArea(int x, int y) const override
	{
		int dX = _x0 + _a;
		int dY = _y0 + _b;
		return ((x - dX) ^ 2) / _a2 + ((y - dY) ^ 2) / _b2 <= 1;
	}

	std::vector<point_coordinate> get_area_sprite(int max_width, int max_height) const override
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
	virtual ~AreaPoly(){}

	bool IsInTheArea(int x, int y) const override
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

	virtual ~AreaRect(){}

	bool IsInTheArea(int x, int y) const override
	{
		return (x > m_x) && (x < (m_x + m_width)) && (y > m_y) && (y < (m_y + m_height));
	}


	std::vector<point_coordinate> get_area_sprite(int max_width, int max_height) const override
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


class AreasProxy : public AreaBase
{
public:
	AreasProxy() : AreaBase(), _refs(0)
	{}

	virtual ~AreasProxy(){}

	virtual bool is_proxy() const override { return true; }

	bool IsInTheArea(int x, int y) const override
	{
		if (_areas.empty())
			return false;

		return std::all_of(_areas.cbegin(), _areas.cend(), [x,y](const AreaBase* area){return area->IsInTheArea(x, y); });
	}

	std::vector<point_coordinate> get_area_sprite(int max_width, int max_height) const override
	{
		return{};
	}

	virtual void SetTemp(float temp) override
	{
		for (auto & area : _areas)
			area->SetTemp(temp);

	}
	virtual void SetTemp(int x, int y, float temp) override
	{
		if (IsInTheArea(x, y))
			SetTemp(temp);
	}


public:

	void set_areas(std::vector<AreaBase*>&& areas) { _areas= std::move(areas); }
	std::vector<AreaBase*> areas() const { return _areas; }

	std::uint32_t use_count() const { return _refs; }
	bool unique() const { return _refs == 1; }
	void add_ref() { ++_refs; }
	void sub_ref() { --_refs; }
private:

	std::vector<AreaBase*> _areas;
	std::uint32_t _refs;
};



#define GET_AREA_MASK_ITEM_KEY(_item) *(_item)
#define SET_AREA_MASK_ITEM_KEY(_item,_key) *(_item) = _key;
#define IS_AREA_MASK_ITEM_SET(_item) (*(_item) > 0)


using mask_key_t = AreaBase*;
using mask_item_t = AreaBase*;
class areas_mask
{
	using areas_proxy_ptr_t = std::unique_ptr<AreasProxy>;
public:
	areas_mask() :width(0), height(0){}

	areas_mask(const areas_mask&) = delete;
	areas_mask & operator = (const areas_mask &) = delete;

	areas_mask(areas_mask&& other)
	{
		if (this == &other)
			return;
		mask = std::move(other.mask);
		_proxies = std::move(other._proxies);
		width = other.width;
		height = other.height;
	}

	areas_mask & operator = (areas_mask &&other)
	{
		if (this != &other){
			mask = std::move(other.mask);
			_proxies = std::move(other._proxies);
			width = other.width;
			height = other.height;
		}
		return (*this);
	}


public:
	void set_size(uint16_t width, uint16_t height)
	{
		this->width = width;
		this->height = height;
		mask.resize(width*height, 0);
	}
	void clear() 
	{
		_proxies.clear();
		std::memset(mask.data(), 0, sizeof(mask_item_t)*mask.size()); 
	}

	bool add_area(const std::unique_ptr<AreaBase> &area)
	{
		auto area_points = area->get_area_sprite(width,height);
		if (area_points.empty())
			return false;
		std::list<AreasProxy*> created_proxies;
		for (auto & point : area_points)
		{
			if (point.first < 0 || point.second < 0)
				continue;
			if (point.first >= width || point.second >= height)
				continue;


			auto p_item = &mask[point.second*width + point.first];

			auto area_key = area.get();

			if (IS_AREA_MASK_ITEM_SET(p_item))
			{
				auto current_item = GET_AREA_MASK_ITEM_KEY(p_item);
				std::vector<AreaBase*> item_areas({ current_item, area_key });
				if (current_item->is_proxy())
				{
					item_areas = reinterpret_cast<AreasProxy*>(current_item)->areas();
					item_areas.push_back(area_key);
				}

				auto find_index = std::find_if(created_proxies.cbegin(), created_proxies.cend(), [&item_areas](const AreasProxy* area)
				{
					return item_areas == area->areas();

				});

				std::unique_ptr<AreasProxy> area_proxy;
				bool add_to_proxies{ false };
				if (find_index == created_proxies.cend())
				{
					area_proxy = std::make_unique<AreasProxy>();

					if (current_item->is_proxy())
					{
						auto prev_proxy = reinterpret_cast<AreasProxy*>(current_item);
						
						if (prev_proxy->unique())
						{
							_proxies.remove_if([prev_proxy](const areas_proxy_ptr_t& areas_proxy)
							{
								return areas_proxy.get() == prev_proxy;
							});
						}
						else
							prev_proxy->sub_ref();
					}

					area_proxy->set_areas(std::move(item_areas));
					created_proxies.push_back(area_proxy.get());
					add_to_proxies = true;
				}
				else
				{
					area_proxy = std::unique_ptr<AreasProxy>(*find_index);
				}

				area_proxy->add_ref();

				area_key = reinterpret_cast<AreaBase*>(area_proxy.get());

				const auto area_proxy_ptr = area_proxy.release();
				if (add_to_proxies)
					_proxies.emplace_back(area_proxy_ptr);
			}

			SET_AREA_MASK_ITEM_KEY(p_item, area_key);

		}
		return true;
	}

	//void delete_area(const std::vector<point_coordinate>& area_points)
	//{
	//	for (auto & point : area_points)
	//	{
	//		if (point.first < 0 || point.second < 0)
	//			continue;
	//		if (point.first >= width || point.second >= height)
	//			continue;

	//		mask[point.second*width + point.first] = 0;
	//	}
	//}


public:
	std::vector<mask_item_t> mask;

	uint16_t width;
	uint16_t height;

public:
	inline mask_key_t get_key(const mask_item_t* item) const { return GET_AREA_MASK_ITEM_KEY(item); }

private:
	std::list<areas_proxy_ptr_t> _proxies;
};


class areas_dispatcher
{
	using area_ptr_t = std::unique_ptr<AreaBase>;

public:
	areas_dispatcher()
	{}

	areas_dispatcher(const areas_dispatcher&) = delete;
	areas_dispatcher & operator = (const areas_dispatcher &) = delete;

	areas_dispatcher(areas_dispatcher&& other)
	{
		if (this == &other)
			return;
		std::lock_guard<decltype(_lock_areas)> lock(_lock_areas);
		std::lock_guard<decltype(_lock_areas)> lock_other(other._lock_areas);
		_areas = std::move(other._areas);
		_areas_mask = std::move(other._areas_mask);
	}

	areas_dispatcher & operator = (areas_dispatcher &&other)
	{
		if (this != &other){
			std::lock_guard<decltype(_lock_areas)> lock(_lock_areas);
			std::lock_guard<decltype(_lock_areas)> lock_other(other._lock_areas);
			_areas = std::move(other._areas);
			_areas_mask = std::move(other._areas_mask);
		}
		return (*this);
	}
private:
	std::vector<area_ptr_t> _areas;
	areas_mask _areas_mask;
	mutable std::mutex _lock_areas;

private:
	const AreaBase * get_area_by_id(const std::vector<area_ptr_t>& container, int id)
	{
		auto find_index = std::find_if(container.cbegin(), container.cend(), [id](const area_ptr_t& area)->bool{return area->id == id; });
		if (find_index == container.cend())
			return nullptr;

		return find_index->get();
	}

public:
	void lock() { _lock_areas.lock(); }
	void unlock() { _lock_areas.unlock(); }
	areas_mask& get_areas_mask() { return _areas_mask; }

	void set_default_areas()
	{
		for (auto & area : _areas){
			area->reset(); area->is_valid = false;
		}
	}

public:
	bool Empty() { return _areas.empty(); }


	bool get_area_temperature_measure(int area_id, area_temperature_measure &measure)
	{
		bool res = false;
		std::lock_guard<decltype(_lock_areas)> lock(_lock_areas);
		auto area = get_area_by_id(_areas, area_id);
		if (area != nullptr && area->is_valid)
		{
			res = true;
			measure.min = area->m_min;
			measure.max = area->m_max;
			measure.avr = (float)area->m_avr;
		}

		return res;
	}

	template<class TFunc>
	void for_each_area_temperature_measure(const TFunc &functor) const//area_temperature_measure &measure)
	{
		std::lock_guard<decltype(_lock_areas)> lock(_lock_areas);
		std::for_each(_areas.cbegin(), _areas.cend(), [&functor](const area_ptr_t& area)
		{
			if (area->is_valid)
			{
				area_temperature_measure measure = { area->m_min, area->m_max, (float)area->m_avr };
				functor(measure);
			}
		}
		);
	}


	void set_areas_mask_size(short width, short height){ _areas_mask.set_size(width, height); }
	void get_areas_mask_size(short &width, short &height) const { width = _areas_mask.width; height = _areas_mask.height; }

	void clear_areas() {
		std::lock_guard<decltype(_lock_areas)> lock(_lock_areas);
		_areas.clear(); _areas_mask.clear();
	}

private:
	template<class TArea>
	bool add_area(const TArea &area)
	{
		std::lock_guard<decltype(_lock_areas)> lock(_lock_areas);

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
		std::lock_guard<decltype(_lock_areas)> lock(_lock_areas);

		auto del_index = std::find_if(_areas.cbegin(), _areas.cend(), [id](const area_ptr_t& area)->bool{return area->id == id; });
		if (del_index != _areas.cend())
		{
			_areas.erase(del_index);
			_areas_mask.clear();
			for (auto & area : _areas)
			{
				_areas_mask.add_area(area);
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


