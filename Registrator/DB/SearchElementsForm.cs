using System;
using System.Windows.Forms;
using System.Collections;
using System.Windows.Documents;
using System.Data.OleDb;
using System.Data;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
//using System.Data.Linq.SqlClient;

namespace Registrator
{
    public partial class SearchElementsForm : Form
    {
        TreeNodeCollection m_classes = null;

        ArrayList objs = new ArrayList();
        private DB.metro_db_controller _db_controller;

        public SearchElementsForm()
        {
            InitializeComponent();
        }

        public SearchElementsForm(TreeNodeCollection classes, DB.metro_db_controller db_controller)
            : this()
        {
            m_classes = classes;
            _db_controller = null;
            if (db_controller != null)
                _db_controller = new DB.metro_db_controller(db_controller);
            InitFilter();
        }

        public void InitFilter()
        {
            InitClasses();
        }

        public void InitClasses()
        {
            classesComboBox.Items.Clear();
            groupsComboBox.Items.Clear();
            linesComboBox.Items.Clear();
            peregonComboBox.Items.Clear();

            classesComboBox.Items.Add("");

            if (m_classes != null)
            {
                for (int i = 0; i < m_classes.Count; i++)
                {
                    EquClass curClass = (m_classes[i] as EquTreeNode).ObjectDB as EquClass;

                    if (curClass != null)
                        classesComboBox.Items.Add(curClass.Name);
                }
            }
            classesComboBox.SelectedIndex = 0;
        }

        private void closeButton_Click(object sender, EventArgs e)
        {
            Close();
            Dispose();
        }

