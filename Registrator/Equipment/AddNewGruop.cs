using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Windows.Documents;

namespace Registrator
{
    public delegate void MyDelegate(int code, string data, string key);

    public partial class AddNewGruop : Form
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
        public AddNewGruop(DB.DataBaseHelper dbHelperArg, MyDelegate sender, string setDataTableArg)
        {
            dbHelper = dbHelperArg;

            InitializeComponent();

      
            button2.Enabled = false;

            switch (setDataTableArg)
            {
                case "Class":
                    foreach (string line in (from r in dbHelper.dataTable_Class.AsEnumerable() select r["Class"]).ToList())
                        listBox1.Items.Add(line);

                    this.Text = "Добавление нового класса";
                    label2.Text = "Список классов";
                    label1.Text = "Введите название нового класса";
                    break;
                case "Group":
                    for (int i = 0; i < dbHelper.dataTable_GroupTable.Rows.Count; i++)
                    {
                        if (Convert.ToString(dbHelper.dataTable_GroupTable.Rows[i].ItemArray[1]) == "notExist") 
                            continue;
                        listBox1.Items.Add(Convert.ToString(dbHelper.dataTable_GroupTable.Rows[i].ItemArray[1]));
                    }

                    this.Text = "Добавление новой группы";
                    label2.Text = "Список групп";
                    label1.Text = "Введите название новой группы";
                    break;

                case "Line":
                    foreach (int line in (from r in dbHelper.dataTable_LayoutTable.AsEnumerable() where r.Line!=0  select r["Line"]).Distinct().ToList())
                        listBox1.Items.Add("Линия " + Convert.ToString(line));

                    this.Text = "Добавление новой линии";
                    label2.Text = "Список линий";
                    label1.Text = "Введите номер существующей либо новой линии";
                    Point p = label1.Location;
                    p.X = p.X - 30;
                    label1.Location = p;
                    break;

                case "Track":
                    foreach (object line in (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.Track != 0 select r["Track"]).Distinct().ToList())
                        listBox1.Items.Add(Convert.ToString(line));

                    this.Text = "Добавление нового пути";
                    label2.Text = "Список путей";
                    label1.Text = "Введите номер нового пути";
                    break;
            }

            d = sender;

            
            //label2.Text = lbl_existNames;
            setDataTable = setDataTableArg;
        }
        public void Class(ref EquClass PicketsArg)
        {
            equClass = PicketsArg;
        }
        public void Group(ref EquGroup GroupArg, ref EquClass PicketsArg)
        {
            equClass = PicketsArg;
            equGroup = GroupArg;
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
       

        private void button1_Click(object sender, EventArgs e)
        {
            Close();
            Dispose();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            StringBuilder sb = new StringBuilder();
            string newElementName = TxtBx_GroupName.Text.Trim();

            if (newElementName.IndexOfAny(new char[] { '@', '.', ',', '!','\'',';','[',']','{','}','"','?','>','<','+','$','%','^','&','*' }) == -1)
            {
                int ObjectIndex;
                int ClassIndex;
                int GroupIndex;

                if (newElementName.Length != 0)
                {
                    switch (setDataTable)
                    {

                        case "Class":
                            var bres = dbHelper.TblAdapter_Class.selectMaxIndex();
                            
                            if (bres == null) 
                                bres = 0;

                            ClassIndex = Convert.ToInt32(bres);        // get Class max number 
                            var res = from r in dbHelper.dataTable_Class.AsEnumerable() where r.Class == newElementName select new { r.Class };  // check name duplicate
                            if (res.Count() == 0)
                            {
                                if (newElementName.Length < 20)
                                {
                                    //-------------------------------------------------------------
                                    //Encoding defaultEncoding = Encoding.Default;
                                    //Encoding Latin1 = Encoding.GetEncoding(1522);

                                    //byte[] koi8Bytes = Latin1.GetBytes(newElementName);
                                    //byte[] unicodeBytes = Encoding.Convert(defaultEncoding, Latin1, koi8Bytes);

                                    //string s = Encoding.GetEncoding(1522).GetString(unicodeBytes);
                                    //-------------------------------------------------------------

                                    int result = dbHelper.TblAdapter_AllEquipment.newClass1(++ClassIndex, newElementName);

                                    equClass.Code = ClassIndex;
                                    equClass.Name = newElementName;

                                    d(ClassIndex, newElementName, "Class");

                                    Close();
                                    Dispose();
                                }
                                else
                                    MessageBox.Show("Введено слишком длинное название");
                            }
                            else
                                MessageBox.Show("Класс с таким именем уже существует");
                            break;
                        case "Group":
                            GroupIndex = Convert.ToInt32(dbHelper.TblAdapter_Group.selectGroupMaxIndex());
                            ObjectIndex = Convert.ToInt32(dbHelper.TblAdapter_Objects.selectObjectMaxIndex());

                            var res1 = from r in dbHelper.dataTable_GroupTable.AsEnumerable() where r.Group == newElementName select new { r.Code };  // check name duplicate

                            if (res1.Count() == 0)
                            {
                                if (newElementName.Length < 20)
                                {
                                    int clsInd = equClass.Code;
                                    // int? g = 0;

                                    int result1 = dbHelper.TblAdapter_AllEquipment.newGroup1(clsInd, ++GroupIndex, newElementName, ++ObjectIndex, equClass.Nodes.Count);
                                    d(GroupIndex, newElementName, "Group");

                                    Close();
                                    Dispose();
                                }
                                else
                                    MessageBox.Show("Введено слишком длинное название");
                            }
                            else
                                MessageBox.Show("Группа с таким именем уже существует");
                            break;

                        case "Line":
                            int lineNum;
                            if (int.TryParse(newElementName, out lineNum))
                            {
                                if (lineNum >= 1)
                                {
                                    if (lineNum < 10000)
                                    {
                                        var res2 = from r in dbHelper.dataTable_LayoutTable.AsEnumerable() where r.Line == lineNum select new { r.Line };  // check name duplicate

                                        dbHelper.TblAdapter_AllEquipment.Line1(equClass.Code, equGroup.Code, lineNum, res2.Count());
                                        d(lineNum, newElementName, "Line");

                                        Close();
                                        Dispose();
                                    }
                                    else
                                        MessageBox.Show("Введено слишком большое число");
                                }
                                else
                                    MessageBox.Show("Номер линии должен быть больше нуля");
                            }
                            else
                                MessageBox.Show("Некорректно введен номер линии");
                            break;

                        case "Track":
                            int trackNum;
                            if (int.TryParse(newElementName, out trackNum))
                            {
                                if (trackNum >= 1 )
                                {
                                    if (trackNum < 10000)
                                    {
                                        var res22 = from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.ClassNum == equClass.Code && r.LineNum == equLine.Code && r.GroupNum == equGroup.Code select new { r.Track };  // check name duplicate
                                        var itemTrack = res22.First();

                                        if (trackNum != itemTrack.Track)
                                        {
                                            dbHelper.TblAdapter_AllEquipment.Path1(equClass.Code, equGroup.Code, equLine.Code, trackNum);

                                            d(trackNum, newElementName, "Track");

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

        private void TxtBx_GroupName_TextChanged(object sender, EventArgs e)
        {
            if (TxtBx_GroupName.Text.Length > 0)
                button2.Enabled = true;
        }
    }
}
