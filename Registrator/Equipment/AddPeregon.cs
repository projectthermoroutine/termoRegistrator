using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Registrator.Equipment
{
    public partial class AddPeregon : Form
    {
        private DisplayTheAddedObject displayTheAddedObjectOnParentForm;
        public DB.metro_db_controller _db_controller;
        public string newGroupName;
        public int lineNumer;
        public int Track;
        //
        public Peregons peregonObj;
        public Pickets PicketsObj;
        public EquClass equClass;
        public EquGroup equGroup;
        public EquLine equLine;
        public EquPath equPath;
        public equipment equipObj;
        public int peregonNumber;
        //
        public AddPeregon(DB.metro_db_controller db_controller, DisplayTheAddedObject sender)
        {
            _db_controller = new DB.metro_db_controller(db_controller);

            InitializeComponent();

            this.listBox1.SelectedIndexChanged += new System.EventHandler(this.listBox1_SelectedIndexChanged);
            this.lstBx_peregon.SelectedIndexChanged += new System.EventHandler(this.lstBx_peregon_SelectedIndexChanged);
            
            button2.Enabled = false;
            displayTheAddedObjectOnParentForm = sender;
        }
        public void peregons(int lineNumberArg, int trackNumber, ref Peregons peregonsObjArg)
        {
            lineNumer = lineNumberArg;
            Track = trackNumber;
            peregonObj = peregonsObjArg;
            TxtBx_GroupName.Enabled = false;
            listBox1.Items.AddRange(peregonObj.lstPeregonNames.ToArray());
            listBox1.DisableItem(listBox1.Items.Count - 2);
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
           if( peregonObj.processListBoxSelect(listBox1.SelectedIndex, ref lstBx_peregon, ref TxtBx_GroupName) == 1)
           {
               button1.Enabled = false;
           }
        }
        private void lstBx_peregon_SelectedIndexChanged(object sender, EventArgs e)
        {
            peregonObj.processSecondSelectionListBox(lstBx_peregon.SelectedIndex);
            TxtBx_GroupName.Enabled = true;
        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            StringBuilder sb = new StringBuilder();
            string newElementName = TxtBx_GroupName.Text.Trim();

            if (newElementName.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*' }) == -1)
            {
                if (newElementName.Length != 0)
                {
               
                    peregonObj.newLayoutName = newElementName; // дублирует

                    if (newElementName.Length < 100)
                    {
                        var res4 = from r in _db_controller.layout_table.AsEnumerable() where r.Layout == newElementName select new { r.Layout };  // check name duplicate
                        if (res4.Count() == 0)
                        {
                            peregonObj.calcNewPregonNumber();
                            peregonObj.layoutNameShort = "SN";
                            _db_controller.layout_adapter.peregonCreate(lineNumer, peregonObj.indexSelectedfistOrLastItem, peregonObj.layoutNumber, peregonObj.after1, peregonObj.before2, peregonObj.after2, peregonObj.before3, peregonObj.newLayoutName, peregonObj.layoutNameShort, (int)num_PeregonLength.Value);
                            
                            displayTheAddedObjectOnParentForm(newElementName, "Peregon");
                            
                            Close();
                            Dispose();
                        }
                        else
                            MessageBox.Show("Перегон с таким именем уже существует", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
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

        private void button1_Click_1(object sender, EventArgs e)
        {
            Dispose();
            Close();
        }

        private void TxtBx_GroupName_TextChanged_1(object sender, EventArgs e)
        {
            if (TxtBx_GroupName.Text.Length > 0)
                button2.Enabled = true;
        }
    }
}
