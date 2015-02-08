using System;
using System.Windows.Forms;
using System.Collections;
using System.Windows.Documents;
using System.Data.OleDb;
using System.Data;

namespace Registrator
{
    public partial class SearchElementsForm : Form
    {

        TreeNodeCollection m_classes = null;

        String searchString = "";

        ArrayList objs = new ArrayList();


        public SearchElementsForm()
        {
            InitializeComponent();
        }

        public SearchElementsForm(TreeNodeCollection classes)
            : this()
        {
            m_classes = classes;
            InitFilter();
        }

        public TreeNodeCollection Classes
        {
            get
            {
                return m_classes;
            }

            set
            {
                m_classes = value;
                InitFilter();
            }
        }

        public String SearchStr
        {
            get
            {
                return searchString;
            }

            set
            {
                searchString = value;
            }
        }

        public String ParseSearchString(String str)
        {

            String subquery = "";

            subquery += CreateFilter();

            if (str.Trim().Equals(""))
                return subquery;
            
            subquery += " AND ( ";

            string[] strs = str.Split(new string[] { " " }, StringSplitOptions.RemoveEmptyEntries);

            for (int i = 0; i < strs.Length; subquery += ((i == strs.Length - 1) ? (" Objects.Object LIKE '%" + strs[i] + "%' ") : (" Objects.Object LIKE '%" + strs[i] + "%' OR ")), i++) ;
            subquery += " ) ";
            
            return subquery;

        }

        public String CreateFilter()
        {

            String query = "";

            if (m_classes == null || classesComboBox.SelectedIndex < 1)
                return query;
            EquClass equClass = m_classes[classesComboBox.SelectedIndex - 1] as EquClass;
            if (equClass == null)
                return query;
            query += " AND Class.Code = " + equClass.Code + " ";

            if (groupsComboBox.SelectedIndex < 1)
                return query;
            EquGroup equGroup = equClass.Nodes[groupsComboBox.SelectedIndex - 1] as EquGroup;
            if(equGroup == null)
                return query;
            query += " AND [Group].Code = " + equGroup.Code +" ";

            if (linesComboBox.SelectedIndex < 1)
                return query;
            EquDbObject equLine = equGroup.Nodes[linesComboBox.SelectedIndex - 1] as EquDbObject;
            if (equLine == null)
                return query;
            query += " AND INT(Main.Layout / 100000) = " + equLine.Code + " ";

            if (pathsComboBox.SelectedIndex < 1)
                return query;
            EquDbObject equPath = equLine.Nodes[pathsComboBox.SelectedIndex - 1] as EquDbObject;
            if (equPath == null)
                return query;
            query += " AND Main.Track = " + equPath.Code + " ";

            if (peregonComboBox.SelectedIndex < 1)
                return query;
            EquLayout equLayout = equPath.Nodes[peregonComboBox.SelectedIndex - 1] as EquLayout;
            if (equLayout == null)
                return query;
            query += " AND Main.Layout = " + equLayout.Code + " ";

            if (!picketCb.Checked)
                return query;
            query += " AND Main.Point = " + (int)picketUpDown.Value +" ";

            if (!offsCb.Checked)
                return query;
            query += " AND Main.Displacement = " + (int)offsUpDown.Value + " ";

            return query;
        
        }

        public void Filter()
        {

            String subquery = "";

            subquery = ParseSearchString(searchBox.Text.Trim());

            String query = "SELECT Main.* "+
                           "     , Objects.[Group] " +
                           "     , [Group].Class " +
                           "     , Objects.Object AS ObjName " +
                           "     , [Group].[Group] AS GrpName " +
                           "     , Class.Class AS ClsName " +
                           "     , Layout.Layout AS LtName " +
                           "     , Layout.SName AS LtSName " +
                           "     , INT(Layout.Code/100000) AS Line" +
                           "  FROM Main " +
                           "     , Objects " +
                           "     , Class " + 
                           "     , [Group] " +
                           "     , Layout " +
                           " WHERE Objects.Code = Main.Code " +
                           "   AND [Group].Code = Objects.[Group] " + 
                           "   AND Class.Code = [Group].Class " + 
                           "   AND Layout.Code = Main.Layout " + subquery +
                           " ORDER BY [Group].Class " +
                           "     , Objects.[Group] " +
                           "     , INT(Main.Layout/100000) " +
                           "     , Main.Track " + 
                           "     , Main.Layout " +
                           "     , Main.Code ";

          //  m_dataAdapter = new System.Data.OleDb.OleDbDataAdapter(query, Properties.Settings.Default.MetrocardConnectionString);//"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=D:\\Registrator\\Registrator\\Metrocard.mdb;Persist Security Info=True");

         //   t = new DataTable();
         //   t.Locale = System.Globalization.CultureInfo.InvariantCulture;
         //   m_dataAdapter.Fill(t);

         //   metrocardDataSet1BindingSource.DataSource = t;

        }

