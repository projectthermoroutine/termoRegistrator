using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;
using Registrator.Equipment;
using Registrator.Equipment.CreateDbObjectsCtrls;
using Registrator.DB;
using Registrator.DB.EFClasses;
using NLog;
using System.Data.Entity.Validation;

namespace Registrator.Equipment
{
    public partial class AllEquipmentTree2 : ToolWindow
    {
        public DB.metro_db_controller _db_controller;
        DB.metro_db_edit_controller _db_edit_controller;
        ContextMetroCard _dbContext;

        CreateClassForm form_addClass;
        CreateEquipmentForm form_newEquip;
        CreateEquipmentTypeForm formAddEquipType;
        CreateStrelkaForm form_Strelka;
        CreateGroupForm form_NewGruop;
        CreateTrackForm form_Track;
        CreateLineForm form_line;
        Equipment.Properties form_properties;
    
        DockPanel  DPanel;

        private static readonly Logger logger = LogManager.GetCurrentClassLogger();

        public AllEquipmentTree2(DB.metro_db_controller db_controller, DockPanel DockPanel_Arg)
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

            FillPath(PathTreeNode, _dbContext);
        }

        private void InitTree()
        {
            try
            {
                _dbContext = _db_controller.dbContext;
                create_classes_nodes(_dbContext);
            }
            catch (Exception e)
            {
                logger.ErrorException("InitTree", e);
            }
        }

        //1 - classes
        void create_classes_nodes(ContextMetroCard db)
        {
            var query = from c in db.Classes where c.Code != 0 select new {c.Code, c.Class1 };

            foreach (var q in query)
            {
                EquTreeNode ClassTreeNode = new EquTreeNode(mnuTextFile, new EquClass(Convert.ToInt32(q.Code), (String)q.Class1), form_properties);
                treeView1.Nodes.Add(ClassTreeNode);
                create_groups_nodes(ClassTreeNode,db);
            }
        }

        //2 - groups
        void create_groups_nodes(EquTreeNode ClassTreeNode, ContextMetroCard db)
        {
            EquClass equ_class = (EquClass)ClassTreeNode.ObjectDB;
            //var resGroup = (from r in _db_controller.all_equipment_table.AsEnumerable() where r.ClassNum == equ_class.Code && r.GroupNum != 0 select new { r.GroupNum, r.GrpName }).Distinct();

            var query = from g in db.Groups where g.Class == equ_class.Code select new { g.Code, g.Group1 };

            foreach (var g in query)
            {
                EquTreeNode GroupTreeNode = new EquTreeNode(contextMenuStrip_Group, new EquGroup(Convert.ToInt32(g.Code), (String)g.Group1, equ_class), form_properties);
                ClassTreeNode.Nodes.Add(GroupTreeNode);
                create_lines_nodes(GroupTreeNode, db);
            }
        }

        class CurTrackNum
        {
            public int? trackNum;
        }

        // 3 - lines
        void create_lines_nodes(EquTreeNode GroupTreeNode, ContextMetroCard db)
        {
            EquGroup equ_group = (EquGroup)GroupTreeNode.ObjectDB;
            EquClass equ_class = equ_group.Class;

            var queryLines = from l in db.Lines select new { l.LineNum };

            foreach (var num in queryLines)
            {
                var q_name = from l in db.Lines where l.LineNum == num.LineNum select new { l.LineName };

                if (q_name.Count() == 1)
                {
                    EquTreeNode LineTreeNode = new EquTreeNode( 
                        contextMenuStrip_Line,
                        new EquLine(
                            Convert.ToInt32(num.LineNum),
                            String.Concat(new object[] { Convert.ToString(q_name.First().LineName) }),
                            equ_group),
                        form_properties);

                    GroupTreeNode.Nodes.Add(LineTreeNode);

                    var trackContext = (from t in db.Tracks where t.LineId == num.LineNum select new CurTrackNum {trackNum = t.ID }).Distinct();

                    create_tracks_nodes(LineTreeNode, db, trackContext);
                }
                else
                {
                    ///TODO log error
                }
            }
        }

