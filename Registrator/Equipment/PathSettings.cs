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
        private DB.metro_db_controller _db_controller;
        private EquPath equPath;
        private int Name = -1;
        private int m_peregonLength;
        private EquGroup equGroupTmp;
        private EquLine equLineTmp;
        private EquClass equClassTmp;
        public PathSettings(DB.metro_db_controller db_controller)
        {
            _db_controller = new DB.metro_db_controller(db_controller);
            
        }

        public void setObjDB( EquPath equPath_Arg)
        {
            equPath = equPath_Arg;
        }


        [DisplayName("номер пути")]
        public int Track
        {
            get
            {
                equLineTmp = (EquLine)equPath.Parent;
                equGroupTmp = (EquGroup)equLineTmp.Parent;
                equClassTmp = (EquClass)equGroupTmp.Parent;

                var res = from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == equClassTmp.Code && r.GroupNum == equGroupTmp.Code && r.LineNum == equLineTmp.Code && r.Track == equPath.Code select r;

                if (res.Count() > 0)
                {
                    Name = res.First().Track;
                    return Name;
                }

                return -1;
            }
            set
            {
                int pathNum = value;
                
                if(pathNum>=0)
                {
                    if (pathNum < 100000)
                    {
                        _db_controller.all_equipment_adapter.renameTrack(pathNum, equPath.Code, equClassTmp.Code, equGroupTmp.Code, equLineTmp.Code);
                        _db_controller.refresh();
                        equPath.Code = pathNum;
                        FireRename(new RenameEvent(Convert.ToString(pathNum)));
                    }
                    else
                        MessageBox.Show("Введено слишком большое значение", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Номер пути не может быть представлен отрицательным числом", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
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
