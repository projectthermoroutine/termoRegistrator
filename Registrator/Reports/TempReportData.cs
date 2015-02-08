using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    public class TempReportData
    {
        int m_number = 1;
        DateTime m_date;
        float m_tempMin = 0;
        float m_tempAvr = 0;
        float m_tempMax = 0;

        String m_state = "Исправно";

        public TempReportData() 
            : base()
        {

        }

        public int Number
        {
            get
            {
                return m_number;
            }

            set
            {
                m_number = value;
            }
        }

        public DateTime Date 
        {
            get
            {
                return m_date;
            }

            set
            {
                m_date = value;
            }
        }

        public String DateStr
        {
            get
            {
                return m_date.ToShortDateString();
            }
        }

        public float TempMin
        {
            get
            {
                return m_tempMin;
            }

            set
            {
                m_tempMin = value;
            }
        }

        public float TempAvr
        {
            get
            {
                return m_tempAvr;
            }

            set
            {
                m_tempAvr = value;
            }
        }

        public float TempMax
        {
            get
            {
                return m_tempMax;
            }

            set
            {
                m_tempMax = value;
            }

        }

        public String State
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

    }
}
