using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using ThermoRoutineLib;
using System.Windows.Input;
using System.Runtime.InteropServices;

namespace Registrator
{
    delegate void ComCreatePredicate();
    delegate void ComDeletePredicate();
    delegate void ComDispatcherPredicate(stopRequestedPredicate stopRequestedFunc);
    delegate bool stopRequestedPredicate();
    internal sealed class COM_dispatcher
    {
        private ComDispatcherPredicate _predicate;
        private ComDeletePredicate _delPred;
        private ComCreatePredicate _createPred;
        public COM_dispatcher(ComCreatePredicate createPred, ComDeletePredicate delPred)
        {
            _predicate = null;
            _delPred = delPred;
            _createPred = createPred;
            _is_object_created = false;
            _job_running = false;
            create_com_communication_thread();
        }

        void create_com_object()
        {
            _createPred();
        }
        ~COM_dispatcher()
        {
            CloseObject();
        }
        public void CloseObject()
        {
            _com_object_delete = true;
            _event.Set();
            _thread.Join();

            release_com_object();

        }
        public void DoPredicate(ComDispatcherPredicate predicate)
        {
            _predicate = predicate;
            if (_job_running)
                StopJob();
            _event.Set();
        }
        bool stop_requested()
        {
            return _stop_requested;
        }
        public void StopJob()
        {
            if (_job_running)
            {
                _stop_requested = true;
                while (_stop_requested)
                {
                    System.Windows.Forms.Application.DoEvents();
                    Thread.Sleep(500);
                }

            }
        }

        void release_com_object()
        {
            _delPred();
        }

        Thread _thread;
        AutoResetEvent _event;
        volatile  bool _com_object_delete;
        bool _job_running; 

        void create_com_communication_thread()
        {
            _event = new AutoResetEvent(false);
            _com_object_delete = false;
            _thread = new Thread(com_communication_thread);
            // _thread.ApartmentState = ApartmentState.STA;
            _thread.IsBackground = true;
            _thread.Start();

            create_com_object();

            while(!_is_object_created)
            {
                Thread.Sleep(200);
            }


        }
        private bool _is_object_created;
        volatile bool _stop_requested;
        private void com_communication_thread()
        {
            //create_com_object();
            _is_object_created = true;
            _job_running = false;
            _stop_requested = false;
            while (!_com_object_delete)
            {
                if (_predicate != null)
                {
                    _job_running = true;
                    _predicate(this.stop_requested);
                    _stop_requested = false;
                    _job_running = false;
                }

                _event.WaitOne();
            }
           // release_com_object();
            _is_object_created = false;
            _job_running = false;
        }

    }


    public partial class PlayerPanel
    {
 
        void initialize_movie_transit_interface()
        {
            _movie_state_lock = new object();
            _movie_state = MovieState.NONE;
        
        }
        void release_movie_transit_interface()
        {
            lock (_movie_state_lock)
            {
                if (_movie_state == MovieState.PLAY)
                {
                    stopPlayingLoop();
                }
            }
            
        }

        void stopPlayingLoop()
        {
            stop_routine_in_com_apartment();
        }

        void create_movie_transit()
        {
            _movie_transit = new MovieTransit();
        }
        void close_movie_transit()
        {
            if (_movie_transit != null)
            {
                _movie_transit.Close();
                _movie_transit = null;
            }
        }


        enum MovieState
        {
            NONE,
            STOP,
            PLAY,
            PAUSE
        }

        MovieState _movie_state;
        object _movie_state_lock;
        void stopMovie()
        {
            StopMoviePlaying();
        }
        void startMovieMode()
        {
            lock (_movie_state_lock)
            {
                set_movie_mode_ctrls_visibility(true);
                _movie_state = MovieState.STOP;
            }
        }

        bool _movie_loaded = false;

