﻿using System;
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
        private List<int> typeEquip;
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

            _db_controller.objects_table.Clear();
            _db_controller.objects_adapter.Fill(_db_controller.objects_table);
            var eqObj = (from r in _db_controller.objects_table.AsEnumerable() where r.Group == equGroup.Code && r.Object != "notExist" && r.typeEquip == 2 select r);
            typeEquip = new List<int>();
            typeEquipStore = new List<int>();
            cmbBx_selEquip.Items.Add("Добавить новое оборудование");

            foreach (var item in eqObj)
            {
                if (!typeEquip.Contains(item.typeId))
                {
                    cmbBx_selEquip.Items.Add(item.Object);
                    typeEquipStore.Add(item.typeId);
                }
                typeEquip.Add(item.typeId);
            }

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

                var res5 = from r in _db_controller.objects_table.AsEnumerable() where r.Object == newEquipName && r.Group != equGroup.Code select new { r.Code };  // check name duplicate
                if (res5.Count() == 0)
                {
                    ObjectIndex = Convert.ToInt32(_db_controller.objects_adapter.selectObjectMaxIndex());      // get Equipment max number 
                    ObjectIndex++;

                    long lineShift = calcCoordinate(shift);

                    int typeInd = 0;
                    //if (typeInd == 0)
                    //    typeInd = calcEquipTypeIndexNumber();

                    _db_controller.objects_adapter.ObjCreate(equClass.Code,
                                                            equGroup.Code,
                                                            equLine.Code,
                                                            equPath.Code,
                                                            equPicket.number,
                                                            ObjectIndex, 
                                                            newEquipName, 
                                                            lineShift, 
                                                            maxTemperature, 
                                                            /*coordinates.X*/0,
                                                            /*coordinates.Y*/0,
                                                            0,
                                                            0, 
                                                            shift, 
                                                            typeInd,
                                                            (int)(numUpDown_Lenght.Value*10),
                                                            (int)equTypes.Strelka,
                                                            strelkaDirect);
                    
                    var res = _db_controller.all_equipment_adapter.ObjAdd(equClass.Code, equGroup.Code, equLine.Code, equPath.Code, 0, equPicket.number, ObjectIndex);

                    _db_controller.objects_table.Clear();
                    _db_controller.objects_adapter.Fill(_db_controller.objects_table);
                    _db_controller.all_equipment_table.Clear();
                    _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);
                    _db_controller.process_equipment_table.Clear();
                    _db_controller.process_equipment_adapter.Fill(_db_controller.process_equipment_table);

                    var new_object = new EquObject(ObjectIndex, newEquipName, equPicket, shift);
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

        public long calcCoordinate(long shift)
        {
            long ObjectCoordinate = 0;
            var Picket = from r in _db_controller.pickets_table.AsEnumerable() where r.number == equPicket.number && r.path == equPath.Code select new { r.EndShiftLine, r.StartShiftLine };

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
