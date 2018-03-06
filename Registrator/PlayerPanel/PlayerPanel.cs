using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;
using System.Runtime.InteropServices;
using System.Threading;
using ThermoRoutineLib;
using System.Collections;
using DrawToolsLib;
using System.IO;
using System.Windows.Input;
using IRControls;
using System.Windows.Threading;
using System.Deployment.Application;
using System.Reflection;
using System.Linq;
using System.Diagnostics;

namespace Registrator
{
    using map_objects_list = List<measure_object>;

    public partial class PlayerPanel : DockContent
    {

       public enum PlayerMode
        {
            MOVIE,
            CAMERA,
            RECORD_PREVIEW
        }

        PlayerMode _mode;
        object _mode_lock;

        public class frame_data_helper
        {
            MovieTransit _movieProxy;
            TRWrapper _cameraProxy;
            PlayerPanel _parent;
            public frame_data_helper(PlayerPanel parent, MovieTransit movieProxy,TRWrapper cameraProxy)
            {
                _parent = parent;
                _movieProxy = movieProxy;
                _cameraProxy = cameraProxy;
            }

            public delegate float get_current_frame_point_temperature_func(ushort x, ushort y);
            public delegate UInt32 get_current_frame_id_func();
            public delegate bool get_area_info_func(int area_id, out _area_temperature_measure measureT);
            public delegate bool get_frame_position_info_func(uint frameNum, out _frame_coordinate coordinate, out double msec);
            public delegate Array get_frame_raw_data_func(int frameId);

            public get_current_frame_point_temperature_func get_current_frame_point_temperature;
            public get_current_frame_id_func get_current_frame_id;
            public get_area_info_func get_area_info;
            public get_frame_position_info_func get_frame_position_info;
            public get_frame_raw_data_func get_frame_raw_data;

            bool get_area_info_movie(int area_id, out _area_temperature_measure measureT)
            {
                return _movieProxy.GetAreaInfo((uint)area_id, out measureT);
            }
            bool get_area_info_camera(int area_id, out _area_temperature_measure measureT)
            {
                return _cameraProxy.GetAreaInfo((uint)area_id, out measureT);
            }

            bool get_frame_position_info_movie(uint frameNum, out _frame_coordinate coordinate, out double msec)
            {
                return _movieProxy.GetFramePositionInfo((uint)frameNum,
                                            out coordinate,
                                            out msec);
            }
            bool get_frame_position_info_camera(uint frameId, out _frame_coordinate coordinate, out double msec)
            {
                return _cameraProxy.GetFramePositionInfo((uint)frameId,
                                            out coordinate,
                                            out msec);
            }

            Array movie_get_frame_raw_data(int frame_index)
            {
                Array raw_data = null;
                try
                {
                    var data_size = _movieProxy.GetFrameRawData((UInt32)frame_index, out raw_data);
                }
                catch (System.Runtime.InteropServices.SafeArrayTypeMismatchException e)
                {
                    Console.WriteLine("frame_data_helper::movie_get_frame_raw_data:SafeArrayTypeMismatchException : " + e.Message);
                    return null;
                }

                catch (COMException e)
                {
                    Console.WriteLine("playerPanel:reloadMovie:COMException : " + e.Message);
                    return null;
                }

                return raw_data;
            }
            internal Array camera_get_frame_raw_data(int frame_id)
            {
                Array raw_data = null;
                try
                {
                    var data_size = _cameraProxy.GetFrameRawData((UInt32)frame_id, out raw_data);
                }
                catch (System.Runtime.InteropServices.SafeArrayTypeMismatchException e)
                {
                    Console.WriteLine("frame_data_helper::movie_get_frame_raw_data:SafeArrayTypeMismatchException : " + e.Message);
                    return null;
                }

                catch (COMException e)
                {
                    Console.WriteLine("playerPanel:reloadMovie:COMException : " + e.Message);
                    return null;
                }

                return raw_data;
            }


            public void selectProxy(PlayerMode proxyId)
            {
                switch (proxyId)
                {
                    case PlayerMode.MOVIE:
                        {
                            get_current_frame_point_temperature = _parent.get_current_frame_point_temperature_movie;
                            get_area_info = get_area_info_movie;
                            get_current_frame_id = _parent.get_current_frame_index_movie;
                            get_frame_raw_data = movie_get_frame_raw_data;
                            get_frame_position_info = get_frame_position_info_movie;

                            break;
                        }
                    case PlayerMode.CAMERA:
                        {
                            get_current_frame_point_temperature = _parent.get_current_frame_point_temperature_camera;
                            get_area_info = get_area_info_camera;
                            get_current_frame_id = _parent.get_current_frame_id_camera;
                            get_frame_raw_data = camera_get_frame_raw_data;
                            get_frame_position_info = get_frame_position_info_camera;
                            break;
                        }
                };
            }
        }//class frame_data_helper