        private void reloadMovie()
        {
            reloadMovieCommand();
        }
        private void reloadMovieCommand()
        {
            ResetPlayerErrors();
            lock (_movie_state_lock)
            {
                if (_movie_state == MovieState.PLAY)
                {
                    stopPlayingLoop();
                }

                _movie_state = MovieState.STOP;
                reset_members();

                _movie_loaded = false;
                _equipment_list = null;
                string[] arr = new string[m_tripProject.Files.Count];

                for (int i = 0; i < arr.Length; i++)
                    arr[i] = (string)m_tripProject.Files[i];

                try
                {
                    Array errors;
                    
                    _movie_transit.SetIRBFiles(arr, out errors);
                    
                    List<string> status_list = new List<string>();
                    long index = 0;
                    int cols = errors.GetLength(errors.Rank - 1);
                    for (index = 0; index < cols; index++)
                    {
                        object status = errors.GetValue(index);
                        status_list.Add((string)status);
                    }

                    m_tripProject.files_loaded(status_list);


                    m_filesNumber = _movie_transit.FilesCount();
                    m_framesNumber = _movie_transit.FramesCount();
                }
                catch (COMException e)
                {
                    Console.WriteLine("playerPanel:reloadMovie:COMException : " + e.Message);
                    return;
                }
                if (m_framesNumber > 0)
                {
                    double msec;
                    _frame_coordinate coordinate = new _frame_coordinate();
                    _movie_transit.GetFramePositionInfo(0, out coordinate, out msec);

                    _first_frame_time = msec;
                }

                m_filterMask = new byte[m_framesNumber];
                SetAllFilterMask(1);
                m_framesToDisplay = m_framesNumber;

                ResetIndicator();

                _equipment_list = get_equipment_list();

                if (_equipment_list != null)
                {
                    FireEquListLoadedEvent(new EquListLoadedEvent(filter_equipment_list(_equipment_list)));
                }

                var map_objects_list = get_map_objects_list();
                FireMapObjectsLoadedEvent(new MapObjectsLoadedEvent(map_objects_list, 0));

                _movie_loaded = true;

                setCtrlsState(_movie_loaded);

            }
        }


        void set_movie_mode_ctrls_visibility(bool visibility)
        {
            setCtrlsState(visibility ? _movie_loaded:false);
            playPauseButton.Visible = visibility;
            pauseButton.Visible = visibility;
            slowPlayBtn.Visible = visibility;
            fastPlayBtn.Visible = visibility;
            speedLabel.Visible = visibility;
            framesFilter.Visible = visibility;
            shotButton.Visible = visibility;
            zoomIn.Visible = visibility;
            zoomOut.Visible = visibility;
            framesFilter.Visible = visibility;

        }
        private void setCtrlsState(bool movie_loaded)
        {
            playPauseButton.Checked = false;
            pauseButton.Checked = false;
            stopButton.Checked = false;

            playPauseButton.Enabled = movie_loaded;
            pauseButton.Enabled = false;
            stopButton.Enabled = false;

           // recordButton.Enabled = false;
            slowPlayBtn.Enabled = false;
            fastPlayBtn.Enabled = false;
            speedLabel.Enabled = false;

            zoomIn.Enabled = true;
            zoomOut.Enabled = true;

            framesFilter.Enabled = movie_loaded;



        }
 
        private void StopMoviePlaying()
        {
            lock (_movie_state_lock)
            {
                if (_movie_state == MovieState.PLAY)
                {
                    stopPlayingLoop();
                }

                // reset_members();
                _movie_state = MovieState.STOP;
                setCtrlsState(_movie_loaded);
                disconnect_playerCtrl_Canvas_MouseMove();
            }
        }

        void reset_members()
        {
            m_filesNumber = 0;
            m_framesNumber = 0;
            m_curFile = 0;
            m_curFrame = 0;
            _first_frame_time = 0;
            m_indexToGo = -1;
            m_speedFactor = 0;
            m_framesToDisplay = 0;

        }
        void startPlayPauseMovie()
        {
            lock (_movie_state_lock)
            {
                bool is_pausing = _movie_state == MovieState.PLAY;
                MovieState new_state = MovieState.PLAY;
              //  bool create_thread = false;
                if (!is_pausing)
                {
                    pauseButton.Enabled = true;
                    pauseButton.Checked = false;
                    stopButton.Enabled = true;
                    if (_movie_state == MovieState.STOP)
                    {
                        m_indexToGo = -1;
                    }

                    disconnect_playerCtrl_Canvas_MouseMove();

                }
                else
                {
                    connect_playerCtrl_Canvas_MouseMove();

                    new_state = MovieState.PAUSE;
                    pauseButton.Checked = true;
                    //stopButton.Enabled = false;
                }
                UpdateSpeedLabel();
                _movie_state = new_state;

                {
                    if (is_pausing)
                    {
                        stopPlayingLoop();
                       // ShowFrame(m_indexToGo);
                    }
                    else
                    {
                       run_routine_in_com_apartment(PlayMovie);
                    }
                }

            }
        }

