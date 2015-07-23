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
    public partial class AddPicket : Form
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
        public AddPicket(DB.metro_db_controller db_controller, DisplayTheAddedObject sender)
        {
            _db_controller = new DB.metro_db_controller(db_controller);

            InitializeComponent();

            this.listBox1.SelectedIndexChanged += new System.EventHandler(this.listBox1_SelectedIndexChanged);

            button2.Enabled = false;

            this.Text = "Добавление нового пикета";
            label2.Text = "Список пикетов";
            label1.Text = "Введите номер нового пикета";

            displayTheAddedObjectOnParentForm = sender;

        }

        public void Pickets(int picketNumberArg, ref Pickets PicketsArg, EquPath equPath_Arg, EquLine equLine_Arg, Peregons peregonArg)
        {
            peregonObj = peregonArg;
            peregonNumber = picketNumberArg;
            TxtBx_GroupName.Enabled = false;
            PicketsObj = PicketsArg;
            listBox1.Items.AddRange(PicketsObj.lstPicketsNumberDislay.ToArray());

            for (int i = 1; i < listBox1.Items.Count - 1; i++)
                listBox1.DisableItem(i);

            equPath = equPath_Arg;
            equLine = equLine_Arg;
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (PicketsObj.picketNumber == -1)
                TxtBx_GroupName.Enabled = true;

            PicketsObj.processListBoxSelect(listBox1.SelectedIndex);
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

        private void button2_Click(object sender, EventArgs e)
        {
            string dlinaPicketa = txtBx_dlinaPicketa.Text;
            int dlinaPicket = 0;

            if (!int.TryParse(dlinaPicketa, out dlinaPicket))
                MessageBox.Show("Некорректно введена длина пикета", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            if (dlinaPicket < 1)
                MessageBox.Show("Длина пикета должна быть больше 0", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            if (dlinaPicket > 100000)
                MessageBox.Show("Введена слишком большая длина пикета", "", MessageBoxButtons.OK, MessageBoxIcon.Information);

            string newElementName = TxtBx_GroupName.Text.Trim();

            int resMaxNumberIndex = Convert.ToInt32(_db_controller.pickets_adapter.selectMaxNumberIndex());

            if (newElementName.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*' }) == -1)
            {
                if (newElementName.Length != 0)
                {
                    int newPicketNum;
                    if (int.TryParse(newElementName, out newPicketNum))
                    {
                        if (newPicketNum >= 1)
                        {
                            if (newPicketNum < 10000)
                            {
                                //var res3 = from r in _db_controller.all_equipment_table.AsEnumerable() where r.Npicket == newPicketNum && r.Track == equPath.Code && r.LineCode == equLine.LineCode  select new { r.Npicket };  // check name duplicate
                                var resDuplicate = from r in _db_controller.pickets_table.AsEnumerable() where r.Peregon == peregonObj.layoutNumber && r.Npiketa == newPicketNum && r.path == equPath.Code && r.line == equLine.Code select new { r.Npiketa };  // check name duplicate
                                if (resDuplicate.Count() == 0)
                                {
                                    resMaxNumberIndex++;
                                    PicketsObj.calcNewPicketNumber(resMaxNumberIndex);
                                    _db_controller.pickets_adapter.PicketCreate(newPicketNum, equLine.Code, equPath.Code, peregonNumber, PicketsObj.typeOfPicketCreation, PicketsObj.newPicketIndex, dlinaPicket, PicketsObj.after1, PicketsObj.before2);

                                    displayTheAddedObjectOnParentForm(newElementName, "Pickets");

                                    Close();
                                    Dispose();
                                }
                                else
                                    MessageBox.Show("Пикет с таким номером уже существует в текущем перегоне(станции)", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                            }
                            else
                                MessageBox.Show("Введено слишком большое число", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                        }
                        else
                            MessageBox.Show("Номер пути должен быть больше нуля", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
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
    }
}
