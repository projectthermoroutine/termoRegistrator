using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;

namespace Registrator.Equipment
{
    public class strelkaSettings
    {
        private string m_peregonName = "test1";
        private DB.DataBaseHelper dbHelper;
        private EquObject equObject;
        private string Name = null;
        private int m_peregonLength;

        public strelkaSettings(DB.DataBaseHelper dbHelper_Arg, EquObject equObject_Arg)
        {
            dbHelper = dbHelper_Arg;

            equObject = equObject_Arg;
        }

        [DisplayName("название")]
        public string equipmentKName
        {
            get
            {
                var res = from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Code == equObject.Code select r;

                if (res.Count() == 1)
                {
                    Name = res.First().Object;
                    return Name;
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
                        dbHelper.TblAdapter_Objects.renameEquipment(equObject.Code, str);
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
