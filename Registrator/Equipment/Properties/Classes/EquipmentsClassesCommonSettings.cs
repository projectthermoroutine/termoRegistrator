using Registrator.Equipment.CreateDbObjectsCtrls;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Registrator.Equipment.Properties
{
    public class EquipmentsClassesCommonSettings: EquipmentSetObject
    {
        protected DB.metro_db_controller _db_controller;

        public EquipmentsClassesCommonSettings(DB.metro_db_controller controller)
        {
            _db_controller = controller;
            //this.code_equip = -1;
        }

        [DisplayName("название")]
        public string EquipmentKName
        {
            get
            {
                return _db_controller.dbContext.EquipmentsClasses.Where(e => e.Id == this.code_equip).Select(n=>n.Name).DefaultIfEmpty("").FirstOrDefault();
            }
            set
            {
                string str = value;
                if (str.IndexOfAny(RegistratorFormStrings.incorrect_symbols) == -1)
                {
                    if (str.Length < 100)
                    {
                        DB.EFClasses.EquipmentsClass equip = _db_controller.dbContext.EquipmentsClasses.Where(eq => eq.Id == this.code_equip).Distinct().FirstOrDefault();
                        equip.Name = str;
                        _db_controller.dbContext.EquipmentsClasses.Attach(equip);
                        var entry = _db_controller.dbContext.Entry(equip);
                        entry.Property(e => e.Name).IsModified = true;

                        // change name in all equipments record of current type
                        IQueryable<DB.EFClasses.AllEquipment> equipAll = _db_controller.dbContext.AllEquipments.Where(eq => eq.EquipID == code_equip).Select(s => s);

                        foreach (var eq in equipAll)
                        {
                            eq.Name = str;
                            _db_controller.dbContext.AllEquipments.Attach(eq);
                            var entryAll = _db_controller.dbContext.Entry(eq);
                            entryAll.Property(e => e.Name).IsModified = true;
                        }

                        _db_controller.dbContext.SaveChanges();
                    }
                    else
                        MessageBox.Show("Введено слишком длинное название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Некорректно введено название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

       

       
    }
}
