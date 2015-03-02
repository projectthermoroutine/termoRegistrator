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
    //
    public partial class AddClass : Form
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
        public AddClass(DB.DataBaseHelper dbHelperArg, MyDelegate sender, string setDataTableArg)
        {
            dbHelper = dbHelperArg;
            
            InitializeComponent();

            foreach (string line in (from r in dbHelper.dataTable_Class.AsEnumerable() select r["Class"]).ToList())
                listBox1.Items.Add(line);

            d = sender;

        }
        public void Class(ref EquClass PicketsArg)
        {
            equClass = PicketsArg;
        }
        private void button2_Click(object sender, EventArgs e)
        {
            StringBuilder sb = new StringBuilder();
            string newElementName = TxtBx_GroupName.Text.Trim();

            if (newElementName.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*', '`', '№', '\\', '|' }) == -1)
            {
                int ClassIndex;

                if (newElementName.Length != 0)
                {

                    var bres = dbHelper.TblAdapter_Class.selectMaxIndex();

                    if (bres == null)
                        bres = 0;

                    ClassIndex = Convert.ToInt32(bres);        // get Class max number 

                    if (newElementName.Length < 20)
                    {
                        var res = from r in dbHelper.dataTable_Class.AsEnumerable() where r.Class == newElementName select new { r.Class };  // check name duplicate

                        if (res.Count() == 0)
                        {
                            int result = dbHelper.TblAdapter_AllEquipment.newClass1(++ClassIndex, newElementName);

                            equClass.Code = ClassIndex;
                            equClass.Name = newElementName;

                            d(ClassIndex, newElementName, "Class");

                            Close();
                            Dispose();
                        }
                        else
                            MessageBox.Show("Класс с таким именем уже существует", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    else
                        MessageBox.Show("Введено слишком длинное название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }

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
    }
}
