using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Threading.Tasks;

using ThermoRoutineLib;

namespace Registrator
{
    using db_object_info = DB.EFClasses.AllEquipment;

    public partial class EquipmentMonitor : ToolWindow
    {
        public Equipment.ProcessEquipment ProcessEquipObj;
        DB.metro_db_controller _db_controller;

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

        public void setTrackScaleEventHandler(object sender, TrackPanel.TrackScaleEventArgs e){
            ProcessEquipObj.updateLengthOfViewedTrack(e.ZoomCoefficient); 
        }

        public DB.metro_db_controller DB_controller { 
            get { return _db_controller;}
            set {
                    _db_controller = new DB.metro_db_controller(value);
                    ProcessEquipObj = new Equipment.ProcessEquipment(_db_controller);
                    //ProcessEquipObj.DataGridHandler += DataGridDataChangeHandler;
                } 
        }

        List<db_object_info> get_objects_on_current_path()
        {
            return _db_controller.get_objects_by_coordinate(0).ToList();
        }


        public void track_process(_irb_frame_info frameInfo)
        {
            _path_changed = _db_controller?.setLineAndPath(frameInfo.coordinate.line, frameInfo.coordinate.path) ?? false;

            if(_path_changed)
            {
                _current_object_index = -1;
#pragma warning disable CS4014 // Because this call is not awaited, execution of the current method continues before the call is completed
                DisplayObjectsInfo();
#pragma warning restore CS4014 // Because this call is not awaited, execution of the current method continues before the call is completed

                _current_coordinate = long.MaxValue;

            }

            DisplayCurrentPosition(frameInfo.coordinate);

            ProcessEquipObj.track_process(frameInfo, _path_changed);
        }

        public EquipmentMonitor(DB.metro_db_controller db)
        {
            this.Text = "Контролируемое оборудование";
            InitializeComponent();


            dataGridView1.AllowUserToAddRows = false;
            _db_controller = db;
            ProcessEquipObj = new Equipment.ProcessEquipment(_db_controller);
            //ProcessEquipObj.DataGridHandler += DataGridDataChangeHandler;
           // ProcessEquipObj.DataGridRefreshHandler += DataGridDataRefreshHandler;
        }


        void DisplayCurrentPosition(_frame_coordinate coordinate)
        {
            if (_objects == null)
                return;

            int prev_index = -1;
            int direction = (ushort)coordinate.direction == 0 ? 1 : -1;
            long next_object_coordinate = 0;
            while (true)
            {
                if (_current_object_index == -1)
                {
                    int object_index = object_index = _objects.FindIndex((obj) => obj.shiftLine * direction >= coordinate.coordinate * direction);
                    if (object_index == -1)
                        return;

                    var equ_object = _objects[object_index];
                    next_object_coordinate = equ_object.shiftLine;

                    if (equ_object.shiftLine != coordinate.coordinate)
                    {
                        object_index -= direction;

                        if (object_index == -1 || object_index == _objects.Count)
                            object_index += direction;
                    }

                    _current_object_index = object_index;
                }
                else
                {
                    if (_current_coordinate * direction > coordinate.coordinate * direction)
                    {
                        if (_current_object_index < dataGridView1.Rows.Count)
                            dataGridView1.Rows[_current_object_index].Selected = false;
                        _current_object_index = -1;
                        continue;
                    }

                    prev_index = _current_object_index;
                    var equ_object = _objects[_current_object_index];
                    next_object_coordinate = equ_object.shiftLine;

                    if (equ_object.shiftLine * direction < coordinate.coordinate * direction)
                    {
                        _current_object_index += direction;
                    }

                    if (_current_object_index == -1 || _current_object_index == _objects.Count)
                    {
                        _current_object_index += direction;
                    }
                }
                break;
            }

            if (prev_index != _current_object_index)
            {
                if (prev_index != -1 && prev_index < dataGridView1.Rows.Count)
                    dataGridView1.Rows[prev_index].Selected = false;

                if (_current_object_index < dataGridView1.Rows.Count)
                {
                    dataGridView1.Rows[_current_object_index].Selected = true;
                    scrollGrid();
                }

                var next_object_distance = Math.Abs(next_object_coordinate - coordinate.coordinate) / 1000;

                DistanceStatusLabel.Text = $"{next_object_distance} м";

            }

        }


        private void scrollGrid()
        {


            int halfWay = (dataGridView1.DisplayedRowCount(false) / 2);
            if ( dataGridView1.FirstDisplayedScrollingRowIndex + halfWay > _current_object_index ||
                (dataGridView1.FirstDisplayedScrollingRowIndex + dataGridView1.DisplayedRowCount(false) - halfWay) <= _current_object_index
               )
            {
                int targetRow = _current_object_index;

                targetRow = Math.Max(targetRow - halfWay, 0);
                dataGridView1.FirstDisplayedScrollingRowIndex = targetRow;

            }
        }

        const long default_non_updated_coords_span = 300 * 1000; //300 m


        bool _path_changed = false;
        long _current_coordinate = long.MaxValue;
        int  _current_object_index = -1;
        bool _grid_filled = false;

//        ILookup<long, db_object_info> _objects = null;
        List<db_object_info> _objects = null;

        async Task DisplayObjectsInfo()
        {
            dataGridView1.Rows.Clear();

            _objects = await Task.Run(() =>
            {
                return get_objects_on_current_path().OrderBy(i => i.shiftLine).ToList();
                //return get_objects_on_current_path().OrderBy(i => i.shiftLine).ToLookup(x => x.shiftLine);
            });

            foreach (var item in _objects)
            {
                //string picket_name = (await _db_controller.GetPicketInfoAsync(item.Picket)).Npiketa;
                string picket_name = await Task.Run(() =>
                {
                    return (_db_controller.GetPicketInfo(item.Picket)).Npiketa;
                });

                dataGridView1.Rows.Add(new object[] { item.Name, picket_name, item.shiftFromPicket / 1000, "0", item.maxTemperature });
            }

            //foreach (var item in _objects)
            //{
            //    foreach (var equ_object in item)
            //    {
            //        string picket_name = await Task.Run(() =>
            //        {
            //            return (_db_controller.GetPicketInfo(equ_object.Picket)).Npiketa;
            //        });


            //        dataGridView1.Rows.Add(new object[] { equ_object.Name, picket_name, equ_object.shiftFromPicket / 1000, "0", equ_object.maxTemperature });
            //    }
            //}

        }

        private void toolStripButton1_Click(object sender, EventArgs e) // add filter
        {
            form_EquipFilter = new Equipment.EquipmentFilterNew(_db_controller);
            form_EquipFilter.Show();
        }
    }
}
