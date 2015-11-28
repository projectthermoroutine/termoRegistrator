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
    public partial class AddTrafficLight : Form
    {
        private AddObjectTreeView addObjectOnTreeView;
        private DB.metro_db_controller _db_controller;
        private EquGroup    equGroup;
        private EquLine     equLine;
        private EquClass    equClass;
        private EquPicket   equPicket;
        private EquPath     equPath;
        private EquTreeNode PathTreeNode;
        private EquTreeNode _PicketTreeNode;
        private EquTreeNode ObjectTreeNode;

        private Graphics g;
       

        public AddTrafficLight( DB.metro_db_controller db_controller,
                                AddObjectTreeView sender,
                                EquTreeNode PicketTreeNode,
                                EquTreeNode objectTreeNode)
        {
            InitializeComponent();

            _db_controller = new DB.metro_db_controller(db_controller);

            addObjectOnTreeView = sender;

            equClass = (PicketTreeNode.Parent.Parent.Parent.Parent as EquTreeNode).ObjectDB as EquClass;
            equGroup = (PicketTreeNode.Parent.Parent.Parent as EquTreeNode).ObjectDB as EquGroup;
            equLine = (PicketTreeNode.Parent.Parent as EquTreeNode).ObjectDB as EquLine;
            PathTreeNode = PicketTreeNode.Parent as EquTreeNode;
            equPath = PathTreeNode.ObjectDB as EquPath;
            equPicket = PicketTreeNode.ObjectDB as EquPicket;
            _PicketTreeNode = PicketTreeNode;
            ObjectTreeNode = objectTreeNode;

            if(equPicket.npicket[0] == '-')
            {
                n_picketShift.Minimum = -90000000;
                n_picketShift.Maximum = 0;
            }

        }

        private void OK_Click(object sender, EventArgs e)
        {

            if(cmbBx_valid.SelectedItem == null)
                MessageBox.Show("Выберите состояние оборудования", "");

            int ObjectIndex = Convert.ToInt32(_db_controller.objects_adapter.selectObjectMaxIndex());      // get Equipment max number 
            
            ObjectIndex++;

            long shift = (long)n_picketShift.Value;

            long lineShift = calcCoordinate(shift);
            int typeInd = 0;

            _db_controller.objects_adapter.ObjCreate(equClass.Code, equGroup.Code, equLine.Code, @equPath.Code, @equPicket.number, 
                                                        ObjectIndex,
                                                        "светофор",
                                                        lineShift,
                                                        0,
                                                        0,
                                                        0,
                                                        0,
                                                        cmbBx_valid.SelectedIndex,
                                                        (int)shift,
                                                        typeInd,
                                                        (int)equTypes.TrafficLight,
                                                        -1,
                                                        0);

           var  result = _db_controller.all_equipment_adapter.ObjAdd(equClass.Code, equGroup.Code, equLine.Code, equPath.Code, 0, equPicket.Code, ObjectIndex);

           EquTreeNode TrafficLightTreeNode = ObjectTreeNode.DeepCopy();

           TrafficLightTreeNode.ObjectDB = new EquObject(ObjectIndex,"светофор", equGroup, null, equPath, equPicket, lineShift);

           addObjectOnTreeView(TrafficLightTreeNode);

           _db_controller.all_equipment_table.Clear();
           _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);
           _db_controller.objects_table.Clear();
           _db_controller.objects_adapter.Fill(_db_controller.objects_table);

           Dispose();
           Close();
        }

        public long calcCoordinate(long shift)
        {
            long ObjectCoordinate =0;
            var Picket = from r in _db_controller.pickets_table.AsEnumerable() where r.number == equPicket.number && r.path == equPath.Code select new { r.EndShiftLine , r.StartShiftLine };

            if (Picket.Count() != 1)
                MessageBox.Show("Ошибка Базы Данных", "Ошибка");

            if(equPicket.npicket[0] == '-')
                ObjectCoordinate = Picket.First().EndShiftLine + shift;
            else
                ObjectCoordinate = Picket.First().StartShiftLine + shift;

            return ObjectCoordinate;
        }
    }
}
