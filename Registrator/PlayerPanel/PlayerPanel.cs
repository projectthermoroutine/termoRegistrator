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


namespace Registrator
{
    using map_objects_list = List<measure_object>;

    public partial class PlayerPanel : DockContent
    {

        enum PlayerMode
        {
            MOVIE,
            CAMERA
        }

        PlayerMode _mode;
        object _mode_lock;

        void setMode(PlayerMode mode)
        {
            lock (_mode_lock)
            {

                disableCtrlsToolbar();

                if (mode == PlayerMode.MOVIE)
                {
                    stopCameraMode();
                    set_camera_mode_ctrls_visibility(false);
                    startMovieMode();
                    if (_is_need_reload_project){
                        reloadMovie();
                        _is_need_reload_project = false;
                    }
                    //if (_is_need_set_calibration_mode){
                    //    _is_need_set_calibration_mode = false;
                    //    _movie_transit.SetPaletteCalibrationMode((_calibration_mode)_calibration_type);
                    //    _image_helper.set_calibration_type(_calibration_type);
                    //}
                }
                else
                {
                    stopMovie();
                    set_movie_mode_ctrls_visibility(false);
                    m_tripProject.clearTermoFiles();
                    _is_need_reload_project = true;
                    startCameraMode();
                    //if (_is_need_set_calibration_mode)
                    //{
                    //    _is_need_set_calibration_mode = false;
                    //    _image_helper.set_calibration_type(_calibration_type);
                    //}
                }

                _mode = mode;
                enableCtrlsToolbar();
            }
        }

        void disableCtrlsToolbar()
        {
            playerToolBarCtrl.Enabled = false;
            Cursor = System.Windows.Forms.Cursors.WaitCursor;
            PlayerStateAquired(PlayerState.BUSY);
        }
        void enableCtrlsToolbar()
        {
            playerToolBarCtrl.Enabled = true;
            Cursor = System.Windows.Forms.Cursors.Default;
            PlayerStateAquired(PlayerState.READY);
        }

        COM_dispatcher _com_dispacher;

        double m_actualScale = 1;

        PlayerControl m_playerControl;

        protected TripProject m_tripProject = new TripProject();


        AreasPanel m_areasPanel = null;


        UInt32 m_objFilter = 0xFFFFFFFF;

        byte[] m_filterMask = null;

        protected bool m_formClosed;

        private irb_frame_image_helper _image_helper;

        private EquipmentMonitor equipmentMonitor;

        public void setMonitor(ref EquipmentMonitor equipmentMonitorArg)
        {
            equipmentMonitor = equipmentMonitorArg;
        }

        #region Конструктор
        public PlayerPanel()
        {
            _is_need_set_calibration_mode = true;
            _is_need_reload_project = false;
            _calibration_type = IMAGE_CALIBRATION_TYPE.MIN_MAX;
            _image_helper = new irb_frame_image_helper();
            _mode_lock = new object();
            _mode = PlayerMode.MOVIE;
            m_actualScale = 1;

            _movie_frame = new irb_frame_helper();

            m_formClosed = false;

            _grabber_areas_dispatcher = new areas_dispatcher();
            _grabber_areas_dispatcher.set_areas_mask_size(1024,768);
            _movie_transit_areas_dispatcher = new areas_dispatcher();
            _movie_transit_areas_dispatcher.set_areas_mask_size(1024, 768);

            _metro_map = new metro_map();

            KeyPreview = true;
            InitializeComponent();
           
            m_playerControl = new PlayerControl();

            palleteSelectionCtrl.SelectedIndexChanged -= palleteSelectionCtrl_SelectedIndexChanged;
            palleteSelectionCtrl.SelectedIndex = 0;
            palleteSelectionCtrl.SelectedIndexChanged += palleteSelectionCtrl_SelectedIndexChanged;

            m_tripProject.TripProjectChangedHandler += TripProjectChanged;

            m_playerControl.filmProgress.ValueChanged += new System.Windows.RoutedPropertyChangedEventHandler<double>(sliderMoved);

            m_playerControl.drawingCanvas.AreaAddedEventHandler += AreaAddedEventFired;
            m_playerControl.drawingCanvas.AreaChangedEventHandler += AreaChangedEventFired;

            m_playerControl.KeyPressedEventHandler += KeyPressedEventFired;

            m_playerControl.TermoScaleVisible = false;
            m_playerControl.ActualScale = m_actualScale;
            elementHost1.Child = m_playerControl;

            m_playerControl.LimitsChangedEventHandler += LimitsChangedEventFired;
            m_playerControl.LimitsModeChangedEventHandler += LimitsModeChangedEventFired;
            ResetIndicator();

            _com_dispacher = new COM_dispatcher(create_com_objects, close_com_objects);

            initialize_camera_interface();
            initialize_movie_transit_interface();
            
            setMode(PlayerMode.MOVIE);

            setPallete();

            create_map_key_actions();

        }

