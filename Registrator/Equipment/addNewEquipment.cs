
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
        private MyDelegate1 d;
        private DB.DataBaseHelper dbHelper;
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
        private List<int> typeEquip;
        private List<int> typeEquipStore;
        private int typeInd = 0;
        private int equipType=0;
        //private List<int> codesOfEquipment;
        public void getCoordinat(int x, int y)
        {
            coordinates.X = x;
            coordinates.Y = y;
        }
     
        public addNewEquipment( DB.DataBaseHelper dbHelperArg,
                                MyDelegate1 sender,
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
            dbHelper = dbHelperArg;
            namesToExclude = new List<int>();
            //var eqObj = (from r in dbHelper.dataTable_Objects.AsEnumerable() where !namesToExclude.Contains(m.Name)) r.Group == equGroup.Code && r.Object != "notExist" select new { r.Object }).Distinct();
            
            foreach (string line in (from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Object!="notExist" select r["Object"]).ToList())
                lstBxAllEquip.Items.Add(Convert.ToString(line));

            d = sender;
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

            dbHelper.dataTable_Objects.Clear();
            dbHelper.TblAdapter_Objects.Fill(dbHelper.dataTable_Objects);
            var eqObj = (from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Group == equGroup.Code && r.Object != "notExist" && (r.typeEquip == 1 || r.typeEquip == 0 ) select r);
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

                var res5 = from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Object == newEquipName && r.Group != equGroup.Code select new { r.Code };  // check name duplicate
                if (res5.Count() == 0)
                {
                    ObjectIndex = Convert.ToInt32(dbHelper.TblAdapter_Objects.selectObjectMaxIndex());      // get Equipment max number 
                    ObjectIndex++;

                    calcShiftfromLineBegin();

                    if (typeInd == 0)
                        typeInd = calcEquipTypeIndexNumber();

                    if(checkBox1.Checked)
                    {
                        dbHelper.TblAdapter_Objects.ObjCreate(  equGroup.Code,
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
                                                                null
                                                            );
                    }
                    else
                    {
                        dbHelper.TblAdapter_Objects.ObjCreate(  equGroup.Code,
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
                                                                null,
                                                                equipType,
                                                                null
                                                             );

                    }

                    

                    result = dbHelper.TblAdapter_AllEquipment.ObjAdd(equClass.Code, equGroup.Code, equLine.Code, equPath.Code, equLayout.Code, equPicket.Code, ObjectIndex);

                    d(ObjectIndex, newEquipName + ";" + Convert.ToString(typeInd), "Obj");

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
            var resFilterNumber = (from r in dbHelper.dataTable_Objects.AsEnumerable() orderby r.typeId select new { r.typeId }).Distinct();

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
            shiftFromLineBegin=0;

            var res1 = from r in dbHelper.dataTable_PicketsTable.AsEnumerable() where r.Npiketa == equPicket.Code  select new { r.NpicketBefore, r.Dlina };
            var resLineStartCoordinat = from r in dbHelper.dataTable_Lines.AsEnumerable() where r.LineNum == equLine.Code select new { r.StartCoordinate };

            int tmpDlina = (int)res1.First().Dlina;

            shiftFromLineBegin += (ulong)(tmpDlina + Convert.ToInt32(resLineStartCoordinat.First().StartCoordinate));
           
            int NpicketaBeforeTmp = (int)res1.First().NpicketBefore;

            while (NpicketaBeforeTmp != 0)
            {
                var res = from r in dbHelper.dataTable_PicketsTable.AsEnumerable() where r.Npiketa == NpicketaBeforeTmp select new { r.NpicketBefore, r.Dlina };
                tmpDlina = (int)res.First().Dlina;
                NpicketaBeforeTmp = (int)res.First().NpicketBefore;
                shiftFromLineBegin += (ulong)tmpDlina;
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
                typeInd = typeEquipStore[cmbBx_selEquip.SelectedIndex];
            }
            else
            {
                txtBxName.Enabled = true;
                typeInd = 0;
            }
        }

        private void splitContainer1_Panel2_Paint(object sender, PaintEventArgs e)
        {

        }

        private void tableLayoutPanel3_Paint(object sender, PaintEventArgs e)
        {

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
