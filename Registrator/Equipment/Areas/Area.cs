using System;
using System.Collections.Generic;
using System.Text;
using DrawToolsLib;
using System.Drawing;

namespace Registrator
{

    public class area_traits
    {
        public area_traits(Area.AreaType type)
        {
            availible = false;
            if (type == Area.AreaType.AREA_ELLIPS || type == Area.AreaType.AREA_RECT)
                availible = true;
        }

        public area_traits(ToolType type)
        {
            availible = false;
            if (type == ToolType.Ellipse || type == ToolType.Rectangle)
                availible = true;
        }

        public bool availible { get; private set; }
    }

    //public class area_factory
    //{
    //    public static AreaBase create_area<TArea>(int id) where TArea : AreaBase
    //    {
    //        return new TArea(id);
    //    }
    //}


    public class Area : DbObject
    {

        public enum AreaType
        {
            AREA_RECT,
            AREA_ELLIPS,
            AREA_FREE
        }

        public event EventHandler<AreaAlarmedEvent> AreaAlarmedHandler;

        private String m_name = "";

        int m_progID = -1;

        private AreaType m_areaType = AreaType.AREA_RECT;
        private double m_width = 0;
        private double m_height = 0;
        private double m_x = 0;
        private double m_y = 0;

        private bool m_isMinTempVisible = false;
        private bool m_isMaxTempVisible = false;
        private bool m_isAvrgTempVisible = false;

        private bool m_isMinTempAlarmed = false;
        private bool m_isMaxTempAlarmed = false;
        private bool m_isAvrgTempAlarmed = false;

        private float m_borderMinTemp = 50;
        private float m_borderMaxTemp = 50;
        private float m_borderAvrgTemp = 50;

        private float m_minTemp = 0;
        private float m_maxTemp = 0;
        private float m_avrgTemp = 0;

        private Point[] m_freeForm; 

        public Area() {}

        public int ProgID { get { return m_progID; } set { m_progID = value; } }
        public String Name { get { return m_name; } set { m_name = value; } }
        public AreaType Type { get { return m_areaType; } set { m_areaType = value; } }
        public double Height { get { return m_height; } set { m_height = value; } }
        public double Width { get { return m_width; } set { m_width = value; } }
        public double X { get { return m_x; } set { m_x = value; } }
        public double Y { get { return m_y; } set { m_y = value; } }

        public Point[] FreeForm { get { return m_freeForm; } set { m_freeForm = value; } }
        public String FreeFormForDB()
        {
            if (m_freeForm == null)
                return "";
            
            String str = "";

            for (int i = 0; i < m_freeForm.Length; i++ )
            {
                str += m_freeForm[i].X + ";" + m_freeForm[i].Y; 
            }

            return str;
        }

        public bool MinTempVisibility { get { return m_isMinTempVisible; } set { m_isMinTempVisible = value; } }
        public bool MaxTempVisibility { get { return m_isMaxTempVisible; } set { m_isMaxTempVisible = value; } }
        public bool AvrgTempVisibility { get { return m_isAvrgTempVisible; } set { m_isAvrgTempVisible = value; } }
        public bool MinTempAlarmed { get { return m_isMinTempAlarmed; } set { m_isMinTempAlarmed = value; } }
        public bool MaxTempAlarmed { get { return m_isMaxTempAlarmed; } set { m_isMaxTempAlarmed = value; } }
        public bool AvrgTempAlarmed { get { return m_isAvrgTempAlarmed; } set { m_isAvrgTempAlarmed = value; } }
        public float MinTempBorder { get { return m_borderMinTemp; } set { m_borderMinTemp = value; } }
        public float MaxTempBorder { get { return m_borderMaxTemp; } set { m_borderMaxTemp = value; } }
        public float AvrgTempBorder { get { return m_borderAvrgTemp; } set { m_borderAvrgTemp = value; } }

        public float MinTemp
        { 
            get { return m_minTemp; } 
            set
            {
                m_minTemp = value;
                if (m_minTemp > m_borderMinTemp)
                    FireAreaAlarmedEvent(new AreaAlarmedEvent(this, AreaAlarmedEvent.AlarmType.TEMP_MIN, m_minTemp, m_borderMinTemp));
            }
        }

        public float MaxTemp 
        {
            get { return m_maxTemp; } 
            set
            {
                m_maxTemp = value;
                if (m_maxTemp > m_borderMaxTemp)
                    FireAreaAlarmedEvent(new AreaAlarmedEvent(this, AreaAlarmedEvent.AlarmType.TEMP_MAX, m_maxTemp, m_borderMaxTemp));
            }
        }

        public float AvrgTemp 
        { 
            get { return m_avrgTemp; }
            set
            {
                m_avrgTemp = value;
                if (m_avrgTemp > m_borderAvrgTemp)
                    FireAreaAlarmedEvent(new AreaAlarmedEvent(this, AreaAlarmedEvent.AlarmType.TEMP_AVRG, m_avrgTemp, m_borderAvrgTemp));
            }
        }

        public virtual void FireAreaAlarmedEvent(AreaAlarmedEvent e)
        {
            EventHandler<AreaAlarmedEvent> handler = AreaAlarmedHandler;

            if (handler != null)
            {
                handler(this, e);
            }
        }

    }

    public class AreaAlarmedEvent : EventArgs
    {

        public enum AlarmType
        {
            TEMP_MIN,
            TEMP_MAX,
            TEMP_AVRG
        }

        private Area m_area;
        private AlarmType m_alarmType;
        private float m_value;
        private float m_border;

        public AreaAlarmedEvent()
            : base()
        {
        }

        public AreaAlarmedEvent(Area area, AlarmType type, float val, float border)
            : this()
        {
            m_area = area;
            m_alarmType = type;
            m_value = val;
            m_border = border;
        }

        public Area Area { get { return m_area; } set { m_area = value; } }

        public AlarmType Type { get { return m_alarmType; } set { m_alarmType = value; } }

        public float Value { get { return m_value; } set { m_value = value; } }

        public float Border { get { return m_border; } set { m_border = value; } }

    }
}