        void setMode(PlayerMode mode)
        {
            lock (_mode_lock)
            {
                using (disable_toolbar_scoped toolbar_lock = new disable_toolbar_scoped(enableCtrlsToolbar))
                {

                    switch (mode)
                    {
                        case PlayerMode.MOVIE:
                            {
                                stopCameraMode();
                                camera_mode_ctrl_off();
                                //movie_mode_ctrl_on();
                                startMovieMode();
                                file_name_predicate = movie_file_name_predicate;

                                if (_is_need_reload_project)
                                {
                                    reloadMovie();
                                    _is_need_reload_project = false;
                                }
                                break;
                            }
                        case PlayerMode.CAMERA:
                            {
                                stopMovie();
                                camera_mode_ctrl_on();
                                movie_mode_ctrl_off();
                                m_tripProject.clearTermoFiles();
                                _is_need_reload_project = true;
                                startCameraMode();
                                file_name_predicate = movie_file_name_predicate;
                                break;
                            }
                        case PlayerMode.RECORD_PREVIEW:
                            {
                                startMovieMode();
                                if (_is_need_reload_project)
                                {
                                    reloadMovie();
                                    _is_need_reload_project = false;
                                }
                                break;
                            }
                    };

                    _frame_data_helper.selectProxy(mode);
                    _mode = mode;
                }
                FireChangeMode(new EventPlayerChangeMode(_mode));
            }
        }

        void enableCtrlsToolbar(bool enable)
        {
            playerToolBarCtrl.Enabled = cameraToolbarCtrl.Enabled = enable;
            PlayerState player_state = PlayerState.READY;
            System.Windows.Forms.Cursor _cursor = System.Windows.Forms.Cursors.Default;
            if (!enable)
            {
                _cursor = System.Windows.Forms.Cursors.WaitCursor;
                player_state = PlayerState.BUSY;
            }
            Cursor = _cursor;
            PlayerStateAquired(player_state);
        }

        COM_dispatcher _com_dispacher;

        double m_actualScale = 1;

        PlayerControl m_playerControl;

        protected TripProject m_tripProject = new TripProject();

        AreasPanel m_areasPanel = null;

        UInt32 m_objFilter = 0xFFFFFFFF;

        filter_table m_filterMask;

        protected bool m_formClosed;

        private irb_frame_image_helper _image_helper;

        PointsInfoManager _pointsInfoManager;
        frame_data_helper _frame_data_helper;

        private EquipmentMonitor equipmentMonitor = null;

        public void setMonitor(EquipmentMonitor equipmentMonitorArg)
        {
            equipmentMonitor = equipmentMonitorArg;
        }

        public PointsInfoManager pointsInfoManager { get { return _pointsInfoManager; } }

        #region Конструктор

        private DB.metro_db_controller _db_controller;
        
        private int _cameraOffset;
        private bool _autostart;
        private bool _simulator_mode;
        private ThermoRoutineLib.Logger _lib_logger;

        public PlayerPanel(
            DB.metro_db_controller db_controller
            , int cameraOffset_Arg
            , EventHandler<EventPlayerChangeMode> ChangeModeCallback
            , transit_project_settings_t transit_project_settings
            , bool autostart, bool simulator_mode
            )
        {
            _autostart = autostart;
            _simulator_mode = simulator_mode;
            _cameraOffset = cameraOffset_Arg;
            _db_controller = null;
            if (db_controller != null)
                _db_controller = new DB.metro_db_controller(db_controller);
            //_is_need_set_calibration_mode = true;
            _is_need_reload_project = false;
            _calibration_type = IMAGE_CALIBRATION_TYPE.MIN_MAX;
            _image_helper = new irb_frame_image_helper();
            _mode_lock = new object();
            _mode = PlayerMode.MOVIE;
            m_actualScale = 1;

            m_filterMask = new filter_table();
            _movie_frame = new irb_frame_helper();
            _pointsInfoManager = new PointsInfoManager();

            m_formClosed = false;

            _grabber_areas_dispatcher = new areas_dispatcher();
            _grabber_areas_dispatcher.set_areas_mask_size(1024, 768);
            _movie_transit_areas_dispatcher = new areas_dispatcher();
            _movie_transit_areas_dispatcher.set_areas_mask_size(1024, 768);

            _metro_map = new metro_map();

            KeyPreview = true;
            InitializeComponent();
            
            m_playerControl = new PlayerControl(true, Properties.Settings.Default.debug_info);

            temperature_label_height = m_playerControl.Temperature_label.Height;

            palleteSelectionCtrl.SelectedIndexChanged -= palleteSelectionCtrl_SelectedIndexChanged;
            palleteSelectionCtrl.SelectedIndex = 0;
            palleteSelectionCtrl.SelectedIndexChanged += palleteSelectionCtrl_SelectedIndexChanged;
            cameraPalleteSelectionCtrl.SelectedIndexChanged -= palleteSelectionCtrl_SelectedIndexChanged;
            cameraPalleteSelectionCtrl.SelectedIndex = 0;
            cameraPalleteSelectionCtrl.SelectedIndexChanged += palleteSelectionCtrl_SelectedIndexChanged;

            m_tripProject.FilePath = transit_project_settings.project_folder;
            m_tripProject.IRBFilesPath = transit_project_settings.files_folder;
            m_tripProject.TripProjectChangedHandler += TripProjectChanged;
            Text = transit_project_settings.name;
            m_tripProject.CameraDirection = transit_project_settings.camera_side;

            m_playerControl.filmProgress.ValueChanged += new System.Windows.RoutedPropertyChangedEventHandler<double>(sliderMoved);

            m_playerControl.drawingCanvas.AreaAddedEventHandler += AreaAddedEventFired;
            m_playerControl.drawingCanvas.AreaChangedEventHandler += AreaChangedEventFired;

            m_playerControl.KeyPressedEventHandler += KeyPressedEventFired;

            m_playerControl.TermoScaleVisible = false;
            m_playerControl.ActualScale = m_actualScale;
            elementHost1.SuspendLayout();
            elementHost1.Child = m_playerControl;
            elementHost1.PerformLayout();

            m_playerControl.LimitsChangedEventHandler += LimitsChangedEventFired;
            m_playerControl.LimitsModeChangedEventHandler += LimitsModeChangedEventFired;
            ResetIndicator();

            {
                _lib_logger = new Logger();
                string current_directory = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                string logs_dir = current_directory + @"\" + Properties.Settings.Default.logs_dir;

                string log_config_filename = current_directory + @"\" + Properties.Settings.Default.log_config_data;

                var current_time = DateTime.Now;
                string log_name_pattern = "Registrator_" + current_time.ToString("yyyy_MM_dd_HH_mm_ss") + "_" + Process.GetCurrentProcess().Id.ToString();
                _lib_logger.InitializeLogger(log_config_filename, logs_dir, log_name_pattern);
            }

            _com_dispacher = new COM_dispatcher(create_com_objects, close_com_objects);

            initialize_camera_interface();
            initialize_movie_transit_interface();

            EventHandlerChangeMode += ChangeModeCallback;

            _frame_data_helper = new frame_data_helper(this, _movie_transit, m_tvHandler);
            
            
            if(autostart)
                setMode(PlayerMode.CAMERA);
            else
                setMode(PlayerMode.MOVIE);

            setPallete(!autostart);

            create_map_key_actions();

        }

