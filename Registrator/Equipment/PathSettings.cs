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
        private DB.DataBaseHelper dbHelper;
        private EquPath equPath;
        private int Name = -1;
        private int m_peregonLength;
        private EquGroup equGroupTmp;
        private EquLine equLineTmp;
        private EquClass equClassTmp;
        public PathSettings(DB.DataBaseHelper dbHelper_Arg)
        {
            dbHelper = dbHelper_Arg;
            
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

                var res = from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.ClassNum == equClassTmp.Code && r.GroupNum == equGroupTmp.Code && r.LineNum == equLineTmp.Code && r.Track == equPath.Code select r;

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
                        dbHelper.TblAdapter_AllEquipment.renameTrack(pathNum, equPath.Code, equClassTmp.Code, equGroupTmp.Code, equLineTmp.Code);
                        dbHelper.refresh();
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
