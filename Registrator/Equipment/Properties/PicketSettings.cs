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
                float val = 0;
                try
                {
                    val = _db_controller.dbContext.Pickets.Where(p => p.number == equPicket.keyNumber).Distinct().Select(p => p.Dlina).DefaultIfEmpty(Int32.MinValue).FirstOrDefault();
                }
                catch(System.Data.Entity.Core.EntityCommandExecutionException /*e*/)
                {
                    ///TODO close property windows
                }

                float res = val / (10 * 100);
                return res;
            }
            set
            {
                if (_picketTreeNode.Parent == null)
                {
                    MessageBox.Show("Выберите пикет", "Информация", MessageBoxButtons.OK);
                    return;
                }

                const string message = "Вы уверены, что хотите изменить длинну пикета?";
                const string caption = "Предупреждение";
                var result = MessageBox.Show(message, caption,
                                             MessageBoxButtons.YesNo,
                                             MessageBoxIcon.Question);

                // If the no button was pressed ...
                if (result == DialogResult.No)
                {
                    // cancel the closure of the form.
                    return;
                }

                if (value < 0.001)
                {
                    MessageBox.Show("Длина пикета не может быть меньше одного мм", "Информация", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                }

                float max = UInt16.MaxValue;

                if (value > max)
                {
                    MessageBox.Show($"Длина пикета не может быть больше {max} m", "Информация", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                }

                DB.EFClasses.Picket picket = _db_controller.dbContext.Pickets.Where(e => e.number == equPicket.keyNumber).Distinct().Select(e => e).First();
                int val = Convert.ToInt32(value * 10.0 * 100.0);
                _db_controller.queriesAdapter.CheckAllowedLength(picket.number, val);

                if (_db_controller.queriesAdapter.GetResult(24) > 0)
                {
                    MessageBox.Show("Невозможно изменить длину пикета, т. к. есть оборудование смещение которого больше, чем задаваемое значение длины пикета", "Информация", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }

                PicketsManager PM = new PicketsManager(_db_controller);

                EquPath p = (_picketTreeNode.Parent as EquTreeNode).ObjectDB as EquPath;

                PM.createLogicalPicketList(p);

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
