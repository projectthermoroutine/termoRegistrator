using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.ComponentModel;

namespace Registrator.Equipment
{
    public class PathSettings : AbstractProperties
    {
        DB.metro_db_controller _db_controller;

        public PathSettings(DB.metro_db_controller db_controller)
        {
            _db_controller = new DB.metro_db_controller(db_controller);
        }

        public void SetObjDB(EquDbObject equObject)
        {
            _db_object = _db_controller.dbContext.Tracks.Where(eq => eq.ID == equObject.Code).Distinct().FirstOrDefault();
        }

        protected DB.EFClasses.Track _db_object;

        [DisplayName("Номер пути")]
        public int Track
        {
            get
            {
                return _db_object.ID;
            }
        }

        [DisplayName("Название пути (Интеграл)")]
        public string TrackName
        {
            get { return _db_object.Name; }
            set
            {
                if (value != "")
                {
                    //DB.EFClasses.Track track = _db_controller.dbContext.Tracks.Where(l => l.ID == _db_object.ID).Distinct().FirstOrDefault();

                    _db_object.Name = value;
                    _db_controller.dbContext.Tracks.Attach(_db_object);
                    _db_controller.dbContext.Entry(_db_object).State = System.Data.Entity.EntityState.Modified;
                    _db_controller.dbContext.SaveChanges();

                    NameChanged(value);


                }
                else
                    MessageBox.Show("Название пути не может быть пустым", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }
    }
}
