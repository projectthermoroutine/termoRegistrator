using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    using mask_key_t = AreaBase;
    using mask_item_t = AreaBase;


    public struct point_t
    {
        public point_t(short x, short y)
        {
            this.x = x;
            this.y = y;
        }
        public short x;
        public short y;
    }
    abstract public class AreaBase
    {

        public int id;
        public float m_min;
        public float m_max;
        public float m_avr;
        public float m_summary;
        public int pixelsCounter;
        public bool is_valid;
        public AreaBase()
        {
            is_valid = false;
            id = -1;
            reset();
        }
        public AreaBase(int id) : this()
        {
            this.id = id;
        }

        public void reset()
        {
            m_min = 1000;
            m_max = -1000;
            m_avr = 0;
            m_summary = 0;
            pixelsCounter = 0;
        }
        public override bool Equals(object obj)
        {
            return id == ((AreaBase)obj).id;
        }

        public override int GetHashCode()
        {
            return id;
        }

        public abstract bool IsInTheArea(int x, int y);
        public virtual void SetTemp(float temp)
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
        public virtual void SetTemp(int x, int y, float temp)
        {
            if (!IsInTheArea(x, y))
                return;
            SetTemp(temp);
        }


        public abstract List<point_t> get_area_sprite();

    };

    public class AreaEllips : AreaBase
    {
        short _x0;
        short _y0;
        short _a;
        short _b;
        float _e;

        short _a2;
        short _b2;


        public AreaEllips(int id, short x, short y, short a, short b) : base(id)
        {
            _x0 = x;
            _y0 = y;
            _a = (short)(a / (short)2);
            _b = (short)(b / (short)2);
            _a2 = (short)(_a * _a);
            _b2 = (short)(_b * _b);
            _e = (float)Math.Sqrt(1.0 - _b2 / _a2);
        }
        public override bool IsInTheArea(int x, int y)
        {
            int dX = _x0 + _a;
            int dY = _y0 + _b;
            return ((x - dX) ^ 2) / _a2 + ((y - dY) ^ 2) / _b2 <= 1;
        }

        public override List<point_t> get_area_sprite()
        {
            List<point_t> points = new List<point_t>();

            for (short y = (short)(_y0 - _b); y < _y0 + _b; y++)
            {
                int x1 = (int)Math.Sqrt(_a2 * (1 - y / _b2));
                for (short x = (short)(_x0 - x1); x <= _x0 + x1; x++)
                {
                    points.Add(new point_t(x, y));
                }
            }

            return points;
        }


    };

    public class AreaRect : AreaBase
    {
        short m_x;
        short m_y;
        ushort m_width;
        ushort m_height;
        public AreaRect() : base()
        {
            m_x = 0;
            m_y = 0;
            m_width = 0;
            m_height = 0;
        }
        public AreaRect(int id, short x, short y, ushort width, ushort height)
            : base(id)
        {
            m_x = x;
            m_y = y;
            m_width = width;
            m_height = height;
        }

        public override bool IsInTheArea(int x, int y)
        {
            return (x > m_x) && (x < (m_x + m_width)) && (y > m_y) && (y < (m_y + m_height));
        }


        public override List<point_t> get_area_sprite()
        {
            List<point_t> points = new List<point_t>();

            for (short y = m_y; y < m_y + m_height; y++)
                for (short x = m_x; x < m_x + m_width; x++)
                {
                    points.Add(new point_t(x, y));
                }

            return points;
        }

    };


    public class areas_mask
    {
        public mask_item_t[] mask;

        public short width;
        public short height;

        public areas_mask()
        {
            width = height = 0;
        }

        public void set_size(short width, short height)
        {
            this.width = width;
            this.height = height;
            mask = new mask_item_t[width * height];
        }
        public void clear() { mask = new mask_item_t[width * height]; }

        public void add_area(List<point_t> area_points, mask_key_t area_key)
        {
            foreach (var point in area_points)
            {
                if (point.x < 0 || point.y < 0)
                    continue;
                if (point.x >= width || point.y >= height)
                    continue;

                mask[point.y * width + point.x] = area_key;
            }
        }

        public void delete_area(List<point_t> area_points)
        {
            foreach (var point in area_points)
            {
                if (point.x < 0 || point.y < 0)
                    continue;
                if (point.x >= width || point.y >= height)
                    continue;

                mask[point.y * width + point.x] = null;
            }
        }

        public mask_key_t get_key(short x, short y) { return mask[y * width + x]; }
    };

    public class area_temperature_measure_t
    {
        public float min;
        public float max;
        public float avr;
    };

    public class areas_dispatcher
    {
        public areas_dispatcher()
        {
            _areas = new List<mask_item_t>();
            _areas_mask = new areas_mask();
        }
        List<AreaBase> _areas;
        areas_mask _areas_mask;

        AreaBase get_area_by_id(List<AreaBase> container, int id)
        {
            return container.Find(delegate (AreaBase area) { return area.id == id; });
        }
        public void set_default_areas()
        {
            foreach (var area in _areas)
            {
                area.reset(); area.is_valid = false;
            }

        }

        public bool Empty() { return _areas.Count == 0; }

        public AreaBase getArea(short x, short y) { return _areas_mask.get_key(x, y); }

        public bool get_area_temperature_measure(int area_id, area_temperature_measure_t measure)
        {
            bool res = false;
            var area = get_area_by_id(_areas, area_id);
            if (area != null && area.is_valid)
            {
                res = true;
                measure.min = area.m_min;
                measure.max = area.m_max;
                measure.avr = area.m_avr;
            }

            return res;
        }

        public void set_areas_mask_size(short width, short height) { _areas_mask.set_size(width, height); }
        public void clear_areas() { _areas.Clear(); _areas_mask.clear(); }

        public void AddArea(AreaBase area) { _areas.Add(area); _areas_mask.add_area(area.get_area_sprite(), area); }
        public void DelArea(short id)
        {
            var area = get_area_by_id(_areas, id);
            if (area != null)
            {
                _areas.Remove(area);
                _areas_mask.clear();
                foreach (var _area in _areas)
                {
                    _areas_mask.add_area(_area.get_area_sprite(), _area);
                }
            }
        }

        public void ChangeArea(short id, AreaBase area)
        {
            DelArea(id);
            AddArea(area);
        }

    }

}
