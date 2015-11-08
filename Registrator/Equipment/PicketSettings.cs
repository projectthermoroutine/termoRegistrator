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
        private DB.metro_db_controller _db_controller;
        private EquPicket equPicket;

        public PicketSettings(DB.metro_db_controller db_controller)
        {
            _db_controller = new DB.metro_db_controller(db_controller);
        }

        public void setObjDB(EquTreeNode PicketTreeNode)
        {
            equPicket = PicketTreeNode.ObjectDB as EquPicket;
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
        [DisplayName("длинна(см)")]
        public int dlina
        {
            get
            {
                var res = from r in _db_controller.pickets_table.AsEnumerable() where r.number == equPicket.Code select r;

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
