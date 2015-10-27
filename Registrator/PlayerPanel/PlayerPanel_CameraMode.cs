using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ThermoRoutineLib;
using System.Threading;
using System.IO;
using System.Windows.Forms;

namespace Registrator
{
    public partial class PlayerPanel
    {
        void initialize_camera_interface()
        {
            _camera_state_lock = new object();
            _camera_state = CameraState.NONE;
        }

        void release_camera_interface()
        {
            if (_camera_state != CameraState.NONE)
            {
                stopGrabbingLoop();
            }
        }

        void stopGrabbingLoop()
        {
            stop_routine_in_com_apartment();
        }


        void create_camera(int cameraOffset)
        {
            pdDispatcher = null;
            try
            {
                _camera_frame = null;
                m_tvHandler = new TRWrapper();
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message + " TRWrapper create error", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            grabberDispatcher = new GrabberDispatcher(m_tvHandler, cameraOffset);
            try
            {
                pdDispatcher = new PD_dispatcher(m_tvHandler);
                pdDispatcher.start();
                m_tvHandler.FileFromGrabber += new _ITRWrapperEvents_FileFromGrabberEventHandler(FileFromGrabberFired);
            
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message + " PD_dispatcher create error", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
                
           

        }
        void close_camera()
        {
            if (m_tvHandler != null)
            {
                m_tvHandler.FinishAll();
                m_tvHandler.FileFromGrabber -= new _ITRWrapperEvents_FileFromGrabberEventHandler(FileFromGrabberFired);

                grabberDispatcher.Dispose();
                pdDispatcher.Dispose();
                m_tvHandler = null;
            }
        }


        enum CameraState
        {
            NONE,
            SOURCES,
            CONNECT,
            GRAB,
            RECORD,
            PREVIEW_RECORD
        }

        private bool is_camera_grabbing() { return _camera_state == CameraState.GRAB || _camera_state == CameraState.RECORD || _camera_state == CameraState.PREVIEW_RECORD; }


        CameraState _camera_state;
        object _camera_state_lock;

        void camera_mode_ctrl_on()
        {
            set_camera_mode_ctrls_visibility(true);
            cameraToolbarCtrl.Visible = true;
        }

        void camera_mode_ctrl_off()
        {
            cameraToolbarCtrl.Visible = false;
            set_camera_mode_ctrls_visibility(false);
        }

        void set_camera_mode_ctrls_visibility(bool visibility)
        {
            cameraSettingsButton.Checked = visibility;
            cameraSettingsButton.Enabled = visibility;
            cameraSrcComboBox.Visible = visibility;
            cameraSrcComboBox.Enabled = visibility;
            recordButton.Visible = visibility;
            recordButton.Enabled = visibility;

            zoomInBtn.Visible = visibility;
            zoomInBtn.Enabled = visibility;
            zoomOutBtn.Visible = visibility;
            zoomOutBtn.Enabled = visibility;


            recordPreviewBtn.Visible = visibility;
            connectCameraBtn.Visible = visibility;


            if (!visibility)
                cameraSrcComboBox.Items.Clear();

        }

        void stopCameraMode()
        {
            if (grabberDispatcher == null)
                return;
            stopPreviewRecord();

            lock (_camera_state_lock)
            {
                if (_camera_state != CameraState.NONE)
                {
                    grabberDispatcher.disconnectFromNewFrameEvent(FrameFired);
                    stopShowGrabbingFrames();

                    if (_camera_state == CameraState.GRAB || _camera_state == CameraState.RECORD)
                    {
                        grabberDispatcher.stopRecording();
                        grabberDispatcher.stopGrabbing();
                        grabberDispatcher.disconnectFromSourceById(0);
                        m_recStarted = false;
                    }
                    if (_camera_state == CameraState.CONNECT)
                    {
                        grabberDispatcher.disconnectFromSourceById(0);
                    }

                    _camera_state = CameraState.NONE;
                    setCameraModeCtrlsState(_camera_state);

                    FireFrameShotListenerStateChangeEvent(new FrameShotListenerStateChangeEvent(FrameShotListenerStateChangeEvent.StateChangeType.STATE_CHANGE_TYPE_DEL));

                }
            }
        }

        void stopGrabbing()
        {
            if (grabberDispatcher == null)
                return;
            stopPreviewRecord();

            lock (_camera_state_lock)
            {
                grabberDispatcher.disconnectFromNewFrameEvent(FrameFired);
                stopShowGrabbingFrames();

                if (_camera_state == CameraState.GRAB || _camera_state == CameraState.RECORD)
                {
                    grabberDispatcher.stopRecording();
                    grabberDispatcher.stopGrabbing();
                    m_recStarted = false;
                }
                _camera_state = CameraState.CONNECT;
                setCameraModeCtrlsState(_camera_state);

                FireFrameShotListenerStateChangeEvent(new FrameShotListenerStateChangeEvent(FrameShotListenerStateChangeEvent.StateChangeType.STATE_CHANGE_TYPE_DEL));
            }
        }

        void stopRecord()
        {
            if (grabberDispatcher == null)
                return;

            stopPreviewRecord();
            lock (_camera_state_lock)
            {
                if (_camera_state != CameraState.RECORD && _camera_state != CameraState.PREVIEW_RECORD)
                    return;

                grabberDispatcher.stopRecording();
                m_recStarted = false;
                _camera_state = CameraState.GRAB;
                setCameraModeCtrlsState(_camera_state);
            }
        }
        
        void startCameraMode()
        {
            lock (_camera_state_lock)
            {
                m_recStarted = false;
                _camera_state = CameraState.SOURCES;
                cameraSettingsButton.Tag = 1;

                Array sources_list;
                grabberDispatcher.GetGrabberSources(out sources_list);
                displayGrabberSourcesList(sources_list);

                if (m_playerControl != null)
                    m_playerControl.BlockImgUpdate = false;

                var full_path = TripProject.IRBFilesPath;
                if (!Path.IsPathRooted(full_path))
                {
                    string current_directory = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                    full_path = current_directory + "\\" + full_path;
                }

                if (full_path.Length > 0)
                {
                    if (full_path[full_path.Length - 1] != '\\')
                    {
                        full_path += '\\';
                    }
                }

                grabberDispatcher.setRecordedGrabbedFramesPath(full_path);

                setCameraModeCtrlsState(_camera_state);
                //grabberDispatcher.connectToNewFrameEvent(FrameFired);

            }

        }

        private bool is_recording() { return _camera_state == CameraState.RECORD || _camera_state == CameraState.PREVIEW_RECORD; }


        void startRecord()
        {
            lock (_camera_state_lock)
            {
                if (_camera_state == CameraState.CONNECT)
                {
                    startGrabbing();
                }
                if (_camera_state != CameraState.GRAB)
                    return;
                _camera_state = CameraState.RECORD;
                setCameraModeCtrlsState(_camera_state);
                m_recStarted = true;
                grabberDispatcher.startRecording();
            }

        }

        void startStopPreviewRecord()
        {
            var state = _camera_state;
            if (state == CameraState.RECORD)
                startPreviewRecord();
            else
                stopPreviewRecord();
        }
        bool camera_file_name_predicate(string file_name)
        {
            if (file_name == "")
                return false;

            return true;
        }
 
        void startPreviewRecord()
        {
            lock (_camera_state_lock)
            {
                if (_camera_state != CameraState.RECORD)
                    return;

                grabberDispatcher.FlushLastFrames();
                stopShowGrabbingFrames();
                _camera_state = CameraState.PREVIEW_RECORD;
                setCameraModeCtrlsState(_camera_state);
                get_current_frame_point_temperature = get_current_frame_point_temperature_movie;
                get_area_info = get_area_info_movie;
                file_name_predicate = camera_file_name_predicate;
                startMovieMode();
                record_preview_mode_ctrl_on();

                reloadMovie();
                ShowFrame(m_framesNumber);
                m_playerControl.BottomPanelVisible = true;
            }
        }

        void stopPreviewRecord()
        {
            lock (_camera_state_lock)
            {
                if (_camera_state != CameraState.PREVIEW_RECORD)
                    return;

                m_playerControl.BottomPanelVisible = false;
                StopMoviePlaying();
                record_preview_ctrl_off();
                resetMovieTransit();
                m_tripProject.removeFiles(delegate(string file_name) { return !movie_file_name_predicate(file_name); });
                get_current_frame_point_temperature = get_current_frame_point_temperature_camera;
                get_area_info = get_area_info_camera;
                file_name_predicate = movie_file_name_predicate;
                startShowGrabbingFrames();
                _camera_state = CameraState.RECORD;
                setCameraModeCtrlsState(_camera_state);

            }
        }

        void startGrabbing()
        {
            lock (_camera_state_lock)
            {
                if (_camera_state != CameraState.CONNECT)
                    return;
                grabberDispatcher.connectToNewFrameEvent(FrameFired);
                _camera_state = CameraState.GRAB;
                setCameraModeCtrlsState(_camera_state);

                grabberDispatcher.startGrabbing();
                
                startShowGrabbingFrames();
            }

        }

        void connectCamera(bool is_disconnect)
        {
            lock (_camera_state_lock)
            {
                if (is_disconnect)
                {
                    if (_camera_state != CameraState.CONNECT)
                        return;
                    _camera_state = CameraState.SOURCES;
                    setCameraModeCtrlsState(_camera_state);
                    grabberDispatcher.disconnectFromSourceById((byte)cameraSrcComboBox.SelectedIndex);
                }
                else
                {
                    if (_camera_state != CameraState.SOURCES)
                        return;
                    var res = grabberDispatcher.connectToSourceById((byte)cameraSrcComboBox.SelectedIndex);
                    if(res)
                    {
                        _camera_state = CameraState.CONNECT;
                        setCameraModeCtrlsState(_camera_state);
                    }
                }
            }

        }
 
        void setCameraModeCtrlsState(CameraState state)
        {
            switch (state) {
                case CameraState.NONE:
                    set_camera_mode_ctrls_visibility(false);
                    break;
                case CameraState.SOURCES:
                    set_camera_mode_ctrls_visibility(true);

                    connectCameraBtn.Enabled = true;
                    connectCameraBtn.Checked = false;
                    previewCameraBtn.Enabled = false;
                    previewCameraBtn.Checked = false;

                    recordPreviewBtn.Enabled = false;
                    recordPreviewBtn.Checked = false;

                    cameraSettingsButton.Checked = false;
                    cameraSettingsButton.Enabled = false;
                    cameraSrcComboBox.Enabled = true;
                    recordButton.Enabled = false;
                    recordButton.Checked = false;
                    stopCameraPreviewBtn.Enabled = false;

                    zoomInBtn.Enabled = false;
                    zoomInBtn.Checked = false;
                    zoomOutBtn.Enabled = false;
                    zoomOutBtn.Checked = false;


                    break;
                case CameraState.CONNECT:
                    set_camera_mode_ctrls_visibility(true);

                    cameraSrcComboBox.Enabled = false;

                    connectCameraBtn.Checked = true;

                    previewCameraBtn.Enabled = true;
                    previewCameraBtn.Checked = false;

                    recordPreviewBtn.Enabled = false;
                    recordPreviewBtn.Checked = false;

                    cameraSettingsButton.Checked = false;
                    cameraSettingsButton.Enabled = true;
                    recordButton.Enabled = true;
                    recordButton.Checked = false;
                    stopCameraPreviewBtn.Enabled = false;
                    stopCameraPreviewBtn.Checked = false;

                    zoomInBtn.Enabled = false;
                    zoomInBtn.Checked = false;
                    zoomOutBtn.Enabled = false;
                    zoomOutBtn.Checked = false;
                    break;
                case CameraState.GRAB:
                    set_camera_mode_ctrls_visibility(true);

                    cameraSrcComboBox.Enabled = false;

                    connectCameraBtn.Checked = true;

                    previewCameraBtn.Enabled = false;
                    previewCameraBtn.Checked = false;

                    recordPreviewBtn.Enabled = false;
                    recordPreviewBtn.Checked = false;

                    cameraSettingsButton.Checked = false;
                    cameraSettingsButton.Enabled = false;
                    recordButton.Enabled = true;
                    recordButton.Checked = false;
                    stopCameraPreviewBtn.Enabled = true;
                    stopCameraPreviewBtn.Checked = false;

                    zoomInBtn.Enabled = true;
                    zoomInBtn.Checked = false;
                    zoomOutBtn.Enabled = true;
                    zoomOutBtn.Checked = false;

                    break;
                case CameraState.RECORD:
                    set_camera_mode_ctrls_visibility(true);

                    termoScaleBtn.Checked = false;
                    if (m_playerControl != null)
                        m_playerControl.TermoScaleVisible = false;

                    cameraTermoScaleBtn.Visible = true;
                    cameraShotButton.Visible = true;
                    cameraPalleteSelectionCtrl.Visible = true;
                    cameraOffsetBtn.Visible = true;


                    cameraSrcComboBox.Enabled = false;

                    connectCameraBtn.Checked = true;

                    previewCameraBtn.Enabled = false;
                    previewCameraBtn.Checked = false;

                    recordPreviewBtn.Enabled = true;
                    recordPreviewBtn.Checked = false;

                    cameraSettingsButton.Checked = false;
                    cameraSettingsButton.Enabled = false;
                    recordButton.Enabled = true;
                    recordButton.Checked = true;
                    stopCameraPreviewBtn.Enabled = true;
                    stopCameraPreviewBtn.Checked = false;

                    zoomInBtn.Enabled = true;
                    zoomInBtn.Checked = false;
                    zoomOutBtn.Enabled = true;
                    zoomOutBtn.Checked = false;

                    break;
                case CameraState.PREVIEW_RECORD:
                    
                    cameraSettingsButton.Visible = false;
                    zoomInBtn.Visible = false;
                    zoomOutBtn.Visible = false;

                    cameraTermoScaleBtn.Visible = false;

                    cameraTermoScaleBtn.Checked = false;
                    if (m_playerControl != null)
                        m_playerControl.TermoScaleVisible = false;

                    cameraShotButton.Visible = false;
                    cameraPalleteSelectionCtrl.Visible = false;
                    cameraOffsetBtn.Visible = false;

                    cameraSrcComboBox.Enabled = false;

                    connectCameraBtn.Checked = true;

                    previewCameraBtn.Enabled = false;
                    previewCameraBtn.Checked = true;

                    recordPreviewBtn.Enabled = true;
                    recordPreviewBtn.Checked = false;

                    cameraSettingsButton.Checked = false;
                    cameraSettingsButton.Enabled = false;
                    recordButton.Enabled = true;
                    recordButton.Checked = true;
                    stopCameraPreviewBtn.Enabled = true;
                    stopCameraPreviewBtn.Checked = false;

                    zoomInBtn.Enabled = false;
                    zoomInBtn.Checked = false;
                    zoomOutBtn.Enabled = false;
                    zoomOutBtn.Checked = false;

                    break;

            }
        }

        
        protected bool m_recStarted = false;

        protected TRWrapper m_tvHandler;

        areas_dispatcher _grabber_areas_dispatcher;

        public void 
            connect_change_setting_events(ProgramSettings settings_dlg)
        {
            if (pdDispatcher != null)
            {
                settings_dlg.PdSettingsChanged += PositionDetector.PD_SettingsChanged;
                settings_dlg.SyncSettingsChanged += PositionDetector.Sync_SettingsChanged;
                settings_dlg.CommonSettingsChanged += CommonSettingsChanged;
            }
        }

        private void CommonSettingsChanged(common_settings settings)
        {
            if (m_tvHandler != null)
            {
                m_tvHandler.SetMaxFramesInIRBFile(settings.max_frames_in_file);
                m_tvHandler.SetCameraOffset((int)settings.camera_offset);
            }
        }


        private PD_dispatcher pdDispatcher = null;
        public PD_dispatcher PositionDetector { get { return pdDispatcher; } }

        private GrabberDispatcher grabberDispatcher;
        public GrabberDispatcher Grabber { get { return grabberDispatcher; } }

        private void FrameFired(uint frame_id)
        {

            Object raster = new byte[1024 * 770 * 4];
            _irb_frame_info frame_info = new _irb_frame_info();

            try
            {
                var res = m_tvHandler.GetRealTimeFrameRaster(frame_id, out frame_info, ref raster);
            }
            catch (OutOfMemoryException)
            {

            }
            if (raster != null)
            {
                if (frame_info.image_info.width == 1024) SetPlayerControlImage((byte[])raster, 1024, 768);
                else SetPlayerControlImage((byte[])raster, 640, 480);

                var measure = new CTemperatureMeasure(frame_info.measure.tmin, frame_info.measure.tmax, frame_info.measure.tavr,
                                                frame_info.measure.calibration_min, frame_info.measure.calibration_max);

                var args = new object[] { measure };
                Invoke(new SetTemperatureMeasureDelegate(SetTemperatureMeasure), args);
                Invoke(new SetTemperatureCalibrationLimitsDelegate(SetTemperatureCalibrationLimits), args);


                if (m_areasPanel != null && m_areasPanel.Template != null && m_areasPanel.Template.Areas != null)
                {
                    get_areas_temperature_measure();
                }

            }

        }
        private void displayGrabberSourcesList(Array sourcesList)
        {
            cameraSrcComboBox.Items.Clear();
            cameraSrcComboBox.Visible = true;
            long i = 0;
            int cols = sourcesList.GetLength(sourcesList.Rank - 1);
            for (i = 0; i < cols; i++)
            {
                object source = sourcesList.GetValue(i);
                cameraSrcComboBox.Items.Add(source);
            }
            if (cols > 0)
                cameraSrcComboBox.Text = (string)cameraSrcComboBox.Items[0];
        }

        public void FireFileFromGrabberEvent(FileFromGrabberEvent e)
        {
            EventHandler<FileFromGrabberEvent> handler = FileFromGrabberEventHandler;
            if (handler != null)
                handler(this, e);

        }

    }
}
