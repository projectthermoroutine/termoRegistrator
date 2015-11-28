using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;
using Registrator.Equipment;

namespace Registrator
{
    public delegate void AddObjectOnTreeView(int code, string name, string Tag);
    public delegate void AddObjectTreeView(EquTreeNode obj);

    public partial class AllEquipmentTree : ToolWindow
    {
        public DB.metro_db_controller _db_controller;

        AddClass form_addClass;
        addNewEquipment form_newEquip;
        addStrelka form_Strelka;
        AddNewGruop form_NewGruop;
        AddTrack form_Track;
        AddLine form_line;
        Equipment.Properties form_properties;
    
        DockPanel  DPanel;

        public AllEquipmentTree(DB.metro_db_controller db_controller, DockPanel DockPanel_Arg)
        {
            InitializeComponent();
            
            _db_controller = new DB.metro_db_controller(db_controller);
            form_properties_FormClosing(null, null);

            InitTree();
            treeView1.AfterLabelEdit += treeView1_AfterLabelEdit;

            this.treeView1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.treeView1_MouseUp);

            DPanel = DockPanel_Arg;
        }

        void form_properties_FormClosing(object sender, EventArgs e)
        {
            form_properties = new Equipment.Properties(_db_controller);
            form_properties.FormClosing += form_properties_FormClosing;
            form_properties.groupSettings.RenameEventHandler += peregonSettings_RenamePeregonEventHandler;
            form_properties.lineSettings.RenameEventHandler += peregonSettings_RenamePeregonEventHandler;
            form_properties.equipSettings.RenameEventHandler += peregonSettings_RenamePeregonEventHandler;
            form_properties.equipExtSettings.RenameEventHandler += peregonSettings_RenamePeregonEventHandler;
            form_properties.classSettings.RenameEventHandler += peregonSettings_RenamePeregonEventHandler;
            form_properties.strelkaSettings.RenameEventHandler += peregonSettings_RenamePeregonEventHandler;
            form_properties.pathSettings.RenameEventHandler += peregonSettings_RenamePeregonEventHandler;
            form_properties.picketSettings.ChangeLenghtEvent += ChangePicketLengthEventHandler;
        }


        void ChangePicketLengthEventHandler(object s, EventArgs e )
        {
            EquTreeNode PathTreeNode = (curEquTreeNode.Parent as EquTreeNode);
            PathTreeNode.Nodes.Clear();

            EquTreeNode Class = PathTreeNode.Parent.Parent.Parent as EquTreeNode;
            EquTreeNode Group = PathTreeNode.Parent.Parent as EquTreeNode;
            EquTreeNode Line  = PathTreeNode.Parent as EquTreeNode;

            FillPath(new PicketsManager(_db_controller), Class, Group, Line, PathTreeNode);
        }

        private void InitTree()
        {
            PicketsManager currentPicketsManager = new PicketsManager(_db_controller);

            var resRoot = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum != 0 select new { r.ClassNum, r.ClsName }).Distinct();

            foreach (var itemRoot in resRoot)
            {
                EquTreeNode ClassTreeNode = new EquTreeNode(mnuTextFile, new EquClass(Convert.ToInt32(itemRoot.ClassNum), (String)itemRoot.ClsName), form_properties);
                treeView1.Nodes.Add(ClassTreeNode);

                var resClass = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == ((EquClass)ClassTreeNode.ObjectDB).Code && r.GroupNum != 0 select new { r.GroupNum, r.GrpName }).Distinct();

                foreach (var itemClass in resClass)
                {
                    EquTreeNode GroupTreeNode = new EquTreeNode(contextMenuStrip_Group, new EquGroup(Convert.ToInt32(itemClass.GroupNum), (String)itemClass.GrpName, ClassTreeNode.ObjectDB), form_properties);
                    ClassTreeNode.Nodes.Add(GroupTreeNode);

                    var resGroup = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == ((EquClass)ClassTreeNode.ObjectDB).Code && r.GroupNum == ((EquGroup)GroupTreeNode.ObjectDB).Code && r.LineNum != 0 select new { r.LineNum, r.LineName, r.LineCode }).Distinct();

                    foreach (var itemGroup in resGroup)
                    {
                        var resLineOffsetCoordinate = (from r in _db_controller.lines_table.AsEnumerable() where r.LineNum == itemGroup.LineNum select new { r.StartCoordinate });

                        EquTreeNode LineTreeNode = new EquTreeNode( contextMenuStrip_Line,
                                                                    new EquLine(Convert.ToInt32(itemGroup.LineNum),
                                                                    String.Concat(new object[] { "Линия ", Convert.ToString(itemGroup.LineName)}),
                                                                    Convert.ToString(itemGroup.LineCode), resLineOffsetCoordinate.First().StartCoordinate,
                                                                    GroupTreeNode.ObjectDB),
                                                                    form_properties);

                        GroupTreeNode.Nodes.Add(LineTreeNode);

                        var resTrack = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == ((EquClass)ClassTreeNode.ObjectDB).Code && r.GroupNum == ((EquGroup)GroupTreeNode.ObjectDB).Code && r.LineNum == ((EquLine)LineTreeNode.ObjectDB).Code && r.Track != 0 select new { r.Track }).Distinct();

