using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.IO;
using System.Windows.Forms;


namespace Registrator
{
    public partial class PlayerPanel
    {

        private void cameraOnOff_Btn_Click(object sender, EventArgs e)
        {
            if (previewModeButton.Checked) //Если режим камеры
            {
                setMode(PlayerMode.CAMERA);

                FireFrameShotListenerStateChangeEvent(new FrameShotListenerStateChangeEvent(FrameShotListenerStateChangeEvent.StateChangeType.STATE_CHANGE_TYPE_DEL));

                SetPlayerMode(1);

            }
            else
            {
                setMode(PlayerMode.MOVIE);

                FireFrameShotListenerStateChangeEvent(new FrameShotListenerStateChangeEvent(FrameShotListenerStateChangeEvent.StateChangeType.STATE_CHANGE_TYPE_ADD));

                FireNeedToEraseTrackEvent(new NeedToEraseTrackEvent());
                SetPlayerMode(0);
                
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
            if (_mode == PlayerMode.CAMERA)
            {
                startGrabbing();
            }
            else
            {
                startPlayPauseMovie();
            }

        }
        private void pauseBtn_Click(object sender, EventArgs e)
        {
            if (_mode == PlayerMode.MOVIE)
                startPlayPauseMovie();
        }

        private void stopBtn_Click(object sender, EventArgs e)
        {
            if (_mode == PlayerMode.MOVIE)
                StopMoviePlaying();
            else
                stopGrabbing();

            m_playerControl.ResetImage();
            ResetIndicator();
            SetPlayerMode((byte)_mode);

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
                m_playerControl.TermoScaleVisible = termoScaleBtn.Checked;
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

            FrameFilterForm fff = new FrameFilterForm(_movie_transit);
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
            if ((uint)palleteSelectionCtrl.SelectedIndex < pallete_file_names.Length && palleteSelectionCtrl.SelectedIndex != 0)
            {
                string current_directory = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                pallete_filename = current_directory + pallete_file_names[palleteSelectionCtrl.SelectedIndex];
            }
            if (pallete_filename.Length > 0)
            {
                    _movie_transit.SetPallete(pallete_filename);
                    m_tvHandler.SetPallete(pallete_filename);
            }
            else
            {
                    _movie_transit.SetDefaultPallete();
                    m_tvHandler.SetDefaultPallete();
            }

            setPallete();

            if (_mode == PlayerMode.MOVIE)
                show_current_frame();

        }

        private void recordBtn_Click(object sender, EventArgs e)
        {
            byte player_mode_text_code = 1;
            if (!recordButton.Checked)
            {
                startRecord();
                player_mode_text_code = 2;
            }
            else
            {
                stopRecord();
            }
//            FireFrameShotListenerStateChangeEvent(new FrameShotListenerStateChangeEvent(FrameShotListenerStateChangeEvent.StateChangeType.STATE_CHANGE_TYPE_ADD));

            SetPlayerMode(player_mode_text_code);

        }

        private void shotButton_Click(object sender, EventArgs e)
        {
            var frame_index = current_frame_index;
            
            ShotDesc.ShotType shotType = ShotDesc.ShotType.SHOT_TYPE_USER;
            ShotDesc desc = ExtractFrameInfo(frame_index, previewModeButton.Checked);
            desc.TypeOfShot = shotType;
            FireFrameShotedEvent(new FrameShotedEvent(desc));

            SaveFileDialog ofd = new SaveFileDialog();

            ofd.InitialDirectory = TripProject.IRBFilesPath;
            ofd.Filter = "IRB-файлы (*.irb)|*.irb"; //|All files (*.*)|*.*";
            ofd.FilterIndex = 1;
            //ofd.Multiselect = true;
            ofd.RestoreDirectory = true;

            int picket=0;
            uint picketOffset=0;

            DBHelper.getPicketAndPicketOffset(desc,ref picket,ref picketOffset);
            
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                if (!_movie_transit.SaveFrame((uint)frame_index, (uint)picket, picketOffset, ofd.FileName))
                    MessageBox.Show("shot hasn't been saved !!!");
            }
        }
    }
}