        private void PauseMovie()
        {
            lock (_movie_state_lock)
            {

                if (_movie_state != MovieState.PLAY)
                {
                    return;
                }
                stopPlayingLoop();
                
                connect_playerCtrl_Canvas_MouseMove();

                ShowFrame(m_indexToGo);

                pauseButton.Checked = true;
                stopButton.Enabled = false;


                playPauseButton.Checked = false;
                stopButton.Checked = false;

                _movie_state = MovieState.PAUSE;
            } 
        }

        int m_speedFactor = 0;

        protected int m_filesNumber = 0;
        protected int m_framesNumber = 0;
        protected int m_curFile = 0;
        protected int m_curFrame = 0;
        protected double _first_frame_time = 0;

        int m_framesToDisplay = 0;

        protected int m_indexToGo = -1;
        protected MovieTransit _movie_transit;

        areas_dispatcher _movie_transit_areas_dispatcher;
        protected irb_frame_helper _movie_frame;

        void connect_playerCtrl_Canvas_MouseMove()
        {
            m_playerControl.drawingCanvas.MouseMove += new System.Windows.Input.MouseEventHandler(playerCtrl_Canvas_MouseMove);
        }
        void disconnect_playerCtrl_Canvas_MouseMove()
        {
            m_playerControl.drawingCanvas.MouseMove -= new System.Windows.Input.MouseEventHandler(playerCtrl_Canvas_MouseMove);
        }

        public void sliderMoved(object sender, System.Windows.RoutedPropertyChangedEventArgs<double> val)
        {
            if (!_movie_loaded)
                return;

            PauseMovie();

            if (InvokeRequired)
                Invoke(new ShowFrameDelegate(ShowFrame), new object[] { (int)val.NewValue });
            else
                ShowFrame((int)val.NewValue);
        }