        // 4 - tracks
        void create_tracks_nodes(EquTreeNode LineTreeNode, ContextMetroCard db , IQueryable<CurTrackNum> context)
        {
            foreach (var curNum in context)
            {
                var qTrack = from t in db.Tracks where t.ID == curNum.trackNum select new { t.Track1 };

                if (qTrack.Count() == 1)
                {
                    EquTreeNode PathTreeNode = new EquTreeNode( contextMenuStrip_Path,
                                                                new EquPath(Convert.ToInt32(curNum.trackNum),
                                                                            String.Concat(new object[] { qTrack.First().Track1 }),
                                                                            (EquLine)LineTreeNode.ObjectDB),
                                                                form_properties);
                    LineTreeNode.Nodes.Add(PathTreeNode);

                    //var trackContext = (from _c in context where _c.classNum == context.First().classNum && _c.groupNum == context.First().groupNum && _c.lineNum == curLine && _c.trackNum == curNum.TrackNum && _c.equipCode != 0 select new CurContext()).Distinct();
                    FillPath(PathTreeNode, db);
                }
                else
                {
                    //TODO log error
                }

            }
        }

        void FillPath(EquTreeNode Path, ContextMetroCard db)
        {
            PicketsManager PM = new PicketsManager(_db_controller);

            PM.createLogicalPicketList((EquPath)Path.ObjectDB);
            //PM.Matching(equ_path);

            create_pickets_nodes(PM.PicketsList, db, Path);
        }

        void create_pickets_nodes(IEnumerable<Picket> pickets, ContextMetroCard db, EquTreeNode _curPath)
        {
            if (_curPath.ObjectDB == null ||  _curPath.ObjectDB.GetType() != typeof(EquPath) )
                return;

            foreach (Picket p in pickets)
            {
                var PicketTreeNode = new EquTreeNode(contextMenuStrip_Picket, new EquPicket()
                                                                                {
                                                                                    after = p.NpicketAfter,
                                                                                    before = p.NpicketBefore,
                                                                                    keyNumber = p.number,
                                                                                    LeftLineShift = p.StartShiftLine,
                                                                                    RightLineShift = p.EndShiftLine,
                                                                                    npicket = p.Npiketa,
                                                                                    Code = p.number,
                                                                                    lenght = p.Dlina,
                                                                                    Name = p.Npiketa,
                                                                                    Path = (EquPath)_curPath.ObjectDB,
                                                                                    Parent = _curPath.ObjectDB
                                                                                }, 
                                                                                form_properties);

                                                                                _curPath.Nodes.Add(PicketTreeNode);
                                                                                CreateObjectsNodes(PicketTreeNode,db);
            }
        }

        void CreateObjectsNodes(EquTreeNode PicketTreeNode, ContextMetroCard db)
        {
            EquPicket equ_picket = (EquPicket)PicketTreeNode.ObjectDB;
            EquPath equ_path = equ_picket.Path;
            EquLine equ_line = equ_path.Line;
            EquGroup equ_group = equ_line.Group;
            EquClass equ_class = equ_group.Class;

            var queryEquipByGroup = (from g in db.Equipments where g.Group == equ_group.Code select g.Code).Distinct().ToList();
            var queryByPicket = from e in db.Equipments where queryEquipByGroup.Contains(e.Code) && e.Picket == equ_picket.Code select e;

            foreach (var curNum in queryByPicket)
            {
                var equips = from e in db.Equipments where e.Code == curNum.Code select new { e.Code, e.Name, e.EquipTypeID, e.EquipLenght };

                if (equips.Count() == 1)
                {
                    var item = equips.First();

                    EquObject obj = new EquObject(Convert.ToInt32(item.Code), item.Name, equ_picket, 0)
                    {
                        typeEquip = (int)item.EquipTypeID,
                        ObjectLenght = (int)item.EquipLenght
                    };

                    PicketTreeNode.Nodes.Add(new EquTreeNode(contextMenuStrip_Equipment, obj, form_properties));
                }
                else
                {
                    ///TODO log error
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
                    var item = _db_controller.dbContext.Equipments.Where(equip => equip.Code == elObj.Code).Distinct().FirstOrDefault();

                    elObj.Offset = item.shiftFromPicket;
                    elObj.State = (byte)item.EquipWorkState;

                    elObj.ObjectLenght = item.EquipLenght;
                    elObj.strelkaDirection = item.strelkaLeftOrRight;
                    elObj.X = item.Area_X;
                    elObj.Y = item.Area_Y;
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
            treeView1.Nodes.Clear();
            InitTree();
            //treeView1.SelectedNode = sn;
        }

 #region functions of removal of objects
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

            if (_db_edit_controller.deleteEquipmentAsType(_EquObject))
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

            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить пикет: \"" + _EquPicket.keyNumber + "\" из Базы Данных?");

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

            //if (_db_edit_controller.deleteLine(_EquLine))
            //        updateTreeView();
            
        }
       
