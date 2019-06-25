using System;
using System.Collections.Generic;
using ThermoRoutineLib;
using DrawToolsLib;
using System.Windows.Input;

namespace Registrator
{
    public partial class PlayerPanel
    {

        public event EventHandler<AreaAddedEvent> AreaAddedEventHandler;
        public event EventHandler<AreaChangedEvent> AreaChangedEventHandler;

        private bool get_area_temperature_measure(int area_id, out _area_temperature_measure measure, out _point maxT_Point, out _point minT_Point)
        {
            measure = new _area_temperature_measure();
            maxT_Point = new _point();
            minT_Point = new _point();

            return _frame_data_helper.get_area_info(area_id, out measure, out maxT_Point, out minT_Point);
        }


        private void get_areas_temperature_measure()
        {
            List<int> result_ids = new List<int>();
            _area_temperature_measure measure = new _area_temperature_measure();
            var maxT_Point = new _point();
            var minT_Point = new _point();

            for (int i = 0; i < m_areasPanel.Template.Areas.Count; i++)
            {
                if (_frame_data_helper.get_area_info(m_areasPanel.Template.Areas[i].ProgID, out measure, out maxT_Point, out minT_Point))
                {
                    Area area = m_areasPanel.Template.Areas[i] as Area;
                    if (area == null)
                        continue;
                    area.MinTemp = measure.min;
                    area.MaxTemp = measure.max;
                    area.AvrgTemp = measure.avr;

                    result_ids.Add(i);
                }
            }

            m_areasPanel.UpdateAreasRows(result_ids);
        }
        private void get_areas_temperature_measure2(areas_dispatcher areas_dispatcher)
        {

            List<int> result_ids = new List<int>();
            area_temperature_measure_t measure = new area_temperature_measure_t();
            for (int i = 0; i < m_areasPanel.Template.Areas.Count; i++)
            {
                var res = areas_dispatcher.get_area_temperature_measure((int)m_areasPanel.Template.Areas[i].ProgID, measure);
                if (res)
                {
                    Area area = m_areasPanel.Template.Areas[i] as Area;
                    if (area == null)
                        continue;
                    area.MinTemp = measure.min;
                    area.MaxTemp = measure.max;
                    area.AvrgTemp = measure.avr;

                    result_ids.Add(i);
                }
            }

            m_areasPanel.UpdateAreasRows(result_ids);

        }

        UInt32 _canvas_mouse_events_connection_refs = 0;

        void connect_Canvas_MouseEvents()
        {
            ++_canvas_mouse_events_connection_refs;
            if (_canvas_mouse_events_connection_refs == 1)
            {
                m_playerControl.drawingCanvas.MouseLeftButtonUp += new System.Windows.Input.MouseButtonEventHandler(areas_Canvas_MouseLeftButtonUp);
            }
        }
        void disconnect_Canvas_MouseEvents()
        {
            --_canvas_mouse_events_connection_refs;
            if (_canvas_mouse_events_connection_refs == 0)
            {
                m_playerControl.drawingCanvas.MouseLeftButtonUp -= new System.Windows.Input.MouseButtonEventHandler(areas_Canvas_MouseLeftButtonUp);
            }

            _is_cursor_position_valid = false;
            if (InvokeRequired)
                Invoke(new EventHandler(delegate
                {
                    m_playerControl.Temperature_label.Visibility = System.Windows.Visibility.Hidden;
                }
                ));
            else
                m_playerControl.Temperature_label.Visibility = System.Windows.Visibility.Hidden;
        }


        struct area_info_t
        {
            public ToolType tool_type;
            public _area_info data;
        };

        void areas_Canvas_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {

            foreach(var item in _new_areas_cache)
            {
                var area_info = item.Value;
                AreaAdded(item.Key, area_info);
                FireAreaAddedEvent(new AreaAddedEvent(item.Key, area_info.tool_type, area_info.data.x0, area_info.data.y0, area_info.data.width, area_info.data.heigth));
            }

            _new_areas_cache.Clear();

            foreach (var item in _areas_cache)
            {
                var area_info = item.Value;
                AreaChanged(item.Key, area_info);
                FireAreaChangedEvent(new AreaChangedEvent(item.Key, area_info.tool_type, area_info.data.x0, area_info.data.y0, area_info.data.width, area_info.data.heigth));
            }

            _areas_cache.Clear();

            disconnect_Canvas_MouseEvents();

            refresh_frame();

        }

        private Dictionary<short, area_info_t> _areas_cache = new Dictionary<short, area_info_t>();
        private Dictionary<short, area_info_t> _new_areas_cache = new Dictionary<short, area_info_t>();
        public void AreaAddedEventFired(object sender, AreaAddedEvent e)
        {
            if (Mouse.LeftButton == MouseButtonState.Pressed)
            {
                connect_Canvas_MouseEvents();

                var area_info = create_area_info(e.Type, (short)e.X, (short)e.Y, (short)e.Width, (short)e.Height);
                if (area_info.data.type == (_area_type)(-1))
                    return;

                _new_areas_cache.Add((short)e.ID, area_info);
            }
        }

        public void AreaChangedEventFired(object sender, AreaChangedEvent e)
        {

            connect_Canvas_MouseEvents();

            var area_info = create_area_info(e.Type, (short)e.X, (short)e.Y, (short)e.Width, (short)e.Height);
            if (area_info.data.type == (_area_type)(-1))
                return;

            if (_new_areas_cache.ContainsKey((short)e.Id))
            {
                _new_areas_cache[(short)e.Id] = area_info;
            }
            else
            {
                _areas_cache[(short)e.Id] = area_info;
            }
        }

