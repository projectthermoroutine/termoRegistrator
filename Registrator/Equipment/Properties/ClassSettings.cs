using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.ComponentModel;
using Registrator.Equipment.CreateDbObjectsCtrls;

namespace Registrator.Equipment
{
    public class ClassSettings : AbstractProperties
    {
        private DB.metro_db_controller _db_controller;
        private EquClass equClass;
        private string Name = null;

        public ClassSettings(DB.metro_db_controller db_controller)
        {
            _db_controller = new DB.metro_db_controller(db_controller);
            
        }

        public void setObjDB( EquTreeNode GroupTreeNode)
        {
            equClass = GroupTreeNode.ObjectDB as EquClass;
        }


        [DisplayName("название")]
        public string className
        {
            get
            {
                var res = from r in _db_controller.dbContext.Classes where r.Code == equClass.Code select r;

                if (res.Count() == 1)
                {
                    Name = res.First().Name;
                    return Name;
                }

                return "";
            }
            set
            {
                string str = value;

                if (str.IndexOfAny(RegistratorFormStrings.incorrect_symbols) == -1)
                {
                    if (str.Length < 20)
                    {
                        DB.EFClasses.Class @class = _db_controller.dbContext.Classes.Where(eq => eq.Code == equClass.Code).Distinct().FirstOrDefault();
                        @class.Name = str;
                        _db_controller.dbContext.Classes.Attach(@class);
                        var entry = _db_controller.dbContext.Entry(@class);
                        entry.Property(e => e.Name).IsModified = true;

                        _db_controller.dbContext.SaveChanges();

                        NameChanged(str);

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
