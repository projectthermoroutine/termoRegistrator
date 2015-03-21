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

        //MyDelegateFrameProcess dPicBox;
        MyDelegateFrameProcessDataGrid dDataGrid;
        MyDelegateFrameProcessDataGridClear dDataGridClear;
        MyDelegateFrameProcessDataGridClearAll dDataGridClearAll;

        //public delegate void MyDelegateFrameProcess(int rfrsh);
        public delegate void MyDelegateFrameProcessDataGrid(string objName, string distance, string picketNumber, string curTemperature, string maxTemperature, string shiftFromPicket);
        public delegate void MyDelegateFrameProcessDataGridClear();
        public delegate void MyDelegateFrameProcessDataGridClearAll();

        public Equipment.ProcessEquipment ProcessEquipObj;
        private Pen pen;
        DB.DataBaseHelper dbHelper;
        public int curLine = 0;


        // filter
        private Equipment.EquipmentFilterNew form_EquipFilter;

        public void setDBHelper(DB.DataBaseHelper dbHelperArg)
        {
            dbHelper = dbHelperArg;
            ProcessEquipObj = new Equipment.ProcessEquipment(ref dbHelper);

            ProcessEquipObj.setDataGrid(ref dataGridView1);
            ProcessEquipObj.setDelegat(/*ref dPicBox,*/ ref dDataGrid, ref dDataGridClear, dDataGridClearAll);

        }

        public EquipmentMonitor()
        {
            this.Text = "Контролируемое оборудование";
            InitializeComponent();

            dataGridView1.AllowUserToAddRows = false;

            dDataGrid = new MyDelegateFrameProcessDataGrid(funcFrameDataGrid);
            dDataGridClear = new MyDelegateFrameProcessDataGridClear(funcFrameDataGridClear);
            dDataGridClearAll = new MyDelegateFrameProcessDataGridClearAll(funcFrameDataGridClear);

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

        private void toolStripButton1_Click(object sender, EventArgs e) // add filter
        {
            form_EquipFilter = new Equipment.EquipmentFilterNew(dbHelper);
            form_EquipFilter.Show();
        }
    }
}
