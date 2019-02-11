using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;
using System.Globalization;
using Registrator.Equipment.CreateDbObjectsCtrls;
using Registrator.Equipment.Properties;

namespace Registrator.Equipment
{
    public class StrelkaSettings : EquipmentInPicketCommonSettings
    {
        public StrelkaSettings(DB.metro_db_controller db_controller)
            :base(db_controller)
        {
            _db_controller = new DB.metro_db_controller(db_controller);
        }

       

        [DisplayName("Длина (см)")]
        public int ObjectLength
        {
            get
            {
                return _db_object.EquipLenght;
            }
            //set
            //{
            //    int shift = value;

            //    if (shift > 0)
            //    {
            //        if (shift < 900000)
            //        {
            //            DB.EFClasses.AllEquipment equip = _db_controller.dbContext.AllEquipments.Where(eq => eq.Code == code_equip).Distinct().FirstOrDefault();
            //            equip.EquipLenght = shift;
            //            _db_controller.dbContext.AllEquipments.Attach(equip);
            //            var entry = _db_controller.dbContext.Entry(equip);
            //            entry.Property(e => e.EquipLenght).IsModified = true;

            //            _db_controller.dbContext.SaveChanges();
            //        }
            //        else
            //            MessageBox.Show("Значение слишком велико", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            //    }
            //    else
            //        MessageBox.Show("Длина должна быть больше 0", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            //}
        }

        [DisplayName("Левая/правая")]
        [TypeConverter(typeof(StrelkaClassConverter))]
        public bool direction
        {
            get
            {
                return Convert.ToBoolean(_db_object.strelkaLeftOrRight);
            }

            //set
            //{
            //    int direction = (value) ? 1 : 0;

            //    DB.EFClasses.AllEquipment equip = _db_controller.dbContext.AllEquipments.Where(eq => eq.Code == code_equip).Distinct().FirstOrDefault();
            //    equip.strelkaLeftOrRight = direction;
            //    _db_controller.dbContext.AllEquipments.Attach(equip);
            //    var entry = _db_controller.dbContext.Entry(equip);
            //    entry.Property(e => e.strelkaLeftOrRight).IsModified = true;

            //    _db_controller.dbContext.SaveChanges();
            //}
        }

    }

    /// <summary>
    /// Convert bool to string
    /// </summary>
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
