using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Windows.Documents;

namespace Registrator.Equipment.CreateDbObjectsCtrls
{
    public partial class CreateGroupForm : Form
    {
        public event EventHandler<DbObjectEventArg> EquObjectAddedEvent;
        void EquObjectAdded(EquDbObject db_object)
        {
            EventHandler<DbObjectEventArg> handler = EquObjectAddedEvent;
            if (handler != null)
                handler(this, new DbObjectEventArg(db_object));
        }

        DB.metro_db_controller _db_controller;
        EquClass equClass;
        Equipment.GroupColorSetUserControl gruopColorSetControl;
        int countClasses = 0;

        public CreateGroupForm(DB.metro_db_controller db_controller, EquDbObject parent)
        {
            _db_controller = new DB.metro_db_controller(db_controller);

            InitializeComponent();

            gruopColorSetControl = new Equipment.GroupColorSetUserControl();
            elementHost1.Child = gruopColorSetControl;

            button2.Enabled = false;
                
            for (int i = 0; i < _db_controller.groups_table.Rows.Count; i++)
            {
                if (Convert.ToString(_db_controller.groups_table.Rows[i].ItemArray[1]) == "notExist") 
                    continue;
                listBox1.Items.Add(Convert.ToString(_db_controller.groups_table.Rows[i].ItemArray[1]));
            }

            equClass = parent as EquClass;

        }

        private void button1_Click(object sender, EventArgs e)
        {
            Close();
            Dispose();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            string color = gruopColorSetControl.colorPicker.SelectedColor.ToString();

            StringBuilder sb = new StringBuilder();
            string newElementName = TxtBx_GroupName.Text.Trim();

            if (newElementName.IndexOfAny(new char[] { '@', '.', ',', '!','\'',';','[',']','{','}','"','?','>','<','+','$','%','^','&','*','`','№','\\','|'}) == -1)
            {
                int GroupIndex;

                if (newElementName.Length != 0)
                {
                    GroupIndex = Convert.ToInt32(_db_controller.groups_adapter.selectGroupMaxIndex());
                            
                    if (newElementName.Length < 20)
                    {
                        var res1 = from r in _db_controller.groups_table.AsEnumerable() where r.Group == newElementName select new { r.Code }; 

                        if (res1.Count() == 0)
                        {
                            int result1 = _db_controller.all_equipment_adapter.newGroup1(equClass.Code, ++GroupIndex, newElementName, color, countClasses);

                            var equ_group = new EquGroup(GroupIndex, newElementName, equClass);
                            
                            _db_controller.all_equipment_table.Clear();
                            _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);
                            _db_controller.groups_table.Clear();
                            _db_controller.groups_adapter.Fill(_db_controller.groups_table);

                            EquObjectAdded(equ_group);

                            Close();
                            Dispose();
                        }
                        else
                            MessageBox.Show("Группа с таким именем уже существует","", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    else
                        MessageBox.Show("Введено слишком длинное название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Название не должно быть пустым", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                MessageBox.Show("Некорректный символ: '@','.', ',', '!', ... ",
                   "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        private void TxtBx_GroupName_TextChanged(object sender, EventArgs e)
        {
            if (TxtBx_GroupName.Text.Length > 0)
                button2.Enabled = true;
        }
    }
}
