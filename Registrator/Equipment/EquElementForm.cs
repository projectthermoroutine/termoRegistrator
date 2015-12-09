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
using Registrator.Reports;


namespace Registrator
{
    public partial class EquElementForm : Form
    {
        ReportTuning rt = null;

        public event EventHandler<AllAreasDeletedEvent> allAreasDeletedHandler;

        EquObject m_element = null;

        bool m_needToSave = false;

        PlayerControl m_playerControl = new PlayerControl(false);

        DB.metro_db_edit_controller _db_controller;

        ArrayList testDates = new ArrayList();

        float _minT, _avrT, _maxT;
        MovieTransit _movie_transit;

        Area CurrentArea;

        EquElementForm()
        {
            InitializeComponent();

            m_playerControl.TermoScaleVisible = false;
           
            elementHost1.Child = m_playerControl;
        }

        public EquElementForm(EquObject element, DB.metro_db_controller db_controller)
            : this()
        {
            InitForm();

            m_element = element;
            setTextObjectInformation();

            _movie_transit = new MovieTransit();
            _db_controller = new DB.metro_db_edit_controller(db_controller);

            palleteSelection.SelectedIndex = 0;
            SetDataGrid();
        }


        void setTextObjectInformation()
        {
            label_name.Text = m_element.Name;
            label_Path.Text = m_element.Picket.Path.Code.ToString();
            label_line.Text = m_element.Picket.Path.Line.LineCode;
            label_group.Text = m_element.Picket.Path.Line.Group.Name;
            label_class.Text = m_element.Picket.Path.Line.Group.Class.Name;

            label_OffsetFromPicket.Text = m_element.Picket.npicket + " " + (m_element.Offset / 10).ToString() + " см";
            label_ObjectLenght.Text = m_element.ObjectLenght.ToString();

            if (m_element.X < 0)
                comboBox_objectPosition.SelectedIndex = 0;
            else
                comboBox_objectPosition.SelectedIndex = 1;

            if (m_element.State == 0)
                comboBox_technicalState.SelectedIndex = 0;
            else
                comboBox_technicalState.SelectedIndex = 1;

            if(m_element.ObjectLenght==0)
            {
                label_objectLenText.Visible = false;
                label_ObjectLenght.Visible = false;
            }


            if (m_element.strelkaDirection == -1)
            {
                lbl_strelkaDirection.Visible = false;
                label_strelkaDirection.Visible = false;
                return;
            }

            if (m_element.strelkaDirection == 0)
                label_strelkaDirection.Text = "слева направо";
            else
                label_strelkaDirection.Text = "справа налево";

        }

        List<DB.MetrocardDataSet.ObjectsFramesRow> ObjFramesList { get; set; }
        List<DateTime> dateTimeList;

        void SetDataGrid()
        {
            ObjFramesList = _db_controller.getObjMeasurements(m_element.Code);
            IEnumerator<DB.MetrocardDataSet.ObjectsFramesRow> IEnumeratorVar = ObjFramesList.GetEnumerator();
            dateTimeList = new List<DateTime>();

            while (IEnumeratorVar.MoveNext())
            {
                DateTime Time = (IEnumeratorVar.Current).Time;
                dateTimeList.Add(Time);
                string filePath = (IEnumeratorVar.Current).FilePath;

                if (File.Exists(filePath))
                    dg_measurements.Rows.Add(new object[] { Time.ToString(), filePath });
            }

            if (dateTimeList.Count > 0)
                button_reports.Enabled = true;
        }

