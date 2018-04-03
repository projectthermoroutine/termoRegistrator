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
    public partial class CreateTrafficLightTypeForm : Form
    {
        public event EventHandler<DbObjectEventArg> EquObjectAddedEvent;
        void EquObjectAdded(EquDbObject db_object)
        {
            EventHandler<DbObjectEventArg> handler = EquObjectAddedEvent;
            if (handler != null)
                handler(this, new DbObjectEventArg(db_object));
        }

        private DB.metro_db_controller _db_controller;
        
        static readonly Logger logger = LogManager.GetCurrentClassLogger();

        public CreateTrafficLightTypeForm(DB.metro_db_controller db_controller)
        {
            InitializeComponent();

            _db_controller = new DB.metro_db_controller(db_controller);
        }

        private void OK_Click(object sender, EventArgs e)
        {
            if (cmbBx_valid.SelectedItem == null)
                MessageBox.Show("Выберите состояние оборудования", "", MessageBoxButtons.OK, MessageBoxIcon.Information);

            int ObjectIndex = 0;

            if (_db_controller.dbContext.AllEquipments.Count() > 0)
                ObjectIndex = _db_controller.dbContext.EquipmentsClasses.Max(eq => eq.Id);      // get Equipment max number 

            ObjectIndex++;

            string name = textBox_name.Text;

            if(name.Length == 0)
            {
                MessageBox.Show("Введте название светофора", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            if (name.IndexOfAny(RegistratorFormStrings.incorrect_symbols) == -1 || name.Length < 50)
            {
                try
                {
                    var res = _db_controller.dbContext.EquipmentsClasses.Add(
                        new DB.EFClasses.EquipmentsClass()
                        {
                            Id = ObjectIndex,
                            Height = 0,
                            Width = 0,
                            Name = name,
                            X = 0,
                            Y = 0,
                            strelkaLeftOrRight = -1,
                            EquipType = (Int32)EQUIPS_TYPES.TrafficLight,
                            AreaType = (Int32)AREA_TYPE.trafficLight,
                            AdditionalOptions = "<empty/>"
                        });

                    _db_controller.dbContext.SaveChanges();

                    EquObjectAdded(new EquipmentObject(ObjectIndex, name, "equip"));
                }
                catch(Exception exc)
                {
                    logger.ErrorException(exc.Message, exc);
                    MessageBox.Show(exc.Message, "Исключение", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }

                Dispose();
                Close();

            }
            else
            {
                MessageBox.Show("Название светофора введено некорректно.", "Информация", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }
        }

       

        private void Cancel_Click(object sender, EventArgs e)
        {
            Dispose();
            Close();
        }
    }
}
