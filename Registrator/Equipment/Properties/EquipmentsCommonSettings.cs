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

    public class EquipmentSettings : EquipmentsCommonSettings
    {
        public EquipmentSettings(DB.metro_db_controller db_controller)
            : base(db_controller)
        {

        }
    }

    [TypeConverter(typeof(PropertySorter))]
    public class EquipmentsCommonSettings: AbstractProperties
    {
        protected DB.metro_db_controller _db_controller;

        public EquipmentsCommonSettings(DB.metro_db_controller controller)
        {
            _db_controller = controller;
        }

        public new void SetObjDB(EquDbObject equObject)
        {
            _db_object = _db_controller.dbContext.AllEquipments.Where(eq => eq.Code == equObject.Code).Distinct().FirstOrDefault();
        }

        protected DB.EFClasses.AllEquipment _db_object;

        [DisplayName("дополнительная информация")]
        [PropertyOrder(99)]
        public string AdditionalInfo
        {
            get
            {
                return _db_object.Info;
            }
            set
            {
                string info = value;

                if (info.Length < 1024)
                {
                    _db_object.Info = info;
                    _db_controller.dbContext.AllEquipments.Attach(_db_object);
                    _db_controller.dbContext.Entry(_db_object).State = System.Data.Entity.EntityState.Modified;
                    _db_controller.dbContext.SaveChanges();
                }
                else
                    MessageBox.Show("Максимальная длина строки 1024 символа", "Информация", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        [DisplayName("название")]
        [PropertyOrder(1)]
        public string EquipmentKName
        {
            get
            {
                return _db_object.Name;
            }
            set
            {
                string str = value;
                if (str.IndexOfAny(RegistratorFormStrings.incorrect_symbols) == -1)
                {
                    if (str.Length < 100)
                    {
                        _db_object.Name = str;
                        _db_controller.dbContext.AllEquipments.Attach(_db_object);
                        _db_controller.dbContext.Entry(_db_object).State = System.Data.Entity.EntityState.Modified;
                        _db_controller.dbContext.SaveChanges();

                        NameChanged(str);
                    }
                    else
                        MessageBox.Show("Введено слишком длинное название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Недопустимые символы в название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        [DisplayName("Тип")]
        [PropertyOrder(2)]
        [TypeConverter(typeof(TypeEquipmentClassConverter))]
        public Int32 EquipmentType
        {
            get
            {
                return Convert.ToInt32(_db_object.EquipmentsClass.EquipType);
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
