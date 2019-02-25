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
        private Registrator.DB.EFClasses.EquipmentsClass[] equipsTypes;
        private Registrator.DB.EFClasses.EquipmentsClass currentEquipType;


        DB.EFClasses.Picket _picket;

        public CreateTrafficLightForm(DB.metro_db_controller db_controller, EquDbObject parent)
        {
            InitializeComponent();

            _db_controller = new DB.metro_db_controller(db_controller);

            equPicket = parent as EquPicket;
            equPath = equPicket.Path;
            equLine = equPath.Line;
            equGroup = equLine.Parent as EquGroup;
            equClass = equGroup.Parent as EquClass;

            _picket = _db_controller.dbContext.Pickets.Single(p => p.number == equPicket.Code);

            ObjectOffsetCtrl.Minimum = 0;
            ObjectOffsetCtrl.Maximum = _picket.Dlina;

            equipsTypes = _db_controller.dbContext.EquipmentsClasses.Where(e => e.EquipType == (int)EQUIPS_TYPES.TrafficLight).Distinct().OrderBy(n => n.Name).ToArray();
            comboBox_availiable.Items.AddRange(equipsTypes.Select(e => e.Name).OrderBy(n => n).ToArray());
            
        }

        private void OK_Click(object sender, EventArgs e)
        {
            if(cmbBx_valid.SelectedItem == null)
                MessageBox.Show("Выберите состояние оборудования", "");

            int ObjectIndex = 0;

            if(_db_controller.dbContext.AllEquipments.Count() > 0)
                ObjectIndex = _db_controller.dbContext.AllEquipments.Max(eq => eq.Code);      // get Equipment max number 
            
            ObjectIndex++;

            int object_offset = (int)ObjectOffsetCtrl.Value;
            long objectCoordinate = _picket.StartShiftLine + object_offset;

            if (_picket.StartShiftLine < 0)
                object_offset -= _picket.Dlina;

            string additionalInfo = textBox_info.Text;

            if (additionalInfo.Length > 50)
            {
                MessageBox.Show("Длина поля: 'Дополнительная информация' не должна превышать 50 символов", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            if (additionalInfo.IndexOfAny(RegistratorFormStrings.incorrect_symbols) != -1)
            {
                MessageBox.Show("Поле: Дополнительная иформацтя содержит некорректные символы", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            _db_controller.dbContext.AllEquipments.Add(
                new DB.EFClasses.AllEquipment()
                {
                    Area_Height = currentEquipType.Height,
                    Area_Type = currentEquipType.AreaType,
                    Area_Width = currentEquipType.Width,
                    Area_X = 0,
                    Area_Y = 0,
                    Code = ObjectIndex,
                    curTemperature = 0,
                    EquipID = currentEquipType.Id,
                    EquipLenght = currentEquipType.Width,
                    maxTemperature = Int32.MaxValue,
                    Info = additionalInfo,
                    EquipTypeID= (Int32)EQUIPS_TYPES.TrafficLight,
                    Name = currentEquipType.Name,
                    Group = equGroup.Code,
                    Line = equLine.Code,
                    Path = equPath.Code,
                    Picket = equPicket.Code,
                    shiftLine = objectCoordinate,
                    shiftFromPicket = object_offset
                });

            _db_controller.dbContext.SaveChanges();

            var new_object = new EquObject(ObjectIndex, currentEquipType.Name, equPicket, object_offset, EQUIPS_TYPES.TrafficLight);
            EquObjectAdded(new_object);

           Dispose();
           Close();
        }

        private void comboBox_availiable_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBox_availiable.SelectedIndex != -1)
            {
                currentEquipType = equipsTypes[comboBox_availiable.SelectedIndex];

                OK.Enabled = true;
            }
            else
                OK.Enabled = false;
        }
    }
}
