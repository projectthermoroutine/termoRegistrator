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
    

    public partial class AddNewGruop : Form
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
        Equipment.GroupColorSetUserControl gruopColorSetControl;
        public AddNewGruop(DB.DataBaseHelper dbHelperArg, AddObjectOnTreeView sender, string setDataTableArg)
        {
            dbHelper = dbHelperArg;

            InitializeComponent();

            gruopColorSetControl = new Equipment.GroupColorSetUserControl();
            elementHost1.Child = gruopColorSetControl;

            button2.Enabled = false;
                
            for (int i = 0; i < dbHelper.dataTable_GroupTable.Rows.Count; i++)
            {
                if (Convert.ToString(dbHelper.dataTable_GroupTable.Rows[i].ItemArray[1]) == "notExist") 
                    continue;
                listBox1.Items.Add(Convert.ToString(dbHelper.dataTable_GroupTable.Rows[i].ItemArray[1]));
            }

            addObjectOnTreeView = sender;

            setDataTable = setDataTableArg;
        }
  
        public void Group(ref EquGroup GroupArg, ref EquClass PicketsArg)
        {
            equClass = PicketsArg;
            equGroup = GroupArg;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //string color = gruopColorSetControl.colorPicker.SelectedColor.ToString();//#FF4B0082
           // gruopColorSetControl.colorPicker.SelectedColor.
            //System.Windows.Media.Color c = new System.Windows.Media.Color();
           
            //gruopColorSetControl.colorPicker.SelectedColor =  
            Close();
            Dispose();
        }

        private void button2_Click(object sender, EventArgs e)
        {

            string color = gruopColorSetControl.colorPicker.SelectedColor.ToString();

            StringBuilder sb = new StringBuilder();
            string newElementName = TxtBx_GroupName.Text.Trim();

            if (newElementName.IndexOfAny(new char[] { '@', '.', ',', '!','\'',';','[',']','{','}','"','?','>','<','+','$','%','^','&','*','`','№','\\','|'}) == -1)
            {
                int GroupIndex;

                if (newElementName.Length != 0)
                {
                    GroupIndex = Convert.ToInt32(dbHelper.TblAdapter_Group.selectGroupMaxIndex());
                            
                    if (newElementName.Length < 20)
                    {
                        var res1 = from r in dbHelper.dataTable_GroupTable.AsEnumerable() where r.Group == newElementName select new { r.Code };  // check name duplicate

                        if (res1.Count() == 0)
                        {
                            int result1 = dbHelper.TblAdapter_AllEquipment.newGroup1(equClass.Code, ++GroupIndex, newElementName, color, equClass.Nodes.Count);
                            addObjectOnTreeView(GroupIndex, newElementName, "Group");

                            Close();
                            Dispose();
                        }
                        else
                            MessageBox.Show("Группа с таким именем уже существует","", MessageBoxButtons.OK, MessageBoxIcon.Information);
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

        private void TxtBx_GroupName_TextChanged(object sender, EventArgs e)
        {
            if (TxtBx_GroupName.Text.Length > 0)
                button2.Enabled = true;
        }
    }
}
