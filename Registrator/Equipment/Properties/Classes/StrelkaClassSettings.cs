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
    public class StrelkaClassSettings : EquipmentsClassesCommonSettings
    {
        public StrelkaClassSettings(DB.metro_db_controller controller)
            :base(controller)
        {
            //this.code_equip = -1;
        }


        [DisplayName("левая/правая")]
        [TypeConverter(typeof(StrelkaClassConverter))]
        public bool Direction
        {
            get
            {
                return Convert.ToBoolean(_db_controller.dbContext.EquipmentsClasses.Where(eq => eq.Id == code_equip).FirstOrDefault().strelkaLeftOrRight);
            }

            set
            {
                int direction = (value) ? 1 : 0;

                DB.EFClasses.EquipmentsClass equip = _db_controller.dbContext.EquipmentsClasses.Where(eq => eq.Id == code_equip).Distinct().FirstOrDefault();
                equip.strelkaLeftOrRight = direction;
                _db_controller.dbContext.EquipmentsClasses.Attach(equip);
                var entry = _db_controller.dbContext.Entry(equip);
                entry.Property(e => e.strelkaLeftOrRight).IsModified = true;

                // change name in all equipments record of current type
                IQueryable<DB.EFClasses.AllEquipment> equipAll = _db_controller.dbContext.AllEquipments.Where(eq => eq.EquipID == code_equip).Select(s => s);

                foreach (var eq in equipAll)
                {
                    eq.strelkaLeftOrRight = direction;
                    _db_controller.dbContext.AllEquipments.Attach(eq);
                    var entryAll = _db_controller.dbContext.Entry(eq);
                    entryAll.Property(e => e.Name).IsModified = true;
                }

                _db_controller.dbContext.SaveChanges();
            }
        }

        [DisplayName("длина (см)")]
        public int ShiftFromEnd
        {
            get
            {
                return _db_controller.dbContext.EquipmentsClasses.Where(eq => eq.Id == code_equip).Distinct().Select(e => e.Width).DefaultIfEmpty(-1).FirstOrDefault();
            }
            set
            {
                int shift = value;

                if (shift > 0)
                {
                    if (shift < Int32.MaxValue)
                    {
                        DB.EFClasses.EquipmentsClass equip = _db_controller.dbContext.EquipmentsClasses.Where(eq => eq.Id == code_equip).Distinct().FirstOrDefault();
                        equip.Width = shift;
                        _db_controller.dbContext.EquipmentsClasses.Attach(equip);
                        var entry = _db_controller.dbContext.Entry(equip);
                        entry.Property(e => e.Width).IsModified = true;


                        // change lenght in all equipments record of current type
                        IQueryable<DB.EFClasses.AllEquipment> equipAll = _db_controller.dbContext.AllEquipments.Where(eq => eq.EquipID == code_equip).Select(s => s);

                        foreach (var eq in equipAll)
                        {
                            eq.EquipLenght = shift;
                            _db_controller.dbContext.AllEquipments.Attach(eq);
                            var entryAll = _db_controller.dbContext.Entry(eq);
                            entryAll.Property(e => e.EquipLenght).IsModified = true;
                        }


                        _db_controller.dbContext.SaveChanges();
                    }
                    else
                        MessageBox.Show("Значение слишком велико", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Длина должна быть больше 0", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }
    }
}
