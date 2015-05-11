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
    public partial class AddLine : Form
    {
        private AddObjectOnTreeView addObjectOnTreeView;
        public DB.DataBaseHelper dbHelper;
        public string newGroupName;
        private string EditMode;
        public int lineNumer;
        public int Track;
        public int StartCoordinate;
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
        public AddLine(DB.DataBaseHelper dbHelperArg, AddObjectOnTreeView sender, string setDataTableArg)
        {
            InitializeComponent();

            dbHelper = dbHelperArg;

            var line = (from r in dbHelper.dataTable_Lines.AsEnumerable() where r.LineNum != 0 select new { r.LineNum, r.LineName, r.LineCode }).Distinct().ToList();
            
            foreach (var item in line)
                listBox1.Items.Add("Линия " + Convert.ToString(item.LineCode) + " - " + Convert.ToString(item.LineName));

            EditMode = setDataTableArg;

            if (EditMode == "Edit")
            {
                this.Text = "Редактирование линии";
                button2.Text = "Редактировать";
            }

            addObjectOnTreeView = sender;
        }
        public void Line(ref EquLine LineArg, ref EquGroup GroupArg, ref EquClass EquClassArg)
        {
            equLine = LineArg;
            equGroup = GroupArg;
            equClass = EquClassArg;

            if (EditMode == "Edit")
            {
                var lineEdit = (from r in dbHelper.dataTable_Lines.AsEnumerable() where r.LineNum == equLine.Code select new { r.LineNum, r.LineName, r.StartCoordinate });

                var item = lineEdit.First();

                TxtBx_Name.Text = Convert.ToString(item.LineName);
                txtBx_number.Text = Convert.ToString(item.LineNum);
                txtBx_beginCoordinate.Text = Convert.ToString(item.StartCoordinate);
            }
        }
        private void button2_Click(object sender, EventArgs e)
        {
            if (txtBx_number.Text.Length == 0 || TxtBx_Name.Text.Length == 0 || txtBx_beginCoordinate.Text.Length == 0)
            {
                MessageBox.Show("Необходимо заполнить все поля");
                return;
            }

            string newCode = txtBx_number.Text.Trim();
            string newName = TxtBx_Name.Text.Trim();
            string startCoordinat_str = txtBx_beginCoordinate.Text.Trim();

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
                       var resLines = from r in dbHelper.dataTable_Lines.AsEnumerable() where r.LineCode == newCode select new { r.LineNum };

                       if (resLines.Count() == 0)
                       {
                           lineNumer = Convert.ToInt32(dbHelper.TblAdapter_Lines.selectMaxIndex());
                           lineNumer++;

                           if (lineNumer >= 1)
                           {
                               if (lineNumer < 10000)
                               {
                                   if (int.TryParse(startCoordinat_str, out StartCoordinate))
                                   {
                                        dbHelper.TblAdapter_Lines.addLineTblLines(lineNumer, newName, StartCoordinate, newCode);

                                        var res2 = from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.LineNum != 0 && r.GroupNum == equGroup.Code && r.ClassNum == equClass.Code select new { r.LineNum };  // check name duplicate

                                        dbHelper.TblAdapter_AllEquipment.Line1(equClass.Code, equGroup.Code, lineNumer, res2.Count());

                                        addObjectOnTreeView(lineNumer, newName + ";" + newCode+";" + Convert.ToString(StartCoordinate), "Line");

                                        Close();
                                        Dispose();
                                   }
                                   else
                                       MessageBox.Show("Некорректно введено значения начала координат линии.");
                               }
                               else
                                   MessageBox.Show("Введено слишком большое число.");
                           }
                           else
                               MessageBox.Show("Номер линии должен быть больше нуля.");
                       }
                       else
                           MessageBox.Show("Введенный код линии уже присутствует в Базе Данных", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
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
               {
                    //var res = from r in dbHelper.dataTable_Lines.AsEnumerable() where r.LineNum == lineNumer select new { r.LineName, r.StartCoordinate };

                    //if (res.Count() > 0)
                    //{
                    //    TxtBx_Name.Text = res.First().LineName;

                    //    txtBx_beginCoordinate.Text =Convert.ToString(res.First().StartCoordinate);
                    //}
                }
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
