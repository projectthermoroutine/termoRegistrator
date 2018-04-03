using Registrator.Equipment.CreateDbObjectsCtrls;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Registrator.Equipment.Properties.Classes
{
    public class EquipmentsClassesSettings: EquipmentsClassesCommonSettings
    {
        public EquipmentsClassesSettings(DB.metro_db_controller controller)
            :base(controller)
        {


        }

        [DisplayName("Максимально допустимая температура(°C)")]
        public int MaxTemperature
        {
            get
            {
                return _db_controller.dbContext.EquipmentsClasses.Where(eq => eq.Id == code_equip).Distinct().Select(e => e.MaxTemperature).DefaultIfEmpty(-1).FirstOrDefault();
            }
            set
            {
                int maxTemp = value;

                if (maxTemp < 10000)
                {
                    DB.EFClasses.EquipmentsClass equip = _db_controller.dbContext.EquipmentsClasses.Where(eq => eq.Id == code_equip).Distinct().FirstOrDefault();
                    equip.MaxTemperature = maxTemp;
                    _db_controller.dbContext.EquipmentsClasses.Attach(equip);
                    var entry = _db_controller.dbContext.Entry(equip);

                    entry.Property(e => e.MaxTemperature).IsModified = true;

                    // change name in all equipments record of current type
                    IQueryable<DB.EFClasses.AllEquipment> equipAll = _db_controller.dbContext.AllEquipments.Where(eq => eq.EquipID == code_equip).Select(s => s);

                    foreach (var eq in equipAll)
                    {
                        eq.maxTemperature = maxTemp;
                        _db_controller.dbContext.AllEquipments.Attach(eq);
                        var entryAll = _db_controller.dbContext.Entry(eq);
                        entryAll.Property(e => e.maxTemperature).IsModified = true;
                    }



                    _db_controller.dbContext.SaveChanges();
                }
                else
                    MessageBox.Show("Значение слишком велико", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }
    }
}