        public void showTermogramm()
        {
            bool res = false;
            object raster = new byte[1024 * 770 * 4];
            _irb_frame_info frame_info = new _irb_frame_info();

            try
            {
                res = _movie_transit.GetFrameRaster(0,
                                            out frame_info,
                                            ref raster);

                if (res)
                {
                    if (frame_info.image_info.width == 1024) SetPlayerControlImage((byte[])raster, 1024, 768);
                    else SetPlayerControlImage((byte[])raster, 640, 480);

                    //var cur_coord = (long)frame_info.coordinate.coordinate + current_camera_offset;

                    var measure = new CTemperatureMeasure(frame_info.measure.tmin, frame_info.measure.tmax, frame_info.measure.tavr,
                        frame_info.measure.object_tmin, frame_info.measure.object_tmax, 0,
                        frame_info.measure.calibration_min, frame_info.measure.calibration_max);

                    var args = new object[] { measure };

                    SetThermoScaleLimits(measure);

                    Invoke(new SetTemperatureMeasureDelegate(SetTemperatureMeasure), args);
                    //Invoke(new SetTemperatureCalibrationLimitsDelegate(SetTemperatureCalibrationLimits), args);

                    //if (_is_cursor_position_valid)
                    //    get_cursor_point_temperature();

                    //if (m_areasPanel != null && m_areasPanel.Template != null && m_areasPanel.Template.Areas != null)
                    //{
                    //    get_areas_temperature_measure();
                    //}

                }//--------------------
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

        void LoadTermogramm(string fileName)
        {
            try
            {
                Array errors;

                bool _movie_loaded = _movie_transit.SetIRBFiles(new string[] { fileName }, out errors);

                List<string> status_list = new List<string>();
                long index = 0;
                int cols = errors.GetLength(errors.Rank - 1);

                for (index = 0; index < cols; index++)
                {
                    object status = errors.GetValue(index);
                    status_list.Add((string)status);
                }
            }
            catch (COMException e)
            {
                Console.WriteLine("SetIRBFiles:COMException : " + e.Message);
                return;
            }
            catch (OutOfMemoryException)
            {
                _movie_transit.ClearMovieTransitCache();
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

            _movie_transit.SetPaletteCalibration((float)e.Minimum, (float)e.Maximum);
            _movie_transit.SetPaletteCalibrationMode(_calibration_mode.MANUAL);

            minT_limit = (float)e.Minimum;
            maxT_limit = (float)e.Maximum;
            showTermogramm();
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

            _movie_transit.SetPaletteCalibrationMode(_calibration_mode);
            showTermogramm();
        }
        bool AreaAdded(ToolType type, int id, double x, double y, double w, double h)
        {
            var area_info = create_area_info(type, x, y, w, h);
            if (area_info.type == (_area_type)(-1))
                return false;
            try
            {
                _movie_transit.AddArea((short)id, ref area_info);
            }
            catch (ArgumentException)
            {
                return false;
            }

            return true;
        }
        bool AddArea(Area area)
        {
            _area_info area_info = new _area_info();
            area_info.type = (_area_type)(-1);
            area_traits traits = new area_traits(area.Type);
            if (!traits.availible)
                return false;

            area_info.type = _area_type.RECTANGLE;
            if (area.Type == Area.AreaType.AREA_ELLIPS)
                area_info.type = _area_type.ELLIPSE;

            area_info.x0 = (short)area.X;
            area_info.y0 = (short)area.Y;
            area_info.width = (ushort)area.Width;
            area_info.heigth = (ushort)area.Height;
            
            try
            {
                _movie_transit.AddArea((short)area.ProgID, ref area_info);
            }
            catch (ArgumentException)
            {
                return false;
            }

            return true;
        }
        _area_info create_area_info(ToolType type, double x, double y, double w, double h)
        {
            _area_info area_info = new _area_info();
            area_info.type = (_area_type)(-1);
            area_traits traits = new area_traits(type);
            if (!traits.availible)
                return area_info;

            area_info.type = _area_type.RECTANGLE;
            if (type == ToolType.Ellipse)
                area_info.type = _area_type.ELLIPSE;

            area_info.x0 = (short)x;
            area_info.y0 = (short)y;
            area_info.width = (ushort)w;
            area_info.heigth = (ushort)h;
            return area_info;

        }

        Area CreateArea(ToolType type, int id, double x, double y, double w, double h)
        {
            Area a = new Area();
          
            if (type == ToolType.Ellipse)
                a.Type = Area.AreaType.AREA_ELLIPS;

            a.X = (short)x;
            a.Y = (short)y;
            a.Width  = (ushort)w;
            a.Height = (ushort)h;
            a.ProgID = id;

            return a;
        }

        public void AreaAddedEventFired(object sender, AreaAddedEvent e)
        {
            if (AreaAdded(e.Type, (short)e.ID, (short)e.X, (short)e.Y, (short)e.Width, (short)e.Height))
            {
                CurrentArea = CreateArea(e.Type, (short)e.ID, (short)e.X, (short)e.Y, (short)e.Width, (short)e.Height);
                ChangeToolMode(ToolType.Pointer);
               
                rectButton.Enabled = false;
                ellipsButton.Enabled = false;
            }
        }

        private void NeedToSave(bool needToSave)
        {
            m_needToSave = needToSave;
            saveButton.Enabled = needToSave;
        }

        private void ToolButtonDisable(bool objAreaExists)
        {

            toolStripButton1.Enabled = !objAreaExists;
            toolStripButton2.Enabled = !objAreaExists;
            toolStripButton3.Enabled = !objAreaExists;
            toolStripButton5.Enabled = !objAreaExists;
            toolStripButton6.Enabled = !objAreaExists;
            LeftSiteButton.Enabled = !objAreaExists;
            RightSiteButton.Enabled = !objAreaExists;
            rectButton.Enabled = !objAreaExists;
            ellipsButton.Enabled = !objAreaExists;
        }

        private void AddAreaToHandler(Area area, short areaId)
        {

            //short type;

            //m_tvHandler.RemoveArea(areaId, out type);

            //if (area.Type == Area.AreaType.AREA_RECT)
            //    m_tvHandler.AddRectArea(areaId, (short)area.X, (short)area.Y, (short)area.Width, (short)area.Height);
            //if (area.Type == Area.AreaType.AREA_ELLIPS)
            //    m_tvHandler.AddEllipsArea(areaId, (short)area.X, (short)area.Y, (short)area.Width, (short)area.Height);

        }

        void AreaChangedEventFired(object sender, AreaChangedEvent e)
        {
            if(AreaChanged(e.Type, (short)e.Id, (short)e.X, (short)e.Y, (short)e.Width, (short)e.Height))
            {
                CurrentArea = CreateArea(e.Type, (short)e.Id, (short)e.X, (short)e.Y, (short)e.Width, (short)e.Height);
            }
        }

        bool AreaChanged(ToolType type, int id, double x, double y, double w, double h)
        {
            var area_info = create_area_info(type, x, y, w, h);
            
            if (area_info.type == (_area_type)(-1))
                return false;

            try
            {
                _movie_transit.AreaChanged((short)id, ref area_info);
            }
            catch (ArgumentException)
            {
                return false;
            }

            return true;
        }

        public void AreasDeletedEventFired(object sender, AreasDeletedEvent e)
        {
            rectButton.Enabled = true;
            ellipsButton.Enabled = true;

            _area_type area_type;

            foreach (var area_id in e.Areas)
                _movie_transit.RemoveArea((short)area_id, out area_type);
           
            if(e.Areas.Length > 0)
                CurrentArea = null;

        }

        private void SetObjectSite(bool isLeft)
        {
            LeftSiteButton.Checked = isLeft;
            RightSiteButton.Checked = !isLeft;

            NeedToSave(true);

        }

        private void InitForm()
        {
            if (m_element == null)
                return;

            label_name.Text = m_element.Name;
            label_Path.Text = m_element.Path.ToString();
            label_line.Text = m_element.Line.ToString();
            label_class.Text = m_element.Group.Class.Name;
            label_group.Text = m_element.Group.Name;
            // elLayout.Text = m_element.Layout.Name;

            if (m_element.ObjectLenght != -1)
                label_ObjectLenght.Text = Convert.ToString(m_element.ObjectLenght);
            if (m_element.strelkaDirection != -1)
                label_strelkaDirection.Text = (m_element.strelkaDirection == 1) ? "левая" : "правая";

            label_OffsetFromPicket.Text = String.Concat(new object[] { m_element.Picket.ToString(), " + ", m_element.Offset.ToString() });
            comboBox_technicalState.SelectedIndex = m_element.State;

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            NeedToSave(true);
        }

        private void LoadState()
        {
            comboBox_technicalState.SelectedIndex = 0;
        }

        private void LoadInitialFrame()
        {
        }

        public EquObject Element { get { return m_element; } set { m_element = value; InitForm(); } }

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

        private delegate void LoadFrameDelegate(String filePath);

        public delegate void SetToolModeDelegate();

        //public void SetToolMode()
        //{
        //    if (InvokeRequired)
        //        BeginInvoke(new SetToolModeDelegate(SetToolMode));//, new object[] { });
        //    else
        //    {
        //        PlayerControl.ToolMode toolMode = PlayerControl.ToolMode.FrameToolMode;
        //        if (m_playerControl.modeSelection.SelectedIndex == 1)
        //            toolMode = PlayerControl.ToolMode.ObjectToolMode;
        //        if (m_playerControl.modeSelection.SelectedIndex == 2)
        //            toolMode = PlayerControl.ToolMode.DeltaToolMode;
        //        ChangeToolMode(toolMode);
        //    }
        //}

        private delegate void SetPlayerControlImageSource(System.Windows.Media.ImageSource imgSrc);
        public void SetPlayerControlArea(System.Windows.Media.ImageSource imgSrc)
        {
            if (InvokeRequired)
            {
                Invoke(new SetPlayerControlImageSource(SetPlayerControlArea), new object[] { imgSrc });
            }
            else
            {
                m_playerControl.setImageSoucre(imgSrc);
            }
        }
        private void EquElementForm_Load(object sender, EventArgs e)
        {
            System.Windows.Media.Imaging.BitmapImage Bitmap = new System.Windows.Media.Imaging.BitmapImage(new Uri("pack://application:,,,/Registrator;component/Resources/tunnel.png"));

            SetPlayerControlArea(Bitmap);

            CurrentArea = _db_controller.loadArea(m_element.Code, new DateTime(), true);

            DrawArea();
        }

        private void closeButton_Click(object sender, EventArgs e)
        {

            if (m_needToSave)
            {
                DialogResult dr = MessageBox.Show("Данные объекта были изменены.\n Сохранить?", "Данные объекта были изменены", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
                if (dr == DialogResult.Cancel)
                    return;
                if (dr == DialogResult.Yes)
                    Save();
            }

            Close();
            Dispose();

        }

        private void saveButton_Click(object sender, EventArgs e)
        {
            Save();
        }

        private void Save()
        {
            if (m_element == null)
                return;

            SaveElementState();

        }

        private void SaveElementSite()
        {
            if(!(m_element.X<0 && comboBox_objectPosition.SelectedIndex==0) || !(m_element.X>0 && comboBox_objectPosition.SelectedIndex==1))
            {
                _db_controller.updateEquipmentPosition(m_element.Code, -m_element.X,m_element.Y);
            }
            if(m_element.State!= comboBox_technicalState.SelectedIndex)
            {
                _db_controller.updateEquipmentState(m_element.Code, comboBox_technicalState.SelectedIndex);
            }
        }

        private void SaveElementArea(Area area, int elementId, byte elementAreaType = 0)
        {
        }

        private void SaveElementState()
        {
            if (m_element == null)
                return;
            if (m_element.State != (byte)comboBox_technicalState.SelectedIndex)
                _db_controller.objects_adapter.updateEquipState(Element.Code, comboBox_technicalState.SelectedIndex);

        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (dg_measurements.CurrentRow != null)
                rt = new ReportTuning(dateTimeList[dg_measurements.CurrentRow.Index], dateTimeList.Last());
            else
            {
                if (dateTimeList.Count > 0)
                    rt = new ReportTuning(dateTimeList.First(), dateTimeList.Last());
            }

            rt.ReportTunedHandler += ReportTunedFired;
            rt.ShowDialog();
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

        private void fitButton_Click(object sender, EventArgs e)
        {
            m_playerControl.ResetImageStrech();
            m_playerControl.ActualScale = 0.75;
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            ChangeToolMode(ToolType.Rectangle);
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            ChangeToolMode(ToolType.Ellipse);
        }

        private void button3_Click(object sender, EventArgs e)
        {

            OrdersListForm olf = new OrdersListForm(m_element, _db_controller);
            olf.ShowDialog();

        }

        public void ToolChangedEventFired(object sender, ToolModeChangedEvent e)
        {

            //ChangeToolMode(e.Mode);

        }

        public delegate void InitCurrentToolModeDelegate(PlayerControl.ToolMode mode);

        private void ChangeToolMode(ToolType mode)
        {
            m_playerControl.drawingCanvas.Tool = mode;

        }

        //private void DrawAreas(PlayerControl.ToolMode toolMode)
        //{

        //    m_playerControl.drawingCanvas.DeleteAllAreas();

        //    switch (toolMode)
        //    {
        //        case (PlayerControl.ToolMode.ObjectToolMode):
        //            {

        //                if (m_element.ObjectArea != null)
        //                    AddAreaToHandler(m_element.ObjectArea, (short)m_element.ObjectArea.ProgID);

        //                m_playerControl.DrawArea(m_element.ObjectArea);

        //                break;
        //            }
        //        case (PlayerControl.ToolMode.DeltaToolMode):
        //            {

        //                if (m_element.DeltaAreaFirst != null)
        //                    AddAreaToHandler(m_element.DeltaAreaFirst, (short)m_element.DeltaAreaFirst.ProgID);

        //                m_playerControl.DrawArea(m_element.DeltaAreaFirst);

        //                if (m_element.DeltaAreaSecond != null)
        //                    AddAreaToHandler(m_element.DeltaAreaSecond, (short)m_element.DeltaAreaSecond.ProgID);

        //                m_playerControl.DrawArea(m_element.DeltaAreaSecond);

        //                break;
        //            }
        //    }
        //}

        bool get_area_info_movie(int area_id, out _area_temperature_measure measureT)
        {
            return _movie_transit.GetAreaInfo((uint)area_id, out measureT);
        }

        byte[] frameToByteArray(out CTemperatureMeasure TemperatureMeasure)
        {
            object raster = new byte[1024 * 770 * 4];
            _irb_frame_info frame_info = new _irb_frame_info();
            TemperatureMeasure = null;
            try
            {
                bool res = _movie_transit.GetFrameRaster(0,
                                             out frame_info,
                                             ref raster);
                TemperatureMeasure = new CTemperatureMeasure(frame_info.measure.tmin, frame_info.measure.tmax, frame_info.measure.tavr,
                        frame_info.measure.object_tmin, frame_info.measure.object_tmax, 0,
                        frame_info.measure.calibration_min, frame_info.measure.calibration_max);
               
            }
            catch (OutOfMemoryException)
            {
                _movie_transit.ClearMovieTransitCache();
                return null;
            }

            catch (COMException)
            {
                return null;
            }

            return (byte[])raster;
        }

        TempReportDataSet createRows(ReportTunedEvent e, DateTime DateTo)
        {
            TempReportDataSet frameReportData = new TempReportDataSet();
            bool haveMeasure = false;
            _movie_transit = new MovieTransit();


            var setEquipState = new Func<float,string>(max => {
                                                                if (m_element.MaxTemperature < max) return "неисправно";
                                                                else  return "исправно";
                                                              });

            for (int i = 0; i < dateTimeList.Count; i++)
            {
                if (dateTimeList[i].CompareTo(e.DateFrom) >= 0 && dateTimeList[i].CompareTo(DateTo) <= 0)
                {
                    TempReportData row = new TempReportData();
                    
                    row.Date = dateTimeList[i];

                    LoadTermogramm(ObjFramesList[i].FilePath);

                    Area Area_loc = _db_controller.loadArea(m_element.Code, dateTimeList[i], false);
                    if (Area_loc != null && !AddArea(Area_loc))
                        e.ObjectIsNeeded = false;

                    CTemperatureMeasure TemperatureMeasure;
                    byte[] raster = frameToByteArray(out TemperatureMeasure);
                    row.Picture = (byte[])raster;

                    _area_temperature_measure area_temperature_measure;

                    if (e.ObjectIsNeeded)
                    {
                        haveMeasure = get_area_info_movie(Area_loc.ProgID, out area_temperature_measure);
                        row.DeltaTemperature = Math.Round((double)(area_temperature_measure.max - area_temperature_measure.min),2);

                        row.State = setEquipState(area_temperature_measure.max);
                    }
                    else
                    {
                        row.DeltaTemperature = TemperatureMeasure.max - TemperatureMeasure.min;
                        row.State = setEquipState(TemperatureMeasure.max);
                    }

                    frameReportData.Rows.Add(row);
                }
            }

            return frameReportData;
        }

        public void ReportTunedFired(object sender, ReportTunedEvent e)
        {
            if (m_element == null)
                return;

            TempReportDataSet frameReportData = null;
            //TempReportDataSet objectReportData = null;
            //TempReportDataSet deltaReportData = null;

            if (CurrentArea == null)
                return;
          
            if (e.FromDateNeeded && e.ToDateNeeded) {
                frameReportData = createRows(e, e.DateTo);
            }
            else
            {
                if(e.FromDateNeeded)
                {
                    DateTime dateTo_loc = e.DateFrom;
                    dateTo_loc = dateTo_loc.Add(new TimeSpan(TimeSpan.TicksPerDay));

                    frameReportData = createRows(e, dateTo_loc);
                }
            }

            if (frameReportData != null)
            {
                ReportForm frameR = new ReportForm(frameReportData);
                frameR.Show();
            }

           
        }

        private void scaleButton_Click(object sender, EventArgs e)
        {
            m_playerControl.TermoScaleVisible = !m_playerControl.TermoScaleVisible;
        }

        private void toolStripButton7_Click(object sender, EventArgs e)
        {
            m_playerControl.InverseImageStrech();
        }

        public delegate void SetMinTDelegate(float mint);

        public delegate void SetMaxTDelegate(float maxt);

        //public delegate void SetMax_MinTDelegate(float maxmint);

        public delegate void SetAvrTDelegate(float avrt);

        public void SetMinT(float mint)
        {
            if (InvokeRequired)
                BeginInvoke(new SetMinTDelegate(SetMinT), new object[] { mint });
            else
                m_playerControl.MinT = mint;
        }

        public void SetMaxT(float maxt)
        {
            if (InvokeRequired)
                BeginInvoke(new SetMaxTDelegate(SetMaxT), new object[] { maxt });
            else
                m_playerControl.MaxT = maxt;
        }

        public void SetAvrT(float avrt)
        {
            if (InvokeRequired)
                BeginInvoke(new SetAvrTDelegate(SetAvrT), new object[] { avrt });
            else
                m_playerControl.AvrT = avrt;
        }

        public void FireAllAreasDeletedEvent(AllAreasDeletedEvent e)
        {
            EventHandler<AllAreasDeletedEvent> handler = allAreasDeletedHandler;
            if (handler != null)
            {
                handler(this, e);
            }
        }

        private void RightSiteButton_CheckedChanged(object sender, EventArgs e)
        {
            LeftSiteButton.CheckedChanged -= LeftSiteButton_CheckedChanged;
            LeftSiteButton.Checked = !RightSiteButton.Checked;
            m_element.IsLeft = LeftSiteButton.Checked;
            LeftSiteButton.CheckedChanged += LeftSiteButton_CheckedChanged;
        }

        private void LeftSiteButton_CheckedChanged(object sender, EventArgs e)
        {
            RightSiteButton.CheckedChanged -= RightSiteButton_CheckedChanged;
            RightSiteButton.Checked = !LeftSiteButton.Checked;
            m_element.IsLeft = LeftSiteButton.Checked;
            RightSiteButton.CheckedChanged += RightSiteButton_CheckedChanged;
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


            if (pallete_filename.Length > 0)
                _movie_transit.SetPallete(pallete_filename);
            else
                _movie_transit.SetDefaultPallete();

            setPallete();
            showTermogramm();
        }

        private void setPallete()
        {
            uint colors_number = 0;
            int len = 0;
            object pallete = null;
            len = _movie_transit.GetPalleteLength(out colors_number);
            pallete = new Int32[len];
            _movie_transit.GetPallete(ref pallete);

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

        private void comboBox1_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            saveButton.Enabled = true;
        }
        void DrawArea()
        {
            if (CurrentArea == null)
            {
                rectButton.Enabled = true;
                ellipsButton.Enabled = true;
            }
            else
            {
                m_playerControl.DrawArea(CurrentArea);
                rectButton.Enabled = false;
                ellipsButton.Enabled = false;
            }
        }
        private void dg_measurements_Click(object sender, EventArgs e)
        {
            if (dg_measurements.CurrentRow == null)
                return;
            

            m_playerControl.clearImageSource();

            LoadTermogramm(ObjFramesList[dg_measurements.CurrentRow.Index].FilePath);
            showTermogramm();

            palleteSelection.SelectedIndex = 0;
            toolStripButton_saveArea.Enabled = true;
            
            m_playerControl.drawingCanvas.DeleteAllAreas();
            CurrentArea = _db_controller.loadArea(m_element.Code, dateTimeList[dg_measurements.CurrentRow.Index], false);
            DrawArea();

            m_playerControl.LimitsChangedEventHandler += LimitsChangedEventFired;
            m_playerControl.drawingCanvas.AreaAddedEventHandler += AreaAddedEventFired;
            m_playerControl.drawingCanvas.AreasDeletedEventHandler += AreasDeletedEventFired;
            m_playerControl.drawingCanvas.AreaChangedEventHandler += AreaChangedEventFired;
        }

        private void toolStripButton_saveAreaAsDefault_Click(object sender, EventArgs e)
        {
            if(CurrentArea!=null)
                _db_controller.saveArea(m_element.Code, CurrentArea,true);
        }

        private void toolStripButton_saveArea_Click(object sender, EventArgs e)
        {
            if(dg_measurements.CurrentRow != null)
                _db_controller.saveArea(m_element.Code, CurrentArea, false, dateTimeList[dg_measurements.CurrentRow.Index]);
        }
    }

    public class AllAreasDeletedEvent : EventArgs
    {
        AllAreasDeletedEvent()
            : base()
        {
        }
    }
}