        ~PlayerPanel()
        {
            AllResourcesCloser();
         //   _lib_logger.Close();

        }

        #endregion

        void run_routine_in_com_apartment(ComDispatcherPredicate routine)
        {
           _com_dispacher.DoPredicate(routine);
        }
        void stop_routine_in_com_apartment()
        {
            _com_dispacher.StopJob();
        }

        void create_com_objects()
        {
            create_movie_transit();
            create_camera(_cameraOffset,!_autostart);
        }
        void close_com_objects()
        {
            close_movie_transit();
            close_camera();
        }

        public void AllResourcesCloser()
        {
            if (m_formClosed)
                return;
            FinishAll();
            release_camera_interface();
            release_movie_transit_interface();

            _com_dispacher.CloseObject();

            m_formClosed = true;
        }

        #region Свойства
        public PlayerControl PlayerControl { get { return m_playerControl; } set { m_playerControl = value; } }
        public int FilesNumber { get { return m_filesNumber; } }

        //public void CoordEventFired(object sender, CoordEvent e)
        //{
        //    m_curCoord = (int)(e.Km * 100000);
        //    m_curMsec = e.Msec;

        //    if (m_tvHandler != null)
        //        LoadObjectsFromDb();


        //}


        public filter_table FilterMask
        {
            get { return m_filterMask; }
            set { m_filterMask = value; }
        }

        public int FramesToDisplay
        {
            get { return m_framesToDisplay; }
            set { m_framesToDisplay = value; }
        }

        public int SpeedFactor
        {
            get { return m_speedFactor; }
            set { m_speedFactor = value; }
        }

        public AreasPanel AreasControl
        {
           get { return m_areasPanel; }
           set { m_areasPanel = value; }
        }
        #endregion

        public void FireFrameShotListenerStateChangeEvent(FrameShotListenerStateChangeEvent e)
        {
            EventHandler<FrameShotListenerStateChangeEvent> handler = FrameShotListenerStateChangeEventHandler;
            if (handler != null)
                handler(this, e);
            
        }

        private void ResetIndicator()
        {
            if (InvokeRequired)
            {
                Invoke(new SetTemperatureMeasureDelegate(SetTemperatureMeasure), new object[] { new CTemperatureMeasure() });
                Invoke(new SetCurFrameNumDelegate(SetCurFrameNum), new object[] { 0 });
                Invoke(new SetFramesAmountDelegate(SetFramesAmount), new object[] { m_framesToDisplay });
            }
            else
            {
                SetTemperatureMeasure(new CTemperatureMeasure());
                SetCurFrameNum(0);
                SetFramesAmount( m_framesToDisplay );
            }
        }

 
        private void ObjectLimitsFired(float bottom, float top)
        {
            if (m_playerControl == null)
                return;

            m_playerControl.SetBottomObjectLimit(bottom);
            m_playerControl.SetTopObjectLimit(top);

        }

        private void FileFromGrabberFired(String fullFilePath)
        {
            FireFileFromGrabberEvent(new FileFromGrabberEvent(fullFilePath));
            //_is_need_reload_project = true;
        }

