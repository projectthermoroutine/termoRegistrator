using System;
using System.IO;
using System.Windows.Forms;
using ThermoRoutineLib;

namespace Registrator
{
    public partial class PlayerPanel
    {

        private async void cameraOnOff_Btn_Click(object sender, EventArgs e)
        {
            var mode = _mode;
            if (mode == PlayerMode.MOVIE)
            {
                await setMode(PlayerMode.CAMERA);
                SetPlayerMode(_mode);
            }
            else
            {
                await setMode(PlayerMode.MOVIE);
                SetPlayerMode(_mode);
            }

            m_playerControl.ResetImage(true);
            ResetIndicator();
            FireRecModeChangeEvent(new RecModeChangeEvent(previewModeButton.Checked));

        }


        private void connectCameraBtn_Click(object sender, EventArgs e)
        {
            connectCamera(connectCameraBtn.Checked);
        }

        private void playBtn_Click(object sender, EventArgs e)
        {
            startPlayPauseMovie();
        }
        private void previewCameraBtn_Click(object sender, EventArgs e)
        {
            startGrabbing();
        }

        private void pauseBtn_Click(object sender, EventArgs e)
        {
            startPlayPauseMovie();
        }

        private void stopBtn_Click(object sender, EventArgs e)
        {
            StopMoviePlaying();

            m_playerControl.ResetImage();
            ResetIndicator();
            SetPlayerMode(_mode);

        }
        private void stopCameraGrabBtn_Click(object sender, EventArgs e)
        {
            stopGrabbing();
            m_playerControl.ResetImage();
            ResetIndicator();
            SetPlayerMode(_mode);

        }

        private void recModeButton_CheckStateChanged(object sender, EventArgs e)
        {
        }

        private void recModeButton_CheckedChanged(object sender, EventArgs e)
        {
            cameraSettingsButton.Checked = false;
            cameraSettingsButton.Enabled = previewModeButton.Checked;
        }

        private void TermoScaleBtn_Click(object sender, EventArgs e)
        {
            if (m_playerControl != null)
                m_playerControl.TermoScaleVisible = termoScaleBtn.Checked || cameraTermoScaleBtn.Checked;
        }

        private void zoomInBtn_Click(object sender, EventArgs e)
        {
            fitSize.Checked = false;
            fitDisplay.Checked = false;
            m_actualScale += 0.1;
            if (m_actualScale > 3)
                m_actualScale = 3;
            Scale(m_actualScale);
        }

        private void zoomOutBtn_Click(object sender, EventArgs e)
        {
            fitSize.Checked = false;
            fitDisplay.Checked = false;
            m_actualScale -= 0.1;
            if (m_actualScale < 0.1)
                m_actualScale = 0.1;
            Scale(m_actualScale);
        }

        private void fitToFrameSizeBtn_Click(object sender, EventArgs e)
        {
            fitDisplay.Checked = false;
            m_actualScale = 1;
            Scale(m_actualScale);
        }

        private void fitToDisplayBtn_Click(object sender, EventArgs e)
        {
            fitSize.Checked = false;
            Strech();
        }

        private void fastPlayBtn_Click(object sender, EventArgs e)
        {
            SpeedFactorIncrease();
        }

        private void slowPlayBtn_Click(object sender, EventArgs e)
        {
            SpeedFactorDecrease();
        }

        private void framesFilterBtn_Click(object sender, EventArgs e)
        {
            PauseMovie();

            bool areas_exist = false;
            if (m_areasPanel != null && 
                m_areasPanel.Template != null && 
                m_areasPanel.Template.Areas != null &&
                m_areasPanel.Template.Areas.Count > 0
              )
            {
                areas_exist = true;
            }

            FrameFilterForm fff = new FrameFilterForm(_movie_transit, areas_exist);
            fff.FilteredEventHandler += FilteredEventFired;
            fff.ShowDialog();



        }
        private void cameraSettingsBtn_Click(object sender, EventArgs e)
        {
            if (cameraSettingsButton.Tag.ToString() == "1")
            {
                m_tvHandler.ShowCamSettings();
                cameraSettingsButton.Checked = false;
                cameraSettingsButton.Tag = 0;
            }
            else
            {
                cameraSettingsButton.Tag = 1;
                m_tvHandler.HideCamSettings();
            }

        }

        private void palleteSelectionCtrl_SelectedIndexChanged(object sender, EventArgs e)
        {
            string[] pallete_file_names = { "", "\\PAL\\RAIN.PAL", "\\PAL\\IRON.PAL" };
            string pallete_filename = "";
            ToolStripComboBox palleteCtrl = (ToolStripComboBox)sender;

            if ((uint)palleteCtrl.SelectedIndex < pallete_file_names.Length && palleteCtrl.SelectedIndex != 0)
            {
                string current_directory = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                pallete_filename = current_directory + pallete_file_names[palleteCtrl.SelectedIndex];
            }

            bool is_movie_transit = (string)palleteCtrl.Tag == "0" ? true : false;
            if (pallete_filename.Length > 0)
            {
                if (is_movie_transit)
                    _movie_transit.SetPallete(pallete_filename);
                else
                    m_tvHandler.SetPallete(pallete_filename);
            }
            else
            {
                if (is_movie_transit)
                    _movie_transit.SetDefaultPallete();
                else
                    m_tvHandler.SetDefaultPallete();
            }

            setPallete(is_movie_transit);

            show_current_frame();

        }