        private void удалитьЛиниюИзБазыДанныхToolStripMenuItem_Click(object sender, EventArgs e) //  удалить линию из БД
        {
            EquLine _EquLine = curEquTreeNode.ObjectDB as EquLine;

            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить линию - \"" + _EquLine.Name + "\" из текущей группы?  При подтверждении, только зависимые от линии в текущей группе объекты будут удалены из Базы данных.");
            if (result != Equipment.MessageBoxResult.Yes)
                    return;

            //if (_db_edit_controller.deleteLineFromDB(_EquLine))
            if (Convert.ToBoolean(_db_edit_controller.queriesAdapter.DeleteLineSQL(_EquLine.Code)))
            {
                treeView1.Nodes.Clear();
                InitTree();
            }
            else
            {
                ///TODO Error 
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
 #endregion functions of removal of objects

        #region addObjects
        private void ToolStripMenuItemAddClass_Click(object sender, EventArgs e)
        {
            curEquTreeNode = new EquTreeNode(mnuTextFile, new EquClass(), form_properties);

            form_addClass = new CreateClassForm(_db_controller, null);
            form_addClass.EquObjectAddedEvent += ClassAdded;
            form_addClass.ShowDialog();
        }
        void ClassAdded(object sender, DbObjectEventArg e)
        {
            EquClass equ_object = e.DbObject as EquClass;
            EquTreeNode ClassTreeNode = new EquTreeNode(mnuTextFile, equ_object, form_properties);
            ClassTreeNode.ObjectDB = equ_object;
            treeView1.Nodes.Add(ClassTreeNode);
        }


        private void addGroupToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (curEquTreeNode.ObjectDB.GetType() == typeof(EquGroup))
                curEquTreeNode = curEquTreeNode.Parent as EquTreeNode;

            form_NewGruop = new CreateGroupForm(_db_controller, curEquTreeNode.ObjectDB);
            form_NewGruop.EquObjectAddedEvent += GroupAdded;
            form_NewGruop.ShowDialog();
        }

        void GroupAdded(object sender, DbObjectEventArg e)
        {
            EquGroup equ_object = e.DbObject as EquGroup;
            EquTreeNode GroupTreeNode = new EquTreeNode(contextMenuStrip_Group, form_properties);
            GroupTreeNode.ObjectDB = equ_object;

            curEquTreeNode.Nodes.Add(GroupTreeNode);
        }

        private void addLineToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (curEquTreeNode.ObjectDB.GetType() == typeof(EquLine))
                curEquTreeNode = curEquTreeNode.Parent as EquTreeNode;

