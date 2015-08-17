using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;
using System.Globalization;

namespace Registrator.Equipment
{
    public class RenameEvent : EventArgs
    {
        private string m_name;

        public RenameEvent(string name)
            : base()
        {
            m_name = name;
        }

        public string Name { get { return m_name; } set { m_name = value; } }
    }
    public class EquipmentSettings
    {
        private DB.metro_db_controller _db_controller;
        private EquObject equObject;
        private string equipmentName = null;
        private int regularly;
        
        public EquipmentSettings(DB.metro_db_controller db_controller)
        {
            _db_controller = new DB.metro_db_controller(db_controller);
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
                var res = from r in _db_controller.objects_table.AsEnumerable() where r.Code == equObject.Code select r;

                if (res.Count() == 1)
                {
                    equipmentName = res.First().Object;
                    return equipmentName;
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

        [DisplayName("техническое состояние")]
        [TypeConverter(typeof(DrinkerClassConverter))]
        public bool texStatus
        {
            get
            {
                var res = from r in _db_controller.objects_table.AsEnumerable() where r.Code == equObject.Code select r;

                if (res.Count() == 1)
                {
                    regularly = res.First().regularly;
                    
                    if (regularly == 0)
                        return false;
                    if (regularly == 1)
                        return true;
                }

                return false;
            }
            set
            {
                int status = (value)? 1:0;
                _db_controller.objects_adapter.UpdateStatusEquip(equObject.Code, status);
                _db_controller.refresh();
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

                if(shift > 0)
                { 
                    if(shift < 900000)
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

        [DisplayName("максимально допустимая температура(°C)")]
        public int maxTemperature
        {
            get
            {
                var res = from r in _db_controller.objects_table.AsEnumerable() where r.Code == equObject.Code select r;

                int maxTemp;

                if (res.Count() == 1)
                {
                    maxTemp = res.First().maxTemperature;
                    return maxTemp;
                }
                return -1;
            }
            set
            {
                int maxTemp = value;

                if (maxTemp > 0)
                {
                    if (maxTemp < 900000)
                    {
                        _db_controller.objects_adapter.UpdateMaxTemperature(equObject.Code, maxTemp);
                        _db_controller.refresh();
                    }
                    else
                        MessageBox.Show("Значение слишком велико", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Значение должно быть больше 0", "", MessageBoxButtons.OK, MessageBoxIcon.Information);

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
    class DrinkerClassConverter : BooleanConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context,
        CultureInfo culture,
        object value,
        Type destType)
        {
            return (bool)value ?
            "неисправно" : "исправно";
        }

        public override object ConvertFrom(ITypeDescriptorContext context,
        CultureInfo culture,
        object value)
        {
            return (string)value == "неисправно";
        }
    }
}
