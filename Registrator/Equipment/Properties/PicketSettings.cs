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
        EquTreeNode _picketTreeNode;


        public PicketSettings(DB.metro_db_controller db_controller)
        {
            _db_controller = db_controller;
           
        }

        public void setObjDB(EquTreeNode PicketTreeNode_)
        {
            equPicket = PicketTreeNode_.ObjectDB as EquPicket;
            _picketTreeNode = PicketTreeNode_;
        }

        void PicketSettings_UpdateLenghtEvent(object sender, EventArgs e)
        {
            throw new NotImplementedException();
        }

        [ReadOnly(true)]
        [DisplayName("номер")]
        public string EquipmentKName => equPicket.npicket;

        [DisplayName("длинна(м)")]
        public float Dlina
        {
            get
            {
                float val = _db_controller.dbContext.Pickets.Where(p => p.number == equPicket.keyNumber).Distinct().Select(p => p.Dlina).DefaultIfEmpty(Int32.MinValue).FirstOrDefault();
                float res = val / (10 * 100);
                return res;
            }
            set
            {

                PicketsManager PM = new PicketsManager(_db_controller);
                
                EquPath  p = (_picketTreeNode.Parent as EquTreeNode).ObjectDB as EquPath;
               

                PM.createLogicalPicketList(p);

                DB.EFClasses.Picket picket = _db_controller.dbContext.Pickets.Where(e => e.number == equPicket.keyNumber).Distinct().Select(e => e).First();


                int val = Convert.ToInt32(value * 10.0 * 100.0);

                PM.changePicketLength(val, picket);

                FireUpdateLenght(new MyEventArgs { picketTreeNode = _picketTreeNode });
            }
        }

        public class MyEventArgs : EventArgs
        {
            public EquTreeNode picketTreeNode;
        }

        public event EventHandler<MyEventArgs> ChangeLenghtEvent;

        public virtual void FireUpdateLenght(MyEventArgs e)
        {
            EventHandler<MyEventArgs> handler = ChangeLenghtEvent;

            if (handler != null)
            {
                handler(this, e);
            }
        }
    }
}
