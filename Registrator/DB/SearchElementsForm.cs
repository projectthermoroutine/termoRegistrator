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
        private Registrator.DB.EFClasses.Class[] classes = new DB.EFClasses.Class[0];
        private Registrator.DB.EFClasses.Group[] groups;
        private Registrator.DB.EFClasses.Line[]  lines;
        private Registrator.DB.EFClasses.Track[] tracks;

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
            groupsComboBox.Enabled = false;

            classesComboBox.Items.Clear();
            groupsComboBox.Items.Clear();
            linesComboBox.Items.Clear();

            classes = _db_controller.dbContext.Classes.Distinct().OrderBy(n => n.Class1)?.ToArray();

            if (classes != null)
            {
                if (classes.Count() > 0)
                    classesComboBox.Items.AddRange(classes.Select(y => y.Class1).OrderBy(n => n).ToArray());
            }
        }

        private void closeButton_Click(object sender, EventArgs e)
        {
            Close();
            Dispose();
        }


        public class ffff
        {
            public int gr;
            public string name;
        }


        private void searchButton_Click(object sender, EventArgs e)
        {
            dataGridView1.Rows.Clear();
            dataGridView1.Update();

            string searchStr = searchBox.Text;
            Registrator.DB.EFClasses.Class equClass = null;
            Registrator.DB.EFClasses.Group equGroup = null;
            Registrator.DB.EFClasses.Line equLine = null;
            Registrator.DB.EFClasses.Track equPath = null;

            IQueryable<DB.EFClasses.AllEquipment> queryEquips=null;

            if (searchStr.Length > 0)
            {
                if (classesComboBox.SelectedIndex != -1)
                {
                    equClass = classes[classesComboBox.SelectedIndex];
                }
                else
                {
                    //return;
                }

                if (groupsComboBox.SelectedIndex != -1)
                {
                    equGroup = groups[groupsComboBox.SelectedIndex];
                }

                IQueryable<int> queryEquipByGroup = null;

                if (equGroup != null)
                {
                    queryEquipByGroup = from m in _db_controller.dbContext.AllEquipments where m.Group == equGroup.Code select m.Code;
                }
                else
                {
                    queryEquipByGroup = from m in _db_controller.dbContext.AllEquipments where m.Name.IndexOf(searchStr) >= 0 select m.Code;

                    if (queryEquipByGroup == null)
                        return;

                }

                if (linesComboBox.SelectedIndex != -1)
                    equLine = lines[linesComboBox.SelectedIndex];

                if (equLine != null)
                {
                    var queryTracksByLine = from t in _db_controller.dbContext.Tracks where t.LineId == equLine.LineNum select t.ID;

                    if (pathsComboBox.SelectedIndex != -1)
                        equPath = tracks[pathsComboBox.SelectedIndex];

                    if (equPath != null)
                    {
                        if (picketCb.Enabled)
                        {
                            queryEquips = from equip in _db_controller.dbContext.AllEquipments where queryEquipByGroup.Contains(equip.Code) && equip.Path == equPath.ID /*&& equip.Picket == (int)picketUpDown.Value*/ && equip.Name.IndexOf(searchStr) >= 0 select (DB.EFClasses.AllEquipment)equip;

                            foreach (var item in queryEquips)
                            {

                                int class_id = (from t in _db_controller.dbContext.Groups where t.Code == item.Group select t.Class).First();
                                string class_name = (from t in _db_controller.dbContext.Classes where t.Code == class_id select t.Class1).First();

                                WriteToDtaGrid(dataGridView1, item, class_id, class_name, equGroup.Code, equGroup.Group1, equLine.LineNum, equLine.LineName);
                            }

                            return;
                        }
                        else
                        {
                            queryEquips = from equip in _db_controller.dbContext.AllEquipments where queryEquipByGroup.Contains(equip.Code) && queryTracksByLine.Contains(equPath.ID) && equip.Name.IndexOf(searchStr) >= 0 select (DB.EFClasses.AllEquipment)equip;

                            foreach (var item in queryEquips)
                            {
                                //
                                // get lines by track of groups of selected class
                                //
                                int lineId = (from t in _db_controller.dbContext.Tracks where t.ID == item.Path select new { t.LineId }).Distinct().First().LineId;
                                string lineName = (from t in _db_controller.dbContext.Lines where t.LineNum == lineId select new { t.LineName }).Distinct().First().LineName;


                                ffff class_id = (from t in _db_controller.dbContext.Groups where t.Code == item.Group select new ffff { gr = t.Class, name = t.Group1 }).First();
                                string class_name = (from t in _db_controller.dbContext.Classes where t.Code == class_id.gr select t.Class1).First();

                                WriteToDtaGrid(dataGridView1, item, class_id.gr, class_name, item.Group, class_id.name, lineId, lineName);
                            }

                            return;
                        }
                    }
                    else
                    {
                        // Track not select
                        queryEquips = from equip in _db_controller.dbContext.AllEquipments where queryEquipByGroup.Contains(equip.Code) /*&& queryTracksByLine.Contains(equip.Path) */&& equip.Name.IndexOf(searchStr) >= 0 select (DB.EFClasses.AllEquipment)equip;

                        foreach (var item in queryEquips)
                        {
                            //
                            // get lines by track of groups of selected class
                            //
                            //int lineId = (from t in _db_controller.dbContext.Tracks where t.ID == item.Path && t.LineId==equLine.LineNum select new { t.LineId }).Distinct().First().LineId;
                            //string lineName = (from t in _db_controller.dbContext.Lines where t.LineNum == lineId select new { t.LineName }).Distinct().First().LineName;


                            ffff class_id = (from t in _db_controller.dbContext.Groups where t.Code == item.Group select new ffff { gr = t.Class, name = t.Group1 }).First();
                            string class_name = (from t in _db_controller.dbContext.Classes where t.Code == class_id.gr select t.Class1).First();

                            WriteToDtaGrid(dataGridView1, item, class_id.gr, class_name, item.Group, class_id.name, equLine.LineNum, equLine.LineName);
                        }

                        return;
                    }
                }
                else
                {
                    //
                    // Line not selected
                    //
                    queryEquips = from equip in _db_controller.dbContext.AllEquipments where queryEquipByGroup.Contains(equip.Code) && equip.Name.IndexOf(searchStr) >= 0 select (DB.EFClasses.AllEquipment)equip;

                    foreach (var item in queryEquips)
                    {
                        //
                        // get lines by track of groups of selected class
                        //
                        
                        string lineName = (from t in _db_controller.dbContext.Lines where t.LineNum == item.Line select new { t.LineName }).Distinct().First().LineName;


                        ffff class_id = (from t in _db_controller.dbContext.Groups where t.Code == item.Group select new ffff { gr= t.Class, name = t.Group1 }).First();
                        string class_name = (from t in _db_controller.dbContext.Classes where t.Code == class_id.gr select t.Class1).First();

                        WriteToDtaGrid(dataGridView1, item, class_id.gr, class_name, item.Group, class_id.name, item.Line, lineName);
                    }

                    return;
                }

            }
        }





        private void WriteToDtaGrid(DataGridView dg , DB.EFClasses.AllEquipment item,int ClassId, string ClassName, int GroupId, string GroupName, int LineId, string LineName)
        {
           
            dg.Rows.Add(new object[] {  item.Name,
                                        Convert.ToString(item.Code),
                                        ClassName ,
                                        GroupName ,
                                        GroupName ,
                                        Convert.ToString(LineId),
                                        LineName ,
                                        Convert.ToString(item.Path) ,
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
            groupsComboBox.SelectedIndex = -1;
            linesComboBox.SelectedIndex = -1;
            pathsComboBox.SelectedIndex = -1;

            //picketUpDown.Enabled = false;
            //picketUpDown.Value = 0;
            //picketCb.Checked = false;
            //picketCb.Enabled = false;

            if (classesComboBox.SelectedIndex > 0)
            {
                groupsComboBox.Items.Clear();
                groupsComboBox.Enabled = true;
                //groupsComboBox.Items.Add("");
                DB.EFClasses.Class _class = classes[classesComboBox.SelectedIndex];

                groups = _db_controller.dbContext.Groups.Distinct().Where(a => a.Class == _class.Code).OrderBy(n => n.Group1)?.ToArray();

                if (groups != null)
                {
                    if (groups.Count() > 0)
                    {
                        string[] groups_arr = groups.Where(a => a.Class == _class.Code).Select(x => x.Group1).OrderBy(n => n).ToArray();
                        groupsComboBox.Items.AddRange(groups_arr);

                        if (lines == null)
                            lines = _db_controller.dbContext.Lines.Distinct().OrderBy(n => n.LineName)?.ToArray();

                        if (lines != null)
                        {
                            linesComboBox.Items.Clear();
                            linesComboBox.Items.AddRange(lines.Select(a => a.LineName).OrderBy(x => x).ToArray());
                        }
                    }
                }
            }
            else
            {
                groupsComboBox.Enabled = false;
                linesComboBox.Enabled = false;
                pathsComboBox.Enabled = false;
            }
        }

        private void groupsComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            linesComboBox.SelectedIndex=-1;
            
            pathsComboBox.SelectedIndex=-1;
            //picketUpDown.Enabled = false;
            //picketUpDown.Value = 0;
            //picketCb.Checked = false;
            //picketCb.Enabled = false;

            if (groupsComboBox.SelectedIndex != -1)
            {
                linesComboBox.Enabled = true;
            }
            else
            {
                linesComboBox.Enabled = false;
                pathsComboBox.Enabled = false;
            }
        }

        private void linesComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {

            picketUpDown.Enabled = false;
            picketUpDown.Value = 0;
            picketCb.Checked = false;
            picketCb.Enabled = false;

            if (linesComboBox.SelectedIndex > 0)
            {
                pathsComboBox.Items.Clear();

                int line_num = lines[linesComboBox.SelectedIndex].LineNum;

                tracks = _db_controller.dbContext.Tracks.Where(z => z.LineId == line_num)?.Distinct().OrderBy(n => n.Track1)?.ToArray();

                if (tracks != null)
                {
                    pathsComboBox.Items.AddRange(tracks.Select(a => a.Track1).OrderBy(x => x).ToArray());
                }

                pathsComboBox.Enabled = true;
            }
            else
            {
                pathsComboBox.Enabled = false;
            }
           
        }

        private void pathsComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            
        
        }

        

        private void searchBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            // if (e.KeyChar.Equals('\r'))
            //Filter();
        }

    }
}
