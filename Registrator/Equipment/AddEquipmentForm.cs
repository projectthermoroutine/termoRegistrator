
﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Registrator.Equipment
{
    public delegate void DelegateCoordinateEquipmrnt(int x, int y);
    public partial class AddEquipmentForm : Form
    {
        private AddObjectTreeView addObjectOnTreeView;
        private DB.metro_db_controller _db_controller;
        private EquGroup equGroup;
        private EquLine equLine;
        private EquClass equClass;
        private EquPicket equPicket;
        private EquPath equPath;

        private Point coordinates;
        private Graphics g;
        private newEquipmentControl EquipControlXAML;
        private List<int> namesToExclude;
        private List<string> typeEquip;
        EquTreeNode ObjectTreeNodeEmpty;

        public void getCoordinat(int x, int y)
        {
            coordinates.X = x;
            coordinates.Y = y;
        }
     
        public AddEquipmentForm( DB.metro_db_controller db_controller,
                                AddObjectTreeView sender,
                                EquTreeNode PicketTreeNode,
                                EquTreeNode ObjectTreeNode
                              )
        {
            InitializeComponent();
        
            _db_controller = db_controller;
            namesToExclude = new List<int>();

            addObjectOnTreeView = sender;
            EquipControlXAML = new newEquipmentControl( new DelegateCoordinateEquipmrnt(getCoordinat));
            
            equClass  = (PicketTreeNode.Parent.Parent.Parent.Parent as EquTreeNode).ObjectDB as EquClass;
            equGroup = (PicketTreeNode.Parent.Parent.Parent as EquTreeNode).ObjectDB as EquGroup;
            equLine   = (PicketTreeNode.Parent.Parent as EquTreeNode).ObjectDB as EquLine;
            equPath   = (PicketTreeNode.Parent as EquTreeNode).ObjectDB as EquPath;
            equPicket = PicketTreeNode.ObjectDB as EquPicket;
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

            _db_controller.objects_table.Clear();
            _db_controller.objects_adapter.Fill(_db_controller.objects_table);
            var eqObj = (from r in _db_controller.objects_table.AsEnumerable() where r.Group == equGroup.Code && r.Object != "notExist" && (r.typeEquip == 1 || r.typeEquip == 0 ) select r);
            typeEquip = new List<string>();
            //typeEquipStore = new List<int>();
            cmbBx_selEquip.Items.Add("Добавить новое оборудование");

            foreach (var item in eqObj)
            {
                if (!typeEquip.Contains(item.Object))
                    cmbBx_selEquip.Items.Add(item.Object);
                
                typeEquip.Add(item.Object);
            }

            ObjectTreeNodeEmpty = ObjectTreeNode;
        }
     
        private void OK_Click(object sender, EventArgs e)
        {
            string newElementName = txtBxName.Text.Trim();
            int ObjectIndex;
            string newEquipName = newElementName;
            
            if (newElementName.Length<=0)
            {
                MessageBox.Show("Введите название оборудования", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            if (newElementName.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*', '`', '№', '\\', '|' }) == -1)
            {
                int shift = (Int32)n_picketShift.Value;

                int maxTemperature;
                if (!int.TryParse(Convert.ToString(n_MaxTemperature.Value), out maxTemperature))   {
                    MessageBox.Show("Некорректно введена температура", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                }

                if (cmbBx_valid.SelectedIndex == -1) {
                    MessageBox.Show("Выберите техническое состояние оборудования", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                }
                if (coordinates.Y == 0 || coordinates.X == 0) {
                    MessageBox.Show("Укажите местоположение оборудования на схеме", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                }
                
                var res5 = from r in _db_controller.objects_table.AsEnumerable() where r.Object == newEquipName && r.Group != equGroup.Code select new { r.Code };  // check name duplicate
                
                if (res5.Count() == 0)
                {
                    ObjectIndex = Convert.ToInt32(_db_controller.objects_adapter.selectObjectMaxIndex());      // get Equipment max number 
                    ObjectIndex++;

                    long objectCoordinate = calcCoordinate(shift);
                    int typeInd = 0;

                    if(checkBox1.Checked)
                    {
                        _db_controller.objects_adapter.ObjCreate(equClass.Code, equGroup.Code, equLine.Code, @equPath.Code, @equPicket.number,
                                                                ObjectIndex, 
                                                                newEquipName, 
                                                                (int)objectCoordinate,
                                                                maxTemperature,
                                                                coordinates.X,
                                                                coordinates.Y,
                                                                0,
                                                                cmbBx_valid.SelectedIndex,
                                                                shift,
                                                                typeInd,
                                                                0,
                                                                (int)equTypes.Equipment,
                                                                (int)numUpDown_equipLenght.Value
                                                            );

                        var res = _db_controller.all_equipment_adapter.ObjAdd(equClass.Code, equGroup.Code, equLine.Code, equPath.Code, 0, equPicket.number, ObjectIndex);
                        
                    }
                    else
                    {
                        _db_controller.objects_adapter.ObjCreate(equClass.Code, equGroup.Code, equLine.Code,@equPath.Code,@equPicket.number,
                                                                ObjectIndex,
                                                                newEquipName,
                                                                (int)objectCoordinate,
                                                                maxTemperature,
                                                                coordinates.X,
                                                                coordinates.Y,
                                                                0,
                                                                cmbBx_valid.SelectedIndex,
                                                                shift,
                                                                typeInd,
                                                                0,
                                                                (int)equTypes.Equipment,
                                                                0
                                                             );
                       
                        var res = _db_controller.all_equipment_adapter.ObjAdd(equClass.Code, equGroup.Code, equLine.Code, equPath.Code, 0, equPicket.number, ObjectIndex);

                    }

                    EquTreeNode ObjectTreeNode = ObjectTreeNodeEmpty.DeepCopy();
                    ObjectTreeNode.ObjectDB = new EquObject(ObjectIndex, newEquipName, equPicket, shift);
                    addObjectOnTreeView(ObjectTreeNode);

                    _db_controller.objects_table.Clear();
                    _db_controller.objects_adapter.Fill(_db_controller.objects_table);
                    _db_controller.all_equipment_table.Clear();
                    _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);
                    _db_controller.process_equipment_table.Clear();
                    _db_controller.process_equipment_adapter.Fill(_db_controller.process_equipment_table);

                    Close();
                    Dispose();
                }
                else
                    MessageBox.Show("Оборудование с таким именем уже присутствует в другой группе", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
                MessageBox.Show("Имя содержит некорректные символы","", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
        private int calcEquipTypeIndexNumber()
        {
            var resFilterNumber = (from r in _db_controller.objects_table.AsEnumerable() orderby r.typeId select new { r.typeId }).Distinct();

            int ind = 0;

            foreach (var item in resFilterNumber)
            {
                if (ind != Convert.ToInt32(item.typeId))
                    break;
                ind++;
            }

            return ind;
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
            }
            else
            {
                txtBxName.Enabled = true;
                //typeInd = 0;
            }
        }

        private void addNewEquipment_MouseMove(object sender, MouseEventArgs e)
        {
            elementHost1.Refresh();
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked)
            {
                //equipType = 1;
                numUpDown_equipLenght.Enabled = true;
            }
            else
                numUpDown_equipLenght.Enabled = false;
        }
    }
}