            form_line = new CreateLineForm(_db_edit_controller, curEquTreeNode.ObjectDB);
            form_line.EquObjectAddedEvent += LineAdded;
            form_line.ShowDialog();
        }
        void LineAdded(object sender, DbObjectEventArg e)
        {
            EquLine equ_object = e.DbObject as EquLine;
            EquTreeNode LineTreeNode = new EquTreeNode(contextMenuStrip_Line, form_properties);
            LineTreeNode.ObjectDB = equ_object;
            curEquTreeNode.Nodes.Add(LineTreeNode);
        }
       
        private void addPathToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (curEquTreeNode.ObjectDB.GetType() == typeof(EquPath))
                curEquTreeNode = curEquTreeNode.Parent as EquTreeNode;

            form_Track = new CreateTrackForm(_db_controller, curEquTreeNode.ObjectDB);
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

            foreach (EquDbObject p in e.DbObjects)
            {
                EquTreeNode picketTreeNode = new EquTreeNode(contextMenuStrip_Picket,form_properties);
                p.Parent = (curEquTreeNode.Nodes[curEquTreeNode.Nodes.Count - 1] as EquTreeNode).ObjectDB as EquPath;
                picketTreeNode.ObjectDB = p as EquPicket;
                curEquTreeNode.Nodes[curEquTreeNode.Nodes.Count - 1].Nodes.Add(picketTreeNode);
            }
        }

        class TreeNodeTraits<T> where T:EquDbObject
        {
            public TreeNodeTraits(EquDbObject obj) { Equal = obj.GetType() == typeof(EquPicket); }
            public bool Equal { get; private set; }
        }

        private void addPicketToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //var traits = new TreeNodeTraits<EquPicket>(curEquTreeNode.ObjectDB);
            //if (!traits.Equal)
            //    return;
            if (curEquTreeNode.ObjectDB.GetType() == typeof(EquPicket))
                curEquTreeNode = curEquTreeNode.Parent as EquTreeNode;

            var formPicket = new CreatePicketForm(_db_controller, curEquTreeNode.ObjectDB, _dbContext);
            formPicket.PicketAddedEvent += PicketAdded;
            formPicket.ShowDialog();
        }

        void PicketAdded(object sender, DbObjectEventArg e)
        {
            for (int i = 0; i < e.DbObjects.Count(); i++ )
            {
                EquPicket picket_object = e.DbObjects[i] as EquPicket;
                picket_object.Parent = curEquTreeNode.ObjectDB as EquPath;
                EquTreeNode picketTreeNode = new EquTreeNode(contextMenuStrip_Picket, form_properties);
                picketTreeNode.ObjectDB = picket_object;

                if (e.leftPicket)
                    curEquTreeNode.Nodes.Insert(i, picketTreeNode);
                else
                    curEquTreeNode.Nodes.Add(picketTreeNode);
            }
        }

        private void addEquipmentToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (curEquTreeNode.ObjectDB.GetType() == typeof(EquObject))
                curEquTreeNode = curEquTreeNode.Parent as EquTreeNode;

            form_newEquip = new CreateEquipmentForm(_db_controller, curEquTreeNode.ObjectDB);
            form_newEquip.EquObjectAddedEvent += EquipmentAdded;
            form_newEquip.ShowDialog();
        }
     
        void EquipmentAdded(object sender, DbObjectEventArg e)
        {
            EquObject obj = e.DbObject as EquObject;
          
            EquTreeNode ObjTreeNode = new EquTreeNode(contextMenuStrip_Equipment, form_properties);
            ObjTreeNode.ObjectDB = obj;

            curEquTreeNode.Nodes.Add(ObjTreeNode);
        }

        private void addStrelka_Click(object sender, EventArgs e)
        {
            if (curEquTreeNode.ObjectDB.GetType() == typeof(EquObject))
                curEquTreeNode = curEquTreeNode.Parent as EquTreeNode;

            form_Strelka = new CreateStrelkaForm(_db_controller, curEquTreeNode.ObjectDB);
            form_Strelka.EquObjectAddedEvent += EquipmentAdded;
            form_Strelka.ShowDialog();
        }

        private void addTrafficLight_toolStripMenu_Click(object sender, EventArgs e)
        {
            if (curEquTreeNode.ObjectDB.GetType() == typeof(EquObject))
                curEquTreeNode = curEquTreeNode.Parent as EquTreeNode;

            var f = new CreateTrafficLightForm(_db_controller, curEquTreeNode.ObjectDB);
            f.EquObjectAddedEvent += EquipmentAdded;
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

        private void treeView1_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            EquTreeNode ObjectTreeNode = (EquTreeNode)e.Node;

            if (form_properties.setProperties(ObjectTreeNode))
                form_properties.Show(DPanel, DockState.DockRight);
        }

        private void AllEquipmentTree2_VisibleChanged(object sender, EventArgs e)
        {
            if(!Visible)
                form_properties.Hide();
        }

        private void добавитьТипОборудованияToolStripMenuItem_Click(object sender, EventArgs e)
        {
            formAddEquipType = new CreateEquipmentTypeForm(_db_controller);
            //form_newEquip.EquObjectAddedEvent += EquipmentAdded;
            formAddEquipType.ShowDialog();
        }
    }
}
