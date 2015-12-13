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
    public delegate void AddPicketOnTreeView(List<EquTreeNode> obj, bool first);
    public partial class AllEquipmentTree : ToolWindow
    {
        public DB.metro_db_controller _db_controller;
        DB.metro_db_edit_controller _db_edit_controller;

        AddClassForm form_addClass;
        AddEquipmentForm form_newEquip;
        addStrelkaForm form_Strelka;
        AddGruopForm form_NewGruop;
        AddTrackForm form_Track;
        AddLineForm form_line;
        Equipment.Properties form_properties;
    
        DockPanel  DPanel;

        public AllEquipmentTree(DB.metro_db_controller db_controller, DockPanel DockPanel_Arg)
        {
            InitializeComponent();
            
            _db_controller      = new DB.metro_db_controller(db_controller);
            _db_edit_controller = new DB.metro_db_edit_controller(_db_controller);

            form_properties_FormClosing(null, null);

            InitTree();

            this.treeView1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.treeView1_MouseUp);

            DPanel = DockPanel_Arg;
        }

        void form_properties_FormClosing(object sender, EventArgs e)
        {
            form_properties = new Equipment.Properties(_db_controller);
            form_properties.FormClosing += form_properties_FormClosing;
            form_properties.groupSettings.RenameEventHandler += RenameEventHandler;
            form_properties.lineSettings.RenameEventHandler += RenameEventHandler;
            form_properties.equipSettings.RenameEventHandler += RenameEventHandler;
            form_properties.equipExtSettings.RenameEventHandler += RenameEventHandler;
            form_properties.classSettings.RenameEventHandler += RenameEventHandler;
            form_properties.strelkaSettings.RenameEventHandler += RenameEventHandler;
            form_properties.pathSettings.RenameEventHandler += RenameEventHandler;
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
                                                                    String.Concat(new object[] {Convert.ToString(itemGroup.LineName)}),
                                                                    GroupTreeNode.ObjectDB),
                                                                    form_properties);

                        GroupTreeNode.Nodes.Add(LineTreeNode);

                        var resTrack = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == ((EquClass)ClassTreeNode.ObjectDB).Code && r.GroupNum == ((EquGroup)GroupTreeNode.ObjectDB).Code && r.LineNum == ((EquLine)LineTreeNode.ObjectDB).Code && r.Track != 0 select new { r.Track }).Distinct();

                        foreach (var itemTrack in resTrack)
                        {

                            var resTrackName = (from r in _db_controller.trackTable.AsEnumerable() where r.ID == itemTrack.Track select new { r.Track });

                            EquTreeNode PathTreeNode = new EquTreeNode( contextMenuStrip_Path,
                                                                        new EquPath(Convert.ToInt32(itemTrack.Track),
                                                                        String.Concat(new object[] { resTrackName.First().Track }),
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
                                               select new EquPicket( r.PicketDisplayNumber, r.number, r.NpicketAfter, r.NpicketBefore, r.StartShiftLine, r.EndShiftLine, r.Dlina, Path.ObjectDB)).GroupBy(x => x.number).Select(g => g.First());

            PM.createLogicalPicketList((EquPath)Path.ObjectDB);
            PM.Matching((EquPath)Path.ObjectDB);

            FillPickets(PM.PicketsList, Path);
        }

        void FillPickets(IEnumerable<EquPicket> pickets, EquTreeNode _curPath)
        {
            if (_curPath.ObjectDB == null ||  _curPath.ObjectDB.GetType() != typeof(EquPath) )
                return;

            var curPath  = _curPath.ObjectDB as EquPath;
            var curLine  = curPath.Line;
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
                    
                    elObj.Offset = item.shiftFromPicket;
                    elObj.State = (byte)item.regularly;

                    elObj.ObjectLenght = item.ObjectLenght;
                    elObj.strelkaDirection = item.strelkaLeftOrRight;
                    elObj.X = item.x;
                    elObj.Y = item.y;
                    elObj.MaxTemperature = item.maxTemperature;

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

        void AddObjectTreeView(EquTreeNode obj)
        {
            curEquTreeNode.Nodes.Add(obj);
        }
        void AddClassTreeView(EquTreeNode obj)
        {
                treeView1.Nodes.Add(obj);
        }
        void AddPicketTreeView(List<EquTreeNode> pickets, bool addToLeftSide)
        {
            if (addToLeftSide)
            {
                for (int i = 0; i < pickets.Count(); i++)
                    curEquTreeNode.Nodes.Insert(i, pickets[i]);
            }
            else
            {
                for (int i = 0; i < pickets.Count(); i++)
                    curEquTreeNode.Nodes.Add(pickets[i]);
            }
        }

        private void обновитьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //TreeNode sn = treeView1.SelectedNode.Parent;
            _db_controller.refresh();
            treeView1.Nodes.Clear();
            InitTree();
            //treeView1.SelectedNode = sn;
        }

#region delete objects functions
        private void DeleteEquipFromPicket_Click(object sender, EventArgs e)
        {
            EquObject _EquObject = curEquTreeNode.ObjectDB as EquObject;

            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить оборудование - \"" + _EquObject.Name + "\" из текущего пикета?");

            if (result != Equipment.MessageBoxResult.Yes)
                return;

            if (_db_edit_controller.deleteEquipmentFromPicket(_EquObject))
            {
                updateTreeView();
            }
        }

        private void удалитьОборудованиеИзБазыДанныхToolStripMenuItem1_Click(object sender, EventArgs e) // удалить оборудование из БД
        {
            EquObject _EquObject = curEquTreeNode.ObjectDB as EquObject;

            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить объект - \"" + _EquObject.Name + "\" из Базы Данных?  При подтверждении, оборудование будет удалено из всей группы.");

            if (result != Equipment.MessageBoxResult.Yes)
                return;

            if (_db_edit_controller.deleteEquipmentFromDB(_EquObject))
            {
                TreeNode sn = treeView1.SelectedNode;
                treeView1.Nodes.Clear();
                InitTree();
            }
        }
        private void deletePicketFromDataBase_Click(object sender, EventArgs e)
        {
            EquPicket _EquPicket = curEquTreeNode.ObjectDB as EquPicket;

            if (_EquPicket.after != 0 && _EquPicket.before != 0)
            {
                MessageBox.Show("Информация", "Удалить можно первый, либо последний пикеты");
                return;
            }

            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить пикет: \"" + _EquPicket.number + "\" из Базы Данных?");

            if (result != Equipment.MessageBoxResult.Yes)
                return;

            if (_db_edit_controller.deletePicketFromDataBase(_EquPicket))
            {
                treeView1.Nodes.Clear();
                InitTree();
            }
        }
        private void удалитьПутьИзЛинииToolStripMenuItem_Click(object sender, EventArgs e) // удалить пути из выбранной линии
        {
            EquPath _EquPath = curEquTreeNode.ObjectDB as EquPath;
            
            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить путь - \"" + _EquPath.Name + "\" из текущей группы? Все зависимые от пути объекты будут удалены");
            if (result != Equipment.MessageBoxResult.Yes)
                return;

            if (_db_edit_controller.deletePathFromLine(_EquPath))
                updateTreeView();

        }
        private void удалитьЛиниюИзГруппыToolStripMenuItem_Click(object sender, EventArgs e) // удалить линию из группы
        {
            EquLine  _EquLine = curEquTreeNode.ObjectDB as EquLine;

            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить линию - \"" + _EquLine.Name + "\" из Базы Данных?  При подтверждении, также и все зависимые от линии объекты, любой группы, будут удалены из Базы данных.");

            if (result != Equipment.MessageBoxResult.Yes)
                return;

            if (_db_edit_controller.deleteLineFromGroup(_EquLine))
                    updateTreeView();
            
        }
       
        private void удалитьЛиниюИзБазыДанныхToolStripMenuItem_Click(object sender, EventArgs e) //  удалить линию из БД
        {
            EquLine _EquLine = curEquTreeNode.ObjectDB as EquLine;

            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить линию - \"" + _EquLine.Name + "\" из текущей группы?  При подтверждении, только зависимые от линии в текущей группе объекты будут удалены из Базы данных.");
            if (result != Equipment.MessageBoxResult.Yes)
                    return;

            if (_db_edit_controller.deleteLineFromDB(_EquLine))
            {
                treeView1.Nodes.Clear();
                InitTree();
            }
        }

        private void удалитьГруппуИзКлассаToolStripMenuItem_Click(object sender, EventArgs e) // удалить группу
        {
            EquGroup _EquGroup = curEquTreeNode.ObjectDB as EquGroup;

            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить группу - \"" + _EquGroup.Name + "\" из Базы Данных? При подтверждении, таже все зависимые от группы объекты будут удалены из Базы данных.");

            if (result != Equipment.MessageBoxResult.Yes)
                return;

            if (_db_edit_controller.deleteGroupFromClass(_EquGroup))
                updateTreeView();
            
        }

        private void ToolStripMenuDeleteClass_Click(object sender, EventArgs e) // удалить класс
        {
            EquClass _EquClass = curEquTreeNode.ObjectDB as EquClass;

            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить класс - \"" + _EquClass.Name + "\" из Базы Данных? При подтверждении, также все зависимые от класса объекты будут удалены из Базы данных.");
            if (result != Equipment.MessageBoxResult.Yes)
                return;

            if(_db_edit_controller.deleteClass(_EquClass))
                updateTreeView();
        }
