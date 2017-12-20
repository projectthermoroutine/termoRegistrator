﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Registrator.Equipment.CreateDbObjectsCtrls
{
    public partial class CreateClassForm : Form
    {
        public event EventHandler<DbObjectEventArg> EquObjectAddedEvent;
        void EquObjectAdded(EquDbObject db_object)
        {
            EventHandler<DbObjectEventArg> handler = EquObjectAddedEvent;
            if (handler != null)
                handler(this, new DbObjectEventArg(db_object));
        }

        public DB.metro_db_controller _db_controller;

        public CreateClassForm(DB.metro_db_controller db_controller, EquDbObject parent)
        {
            _db_controller = null;
            if (db_controller != null)
                _db_controller = new DB.metro_db_controller(db_controller);
            
            InitializeComponent();

            foreach (string line in (from r in _db_controller.classes_table.AsEnumerable() where r.Code!=0 select r["Class"]).ToList())
                listBox1.Items.Add(line);
        }
   
        private void button2_Click(object sender, EventArgs e)
        {
            StringBuilder sb = new StringBuilder();
            string newElementName = TxtBx_GroupName.Text.Trim();

            if (newElementName.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*', '`', '№', '\\', '|' }) == -1)
            {
                if (newElementName.Length != 0)
                {
                    int classMaxIndex = _db_controller.dbContext.Classes.Max(c => c.Code);
                    classMaxIndex++;

                    if (newElementName.Length < 20)
                    {
                        var res = from r in _db_controller.classes_table.AsEnumerable() where r.Class == newElementName select new { r.Class }; 

                        if (res.Count() == 0)
                        {
                            _db_controller.dbContext.Classes.Add(new DB.EFClasses.Class() { Code = classMaxIndex, Class1 = newElementName });

                            EquObjectAdded(new EquClass(classMaxIndex, newElementName));

                            Close();
                            Dispose();
                        }
                        else
                            MessageBox.Show("Класс с таким именем уже существует", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    else
                        MessageBox.Show("Введено слишком длинное название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Dispose();
            Close();
        }

        private void TxtBx_GroupName_TextChanged(object sender, EventArgs e)
        {
            if (TxtBx_GroupName.Text.Length > 0)
                button2.Enabled = true;
        }
    }
}
