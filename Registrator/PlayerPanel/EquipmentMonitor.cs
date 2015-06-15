using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ThermoRoutineLib;

namespace Registrator
{
    public partial class EquipmentMonitor : ToolWindow
    {
        public Equipment.ProcessEquipment ProcessEquipObj;
        DB.metro_db_controller _db_controller;
        public int curLine = 0;

        // filter
        private Equipment.EquipmentFilterNew form_EquipFilter;

        public int cameraOffset = 0;
        public bool apply_or_not = false;


        public DB.metro_db_controller DB_controller { 
            get { return _db_controller;}
            set {
                    _db_controller = new DB.metro_db_controller(value);
                    ProcessEquipObj = new Equipment.ProcessEquipment(_db_controller);
                    ProcessEquipObj.DataGridHandler += DataGridDataChangeHandler;
                    ProcessEquipObj.DataGridClearHandler += DataGridClearHandler;
                } 
        }

        public void track_process(ref _irb_frame_info frameInfo)
        {
            ProcessEquipObj.track_process(ref frameInfo);
            ProcessEquipObj.cameraOffset = cameraOffset;
            ProcessEquipObj.apply_or_not = apply_or_not;
        }

        public EquipmentMonitor()
        {
            this.Text = "Контролируемое оборудование";
            InitializeComponent();

            dataGridView1.AllowUserToAddRows = false;
        }

        public void DataGridDataChangeHandler(object sender, Equipment.dataGridDataChange e)
        {
            if (dataGridView1 != null && !dataGridView1.IsDisposed && IsHandleCreated)
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
            if (dataGridView1 != null && !dataGridView1.IsDisposed && IsHandleCreated)
            {
                BeginInvoke(new EventHandler(delegate
                {
                    dataGridView1.Rows.Clear();
                }
                ));
            }
        }

        private void toolStripButton1_Click(object sender, EventArgs e) // add filter
        {
            form_EquipFilter = new Equipment.EquipmentFilterNew(_db_controller);
            form_EquipFilter.Show();
        }
    }
}
