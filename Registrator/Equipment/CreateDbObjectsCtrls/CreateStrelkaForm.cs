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
    public partial class CreateStrelkaForm : Form
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
        private TunnelControl EquipControlXAML;
        private List<int> namesToExclude;
        private List<int> typeEquipStore;
        
        public void getCoordinat(int x, int y)
        {
            coordinates.X = x;
            coordinates.Y = y;
        }

        public CreateStrelkaForm(DB.metro_db_controller db_controller, EquDbObject parent)
        {
            InitializeComponent();

            _db_controller = new DB.metro_db_controller(db_controller);
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

            //var eqObj = (from r in _db_controller.objects_table.AsEnumerable() where r.Group == equGroup.Code && r.Object != "notExist" && r.typeEquip == 2 select r);

            //_db_controller.dbContext.Equipments.Where(e=>e.)

            typeEquipStore = new List<int>();
            cmbBx_selEquip.Items.Add("Добавить новое оборудование");

            var equipsTypes = _db_controller.dbContext.EquipmentsTypes.Where(e => e.EquipType == 2).Distinct();

            cmbBx_selEquip.Items.AddRange(equipsTypes.Select(e=>e.Name).ToArray());
            typeEquipStore.AddRange(equipsTypes.Select(e=>e.EquipType).ToArray());

            if (equPicket.npicket[0] == '-')
            {
                n_picketShift.Minimum = -90000000;
                n_picketShift.Maximum = 0;
            }
        }

        private void OK_Click(object sender, EventArgs e)
        {
            string newElementName = txtBxName.Text.Trim();
            int ObjectIndex;
            string newEquipName = newElementName;

            if (newElementName.Length <= 0)
            {
                MessageBox.Show("Введите название оборудования", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            if (newElementName.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*', '`', '№', '\\', '|' }) == -1)
            {
                int shift = (int)n_picketShift.Value;

                int maxTemperature;
                if (!int.TryParse(Convert.ToString("-1"), out maxTemperature))
                {
                    MessageBox.Show("Некорректно введена температура", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                }

                int strelkaDirect;
                if (cmbBox_strelka.SelectedIndex == -1)
                {
                    MessageBox.Show("Укажите параметр стрелки: левая/правая ", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                }

                strelkaDirect = (cmbBox_strelka.SelectedIndex == 0) ? 1 : 0;

                //var res5 = from r in _db_controller.objects_table.AsEnumerable() where r.Object == newEquipName && r.Group != equGroup.Code select new { r.Code };  // check name duplicate

                //if (res5.Count() == 0)
                //{
                ObjectIndex = _db_controller.dbContext.Equipments.Max(eq => eq.Code); //Convert.ToInt32(_db_controller.objects_adapter.selectObjectMaxIndex());      // get Equipment max number 
                ObjectIndex++;

                long lineShift = calcCoordinate(shift);
                int selectedEquip = 0;

                _db_controller.dbContext.Equipments.Add(
                    new DB.EFClasses.Equipment() {
                        EquipID = selectedEquip,
                        Path = equPath.Code,
                        Picket = equPicket.Code,
                        Line = equLine.Code,
                        Code = ObjectIndex,
                        Area_Height = 0,
                        Area_Width = 0,
                        Area_Type = 0,
                        Area_X = 0,
                        Area_Y = 0,
                        curTemperature = 0,
                        EquipLenght = (int)(numUpDown_Lenght.Value * 10),
                        EquipTypeID = (int)EQUIPS_TYPES.Strelka,
                        EquipWorkState = (int)EQUIPS_WORKS_STATE.off,
                        Group = equGroup.Code,
                        maxTemperature = _db_controller.dbContext.EquipmentsTypes.Where(eq => eq.Id == selectedEquip).Distinct().FirstOrDefault().Id,
                        Name = newElementName,
                        shiftFromPicket = shift,
                        shiftLine = lineShift,
                        strelkaLeftOrRight = strelkaDirect

                        });

                        var new_object = new EquObject(ObjectIndex, newEquipName, equPicket, shift);
                        EquObjectAdded(new_object);

                        Close();
                        Dispose();
                //}
                //else
                //    MessageBox.Show("Оборудование с таким именем уже присутствует в другой группе", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
                MessageBox.Show("Имя содержит некорректные символы", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        public long calcCoordinate(long shift)
        {
            var Picket = _db_controller.dbContext.Pickets.Where(p => p.number == equPicket.keyNumber && p.path == equPath.Code).Select(p => new { p.EndShiftLine, p.StartShiftLine });

            if (Picket.Count() != 1)
                MessageBox.Show("Ошибка Базы Данных", "Ошибка");

            long ObjectCoordinate = 0;

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
                //typeInd = typeEquipStore[cmbBx_selEquip.SelectedIndex-1];
            }
            else
            {
                txtBxName.Enabled = true;
                //typeInd = 0;
            }
        }

        private void cmbBx_selEquip_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            if (cmbBx_selEquip.SelectedIndex != -1 && cmbBx_selEquip.SelectedIndex != 0)
            {
                txtBxName.Text = cmbBx_selEquip.SelectedItem.ToString();
                txtBxName.Enabled = false;
                //typeInd = typeEquipStore[cmbBx_selEquip.SelectedIndex-1];
            }
            else
            {
                txtBxName.Enabled = true;
                //typeInd = 0;
            }
        }
    }
}