        public delegate void SetThermoScaleLimitsDelegate(CTemperatureMeasure measure);

        public volatile bool _disable_thermoscale_limits_change = false;

        private void SetThermoScaleLimits(CTemperatureMeasure measure)
        {
            if (InvokeRequired)
            {

                Invoke(new SetThermoScaleLimitsDelegate(SetThermoScaleLimits), new object[] { measure });
            }
            else
            {
                m_playerControl.SetCalibrationLimits(measure.calibration_max, measure.calibration_min);

                m_playerControl.termoScale.ObjectLowerValue = measure.objTmin;
                m_playerControl.termoScale.ObjectTopValue = measure.objTmax;

                if (!_disable_thermoscale_limits_change)
                {
                    m_playerControl.LimitsChangedEventHandler -= LimitsChangedEventFired;

                    m_playerControl.SetFrameLimits(measure.min, measure.max);

                    m_playerControl.LimitsChangedEventHandler += LimitsChangedEventFired;
                }
            }
        }

        void TripProjectChanged(object sender, EventArgs e)
        {
            if (_mode == PlayerMode.MOVIE)
            {
                _is_need_reload_project = false;
                StopGrabbing();
                reloadMovie();
            }
            else
                _is_need_reload_project = true;
        }

        public void SynchronizationCompletedEventFired(object sender, SynchronizationCompletedEvent e)
        {
            reloadMovie();
        }

        public void DesynchroCompletedEventFired(object sender, DesynchroCompletedEvent e)
        {
            reloadMovie();
        }

        public void ObjectFilterSetEventFired(object sender, ObjectFilterSetEvent e)
        {
            if (_equipment_list == null)
                return;

            m_objFilter = e.ObjectFilter;

            var filtered_equipment_list = filter_equipment_list(_equipment_list);
            FireEquListLoadedEvent(new EquListLoadedEvent(filtered_equipment_list));

            List<ulong> objs_coordinate = new List<ulong>();

            foreach (var obj in filtered_equipment_list)
            {
                objs_coordinate.Add((ulong)obj.coord);
            }
        }

  
        private delegate void SetPlayerControlImageDelegate(byte[] raster, int width, int height);

        public void SetPlayerControlImage(byte[] raster, int width, int height)
        {
            if(InvokeRequired)
            {
                Invoke(new SetPlayerControlImageDelegate(SetPlayerControlImage), new object[] {raster, width, height});
            }
            else
            {
                m_playerControl.SetImage(raster, width, height);
            }
        }

        public TripProject TripProject
        {
            get { return m_tripProject; }
            set
            {
                if (m_tripProject != null)
                    m_tripProject.TripProjectChangedHandler -= TripProjectChanged;
                m_tripProject = value;
                m_tripProject.TripProjectChangedHandler += TripProjectChanged;
            }

        }


 
        public delegate void ShowFrameDelegate(int frameNum);

        public void StopGrabbing()
        {
            if (_mode == PlayerMode.CAMERA)
            {
                stopCameraMode();
            }
            else
            {
                StopMoviePlaying();
            }

            m_playerControl.ResetImage();
            ResetIndicator();

            SetPlayerMode(previewModeButton.Checked ? (byte)1 : (byte)0);

     //       long num1 = System.GC.GetTotalMemory(false);
        }

        public void ItemSelectedEventFired(object sender, ItemSelectedEvent e)
        {
            if (_equipment_list != null)
            {
                if (_equipment_list.Count <= e.Index || e.Index < 0)
                    return;
//                m_indexToGo = _movie_transit.GetFrameNum((int)_equipment_list[e.Index].frame_number);
            }
        }

        public delegate void SetPlayerModeDelegate(byte mode);

        public void SetPlayerMode(byte mode)
        {
            if (InvokeRequired)
            {
                BeginInvoke(new SetPlayerModeDelegate(SetPlayerMode), new object[] { mode });
            }
            else
            {
                m_playerControl.SetPlayerMode(mode);
            }
        }

        public void PalleteChangedEventFired(object sender, PalleteChangedEvent e)
        {
            if (e.FileName.Equals(""))
            {
                _movie_transit.SetDefaultPallete();
                m_tvHandler.SetDefaultPallete();
                palleteSelectionCtrl.SelectedIndex = 0;
                return;
            }
            _movie_transit.SetPallete(e.FileName);
            m_tvHandler.SetPallete(e.FileName);
            palleteSelectionCtrl.SelectedIndex = ((e.FileName.Equals(PalleteChangedEvent.PAL_FULLCOLOR)) ? 1 : 2);
        }


