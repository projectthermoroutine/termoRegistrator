
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

        private DB.metro_db_controller _db_controller;
        private EquGroup equGroup;
        private EquLine equLine;
        private EquClass equClass;
        private EquPicket equPicket;
        private EquPath equPath;

        private Point coordinates;
        private Graphics g;
        private TunnelControl EquipControlXAML;
        private List<int> namesToExclude;
        private List<string> typeEquip;

        public void getCoordinat(int x, int y)
        {
            coordinates.X = x;
            coordinates.Y = y;
        }

        public CreateEquipmentForm(DB.metro_db_controller db_controller, EquDbObject parent)
        {
            InitializeComponent();

            _db_controller = db_controller;
            namesToExclude = new List<int>();

            EquipControlXAML = new TunnelControl(new DelegateCoordinateEquipment(getCoordinat));

            equPicket = parent as EquPicket;
            equPath = equPicket.Path;
            equLine = equPath.Line;
            equGroup = equLine.Parent as EquGroup;
            equClass = equGroup.Parent as EquClass;

            coordinates = new Point();

            coordinates.X = 0;
            coordinates.Y = 0;

            elementHost1.Child = EquipControlXAML;

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

            //var eqObj = (from r in _db_controller.objects_table.AsEnumerable() where r.Group == equGroup.Code && r.Object != "notExist" && (r.typeEquip == 1 || r.typeEquip == 0) select r);
            cmbBx_selEquip.Items.Add("Добавить новое оборудование");
            cmbBx_selEquip.Items.AddRange(
                _db_controller.dbContext.EquipmentsTypes.Where(e => e.EquipType == 0 || e.EquipType == 1).Select(e => e.Name).Distinct().ToArray());
        }

        private void OK_Click(object sender, EventArgs e)
        {
            string newElementName = txtBxName.Text.Trim();
            int addingID;
            string newEquipName = newElementName;

            if (newElementName.Length <= 0)
            {
                MessageBox.Show("Введите название оборудования", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            if (newElementName.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*', '`', '№', '\\', '|' }) == -1)
            {
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

                // check duplicate
                if (_db_controller.dbContext.Equipments.Where(eq => eq.Name == newEquipName).Select(eq => eq.Code).Distinct().Count() == 0)
                {
                    addingID = _db_controller.dbContext.Equipments.Max(eq => eq.Code);      // get Equipment max number 
                    addingID++;

                    long objectCoordinate = calcCoordinate(shift);

                    //if (longObjectCheckBox.Checked)
                    _db_controller.dbContext.EquipmentsTypes.Add(
                        new DB.EFClasses.EquipmentsType
                        {
                            Id = addingID,
                            Name = newEquipName,
                            X = coordinates.X,
                            Y = coordinates.Y,
                            EquipType = 0,
                            AreaType = 0,
                            Height = 0,
                            Width = 0,
                            MaxTemperature = maxTemperature,
                            MinTemperature = 0,
                            AdditionalOptions = ""
                        });

                    _db_controller.dbContext.Equipments.Add(
                        new DB.EFClasses.Equipment
                        {
                            Code = equClass.Code,
                            EquipID = addingID,
                            EquipTypeID = 0,                                // identificators
                            Name = newEquipName,
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
                            strelkaLeftOrRight = 0                          // not used
                        });
                        

                    var new_object = new EquObject(addingID, newEquipName, equPicket, shift);

                    EquObjectAdded(new_object);
                    Close();
                    Dispose();
                }
                else
                    MessageBox.Show("Оборудование с таким именем уже присутствует в другой группе", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
                MessageBox.Show("Имя содержит некорректные символы", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
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

        public long calcCoordinate(long shift)
        {
            long ObjectCoordinate = 0;
            var Picket = from r in _db_controller.pickets_table.AsEnumerable() where r.number == equPicket.keyNumber && r.path == equPath.Code select new { r.EndShiftLine, r.StartShiftLine };

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
            if (cmbBx_selEquip.SelectedIndex != -1 && cmbBx_selEquip.SelectedIndex != 0)
            {
                txtBxName.Text = cmbBx_selEquip.SelectedItem.ToString();
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
