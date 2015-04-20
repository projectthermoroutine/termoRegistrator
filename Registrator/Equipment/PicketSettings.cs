using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;
using System.Globalization;

namespace Registrator.Equipment
{
    public class PicketSettings
    {
        private DB.DataBaseHelper dbHelper;
        private Picket equPicket;

        public PicketSettings(DB.DataBaseHelper dbHelper_Arg, Picket equObject_Arg)
        {
            dbHelper = dbHelper_Arg;

            equPicket = equObject_Arg;
        }
        [ReadOnly(true)]  
        [DisplayName("номер")]
        public int equipmentKName
        {
            get
            {
                return equPicket.Code;
            }
        }

        [ReadOnly(true)]
        [DisplayName("длинна")]
        public int dlina
        {
            get
            {
                var res = from r in dbHelper.dataTable_PicketsTable.AsEnumerable() where r.Npiketa == equPicket.Code select r;

                int dlina;

                if (res.Count() == 1)
                {
                    dlina = res.First().Dlina;
                    return dlina;
                }
                return -1;
            }
        }

        public event EventHandler<RenameEvent> RenameEventHandler;

        public virtual void FireRename(RenameEvent e)
        {
            EventHandler<RenameEvent> handler = RenameEventHandler;

            if (handler != null)
            {
                handler(this, e);
            }
        }

    }
}