        private void searchButton_Click(object sender, EventArgs e)
        {
            dataGridView1.Rows.Clear();
            
            string searchStr = searchBox.Text;
            ulong shift = 0;
            EquClass equClass = null;
            EquGroup equGroup = null;
            EquDbObject equLine = null;
            EquDbObject equPath = null;

            IQueryable<DB.EFClasses.Equipment> queryEquips=null;

            if (searchStr.Length > 0)
            {
                if (groupsComboBox.SelectedIndex!=-1)
                    equClass = (m_classes[classesComboBox.SelectedIndex-1] as EquTreeNode).ObjectDB as EquClass;
                if (equClass != null)
                {
                    IQueryable<int> queryGroups = from g in _db_controller.dbContext.Groups where g.Class == equClass.Code select g.Code;

                    if (groupsComboBox.SelectedIndex != -1)
                        equGroup = (m_classes[classesComboBox.SelectedIndex - 1].Parent.Nodes[groupsComboBox.SelectedIndex - 1] as EquTreeNode).ObjectDB as EquGroup;

                    IQueryable<int> queryEquipByGroup = null;

                    if (equGroup != null)
                    {
                        //queryEquipByGroup = from m in _db_controller.dbContext.Mains where m.GroupId == equGroup.Code select m.EquipmentId;
                        //queryEquipByGroup = (from m in _db_controller.dbContext.Equipments where m.Groups.Select(g => g.Code).Contains(equGroup.Code) select m.Code).Distinct();
                        queryEquipByGroup = from m in _db_controller.dbContext.Equipments where m.Group == equGroup.Code select m.Code;

                        if (linesComboBox.SelectedIndex != -1)
                            equLine = (m_classes[classesComboBox.SelectedIndex - 1].Parent.Nodes[groupsComboBox.SelectedIndex - 1].Parent.Nodes[linesComboBox.SelectedIndex - 1] as EquTreeNode).ObjectDB as EquLine;
                        if (equLine != null)
                        {
                            var queryTracksByLine = from t in _db_controller.dbContext.Tracks where t.LineId == equLine.Code select t.ID;

                            if (pathsComboBox.SelectedIndex != -1)
                                equPath = (m_classes[classesComboBox.SelectedIndex - 1].Parent.Nodes[groupsComboBox.SelectedIndex - 1].Parent.Nodes[linesComboBox.SelectedIndex - 1].Parent.Nodes[pathsComboBox.SelectedIndex - 1] as EquTreeNode).ObjectDB as EquPath;

                            if (equPath != null)
                            {
                                if (picketCb.Enabled)
                                {
                                    queryEquips = from equip in _db_controller.dbContext.Equipments where queryEquipByGroup.Contains(equip.Code) && equip.Path == equip.Path && equip.Picket == (int)picketUpDown.Value && equip.Name.IndexOf(searchStr) >= 0 select (DB.EFClasses.Equipment)equip;

                                    foreach (var item in queryEquips)
                                        WriteToDtaGrid(dataGridView1, item, equClass.Code, equClass.Name, equGroup.Code, equGroup.Name, equLine.Code, equLine.Name);

                                    return;
                                }
                                else
                                {
                                    queryEquips = from equip in _db_controller.dbContext.Equipments where queryEquipByGroup.Contains(equip.Code) && queryTracksByLine.Contains(equip.Path) && equip.Name.IndexOf(searchStr) >= 0 select (DB.EFClasses.Equipment)equip;

                                    foreach (var item in queryEquips)
                                        WriteToDtaGrid(dataGridView1, item, equClass.Code, equClass.Name, equGroup.Code, equGroup.Name, equLine.Code, equLine.Name);

                                    return;
                                }
                            }
                            else
                            {
                                // Track not select
                                queryEquips = from equip in _db_controller.dbContext.Equipments where queryEquipByGroup.Contains(equip.Code) && queryTracksByLine.Contains(equip.Path) && equip.Name.IndexOf(searchStr) >= 0 select (DB.EFClasses.Equipment)equip;

                                foreach (var item in queryEquips)
                                    WriteToDtaGrid(dataGridView1, item, equClass.Code, equClass.Name, equGroup.Code, equGroup.Name, equLine.Code, equLine.Name);

                                return;
                            }
                        }
                        else
                        {
                            //
                            // Line not selected
                            //
                            queryEquips = from equip in _db_controller.dbContext.Equipments where queryEquipByGroup.Contains(equip.Code) && equip.Name.IndexOf(searchStr) >= 0 select (DB.EFClasses.Equipment)equip;

                            foreach (var item in queryEquips)
                            {
                                //
                                // get lines by track of groups of selected class
                                //
                                int lineId = (from t in _db_controller.dbContext.Tracks where t.ID == item.Path select new { t.LineId }).Distinct().First().LineId;
                                string lineName = (from t in _db_controller.dbContext.Lines where t.LineNum == lineId select new { t.LineName }).Distinct().First().LineName;

                                WriteToDtaGrid(dataGridView1, item, equClass.Code, equClass.Name, equGroup.Code, equGroup.Name, lineId, lineName);
                            }

                            return;
                        }
                    }
                    else
                    {
                        //var equipByGroup = (from m in _db_controller.dbContext.Mains where queryGroups.Contains(m.GroupId) select new { m.EquipmentId, m.GroupId }).Distinct();

                        var equipByGroup = (from m in _db_controller.dbContext.Equipments where queryGroups.Contains(m.Group) select m).Distinct();

                        //queryEquips = from equip in _db_controller.dbContext.Equipments where (equipByGroup.Any(x => x.EquipmentId == equip.Code)) select (DB.EFClasses.Equipment)equip;

                        //queryEquips = from equip in _db_controller.dbContext.Equipments where (equipByGroup.Any(x => x.Code == equip.Code)) select equip;

                        foreach (var item in equipByGroup)
                        {
                            //var groups = from g in equipByGroup where g.EquipmentId == item.Code select g.GroupId;
                            //var groups = from g in equipByGroup where g.Code == item.Code select g;

                            //foreach (var curGroupId in groups)
                            //{
                                var groupName = (from gr in _db_controller.dbContext.Groups where gr.Code == item.Group select gr.Group1).Distinct();
                                var line = (from l in _db_controller.dbContext.Tracks where l.ID == item.Path select l.LineId).Distinct().First();

                                var lineName = (from l in _db_controller.dbContext.Lines where l.LineNum == line select l.LineName).Distinct().First();

                                WriteToDtaGrid(dataGridView1, item, equClass.Code, equClass.Name, item.Group, groupName.First(), line, lineName);
                            //}
                        }

                        return;
                    }
                }
                else
                {
                    // class not select
                    var equips = from r in _db_controller.dbContext.Equipments where r.Name.IndexOf(searchStr) >= 0 select r;

                    foreach (var item in equips)
                    {
                        //var groups = (from g in _db_controller.dbContext.Mains where g.EquipmentId == item.Code select  g.GroupId).Distinct();
                        var groups = (from g in _db_controller.dbContext.Equipments where g.Code == item.Code select g.Group).Distinct();

                        foreach (var curGroup in groups)
                        {
                            var curClass = (from g in _db_controller.dbContext.Groups where g.Code == curGroup select g.Class).Distinct().First();
                            var groupName = (from g in _db_controller.dbContext.Groups where g.Code == curGroup select g.Group1).Distinct().First();

                            //
                            //Get line id and name
                            //
                            var lineId = (from t in _db_controller.dbContext.Tracks where t.ID == item.Path select t.LineId).Distinct().First();
                            var lineName = (from l in _db_controller.dbContext.Lines where l.LineNum == lineId select l.LineName).Distinct().First();

                            string className = _db_controller.dbContext.Classes.Where(cl => cl.Code == curClass).Distinct().Select(cl=>cl.Class1).FirstOrDefault(); ///TODO When Foreign key will be exist - refactor

                            WriteToDtaGrid(dataGridView1, item, curClass, className, curGroup, groupName, lineId, lineName);
                            
                        }
                    }
                    return;
                }
            }
        }

