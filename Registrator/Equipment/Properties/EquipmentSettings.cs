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
        private EquPicket equPicket;
        
        public EquipmentSettings(DB.metro_db_controller db_controller)
        {
            _db_controller = new DB.metro_db_controller(db_controller);
        }

        public void setObjDB(EquTreeNode ObjectTreeNode)
        {
            equObject = ObjectTreeNode.ObjectDB as EquObject;
            equPicket = (ObjectTreeNode.Parent as EquTreeNode).ObjectDB as EquPicket;
        }

        [DisplayName("название")]
        public string equipmentKName
        {
            get
            {
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

        [DisplayName("техническое состояние")]
        [TypeConverter(typeof(DrinkerClassConverter))]
        public bool texStatus
        {
            get { return Convert.ToBoolean(_db_controller.dbContext.Equipments.Where(eq => eq.Code == equObject.Code).Distinct().FirstOrDefault().EquipWorkState); }
            set
            {
                int status = (value) ? 1 : 0;
                _db_controller.queriesAdapter.UpdateStatusEquip(equObject.Code, status);
            }
        }

        [DisplayName("смещение от начала пикета(см)")]
        public int shiftFromBegin
        {
            get { return _db_controller.dbContext.Equipments.Where(eq => eq.Code == equObject.Code).Distinct().FirstOrDefault().shiftFromPicket; }
            set {
                int shift = value;
               
                if(equPicket.lenght > shift)   {
                    _db_controller.queriesAdapter.UpdateShiftBeginEquip(equObject.Code, shift);
                }
                else
                    MessageBox.Show("Значение выходит за пределы пикета", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

        }

        [DisplayName("максимально допустимая температура(°C)")]
        public int maxTemperature
        {
            get { return _db_controller.dbContext.Equipments.Where(eq => eq.Code == equObject.Code).Distinct().FirstOrDefault().maxTemperature; }
            set
            {
                int maxTemp = value;

              
                if (maxTemp < 900000)
                {
                    _db_controller.queriesAdapter.UpdateMaxTemperature(equObject.Code, maxTemp);
                }
                else
                    MessageBox.Show("Значение слишком велико", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
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
