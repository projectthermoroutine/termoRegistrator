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

namespace Registrator.Equipment.CreateDbObjectsCtrls
{
    public partial class CreateEquipmentTypeForm : Form
    {
        private DB.metro_db_controller _db_controller;
        private Point coordinates;
        private EquipmentsType[] equipsTypes;

        public CreateEquipmentTypeForm(DB.metro_db_controller db_controller)
        {
            InitializeComponent();

            _db_controller = db_controller;

            coordinates = new Point(0,0);

            equipsTypes = _db_controller.dbContext.EquipmentsTypes.Distinct().ToArray();
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
                
                // check duplicate
                if (_db_controller.dbContext.Equipments.Where(eq => eq.Name == newEquipName).Select(eq => eq.Code).Distinct().Count() == 0)
                {
                    int addingID = 0;
                    try
                    {
                        addingID = _db_controller.dbContext.EquipmentsTypes.Select(eq=>eq.Id).DefaultIfEmpty(0).Max();   // get Equipment max number 
                        addingID++;

                        //if (longObjectCheckBox.Checked)
                        _db_controller.dbContext.EquipmentsTypes.Add(
                            new DB.EFClasses.EquipmentsType
                            {
                                Id = addingID,
                                Name = newEquipName,
                                X = coordinates.X,
                                Y = coordinates.Y,
                                //EquipType = 0,
                                AreaType = 0,
                                Height = 0,
                                Width = 0,
                                MaxTemperature = maxTemperature,
                                MinTemperature = 0,
                                AdditionalOptions = "<empty/>",
                                strelkaLeftOrRight = 0
                            });
                       
                        _db_controller.dbContext.SaveChanges();
                    }
                    catch(DbEntityValidationException ex)
                    {
                        foreach(var eve in ex.EntityValidationErrors)
                        {
                            Debug.Write(eve.Entry.Entity.GetType().Name);
                            Debug.Write(eve.Entry.State);

                            foreach (var ve in eve.ValidationErrors)
                            {
                                Debug.Write(ve.PropertyName);
                                Debug.Write(ve.ErrorMessage);
                                Debug.Write(eve.Entry.CurrentValues.GetValue<object>(ve.PropertyName));
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
