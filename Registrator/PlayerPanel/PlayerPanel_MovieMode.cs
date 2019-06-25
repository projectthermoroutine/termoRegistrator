using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using ThermoRoutineLib;
using System.Windows.Input;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.IO;
using System.Threading.Tasks;
using System.Timers;

namespace Registrator
{

    public partial class PlayerPanel
    {

        public delegate void MovieFilesLoadingEvent(Int32 value);
        public event MovieFilesLoadingEvent MovieFilesLoading;

        public void MovieFilesLoadingProgress(Int32 value)
        {
            MovieFilesLoading?.Invoke(value);
        }

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
            _movie_transit.EnableBadPixelsControl(Properties.Settings.Default.enableBadPixelsControl,Properties.Settings.Default.BadPixelsSettings);

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

        private bool is_movie_playing() { return _movie_state == MovieState.PLAY; }

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
                movie_mode_ctrl_on();
                _movie_state = MovieState.STOP;
                reset_members();
                m_filterMask.all_enabled = true;
                m_framesToDisplay = 0;
            }
        }

        bool _movie_loaded = false;

        delegate bool FileNamePredicate(string file_name);
        FileNamePredicate file_name_predicate;

        bool movie_file_name_predicate(string file_name)
        {
            const string tmp_file_extention = ".irb.tmp";
            if (file_name == "")
                return false;

            if (file_name.Length <= tmp_file_extention.Length)
                return true;

            if (file_name.EndsWith(tmp_file_extention))
                return false;

            return true;
        }
        private void setIRBFiles()
        {
            List<string> files = new List<string>();
            foreach (var file_name in m_tripProject.Files)
                if (file_name_predicate(file_name))
                    files.Add(file_name);
            string[] arr = new string[files.Count];

            for (int i = 0; i < arr.Length; i++)
                arr[i] = files[i];

            int tryes = 2;
            while (tryes > 0)
            {
                try
                {
                    Array errors;

                   _movie_loaded = _movie_transit.SetIRBFiles(arr, out errors);

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
                    tryes = 0;
                }
                catch (COMException e)
                {
                    Console.WriteLine("playerPanel:reloadMovie:COMException : " + e.Message);
                    return;
                }
                catch (OutOfMemoryException)
                {
                    _movie_transit.ClearMovieTransitCache();
                    tryes--;
                }

            }
        }

        void resetMovieTransit()
        {
            _movie_transit.Reset();
        }
        private async Task reloadMovieBackground()
        {
            using (disable_toolbar_scoped toolbar_lock = new disable_toolbar_scoped(enableCtrlsToolbar))
            using (System.Timers.Timer timer = new System.Timers.Timer(100))
            {
                MovieFilesLoadingProgress(0);
                Int32 step = 0;

                timer.Elapsed += (sender, e) => { step += 10; Invoke(new Action<Int32>(MovieFilesLoadingProgress), new object[] { step }); };
                timer.Start();
                
                await Task.Run(new Action(setIRBFiles));
                
                MovieFilesLoadingProgress(-1);
            }
        }


        private void reloadMovie()
        {
            if (InvokeRequired)
            {
                Invoke(new EventHandler(delegate { reloadMovieCommand(); }));
            }
            else
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
                setIRBFiles();
                //reloadMovieBackground();

                if (m_framesNumber > 0)
                {
                    if (m_framesNumber > 0)
                    {
                        double msec;
                        _frame_coordinate coordinate = new _frame_coordinate();
                        _movie_transit.GetFramePositionInfo(0, out coordinate, out msec);

                        _first_frame_time = msec;
                    }

                }

                m_filteredFramesNumber = m_framesNumber;
                m_framesToDisplay = m_framesNumber;

                if (!m_filterMask.all_enabled)
                {
                    m_filteredFramesNumber = m_filterMask.Size;
                    m_framesToDisplay = m_filteredFramesNumber;
                }

                ResetIndicator();

                setCtrlsState(_movie_loaded);

            }
        }


        void movie_mode_ctrl_on()
        {
            previewModeButton.Visible = true;
            set_movie_mode_ctrls_visibility(true);
            playerToolBarCtrl.Visible = true;
        }

        void movie_mode_ctrl_off()
        {
            playerToolBarCtrl.Visible = false;
            set_movie_mode_ctrls_visibility(false);
        }


        void record_preview_mode_ctrl_on()
        {
            movie_mode_ctrl_on();
            previewModeButton.Visible = false;
        }

        void record_preview_ctrl_off()
        {
            movie_mode_ctrl_off();
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
            slowPlayBtn.Enabled = true;
            fastPlayBtn.Enabled = true;
            speedLabel.Enabled = true;

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

                current_frame_index = 0;
                // reset_members();
                _movie_state = MovieState.STOP;
                setCtrlsState(_movie_loaded);
                disconnect_playerCtrl_Canvas_MouseEvents();
            }
        }

        private void ApplyFilterToMovie()
        {
            m_indexToGo = -1;
            m_curFrame = 0;
            m_filteredFramesNumber = m_framesNumber;
            m_framesToDisplay = m_framesNumber;

            if (!m_filterMask.all_enabled)
            {
                m_filteredFramesNumber = m_filterMask.Size;
                m_framesToDisplay = m_filteredFramesNumber;
            }

            ResetIndicator();

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

                    disconnect_playerCtrl_Canvas_MouseEvents();

                }

                UpdateSpeedLabel();

                {
                    if (is_pausing)
                    {
                        stopPlayingLoop();
                       // ShowFrame(m_indexToGo);
                    }
                    else
                    {
                        _movie_state = new_state;
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
                
                connect_playerCtrl_Canvas_MouseEvents();

                _movie_state = MovieState.PAUSE;
                ShowFrame(m_indexToGo);

                pauseButton.Checked = true;
                stopButton.Enabled = false;


                playPauseButton.Checked = false;
                stopButton.Checked = false;

            } 
        }

        int m_speedFactor = 0;

        protected int m_filesNumber = 0;
        protected int m_framesNumber = 0;
        protected int m_filteredFramesNumber = 0;


        protected int m_curFile = 0;
        protected int m_curFrame = -1;
        protected double _first_frame_time = 0;

        int m_framesToDisplay = 0;

        protected int m_indexToGo = -1;
        protected MovieTransit _movie_transit;

        areas_dispatcher _movie_transit_areas_dispatcher;
        protected irb_frame_helper _movie_frame;
        private bool _is_cursor_position_valid = false;
        void connect_playerCtrl_Canvas_MouseEvents()
        {
            m_playerControl.drawingCanvas.MouseMove += new System.Windows.Input.MouseEventHandler(playerCtrl_Canvas_MouseMove);
            m_playerControl.drawingCanvas.MouseEnter += new System.Windows.Input.MouseEventHandler(playerCtrl_Canvas_MouseEnter);
            m_playerControl.drawingCanvas.MouseLeave += new System.Windows.Input.MouseEventHandler(playerCtrl_Canvas_MouseLeave);

            if (InvokeRequired)
                Invoke(new EventHandler(delegate
                {
                    DisplayMinMaxTemperaturePointCtrls(true);
                }
                ));
            else
                DisplayMinMaxTemperaturePointCtrls(true);

        }
        void disconnect_playerCtrl_Canvas_MouseEvents()
        {
            m_playerControl.drawingCanvas.MouseMove -= new System.Windows.Input.MouseEventHandler(playerCtrl_Canvas_MouseMove);
            m_playerControl.drawingCanvas.MouseEnter -= new System.Windows.Input.MouseEventHandler(playerCtrl_Canvas_MouseEnter);
            m_playerControl.drawingCanvas.MouseLeave -= new System.Windows.Input.MouseEventHandler(playerCtrl_Canvas_MouseLeave);

            _is_cursor_position_valid = false;
            if (InvokeRequired)
                Invoke(new EventHandler(delegate
                {
                    m_playerControl.Temperature_label.Visibility = System.Windows.Visibility.Hidden;
                    DisplayMinMaxTemperaturePointCtrls(false);
                }
                ));
            else
            {
                m_playerControl.Temperature_label.Visibility = System.Windows.Visibility.Hidden;
                DisplayMinMaxTemperaturePointCtrls(false);
            }
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
            if (is_movie_playing())
                return;
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

            real_frame_index = frameNum;
            if (!m_filterMask.is_filtered(ref real_frame_index))
                return;

            bool res = false;
            object raster = new byte[1024 * 770 * 4];
            _irb_frame_info frame_info = new _irb_frame_info();
            
            try
            {
                res = _movie_transit.GetFrameRaster((uint)real_frame_index,
                                                    out frame_info,
                                                    ref raster);

            if (res)
            {
                    //-------------------------------------------------------  PROCESS EQUIPMENT ----------------------------------------------------------
                    if (!apply_camera_offset && camera_offset_valid)
                    {
                        current_camera_offset = frame_info.coordinate.camera_offset;
                    }

                    frame_info.coordinate.coordinate += current_camera_offset;

                    if (frame_info.coordinate.line != "" && frame_info.coordinate.path != "") {
                        _processFrameFunc(frame_info);
                    }
                    //--------------------------------------------------------------------------------------------------------------------------------------

                    if (frame_info.image_info.width == 1024) SetPlayerControlImage((byte[])raster, 1024, 768, false);
                else SetPlayerControlImage((byte[])raster, 640, 480, false);

                _measure = new CTemperatureMeasure(frame_info.measure.tmin, frame_info.measure.tmax, frame_info.measure.tavr,
                    frame_info.measure.object_tmin, frame_info.measure.object_tmax, 0,                            
                    frame_info.measure.calibration_min, frame_info.measure.calibration_max);

                var args = new object[] { _measure };

                SetThermoScaleLimits(_measure);

                Invoke(new SetTemperatureMeasureDelegate(SetTemperatureMeasure), args);
                //Invoke(new SetTemperatureCalibrationLimitsDelegate(SetTemperatureCalibrationLimits), args);

                Invoke(new SetCurFrameNumDelegate(SetCurFrameNum), new object[] { (frameNum == 0) ? 0 : m_curFrame + 1 });
                Invoke(new SetTimeDelegate(SetTime), new object[] { frame_info.timestamp });
                Invoke(new SetIRBFramePositionDelegate(SetIRBFramePosition), new object[] { frame_info.coordinate.line, frame_info.coordinate.path, frame_info.coordinate.coordinate, frame_info.coordinate.picket, frame_info.coordinate.offset, frame_info.coordinate.counter });

                if (_is_cursor_position_valid)
                    get_cursor_point_temperature();

                _max_T_point = frame_info.max_T_point;
                _min_T_point = frame_info.min_T_point;

                display_min_max_point_temperature(frame_info.max_T_point, frame_info.min_T_point, _measure);

                if (m_areasPanel != null && m_areasPanel.Template != null && m_areasPanel.Template.Areas != null)
                {
                    get_areas_temperature_measure();
                }

            }
            }
            catch (OutOfMemoryException)
            {
                _movie_transit.ClearMovieTransitCache();
            }

            catch (COMException)
            {
                return;
            }



        }

        private int current_frame_index = 0;
        private int real_frame_index = 0;

        private void PlayMovie(stopRequestedPredicate stopRequestedFunc)
        {

            if (_movie_state != MovieState.PLAY || stopRequestedFunc())
            {
                return;
            }

            // disconnect_playerCtrl_Canvas_MouseMove();
            if (m_playerControl != null)
                m_playerControl.BlockImgUpdate = false;
            current_frame_index = 0;
            real_frame_index = m_indexToGo;
            object raster = new byte[1024 * 770 * 4];
            //object temp_values = new float[300];

            connect_playerCtrl_Canvas_MouseEvents();

            for (; current_frame_index < m_filteredFramesNumber; ++current_frame_index)
            {
                if (stopRequestedFunc())
                {
                    if (--current_frame_index < 0)
                        current_frame_index = 0;
                    break;
                }

                if (m_indexToGo > -1)
                {
                    current_frame_index = m_indexToGo;
                    m_indexToGo = -1;
                }


                real_frame_index = current_frame_index;
                if (!m_filterMask.is_filtered(ref real_frame_index))
                    continue;

                if (current_frame_index >= m_framesNumber)
                    current_frame_index = m_framesNumber - 1;

                bool res = false;
                _irb_frame_info frame_info = new _irb_frame_info();
                try
                {
                    res = _movie_transit.GetFrameRaster((uint)real_frame_index,
                                                out frame_info,
                                                ref raster);


                    if (stopRequestedFunc())
                    {
                        if (--current_frame_index < 0)
                            current_frame_index = 0;
                        break;
                    }

                    if (res)
                    {
                        //------------------------------------------------------- PROCESS EQUIPMENT ------------------------------------------------------------
                        if (!apply_camera_offset && camera_offset_valid)
                        {
                            current_camera_offset = frame_info.coordinate.camera_offset;
                        }

                        frame_info.coordinate.coordinate += current_camera_offset;

                        if (frame_info.coordinate.line != "" && frame_info.coordinate.path != "")
                        {
                            _processFrameFunc(frame_info);
                        }

                        //--------------------------------------------------------------------------------------------------------------------------------------

                        if (frame_info.image_info.width == 1024) SetPlayerControlImage((byte[])raster, 1024, 768, true);
                        else SetPlayerControlImage((byte[])raster, 640, 480, true);

                        _measure = new CTemperatureMeasure(frame_info.measure.tmin, frame_info.measure.tmax, frame_info.measure.tavr,
                            frame_info.measure.object_tmin, frame_info.measure.object_tmax, 0,
                            frame_info.measure.calibration_min, frame_info.measure.calibration_max);

                        var args = new object[] { _measure };


                        SetThermoScaleLimits(_measure);

                        Invoke(new SetTemperatureMeasureDelegate(SetTemperatureMeasure), args);
                        //Invoke(new SetTemperatureCalibrationLimitsDelegate(SetTemperatureCalibrationLimits), args);

                        Invoke(new SetCurFrameNumDelegate(SetCurFrameNum), new object[] { (current_frame_index == 0) ? 0 : current_frame_index + 1 });
                        Invoke(new SetTimeDelegate(SetTime), new object[] { frame_info.timestamp });
                        Invoke(new SetIRBFramePositionDelegate(SetIRBFramePosition), new object[] { frame_info.coordinate.line, frame_info.coordinate.path, frame_info.coordinate.coordinate, frame_info.coordinate.picket, frame_info.coordinate.offset, frame_info.coordinate.counter });
                        if (_is_cursor_position_valid)
                            get_cursor_point_temperature();

                        _max_T_point = frame_info.max_T_point;
                        _min_T_point = frame_info.min_T_point;

                        display_min_max_point_temperature(frame_info.max_T_point, frame_info.min_T_point, _measure);

                    }

                    if (stopRequestedFunc())
                    break;

                if (m_areasPanel != null && m_areasPanel.Template != null && m_areasPanel.Template.Areas != null)
                {
                    get_areas_temperature_measure();
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
                catch (OutOfMemoryException)
                {
                    _movie_transit.ClearMovieTransitCache();
                }
                catch (COMException exc)
                {
                    NLog.LogManager.GetCurrentClassLogger().Warn(exc.Message);
                    break;
                }

            }

            disconnect_playerCtrl_Canvas_MouseEvents();
            m_indexToGo = current_frame_index;

            BeginInvoke(new EventHandler(delegate { PauseMovie(); }));
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
