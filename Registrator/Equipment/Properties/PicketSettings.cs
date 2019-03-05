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
        EquTreeNode _node;


        public PicketSettings(DB.metro_db_controller db_controller)
        {
            _db_controller = db_controller;
           
        }

        public void SetObjDB(EquDbObject equObject, EquTreeNode node)
        {
            _db_object = _db_controller.dbContext.Pickets.Where(eq => eq.number == equObject.Code).Distinct().FirstOrDefault();
            _node = node;
        }

        protected DB.EFClasses.Picket _db_object;


        void PicketSettings_UpdateLenghtEvent(object sender, EventArgs e)
        {
            throw new NotImplementedException();
        }

        [ReadOnly(true)]
        [DisplayName("номер")]
        public string EquipmentKName => _db_object.Npiketa;

        [DisplayName("длина(м)")]
        public float Dlina
        {
            get
            {
                return _db_object.Dlina / (10 * 100);
            }
            set
            {
                //if (_picketTreeNode.Parent == null)
                //{
                //    MessageBox.Show("Выберите пикет", "Информация", MessageBoxButtons.OK);
                //    return;
                //}

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

                int val = Convert.ToInt32(value * 10.0 * 100.0);

                PicketsManager PM = new PicketsManager(_db_controller);

                PM.changePicketLength(val, _db_object);

                FireUpdateLength(new MyEventArgs { picketTreeNode = _node });
            }
        }

        public class MyEventArgs : EventArgs
        {
            public EquTreeNode picketTreeNode;
        }

        public event EventHandler<MyEventArgs> ChangeLenghtEvent;

        public virtual void FireUpdateLength(MyEventArgs e)
        {
            ChangeLenghtEvent?.Invoke(this, e);
        }
    }
}
