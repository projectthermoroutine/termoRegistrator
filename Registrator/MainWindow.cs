using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;
using System.Runtime.InteropServices;
using System.Collections;
using System.IO;
using System.Threading;
using System.Runtime.Serialization.Formatters.Binary;
using NLog;
using Registrator.Equipment;
namespace Registrator
{
    
    public partial class MainWindow : Form
    {
        public DB.metro_db_controller db_manager;

        private bool dataBaseEnable = false;
        private FramesPanel m_filmFrames = null;// = new FramesPanel();
        private ProjectFilesPanel m_projectFiles = new ProjectFilesPanel();
        private EquipmentListPanel m_equipmentList = new EquipmentListPanel();
        private TrackPanel m_trackPanel = new TrackPanel();
        private AreasPanel m_areasPanel = new AreasPanel();
        private PlayerPanel m_doc = null;
        
        private ArrayList m_docs = null;
        private int m_curDocIndex = 0;

        private DeserializeDockContent m_deserializeDockContent;
        private bool m_bSaveLayout = true;

        public event EventHandler<TripProjectRoutineEvent> TripProjectOpenedHandler;
        public event EventHandler<TripProjectRoutineEvent> TripProjectSavedHandler;
        public event EventHandler<TripProjectRoutineEvent> TripProjectSyncedHandler;
        public event EventHandler<TripProjectRoutineEvent> TripProjectClosedHandler;
        public event EventHandler<PalleteChangedEvent> PalleteChangedHandler;

        private AllEquipmentTree2 m_equTree;
        private EquipmentMonitor m_equipMonitor = null;

        private d_statusChange statusChange;
        private void databaseStatus(string str)
        {
            toolStripStatusDataBaseLoad.Text = str;
        }
      

        static readonly Logger Log_ = LogManager.GetCurrentClassLogger();
        public MainWindow()
        {
            KeyPreview = true;

            InitializeComponent();
            m_equTree = null;

            statusChange = new d_statusChange(databaseStatus);

            DB_Loader_backgroundWorker.WorkerReportsProgress = true;
            DB_Loader_backgroundWorker.ProgressChanged += DB_Loader_backgroundWorker_ProgressChanged;
            DB_Loader_backgroundWorker.RunWorkerCompleted += DB_Loader_backgroundWorker_RunWorkerCompleted;
            DB_Loader_backgroundWorker.RunWorkerAsync();
            
            m_deserializeDockContent = new DeserializeDockContent(GetContentFromPersistString);
            m_projectFiles.VisibleChanged +=new EventHandler(m_projectFiles_VisibleChanged);
            m_equipmentList.VisibleChanged += new EventHandler(m_equipmentList_VisibleChanged);
            m_areasPanel.VisibleChanged += new EventHandler(m_areasPanel_VisibleChanged);

            m_projectFiles.HideOnClose = true;
            m_equipmentList.HideOnClose = true;
            m_areasPanel.HideOnClose = true;

            showFilmFiles(); 
            showEquipment();
            showTrack();
           
        }
        
        void DB_Loader_backgroundWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            Thread.Sleep(200);
            toolStripProgressBar1.Enabled = false;
            toolStripProgressBar1.Visible = false;
        }

