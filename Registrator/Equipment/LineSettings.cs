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
        private DB.DataBaseHelper dbHelper;
        private EquLine equLine;
        public LineSettings(DB.DataBaseHelper dbHelper_Arg)
        {
            dbHelper = dbHelper_Arg;
        }

        public void setObjDB(EquLine equLine_Arg)
        {
            equLine = equLine_Arg;
        }

        [DisplayName("код линии")]
        public string LineCode
        {
            get {

                var res = from r in dbHelper.dataTable_Lines.AsEnumerable() where r.LineNum == equLine.Code select r;

                if(res.Count() == 1)
                {
                    return res.First().LineCode;
                }

                return ""; 
            }
            set 
            {
                string str = value;
                if (str.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*', '`', '№', '\\', '|' }) == -1)
                {
                    if (str.Length < 100)
                    {
                        dbHelper.TblAdapter_Lines.renameLineCode(equLine.Code, str);
                        dbHelper.refresh();
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
                var res = from r in dbHelper.dataTable_Lines.AsEnumerable() where r.LineCode == equLine.LineCode select r;

                if (res.Count() == 1)
                {
                    return res.First().StartCoordinate;
                }

                return -1;
            }
            set 
            {
                long startCoordinate = (long)value;
                dbHelper.TblAdapter_Lines.updateStartLineCoordinate(equLine.Code, startCoordinate);
                dbHelper.refresh();
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
