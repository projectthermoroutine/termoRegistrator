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
    public partial class AddTrack : Form
    {
        private AddObjectOnTreeView addObjectOnTreeView;
        public DB.DataBaseHelper dbHelper;
        public string newGroupName;
        private string setDataTable;
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
        public AddTrack(DB.DataBaseHelper dbHelperArg, AddObjectOnTreeView sender, string setDataTableArg)
        {
            InitializeComponent();

            dbHelper = dbHelperArg;
            
            //foreach (object Track in (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.Track != 0 select r["Track"]).Distinct().ToList())
            //    listBox1.Items.Add(Convert.ToString(Track));

            //TxtBx_Name.Enabled = false;
            button2.Enabled = false;

            setDataTable = setDataTableArg;

            addObjectOnTreeView = sender;
        }
        public void Line(ref EquLine LineArg, ref EquGroup GroupArg, ref EquClass EquClassArg)
        {
            equLine = LineArg;
            equGroup = GroupArg;
            equClass = EquClassArg;
        }
        public void Path(ref EquPath PathArg, ref EquLine LineArg, ref EquGroup GroupArg, ref EquClass EquClassArg)
        {
            equPath = PathArg;
            equLine = LineArg;
            equGroup = GroupArg;
            equClass = EquClassArg;
        }
        private void button2_Click(object sender, EventArgs e)
        {
            string newCode =txtBx_number.Text.Trim();
            //string newName = TxtBx_Name.Text.Trim();

            //if (newName.Length>49)
            //{
            //    MessageBox.Show("Слишком длинное название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            //    return;
            //}


            if (newCode.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*' }) == -1
                /*&& newName.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*' }) == -1*/)
            {
                if (newCode.Length != 0)
                {
                    switch (setDataTable)
                    {
                        case "Track":
                            int trackNum;
                            if (int.TryParse(newCode, out trackNum))
                            {
                                if (trackNum >= 1)
                                {
                                    if (trackNum < 10000)
                                    {
                                        var res22 = from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.ClassNum == equClass.Code && r.LineNum == equLine.Code && r.GroupNum == equGroup.Code select new { r.Track };  // check name duplicate
                                        var itemTrack = res22.First();

                                        if (trackNum != itemTrack.Track)
                                        {
                                            dbHelper.TblAdapter_AllEquipment.Path1(equClass.Code, equGroup.Code, equLine.Code, trackNum);

                                            addObjectOnTreeView(trackNum, newCode, "Track");

                                            Close();
                                            Dispose();
                                        }
                                        else
                                            MessageBox.Show("Путь с таким номер уже существует", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                    }
                                    else
                                        MessageBox.Show("Введено слишком большое число", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                }
                                else
                                    MessageBox.Show("Номер пути должен быть больше нуля", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                            }
                            else
                                MessageBox.Show("Некорректно введен номер пути", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                            break;
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


        private void txtBx_number_TextChanged(object sender, EventArgs e)
        {
            if(txtBx_number.Text.Length > 0)
            {
                //TxtBx_Name.Enabled = true;
                button2.Enabled = true;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Close();
            Dispose();
        }

        private void TxtBx_Name_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
