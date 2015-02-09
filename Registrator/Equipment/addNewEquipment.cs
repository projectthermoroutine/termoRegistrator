using System;
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
        //private List<int> codesOfEquipment;
        public void getCoordinat(int x, int y)
        {
     
            coordinates.X = x;
            coordinates.Y = y;
        }
     
        public addNewEquipment(DB.DataBaseHelper dbHelperArg, MyDelegate1 sender/*, equipment equipArg*/, EquGroup equGroupArg, EquLine equLineArg, EquClass equClassArg,EquLayout equLayoutNew,Picket equPicketNew,EquPath equPathArg)
        {
            InitializeComponent();

            dbHelper = dbHelperArg;

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
            //pictureBox1.Paint += new System.Windows.Forms.PaintEventHandler(this.pictureBox1_Paint);
            //pictureBox1.Click += pictureBox1_Click;

            //penEquip = new Pen(Color.Black, 2);
            //brush = new SolidBrush(Color.Chocolate);
            //g = pictureBox1.CreateGraphics();

         
            //pictureBox1.BackColor = Color.Gray;

            //codesOfEquipment = new List<int>();
            dbHelper.dataTable_Objects.Clear();
            dbHelper.TblAdapter_Objects.Fill(dbHelper.dataTable_Objects);
            var eqObj = (from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Group == equGroup.Code && r.Object != "notExist" select new { r.Object }).Distinct();
            //var eqObj = (from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Group == equGroup.Code && r.Object != "notExist" select new { r.Code, r.Object }).Distinct(;
            foreach (var item in eqObj)
            {
                cmbBx_selEquip.Items.Add(item.Object);
                //codesOfEquipment.Add(item.Code);
            }

        }
     
        void pictureBox1_Click(object sender, EventArgs e)
        {
            pictureBox1.Refresh();
            coordinates = ((MouseEventArgs)e).Location;

            g.FillEllipse(brush, coordinates.X, coordinates.Y, 10, 10);
            g.DrawEllipse(penEquip, coordinates.X, coordinates.Y, 10, 10);
        }

        private void pictureBox1_Paint(object sender, System.Windows.Forms.PaintEventArgs e)
        {
            if (pen != null)
                pen.Dispose();
            pen = new Pen(Color.Black, 2);
            //Brush brush = new SolidBrush(this.pb.BackColor);
            //e.Graphics.FillEllipse(brush, this.pb.Bounds);
            e.Graphics.DrawEllipse(pen, 12, 12, pictureBox1.Bounds.Width - 24, pictureBox1.Bounds.Height - 24);
        }

        private void OK_Click(object sender, EventArgs e)
        {
            string newElementName = txtBxName.Text.Trim();
            int ObjectIndex;
            int result;
            string newEquipName = newElementName;
            //equipObj.equipName = newEquipName;

            int shift;
            if (!int.TryParse(txtBxShift.Text.Trim(), out shift))
            {
                MessageBox.Show("Некорректно введено смещение");
                return;
            }

            int maxTemperature;
            if (!int.TryParse(txtBx_MaxTemperature.Text.Trim(), out maxTemperature))
            {
                MessageBox.Show("Некорректно введена температура");
                return;
            }

            if (cmbBx_valid.SelectedIndex == -1)
            {
                MessageBox.Show("Выберите техническое состояние оборудования");
                return;
            }
            if (coordinates.Y == 0 || coordinates.X == 0)
            {
                MessageBox.Show("Укажите местоположение оборудования на схеме");
                return;
            }



            var res5 = from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Object == newEquipName && r.Group != equGroup.Code select new { r.Code };  // check name duplicate
            if (res5.Count() == 0)
            {
                ObjectIndex = Convert.ToInt32(dbHelper.TblAdapter_Objects.selectObjectMaxIndex());      // get Equipment max number 
                ObjectIndex++;

                calcShiftfromLineBegin();

                dbHelper.TblAdapter_Objects.ObjCreate(equGroup.Code, ObjectIndex, newEquipName, shiftFromLineBegin, maxTemperature, coordinates.X,  coordinates.Y, 0, cmbBx_valid.SelectedIndex, shift);

                result = dbHelper.TblAdapter_AllEquipment.ObjAdd(equClass.Code, equGroup.Code, equLine.Code, equPath.Code, equLayout.Code, equPicket.Code, ObjectIndex);
                
                d(ObjectIndex, newEquipName, "Obj");
                
                Close();
                Dispose();
            }
            else
                MessageBox.Show("Оборудование с таким именем уже присутствует в другой группе");
        }
        private int shiftFromLineBegin = 0;
        public void calcShiftfromLineBegin()
        {
            shiftFromLineBegin=0;

            var res1 = from r in dbHelper.dataTable_PicketsTable.AsEnumerable() where r.Npiketa == equPicket.Code  select new { r.NpicketBefore, r.Dlina };

            int tmpDlina = (int)res1.First().Dlina;

            shiftFromLineBegin += tmpDlina;
           
            int NpicketaBeforeTmp = (int)res1.First().NpicketBefore;

            while (NpicketaBeforeTmp != 0)
            {
                var res = from r in dbHelper.dataTable_PicketsTable.AsEnumerable() where r.Npiketa == NpicketaBeforeTmp select new { r.NpicketBefore, r.Dlina };
                tmpDlina = (int)res.First().Dlina;
                NpicketaBeforeTmp = (int)res.First().NpicketBefore;
                shiftFromLineBegin += tmpDlina;
            }
        }

        private void Cancel_Click(object sender, EventArgs e)
        {
            Dispose();
            Close();
        }

        private void cmbBx_selEquip_SelectedIndexChanged(object sender, EventArgs e)
        {
            txtBxName.Text = cmbBx_selEquip.SelectedItem.ToString();
        }

        private void splitContainer1_Panel2_Paint(object sender, PaintEventArgs e)
        {

        }

        private void tableLayoutPanel3_Paint(object sender, PaintEventArgs e)
        {

        }
    }
}
