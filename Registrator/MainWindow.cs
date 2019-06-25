﻿using System;
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
using System.Threading.Tasks;
using System.Runtime.Serialization.Formatters.Binary;
using NLog;
using Registrator.Equipment;
using ThermoRoutineLib;

namespace Registrator
{
    
    public partial class MainWindow : Form
    {
     //   public DB.metro_db_controller db_manager;

        private bool dataBaseEnable = true;
        private FramesPanel m_filmFrames = null;
        bool _needCreateFilmFramesPanel = false;
        private ProjectFilesPanel m_projectFiles = null;
        private TrackPanel m_trackPanel = null;
        private AreasPanel m_areasPanel = null;
        private PlayerPanel m_doc = null;
        
        private DeserializeDockContent m_deserializeDockContent;
        private bool m_bSaveLayout = true;

        public event EventHandler<TripProjectRoutineEvent> TripProjectOpenedHandler;
        public event EventHandler<TripProjectRoutineEvent> TripProjectSavedHandler;
        public event EventHandler<TripProjectRoutineEvent> TripProjectSyncedHandler;
        public event EventHandler<TripProjectRoutineEvent> TripProjectClosedHandler;
        public event EventHandler<PalleteChangedEvent> PalleteChangedHandler;

        private AllEquipmentTree2 m_equTree = null;
        private EquipmentMonitor m_equipMonitor = null;

        private d_statusChange statusChange;

        Equipment.ProcessEquipment ProcessEquipObj = null;
        bool EquViewsActivatedAny = false;

        private void databaseStatus(string str)
        {
            toolStripStatusDataBaseLoad.Text = str;
        }

        static readonly NLog.Logger Log_ = LogManager.GetCurrentClassLogger();
        private StartupParams _startupParams;

        public MainWindow(StartupParams startupParams)
        {
            _startupParams = startupParams;
            KeyPreview = true;

            InitializeComponent();

            try
            {
                string current_directory = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                AppDomain.CurrentDomain.SetData("DataDirectory", current_directory);
                string image_path = current_directory + "\\icons\\mainTitleIcon.png";

                var bitmap = new Bitmap(image_path);
                var iconHandle = bitmap.GetHicon();
                this.Icon = System.Drawing.Icon.FromHandle(iconHandle);
            }
            catch(Exception e)
            {
                Log_.ErrorException("MainWindow", e);
            }

            //var data_directory = AppDomain.CurrentDomain.GetData("DataDirectory");

            statusChange = new d_statusChange(databaseStatus);

            databaseStatus("База данных подключена");

            m_deserializeDockContent = new DeserializeDockContent(GetContentFromPersistString);

            if (_startupParams.auto)
            {
                startPlayerPanelAsync();
            }
        }
        
        ~MainWindow()
        { }

        void startPlayerPanelAsync()
        {
            Task.Factory.StartNew(() =>
            {
                while(!Created && !Disposing && !IsDisposed)
                    Thread.Sleep(200);

                if (Disposing || IsDisposed)
                    return;

                var project_name = ProjectSettingsDefault.gen_name();
                var project_dir_path = Path.Combine(ProjectSettingsDefault.project_directory, project_name);

                var settings = new transit_project_settings_t(
                                                                project_name,
                                                                project_dir_path,
                                                                Path.Combine(project_dir_path, ProjectSettingsDefault.files_folder_name),
                                                                TripProject.CameraDirections.Left
                                                              );
                settings.CreateFolders();
                BeginInvoke(new EventHandler(delegate { NewProjectCreate(settings); }));

            });
        }


        void ProcessFrameInfo(_irb_frame_info frameInfo)
        {
            if (EquViewsActivatedAny && ProcessEquipObj != null)
            {
                bool path_changed = ProcessEquipObj.ProcessFrame(frameInfo);
                m_equipMonitor?.ProcessFrameCoordinate(frameInfo.coordinate, path_changed);
            }
        }

        IDockContent createEquipmentMonitor()
        {
            if(m_equipMonitor == null)
            {
                if(ProcessEquipObj == null)
                    ProcessEquipObj = new Equipment.ProcessEquipment();

                m_equipMonitor = new EquipmentMonitor(ProcessEquipObj.DbController);
                m_equipMonitor.VisibleChanged += m_equipmentList_VisibleChanged;
                m_equipMonitor.HideOnClose = true;
                m_equipMonitor.Shown += (s, e) => { EquToolStripMenuItem.Checked = true; EquViewsActivatedAny = true; };
            }

            return m_equipMonitor;
        }

