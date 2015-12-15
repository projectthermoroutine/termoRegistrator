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
    public partial class CreatePicketForm : Form
    {
        public event EventHandler<DbObjectEventArg> PicketAddedEvent;
        void PicketAdded(EquDbObject db_object)
        {
            EventHandler<DbObjectEventArg> handler = PicketAddedEvent;
            if (handler != null)
                handler(this, new DbObjectEventArg(db_object));
        }

        DB.metro_db_controller _db_controller;
        //
        PicketsManager PicketsManager;
        EquClass equClass;
        EquGroup equGroup;
        EquLine equLine;
        EquPath equPath;

        public CreatePicketForm(DB.metro_db_controller db_controller, EquDbObject parent)
        {
            _db_controller = new DB.metro_db_controller(db_controller);

            InitializeComponent();

            numUpDownSingleLength.Value = (decimal)Registrator.Properties.Settings.Default.DefaultPicketLength;
            
            buttonApply.Enabled = false;
            equPath = parent as EquPath;
            equLine = equPath.Parent as EquLine;
            equGroup = equLine.Parent as EquGroup;
            equClass = equGroup.Parent as EquClass;

            PicketsManager = new PicketsManager(db_controller);
            PicketsManager.createLogicalPicketList(equPath);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Dispose();
            Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            int addedPicketID = Convert.ToInt32(_db_controller.pickets_adapter.selectMaxNumberIndex());
            
            if (chBxRange.Checked)
                addRangePickets(addedPicketID);
            else
                addOnePicket(addedPicketID);

            _db_controller.all_equipment_table.Clear();
            _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);
            _db_controller.pickets_table.Clear();
            _db_controller.pickets_adapter.Fill(_db_controller.pickets_table);

            PicketAdded(equPath);

            Close();
            Dispose();
        }
        void addOnePicket(int addedPicketID)
        {
            addPicket(numUpDownNum.Value.ToString(), addedPicketID);
        }
        void addRangePickets(int addedPicketID)
        {
            if (numUpDownFrom.Value > numUpDownTo.Value)
            {
                MessageBox.Show("Диапазон пикетов задан не верно", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            bool tmp = false;

            if(numUpDownFrom.Value < 0)
                tmp = true;

            if (PicketsManager.AddToLeft) 
            {
                for (int i = (int)numUpDownTo.Value; i >= (int)numUpDownFrom.Value; i--) 
                {
                    if(tmp && i==0)
                    {
                        if (!addPicket("-0", addedPicketID)) return;
                    }
                    if (!addPicket(i.ToString(), addedPicketID)) return;
                }
            }
            else 
            {
                
                for (int i = (int)numUpDownFrom.Value; i <= (int)numUpDownTo.Value; i++) 
                {
                    if (tmp && i == 0)
                    {
                        if (!addPicket("-0", addedPicketID)) return;
                    }
                    if (!addPicket(i.ToString(), addedPicketID)) return;
                }
            }
        }

        bool addPicket(string picketDisplayNum, int addedPicketID)
        {
            if (!checkDuplicate(picketDisplayNum))
            {
                MessageBox.Show("Пикет " + picketDisplayNum + " уже создан", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return false;
            }

            addedPicketID++;
            EquPicket picket = PicketsManager.AddPicketToDB(picketDisplayNum, equClass.Code, equGroup.Code, equLine.Code, equPath.Code, addedPicketID, (int)numUpDownSingleLength.Value * 10);

            var empData = from r in _db_controller.all_equipment_table.AsEnumerable() where r.number == addedPicketID && r.number != 0 && r.LineNum == equLine.Code && r.Track == equPath.Code select new { r.number };

            if (empData.Count() == 0)
            {
                _db_controller.all_equipment_adapter.PicketAdd(equClass.Code, equGroup.Code, equLine.Code, equPath.Code, 0, addedPicketID);
            }
            else
            {
                MessageBox.Show("Пикет с таким номером уже присутствует на пути", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return false;
            }

            return true;
        }

        bool checkDuplicate(string PicketNum)
        {
            var resDuplicate = from r in _db_controller.pickets_table.AsEnumerable() where r.Npiketa == PicketNum && r.path == equPath.Code && r.line == equLine.Code select new { r.Npiketa };

            if (resDuplicate.Count() != 0)
            {
                MessageBox.Show("Пикет с таким номером уже существует в текущем перегоне(станции)", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return false;
            }

            return true;
        }
        private void chBxRange_CheckedChanged(object sender, EventArgs e)
        {
            if (chBxRange.Checked) 
            {
                numUpDownFrom.Enabled = true;
                numUpDownTo.Enabled   = true;
                numUpDownNum.Enabled  = false;
            }
            else {
                numUpDownFrom.Enabled = false;
                numUpDownTo.Enabled   = false;
                numUpDownNum.Enabled  = true;
            }
        }

        private void buttonApplyDefault_Click(object sender, EventArgs e)
        {
             Registrator.Properties.Settings.Default.DefaultPicketLength = (int)numUpDownSingleLength.Value;
        }

        private void chBxNonDefault_CheckedChanged(object sender, EventArgs e)
        {
            if (chBxNonDefault.Checked)
                numUpDownSingleLength.Enabled = true;
            else
                numUpDownSingleLength.Enabled = false;
        }

        private void radioButtonLeft_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButtonLeft.Enabled)
            {
                PicketsManager.AddToLeft = true;
                buttonApply.Enabled = true;

                if (PicketsManager.PicketsList.Count != 0)
                {
                    numUpDownNum.Value = Convert.ToInt32(PicketsManager.PicketsList[0].npicket) - 1;
                    numUpDownFrom.Value = Convert.ToInt32(PicketsManager.PicketsList[0].npicket) - 2;
                    numUpDownTo.Value = Convert.ToInt32(PicketsManager.PicketsList[0].npicket) - 1;
                }
                else
                {
                    numUpDownNum.Value = 0;
                    numUpDownFrom.Value = 0;
                    numUpDownTo.Value = 0;
                }
            }
        }

        private void radioButtonRight_CheckedChanged(object sender, EventArgs e)
        {
            if(radioButtonRight.Enabled)
            {
                PicketsManager.AddToLeft = false;
                buttonApply.Enabled = true;

                if (PicketsManager.PicketsList.Count != 0)
                {
                    numUpDownNum.Value = Convert.ToInt32(PicketsManager.PicketsList[PicketsManager.PicketsList.Count - 1].npicket ) + 1;
                    numUpDownFrom.Value = Convert.ToInt32(PicketsManager.PicketsList[PicketsManager.PicketsList.Count - 1].npicket) + 1;
                    numUpDownTo.Value = Convert.ToInt32(PicketsManager.PicketsList[PicketsManager.PicketsList.Count - 1].npicket)   + 2;
                }
                else
                {
                    numUpDownNum.Value = 0;
                    numUpDownFrom.Value = 0;
                    numUpDownTo.Value = 0;
                }
            }
        }
    }
}
