using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;

namespace Registrator
{
    //public delegate void MyDelegate(int code, string data, string key);
    public delegate void AddObjectOnTreeView(int code, string name, string Tag);

    public partial class AllEquipmentTree : ToolWindow
    {
        public DB.metro_db_controller _db_controller;

        //  forms 
        private Equipment.AddClass form_addClass;
        private Equipment.addNewEquipment form_newEquip;
        private Equipment.addStrelka form_Strelka;
        private AddNewElementToDatabase form1;
        private AddNewGruop form_NewGruop;
        private Equipment.AddTrack form_Track;
        private Equipment.AddLine form_line;
        private Equipment.Properties form_properties;
        // database objects
        private EquClass equClassNew;
        private EquGroup equGroupNew;
        private EquLine equLineNew;
        private EquPath equPathNew;
        private EquLayout equLayoutNew;
        private Picket equPicketNew;
        private EquTreeNode objNodeNew;
        private EquObject equObjMew;
        private DockPanel DPanel;


        List<int> picketCode;
        List<int> peregonCode;

        public AllEquipmentTree(DB.metro_db_controller db_controller, DockPanel DockPanel_Arg)
        {
            InitializeComponent();
            
            _db_controller = new DB.metro_db_controller(db_controller);
            form_properties = new Equipment.Properties(_db_controller);
            InitTree();
            //treeView1.MouseDown += treeView1_MouseDown;
            treeView1.AfterLabelEdit += treeView1_AfterLabelEdit;

            // this.refreshToolStripMenuItem.Click += new System.EventHandler(this.refreshToolStripMenuItem_Click);
            // this.mnuNewFile.Click += new System.EventHandler(this.mnuNewFile_Click);
            this.treeView1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.treeView1_MouseUp);

            DPanel = DockPanel_Arg;

            form_properties.groupSettings.RenameEventHandler += peregonSettings_RenamePeregonEventHandler;
            form_properties.lineSettings.RenameEventHandler += peregonSettings_RenamePeregonEventHandler;
            form_properties.equipSettings.RenameEventHandler += peregonSettings_RenamePeregonEventHandler;
            form_properties.equipExtSettings.RenameEventHandler += peregonSettings_RenamePeregonEventHandler;
            form_properties.peregonSettings.RenamePeregonEventHandler += peregonSettings_RenamePeregonEventHandler;
            form_properties.classSettings.RenameEventHandler += peregonSettings_RenamePeregonEventHandler;
            form_properties.strelkaSettings.RenameEventHandler += peregonSettings_RenamePeregonEventHandler;
            form_properties.pathSettings.RenameEventHandler += peregonSettings_RenamePeregonEventHandler;

        }
        private int lastLine=0;

        public void updatePeregon(ref EquPath curPathArg, int curLineCode, ref EquLayout curLayout)
        {
            if (curPathArg.Nodes.Count != 0)
            {
                int indexTmp = 0;
                
                if (curLineCode != lastLine)
                {
                    // create database logical list of peregon
                    lastLine = curLineCode;
                    var empData = from r in _db_controller.layout_table.AsEnumerable() where r.Line == curLineCode select new { r.Code, r.NperegonBefore, r.NperegonAfter };

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
                }
                // create current logical list of peregons
                int addPicketIndex = peregonCode.IndexOf(curLayout.Code);// индекс добавляемого перегона   
                // create current logical list of peregons

                if (addPicketIndex == 0) // если добавляемый пикет первый в перегоне
                    curPathArg.Nodes.Insert(0, curLayout);
                else
                {
                    int tmp = 0;
                    foreach (var item in curPathArg.Nodes)
                    {
                        if (peregonCode.Contains(((EquLayout)item).Code))
                        {
                            tmp = peregonCode.IndexOf(((EquLayout)item).Code);
                            if (tmp > addPicketIndex)
                                break;
                            else
                                indexTmp++;
                        }
                    }
                    if (indexTmp != 0)
                    {

                        curPathArg.Nodes.Insert(indexTmp, curLayout);
                    }
                    else
                        curPathArg.Nodes.Add(curLayout);
                }
            }
            else
                curPathArg.Nodes.Add(curLayout);
        }
        public void updatePicket(ref EquLayout curPathArg, int curPeregonCode, ref Picket PicketObj)
        {
            if (curPathArg.Nodes.Count != 0)
            {
                //int indexTmp = 0;
                if (lastPicket != curPeregonCode)
                {
                    var empData = from r in _db_controller.pickets_table.AsEnumerable() where r.Peregon == curPeregonCode && r.Npiketa != 0 orderby r.Npiketa select new { r.Npiketa, r.Peregon, r.NpicketBefore, r.NpicketAfter,r.number };
                    lastPicket = curPeregonCode;
                    picketCode.Clear();

                    int whileIndex = 0;
                    var val1 = empData.First();

                    while (true)
                    {
                        whileIndex = 0;
                        foreach (var item in empData)
                        {
                            if (val1.number == item.NpicketAfter)
                            {
                                val1 = item;
                                whileIndex = 1;
                            }
                        }
                        if (val1.NpicketBefore == 0 || whileIndex == 0) break;
                    }
                    picketCode.Add(val1.number);

                    while (true)
                    {
                        whileIndex = 0;
                        foreach (var item in empData)
                        {
                            if (val1.number == item.NpicketBefore)
                            {
                                val1 = item;
                                picketCode.Add(val1.number);
                                whileIndex = 1;
                            }
                        }
                        if (whileIndex == 0) break;
                    }
                }
                int addPicketIndex = picketCode.IndexOf(PicketObj.Code);// индекс добавляемого пикета


                int indexTmp = 0;

                // create current logical list of peregons

                if (addPicketIndex == 0) // если добавляемый пикет первый в перегоне
                    curPathArg.Nodes.Insert(0, PicketObj);
                else
                {
                    int tmp = 0;
                    foreach (var item in curPathArg.Nodes)
                    {
                        if (picketCode.Contains(((Picket)item).Code))
                        {
                            tmp = picketCode.IndexOf(((Picket)item).Code);
                            if (tmp > addPicketIndex)
                                break;
                            else
                                indexTmp++;
                        }
                    }
                    if (indexTmp != 0)
                    {

                        curPathArg.Nodes.Insert(indexTmp, PicketObj);
                    }
                    else
                        curPathArg.Nodes.Add(PicketObj);
                }
            }
            else
                curPathArg.Nodes.Add(PicketObj);
        }

