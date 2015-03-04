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
    public partial class EquElementForm : Form
    {
        ReportTuning rt = null;

        public event EventHandler<AllAreasDeletedEvent> allAreasDeletedHandler;

        protected ITRWrapper m_tvHandler = new TRWrapper();

        EquObject m_element = null;

        private bool m_needToSave = false;

        PlayerControl m_playerControl = new PlayerControl(false);

        //Registrator.teplovizorDataSet.shotsDataTable m_shotTable = null;

        ArrayList testDates = new ArrayList();

        float _minT, _avrT, _maxT;

        public EquElementForm()
        {

            InitializeComponent();

            palleteSelection.SelectedIndex = 0;

            //if (m_playerControl == null)
            //    m_playerControl = new PlayerControl();
            //m_playerControl.TermoScaleVisible = false;

            //m_playerControl.LimitsChangedEventHandler += LimitsChangedEventFired;

            //elementHost1.Child = m_playerControl;
            //m_playerControl.ToolModeChangedEventHandler += ToolChangedEventFired;
            //m_playerControl.drawingCanvas.AreaAddedEventHandler += AreaAddedEventFired;
            //m_playerControl.drawingCanvas.AreasDeletedEventHandler += AreasDeletedEventFired;
            //m_playerControl.drawingCanvas.AreaChangedEventHandler += AreaChangedEventFired;
        }

        public EquElementForm(EquObject element,ref DB.DataBaseHelper dbHelperArg) 
            : this()
        {
            
            m_element = element;

            InitForm();

            dbHelper = dbHelperArg;
        
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
            polyButton.Enabled = !objAreaExists;

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
            elpNoffset.Text = String.Concat(new object[]{m_element.Picket.ToString(), " + ", m_element.Offset.ToString()});



            //Registrator.teplovizorDataSet.ElementAreasTableDataTable eadt = elementAreasTableAdapter1.GetDataByElement(m_element.ID);

            //for (int i = 0; i < eadt.Rows.Count; i++ )
            //{
            //    byte aType = Convert.ToByte(eadt.Rows[i].ItemArray[15]);
            //    byte aaType = Convert.ToByte(eadt.Rows[i].ItemArray[13]);
                        
            //    switch (aType)
            //    {
            //        case(0):
            //        {
            //            m_element.ObjectArea = new Area();
            //            m_element.ObjectArea.Type = Area.AreaType.AREA_RECT;
            //            m_element.ObjectArea.ProgID = 0;
                        
            //            if(aaType == 1)
            //                m_element.ObjectArea.Type = Area.AreaType.AREA_ELLIPS;

            //            if (aaType == 2)
            //            {

            //                throw new Exception("Need to realize FREE AREA");

            //                break;
            //            }

            //            m_element.ObjectArea.DbId = Convert.ToInt32(eadt.Rows[i].ItemArray[0]);
            //            m_element.ObjectArea.X = Convert.ToInt32(eadt.Rows[i].ItemArray[11]);
            //            m_element.ObjectArea.Y = Convert.ToInt32(eadt.Rows[i].ItemArray[12]);
            //            m_element.ObjectArea.Width = Convert.ToInt32(eadt.Rows[i].ItemArray[9]);
            //            m_element.ObjectArea.Height = Convert.ToInt32(eadt.Rows[i].ItemArray[10]);

            //            m_element.ObjectArea.MinTempBorder = Convert.ToSingle(eadt.Rows[i].ItemArray[2]);
            //            m_element.ObjectArea.AvrgTempBorder = Convert.ToSingle(eadt.Rows[i].ItemArray[6]);
            //            m_element.ObjectArea.MaxTempBorder = Convert.ToSingle(eadt.Rows[i].ItemArray[4]);

            //            break;
            //        }
            //        case (1):
            //        {
            //            m_element.DeltaAreaFirst = new Area();
            //            m_element.DeltaAreaFirst.Type = Area.AreaType.AREA_RECT;
            //            m_element.DeltaAreaFirst.ProgID = 0;
                        
            //            if (aaType == 1)
            //                m_element.DeltaAreaFirst.Type = Area.AreaType.AREA_ELLIPS;

            //            if (aaType == 2)
            //            {

            //                throw new Exception("Need to realize FREE AREA");

            //            }

            //            m_element.DeltaAreaFirst.DbId = Convert.ToInt32(eadt.Rows[i].ItemArray[0]);
            //            m_element.DeltaAreaFirst.X = Convert.ToInt32(eadt.Rows[i].ItemArray[11]);
            //            m_element.DeltaAreaFirst.Y = Convert.ToInt32(eadt.Rows[i].ItemArray[12]);
            //            m_element.DeltaAreaFirst.Width = Convert.ToInt32(eadt.Rows[i].ItemArray[9]);
            //            m_element.DeltaAreaFirst.Height = Convert.ToInt32(eadt.Rows[i].ItemArray[10]);

            //            m_element.DeltaAreaFirst.MinTempBorder = Convert.ToSingle(eadt.Rows[i].ItemArray[2]);
            //            m_element.DeltaAreaFirst.AvrgTempBorder = Convert.ToSingle(eadt.Rows[i].ItemArray[6]);
            //            m_element.DeltaAreaFirst.MaxTempBorder = Convert.ToSingle(eadt.Rows[i].ItemArray[4]);

            //            break;
            //        }

            //        case (2):
            //        {
            //            m_element.DeltaAreaSecond = new Area();
            //            m_element.DeltaAreaSecond.Type = Area.AreaType.AREA_RECT;
            //            m_element.DeltaAreaSecond.ProgID = 1;
                        
            //            if (aaType == 1)
            //                m_element.DeltaAreaSecond.Type = Area.AreaType.AREA_ELLIPS;

            //            if (aaType == 2)
            //            {

            //                throw new Exception("Need to realize FREE AREA");

            //            }

            //            m_element.DeltaAreaSecond.DbId = Convert.ToInt32(eadt.Rows[i].ItemArray[0]);
            //            m_element.DeltaAreaSecond.X = Convert.ToInt32(eadt.Rows[i].ItemArray[11]);
            //            m_element.DeltaAreaSecond.Y = Convert.ToInt32(eadt.Rows[i].ItemArray[12]);
            //            m_element.DeltaAreaSecond.Width = Convert.ToInt32(eadt.Rows[i].ItemArray[9]);
            //            m_element.DeltaAreaSecond.Height = Convert.ToInt32(eadt.Rows[i].ItemArray[10]);

            //            m_element.DeltaAreaSecond.MinTempBorder = Convert.ToSingle(eadt.Rows[i].ItemArray[2]);
            //            m_element.DeltaAreaSecond.AvrgTempBorder = Convert.ToSingle(eadt.Rows[i].ItemArray[6]);
            //            m_element.DeltaAreaSecond.MaxTempBorder = Convert.ToSingle(eadt.Rows[i].ItemArray[4]);

            //            break;
            //        }

            //    }



            //}

            //InitTestDates();
            
            //LoadInitialFrame();

            //LoadState();

            //comboBox1.SelectedIndexChanged += new EventHandler(comboBox1_SelectedIndexChanged);

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            NeedToSave(true);
        }

        private void LoadState()
        {

            comboBox1.SelectedIndex = 0;
            
            //teplovizorDataSet.stateDataTable sdt = stateTableAdapter1.GetDataByLastElementState(m_element.ID);

            //if (sdt.Rows.Count < 1)
            //    return;

            //comboBox1.SelectedIndex = Convert.ToInt32(sdt.Rows[0].ItemArray[3]); 

        }

        private void LoadInitialFrame()
        {

            if (availableTests.Items.Count > 0)
                availableTests.SetSelected(availableTests.Items.Count - 1, true);
        
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

            //if (InvokeRequired)
            //{
            //    Invoke(new LoadFrameDelegate(LoadFrame), new object[] { filePath });
            //}
            //else
            //{
            //    if (m_tvHandler == null)
            //        return;

            //    m_tvHandler.SetFiles(new String[] { filePath }, 0, 0);

            //    float avr = 0;

            //    float frameTempMin = 0;
            //    float frameTempMax = 0;
            //    float calTempMin = 0;
            //    float calTempMax = 0;

            //    int imgW, imgH;

            //    byte[] raster = m_tvHandler.getGetFrameRaster((short)0, out avr, out frameTempMin, out frameTempMax, out calTempMin, out calTempMax, out imgW, out imgH);

            //    if (raster != null)
            //    {
            //        if(imgW==1024) SetPlayerControlImage(raster, 1024, 768);
            //        else SetPlayerControlImage(raster, 640, 480);
            //    }

            //    SetToolMode();

            //}

        }

        private void RepaintCurrentMeasure()
        {

            //if (m_tvHandler == null || availableTests.Items.Count < 1 || availableTests.SelectedIndices.Count < 1)
            //    return;

            //float frameTempMin = 0;
            //float frameTempMax = 0;
            //float calTempMin = 0;
            //float calTempMax = 0;

            //m_tvHandler.SetCurrentFrameNumber(0);

            //object raster = new byte[1024 * 770 * 4];
            //int imgW, imgH;

            //m_tvHandler.GetCurrentFrameRaster(ref raster, out frameTempMin, out frameTempMax, out calTempMin, out calTempMax, out imgW, out imgH);

            //if (raster != null)
            //{
            //    if(imgW==1024) SetPlayerControlImage((byte[])raster, 1024, 768);
            //    else SetPlayerControlImage((byte[])raster, 640, 480);
            //}

            //SetToolMode();

        }

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
            // TODO: данная строка кода позволяет загрузить данные в таблицу "allEquipmentDataSet.EquipmentData". При необходимости она может быть перемещена или удалена.
            //this.equipmentDataTableAdapter.Fill(this.allEquipmentDataSet.EquipmentData);

            //this.reportViewer1.RefreshReport();
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

            SaveElementArea(m_element.ObjectArea, m_element.ID, 0);

            SaveElementArea(m_element.DeltaAreaFirst, m_element.ID, 1);

            SaveElementArea(m_element.DeltaAreaSecond, m_element.ID, 2);

            SaveElementState();

            NeedToSave(false);

        }

        private void SaveElementSite()
        {

            //equipmentTableAdapter1.UpdateSiteForElement((byte)((m_element.IsLeft) ? 1 : 0), m_element.ID);

        }

        private void SaveElementArea(Area area, int elementId, byte elementAreaType = 0)
        {

            //int affectedRows = 0;

            //affectedRows = equipment_areasTableAdapter1.DeleteByElementType(m_element.ID, elementAreaType);

            //if (area == null)
            //    return;

            //byte aType = 0;
            //if(area.Type == Area.AreaType.AREA_ELLIPS )
            //    aType = 1;
            //if(area.Type == Area.AreaType.AREA_FREE)
            //    aType = 2;

            //if (area.DbId == -1)
            //{
            //    affectedRows = areasTableAdapter1.Insert(area.Name
            //                             , area.MinTempBorder
            //                             , -1000
            //                             , area.MaxTempBorder
            //                             , -1000
            //                             , area.AvrgTempBorder
            //                             , -1000
            //                             , area.FreeFormForDB()
            //                             , (int)area.Width
            //                             , (int)area.Height
            //                             , (int)area.X
            //                             , (int)area.Y
            //                             , aType);

            //    if (affectedRows < 1)
            //        return;

            //    area.DbId = (int)areasTableAdapter1.LastIdentity();

            //}
            //else
            //{
            //    affectedRows = areasTableAdapter1.UpdateAreaById( area.MaxTempBorder
            //        , area.MinTempBorder
            //        , area.AvrgTempBorder
            //        , area.FreeFormForDB()
            //        , (int)area.Width
            //        , (int)area.Height
            //        , (int)area.X
            //        , (int)area.Y
            //        , aType
            //        , (int)area.DbId);
            //}

            //affectedRows = equipment_areasTableAdapter1.Insert(m_element.ID, (int)area.DbId, elementAreaType);

        }

        private void SaveElementState()
        {

            //int affectedRows = 0;

            //if (m_element == null)
            //    return;

            //if(m_element.State != (byte)comboBox1.SelectedIndex)
            //    affectedRows = stateTableAdapter1.Insert(m_element.ID, DateTime.Now, comboBox1.SelectedIndex);
        
        }

        private void button1_Click(object sender, EventArgs e)
        {
            
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

        public DB.DataBaseHelper dbHelper;
        
        private void button3_Click(object sender, EventArgs e)
        {

            OrdersListForm olf = new OrdersListForm(m_element,ref dbHelper);
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

        public void TempRefresh()
        {

            //bool objectMode = m_playerControl.modeSelection.SelectedIndex == 1;
            //bool frameMode = m_playerControl.modeSelection.SelectedIndex == 0;
            //bool deltaMode = m_playerControl.modeSelection.SelectedIndex == 2;

            //m_tvHandler.GetCurFrameTemperatures(out _minT, out _avrT, out _maxT);

            //SetAvrT(0f);
            //SetMinT(0f);
            //SetMaxT(0f);

            //if (frameMode)
            //{
            //    SetAvrT(_avrT);
            //    SetMinT(_minT);
            //    SetMaxT(_maxT);
            //}
            //if (objectMode)
            //{
            //    if (m_element.ObjectArea == null)
            //        return;
            //    m_tvHandler.GetAreaInfo((short)m_element.ObjectArea.ProgID, out _minT, out _maxT, out _avrT);
            //    m_element.ObjectArea.MinTemp = _minT;
            //    m_element.ObjectArea.AvrgTemp = _avrT;
            //    m_element.ObjectArea.MaxTemp = _maxT;
            //    SetMinT(m_element.ObjectArea.MinTemp);
            //    SetAvrT(m_element.ObjectArea.AvrgTemp);
            //    SetMaxT(m_element.ObjectArea.MaxTemp);
            //}
            //if (deltaMode)
            //{

            //    SetMinT(0);
            //    SetAvrT(0);
            //    SetMaxT(0);

            //    if (m_element.DeltaAreaFirst == null && m_element.DeltaAreaSecond == null)
            //        return;

            //    if (m_element.DeltaAreaFirst != null)
            //    {
            //        m_tvHandler.GetAreaInfo((short)m_element.DeltaAreaFirst.ProgID, out _minT, out _maxT, out _avrT);
            //        m_element.DeltaAreaFirst.MinTemp = _minT;
            //        m_element.DeltaAreaFirst.AvrgTemp = _avrT;
            //        m_element.DeltaAreaFirst.MaxTemp = _maxT;

            //        SetMinT(_minT);
            //        SetAvrT(_avrT);
            //        SetMaxT(_maxT);

            //    }

            //    if (m_element.DeltaAreaSecond != null)
            //    {
            //        m_tvHandler.GetAreaInfo((short)m_element.DeltaAreaSecond.ProgID, out _minT, out _maxT, out _avrT);
            //        m_element.DeltaAreaSecond.MinTemp = _minT;
            //        m_element.DeltaAreaSecond.AvrgTemp = _avrT;
            //        m_element.DeltaAreaSecond.MaxTemp = _maxT;

            //        SetMinT(m_element.DeltaTempMin());
            //        SetAvrT(m_element.DeltaTempAvr());
            //        SetMaxT(m_element.DeltaTempMax());

            //    }

                
            //}
        }

        

        private void InitTestDates()
        {

            //if (m_element == null)
            //    return;

            //testDates = new ArrayList();

            //availableTests.Items.Clear();

            //Registrator.teplovizorDataSet.shotsDataTable sdt = shotsTableAdapter1.GetDataByElement(m_element.ID);

            //for (int i = 0; i < sdt.Rows.Count; i++)
            //{
            //    bool presence = false;
            //    DateTime date = Convert.ToDateTime(sdt.Rows[i].ItemArray[1]);

            //    for(int j = 0 ; j < i ; j++)
            //        if (((DateTime)testDates[j]) == date)
            //        {
            //            presence = true;
            //            break;
            //        }

            //    if (!presence)
            //    {
            //        testDates.Add(date);
            //        availableTests.Items.Add(testDates.Count + ". " + date.ToShortDateString());
            //    }
            
            //}


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

        private void availableTests_SelectedIndexChanged(object sender, EventArgs e)
        {
            //shotsTableAdapter1.FillByElementShotForDate(teplovizorDataSet1.shots, m_element.ID, Convert.ToString(testDates[availableTests.SelectedIndex]));
            loadMeasure();
        }

        private void loadMeasure()
        {
            //int curIndex = 0;
            //if (availableTests.SelectedIndex > -1)
            //    curIndex = availableTests.SelectedIndex;
            //if (testDates.Count - 1 < curIndex)
            //    return;
            //String dateStr = Convert.ToString(testDates[curIndex]);
            
            //m_shotTable = shotsTableAdapter1.GetDataByElementShotForDate(m_element.ID, (DateTime)testDates[curIndex]);
            
            //if (m_shotTable.Rows.Count > 0 && Convert.ToString(m_shotTable.Rows[0].ItemArray[2]) != null)
            //    LoadFrame(Convert.ToString(m_shotTable.Rows[0].ItemArray[2]));
        
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

        public void FireAllAreasDeletedEvent(AllAreasDeletedEvent e)
        {
            EventHandler<AllAreasDeletedEvent> handler = allAreasDeletedHandler;
            if(handler != null)
            {
                handler(this, e);
            }
        }

        private void toolStripButton4_Click(object sender, EventArgs e)
        {

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

        private void toolStripComboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_tvHandler.SetDefaultPallete();

            string cd = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);//System. Directory.GetCurrentDirectory();
            //cd = "d:";
            switch (palleteSelection.SelectedIndex)
            {
                case (1):
                    {
                        m_tvHandler.SetPallete(cd + "\\PAL\\RAIN.PAL");//PalleteChangedEvent.PAL_FULLCOLOR);
                        break;
                    }
                case (2):
                    {

                        m_tvHandler.SetPallete(cd + "\\PAL\\IRON.PAL");
                        break;
                    } 

            }

            //LoadFrame();
            loadMeasure();
            uint num_col;

            int len = m_tvHandler.GetPalleteLength(out num_col);

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

        public void LimitsChangedEventFired(object sender, LimitsChangedEvent e)
        {
            //m_tvHandler.SetPalScaleBottom((float)e.Minimum);
            //m_tvHandler.SetPalScaleTop((float)e.Maximum);
            //m_tvHandler.SetNeedToCalcPalDiap(false);

            //RepaintCurrentMeasure();
            //loadMeasure();

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
