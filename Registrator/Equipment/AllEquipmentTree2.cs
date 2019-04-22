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
using static Registrator.Equipment.PicketSettings;

namespace Registrator.Equipment
{
    public partial class AllEquipmentTree2 : DockContent
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
        Equipment.DBProperties form_properties;
    
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
            form_properties = new Equipment.DBProperties(_db_controller);
            form_properties.FormClosing += form_properties_FormClosing;
            form_properties.picketSettings.ChangeLenghtEvent += ChangePicketLengthEventHandler;
        }

        void ChangePicketLengthEventHandler(object s, MyEventArgs e )
        {
            EquTreeNode PathTreeNode = (e.picketTreeNode.Parent as EquTreeNode);
            PathTreeNode.Nodes.Clear();

            FillPath(PathTreeNode, _dbContext);
        }


        EquTreeNode tree_node_title_classes;

        private void InitTree()
        {
            try
            {
                _dbContext = _db_controller.dbContext;

                create_equips_title_nodes();

                tree_node_title_classes = create_class_title_node();

                create_classes_nodes(_dbContext, tree_node_title_classes);
            }
            catch (Exception e)
            {
                logger.ErrorException("InitTree", e);
            }
        }

        EquTreeNode tree_nodes_title_equipment;
        EquTreeNode tree_nodes_title_strelka;
        EquTreeNode tree_nodes_title_traffic_light;


        // ------------------- All Equipments presented in database -------------------
        void create_equips_title_nodes()
        {
            EquTreeNode EquipTreeNode = new EquTreeNode(new ContextMenuStrip(), "Типы оборудования");

            treeView1.Nodes.Add(EquipTreeNode);

            tree_nodes_title_equipment = new EquTreeNode(contextMenuStrip_createEquipmentClass, "Оборудование");
            tree_nodes_title_strelka = new EquTreeNode(contextMenuStrip_createStrelkaType, "Стрелки");
            tree_nodes_title_traffic_light = new EquTreeNode(contextMenuStrip_trafficLightType, "Светофоры");


            if(_dbContext.EquipmentsClasses.Where(e=>e.EquipType == (int)EQUIPS_TYPES.Equipment).Any())
                create_stub_node(tree_nodes_title_equipment, _dbContext, EquTreeNodeStrings.StubTags.equipments_title);
            if (_dbContext.EquipmentsClasses.Where(e => e.EquipType == (int)EQUIPS_TYPES.Strelka).Any())
                create_stub_node(tree_nodes_title_strelka, _dbContext, EquTreeNodeStrings.StubTags.strelka_title);
            if (_dbContext.EquipmentsClasses.Where(e => e.EquipType == (int)EQUIPS_TYPES.TrafficLight).Any())
                create_stub_node(tree_nodes_title_traffic_light, _dbContext, EquTreeNodeStrings.StubTags.traffic_light_title);

            EquipTreeNode.Nodes.Add(tree_nodes_title_equipment);
            EquipTreeNode.Nodes.Add(tree_nodes_title_strelka);
            EquipTreeNode.Nodes.Add(tree_nodes_title_traffic_light);

            return;
        }

        void create_stub_node(EquTreeNode treeNode, ContextMetroCard db, string tag)
        {
            treeNode.Tag = tag;
            EquTreeNode stubTreeNode = new EquTreeNode(
                    new ContextMenuStrip(),
                    new EquDbObject(
                        0,
                        "stub",
                        (EquLine)treeNode.ObjectDB),
                    form_properties);

            stubTreeNode.Tag = "stub";
            treeNode.Nodes.Add(stubTreeNode);
        }

        EquTreeNode create_class_title_node()
        {
            EquTreeNode ClassesTreeNode = new EquTreeNode(contextMenuStrip_Classes, "Классы");

            treeView1.Nodes.Add(ClassesTreeNode);

            return ClassesTreeNode;
        }

        void CreateAllTrafficLightNodes(EquTreeNode parentNode, ContextMetroCard db)
        {
            var queryByPicket = (from e in db.EquipmentsClasses where e.EquipType == (int)EQUIPS_TYPES.TrafficLight select e).Distinct();

            foreach (var item in queryByPicket)
            {
                EquipmentObject obj = new EquipmentObject(Convert.ToInt32(item.Id), item.Name, "traffic_light");

                parentNode.Nodes.Add(new EquTreeNode(contextMenuStrip_trafficLightTypeChildNode, obj, form_properties));
            }

        }

        void CreateAllStrelkaNodes(EquTreeNode parentNode, ContextMetroCard db)
        {
            var queryByPicket = (from e in db.EquipmentsClasses where e.EquipType == (int)EQUIPS_TYPES.Strelka select e).Distinct();

            foreach (var item in queryByPicket)
            {
                EquipmentObject obj = new EquipmentObject(Convert.ToInt32(item.Id), item.Name, "strelka");

                parentNode.Nodes.Add(new EquTreeNode(contextMenuStrip__createStrelkaTypeChildNode, obj, form_properties));
            }

        }

        void CreateAllEquipmentsNodes(EquTreeNode parentNode, ContextMetroCard db)
        {
            var queryByPicket = (from e in db.EquipmentsClasses where e.EquipType== (int)EQUIPS_TYPES.Equipment select e).Distinct();

            foreach (var item in queryByPicket)
            {
                EquipmentObject obj = new EquipmentObject(Convert.ToInt32(item.Id), item.Name, "equip");

                parentNode.Nodes.Add(new EquTreeNode(contextMenuStrip_createEquipmentClassChildNode, obj, form_properties));
            }
        }

        //
        // OnClick  processing
        //
        private void treeView1_BeforeExpand(object sender, TreeViewCancelEventArgs e)
        {
            if (Convert.ToString(e.Node.Tag) == EquTreeNodeStrings.StubTags.line_stub)
            {
                BeforeExpandeTreeNode(e, create_tracks_nodes);
                return;
            }

            if (Convert.ToString(e.Node.Tag) == EquTreeNodeStrings.StubTags.equipments_title)
            {
                BeforeExpandeTreeNode(e, CreateAllEquipmentsNodes);
                return;
            }

            if (Convert.ToString(e.Node.Tag) == EquTreeNodeStrings.StubTags.strelka_title)
            {
                BeforeExpandeTreeNode(e, CreateAllStrelkaNodes);
                return;
            }

            if (Convert.ToString(e.Node.Tag) == EquTreeNodeStrings.StubTags.traffic_light_title)
            {
                BeforeExpandeTreeNode(e, CreateAllTrafficLightNodes);
                return;
            }
        }

        void BeforeExpandeTreeNode(TreeViewCancelEventArgs e, Action<EquTreeNode, ContextMetroCard> action)
        {
            EquTreeNode LineTreeNode = e.Node as EquTreeNode;

            if (LineTreeNode.Nodes.Count == 1)
            {
                if (Convert.ToString(LineTreeNode.Nodes[0].Tag) == "stub")
                {
                    LineTreeNode.Nodes.Clear();
                    action(LineTreeNode, _dbContext);
                }
            }
        }

        //------------------- Classes -------------------
        // 1 - classes
        void create_classes_nodes(ContextMetroCard db, EquTreeNode title_node)
        {
            var query = from c in db.Classes where c.Code != 0 select new {c.Code, c.Name };

            foreach (var q in query)
            {
                EquTreeNode ClassTreeNode = new EquTreeNode(contextMenuStrip_Classes, new EquClass(Convert.ToInt32(q.Code), (String)q.Name), form_properties);
                title_node.Nodes.Add(ClassTreeNode);
                create_groups_nodes(ClassTreeNode,db);
            }
        }

        //2 - groups
        void create_groups_nodes(EquTreeNode ClassTreeNode, ContextMetroCard db)
        {
            EquClass equ_class = (EquClass)ClassTreeNode.ObjectDB;

            var query = from g in db.Groups where g.ClassId == equ_class.Code select new { g.Code, g.Name };

            foreach (var g in query)
            {
                EquTreeNode GroupTreeNode = new EquTreeNode(contextMenuStrip_Group, new EquGroup(Convert.ToInt32(g.Code), (String)g.Name, equ_class), form_properties);
                ClassTreeNode.Nodes.Add(GroupTreeNode);
                create_lines_nodes(GroupTreeNode, db);
            }
        }



        // 3 - lines
        void create_lines_nodes(EquTreeNode GroupTreeNode, ContextMetroCard db)
        {
            EquGroup equ_group = (EquGroup)GroupTreeNode.ObjectDB;
            EquClass equ_class = equ_group.Class;

            var queryLines = (from l in db.Lines select l ).Distinct();

            foreach (var l in queryLines)
            {
                EquTreeNode LineTreeNode = new EquTreeNode( 
                    contextMenuStrip_Line,
                    new EquLine(
                        l.LineNum,
                        l.LineName,
                        equ_group), 
                    form_properties);

                GroupTreeNode.Nodes.Add(LineTreeNode);

                if(_dbContext.Tracks.Where(e => e.LineId == l.LineNum).Any())
                    create_stub_node(LineTreeNode, db, EquTreeNodeStrings.StubTags.line_stub);
            }
        }


        // 4 - tracks
        void create_tracks_nodes(EquTreeNode LineTreeNode, ContextMetroCard db)
        {
            var tracks = (from t in db.Tracks where t.LineId == LineTreeNode.ObjectDB.Code select t).Distinct();
            foreach (var t in tracks)
            {
                EquTreeNode PathTreeNode = new EquTreeNode( 
                    contextMenuStrip_Path,
                    new EquPath(
                        t.ID,
                        t.Name,
                        (EquLine)LineTreeNode.ObjectDB),
                    form_properties);

                LineTreeNode.Nodes.Add(PathTreeNode);

                FillPath(PathTreeNode, db);
            }
        }

        void FillPath(EquTreeNode Path, ContextMetroCard db)
        {
            create_pickets_nodes(db, Path);
        }

        void create_pickets_nodes(ContextMetroCard db, EquTreeNode _curPath)
        {
            if (_curPath.ObjectDB == null ||  _curPath.ObjectDB.GetType() != typeof(EquPath) )
                return;

            var pickets = _db_controller.PicketsByPath(_curPath.ObjectDB.Code);

            foreach (Picket p in pickets)
            {
                var PicketTreeNode = new EquTreeNode(
                    contextMenuStrip_Picket, 
                    new EquPicket()
                    {
                        after = p.NpicketAfter,
                        before = p.NpicketBefore,
                        keyNumber = p.number,
                        LeftLineShift = p.StartShiftLine,
                        RightLineShift = p.EndShiftLine,
                        npicket = p.Npiketa,
                        Code = p.number,
                        Length = p.Dlina,
                        Name = p.Npiketa,
                        Path = (EquPath)_curPath.ObjectDB,
                        Parent = _curPath.ObjectDB
                    }, 
                    form_properties);

                if(p.NonStandartLength)
                    PicketTreeNode.NodeFont = new Font(treeView1.Font, FontStyle.Bold);

                _curPath.Nodes.Add(PicketTreeNode);
                CreateObjectsNodes(PicketTreeNode,db);
            }

            if(_curPath.Nodes.Count > 0)
            {
                EquTreeNode first_node = (EquTreeNode)_curPath.Nodes[0];
                EquTreeNode last_node = (EquTreeNode)_curPath.Nodes[_curPath.Nodes.Count - 1];

                string pickets_range = " [" + first_node.ObjectDB.Name + " - " + last_node.ObjectDB.Name + "]";

                _curPath.Text += pickets_range;
            }

        }

        void CreateObjectsNodes(EquTreeNode PicketTreeNode, ContextMetroCard db)
        {
            EquPicket equ_picket = (EquPicket)PicketTreeNode.ObjectDB;
            EquPath equ_path = equ_picket.Path;
            EquLine equ_line = equ_path.Line;
            EquGroup equ_group = equ_line.Group;
            EquClass equ_class = equ_group.Class;

            var queryByPicket = (from e in db.AllEquipments where e.Group == equ_group.Code && e.Picket == equ_picket.Code select e).Distinct();

            foreach (var item in queryByPicket)
            {
                EquObject obj = new EquObject(Convert.ToInt32(item.Code), item.Name, equ_picket, 0)
                {
                    typeEquip = (EQUIPS_TYPES)item.EquipTypeID,
                    ObjectLenght = (int)item.EquipLenght
                };

                PicketTreeNode.Nodes.Add(new EquTreeNode(contextMenuStrip_EquipmentCreate, obj, form_properties));
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
                    var item = _db_controller.dbContext.AllEquipments.Where(equip => equip.Code == elObj.Code).Distinct().FirstOrDefault();

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

            if (_db_edit_controller.deleteEquipmentFromPicket(_EquObject.Code))
            {
                updateTreeView();
            }
        }

        private void deletePicketFromDataBase_Click(object sender, EventArgs e)
        {
            EquPicket _EquPicket = curEquTreeNode.ObjectDB as EquPicket;

            if (_EquPicket.after != 0 && _EquPicket.before != 0)
            {
                MessageBox.Show("Удалить можно первый, либо последний пикеты", "Информация", MessageBoxButtons.OK, MessageBoxIcon.Information );
                return;
            }

            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить пикет: \"" + _EquPicket.Name + "\" из Базы Данных?");

            if (result != Equipment.MessageBoxResult.Yes)
                return;

            _db_edit_controller.deletePicket(_EquPicket.Code);
            EquTreeNode PathEquTreeNode = curEquTreeNode.Parent as EquTreeNode;

            PathEquTreeNode.Nodes.Clear();
            create_pickets_nodes(_dbContext, PathEquTreeNode);
        }

        private void удалитьПутьИзЛинииToolStripMenuItem_Click(object sender, EventArgs e) // удалить пути из выбранной линии
        {
            EquPath _EquPath = curEquTreeNode.ObjectDB as EquPath;
            
            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить путь - \"" + _EquPath.Name + "\" из текущей группы? Все зависимые от пути объекты будут удалены");
            if (result != Equipment.MessageBoxResult.Yes)
                return;

            _db_edit_controller.DeletePath(_EquPath.Code);


            EquTreeNode LineNode = curEquTreeNode.Parent as EquTreeNode;
            LineNode.Nodes.Clear();

            create_tracks_nodes(LineNode, _dbContext);
        }

        private void удалитьЛиниюИзГруппыToolStripMenuItem_Click(object sender, EventArgs e) // удалить линию из группы
        {
            EquLine  _EquLine = curEquTreeNode.ObjectDB as EquLine;

            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить линию - \"" + _EquLine.Name + "\" из Базы Данных?  При подтверждении, также и все зависимые от линии объекты, любой группы, будут удалены из Базы данных.");

            if (result != Equipment.MessageBoxResult.Yes)
                return;

            _db_edit_controller.deleteLine(_EquLine.Code);
             updateTreeView();
            
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
                updateTreeView();
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
            curEquTreeNode = new EquTreeNode(contextMenuStrip_Classes, new EquClass(), form_properties);

            form_addClass = new CreateClassForm(_db_controller, null);
            form_addClass.EquObjectAddedEvent += ClassAdded;
            form_addClass.ShowDialog();
        }

        void ClassAdded(object sender, DbObjectEventArg e)
        {
            EquClass equ_object = e.DbObject as EquClass;
            EquTreeNode ClassTreeNode = new EquTreeNode(contextMenuStrip_Classes, equ_object, form_properties);
            ClassTreeNode.ObjectDB = equ_object;
            tree_node_title_classes.Nodes.Add(ClassTreeNode);
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
            EquTreeNode GroupTreeNode = new EquTreeNode(contextMenuStrip_Group, form_properties)
            {
                ObjectDB = equ_object
            };

            //curEquTreeNode.Nodes.Add(GroupTreeNode);

            treeView1.Nodes.Clear();
            InitTree();
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
            //EquLine equ_object = e.DbObject as EquLine;
            //EquTreeNode LineTreeNode = new EquTreeNode(contextMenuStrip_Line, form_properties);
            //LineTreeNode.ObjectDB = equ_object;
            //curEquTreeNode.Nodes.Add(LineTreeNode);

            treeView1.Nodes.Clear();
            InitTree();
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

            create_pickets_nodes(_dbContext, PathTreeNode);
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
            curEquTreeNode.Nodes.Clear();
            create_pickets_nodes(_dbContext, curEquTreeNode);
        }

        private void addEquipmentToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (curEquTreeNode.ObjectDB.GetType() == typeof(EquObject))
                curEquTreeNode = curEquTreeNode.Parent as EquTreeNode;

            form_newEquip = new CreateEquipmentForm(_db_controller, curEquTreeNode.ObjectDB);
            form_newEquip.EquObjectAddedEvent += EquipmentInPicketAdded;
            form_newEquip.ShowDialog();
        }
     
        void EquipmentInPicketAdded(object sender, DbObjectEventArg e)
        {
            EquObject obj = e.DbObject as EquObject;
          
            EquTreeNode ObjTreeNode = new EquTreeNode(contextMenuStrip_EquipmentCreate, form_properties);
            ObjTreeNode.ObjectDB = obj;

            curEquTreeNode.Nodes.Add(ObjTreeNode);
        }

        void EquipmentAdded(object sender, DbObjectEventArg e)
        {
            EquDbObject obj = e.DbObject as EquDbObject;

            EquTreeNode ObjTreeNode = new EquTreeNode(contextMenuStrip_createEquipmentClassChildNode, form_properties);
            ObjTreeNode.ObjectDB = obj;

            tree_nodes_title_equipment.Nodes.Add(ObjTreeNode);
        }

        void StrelkaAdded(object sender, DbObjectEventArg e)
        {
            EquDbObject obj = e.DbObject as EquDbObject;

            EquTreeNode ObjTreeNode = new EquTreeNode(contextMenuStrip__createStrelkaTypeChildNode, form_properties)
            {
                ObjectDB = obj
            };

            tree_nodes_title_strelka.Nodes.Add(ObjTreeNode);
        }

        void TrafficLightAdded(object sender, DbObjectEventArg e)
        {
            EquDbObject obj = e.DbObject as EquDbObject;

            EquTreeNode ObjTreeNode = new EquTreeNode(contextMenuStrip_trafficLightTypeChildNode, form_properties)
            {
                ObjectDB = obj
            };

            tree_nodes_title_traffic_light.Nodes.Add(ObjTreeNode);
        }


        private void addStrelka_Click(object sender, EventArgs e)
        {
            if (curEquTreeNode.ObjectDB.GetType() == typeof(EquObject))
                curEquTreeNode = curEquTreeNode.Parent as EquTreeNode;

            form_Strelka = new CreateStrelkaForm(_db_controller, curEquTreeNode.ObjectDB);
            form_Strelka.EquObjectAddedEvent += EquipmentInPicketAdded;
            form_Strelka.ShowDialog();
        }

        private void addTrafficLight_toolStripMenu_Click(object sender, EventArgs e)
        {
            if (curEquTreeNode.ObjectDB.GetType() == typeof(EquObject))
                curEquTreeNode = curEquTreeNode.Parent as EquTreeNode;

            var f = new CreateTrafficLightForm(_db_controller, curEquTreeNode.ObjectDB);
            f.EquObjectAddedEvent += EquipmentInPicketAdded;
            f.ShowDialog();
        }