                        foreach (var itemTrack in resTrack)
                        {

                            var resTrackName = (from r in _db_controller.trackTable.AsEnumerable() where r.ID == itemTrack.Track select new { r.Track });

                            EquTreeNode PathTreeNode = new EquTreeNode( contextMenuStrip_Path,
                                                                        new EquPath(Convert.ToInt32(itemTrack.Track),
                                                                        String.Concat(new object[] { "Путь ", resTrackName.First().Track }),
                                                                        LineTreeNode.ObjectDB),
                                                                        form_properties);
                            LineTreeNode.Nodes.Add(PathTreeNode);
                            FillPath(currentPicketsManager, ClassTreeNode, GroupTreeNode, LineTreeNode, PathTreeNode);
                        }
                    }
                }
            }
        }
        void FillPath(PicketsManager PM, EquTreeNode Class, EquTreeNode Group, EquTreeNode Line, EquTreeNode Path)
        {
            IEnumerable<EquPicket> IPickets = (from r in _db_controller.all_equipment_table.AsEnumerable()
                                               where r.ClassNum == ((EquClass)Class.ObjectDB).Code && r.GroupNum == ((EquGroup)Group.ObjectDB).Code && r.LineNum == ((EquLine)Line.ObjectDB).Code && r.Track == ((EquPath)Path.ObjectDB).Code && r.number != 0
                                               select new EquPicket("Пикет " + r.PicketDisplayNumber, r.number, r.PicketDisplayNumber, r.number, r.NpicketAfter, r.NpicketBefore, r.StartShiftLine, r.EndShiftLine, 0, Path.ObjectDB)).GroupBy(x => x.number).Select(g => g.First());

            PM.createLogicalPicketList(((EquPath)Path.ObjectDB).Code, ((EquLine)Line.ObjectDB).Code, ((EquGroup)Group.ObjectDB).Code, ((EquClass)Class.ObjectDB).Code);
            PM.Matching(IPickets);

            FillPickets(PM.PicketsList, Path);
        }

        void FillPickets(IEnumerable<EquPicket> pickets, EquTreeNode _curPath)
        {
            if (_curPath.ObjectDB == null ||
                _curPath.ObjectDB.GetType() != typeof(EquPath)
                )
                return;

            var curPath = _curPath.ObjectDB as EquPath;
            var curLine = curPath.Line;
            var curGroup = curLine.Group;
            var curClass = curGroup.Class;
            foreach (EquPicket picket in pickets)
            {
                var PicketTreeNode = new EquTreeNode(contextMenuStrip_Picket, picket, form_properties);
                _curPath.Nodes.Add(PicketTreeNode);

                var resPicketEquipment = (from r in _db_controller.all_equipment_table.AsEnumerable()
                                          where r.ClassNum == curClass.Code && r.GroupNum == curGroup.Code && r.LineNum == curLine.Code && r.Track == curPath.Code && r.Npicket == picket.Code && r.Code != 0
                                          select new { r.Code, r.ObjName, r.typeEquip, r.ObjectLenght }).Distinct();

                foreach (var itemEquip in resPicketEquipment)
                {
                    EquObject obj = new EquObject(  Convert.ToInt32(itemEquip.Code),
                                                    (String)itemEquip.ObjName,
                                                    curGroup,
                                                    null,
                                                    curPath,
                                                    picket,
                                                    0 );

                    obj.typeEquip = itemEquip.typeEquip;
                    obj.ObjectLenght = itemEquip.ObjectLenght;

                    PicketTreeNode.Nodes.Add(new EquTreeNode(contextMenuStrip_Equipment, obj, form_properties));
                }
            }
        }

        private void treeView1_DoubleClick(object sender, EventArgs e)
        {
            if (treeView1.SelectedNode != null )
            {
                EquTreeNode ObjectTreeNode = (EquTreeNode)treeView1.SelectedNode;

                EquObject elObj = ObjectTreeNode.ObjectDB as EquObject;
                if (elObj != null)
                {
                    _db_controller.objects_table.Clear();
                    _db_controller.objects_adapter.Fill(_db_controller.objects_table);

                    var empData1 = (from r in _db_controller.objects_table.AsEnumerable() where r.Code == elObj.Code select r);
                    var item = empData1.First();

                    EquClass equClassNew;
                    EquGroup equGroupNew;
                    EquLine equLineNew;
                    EquPicket equPicketNew;

                    equPicketNew = ((EquTreeNode)ObjectTreeNode.Parent).ObjectDB as EquPicket;

                    EquTreeNode PathTreeNode = (EquTreeNode)ObjectTreeNode.Parent.Parent;
                    equLineNew = ((EquTreeNode)PathTreeNode.Parent).ObjectDB as EquLine;
                    equGroupNew = ((EquTreeNode)PathTreeNode.Parent.Parent).ObjectDB as EquGroup;
                    equClassNew = ((EquTreeNode)PathTreeNode.Parent.Parent.Parent).ObjectDB as EquClass;

                    elObj.Line = equLineNew;
                    elObj.Picket = equPicketNew;
                    elObj.Group.Class = equClassNew;
                    elObj.Offset = item.shiftFromPicket;
                    elObj.State = (byte)item.regularly;

                    elObj.ObjectLenght = item.ObjectLenght;
                    elObj.strelkaDirection = item.strelkaLeftOrRight;

                    EquElementForm eqf = new EquElementForm(elObj, _db_controller);
                    eqf.ShowDialog(this);
                }
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
        TreeNode m_OldSelectNode;

        EquTreeNode curEquTreeNode;
        private void treeView1_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            Point p = new Point(e.X, e.Y);
            if (e.Button == MouseButtons.Right)
            {
                TreeNode node = treeView1.GetNodeAt(p);
                if (node != null)
                {
                    m_OldSelectNode = treeView1.SelectedNode;
                    treeView1.SelectedNode = node;

                    curEquTreeNode = (EquTreeNode)node;
                    curEquTreeNode.ShowMenu(treeView1, p);

                    treeView1.SelectedNode = m_OldSelectNode;
                    m_OldSelectNode = null;
                }
                else
                    contextMenuStrip3.Show(treeView1, p);
            }
        }

        private void mnuNewFile_Click(object sender, EventArgs e)
        {
            form_addClass = new Equipment.AddClass(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView));
            form_addClass.ShowDialog();
        }
        private void addNewGroupToolStripMenuItem_Click(object sender, EventArgs e) 
        {
            curEquTreeNode = curEquTreeNode.Parent as EquTreeNode;
            form_NewGruop = new AddNewGruop(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView), curEquTreeNode);
            form_NewGruop.ShowDialog();
        }

        private void addNewLineToolStripMenuItem_Click(object sender, EventArgs e) 
        {
            curEquTreeNode = curEquTreeNode.Parent as EquTreeNode;
            form_line = new Equipment.AddLine(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView), curEquTreeNode);
            form_line.ShowDialog();
        }

        private void addNewPathToolStripMenuItem_Click(object sender, EventArgs e) 
        {
            curEquTreeNode = curEquTreeNode.Parent as EquTreeNode;
            form_Track = new Equipment.AddTrack(_db_controller, curEquTreeNode.ObjectDB);
            form_Track.TrackAddedEvent += TrackAdded;
            form_Track.ShowDialog();

        }

        private void addNewStationToolStripMenuItem_Click(object sender, EventArgs e) 
        {
            Equipment.AddPicket formPicket = new Equipment.AddPicket(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView), curEquTreeNode);
            formPicket.ShowDialog();
        }
        private void добавитьПикетToolStripMenuItem_Click(object sender, EventArgs e) 
        {
            Equipment.AddPicket formPicket = new Equipment.AddPicket(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView), curEquTreeNode);
            formPicket.ShowDialog();
        }
        private void addNewEquipmentToolStripMenuItem_Click(object sender, EventArgs e)
        {
            curEquTreeNode = curEquTreeNode.Parent as EquTreeNode;
            form_newEquip = new Equipment.addNewEquipment(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView), curEquTreeNode);
            form_newEquip.ShowDialog();
        }

        void AddObjectTreeView(EquTreeNode obj)
        {
            curEquTreeNode.Nodes.Add(obj);
        }

        void addObjectOnTreeView(int code, string NAME, string key)
        {
            EquPath _EquPath;
            EquLine _EquLine;
            EquGroup _EquGroup;
            EquClass _EquClass;
            EquPicket _EquPicket;

            switch (key)
            {
                case "Class":
                    treeView1.Nodes.Add(new EquTreeNode(mnuTextFile, new EquClass(code, NAME), form_properties));
                    treeView1.Refresh();
                    _db_controller.all_equipment_table.Clear();
                    _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);
                    _db_controller.classes_table.Clear();
                    _db_controller.classes_adapter.Fill(_db_controller.classes_table);
                    break;
                case "Group":
                    curEquTreeNode.Nodes.Add(new EquTreeNode(contextMenuStrip_Group, new EquGroup(code, NAME, curEquTreeNode.ObjectDB), form_properties));
                    treeView1.Refresh();
                    _db_controller.all_equipment_table.Clear();
                    _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);
                    _db_controller.groups_table.Clear();
                    _db_controller.groups_adapter.Fill(_db_controller.groups_table);
                    break;
                case "Line":
                    string[] strLine = NAME.Split(';');
                    curEquTreeNode.Nodes.Add(new EquTreeNode(contextMenuStrip_Line, new EquLine(code, "Линия " + strLine[1] + " - " + strLine[0], "", Convert.ToInt64(strLine[2]), curEquTreeNode.ObjectDB), form_properties));
                    treeView1.Refresh();

                    _db_controller.all_equipment_table.Clear();
                    _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);
                    _db_controller.layout_table.Clear();
                    _db_controller.layout_adapter.Fill(_db_controller.layout_table);
                    _db_controller.lines_table.Clear();
                    _db_controller.lines_adapter.Fill(_db_controller.lines_table);
                    break;
                
                case "Picket":
                    curEquTreeNode.Nodes.Clear();
                    _db_controller.all_equipment_table.Clear();
                    _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);

                   

                    _EquPath  = curEquTreeNode.ObjectDB as EquPath;
                    _EquLine  = (curEquTreeNode.Parent as EquTreeNode).ObjectDB as EquLine;
                    _EquGroup = (curEquTreeNode.Parent.Parent as EquTreeNode).ObjectDB as EquGroup;
                    _EquClass = (curEquTreeNode.Parent.Parent.Parent as EquTreeNode).ObjectDB as EquClass;

                    IEnumerable<EquPicket> IPickets = (from r in _db_controller.all_equipment_table.AsEnumerable()
                                                       where r.ClassNum == _EquClass.Code && r.GroupNum == _EquGroup.Code && r.LineNum == _EquLine.Code && r.Track == _EquPath.Code && r.number != 0
                                                       select new EquPicket("Пикет " + r.PicketDisplayNumber, r.number, r.PicketDisplayNumber, r.number, r.NpicketAfter, r.NpicketBefore, r.StartShiftLine, r.EndShiftLine, 0, curEquTreeNode.ObjectDB)).GroupBy(x => x.number).Select(g => g.First());

                    PicketsManager PManager = new PicketsManager(_db_controller);
                    PManager.createLogicalPicketList(((EquPath)curEquTreeNode.ObjectDB).Code, _EquLine.Code, _EquGroup.Code, _EquClass.Code);
                    PManager.Matching(IPickets);

                    FillPickets(PManager.PicketsList, curEquTreeNode);
                    
                    treeView1.Update();
                    break;

                case "Obj":
                    string[] str = NAME.Split(';');

                    _EquPicket = curEquTreeNode.ObjectDB as EquPicket;
                    _EquPath   = (curEquTreeNode.Parent as EquTreeNode).ObjectDB as EquPath;
                    _EquGroup  = (curEquTreeNode.Parent.Parent.Parent as EquTreeNode).ObjectDB as EquGroup;
                    
                    EquObject obj = new EquObject(  code,
                                                    str[0],
                                                    _EquGroup,
                                                    null,
                                                    _EquPath,
                                                    _EquPicket,
                                                    0);

                    obj.typeEquip = Convert.ToInt32(str[1]);
                    obj.ObjectLenght = Convert.ToInt32(str[2]);
                    EquTreeNode objNode = new EquTreeNode(contextMenuStrip_Equipment, obj, form_properties);

                    curEquTreeNode.Nodes.Add(objNode);
                    
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

        private void DeleteEquipFromPicket_Click(object sender, EventArgs e) 
        {
            EquObject _EquObject = curEquTreeNode.ObjectDB as EquObject;
            EquPicket _EquPicket = (curEquTreeNode.Parent as EquTreeNode).ObjectDB as EquPicket;
            EquPath   _EquPath   = (curEquTreeNode.Parent.Parent as EquTreeNode).ObjectDB as EquPath;
            EquLine   _EquLine   = (curEquTreeNode.Parent.Parent.Parent as EquTreeNode).ObjectDB as EquLine;
            EquGroup  _EquGroup  = (curEquTreeNode.Parent.Parent.Parent.Parent as EquTreeNode).ObjectDB as EquGroup;
            EquClass  _EquClass  = (curEquTreeNode.Parent.Parent.Parent.Parent.Parent as EquTreeNode).ObjectDB as EquClass;

            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить оборудование - \"" + _EquObject.Name + "\" из текущего пикета?");
           
            if (result == Equipment.MessageBoxResult.Yes)
            {
                var empData = from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == _EquClass.Code && r.GroupNum == _EquGroup.Code && r.LineNum == _EquLine.Code && r.Track == _EquPath.Code && r.Npicket == _EquPicket.Code && r.Code == _EquObject.Code select new { r.Code };

                int cnt = empData.Count();
                int res = _db_controller.all_equipment_adapter.delEquip(_EquGroup.Code, _EquPath.Code, _EquPicket.Code, _EquObject.Code, cnt);

                var emp1 = from r in _db_controller.objects_table.AsEnumerable() where r.Object == _EquObject.Name select new { r.Code };

                if (emp1.Count() > 1)
                    _db_controller.objects_adapter.DeleteQueryByCode(_EquObject.Code);

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
            EquObject _EquObject = curEquTreeNode.ObjectDB as EquObject;
            EquPicket _EquPicket = (curEquTreeNode.Parent as EquTreeNode).ObjectDB as EquPicket;
            EquPath _EquPath = (curEquTreeNode.Parent.Parent as EquTreeNode).ObjectDB as EquPath;
            EquLine _EquLine = (curEquTreeNode.Parent.Parent.Parent as EquTreeNode).ObjectDB as EquLine;
            EquGroup _EquGroup = (curEquTreeNode.Parent.Parent.Parent.Parent as EquTreeNode).ObjectDB as EquGroup;
            EquClass _EquClass = (curEquTreeNode.Parent.Parent.Parent.Parent.Parent as EquTreeNode).ObjectDB as EquClass;

            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить объект - \"" + _EquObject.Name + "\" из Базы Данных?  При подтверждении, оборудование будет удалено из всей группы.");
            if (result == Equipment.MessageBoxResult.Yes)
            {
                var resAllGroupForEquip = from r in _db_controller.all_equipment_table.AsEnumerable() where r.GroupNum == _EquGroup.Code && r.typeId == _EquObject.typeEquip select new { r.GroupNum };

                foreach (var item in resAllGroupForEquip)
                {
                    var resAllTracksInGroup = from r in _db_controller.all_equipment_table.AsEnumerable() where r.GroupNum == item.GroupNum && r.typeId == _EquObject.typeEquip select new { r.Track };

                    foreach (var itemTrack in resAllTracksInGroup)
                    {
                        var resAllPicketInTrack = from r in _db_controller.all_equipment_table.AsEnumerable() where r.Track == itemTrack.Track && r.GroupNum == item.GroupNum && r.typeId == _EquObject.typeEquip select new { r.Npicket };

                        foreach (var itemPicket in resAllPicketInTrack)
                        {
                            var resAllSelectedEquipInPicket = from r in _db_controller.all_equipment_table.AsEnumerable() where r.Track == itemTrack.Track && r.GroupNum == item.GroupNum && r.Npicket == itemPicket.Npicket && r.typeId == _EquObject.typeEquip select new { r.Code };
                            var resAllEquipInPicket = from r in _db_controller.all_equipment_table.AsEnumerable() where r.Track == itemTrack.Track && r.GroupNum == item.GroupNum && r.Npicket == itemPicket.Npicket select new { r.Code };

                            if (resAllEquipInPicket.Count() > resAllSelectedEquipInPicket.Count())
                            {
                                foreach (var itemEquip in resAllSelectedEquipInPicket)
                                    _db_controller.all_equipment_adapter.delEquip(_EquGroup.Code, _EquPath.Code, itemPicket.Npicket, itemEquip.Code, 0);

                            }
                            else
                            {
                                int i = 0;
                                foreach (var itemEquip in resAllSelectedEquipInPicket)
                                {
                                    if (i == resAllSelectedEquipInPicket.Count() - 1)
                                    {
                                        _db_controller.all_equipment_adapter.delEquip(_EquGroup.Code, _EquPath.Code, itemPicket.Npicket, itemEquip.Code, 1);
                                        break;
                                    }

                                    i++;
                                    _db_controller.all_equipment_adapter.delEquip(_EquGroup.Code, _EquPath.Code, itemPicket.Npicket, itemEquip.Code, 0);
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

        private void добавитьГруппуToolStripMenuItem_Click(object sender, EventArgs e) 
        {
            form_NewGruop = new AddNewGruop(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView),curEquTreeNode);
            form_NewGruop.ShowDialog();
        }

        private void добавитьЛиниюToolStripMenuItem_Click(object sender, EventArgs e) 
        {
            form_line = new Equipment.AddLine(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView),curEquTreeNode);
            form_line.ShowDialog();
        }

        private void добавитьПутьToolStripMenuItem_Click(object sender, EventArgs e) 
        {
            //form_Track = new Equipment.AddTrack(_db_controller, new AddObjectTreeView(AddObjectTreeView), curEquTreeNode, new EquTreeNode(contextMenuStrip_Path, form_properties), new EquTreeNode(contextMenuStrip_Picket, form_properties));

            form_Track = new Equipment.AddTrack(_db_controller, curEquTreeNode.ObjectDB);
            form_Track.TrackAddedEvent += TrackAdded;
            form_Track.ShowDialog();
        }

        void TrackAdded(object sender, DbObjectEventArg e)
        {
            EquPath path_object = e.DbObject as EquPath;
            EquTreeNode PathTreeNode = new EquTreeNode(contextMenuStrip_Path,
                                            path_object,
                                            form_properties);

            curEquTreeNode.Nodes.Add(PathTreeNode);

            curEquTreeNode = PathTreeNode;
            _db_controller.all_equipment_table.Clear();
            _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);

            var _EquLine = path_object.Line;
            var _EquGroup = _EquLine.Group;
            var _EquClass = _EquGroup.Class;

            IEnumerable<EquPicket> IPickets = (from r in _db_controller.all_equipment_table.AsEnumerable()
                                               where r.ClassNum == _EquClass.Code && r.GroupNum == _EquGroup.Code && r.LineNum == _EquLine.Code && r.Track == path_object.Code && r.number != 0
                                               select new EquPicket("Пикет " + r.PicketDisplayNumber, r.number, r.PicketDisplayNumber, r.number, r.NpicketAfter, r.NpicketBefore, r.StartShiftLine, r.EndShiftLine, 0, path_object)).GroupBy(x => x.number).Select(g => g.First());

            PicketsManager PManager = new PicketsManager(_db_controller);
            PManager.createLogicalPicketList(((EquPath)curEquTreeNode.ObjectDB).Code, _EquLine.Code, _EquGroup.Code, _EquClass.Code);
            PManager.Matching(IPickets);

            FillPickets(PManager.PicketsList, curEquTreeNode);

            treeView1.Update();
        }

        private void добавитьПерегонстанциюToolStripMenuItem_Click(object sender, EventArgs e) 
        {
            Equipment.AddPicket formPicket = new Equipment.AddPicket(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView), curEquTreeNode);
            formPicket.ShowDialog();
        }

        private void добавитьПикетToolStripMenuItem1_Click(object sender, EventArgs e) 
        {
            Equipment.AddPicket formPicket = new Equipment.AddPicket(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView), curEquTreeNode);
            formPicket.ShowDialog();
        }

        private void deletePicketFromDataBase_Click(object sender, EventArgs e)
        {
            EquPicket _EquPicket = curEquTreeNode.ObjectDB as EquPicket;

            if(_EquPicket.after != 0 && _EquPicket.before != 0)
            {
                MessageBox.Show("Информация", "Удалить можно первый, либо последний пикеты");
                return;
            }

            EquPath _EquPath = (curEquTreeNode.Parent as EquTreeNode).ObjectDB as EquPath;
            EquLine _EquLine = (curEquTreeNode.Parent.Parent as EquTreeNode).ObjectDB as EquLine;
            EquGroup _EquGroup = (curEquTreeNode.Parent.Parent.Parent as EquTreeNode).ObjectDB as EquGroup;
            EquClass _EquClass = (curEquTreeNode.Parent.Parent.Parent.Parent as EquTreeNode).ObjectDB as EquClass;

            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить пикет: \"" + _EquPicket.number + "\" из Базы Данных?");
            
            if (result == Equipment.MessageBoxResult.Yes)
            {
                var empData1 = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.number == _EquPicket.Code select new { r.ClassNum, r.GroupNum, r.LineNum, r.Track, r.Layout, r.number }).Distinct();

                foreach (var item in empData1)
                {
                    var emp = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == item.ClassNum && r.GroupNum == item.GroupNum && r.Track == item.Track && r.Layout == item.Layout select new { r.Npicket });

                    int res = 0;
                    int cnt = emp.Count();
                    res = _db_controller.all_equipment_adapter.delPicket(_EquClass.Code, _EquGroup.Code, _EquLine.Code, _EquPath.Code, 0, _EquPicket.Code, cnt);
                }

                object resDelPicket = _db_controller.pickets_adapter.delPicketFromDB(_EquPicket.number);

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

        private void addEquipmentPicketsMenu_Click(object sender, EventArgs e)
        {
            form_newEquip = new Equipment.addNewEquipment(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView),curEquTreeNode);
            form_newEquip.ShowDialog();

        }
        

        private void удалитьПутьИзЛинииToolStripMenuItem_Click(object sender, EventArgs e) // удалить пути из выбранной линии
        {
            EquPath _EquPath = curEquTreeNode.ObjectDB as EquPath;
            EquLine _EquLine = (curEquTreeNode.Parent as EquTreeNode).ObjectDB as EquLine;
            EquGroup _EquGroup = (curEquTreeNode.Parent.Parent as EquTreeNode).ObjectDB as EquGroup;
            EquClass _EquClass = (curEquTreeNode.Parent.Parent.Parent as EquTreeNode).ObjectDB as EquClass;

            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить путь - \"" + _EquPath.Name + "\" из текущей группы? Все зависимые от пути объекты будут удалены");
            if (result == Equipment.MessageBoxResult.Yes)
            {
                var empData1 = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == _EquClass.Code && r.GroupNum == _EquGroup.Code && r.LineNum == _EquLine.Code select new { r.Track }).Distinct();
                _db_controller.trackAdapter.delPath(_EquPath.Code, empData1.Count());

                _db_controller.all_equipment_table.Clear();
                _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);
                _db_controller.trackTable.Clear();
                _db_controller.trackAdapter.Fill(_db_controller.trackTable);
                _db_controller.pickets_table.Clear();
                _db_controller.pickets_adapter.Fill(_db_controller.pickets_table);

                TreeNode sn = treeView1.SelectedNode;
                treeView1.Nodes.Remove(sn);
                treeView1.Update();
            }

        }
        private void удалитьЛиниюИзГруппыToolStripMenuItem_Click(object sender, EventArgs e) // удалить линию из группы
        {
            EquLine  _EquLine = curEquTreeNode.ObjectDB as EquLine;
            EquGroup _EquGroup = (curEquTreeNode.Parent as EquTreeNode).ObjectDB as EquGroup;
            EquClass _EquClass = (curEquTreeNode.Parent.Parent as EquTreeNode).ObjectDB as EquClass;

            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить линию - \"" + _EquLine.Name + "\" из Базы Данных?  При подтверждении, также и все зависимые от линии объекты, любой группы, будут удалены из Базы данных.");
             
            if (result == Equipment.MessageBoxResult.Yes)
             {
                 var empData1 = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == _EquClass.Code && r.GroupNum == _EquGroup.Code && r.LineNum == _EquLine.Code select new { r.Track }).Distinct();
                 _db_controller.all_equipment_adapter.delLine(_EquClass.Code, _EquGroup.Code, _EquLine.Code, empData1.Count());

                 _db_controller.all_equipment_table.Clear();
                 _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);

                 TreeNode sn = treeView1.SelectedNode;
                 treeView1.Nodes.Remove(sn);
                 treeView1.Update();
             }
        }

        private void удалитьЛиниюИзБазыДанныхToolStripMenuItem_Click(object sender, EventArgs e) //  удалить линию из БД
        {
            EquLine _EquLine = curEquTreeNode.ObjectDB as EquLine;
            EquGroup _EquGroup = (curEquTreeNode.Parent as EquTreeNode).ObjectDB as EquGroup;
            EquClass _EquClass = (curEquTreeNode.Parent.Parent as EquTreeNode).ObjectDB as EquClass;

            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить линию - \"" + _EquLine.Name + "\" из текущей группы?  При подтверждении, только зависимые от линии в текущей группе объекты будут удалены из Базы данных.");
            if (result == Equipment.MessageBoxResult.Yes)
            {
                var empData1 = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.LineNum == _EquLine.Code select new { r.GroupNum }).Distinct();

                foreach (var item in empData1)
                {
                    var emp = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.GroupNum == item.GroupNum select new { r.LineNum }).Distinct();

                    int res = 0;
                    int cnt = emp.Count();
                    res = _db_controller.all_equipment_adapter.delLine(_EquClass.Code, _EquGroup.Code, _EquLine.Code, cnt);
                }

                _db_controller.all_equipment_adapter.delLineFromDB(_EquLine.Code);

                _db_controller.refresh();
                treeView1.Nodes.Clear();
                InitTree();
            }
        }

        private void удалитьГруппуИзКлассаToolStripMenuItem_Click(object sender, EventArgs e) // удалить группу
        {
            EquGroup _EquGroup = curEquTreeNode.ObjectDB as EquGroup;
            EquClass _EquClass = (curEquTreeNode.Parent as EquTreeNode).ObjectDB as EquClass;

            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить группу - \"" + _EquGroup.Name + "\" из Базы Данных? При подтверждении, таже все зависимые от группы объекты будут удалены из Базы данных.");
            
            if (result == Equipment.MessageBoxResult.Yes)
            {
                var empData1 = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == _EquClass.Code select new { r.GroupNum }).Distinct();
                _db_controller.all_equipment_adapter.delGroup(_EquClass.Code, _EquGroup.Code, empData1.Count());

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
            EquClass _EquClass = curEquTreeNode.ObjectDB as EquClass;

            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить класс - \"" + _EquClass.Name + "\" из Базы Данных? При подтверждении, также все зависимые от класса объекты будут удалены из Базы данных.");
            if (result == Equipment.MessageBoxResult.Yes)
            {
                _db_controller.classes_adapter.delClass(_EquClass.Code);

                _db_controller.refresh();
                TreeNode sn = treeView1.SelectedNode;
                treeView1.Nodes.Remove(sn);
                treeView1.Update();
            }
        }
      
        private void переименоватьToolStripMenuItem4_Click(object sender, EventArgs e) 
        {
            renameWrapper();
        }
     
        public void renameWrapper()
        {
            TreeNode TN = curEquTreeNode as TreeNode;
            if (curEquTreeNode != null)
            {
                treeView1.SelectedNode = TN;
                treeView1.LabelEdit = true;

                if (!TN.IsEditing)
                    TN.BeginEdit();
            }
            else
                MessageBox.Show("Для переименования необходимо выбрать объект");
        }

        private void treeView1_AfterLabelEdit(object sender, System.Windows.Forms.NodeLabelEditEventArgs e)
        {
            if (e.Label != null)
            {
                if (e.Label.Length > 0)
                {
                    if (e.Label.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*' }) == -1)
                    {

                        switch (curEquTreeNode.ObjectDB.GetType().ToString())
                        {
                            case "Registrator.EquClass":
                                //EquClass classTmp = (EquClass)mySelectedNode;
                                string newClassName = e.Label.Trim();
                                var machesClass = from r in _db_controller.classes_table.AsEnumerable() where r.Class == newClassName select new { r.Code };

                                if (machesClass.Count() == 0)
                                {
                                    _db_controller.classes_adapter.renameClass(e.Label, curEquTreeNode.ObjectDB.Code);
                                    e.Node.EndEdit(false);
                                }
                                else
                                {
                                    e.CancelEdit = true;
                                    MessageBox.Show("Класс с таким именем уже существует", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                }
                                
                                break;

                            case "Registrator.EquGroup": // new picket
                                //EquGroup groupTmp = (EquGroup)mySelectedNode;
                                string newGroupName = e.Label.Trim();
                                var machesGroup = from r in _db_controller.groups_table.AsEnumerable() where r.Group == newGroupName select new { r.Code };

                                if (machesGroup.Count() == 0)
                                {
                                    _db_controller.groups_adapter.renameGroup(e.Label, (short)curEquTreeNode.ObjectDB.Code);
                                    e.Node.EndEdit(false);
                                }
                                else
                                {
                                    e.CancelEdit = true;
                                    MessageBox.Show("Группа с таким именем уже существует", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                }
                                
                                break;

                            case "Registrator.EquPath":
                                int trackNum = 0;
                                if (int.TryParse(e.Label, out trackNum))
                                {
                                    var machesTrack = from r in _db_controller.all_equipment_table.AsEnumerable() where r.Track == trackNum select new { r.Code };
                                    
                                    if (machesTrack.Count() == 0)
                                    {
                                        EquPath _EquPath = curEquTreeNode.ObjectDB as EquPath;
                                        EquLine _EquLine = (curEquTreeNode.Parent as EquTreeNode).ObjectDB as EquLine;
                                        EquGroup _EquGroup = (curEquTreeNode.Parent.Parent as EquTreeNode).ObjectDB as EquGroup;
                                        EquClass _EquClass = (curEquTreeNode.Parent.Parent.Parent as EquTreeNode).ObjectDB as EquClass;

                                        _db_controller.all_equipment_adapter.renameTrack(trackNum, _EquPath.Code, _EquClass.Code, _EquGroup.Code, _EquLine.Code);
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
            form_addClass = new Equipment.AddClass(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView));
            form_addClass.ShowDialog();
        }

        private void addStrelka_Click(object sender, EventArgs e)
        {
            curEquTreeNode = curEquTreeNode.Parent as EquTreeNode;
            form_Strelka = new Equipment.addStrelka(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView), curEquTreeNode);
            form_Strelka.ShowDialog();
        }

        private void добавитьСтрелкуToolStripMenuItem_Click(object sender, EventArgs e)
        {
            form_Strelka = new Equipment.addStrelka(_db_controller, new AddObjectOnTreeView(addObjectOnTreeView), curEquTreeNode);
            form_Strelka.ShowDialog();
        }

        private void свойстваToolStripMenuItem_Click(object sender, EventArgs e)
        {
            form_properties.setProperties(curEquTreeNode);
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
            form_properties.setProperties(curEquTreeNode);
            form_properties.Show(DPanel, DockState.DockRight);
        }
        private void переименоватьToolStripMenuItem6_Click(object sender, EventArgs e) //equipment properties
        {
            form_properties.setProperties(curEquTreeNode);
            form_properties.Show(DPanel, DockState.DockRight);
        }
        private void переименоватьToolStripMenuItem3_Click(object sender, EventArgs e) //properties of Line
        {
            form_properties.setProperties(curEquTreeNode);
            form_properties.Show(DPanel, DockState.DockRight);
        }
        private void переименоватьToolStripMenuItem7_Click(object sender, EventArgs e) // переименовать путь
        {
            form_properties.setProperties(curEquTreeNode);
            form_properties.Show(DPanel, DockState.DockRight);
        }
        private void свойстваToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            form_properties.setProperties(curEquTreeNode);
            form_properties.Show(DPanel, DockState.DockRight);
        }
        private void RenameClassToolBarBtn_Click(object sender, EventArgs e) // переименовать класс
        {
            form_properties.setProperties(curEquTreeNode);
            form_properties.Show(DPanel, DockState.DockRight); 
        }

        private void toolStripButton5_Click(object sender, EventArgs e)
        {
            if (treeView1.SelectedNode != null)
            {
                curEquTreeNode = treeView1.SelectedNode as EquTreeNode;
                form_properties.setProperties(curEquTreeNode);
                form_properties.Show(DPanel, DockState.DockRight);
            }
        }

        private void addTrafficLight_toolStripMenu_Click(object sender, EventArgs e)
        {
            curEquTreeNode = curEquTreeNode.Parent as EquTreeNode;
            AddTrafficLight f = new AddTrafficLight(_db_controller, new AddObjectTreeView(AddObjectTreeView), curEquTreeNode, new EquTreeNode(contextMenuStrip_Equipment, form_properties));
            f.ShowDialog();
        }

        private void addTrafficLight_PicketToolStripMenu_Click(object sender, EventArgs e)
        {
            AddTrafficLight f = new AddTrafficLight(_db_controller, new AddObjectTreeView(AddObjectTreeView), curEquTreeNode, new EquTreeNode(contextMenuStrip_Equipment, form_properties));
            f.ShowDialog();
        }
    }
}
