using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace Registrator.Equipment.Properties
{
    public class TemperatureSetting: EquipmentInPicketCommonSettings
    {
        public TemperatureSetting(DB.metro_db_controller db_controller)
            : base(db_controller)
        {

        }

        [DisplayName("максимально допустимая температура(°C)")]
        public int maxTemperature
        {
            get
            {
                return _db_controller.dbContext.AllEquipments.Where(eq => eq.Code == code_equip).Distinct().Select(e => e.maxTemperature).DefaultIfEmpty(-1).FirstOrDefault();
            }
            //set
            //{
            //    int maxTemp = value;

            //    if (maxTemp < 900000)
            //    {
            //        DB.EFClasses.AllEquipment equip = _db_controller.dbContext.AllEquipments.Where(eq => eq.Code == code_equip).Distinct().FirstOrDefault();
            //        equip.maxTemperature = maxTemp;
            //        _db_controller.dbContext.AllEquipments.Attach(equip);
            //        var entry = _db_controller.dbContext.Entry(equip);

            //        entry.Property(e => e.maxTemperature).IsModified = true;
            //        _db_controller.dbContext.SaveChanges();
            //    }
            //    else
            //        MessageBox.Show("Значение слишком велико", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            //}
        }
    }
}
