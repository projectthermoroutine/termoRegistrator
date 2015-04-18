﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;

namespace Registrator.Equipment
{
    public class LineSettings
    {
        private string m_peregonName = "test1";
        private DB.DataBaseHelper dbHelper;
        private EquLine equLine;
        public LineSettings(DB.DataBaseHelper dbHelper_Arg, EquLine equLine_Arg)
        {
            dbHelper = dbHelper_Arg;

            equLine = equLine_Arg;
        }

        [ReadOnly(true)]
        [DisplayName("код линии")]
        public string LineCode
        {
            get {

                var res = from r in dbHelper.dataTable_Lines.AsEnumerable() where r.LineCode == equLine.LineCode select r;

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

        [ReadOnly(true)]
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
