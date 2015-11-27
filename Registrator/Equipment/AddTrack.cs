using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Registrator.Equipment
{
    public partial class AddTrack : Form
    {
        public AddObjectTreeView addObjectTreeView;
        public DB.metro_db_controller _db_controller;
        public string newGroupName;
        public int lineNumer;
        public int Track;
        //
        public PicketsManager PicketsObj;
        public EquClass equClass;
        public EquGroup equGroup;
        public EquLine equLine;
        public int peregonNumber;
        PicketsManager _PicketsManager;
        int defaultPicketLength;
        EquTreeNode LineTreeNode;
        EquTreeNode PathTreeNode;
        EquTreeNode PicketTreeNode;

        public AddTrack( DB.metro_db_controller db_controller,
                         AddObjectTreeView sender,
                         EquTreeNode lineTreeNode,
                         EquTreeNode pathTreeNode,
                         EquTreeNode picketTreeNode
                         )
        {
            InitializeComponent();

            _db_controller = new DB.metro_db_controller(db_controller);
            
            button2.Enabled = false;

            addObjectTreeView = sender;
            defaultPicketLength = Registrator.Properties.Settings.Default.DefaultPicketLength;

            LineTreeNode = lineTreeNode;
            equLine  = LineTreeNode.ObjectDB as EquLine;
            equGroup = (LineTreeNode.Parent as EquTreeNode).ObjectDB as EquGroup;
            equClass = (LineTreeNode.Parent.Parent as EquTreeNode).ObjectDB as EquClass;

            _PicketsManager = new PicketsManager(_db_controller);

            PathTreeNode   = pathTreeNode;
            PicketTreeNode = picketTreeNode;
        }
    
        private void button2_Click(object sender, EventArgs e)
        {
            string trackName = txtBx_number.Text.Trim();

            if (trackName.Length > 49)
            {
                MessageBox.Show("Слишком длинное название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            if (trackName.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*' }) == -1)
            {
                if (trackName.Length != 0)
                {
                    var resMatch = from r in _db_controller.trackTable.AsEnumerable() where r.Track == trackName select new { r.Track };
                                        
                    if (resMatch.Count() == 0)
                    {
                        _db_controller.trackAdapter.Insert1(trackName);

                        _db_controller.trackTable.Clear();
                        _db_controller.trackAdapter.Fill(_db_controller.trackTable);

                        var resID = from r in _db_controller.trackTable.AsEnumerable() 
                                    where r.Track == trackName 
                                    select new {r.ID, r.Track };

                        _db_controller.all_equipment_adapter.Path1(equClass.Code, equGroup.Code, equLine.Code, resID.First().ID);

                        PathTreeNode.ObjectDB = new EquPath(resID.First().ID, trackName);
                        addRangePickets();

                        _db_controller.pickets_table.Clear();
                        _db_controller.pickets_adapter.Fill(_db_controller.pickets_table);
                        _db_controller.all_equipment_table.Clear();
                        _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);

                        addObjectTreeView(PathTreeNode);

                        Close();
                        Dispose();
                    }
                    else
                        MessageBox.Show("Путь с таким именем уже существует", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Название не должно быть пустым", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                MessageBox.Show("Некорректный символ: '@','.', ',', '!', ... ",
                    "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }


        private void txtBx_number_TextChanged(object sender, EventArgs e)
        {
            if(txtBx_number.Text.Length > 0)
                button2.Enabled = true;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Close();
            Dispose();
        }

        void addRangePickets()
        {
            if (numUpDownFrom.Value > numUpDownTo.Value)
            {
                MessageBox.Show("Диапазон пикетов задан не верно", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            int addedPicketID = Convert.ToInt32(_db_controller.pickets_adapter.selectMaxNumberIndex());

            bool flagMinus0 = false;
            if (numUpDownFrom.Value < 0)
                flagMinus0 = true;

            for (int i = (int)numUpDownFrom.Value; i <= (int)numUpDownTo.Value; i++)
            {
                addedPicketID++;

                if (flagMinus0 && i == 0)
                {
                    EquPicket p1 = _PicketsManager.AddPicketToDB("-0", equClass.Code, equGroup.Code, equLine.Code, PathTreeNode.ObjectDB.Code, addedPicketID, defaultPicketLength * 10);

                    var empData1 = from r in _db_controller.all_equipment_table.AsEnumerable() where r.number == addedPicketID && r.number != 0 && r.LineNum == equLine.Code && r.Track == PathTreeNode.ObjectDB.Code select new { r.number };

                    if (empData1.Count() == 0)
                    {
                        _db_controller.all_equipment_adapter.PicketAdd(equClass.Code, equGroup.Code, equLine.Code, PathTreeNode.ObjectDB.Code, 0, addedPicketID);
                        EquTreeNode picketTreeNode = PicketTreeNode.DeepCopy() as EquTreeNode;
                        picketTreeNode.ObjectDB = p1;
                        PathTreeNode.Nodes.Add(picketTreeNode);
                    }
                    else
                    {
                        MessageBox.Show("Пикет с таким номером уже присутствует на пути", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                        return;
                    }

                    addedPicketID++;
                }

                EquPicket p = _PicketsManager.AddPicketToDB(i.ToString(), equClass.Code, equGroup.Code, equLine.Code, PathTreeNode.ObjectDB.Code, addedPicketID, defaultPicketLength * 10);

                var empData = from r in _db_controller.all_equipment_table.AsEnumerable() where r.number == addedPicketID && r.number != 0 && r.LineNum == equLine.Code && r.Track == PathTreeNode.ObjectDB.Code select new { r.number };

                if (empData.Count() == 0)
                {
                    _db_controller.all_equipment_adapter.PicketAdd(equClass.Code, equGroup.Code, equLine.Code, PathTreeNode.ObjectDB.Code, 0, addedPicketID);
                    EquTreeNode picketTreeNode = PicketTreeNode.DeepCopy() as EquTreeNode;
                    picketTreeNode.ObjectDB = p;
                    PathTreeNode.Nodes.Add(picketTreeNode);
                }
                else
                {
                    MessageBox.Show("Пикет с таким номером уже присутствует на пути", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                }
            }
        }
    }
}