        public void ShowFrame(bool left = false)
        {
            if(m_indexToGo == -1)
                m_indexToGo = 0;
            if (!left)
            {
                m_indexToGo++;

                if (m_speedFactor > 0)
                {
                    m_indexToGo += m_speedFactor;
                    if (m_indexToGo >= m_framesNumber)
                        m_indexToGo = m_framesNumber - 1;
                }
                if (m_speedFactor < 0)
                    System.Threading.Thread.Sleep(Math.Abs(m_speedFactor * 100));

                if (m_indexToGo >= m_framesNumber)
                    m_indexToGo = m_framesNumber - 1;
            }
            else
            {
                m_indexToGo--;

                if (m_speedFactor > 0)
                {
                    m_indexToGo -= m_speedFactor;
                    if (m_indexToGo < 0)
                        m_indexToGo = 0;
                }
                if (m_speedFactor < 0)
                    System.Threading.Thread.Sleep(Math.Abs(m_speedFactor * 100));

                if (m_indexToGo < 0)
                    m_indexToGo = 0;
            }
            if (InvokeRequired)
                Invoke(new ShowFrameDelegate(ShowFrame), new object[] { m_indexToGo });
            else
                ShowFrame(m_indexToGo);
        }
        void show_current_frame()
        {
            if (m_indexToGo != -1)
                ShowFrame(m_indexToGo);
        }
        public void ShowFrame(int frameNum)
        {
            if (_movie_transit == null)
                return;

            if (!_movie_loaded)
                return;

            if (_movie_state == MovieState.NONE || _movie_state == MovieState.PLAY)
                return;

            if (m_playerControl != null)
                m_playerControl.BlockImgUpdate = false;

 
            if (frameNum >= m_framesNumber && m_framesNumber - 1 > -1)
                frameNum = m_framesNumber - 1;

            m_curFrame = m_indexToGo = frameNum;

            object pixels = new ushort[1024 * 770];
            object temp_values = new float[300];
            bool res = false;
            _irb_frame_info frame_info = new _irb_frame_info();
            try
            {
                res = _movie_transit.GetFrame((short)frameNum,
                                            out frame_info,
                                                    ref pixels,
                                                    ref temp_values
                                                    );
            }
            catch (OutOfMemoryException)
            {
                _movie_transit.ClearMovieTransitCache();
            }

             if (res)
            {
                _movie_frame.reset_measure();
                _movie_frame.header.width = frame_info.image_info.width;
                _movie_frame.header.height = frame_info.image_info.height;


                _movie_frame.header.firstValidX = 0;
                _movie_frame.header.lastValidX = (ushort)(frame_info.image_info.width - 1);
                _movie_frame.header.firstValidY = 0;
                _movie_frame.header.lastValidY = (ushort)(frame_info.image_info.height - 1);
                 
                 // _movie_frame.header.firstValidX = frame_info.image_info.firstValidX;
                //_movie_frame.header.lastValidX = frame_info.image_info.lastValidX;
                //_movie_frame.header.firstValidY = frame_info.image_info.firstValidY;
                //_movie_frame.header.lastValidY = frame_info.image_info.lastValidY;
               
                 _movie_frame.header.calibration_min = frame_info.measure.calibration_min;
                 _movie_frame.header.calibration_max = frame_info.measure.calibration_max;
                _movie_frame.pixels = (ushort[])pixels;
                _movie_frame.temp_values = (float[])temp_values;

                byte[] raster;
                temp_interval calibration_interval = new temp_interval(0, 0);
                res = _image_helper.get_formated_frame_raster(_movie_frame, _grabber_areas_dispatcher,calibration_interval, out raster);

                if (frame_info.image_info.width == 1024) SetPlayerControlImage((byte[])raster, 1024, 768);
                else SetPlayerControlImage((byte[])raster, 640, 480);

                var measure = new CTemperatureMeasure(_movie_frame.min_temperature, _movie_frame.max_temperature, _movie_frame.avr_temperature,
                    frame_info.measure.object_tmin, frame_info.measure.object_tmax, 0,
                    frame_info.measure.calibration_min, frame_info.measure.calibration_max);

                var args = new object[] { measure };

                SetThermoScaleLimits(measure);

                 Invoke(new SetTemperatureMeasureDelegate(SetTemperatureMeasure), args);
                //Invoke(new SetTemperatureCalibrationLimitsDelegate(SetTemperatureCalibrationLimits), args);

                Invoke(new SetTimeDelegate(SetTime), new object[] { frame_info.timestamp });
                Invoke(new SetCurFrameNumDelegate(SetCurFrameNum), new object[] { (m_framesNumber < 1) ? 0 : m_curFrame + 1 });
            }//--------------------

            List<ulong> objs_coordinate = new List<ulong>();

            if (_equipment_list.Count != 0)
                foreach (var obj in _equipment_list)
                {
                    objs_coordinate.Add((ulong)obj.coord);
                }

            if (objs_coordinate.Count != 0)
            {
                //    uint distance;
                //    uint coord = 0;
                //    var index = _movie_transit.ChangeFrame(objs_coordinate.ToArray(), out distance, out coord);
                //    if (index != -1)
                //        FireFrameChangedEvent(new FrameChangedEvent(index, (int)distance, (int)coord));
            }

        }

