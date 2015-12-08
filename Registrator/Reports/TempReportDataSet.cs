using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;

namespace Registrator
{
    
    public class TempReportDataSet 
    {

        String m_title = "";
        DateTime m_date;
        String m_person = "";
        bool m_chart = true;
        bool m_table = true;
        ArrayList m_rows = new ArrayList();

        public TempReportDataSet()
            : base()
        {

        }
        public String Title { get { return m_title; } set { m_title = value; } }
        public DateTime Date { get { return m_date; } set { m_date = value; }}
        public String Person { get { return m_person; } set { m_person = value; } }
        public ArrayList Rows { get { return m_rows; } set { m_rows = value; } }
        public bool Chart { get{ return m_chart;} set { m_chart = value; } }
        public bool Table { get{ return m_table; } set { m_table = value;} }
    }
}