        IDockContent createAllEquipmentCtrl()
        {
            if (m_equTree == null)
            {
                m_equTree = new AllEquipmentTree2(dockPanel);
                m_equTree.VisibleChanged += m_equTree_VisibleChanged;
                m_equTree.HideOnClose = true;

                m_equTree.Shown += (s, e) => equTreeToolStripMenuItem.Checked = true;
            }

            return m_equTree;
        }
        IDockContent createTrackPanel()
        {
            if (m_trackPanel == null)
            {
                if (ProcessEquipObj == null)
                    ProcessEquipObj = new Equipment.ProcessEquipment();

                m_trackPanel = new TrackPanel(ProcessEquipObj.DbController);
                m_trackPanel.VisibleChanged += m_trackPanel_VisibleChanged;
                m_trackPanel.HideOnClose = true;

                ProcessEquipObj.DrawTrackControlEventHandler += DrawTrackControlEventFired;
                ProcessEquipObj.TrasformTrackHandler += FiredTransformTrackEvent;

                m_trackPanel.trackScaleEventHandler += setTrackScaleEventHandler;

                m_trackPanel.Shown += (s, e) => {TrackToolStripMenuItem.Checked = true; EquViewsActivatedAny = true;};
            }

            return m_trackPanel;
        }

        void setTrackScaleEventHandler(object sender, TrackPanel.TrackScaleEventArgs e)
        {
            ProcessEquipObj.updateLengthOfViewedTrack(e.ZoomCoefficient);
        }


        IDockContent createAreasPanel()
        {
            if (m_areasPanel == null)
            {
                m_areasPanel = new AreasPanel();
                m_areasPanel.VisibleChanged += new EventHandler(m_areasPanel_VisibleChanged);
                m_areasPanel.HideOnClose = true;

                m_areasPanel.Shown += (s, e) => areasToolStripMenuItem.Checked = true;

            }

            return m_areasPanel;
        }

        IDockContent createProjectFilesPanel()
        {
            if (m_projectFiles == null)
            {
                m_projectFiles = new ProjectFilesPanel();
                m_projectFiles.VisibleChanged += new EventHandler(m_projectFiles_VisibleChanged);
                m_projectFiles.HideOnClose = true;

                m_projectFiles.Shown += (s, e) => FilesToolStripMenuItem.Checked = true;

            }

            return m_projectFiles;
        }

