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
        private DB.DataBaseHelper dbHelper;
        private EquObject equObject;
        private string Name = null;
        private int m_peregonLength;

        public strelkaSettings(DB.DataBaseHelper dbHelper_Arg)
        {
            dbHelper = dbHelper_Arg;
        }
        public void setObjDB(EquObject equObject_Arg)
        {
            equObject = equObject_Arg;
        }

        [DisplayName("название")]
        public string equipmentKName
        {
            get
            {
                var res = from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Code == equObject.Code select r;

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
                        dbHelper.TblAdapter_Objects.renameEquipment(equObject.Code, str);
                        dbHelper.refresh();
                        FireRename(new RenameEvent(str));
                    }
                    else
                        MessageBox.Show("Введено слишком длинное название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Некорректно введено название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        [DisplayName("смещение от начала пикета")]
        public int shiftFromBegin
        {
            get
            {
                var res = from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Code == equObject.Code select r;

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
                        dbHelper.TblAdapter_Objects.UpdateShiftBeginEquip(equObject.Code, shift);
                        dbHelper.refresh();
                    }
                    else
                        MessageBox.Show("Значение слишком велико", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Смещение должно быть больше 0", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        [DisplayName("смещение от конца пикета")]
        public int shiftFromEnd
        {
            get
            {
                var res = from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Code == equObject.Code select r;

                int shift;

                if (res.Count() == 1)
                {
                    shift = res.First().shiftFromEndPicket;
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
                        dbHelper.TblAdapter_Objects.UpdateShiftEndEquip(equObject.Code, shift);
                        dbHelper.refresh();
                    }
                    else
                        MessageBox.Show("Значение слишком велико", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Смещение должно быть больше 0", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        private int direction_;

        [DisplayName("левая/правая")]
        [TypeConverter(typeof(StrelkaClassConverter))]
        public bool direction
        {
            get
            {
                var res = from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Code == equObject.Code select r;

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
                dbHelper.TblAdapter_Objects.UpdateStrelkaDirect(equObject.Code, direction);
                dbHelper.refresh();
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