#endregion delete objects functions

#region addObjects
        private void ToolStripMenuItemAddClass_Click(object sender, EventArgs e)
        {
            curEquTreeNode = new EquTreeNode(mnuTextFile, new EquClass(), form_properties);

            form_addClass = new Equipment.AddClassForm(_db_controller, new AddObjectTreeView(AddClassTreeView), curEquTreeNode);
            form_addClass.ShowDialog();
        }

        private void addGroupToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (curEquTreeNode.ObjectDB.GetType() == typeof(EquGroup))
                curEquTreeNode = curEquTreeNode.Parent as EquTreeNode;

            form_NewGruop = new AddGruopForm(_db_controller, new AddObjectTreeView(AddObjectTreeView), curEquTreeNode, new EquTreeNode(contextMenuStrip_Group,form_properties));
            form_NewGruop.ShowDialog();
        }

        private void addLineToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (curEquTreeNode.ObjectDB.GetType() == typeof(EquLine))
                curEquTreeNode = curEquTreeNode.Parent as EquTreeNode;

            form_line = new Equipment.AddLineForm(_db_controller, new AddObjectTreeView(AddObjectTreeView), curEquTreeNode, new EquTreeNode(contextMenuStrip_Line,form_properties));
            form_line.ShowDialog();
        }
       
        private void addPathToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (curEquTreeNode.ObjectDB.GetType() == typeof(EquPath))
                curEquTreeNode = curEquTreeNode.Parent as EquTreeNode;

            form_Track = new Equipment.AddTrackForm(_db_controller, new AddObjectTreeView(AddObjectTreeView), curEquTreeNode.ObjectDB, new EquTreeNode(contextMenuStrip_Path, form_properties),new EquTreeNode(contextMenuStrip_Picket,form_properties));
            form_Track.ShowDialog();
        }
       
        private void addPicketToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (curEquTreeNode.ObjectDB.GetType() == typeof(EquPicket))
                curEquTreeNode = curEquTreeNode.Parent as EquTreeNode;

            Equipment.AddPicketForm formPicket = new Equipment.AddPicketForm(_db_controller, new AddPicketOnTreeView(AddPicketTreeView), curEquTreeNode, new EquTreeNode(contextMenuStrip_Picket,form_properties));
            formPicket.ShowDialog();
        }
      
        private void addEquipmentToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (curEquTreeNode.ObjectDB.GetType() == typeof(EquObject))
                curEquTreeNode = curEquTreeNode.Parent as EquTreeNode;

            form_newEquip = new Equipment.AddEquipmentForm(_db_controller, new AddObjectTreeView(AddObjectTreeView), curEquTreeNode,new EquTreeNode(contextMenuStrip_Equipment,form_properties));
            form_newEquip.ShowDialog();
        }
     
        private void addStrelka_Click(object sender, EventArgs e)
        {
            if (curEquTreeNode.ObjectDB.GetType() == typeof(EquObject))
                curEquTreeNode = curEquTreeNode.Parent as EquTreeNode;

            form_Strelka = new Equipment.addStrelkaForm(_db_controller, new AddObjectTreeView(AddObjectTreeView), curEquTreeNode,new EquTreeNode(contextMenuStrip_Equipment,form_properties));
            form_Strelka.ShowDialog();
        }

        private void addTrafficLight_toolStripMenu_Click(object sender, EventArgs e)
        {
            if (curEquTreeNode.ObjectDB.GetType() == typeof(EquObject))
                curEquTreeNode = curEquTreeNode.Parent as EquTreeNode;

            AddTrafficLightForm f = new AddTrafficLightForm(_db_controller, new AddObjectTreeView(AddObjectTreeView), curEquTreeNode, new EquTreeNode(contextMenuStrip_Equipment, form_properties));
            f.ShowDialog();
        }

#endregion addObjects

        void updateTreeView()
        {
            TreeNode sn = treeView1.SelectedNode;
            treeView1.Nodes.Remove(sn);
            treeView1.Update();
        }

        void RenameEventHandler(object sender, Equipment.RenameEvent e)
        {
            _db_controller.refresh();
            treeView1.Nodes.Clear();
            InitTree();
        }

        private void toolStripMenu_item_properties_click(object sender, EventArgs e)
        {
            form_properties.setProperties(curEquTreeNode);
            form_properties.Show(DPanel, DockState.DockRight);
        }

        private void toolStripButton_Properties_Click(object sender, EventArgs e)
        {
            if (treeView1.SelectedNode != null)
            {
                curEquTreeNode = treeView1.SelectedNode as EquTreeNode;
                form_properties.setProperties(curEquTreeNode);
                form_properties.Show(DPanel, DockState.DockRight);
            }
        }
    }
}
