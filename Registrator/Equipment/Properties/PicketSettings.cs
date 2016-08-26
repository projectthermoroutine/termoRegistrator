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
        EquTreeNode PicketTreeNode;


        public PicketSettings(DB.metro_db_controller db_controller)
        {
            _db_controller = db_controller;
           
        }

        public void setObjDB(EquTreeNode PicketTreeNode_)
        {
            equPicket = PicketTreeNode_.ObjectDB as EquPicket;
            PicketTreeNode = PicketTreeNode_;
        }

        void PicketSettings_UpdateLenghtEvent(object sender, EventArgs e)
        {
            throw new NotImplementedException();
        }

        [ReadOnly(true)]  
        [DisplayName("номер")]
        public string equipmentKName
        {
            get  { return equPicket.npicket; }
        }

        [DisplayName("длинна(см)")]
        public int dlina
        {
            get
            {
               
                var res = from r in _db_controller.pickets_table.AsEnumerable() where r.number == equPicket.number select r;

                int dlina;

                if (res.Count() == 1)
                {
                    dlina = res.First().Dlina;
                    return dlina;
                }
                return -1;
            }
            set {

                PicketsManager PM = new PicketsManager(_db_controller);
                
                EquPath  p = (PicketTreeNode.Parent as EquTreeNode).ObjectDB as EquPath;
               

                PM.createLogicalPicketList(p);
                PM.changePicketLength(equPicket, value);

                FireUpdateLenght(new EventArgs());
            }
        }

        public event EventHandler<EventArgs> ChangeLenghtEvent;

        public virtual void FireUpdateLenght(EventArgs e)
        {
            EventHandler<EventArgs> handler = ChangeLenghtEvent;

            if (handler != null)
            {
                handler(this, e);
            }
        }
    }
}
