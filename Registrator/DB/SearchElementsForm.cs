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

        String searchString = "";

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

        public ArrayList Objects
        {
            get { return objs; }
            set { objs = value; }
        }

        private void closeButton_Click(object sender, EventArgs e)
        {
            Close();
            Dispose();
        }

        private void searchButton_Click(object sender, EventArgs e)
        {
            string searchStr = searchBox.Text;

            if (searchStr.Length > 0)
            {
                EquClass equClass = m_classes[classesComboBox.SelectedIndex] as EquClass;
                if (equClass != null)
                {
                    EquGroup equGroup = equClass.Nodes[groupsComboBox.SelectedIndex] as EquGroup;
                    if (equGroup != null)
                    {
                        EquDbObject equLine = equGroup.Nodes[linesComboBox.SelectedIndex] as EquDbObject;
                        if (equLine != null)
                        {
                            EquDbObject equPath = equLine.Nodes[pathsComboBox.SelectedIndex] as EquDbObject;
                            if (equPath != null)
                            {
                                EquLayout equLayout = equPath.Nodes[peregonComboBox.SelectedIndex] as EquLayout;
                                if (equLayout != null)
                                {
                                    var res = from r in dbHelper.dataTable_AllEquipment.AsEnumerable()
                                              where r.ClassNum == equClass.Code &&
                                                     r.GroupNum == equGroup.Code &&
                                                     r.LineNum == equLine.Code &&
                                                     r.Track == equPath.Code &&
                                                     r.Layout == equLayout.Code &&
                                                     r.ObjName.IndexOf(searchStr) >= 0
                                              select r;
                                    int shift = 0;

                                    foreach (var item in res)
                                    {
                                        var resEquipShift = from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Code == item.Code select new { r.shiftLine };
                                        shift = resEquipShift.First().shiftLine;
                                        dataGridView1.Rows.Add(new object[] { Convert.ToString(item.ObjName), Convert.ToString(item.Code), Convert.ToString(item.ClassNum), Convert.ToString(item.GroupNum), Convert.ToString(item.Layout), Convert.ToString(item.ClsName), Convert.ToString(item.GrpName), Convert.ToString(item.LineNum), Convert.ToString(item.Track), Convert.ToString(item.LtName), Convert.ToString(item.Npicket), Convert.ToString(shift) });
                                    }
                                }
                            }
                        }
                    }

                }
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

            EquObject elObj = new EquObject(Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[2].Value),
                                                 (String)dataGridView1.SelectedRows[0].Cells[0].Value,
                                                 curGroup,
                                                 curLayout,
                                                 curPath.Code,
                                                 Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[10].Value),
                                                 Convert.ToSingle(dataGridView1.SelectedRows[0].Cells[11].Value)
                                             );

            elObj.ID = Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[2].Value);


            EquElementForm eqf = new EquElementForm(elObj, ref dbHelper);
            eqf.ShowDialog(this);
        }

        private void picketCb_CheckedChanged(object sender, EventArgs e)
        {
            //picketUpDown.Enabled = picketCb.Checked;
            //offsCb.Enabled = picketCb.Checked;
        }

        private void offsCb_CheckedChanged(object sender, EventArgs e)
        {
            //offsUpDown.Enabled = offsCb.Checked;
        }

        private void classesComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {

            groupsComboBox.Items.Clear();
            linesComboBox.Items.Clear();
            peregonComboBox.Items.Clear();

            if (classesComboBox.SelectedIndex != -1)
            {
                EquClass equClass = m_classes[classesComboBox.SelectedIndex] as EquClass;

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

            //groupsComboBox.SelectedIndex = 0;
            //linesComboBox.SelectedIndex = 0;
            //peregonComboBox.SelectedIndex = 0;

        }

        private void groupsComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            linesComboBox.Items.Clear();
            peregonComboBox.Items.Clear();

            if (groupsComboBox.SelectedIndex != -1)
            {
                EquClass equClass = m_classes[classesComboBox.SelectedIndex] as EquClass;
                EquGroup equGroup = null;

                if (equClass != null)
                    equGroup = equClass.Nodes[groupsComboBox.SelectedIndex] as EquGroup;
                if (equGroup != null)
                {
                    for (int i = 0; i < equGroup.Nodes.Count; i++)
                    {
                        EquDbObject curLine = equGroup.Nodes[i] as EquDbObject;
                        if (curLine != null)
                            linesComboBox.Items.Add(curLine.Code);
                    }
                }
            }

            //linesComboBox.SelectedIndex = 0;
            //pathsComboBox.SelectedIndex = 0;
            //peregonComboBox.SelectedIndex = 0;
        }

        private void linesComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            pathsComboBox.Items.Clear();
            peregonComboBox.Items.Clear();

            if (linesComboBox.SelectedIndex != -1)
            {
                EquClass equClass = m_classes[classesComboBox.SelectedIndex] as EquClass;
                EquGroup equGroup = null;
                EquDbObject equLine = null;

                if (equClass != null)
                    equGroup = equClass.Nodes[groupsComboBox.SelectedIndex] as EquGroup;
                if (equGroup != null)
                    equLine = equGroup.Nodes[linesComboBox.SelectedIndex] as EquDbObject;

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

            //pathsComboBox.SelectedIndex = 0;
            //peregonComboBox.SelectedIndex = 0;
        }

        private void pathsComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            peregonComboBox.Items.Clear();

            if (pathsComboBox.SelectedIndex != -1)
            {
                EquClass equClass = m_classes[classesComboBox.SelectedIndex] as EquClass;
                EquGroup equGroup = null;
                EquDbObject equLine = null;
                EquDbObject equPath = null;
                if (equClass != null)
                    equGroup = equClass.Nodes[groupsComboBox.SelectedIndex] as EquGroup;
                if (equGroup != null)
                    equLine = equGroup.Nodes[linesComboBox.SelectedIndex] as EquDbObject;
                if (equLine != null)
                    equPath = equLine.Nodes[pathsComboBox.SelectedIndex] as EquDbObject;
                if (equPath != null)
                {
                    for (int i = 0; i < equPath.Nodes.Count; i++)
                    {
                        EquLayout curLayout = equPath.Nodes[i] as EquLayout;
                        if (curLayout != null)
                            peregonComboBox.Items.Add(curLayout.Name);
                    }
                }
            }
            //peregonComboBox.SelectedIndex = 0;
        }

        private void peregonComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            //picketCb.Enabled = (peregonComboBox.Enabled && peregonComboBox.SelectedIndex > 0);
            //if(!picketCb.Enabled)
            //picketCb.Checked = false;
        }

        private void searchBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            // if (e.KeyChar.Equals('\r'))
            //Filter();
        }

    }
}
