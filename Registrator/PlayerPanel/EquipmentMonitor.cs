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

        public void setTrackScaleEventHandler(object sender, Registrator.TrackPanel.TrackScaleEventArgs e){
            ProcessEquipObj.updateLengthOfViewedTrack(e.ZoomCoefficient); 
        }

        public DB.metro_db_controller DB_controller { 
            get { return _db_controller;}
            set {
                    _db_controller = new DB.metro_db_controller(value);
                    ProcessEquipObj = new Equipment.ProcessEquipment(_db_controller);
                    ProcessEquipObj.DataGridHandler += DataGridDataChangeHandler;
                } 
        }

        public void track_process(ref _irb_frame_info frameInfo)
        {
            ProcessEquipObj.track_process(ref frameInfo);
            ProcessEquipObj.cameraOffset = cameraOffset;
            ProcessEquipObj.apply_or_not = apply_or_not;
        }

        public EquipmentMonitor(DB.metro_db_controller db)
        {
            this.Text = "Контролируемое оборудование";
            InitializeComponent();

            dataGridView1.AllowUserToAddRows = false;
            _db_controller = db;
            ProcessEquipObj = new Equipment.ProcessEquipment(_db_controller);
            ProcessEquipObj.DataGridHandler += DataGridDataChangeHandler;
            ProcessEquipObj.DataGridRefreshHandler += DataGridDataRefreshHandler;
        }
        long previousCoord = 0;
        public void DataGridDataChangeHandler(object sender, Equipment.RefreshEquip e)
        {
            if (dataGridView1 != null && !dataGridView1.IsDisposed && IsHandleCreated)
            {
                BeginInvoke(new EventHandler(delegate
                        {
                            foreach (var item in _db_controller.get_objects_by_coordinate(e.mmCoordinate,e.LengthOfViewedTrack))
                            {
                                if ((item.shiftLine > previousCoord + e.LengthOfViewedTrack / 2) && (item.shiftLine < e.mmCoordinate + e.LengthOfViewedTrack / 2))
                                {
                                    dataGridView1.Rows.Insert(0, new object[] { item.name, e.mmCoordinate.ToString(), item.Npicket.ToString(), "0", item.maxTemperature.ToString(), item.shiftFromPicket.ToString() });
                                    if (dataGridView1.Rows.Count > 200)
                                        dataGridView1.Rows.RemoveAt(dataGridView1.Rows.Count - 1);
                                }
                            }
                            previousCoord = e.mmCoordinate;
                        }
                        ));
            }
        }
        public void DataGridDataRefreshHandler(object sender, Equipment.RefreshEquip e)
        {
            if (dataGridView1 != null && !dataGridView1.IsDisposed && IsHandleCreated)
            {
                BeginInvoke(new EventHandler(delegate
                {
                    dataGridView1.Rows.Clear();
                    previousCoord = e.mmCoordinate;
                    
                    foreach (var item in _db_controller.get_objects_by_coordinate(e.mmCoordinate,e.LengthOfViewedTrack))
                    {
                        if ((item.shiftLine < e.mmCoordinate + e.LengthOfViewedTrack) && (item.shiftLine > e.mmCoordinate - e.LengthOfViewedTrack / 2))
                        {
                            dataGridView1.Rows.Insert(0, new object[] { item.name, e.mmCoordinate.ToString(), item.Npicket.ToString(), "0", item.maxTemperature.ToString(), item.shiftFromPicket.ToString() });
                            if (dataGridView1.Rows.Count > 200)
                                dataGridView1.Rows.RemoveAt(dataGridView1.Rows.Count - 1);
                        }
                    }
                }
                        ));
            }
        }
        public void DataGridClearHandler(object sender, Equipment.dataGridClearEvent e)
        {
            if (dataGridView1 != null && !dataGridView1.IsDisposed && IsHandleCreated) {
                BeginInvoke(new EventHandler(delegate { dataGridView1.Rows.Clear(); } ));
            }
        }

        private void toolStripButton1_Click(object sender, EventArgs e) // add filter
        {
            form_EquipFilter = new Equipment.EquipmentFilterNew(_db_controller);
            form_EquipFilter.Show();
        }
    }
}