        delegate void SetPalleteDelegate(System.Windows.Media.Color[] colors);
        private void setPallete(bool is_movie_transit)
        {
            uint colors_number = 0;
            int len = 0;
            object pallete = null;
            if (is_movie_transit)
             {
                 len = _movie_transit.GetPalleteLength(out colors_number);
                 pallete = new Int32[len];
                 _movie_transit.GetPallete(ref pallete);
             }
             else
             {
                 len = m_tvHandler.GetPalleteLength(out colors_number);
                 pallete = new Int32[len];
                 m_tvHandler.GetPallete(ref pallete);
             }

             CPalette _palette = new CPalette();
             _palette.image = (Int32[])pallete;
             _palette.numI = colors_number;

             _image_helper.Palette = _palette;

            Int32[] pal = (Int32[])pallete;

            System.Windows.Media.Color[] colors = new System.Windows.Media.Color[len];

            for (int i = 0; i < len; i++)
            {
                Color color = Color.FromArgb(pal[i]);
                colors[i] = System.Windows.Media.Color.FromArgb(color.A, color.R, color.G, color.B);// 
            }

           // if (m_playerControl.termoScale.InvokeRequired)
            //    Invoke(new SetPalleteDelegate(delegate(System.Windows.Media.Color[] Colors) { m_playerControl.termoScale.Palette = Colors; }), new object[] { colors });
          //  else
              m_playerControl.termoScale.Palette = colors;
        }

  
        public virtual void CoordEventFired(object sender, CoordEvent e)
        {
            if (!previewModeButton.Checked)
                return;

            List<ulong> objs_coordinate = new List<ulong>();

            foreach (var obj in _equipment_list)
            {
                objs_coordinate.Add((ulong)obj.coord);
            }

            //uint distance;
            //uint coord = (uint)(e.Km * 100000);
            //var index = _movie_transit.ChangeFrame_proezd(objs_coordinate.ToArray(), coord, out distance);
            //if (index != -1)
            //    FireFrameChangedEvent(new FrameChangedEvent(index, (int)distance, (int)coord));
        }

        public virtual void MapReceivedEventFired(object sender, MapReceivedEvent e)
        {

            if (!previewModeButton.Checked)
                return;

            var map_object_list = _metro_map.GetMapObjectsFromFile(e.File);
            FireMapObjectsLoadedEvent(new MapObjectsLoadedEvent(map_object_list, 0));

            _equipment_list = _metro_map.LoadEquipmentList();

            if (_equipment_list != null)
            {
                FireEquListLoadedEvent(new EquListLoadedEvent(_equipment_list));
            }

        }

        public delegate void PerformStopClickDelegate();

        public void PerformStopClick()
        {
            if(InvokeRequired)
                BeginInvoke(new PerformStopClickDelegate(PerformStopClick));
            else
                stopButton.PerformClick();
        }

        public delegate void PerformClickDelegate();

        public void PerformClick()
        {
            BeginInvoke(new PerformClickDelegate(PerformClick2));
        }

        public void PerformClick2()
        {
            recordButton.PerformClick();
        }

        public delegate void StartStopEventFiredDelegate(object sender, StartStopEvent e);

        public virtual void StartStopEventFired(object sender, StartStopEvent e)
        {

            if (InvokeRequired)
            {
                BeginInvoke(new StartStopEventFiredDelegate(StartStopEventFired), new object[]{ sender, e});
                return;
            }

            if (!previewModeButton.Checked)
                return;
            if (!m_recStarted)
                recordButton.PerformClick();
            else
                stopButton.PerformClick();
        }

        public virtual void FireEquListLoadedEvent(EquListLoadedEvent e)
        {
            EventHandler<EquListLoadedEvent> handler = EquListLoadedHandler;

            if (handler != null)
            {
                handler(this, e);
            }
        }

        public virtual void FireFrameChangedEvent(FrameChangedEvent e)
        {
            EventHandler<FrameChangedEvent> handler = FrameChangedHandler;

            if (handler != null)
            {
                handler(this, e);
            }
        }

        public virtual void FireMapObjectsLoadedEvent(MapObjectsLoadedEvent e)
        {

            EventHandler<MapObjectsLoadedEvent> handler = MapObjectsLoadedHandler;

            if (handler != null)
            {

                handler(this, e);

            }

        }

        public virtual void FireEditingFinishedEvent(EditingFinishedEvent e)
        {

            EventHandler<EditingFinishedEvent> handler = EditingFinishedHandler;

            if (handler != null)
            {

                handler(this, e);

            }

        }

         private void FireRecModeChangeEvent(RecModeChangeEvent e)
        {
            EventHandler<RecModeChangeEvent> handler = RecModeChangeEventHandler;
            if (handler != null)
                handler(this, e);
        }

        private void FireNeedToEraseTrackEvent(NeedToEraseTrackEvent e)
        {
            EventHandler<NeedToEraseTrackEvent> handler = NeedToEraseTrackEventHandler;
            if (handler != null)
                handler(this, e);
        }


        public void SetAllFilterMask(byte flag = 1)
        {
            if (flag == 1)
                m_filterMask.all_enabled = true;
            if (flag == 0)
                m_filterMask.all_disabled = true;

        }

        private bool _is_need_reload_project;
        public void UpdateWithProjectFiles()
        {
            TripProject.Update();
        }

        public void FinishAll()
        {
            if(_mode == PlayerMode.CAMERA)
            {
                StopGrabbing();
            }
            else
            {
                StopMoviePlaying();
            }

       }

        private delegate void StrechDelegate();

