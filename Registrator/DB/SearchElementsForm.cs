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
            EquLayout equLayout = null;

            if (searchStr.Length > 0)
            {
                if(groupsComboBox.SelectedIndex!=-1)
                    equClass = (m_classes[classesComboBox.SelectedIndex-1] as EquTreeNode).ObjectDB as EquClass;
                if (equClass != null)
                {
                    if(groupsComboBox.SelectedIndex!=-1)
                        equGroup = (m_classes[classesComboBox.SelectedIndex - 1].Parent.Nodes[groupsComboBox.SelectedIndex - 1] as EquTreeNode).ObjectDB as EquGroup;
                    if (equGroup != null)
                    {
                        if(linesComboBox.SelectedIndex != -1)
                            equLine = (m_classes[classesComboBox.SelectedIndex - 1].Parent.Nodes[groupsComboBox.SelectedIndex - 1].Parent.Nodes[linesComboBox.SelectedIndex - 1] as EquTreeNode).ObjectDB as EquLine;
                        if (equLine != null)
                        {
                            if(pathsComboBox.SelectedIndex != -1)
                                equPath = (m_classes[classesComboBox.SelectedIndex - 1].Parent.Nodes[groupsComboBox.SelectedIndex - 1].Parent.Nodes[linesComboBox.SelectedIndex - 1].Parent.Nodes[pathsComboBox.SelectedIndex - 1] as EquTreeNode).ObjectDB as EquPath;
                            if (equPath != null)
                            {
                                //if (peregonComboBox.SelectedIndex != -1)
                                //    equLayout = equPath.Nodes[peregonComboBox.SelectedIndex-1] as EquLayout;
                                if (equLayout != null)
                                {
                                    int npicket=0;
                                    if(picketCb.Enabled)
                                    {
                                        npicket = (int)picketUpDown.Value;

                                        var res = from r in _db_controller.all_equipment_table.AsEnumerable()   where  r.ClassNum == equClass.Code &&
                                                                                                                        r.GroupNum == equGroup.Code &&
                                                                                                                        r.LineNum == equLine.Code &&
                                                                                                                        r.Track == equPath.Code &&
                                                                                                                        r.Layout == equLayout.Code &&
                                                                                                                        r.Npicket == npicket &&
                                                                                                                        r.ObjName.IndexOf(searchStr) >= 0    select r;

                                        foreach (var item in res)
                                        {
                                            var resEquipShift = from r in _db_controller.objects_table.AsEnumerable() where r.Code == item.Code select new { r.shiftLine };
                                            shift = (ulong)resEquipShift.First().shiftLine;
                                            dataGridView1.Rows.Add(new object[] { Convert.ToString(item.ObjName)    /*0*/,
                                                          Convert.ToString(item.Code)       /*1*/,
                                                          Convert.ToString(item.ClsName)    /*2*/,
                                                          Convert.ToString(item.GrpName)    /*3*/,
                                                          Convert.ToString(item.GroupNum)   /*4*/,
                                                          Convert.ToString(item.LineNum)    /*5*/,
                                                          Convert.ToString(item.LineName)   /*6*/,
                                                          Convert.ToString(item.Track)      /*7*/,
                                                          Convert.ToString(item.LtName)     /*8*/,
                                                          Convert.ToString(item.Npicket)    /*9*/,
                                                          Convert.ToString(shift)           /*10*/,
                                                          Convert.ToString(item.Layout)     /*11*/
                                                        });
                                        }
                                        return;
                                    }
                                    else
                                    {
                                        var res = from r in _db_controller.all_equipment_table.AsEnumerable()  where   r.ClassNum == equClass.Code &&
                                                                                                                    r.GroupNum == equGroup.Code &&
                                                                                                                    r.LineNum == equLine.Code &&
                                                                                                                    r.Track == equPath.Code &&
                                                                                                                    r.Layout == equLayout.Code &&
                                                                                                                    r.ObjName.IndexOf(searchStr) >= 0    select r;

                                        foreach (var item in res)
                                        {
                                            var resEquipShift = from r in _db_controller.objects_table.AsEnumerable() where r.Code == item.Code select new { r.shiftLine };
                                            shift = (ulong)resEquipShift.First().shiftLine;
                                            dataGridView1.Rows.Add(new object[] { Convert.ToString(item.ObjName)    /*0*/,
                                                          Convert.ToString(item.Code)       /*1*/,
                                                          Convert.ToString(item.ClsName)    /*2*/,
                                                          Convert.ToString(item.GrpName)    /*3*/,
                                                          Convert.ToString(item.GroupNum)   /*4*/,
                                                          Convert.ToString(item.LineNum)    /*5*/,
                                                          Convert.ToString(item.LineName)   /*6*/,
                                                          Convert.ToString(item.Track)      /*7*/,
                                                          Convert.ToString(item.LtName)     /*8*/,
                                                          Convert.ToString(item.Npicket)    /*9*/,
                                                          Convert.ToString(shift)           /*10*/,
                                                          Convert.ToString(item.Layout)     /*11*/
                                                        });
                                        }
                                        return;
                                    }
                                }
                                // layout not select
                                var resPath = from r in _db_controller.all_equipment_table.AsEnumerable() where  r.ClassNum  == equClass.Code &&
                                                                                                              r.GroupNum == equGroup.Code  &&
                                                                                                              r.LineNum  == equLine.Code   &&
                                                                                                              r.Track    == equPath.Code   &&
                                                                                                              r.ObjName.IndexOf(searchStr) >= 0    select r;

                                foreach (var item in resPath)
                                {
                                    var resEquipShift = from r in _db_controller.objects_table.AsEnumerable() where r.Code == item.Code select new { r.shiftLine };
                                    shift = (ulong)resEquipShift.First().shiftLine;
                                    dataGridView1.Rows.Add(new object[] { Convert.ToString(item.ObjName)    /*0*/,
                                                          Convert.ToString(item.Code)       /*1*/,
                                                          Convert.ToString(item.ClsName)    /*2*/,
                                                          Convert.ToString(item.GrpName)    /*3*/,
                                                          Convert.ToString(item.GroupNum)   /*4*/,
                                                          Convert.ToString(item.LineNum)    /*5*/,
                                                          Convert.ToString(item.LineName)   /*6*/,
                                                          Convert.ToString(item.Track)      /*7*/,
                                                          Convert.ToString(item.LtName)     /*8*/,
                                                          Convert.ToString(item.Npicket)    /*9*/,
                                                          Convert.ToString(shift)           /*10*/,
                                                          Convert.ToString(item.Layout)     /*11*/
                                                        });
                                }
                                return;
                            }
                            // Track not select
                            var resLine = from r in _db_controller.all_equipment_table.AsEnumerable()  where  r.ClassNum == equClass.Code &&
                                                                                                           r.GroupNum == equGroup.Code &&
                                                                                                           r.LineNum == equLine.Code &&
                                                                                                           r.ObjName.IndexOf(searchStr) >= 0    select r;
                            foreach (var item in resLine)
                            {
                                var resEquipShift = from r in _db_controller.objects_table.AsEnumerable() where r.Code == item.Code select new { r.shiftLine };
                                shift = (ulong)resEquipShift.First().shiftLine;
                                dataGridView1.Rows.Add(new object[] { Convert.ToString(item.ObjName)    /*0*/,
                                                          Convert.ToString(item.Code)       /*1*/,
                                                          Convert.ToString(item.ClsName)    /*2*/,
                                                          Convert.ToString(item.GrpName)    /*3*/,
                                                          Convert.ToString(item.GroupNum)   /*4*/,
                                                          Convert.ToString(item.LineNum)    /*5*/,
                                                          Convert.ToString(item.LineName)   /*6*/,
                                                          Convert.ToString(item.Track)      /*7*/,
                                                          Convert.ToString(item.LtName)     /*8*/,
                                                          Convert.ToString(item.Npicket)    /*9*/,
                                                          Convert.ToString(shift)           /*10*/,
                                                          Convert.ToString(item.Layout)     /*11*/
                                                        });
                            }
                            return;
                        }
                        // line not select
                        var resGroup = from r in _db_controller.all_equipment_table.AsEnumerable()  where r.ClassNum == equClass.Code &&
                                                                                                       r.GroupNum == equGroup.Code &&
                                                                                                       r.ObjName.IndexOf(searchStr) >= 0     select r;
                        foreach (var item in resGroup)
                        {
                            var resEquipShift = from r in _db_controller.objects_table.AsEnumerable() where r.Code == item.Code select new { r.shiftLine };
                            shift = (ulong)resEquipShift.First().shiftLine;
                            dataGridView1.Rows.Add(new object[] { Convert.ToString(item.ObjName)    /*0*/,
                                                          Convert.ToString(item.Code)       /*1*/,
                                                          Convert.ToString(item.ClsName)    /*2*/,
                                                          Convert.ToString(item.GrpName)    /*3*/,
                                                          Convert.ToString(item.GroupNum)   /*4*/,
                                                          Convert.ToString(item.LineNum)    /*5*/,
                                                          Convert.ToString(item.LineName)   /*6*/,
                                                          Convert.ToString(item.Track)      /*7*/,
                                                          Convert.ToString(item.LtName)     /*8*/,
                                                          Convert.ToString(item.Npicket)    /*9*/,
                                                          Convert.ToString(shift)           /*10*/,
                                                          Convert.ToString(item.Layout)     /*11*/
                                                        });
                        }
                        return;
                    }
                    // group not select
                    var resClass = from r in _db_controller.all_equipment_table.AsEnumerable() where   r.ClassNum == equClass.Code &&
                                                                                                    r.ObjName.IndexOf(searchStr) >= 0   select r;
                    foreach (var item in resClass)
                    {
                        var resEquipShift = from r in _db_controller.objects_table.AsEnumerable() where r.Code == item.Code select new { r.shiftLine };
                        shift = (ulong)resEquipShift.First().shiftLine;
                        dataGridView1.Rows.Add(new object[] { Convert.ToString(item.ObjName)    /*0*/,
                                                          Convert.ToString(item.Code)       /*1*/,
                                                          Convert.ToString(item.ClsName)    /*2*/,
                                                          Convert.ToString(item.GrpName)    /*3*/,
                                                          Convert.ToString(item.GroupNum)   /*4*/,
                                                          Convert.ToString(item.LineNum)    /*5*/,
                                                          Convert.ToString(item.LineName)   /*6*/,
                                                          Convert.ToString(item.Track)      /*7*/,
                                                          Convert.ToString(item.LtName)     /*8*/,
                                                          Convert.ToString(item.Npicket)    /*9*/,
                                                          Convert.ToString(shift)           /*10*/,
                                                          Convert.ToString(item.Layout)     /*11*/
                                                        });
                    }
                    return;
                }
                // class not select
                var resEquip = from r in _db_controller.all_equipment_table.AsEnumerable()  where  r.ObjName.IndexOf(searchStr) >= 0 select r;
                foreach (var item in resEquip)
                {
                    var resEquipShift = from r in _db_controller.objects_table.AsEnumerable() where r.Code == item.Code select new { r.shiftLine };
                    shift = (ulong)resEquipShift.First().shiftLine;
                    dataGridView1.Rows.Add(new object[] { Convert.ToString(item.ObjName)    /*0*/,
                                                          Convert.ToString(item.Code)       /*1*/,
                                                          Convert.ToString(item.ClsName)    /*2*/,
                                                          Convert.ToString(item.GrpName)    /*3*/,
                                                          Convert.ToString(item.GroupNum)   /*4*/,
                                                          Convert.ToString(item.LineNum)    /*5*/,
                                                          Convert.ToString(item.LineName)   /*6*/,
                                                          Convert.ToString(item.Track)      /*7*/,
                                                          Convert.ToString(item.LtName)     /*8*/,
                                                          Convert.ToString(item.Npicket)    /*9*/,
                                                          Convert.ToString(shift)           /*10*/,
                                                          Convert.ToString(item.Layout)     /*11*/
                                                        });
                }
                return;
            }
        }

        private void dataGridView1_DoubleClick(object sender, EventArgs e)
        {
            if (dataGridView1.SelectedRows.Count == 0)
                return;

            int i = dataGridView1.SelectedRows[0].Index;

            EquGroup curGroup = new EquGroup(Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[4].Value), (String)dataGridView1.SelectedRows[0].Cells[3].Value);
            curGroup.Class = new EquClass(Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[1].Value), (String)dataGridView1.SelectedRows[0].Cells[2].Value);
            EquDbObject curLine = new EquDbObject(Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[5].Value), String.Concat(new object[] { "Линия ", Convert.ToString(Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[5].Value)) }));
            EquPath curPath = new EquPath(Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[7].Value), String.Concat(new object[] { "Путь ", Convert.ToString(dataGridView1.SelectedRows[0].Cells[7].Value) }));
            EquLayout curLayout = new EquLayout(Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[11].Value), (String)dataGridView1.SelectedRows[0].Cells[8].Value);

            EquObject elObj = new EquObject( Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[1].Value),
                                             (String)dataGridView1.SelectedRows[0].Cells[0].Value,
                                             curGroup,
                                             curLayout,
                                             curPath,
                                             new EquPicket(Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[9].Value),""),
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
