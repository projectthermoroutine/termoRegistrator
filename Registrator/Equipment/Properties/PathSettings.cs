using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.ComponentModel;

namespace Registrator.Equipment
{
    public class PathSettings
    {
        DB.metro_db_controller _db_controller;
        EquPath equPath;
        public PathSettings(DB.metro_db_controller db_controller)
        {
            _db_controller = new DB.metro_db_controller(db_controller);
            
        }

        public void setObjDB(EquDbObject db_object)
        {
            equPath = db_object as EquPath;
        }

        [DisplayName("номер пути")]
        public int Track
        {
            get
            {
                return equPath.Code;
            }
            //set
            //{
            //    if (value >= 0)
            //    {
            //        if (value < 100000)
            //        {
            //            _db_controller.all_equipment_adapter.renameTrack(value, equPath.Code, equClassTmp.Code, equGroupTmp.Code, equLineTmp.Code);
            //            _db_controller.refresh();
            //            equPath.Code = value;
            //            FireRename(new RenameEvent(Convert.ToString(value)));
            //        }
            //        else
            //            MessageBox.Show("Введено слишком большое значение", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            //    }
            //    else
            //        MessageBox.Show("Номер пути не может быть представлен отрицательным числом", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            //}
        }

        [DisplayName("Название пути")]
        public string TrackName
        {
            get { return _db_controller.dbContext.Tracks.Where(t => t.ID == equPath.Code)
                                                        .Distinct()
                                                        .Select(t=>t.Track1)
                                                        .DefaultIfEmpty("empty")
                                                        .FirstOrDefault(); }
            set
            {
                if (value != "")
                {
                    _db_controller.queriesAdapter.renameTrack(value, equPath.Code);
                    FireRename(new RenameEvent(value));
                }
                else
                    MessageBox.Show("Название пути не может быть пустым", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        public event EventHandler<RenameEvent> RenameEventHandler;

        public virtual void FireRename(RenameEvent e)
        {
            EventHandler<RenameEvent> handler = RenameEventHandler;

            if (handler != null)
            {
                handler(this, e);
            }
        }
    }
}