#endregion addObjects

        void updateTreeView()
        {
            treeView1.Nodes.Clear();
            InitTree();
        }

        private void toolStripMenu_item_properties_click(object sender, EventArgs e)
        {
            form_properties.SetProperties(curEquTreeNode);
            form_properties.Show(DPanel, DockState.DockRight);
        }

        private void toolStripButton_Properties_Click(object sender, EventArgs e)
        {
            if (treeView1.SelectedNode != null)
            {
                curEquTreeNode = treeView1.SelectedNode as EquTreeNode;
                form_properties.SetProperties(curEquTreeNode);
                form_properties.Show(DPanel, DockState.DockRight);
            }
        }

        private void treeView1_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            EquTreeNode ObjectTreeNode = (EquTreeNode)e.Node;

            if (ObjectTreeNode.ObjectDB != null)
            {
                if (form_properties.SetProperties(ObjectTreeNode))
                    form_properties.Show(DPanel, DockState.DockRight);
            }
        }

        private void AllEquipmentTree2_VisibleChanged(object sender, EventArgs e)
        {
            if(!Visible)
                form_properties.Hide();
        }

        private void добавитьТипОборудованияToolStripMenuItem_Click(object sender, EventArgs e)
        {
            formAddEquipType = new CreateEquipmentTypeForm(_db_controller);
            formAddEquipType.EquObjectAddedEvent += EquipmentInPicketAdded;
            formAddEquipType.ShowDialog();
        }

        private void toolStripComboBox1_Click(object sender, EventArgs e)
        {

        }

        private void создатьОборудованиеToolStripMenuItem_Click(object sender, EventArgs e)
        {
            formAddEquipType = new CreateEquipmentTypeForm(_db_controller);
            formAddEquipType.EquObjectAddedEvent += EquipmentAdded;
            formAddEquipType.ShowDialog();
        }

        private void добавитьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateStrelkaTypeForm form_Strelka = new CreateStrelkaTypeForm(_db_controller);
            form_Strelka.EquObjectAddedEvent += StrelkaAdded;
            form_Strelka.ShowDialog();
        }

        private void добавитьToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            CreateTrafficLightTypeForm form = new CreateTrafficLightTypeForm(_db_controller);
            form.EquObjectAddedEvent += TrafficLightAdded;
            form.ShowDialog();
        }


        private void удалитьToolStripMenuItem2_Click(object sender, EventArgs e)
        {
            EquipmentObject _EquObject = curEquTreeNode.ObjectDB as EquipmentObject;

            if(_EquObject == null)
            {
                logger.Error("deleting trafficlight error. Selected node is null");
                MessageBox.Show("Невозможно определить идентификатор светофора", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            Equipment.MessageBoxResult result = Equipment.CustomMessageBox.Show("Предупреждение", "Вы уверены что хотите удалить светофор - \"" + _EquObject.Name + "\" из базы данных?");

            if (result != Equipment.MessageBoxResult.Yes)
                return;

            try
            {

                _db_edit_controller.queriesAdapter.delEquipAsType(_EquObject.Code);

                var status_code = (Int32)_db_edit_controller.queriesAdapter.GetReturnValue(5);

                if (status_code == 0)
                    updateTreeView();
                else
                    MessageBox.Show("Хранимая процедура не выполнена. Код ошибки: " + status_code.ToString(), "Ошибка базы данных", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            catch (Exception exc)
            {
                logger.ErrorException(exc.Message, exc);
                MessageBox.Show(exc.Message,"Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

      
    }
}
