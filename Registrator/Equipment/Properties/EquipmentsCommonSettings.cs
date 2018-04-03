using Registrator.Equipment.CreateDbObjectsCtrls;
using Registrator.Equipment.Properties;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Registrator.Equipment
{
    public class EquipmentsCommonSettings: EquipmentSetObject
    {
        protected DB.metro_db_controller _db_controller;

        public EquipmentsCommonSettings(DB.metro_db_controller controller)
        {
            _db_controller = controller;
            this.code_equip = -1;
        }

        [DisplayName("дополнительная информация")]
        public string AdditionalInfo
        {
            get
            {
                return _db_controller.dbContext.AllEquipments.Where(eq => eq.Code == code_equip).Distinct().Select(e => e.Info).DefaultIfEmpty("").FirstOrDefault();
            }
            set
            {
                string info = value;

                if (info.Length < 1024)
                {
                    DB.EFClasses.AllEquipment equip = _db_controller.dbContext.AllEquipments.Where(eq => eq.Code == code_equip).Distinct().FirstOrDefault();
                    equip.Info = info;
                    _db_controller.dbContext.AllEquipments.Attach(equip);
                    var entry = _db_controller.dbContext.Entry(equip);

                    entry.Property(e => e.Info).IsModified = true;
                    _db_controller.dbContext.SaveChanges();
                }
                else
                    MessageBox.Show("Максимальная длина строки 1024 символа", "Информация", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        [DisplayName("название")]
        public string EquipmentKName
        {
            get
            {
                return _db_controller.dbContext.AllEquipments.Where(e => e.Code == this.code_equip).Distinct().FirstOrDefault().Name;
            }
            //set
            //{
            //    string str = value;
            //    if (str.IndexOfAny(RegistratorFormStrings.incorrect_symbols) == -1)
            //    {
            //        if (str.Length < 100)
            //        {
            //            DB.EFClasses.AllEquipment equip = _db_controller.dbContext.AllEquipments.Where(eq => eq.Code == this.code_equip).Distinct().FirstOrDefault();
            //            equip.Name = str;
            //            _db_controller.dbContext.AllEquipments.Attach(equip);
            //            var entry = _db_controller.dbContext.Entry(equip);
            //            entry.Property(e => e.Name).IsModified = true;
            //            _db_controller.dbContext.SaveChanges();
            //        }
            //        else
            //            MessageBox.Show("Введено слишком длинное название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            //    }
            //    else
            //        MessageBox.Show("Некорректно введено название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            //}
        }

        [DisplayName("Тип")]
        [TypeConverter(typeof(TypeEquipmentClassConverter))]
        public Int32 EquipmentType
        {
            get
            {
                return Convert.ToInt32( _db_controller.dbContext.AllEquipments.Where(e => e.Code == code_equip).Distinct().FirstOrDefault().EquipmentsClass.EquipType);
            }
        }
    }

    class TypeEquipmentClassConverter : Int32Converter
    {
        public override object ConvertTo(ITypeDescriptorContext context,
        CultureInfo culture,
        object value,
        Type destType)
        {

            Int32 tmp = (Int32)value;
            EQUIPS_TYPES v = (EQUIPS_TYPES)Enum.ToObject(typeof(EQUIPS_TYPES), tmp);

            string res = "";

            switch(v)
            {
                case EQUIPS_TYPES.Equipment:
                    res = "Оборудование";
                    break;
                case EQUIPS_TYPES.Strelka:
                    res = "Стрелка";
                    break;
                case EQUIPS_TYPES.TrafficLight:
                    res = "Светофор";
                    break;
            }

            return res;
        }

        public override object ConvertFrom(ITypeDescriptorContext context,
        CultureInfo culture,
        object value)
        {
            string v = (string)value;

            Int32 res = 0;

            switch (v)
            {
                case "Оборудование" :
                    res = (Int32)EQUIPS_TYPES.Equipment;
                    break;
                case "Стрелка":
                    res = (Int32)EQUIPS_TYPES.Strelka;
                    break;
                case "Светофор":
                    res = (Int32)EQUIPS_TYPES.TrafficLight;
                    break;
            }

            return res;
        }
    }
}
