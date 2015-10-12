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

namespace Registrator
{
    public partial class ShotForm : Form
    {
        ReportTuning rt = null;

        //public event EventHandler<AllAreasDeletedEvent> allAreasDeletedHandler;

        protected IMovieTransit m_tvHandler = new MovieTransit();

        //String m_framePath = "";

        EquObject m_element = null;

        bool isChanged = false;

        PlayerControl m_playerControl;

        Registrator.DB.teplovizorDataSet.shotsDataTable m_shotTable = null;

        ArrayList testDates = new ArrayList();

        public ShotForm()
        {

            InitializeComponent();

            palleteSelection.SelectedIndex = 0;

            m_playerControl = new PlayerControl(false);
            m_playerControl.TermoScaleVisible = false;

            m_playerControl.LimitsChangedEventHandler += LimitsChangedEventFired;

            elementHost1.Child = m_playerControl;
            m_playerControl.ToolModeChangedEventHandler += ToolChangedEventFired;
            m_playerControl.drawingCanvas.AreaAddedEventHandler += AreaAddedEventFired;
            m_playerControl.drawingCanvas.AreasDeletedEventHandler += AreasDeletedEventFired;
            m_playerControl.drawingCanvas.AreaChangedEventHandler += AreaChangedEventFired;
        }

        public ShotForm(EquObject element) 
            : this()
        {
            
            m_element = element;

            InitForm();
        
        }

        public void AreaAddedEventFired(object sender, AreaAddedEvent e)
        {

            if(m_element == null)
                return;

            if (m_playerControl.modeSelection.SelectedIndex == 1)
            {
                m_element.ObjectArea = new Area();
                m_element.ObjectArea.X = e.X;
                m_element.ObjectArea.Y = e.Y;
                m_element.ObjectArea.Width = e.Width;
                m_element.ObjectArea.Height = e.Height;
                m_element.ObjectArea.Type = (e.Type == ToolType.Rectangle) ? Area.AreaType.AREA_RECT : ((e.Type == ToolType.Ellipse) ? Area.AreaType.AREA_ELLIPS : Area.AreaType.AREA_FREE);

                AddAreaToHandler(m_element.ObjectArea, -1);

                toolStripButton1.Enabled = false;
                //toolStripButton2.Enabled = false;
                //toolStripButton3.Enabled = false;
                //toolStripButton5.Enabled = false;
                //toolStripButton6.Enabled = false;
                //LeftSiteButton.Enabled = false;
                //RightSiteButton.Enabled = false;
                //rectButton.Enabled = false;
                //ellipsButton.Enabled = false;
                //polyButton.Enabled = false;

            }

            if (m_playerControl.modeSelection.SelectedIndex == 2)
            {

                if (m_element.DeltaAreaFirst == null)
                {
                    m_element.DeltaAreaFirst = new Area();
                    m_element.DeltaAreaFirst.X = e.X;
                    m_element.DeltaAreaFirst.Y = e.Y;
                    m_element.DeltaAreaFirst.Width = e.Width;
                    m_element.DeltaAreaFirst.Height = e.Height;
                    m_element.DeltaAreaFirst.Type = (e.Type == ToolType.Rectangle) ? Area.AreaType.AREA_RECT : ((e.Type == ToolType.Ellipse) ? Area.AreaType.AREA_ELLIPS : Area.AreaType.AREA_FREE);

                    AddAreaToHandler(m_element.DeltaAreaFirst, -2);

                }
                else
                {
                    m_element.DeltaAreaSecond = new Area();
                    m_element.DeltaAreaSecond.X = e.X;
                    m_element.DeltaAreaSecond.Y = e.Y;
                    m_element.DeltaAreaSecond.Width = e.Width;
                    m_element.DeltaAreaSecond.Height = e.Height;
                    m_element.DeltaAreaSecond.Type = (e.Type == ToolType.Rectangle) ? Area.AreaType.AREA_RECT : ((e.Type == ToolType.Ellipse) ? Area.AreaType.AREA_ELLIPS : Area.AreaType.AREA_FREE);

                    AddAreaToHandler(m_element.DeltaAreaSecond, -3);

                    //rectButton.Enabled = false;
                    //ellipsButton.Enabled = false;
                    //polyButton.Enabled = false;

                }

            }

            TempRefresh();

        }

        private void AddAreaToHandler(Area area, short areaId)
        {

            _area_type type;

            m_tvHandler.RemoveArea(areaId, out type);

            _area_info area_info = new _area_info();
            area_info.x0 = (short)area.X;
            area_info.y0 = (short)area.Y;
            area_info.width = (ushort)area.Width;
            area_info.heigth = (ushort)area.Height;

            if (area.Type == Area.AreaType.AREA_RECT)
            {
                area_info.type = _area_type.RECTANGLE;

            }
            else if (area.Type == Area.AreaType.AREA_ELLIPS)
            {
                area_info.type = _area_type.ELLIPSE;
            }
            else
                return;

            m_tvHandler.AddArea(areaId, ref area_info);
                  
        }

