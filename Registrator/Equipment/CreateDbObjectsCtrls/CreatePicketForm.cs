using Registrator.DB.EFClasses;
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
        void PicketAdded(EquDbObject[] db_objects, bool add_to_left)
        {
            EventHandler<DbObjectEventArg> handler = PicketAddedEvent;
            if (handler != null)
                handler(this, new DbObjectEventArg(db_objects,add_to_left));
        }

        DB.metro_db_controller _db_controller;
        //
        PicketsManager PicketsManager;
        EquClass equClass;
        EquGroup equGroup;
        EquLine equLine;
        EquPath equPath;
        List<EquDbObject> PicketsList;
        ContextMetroCard _dbContext;

        public CreatePicketForm(DB.metro_db_controller db_controller, EquDbObject parent, ContextMetroCard dbContext)
        {
            _dbContext = dbContext;
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

            PicketsList = new List<EquDbObject>();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Dispose();
            Close();
        }

        private void Button2Click(object sender, EventArgs e)
        {
            int addedPicketID = _db_controller.dbContext.Pickets.Max(p=>p.number);

            addedPicketID++;

            if (chBxRange.Checked)
                addRangePickets(addedPicketID);
            else
                addOnePicket(addedPicketID);

            PicketAdded(PicketsList.ToArray(), PicketsManager.AddToLeft);

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

            var qPicket = from p in _dbContext.Pickets where p.number == addedPicketID select new { p.number };

            if (qPicket.Count() == 0)
            { 
                Picket picket = PicketsManager.AddPicketToDB(picketDisplayNum, equClass.Code, equGroup.Code, equLine.Code, equPath.Code, addedPicketID, (int)numUpDownSingleLength.Value * 10);

                //var empData = from r in _db_controller.all_equipment_table.AsEnumerable() where r.number == addedPicketID && r.number != 0 && r.LineNum == equLine.Code && r.Track == equPath.Code select new { r.number };
            
                _db_controller.queriesAdapter.PicketAdd(equClass.Code, equGroup.Code, equLine.Code, equPath.Code, 0, addedPicketID);

                PicketsList.Add(
                    new EquPicket
                    {   after = picket.NpicketAfter,
                        before = picket.NpicketBefore,
                        keyNumber = picket.number,
                        LeftLineShift = picket.StartShiftLine,
                        RightLineShift = picket.EndShiftLine,
                        npicket = picket.Npiketa,
                        Code = picket.number,
                        lenght = picket.Dlina,
                        Name = picket.Npiketa,
                        Path = equPath,
                        Parent = equPath.Parent
                    });
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
            if (_db_controller.dbContext.Pickets.Where(p => p.Npiketa == PicketNum && p.path == equPath.Code).Distinct().Count()!= 0)
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
                    numUpDownNum.Value = Convert.ToInt32(PicketsManager.PicketsList[0].Npiketa) - 1;
                    numUpDownFrom.Value = Convert.ToInt32(PicketsManager.PicketsList[0].Npiketa) - 2;
                    numUpDownTo.Value = Convert.ToInt32(PicketsManager.PicketsList[0].Npiketa) - 1;
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
                    numUpDownNum.Value = Convert.ToInt32(PicketsManager.PicketsList[PicketsManager.PicketsList.Count - 1].Npiketa) + 1;
                    numUpDownFrom.Value = Convert.ToInt32(PicketsManager.PicketsList[PicketsManager.PicketsList.Count - 1].Npiketa) + 1;
                    numUpDownTo.Value = Convert.ToInt32(PicketsManager.PicketsList[PicketsManager.PicketsList.Count - 1].Npiketa)   + 2;
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
