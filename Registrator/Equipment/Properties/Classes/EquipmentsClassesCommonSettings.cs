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
    public class EquipmentsClassesCommonSettings: DBObjectSetter
    {
        protected DB.metro_db_controller _db_controller;

        public EquipmentsClassesCommonSettings(DB.metro_db_controller controller)
        {
            _db_controller = controller;
            //this.code_equip = -1;
        }

        public new void SetObjDB(EquDbObject equObject)
        {
            _db_object = _db_controller.dbContext.EquipmentsClasses.Where(eq => eq.Id == equObject.Code).Distinct().FirstOrDefault();
            code_equip = equObject.Code;
        }

        protected DB.EFClasses.EquipmentsClass _db_object;


        [DisplayName("название")]
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
                        DB.EFClasses.EquipmentsClass equip = _db_controller.dbContext.EquipmentsClasses.Where(eq => eq.Id == this.code_equip).Distinct().FirstOrDefault();
                        _db_object.Name = str;
                        _db_controller.dbContext.EquipmentsClasses.Attach(_db_object);
                        _db_controller.dbContext.Entry(_db_object).State = System.Data.Entity.EntityState.Modified;

                        // change name in all equipments record of current type
                        //foreach (var eq in _db_controller.dbContext.AllEquipments.Where(eq => eq.EquipID == code_equip))
                        //{
                        //    eq.Name = str;
                        //    _db_controller.dbContext.AllEquipments.Attach(eq);
                        //    _db_controller.dbContext.Entry(eq).State = System.Data.Entity.EntityState.Modified;
                        //}

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
