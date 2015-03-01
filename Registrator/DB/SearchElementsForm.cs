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
        private DB.DataBaseHelper dbHelper;

        public SearchElementsForm()
        {
            InitializeComponent();
        }

        public SearchElementsForm(TreeNodeCollection classes, ref DB.DataBaseHelper dbHelperArg)
            : this()
        {
            m_classes = classes;
            dbHelper = dbHelperArg;
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
                    EquClass curClass = m_classes[i] as EquClass;

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
            int shift = 0;
            EquClass equClass = null;
            EquGroup equGroup = null;
            EquDbObject equLine = null;
            EquDbObject equPath = null;
            EquLayout equLayout = null;

            if (searchStr.Length > 0)
            {
                if(groupsComboBox.SelectedIndex!=-1)
                    equClass = m_classes[classesComboBox.SelectedIndex-1] as EquClass;
                if (equClass != null)
                {
                    if(groupsComboBox.SelectedIndex!=-1)
                        equGroup = equClass.Nodes[groupsComboBox.SelectedIndex-1] as EquGroup;
                    if (equGroup != null)
                    {
                        if(linesComboBox.SelectedIndex != -1)
                            equLine = equGroup.Nodes[linesComboBox.SelectedIndex-1] as EquDbObject;
                        if (equLine != null)
                        {
                            if(pathsComboBox.SelectedIndex != -1)
                                equPath = equLine.Nodes[pathsComboBox.SelectedIndex-1] as EquDbObject;
                            if (equPath != null)
                            {
                                if (peregonComboBox.SelectedIndex != -1)
                                    equLayout = equPath.Nodes[peregonComboBox.SelectedIndex-1] as EquLayout;
                                if (equLayout != null)
                                {

                                    int npicket=0;
                                    if(picketCb.Enabled)
                                    {
                                        npicket = (int)picketUpDown.Value;

                                        var res = from r in dbHelper.dataTable_AllEquipment.AsEnumerable()   where  r.ClassNum == equClass.Code &&
                                                                                                                    r.GroupNum == equGroup.Code &&
                                                                                                                    r.LineNum == equLine.Code &&
                                                                                                                    r.Track == equPath.Code &&
                                                                                                                    r.Layout == equLayout.Code &&
                                                                                                                    r.Npicket == npicket &&
                                                                                                                    r.ObjName.IndexOf(searchStr) >= 0    select r;

                                        foreach (var item in res)
                                        {
                                            var resEquipShift = from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Code == item.Code select new { r.shiftLine };
                                            shift = resEquipShift.First().shiftLine;
                                            dataGridView1.Rows.Add(new object[] { Convert.ToString(item.ObjName), Convert.ToString(item.Code), Convert.ToString(item.ClassNum), Convert.ToString(item.GroupNum), Convert.ToString(item.Layout), Convert.ToString(item.ClsName), Convert.ToString(item.GrpName), Convert.ToString(item.LineNum), Convert.ToString(item.Track), Convert.ToString(item.LtName), Convert.ToString(item.Npicket), Convert.ToString(shift) });
                                        }
                                        return;
                                    }
                                    else
                                    {
                                        var res = from r in dbHelper.dataTable_AllEquipment.AsEnumerable()  where   r.ClassNum == equClass.Code &&
                                                                                                                    r.GroupNum == equGroup.Code &&
                                                                                                                    r.LineNum == equLine.Code &&
                                                                                                                    r.Track == equPath.Code &&
                                                                                                                    r.Layout == equLayout.Code &&
                                                                                                                    r.ObjName.IndexOf(searchStr) >= 0    select r;

                                        foreach (var item in res)
                                        {
                                            var resEquipShift = from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Code == item.Code select new { r.shiftLine };
                                            shift = resEquipShift.First().shiftLine;
                                            dataGridView1.Rows.Add(new object[] { Convert.ToString(item.ObjName), Convert.ToString(item.Code), Convert.ToString(item.ClassNum), Convert.ToString(item.GroupNum), Convert.ToString(item.Layout), Convert.ToString(item.ClsName), Convert.ToString(item.GrpName), Convert.ToString(item.LineNum), Convert.ToString(item.Track), Convert.ToString(item.LtName), Convert.ToString(item.Npicket), Convert.ToString(shift) });
                                        }
                                        return;
                                    }
                                }
                                // layout not select
                                var resPath = from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where  r.ClassNum  == equClass.Code &&
                                                                                                              r.GroupNum == equGroup.Code  &&
                                                                                                              r.LineNum  == equLine.Code   &&
                                                                                                              r.Track    == equPath.Code   &&
                                                                                                              r.ObjName.IndexOf(searchStr) >= 0    select r;

                                foreach (var item in resPath)
                                {
                                    var resEquipShift = from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Code == item.Code select new { r.shiftLine };
                                    shift = resEquipShift.First().shiftLine;
                                    dataGridView1.Rows.Add(new object[] { Convert.ToString(item.ObjName), Convert.ToString(item.Code), Convert.ToString(item.ClassNum), Convert.ToString(item.GroupNum), Convert.ToString(item.Layout), Convert.ToString(item.ClsName), Convert.ToString(item.GrpName), Convert.ToString(item.LineNum), Convert.ToString(item.Track), Convert.ToString(item.LtName), Convert.ToString(item.Npicket), Convert.ToString(shift) });
                                }
                                return;
                            }
                            // Track not select
                            var resLine = from r in dbHelper.dataTable_AllEquipment.AsEnumerable()  where  r.ClassNum == equClass.Code &&
                                                                                                           r.GroupNum == equGroup.Code &&
                                                                                                           r.LineNum == equLine.Code &&
                                                                                                           r.ObjName.IndexOf(searchStr) >= 0    select r;
                            foreach (var item in resLine)
                            {
                                var resEquipShift = from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Code == item.Code select new { r.shiftLine };
                                shift = resEquipShift.First().shiftLine;
                                dataGridView1.Rows.Add(new object[] { Convert.ToString(item.ObjName), Convert.ToString(item.Code), Convert.ToString(item.ClassNum), Convert.ToString(item.GroupNum), Convert.ToString(item.Layout), Convert.ToString(item.ClsName), Convert.ToString(item.GrpName), Convert.ToString(item.LineNum), Convert.ToString(item.Track), Convert.ToString(item.LtName), Convert.ToString(item.Npicket), Convert.ToString(shift) });
                            }
                            return;
                        }
                        // line not select
                        var resGroup = from r in dbHelper.dataTable_AllEquipment.AsEnumerable()  where r.ClassNum == equClass.Code &&
                                                                                                       r.GroupNum == equGroup.Code &&
                                                                                                       r.ObjName.IndexOf(searchStr) >= 0     select r;
                        foreach (var item in resGroup)
                        {
                            var resEquipShift = from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Code == item.Code select new { r.shiftLine };
                            shift = resEquipShift.First().shiftLine;
                            dataGridView1.Rows.Add(new object[] { Convert.ToString(item.ObjName), Convert.ToString(item.Code), Convert.ToString(item.ClassNum), Convert.ToString(item.GroupNum), Convert.ToString(item.Layout), Convert.ToString(item.ClsName), Convert.ToString(item.GrpName), Convert.ToString(item.LineNum), Convert.ToString(item.Track), Convert.ToString(item.LtName), Convert.ToString(item.Npicket), Convert.ToString(shift) });
                        }
                        return;
                    }
                    // group not select
                    var resClass = from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where   r.ClassNum == equClass.Code &&
                                                                                                    r.ObjName.IndexOf(searchStr) >= 0   select r;
                    foreach (var item in resClass)
                    {
                        var resEquipShift = from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Code == item.Code select new { r.shiftLine };
                        shift = resEquipShift.First().shiftLine;
                        dataGridView1.Rows.Add(new object[] { Convert.ToString(item.ObjName), Convert.ToString(item.Code), Convert.ToString(item.ClassNum), Convert.ToString(item.GroupNum), Convert.ToString(item.Layout), Convert.ToString(item.ClsName), Convert.ToString(item.GrpName), Convert.ToString(item.LineNum), Convert.ToString(item.Track), Convert.ToString(item.LtName), Convert.ToString(item.Npicket), Convert.ToString(shift) });
                    }
                    return;
                }
                // class not select
                var resEquip = from r in dbHelper.dataTable_AllEquipment.AsEnumerable()  where  r.ObjName.IndexOf(searchStr) >= 0 select r;
                foreach (var item in resEquip)
                {
                    var resEquipShift = from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Code == item.Code select new { r.shiftLine };
                    shift = resEquipShift.First().shiftLine;
                    dataGridView1.Rows.Add(new object[] { Convert.ToString(item.ObjName), Convert.ToString(item.Code), Convert.ToString(item.ClassNum), Convert.ToString(item.GroupNum), Convert.ToString(item.Layout), Convert.ToString(item.ClsName), Convert.ToString(item.GrpName), Convert.ToString(item.LineNum), Convert.ToString(item.Track), Convert.ToString(item.LtName), Convert.ToString(item.Npicket), Convert.ToString(shift) });
                }
                return;
            }
        }

        private void dataGridView1_DoubleClick(object sender, EventArgs e)
        {
            if (dataGridView1.SelectedRows.Count == 0)
                return;

            int i = dataGridView1.SelectedRows[0].Index;

            EquGroup curGroup = new EquGroup(Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[3].Value), (String)dataGridView1.SelectedRows[0].Cells[8].Value);
            curGroup.Class = new EquClass(Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[2].Value), (String)dataGridView1.SelectedRows[0].Cells[5].Value);
            EquDbObject curLine = new EquDbObject(Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[7].Value), String.Concat(new object[] { "Линия ", Convert.ToString(Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[7].Value)) }));
            EquDbObject curPath = new EquDbObject(Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[8].Value), String.Concat(new object[] { "Путь ", Convert.ToString(dataGridView1.SelectedRows[0].Cells[8].Value) }));
            EquLayout curLayout = new EquLayout(Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[4].Value), (String)dataGridView1.SelectedRows[0].Cells[9].Value);

            EquObject elObj = new EquObject( Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[2].Value),
                                             (String)dataGridView1.SelectedRows[0].Cells[0].Value,
                                             curGroup,
                                             curLayout,
                                             curPath.Code,
                                             Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[10].Value),
                                             Convert.ToSingle(dataGridView1.SelectedRows[0].Cells[11].Value)  );

            elObj.ID = Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[2].Value);

            EquElementForm eqf = new EquElementForm(elObj, ref dbHelper);
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
                EquClass equClass = m_classes[classesComboBox.SelectedIndex-1] as EquClass;

                if (equClass != null)
                {
                    for (int i = 0; i < equClass.Nodes.Count; i++)
                    {
                        EquGroup curGroup = equClass.Nodes[i] as EquGroup;

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
                EquClass equClass = m_classes[classesComboBox.SelectedIndex-1] as EquClass;
                EquGroup equGroup = null;

                if (equClass != null && groupsComboBox.SelectedIndex > 0)
                    equGroup = equClass.Nodes[groupsComboBox.SelectedIndex - 1] as EquGroup;
                else
                    return;
                if (equGroup != null)
                {
                    linesComboBox.Items.Add("");
                    for (int i = 0; i < equGroup.Nodes.Count; i++)
                    {
                        EquDbObject curLine = equGroup.Nodes[i] as EquDbObject;
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

                EquClass equClass = m_classes[classesComboBox.SelectedIndex-1] as EquClass;
                EquGroup equGroup = null;
                EquDbObject equLine = null;

                if (equClass != null && groupsComboBox.SelectedIndex > 0)
                    equGroup = equClass.Nodes[groupsComboBox.SelectedIndex - 1] as EquGroup;
                else
                    return;

                if (equGroup != null && linesComboBox.SelectedIndex > 0)
                    equLine = equGroup.Nodes[linesComboBox.SelectedIndex - 1] as EquDbObject;
                else
                    return;

                if (equLine != null)
                {
                    for (int i = 0; i < equLine.Nodes.Count; i++)
                    {
                        EquDbObject curPath = equLine.Nodes[i] as EquDbObject;
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
                EquClass equClass = m_classes[classesComboBox.SelectedIndex - 1] as EquClass;
                EquGroup equGroup = null;
                EquDbObject equLine = null;
                EquDbObject equPath = null;
                if (equClass != null && groupsComboBox.SelectedIndex > 0)
                    equGroup = equClass.Nodes[groupsComboBox.SelectedIndex - 1] as EquGroup;
                else
                    return;
                if (equGroup != null && linesComboBox.SelectedIndex > 0)
                    equLine = equGroup.Nodes[linesComboBox.SelectedIndex - 1] as EquDbObject;
                else
                    return;
                if (equLine != null && pathsComboBox.SelectedIndex > 0)
                    equPath = equLine.Nodes[pathsComboBox.SelectedIndex - 1] as EquDbObject;
                else
                    return;
                if (equPath != null)
                {
                    peregonComboBox.Items.Add("");
                    for (int i = 0; i < equPath.Nodes.Count; i++)
                    {
                        EquLayout curLayout = equPath.Nodes[i] as EquLayout;
                        if (curLayout != null)
                            peregonComboBox.Items.Add(curLayout.Name);
                    }
                }
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