        public void calcPeregon(ref EquPath curPathArg, int curLineCode, ref EquLayout curLayout)
        {
            if (curPathArg.Nodes.Count != 0)
            {
                int indexTmp = 0;
                if (curLineCode != lastLine)
                {
                    // create database logical list of peregon
                    lastLine = curLineCode;
                    var empData = from r in _db_controller.layout_table.AsEnumerable() where r.Line == curLineCode select new { r.Code, r.NperegonBefore, r.NperegonAfter };

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
                }
                // create current logical list of peregons
                int addPicketIndex = peregonCode.IndexOf(curLayout.Code);// индекс добавляемого перегона
                
                if (addPicketIndex == 0) // если добавляемый пикет первый в перегоне
                    curPathArg.Nodes.Insert(0, curLayout);
                else
                {
                   // if (addPicketIndex == picketCode.Count - 1)// если добавляемый пикет последний в перегоне
                  //      curPathArg.Nodes.Add(curLayout); // curLayoutArg.Nodes.Insert((picketCode.Count - 1), PicketObj);
                 //   else
                    {
                        int tmp = 0;

                        foreach (var item in curPathArg.Nodes)
                        {
                            if (peregonCode.Contains(((EquLayout)item).Code))
                            {
                                tmp = peregonCode.IndexOf(((EquLayout)item).Code);
                                if (tmp > addPicketIndex)
                                {
                                    if (indexTmp == 0) indexTmp = tmp;
                                    if (tmp < indexTmp) indexTmp = tmp;
                                }
                            }
                        }
                        if (indexTmp != 0)
                            curPathArg.Nodes.Insert(indexTmp-1, curLayout);
                        else
                            curPathArg.Nodes.Add(curLayout);
                    }
                }
            }
            else
                curPathArg.Nodes.Add(curLayout);
        }