        private void WriteToDtaGrid(DataGridView dg , DB.EFClasses.Equipment item,int ClassId, string ClassName, int GroupId, string GroupName, int LineId, string LineName)
        {
            dg.Rows.Add(new object[] {  item.Name,
                                        Convert.ToString(item.Code),
                                        ClassName ,
                                        GroupName ,
                                        GroupName ,
                                        Convert.ToString(LineId),
                                        LineName ,
                                        Convert.ToString(item.Path) ,
                                        "" ,
                                        Convert.ToString(item.Picket),
                                        Convert.ToString(item.shiftFromPicket) ,
                                        "" });
        }

        private void dataGridView1_DoubleClick(object sender, EventArgs e)
        {
            if (dataGridView1.SelectedRows.Count == 0)
                return;

            int i = dataGridView1.SelectedRows[0].Index;
            
            EquClass curClass = new EquClass(Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[1].Value), (String)dataGridView1.SelectedRows[0].Cells[2].Value);
            EquGroup curGroup = new EquGroup(Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[4].Value), (String)dataGridView1.SelectedRows[0].Cells[3].Value,curClass);
            EquLine curLine = new EquLine(Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[5].Value), String.Concat(new object[] { Convert.ToString(Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[5].Value)) }), curGroup);
            EquPath curPath = new EquPath(Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[7].Value), String.Concat(new object[] { Convert.ToString(dataGridView1.SelectedRows[0].Cells[7].Value) }), curLine);

            EquObject elObj = new EquObject( Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[1].Value),
                                             (String)dataGridView1.SelectedRows[0].Cells[0].Value,
                                             new EquPicket(Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[9].Value),"", curPath),
                                             Convert.ToSingle(dataGridView1.SelectedRows[0].Cells[10].Value)  );

            elObj.Code = Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[1].Value);

            EquElementForm eqf = new EquElementForm(elObj,_db_controller);
            eqf.ShowDialog(this);
        }

        private void picketCb_CheckedChanged(object sender, EventArgs e)
        {
            picketUpDown.Enabled = picketCb.Checked;
            //offsCb.Enabled = picketCb.Checked;
        }

        private void offsCb_CheckedChanged(object sender, EventArgs e)
        {
            offsUpDown.Enabled = offsCb.Checked;
        }

        private void classesComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            groupsComboBox.Items.Clear();
            linesComboBox.Items.Clear();
            peregonComboBox.Items.Clear();
            pathsComboBox.Items.Clear();
            picketUpDown.Enabled = false;
            picketUpDown.Value = 0;
            picketCb.Checked = false;
            picketCb.Enabled = false;

            if (classesComboBox.SelectedIndex > 0)
            {
                groupsComboBox.Items.Add("");
                EquTreeNode ClassTreeNode = m_classes[classesComboBox.SelectedIndex - 1] as EquTreeNode;

                if (ClassTreeNode != null)
                {
                    for (int i = 0; i < ClassTreeNode.Nodes.Count; i++)
                    {
                        EquGroup curGroup = (ClassTreeNode.Nodes[i] as EquTreeNode).ObjectDB as EquGroup;

                        if (curGroup != null)
                            groupsComboBox.Items.Add(curGroup.Name);
                    }
                }
            }
        }

        private void groupsComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            linesComboBox.Items.Clear();
            peregonComboBox.Items.Clear();
            pathsComboBox.Items.Clear();
            picketUpDown.Enabled = false;
            picketUpDown.Value = 0;
            picketCb.Checked = false;
            picketCb.Enabled = false;

            if (groupsComboBox.SelectedIndex != -1)
            {
                //EquClass equClass = m_classes[classesComboBox.SelectedIndex-1] as EquClass;

                EquTreeNode ClassTreeNode = m_classes[classesComboBox.SelectedIndex - 1] as EquTreeNode;

                EquTreeNode GroupTreeNode = null;

                if (ClassTreeNode != null && groupsComboBox.SelectedIndex > 0)
                    GroupTreeNode = ClassTreeNode.Nodes[groupsComboBox.SelectedIndex - 1] as EquTreeNode;
                else
                    return;
                if (GroupTreeNode != null)
                {
                    linesComboBox.Items.Add("");
                    for (int i = 0; i < GroupTreeNode.Nodes.Count; i++)
                    {
                        EquDbObject curLine = (GroupTreeNode.Nodes[i] as EquTreeNode).ObjectDB as EquDbObject;
                        if (curLine != null)
                            linesComboBox.Items.Add(curLine.Code);
                    }
                }
            }
        }

        private void linesComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            pathsComboBox.Items.Clear();
            peregonComboBox.Items.Clear();
            picketUpDown.Enabled = false;
            picketUpDown.Value = 0;
            picketCb.Checked = false;
            picketCb.Enabled = false;

            if (linesComboBox.SelectedIndex > 0)
            {
                pathsComboBox.Items.Add("");
                EquTreeNode ClassTreeNode = m_classes[classesComboBox.SelectedIndex - 1] as EquTreeNode;
                //EquClass equClass = m_classes[classesComboBox.SelectedIndex-1] as EquClass;
                EquTreeNode GroupTreeNode = null;
                EquTreeNode equLine = null;

                if (ClassTreeNode != null && groupsComboBox.SelectedIndex > 0)
                    GroupTreeNode = ClassTreeNode.Nodes[groupsComboBox.SelectedIndex - 1] as EquTreeNode;
                else
                    return;

                if (GroupTreeNode != null && linesComboBox.SelectedIndex > 0)
                    equLine = GroupTreeNode.Nodes[linesComboBox.SelectedIndex - 1] as EquTreeNode;
                else
                    return;

                if (equLine != null)
                {
                    for (int i = 0; i < equLine.Nodes.Count; i++)
                    {
                        EquDbObject curPath = (equLine.Nodes[i] as EquTreeNode).ObjectDB as EquDbObject;
                        if (curPath != null)
                            pathsComboBox.Items.Add(curPath.Code);
                    }
                }
            }
        }

        private void pathsComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            peregonComboBox.Items.Clear();
            picketUpDown.Enabled = false;
            picketUpDown.Value = 0;
            picketCb.Checked = false;
            picketCb.Enabled = false;

            if (pathsComboBox.SelectedIndex > 0)
            {
                EquTreeNode ClassTreeNode = m_classes[classesComboBox.SelectedIndex - 1] as EquTreeNode;
                EquTreeNode GroupTreeNode = null;
                EquTreeNode equLine = null;
                EquTreeNode equPath = null;

                if (ClassTreeNode != null && groupsComboBox.SelectedIndex > 0)
                    GroupTreeNode = ClassTreeNode.Nodes[groupsComboBox.SelectedIndex - 1] as EquTreeNode;
                else
                    return;
                if (GroupTreeNode != null && linesComboBox.SelectedIndex > 0)
                    equLine = GroupTreeNode.Nodes[linesComboBox.SelectedIndex - 1] as EquTreeNode;
                else
                    return;
                if (equLine != null && pathsComboBox.SelectedIndex > 0)
                    equPath = equLine.Nodes[pathsComboBox.SelectedIndex - 1] as EquTreeNode;
                else
                    return;
                //if (equPath != null)
                //{
                //    peregonComboBox.Items.Add("");
                //    for (int i = 0; i < equPath.Nodes.Count; i++)
                //    {
                //        EquLayout curLayout = equPath.Nodes[i] as EquLayout;
                //        if (curLayout != null)
                //            peregonComboBox.Items.Add(curLayout.Name);
                //    }
                //}
            }
        }

        private void peregonComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            picketCb.Enabled = (peregonComboBox.Enabled && peregonComboBox.SelectedIndex > 0);
            if(!picketCb.Enabled)
                picketCb.Checked = false;
        }

        private void searchBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            // if (e.KeyChar.Equals('\r'))
            //Filter();
        }

    }
}
