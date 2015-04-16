using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace Registrator.Equipment
{
    class PeregonProperties
    {
        private string m_peregonName = "test1";
        private DB.DataBaseHelper dbHelper;
        private EquLayout equLayout;
        public PeregonProperties(DB.DataBaseHelper dbHelper_Arg, EquLayout equLayout_Arg)
        {
            dbHelper = dbHelper_Arg;

            equLayout = equLayout_Arg;
        }

        [ReadOnly(true)]
        [DisplayName("название")]
        public string peregonKName
        {
            get {

                var res = from r in dbHelper.dataTable_LayoutTable.AsEnumerable() where r.Code == equLayout.Code select r;

                if(res.Count() == 1)
                {
                    return res.First().Layout;
                }

                return ""; 
            }
            set { m_peregonName = value; }
        }
        [ReadOnly(true)]
        [DisplayName("длина перегона(см)")]
        public int peregonLength
        {
            get 
            {
                var res = from r in dbHelper.dataTable_LayoutTable.AsEnumerable() where r.Code == equLayout.Code select r;

                if (res.Count() == 1)
                {
                    return res.First().PeregonLength;
                }

                return -1;
            }
            set
            { }

        }
    }
}