        public void AreaChangedEventFired(object sender, AreaChangedEvent e)
        {

            
                if(m_playerControl.modeSelection.SelectedIndex == 1)
                {
                    m_element.ObjectArea.X = e.X;
                    m_element.ObjectArea.Y = e.Y;
                    m_element.ObjectArea.Width = e.Width;
                    m_element.ObjectArea.Height = e.Height;
                }

                if(m_playerControl.modeSelection.SelectedIndex == 2)
                {
                    if(e.Id == 0 && m_element.DeltaAreaFirst != null)
                    {
                        m_element.DeltaAreaFirst.X = e.X;
                        m_element.DeltaAreaFirst.Y = e.Y;
                        m_element.DeltaAreaFirst.Width = e.Width;
                        m_element.DeltaAreaFirst.Height = e.Height;
                    }
                    else if (e.Id == 1 && m_element.DeltaAreaSecond != null)
                    {
                        m_element.DeltaAreaSecond.X = e.X;
                        m_element.DeltaAreaSecond.Y = e.Y;
                        m_element.DeltaAreaSecond.Width = e.Width;
                        m_element.DeltaAreaSecond.Height = e.Height;
                    }
                }

                _area_info area_info = new _area_info();
                area_info.x0 = (short)e.X;
                area_info.y0 = (short)e.Y;
                area_info.width = (ushort)e.Width;
                area_info.heigth = (ushort)e.Height;

                if (e.Type == ToolType.Rectangle)
                {
                    area_info.type = _area_type.RECTANGLE;

                }
                else if (e.Type == ToolType.Ellipse)
                {
                    area_info.type = _area_type.ELLIPSE;
                }
                else
                    return;

                m_tvHandler.AreaChanged((short)e.Id, ref area_info);
 
             TempRefresh();

        }

        public virtual void AreasDeletedEventFired(object sender, AreasDeletedEvent e)
        {
            if(m_tvHandler == null)
                return;

            if (m_playerControl.modeSelection.SelectedIndex == 1)
            {

                m_element.ObjectArea = null;

                _area_type type;

                m_tvHandler.RemoveArea(-1, out type);

                toolStripButton1.Enabled = true;

            }
            if (m_playerControl.modeSelection.SelectedIndex == 2)
            {

                _area_type type;

                if (e.Areas.Length == 1)
                {
                    if (e.Areas[0] == 0)
                    {
                        m_element.DeltaAreaFirst = null;
                        m_tvHandler.RemoveArea(-2, out type);
                    }
                    else
                    {
                        m_element.DeltaAreaSecond = null;
                        m_tvHandler.RemoveArea(-3, out type);
                    }

                }
                else
                {
                    m_element.DeltaAreaFirst = null;
                    m_tvHandler.RemoveArea(-2, out type);
                    m_element.DeltaAreaSecond = null;
                    m_tvHandler.RemoveArea(-3, out type);
                }

            }


            TempRefresh();

        }

