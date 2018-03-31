
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Registrator.DB.EFClasses;
using NLog;

namespace Registrator.Equipment.CreateDbObjectsCtrls
{
    public delegate void DelegateCoordinateEquipment(int x, int y);

    public partial class CreateEquipmentForm : Form
    {
        static readonly Logger logger = LogManager.GetCurrentClassLogger();
        public event EventHandler<DbObjectEventArg> EquObjectAddedEvent;

        void EquObjectAdded(EquDbObject db_object)
        {
            EventHandler<DbObjectEventArg> handler = EquObjectAddedEvent;
            if (handler != null)
                handler(this, new DbObjectEventArg(db_object));
        }

        private Registrator.DB.EFClasses.EquipmentsClass[] equipsTypes;
        private Registrator.DB.EFClasses.EquipmentsClass currentEquipType;
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


            equipsTypes = _db_controller.dbContext.EquipmentsClasses.Where(e => e.EquipType == (int)EQUIPS_TYPES.Equipment).Distinct().OrderBy(n => n.Name).ToArray();
            selectEquip.Items.AddRange(equipsTypes.Select(e => e.Name).OrderBy(n => n).ToArray());
        }

        private void OK_Click(object sender, EventArgs e)
        {
            string additionalInfo = txtBxName.Text.Trim();

            if (selectEquip.SelectedItem == null)
            {
                MessageBox.Show("Для добавления оборудования необходимо создать хотя бы один тип оборудования", "Информация", MessageBoxButtons.OK, MessageBoxIcon.Information);

                Close();
                Dispose();

                return;
            }

            string equipName = currentEquipType.Name;

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

            int shift = (Int32)n_picketShift.Value;

            //int maxTemperature;
            //if (!int.TryParse(Convert.ToString(n_MaxTemperature.Value), out maxTemperature))
            //{
            //    MessageBox.Show("Некорректно введена температура", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            //    return;
            //}

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

            if (_db_controller.dbContext.AllEquipments.Count() > 0)
                addingID = _db_controller.dbContext.AllEquipments.Max(eq => eq.Code);      // get Equipment max number
            
            addingID++;

            try
            {
                long objectCoordinate = CalcCoordinate(shift);

                _db_controller.dbContext.AllEquipments.Add(
                    new DB.EFClasses.AllEquipment
                    {
                        Code = addingID,
                        EquipID = equipsTypes[selectEquip.SelectedIndex].Id,
                        EquipTypeID = (int)EQUIPS_TYPES.Equipment,                                // identificators
                        Name = equipName,
                        Group = equGroup.Code,
                        Line = equLine.Code,
                        Path = equPath.Code,
                        Picket = equPicket.keyNumber,                   // equipment displacment in database hierarhy
                        Area_X = coordinates.X,
                        Area_Y = coordinates.Y,
                        Area_Height = 0,
                        Area_Width = 0,
                        Area_Type = currentEquipType.AreaType,                                  // Area settings
                        curTemperature = 0,
                        maxTemperature = currentEquipType.MaxTemperature /*maxTemperature*/,                // temperatures
                        EquipWorkState = cmbBx_valid.SelectedIndex,
                        shiftFromPicket = shift,
                        shiftLine = objectCoordinate,
                        EquipLenght = currentEquipType.Width /*(int)numUpDown_equipLenght.Value*/,
                        strelkaLeftOrRight = 0,                          // not used
                        Info = additionalInfo
                    });

                _db_controller.dbContext.SaveChanges();

                var new_object = new EquObject(addingID, equipName, equPicket, shift, EQUIPS_TYPES.Equipment);

                EquObjectAdded(new_object);
            }
            catch (Exception exc)
            {
                logger.ErrorException(exc.Message, exc);
                MessageBox.Show(exc.Message, "Исключение", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
           
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
                throw new Exception("Cannot find picket needed for calculate line offset");

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
            if (selectEquip.SelectedIndex != -1)
            {
                currentEquipType = equipsTypes[selectEquip.SelectedIndex];

                txtBxName.Text = currentEquipType.Name;

                if (currentEquipType.Width != 0)
                {
                    longObjectCheckBox.Checked = true;
                    numUpDown_equipLenght.Value = currentEquipType.Width;
                }
                else
                {
                    longObjectCheckBox.Checked = false;
                    numUpDown_equipLenght.Value = 0;
                }

                OK.Enabled = true;
            }
        }

        private void addNewEquipment_MouseMove(object sender, MouseEventArgs e)
        {
            elementHost1.Refresh();
        }

    }
}
