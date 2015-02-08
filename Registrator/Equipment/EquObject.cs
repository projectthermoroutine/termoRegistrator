using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    public class EquObject
    {

        private int m_id = -1;
        private int m_code = -1;
        private String m_name = "";
        private EquGroup m_group;
        private EquLayout m_layout;
        private int m_path;
        private int m_picket;
        private float m_offset;

        Area m_objArea = null;
        Area m_deltaAreaFirst = null;
        Area m_deltaAreaSecond = null;

        private byte m_state = 0;

        bool m_isLeft = false;

        public EquObject() 
            : base()
        {

        }

        public EquObject(
                            int code,
                            String name,
                            EquGroup group,
                            EquLayout layout,
                            int path,
                            int picket,
                            float offset
                        )
            : this()
        {
            m_code = code;
            m_name = name;
            m_group = group;
            m_layout = layout;
            m_path = path;
            m_picket = picket;
            m_offset = offset;
        }

        public EquObject(
                            int id,
                            int code,
                            String name,
                            EquGroup group,
                            EquLayout layout,
                            int path,
                            int picket,
                            float offset
                        )
            : this()
        {
            m_id = id;
            m_code = code;
            m_name = name;
            m_group = group;
            m_layout = layout;
            m_path = path;
            m_picket = picket;
            m_offset = offset;
        }

        public byte State
        {
            get
            {
                return m_state;
            }

            set
            {
                m_state = value;
            }
        }

        public Area ObjectArea
        {
            get
            {
                return m_objArea;
            }

            set
            {
                m_objArea = value;
            }
        }

        public Area DeltaAreaFirst
        {
            get
            {
                return m_deltaAreaFirst;
            }

            set
            {
                m_deltaAreaFirst = value;
            }

        }

        public bool IsLeft
        {
            get
            {
                return m_isLeft;
            }

            set
            {
                m_isLeft = value;
            }
        }

        public Area DeltaAreaSecond
        {
            get
            {
                return m_deltaAreaSecond;
            }

            set
            {
                m_deltaAreaSecond = value;
            }

        }

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

        public int ID
        {
            get
            {
                return m_id;
            }

            set
            {
                m_id = value;
            }
        }

        public int Code
        {
            get
            {
                return m_code;
            }

            set
            {
                m_code = value;
            }
        }

        public String Name
        {
            get
            {
                return m_name;
            }

            set
            {
                m_name = value;
            }
        }

        public EquGroup Group
        {
            get
            {
                return m_group;
            }

            set
            {
                m_group = value;
            }
        }

        public EquLayout Layout
        {
            get
            {
                return m_layout;
            }

            set
            {
                m_layout = value;
            }
        }

        public int Path
        {
            get
            {
                return m_path;
            }

            set
            {
                m_path = value;
            }
        }

        public int Picket
        {
            get
            {
                return m_picket;
            }

            set
            {
                m_picket = value;
            }
        }

        public float Offset
        {
            get
            {
                return m_offset;
            }

            set
            {
                m_offset = value;
            }
        }

        public int Line
        {
            get
            {
                return (int)(m_layout.Code / 100000);
            }
        }

        public void LoadMeasures()
        {

        }

    }
}
