using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;

namespace Registrator.Equipment
{
    public class PeregonProperties
    {
        private string m_peregonName = "test1";
        private DB.metro_db_controller _db_controller;
        private EquLayout equLayout;
        private string peregonName = null;
        private int m_peregonLength;
        public PeregonProperties(DB.metro_db_controller db_controller )
        {
            _db_controller = new DB.metro_db_controller(db_controller);

           
        }

        public void setObjDb(EquLayout equLayout_Arg)
        {
             equLayout = equLayout_Arg;
        }

        //[ReadOnly(true)]
        [DisplayName("название")]
        public string peregonKName
        {
            get {
                
                var res = from r in _db_controller.layout_table.AsEnumerable() where r.Code == equLayout.Code select r;

                if(res.Count() == 1)
                {
                    peregonName = res.First().Layout;
                    return res.First().Layout;
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
                        _db_controller.layout_adapter.renamePeregon(equLayout.Code, str);

                        _db_controller.layout_table.Clear();
                        _db_controller.layout_adapter.Fill(_db_controller.layout_table);

                        FireRenamePeregon(new RenameEvent(str));

                        m_peregonName = value;
                    }
                    else
                        MessageBox.Show("Введено слишком длинное название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Некорректно введено название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }


        }
        [ReadOnly(true)]
        [DisplayName("длина перегона(см)")]
        public  int peregonLength
        {
            get 
            {
                var res = from r in _db_controller.layout_table.AsEnumerable() where r.Code == equLayout.Code select r;

                if (res.Count() == 1)
                {
                    m_peregonLength = res.First().PeregonLength;
                    return res.First().PeregonLength;
                }

                return -1;
            }
            set
            {
                m_peregonLength=value;
                if (m_peregonLength > 0)
                {
                    if (m_peregonLength < 900000)
                        _db_controller.layout_adapter.setPeregonLength(equLayout.Code, m_peregonLength);
                    else
                        MessageBox.Show("Значение слишком велико", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Значение должно быть больше 0", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            
            }

        }

        public event EventHandler<RenameEvent> RenamePeregonEventHandler;

        public virtual void FireRenamePeregon(RenameEvent e)
        {
            EventHandler<RenameEvent> handler = RenamePeregonEventHandler;

            if (handler != null)
            {
                handler(this, e);
            }
        }
    }
}
