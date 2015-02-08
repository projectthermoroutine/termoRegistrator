using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;


namespace Registrator
{
    public partial class EquipmentMonitor : ToolWindow
    {

        MyDelegateFrameProcess dPicBox;
        MyDelegateFrameProcessDataGrid dDataGrid;
        MyDelegateFrameProcessDataGridClear dDataGridClear;
        MyDelegateFrameProcessDataGridClearAll dDataGridClearAll;

        public Equipment.ProcessEquipment ProcessEquipObj;
        private Pen pen;
        DB.DataBaseHelper dbHelper;
        public void setDBHelper(DB.DataBaseHelper dbHelperArg)
        {
            dbHelper = dbHelperArg;
            ProcessEquipObj = new Equipment.ProcessEquipment(dbHelper);
            ProcessEquipObj.setGraphics(ref pb);

            ProcessEquipObj.setDataGrid(ref dataGridView1);
            ProcessEquipObj.setDelegat(ref dPicBox, ref dDataGrid, ref dDataGridClear, dDataGridClearAll);

        }

        public EquipmentMonitor()
        {
            InitializeComponent();

            //-------------------------------- PictureBox -------------------------------------------

            dataGridView1.AllowUserToAddRows = false;

            dPicBox = new MyDelegateFrameProcess(funcFrame);
            dDataGrid = new MyDelegateFrameProcessDataGrid(funcFrameDataGrid);
            dDataGridClear = new MyDelegateFrameProcessDataGridClear(funcFrameDataGridClear);
            dDataGridClearAll = new MyDelegateFrameProcessDataGridClearAll(funcFrameDataGridClear);
            pb.Paint += new System.Windows.Forms.PaintEventHandler(this.pictureBox1_Paint);

            //ProcessEquipObj = new Equipment.ProcessEquipment(ref dbHelperArg);
            pen = new Pen(Color.Black, 2);


            
        }
        private void pictureBox1_Paint(object sender, System.Windows.Forms.PaintEventArgs e)
        {
            pen.Dispose();
            pen = new Pen(Color.Black, 2);
            //Brush brush = new SolidBrush(this.pb.BackColor);

            //e.Graphics.FillEllipse(brush, this.pb.Bounds);
            e.Graphics.DrawEllipse(pen, 12, 12, this.pb.Bounds.Width - 100, this.pb.Bounds.Height - 100);

            ProcessEquipObj.StartCoordinatX = (this.pb.Bounds.Width - 100) / 2 + 12;
            ProcessEquipObj.StartCoordinatY = (this.pb.Bounds.Height - 100) / 2 + 12;
        }

        //
        // ------------------------ PROCESS  FRAME -------------------------------------
        //
        public delegate void MyDelegateFrameProcess(int rfrsh);
        public delegate void MyDelegateFrameProcessDataGrid(string objName, string distance, string picketNumber, string curTemperature, string maxTemperature, string shiftFromPicket);
        public delegate void MyDelegateFrameProcessDataGridClear();
        public delegate void MyDelegateFrameProcessDataGridClearAll();
        //
        public void funcFrame(int i)
        {
            pb.Refresh();
        }

        public void funcFrameDataGrid(string objName,string distance,string picketNumber, string curTemperature, string maxTemperature, string shiftFromPicket)
        {
            dataGridView1.Rows.Insert(0, new object[] { objName, distance, picketNumber, curTemperature, maxTemperature, shiftFromPicket });

            if (dataGridView1.Rows.Count > 200)
                dataGridView1.Rows.RemoveAt(dataGridView1.Rows.Count - 1);
        }
        public void funcFrameDataGridClear()
        {
            dataGridView1.Rows.Clear();
        }

    

        //
        //------------------------------------------------------------------------------
        //
    }
}