        void DB_Loader_backgroundWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            toolStripProgressBar1.Value = e.ProgressPercentage;
        }
        ~MainWindow()
        {
            while (DB_Loader_backgroundWorker.IsBusy)
            {
                Thread.Sleep(200);
                Application.DoEvents();
            }

        }
        void m_equTree_VisibleChanged(object sender, EventArgs e)
        {
            if(m_equTree.IsHidden) 
                equTreeToolStripMenuItem.Checked = false;
        }

        void m_areasPanel_VisibleChanged(object sender, EventArgs e)
        {
            if(m_areasPanel.IsHidden)
                areasToolStripMenuItem.Checked = false;
        }

        void m_trackPanel_VisibleChanged(object sender, EventArgs e)
        {
            if(m_trackPanel.IsHidden)
                TrackToolStripMenuItem.Checked = false;
        }

        void m_equipmentList_VisibleChanged(object sender, EventArgs e)
        {
            if(m_equipmentList.IsHidden)
                EquToolStripMenuItem.Checked = false;
        }

        void m_filmFrames_VisibleChanged(object sender, EventArgs e)
        {
            if (m_filmFrames == null || m_filmFrames.IsHidden) 
                ReportFramesToolStripMenuItem.Checked = false;
        }

        void m_projectFiles_VisibleChanged(object sender, EventArgs e)
        {
            if(m_projectFiles.IsHidden)
                FilesToolStripMenuItem.Checked = false;
        }

        public ArrayList Docs
        {

            get
            {
                return m_docs;
            }

            set
            {
                m_docs = value;
            }

        }

        public void AddDocument(PlayerPanel document)
        {
            m_docs.Add(document);
        }

        public void DelDocument(PlayerPanel document)
        {
            m_docs.Remove(document);
        }

        public void DelDocument(int docIndex)
        {
            m_docs.Remove(docIndex);
        }

        public int CurrentDocIndex
        {
            get
            {
                return m_curDocIndex;
            }

            set
            {
                m_curDocIndex = value;
            }
        }

        private void filesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            showFilmFiles();
        }

        private void equipmentToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if(dataBaseEnable)
                showEquMonitor();
            else
                MessageBox.Show("База данных не загружена", "Информация", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void pathToolStripMenuItem_Click(object sender, EventArgs e)
        {
            showTrack();

        }

        private void HideDocks()
        {
            m_areasPanel.Hide();
            m_equipmentList.Hide();
            m_projectFiles.Hide();
            m_trackPanel.Hide();

            if (m_equTree != null)
                m_equTree.Hide();
        }

        private void CloseDocks()
        {
            m_areasPanel.Close();
            m_equipmentList.Close();
            m_projectFiles.Close();
            m_trackPanel.Close();

            if (m_equTree != null)
                m_equTree.Close();
        }

        private void CloseDoc()
        {
            if (m_doc != null)
            {
                m_doc.StopGrabbing();
                m_doc.Hide();
                PalleteChangedHandler -= m_doc.PalleteChangedEventFired;
                m_doc.EquListLoadedHandler -= EquListLoadedEventFired;
                m_equipmentList.ItemSelectedHandler -= m_doc.ItemSelectedEventFired;
                m_equipmentList.ObjectFilterSetEventHandler -= m_doc.ObjectFilterSetEventFired;
                m_areasPanel.newAreaEventHandler -= m_doc.AreaToolChanged;
                m_projectFiles.AnalizeEventHandler -= m_doc.AnalizedEventFired;

                m_doc.FileFromGrabberEventHandler -= m_projectFiles.FileFromGrabberEventFired;

                m_doc.AreaAddedEventHandler -= m_areasPanel.AreaAddedEventFired;
                m_doc.AreaChangedEventHandler -= m_areasPanel.AreaChangedEventFired;
                m_doc.PlayerControl.drawingCanvas.AreasDeletedEventHandler -= m_areasPanel.AreasDeletedEventFired;
                m_areasPanel.AreasDeletedInEditorEventHandler -= m_doc.PlayerControl.drawingCanvas.AreasDeletedInEditorEventFired;
                m_areasPanel.AreasTemplateSelectedEventHandler -= m_doc.AreasTemplateSelectedEventFired;

                m_doc.PlayerControl.drawingCanvas.AreasDeletedEventHandler -= m_doc.AreasDeletedFired;
                m_areasPanel.AreasDeletedInEditorEventHandler -= m_doc.AreasDeletedInEditorFired;

                m_doc.RecModeChangeEventHandler -= RecModeChangeEventFired;

                HideDocks();
                m_doc.Close();

                m_projectFiles = new ProjectFilesPanel();
                m_filmFrames = null;

            }
        }
        private int cameraOffset=0;
        private void NewProjectMenuItem_Click(object sender, EventArgs e)
        {
            ProjectProperties dlg = new ProjectProperties();
            dlg.camShiftSetHandler += dlg_camShiftSetHandler;
            DialogResult res = dlg.ShowDialog();
            if (res != System.Windows.Forms.DialogResult.OK)
            {
                return;
            }
            if (dlg.WasError) return;

            CloseDoc();

            while (DB_Loader_backgroundWorker.IsBusy)
            {
                Thread.Sleep(200);
                Application.DoEvents();
            }

            m_doc = CreateNewDocument();


            //if(db_manager!=null)
            //{
            //    createComponentDBDepend();
            //}

            m_doc.setMonitor(m_equipMonitor);

            m_doc.Text = dlg.ProjectName;
            m_doc.TripProject.FilePath = dlg.ProjectFolder;
            m_doc.TripProject.IRBFilesPath = dlg.IRBFolder;
            if (dlg.CameraTurnedToLeftSide) m_doc.TripProject.CameraDirection = TripProject.CameraDirections.Left;
            else m_doc.TripProject.CameraDirection = TripProject.CameraDirections.Right;

            InitializeDocument();

            m_projectFiles.TripProject = m_doc.TripProject;

            FireTripProjectOpened(new TripProjectRoutineEvent(m_doc.TripProject, TripProjectRoutineEvent.PROJECT_OPENED_EVENT));

            saveToolStripMenuItem.Enabled = true;
        }

        void dlg_camShiftSetHandler(object sender, eventCameraOffset e)
        {
            cameraOffset = e.offset;
        }

        private void InitializeDocument()
        {
            PlayerPanel_Load();

            m_doc.AreasControl = m_areasPanel;

            m_doc.FormClosing += MainForm_Closing;

            PalleteChangedHandler += m_doc.PalleteChangedEventFired;
            m_doc.EquListLoadedHandler += EquListLoadedEventFired;
            m_equipmentList.ObjectFilterSetEventHandler += m_doc.ObjectFilterSetEventFired;
            m_equipmentList.ItemSelectedHandler += m_doc.ItemSelectedEventFired;
            m_areasPanel.newAreaEventHandler += m_doc.AreaToolChanged;

            
            m_projectFiles.AnalizeEventHandler += m_doc.AnalizedEventFired;

            m_doc.FileFromGrabberEventHandler += m_projectFiles.FileFromGrabberEventFired;

            m_doc.AreaAddedEventHandler += m_areasPanel.AreaAddedEventFired;
            m_doc.AreaChangedEventHandler += m_areasPanel.AreaChangedEventFired;
            m_doc.PlayerControl.drawingCanvas.AreasDeletedEventHandler += m_areasPanel.AreasDeletedEventFired;
            m_areasPanel.AreasDeletedInEditorEventHandler += m_doc.PlayerControl.drawingCanvas.AreasDeletedInEditorEventFired;
            m_areasPanel.AreasTemplateSelectedEventHandler += m_doc.AreasTemplateSelectedEventFired;

            m_doc.PlayerControl.drawingCanvas.AreasDeletedEventHandler += m_doc.AreasDeletedFired;
            m_areasPanel.AreasDeletedInEditorEventHandler += m_doc.AreasDeletedInEditorFired;

            m_doc.RecModeChangeEventHandler += RecModeChangeEventFired;

            connect_player_errors();
            connect_pd_dispatcher_events();
            connect_grabber_dispatcher_events();
            connect_player_state_event();

            m_doc.Show(dockPanel);
        }


        public void RecModeChangeEventFired(object sender, RecModeChangeEvent e)
        {
            if (e.IsRecModeActivated)
            {
            }
        }

        private PlayerPanel CreateNewDocument()
        {

            // add hander for set hide or visibile Analyze button

            PlayerPanel dummyDoc = new PlayerPanel(db_manager,cameraOffset,m_projectFiles.setAnalyzeButtonVisibility);
            int count = 1;
            
            string text = "Проезд " + count.ToString();
            
            dummyDoc.Text = text;
            return dummyDoc;
        }

        private PlayerPanel CreateNewDocument(string text)
        {
            PlayerPanel dummyDoc = new PlayerPanel(db_manager, cameraOffset, m_projectFiles.setAnalyzeButtonVisibility);
            dummyDoc.Text = text;
            return dummyDoc;
        }

        private void showFilmFiles()
        {
            if (FilesToolStripMenuItem.Checked)
            {
                m_projectFiles.Show(dockPanel, DockState.DockLeft);
            }
            else
            {
                if (!m_projectFiles.IsHidden)
                    m_projectFiles.Hide();
            }
        }

        private void showReportFrames()
        {
            if (ReportFramesToolStripMenuItem.Checked)
            {
                if (m_filmFrames != null)
                {
                    m_filmFrames.Show(dockPanel, DockState.DockLeft | DockState.DockBottom);
                }
                else
                {
                    if (m_doc != null)
                    {
                        m_filmFrames = new FramesPanel(m_doc.pointsInfoManager, db_manager);
                        m_filmFrames.VisibleChanged += new EventHandler(m_filmFrames_VisibleChanged);
                        m_filmFrames.HideOnClose = true;
                        m_filmFrames.Show(dockPanel, DockState.DockLeft | DockState.DockBottom);
                    }
                    else
                        ReportFramesToolStripMenuItem.Checked = false;
                }
            }
            else
            {
                if(m_filmFrames != null && !m_filmFrames.IsHidden)
                    m_filmFrames.Hide();
            }
        }
        private void showEquMonitor()
        {
            while (DB_Loader_backgroundWorker.IsBusy)
            {
                Thread.Sleep(200);
                Application.DoEvents();
            }

            if (m_equipMonitor == null)
                return;

            if (EquToolStripMenuItem.Checked)
            {
                m_equipMonitor.Show(dockPanel, DockState.DockBottom);
            }
            else
            {
                if (!m_equipMonitor.IsHidden)
                    m_equipMonitor.Hide();
            }
        }
        private void showEquipment()
        {

            if (m_equipmentList == null)
                return;

            if (EquToolStripMenuItem.Checked)
            {
                m_equipmentList.Show(dockPanel, DockState.DockRight);
            }
            else
            {
                if(!m_equipmentList.IsHidden)
                    m_equipmentList.Hide();
            }
        }

        private void showTrack()
        {
            if (m_trackPanel == null)
                return;

            if (TrackToolStripMenuItem.Checked)
            {
                m_trackPanel.Show(dockPanel, DockState.DockBottom);
               // m_trackPanel.DrawMap();
            }
            else
            {
                if (!m_trackPanel.IsHidden)
                    m_trackPanel.Hide();
            }
        }

        private void showAreas()
        {
            if (areasToolStripMenuItem.Checked)
            {
                m_areasPanel.Show(dockPanel, DockState.DockBottom );
            }
            else
            {
                if(!m_areasPanel.IsHidden)
                    m_areasPanel.Hide();
            }
        }

        private void showEquTree()
        {
            if (m_equTree == null)
                return;

            if (equTreeToolStripMenuItem.Checked)
            {
                m_equTree.Show(dockPanel, DockState.DockLeft);
            }
            else
            {
                if (!m_equTree.IsHidden) 
                    m_equTree.Hide();
            }

        }

        private void openFilmToolStripMenuItem_Click(object sender, EventArgs e)
        {

            OpenFileDialog ofd = new OpenFileDialog();

            ofd.InitialDirectory = Application.UserAppDataPath;
            ofd.Filter = "Файлы проекта (*.mpr)|*.mpr";
            ofd.FilterIndex = 1;
            ofd.RestoreDirectory = true;

            if (ofd.ShowDialog() == DialogResult.OK)
            {
                TripProject tp = new TripProject();
                if (!tp.LoadProject(ofd.FileName))
                {
                    MessageBox.Show("Не удалось открыть проезд!", "Ошибка при чтении", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
                
                CloseDoc();

                if (m_doc != null)
                    m_doc.Hide();

                m_doc = CreateNewDocument();
                
                while (DB_Loader_backgroundWorker.IsBusy)
                {
                    Thread.Sleep(200);
                    Application.DoEvents();
                }

                m_doc.setMonitor(m_equipMonitor);

                m_doc.TripProject = tp;
                InitializeDocument();
                m_projectFiles.TripProject = tp;
                m_projectFiles.FillTheTree();
                m_doc.FilesNumber = tp.Files.Count;
                m_doc.UpdateWithProjectFiles((string[])tp.Files.ToArray());

                saveToolStripMenuItem.Enabled = true;
                saveAsToolStripMenuItem.Enabled = true;

                FireTripProjectOpened(new TripProjectRoutineEvent(m_doc.TripProject, TripProjectRoutineEvent.PROJECT_OPENED_EVENT));

            }

        }

        private IDockContent GetContentFromPersistString(string persistString)
        {
            if (persistString == typeof(AreasPanel).ToString())
                return m_areasPanel;
            else if (persistString == typeof(EquipmentListPanel).ToString())
                return m_equipmentList;
            else if (persistString == typeof(ProjectFilesPanel).ToString())
                return m_projectFiles;
            else if (persistString == typeof(TrackPanel).ToString())
                return m_trackPanel;
            else if (persistString == typeof(FramesPanel).ToString())
                return m_filmFrames;
            else
            {
                return null;
            }
        }

        private void PlayerPanel_Load()
        {
            string configFile = Path.Combine(Path.GetDirectoryName(Application.ExecutablePath), "DockPanel.config");

            if (File.Exists(configFile) && dockPanel.Contents.Count == 0)
                dockPanel.LoadFromXml(configFile, m_deserializeDockContent);
        }

        private void MainForm_Load(object sender, System.EventArgs e)
        {
            string configFile = Path.Combine(Path.GetDirectoryName(Application.ExecutablePath), "DockPanel.config");

            if (File.Exists(configFile))
                dockPanel.LoadFromXml(configFile, m_deserializeDockContent);
        }

        private void FinishAll()
        {
            string configFile = Path.Combine(Path.GetDirectoryName(Application.ExecutablePath), "DockPanel.config");
            if (m_bSaveLayout)
                dockPanel.SaveAsXml(configFile);
            else if (File.Exists(configFile))
                File.Delete(configFile);
            if (m_doc != null)
            {
                m_doc.FormClosing -= MainForm_Closing;

                m_doc.AllResourcesCloser();

                CloseDoc();

            }

            CloseDocks();


        }

        private void MainForm_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            FinishAll();
            while (DB_Loader_backgroundWorker.IsBusy)
            {
                Thread.Sleep(200);
                Application.DoEvents();
            }

        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void addFilesToolStripMenuItem_Click(object sender, EventArgs e)
        {

            if (m_doc == null || m_doc.TripProject == null)
                return;
            
            OpenFileDialog ofd = new OpenFileDialog();

            ofd.InitialDirectory = Application.UserAppDataPath;
            ofd.Filter = "IRB-файлы (*.irb)|*.irb";//|All files (*.*)|*.*";
            ofd.FilterIndex = 1;
            ofd.Multiselect = true;
            ofd.RestoreDirectory = true;

            if (ofd.ShowDialog() == DialogResult.OK)
            {
                m_doc.TripProject.addTermoFiles(ofd.FileNames);
            }

        }

        private void syncToolStripMenuItem_Click(object sender, EventArgs e)
        {

            if(m_doc != null && m_doc.TripProject != null )
            {


                FireTripProjectSynchronized(new TripProjectRoutineEvent(m_doc.TripProject, TripProjectRoutineEvent.PROJECT_SYNCED_EVENT));
            }

        }

        public void FireTripProjectOpened(TripProjectRoutineEvent e)
        {
            EventHandler<TripProjectRoutineEvent> handler = TripProjectOpenedHandler;

            if (handler != null)
            {
                handler(this, e);
            }
        }

        public void FireTripProjectSaved(TripProjectRoutineEvent e)
        {
            EventHandler<TripProjectRoutineEvent> handler = TripProjectSavedHandler;

            if (handler != null)
            {

                handler(this, e);

            }
        }

        public void FireTripProjectSynchronized(TripProjectRoutineEvent e)
        {
            EventHandler<TripProjectRoutineEvent> handler = TripProjectSyncedHandler;

            if (handler != null)
            {

                handler(this, e);

            }
        }

        public void FireTripProjectClosed(TripProjectRoutineEvent e)
        {
            EventHandler<TripProjectRoutineEvent> handler = TripProjectClosedHandler;

            if (handler != null)
            {

                handler(this, e);

            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (m_doc != null && m_doc.TripProject != null)
            {
                SaveProject();
                FireTripProjectSaved(new TripProjectRoutineEvent(m_doc.TripProject, TripProjectRoutineEvent.PROJECT_SAVED_EVENT));
            }
        }

        private void SaveProject()
        {
            string fname = m_doc.Text + ".mpr";

            string current_directory = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
            string project_filename = current_directory + "\\" + m_doc.TripProject.FilePath + "\\" + fname;

            if (!m_doc.TripProject.SaveProject(project_filename))
            {
                MessageBox.Show("Не удалось сохранить проезд!", "Ошибка записи в файл", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (m_doc != null && m_doc.TripProject != null)
                FireTripProjectSaved(new TripProjectRoutineEvent(m_doc.TripProject, TripProjectRoutineEvent.PROJECT_SAVED_EVENT));
        }

        private void ReportFramesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            showReportFrames();
        }

        private void bnwToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FirePalleteChangedEvent(new PalleteChangedEvent(PalleteChangedEvent.PAL_BNW));
        }

        private void ferrumToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FirePalleteChangedEvent(new PalleteChangedEvent(PalleteChangedEvent.PAL_FERRUM));
        }

        private void fullColorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FirePalleteChangedEvent(new PalleteChangedEvent(PalleteChangedEvent.PAL_FULLCOLOR));
        }

        public virtual void FirePalleteChangedEvent(PalleteChangedEvent e)
        {
            EventHandler<PalleteChangedEvent> handler = PalleteChangedHandler;
            if (handler != null)
            {
                handler(this, e);
            }
        }

        private void areasToolStripMenuItem_Click(object sender, EventArgs e)
        {
            showAreas();
        }

        public void EquListLoadedEventFired(object sender, EquListLoadedEvent e)
        {
            m_equipmentList.SetList(e.EquipmentList);
            
        }
        public void DrawTrackControlEventFired(object sender, Equipment.RefreshEquip e)
        {
            m_trackPanel.DrawTrackControl(e); 
        }
        public void FiredTransformTrackEvent(object sender, Equipment.TrasformTrackEvent e)
        {
            m_trackPanel.callTransformTrack(e);
        }

        private void equTreeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if(dataBaseEnable)
                showEquTree();
            else
                MessageBox.Show("База данных не загружена", "Информация", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void MainWindow_FormClosing(object sender, FormClosingEventArgs e)
        {
            FinishAll();
        }

        private void diagnosticToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ThermoJournalReportDlg dlg = new ThermoJournalReportDlg();
            dlg.ShowDialog();
        }

        private void makeReportToolStripMenuItem_Click(object sender, EventArgs e)
        {
            GraphJournalReportDlg dlg = new GraphJournalReportDlg();
            dlg.ShowDialog();
        }

        private void MainWindow_KeyUp(object sender, KeyEventArgs e)
        {
            if(m_doc == null)
                return;

            switch (e.KeyCode)
            {
                case (Keys.Space):
                    {
                //        m_doc.PausePlay();
                        break;
                    }
                case (Keys.Down):
                    {
                        m_doc.SpeedFactorDecrease();
                        break;
                    }
                case (Keys.Up):
                    {
                        m_doc.SpeedFactorIncrease();
                        break;
                    }
                case (Keys.Left):
                    {
                        m_doc.ShowFrame(true);
                        break;
                    }
                case (Keys.Right):
                    {
                        m_doc.ShowFrame();
                        break;
                    }
            }
        }

        private void addMovieTransitFilesMenuItem_Click(object sender, EventArgs e)
        {
            if (m_doc == null || m_doc.TripProject == null)
                return;

            OpenFileDialog ofd = new OpenFileDialog();

            ofd.InitialDirectory = Application.UserAppDataPath;
            ofd.Filter = "IRB-файлы (*.irb)|*.irb";//|All files (*.*)|*.*";
            ofd.FilterIndex = 1;
            ofd.Multiselect = true;
            ofd.RestoreDirectory = true;

            if (ofd.ShowDialog() == DialogResult.OK)
            {
                m_doc.TripProject.addTermoFiles(ofd.FileNames);
            }
        }

        private void programSettingsMenuItem_Click(object sender, EventArgs e)
        {
            ProgramSettings settingsDlg = new ProgramSettings();
            if (m_doc != null)
            {
                m_doc.connect_change_setting_events(settingsDlg);
            }

            settingsDlg.ShowDialog();

        }
        public delegate void d_statusChange(string data);

        private void db_loading_progress(object e, DB.LoadingProgressEvent args)
        {
            DB_Loader_backgroundWorker.ReportProgress(args.percent);
        }


        private void DB_Loader_backgroundWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            dataBaseEnable = false;
            DB.metro_db_controller.LoadingProgressChanged += db_loading_progress;

            Thread.Sleep(200);
            BeginInvoke(statusChange, new object[] { "Соединение с Базой данных" });
            try
            {
                db_manager = new DB.metro_db_controller(null);
                BeginInvoke(statusChange, new object[] { "База данных подключена" });
                dataBaseEnable = true;
            }
            catch (Exception exception)
            {
                DB.metro_db_controller.LoadingProgressChanged -= db_loading_progress;
                db_manager = null;
                BeginInvoke(statusChange, new object[] { "Ошибка Базы данных" });
                MessageBox.Show(exception.Message, "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            BeginInvoke(new EventHandler(delegate { createComponentDBDepend(); }));
            m_trackPanel.VisibleChanged += m_trackPanel_VisibleChanged;
            m_trackPanel.HideOnClose = true;
            
        }
        public void createComponentDBDepend()
        {
            if (m_equipMonitor == null)
            {
                m_equipMonitor = new EquipmentMonitor(db_manager);
                m_equipMonitor.ProcessEquipObj.DrawTrackControlEventHandler += DrawTrackControlEventFired;
                m_equipMonitor.ProcessEquipObj.TrasformTrackHandler += FiredTransformTrackEvent;
                m_trackPanel.trackScaleEventHandler += m_equipMonitor.setTrackScaleEventHandler;
                m_trackPanel.setDBController( db_manager);
            }
            if (m_equTree == null)
            {
                m_equTree = new AllEquipmentTree2(db_manager, dockPanel);
                m_equTree.VisibleChanged += m_equTree_VisibleChanged;
                m_equTree.HideOnClose = true;
            }
        }
    }

    public class TripProjectRoutineEvent : EventArgs
    {

        TripProject m_tripProject = null;
        int m_eventType = -1;

        public const int PROJECT_NO_EVENT = -1;
        public const int PROJECT_OPENED_EVENT = 0;
        public const int PROJECT_SAVED_EVENT = 1;
        public const int PROJECT_SYNCED_EVENT = 2;
        public const int PROJECT_CLOSED_EVENT = 3;

        public TripProjectRoutineEvent(TripProject tripProject, int eventType)
            : base()
        {
            m_tripProject = tripProject;
            m_eventType = eventType;
        }

        public int EventType { get { return m_eventType; } set { m_eventType = value; } }
        public TripProject TripProject { get { return m_tripProject; } set { m_tripProject = value; } }
    }

    public class PalleteChangedEvent : EventArgs
    {
        private String m_fileName = null;

        public const String PAL_BNW = "";
        public const String PAL_FERRUM = ".\\PAL\\IRON.PAL";
        public const String PAL_FULLCOLOR = ".\\PAL\\RAIN.PAL";

        public PalleteChangedEvent(String filePath)
            : base()
        {
            m_fileName = filePath;
        }

        public String FileName { get { return m_fileName; } set { m_fileName = value; } }
    }
}