        private void SetObjectSite(bool isLeft)
        {
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
            elpNoffset.Text = String.Concat(new object[]{m_element.Picket.ToString(), " + ", m_element.Offset.ToString()});



            Registrator.DB.teplovizorDataSet.ElementAreasTableDataTable eadt = elementAreasTableAdapter1.GetDataByElement(m_element.ID);

            for (int i = 0; i < eadt.Rows.Count; i++ )
            {
                byte aType = Convert.ToByte(eadt.Rows[i].ItemArray[15]);
                byte aaType = Convert.ToByte(eadt.Rows[i].ItemArray[13]);
                        
                switch (aType)
                {
                    case(0):
                    {
                        m_element.ObjectArea = new Area();
                        m_element.ObjectArea.Type = Area.AreaType.AREA_RECT;
                        
                        if(aaType == 1)
                            m_element.ObjectArea.Type = Area.AreaType.AREA_ELLIPS;

                        if (aaType == 2)
                        {
                            throw new Exception("Need to realize FREE AREA");
                        }

                        m_element.ObjectArea.DbId = Convert.ToInt32(eadt.Rows[i].ItemArray[0]);
                        m_element.ObjectArea.X = Convert.ToInt32(eadt.Rows[i].ItemArray[11]);
                        m_element.ObjectArea.Y = Convert.ToInt32(eadt.Rows[i].ItemArray[12]);
                        m_element.ObjectArea.Width = Convert.ToInt32(eadt.Rows[i].ItemArray[9]);
                        m_element.ObjectArea.Height = Convert.ToInt32(eadt.Rows[i].ItemArray[10]);

                        m_element.ObjectArea.MinTempBorder = Convert.ToSingle(eadt.Rows[i].ItemArray[2]);
                        m_element.ObjectArea.AvrgTempBorder = Convert.ToSingle(eadt.Rows[i].ItemArray[6]);
                        m_element.ObjectArea.MaxTempBorder = Convert.ToSingle(eadt.Rows[i].ItemArray[4]);

                        break;
                    }
                    case (1):
                    {
                        m_element.DeltaAreaFirst = new Area();
                        m_element.DeltaAreaFirst.Type = Area.AreaType.AREA_RECT;
                        
                        if (aaType == 1)
                            m_element.DeltaAreaFirst.Type = Area.AreaType.AREA_ELLIPS;

                        if (aaType == 2)
                        {
                            throw new Exception("Need to realize FREE AREA");
                        }

                        m_element.DeltaAreaFirst.DbId = Convert.ToInt32(eadt.Rows[i].ItemArray[0]);
                        m_element.DeltaAreaFirst.X = Convert.ToInt32(eadt.Rows[i].ItemArray[11]);
                        m_element.DeltaAreaFirst.Y = Convert.ToInt32(eadt.Rows[i].ItemArray[12]);
                        m_element.DeltaAreaFirst.Width = Convert.ToInt32(eadt.Rows[i].ItemArray[9]);
                        m_element.DeltaAreaFirst.Height = Convert.ToInt32(eadt.Rows[i].ItemArray[10]);

                        m_element.DeltaAreaFirst.MinTempBorder = Convert.ToSingle(eadt.Rows[i].ItemArray[2]);
                        m_element.DeltaAreaFirst.AvrgTempBorder = Convert.ToSingle(eadt.Rows[i].ItemArray[6]);
                        m_element.DeltaAreaFirst.MaxTempBorder = Convert.ToSingle(eadt.Rows[i].ItemArray[4]);

                        break;
                    }

                    case (2):
                    {
                        m_element.DeltaAreaSecond = new Area();
                        m_element.DeltaAreaSecond.Type = Area.AreaType.AREA_RECT;
                        
                        if (aaType == 1)
                            m_element.DeltaAreaSecond.Type = Area.AreaType.AREA_ELLIPS;

                        if (aaType == 2)
                        {
                            throw new Exception("Need to realize FREE AREA");
                        }

                        m_element.DeltaAreaSecond.DbId = Convert.ToInt32(eadt.Rows[i].ItemArray[0]);
                        m_element.DeltaAreaSecond.X = Convert.ToInt32(eadt.Rows[i].ItemArray[11]);
                        m_element.DeltaAreaSecond.Y = Convert.ToInt32(eadt.Rows[i].ItemArray[12]);
                        m_element.DeltaAreaSecond.Width = Convert.ToInt32(eadt.Rows[i].ItemArray[9]);
                        m_element.DeltaAreaSecond.Height = Convert.ToInt32(eadt.Rows[i].ItemArray[10]);

                        m_element.DeltaAreaSecond.MinTempBorder = Convert.ToSingle(eadt.Rows[i].ItemArray[2]);
                        m_element.DeltaAreaSecond.AvrgTempBorder = Convert.ToSingle(eadt.Rows[i].ItemArray[6]);
                        m_element.DeltaAreaSecond.MaxTempBorder = Convert.ToSingle(eadt.Rows[i].ItemArray[4]);

                        break;
                    }

                }



            }

            //InitTestDates();
            
            shotsTableAdapter1.FillByLastElementShot(teplovizorDataSet1.shots, m_element.ID);

            m_shotTable = shotsTableAdapter1.GetDataByLastElementShot(m_element.ID);

            LoadFrame();

            //LoadState();

        }

        //private void LoadState()
        //{

        //    comboBox1.SelectedIndex = 0;
            
        //    teplovizorDataSet.stateDataTable sdt = stateTableAdapter1.GetDataByLastElementState(m_element.ID);

        //    if (sdt.Rows.Count < 1)
        //        return;

        //    comboBox1.SelectedIndex = Convert.ToInt32(sdt.Rows[0].ItemArray[3]); 

        //}

        private void LoadFrame()
        {
            if(m_shotTable == null)
                return;// shotsTableAdapter1.GetData();

            if (m_shotTable.Rows.Count > 0)
            {
                if (m_shotTable.Rows.Count > 1)
                    MessageBox.Show(" st.Rows.Count > 1 ");

                //monthCalendar1.DateSelected -= monthCalendar1_DateSelected;

                //monthCalendar1.SetDate(Convert.ToDateTime(m_shotTable.Rows[0].ItemArray[1]));

                //monthCalendar1.DateSelected += monthCalendar1_DateSelected;

                backgroundWorker1.RunWorkerAsync(Convert.ToString(m_shotTable.Rows[0].ItemArray[2]));

            }
            else
            {
                //monthCalendar1.DateSelected -= monthCalendar1_DateSelected;

                //monthCalendar1.SetDate(DateTime.Now);

                //monthCalendar1.DateSelected += monthCalendar1_DateSelected;

            }
        }