        private void PlayMovie(stopRequestedPredicate stopRequestedFunc)
        {

            if (_movie_state != MovieState.PLAY || stopRequestedFunc())
            {
                return;
            }

            
            if (equipmentMonitor != null)
            {
                equipmentMonitor.ProcessEquipObj.refresh();
#if DEBUG
                equipmentMonitor.ProcessEquipObj.setLine(1);
                equipmentMonitor.ProcessEquipObj.metrCoordinate = 0;
                equipmentMonitor.ProcessEquipObj.tempCounter1 = 0;
#endif      
            }


            disconnect_playerCtrl_Canvas_MouseMove();
            if (m_playerControl != null)
                m_playerControl.BlockImgUpdate = false;
            int current_frame_index = 0;
            object raster = new byte[1024 * 770 * 4];

            for (int counter = 0; current_frame_index < m_framesNumber; current_frame_index++)
            {
                if (stopRequestedFunc())
                {
                    break;
                }

                if (m_indexToGo > -1)
                {
                    current_frame_index = m_indexToGo;
                    m_indexToGo = -1;
                }

                if (m_filterMask == null || current_frame_index >= m_filterMask.Length || m_filterMask[current_frame_index] == 0)
                    continue;

                counter++;

                if (current_frame_index >= m_framesNumber)
                    current_frame_index = m_framesNumber - 1;

                bool res = false;
                _irb_frame_info frame_info = new _irb_frame_info();
                try
                {
                    res = _movie_transit.GetFrameRaster((short)current_frame_index,
                                                out frame_info,
                                                ref raster);
                }
                catch (OutOfMemoryException)
                {
                    _movie_transit.ClearMovieTransitCache();
                }

  #if !DEBUG
                
                    if (equipmentMonitor.ProcessEquipObj.curLine != frame_info.coordinate.line)
                        equipmentMonitor.ProcessEquipObj.setLine(frame_info.coordinate.line);
                #endif
                //------------------------------------------------------- PROCESS EQUIPMENT ------------------------------------------------------------
                if (equipmentMonitor != null)
                   equipmentMonitor.ProcessEquipObj.process(ref frame_info);
                //--------------------------------------------------------------------------------------------------------------------------------------

                if (res)
                {
                    if (frame_info.image_info.width == 1024) SetPlayerControlImage((byte[])raster, 1024, 768);
                    else SetPlayerControlImage((byte[])raster, 640, 480);

                   //SetThermoScaleLimits(frame_info.measure.min, frame_info.measure.max, frame_info.measure.calibration_min, frame_info.measure.calibration_max);



                    var measure = new CTemperatureMeasure(frame_info.measure.tmin, frame_info.measure.tmax, frame_info.measure.tavr,
                        frame_info.measure.object_tmin, frame_info.measure.object_tmax, 0,                            
                        frame_info.measure.calibration_min, frame_info.measure.calibration_max);

                    var args = new object[] { measure };

                    SetThermoScaleLimits(measure);

                    Invoke(new SetTemperatureMeasureDelegate(SetTemperatureMeasure), args);
                    //Invoke(new SetTemperatureCalibrationLimitsDelegate(SetTemperatureCalibrationLimits), args);

                    Invoke(new SetCurFrameNumDelegate(SetCurFrameNum), new object[] { (current_frame_index == 0) ? 0 : current_frame_index + 1 });
                    Invoke(new SetTimeDelegate(SetTime), new object[] { frame_info.timestamp });
                }

                if (m_areasPanel != null && m_areasPanel.Template != null && m_areasPanel.Template.Areas != null)
                {
                    get_areas_temperature_measure();
                }

                if (_equipment_list.Count != 0)
                {
                    var filtered_equipment_list = filter_equipment_list(_equipment_list);

                    List<ulong> objs_coordinate = new List<ulong>();

                    foreach (var obj in filtered_equipment_list)
                    {
                        objs_coordinate.Add((ulong)obj.coord);
                    }

                    if (objs_coordinate.Count != 0)
                    {
                        //uint distance;
                        //uint coord;
                        //var index = _movie_transit.ChangeFrame(objs_coordinate.ToArray(), out distance, out coord);

                        //if (index != -1)
                        //    FireFrameChangedEvent(new FrameChangedEvent(index, (int)distance, (int)coord));
                    }
                }


                if (m_speedFactor > 0)
                {
                    current_frame_index += m_speedFactor;
                    if (current_frame_index >= m_framesNumber)
                        current_frame_index = m_framesNumber - 2;
                }
                if (m_speedFactor < 0)
                    System.Threading.Thread.Sleep(Math.Abs(m_speedFactor * 100));


            }

            m_indexToGo = current_frame_index;

           // lock (_movie_state_lock)
            {
                _movie_state = MovieState.PAUSE;
                connect_playerCtrl_Canvas_MouseMove();
            }
        }

        public void UpdateSpeedLabel()
        {
            if (InvokeRequired)
                BeginInvoke(new UpdateSpeedLabelDelegate(UpdateSpeedLabel));
            else
            {
                if (m_speedFactor == 0)
                    speedLabel.Text = "x1";
                else if (m_speedFactor > 0)
                    speedLabel.Text = "x" + (m_speedFactor + 1);
                else if (m_speedFactor < 0)
                    speedLabel.Text = "x" + (float)(1 + (float)m_speedFactor / 10);
            }
        }

        public void SpeedFactorIncrease()
        {
            if (m_speedFactor == 9)
                return;
            m_speedFactor += 1;
            UpdateSpeedLabel();
        }

        public void SpeedFactorDecrease()
        {
            if (m_speedFactor == -9)
                return;
            m_speedFactor -= 1;
            UpdateSpeedLabel();
        }

    }
}
