
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Registrator.DB.EFClasses;

namespace Registrator.Equipment.CreateDbObjectsCtrls
{
    public delegate void DelegateCoordinateEquipment(int x, int y);
    public partial class CreateEquipmentForm : Form
    {
        public event EventHandler<DbObjectEventArg> EquObjectAddedEvent;
        void EquObjectAdded(EquDbObject db_object)
        {
            EventHandler<DbObjectEventArg> handler = EquObjectAddedEvent;
            if (handler != null)
                handler(this, new DbObjectEventArg(db_object));
        }

        private EquipmentsType[] equipsTypes;
        private DB.metro_db_controller _db_controller;
        private EquGroup equGroup;
        private EquLine equLine;
        private EquClass equClass;
        private EquPicket equPicket;
        private EquPath equPath;

        private Point coordinates;
        private TunnelControl equipControlXAML;

        public void getCoordinat(int x, int y)
        {
            coordinates.X = x;
            coordinates.Y = y;
        }

        public CreateEquipmentForm(DB.metro_db_controller db_controller, EquDbObject parent)
        {
            InitializeComponent();

            _db_controller = db_controller;

            equipControlXAML = new TunnelControl(new DelegateCoordinateEquipment(getCoordinat));

            equPicket = parent as EquPicket;
            equPath = equPicket.Path;
            equLine = equPath.Line;
            equGroup = equLine.Parent as EquGroup;
            equClass = equGroup.Parent as EquClass;

            coordinates = new Point(0,0);

            elementHost1.Child = equipControlXAML;

            if (equPicket.npicket[0] == '-')
            {
                n_picketShift.Minimum = -equPicket.lenght;
                n_picketShift.Maximum = 0;
            }
            else
            {
                n_picketShift.Minimum = 0;
                n_picketShift.Maximum = equPicket.lenght;
            }

            equipsTypes = _db_controller.dbContext.EquipmentsTypes.Where(e => e.EquipType == 0 || e.EquipType == 1).Distinct().ToArray();
            selectEquip.Items.AddRange(equipsTypes.Select(e => e.Name).ToArray());
        }

        private void OK_Click(object sender, EventArgs e)
        {
            string additionalInfo = txtBxName.Text.Trim();
            string equipName = selectEquip.SelectedItem.ToString();

            if (additionalInfo.Length > 50)
            {
                MessageBox.Show("Длина поля: 'Дополнительная информация' не должна превышать 50 символов", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }


            if (additionalInfo.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*', '`', '№', '\\', '|' }) != -1)
            {
                MessageBox.Show("Поле: Дополнительная иформацтя содержит некорректные символы", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            int shift = (Int32)n_picketShift.Value;

            int maxTemperature;
            if (!int.TryParse(Convert.ToString(n_MaxTemperature.Value), out maxTemperature))
            {
                MessageBox.Show("Некорректно введена температура", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            if (cmbBx_valid.SelectedIndex == -1)
            {
                MessageBox.Show("Выберите техническое состояние оборудования", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }
            if (coordinates.Y == 0 && coordinates.X == 0)
            {
                MessageBox.Show("Укажите местоположение оборудования на схеме", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            int addingID = 0;
            try
            {

                addingID = _db_controller.dbContext.Equipments.Max(eq => eq.Code);      // get Equipment max number 
                addingID++;

                long objectCoordinate = CalcCoordinate(shift);

                //if (longObjectCheckBox.Checked)

                _db_controller.dbContext.Equipments.Add(
                    new DB.EFClasses.Equipment
                    {
                        Code = addingID,
                        EquipID = equipsTypes[selectEquip.SelectedIndex].Id,
                        EquipTypeID = 0,                                // identificators
                        Name = equipName,
                        Group = (short)equGroup.Code,
                        Line = equLine.Code,
                        Path = equPath.Code,
                        Picket = equPicket.keyNumber,                   // equipment displacment in database hierarhy
                        Area_X = coordinates.X,
                        Area_Y = coordinates.Y,
                        Area_Height = 0,
                        Area_Width = 0,
                        Area_Type = 0,                                  // Area settings
                        curTemperature = 0,
                        maxTemperature = maxTemperature,                // temperatures
                        EquipWorkState = cmbBx_valid.SelectedIndex,
                        shiftFromPicket = shift,
                        shiftLine = 0,
                        EquipLenght = (int)numUpDown_equipLenght.Value,
                        strelkaLeftOrRight = 0,                          // not used
                        Info = additionalInfo
                    });

                _db_controller.dbContext.SaveChanges();
            }



            catch (Exception exp)
            {
                ///TODO Database exception
            }

            var new_object = new EquObject(addingID, equipName, equPicket, shift);

            EquObjectAdded(new_object);
            Close();
            Dispose();
        }

        //private int calcEquipTypeIndexNumber()
        //{
        //    _db_controller.dbContext 

        //    var resFilterNumber = (from r in _db_controller.objects_table.AsEnumerable() orderby r.typeId select new { r.typeId }).Distinct();

        //    int ind = 0;

        //    foreach (var item in resFilterNumber)
        //    {
        //        if (ind != Convert.ToInt32(item.typeId))
        //            break;
        //        ind++;
        //    }

        //    return ind;
        //}

        public long CalcCoordinate(long shift)
        {
            long ObjectCoordinate = 0;

            var Picket = _db_controller.dbContext.Pickets.Where(p => p.number == equPicket.keyNumber && p.path == equPath.Code).Select(e => new { e.EndShiftLine, e.StartShiftLine });

            if (Picket.Count() != 1)
                MessageBox.Show("Ошибка Базы Данных", "Ошибка");

            if (equPicket.npicket[0] == '-')
                ObjectCoordinate = Picket.First().EndShiftLine + shift;
            else
                ObjectCoordinate = Picket.First().StartShiftLine + shift;

            return ObjectCoordinate;
        }

        private void Cancel_Click(object sender, EventArgs e)
        {
            Dispose();
            Close();
        }

        private void cmbBx_selEquip_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (selectEquip.SelectedIndex != -1 && selectEquip.SelectedIndex != 0)
            {
                txtBxName.Text = selectEquip.SelectedItem.ToString();
                txtBxName.Enabled = false;
            }
            else
            {
                txtBxName.Enabled = true;
            }
        }

        private void addNewEquipment_MouseMove(object sender, MouseEventArgs e)
        {
            elementHost1.Refresh();
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (longObjectCheckBox.Checked)
            {
                //equipType = 1;
                numUpDown_equipLenght.Enabled = true;
            }
            else
                numUpDown_equipLenght.Enabled = false;
        }
    }
}
