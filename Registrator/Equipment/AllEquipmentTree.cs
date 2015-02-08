using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Registrator
{
    public partial class AllEquipmentTree : ToolWindow
    {
        public DB.DataBaseHelper dbHelper;
        //DB.MetrocardDataSet.EquipmentAllDataTable m_dataTable;
    
        //----------------------------------------------- forms ----------------------------------
        private Equipment.addNewEquipment form_newEquip;
        private AddNewElementToDatabase form1;
        private AddNewGruop form_NewGruop;
        //private AddNewEquipment form_NewEquipment;


        private EquClass equClassNew;
        private EquGroup equGroupNew;
        private EquLine equLineNew;
        private EquPath equPathNew;
        private EquLayout equLayoutNew;
        private Picket equPicketNew;
        private EquTreeNode objNodeNew;
        private EquObject equObjMew;

        Peregons peregonsObj;
        List<int> picketCode;
        List<int> peregonCode;
        //---------------------------------------------------------------------------------------

        //---------------------------------------------------------------------------------------
        EquGroup EquGroupArg;
        //---------------------------------------------------------------------------------------
        public AllEquipmentTree(DB.DataBaseHelper dbHelperArg)
        {
            InitializeComponent();
            dbHelper = dbHelperArg;
            InitTree();
            //treeView1.MouseDown += treeView1_MouseDown;
            treeView1.AfterLabelEdit+=treeView1_AfterLabelEdit;
           
            // this.refreshToolStripMenuItem.Click += new System.EventHandler(this.refreshToolStripMenuItem_Click);
            // this.mnuNewFile.Click += new System.EventHandler(this.mnuNewFile_Click);
            this.treeView1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.treeView1_MouseUp);

            
        }
        public void calcPeregon(ref EquPath curPathArg, int curLineCode, ref EquLayout curLayout)
        {
            if (curPathArg.Nodes.Count != 0)
            {
                var empData = from r in dbHelper.dataTable_LayoutTable.AsEnumerable() where r.Line == curLineCode select new { r.Code, r.NperegonBefore, r.NperegonAfter };

                peregonCode.Clear();

                int whileIndex = 0;
                var val1 = empData.First();

                while (true)
                {
                    whileIndex = 0;
                    foreach (var item in empData)
                    {
                        if (val1.Code == item.NperegonAfter)
                        {
                            val1 = item;
                            whileIndex = 1;
                        }
                    }
                    if (val1.NperegonBefore == 0 || whileIndex == 0) break;
                }

                //
                // create logical list of pickets
                peregonCode.Add(val1.Code);

                while (true)
                {
                    whileIndex = 0;
                    foreach (var item in empData)
                    {
                        if (val1.Code == item.NperegonBefore)
                        {
                            val1 = item;
                            peregonCode.Add(val1.Code);
                            whileIndex = 1;
                        }
                    }
                    if (whileIndex == 0) break;
                }

                int addPicketIndex = peregonCode.IndexOf(curLayout.Code);// индекс добавляемого пикета

                if (addPicketIndex == 0) // если добавляемый пикет первый в перегоне
                    curPathArg.Nodes.Insert(0, curLayout);
                else
                {
                    if (addPicketIndex == peregonCode.Count - 1) // если добавляемый пикет последний в перегоне
                        curPathArg.Nodes.Insert((peregonCode.Count - 1), curLayout);
                    else
                    {
                        addPicketIndex--;
                        int end = 0;
                        for (int y = 0; y < addPicketIndex; y++)
                        {
                            for (int i1 = 0; i1 < curPathArg.Nodes.Count; i1++)
                            {
                                if (((EquLayout)curPathArg.Nodes[i1]).Code == peregonCode[addPicketIndex-y])
                                {
                                    curPathArg.Nodes.Insert(i1 + 1, curLayout);
                                    end = 1;
                                    break;
                                }
                            }
                            if (end == 1) break;
                                
                        }
                        if(end!=1)
                            curPathArg.Nodes.Insert(0,curLayout);
                    }
                }
            }
            else
                curPathArg.Nodes.Add(curLayout);
        }

        public void calcPicket(ref EquLayout curLayoutArg, int curPeregonCode, ref Picket PicketObj)
        {
            if (curLayoutArg.Nodes.Count != 0)
            {
                var empData = from r in dbHelper.dataTable_PicketsTable.AsEnumerable() where r.Peregon == curPeregonCode && r.Npiketa != 0 orderby r.Npiketa select new { r.Npiketa, r.Peregon, r.NpicketBefore, r.NpicketAfter };

                picketCode.Clear();

                int whileIndex = 0;
                var val1 = empData.First();

                while (true)
                {
                    whileIndex = 0;
                    foreach (var item in empData)
                    {
                        if (val1.Npiketa == item.NpicketAfter)
                        {
                            val1 = item;
                            whileIndex = 1;
                        }
                    }
                    if (val1.NpicketBefore == 0 || whileIndex == 0) break;
                }
                //
                // create logical list of pickets
                picketCode.Add(val1.Npiketa);

                while (true)
                {
                    whileIndex = 0;
                    foreach (var item in empData)
                    {
                        if (val1.Npiketa == item.NpicketBefore)
                        {
                            val1 = item;
                            picketCode.Add(val1.Npiketa);
                            whileIndex = 1;
                        }
                    }
                    if (whileIndex == 0) break;
                }

                int addPicketIndex = picketCode.IndexOf(PicketObj.Code);// индекс добавляемого пикета

                if (addPicketIndex == 0) // если добавляемый пикет первый в перегоне
                    curLayoutArg.Nodes.Insert(0, PicketObj);
                else
                {
                    if (addPicketIndex == picketCode.Count - 1)// если добавляемый пикет последний в перегоне
                        curLayoutArg.Nodes.Insert((picketCode.Count - 1), PicketObj);
                    else
                    {
                        addPicketIndex--;
                        int end = 0;
                        for (int y = 0; y < addPicketIndex; y++)
                        {
                            for (int i1 = 0; i1 < curLayoutArg.Nodes.Count; i1++)
                            {
                                if (((Picket)curLayoutArg.Nodes[i1]).Code == picketCode[addPicketIndex-y])
                                {
                                    curLayoutArg.Nodes.Insert(i1 + 1, PicketObj);
                                    end = 1;
                                    break;
                                }
                            }
                            if (end == 1) break;
                        }
                        if (end != 1)
                            curLayoutArg.Nodes.Insert(0,PicketObj);
                    }
                }
            }
            else
                curLayoutArg.Nodes.Add(PicketObj);
        
        }
        private void InitTree()
        {
            //dbHelper.TblAdapter_AllEquipment.Fill()
            //allEquipmentTableAdapter1.Fill(metrocardDataSet1.EquipmentAll);
            //m_dataTable = allEquipmentTableAdapter1.GetData();
            //dbHelper.dataTable_AllEquipment.Rows.Count;
            //int count = m_dataTable.Rows.Count;

            int count = dbHelper.dataTable_AllEquipment.Rows.Count;

            EquLayout curLayout = null;
            EquGroup curGroup = null;
            EquClass curClass = null;
            EquPath curPath = null;
            EquLine curLine = null;
            Picket PicketObj = null;

            picketCode = new List<int>();
            peregonCode = new List<int>();

            for (int i = 0; i < count; i++)
            {
                if (curClass == null || (Convert.ToInt32(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[20])) != curClass.Code)
                {
                    curClass = new EquClass(Convert.ToInt32(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[20]), (String)dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[11]);
                    treeView1.Nodes.Add(curClass);
                    curLine = null;
                    curPath = null;
                    curLayout = null;
                    PicketObj = null;
                }
                if (Convert.ToInt32(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[19]) != 0)
                {
                    if ((curGroup == null || (Convert.ToInt32(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[19])) != curGroup.Code))
                    {
                        curGroup = new EquGroup(Convert.ToInt32(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[19]), (String)dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[10]);
                        curClass.Nodes.Add(curGroup);
                        curLine = null;
                    }
                    if (Convert.ToInt32(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[18]) != 0)
                    {
                        if (curLine == null || curLine.Code != Convert.ToInt32(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[18]))
                        {
                            curLine = new EquLine(Convert.ToInt32(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[18]), String.Concat(new object[] { "Линия ", Convert.ToString(Convert.ToInt32(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[18])) }));
                            curGroup.Nodes.Add(curLine);

                        }
                        if (Convert.ToInt32(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[2]) != 0)
                        {
                            if (curPath == null || (Convert.ToInt32(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[2])) != curPath.Code)
                            {
                                curPath = new EquPath(Convert.ToInt32(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[2]), String.Concat(new object[] { "Путь ", Convert.ToString(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[2]) }));
                                curLine.Nodes.Add(curPath);
                            }
                            if (Convert.ToInt32(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[15]) != 0)
                            {
                                //---------- peregons --------------------------------------------------------------------------------------------------------------
                                if (curLayout == null || (Convert.ToInt32(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[15])) != curLayout.Code)
                                {
                                    curLayout = new EquLayout(Convert.ToInt32(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[15]), (String)dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[12]);
                                    calcPeregon(ref curPath, curLine.Code, ref curLayout);
                                    
                                }

                                if (Convert.ToInt32(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[1]) != 0)
                                {
                                    if (PicketObj == null || (Convert.ToInt32(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[1])) != PicketObj.Code)
                                    {
                                        PicketObj = new Picket(Convert.ToInt32(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[1]), String.Concat(new object[] { "Пикет ", Convert.ToString(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[1]) }));
                                        calcPicket(ref curLayout, curLayout.Code, ref PicketObj);
                                    }

                                   //curLayout.Nodes.Add(PicketObj);
                                }
                                if (Convert.ToInt32(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[0]) != 0)
                                {
                                    EquObject obj = new EquObject(
                                                                        Convert.ToInt32(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[0]),
                                                                        (String)dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[9],
                                                                        curGroup,
                                                                        curLayout,
                                                                        curPath.Code,
                                                                        Convert.ToInt32(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[6]),
                                                                        Convert.ToSingle(dbHelper.dataTable_AllEquipment.Rows[i].ItemArray[7])
                                                                    );

                                    EquTreeNode objNode = new EquTreeNode(String.Concat(new object[] { curGroup.Name, " ", obj.Name }));
                                    objNode.UserObject = obj;
                                    PicketObj.Nodes.Add(objNode);
                                }
                            }
                        }
                    }
                }
            }
        }


        private void treeView1_DoubleClick(object sender, EventArgs e)
        {

            if (treeView1.SelectedNode != null && (treeView1.SelectedNode as EquTreeNode).UserObject != null)
            {
                EquObject elObj = (treeView1.SelectedNode as EquTreeNode).UserObject as EquObject;

                //Registrator.teplovizorDataSet.equipment1DataTable dt = equipment1TableAdapter1.GetData(
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
                //    catch (Exception ex)
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

                //elObj.IsLeft = ((byte)equipmentTableAdapter1.EquipmentSiteForID(elObj.ID)) == 1;

                //Registrator.teplovizorDataSet.stateDataTable sdt = stateTableAdapter1.GetDataByLastElementState(elObj.ID);

                //if (sdt.Rows.Count > 0)
                //{
                //    elObj.State = (byte)Convert.ToInt32(sdt.Rows[0].ItemArray[3]);
                //}

                dbHelper.dataTable_Objects.Clear();
                dbHelper.TblAdapter_Objects.Fill(dbHelper.dataTable_Objects);

                var empData1 = (from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Code == elObj.Code select new { r.x, r.y, r.shiftFromPicket, r.regularly });
                var item = empData1.First();

                //dbHelper.dataTable_GroupTable.Clear();
                //dbHelper.TblAdapter_Group.Fill(dbHelper.dataTable_GroupTable);

                //var empClass = (from r in dbHelper.dataTable_GroupTable.AsEnumerable() where r.Code == elObj.Group.Code select new {r.Class });
                //var empClassName = (from r in dbHelper.dataTable_Class.AsEnumerable() where r.Code == empClass.First().Class select new {r.Class });

                objNodeNew   = (EquTreeNode)treeView1.SelectedNode;
                equObjMew    = (EquObject)objNodeNew.UserObject;
                equPicketNew = (Picket)objNodeNew.Parent;
                equLayoutNew = (EquLayout)equPicketNew.Parent;
                equPathNew   = (EquPath)equLayoutNew.Parent;
                equLineNew   = (EquLine)equPathNew.Parent;
                equGroupNew  = (EquGroup)equLineNew.Parent;
                equClassNew  = (EquClass)equGroupNew.Parent;

                //Eq rn = treeView1.SelectedNode
                elObj.Picket = equPicketNew.Code;
                elObj.Group.Class = equClassNew;
                elObj.Offset = item.shiftFromPicket;
                elObj.State = (byte)item.regularly;
                
                EquElementForm eqf = new EquElementForm(elObj,ref dbHelper);
                eqf.ShowDialog(this);
            }

        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            EquipmentFilterTool eft = new EquipmentFilterTool();
            eft.ShowDialog();
        }

        private void searchButton_Click(object sender, EventArgs e)
        {
            SearchElementsForm sef = new SearchElementsForm(treeView1.Nodes);
            sef.ShowDialog();
        }



        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {

        }
        private TreeNode m_OldSelectNode;
        private void treeView1_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            Point p = new Point(e.X, e.Y);
            // Show menu only if the right mouse button is clicked.
            if (e.Button == MouseButtons.Right)
            {

                // Point where the mouse is clicked.


                // Get the node that the user has clicked.
                TreeNode node = treeView1.GetNodeAt(p);
                mySelectedNode = node;
                if (node != null)
                {

                    // Select the node the user has clicked.
                    // The node appears selected until the menu is displayed on the screen.
                    m_OldSelectNode = treeView1.SelectedNode;

                    treeView1.SelectedNode = node;

                    string str = node.GetType().Name;

                    switch (node.GetType().Name)
                    {
                        case "EquClass":
                            equClassNew = (EquClass)node;
                            mnuTextFile.Show(treeView1, p);

                            break;
                        case "EquLayout": // new picket
                            equLayoutNew = (EquLayout)node;
                            equPathNew = (EquPath)equLayoutNew.Parent;
                            equLineNew = (EquLine)equPathNew.Parent;
                            equGroupNew = (EquGroup)equLineNew.Parent;
                            equClassNew = (EquClass)equGroupNew.Parent;
                            contextMenuStrip_Station.Show(treeView1, p);
                            break;

                        case "EquPath": // new Peregon
                            equPathNew = (EquPath)node;
                            equLineNew = (EquLine)equPathNew.Parent;
                            equGroupNew = (EquGroup)equLineNew.Parent;
                            equClassNew = (EquClass)equGroupNew.Parent;
                            contextMenuStrip_Path.Show(treeView1, p);

                            break;
                        case "EquLine": // new Path
                            equLineNew = (EquLine)node;
                            equGroupNew = (EquGroup)equLineNew.Parent;
                            equClassNew = (EquClass)equGroupNew.Parent;
                            contextMenuStrip_Line.Show(treeView1, p);

                            break;
                        case "EquGroup": // new Line
                            equGroupNew = (EquGroup)node;
                            equClassNew = (EquClass)equGroupNew.Parent;
                            contextMenuStrip_Group.Show(treeView1, p);


                            break;
                        case "Picket":
                            equPicketNew = (Picket)node;
                            equLayoutNew = (EquLayout)equPicketNew.Parent;
                            equPathNew = (EquPath)equLayoutNew.Parent;
                            equLineNew = (EquLine)equPathNew.Parent;
                            equGroupNew = (EquGroup)equLineNew.Parent;
                            equClassNew = (EquClass)equGroupNew.Parent;
                            contextMenuStrip1.Show(treeView1, p);

                            break;
                        case "EquTreeNode":
                            objNodeNew = (EquTreeNode)node;
                            equObjMew = (EquObject)objNodeNew.UserObject;
                            equPicketNew = (Picket)objNodeNew.Parent;
                            equLayoutNew = (EquLayout)equPicketNew.Parent;
                            equPathNew = (EquPath)equLayoutNew.Parent;
                            equLineNew = (EquLine)equPathNew.Parent;
                            equGroupNew = (EquGroup)equLineNew.Parent;
                            equClassNew = (EquClass)equGroupNew.Parent;
                            contextMenuStrip_Equipment.Show(treeView1, p);

                            //contextMenuStrip2.contextMenuStrip2.Show(treeView1, p);
                            break;
                    }

                    // Highlight the selected node.
                    treeView1.SelectedNode = m_OldSelectNode;
                    m_OldSelectNode = null;
                }
                else {
                    contextMenuStrip3.Show(treeView1, p);
                }
            }
        }

        private void mnuNewFile_Click(object sender, EventArgs e) //----- CLASS --------
        {
            equClassNew = new EquClass();
            form_NewGruop = new AddNewGruop(dbHelper, new MyDelegate(func), "Class");
            form_NewGruop.Class(ref equClassNew);
            form_NewGruop.Show();
        }
        private void addNewGroupToolStripMenuItem_Click(object sender, EventArgs e) // ---- GROUP ---------------
        {
            form_NewGruop = new AddNewGruop(dbHelper, new MyDelegate(func), "Group");
            form_NewGruop.Group(ref equGroupNew, ref equClassNew);
            form_NewGruop.Show();
        }

        private void addNewLineToolStripMenuItem_Click(object sender, EventArgs e) // ---- LINE ---------------
        {
            form_NewGruop = new AddNewGruop(dbHelper, new MyDelegate(func), "Line");
            form_NewGruop.Line(ref equLineNew, ref equGroupNew, ref equClassNew);
            form_NewGruop.Show();

        }

        private void addNewPathToolStripMenuItem_Click(object sender, EventArgs e) // ---- PATH ---------------
        {
            form_NewGruop = new AddNewGruop(dbHelper, new MyDelegate(func), "Track");
            form_NewGruop.Path(ref equPathNew, ref equLineNew, ref equGroupNew, ref equClassNew);
            form_NewGruop.Show();
        }

        private void addNewStationToolStripMenuItem_Click(object sender, EventArgs e) // ---- PEREGON ---------------
        {
            form1 = new AddNewElementToDatabase(new MyDelegate1(func1), "Peregon", dbHelper, equClassNew, equGroupNew, equLineNew, equPathNew, null, null, null);
            form1.Show();
        }
        private void добавитьПикетToolStripMenuItem_Click(object sender, EventArgs e) // ---- PICKET ---------------
        {
            form1 = new AddNewElementToDatabase(new MyDelegate1(func1), "Picket", dbHelper, equClassNew, equGroupNew, equLineNew, equPathNew, equLayoutNew, null, null);
            form1.Show();
        }
        private void addNewEquipmentToolStripMenuItem_Click(object sender, EventArgs e)
        {
            form_newEquip = new Equipment.addNewEquipment(dbHelper, new MyDelegate1(func1), equGroupNew, equLineNew, equClassNew, equLayoutNew, equPicketNew, equPathNew);
            form_newEquip.Show();
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            int i = 0;
        }
        void func(int code, string newGroupName, string key)
        {
            switch (key)
            {
                case "Class":
                    treeView1.Nodes.Add(new EquClass(code,newGroupName));
                    treeView1.Refresh();
                    dbHelper.dataTable_AllEquipment.Clear();
                    dbHelper.TblAdapter_AllEquipment.Fill(dbHelper.dataTable_AllEquipment);
                    dbHelper.dataTable_Class.Clear();
                    dbHelper.TblAdapter_Class.Fill(dbHelper.dataTable_Class);
                    break;
                case "Group":
                    equClassNew.Nodes.Add(new EquLine(code, newGroupName));
                    treeView1.Refresh();
                    dbHelper.dataTable_AllEquipment.Clear();
                    dbHelper.TblAdapter_AllEquipment.Fill(dbHelper.dataTable_AllEquipment);
                    dbHelper.dataTable_GroupTable.Clear();
                    dbHelper.TblAdapter_Group.Fill(dbHelper.dataTable_GroupTable);
                    break;
                case "Line":
                    equGroupNew.Nodes.Add(new EquLine(code, "Линия " + newGroupName));
                    treeView1.Refresh();
                    dbHelper.dataTable_AllEquipment.Clear();
                    dbHelper.TblAdapter_AllEquipment.Fill(dbHelper.dataTable_AllEquipment);
                    dbHelper.dataTable_LayoutTable.Clear();
                    dbHelper.TblAdapter_Layout.Fill(dbHelper.dataTable_LayoutTable);
                    break;
                case "Track":
                    equLineNew.Nodes.Add(new EquPath(code,"Путь" + newGroupName));
                    treeView1.Refresh();
                    dbHelper.dataTable_AllEquipment.Clear();
                    dbHelper.TblAdapter_AllEquipment.Fill(dbHelper.dataTable_AllEquipment);
                    break;
                case "Peregon":
                    break;
                case "Pickets":
                    break;
                case "Objects":
                    break;
            }
        }
        void func1(int code, string name, string tag)
        {
            switch (tag)
            {
                case "Peregon":
                    EquLayout layout = new EquLayout(code,name);
                    calcPeregon(ref equPathNew, equLineNew.Code, ref layout);
                    dbHelper.dataTable_AllEquipment.Clear();
                    dbHelper.TblAdapter_AllEquipment.Fill(dbHelper.dataTable_AllEquipment);
                    treeView1.Update();
                    break;
                case "Picket":
                    Picket EquPicket = new Picket(code,"Пикет " + name);
                    calcPicket(ref equLayoutNew, equLayoutNew.Code, ref EquPicket);
                    dbHelper.dataTable_AllEquipment.Clear();
                    dbHelper.TblAdapter_AllEquipment.Fill(dbHelper.dataTable_AllEquipment);
                    treeView1.Update();
                    break;
                case "Obj":
                    EquObject obj = new EquObject(  code,
                                                    name,
                                                    equGroupNew,
                                                    equLayoutNew,
                                                    equPathNew.Code,
                                                    0,
                                                    0);

                    EquTreeNode objNode = new EquTreeNode(String.Concat(new object[] { equGroupNew.Name, " ", obj.Name }));
                    objNode.UserObject = obj;
                    equPicketNew.Nodes.Add(objNode);

                    dbHelper.dataTable_AllEquipment.Clear();
                    dbHelper.TblAdapter_AllEquipment.Fill(dbHelper.dataTable_AllEquipment);

                    treeView1.Update();

                    break;
            }
        }
        private void удалитьОборудованиеИзБазыДанныхToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }


        private void удалитьОборудованиеИзПикетаToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void переименоватьToolStripMenuItem_Click(object sender, EventArgs e)   // equipment
        {

        }

      

        private void удалитьПикетИзБДToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void переименоватьToolStripMenuItem1_Click(object sender, EventArgs e) // Picket
        {

        }

      

       

        private void переименоватьToolStripMenuItem2_Click(object sender, EventArgs e)  // Peregon
        {

        }

        

       

        private void переименоватьToolStripMenuItem3_Click(object sender, EventArgs e) // Line
        {

        }

       

      

        private void обновитьСодержимоеБазыДанныхToolStripMenuItem_Click(object sender, EventArgs e)// Class
        {
            TreeNode sn = treeView1.SelectedNode.Parent;
            treeView1.Nodes.Clear();
            InitTree();
            treeView1.SelectedNode = sn;
        }
      

        

        private void удалитьПикетИзПерегонаToolStripMenuItem1_Click(object sender, EventArgs e) // удалить оборудование из пикета
        {

            var empData = from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.ClassNum == equClassNew.Code && r.GroupNum == equGroupNew.Code && r.LineNum == equLineNew.Code && r.Track == equPathNew.Code && r.Layout == equLayoutNew.Code && r.Npicket == equPicketNew.Code /*&& r.Code == equObjMew.Code */select new { r.Code };

            int cnt = empData.Count();
            int res = dbHelper.TblAdapter_AllEquipment.delEquip(equPicketNew.Code, equObjMew.Code, cnt);

            var emp1 = from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Object == equObjMew.Name select new { r.Code };

            if (emp1.Count() > 1)
                dbHelper.TblAdapter_Objects.DeleteQueryByCode(equObjMew.Code);

            if (res == -1)
            {
                TreeNode sn = treeView1.SelectedNode;
                treeView1.Nodes.Remove(sn);
                treeView1.Update();
            }
        }
 

        private void удалитьОборудованиеИзБазыДанныхToolStripMenuItem1_Click(object sender, EventArgs e) // удалить оборудование из БД
        {

            var empData = from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.ObjName == equObjMew.Name select new { r.Npicket };
            
            foreach(var item in empData)
            {
                var empDataPicket = from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.Npicket == item.Npicket select new {r.Code}; // получаем количество оборудования в пикете
                var empDataPicketName = from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.Npicket == item.Npicket && r.ObjName == equObjMew.Name  select new {r.Code};

                dbHelper.TblAdapter_Objects.DeleteEquipByName(equObjMew.Name);

                if(empDataPicket.Count() > empDataPicketName.Count())
                {
                    foreach (var itemEquipCode in empDataPicketName)
                        dbHelper.TblAdapter_AllEquipment.delEquip(itemEquipCode.Code, item.Npicket, 0);
                }
                else
                {
                    int i = 0;
                    foreach (var itemEquipCode in empDataPicketName)
                    {
                        
                        if (i == empDataPicketName.Count() - 1) 
                            break;
                        i++;
                        dbHelper.TblAdapter_AllEquipment.delEquip(itemEquipCode.Code, item.Npicket, 0);
                        
                    }
                    dbHelper.TblAdapter_AllEquipment.delEquip(empDataPicketName.Last().Code, item.Npicket, 1);
                }
            }

            //int res = dbHelper.TblAdapter_AllEquipment.delEquipFromDB(equObjMew.Code);
            dbHelper.refresh();
            //if (res == -1)
            //{
                TreeNode sn = treeView1.SelectedNode;
                treeView1.Nodes.Clear();
                InitTree();
            //}

        }
        private void переименоватьToolStripMenuItem6_Click(object sender, EventArgs e)
        {

        }
        

        private void добавитьГруппуToolStripMenuItem_Click(object sender, EventArgs e) // добавить группу (меню класса)
        {
            form_NewGruop = new AddNewGruop(dbHelper, new MyDelegate(func), "Group");
            form_NewGruop.Group(ref equGroupNew, ref equClassNew);
            form_NewGruop.Show();
        }

        private void добавитьЛиниюToolStripMenuItem_Click(object sender, EventArgs e) // добавить линию (меню группы)
        {
            form_NewGruop = new AddNewGruop(dbHelper, new MyDelegate(func), "Line");
            form_NewGruop.Line(ref equLineNew, ref equGroupNew, ref equClassNew);
            form_NewGruop.Show();
        }

        private void добавитьПутьToolStripMenuItem_Click(object sender, EventArgs e) // добавить путь (меню линии)
        {
            form_NewGruop = new AddNewGruop(dbHelper, new MyDelegate(func), "Track");
            form_NewGruop.Path(ref equPathNew, ref equLineNew, ref equGroupNew, ref equClassNew);
            form_NewGruop.Show();
        }

        private void добавитьПерегонстанциюToolStripMenuItem_Click(object sender, EventArgs e) // добавить перегон (меню пути)
        {
            form1 = new AddNewElementToDatabase(new MyDelegate1(func1), "Peregon", dbHelper, equClassNew, equGroupNew, equLineNew, equPathNew, null, null, null);
            form1.Show();
        }

        private void добавитьПикетToolStripMenuItem1_Click(object sender, EventArgs e) // добавление пикета(меню станций)
        {
            form1 = new AddNewElementToDatabase(new MyDelegate1(func1), "Picket", dbHelper, equClassNew, equGroupNew, equLineNew, equPathNew, equLayoutNew, null, null);
            form1.Show();
        }

        private void добавитьОбъекьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var empData1 = (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.Npicket == equPicketNew.Code select new { r.ClassNum, r.GroupNum, r.LineNum, r.Track, r.Layout, r.Npicket }).Distinct();

            foreach (var item in empData1)
            {
                var emp = (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.ClassNum == item.ClassNum && r.GroupNum == item.GroupNum && r.Track == item.Track && r.Layout == item.Layout select new { r.Npicket });

                int res = 0;
                int cnt = emp.Count();
                res = dbHelper.TblAdapter_AllEquipment.delPicket(equClassNew.Code, equGroupNew.Code, equLineNew.Code, equPathNew.Code, equLayoutNew.Code, equPicketNew.Code, cnt);
            }

            dbHelper.TblAdapter_Pickets.delPicketFromDB(equPicketNew.Code);

            dbHelper.refresh();
            treeView1.Nodes.Clear();
            InitTree();
        }

        private void обновитьToolStripMenuItem_Click(object sender, EventArgs e)
        {

            dbHelper.refresh();
            treeView1.Nodes.Clear();
            InitTree();
        }

        private void удалитьПикетИзБазыДанныхToolStripMenuItem_Click(object sender, EventArgs e) // удалить пикет из базы данных
        {
            form_newEquip = new Equipment.addNewEquipment(dbHelper, new MyDelegate1(func1), equGroupNew, equLineNew, equClassNew, equLayoutNew, equPicketNew, equPathNew);
            form_newEquip.Show();

        }
        private void удалитьПикетИзПерегонаToolStripMenuItem_Click(object sender, EventArgs e)  // удалить пикет из перегона
        {
            var empData = (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.ClassNum == equClassNew.Code && r.GroupNum == equGroupNew.Code && r.LineNum == equLineNew.Code && r.Track == equPathNew.Code && r.Layout == equLayoutNew.Code select new { r.Npicket }).Distinct();

            int cnt = empData.Count();
            int res = dbHelper.TblAdapter_AllEquipment.delPicket(equClassNew.Code, equGroupNew.Code, equLineNew.Code, equPathNew.Code, equLayoutNew.Code, equPicketNew.Code, cnt);

            dbHelper.dataTable_AllEquipment.Clear();
            dbHelper.TblAdapter_AllEquipment.Fill(dbHelper.dataTable_AllEquipment);

            if (res == -1)
            {
                TreeNode sn = treeView1.SelectedNode;
                treeView1.Nodes.Remove(sn);
                treeView1.Update();
            }

            
        }
        private void удалитьToolStripMenuItem_Click(object sender, EventArgs e)// удалить перегон из текущего пути
        {

            var empData = (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.ClassNum == equClassNew.Code && r.GroupNum == equGroupNew.Code && r.LineNum == equLineNew.Code && r.Track == equPathNew.Code  select new { r.Layout }).Distinct();

            int cnt = empData.Count();
            int res = dbHelper.TblAdapter_AllEquipment.delPeregon(equClassNew.Code, equGroupNew.Code, equLineNew.Code, equPathNew.Code, equLayoutNew.Code, cnt);

            dbHelper.dataTable_AllEquipment.Clear();
            dbHelper.TblAdapter_AllEquipment.Fill(dbHelper.dataTable_AllEquipment);

            if (res == -1)
            {
                TreeNode sn = treeView1.SelectedNode;
                treeView1.Nodes.Remove(sn);
                treeView1.Update();
            }


        }
        private void удалитьИзБазыДанныхToolStripMenuItem_Click(object sender, EventArgs e) // удалить перегон из БД
        {
            var empData1 = (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.Layout== equLayoutNew.Code select new { r.ClassNum, r.GroupNum, r.LineNum, r.Track, r.Layout }).Distinct();

            foreach (var item in empData1)
            {
                var emp = (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.ClassNum == item.ClassNum && r.GroupNum == item.GroupNum && r.Track == item.Track select new { r.Layout }).Distinct();

                int res = 0;
                int cnt = emp.Count();
                res = dbHelper.TblAdapter_AllEquipment.delPeregon(equClassNew.Code, equGroupNew.Code, equLineNew.Code, equPathNew.Code, equLayoutNew.Code, cnt);
            }

            dbHelper.TblAdapter_Layout.delPeregonFromDB(equLayoutNew.Code);

            //dbHelper.dataTable_AllEquipment.Clear();
            //dbHelper.TblAdapter_AllEquipment.Fill(dbHelper.dataTable_AllEquipment);

            dbHelper.refresh();
            treeView1.Nodes.Clear();
            InitTree();
        }
        
        private void удалитьПутьИзЛинииToolStripMenuItem_Click(object sender, EventArgs e) // удалить пути из выбранной линии
        {
            var empData1 = (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.ClassNum == equClassNew.Code && r.GroupNum == equGroupNew.Code && r.LineNum == equLineNew.Code select new { r.Track }).Distinct();
            dbHelper.TblAdapter_AllEquipment.delPath(equClassNew.Code, equGroupNew.Code, equLineNew.Code, equPathNew.Code, empData1.Count());

            dbHelper.dataTable_AllEquipment.Clear();
            dbHelper.TblAdapter_AllEquipment.Fill(dbHelper.dataTable_AllEquipment);

            TreeNode sn = treeView1.SelectedNode;
            treeView1.Nodes.Remove(sn);
            treeView1.Update();

        }
        private void удалитьЛиниюИзГруппыToolStripMenuItem_Click(object sender, EventArgs e) // удалить линию из группы
        {
            var empData1 = (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.ClassNum == equClassNew.Code && r.GroupNum == equGroupNew.Code && r.LineNum == equLineNew.Code select new { r.Track }).Distinct();
            dbHelper.TblAdapter_AllEquipment.delLine(equClassNew.Code, equGroupNew.Code, equLineNew.Code, empData1.Count());

            dbHelper.dataTable_AllEquipment.Clear();
            dbHelper.TblAdapter_AllEquipment.Fill(dbHelper.dataTable_AllEquipment);

            TreeNode sn = treeView1.SelectedNode;
            treeView1.Nodes.Remove(sn);
            treeView1.Update();


        }
        private void удалитьЛиниюИзБазыДанныхToolStripMenuItem_Click(object sender, EventArgs e) //  удалить линию из БД
        {
            var empData1 = (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.LineNum == equLineNew.Code select new {r.GroupNum }).Distinct();

            foreach (var item in empData1)
            {
                var emp = (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.GroupNum == item.GroupNum select new { r.LineNum }).Distinct();

                int res = 0;
                int cnt = emp.Count();
                res = dbHelper.TblAdapter_AllEquipment.delLine(equClassNew.Code, equGroupNew.Code, equLineNew.Code, cnt);
            }

            dbHelper.TblAdapter_AllEquipment.delLineFromDB(equLineNew.Code);


            dbHelper.refresh();
            treeView1.Nodes.Clear();
            InitTree();
        }
        
        private void удалитьГруппуИзКлассаToolStripMenuItem_Click(object sender, EventArgs e) // удалить группу
        {
            var empData1 = (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.ClassNum == equClassNew.Code select new { r.GroupNum }).Distinct();
            dbHelper.TblAdapter_AllEquipment.delGroup(equClassNew.Code, equGroupNew.Code, empData1.Count());

            TreeNode sn = treeView1.SelectedNode;
            treeView1.Nodes.Remove(sn);
            treeView1.Update();
        }
        
        private void refreshToolStripMenuItem_Click(object sender, EventArgs e) // удалить класс
        {
            dbHelper.TblAdapter_AllEquipment.delClass(equClassNew.Code);
            
            TreeNode sn = treeView1.SelectedNode;
            treeView1.Nodes.Remove(sn);
            treeView1.Update();
        }
        private void переименоватьToolStripMenuItem5_Click(object sender, EventArgs e) // переименовать класс
        {
            renameWrapper();
        }
        private void переименоватьToolStripMenuItem4_Click(object sender, EventArgs e) // group rename
        {
            renameWrapper();
        }
        private void переименоватьToolStripMenuItem7_Click(object sender, EventArgs e) // переименовать путь
        {
            renameWrapper();
        }
        public void renameWrapper()
        {
            if (mySelectedNode != null /*&& mySelectedNode.Parent != null*/)
            {
                treeView1.SelectedNode = mySelectedNode;
                treeView1.LabelEdit = true;

                if (!mySelectedNode.IsEditing)
                    mySelectedNode.BeginEdit();
            }
            else
                MessageBox.Show("Для переименования необходимо выбрать объект");
        }


        TreeNode mySelectedNode=null;
        
        private void treeView1_AfterLabelEdit(object sender, System.Windows.Forms.NodeLabelEditEventArgs e)
        {
            if (e.Label != null)
            {
                if (e.Label.Length > 0)
                {
                    if (e.Label.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*' }) == -1)
                    {
                        switch( mySelectedNode.GetType().Name)
                        {
                            case "EquClass":
                                //EquClass classTmp = (EquClass)mySelectedNode;
                                dbHelper.TblAdapter_Class.renameClass(e.Label, equClassNew.Code);
                                e.Node.EndEdit(false);
                                break;

                            case "EquGroup": // new picket
                                //EquGroup groupTmp = (EquGroup)mySelectedNode;
                                dbHelper.TblAdapter_Group.renameGroup(e.Label, (short)equGroupNew.Code);
                                e.Node.EndEdit(false);
                                break;

                            case "EquPath":
                                int trackNum=0;
                                 if(int.TryParse(e.Label,out trackNum))
                                 {
                                     dbHelper.TblAdapter_AllEquipment.renameTrack(equPicketNew.Code, equClassNew.Code, equGroupNew.Code, equLineNew.Code);
                                     e.Node.EndEdit(false);
                                 }
                                 else 
                                 {
                                     MessageBox.Show("Для переименования необходимо ввести только число");
                                     return;
                                 }

                                break;
                        }
                    }
                    else
                    {
                        /* Cancel the label edit action, inform the user, and 
                           place the node in edit mode again. */
                        e.CancelEdit = true;
                        MessageBox.Show("В названии присутствует некорректный символ: '@','.', ',', '!', ... ",
                           "Переименование");
                        e.Node.BeginEdit();
                    }
                }
                else
                {
                    /* Cancel the label edit action, inform the user, and 
                       place the node in edit mode again. */
                    e.CancelEdit = true;
                    MessageBox.Show("Название не должно быть пустым",
                       "Переименование");
                    e.Node.BeginEdit();
                }
            }
        }


        private void добавитьКлассToolStripMenuItem_Click(object sender, EventArgs e)
        {
            equClassNew = new EquClass();
            form_NewGruop = new AddNewGruop(dbHelper, new MyDelegate(func), "Class");
            form_NewGruop.Class(ref equClassNew);
            form_NewGruop.Show();
        }
    }
}
