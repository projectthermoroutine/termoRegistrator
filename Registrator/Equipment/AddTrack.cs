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
        private MyDelegate d;
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
        public AddTrack(DB.DataBaseHelper dbHelperArg, MyDelegate sender, string setDataTableArg)
        {
            InitializeComponent();

            dbHelper = dbHelperArg;

            
                    foreach (object Track in (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.Track != 0 select r["Track"]).Distinct().ToList())
                        listBox1.Items.Add(Convert.ToString(Track));

                    this.Text = "Добавление нового пути";
                    lbl_created.Text = "Список путей";
                    lbl_Name.Text = "Введите номер нового пути";
                  

       

            TxtBx_Name.Enabled = false;
            button2.Enabled = false;

            setDataTable = setDataTableArg;

            d = sender;
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
            string newNumber =txtBx_number.Text.Trim();
            string newName = TxtBx_Name.Text.Trim();

            if (newName.Length>49)
            {
                MessageBox.Show("Слишком длинное название");
                return;
            }


            if (newNumber.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*' }) == -1
                && newName.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*' }) == -1)
            {
                if (newNumber.Length != 0)
                {
                    switch (setDataTable)
                    {
                        case "Track":
                            int trackNum;
                            if (int.TryParse(newNumber, out trackNum))
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

                                            d(trackNum, newNumber, "Track");

                                            Close();
                                            Dispose();
                                        }
                                        else
                                            MessageBox.Show("Путь с таким номер уже существует");
                                    }
                                    else
                                        MessageBox.Show("Введено слишком большое число");
                                }
                                else
                                    MessageBox.Show("Номер пути должен быть больше нуля");
                            }
                            else
                                MessageBox.Show("Некорректно введен номер пути");
                            break;
                    }
                }
                else
                    MessageBox.Show("Название не должно быть пустым");
            }
            else
            {
                MessageBox.Show("Некорректный символ: '@','.', ',', '!', ... ",
                   "Введите другое название");
            }
        }


        private void txtBx_number_TextChanged(object sender, EventArgs e)
        {
            if(txtBx_number.Text.Length > 0)
            {

            //    if (int.TryParse(txtBx_number.Text, out lineNumer))
            //    {
            //        var res = from r in dbHelper.dataTable_Lines.AsEnumerable() where r.LineNum == lineNumer select new { r.LineName };

            //        if (res.Count() > 0)
            //            TxtBx_Name.Text = res.First().LineName;
            //        else
                        TxtBx_Name.Enabled = true;

                    button2.Enabled = true;
            //    }
            //    else
            //    {
            //        MessageBox.Show("Некорректно введен номер");
            //        TxtBx_Name.Enabled = false;
            //        button2.Enabled = false;
            //    }
            //}
            //else
            //{
            //    TxtBx_Name.Enabled = false;
            //    button2.Enabled = false;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Close();
            Dispose();
        }
    }
}
