using NLog;
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
        private static readonly Logger logger = LogManager.GetCurrentClassLogger();
        public event EventHandler<DbObjectEventArg> EquObjectAddedEvent;
        void EquObjectAdded(EquDbObject db_object)
        {
            EquObjectAddedEvent?.Invoke(this, new DbObjectEventArg(db_object));
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

            foreach (var item in _db_controller.dbContext.Lines)
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

            if (name.IndexOfAny(RegistratorFormStrings.incorrect_symbols) == -1)
            {
                if (line_code.Length != 0)
                {
                    if (line_code.IndexOfAny(RegistratorFormStrings.incorrect_symbols) == -1 || line_code.Length < 50  )
                    {
                        int lineNum = 0;

                        if (_db_controller.dbContext.Lines.Count() > 0)
                            lineNum = _db_controller.dbContext.Lines.Select(x => x.LineNum).Max();

                        lineNum++;
                        

                        DB.EFClasses.Line l = _db_controller.dbContext.Lines.Add(new DB.EFClasses.Line { LineCode = line_code, LineName = name, LineNum = lineNum, StartCoordinate = 0 });

                        try
                        {
                            _db_controller.dbContext.SaveChanges();

                            var new_object = new EquLine(lineNum, name, equGroup);

                            EquObjectAdded(new_object);
                        }
                        catch(System.Data.Entity.Infrastructure.DbUpdateException e0)
                        {
                            logger.ErrorException(e0.Message,e0);
                            MessageBox.Show(e0.Message, "Database update error",
                                MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                        
                        catch(System.Data.Entity.Validation.DbEntityValidationException e1)
                        {
                            logger.ErrorException(e1.Message, e1);
                            MessageBox.Show(e1.Message, "Database validation error",
                                MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }

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
                if (TxtBx_Name.Text.IndexOfAny(RegistratorFormStrings.incorrect_symbols) != -1)
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