        public EquObject Element
        {

            get
            {

                return m_element;
            
            }

            set
            {

                m_element = value;
                InitForm();

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

        private delegate void LoadFrameDelegate(String filePath);

        public void LoadFrame(String filePath)
        {

            if (InvokeRequired)
            {
                Invoke(new LoadFrameDelegate(LoadFrame), new object[] { filePath });
            }
            else
            {
                if (m_tvHandler == null)
                    return;

                Array errors;
                bool res = false;
                res = m_tvHandler.SetIRBFiles(new String[] { filePath }, out errors);
                if (!res)
                {
                    return;
                }


                _area_type type;

                m_tvHandler.RemoveArea(-1, out type);
                m_tvHandler.RemoveArea(-2, out type);
                m_tvHandler.RemoveArea(-3, out type);

                if (m_element != null)
                {
                //    if (m_element.ObjectArea != null)
                //    {
                //        if (m_element.ObjectArea.Type == Area.AreaType.AREA_RECT)
                //            m_tvHandler.AddRectArea(-1, (short)m_element.ObjectArea.X, (short)m_element.ObjectArea.Y, (short)m_element.ObjectArea.Width, (short)m_element.ObjectArea.Height);
                //        if (m_element.ObjectArea.Type == Area.AreaType.AREA_ELLIPS)
                //            m_tvHandler.AddEllipsArea(-1, (short)m_element.ObjectArea.X, (short)m_element.ObjectArea.Y, (short)m_element.ObjectArea.Width, (short)m_element.ObjectArea.Height);
                    
                //    }
                //    if (m_element.DeltaAreaFirst != null)
                //    {
                //        if (m_element.DeltaAreaFirst.Type == Area.AreaType.AREA_RECT)
                //            m_tvHandler.AddRectArea(-2, (short)m_element.DeltaAreaFirst.X, (short)m_element.DeltaAreaFirst.Y, (short)m_element.DeltaAreaFirst.Width, (short)m_element.DeltaAreaFirst.Height);
                //        if (m_element.DeltaAreaFirst.Type == Area.AreaType.AREA_ELLIPS)
                //            m_tvHandler.AddEllipsArea(-2, (short)m_element.DeltaAreaFirst.X, (short)m_element.DeltaAreaFirst.Y, (short)m_element.DeltaAreaFirst.Width, (short)m_element.DeltaAreaFirst.Height);

                //    }
                //    if (m_element.DeltaAreaSecond != null)
                //    {
                //        if (m_element.DeltaAreaSecond.Type == Area.AreaType.AREA_RECT)
                //            m_tvHandler.AddRectArea(-3, (short)m_element.DeltaAreaSecond.X, (short)m_element.DeltaAreaSecond.Y, (short)m_element.DeltaAreaSecond.Width, (short)m_element.DeltaAreaSecond.Height);
                //        if (m_element.DeltaAreaSecond.Type == Area.AreaType.AREA_ELLIPS)
                //            m_tvHandler.AddEllipsArea(-3, (short)m_element.DeltaAreaSecond.X, (short)m_element.DeltaAreaSecond.Y, (short)m_element.DeltaAreaSecond.Width, (short)m_element.DeltaAreaSecond.Height);

                //    }
                }

                res = false;
                object raster = new byte[1024 * 770 * 4];
               
                _irb_frame_info frame_info = new _irb_frame_info();
                try
                {
                    res = m_tvHandler.GetFrameRaster(0,
                                                out frame_info,
                                                ref raster);
                }
                catch (OutOfMemoryException)
                {
                    m_tvHandler.ClearMovieTransitCache();
                }

                if(m_playerControl != null)
                {
                    m_playerControl.LimitsChangedEventHandler -= LimitsChangedEventFired;
                    m_playerControl.termoScale.CalibrationLowerValue = frame_info.measure.calibration_min;
                    m_playerControl.termoScale.CalibrationTopValue = frame_info.measure.calibration_max;
                    m_playerControl.termoScale.ObjectLowerValue = frame_info.measure.tmin;
                    m_playerControl.termoScale.ObjectTopValue = frame_info.measure.tmax;
                    m_playerControl.LimitsChangedEventHandler += LimitsChangedEventFired;

                }

                if (res)
                {
                    if (frame_info.image_info.width == 1024) SetPlayerControlImage((byte[])raster, 1024, 768);
                    else SetPlayerControlImage((byte[])raster, 640, 480);
                }

                //SetAvrT(_avrT);
                //SetMinT(_minT);
                //SetMaxT(_maxT);

                SetToolMode();

                //PlayerControl.ToolMode toolMode = PlayerControl.ToolMode.FrameToolMode;

                //if (m_playerControl.modeSelection.SelectedIndex == 1)
                //    toolMode = PlayerControl.ToolMode.ObjectToolMode;
                //if (m_playerControl.modeSelection.SelectedIndex == 2)
                //    toolMode = PlayerControl.ToolMode.DeltaToolMode;

                //InitCurrentToolMode(toolMode);

            }

        }

        public delegate void SetToolModeDelegate();

        public void SetToolMode()
        {
            if (InvokeRequired)
                BeginInvoke(new SetToolModeDelegate(SetToolMode));//, new object[] { });
        }

        private void EquElementForm_Load(object sender, EventArgs e)
        {
        }

        private void closeButton_Click(object sender, EventArgs e)
        {

            if (isChanged)
            {
                Save();
            }
            Close();
            Dispose();
        }

        private void saveButton_Click(object sender, EventArgs e)
        {
            Save();
            Close();
            Dispose();
        }

        private void Save()
        {
            if (m_element == null)
                return;

            SaveElementArea(m_element.ObjectArea, m_element.ID, 0);

            SaveElementArea(m_element.DeltaAreaFirst, m_element.ID, 1);

            SaveElementArea(m_element.DeltaAreaSecond, m_element.ID, 2);

            //SaveElementState();

        }

        private void SaveElementSite()
        {

            equipmentTableAdapter1.UpdateSiteForElement((byte)((m_element.IsLeft) ? 1 : 0), m_element.ID);

        }

        private void SaveElementArea(Area area, int elementId, byte elementAreaType = 0)
        {

            int affectedRows = 0;

            affectedRows = equipment_areasTableAdapter1.DeleteByElementType(m_element.ID, elementAreaType);

            if (area == null)
                return;

            byte aType = 0;
            if(area.Type == Area.AreaType.AREA_ELLIPS )
                aType = 1;
            if(area.Type == Area.AreaType.AREA_FREE)
                aType = 2;

            if (area.DbId == -1)
            {
                affectedRows = areasTableAdapter1.Insert(area.Name
                                         , area.MinTempBorder
                                         , -1000
                                         , area.MaxTempBorder
                                         , -1000
                                         , area.AvrgTempBorder
                                         , -1000
                                         , area.FreeFormForDB()
                                         , (int)area.Width
                                         , (int)area.Height
                                         , (int)area.X
                                         , (int)area.Y
                                         , aType);

                if (affectedRows < 1)
                    return;

                area.DbId = (int)areasTableAdapter1.LastIdentity();

            }
            else
            {
                affectedRows = areasTableAdapter1.UpdateAreaById( area.MaxTempBorder
                    , area.MinTempBorder
                    , area.AvrgTempBorder
                    , area.FreeFormForDB()
                    , (int)area.Width
                    , (int)area.Height
                    , (int)area.X
                    , (int)area.Y
                    , aType
                    , (int)area.DbId);
            }

            affectedRows = equipment_areasTableAdapter1.Insert(m_element.ID, (int)area.DbId, elementAreaType);

        }

        //private void SaveElementState()
        //{

        //    int affectedRows = 0;

        //    if (m_element == null)
        //        return;

        //    if(m_element.State != (byte)comboBox1.SelectedIndex)
        //        affectedRows = stateTableAdapter1.Insert(m_element.ID, DateTime.Now, comboBox1.SelectedIndex);
        
        //}

        private void button1_Click(object sender, EventArgs e)
        {
            //TempGraphDialog dlg = new TempGraphDialog();
            //dlg.ShowDialog();

            //TestReport r = new TestReport();
            //r.Show();// Dialog();
            //return;

            rt = new ReportTuning();

            rt.ReportTunedHandler += ReportTunedFired;
            rt.ShowDialog();



        }

        private void button2_Click(object sender, EventArgs e)
        {

        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            String framePath = Convert.ToString(e.Argument);
            if (framePath != null && File.Exists(framePath))
                LoadFrame(framePath);
        }

        //private void monthCalendar1_DateSelected(object sender, DateRangeEventArgs e)
        //{
        //    shotsTableAdapter1.FillByElementShotForDate(teplovizorDataSet1.shots, m_element.ID, Convert.ToString(e.Start));
        //    m_shotTable = shotsTableAdapter1.GetDataByElementShotForDate(m_element.ID, Convert.ToString(e.Start));
        //    LoadFrame();
        //}

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
        private  DB.metro_db_controller _db_controller;
        private void button3_Click(object sender, EventArgs e)
        {

            OrdersListForm olf = new OrdersListForm(m_element,_db_controller);
            olf.ShowDialog();

        }

        public void ToolChangedEventFired(object sender, ToolModeChangedEvent e)
        {
            //throw new Exception("need to realize ToolChangedEventFired ");

            //bool objectMode = e.Mode == PlayerControl.ToolMode.ObjectToolMode;
            //bool frameMode = e.Mode == PlayerControl.ToolMode.FrameToolMode;
            //bool deltaMode = e.Mode == PlayerControl.ToolMode.DeltaToolMode;

            //toolStripButton1.Enabled = objectMode;
            //toolStripButton2.Enabled = objectMode;
            //toolStripButton3.Enabled = objectMode;
            //toolStripButton5.Enabled = objectMode;
            //toolStripButton6.Enabled = objectMode;
            //toolStripButton8.Enabled = objectMode;
            //toolStripButton9.Enabled = objectMode;

            //rectButton.Enabled = !frameMode;
            //ellipsButton.Enabled = !frameMode;
            //polyButton.Enabled = !frameMode;

            //InitCurrentToolMode(e.Mode);

        }

        public delegate void InitCurrentToolModeDelegate(PlayerControl.ToolMode mode);

        //private void InitCurrentToolMode(PlayerControl.ToolMode mode)
        //{
        //    if(InvokeRequired)
        //    {
        //        BeginInvoke(new InitCurrentToolModeDelegate(InitCurrentToolMode), new object[] { mode });
        //        return;
        //    }
        //    toolStripButton1.Enabled = false;
        //    //toolStripButton2.Enabled = false;
        //    //toolStripButton3.Enabled = false;
        //    //toolStripButton5.Enabled = false;
        //    //toolStripButton6.Enabled = false;
        //    //LeftSiteButton.Enabled = false;
        //    //RightSiteButton.Enabled = false;
        //    //rectButton.Enabled = false;
        //    //ellipsButton.Enabled = false;
        //    //polyButton.Enabled = false;

        //    if (m_element.ObjectArea == null)
        //    {
        //        toolStripButton1.Enabled = true;
        //        toolStripButton2.Enabled = true;
        //        toolStripButton3.Enabled = true;
        //        toolStripButton5.Enabled = true;
        //        toolStripButton6.Enabled = true;
        //        LeftSiteButton.Enabled = true;
        //        RightSiteButton.Enabled = true;
        //        rectButton.Enabled = true;
        //        ellipsButton.Enabled = true;
        //        polyButton.Enabled = true;
        //    }

        //    if (m_element.DeltaAreaFirst == null || m_element.DeltaAreaSecond == null)
        //    {
        //        rectButton.Enabled = true;
        //        ellipsButton.Enabled = true;
        //        polyButton.Enabled = true;
        //    }

        //    DrawAreas(mode);

        //    TempRefresh();

        //}

        private void DrawAreas(PlayerControl.ToolMode toolMode)
        {

            m_playerControl.drawingCanvas.DeleteAllAreas();

            switch (toolMode)
            {
                case (PlayerControl.ToolMode.ObjectToolMode):
                    {

                        m_playerControl.DrawArea(m_element.ObjectArea);

                        break;
                    }
                case (PlayerControl.ToolMode.DeltaToolMode):
                    {

                        m_playerControl.DrawArea(m_element.DeltaAreaFirst);

                        m_playerControl.DrawArea(m_element.DeltaAreaSecond);

                        break;
                    }
            }

        }

        public void TempRefresh()
        {

            bool objectMode = m_playerControl.modeSelection.SelectedIndex == 1;
            bool frameMode = m_playerControl.modeSelection.SelectedIndex == 0;
            bool deltaMode = m_playerControl.modeSelection.SelectedIndex == 2;
            _temperature_measure measure = new _temperature_measure();

            m_tvHandler.GetCurFrameTemperatures(out measure);

            SetAvrT(0f);
            SetMinT(0f);
            SetMaxT(0f);

            if (frameMode)
            {
                SetAvrT(measure.tavr);
                SetMinT(measure.tmin);
                SetMaxT(measure.tmax);
            }
           #region To Refactor
            //if (objectMode)
            //{
            //    if (m_element.ObjectArea == null)
            //        return;
            //    m_tvHandler.GetAreaInfo(-1, out _minT, out _maxT, out _avrT);
            //    m_element.ObjectArea.MinTemp = _minT;
            //    m_element.ObjectArea.AvrgTemp = _avrT;
            //    m_element.ObjectArea.MaxTemp = _maxT;
            //    SetMinT(m_element.ObjectArea.MinTemp);
            //    SetAvrT(m_element.ObjectArea.AvrgTemp);
            //    SetMaxT(m_element.ObjectArea.MaxTemp);
            //}
            //if (deltaMode)
            //{
            //    if (m_element.DeltaAreaFirst == null || m_element.DeltaAreaSecond == null)
            //        return;

            //    m_tvHandler.GetAreaInfo(-2, out _minT, out _maxT, out _avrT);
            //    m_element.DeltaAreaFirst.MinTemp = _minT;
            //    m_element.DeltaAreaFirst.AvrgTemp = _avrT;
            //    m_element.DeltaAreaFirst.MaxTemp = _maxT;

            //    m_tvHandler.GetAreaInfo(-3, out _minT, out _maxT, out _avrT);
            //    m_element.DeltaAreaSecond.MinTemp = _minT;
            //    m_element.DeltaAreaSecond.AvrgTemp = _avrT;
            //    m_element.DeltaAreaSecond.MaxTemp = _maxT;

            //    SetMinT(m_element.DeltaTempMin());
            //    SetAvrT(m_element.DeltaTempAvr());
            //    SetMaxT(m_element.DeltaTempMax());
            //}
            #endregion
        }

        

        //private void InitTestDates()
        //{

        //    if (m_element == null)
        //        return;

        //    testDates = new ArrayList();

        //    availableTests.Items.Clear();

        //    Registrator.teplovizorDataSet.shotsDataTable sdt = shotsTableAdapter1.GetDataByElement(m_element.ID);

        //    for (int i = 0; i < sdt.Rows.Count; i++)
        //    {
        //        bool presence = false;
        //        DateTime date = Convert.ToDateTime(sdt.Rows[i].ItemArray[1]);

        //        for(int j = 0 ; j < i ; j++)
        //            if (((DateTime)testDates[j]) == date)
        //            {
        //                presence = true;
        //                break;
        //            }

        //        if (!presence)
        //        {
        //            testDates.Add(date);
        //            availableTests.Items.Add(testDates.Count + ". " + date.ToShortDateString());
        //        }
            
        //    }


        //}

        //private void availableTests_SelectedValueChanged(object sender, EventArgs e)
        //{
        //    shotsTableAdapter1.FillByElementShotForDate(teplovizorDataSet1.shots, m_element.ID, Convert.ToString(testDates[availableTests.SelectedIndex]));
        //    m_shotTable = shotsTableAdapter1.GetDataByElementShotForDate(m_element.ID, Convert.ToString(testDates[availableTests.SelectedIndex]));
        //    LoadFrame();
        //}

        public void ReportTunedFired(object sender, ReportTunedEvent e)
        {
            int shotsCount = 0;

            if (m_element == null)
                return;

            TempReportDataSet frameReportData = null;
            TempReportDataSet objectReportData = null;
            TempReportDataSet deltaReportData = null;

            if (e.FrameIsNeeded)
            {
                frameReportData = new TempReportDataSet();
            }

            if (e.ObjectIsNeeded)
            {
                objectReportData = new TempReportDataSet();
            }

            if (e.DeltaIsNeeded)
            {
                deltaReportData = new TempReportDataSet();
            }

            for (int i = 0; i < testDates.Count; i++ )
            {
                if (e.FromDateNeeded && ((DateTime)testDates[i]) < e.DateFrom)
                    continue;
                if (e.ToDateNeeded && ((DateTime)testDates[i]) > e.DateFrom)
                    continue;

                Registrator.DB.teplovizorDataSet.shotsDataTable sdt = shotsTableAdapter1.GetDataByElementDate(m_element.ID, (DateTime)testDates[i]);
                Registrator.DB.teplovizorDataSet.stateDataTable stateDt = stateTableAdapter1.GetDataByElementStateForDate(m_element.ID, (DateTime)testDates[i]);

                String state = "Исправно";
                if(stateDt.Rows.Count > 0)
                {
                    switch(Convert.ToInt32(stateDt.Rows[0].ItemArray[3]))
                    {
                        case(0) :
                        {
                            state = "Исправно";
                            break;
                        }
                        case(1) :
                        {
                            state = "Неисправно";
                            break;
                        }
                    }
                }

                if (sdt.Rows.Count < 1)
                    continue;

                if (m_tvHandler == null)
                    return;

                Array errors;
                var res = m_tvHandler.SetIRBFiles(new String[] { Convert.ToString(sdt.Rows[0].ItemArray[2]) }, out errors);
                if (!res)
                {
                    return;
                }


                _temperature_measure measure = new _temperature_measure();
                m_tvHandler.GetCurFrameTemperatures(out measure);

                if(e.FrameIsNeeded)
                {

                    if (frameReportData != null)
                    {
                        
                        TempReportData row = new TempReportData();

                        row.Date = Convert.ToDateTime(sdt.Rows[0].ItemArray[1]);
                        row.Number = i + 1;
                        row.State = state;

                        row.TempMin = measure.tmin;
                        row.TempAvr = measure.tavr;
                        row.TempMax = measure.tmax;

                        frameReportData.Rows.Add(row);
                    
                    }

                }
                #region To Refactor

                //if (e.ObjectIsNeeded && m_element.ObjectArea != null)
                //{
                    
                //    m_tvHandler.GetAreaInfo(-1, out _minT, out _maxT, out _avrT);
                //    m_element.ObjectArea.MinTemp = _minT;
                //    m_element.ObjectArea.AvrgTemp = _avrT;
                //    m_element.ObjectArea.MaxTemp = _maxT;

                //    if(objectReportData != null)
                //    {
                //        TempReportData row = new TempReportData();
                        
                //        row.Date = Convert.ToDateTime(sdt.Rows[0].ItemArray[1]);
                //        row.Number = i + 1;
                //        row.State = state;
                //        row.TempMin = m_element.ObjectArea.MinTemp;
                //        row.TempAvr = m_element.ObjectArea.AvrgTemp;
                //        row.TempMax = m_element.ObjectArea.MaxTemp;
                        
                //        objectReportData.Rows.Add(row);
                //    }

                //}

                //if (e.DeltaIsNeeded && m_element.DeltaAreaFirst != null && m_element.DeltaAreaSecond != null)
                //{

                //    m_tvHandler.GetAreaInfo(-2, out _minT, out _maxT, out _avrT);
                //    m_element.DeltaAreaFirst.MinTemp = _minT;
                //    m_element.DeltaAreaFirst.AvrgTemp = _avrT;
                //    m_element.DeltaAreaFirst.MaxTemp = _maxT;
                    
                //    m_tvHandler.GetAreaInfo(-3, out _minT, out _maxT, out _avrT);
                //    m_element.DeltaAreaSecond.MinTemp = _minT;
                //    m_element.DeltaAreaSecond.AvrgTemp = _avrT;
                //    m_element.DeltaAreaSecond.MaxTemp = _maxT;

                //    if (deltaReportData != null)
                //    {
                //        TempReportData row = new TempReportData();

                //        row.Date = Convert.ToDateTime(sdt.Rows[0].ItemArray[1]);
                //        row.Number = i + 1;
                //        row.State = state;
                //        row.TempMin = m_element.DeltaTempMin();
                //        row.TempAvr = m_element.DeltaTempAvr();
                //        row.TempMax = m_element.DeltaTempMax();

                //        deltaReportData.Rows.Add(row);
                //    }


                //}
                #endregion


                shotsCount++;
            }

            if (shotsCount == 0)
            {
                if (MessageBox.Show("За указанный период измерения в БД отсутствуют! /n Скорректируйте период составления отчета.", "Внимание!", MessageBoxButtons.OKCancel) == DialogResult.Cancel)
                {
                    rt.ReportTunedHandler -= ReportTunedFired;
                    rt.Close();
                    rt.Dispose();
                    rt = null;

                    return;
                }
                else
                {
                    return;
                }
            }

            if(frameReportData != null)
            {    
                ReportForm frameR = new ReportForm(frameReportData.Rows);
                frameR.Show();
            }

            if (objectReportData != null)
            {
                ReportForm objectR = new ReportForm(objectReportData.Rows);
                objectR.Show();
            }

            if (deltaReportData != null)
            {
                ReportForm deltaR = new ReportForm(deltaReportData.Rows);
                deltaR.Show();
            }

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

        private void availableTests_SelectedIndexChanged(object sender, EventArgs e)
        {
            //shotsTableAdapter1.FillByElementShotForDate(teplovizorDataSet1.shots, m_element.ID, Convert.ToString(testDates[availableTests.SelectedIndex]));
            //loadMeasure();
        }

        //private void loadMeasure()
        //{
        //    int curIndex = 0;
        //    if (availableTests.SelectedIndex > -1)
        //        curIndex = availableTests.SelectedIndex;
        //    if (testDates.Count - 1 < curIndex)
        //        return;
        //    String dateStr = Convert.ToString(testDates[curIndex]);
        //    m_shotTable = shotsTableAdapter1.GetDataByElementShotForDate(m_element.ID, (DateTime)testDates[curIndex]);
        //    if (m_shotTable.Rows.Count > 0 && Convert.ToString(m_shotTable.Rows[curIndex].ItemArray[2]) != null)
        //        LoadFrame(Convert.ToString(m_shotTable.Rows[curIndex].ItemArray[2]));
        //}

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

        //public void SetMax_MinT(float maxmint)
        //{
        //    m_playerControl.Max_MinT = maxmint;
        //}

        public void SetAvrT(float avrt)
        {
            if (InvokeRequired)
                BeginInvoke(new SetAvrTDelegate(SetAvrT), new object[] { avrt });
            else
                m_playerControl.AvrT = avrt;
        }

        //public void FireAllAreasDeletedEvent(AllAreasDeletedEvent e)
        //{
        //    EventHandler<AllAreasDeletedEvent> handler = allAreasDeletedHandler;
        //    if(handler != null)
        //    {
        //        handler(this, e);
        //    }
        //}

        private void toolStripButton4_Click(object sender, EventArgs e)
        {

        }

        private void setPallete()
        {
            uint colors_number;
            int len = m_tvHandler.GetPalleteLength(out colors_number);

            object pallete = new Int32[len];

            m_tvHandler.GetPallete(ref pallete);

            Int32[] pal = (Int32[])pallete;

            System.Windows.Media.Color[] colors = new System.Windows.Media.Color[len];

            for (int i = 0; i < len; i++)
            {
                Color color = Color.FromArgb(pal[i]);
                colors[i] = System.Windows.Media.Color.FromArgb(color.A, color.R, color.G, color.B);// 
            }

            m_playerControl.termoScale.Palette = colors;
        }

        private void toolStripComboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            string[] pallete_file_names = { "", "\\PAL\\RAIN.PAL", "\\PAL\\IRON.PAL" };
            string pallete_filename = "";
            if ((uint)palleteSelection.SelectedIndex < pallete_file_names.Length && palleteSelection.SelectedIndex != 0)
            {
                string current_directory = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                pallete_filename = current_directory + pallete_file_names[palleteSelection.SelectedIndex];
            }
            if (pallete_filename.Length > 0)
            {
                m_tvHandler.SetPallete(pallete_filename);
            }
            else
            {
                m_tvHandler.SetDefaultPallete();
            }

            setPallete();

        }

        public void LimitsChangedEventFired(object sender, LimitsChangedEvent e)
        {
            //m_tvHandler.SetPalScaleBottom((float)e.Minimum);
            //m_tvHandler.SetPalScaleTop((float)e.Maximum);
            //m_tvHandler.SetNeedToCalcPalDiap(false);

            //loadMeasure();

        }

    }

    //public class AllAreasDeletedEvent : EventArgs
    //{

    //    AllAreasDeletedEvent()
    //        : base()
    //    {

    //    }

    //}
}
