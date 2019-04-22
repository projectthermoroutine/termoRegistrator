using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;
using Registrator.Equipment.CreateDbObjectsCtrls;

namespace Registrator.Equipment
{
    public class LineSettings
    {
        private DB.metro_db_controller _db_controller;
        public LineSettings(DB.metro_db_controller db_controller)
        {
            _db_controller = new DB.metro_db_controller(db_controller);
        }

        public void SetObjDB(EquDbObject equObject)
        {
            _db_object = _db_controller.dbContext.Lines.Where(eq => eq.LineNum == equObject.Code).Distinct().FirstOrDefault();
        }

        protected DB.EFClasses.Line _db_object;

        [DisplayName("Код линии (Интеграл)")]
        public string LineCode
        {
            get { return _db_object.LineCode; }
            set 
            {
                string str = value;

                if (str.IndexOfAny(RegistratorFormStrings.incorrect_symbols_for_line_code) == -1)
                {
                    if (str.Length < 100)
                    {
                        _db_object.LineCode = str;
                        _db_controller.dbContext.Lines.Attach(_db_object);
                        _db_controller.dbContext.Entry(_db_object).State = System.Data.Entity.EntityState.Modified;
                        _db_controller.dbContext.SaveChanges();
                    }
                    else
                        MessageBox.Show("Введено слишком длинное название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Некорректно введено название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        [DisplayName("Название линии")]
        public string TrackName
        {
            get { return _db_object.LineName; }
            set
            {
                string str = value;

                if (str.IndexOfAny(RegistratorFormStrings.incorrect_symbols_for_line_code) == -1)
                {
                    if (str.Length < 100)
                    {
                        _db_object.LineName = str;
                        _db_controller.dbContext.Lines.Attach(_db_object);
                        _db_controller.dbContext.Entry(_db_object).State = System.Data.Entity.EntityState.Modified;
                        _db_controller.dbContext.SaveChanges();
                    }
                    else
                        MessageBox.Show("Введено слишком длинное название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Название содержит недопустимые символы", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }


    }
}
