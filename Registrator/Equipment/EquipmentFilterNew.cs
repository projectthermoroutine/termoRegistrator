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
    public partial class EquipmentFilterNew : Form
    {
        DB.DataBaseHelper dbHelper;
        List<int> lstClasses = new List<int>();
        List<int> lstGroup = new List<int>();

        private List<int> lstSelectedGroupsNumbers = new List<int>();

        private int selectedClass = 0;
        private int selectedGroup = 0;
        private int filteredClassNumber = 0;

        public EquipmentFilterNew(DB.DataBaseHelper dbHelperArg)
        {
            InitializeComponent();

            dbHelper = dbHelperArg;

            var resClasses = (from r in dbHelper.dataTable_Class.AsEnumerable() select new { r.Class, r.Code });

            foreach (var item in resClasses)
            {
                lstBx_Classes.Items.Add(Convert.ToString(item.Class));
                lstClasses.Add(Convert.ToInt32(item.Code));
            }

            lstBx_Group.SelectionMode = SelectionMode.MultiExtended;
        }

        private void apply_Click(object sender, EventArgs e)
        {


        }

        private void lstBx_Classes_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(lstBx_Classes.SelectedIndex!=-1)
            {
                selectedGroup = 0;
                selectedClass = lstBx_Classes.SelectedIndex;

                lstBx_Group.Items.Clear();
                lstGroup.Clear();


                filteredClassNumber = lstClasses[selectedClass];

                var resGroup = (from r in dbHelper.dataTable_GroupTable.AsEnumerable() where r.Group != "notExist" && r.Class == filteredClassNumber select new { r.Group, r.Code });

                foreach(var item in resGroup)
                {
                    lstBx_Group.Items.Add(Convert.ToString(item.Group));
                    lstGroup.Add(Convert.ToInt32(item.Code));
                }
            }
        }

        private void lstBx_Group_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (lstBx_Classes.SelectedIndices.Count > 0)
            {
                selectedGroup = 1;
            }
            else
                selectedGroup = 0;
            
        }

        private void btn_Apply_Click(object sender, EventArgs e)
        {
            if (filteredClassNumber == 0)
            {
                MessageBox.Show("Необходимо выбрать класс");
                return;
            }

            string strGroups = null;
            lstSelectedGroupsNumbers.Clear();
            int intItem = 0;

            if(selectedGroup == 0)
            {
                var resGroup = (from r in dbHelper.dataTable_GroupTable.AsEnumerable() where r.Group != "notExist" && r.Class == filteredClassNumber select new { r.Group, r.Code });

                foreach (var item in resGroup)
                {

                    lstSelectedGroupsNumbers.Add(Convert.ToInt32(item.Code));
                    strGroups += Convert.ToString(item.Group) + "; ";
                }
            }


            string strClassName = Convert.ToString(lstBx_Classes.Items[lstBx_Classes.SelectedIndex]);
            
            foreach(var item in lstBx_Group.SelectedIndices)
            {
                intItem = Convert.ToInt32(item);
                lstSelectedGroupsNumbers.Add(lstGroup[intItem]);
                strGroups += lstBx_Group.Items[intItem] + "; ";
            }

            dataGridView1.Rows.Add(new object[] {true, strClassName, strGroups });
        }

        private void Cancel_Click(object sender, EventArgs e)
        {
            Dispose();
            Close();
        }
    }
}
