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
    public partial class EquElementForm : Form
    {
        ReportTuning rt = null;

        public event EventHandler<AllAreasDeletedEvent> allAreasDeletedHandler;

        EquObject m_element = null;

        private bool m_needToSave = false;

        PlayerControl m_playerControl = new PlayerControl(false);

        DB.metro_db_controller _db_controller;

        ArrayList testDates = new ArrayList();

        float _minT, _avrT, _maxT;
        MovieTransit _movie_transit; 

        EquElementForm()
        {

            InitializeComponent();

            m_playerControl.TermoScaleVisible = false;
            m_playerControl.LimitsChangedEventHandler += LimitsChangedEventFired;
            elementHost1.Child = m_playerControl;
            m_playerControl.ToolModeChangedEventHandler += ToolChangedEventFired;
            m_playerControl.drawingCanvas.AreaAddedEventHandler += AreaAddedEventFired;
            m_playerControl.drawingCanvas.AreasDeletedEventHandler += AreasDeletedEventFired;
            m_playerControl.drawingCanvas.AreaChangedEventHandler += AreaChangedEventFired;
        }

        public EquElementForm(EquObject element,DB.metro_db_controller db_controller) 
            : this()
        {
            
            m_element = element;

            InitForm();

            _movie_transit = new MovieTransit();
            _db_controller = new DB.metro_db_controller(db_controller);
            
            palleteSelection.SelectedIndex = 0;
            IEnumerable<Registrator.DB.MetrocardDataSet.ObjectsFramesRow> ObjFramesList;
            ObjFramesList = _db_controller.getObjMeasurements(m_element.Code);
            DataGridViewRow[] DGRows = new DataGridViewRow[ObjFramesList.Count<Registrator.DB.MetrocardDataSet.ObjectsFramesRow>()];

            IEnumerator<Registrator.DB.MetrocardDataSet.ObjectsFramesRow> IEnumeratorVar = ObjFramesList.GetEnumerator();
            
            for(int i=0; i<ObjFramesList.Count<Registrator.DB.MetrocardDataSet.ObjectsFramesRow>(); i++)
            {
                IEnumeratorVar.MoveNext();
                string strTime = ((Registrator.DB.MetrocardDataSet.ObjectsFramesRow)IEnumeratorVar.Current).Time.ToString();
                string filePath = ((Registrator.DB.MetrocardDataSet.ObjectsFramesRow)IEnumeratorVar.Current).FilePath;

                if(!File.Exists(filePath))

                dg_measurements.Rows.Add(new object[]{strTime, filePath});
             
            }
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

                bool _movie_loaded = _movie_transit.SetIRBFiles(new string[]{fileName}, out errors);

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



        public void AreaAddedEventFired(object sender, AreaAddedEvent e)
        {

            //if(m_element == null)
            //    return;

            //if (m_playerControl.modeSelection.SelectedIndex == 1)
            //{
            //    m_element.ObjectArea = new Area();
            //    m_element.ObjectArea.ProgID = e.ID;
            //    m_element.ObjectArea.X = e.X;
            //    m_element.ObjectArea.Y = e.Y;
            //    m_element.ObjectArea.Width = e.Width;
            //    m_element.ObjectArea.Height = e.Height;
            //    m_element.ObjectArea.Type = (e.Type == ToolType.Rectangle) ? Area.AreaType.AREA_RECT : ((e.Type == ToolType.Ellipse) ? Area.AreaType.AREA_ELLIPS : Area.AreaType.AREA_FREE);

            //    AddAreaToHandler(m_element.ObjectArea, (short)e.ID);

            //    ToolButtonDisable(true);

            //}

            //if (m_playerControl.modeSelection.SelectedIndex == 2)
            //{

            //    if (m_element.DeltaAreaFirst == null)
            //    {
            //        m_element.DeltaAreaFirst = new Area();
            //        m_element.DeltaAreaFirst.ProgID = e.ID;
            //        m_element.DeltaAreaFirst.X = e.X;
            //        m_element.DeltaAreaFirst.Y = e.Y;
            //        m_element.DeltaAreaFirst.Width = e.Width;
            //        m_element.DeltaAreaFirst.Height = e.Height;
            //        m_element.DeltaAreaFirst.Type = (e.Type == ToolType.Rectangle) ? Area.AreaType.AREA_RECT : ((e.Type == ToolType.Ellipse) ? Area.AreaType.AREA_ELLIPS : Area.AreaType.AREA_FREE);

            //        AddAreaToHandler(m_element.DeltaAreaFirst, (short)e.ID);

            //    }
            //    else
            //    {
            //        m_element.DeltaAreaSecond = new Area();
            //        m_element.DeltaAreaSecond.ProgID = e.ID;
            //        m_element.DeltaAreaSecond.X = e.X;
            //        m_element.DeltaAreaSecond.Y = e.Y;
            //        m_element.DeltaAreaSecond.Width = e.Width;
            //        m_element.DeltaAreaSecond.Height = e.Height;
            //        m_element.DeltaAreaSecond.Type = (e.Type == ToolType.Rectangle) ? Area.AreaType.AREA_RECT : ((e.Type == ToolType.Ellipse) ? Area.AreaType.AREA_ELLIPS : Area.AreaType.AREA_FREE);

            //        AddAreaToHandler(m_element.DeltaAreaSecond, (short)e.ID);

            //        rectButton.Enabled = false;
            //        ellipsButton.Enabled = false;
            //        polyButton.Enabled = false;

            //    }

            //}

            //NeedToSave(true);

            //TempRefresh();

            //Console.Out.WriteLine("EquElementForm.AreaAddedEventFired : end");

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

        public void AreaChangedEventFired(object sender, AreaChangedEvent e)
        {

            //try
            //{
            //    if (m_playerControl.modeSelection.SelectedIndex == 1)
            //    {
            //        m_element.ObjectArea.X = e.X;
            //        m_element.ObjectArea.Y = e.Y;
            //        m_element.ObjectArea.Width = e.Width;
            //        m_element.ObjectArea.Height = e.Height;
            //    }

            //    if (m_playerControl.modeSelection.SelectedIndex == 2)
            //    {
            //        if (e.Id == 0 && m_element.DeltaAreaFirst != null)
            //        {
            //            m_element.DeltaAreaFirst.X = e.X;
            //            m_element.DeltaAreaFirst.Y = e.Y;
            //            m_element.DeltaAreaFirst.Width = e.Width;
            //            m_element.DeltaAreaFirst.Height = e.Height;
            //        }
            //        else if (e.Id == 1 && m_element.DeltaAreaSecond != null)
            //        {
            //            m_element.DeltaAreaSecond.X = e.X;
            //            m_element.DeltaAreaSecond.Y = e.Y;
            //            m_element.DeltaAreaSecond.Width = e.Width;
            //            m_element.DeltaAreaSecond.Height = e.Height;
            //        }
            //    }

            //    if (e.Type == ToolType.Rectangle)
            //        m_tvHandler.RectAreaChanged((short)e.Id, (short)e.X, (short)e.Y, (short)e.Width, (short)e.Height);
            //    if (e.Type == ToolType.Ellipse)
            //        m_tvHandler.EllipsAreaChanged((short)e.Id, (short)e.X, (short)e.Y, (short)e.Width, (short)e.Height);

            //    NeedToSave(true);

            //    TempRefresh();

            //}
            //catch(Exception ex)
            //{
            //    Console.Out.WriteLine("EquElement.AreaChangedEventFired Exception : " + ex.Message);
            //    Console.Out.WriteLine("   EquElement.AreaChangedEventFired Exception : " + ex.StackTrace);
            //}

        }

        public virtual void AreasDeletedEventFired(object sender, AreasDeletedEvent e)
        {
            //if(m_tvHandler == null)
            //    return;

            //if (m_playerControl.modeSelection.SelectedIndex == 1)
            //{

                
            //    short type;

            //    if (m_element.ObjectArea == null)
            //        return;

            //    m_tvHandler.RemoveArea((short)m_element.ObjectArea.ProgID, out type);
            //    m_element.ObjectArea = null;

            //    ToolButtonDisable(false);

            //}
            //if (m_playerControl.modeSelection.SelectedIndex == 2)
            //{

            //    short type;

            //    if (e.Areas.Length == 1)
            //    {
            //        if (e.Areas[0] == 0)
            //        {

            //            if (m_element.DeltaAreaFirst == null)
            //                return;

            //            m_tvHandler.RemoveArea((short)m_element.DeltaAreaFirst.ProgID, out type);
            //            m_element.DeltaAreaFirst = null;
                        
            //        }
            //        else
            //        {

            //            if (m_element.DeltaAreaSecond == null)
            //                return;

            //            m_tvHandler.RemoveArea((short)m_element.DeltaAreaSecond.ProgID, out type);
            //            m_element.DeltaAreaSecond = null;
                        
            //        }

            //    }
            //    else
            //    {

            //        if (m_element.DeltaAreaFirst == null)
            //            return;

            //        m_tvHandler.RemoveArea((short)m_element.DeltaAreaFirst.ProgID, out type);
            //        m_element.DeltaAreaFirst = null;
                    
            //        if (m_element.DeltaAreaSecond == null)
            //            return;

            //        m_tvHandler.RemoveArea((short)m_element.DeltaAreaSecond.ProgID, out type);
            //        m_element.DeltaAreaSecond = null;
                    
            //    }

            //    rectButton.Enabled = true;
            //    ellipsButton.Enabled = true;
            //    polyButton.Enabled = true;

            //}

            //NeedToSave(true);

            //TempRefresh();

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

            elName.Text = m_element.Name;
            elPath.Text = m_element.Path.ToString();
            elLine.Text = m_element.Line.ToString();
            elClass.Text = m_element.Group.Class.Name;
            elGroup.Text = m_element.Group.Name;
            elLayout.Text = m_element.Layout.Name;

            if (m_element.OffsetFromEnd != -1)
                lbl_shiftFromEndValue.Text = Convert.ToString(m_element.OffsetFromEnd);
            if(m_element.strelkaDirection!=-1)
                lbl_strelkaValue.Text = (m_element.strelkaDirection==1)? "левая": "правая";

            elpNoffset.Text = String.Concat(new object[]{m_element.Picket.ToString(), " + ", m_element.Offset.ToString()});
            comboBox1.SelectedIndex = m_element.State;

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            NeedToSave(true);
        }

        private void LoadState()
        {
            comboBox1.SelectedIndex = 0;
        }

        private void LoadInitialFrame()
        {
        }

        public EquObject Element  { get { return m_element;}set{m_element = value;InitForm();}}

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

        public void SetToolMode()
        {
            if (InvokeRequired)
                BeginInvoke(new SetToolModeDelegate(SetToolMode));//, new object[] { });
            else
            {
                PlayerControl.ToolMode toolMode = PlayerControl.ToolMode.FrameToolMode;

                if (m_playerControl.modeSelection.SelectedIndex == 1)
                    toolMode = PlayerControl.ToolMode.ObjectToolMode;
                if (m_playerControl.modeSelection.SelectedIndex == 2)
                    toolMode = PlayerControl.ToolMode.DeltaToolMode;

                InitCurrentToolMode(toolMode);
            }
        }

        private void EquElementForm_Load(object sender, EventArgs e)
        {
        }

        private void closeButton_Click(object sender, EventArgs e)
        {

            if (m_needToSave)
            {
                DialogResult dr = MessageBox.Show("Данные объекта были изменены.\n Сохранить?", "Данные объекта были изменены", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
                if (dr == DialogResult.Cancel)
                    return;
                if(dr == DialogResult.Yes)
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
        }

        private void SaveElementArea(Area area, int elementId, byte elementAreaType = 0)
        {
        }

        private void SaveElementState()
        {
            if (m_element == null)
                 return;
            if (m_element.State != (byte)comboBox1.SelectedIndex)
                _db_controller.objects_adapter.updateEquipState(Element.Code, comboBox1.SelectedIndex);
         
        }

        private void button1_Click(object sender, EventArgs e)
        {
            
            rt = new ReportTuning();

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
            ToolType ttype = ToolType.Rectangle;

            m_playerControl.drawingCanvas.Tool = ttype;
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            ToolType ttype = ToolType.Ellipse;

            m_playerControl.drawingCanvas.Tool = ttype;
        }

        private void button3_Click(object sender, EventArgs e)
        {

            OrdersListForm olf = new OrdersListForm(m_element,_db_controller);
            olf.ShowDialog();

        }

        public void ToolChangedEventFired(object sender, ToolModeChangedEvent e)
        {
            
            InitCurrentToolMode(e.Mode);

        }

        public delegate void InitCurrentToolModeDelegate(PlayerControl.ToolMode mode);

        private void InitCurrentToolMode(PlayerControl.ToolMode mode)
        {
            //if(InvokeRequired)
            //{
            //    BeginInvoke(new InitCurrentToolModeDelegate(InitCurrentToolMode), new object[] { mode });
            //    return;
            //}

            //toolStripButton1.Enabled = false;
            //toolStripButton2.Enabled = false;
            //toolStripButton3.Enabled = false;
            //toolStripButton5.Enabled = false;
            //toolStripButton6.Enabled = false;
            //LeftSiteButton.Enabled = false;
            //RightSiteButton.Enabled = false;
            //rectButton.Enabled = false;
            //ellipsButton.Enabled = false;
            //polyButton.Enabled = false;

            //if (m_element == null)
            //    return;

            //m_tvHandler.RemoveAllAreas();

            //if (mode == PlayerControl.ToolMode.ObjectToolMode && m_element.ObjectArea == null)
            //{
            //    toolStripButton1.Enabled = true;
            //    toolStripButton2.Enabled = true;
            //    toolStripButton3.Enabled = true;
            //    toolStripButton5.Enabled = true;
            //    toolStripButton6.Enabled = true;
            //    LeftSiteButton.Enabled = true;
            //    RightSiteButton.Enabled = true;
            //    rectButton.Enabled = true;
            //    ellipsButton.Enabled = true;
            //    polyButton.Enabled = true;
                
            //}

            //if (mode == PlayerControl.ToolMode.DeltaToolMode && (m_element.DeltaAreaFirst == null || m_element.DeltaAreaSecond == null))
            //{
            //    rectButton.Enabled = true;
            //    ellipsButton.Enabled = true;
            //    polyButton.Enabled = true;

            //}

            //DrawAreas(mode);

            //TempRefresh();

        }

        private void DrawAreas(PlayerControl.ToolMode toolMode)
        {

            m_playerControl.drawingCanvas.DeleteAllAreas();

            switch (toolMode)
            {
                case (PlayerControl.ToolMode.ObjectToolMode):
                    {

                        if (m_element.ObjectArea != null)
                            AddAreaToHandler(m_element.ObjectArea, (short)m_element.ObjectArea.ProgID);

                        m_playerControl.DrawArea(m_element.ObjectArea);

                        break;
                    }
                case (PlayerControl.ToolMode.DeltaToolMode):
                    {

                        if (m_element.DeltaAreaFirst != null)
                            AddAreaToHandler(m_element.DeltaAreaFirst, (short)m_element.DeltaAreaFirst.ProgID);

                        m_playerControl.DrawArea(m_element.DeltaAreaFirst);

                        if (m_element.DeltaAreaSecond != null)
                            AddAreaToHandler(m_element.DeltaAreaSecond, (short)m_element.DeltaAreaSecond.ProgID); 

                        m_playerControl.DrawArea(m_element.DeltaAreaSecond);

                        break;
                    }
            }

        }

        public void ReportTunedFired(object sender, ReportTunedEvent e)
        {
            //int shotsCount = 0;

            //if (m_element == null)
            //    return;

            //TempReportDataSet frameReportData = null;
            //TempReportDataSet objectReportData = null;
            //TempReportDataSet deltaReportData = null;

            //if (e.FrameIsNeeded)
            //{
            //    frameReportData = new TempReportDataSet();
            //}

            //if (e.ObjectIsNeeded)
            //{
            //    objectReportData = new TempReportDataSet();
            //}

            //if (e.DeltaIsNeeded)
            //{
            //    deltaReportData = new TempReportDataSet();
            //}

            //for (int i = 0; i < testDates.Count; i++ )
            //{
            //    if (e.FromDateNeeded && ((DateTime)testDates[i]) < e.DateFrom)
            //        continue;
            //    if (e.ToDateNeeded && ((DateTime)testDates[i]) > e.DateFrom)
            //        continue;

            //    Registrator.teplovizorDataSet.shotsDataTable sdt = shotsTableAdapter1.GetDataByElementDate(m_element.ID, (DateTime)testDates[i]);
            //    Registrator.teplovizorDataSet.stateDataTable stateDt = stateTableAdapter1.GetDataByElementStateForDate(m_element.ID, (DateTime)testDates[i]);

            //    String state = "Исправно";
            //    if(stateDt.Rows.Count > 0)
            //    {
            //        switch(Convert.ToInt32(stateDt.Rows[0].ItemArray[3]))
            //        {
            //            case(0) :
            //            {
            //                state = "Исправно";
            //                break;
            //            }
            //            case(1) :
            //            {
            //                state = "Неисправно";
            //                break;
            //            }
            //        }
            //    }

            //    if (sdt.Rows.Count < 1)
            //        continue;

            //    if (m_tvHandler == null)
            //        return;

            //    m_tvHandler.SetFiles(new String[] { Convert.ToString(sdt.Rows[0].ItemArray[2]) }, 0, 0);

            //    float _minT, _avrT, _maxT;

            //    m_tvHandler.GetCurFrameTemperatures(out _minT, out _avrT, out _maxT);

            //    if(e.FrameIsNeeded)
            //    {

            //        if (frameReportData != null)
            //        {
                        
            //            TempReportData row = new TempReportData();

            //            row.Date = Convert.ToDateTime(sdt.Rows[0].ItemArray[1]);
            //            row.Number = i + 1;
            //            row.State = state;
                        
            //            row.TempMin = _minT;
            //            row.TempAvr = _avrT;
            //            row.TempMax = _maxT;

            //            frameReportData.Rows.Add(row);
                    
            //        }

            //    }

            //    if(e.ObjectIsNeeded && m_element.ObjectArea != null)
            //    {
                    
            //        m_tvHandler.GetAreaInfo((short)m_element.ObjectArea.ProgID, out _minT, out _maxT, out _avrT);
            //        m_element.ObjectArea.MinTemp = _minT;
            //        m_element.ObjectArea.AvrgTemp = _avrT;
            //        m_element.ObjectArea.MaxTemp = _maxT;

            //        if(objectReportData != null)
            //        {
            //            TempReportData row = new TempReportData();
                        
            //            row.Date = Convert.ToDateTime(sdt.Rows[0].ItemArray[1]);
            //            row.Number = i + 1;
            //            row.State = state;
            //            row.TempMin = m_element.ObjectArea.MinTemp;
            //            row.TempAvr = m_element.ObjectArea.AvrgTemp;
            //            row.TempMax = m_element.ObjectArea.MaxTemp;
                        
            //            objectReportData.Rows.Add(row);
            //        }
                
            //    }

            //    if (e.DeltaIsNeeded && m_element.DeltaAreaFirst != null && m_element.DeltaAreaSecond != null)
            //    {

            //        m_tvHandler.GetAreaInfo((short)m_element.DeltaAreaFirst.ProgID, out _minT, out _maxT, out _avrT);
            //        m_element.DeltaAreaFirst.MinTemp = _minT;
            //        m_element.DeltaAreaFirst.AvrgTemp = _avrT;
            //        m_element.DeltaAreaFirst.MaxTemp = _maxT;

            //        m_tvHandler.GetAreaInfo((short)m_element.DeltaAreaSecond.ProgID, out _minT, out _maxT, out _avrT);
            //        m_element.DeltaAreaSecond.MinTemp = _minT;
            //        m_element.DeltaAreaSecond.AvrgTemp = _avrT;
            //        m_element.DeltaAreaSecond.MaxTemp = _maxT;

            //        if (deltaReportData != null)
            //        {
            //            TempReportData row = new TempReportData();

            //            row.Date = Convert.ToDateTime(sdt.Rows[0].ItemArray[1]);
            //            row.Number = i + 1;
            //            row.State = state;
            //            row.TempMin = m_element.DeltaTempMin();
            //            row.TempAvr = m_element.DeltaTempAvr();
            //            row.TempMax = m_element.DeltaTempMax();

            //            deltaReportData.Rows.Add(row);
            //        }

                
            //    }






            //    shotsCount++;
            //}

            //if (shotsCount == 0)
            //{
            //    if (MessageBox.Show("За указанный период измерения в БД отсутствуют! /n Скорректируйте период составления отчета.", "Внимание!", MessageBoxButtons.OKCancel) == DialogResult.Cancel)
            //    {
            //        rt.ReportTunedHandler -= ReportTunedFired;
            //        rt.Close();
            //        rt.Dispose();
            //        rt = null;

            //        return;
            //    }
            //    else
            //    {
            //        return;
            //    }
            //}

            //if(frameReportData != null)
            //{    
            //    ReportForm frameR = new ReportForm(frameReportData.Rows);
            //    frameR.Show();
            //}

            //if (objectReportData != null)
            //{
            //    ReportForm objectR = new ReportForm(objectReportData.Rows);
            //    objectR.Show();
            //}

            //if (deltaReportData != null)
            //{
            //    ReportForm deltaR = new ReportForm(deltaReportData.Rows);
            //    deltaR.Show();
            //}

            //if (availableTests.SelectedIndex > -1)
            //{
            //    //shotsTableAdapter1.FillByElementShotForDate(teplovizorDataSet1.shots, m_element.ID, Convert.ToString(testDates[availableTests.SelectedIndex]));
            //    //m_shotTable = shotsTableAdapter1.GetDataByElementShotForDate(m_element.ID, Convert.ToString(testDates[availableTests.SelectedIndex]));
            //    //LoadFrame();
            //}

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
            if(handler != null)
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

    }

    public class AllAreasDeletedEvent : EventArgs
    {

        AllAreasDeletedEvent()
            : base()
        {

        }

    }
}
