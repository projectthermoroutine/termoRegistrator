
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
    public partial class addNewEquipment : Form
    {
        private AddObjectOnTreeView addObjectOnTreeView;
        private DB.metro_db_controller _db_controller;
        private EquGroup equGroup;
        private EquLine equLine;
        private EquClass equClass;
        private EquLayout equLayout;
        private Picket equPicket;
        private EquPath equPath;

        //private equipment equipObj;
        private Pen pen;
        private Pen penEquip;
        private Brush brush;

        private Point coordinates;
        private Graphics g;
        private newEquipmentControl EquipControlXAML;
        private List<int> namesToExclude;
        private List<string> typeEquip;
        //private List<int> typeEquipStore;
        private int typeInd = 0;
        private int equipType=0;
        //private List<int> codesOfEquipment;
        public void getCoordinat(int x, int y)
        {
            coordinates.X = x;
            coordinates.Y = y;
        }
     
        public addNewEquipment( DB.metro_db_controller db_controller,
                                AddObjectOnTreeView sender,
                                EquGroup equGroupArg,
                                EquLine equLineArg,
                                EquClass equClassArg,
                                EquLayout equLayoutNew,
                                Picket equPicketNew,
                                EquPath equPathArg
                                /*int equipTypeArg*/
                              )
        {
            InitializeComponent();
           
           // equipType = equipTypeArg;
            _db_controller = new DB.metro_db_controller(db_controller);
            namesToExclude = new List<int>();
            //var eqObj = (from r in _db_controller.objects_table.AsEnumerable() where !namesToExclude.Contains(m.Name)) r.Group == equGroup.Code && r.Object != "notExist" select new { r.Object }).Distinct();
            
            //foreach (string line in (from r in _db_controller.objects_table.AsEnumerable() where r.Object!="notExist" select r["Object"]).ToList())
            //    lstBxAllEquip.Items.Add(Convert.ToString(line));

            addObjectOnTreeView = sender;
            EquipControlXAML = new newEquipmentControl( new DelegateCoordinateEquipmrnt(getCoordinat));
            //equipObj = equipArg;
            equGroup = equGroupArg;
            equLine = equLineArg;
            equPicket = equPicketNew;
            equLayout = equLayoutNew;
            equClass = equClassArg;
            equPath = equPathArg;

            coordinates = new Point();
            
            coordinates.X = 0;
            coordinates.Y = 0;

            elementHost1.Child = EquipControlXAML;

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
        }
     
        private void OK_Click(object sender, EventArgs e)
        {
            string newElementName = txtBxName.Text.Trim();
            int ObjectIndex;
            int result;
            string newEquipName = newElementName;
            //equipObj.equipName = newEquipName;
            
            if (newElementName.Length<=0)
            {
                MessageBox.Show("Введите название оборудования", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            if (newElementName.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*', '`', '№', '\\', '|' }) == -1)
            {
                int shift;
                if (!int.TryParse(Convert.ToString(n_picketShift.Value), out shift))
                {
                    MessageBox.Show("Некорректно введено смещение", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                }

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
                if (coordinates.Y == 0 || coordinates.X == 0)
                {
                    MessageBox.Show("Укажите местоположение оборудования на схеме", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                }

                var res5 = from r in _db_controller.objects_table.AsEnumerable() where r.Object == newEquipName && r.Group != equGroup.Code select new { r.Code };  // check name duplicate
                if (res5.Count() == 0)
                {
                    ObjectIndex = Convert.ToInt32(_db_controller.objects_adapter.selectObjectMaxIndex());      // get Equipment max number 
                    ObjectIndex++;

                    calcShiftfromLineBegin();

                    //if (typeInd == 0)
                    //    typeInd = calcEquipTypeIndexNumber();

                    if(checkBox1.Checked)
                    {
                        _db_controller.objects_adapter.ObjCreate(  equGroup.Code,
                                                                ObjectIndex, 
                                                                newEquipName, 
                                                                Convert.ToInt64(shiftFromLineBegin),
                                                                maxTemperature,
                                                                coordinates.X,
                                                                coordinates.Y,
                                                                0,
                                                                cmbBx_valid.SelectedIndex,
                                                                shift,
                                                                typeInd,
                                                                (int)numUpDown_shiftFromEndPicket.Value,
                                                                equipType,
                                                                -1
                                                            );

                        result = _db_controller.all_equipment_adapter.ObjAdd(equClass.Code, equGroup.Code, equLine.Code, equPath.Code, equLayout.Code, equPicket.Code, ObjectIndex);


                        // get line objects

                        addObjectOnTreeView(ObjectIndex, newEquipName + ";" + Convert.ToString(typeInd) + ";" + Convert.ToString(numUpDown_shiftFromEndPicket.Value) + ";" + "equipment", "Obj");
                    }
                    else
                    {
                        _db_controller.objects_adapter.ObjCreate(  equGroup.Code,
                                                                ObjectIndex,
                                                                newEquipName,
                                                                Convert.ToInt64(shiftFromLineBegin),
                                                                maxTemperature,
                                                                coordinates.X,
                                                                coordinates.Y,
                                                                0,
                                                                cmbBx_valid.SelectedIndex,
                                                                shift,
                                                                typeInd,
                                                                -1,
                                                                equipType,
                                                                -1
                                                             );

                        result = _db_controller.all_equipment_adapter.ObjAdd(equClass.Code, equGroup.Code, equLine.Code, equPath.Code, equLayout.Code, equPicket.Code, ObjectIndex);

                        addObjectOnTreeView(ObjectIndex, newEquipName + ";" + Convert.ToString(typeInd) + ";" + "-1" + ";" + "equipment", "Obj");

                    }
                    

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

        private ulong shiftFromLineBegin = 0;
        public void calcShiftfromLineBegin()
        {
            shiftFromLineBegin = 0;

            var resCurrentPicket = from r in _db_controller.pickets_table.AsEnumerable() where r.number == equPicket.number && r.path == equPath.Code  select new {r.NpicketAfter, r.NpicketBefore, r.Dlina };
            
            shiftFromLineBegin += (ulong)(n_picketShift.Value + equLine.offsetLineCoordinate);

            int NpicketaBeforeTmp = resCurrentPicket.First().NpicketBefore;
            while (NpicketaBeforeTmp != 0)
            {
                var resNextPicket = (from r in _db_controller.pickets_table.AsEnumerable() where r.number == NpicketaBeforeTmp && r.path == equPath.Code select new { r.NpicketBefore, r.Dlina }).Distinct();
                shiftFromLineBegin += (ulong)resNextPicket.First().Dlina;
                NpicketaBeforeTmp = (int)resNextPicket.First().NpicketBefore;
            }
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
                typeInd = 0;
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
                equipType = 1;
                numUpDown_shiftFromEndPicket.Enabled = true;
            }
            else
                numUpDown_shiftFromEndPicket.Enabled = false;
        }

    }
}