        public void ClearCombos()
        {
            while (classesComboBox.Items.Count > 1)
                classesComboBox.Items.RemoveAt(classesComboBox.Items.Count - 1);

            while (groupsComboBox.Items.Count > 1)
                groupsComboBox.Items.RemoveAt(groupsComboBox.Items.Count - 1);

            while (linesComboBox.Items.Count > 1)
                linesComboBox.Items.RemoveAt(linesComboBox.Items.Count - 1);

            while (pathsComboBox.Items.Count > 1)
                pathsComboBox.Items.RemoveAt(pathsComboBox.Items.Count - 1);

            while (peregonComboBox.Items.Count > 1)
                peregonComboBox.Items.RemoveAt(peregonComboBox.Items.Count - 1);

        }

        public void InitFilter()
        {
            ClearCombos();
            InitClasses();
            //InitGroups();
            //InitLines();
            //InitPaths();
            //InitPeregon();
        }

        public void InitClasses()
        {

            while (classesComboBox.Items.Count > 1)
                classesComboBox.Items.RemoveAt(classesComboBox.Items.Count - 1);

            while (groupsComboBox.Items.Count > 1)
                groupsComboBox.Items.RemoveAt(groupsComboBox.Items.Count - 1);

            while (linesComboBox.Items.Count > 1)
                linesComboBox.Items.RemoveAt(linesComboBox.Items.Count - 1);

            while (pathsComboBox.Items.Count > 1)
                pathsComboBox.Items.RemoveAt(pathsComboBox.Items.Count - 1);

            while (peregonComboBox.Items.Count > 1)
                peregonComboBox.Items.RemoveAt(peregonComboBox.Items.Count - 1);

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
            groupsComboBox.SelectedIndex = 0;
            linesComboBox.SelectedIndex = 0;
            pathsComboBox.SelectedIndex = 0;
            peregonComboBox.SelectedIndex = 0;
        }

