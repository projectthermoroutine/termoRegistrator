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
        int Name = -1;
        EquGroup equGroupTmp;
        EquLine equLineTmp;
        EquClass equClassTmp;
        EquPath equPath;
        public PathSettings(DB.metro_db_controller db_controller)
        {
            _db_controller = new DB.metro_db_controller(db_controller);
            
        }

        public void setObjDB(EquTreeNode PathTreeNode)
        {
            equPath     = PathTreeNode.ObjectDB as EquPath;
            equLineTmp  = (PathTreeNode.Parent as EquTreeNode).ObjectDB as EquLine;
            equGroupTmp = (PathTreeNode.Parent.Parent as EquTreeNode).ObjectDB as EquGroup;
            equClassTmp = (PathTreeNode.Parent.Parent.Parent as EquTreeNode).ObjectDB as EquClass;
        }


        [DisplayName("номер пути")]
        public int Track
        {
            get
            {
                var res = from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == equClassTmp.Code && r.GroupNum == equGroupTmp.Code && r.LineNum == equLineTmp.Code && r.Track == equPath.Code select r;

                if (res.Count() == 0)
                 return -1;
    
                Name = res.First().Track;
                return Name;
            }
            set
            {
                if (value >= 0)
                {
                    if (value < 100000)
                    {
                        _db_controller.all_equipment_adapter.renameTrack(value, equPath.Code, equClassTmp.Code, equGroupTmp.Code, equLineTmp.Code);
                        _db_controller.refresh();
                        equPath.Code = value;
                        FireRename(new RenameEvent(Convert.ToString(value)));
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
