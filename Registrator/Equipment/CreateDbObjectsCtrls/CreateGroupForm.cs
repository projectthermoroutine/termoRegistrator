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

        public CreateGroupForm(DB.metro_db_controller db_controller, EquDbObject parent)
        {
            _db_controller = new DB.metro_db_controller(db_controller);

            InitializeComponent();

            gruopColorSetControl = new Equipment.GroupColorSetUserControl();
            elementHost1.Child = gruopColorSetControl;

            button2.Enabled = false;
                
            listBox1.Items.AddRange(_db_controller.dbContext.Groups.Distinct().Select(g => g.Group1).ToArray());

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
            string addingGroupName = TxtBx_GroupName.Text.Trim();

            if (addingGroupName.IndexOfAny(new char[] { '@', '.', ',', '!','\'',';','[',']','{','}','"','?','>','<','+','$','%','^','&','*','`','№','\\','|'}) == -1)
            {
                if (addingGroupName.Length != 0)
                {
                    int GroupIndex = 0;

                    try
                    {
                        GroupIndex = _db_controller.dbContext.Groups.Max(g => g.Code);
                    }
                    catch (System.Data.SqlClient.SqlException exception)
                    {
                        MessageBox.Show("Ошибка базы данных. Операция отменена. Код ошибки: " + exception.ErrorCode + "\n " + exception.Message, "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        
                        Close();
                        Dispose();

                        return;
                    }
                            
                    if (addingGroupName.Length < 20)
                    {
                        if (_db_controller.dbContext.Groups.Where(g=>g.Group1 == addingGroupName).Select(g=>g.Code).Distinct().Count()==0)
                        {
                            _db_controller.queriesAdapter.create_group(equClass.Code, ++GroupIndex, addingGroupName, color);

                            var equ_group = new EquGroup(GroupIndex, addingGroupName, equClass);

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
