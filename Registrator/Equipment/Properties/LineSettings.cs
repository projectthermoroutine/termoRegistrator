using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;

namespace Registrator.Equipment
{
    public class LineSettings
    {
        private DB.metro_db_controller _db_controller;
        private EquLine equLine;
        public LineSettings(DB.metro_db_controller db_controller)
        {
            _db_controller = new DB.metro_db_controller(db_controller);
        }

        public void setObjDB(EquTreeNode LineTreeNode)
        {
            equLine = LineTreeNode.ObjectDB as EquLine;
        }

        [DisplayName("код линии")]
        public string LineCode
        {
            get { return _db_controller.dbContext.Lines.Where(l => l.LineNum == equLine.Code).Distinct().FirstOrDefault().LineCode; }
            set 
            {
                string str = value;
                if (str.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*', '`', '№', '\\', '|' }) == -1)
                {
                    if (str.Length < 100)
                    {
                        _db_controller.queriesAdapter.renameLineCode(equLine.Code, str);
                        _db_controller.refresh();
                        FireRename(new RenameEvent(str));
                    }
                    else
                        MessageBox.Show("Введено слишком длинное название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Некорректно введено название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        [DisplayName(" смещение от начала координат(см)")]
        public long shiftFromBegin
        { 
            get
            {
                return _db_controller.dbContext.Lines.Where(l => l.LineCode == equLine.LineCode).Where(l => l.LineNum == equLine.Code).Distinct().FirstOrDefault().StartCoordinate;
            }
            set 
            {
                long startCoordinate = (long)value;
                _db_controller.queriesAdapter.updateStartLineCoordinate(equLine.Code, startCoordinate);
                _db_controller.refresh();
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
