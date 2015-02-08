using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{

    public class EquMeasure
    {
        int m_id;
        DateTime m_dateTime;
        String m_filePath;

        public EquMeasure()
            : base()
        {

        }

        public EquMeasure(int id, DateTime dateTime, String filePath)
            : this()
        {
            m_id = id;
            m_dateTime = dateTime;
            m_filePath = filePath;
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

        public DateTime DateTime
        {
            get
            {
                return m_dateTime;
            }

            set
            {
                m_dateTime = value;
            }

        }

        public String FilePath
        {
            get
            {
                return m_filePath;
            }

            set
            {
                m_filePath = value;
            }
        }

    }
}
