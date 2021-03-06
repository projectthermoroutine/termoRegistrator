﻿using System;
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
        public void setCameraOffsetManual(long offset)
        {
            if(ProcessEquipObj!=null)
                ProcessEquipObj.SetCameraOffset(offset);
        }

        public void ResetCameraOffset()
        {
            if(ProcessEquipObj!=null)
                ProcessEquipObj.ResetCameraOffset();
        }

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
        }

        public EquipmentMonitor(DB.metro_db_controller db)
        {
            coordinates_interval = new Interval(0, 0);
            this.Text = "Контролируемое оборудование";
            InitializeComponent();


            dataGridView1.AllowUserToAddRows = false;
            _db_controller = db;
            ProcessEquipObj = new Equipment.ProcessEquipment(_db_controller);
            ProcessEquipObj.DataGridHandler += DataGridDataChangeHandler;
            ProcessEquipObj.DataGridRefreshHandler += DataGridDataRefreshHandler;
        }


        const long default_non_updated_coords_span = 300 * 1000; //300 m
        long non_updated_coords_span = default_non_updated_coords_span;
        List<Registrator.DB.ResultEquipCode> cached_objects = new List<DB.ResultEquipCode>();
        long viewing_coords_span = default_non_updated_coords_span / 3; // 100 m


        public void DataGridDataChangeHandler(object sender, Equipment.RefreshEquip e)
        {
            if (dataGridView1 != null && !dataGridView1.IsDisposed && IsHandleCreated)
            {
                BeginInvoke(new EventHandler(delegate
                        {
                            if (e.mmCoordinate - non_updated_coords_span < coordinates_interval.min ||
                                e.mmCoordinate + non_updated_coords_span > coordinates_interval.max)
                            {
                                coordinates_interval.max = e.mmCoordinate + non_updated_coords_span;
                                coordinates_interval.min = e.mmCoordinate - non_updated_coords_span;
                                cached_objects = _db_controller.get_objects_by_coordinate(e.mmCoordinate, non_updated_coords_span).ToList();
                            }

                            DisplayObjectsInfo(e.mmCoordinate);
                        }
                        ));
            }
        }

        public struct Interval
        {
            public Interval(long min, long max)
            {
                this.min = min; this.max = max;
            }
            public long min;
            public long max;
        }

        Interval coordinates_interval;

        void DisplayObjectsInfo(long coordinate)
        {
            dataGridView1.Rows.Clear();
            foreach (var item in cached_objects)
            {
                if ((item.shiftLine < coordinate + viewing_coords_span / 2) && (item.shiftLine > coordinate - viewing_coords_span / 2))
                {
                    dataGridView1.Rows.Insert(0, new object[] { item.name, item.picket, ((int)(item.shiftFromPicket / 1000)).ToString(), "0", item.maxTemperature.ToString()});
                    if (dataGridView1.Rows.Count > 200)
                        dataGridView1.Rows.RemoveAt(dataGridView1.Rows.Count - 1);
                }
            }
        }

        public void DataGridDataRefreshHandler(object sender, Equipment.RefreshEquip e)
        {
            if (dataGridView1 != null && !dataGridView1.IsDisposed && IsHandleCreated)
            {
                BeginInvoke(new EventHandler(delegate
                {

                    coordinates_interval.max = e.mmCoordinate + non_updated_coords_span;
                    coordinates_interval.min = e.mmCoordinate - non_updated_coords_span;
                    cached_objects = _db_controller.get_objects_by_coordinate(e.mmCoordinate, non_updated_coords_span).ToList();

                    DisplayObjectsInfo(e.mmCoordinate);
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
