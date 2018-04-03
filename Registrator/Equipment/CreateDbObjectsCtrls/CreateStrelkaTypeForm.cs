using NLog;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Registrator.Equipment.CreateDbObjectsCtrls
{
    public partial class CreateStrelkaTypeForm : Form
    {
        public CreateStrelkaTypeForm()
        {
            InitializeComponent();
        }
        public event EventHandler<DbObjectEventArg> EquObjectAddedEvent;
        void EquObjectAdded(EquDbObject db_object)
        {
            EventHandler<DbObjectEventArg> handler = EquObjectAddedEvent;
            if (handler != null)
                handler(this, new DbObjectEventArg(db_object));
        }

        private DB.metro_db_controller _db_controller;

        private Point coordinates;
        private TunnelControl EquipControlXAML;
        private List<int> namesToExclude;
        private List<int> typeEquipStore;
        static readonly Logger logger = LogManager.GetCurrentClassLogger();

        public void getCoordinat(int x, int y)
        {
            coordinates.X = x;
            coordinates.Y = y;
        }

        public CreateStrelkaTypeForm(DB.metro_db_controller db_controller)
        {
            InitializeComponent();

            _db_controller = new DB.metro_db_controller(db_controller);
            namesToExclude = new List<int>();

            EquipControlXAML = new TunnelControl(new DelegateCoordinateEquipment(getCoordinat));

            coordinates = new Point
            {
                X = 0,
                Y = 0
            };

          
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
                int strelkaDirect;
                if (cmbBox_strelka.SelectedIndex == -1)
                {
                    MessageBox.Show("Укажите параметр стрелки: левая/правая ", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                }

                strelkaDirect = (cmbBox_strelka.SelectedIndex == 0) ? (int)STRELKA_DIRECT.Left : (int)STRELKA_DIRECT.Right;

                try
                {
                    if (_db_controller.dbContext.EquipmentsClasses.Count() > 0)
                        ObjectIndex = _db_controller.dbContext.EquipmentsClasses.Max(eq => eq.Id); //Convert.ToInt32(_db_controller.objects_adapter.selectObjectMaxIndex());      // get Equipment max number 

                    ObjectIndex++;

                    _db_controller.dbContext.EquipmentsClasses.Add(new DB.EFClasses.EquipmentsClass()
                    {
                        Id = ObjectIndex,
                        Width = (int)(numUpDown_Lenght.Value * 10),
                        Height = 1,
                        AdditionalOptions="<empty/>",
                        MaxTemperature=int.MaxValue,
                        MinTemperature=int.MinValue,
                        EquipType = (int)EQUIPS_TYPES.Strelka,
                        X=0,
                        Y=0,
                        AreaType=0,
                        Name = newElementName,
                        strelkaLeftOrRight= strelkaDirect
                    });

                    _db_controller.dbContext.SaveChanges();

                    EquObjectAdded(new EquipmentObject(ObjectIndex, newElementName, "strelka"));
                }
                catch (Exception exc)
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

        private void Cancel_Click(object sender, EventArgs e)
        {
            Dispose();
            Close();
        }

     
    }
}