        private int lastPicket=0;
        public void calcPicket(ref EquLayout curLayoutArg, int curPeregonCode, ref Picket PicketObj)
        {
            if (curLayoutArg.Nodes.Count != 0)
            {
                int indexTmp = 0;
                if (lastPicket != curPeregonCode)
                {
                    var empData = from r in _db_controller.pickets_table.AsEnumerable() where r.Peregon == curPeregonCode && r.Npiketa != 0 orderby r.Npiketa select new { r.Npiketa, r.Peregon, r.NpicketBefore, r.NpicketAfter, r.number };
                    lastPicket = curPeregonCode;
                    picketCode.Clear();

                    int whileIndex = 0;
                    var val1 = empData.First();

                    while (true)
                    {
                        whileIndex = 0;
                        foreach (var item in empData)
                        {
                            if (val1.number == item.NpicketAfter)
                            {
                                val1 = item;
                                whileIndex = 1;
                            }
                        }
                        if (val1.NpicketBefore == 0 || whileIndex == 0) break;
                    }
                    picketCode.Add(val1.Npiketa);

                    while (true)
                    {
                        whileIndex = 0;
                        foreach (var item in empData)
                        {
                            if (val1.number == item.NpicketBefore)
                            {
                                val1 = item;
                                picketCode.Add(val1.Npiketa);
                                whileIndex = 1;
                            }
                        }
                        if (whileIndex == 0) break;
                    }
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
            int count = _db_controller.all_equipment_table.Rows.Count;
            lastPicket = 0;
            lastLine = 0;
            EquLayout curLayout = null;
            EquGroup curGroup = null;
            EquClass curClass = null;
            EquPath curPath = null;
            EquLine curLine = null;
            Picket PicketObj = null;

            picketCode = new List<int>();
            peregonCode = new List<int>();

            //List<int> peregons = new List<int>();
            List<int> pickets = new List<int>();

            var resRoot = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum != 0 select new { r.ClassNum, r.ClsName }).Distinct();

            foreach (var itemRoot in resRoot)
            {
                curClass = new EquClass(Convert.ToInt32(itemRoot.ClassNum), (String)itemRoot.ClsName);
                curClass.Tag = "Class";
                treeView1.Nodes.Add(curClass);
                curLine = null;
                curPath = null;
                curLayout = null;
                PicketObj = null;

                var resClass = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == curClass.Code && r.GroupNum != 0 select new { r.GroupNum, r.GrpName }).Distinct();

                foreach (var itemClass in resClass)
                {
                    curGroup = new EquGroup(Convert.ToInt32(itemClass.GroupNum), (String)itemClass.GrpName);
                    curGroup.Tag = "Group";
                    curClass.Nodes.Add(curGroup);

                    var resGroup = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == curClass.Code && r.GroupNum == curGroup.Code && r.LineNum != 0 select new { r.LineNum, r.LineName, r.LineCode }).Distinct();

                    foreach (var itemGroup in resGroup)
                    {
                        var resLineOffsetCoordinate = (from r in _db_controller.lines_table.AsEnumerable() where r.LineNum == itemGroup.LineNum select new { r.StartCoordinate });

                        curLine = new EquLine(Convert.ToInt32(itemGroup.LineNum), String.Concat(new object[] { "Линия ", Convert.ToString(itemGroup.LineCode), " - ", Convert.ToString(itemGroup.LineName) }));
                        curLine.LineCode = Convert.ToString(itemGroup.LineCode);
                        curLine.Tag = "Line";
                        curLine.offsetLineCoordinate = resLineOffsetCoordinate.First().StartCoordinate;
                        curGroup.Nodes.Add(curLine);

                        var resTrack = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == curClass.Code && r.GroupNum == curGroup.Code && r.LineNum == curLine.Code && r.Track != 0 select new { r.Track }).Distinct();



                        foreach (var itemTrack in resTrack)
                        {

                            var resTrackName = (from r in _db_controller.trackTable.AsEnumerable() where r.ID == itemTrack.Track select new { r.Track });

                            curPath = new EquPath(Convert.ToInt32(itemTrack.Track), String.Concat(new object[] { "Путь ", resTrackName.First().Track }));
                            curPath.Tag = "Path";
                            curLine.Nodes.Add(curPath);

                            var resPeregons = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == curClass.Code && r.GroupNum == curGroup.Code && r.LineNum == curLine.Code && r.Track == curPath.Code && r.Layout != 0 select new {r.Layout, r.LtName, r.NperegonBefore, r.NperegonAfter }).Distinct();

                            foreach (var itemPeregon in resPeregons)
                            {
                                curLayout = new EquLayout(itemPeregon.Layout, itemPeregon.LtName);
                                curLayout.Tag = "Peregon";
                                calcPeregon(ref curPath, curLine.Code, ref curLayout);
                                curLayout.beforePeregon = Convert.ToInt32(itemPeregon.NperegonBefore);
                                curLayout.aftrerPeregon = Convert.ToInt32(itemPeregon.NperegonAfter);

                                var res4 = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == curClass.Code && r.GroupNum == curGroup.Code && r.LineNum == curLine.Code && r.Track == curPath.Code && r.Layout == curLayout.Code && r.Npicket != 0 select new {r.number, r.Npicket, r.NpicketBefore, r.NpicketAfter, r.Npiketa }).Distinct();

                                foreach (var item in res4)
                                {

                                    PicketObj = new Picket(Convert.ToInt32(item.Npicket), String.Concat(new object[] { "Пикет ", Convert.ToString(item.Npiketa) }));
                                    PicketObj.Tag = "Picket";
                                    PicketObj.before = Convert.ToInt32(item.Npicket);
                                    PicketObj.after = Convert.ToInt32(item.Npicket);
                                    PicketObj.number = Convert.ToInt32(item.number);

                                    calcPicket(ref curLayout, curLayout.Code, ref PicketObj);

                                    var res5 = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == curClass.Code && r.GroupNum == curGroup.Code && r.LineNum == curLine.Code && r.Track == curPath.Code && r.Layout == curLayout.Code && r.Npicket == PicketObj.Code && r.Code != 0 select new { r.Code, r.ObjName, r.typeEquip , r.shiftFromEndPicket}).Distinct();

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

                                        obj.typeEquip = itemEquip.typeEquip;
                                        obj.shiftFromEndPicket = itemEquip.shiftFromEndPicket;
                                        EquTreeNode objNode = new EquTreeNode(String.Concat(new object[] {  obj.Name }));
                                        objNode.UserObject = obj;
                                        objNode.Tag = "equipment";
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

                _db_controller.objects_table.Clear();
                _db_controller.objects_adapter.Fill(_db_controller.objects_table);

                var empData1 = (from r in _db_controller.objects_table.AsEnumerable() where r.Code == elObj.Code select r);
                var item = empData1.First();

                objNodeNew = (EquTreeNode)treeView1.SelectedNode;
                equObjMew = (EquObject)objNodeNew.UserObject;
                equPicketNew = (Picket)objNodeNew.Parent;
                equLayoutNew = (EquLayout)equPicketNew.Parent;
                equPathNew = (EquPath)equLayoutNew.Parent;
                equLineNew = (EquLine)equPathNew.Parent;
                equGroupNew = (EquGroup)equLineNew.Parent;
                equClassNew = (EquClass)equGroupNew.Parent;

                //Eq rn = treeView1.SelectedNode
                elObj.Line = equLineNew.Code;
                elObj.Picket = equPicketNew.Code;
                elObj.Group.Class = equClassNew;
                elObj.Offset = item.shiftFromPicket;
                elObj.State = (byte)item.regularly;

                //if(item.shiftFromEndPicket != -1)
                    elObj.OffsetFromEnd = item.shiftFromEndPicket;
                //if(item.strelkaLeftOrRight != -1)
                    elObj.strelkaDirection = item.strelkaLeftOrRight;

                EquElementForm eqf = new EquElementForm(elObj,_db_controller);
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
            SearchElementsForm sef = new SearchElementsForm(treeView1.Nodes,_db_controller);
            sef.ShowDialog();
        }



        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {

        }
        private TreeNode m_OldSelectNode;
        private void treeView1_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            Point p = new Point(e.X, e.Y);
            if (e.Button == MouseButtons.Right)
            {
                TreeNode node = treeView1.GetNodeAt(p);
                mySelectedNode = node;
                if (node != null)
                {
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
                            break;
                    }
                    treeView1.SelectedNode = m_OldSelectNode;
                    m_OldSelectNode = null;
                }
                else
                    contextMenuStrip3.Show(treeView1, p);
            }
        }

        private void mnuNewFile_Click(object sender, EventArgs e) //----- CLASS --------
        {
            equClassNew = new EquClass();
            form_addClass = new Equipment.AddClass(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView), "Class");
            form_addClass.Class(ref equClassNew);
            form_addClass.ShowDialog();
        }
        private void addNewGroupToolStripMenuItem_Click(object sender, EventArgs e) // ---- GROUP ---------------
        {
            form_NewGruop = new AddNewGruop(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView), "Group");
            form_NewGruop.Group(ref equGroupNew, ref equClassNew);
            form_NewGruop.ShowDialog();
        }

        private void addNewLineToolStripMenuItem_Click(object sender, EventArgs e) // ---- LINE ---------------
        {
            form_line = new Equipment.AddLine(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView), "Line");
            form_line.Line(ref equLineNew, ref equGroupNew, ref equClassNew);
            form_line.ShowDialog();
        }

        private void addNewPathToolStripMenuItem_Click(object sender, EventArgs e) // ---- PATH ---------------
        {
            form_Track = new Equipment.AddTrack(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView), "Track");
            form_Track.Path(ref equPathNew, ref equLineNew, ref equGroupNew, ref equClassNew);
            form_Track.ShowDialog();
        }

        private void addNewStationToolStripMenuItem_Click(object sender, EventArgs e) // ---- PEREGON ---------------
        {
            form1 = new AddNewElementToDatabase(new AddObjectOnTreeView(addObjectOnTreeView), "Peregon", _db_controller, equClassNew, equGroupNew, equLineNew, equPathNew, null, null, null);
            form1.ShowDialog();
        }
        private void добавитьПикетToolStripMenuItem_Click(object sender, EventArgs e) // ---- PICKET ---------------
        {
            form1 = new AddNewElementToDatabase(new AddObjectOnTreeView(addObjectOnTreeView), "Picket", _db_controller, equClassNew, equGroupNew, equLineNew, equPathNew, equLayoutNew, null, null);
            form1.ShowDialog();
        }
        private void addNewEquipmentToolStripMenuItem_Click(object sender, EventArgs e)
        {
            form_newEquip = new Equipment.addNewEquipment(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView), equGroupNew, equLineNew, equClassNew, equLayoutNew, equPicketNew, equPathNew);
            form_newEquip.ShowDialog();
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            int i = 0;
        }

        void addObjectOnTreeView(int code, string NAME, string key)
        {
            switch (key)
            {
                case "Class":
                    EquClass Class = new EquClass(code, NAME);
                    Class.Tag="Class";
                    treeView1.Nodes.Add(Class);
                    treeView1.Refresh();
                    _db_controller.all_equipment_table.Clear();
                    _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);
                    _db_controller.classes_table.Clear();
                    _db_controller.classes_adapter.Fill(_db_controller.classes_table);
                    break;
                case "Group":
                    EquGroup Group = new EquGroup(code, NAME);
                    Group.Tag="Group";
                    equClassNew.Nodes.Add(Group);
                    treeView1.Refresh();
                    _db_controller.all_equipment_table.Clear();
                    _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);
                    _db_controller.groups_table.Clear();
                    _db_controller.groups_adapter.Fill(_db_controller.groups_table);
                    break;
                case "Line":
                    string[] strLine = NAME.Split(';');
                    EquLine line = new EquLine(code, "Линия " + strLine[1] + " - " + strLine[0]);
                    line.Tag ="Line";
                    line.offsetLineCoordinate = Convert.ToInt64(strLine[2]);
                    equGroupNew.Nodes.Add(line);
                    treeView1.Refresh();

                    _db_controller.all_equipment_table.Clear();
                    _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);
                    _db_controller.layout_table.Clear();
                    _db_controller.layout_adapter.Fill(_db_controller.layout_table);
                    _db_controller.lines_table.Clear();
                    _db_controller.lines_adapter.Fill(_db_controller.lines_table);
                    break;

                case "Track":

                    EquPath Path = new EquPath(code, "Путь" + NAME);
                    Path.Tag = "Path";
                    equLineNew.Nodes.Add(Path);
                    treeView1.Refresh();
                    _db_controller.all_equipment_table.Clear();
                    _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);
                    break;
                case "Peregon":
                    string[] strPeregon = NAME.Split(';');

                    EquLayout layout = new EquLayout(code, strPeregon[0]);
                    layout.Tag = "Peregon";
                    updatePeregon(ref equPathNew, equLineNew.Code, ref layout);
                    _db_controller.all_equipment_table.Clear();
                    _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);
                    treeView1.Update();
                    break;
                case "Picket":
                    string[] strPicket = NAME.Split(';');
                    Picket _EquPicket = new Picket(code, "Пикет " + strPicket[0]);
                    _EquPicket.Tag = "Picket";
                    int i = Convert.ToInt32(strPicket[1]);
                    _EquPicket.number = i;
                    updatePicket(ref equLayoutNew, equLayoutNew.Code, ref _EquPicket);
                    _db_controller.all_equipment_table.Clear();
                    _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);
                    treeView1.Update();
                    break;
                case "Obj":
                    string[] str = NAME.Split(';');

                    EquObject obj = new EquObject(code,
                                                    str[0],
                                                    equGroupNew,
                                                    equLayoutNew,
                                                    equPathNew.Code,
                                                    0,
                                                    0);

                    obj.typeEquip = Convert.ToInt32(str[1]);
                    obj.shiftFromEndPicket = Convert.ToInt32(str[2]);
                    obj.Tag = str[3];
                    EquTreeNode objNode = new EquTreeNode(String.Concat(new object[] { /*equGroupNew.Name, " ",*/ obj.Name }));
                    objNode.UserObject = obj;
                    objNode.Tag = str[3];
                    
                    equPicketNew.Nodes.Add(objNode);
                    
                    _db_controller.objects_table.Clear();
                    _db_controller.objects_adapter.Fill(_db_controller.objects_table);
                    _db_controller.all_equipment_table.Clear();
                    _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);
                    _db_controller.process_equipment_table.Clear();
                    _db_controller.process_equipment_adapter.Fill(_db_controller.process_equipment_table);

                    treeView1.Update();
                    break;
            }
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
            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить оборудование - \"" + equObjMew.Name + "\" из текущего пикета?");
            if (result == Equipment.MessageBoxResult.Yes)
            {
                var empData = from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == equClassNew.Code && r.GroupNum == equGroupNew.Code && r.LineNum == equLineNew.Code && r.Track == equPathNew.Code && r.Layout == equLayoutNew.Code && r.Npicket == equPicketNew.Code /*&& r.Code == equObjMew.Code */select new { r.Code };

                int cnt = empData.Count();
                int res = _db_controller.all_equipment_adapter.delEquip(equGroupNew.Code, equPathNew.Code, equPicketNew.Code, equObjMew.Code, cnt);

                var emp1 = from r in _db_controller.objects_table.AsEnumerable() where r.Object == equObjMew.Name select new { r.Code };

                if (emp1.Count() > 1)
                    _db_controller.objects_adapter.DeleteQueryByCode(equObjMew.Code);

                if (res == -1)
                {
                    TreeNode sn = treeView1.SelectedNode;
                    treeView1.Nodes.Remove(sn);
                    treeView1.Update();
                }
            }
        }


        private void удалитьОборудованиеИзБазыДанныхToolStripMenuItem1_Click(object sender, EventArgs e) // удалить оборудование из БД
        {
            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить объект - \"" + equObjMew.Name + "\" из Базы Данных?  При подтверждении, оборудование будет удалено из всей группы.");
            if (result == Equipment.MessageBoxResult.Yes)
            {
                var resAllGroupForEquip = from r in _db_controller.all_equipment_table.AsEnumerable() where r.GroupNum == equGroupNew.Code && r.typeId == equObjMew.typeEquip select new { r.GroupNum };

                foreach (var item in resAllGroupForEquip)
                {
                    var resAllTracksInGroup = from r in _db_controller.all_equipment_table.AsEnumerable() where r.GroupNum == item.GroupNum && r.typeId == equObjMew.typeEquip select new { r.Track };

                    foreach (var itemTrack in resAllTracksInGroup)
                    {
                        var resAllPicketInTrack = from r in _db_controller.all_equipment_table.AsEnumerable() where r.Track == itemTrack.Track && r.GroupNum == item.GroupNum && r.typeId == equObjMew.typeEquip select new { r.Npicket };

                        foreach (var itemPicket in resAllPicketInTrack)
                        {
                            var resAllSelectedEquipInPicket = from r in _db_controller.all_equipment_table.AsEnumerable() where r.Track == itemTrack.Track && r.GroupNum == item.GroupNum && r.Npicket == itemPicket.Npicket && r.typeId == equObjMew.typeEquip select new { r.Code };
                            var resAllEquipInPicket = from r in _db_controller.all_equipment_table.AsEnumerable() where r.Track == itemTrack.Track && r.GroupNum == item.GroupNum && r.Npicket == itemPicket.Npicket select new { r.Code };

                            if (resAllEquipInPicket.Count() > resAllSelectedEquipInPicket.Count())
                            {
                                foreach (var itemEquip in resAllSelectedEquipInPicket)
                                    _db_controller.all_equipment_adapter.delEquip(equGroupNew.Code, equPathNew.Code, itemPicket.Npicket, itemEquip.Code, 0);

                            }
                            else
                            {
                                int i = 0;
                                foreach (var itemEquip in resAllSelectedEquipInPicket)
                                {
                                    if (i == resAllSelectedEquipInPicket.Count() - 1)
                                    {
                                        _db_controller.all_equipment_adapter.delEquip(equGroupNew.Code, equPathNew.Code, itemPicket.Npicket, itemEquip.Code, 1);
                                        break;
                                    }

                                    i++;
                                    _db_controller.all_equipment_adapter.delEquip(equGroupNew.Code, equPathNew.Code, itemPicket.Npicket, itemEquip.Code, 0);
                                }
                            }
                        }
                    }

                }

                _db_controller.refresh();
                TreeNode sn = treeView1.SelectedNode;
                treeView1.Nodes.Clear();
                InitTree();
            }
        }
      


        private void добавитьГруппуToolStripMenuItem_Click(object sender, EventArgs e) // добавить группу (меню класса)
        {
            form_NewGruop = new AddNewGruop(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView), "Group");
            form_NewGruop.Group(ref equGroupNew, ref equClassNew);
            form_NewGruop.ShowDialog();
        }

        private void добавитьЛиниюToolStripMenuItem_Click(object sender, EventArgs e) // добавить линию (меню группы)
        {
            form_line = new Equipment.AddLine(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView), "Line");
            form_line.Line(ref equLineNew, ref equGroupNew, ref equClassNew);
            form_line.ShowDialog();
        }

        private void добавитьПутьToolStripMenuItem_Click(object sender, EventArgs e) // добавить путь (меню линии)
        {
            form_Track = new Equipment.AddTrack(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView), "Track");
            form_Track.Path(ref equPathNew, ref equLineNew, ref equGroupNew, ref equClassNew);
            form_Track.ShowDialog();
        }

        private void добавитьПерегонстанциюToolStripMenuItem_Click(object sender, EventArgs e) // добавить перегон (меню пути)
        {
            form1 = new AddNewElementToDatabase(new AddObjectOnTreeView(addObjectOnTreeView), "Peregon", _db_controller, equClassNew, equGroupNew, equLineNew, equPathNew, null, null, null);
            form1.ShowDialog();
        }

        private void добавитьПикетToolStripMenuItem1_Click(object sender, EventArgs e) // добавление пикета(меню станций)
        {
            form1 = new AddNewElementToDatabase(new AddObjectOnTreeView(addObjectOnTreeView), "Picket", _db_controller, equClassNew, equGroupNew, equLineNew, equPathNew, equLayoutNew, null, null);
            form1.ShowDialog();
        }

        private void добавитьОбъекьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить пикет: \"" + equPicketNew.Code + "\" из Базы Данных?  При подтверждении, также все зависимые от пикета объекты, независимо от группы, будут удалены из Базы данных.");
            if (result == Equipment.MessageBoxResult.Yes)
            {
                var empData1 = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.number == equPicketNew.Code select new { r.ClassNum, r.GroupNum, r.LineNum, r.Track, r.Layout, r.number }).Distinct();

                foreach (var item in empData1)
                {
                    var emp = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == item.ClassNum && r.GroupNum == item.GroupNum && r.Track == item.Track && r.Layout == item.Layout select new { r.Npicket });

                    int res = 0;
                    int cnt = emp.Count();
                    res = _db_controller.all_equipment_adapter.delPicket(equClassNew.Code, equGroupNew.Code, equLineNew.Code, equPathNew.Code, equLayoutNew.Code, equPicketNew.Code, cnt);
                }

                _db_controller.pickets_adapter.delPicketFromDB(equPicketNew.Code);

                _db_controller.refresh();
                treeView1.Nodes.Clear();
                InitTree();
            }
        }

        private void обновитьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            _db_controller.refresh();
            treeView1.Nodes.Clear();
            InitTree();
        }

        private void удалитьПикетИзБазыДанныхToolStripMenuItem_Click(object sender, EventArgs e) // удалить пикет из базы данных
        {
            form_newEquip = new Equipment.addNewEquipment(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView), equGroupNew, equLineNew, equClassNew, equLayoutNew, equPicketNew, equPathNew);
            form_newEquip.ShowDialog();

        }
        private void удалитьПикетИзПерегонаToolStripMenuItem_Click(object sender, EventArgs e)  // удалить пикет из перегона
        {
            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить пикет: \"" + equPicketNew.Code + "\" из текущего перегона?  При подтверждении, также все зависимые от пикета объекты, в текущем перегоне, будут удалены из Базы данных.");
            if (result == Equipment.MessageBoxResult.Yes)
            {
                var empData = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == equClassNew.Code && r.GroupNum == equGroupNew.Code && r.LineNum == equLineNew.Code && r.Track == equPathNew.Code && r.Layout == equLayoutNew.Code select new { r.Npicket }).Distinct();

                int cnt = empData.Count();
                int res = _db_controller.all_equipment_adapter.delPicket(equClassNew.Code, equGroupNew.Code, equLineNew.Code, equPathNew.Code, equLayoutNew.Code, equPicketNew.Code, cnt);

                _db_controller.all_equipment_table.Clear();
                _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);

                if (res == -1)
                {
                    TreeNode sn = treeView1.SelectedNode;
                    treeView1.Nodes.Remove(sn);
                    treeView1.Update();
                }
            }
        }

        private void удалитьToolStripMenuItem_Click(object sender, EventArgs e)// удалить перегон из текущего пути
        {
            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить перегон: \"" + equLayoutNew.ObjName + "\" из текущей группы?  При подтверждении, также все зависимые от перегона объекты, в текущей группе, будут удалены из Базы данных.");
            if (result == Equipment.MessageBoxResult.Yes)
            {
                var empData = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == equClassNew.Code && r.GroupNum == equGroupNew.Code && r.LineNum == equLineNew.Code && r.Track == equPathNew.Code select new { r.Layout }).Distinct();

                int cnt = empData.Count();
                int res = _db_controller.all_equipment_adapter.delPeregon(equClassNew.Code, equGroupNew.Code, equLineNew.Code, equPathNew.Code, equLayoutNew.Code, cnt);

                _db_controller.all_equipment_table.Clear();
                _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);

                if (res == -1)
                {
                    TreeNode sn = treeView1.SelectedNode;
                    treeView1.Nodes.Remove(sn);
                    treeView1.Update();
                }
            }
        }

        private void удалитьИзБазыДанныхToolStripMenuItem_Click(object sender, EventArgs e) // удалить перегон из БД
        {
             Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить перегон - \"" + equLayoutNew.ObjName + "\" из Базы Данных?  При подтверждении, также все зависимые от перегона объекты будут удалены из Базы данных.");
             if (result == Equipment.MessageBoxResult.Yes)
             {
                 var empData1 = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.Layout == equLayoutNew.Code select new { r.ClassNum, r.GroupNum, r.LineNum, r.Track, r.Layout }).Distinct();

                 foreach (var item in empData1)
                 {
                     var emp = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == item.ClassNum && r.GroupNum == item.GroupNum && r.Track == item.Track select new { r.Layout }).Distinct();

                     int res = 0;
                     int cnt = emp.Count();
                     res = _db_controller.all_equipment_adapter.delPeregon(equClassNew.Code, equGroupNew.Code, equLineNew.Code, equPathNew.Code, equLayoutNew.Code, cnt);
                 }

                 _db_controller.layout_adapter.delPeregonFromDB(equLayoutNew.Code);

                 _db_controller.refresh();
                 treeView1.Nodes.Clear();
                 InitTree();
             }
        }

        private void удалитьПутьИзЛинииToolStripMenuItem_Click(object sender, EventArgs e) // удалить пути из выбранной линии
        {
            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить путь - \"" + equPathNew.Code + "\" из текущей группы?  При подтверждении, также все зависимые от пути объекты будут удалены из Базы данных.");
            if (result == Equipment.MessageBoxResult.Yes)
            {
                var empData1 = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == equClassNew.Code && r.GroupNum == equGroupNew.Code && r.LineNum == equLineNew.Code select new { r.Track }).Distinct();
                _db_controller.all_equipment_adapter.delPath(equClassNew.Code, equGroupNew.Code, equLineNew.Code, equPathNew.Code, empData1.Count());

                _db_controller.all_equipment_table.Clear();
                _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);

                TreeNode sn = treeView1.SelectedNode;
                treeView1.Nodes.Remove(sn);
                treeView1.Update();
            }

        }
        private void удалитьЛиниюИзГруппыToolStripMenuItem_Click(object sender, EventArgs e) // удалить линию из группы
        {
             Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить линию - \"" + equLineNew.ObjName + "\" из Базы Данных?  При подтверждении, также и все зависимые от линии объекты, любой группы, будут удалены из Базы данных.");
             if (result == Equipment.MessageBoxResult.Yes)
             {
                 var empData1 = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == equClassNew.Code && r.GroupNum == equGroupNew.Code && r.LineNum == equLineNew.Code select new { r.Track }).Distinct();
                 _db_controller.all_equipment_adapter.delLine(equClassNew.Code, equGroupNew.Code, equLineNew.Code, empData1.Count());

                 _db_controller.all_equipment_table.Clear();
                 _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);

                 TreeNode sn = treeView1.SelectedNode;
                 treeView1.Nodes.Remove(sn);
                 treeView1.Update();
             }
        }

        private void удалитьЛиниюИзБазыДанныхToolStripMenuItem_Click(object sender, EventArgs e) //  удалить линию из БД
        {
            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить линию - \"" + equLineNew.ObjName + "\" из текущей группы?  При подтверждении, только зависимые от линии в текущей группе объекты будут удалены из Базы данных.");
            if (result == Equipment.MessageBoxResult.Yes)
            {
                var empData1 = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.LineNum == equLineNew.Code select new { r.GroupNum }).Distinct();

                foreach (var item in empData1)
                {
                    var emp = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.GroupNum == item.GroupNum select new { r.LineNum }).Distinct();

                    int res = 0;
                    int cnt = emp.Count();
                    res = _db_controller.all_equipment_adapter.delLine(equClassNew.Code, equGroupNew.Code, equLineNew.Code, cnt);
                }

                _db_controller.all_equipment_adapter.delLineFromDB(equLineNew.Code);

                _db_controller.refresh();
                treeView1.Nodes.Clear();
                InitTree();
            }
        }

        private void удалитьГруппуИзКлассаToolStripMenuItem_Click(object sender, EventArgs e) // удалить группу
        {
            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить группу - \"" + equGroupNew.ObjName + "\" из Базы Данных? При подтверждении, таже все зависимые от группы объекты будут удалены из Базы данных.");
            if (result == Equipment.MessageBoxResult.Yes)
            {
                var empData1 = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == equClassNew.Code select new { r.GroupNum }).Distinct();
                _db_controller.all_equipment_adapter.delGroup(equClassNew.Code, equGroupNew.Code, empData1.Count());

                _db_controller.groups_table.Clear();
                _db_controller.groups_adapter.Fill(_db_controller.groups_table);
                _db_controller.objects_table.Clear();
                _db_controller.objects_adapter.Fill(_db_controller.objects_table);

                TreeNode sn = treeView1.SelectedNode;
                treeView1.Nodes.Remove(sn);
                treeView1.Update();
            }
        }

        private void refreshToolStripMenuItem_Click(object sender, EventArgs e) // удалить класс
        {
              Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить класс - \"" + equClassNew.ObjName + "\" из Базы Данных? При подтверждении, также все зависимые от класса объекты будут удалены из Базы данных.");
              if (result == Equipment.MessageBoxResult.Yes)
              {
                  _db_controller.classes_adapter.delClass(equClassNew.Code);

                  _db_controller.refresh();
                  TreeNode sn = treeView1.SelectedNode;
                  treeView1.Nodes.Remove(sn);
                  treeView1.Update();
              }
        }
      
        private void переименоватьToolStripMenuItem4_Click(object sender, EventArgs e) // group rename
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
                                string newClassName = e.Label.Trim();
                                var machesClass = from r in _db_controller.classes_table.AsEnumerable() where r.Class == newClassName select new { r.Code };

                                if (machesClass.Count() == 0)
                                {
                                    _db_controller.classes_adapter.renameClass(e.Label, equClassNew.Code);
                                    e.Node.EndEdit(false);
                                }
                                else
                                {
                                    e.CancelEdit = true;
                                    MessageBox.Show("Класс с таким именем уже существует", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                }
                                
                                break;

                            case "EquGroup": // new picket
                                //EquGroup groupTmp = (EquGroup)mySelectedNode;
                                string newGroupName = e.Label.Trim();
                                var machesGroup = from r in _db_controller.groups_table.AsEnumerable() where r.Group == newGroupName select new { r.Code };

                                if (machesGroup.Count() == 0)
                                {
                                    _db_controller.groups_adapter.renameGroup(e.Label, (short)equGroupNew.Code);
                                    e.Node.EndEdit(false);
                                }
                                else
                                {
                                    e.CancelEdit = true;
                                    MessageBox.Show("Группа с таким именем уже существует", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                }
                                
                                break;

                            case "EquPath":
                                int trackNum = 0;
                                if (int.TryParse(e.Label, out trackNum))
                                {
                                    var machesTrack = from r in _db_controller.all_equipment_table.AsEnumerable() where r.Track == trackNum select new { r.Code };
                                    if (machesTrack.Count() == 0)
                                    {
                                        
                                        _db_controller.all_equipment_adapter.renameTrack(trackNum,equPathNew.Code, equClassNew.Code, equGroupNew.Code, equLineNew.Code);
                                        this.BeginInvoke((MethodInvoker)delegate { e.Node.Text = "Путь " + e.Node.Text; });
                                    }
                                    else
                                    {
                                        e.CancelEdit = true;
                                        MessageBox.Show("Путь с таким номером уже присутствует в текущей линии", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                    }
                                }
                                else
                                {
                                    e.CancelEdit = true;
                                    MessageBox.Show("Для переименования необходимо ввести только номер пути","", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                    return;
                                }

                                break;
                        }
                    }
                    else
                    {
                        e.CancelEdit = true;
                        MessageBox.Show("В названии присутствует некорректный символ: '@','.', ',', '!', ... ",
                           "Переименование", MessageBoxButtons.OK, MessageBoxIcon.Information);
                        e.Node.BeginEdit();
                    }
                }
                else
                {
                    e.CancelEdit = true;
                    MessageBox.Show("Название не должно быть пустым",
                       "Переименование", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    e.Node.BeginEdit();
                }
            }
        }


        private void добавитьКлассToolStripMenuItem_Click(object sender, EventArgs e)
        {
            equClassNew = new EquClass();
            form_addClass = new Equipment.AddClass(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView), "Class");
            form_addClass.Class(ref equClassNew);
            form_addClass.ShowDialog();
        }

        private void addStrelka_Click(object sender, EventArgs e)
        {
            form_Strelka = new Equipment.addStrelka(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView), equGroupNew, equLineNew, equClassNew, equLayoutNew, equPicketNew, equPathNew);
            form_Strelka.ShowDialog();
        }

        private void добавитьСтрелкуToolStripMenuItem_Click(object sender, EventArgs e)
        {
            form_Strelka = new Equipment.addStrelka(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView), equGroupNew, equLineNew, equClassNew, equLayoutNew, equPicketNew, equPathNew);
            form_Strelka.ShowDialog();
        }

        private void свойстваToolStripMenuItem_Click(object sender, EventArgs e)
        {
            form_properties.setProperties("Peregon", equLayoutNew);
            form_properties.Show(DPanel, DockState.DockRight);
        }

        void peregonSettings_RenamePeregonEventHandler(object sender, Equipment.RenameEvent e)
        {
            _db_controller.refresh();
            treeView1.Nodes.Clear();
            InitTree();

        }

        private void propertiesPicket_Click(object sender, EventArgs e)
        {
            form_properties.setProperties("Picket", equPicketNew);
            form_properties.Show(DPanel, DockState.DockRight);
        }
        private void переименоватьToolStripMenuItem6_Click(object sender, EventArgs e) //equipment properties
        {
            form_properties.setProperties("Equipment", equObjMew);
            form_properties.Show(DPanel, DockState.DockRight);
        }
        private void переименоватьToolStripMenuItem3_Click(object sender, EventArgs e) //properties of Line
        {
            form_properties.setProperties("Line", equLineNew);
            form_properties.Show(DPanel, DockState.DockRight);
        }
        private void переименоватьToolStripMenuItem7_Click(object sender, EventArgs e) // переименовать путь
        {
            form_properties.setProperties("Path", equPathNew);
            form_properties.Show(DPanel, DockState.DockRight);
            //renameWrapper();
        }
        private void свойстваToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            form_properties.setProperties("Group",equGroupNew);
            form_properties.Show(DPanel, DockState.DockRight);
        }
        private void переименоватьToolStripMenuItem5_Click(object sender, EventArgs e) // переименовать класс
        {
            form_properties.setProperties("Class", equClassNew);
            form_properties.Show(DPanel, DockState.DockRight); 
        }

        private void toolStripButton5_Click(object sender, EventArgs e)
        {
            if (treeView1.SelectedNode != null)
            {
                objNodeNew   = (EquTreeNode)treeView1.SelectedNode;
                switch(Convert.ToString(objNodeNew.Tag))
                {
                    case "Class":
                        equClassNew = (EquClass)objNodeNew;
                        form_properties.setProperties("Class", equClassNew);
                        form_properties.Show(DPanel, DockState.DockRight); 
                        break;
                    case "Group":
                        equGroupNew = (EquGroup)objNodeNew;
                        form_properties.setProperties("Group",equGroupNew);
                        form_properties.Show(DPanel, DockState.DockRight);
                        break;
                    case "Line":
                        equLineNew = (EquLine)objNodeNew;
                        form_properties.setProperties("Line", equLineNew);
                        form_properties.Show(DPanel, DockState.DockRight);
                        break;
                    case "Path":
                        equPathNew = (EquPath)objNodeNew;
                        form_properties.setProperties("Path", equPathNew);
                        form_properties.Show(DPanel, DockState.DockRight);
                        break;
                    case "Peregon":
                        equLayoutNew = (EquLayout)objNodeNew;
                        form_properties.setProperties("Peregon", equLayoutNew);
                        form_properties.Show(DPanel, DockState.DockRight);
                        break;
                    case "Picket":
                        equPicketNew = (Picket)objNodeNew;
                        form_properties.setProperties("Picket", equPicketNew);
                        form_properties.Show(DPanel, DockState.DockRight);
                        break;
                    case "equipment":
                        equObjMew = (EquObject)objNodeNew.UserObject;
                        form_properties.setProperties("Equipment", equObjMew);
                        form_properties.Show(DPanel, DockState.DockRight);
                        break;
                }
            }
        }
    }
}