        IDockContent createFilmFramesPanel()
        {
            if (m_doc != null && m_filmFrames == null)
            {
                m_filmFrames = new FramesPanel(m_doc.pointsInfoManager);
                m_filmFrames.VisibleChanged += new EventHandler(m_filmFrames_VisibleChanged);
                m_filmFrames.HideOnClose = true;

                m_filmFrames.Shown += (s, e) => ReportFramesToolStripMenuItem.Checked = true;

            }

            return m_filmFrames;
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
            if (m_equipMonitor.IsHidden)
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

        public void PlayerModeChanged(object sender, EventPlayerChangeMode e)
        {
            m_projectFiles?.setAnalyzeButtonVisibility(sender, e);
        }


        private void HideDocks()
        {
            m_areasPanel?.Hide();
            m_projectFiles?.Hide();
            m_trackPanel?.Hide();

            m_equTree?.Hide();
            m_equipMonitor?.Hide();

        }

        private void CloseDocks()
        {
            m_areasPanel?.Close();
            m_projectFiles?.Close();
            m_trackPanel?.Close();

            //m_equTree?.Close();
            m_equipMonitor?.Close();
        }

        private void CloseDoc()
        {
            if (m_doc != null)
            {
                m_doc.StopGrabbing();
                m_doc.Hide();
                PalleteChangedHandler -= m_doc.PalleteChangedEventFired;
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

                CloseDocks();
                m_doc.Close();

                //m_projectFiles.Close();
                m_areasPanel = null;
                m_projectFiles = null;

                EquViewsActivatedAny = false;

                if (m_trackPanel != null)
                {
                    m_trackPanel.VisibleChanged -= m_trackPanel_VisibleChanged;
                    ProcessEquipObj.DrawTrackControlEventHandler -= DrawTrackControlEventFired;
                    ProcessEquipObj.TrasformTrackHandler -= FiredTransformTrackEvent;
                    m_trackPanel.trackScaleEventHandler -= setTrackScaleEventHandler;

                    m_trackPanel = null;
                }

                if (m_equipMonitor != null)
                {
                    m_equipMonitor.VisibleChanged -= m_equipmentList_VisibleChanged;
                    m_equipMonitor = null;
                }

                ProcessEquipObj = null;

                //m_filmFrames.Close();
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

            NewProjectCreate(new transit_project_settings_t(dlg.ProjectName, dlg.ProjectFolder, dlg.IRBFolder, dlg.CameraTurnedToLeftSide ? TripProject.CameraDirections.Left : TripProject.CameraDirections.Right ));
        }

        private void NewProjectCreate(transit_project_settings_t transit_project_settings)
        {
            CloseDoc();

            m_doc = CreateNewDocument(transit_project_settings);

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

            if (_needCreateFilmFramesPanel)
            {
                _needCreateFilmFramesPanel = false;
                createFilmFramesPanel();
            }

            createAreasPanel();
            createProjectFilesPanel();

            m_doc.AreasControl = m_areasPanel;

            m_doc.FormClosing += MainForm_Closing;

            PalleteChangedHandler += m_doc.PalleteChangedEventFired;
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

            connect_player_errors();
            connect_pd_dispatcher_events();
            connect_grabber_dispatcher_events();
            connect_player_state_event();

            m_doc.Show(dockPanel);
        }

        private PlayerPanel CreateNewDocument(transit_project_settings_t transit_project_settings)
        {
            // add hander for set hide or visibile Analyze button

            PlayerPanel dummyDoc = new PlayerPanel(
                new DB.metro_db_controller(null), 
                cameraOffset,
                PlayerModeChanged, 
                transit_project_settings, 
                _startupParams,
                ProcessFrameInfo
                //new PlayerPanel.processFrameFunc(this.ProcessFrameInfo)
                );
            int count = 1;
            
            string text = "Проезд " + count.ToString();
            
            dummyDoc.Text = text;
            return dummyDoc;
        }

        //private PlayerPanel CreateNewDocument(string text, transit_project_settings_t transit_project_settings)
        //{
        //    PlayerPanel dummyDoc = new PlayerPanel(db_manager, cameraOffset, m_projectFiles.setAnalyzeButtonVisibility, transit_project_settings, _startupParams);
        //    dummyDoc.Text = text;
        //    return dummyDoc;
        //}

        private void showFilmFiles()
        {

            if (m_projectFiles == null)
                createProjectFilesPanel();

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
                    if(createFilmFramesPanel() != null)
                        m_filmFrames.Show(dockPanel, DockState.DockLeft | DockState.DockBottom);
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
            if (m_equipMonitor == null)
                createEquipmentMonitor();

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

        private void showTrack()
        {
            if (m_trackPanel == null)
                createTrackPanel();

            if (TrackToolStripMenuItem.Checked)
            {
                m_trackPanel.Show(dockPanel, DockState.DockBottom);
            }
            else
            {
                if (!m_trackPanel.IsHidden)
                    m_trackPanel.Hide();
            }
        }

        private void showAreas()
        {
            if (m_areasPanel == null)
                createAreasPanel();

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
                createAllEquipmentCtrl();

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

            ofd.InitialDirectory = Properties.Settings.Default.lastProjDir;
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

                m_doc = CreateNewDocument(new transit_project_settings_t(Path.GetFileNameWithoutExtension(ofd.FileName) , tp.FilePath, tp.IRBFilesPath, tp.CameraDirection));

                m_projectFiles.TripProject = tp;
                m_doc.TripProject = tp;
                InitializeDocument();
                m_doc.UpdateWithProjectFiles();

                saveToolStripMenuItem.Enabled = true;
                saveAsToolStripMenuItem.Enabled = true;

                FireTripProjectOpened(new TripProjectRoutineEvent(m_doc.TripProject, TripProjectRoutineEvent.PROJECT_OPENED_EVENT));

            }

        }

        private IDockContent GetContentFromPersistString(string persistString)
        {
            if (persistString == typeof(AreasPanel).ToString())
            {
                return createAreasPanel();
            }
            else if (persistString == typeof(EquipmentMonitor).ToString())
            {
                return createEquipmentMonitor();
            }
            else if (persistString == typeof(ProjectFilesPanel).ToString())
            {
                return createProjectFilesPanel();
            }
            else if (persistString == typeof(TrackPanel).ToString())
            {
                return createTrackPanel();
            }
            else if (persistString == typeof(FramesPanel).ToString())
            {
                _needCreateFilmFramesPanel = true;
                return createFilmFramesPanel();
            }
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

       //     CloseDocks();

        }

        private void MainForm_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            FinishAll();
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

            ofd.InitialDirectory = Properties.Settings.Default.lastProjDir;
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
            TripProjectOpenedHandler?.Invoke(this, e);
        }

        public void FireTripProjectSaved(TripProjectRoutineEvent e)
        {
            TripProjectSavedHandler?.Invoke(this, e);
        }

        public void FireTripProjectSynchronized(TripProjectRoutineEvent e)
        {
            TripProjectSyncedHandler?.Invoke(this, e);
        }

        public void FireTripProjectClosed(TripProjectRoutineEvent e)
        {
            TripProjectClosedHandler?.Invoke(this, e);
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

            string project_filename = m_doc.TripProject.FilePath + "\\" + fname;

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
            PalleteChangedHandler?.Invoke(this, e);
        }

        private void areasToolStripMenuItem_Click(object sender, EventArgs e)
        {
            showAreas();
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


        private void contentsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if(File.Exists(Application.StartupPath + "\\Help\\Help.chm"))
                Help.ShowHelp(this, "file://" + Application.StartupPath + "\\Help\\Help.chm");
            else
                MessageBox.Show("Не удается найти файл справки. Путь к файлу справки: " + Application.StartupPath + "\\Help\\Help.chm", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
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
