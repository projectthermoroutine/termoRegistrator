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
            ProcessEquipObj.DataGridHandler += DataGridDataChangeHandler;
            ProcessEquipObj.DataGridClearHandler += DataGridClearHandler;
        }

        public EquipmentMonitor()
        {
            this.Text = "Контролируемое оборудование";
            InitializeComponent();

            dataGridView1.AllowUserToAddRows = false;
        }

        public void DataGridDataChangeHandler(object sender, Equipment.dataGridDataChange e)
        {
            if (dataGridView1 != null && !dataGridView1.IsDisposed)
            {
                BeginInvoke(new EventHandler(delegate
                    {
                        dataGridView1.Rows.Insert(0, new object[] { e.Name, e.mmCoordinate, e.Npicket, e.curMaxTemperature, e.maxTemperature, e.shiftFromPicket });
                        
                        if (dataGridView1.Rows.Count > 200)
                            dataGridView1.Rows.RemoveAt(dataGridView1.Rows.Count - 1);
                    }
                    ));
            }
        }
        public void DataGridClearHandler(object sender, Equipment.dataGridClearEvent e)
        {
            if (dataGridView1 != null && !dataGridView1.IsDisposed)
            {
                Invoke(new EventHandler(delegate
                {
                    dataGridView1.Rows.Clear();
                }
                ));
            }
        }

        private void toolStripButton1_Click(object sender, EventArgs e) // add filter
        {
            form_EquipFilter = new Equipment.EquipmentFilterNew(dbHelper);
            form_EquipFilter.Show();
        }
    }
}
