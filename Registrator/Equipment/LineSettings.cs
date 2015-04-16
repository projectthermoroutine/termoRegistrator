using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace Registrator.Equipment
{
    class LineSettings
    {
        private string m_peregonName = "test1";
        private DB.DataBaseHelper dbHelper;
        private EquLine equLine;
        public LineSettings(DB.DataBaseHelper dbHelper_Arg, EquLine equLine_Arg)
        {
            dbHelper = dbHelper_Arg;

            equLine = equLine_Arg;
        }
        [ReadOnly(true)]
        [DisplayName("код линии")]
        public string LineCode
        {
            get {

                var res = from r in dbHelper.dataTable_Lines.AsEnumerable() where r.LineCode == equLine.LineCode select r;

                if(res.Count() == 1)
                {
                    return res.First().LineCode;
                }

                return ""; 
            }
            set { m_peregonName = value; }
        }
        [ReadOnly(true)]
        [DisplayName(" смещение от начала координат(см)")]
        public long shiftFromBegin
        { 
            get
            {
                var res = from r in dbHelper.dataTable_Lines.AsEnumerable() where r.LineCode == equLine.LineCode select r;

                if (res.Count() == 1)
                {
                    return res.First().StartCoordinate;
                }

                return -1;
            }
            set 
            {
            
            }
        }

    }
}
