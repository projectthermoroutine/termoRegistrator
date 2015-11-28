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
        private string Name = null;
        private int m_peregonLength;

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
                var res = from r in _db_controller.objects_table.AsEnumerable() where r.Code == equObject.Code select r;

                if (res.Count() == 1)
                {
                    Name = res.First().Object;
                    return Name;
                }

                return "";
            }
            set
            {
                string str = value;
                if (str.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*', '`', '№', '\\', '|' }) == -1)
                {
                    if (str.Length < 100)
                    {
                        _db_controller.objects_adapter.renameEquipment(equObject.Code, str);
                        _db_controller.refresh();
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
                var res = from r in _db_controller.objects_table.AsEnumerable() where r.Code == equObject.Code select r;

                int shift;

                if (res.Count() == 1)
                {
                    shift = res.First().shiftFromPicket;
                    return shift;
                }
                return -1;
            }
            set
            {
                int shift = value;

                if (shift > 0)
                {
                    if (shift < 900000)
                    {
                        _db_controller.objects_adapter.UpdateShiftBeginEquip(equObject.Code, shift);
                        _db_controller.refresh();
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
            get
            {
                var res = from r in _db_controller.objects_table.AsEnumerable() where r.Code == equObject.Code select r;

                int shift;

                if (res.Count() == 1)
                {
                    shift = res.First().ObjectLenght;
                    return shift;
                }

                return -1;
            }
            set
            {
                int shift = value;

                if (shift > 0)
                {
                    if (shift < 900000)
                    {
                        _db_controller.objects_adapter.UpdateEquipLenght(equObject.Code, shift);
                        _db_controller.refresh();
                    }
                    else
                        MessageBox.Show("Значение слишком велико", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Длина должна быть больше 0", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        private int direction_;

        [DisplayName("левая/правая")]
        [TypeConverter(typeof(StrelkaClassConverter))]
        public bool direction
        {
            get
            {
                var res = from r in _db_controller.objects_table.AsEnumerable() where r.Code == equObject.Code select r;

                if (res.Count() == 1)
                {
                    direction_ = res.First().strelkaLeftOrRight;

                    if (direction_ == 0)
                        return false;
                    if (direction_ == 1)
                        return true;
                }

                return false;
            }
            set
            {
                int direction = (value) ? 1 : 0;
                _db_controller.objects_adapter.UpdateStrelkaDirect(equObject.Code, direction);
                _db_controller.refresh();
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
