using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Registrator.Equipment.CreateDbObjectsCtrls
{
    public partial class CreateLineForm : Form
    {
        public event EventHandler<DbObjectEventArg> EquObjectAddedEvent;
        void EquObjectAdded(EquDbObject db_object)
        {
            EventHandler<DbObjectEventArg> handler = EquObjectAddedEvent;
            if (handler != null)
                handler(this, new DbObjectEventArg(db_object));
        }

        DB.metro_db_edit_controller _db_controller;
        
        EquClass equClass;
        EquGroup equGroup;
    
        public CreateLineForm(DB.metro_db_edit_controller db_controller, EquDbObject parent)
        {
            InitializeComponent();
            
            _db_controller = null;
            if (db_controller != null)
                _db_controller = new DB.metro_db_edit_controller(db_controller);

            foreach (var item in (from r in _db_controller.lines_table.AsEnumerable() where r.LineNum != 0 select new {r.LineName, r.LineCode }).ToList())
                dataGridView1.Rows.Add(new object[] { Convert.ToString(item.LineName), Convert.ToString(item.LineCode) });

            equGroup = parent as EquGroup;
            equClass = equGroup.Class;
        }
    
        private void button2_Click(object sender, EventArgs e)
        {
            if (txtBx_line_code.Text.Length == 0 || TxtBx_Name.Text.Length == 0 )
            {
                MessageBox.Show("Необходимо заполнить все поля");
                return;
            }

            string line_code = txtBx_line_code.Text.Trim();
            string name = TxtBx_Name.Text.Trim();

            if (name.Length > 49)
            {
                MessageBox.Show("Слишком длинное название");
                return;
            }

            if (name.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*' }) == -1)
            {
                if (line_code.Length != 0)
                {
                    if (line_code.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*' }) == -1 || line_code.Length<50  )
                    {
                        string error_msg = "";
                        int line_number = _db_controller.add_line(equClass.Code, equGroup.Code, name, line_code, ref error_msg);
                        
                        if(line_number == 0)
                        {
                            MessageBox.Show("Ошибка базы данных. Операция отменена." + error_msg, "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            return;
                        }

                        var new_object = new EquLine(line_number, name, equGroup);

                        EquObjectAdded(new_object);

                        Close();
                        Dispose();
                     }
                     else
                        MessageBox.Show("Некорректно введен код линии, либо слишком длинное название.");
                }
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Dispose();
            Close();
        }

        private void TxtBx_Name_TextChanged(object sender, EventArgs e)
        {
            if (TxtBx_Name.Text.Length > 0)
            {
                if (TxtBx_Name.Text.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*' }) != -1)
                    MessageBox.Show("Некорректно введен код");
                else
                    txtBx_line_code.Text = TxtBx_Name.Text;
            }
        }

        private void dataGridView1_MouseClick(object sender, MouseEventArgs e)
        {
            if(dataGridView1.SelectedRows.Count > 0)
            {
                TxtBx_Name.Text = dataGridView1.SelectedRows[0].Cells[0].Value as string;
                txtBx_line_code.Text = dataGridView1.SelectedRows[0].Cells[1].Value as string;
            }
        }
    }
}
