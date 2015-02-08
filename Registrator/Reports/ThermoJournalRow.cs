using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    public class ThermoJournalRow
    {
        private int num = 1;

        public int Num
        {
            get { return num; }
            set { num = value; }
        }
        private string date = "22.12.2010";

        public string Date
        {
            get { return date; }
            set { date = value; }
        }
        private string equipment = "ЛР011";

        public string Equipment
        {
            get { return equipment; }
            set { equipment = value; }
        }
        private string line_path_pick = "1 линия 2 путь";

        public string Line_path_pick
        {
            get { return line_path_pick; }
            set { line_path_pick = value; }
        }
        private float object_condition = 2.1f;

        public float Object_condition
        {
            get { return object_condition; }
            set { object_condition = value; }
        }
        private string service = "Электроснабжения";

        public string Service
        {
            get { return service; }
            set { service = value; }
        }
        private string date_time_def = "22.12.2010 10.30";

        public string Date_time_def
        {
            get { return date_time_def; }
            set { date_time_def = value; }
        }
        private string date_time_avdef = "";

        public string Date_time_avdef
        {
            get { return date_time_avdef; }
            set { date_time_avdef = value; }
        }
        private string who = "";

        public string Who
        {
            get { return who; }
            set { who = value; }
        }

        private string planned_date = "30.12.2010";

        public string Planned_date
        {
            get { return planned_date; }
            set { planned_date = value; }
        }
        private string fact_date = "29.12.2010";

        public string Fact_date
        {
            get { return fact_date; }
            set { fact_date = value; }
        }

        private int[] xaxe = new int[] { 1, 2, 3, 4 };

        public int[] Xaxe
        {
            get { return xaxe; }
            set { xaxe = value; }
        }
        private int[] yaxe = new int[] { 2, 3, 5, 7 };

        public int[] Yaxe
        {
            get { return yaxe; }
            set { yaxe = value; }
        }
    }
}
