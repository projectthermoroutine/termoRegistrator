using Registrator.DB.EFClasses;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Data.Entity.Validation;
using System.Diagnostics;
using NLog;

namespace Registrator.Equipment.CreateDbObjectsCtrls
{
    public partial class CreateEquipmentTypeForm : Form
    {
        private DB.metro_db_controller _db_controller;
        private Point coordinates;
        private Registrator.DB.EFClasses.EquipmentsClass[] equipsTypes;
        private static readonly Logger logger = LogManager.GetCurrentClassLogger();

        public event EventHandler<DbObjectEventArg> EquObjectAddedEvent;
        void EquObjectAdded(EquDbObject db_object)
        {
            EventHandler<DbObjectEventArg> handler = EquObjectAddedEvent;
            if (handler != null)
                handler(this, new DbObjectEventArg(db_object));
        }


        public CreateEquipmentTypeForm(DB.metro_db_controller db_controller)
        {
            InitializeComponent();

            _db_controller = db_controller;

            coordinates = new Point(0,0);

            equipsTypes = _db_controller.dbContext.EquipmentsClasses.Distinct().ToArray();
            cmbBx_selEquip.Items.AddRange(equipsTypes.Select(e => e.Name).ToArray());

        }

        private void OK_Click(object sender, EventArgs e)
        {
            string newElementName = txtBxName.Text.Trim();
            
            string newEquipName = newElementName;

            if (newElementName.Length <= 0)
            {
                MessageBox.Show("Введите название оборудования", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            if (newElementName.IndexOfAny(RegistratorFormStrings.incorrect_symbols) == -1)
            {
                int shift = (Int32)n_picketShift.Value;

                int maxTemperature;
                if (!int.TryParse(Convert.ToString(n_MaxTemperature.Value), out maxTemperature))
                {
                    MessageBox.Show("Некорректно введена температура", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                }

                
                
                // check duplicate
                if (_db_controller.dbContext.AllEquipments.Where(eq => eq.Name == newEquipName).Select(eq => eq.Code).Distinct().Count() == 0)
                {
                    int addingID = 0;
                    try
                    {
                        addingID = _db_controller.dbContext.EquipmentsClasses.Select(eq=>eq.Id).DefaultIfEmpty(0).Max();   // get Equipment max number 
                        addingID++;

                        //if (longObjectCheckBox.Checked)
                        _db_controller.dbContext.EquipmentsClasses.Add(
                            new DB.EFClasses.EquipmentsClass
                            {
                                Id = addingID,
                                Name = newEquipName,
                                X = coordinates.X,
                                Y = coordinates.Y,
                                EquipType = (int)EQUIPS_TYPES.Equipment,
                                AreaType = 0,
                                Height = 0,
                                Width = 0,
                                MaxTemperature = maxTemperature,
                                MinTemperature = 0,
                                AdditionalOptions = "<empty/>",
                                strelkaLeftOrRight = 0
                            });
                       
                        _db_controller.dbContext.SaveChanges();

                        EquObjectAdded(new EquipmentObject(addingID, newEquipName,"equip"));
                    }
                    catch(DbEntityValidationException ex)
                    {
                        foreach(var eve in ex.EntityValidationErrors)
                        {
                            logger.Log(LogLevel.Error, eve.Entry.Entity.GetType().Name);
                            logger.Log(LogLevel.Error, eve.Entry.State.ToString());

                            foreach (var ve in eve.ValidationErrors)
                            {
                                logger.Log(LogLevel.Error, ve.PropertyName);
                                logger.Log(LogLevel.Error, ve.ErrorMessage);
                                logger.Log(LogLevel.Error, eve.Entry.CurrentValues.GetValue<object>(ve.PropertyName));
                            }
                        }
                    }
                    catch (Exception)
                    {
                        ///TODO Database exception
                    }
                    

                    Close();
                    Dispose();
                }
                else
                    MessageBox.Show("Оборудование с таким именем уже присутствует в другой группе", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
                MessageBox.Show("Имя содержит некорректные символы", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
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