        private void Strech()
        {
            if (InvokeRequired)
            {
                Invoke(new StrechDelegate(Strech));
            }
            else
            {
                m_playerControl.InverseImageStrech();
            }
        }

        private delegate void ScaleDelegate(double scale);

        private void Scale(double scale)
        {

            if (InvokeRequired)
            {
                Invoke(new ScaleDelegate(Scale), new object[] { scale });
            }
            else
            {
                m_playerControl.ResetImageStrech();
                m_playerControl.ActualScale = m_actualScale;
            }
        }

        public void FilteredEventFired(object sender, FilteredEvent e)
        {
            if (e == null)
                return;
            m_framesToDisplay = e.FramesToDisplay;
            if (e.FilterMask != null)
                m_filterMask = e.FilterMask;
            //reloadMovie();
            ApplyFilterToMovie();

        }

        public virtual void AnalizedEventFired(object sender, AnalizeEvent e)
        {
            AnalyzeForm af = new AnalyzeForm(_movie_transit, _db_controller);
            af.ShowDialog(this);
        }

        List<object_info> get_objects_by_coordinate(_frame_coordinate frame_coordinate)
        {
            List<object_info> result = new List<object_info>();
            var objects = _db_controller.get_objects_by_coordinate(frame_coordinate.coordinate /*+ frame_coordinate.camera_offset*/, 50);

            foreach (var cur_object in objects)
            {
                result.Add(new object_info(cur_object.Code));
            }
            return result;
        }

