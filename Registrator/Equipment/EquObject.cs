using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    public class EquObject : EquDbObject
    {
        EquClass  m_class  = null;
        EquGroup  m_group  = null;
        EquLine   m_line   = null;
        EquLayout m_layout = null;
        EquPath   m_path   = null;
        EquPicket m_picket = null;

        Area m_objArea = null;
        Area m_deltaAreaFirst = null;
        Area m_deltaAreaSecond = null;
        
        float m_offset;
        byte m_state = 0;
        bool m_isLeft = false;

        public int strelkaDirection;
        public int typeEquip = 0;

        public int X { get; set; }
        public int Y { get; set; }
       
        public EquObject(   int code,
                            String name,
                            EquPicket picket,
                            float offset
                        )
            : base(code, name, picket)
        {
            
            m_picket = picket;
            m_path = picket.Path;
            m_line = m_path.Line;
            m_group = m_line.Group;
            m_class = m_group.Class;
            m_offset = offset;
            X = 0;
            Y = 0;
            ObjectLenght = 0;

        }
        public int MaxTemperature{get;set;}
        public int ObjectLenght { get; set; }
        public byte State  { get { return m_state;  } set { m_state = value; }}
        public Area ObjectArea { get{ return m_objArea; } set{ m_objArea = value;} }
        public Area DeltaAreaFirst { get{ return m_deltaAreaFirst; }  set{ m_deltaAreaFirst = value; } }
        public bool IsLeft { get { return m_isLeft; } set { m_isLeft = value; } }
        public Area DeltaAreaSecond { get{ return m_deltaAreaSecond; } set{ m_deltaAreaSecond = value; } }
        public float DeltaTempMin()
        {
            float res = 0f;

            if (m_deltaAreaFirst != null && m_deltaAreaSecond != null)
            {
                if (m_deltaAreaFirst.MinTemp >= m_deltaAreaSecond.MinTemp)
                    res = Math.Abs(m_deltaAreaFirst.MinTemp - m_deltaAreaSecond.MinTemp);
                else
                    res = Math.Abs(m_deltaAreaSecond.MinTemp - m_deltaAreaFirst.MinTemp);
            }

            return res;
        }

        public float DeltaTempAvr()
        {
            float res = 0f;

            if (m_deltaAreaFirst != null && m_deltaAreaSecond != null)
            {
                if (m_deltaAreaFirst.AvrgTemp >= m_deltaAreaSecond.AvrgTemp)
                    res = Math.Abs(m_deltaAreaFirst.AvrgTemp - m_deltaAreaSecond.AvrgTemp);
                else
                    res = Math.Abs(m_deltaAreaSecond.AvrgTemp - m_deltaAreaFirst.AvrgTemp);
            }

            return res;
        }

        public float DeltaTempMax()
        {

            float res = 0f;

            if (m_deltaAreaFirst != null && m_deltaAreaSecond != null)
            {
                if (m_deltaAreaFirst.MaxTemp >= m_deltaAreaSecond.MaxTemp)
                    res = Math.Abs(m_deltaAreaFirst.MaxTemp - m_deltaAreaSecond.MaxTemp);
                else
                    res = Math.Abs(m_deltaAreaSecond.MaxTemp - m_deltaAreaFirst.MaxTemp);
            }

            return res;
        }

        public EquClass Class
        {
            get { return m_class; }
            set { m_class = value; }
        }
        public EquGroup Group
        {
            get { return m_group; }
            set { m_group = value; }
        }

        public EquLayout Layout
        {
            get { return m_layout; }
            set { m_layout = value; }
        }
        public EquLine Line
        {
            get { return m_line; }
            set { m_line = value; }
        }
        public EquPath Path
        {
            get{ return m_path;}
            set { m_path = value; }
        }

        public EquPicket Picket
        {
            get  { return m_picket; }
            set  { m_picket = value; }
        }

        public float Offset { get{ return m_offset; } set{ m_offset = value; } }
    }
}
