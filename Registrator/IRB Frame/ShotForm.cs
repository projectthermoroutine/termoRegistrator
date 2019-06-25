using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ThermoRoutineLib;
using System.IO;
using DrawToolsLib;
using System.Collections;
using IRControls;
using System.Runtime.InteropServices;

namespace Registrator
{
    public partial class ShotForm : Form
    {
        MovieTransit _irb_frame_helper;
        point_info m_element = null;
        PlayerControl m_playerControl;
        PointsInfoView PointsInfoViewCtrl;

        string _temp_movie_file_name;

        ShotForm()
        {
            InitializeComponent();

            _irb_frame_helper = new MovieTransit();
            palleteSelectionCtrl.SelectedIndexChanged -= palleteSelectionCtrl_SelectedIndexChanged;
            palleteSelectionCtrl.SelectedIndex = 0;
            palleteSelectionCtrl.SelectedIndexChanged += palleteSelectionCtrl_SelectedIndexChanged;

            m_playerControl = new PlayerControl(false);
            m_playerControl.TermoScaleVisible = false;

            m_playerControl.LimitsChangedEventHandler += LimitsChangedEventFired;
            m_playerControl.LimitsModeChangedEventHandler += LimitsModeChangedEventFired;
            
            elementHost1.Child = m_playerControl;
        }

        public ShotForm(point_info point_info) 
            : this()
        {
            PointsInfoViewCtrl = new PointsInfoView();
            PointsInfoViewCtrl.AddPointInfo(point_info);
            this.tableLayoutPanel1.Controls.Add(this.PointsInfoViewCtrl, 0, 0);
            this.PointsInfoViewCtrl.Dock = System.Windows.Forms.DockStyle.Fill;
            m_element = point_info;

            try
            {
                _temp_movie_file_name = Path.GetTempFileName();
            }
            catch(IOException)
            {

                _temp_movie_file_name = Path.GetTempPath() + "_Registrator_Frame_Shot.tmp";
            }

            load_frame_to_temp_file(_temp_movie_file_name);

        }

        void load_frame_to_temp_file(string temp_file_name)
        {
           bool res = _irb_frame_helper.SaveFrameFromRawData(m_element.frame_info.raw_data, temp_file_name);
           if (res)
           {
               try
               {
                   Array errors;
                   res = _irb_frame_helper.SetIRBFiles(new string[] { temp_file_name }, out errors);
               }
               catch (COMException)
               {

               }
           }
        }

        private delegate void SetPlayerControlImageDelegate(byte[] raster, int width, int height);

        public void SetPlayerControlImage(byte[] raster, int width, int height)
        {
            if (InvokeRequired)
            {
                Invoke(new SetPlayerControlImageDelegate(SetPlayerControlImage), new object[] { raster, width, height });
            }
            else
            {
                m_playerControl.SetImage(raster, width, height);
                m_playerControl.ActualScale = 0.75;
            }
        }

        string pallete_filename = "";
        public void ShowFrame()
        {
            bool res = false;
            object raster = new byte[1024 * 770 * 4];
            _irb_frame_info frame_info = new _irb_frame_info();

            try
            {
                res = _irb_frame_helper.GetFrameRaster(0,
                                                    out frame_info,
                                                    ref raster);
            }
            catch (COMException)
            {
                return;
            }

            if (res)
            {
                if (frame_info.image_info.width == 1024) SetPlayerControlImage((byte[])raster, 1024, 768);
                else SetPlayerControlImage((byte[])raster, 640, 480);

                var measure = new CTemperatureMeasure(frame_info.measure.tmin, frame_info.measure.tmax, frame_info.measure.tavr,
                    frame_info.measure.object_tmin, frame_info.measure.object_tmax, 0,
                    frame_info.measure.calibration_min, frame_info.measure.calibration_max);

                var args = new object[] { measure };

                SetThermoScaleLimits(measure);

                Invoke(new SetTemperatureMeasureDelegate(SetTemperatureMeasure), args);

            }

        }