        public PlayerPanel(UInt32 objFilter)
            : this()
        {
            m_objFilter = objFilter;
        }

        public PlayerPanel(String tripProjectDirPath)
            : this(new TripProject(tripProjectDirPath))
        {

        }

        public PlayerPanel(TripProject tripProject)
            : this()
        {
            TripProject = tripProject;
        }

        ~PlayerPanel()
        {
            AllResourcesCloser();

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
            create_camera();
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
        public int FilesNumber { get { return m_filesNumber; } set { m_filesNumber = value; } }

        //public void CoordEventFired(object sender, CoordEvent e)
        //{
        //    m_curCoord = (int)(e.Km * 100000);
        //    m_curMsec = e.Msec;

        //    if (m_tvHandler != null)
        //        LoadObjectsFromDb();


        //}

 
        public byte[] FilterMask
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

        private void SetThermoScaleLimits(CTemperatureMeasure measure)
        {
            if (InvokeRequired)
            {

                Invoke(new SetThermoScaleLimitsDelegate(SetThermoScaleLimits), new object[] { measure });
            }
            else
            {
                m_playerControl.LimitsChangedEventHandler -= LimitsChangedEventFired;

                m_playerControl.SetCalibrationLimits(measure.calibration_max, measure.calibration_min);
                m_playerControl.SetFrameLimits(measure.min, measure.max);
                m_playerControl.termoScale.ObjectLowerValue = measure.objTmin;
                m_playerControl.termoScale.ObjectTopValue = measure.objTmax;

                m_playerControl.LimitsChangedEventHandler += LimitsChangedEventFired;

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

            uint distance;
            uint coord;
            List<ulong> objs_coordinate = new List<ulong>();

            foreach (var obj in filtered_equipment_list)
            {
                objs_coordinate.Add((ulong)obj.coord);
            }
            //var index = _movie_transit.ChangeFrame(objs_coordinate.ToArray(), out distance, out coord);
            //if (index != -1)
            //    FireFrameChangedEvent(new FrameChangedEvent(index, (int)distance, (int)coord));

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
        private void setPallete()
        {
            uint colors_number = 0;
            int len = 0;
            object pallete = null;
             if (_mode == PlayerMode.MOVIE)
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
            if (m_filterMask == null)
                return;
            for(int i = 0 ; i < m_filterMask.Length ; i++)
                m_filterMask[i] = flag;

        }

        private bool _is_need_reload_project;
        public void UpdateWithProjectFiles(string[] files)
        {
            TripProject.Files = new List<string>(files);
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
            reloadMovie();
        }

        public void AreaAddedEventFired(object sender, AreaAddedEvent e)
        {
            if(e.Type == ToolType.Rectangle)
                AddRectArea((short)e.ID, (short)e.X, (short)e.Y, (short)e.Width, (short)e.Height);
            if (e.Type == ToolType.Ellipse)
                AddEllipsArea((short)e.ID, (short)e.X, (short)e.Y, (short)e.Width, (short)e.Height);
            
            FireAreaAddedEvent(e);
        }

        public void AreaChangedEventFired(object sender, AreaChangedEvent e)
        {
            if (e.Type == ToolType.Rectangle)
                ChangeRectArea((short)e.Id, (short)e.X, (short)e.Y, (short)e.Width, (short)e.Height);
            if (e.Type == ToolType.Ellipse)
                ChangeEllipsArea((short)e.Id, (short)e.X, (short)e.Y, (short)e.Width, (short)e.Height);
            
            FireAreaChangedEvent(e);
        }

        public virtual void FireAreaChangedEvent(AreaChangedEvent e)
        {
            EventHandler<AreaChangedEvent> handler = AreaChangedEventHandler;

            if (handler != null)
                handler(this, e);
        }

        public virtual void FireAreaAddedEvent(AreaAddedEvent e)
        {
            EventHandler<AreaAddedEvent> handler = AreaAddedEventHandler;

            if (handler != null)
                handler(this, e);

        }

        public virtual void AnalizedEventFired(object sender, AnalizeEvent e)
        {
            AnalyzeForm af = new AnalyzeForm(_movie_transit, _metro_map,_equipment_list);
            af.ShowDialog(this);
        }

        public void NeedShotEventFired(object sender, NeedShotEvent e)
        {

            ShotDesc.ShotType shotType = e.Type;

            ShotDesc desc = ExtractFrameInfo(m_curFrame, previewModeButton.Checked);

            desc.TypeOfShot = shotType;

            FireFrameShotedEvent(new FrameShotedEvent(desc));
        }

        private void FireFrameShotedEvent(FrameShotedEvent e)
        {
            EventHandler<FrameShotedEvent> handler = FrameShotedEventHandler;
            if (handler != null)
                handler(this, e);
        }

        protected ShotDesc ExtractFrameInfo(Int32 frameNum, bool isRecording)
        {
            if (_equipment_list == null)
                return new ShotDesc();
            
            ShotDesc desc = new ShotDesc();

            double msec = 0;
            _frame_coordinate coordinate = new _frame_coordinate();

            if (!previewModeButton.Checked)
            {
                _movie_transit.GetFramePositionInfo((uint)frameNum,
                                            out coordinate,
                                            out msec);

                msec -= _first_frame_time;
            }
            //else
            //    m_tvHandler.GetCamFrameInfo(filter_equipment_list(_equipment_list).ToArray(),
            //                                frameNum,
            //                                out distance,
            //                                out msec,
            //                                out line,
            //                                out path,
            //                                out picket,
            //                                out offset,
            //                                out objName,
            //                                out peregon);


            ulong days = (ulong)msec;
            ulong time = (ulong)((msec - days) * 86400*1000);


            map_point_info point_info = new map_point_info();

            map_point point = new map_point((short)coordinate.path, (short)coordinate.line, (long)coordinate.coordinate);

            point_info.get_info(point);

            desc.Msec = time;
            desc.map_point_info = point_info;
            desc.PicketNOffset = point.picket + "+" + (int)(point.offset / 100);
            desc.FrameNum = frameNum;

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

 
        public delegate void UpdateSpeedLabelDelegate();


 
        public void SynchroNeedEventFired(object sender, SynchroNeedEvent e)
        {
            if(e.RzFilePath == null || e.RzFilePath.Equals(""))
                return;
            StopGrabbing();
            SynchroDialog sd = new SynchroDialog(_movie_transit, e.RzFilePath);
            sd.SynchronizationCompletedEventHandler += SynchronizationCompletedEventFired;
            sd.ShowDialog();
        }

        public void DesynchroNeedEventFired(object sender, DesynchroNeedEvent e)
        {
            StopGrabbing();
            DesynchroDialog dsd = new DesynchroDialog();
            dsd.TvHandler = _movie_transit;
            dsd.DesynchroCompletedEventHandler += DesynchroCompletedEventFired;
            dsd.ShowDialog();
        }

 
        public void LimitsChangedEventFired(object sender, LimitsChangedEvent e)
        {
            _calibration_type = IMAGE_CALIBRATION_TYPE.MANUAL;

            _movie_transit.SetPaletteCalibration((float)e.Minimum, (float)e.Maximum);
            m_tvHandler.SetPaletteCalibration((float)e.Minimum, (float)e.Maximum);


            if (_mode == PlayerMode.MOVIE)
                show_current_frame();

        }

        private bool _is_need_set_calibration_mode;
        IMAGE_CALIBRATION_TYPE _calibration_type;
        public void LimitsModeChangedEventFired(object sender, LimitsModeChangedEvent e)
        {
            _is_need_set_calibration_mode = true;
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
            
            //_image_helper.set_calibration_type(_calibration_type);
           
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


        irb_frame_helper get_current_frame()
        {
            if (_mode == PlayerMode.MOVIE)
                return _movie_frame;

            return null;
        }


        void playerCtrl_Canvas_MouseMove(object sender, System.Windows.Input.MouseEventArgs e)
        {
            var frame = get_current_frame();
            if (frame == null)
                return;
            if (e.MiddleButton == MouseButtonState.Released && e.RightButton == MouseButtonState.Released)
            {
                var position = e.GetPosition(m_playerControl.drawingCanvas);
                ushort x = (ushort)position.X;
                ushort y = (ushort)position.Y;

                if (frame.is_position_valid(x, y))
                {
                    float point_temperature;
                    frame.get_pixel_temperature(x, y, out point_temperature);
                    point_temperature -= 273.15f;
                    m_playerControl.t_point.Content = "Tp: " + point_temperature.ToString("f1") + " \u00B0" + "T";
                }
            }
        }


    }
}
