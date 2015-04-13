using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;
using System.Runtime.InteropServices;
using System.Threading;
using ThermoRoutineLib;
using System.Collections;
using DrawToolsLib;
using System.IO;
using System.Windows.Input;
using IRControls;
using System.Windows.Threading;
using System.Deployment.Application;
using System.Reflection;

namespace Registrator
{
    public partial class PlayerPanel
    {

        public event EventHandler<AreaAddedEvent> AreaAddedEventHandler;
        public event EventHandler<AreaChangedEvent> AreaChangedEventHandler;

        private void get_areas_temperature_measure()
        {

            List<int> result_ids = new List<int>();
            _area_temperature_measure measure = new _area_temperature_measure();
            for (int i = 0; i < m_areasPanel.Template.Areas.Count; i++)
            {
                bool res = false;
                if (_mode == PlayerMode.CAMERA)
                    res = m_tvHandler.GetAreaInfo((uint)m_areasPanel.Template.Areas[i].ProgID, out measure);
                else
                    res = _movie_transit.GetAreaInfo((uint)m_areasPanel.Template.Areas[i].ProgID, out measure);

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

        public void NewAreaEventFired(object sender, NewAreaEvent e)
        {

            ToolType ttype = ToolType.Rectangle;

            if (e.Area.Type == Area.AreaType.AREA_ELLIPS)
                ttype = ToolType.Ellipse;

            m_playerControl.drawingCanvas.Tool = ttype; //(ToolType)Enum.Parse(typeof(ToolType), ((MenuItem)sender).Tag.ToString());

        }

        public void AreasChangedEventFired(object sender, AreasChangedEvent e)
        {

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
                    areaBase = new AreaRect(i,(short)area.X, (short)area.Y, (ushort)area.Width, (ushort)area.Height);

                }
                else if (area.Type == Area.AreaType.AREA_ELLIPS)
                {
                    area_info.type = _area_type.ELLIPSE;
                    areaBase = new AreaEllips(i,(short)area.X, (short)area.Y, (short)area.Width, (short)area.Height);
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


        public delegate void AddRectAreaDelegate(int id, double x, double y, double w, double h);

        public void AddRectArea(int id, double x, double y, double w, double h)
        {
            if (InvokeRequired)
            {
                BeginInvoke(new AddRectAreaDelegate(AddRectArea), new object[] { id, x, y, w, h });
            }
            else
            {
                try
                {
                _area_info area_info = new _area_info();
                area_info.type = _area_type.RECTANGLE;
                area_info.x0 = (short)x;
                area_info.y0 = (short)y;
                area_info.width = (ushort)w;
                area_info.heigth = (ushort)h;
                _movie_transit.AddArea((short)id, ref area_info);
                m_tvHandler.AddArea((short)id, ref area_info);
 //               _grabber_areas_dispatcher.AddArea(new AreaRect(id,(short)x, (short)y, (ushort)w, (ushort)h));

                }
                catch (ArgumentException)
                {
                    return;
                }
            }
        }

        public delegate void AddEllipsAreaDelegate(int id, double x, double y, double w, double h);

        public void AddEllipsArea(int id, double x, double y, double w, double h)
        {
            if (InvokeRequired)
            {
                BeginInvoke(new AddEllipsAreaDelegate(AddEllipsArea), new object[] { id, x, y, w, h });
            }
            else
            {
                try 
                {
                _area_info area_info = new _area_info();
                area_info.type = _area_type.ELLIPSE;
                area_info.x0 = (short)x;
                area_info.y0 = (short)y;
                area_info.width = (ushort)w;
                area_info.heigth = (ushort)h;
                _movie_transit.AddArea((short)id, ref area_info);
                m_tvHandler.AddArea((short)id, ref area_info);
 //               _grabber_areas_dispatcher.AddArea(new AreaEllips(id,(short)x, (short)y, (short)w, (short)h));

                }
                catch (ArgumentException)
                {
                    return;
                }
           
            }
        }

        public delegate void ChangeRectAreaDelegate(int id, double x, double y, double w, double h);

        public void ChangeRectArea(int id, double x, double y, double w, double h)
        {
            if (InvokeRequired)
            {
                BeginInvoke(new ChangeRectAreaDelegate(ChangeRectArea), new object[] { id, x, y, w, h });
            }
            else
            {
                try
                {
                    _area_info area_info = new _area_info();
                    area_info.type = _area_type.RECTANGLE;
                    area_info.x0 = (short)x;
                    area_info.y0 = (short)y;
                    area_info.width = (ushort)w;
                    area_info.heigth = (ushort)h;
                    _movie_transit.AreaChanged((short)id, ref area_info);
                    m_tvHandler.AreaChanged((short)id, ref area_info);
//                    _grabber_areas_dispatcher.ChangeArea((short)id,new AreaRect(id,(short)x, (short)y, (ushort)w, (ushort)h));

 
                }
                catch (ArgumentException)
                {
                    return;
                }
            }
        }

        public delegate void ChangeEllipsAreaDelegate(int id, double x, double y, double w, double h);

        public void ChangeEllipsArea(int id, double x, double y, double w, double h)
        {
            if (InvokeRequired)
            {
                BeginInvoke(new ChangeEllipsAreaDelegate(ChangeEllipsArea), new object[] { id, x, y, w, h });
            }
            else
            {
                try
                {
                    _area_info area_info = new _area_info();
                    area_info.type = _area_type.ELLIPSE;
                    area_info.x0 = (short)x;
                    area_info.y0 = (short)y;
                    area_info.width = (ushort)w;
                    area_info.heigth = (ushort)h;
                    _movie_transit.AreaChanged((short)id, ref area_info);
                    m_tvHandler.AreaChanged((short)id, ref area_info);
                    //_grabber_areas_dispatcher.ChangeArea((short)id, new AreaEllips(id,(short)x, (short)y, (short)w, (short)h));
                }
                catch (ArgumentException)
                {
                    return;
                }
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