        public virtual void FireAreaChangedEvent(AreaChangedEvent e)
        {
            AreaChangedEventHandler?.Invoke(this, e);
        }

        public virtual void FireAreaAddedEvent(AreaAddedEvent e)
        {
            AreaAddedEventHandler?.Invoke(this, e);

        }

        public void AreaToolChanged(object sender, AreaToolEvent e)
        {
            area_traits traits = new area_traits(e.AreaType);
            if (!traits.availible)
                return;

            ToolType ttype = ToolType.Rectangle;

            if (e.AreaType == Area.AreaType.AREA_ELLIPS)
                ttype = ToolType.Ellipse;

            m_playerControl.drawingCanvas.Tool = ttype;
        }

        public void AreasTemplateSelectedEventFired(object sender, AreasTemplateSelectedEvent e)
        {
            _movie_transit.RemoveAllAreas();
            m_tvHandler.RemoveAllAreas();

            m_playerControl.RemoveAllAreas();

            for (int i = 0; i < e.Template.Areas.Count; i++)
            {
                try
                {
                    AreaBase areaBase;
                    Area area = (Area)e.Template.Areas[i];
                    _area_info area_info = new _area_info();
                    area_info.x0 = (short)area.X;
                    area_info.y0 = (short)area.Y;
                    area_info.width = (ushort)area.Width;
                    area_info.heigth = (ushort)area.Height;

                    if (area.Type == Area.AreaType.AREA_RECT)
                    {
                        area_info.type = _area_type.RECTANGLE;
                        areaBase = new AreaRect(i, (short)area.X, (short)area.Y, (ushort)area.Width, (ushort)area.Height);

                    }
                    else if (area.Type == Area.AreaType.AREA_ELLIPS)
                    {
                        area_info.type = _area_type.ELLIPSE;
                        areaBase = new AreaEllips(i, (short)area.X, (short)area.Y, (short)area.Width, (short)area.Height);
                    }
                    else
                        continue;

                    _movie_transit.AddArea((short)i, ref area_info);
                    m_tvHandler.AddArea((short)i, ref area_info);
                    _grabber_areas_dispatcher.AddArea(areaBase);


                    m_playerControl.DrawArea(area);
                }
                catch (ArgumentException)
                {
                    return;
                }
 
            }


            refresh_frame();
        }

        area_info_t create_area_info(ToolType type, double x, double y, double w, double h)
        {
            area_info_t area_info = new area_info_t();
            area_info.data.type = (_area_type)(-1);
            area_traits traits = new area_traits(type);
            if (!traits.availible)
                return area_info;

            area_info.tool_type = type;
            area_info.data.type = _area_type.RECTANGLE;
            if (type == ToolType.Ellipse)
                area_info.data.type = _area_type.ELLIPSE;

            area_info.data.x0 = (short)x;
            area_info.data.y0 = (short)y;
            area_info.data.width = (ushort)w;
            area_info.data.heigth = (ushort)h;
            return area_info;

        }

        public delegate void AreaControlDelegate(int id, _area_info area_info);
        void AreaAdded(int id, area_info_t area_info)
        {
            if (InvokeRequired)
            {
                BeginInvoke(new AreaControlDelegate(AddNewArea), new object[] { id, area_info.data });
            }
            else
            {
                AddNewArea(id,area_info.data);
            }
        }
        void AddNewArea(int id, _area_info area_info)
        {
            try
            {
                _movie_transit.AddArea((short)id, ref area_info);
                m_tvHandler.AddArea((short)id, ref area_info);
                //               _grabber_areas_dispatcher.AddArea(new AreaRect(id,(short)x, (short)y, (ushort)w, (ushort)h));

            }
            catch (ArgumentException)
            {
                return;
            }
        }

        void AreaChanged(int id, area_info_t area_info)
        {
            if (InvokeRequired)
            {
                BeginInvoke(new AreaControlDelegate(ChangeArea), new object[] { id, area_info.data });
            }
            else
            {
                ChangeArea(id, area_info.data);
            }
        }
        void ChangeArea(int id, _area_info area_info)
        {
            try
            {
                _movie_transit.AreaChanged((short)id, ref area_info);
                m_tvHandler.AreaChanged((short)id, ref area_info);
            }
            catch (ArgumentException)
            {
                return;
            }
        }

        public virtual void AreasDeletedFired(object sender, AreasDeletedEvent e)
        {
            if (_movie_transit != null)
            {
                _area_type area_type;
                foreach (var area_id in e.Areas)
                {
                    _movie_transit.RemoveArea((short)area_id, out area_type);
                    m_tvHandler.RemoveArea((short)area_id, out area_type);
                   // _grabber_areas_dispatcher.DelArea((short)area_id);

                }

            }

            if (m_areasPanel != null && m_areasPanel.Template != null && m_areasPanel.Template.Areas != null)
            {
                get_areas_temperature_measure();
            }
        
        }

        public virtual void AreasDeletedInEditorFired(object sender, AreasDeletedInEditorEvent e)
        {
            if (_movie_transit != null)
            {
                _area_type area_type;
                foreach (var area_id in e.Areas)
                {
                    _movie_transit.RemoveArea((short)area_id, out area_type);
                    m_tvHandler.RemoveArea((short)area_id, out area_type);
                   // _grabber_areas_dispatcher.DelArea((short)area_id);
                }
            }

        }


    }
}
