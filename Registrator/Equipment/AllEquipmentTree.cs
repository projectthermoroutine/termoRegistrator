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
        private Equipment.AddTrack form_Track;
        private Equipment.AddLine form_line;
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
            treeView1.AfterLabelEdit += treeView1_AfterLabelEdit;

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
                                if (((EquLayout)curPathArg.Nodes[i1]).Code == peregonCode[addPicketIndex - y])
                                {
                                    curPathArg.Nodes.Insert(i1 + 1, curLayout);
                                    end = 1;
                                    break;
                                }
                            }
                            if (end == 1) break;

                        }
                        if (end != 1)
                            curPathArg.Nodes.Insert(0, curLayout);
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
                int indexTmp = 0;
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
                        curLayoutArg.Nodes.Add(PicketObj); // curLayoutArg.Nodes.Insert((picketCode.Count - 1), PicketObj);
                    else
                    {
                        int tmp = 0;

                        foreach (var item in curLayoutArg.Nodes)
                        {
                            if (picketCode.Contains(((Picket)item).Code))
                            {
                                tmp = picketCode.IndexOf(((Picket)item).Code);
                                if (tmp > addPicketIndex)
                                {
                                    if (indexTmp == 0) indexTmp = tmp;
                                    if (tmp < indexTmp) indexTmp = tmp;
                                }
                            }
                        }
                        if (indexTmp != 0)
                            curLayoutArg.Nodes.Insert(indexTmp, PicketObj);
                        else
                            curLayoutArg.Nodes.Add(PicketObj);
                    }
                }
            }
            else
                curLayoutArg.Nodes.Add(PicketObj);

        }
        private void InitTree()
        {
            int count = dbHelper.dataTable_AllEquipment.Rows.Count;

            EquLayout curLayout = null;
            EquGroup curGroup = null;
            EquClass curClass = null;
            EquPath curPath = null;
            EquLine curLine = null;
            Picket PicketObj = null;

            picketCode = new List<int>();
            peregonCode = new List<int>();

            List<int> peregons = new List<int>();
            List<int> pickets = new List<int>();

            var resRoot = (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.ClassNum != 0 select new { r.ClassNum, r.ClsName }).Distinct();

            foreach (var itemRoot in resRoot)
            {
                curClass = new EquClass(Convert.ToInt32(itemRoot.ClassNum), (String)itemRoot.ClsName);
                treeView1.Nodes.Add(curClass);
                curLine = null;
                curPath = null;
                curLayout = null;
                PicketObj = null;

                var resClass = (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.ClassNum == curClass.Code && r.GroupNum != 0 select new { r.GroupNum, r.GrpName }).Distinct();

                foreach (var itemClass in resClass)
                {
                    curGroup = new EquGroup(Convert.ToInt32(itemClass.GroupNum), (String)itemClass.GrpName);
                    curClass.Nodes.Add(curGroup);

                    var resGroup = (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.ClassNum == curClass.Code && r.GroupNum == curGroup.Code && r.LineNum != 0 select new { r.LineNum, r.LineName }).Distinct();

                    foreach (var itemGroup in resGroup)
                    {
                        curLine = new EquLine(Convert.ToInt32(itemGroup.LineNum), String.Concat(new object[] { "Линия ", Convert.ToString(Convert.ToInt32(itemGroup.LineNum)), " - ",Convert.ToString(itemGroup.LineName) }));
                        curGroup.Nodes.Add(curLine);

                        var resTrack = (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.ClassNum == curClass.Code && r.GroupNum == curGroup.Code && r.LineNum == curLine.Code && r.Track != 0 select new { r.Track }).Distinct();

                        foreach (var itemTrack in resTrack)
                        {
                            curPath = new EquPath(Convert.ToInt32(itemTrack.Track), String.Concat(new object[] { "Путь ", Convert.ToString(itemTrack.Track) }));
                            curLine.Nodes.Add(curPath);

                            var resPeregon = (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.ClassNum == curClass.Code && r.GroupNum == curGroup.Code && r.LineNum == curLine.Code && r.Track == curPath.Code && r.Layout != 0 select new { r.Layout, r.LtName, r.NperegonBefore, r.NperegonAfter }).Distinct();

                            foreach (var itemPeregon in resPeregon)
                            {
                                curLayout = new EquLayout(itemPeregon.Layout, itemPeregon.LtName);
                                calcPeregon(ref curPath, curLine.Code, ref curLayout);
                                curLayout.beforePeregon = Convert.ToInt32(itemPeregon.NperegonBefore);
                                curLayout.aftrerPeregon = Convert.ToInt32(itemPeregon.NperegonAfter);
                                peregons.Add(curLayout.Code);

                                var res4 = (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.ClassNum == curClass.Code && r.GroupNum == curGroup.Code && r.LineNum == curLine.Code && r.Track == curPath.Code && r.Layout == curLayout.Code && r.Npicket != 0 select new { r.Npicket, r.NpicketBefore, r.NpicketAfter }).Distinct();

                                foreach (var item in res4)
                                {

                                    PicketObj = new Picket(Convert.ToInt32(item.Npicket), String.Concat(new object[] { "Пикет ", Convert.ToString(item.Npicket) }));
                                    PicketObj.before = Convert.ToInt32(item.Npicket);
                                    PicketObj.after = Convert.ToInt32(item.Npicket);

                                    calcPicket(ref curLayout, curLayout.Code, ref PicketObj);


                                    var res5 = (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.ClassNum == curClass.Code && r.GroupNum == curGroup.Code && r.LineNum == curLine.Code && r.Track == curPath.Code && r.Layout == curLayout.Code && r.Npicket == PicketObj.Code && r.Code != 0 select new { r.Code, r.ObjName,r.typeId }).Distinct();

                                    foreach (var itemEquip in res5)
                                    {
                                        EquObject obj = new EquObject(
                                                            Convert.ToInt32(itemEquip.Code),
                                                            (String)itemEquip.ObjName,
                                                            curGroup,
                                                            curLayout,
                                                            curPath.Code,
                                                            0,
                                                            0
                                                        );
                                        obj.typeEquip = itemEquip.typeId;
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

                objNodeNew = (EquTreeNode)treeView1.SelectedNode;
                equObjMew = (EquObject)objNodeNew.UserObject;
                equPicketNew = (Picket)objNodeNew.Parent;
                equLayoutNew = (EquLayout)equPicketNew.Parent;
                equPathNew = (EquPath)equLayoutNew.Parent;
                equLineNew = (EquLine)equPathNew.Parent;
                equGroupNew = (EquGroup)equLineNew.Parent;
                equClassNew = (EquClass)equGroupNew.Parent;

                //Eq rn = treeView1.SelectedNode
                elObj.Picket = equPicketNew.Code;
                elObj.Group.Class = equClassNew;
                elObj.Offset = item.shiftFromPicket;
                elObj.State = (byte)item.regularly;

                EquElementForm eqf = new EquElementForm(elObj, ref dbHelper);
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
                else
                {
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
            form_line = new Equipment.AddLine(dbHelper, new MyDelegate(func), "Line");
            form_line.Line(ref equLineNew, ref equGroupNew, ref equClassNew);
            form_line.Show();
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
                    treeView1.Nodes.Add(new EquClass(code, newGroupName));
                    treeView1.Refresh();
                    dbHelper.dataTable_AllEquipment.Clear();
                    dbHelper.TblAdapter_AllEquipment.Fill(dbHelper.dataTable_AllEquipment);
                    dbHelper.dataTable_Class.Clear();
                    dbHelper.TblAdapter_Class.Fill(dbHelper.dataTable_Class);
                    break;
                case "Group":
                    equClassNew.Nodes.Add(new EquGroup(code, newGroupName));
                    treeView1.Refresh();
                    dbHelper.dataTable_AllEquipment.Clear();
                    dbHelper.TblAdapter_AllEquipment.Fill(dbHelper.dataTable_AllEquipment);
                    dbHelper.dataTable_GroupTable.Clear();
                    dbHelper.TblAdapter_Group.Fill(dbHelper.dataTable_GroupTable);
                    break;
                case "Line":
                    equGroupNew.Nodes.Add(new EquLine(code, "Линия " + Convert.ToString(code) +" - " + newGroupName));
                    treeView1.Refresh();
                    dbHelper.dataTable_AllEquipment.Clear();
                    dbHelper.TblAdapter_AllEquipment.Fill(dbHelper.dataTable_AllEquipment);
                    dbHelper.dataTable_LayoutTable.Clear();
                    dbHelper.TblAdapter_Layout.Fill(dbHelper.dataTable_LayoutTable);
                    break;

                case "LineEdit":
                    
                    int ind = equGroupNew.Nodes.IndexOf(equLineNew);
                    equGroupNew.Nodes[ind].Text ="Линия" + Convert.ToString(code) + newGroupName;
                    treeView1.Refresh();
                    break;
                case "Track":
                    equLineNew.Nodes.Add(new EquPath(code, "Путь" + newGroupName));
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
                    EquLayout layout = new EquLayout(code, name);
                    calcPeregon(ref equPathNew, equLineNew.Code, ref layout);
                    dbHelper.dataTable_AllEquipment.Clear();
                    dbHelper.TblAdapter_AllEquipment.Fill(dbHelper.dataTable_AllEquipment);
                    treeView1.Update();
                    break;
                case "Picket":
                    Picket EquPicket = new Picket(code, "Пикет " + name);
                    calcPicket(ref equLayoutNew, equLayoutNew.Code, ref EquPicket);
                    dbHelper.dataTable_AllEquipment.Clear();
                    dbHelper.TblAdapter_AllEquipment.Fill(dbHelper.dataTable_AllEquipment);
                    treeView1.Update();
                    break;
                case "Obj":


                    string[] str = name.Split(';');





                    EquObject obj = new EquObject(code,
                                                    str[0],
                                                    equGroupNew,
                                                    equLayoutNew,
                                                    equPathNew.Code,
                                                    0,
                                                    0);

                    obj.typeEquip = Convert.ToInt32(str[1]);

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

        private void переименоватьToolStripMenuItem3_Click(object sender, EventArgs e) //edit Line
        {
            form_line = new Equipment.AddLine(dbHelper, new MyDelegate(func), "Edit");
            form_line.Line(ref equLineNew, ref equGroupNew, ref equClassNew);
            form_line.Show();
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
            int res = dbHelper.TblAdapter_AllEquipment.delEquip(equGroupNew.Code, equPathNew.Code, equPicketNew.Code, equObjMew.Code, cnt);

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

           // var empData = from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.ObjName == equObjMew.Name select new { r.Npicket, r.GroupNum };


            var resAllGroupForEquip = from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.GroupNum == equGroupNew.Code && r.typeId == equObjMew.typeEquip select new { r.GroupNum };

            foreach(var item in resAllGroupForEquip)
            {
                var resAllTracksInGroup = from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.GroupNum == item.GroupNum && r.typeId == equObjMew.typeEquip select new { r.Track };

                foreach(var itemTrack in resAllTracksInGroup)
                {
                    var resAllPicketInTrack = from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.Track == itemTrack.Track && r.GroupNum == item.GroupNum && r.typeId == equObjMew.typeEquip select new { r.Npicket };

                    foreach(var itemPicket in resAllPicketInTrack)
                    {
                        var resAllSelectedEquipInPicket = from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.Track == itemTrack.Track && r.GroupNum == item.GroupNum && r.Npicket == itemPicket.Npicket && r.typeId == equObjMew.typeEquip select new { r.Code };
                        var resAllEquipInPicket = from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.Track == itemTrack.Track && r.GroupNum == item.GroupNum && r.Npicket == itemPicket.Npicket  select new { r.Code };

                        if(resAllEquipInPicket.Count() > resAllSelectedEquipInPicket.Count())
                        {
                            foreach (var itemEquip in resAllSelectedEquipInPicket)
                                dbHelper.TblAdapter_AllEquipment.delEquip(equGroupNew.Code,equPathNew.Code,itemPicket.Npicket, itemEquip.Code, 0);

                        }
                        else
                        {
                            int i = 0;
                            foreach(var itemEquip in resAllSelectedEquipInPicket)
                            {
                                if (i == resAllSelectedEquipInPicket.Count() - 1)
                                {
                                    dbHelper.TblAdapter_AllEquipment.delEquip(equGroupNew.Code, equPathNew.Code, itemPicket.Npicket, itemEquip.Code, 1);
                                    break;
                                }

                                i++;
                                dbHelper.TblAdapter_AllEquipment.delEquip(equGroupNew.Code, equPathNew.Code, itemPicket.Npicket, itemEquip.Code, 0);
                            }
                        }
                    }
                }

            }



            //foreach (var item in empData)
            //{
            //    var empDataPicket = from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.GroupNum == item.GroupNum && r.Npicket == item.Npicket select new { r.Code }; // получаем количество оборудования в пикете
            //    var empDataPicketName = from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.Npicket == item.Npicket && r.ObjName == equObjMew.Name select new { r.Code };

            //    dbHelper.TblAdapter_Objects.DeleteEquipByName(equObjMew.Name);

            //    if (empDataPicket.Count() > empDataPicketName.Count())
            //    {
            //        foreach (var itemEquipCode in empDataPicketName)
            //            dbHelper.TblAdapter_AllEquipment.delEquip(itemEquipCode.Code, item.Npicket, 0);
            //    }
            //    else
            //    {
            //        int i = 0;
            //        foreach (var itemEquipCode in empDataPicketName)
            //        {

            //            if (i == empDataPicketName.Count() - 1)
            //                break;
            //            i++;
            //            dbHelper.TblAdapter_AllEquipment.delEquip(itemEquipCode.Code, item.Npicket, 0);

            //        }
            //        dbHelper.TblAdapter_AllEquipment.delEquip(empDataPicketName.Last().Code, item.Npicket, 1);
            //    }
            //}

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
            form_line = new Equipment.AddLine(dbHelper, new MyDelegate(func), "Line");
            form_line.Line(ref equLineNew, ref equGroupNew, ref equClassNew);
            form_line.Show();
        }

        private void добавитьПутьToolStripMenuItem_Click(object sender, EventArgs e) // добавить путь (меню линии)
        {
            form_Track = new Equipment.AddTrack(dbHelper, new MyDelegate(func), "Track");
            form_Track.Path(ref equPathNew, ref equLineNew, ref equGroupNew, ref equClassNew);
            form_Track.Show();
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

            var empData = (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.ClassNum == equClassNew.Code && r.GroupNum == equGroupNew.Code && r.LineNum == equLineNew.Code && r.Track == equPathNew.Code select new { r.Layout }).Distinct();

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
            var empData1 = (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.Layout == equLayoutNew.Code select new { r.ClassNum, r.GroupNum, r.LineNum, r.Track, r.Layout }).Distinct();

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
            var empData1 = (from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.LineNum == equLineNew.Code select new { r.GroupNum }).Distinct();

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


        TreeNode mySelectedNode = null;

        private void treeView1_AfterLabelEdit(object sender, System.Windows.Forms.NodeLabelEditEventArgs e)
        {
            if (e.Label != null)
            {
                if (e.Label.Length > 0)
                {
                    if (e.Label.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*' }) == -1)
                    {
                        switch (mySelectedNode.GetType().Name)
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
                                int trackNum = 0;
                                if (int.TryParse(e.Label, out trackNum))
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