        protected ShotDesc ExtractFrameInfo(Int32 frameNum)
        {
            if (_equipment_list == null)
                return new ShotDesc();
            
            ShotDesc desc = new ShotDesc();

            double msec = 0;
            _frame_coordinate coordinate = new _frame_coordinate();

           _frame_data_helper.get_frame_position_info((uint)frameNum,
                                            out coordinate,
                                            out msec);

            map_point_info point_info = new map_point_info();
            map_point point = new map_point(coordinate.path, coordinate.line, (long)coordinate.coordinate);

            point_info.get_info(point);

            desc.Msec = msec;
            desc.map_point_info = point_info;
            desc.FrameNum = frameNum;

            /* TEMP */

            _pointsInfoManager.Add(point_info_factory.create_point_info(frameNum, coordinate, msec,
                                                                        frameId => _frame_data_helper.get_frame_raw_data(frameId), 
                                                                        get_objects_by_coordinate));

            /* ---------- */


            // ----------------- 08.05.15 -----------------------------------------------------------------------------------------------------------
            desc.Distance = coordinate.coordinate + coordinate.camera_offset;
            var resStartCoordLine = _db_controller.get_line_ID(coordinate.line);
            if (resStartCoordLine != -1)
            {
                desc.Line = resStartCoordLine;
                try
                {
                    desc.Path = Convert.ToInt32(coordinate.path);
                }
                catch(InvalidCastException)
                {
                    MessageBox.Show("Не удается преобразовать номер пути из строки в число.", "", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }

            return desc;
        }

 
        public void PausePlay()
        {
            startPlayPauseMovie();
        }

        delegate void KeyActionDelegate();
        private Dictionary<Key, KeyActionDelegate> _mapKeyToAction;
        
        void create_map_key_actions()
        {
            _mapKeyToAction = new Dictionary<Key, KeyActionDelegate> { 
                                                { Key.Space, new KeyActionDelegate(startPlayPauseMovie) },
                                                { Key.Up, new KeyActionDelegate(SpeedFactorIncrease) }, 
                                                { Key.Down, new KeyActionDelegate(SpeedFactorDecrease) }, 
                                                { Key.Left, new KeyActionDelegate(ShowLeftFrame) }, 
                                                { Key.Right, new KeyActionDelegate(ShowRightFrame) } 
            };
        }

        void ShowLeftFrame()
        {
            ShowFrame(true);
        }
        void ShowRightFrame()
        {
            ShowFrame(false);
        }
        public void KeyPressedEventFired(object sender, KeyPressed e)
        {
            if(_mapKeyToAction.ContainsKey(e.PressedKey))
            {
                var key_action = _mapKeyToAction[e.PressedKey];
                key_action();
            }
        }

        public delegate void SetFramesAmountDelegate(int amount);
        public delegate void SetCurFrameNumDelegate(int num);
        public delegate void SetTimeDelegate(double time);
        public delegate void SetIRBFramePositionDelegate(string line, string path, long coords, Int32 picket, Int32 offset, UInt32 counter);


        public delegate void SetTemperatureMeasureDelegate(CTemperatureMeasure measure);
        public void SetTemperatureMeasure(CTemperatureMeasure measure)
        {
            m_playerControl.MinT = measure.min;
            m_playerControl.MaxT = measure.max;
            m_playerControl.Max_MinT = measure.max - measure.min;
            m_playerControl.AvrT = measure.avg;
            //m_playerControl.SetFrameLimits(measure.min, measure.max);

        }

        public delegate void SetTemperatureCalibrationLimitsDelegate(CTemperatureMeasure measure);
        public void SetTemperatureCalibrationLimits(CTemperatureMeasure measure)
        {
            m_playerControl.SetCalibrationLimits(measure.calibration_max, measure.calibration_min);
        }



        public void SetFramesAmount(int amount)
        {
            m_playerControl.filmProgress.ValueChanged -= new System.Windows.RoutedPropertyChangedEventHandler<double>(sliderMoved);
            m_playerControl.FrameAmount = amount;
            m_playerControl.filmProgress.ValueChanged += new System.Windows.RoutedPropertyChangedEventHandler<double>(sliderMoved);
        }

        public void SetCurFrameNum(int num)
        {
            m_playerControl.filmProgress.ValueChanged -= new System.Windows.RoutedPropertyChangedEventHandler<double>(sliderMoved);
            m_playerControl.CurFrameNum = num;
            m_playerControl.filmProgress.ValueChanged += new System.Windows.RoutedPropertyChangedEventHandler<double>(sliderMoved);
        }

        public void SetTime(double time)
        {
            m_playerControl.Time = irb_frame_time_helper.build_time_string_from_time(time);
        }

        public void SetIRBFramePosition(string line, string path, long coords, Int32 picket, Int32 offset, UInt32 counter)
        {
            m_playerControl.Line = line;
            m_playerControl.Path = path;
            m_playerControl.Position = coords;
            m_playerControl.setPositionByPicket(picket,offset);
            m_playerControl.Counter = counter;
        }

 
        public delegate void UpdateSpeedLabelDelegate();

        public void LimitsChangedEventFired(object sender, LimitsChangedEvent e)
        {
            _disable_thermoscale_limits_change = true;

            _calibration_type = IMAGE_CALIBRATION_TYPE.MANUAL;


            _movie_transit.SetPaletteCalibration((float)e.Minimum, (float)e.Maximum);
            m_tvHandler.SetPaletteCalibration((float)e.Minimum, (float)e.Maximum);
            _image_helper.SetPaletteCalibration((float)e.Minimum, (float)e.Maximum);
            
            _movie_transit.SetPaletteCalibrationMode(_calibration_mode.MANUAL);
            m_tvHandler.SetPaletteCalibrationMode(_calibration_mode.MANUAL);
            _image_helper.set_calibration_type(_calibration_type);

            if (_mode == PlayerMode.MOVIE)
                show_current_frame();

        }

        //private bool _is_need_set_calibration_mode;
        IMAGE_CALIBRATION_TYPE _calibration_type;
        public void LimitsModeChangedEventFired(object sender, LimitsModeChangedEvent e)
        {
            //_is_need_set_calibration_mode = true;
            _calibration_mode mode = _calibration_mode.MIN_MAX;
            _calibration_type = IMAGE_CALIBRATION_TYPE.MIN_MAX;

            if (e.Mode == LimitsModeChangedEvent.LimitsMode.CALIBRATION_MODE)
            {
                mode = _calibration_mode.NONE;
                _calibration_type = IMAGE_CALIBRATION_TYPE.NONE;
            }
            else if (e.Mode == LimitsModeChangedEvent.LimitsMode.FRAME_MODE)
            {
                mode = _calibration_mode.MIN_MAX;
                _calibration_type = IMAGE_CALIBRATION_TYPE.MIN_MAX;
            }
            else if (e.Mode == LimitsModeChangedEvent.LimitsMode.OBJECT_MODE)
            {
                mode = _calibration_mode.AVERAGE;
                _calibration_type = IMAGE_CALIBRATION_TYPE.AVERAGE;
            }

            //if (_mode == PlayerMode.MOVIE)
            _movie_transit.SetPaletteCalibrationMode(mode);
            m_tvHandler.SetPaletteCalibrationMode(mode);
            _image_helper.set_calibration_type(_calibration_type);

            _disable_thermoscale_limits_change = false;

            if (_mode == PlayerMode.MOVIE)
                show_current_frame();

           
        }

        void set_pallete_calibration_mode(_calibration_mode mode)
        {
            _movie_transit.SetPaletteCalibrationMode(mode);
            m_tvHandler.SetPaletteCalibrationMode(mode);
        }

        private void PlayerPanel_FormClosing(object sender, FormClosingEventArgs e)
        {
            AllResourcesCloser();
        }

        private System.Windows.Point _cursor_position = new System.Windows.Point(0, 0);
        private System.Windows.Point _cursor_position_for_temp_label = new System.Windows.Point(0, 0);
        void get_cursor_point_temperature()
        {
            var temperature_point = _frame_data_helper.get_current_frame_point_temperature((ushort)_cursor_position.X, (ushort)_cursor_position.Y);
            var temperature_str = temperature_point.ToString("f1");

            if (InvokeRequired)
                Invoke(new EventHandler(delegate
                {
                    m_playerControl.t_point.Content = "Tp: " + temperature_str + " \u00B0" + "C";
                    m_playerControl.Temperature_label.RenderTransform = new System.Windows.Media.TranslateTransform(_cursor_position_for_temp_label.X + 5, _cursor_position_for_temp_label.Y - temperature_label_height);
                    m_playerControl.Temperature_label.Content = temperature_str + " \u00B0" + "C";
                }
                ));
            else
            {
                m_playerControl.t_point.Content = "Tp: " + temperature_str + " \u00B0" + "C";

                m_playerControl.Temperature_label.RenderTransform = new System.Windows.Media.TranslateTransform(_cursor_position_for_temp_label.X + 5, _cursor_position_for_temp_label.Y - temperature_label_height);
                m_playerControl.Temperature_label.Content = temperature_str + " \u00B0" + "C";
            }
        }

        public float get_current_frame_point_temperature_movie(ushort x, ushort y)
        {

            float point_temperature = 0;
            int frame_index = m_curFrame;
            if (is_movie_playing())
            {
                frame_index = current_frame_index;
            }
            if (frame_index >= 0)
            {

                var real_frame_index = frame_index;
                if (!m_filterMask.is_filtered(ref real_frame_index))
                    return point_temperature;


                var res = _movie_transit.get_pixel_temperature((uint)real_frame_index,
                                        x, y,
                                        out point_temperature
                                        );
            }
            return point_temperature;
        }

        float get_current_frame_point_temperature_camera(ushort x, ushort y)
        {

            float point_temperature = 0;
            if (is_camera_grabbing())
            {
                var res = m_tvHandler.get_pixel_temperature(_current_camera_frame_id,
                                            x, y,
                                            out point_temperature
                                            );
            }

            return point_temperature;
        }

        void playerCtrl_Canvas_MouseEnter(object sender, System.Windows.Input.MouseEventArgs e)
        {
            _is_cursor_position_valid = true;
            m_playerControl.Temperature_label.Visibility = System.Windows.Visibility.Visible;
        }
        void playerCtrl_Canvas_MouseLeave(object sender, System.Windows.Input.MouseEventArgs e)
        {
            m_playerControl.Temperature_label.Visibility = System.Windows.Visibility.Hidden;
            _is_cursor_position_valid = false;
        }

        double temperature_label_height = 0;
        void playerCtrl_Canvas_MouseMove(object sender, System.Windows.Input.MouseEventArgs e)
        {
            if (e.MiddleButton == MouseButtonState.Released && e.RightButton == MouseButtonState.Released)
            {
                _cursor_position = e.GetPosition(m_playerControl.drawingCanvas);


                ushort x = (ushort)_cursor_position.X;
                ushort y = (ushort)_cursor_position.Y;
                var point_temperature = _frame_data_helper.get_current_frame_point_temperature(x, y);

                var temperature_str = point_temperature.ToString("f1");

                m_playerControl.t_point.Content = "Tp: " + temperature_str + " \u00B0" + "C";

                _cursor_position_for_temp_label = e.GetPosition(m_playerControl.frameContainer);

                m_playerControl.Temperature_label.RenderTransform = 
                    new System.Windows.Media.TranslateTransform(_cursor_position_for_temp_label.X + 5,
                                                                _cursor_position_for_temp_label.Y - temperature_label_height
                                                                );
                m_playerControl.Temperature_label.Content = temperature_str + " \u00B0" + "C";

            }
        }

        void refresh_frame()
        {
            object raster = new byte[1024 * 770 * 4];
            bool res = false;
            _irb_frame_info frame_info = new _irb_frame_info();
           
            try
            {
                if (_mode == PlayerMode.CAMERA)
                {
                    UInt32 frameId;
                    res = m_tvHandler.GetNextRealTimeFrameRaster(out frameId,
                                           out frame_info,
                                           ref raster);
                }

                else
                {
                    if (!is_movie_playing() && m_curFrame >= 0)
                        res = _movie_transit.GetFrameRaster((uint)m_curFrame,
                                           out frame_info,
                                           ref raster);
                }

            }
            catch (OutOfMemoryException)
            {
                _movie_transit.ClearMovieTransitCache();
                m_tvHandler.ClearGrabbingCache();
            }

            if (res && m_areasPanel != null && m_areasPanel.Template != null && m_areasPanel.Template.Areas != null)
            {
                get_areas_temperature_measure();
            }
        }
        
        private void ts_cameraOffset_Click(object sender, EventArgs e)
        {
            camShiftSettings form_cameraOffsetSetting = new camShiftSettings(current_camera_offset, apply_camera_offset);
            form_cameraOffsetSetting.camShiftSetHandler += form_camShift_camShiftSetHandler;
            form_cameraOffsetSetting.cameraOffsetCheckedSetHandler += form_cameraOffsetSetting_cameraOffsetCheckedSetHandler;
            form_cameraOffsetSetting.ShowDialog();
        }

        void form_cameraOffsetSetting_cameraOffsetCheckedSetHandler(object sender, eventCameraOffset e)
        {
            apply_camera_offset = false;
        }

        private bool apply_camera_offset = false;
        private int  current_camera_offset = 0;

        void form_camShift_camShiftSetHandler(object sender, eventCameraOffset e)
        {
            if (_mode == PlayerMode.MOVIE)
            {
                if (e.save)
                {
                    _movie_transit.WriteCameraOffset(e.offset);
                }

                apply_camera_offset = true;
                current_camera_offset = e.offset;
            }
        }
     }
}