        public void InitGroups()
        {

            while (groupsComboBox.Items.Count > 1)
                groupsComboBox.Items.RemoveAt(groupsComboBox.Items.Count - 1);

            while (linesComboBox.Items.Count > 1)
                linesComboBox.Items.RemoveAt(linesComboBox.Items.Count - 1);

            while (pathsComboBox.Items.Count > 1)
                pathsComboBox.Items.RemoveAt(pathsComboBox.Items.Count - 1);

            while (peregonComboBox.Items.Count > 1)
                peregonComboBox.Items.RemoveAt(peregonComboBox.Items.Count - 1);

            if (classesComboBox.SelectedIndex > 0)
            {
                EquClass equClass = m_classes[classesComboBox.SelectedIndex - 1] as EquClass;

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

            groupsComboBox.SelectedIndex = 0;
            linesComboBox.SelectedIndex = 0;
            pathsComboBox.SelectedIndex = 0;
            peregonComboBox.SelectedIndex = 0;
        }

        public void InitLines()
        {

            while (linesComboBox.Items.Count > 1)
                linesComboBox.Items.RemoveAt(linesComboBox.Items.Count - 1);
            
            while (pathsComboBox.Items.Count > 1)
                pathsComboBox.Items.RemoveAt(pathsComboBox.Items.Count - 1);

            while (peregonComboBox.Items.Count > 1)
                peregonComboBox.Items.RemoveAt(peregonComboBox.Items.Count - 1);

            if (groupsComboBox.SelectedIndex > 0)
            {
                EquClass equClass = m_classes[classesComboBox.SelectedIndex - 1] as EquClass;
                EquGroup equGroup = null;
                if (equClass != null)
                    equGroup = equClass.Nodes[groupsComboBox.SelectedIndex - 1] as EquGroup;
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

            linesComboBox.SelectedIndex = 0;
            pathsComboBox.SelectedIndex = 0;
            peregonComboBox.SelectedIndex = 0;
        }

        public void InitPaths()
        {

            while (pathsComboBox.Items.Count > 1)
                pathsComboBox.Items.RemoveAt(pathsComboBox.Items.Count - 1);

            while (peregonComboBox.Items.Count > 1)
                peregonComboBox.Items.RemoveAt(peregonComboBox.Items.Count - 1);

            if (linesComboBox.SelectedIndex > 0)
            {
                EquClass equClass = m_classes[classesComboBox.SelectedIndex - 1] as EquClass;
                EquGroup equGroup = null;
                EquDbObject equLine = null;
                if (equClass != null)
                    equGroup = equClass.Nodes[groupsComboBox.SelectedIndex - 1] as EquGroup;
                if (equGroup != null)
                    equLine = equGroup.Nodes[linesComboBox.SelectedIndex - 1] as EquDbObject;
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

            pathsComboBox.SelectedIndex = 0;
            peregonComboBox.SelectedIndex = 0;
        }

        public void InitPeregon()
        {

            while (peregonComboBox.Items.Count > 1)
                peregonComboBox.Items.RemoveAt(peregonComboBox.Items.Count - 1);

            if (pathsComboBox.SelectedIndex > 0)
            {

                EquClass equClass = m_classes[classesComboBox.SelectedIndex - 1] as EquClass;
                EquGroup equGroup = null;
                EquDbObject equLine = null;
                EquDbObject equPath = null;
                if (equClass != null)
                    equGroup = equClass.Nodes[groupsComboBox.SelectedIndex - 1] as EquGroup;
                if (equGroup != null)
                    equLine = equGroup.Nodes[linesComboBox.SelectedIndex - 1] as EquDbObject;
                if (equLine != null)
                    equPath = equLine.Nodes[pathsComboBox.SelectedIndex - 1] as EquDbObject;
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
            peregonComboBox.SelectedIndex = 0;
        }

        public ArrayList Objects
        {
            get
            {
                return objs;
            }

            set
            {
                objs = value;
            }

        }

        

        private void closeButton_Click(object sender, EventArgs e)
        {
            Close();
            Dispose();
        }

        

        private void searchButton_Click(object sender, EventArgs e)
        {
            Filter();
        }

        private void dataGridView1_DoubleClick(object sender, EventArgs e)
        {

            //if (t == null || t.Rows.Count < 1)
            //    return;

            //int i = dataGridView1.SelectedRows[0].Index;

            //EquGroup curGroup = new EquGroup(Convert.ToInt32(t.Rows[i].ItemArray[10]), (String)t.Rows[i].ItemArray[13]);
            //curGroup.Class = new EquClass(Convert.ToInt32(t.Rows[i].ItemArray[11]), (String)t.Rows[i].ItemArray[14]);
            //EquDbObject curLine = new EquDbObject(Convert.ToInt32(t.Rows[i].ItemArray[1]) / 100000, String.Concat(new object[] { "Линия ", Convert.ToString(Convert.ToInt32(t.Rows[i].ItemArray[1]) / 100000) }));
            //EquDbObject curPath = new EquDbObject(Convert.ToInt32(t.Rows[i].ItemArray[2]), String.Concat(new object[] { "Путь ", Convert.ToString(t.Rows[i].ItemArray[2]) }));
            //EquLayout curLayout = new EquLayout(Convert.ToInt32(t.Rows[i].ItemArray[1]), (String)t.Rows[i].ItemArray[15]);

            //EquObject elObj = new EquObject(
            //                                     Convert.ToInt32(t.Rows[i].ItemArray[0]),
            //                                     (String)t.Rows[i].ItemArray[12],
            //                                     curGroup,
            //                                     curLayout,
            //                                     curPath.Code,
            //                                     Convert.ToInt32(t.Rows[i].ItemArray[6]),
            //                                     Convert.ToSingle(t.Rows[i].ItemArray[7])
            //                                 );

            //Registrator.DB.teplovizorDataSet.equipment1DataTable dt = equipment1TableAdapter1.GetData(
            //                                                                        elObj.Code,
            //                                                                        elObj.Layout.Code,
            //                                                                        elObj.Path,
            //                                                                        elObj.Picket,
            //                                                                        elObj.Offset
            //                                                                        );

            //if (dt.Rows.Count < 1)
            //{
            //    int r1 = equipmentTableAdapter1.InsertAutoQuery(
            //                                                    elObj.Code,
            //                                                    elObj.Layout.Code,
            //                                                    elObj.Path,
            //                                                    elObj.Picket,
            //                                                    elObj.Offset
            //                                                    );

            //    try
            //    {
            //        int r4 = equipmentTableAdapter1.Update(teplovizorDataSet1.equipment);

            //        dt = equipment1TableAdapter1.GetData(
            //                                                                        elObj.Code,
            //                                                                        elObj.Layout.Code,
            //                                                                        elObj.Path,
            //                                                                        elObj.Picket,
            //                                                                        elObj.Offset
            //                                                                        );

            //        if (dt.Rows.Count > 0)
            //        {
            //            elObj.ID = Convert.ToInt32(dt.Rows[0].ItemArray[0]);
            //        }

            //    }
            //    catch (Exception )
            //    {
            //        MessageBox.Show("1 failed");
            //    }

            //}
            //else
            //{
            //    //if (dt.Rows.Count > 0)
            //    //{
            //    elObj.ID = Convert.ToInt32(dt.Rows[0].ItemArray[0]);
            //    //}
            //}

            //EquElementForm eqf = new EquElementForm(elObj);
            //eqf.ShowDialog(this);

        }

        private void picketCb_CheckedChanged(object sender, EventArgs e)
        {
            picketUpDown.Enabled = picketCb.Checked;
            offsCb.Enabled = picketCb.Checked;
        }

        private void offsCb_CheckedChanged(object sender, EventArgs e)
        {
            offsUpDown.Enabled = offsCb.Checked;
        }

        private void classesComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {

            InitGroups();

        }

        private void groupsComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            InitLines();
        }

        private void linesComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            InitPaths();
        }

        private void pathsComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            InitPeregon();
        }

        private void peregonComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            picketCb.Enabled = (peregonComboBox.Enabled && peregonComboBox.SelectedIndex > 0);
            if(!picketCb.Enabled)
                picketCb.Checked = false;
        }

        private void searchBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar.Equals('\r'))
                Filter();
        }
                
    }
}