        private void recordBtn_Click(object sender, EventArgs e)
        {
            PlayerMode mode = _mode;
            if (!is_recording())
            {
                startRecord();
                mode = PlayerMode.RECORD;
            }
            else
            {
                stopRecord();
            }
//            FireFrameShotListenerStateChangeEvent(new FrameShotListenerStateChangeEvent(FrameShotListenerStateChangeEvent.StateChangeType.STATE_CHANGE_TYPE_ADD));

            SetPlayerMode(mode);

        }

        public UInt32 get_current_frame_index_movie()
        {
            return (UInt32)real_frame_index;
        }
        public UInt32 get_current_frame_id_camera()
        {
            return _current_camera_frame_id;
        }

        private void shotButton_Click(object sender, EventArgs e)
        {
            ExtractFrameInfo((int)_frame_data_helper.get_current_frame_id());
        }

        //private void shotButton_Click_old(object sender, EventArgs e)
        //{

        //    if (is_movie_playing())
        //    {
        //        PauseMovie();
        //    }
        //    if (m_indexToGo == -1)
        //        return;

        //    var frame_index = m_indexToGo;

        //    ShotDesc.ShotType shotType = ShotDesc.ShotType.SHOT_TYPE_USER;
        //    ShotDesc desc = ExtractFrameInfo(frame_index);
        //    desc.TypeOfShot = shotType;

        //    SaveFileDialog ofd = new SaveFileDialog();

        //    ofd.InitialDirectory = TripProject.IRBFilesPath;
        //    ofd.Filter = "IRB-файлы (*.irb)|*.irb"; //|All files (*.*)|*.*";
        //    ofd.FilterIndex = 1;
        //    //ofd.Multiselect = true;
        //    ofd.RestoreDirectory = true;

        //    var objects = _db_controller.get_objects_by_coordinate(desc.Distance, 500);
        //    string device_name = "";
        //    if (objects.Count() > 0)
        //    {
        //        device_name = objects.ElementAt(0).name;
        //    }

        //    int picket = 0;
        //    uint picketOffset = 0;

        //    _db_controller.GetPicketAndPicketOffset(desc, ref picket, ref picketOffset);

        //    if (ofd.ShowDialog() == DialogResult.OK)
        //    {
        //        if (!_movie_transit.SaveFrame((uint)frame_index, device_name, picket, (int)picketOffset, ofd.FileName))
        //            MessageBox.Show("shot hasn't been saved !!!");
        //    }
        //}

        private void previewRecordBtn_Click(object sender, EventArgs e)
        {
            if (!is_recording())
                return;

            startStopPreviewRecord();

        }

        private void autoFocusBtn_Click(object sender, EventArgs e)
        {
            SendAutoFocus();
        }
        private void NUC_Btn_Click(object sender, EventArgs e)
        {
            SendAutoNUC();
        }

        private void runtimeAlarmSettingsBtn_Click(object sender, EventArgs e)
        {
            if(_RuntimeAlarmSettingsForm == null)
            {
                IRB_Frame.RunTimeAlarmController.settings AlarmSettings = _RuntimeAlarmCtrl?.Settings ?? IRB_Frame.RunTimeAlarmController.default_settings();

                _RuntimeAlarmSettingsForm = new Views.RuntimeAlarmSettingsForm(AlarmSettings);
            }

            if(_RuntimeAlarmSettingsForm.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                if(_RuntimeAlarmCtrl == null)
                {
                    var ctrl = new IRB_Frame.RunTimeAlarmController(_db_controller)
                    {
                        Settings = _RuntimeAlarmSettingsForm.Settings
                    };

                    var DirPathForAlarmFrames = Properties.Settings.Default.RuntimeAlarmFramesPath;
                    if (!Path.IsPathRooted(DirPathForAlarmFrames))
                    {
                        DirPathForAlarmFrames = Path.Combine(this.TripProject.IRBFilesPath, DirPathForAlarmFrames);
                        if (!Path.IsPathRooted(DirPathForAlarmFrames))
                        {
                            DirPathForAlarmFrames = Path.Combine(Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location), DirPathForAlarmFrames);
                        }
                    }

                    _AlarmFrameWriter = new IRB_Frame.AlarmFrameWriter(ctrl/*, _db_controller*/, DirPathForAlarmFrames);

                    ctrl.SetFrameRawDataDelegate(_frame_data_helper.camera_get_frame_raw_data);

                    _RuntimeAlarmCtrl = ctrl;
                }
                else
                    _RuntimeAlarmCtrl.Settings = _RuntimeAlarmSettingsForm.Settings;


                if(_active_predefined_area_index != -1)
                {
                    if (!_RuntimeAlarmCtrl.Settings.filter_objects)
                    {
                        _active_predefined_area_index = -1;
                        _predefined_area_measure = new _area_temperature_measure { min = float.NaN, max = float.NaN, avr = float.NaN };
                        m_tvHandler.RemoveArea(_predefined_area_id, out _area_type type);
                    }
                }
                else if(_RuntimeAlarmCtrl.Settings.filter_objects)
                {
                    _active_predefined_area_index = 0;
                    _area_info areaInfo = _PredefinedAreas[_active_predefined_area_index];
                    m_tvHandler.AddArea(_predefined_area_id, ref areaInfo);
                }
            }
        }

    }
}
