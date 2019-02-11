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
        DB.metro_db_controller _db_controller;
        List<int> lstClasses = new List<int>();
        List<int> lstGroup = new List<int>();

        private List<int> lstSelectedGroupsNumbers = new List<int>();

        private int selectedClass = 0;
        private int selectedGroup = 0;
        private int filteredClassNumber = 0;
        string strGroups = null;

        List<DB.EFClasses.EquipmentFilter_Tbl> _exist_filters;

        int _last_filter_id = 0;

        List<int> _not_in_db_filter_ids = new List<int>();

        public EquipmentFilterNew(DB.metro_db_controller db_controller)
        {
            InitializeComponent();

            _db_controller = new DB.metro_db_controller(db_controller);

            foreach (var item in _db_controller.dbContext.Classes)
            {
                lstBx_Classes.Items.Add(Convert.ToString(item.Name));
                lstClasses.Add(Convert.ToInt32(item.Code));
            }

            lstBx_Group.SelectionMode = SelectionMode.MultiExtended;

            UpdateData();

        }

      
        void UpdateData()
        {
            _exist_filters = _db_controller.GetFilters().ToList();

            dataGridView1.Rows.Clear();
            foreach (var item in _exist_filters)
            {
                dataGridView1.Rows.Add(new object[] { Convert.ToString(item.filter_id),
                                                      Convert.ToString(item.class_id),
                                                      Convert.ToBoolean(item.apply),
                                                      Convert.ToString(item.groups_Numbers),
                                                      Convert.ToString(item.class_Name),
                                                      Convert.ToString(item.groups_Names)});
            }


            _last_filter_id = _exist_filters.Count() > 0 ? _exist_filters.Max(f => f.filter_id) + 1 : 1;

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
                var groups = _db_controller.dbContext.Groups.Where(gr => gr.ClassId == filteredClassNumber);

                if(groups.Count() == 0)
                {
                    MessageBox.Show("Выбранный класс не содержит групп");
                    return;
                }

                foreach (var item in groups)
                {
                    strGroupsCodes  += Convert.ToString(item.Code);
                    strGroups       += item.Name + ";";
                }
            }

            string strClassName = Convert.ToString(lstBx_Classes.Items[lstBx_Classes.SelectedIndex]);
            
            foreach(var item in lstBx_Group.SelectedIndices)
            {
                intItem = Convert.ToInt32(item);
                strGroupsCodes += lstGroup[intItem]+";";
                strGroups += lstBx_Group.Items[intItem] + ";";
            }

            _not_in_db_filter_ids.Add(_last_filter_id);
            dataGridView1.Rows.Add(new object[] { Convert.ToString(_last_filter_id++), Convert.ToString(filteredClassNumber), true, strGroupsCodes, strClassName, strGroups });
        }
        
        private void button2_Click(object sender, EventArgs e) // Применить
        {

            foreach (DataGridViewRow item in dataGridView1.Rows)
            {
                int filterID = Convert.ToInt32(((DataGridViewTextBoxCell)item.Cells[0]).Value);

                if (_not_in_db_filter_ids.Contains(filterID))
                {
                    int classCode = Convert.ToInt32(((DataGridViewTextBoxCell)item.Cells[1]).Value);
                    int status = Convert.ToInt32(((DataGridViewCheckBoxCell)item.Cells[2]).Value);
                    string className = Convert.ToString(((DataGridViewTextBoxCell)item.Cells[4]).Value);
                    string groupsCodes = Convert.ToString(((DataGridViewTextBoxCell)item.Cells[3]).Value);
                    string groupsNames = Convert.ToString(((DataGridViewTextBoxCell)item.Cells[5]).Value);

                    DB.EFClasses.EquipmentFilter_Tbl filter = new DB.EFClasses.EquipmentFilter_Tbl
                    {
                        filter_id = filterID,
                        class_id = classCode,
                        apply = status,
                        class_Name = className,
                        groups_Numbers = groupsCodes,
                        groups_Names = groupsNames
                    };

                    _db_controller.dbContext.EquipmentFilter_Tbl.Add(filter);
                    continue;
                }

                foreach(var filter in _exist_filters.FindAll(f => f.filter_id == filterID))
                {
                    _db_controller.dbContext.EquipmentFilter_Tbl.Attach(filter);
                    _db_controller.dbContext.Entry(filter).State = System.Data.Entity.EntityState.Modified;
                }

                _exist_filters.RemoveAll(f => f.filter_id == filterID);
            }

            _db_controller.dbContext.EquipmentFilter_Tbl.RemoveRange(_exist_filters);
            _db_controller.dbContext.SaveChanges();

            _db_controller.retrieve_groups();

            UpdateData();

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

                var resGroup = _db_controller.dbContext.Groups.Where(gr=>gr.ClassId == filteredClassNumber);

                foreach (var item in resGroup)
                {
                    lstBx_Group.Items.Add(Convert.ToString(item.Name));
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
