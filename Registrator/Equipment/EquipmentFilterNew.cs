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
        string strGroups = null;

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

            var resFilters = (from r in dbHelper.dataTable_EquipmentFilter.AsEnumerable() select r);

            foreach(var item in resFilters)
            {
                dataGridView1.Rows.Add(new object[] { Convert.ToString(item.filter_id),
                                                      Convert.ToString(item.class_id),
                                                      Convert.ToBoolean(item.apply),
                                                      Convert.ToString(item.groups_Numbers),
                                                      Convert.ToString(item.class_Name),
                                                      Convert.ToString(item.groups_Names)});
            }
        }

      
        private void btn_Apply_Click(object sender, EventArgs e) // add filter
        {
            if (filteredClassNumber == 0)
            {
                MessageBox.Show("Необходимо выбрать класс");
                return;
            }

            strGroups = null;
            string strGroupsCodes = null;

            lstSelectedGroupsNumbers.Clear();
            int intItem = 0;

            if(selectedGroup == 0) // if group not selected select all groups
            {
                var resGroup = (from r in dbHelper.dataTable_GroupTable.AsEnumerable() where r.Group != "notExist" && r.Class == filteredClassNumber select new { r.Group, r.Code });

                if(resGroup.Count() == 0)
                {
                    MessageBox.Show("Выбранный класс не содержит групп");
                    return;
                }

                foreach (var item in resGroup)
                {
                    strGroupsCodes  += Convert.ToString(item.Code);
                    strGroups       += Convert.ToString(item.Group) + ";";
                }
            }

            string strClassName = Convert.ToString(lstBx_Classes.Items[lstBx_Classes.SelectedIndex]);
            
            foreach(var item in lstBx_Group.SelectedIndices)
            {
                intItem = Convert.ToInt32(item);
                strGroupsCodes += lstGroup[intItem]+";";
                strGroups += lstBx_Group.Items[intItem] + ";";
            }

            int filterInd = calcFilterNumber();
            dataGridView1.Rows.Add(new object[] { Convert.ToString( filterInd), Convert.ToString(filteredClassNumber), true, strGroupsCodes, strClassName, strGroups });
        }
        
        private int calcFilterNumber()
        {
            var resFilterNumber = (from r in dbHelper.dataTable_EquipmentFilter.AsEnumerable() orderby r.filter_id select new { r.filter_id });

            int ind = 1;


            foreach(var item in resFilterNumber)
            {
                if(ind != Convert.ToInt32(item.filter_id))
                    break;
                ind++;
            }

            return ind;
        }

        private void Cancel_Click(object sender, EventArgs e)
        {
            Dispose();
            Close();
        }
        private void button2_Click(object sender, EventArgs e) // Применить
        {
            if (dataGridView1.Rows.Count == 0)
            {
                dbHelper.TblAdapter_EquipmentFilter.DeleteAll();
                
                dbHelper.dataTable_EquipmentFilter.Clear();
                dbHelper.TblAdapter_EquipmentFilter.Fill(dbHelper.dataTable_EquipmentFilter);

                dbHelper.fill_Equip_Filter_Object();
                return;
            }

            dbHelper.TblAdapter_EquipmentFilter.DeleteAll();
            
            foreach (DataGridViewRow  item in dataGridView1.Rows)
            {

                int     filterID    = Convert.ToInt32(((DataGridViewTextBoxCell)item.Cells[0]).Value);
                int     classCode   = Convert.ToInt32(((DataGridViewTextBoxCell)item.Cells[1]).Value);
                int     status      = Convert.ToInt32(((DataGridViewCheckBoxCell)item.Cells[2]).Value);
                string  className   = Convert.ToString(((DataGridViewTextBoxCell)item.Cells[4]).Value);
                string  groupsCodes = Convert.ToString(((DataGridViewTextBoxCell)item.Cells[3]).Value);
                string  groupsNames = Convert.ToString(((DataGridViewTextBoxCell)item.Cells[5]).Value);

                dbHelper.TblAdapter_EquipmentFilter.insertFilter(filterID,classCode,className,groupsCodes,groupsNames,status);
            }

            dbHelper.dataTable_EquipmentFilter.Clear();
            dbHelper.TblAdapter_EquipmentFilter.Fill(dbHelper.dataTable_EquipmentFilter);

            dbHelper.fill_Equip_Filter_Object();
        }

        private void button1_Click(object sender, EventArgs e)  // delete seleced filter
        {
            foreach(DataGridViewRow item in dataGridView1.SelectedRows)
                dataGridView1.Rows.Remove(item);
        }
        private void Close_Click(object sender, EventArgs e)
        {
            Dispose();
            Close();
        }

        //-------------- ListBoxes event handlers ----------------------------------------------------------------------------------------
        private void lstBx_Classes_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (lstBx_Classes.SelectedIndex != -1)
            {
                selectedGroup = 0;
                selectedClass = lstBx_Classes.SelectedIndex;

                lstBx_Group.Items.Clear();
                lstGroup.Clear();


                filteredClassNumber = lstClasses[selectedClass];

                var resGroup = (from r in dbHelper.dataTable_GroupTable.AsEnumerable() where r.Group != "notExist" && r.Class == filteredClassNumber select new { r.Group, r.Code });

                foreach (var item in resGroup)
                {
                    lstBx_Group.Items.Add(Convert.ToString(item.Group));
                    lstGroup.Add(Convert.ToInt32(item.Code));
                }
            }
        }

        private void lstBx_Group_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (lstBx_Classes.SelectedIndices.Count > 0)
                selectedGroup = 1;
            else
                selectedGroup = 0;
        }
    }
}
