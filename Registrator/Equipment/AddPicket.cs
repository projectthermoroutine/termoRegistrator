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
        private AddObjectOnTreeView UpdateTreeNode;
        public DB.metro_db_controller _db_controller;
        public string newGroupName;
        public int lineNumer;
        public int Track;
        //
        public PicketsManager PicketsManager;
        public EquClass equClass;
        public EquGroup equGroup;
        public EquLine equLine;
        public EquPath equPath;

        public AddPicket(DB.metro_db_controller db_controller, AddObjectOnTreeView sender,  EquTreeNode PathTreeNode)
        {
            _db_controller = new DB.metro_db_controller(db_controller);

            InitializeComponent();

            numUpDownSingleLength.Value = (decimal)Registrator.Properties.Settings.Default.DefaultPicketLength;
            UpdateTreeNode = sender;
            
            buttonApply.Enabled = false;
            equPath  = PathTreeNode.ObjectDB as EquPath;
            equLine  = (PathTreeNode.Parent as EquTreeNode).ObjectDB as EquLine;
            equGroup = (PathTreeNode.Parent.Parent as EquTreeNode).ObjectDB as EquGroup;
            equClass = (PathTreeNode.Parent.Parent.Parent as EquTreeNode).ObjectDB as EquClass;
            
            PicketsManager = new PicketsManager(db_controller,equLine.OffsetLineCoordinate);
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
            PicketsManager.AddPicketToDB(picketDisplayNum, equLine.Code, equPath.Code, addedPicketID, (int)numUpDownSingleLength.Value * 10);

            var empData = from r in _db_controller.all_equipment_table.AsEnumerable() where r.number == addedPicketID && r.number != 0 && r.LineNum == equLine.Code && r.Track == equPath.Code select new { r.number };

            if (empData.Count() == 0)
                _db_controller.all_equipment_adapter.PicketAdd(equClass.Code, equGroup.Code, equLine.Code, equPath.Code, 0, addedPicketID);
            else
            {
                MessageBox.Show("Пикет с таким номером уже присутствует на пути", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                UpdateTreeNode(addedPicketID, Convert.ToString(picketDisplayNum) + ";" + Convert.ToString(addedPicketID), "Picket");
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
                numUpDownNum.Value = Convert.ToInt32(PicketsManager.PicketsList[0].npicket) - 1;
                numUpDownFrom.Value = Convert.ToInt32(PicketsManager.PicketsList[0].npicket)- 2;
                numUpDownTo.Value = Convert.ToInt32(PicketsManager.PicketsList[0].npicket) - 1;
            }
        }

        private void radioButtonRight_CheckedChanged(object sender, EventArgs e)
        {
            if(radioButtonRight.Enabled)
            {
                PicketsManager.AddToLeft = true;
                buttonApply.Enabled = true;
                numUpDownNum.Value = Convert.ToInt32(PicketsManager.PicketsList[PicketsManager.PicketsList.Count - 1].npicket )+ 1;
                numUpDownFrom.Value = Convert.ToInt32(PicketsManager.PicketsList[PicketsManager.PicketsList.Count - 1].npicket) + 1;
                numUpDownTo.Value = Convert.ToInt32(PicketsManager.PicketsList[PicketsManager.PicketsList.Count - 1].npicket) + 2;
            }
        }
    }
}
