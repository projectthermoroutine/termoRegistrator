using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.ComponentModel;

namespace Registrator.Equipment
{
    public class ClassSettings
    {
        private DB.DataBaseHelper dbHelper;
        private EquClass equClass;
        private string Name = null;
        private int m_peregonLength;

        public ClassSettings(DB.DataBaseHelper dbHelper_Arg)
        {
            dbHelper = dbHelper_Arg;
            
        }

        public void setObjDB( EquClass equGroup_Arg)
        {
            equClass = equGroup_Arg;
        }


        [DisplayName("название")]
        public string className
        {
            get
            {
                var res = from r in dbHelper.dataTable_Class.AsEnumerable() where r.Code == equClass.Code select r;

                if (res.Count() == 1)
                {
                    Name = res.First().Class;
                    return Name;
                }

                return "";
            }
            set
            {
                string str = value;
                if (str.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*', '`', '№', '\\', '|' }) == -1)
                {
                    if (str.Length < 20)
                    {
                        dbHelper.TblAdapter_Class.renameClass(str, equClass.Code);
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
