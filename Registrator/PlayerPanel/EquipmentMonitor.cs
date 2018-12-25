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
using WeifenLuo.WinFormsUI.Docking;

using ThermoRoutineLib;

namespace Registrator
{
    using db_object_info = DB.EFClasses.AllEquipment;

    public partial class EquipmentMonitor : DockContent
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

        sealed class CancelableTask
        {
            public Task task = null;
            public bool cancel = false;
        }

        List<CancelableTask> _display_objects_tasks = new List<CancelableTask>();

        public void track_process(_irb_frame_info frameInfo)
        {
            _path_changed = _db_controller?.setLineAndPath(frameInfo.coordinate.line, frameInfo.coordinate.path) ?? false;

            if(_path_changed)
            {
                _objects = null;

                foreach (var cancelable_task in _display_objects_tasks)
                {
                    cancelable_task.cancel = true;
                }

                _current_coordinate = long.MaxValue;
                _current_object_index = -1;

                var task = new CancelableTask();

#pragma warning disable CS4014 // Because this call is not awaited, execution of the current method continues before the call is completed
                task.task = DisplayObjectsInfo(task);
#pragma warning restore CS4014 // Because this call is not awaited, execution of the current method continues before the call is completed

                _display_objects_tasks.Add(task);

            }

            if (!string.IsNullOrEmpty(frameInfo.coordinate.path))
            {
                _current_coordinate = frameInfo.coordinate.coordinate;
                _current_frame_coordinate = frameInfo.coordinate;
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


        int DeterminCurrentPositionAmongObjects(_frame_coordinate coordinate)
        {

            if (coordinate.coordinate <= _objects_coordinates[0])
                return 0;

            if (coordinate.coordinate >= _objects_coordinates.Last())
                return _objects_coordinates.Count - 1;


            int direction = (ushort)coordinate.direction == 0 ? 1 : -1;

            int object_index = _objects_coordinates.BinarySearch(coordinate.coordinate);

            if (object_index < 0)
                object_index = ~object_index;

            if (object_index >= _objects_coordinates.Count)
                object_index--;

            if (object_index >= 0)
            {
                var found_coordinate = _objects_coordinates[object_index];

                while(true)
                {
                    object_index -= direction;
                    if (object_index == -1 || object_index == _objects.Count)
                    {
                        object_index += direction;
                        break;
                    }

                    if(found_coordinate > _objects_coordinates[object_index])
                    {
                        object_index += direction;
                        break;
                    }
                }
            }

            return object_index;
        }

        void DisplayCurrentPosition(_frame_coordinate coordinate)
        {
            if ((_objects?.Count ?? 0) == 0)
                return;

            int prev_index = _current_object_index;

            _current_object_index = DeterminCurrentPositionAmongObjects(coordinate);

            if (prev_index != _current_object_index)
            {
                if (prev_index != -1 && prev_index < dataGridView1.Rows.Count)
                    dataGridView1.Rows[prev_index].Selected = false;

                if (_current_object_index < dataGridView1.Rows.Count)
                {
                    dataGridView1.Rows[_current_object_index].Selected = true;
                    scrollGrid();
                }
            }

            long next_object_coordinate = _objects[_current_object_index].shiftLine;

            var next_object_distance = Math.Abs(next_object_coordinate - coordinate.coordinate) / 1000;
            DistanceStatusLabel.Text = $"{next_object_distance} м";

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


        bool _path_changed = false;
        long _current_coordinate = long.MaxValue;
        int  _current_object_index = -1;

        _frame_coordinate _current_frame_coordinate;

        List<db_object_info> _objects = null;
        List<long> _objects_coordinates = null;

        sealed class OnExit : IDisposable
        {
            public void Dispose()
            {
                var _action = Action;
                if (_action != null)
                {
                    Action = null;
                    _action();
                }
            }

            public Action Action;
        }

        async Task DisplayObjectsInfo(CancelableTask task)
        {
            dataGridView1.Rows.Clear();

            var objects = await Task.Run(() =>
            {
                return get_objects_on_current_path().OrderBy(i => i.shiftLine).ToList();
            });

            _objects = objects;

            using (var on_exit = new OnExit { Action = () => _display_objects_tasks.Remove(task) })
            {
                if (task.cancel)
                    return;

                List<long> objects_coordinates = new List<long>();

                _objects_coordinates = objects_coordinates;

                foreach (var item in objects)
                {
                    if (task.cancel)
                        return;

                    objects_coordinates.Add(item.shiftLine);
                    //string picket_name = (await _db_controller.GetPicketInfoAsync(item.Picket)).Npiketa;
                    string picket_name = await Task.Run(() =>
                    {
                        return (_db_controller.GetPicketInfo(item.Picket)).Npiketa;
                    });

                    if (task.cancel || IsDisposed)
                        return;

                    dataGridView1.Rows.Add(new object[] { item.Name, picket_name, item.shiftFromPicket / 1000, "0", item.maxTemperature });
                }

                if (task.cancel)
                    return;

            }

            var current_frame_coordinate = _current_frame_coordinate;
            if (!string.IsNullOrEmpty(current_frame_coordinate.path))
            {
                DisplayCurrentPosition(current_frame_coordinate);
            }
        }

        private void toolStripButton1_Click(object sender, EventArgs e) // add filter
        {
            form_EquipFilter = new Equipment.EquipmentFilterNew(_db_controller);
            form_EquipFilter.Show();
        }
    }
}
