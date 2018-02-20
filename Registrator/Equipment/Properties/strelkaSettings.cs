using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;
using System.Globalization;

namespace Registrator.Equipment
{
    public class strelkaSettings
    {
        private DB.metro_db_controller _db_controller;
        private EquObject equObject;

        public strelkaSettings(DB.metro_db_controller db_controller)
        {
            _db_controller = new DB.metro_db_controller(db_controller);
        }
        public void setObjDB(EquTreeNode ObjectTreeNode)
        {
            equObject = ObjectTreeNode.ObjectDB as EquObject;
        }

        [DisplayName("название")]
        public string equipmentKName
        {
            get
            {
                ///TODO this code is often repeated
                return _db_controller.dbContext.Equipments.Where(e => e.Code == equObject.Code).Distinct().FirstOrDefault().Name;
            }
            set
            {
                string str = value;
                if (str.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*', '`', '№', '\\', '|' }) == -1)
                {
                    if (str.Length < 100)
                    {
                        _db_controller.queriesAdapter.renameEquipment(equObject.Code, str);
                        FireRename(new RenameEvent(str));
                    }
                    else
                        MessageBox.Show("Введено слишком длинное название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Некорректно введено название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        [DisplayName("смещение от начала пикета(см)")]
        public int shiftFromBegin
        {
            get
            {
                return _db_controller.dbContext.Equipments.Where(eq => eq.Code == equObject.Code).Distinct().FirstOrDefault().shiftFromPicket;
            }
            set
            {
                int shift = value;

                if (shift > 0)
                {
                    if (shift < 900000)
                    {
                        _db_controller.queriesAdapter.UpdateShiftBeginEquip(equObject.Code, shift);
                    }
                    else
                        MessageBox.Show("Значение слишком велико", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Смещение должно быть больше 0", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        [DisplayName("длина (см)")]
        public int shiftFromEnd
        {
            get {  return _db_controller.dbContext.Equipments.Where(eq => eq.Code == equObject.Code).Distinct().FirstOrDefault().EquipLenght;   }
            set
            {
                int shift = value;

                if (shift > 0)
                {
                    if (shift < 900000)
                    {
                        _db_controller.queriesAdapter.UpdateEquipLenght(equObject.Code, shift);
                    }
                    else
                        MessageBox.Show("Значение слишком велико", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Длина должна быть больше 0", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        [DisplayName("левая/правая")]
        [TypeConverter(typeof(StrelkaClassConverter))]
        public bool direction
        {
            get { return Convert.ToBoolean(_db_controller.dbContext.Equipments.Where(eq => eq.Code == equObject.Code).FirstOrDefault().strelkaLeftOrRight);   }
            set
            {
                int direction = (value) ? 1 : 0;
                _db_controller.queriesAdapter.UpdateStrelkaDirect(equObject.Code, direction);
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
    class StrelkaClassConverter : BooleanConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context,
        CultureInfo culture,
        object value,
        Type destType)
        {
            return (bool)value ?
            "левая" : "правая";
        }

        public override object ConvertFrom(ITypeDescriptorContext context,
        CultureInfo culture,
        object value)
        {
            return (string)value == "левая";
        }
    }
}
