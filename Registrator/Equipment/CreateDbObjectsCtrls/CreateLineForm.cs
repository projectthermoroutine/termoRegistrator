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
    public partial class CreateLineForm : Form
    {
        public event EventHandler<DbObjectEventArg> EquObjectAddedEvent;
        void EquObjectAdded(EquDbObject db_object)
        {
            EventHandler<DbObjectEventArg> handler = EquObjectAddedEvent;
            if (handler != null)
                handler(this, new DbObjectEventArg(db_object));
        }

        public DB.metro_db_controller _db_controller;
        public string newGroupName;
        public int lineNumer;
        public int Track;
        
        //
        public PicketsManager PicketsObj;
        public EquClass equClass;
        public EquGroup equGroup;
        public EquPath equPath;
        //
        public CreateLineForm(DB.metro_db_controller db_controller, EquDbObject parent)
        {
            InitializeComponent();

            _db_controller = null;
            if (db_controller != null)
                _db_controller = new DB.metro_db_controller(db_controller);

            var line = (from r in _db_controller.lines_table.AsEnumerable() where r.LineNum != 0 select new { r.LineNum, r.LineName, r.LineCode }).Distinct().ToList();
            
            foreach (var item in line)
                listBox1.Items.Add("Линия " + Convert.ToString(item.LineCode) + " - " + Convert.ToString(item.LineName));

            equGroup = parent as EquGroup;
            equClass = equGroup.Class;
        }
    
        private void button2_Click(object sender, EventArgs e)
        {
            Int32 StartCoordinate = (Int32)numericUpDown_offset.Value;

            if (txtBx_number.Text.Length == 0 || TxtBx_Name.Text.Length == 0 )
            {
                MessageBox.Show("Необходимо заполнить все поля");
                return;
            }

            string newCode = txtBx_number.Text.Trim();
            string newName = TxtBx_Name.Text.Trim();

            if (newName.Length > 49)
            {
                MessageBox.Show("Слишком длинное название");
                return;
            }

            if (newName.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*' }) == -1)
            {
                if (newCode.Length != 0)
                {
                    if (newCode.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*' }) == -1 || newCode.Length<50  )
                    {
                        lineNumer = Convert.ToInt32(_db_controller.lines_adapter.selectMaxIndex());
                        lineNumer++;

                        var res = from r in _db_controller.lines_table.AsEnumerable() where r.LineNum != 0 && r.LineCode == newCode select new { r.LineNum };
                        
                        if (res.Count() == 0)
                        {
                            _db_controller.lines_adapter.addLineTblLines(lineNumer, newName, StartCoordinate, newCode);
                        }

                        _db_controller.all_equipment_adapter.AddLineToMainTable(equClass.Code, equGroup.Code, lineNumer);

                        var new_object = new EquLine(lineNumer, newName, equGroup);

                        _db_controller.all_equipment_table.Clear();
                        _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);
                        _db_controller.layout_table.Clear();
                        _db_controller.layout_adapter.Fill(_db_controller.layout_table);
                        _db_controller.lines_table.Clear();
                        _db_controller.lines_adapter.Fill(_db_controller.lines_table);

                        EquObjectAdded(new_object);
                        Close();
                        Dispose();
                      
                     }
                     else
                        MessageBox.Show("Некорректно введен код линии, либо слишком длинное название.");
                }
            }
        }

        private void txtBx_number_TextChanged(object sender, EventArgs e)
        {
            if (txtBx_number.Text.Length > 0)
            {
               if (txtBx_number.Text.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*' }) == -1 )
               {  }
                else
                    MessageBox.Show("Некорректно введен код");
            }
      
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Dispose();
            Close();
        }

        private void TxtBx_Name_TextChanged(object sender, EventArgs e)
        {

        }
    }
}