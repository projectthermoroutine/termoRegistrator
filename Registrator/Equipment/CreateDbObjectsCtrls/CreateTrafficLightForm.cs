using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Registrator.Equipment.CreateDbObjectsCtrls
{
    public partial class CreateTrafficLightForm : Form
    {
        public event EventHandler<DbObjectEventArg> EquObjectAddedEvent;
        void EquObjectAdded(EquDbObject db_object)
        {
            EventHandler<DbObjectEventArg> handler = EquObjectAddedEvent;
            if (handler != null)
                handler(this, new DbObjectEventArg(db_object));
        }

        private DB.metro_db_controller _db_controller;
        private EquGroup    equGroup;
        private EquLine     equLine;
        private EquClass    equClass;
        private EquPicket   equPicket;
        private EquPath     equPath;

        private Graphics g;


        public CreateTrafficLightForm(DB.metro_db_controller db_controller, EquDbObject parent)
        {
            InitializeComponent();

            _db_controller = new DB.metro_db_controller(db_controller);

            equPicket = parent as EquPicket;
            equPath = equPicket.Path;
            equLine = equPath.Line;
            equGroup = equLine.Parent as EquGroup;
            equClass = equGroup.Parent as EquClass;

            if(equPicket.npicket[0] == '-')
            {
                n_picketShift.Minimum = -equPicket.lenght;
                n_picketShift.Maximum = 0;
            }
            else
            {
                n_picketShift.Minimum = 0;
                n_picketShift.Maximum = equPicket.lenght;
            }
        }

        private void OK_Click(object sender, EventArgs e)
        {
            if(cmbBx_valid.SelectedItem == null)
                MessageBox.Show("Выберите состояние оборудования", "");

            int ObjectIndex = _db_controller.dbContext.Equipments.Max(eq => eq.Code);      // get Equipment max number 
            
            ObjectIndex++;

            long shift = (long)n_picketShift.Value;


            long lineShift = calcCoordinate(shift);
           // int typeInd = 0;

           // var res = _db_controller.objects_adapter.ObjCreate(equClass.Code, equGroup.Code, equLine.Code, @equPath.Code, @equPicket.keyNumber, 
           //                                             ObjectIndex,
           //                                             "светофор",
           //                                             lineShift,
           //                                             0,
           //                                             0,
           //                                             0,
           //                                             0,
           //                                             cmbBx_valid.SelectedIndex,
           //                                             (int)shift,
           //                                             typeInd,
           //                                             0,
           //                                             (int)EQUIPS_TYPES.TrafficLight,
           //                                             0);
           

           //var  result = _db_controller.all_equipment_adapter.ObjAdd(equClass.Code, equGroup.Code, equLine.Code, equPath.Code, 0, equPicket.keyNumber, ObjectIndex);


           var new_object = new EquObject(ObjectIndex,"светофор", equPicket, lineShift);
           EquObjectAdded(new_object);

           Dispose();
           Close();
        }

        public long calcCoordinate(long shift)
        {
            long ObjectCoordinate =0;
            var Picket = from r in _db_controller.pickets_table.AsEnumerable() where r.number == equPicket.keyNumber && r.path == equPath.Code select new { r.EndShiftLine , r.StartShiftLine };

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