        public delegate void SetTemperatureMeasureDelegate(CTemperatureMeasure measure);
        public void SetTemperatureMeasure(CTemperatureMeasure measure)
        {
            m_playerControl.MinT = measure.min;
            m_playerControl.MaxT = measure.max;
            m_playerControl.Max_MinT = measure.max - measure.min;
            m_playerControl.AvrT = measure.avg;
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

        float minT_limit = 0;
        float maxT_limit = 0;
        public void LimitsChangedEventFired(object sender, LimitsChangedEvent e)
        {
            _disable_thermoscale_limits_change = true;
            _calibration_mode = _calibration_mode.MANUAL;

            _irb_frame_helper.SetPaletteCalibration((float)e.Minimum, (float)e.Maximum);
            _irb_frame_helper.SetPaletteCalibrationMode(_calibration_mode.MANUAL);

            minT_limit = (float)e.Minimum;
            maxT_limit = (float)e.Maximum;
            ShowFrame();
        }
        _calibration_mode _calibration_mode = _calibration_mode.MIN_MAX;
        public void LimitsModeChangedEventFired(object sender, LimitsModeChangedEvent e)
        {
            _calibration_mode = _calibration_mode.MIN_MAX;

            if (e.Mode == LimitsModeChangedEvent.LimitsMode.CALIBRATION_MODE)
            {
                _calibration_mode = _calibration_mode.NONE;
            }
            else if (e.Mode == LimitsModeChangedEvent.LimitsMode.FRAME_MODE)
            {
                _calibration_mode = _calibration_mode.MIN_MAX;
            }
            else if (e.Mode == LimitsModeChangedEvent.LimitsMode.OBJECT_MODE)
            {
                _calibration_mode = _calibration_mode.AVERAGE;
            }

            _irb_frame_helper.SetPaletteCalibrationMode(_calibration_mode);

            ShowFrame();
        }


        private void zoomInButton_Click(object sender, EventArgs e)
        {
            m_playerControl.ResetImageStrech();
            if (m_playerControl.ActualScale < 3)
                m_playerControl.ActualScale += 0.1;
        }

        private void zoomOutButton_Click(object sender, EventArgs e)
        {
            m_playerControl.ResetImageStrech();
            if (m_playerControl.ActualScale >= 0.2) 
                m_playerControl.ActualScale -= 0.1;
        }

        private void TermoScaleBtn_Click(object sender, EventArgs e)
        {
            if (m_playerControl != null)
                m_playerControl.TermoScaleVisible = termoScaleBtn.Checked;
        }

        private void palleteSelectionCtrl_SelectedIndexChanged(object sender, EventArgs e)
        {
            string[] pallete_file_names = { "", "\\PAL\\RAIN.PAL", "\\PAL\\IRON.PAL" };
            pallete_filename = "";
            ToolStripComboBox palleteCtrl = (ToolStripComboBox)sender;

            if ((uint)palleteCtrl.SelectedIndex < pallete_file_names.Length && palleteCtrl.SelectedIndex != 0)
            {
                string current_directory = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                pallete_filename = current_directory + pallete_file_names[palleteCtrl.SelectedIndex];
            }


            if (pallete_filename.Length > 0)
                _irb_frame_helper.SetPallete(pallete_filename);
            else
                _irb_frame_helper.SetDefaultPallete();

            setPallete();
            ShowFrame();
        }

        private void setPallete()
        {
            uint colors_number = 0;
            int len = 0;
            object pallete = null;
            len = _irb_frame_helper.GetPalleteLength(out colors_number);
            pallete = new Int32[len];
            _irb_frame_helper.GetPallete(ref pallete);

            CPalette _palette = new CPalette();
            _palette.image = (Int32[])pallete;
            _palette.numI = colors_number;

            Int32[] pal = (Int32[])pallete;

            System.Windows.Media.Color[] colors = new System.Windows.Media.Color[len];

            for (int i = 0; i < len; i++)
            {
                Color color = Color.FromArgb(pal[i]);
                colors[i] = System.Windows.Media.Color.FromArgb(color.A, color.R, color.G, color.B);
            }
            m_playerControl.termoScale.Palette = colors;
        }


        private void ShotForm_Load(object sender, EventArgs e)
        {
            ShowFrame();
        }

        private void ShotForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            _irb_frame_helper.ReleaseIRBFiles();
            try
            {
                File.Delete(_temp_movie_file_name);
            }
            catch (System.IO.IOException)
            {

            }

        }
    }
}
