using NLog;
using Registrator.DB.EFClasses;
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
        private List<EquipmentsClass> typeEquipStore;
        private EquipmentsClass currentSelectedStrelka;


        static readonly Logger logger = LogManager.GetCurrentClassLogger();

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

            typeEquipStore = new List<EquipmentsClass>();

            var equipsTypes = _db_controller.dbContext.EquipmentsClasses.Where(e=>e.EquipType == (int)EQUIPS_TYPES.Strelka).Distinct();

            cmbBx_selEquip.Items.AddRange(equipsTypes.Select(e=>e.Name).OrderBy(n=>n).Distinct().ToArray());
            typeEquipStore.AddRange(equipsTypes.OrderBy(o => o.Name).Distinct().ToArray());

            if (equPicket.npicket[0] == '-')
            {
                n_picketShift.Minimum = decimal.MinValue;
                n_picketShift.Maximum = 0;
            }
            else
            {

            }
        }

        private void OK_Click(object sender, EventArgs e)
        {
            string newElementName = txtBxName.Text.Trim();
            int ObjectIndex = 0;

            string newEquipName = newElementName;

            if (newElementName.Length <= 0)
            {
                MessageBox.Show("Введите название оборудования", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            if (newElementName.IndexOfAny(RegistratorFormStrings.incorrect_symbols) == -1)
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

                
                strelkaDirect = (cmbBox_strelka.SelectedIndex == 0) ? (int)STRELKA_DIRECT.Right : (int)STRELKA_DIRECT.Left;

                try
                {
                    if (_db_controller.dbContext.AllEquipments.Count() > 0)
                        ObjectIndex = _db_controller.dbContext.AllEquipments.Max(eq => eq.Code); //Convert.ToInt32(_db_controller.objects_adapter.selectObjectMaxIndex());      // get Equipment max number 

                    ObjectIndex++;

                    long lineShift = calcCoordinate(shift);
                    int selectedEquip = currentSelectedStrelka.Id;

                    _db_controller.dbContext.AllEquipments.Add(
                        new DB.EFClasses.AllEquipment()
                        {
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
                            maxTemperature = _db_controller.dbContext.EquipmentsClasses.Where(eq => eq.Id == selectedEquip).Distinct().FirstOrDefault().Id,
                            Name = newElementName,
                            shiftFromPicket = shift,
                            shiftLine = lineShift,
                            strelkaLeftOrRight = strelkaDirect
                        });

                    _db_controller.dbContext.SaveChanges();

                    var new_object = new EquObject(ObjectIndex, newEquipName, equPicket, shift, EQUIPS_TYPES.Strelka);
                    EquObjectAdded(new_object);
                    
                }
                catch(Exception exc)
                {
                    logger.ErrorException(exc.Message, exc);
                    MessageBox.Show(exc.Message, "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }

                Close();
                Dispose();
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

        private void cmbBx_selEquip_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            if (cmbBx_selEquip.SelectedIndex != -1 )
            {
                currentSelectedStrelka = typeEquipStore[cmbBx_selEquip.SelectedIndex];
                txtBxName.Text = typeEquipStore[cmbBx_selEquip.SelectedIndex].Name;

                cmbBox_strelka.SelectedIndex = currentSelectedStrelka.strelkaLeftOrRight == 1 ?  1 : 0;
                numUpDown_Lenght.Value = currentSelectedStrelka.Width;
            }
        }
    }
}
