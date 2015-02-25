using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace Registrator
{
    public partial class ProjectFilesPanel : ToolWindow
    {

        private TripProject m_tripProject = null;
        private bool _files_loaded;

        public event EventHandler<SynchroNeedEvent> SynchroNeedEventHandler;
        public event EventHandler<DesynchroNeedEvent> DesynchroNeedEventHandler;
        public event EventHandler<AnalizeEvent> AnalizeEventHandler;

        public ProjectFilesPanel()
        {
            _files_loaded = false;
            InitializeComponent();
        }

        public ProjectFilesPanel(TripProject tripProject)
            : this()
        {

            TripProject = tripProject;

        }

        public TripProject TripProject
        {

            get
            {
                return m_tripProject;
            }

            set
            {
                _files_loaded = false;
                if (m_tripProject != null)
                {
                    m_tripProject.TripProjectChangedHandler -= TripProjectChangedEventFired;
                    m_tripProject.FilesLoadedHandler -= FileLoadedEventAquired;
                }
                m_tripProject = value;
                m_tripProject.TripProjectChangedHandler += TripProjectChangedEventFired;
                m_tripProject.FilesLoadedHandler += FileLoadedEventAquired;
            }

        }


        public delegate void FillTheTreeDelegate();

        public void FillTheTree()
        {
            if (_files_loaded)
                return;

            if(InvokeRequired)
            {
                BeginInvoke(new FillTheTreeDelegate(FillTheTree));
                return;
            }

            fileTreeView.Nodes.Clear();

            if (m_tripProject == null)
                return;

            TreeNode root1 = new TreeNode("Файлы тепловизора");

            var file_names = m_tripProject.Files;
            for (int i = 0; i < file_names.Count; i++)
            {
                var node = new TreeNode(file_names[i]);
                node.Tag = file_names[i];
                root1.Nodes.Add(node);
            }

            fileTreeView.Nodes.Add(root1);
            
        }

        public delegate void UpdateTreeViewDelegate();
        public void UpdateTreeView()
        {
            if (InvokeRequired)
            {
                BeginInvoke(new UpdateTreeViewDelegate(UpdateTreeView));
                return;
            }

            if (m_tripProject == null)
                return;

            TreeNode root1 = fileTreeView.Nodes[0];

            var status_list = m_tripProject.filesLoadedStatus;
            if (status_list.Count == 0)
                return;

            _files_loaded = false;

            if (root1.Nodes.Count != status_list.Count)
            {
                FillTheTree();
                root1 = fileTreeView.Nodes[0];
            }

            var file_names = m_tripProject.Files;

            for (int i = 0; i < file_names.Count && i < root1.Nodes.Count; i++)
            {
                var node = root1.Nodes[i];
                bool add_tool_tip = true;
                var file_name = file_names[i];
                var node_text = file_name;
                var status = status_list[i];
                if (status == "OK")
                {
                    add_tool_tip = false;
                    node_text += " - Loaded";
                }
                else{
                    node_text += " - Error";
                }

                node.Text = node_text;
                node.Tag = file_name;
                if (add_tool_tip)
                    node.ToolTipText = status;

            }

            if (root1.Nodes.Count > 0)
                _files_loaded = true;

        }

        void FileLoadedEventAquired(object sender, EventArgs e)
        {
            UpdateTreeView();
        }


        public void FileFromGrabberEventFired(object sender, FileFromGrabberEvent e)
        {
            m_tripProject.addTermoFiles(new string[] { e.FilePath });
        }

        public void TripProjectChangedEventFired(object sender, EventArgs e)
        {
            _files_loaded = false;
            FillTheTree();
        }

        private void addButton_Click(object sender, EventArgs e)
        {

            OpenFileDialog ofd = new OpenFileDialog();

            ofd.InitialDirectory = Application.UserAppDataPath;
            ofd.Filter = "IRB-файлы (*.irb)|*.irb|RZ-файл (*.rz)|*.rz|All files (*.*)|*.*";
            ofd.FilterIndex = 1;
            ofd.Multiselect = true;
            ofd.RestoreDirectory = true;

            if (ofd.ShowDialog() == DialogResult.OK)
            {
                String[] files = ofd.FileNames;
                List<string> movie_transit_files = new List<string>();
                for (int i = 0; i < files.Length; i++ )
                {
                    if (files[i].EndsWith(".irb"))
                    {
                        movie_transit_files.Add(files[i]);
                    }
                }

                if (movie_transit_files.Count > 0)
                {
                    m_tripProject.addTermoFiles(movie_transit_files.ToArray());
                }
            }

        }

        private void toolStripButton3_Click(object sender, EventArgs e)
        {
            if(m_tripProject.TrackFile != null && !m_tripProject.TrackFile.Equals(""))
                FireSynchroNeedEvent(new SynchroNeedEvent(m_tripProject.TrackFile));
 
        }

        private void FireSynchroNeedEvent(SynchroNeedEvent e)
        {

            EventHandler<SynchroNeedEvent> handler = SynchroNeedEventHandler;
            if (handler != null)
                handler(this, e);

        }

        private void FireAnalizeEvent(AnalizeEvent e)
        {

            EventHandler<AnalizeEvent> handler = AnalizeEventHandler;

            if(handler != null)
            {

                handler(this, e);

            }

        }

        private void analizeButton_Click(object sender, EventArgs e)
        {

            FireAnalizeEvent(new AnalizeEvent());
        
        }

        private void fileTreeView_Click(object sender, EventArgs e)
        {
            delButton.Enabled = (fileTreeView.SelectedNode != null && fileTreeView.SelectedNode.Nodes.Count == 0);
        }

        private void delButton_Click(object sender, EventArgs e)
        {
            if(m_tripProject == null)
                return;

            if (fileTreeView.SelectedNode == null)
                return;
            var node = fileTreeView.SelectedNode;
            var node_tag = node.Tag;
            fileTreeView.Nodes.Remove(node);
            if (node_tag != null && node_tag.GetType() == "".GetType())
            {
                m_tripProject.deleteTermoFiles(new string[] { (string)node_tag });
            }
            
        }

        private void toolStripButton4_Click(object sender, EventArgs e)
        {
            FireDesynchroNeedEvent(new DesynchroNeedEvent());
        }

        private void FireDesynchroNeedEvent(DesynchroNeedEvent e)
        {
            EventHandler<DesynchroNeedEvent> handler = DesynchroNeedEventHandler;
            if (handler != null)
                handler(this, e);
        }
        

    }

    public class DesynchroNeedEvent : EventArgs
    {

        public DesynchroNeedEvent()
            : base()
        {

        }

    }

    public class SynchroNeedEvent : EventArgs
    {

        private String m_rzFilePath = null;

        public SynchroNeedEvent()
            : base()
        {

        }

        public SynchroNeedEvent(String rzFilePath)
            : base()
        {
            m_rzFilePath = rzFilePath;
        }

        public String RzFilePath
        {
            get
            {
                return m_rzFilePath;
            }

            set
            {
                m_rzFilePath = value;
            }
        }

    }

    public class AnalizeEvent : EventArgs
    {

        public AnalizeEvent()
            : base()
        {

        }

    }

}
