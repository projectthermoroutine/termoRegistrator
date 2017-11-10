using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace Registrator
{
    public partial class ProjectProperties : Form
    {
        private bool wasErrors = false;
        private ProjectSettings _project_settings = new ProjectSettings();
        public ProjectProperties()
        {
            InitializeComponent();
            UpdateProjectName();
            UpdateIRBFolderText();

            numericUpDown1.Value = Properties.Settings.Default.camera_offset;
        }

        private void UpdateIRBFolderText()
        {
            _project_settings.FilesDirectory = IRBFolderBox.Text = _project_settings.ProjectDirectory + "\\" + ProjectSettingsDefault.files_folder_name;
        }

        private void UpdateProjectName()
        {
            ProjectNameTextBox.Text = ProjectSettingsDefault.name_prefix + "_" + ProjectDatePicker.Value.ToShortDateString();

        }

        private void ProjectNameTextBox_TextChanged(object sender, EventArgs e)
        {
            _project_settings.Name = ProjectNameTextBox.Text;
            UpdateProjectFolderText();
        }

        private void UpdateProjectFolderText()
        {
            string dir = ProjectSettings.lastProjectParentDirectory;
            dir += "\\" + _project_settings.Name;
            ProjectFolderBox.Text = _project_settings.ProjectDirectory = dir;
            UpdateIRBFolderText();
        }

        private void ProjectDatePicker_ValueChanged(object sender, EventArgs e)
        {
            UpdateProjectName();
        }

        private void ChangeProjectFolderButton_Click(object sender, EventArgs e)
        {
            folderBrowserDialog1.SelectedPath = ProjectSettings.lastProjectParentDirectory;
            DialogResult res = folderBrowserDialog1.ShowDialog();
            if (res == System.Windows.Forms.DialogResult.OK)
            {
                ProjectSettings.lastProjectParentDirectory = folderBrowserDialog1.SelectedPath;
                UpdateProjectFolderText();
            }
        }

        private void LeftRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            //if (LeftRadioButton.Checked) RightRadioButton.Checked = false;
        }

        private void RightRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            //if (RightRadioButton.Checked) LeftRadioButton.Checked = false;
        }

        private void IRBFolderButton_Click(object sender, EventArgs e)
        {
            folderBrowserDialog1.SelectedPath = ProjectSettings.lastFilesDirectory;
            DialogResult res = folderBrowserDialog1.ShowDialog();
            if (res == System.Windows.Forms.DialogResult.OK)
            {
                _project_settings.FilesDirectory = folderBrowserDialog1.SelectedPath;
                IRBFolderBox.Text = folderBrowserDialog1.SelectedPath;
            }
        }

        public string ProjectName
        {
            get
            {
                return _project_settings.Name;
            }
        }

        public string ProjectFolder
        {
            get
            {
                return _project_settings.ProjectDirectory;
            }
        }

        public string IRBFolder
        {
            get
            {
                return _project_settings.FilesDirectory;
            }
        }

        public bool CameraTurnedToLeftSide
        {
            get
            {
                //if (LeftRadioButton.Checked) return true;
                return false;
            }
        }

        public DateTime ProjectDate
        {
            get
            {
                return ProjectDatePicker.Value;
            }
        }

        public bool WasError
        {
            get
            {
                return wasErrors;
            }
        }

        private void CreateButton_Click(object sender, EventArgs e)
        {
            wasErrors = !_project_settings.CreateFolders();
            if (wasErrors)
                return;

            FireCamShiftChange(new eventCameraOffset((int)numericUpDown1.Value * 1000, false));

            DialogResult = System.Windows.Forms.DialogResult.OK;
            Hide();
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked == true)
                numericUpDown1.Enabled = true;
            else
                numericUpDown1.Enabled = false;
        }

        public event EventHandler<eventCameraOffset> camShiftSetHandler;

        public virtual void FireCamShiftChange(eventCameraOffset e)
        {
            EventHandler<eventCameraOffset> handler = camShiftSetHandler;

            if (handler != null)
            {
                handler(this, e);
            }
        }
     }

    struct transit_project_settings_t
    {
        public transit_project_settings_t(string name, string project_folder, string files_folder, TripProject.CameraDirections camera_side)
        {
            this.name = name;
            this.project_folder = project_folder;
            this.files_folder = files_folder;
            this.camera_side = camera_side;
        }
        public string name;
        public string project_folder;
        public string files_folder;
        public TripProject.CameraDirections camera_side;

        public bool CreateFolders()
        {
            DirectoryInfo info;

            if (!Directory.Exists(project_folder))
            {
                try
                {
                    info = Directory.CreateDirectory(project_folder);
                    if (!info.Exists)
                    {
                        MessageBox.Show("Невозможно создать каталог:\n", "Ошибка при создании каталога", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return false;
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Невозможно создать каталог:\n" + project_folder + "\nТекст ошибки: " + ex.ToString(), "Ошибка при создании каталога", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
            }

            try
            {
                if (!Directory.Exists(files_folder))
                {
                    info = Directory.CreateDirectory(files_folder);
                    if (!info.Exists)
                    {
                        MessageBox.Show("Невозможно создать каталог:\n", "Ошибка при создании каталога", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return false;
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Невозможно создать каталог:\n" + files_folder + "\nТекст ошибки: " + ex.ToString(), "Ошибка при создании каталога", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            return true;
        }


    };


    //public delegate string get_string_value_f();

    internal sealed class ProjectSettingsDefault
    {
        static public string name_prefix = "Transit";
        static public string files_folder_name = "Files";
        static public string gen_name() { return name_prefix + "_" + System.DateTime.Now.ToShortDateString(); }
        static public string project_directory { get { return Properties.Settings.Default.lastProjDir; } }
        static public string files_directory { get { return project_directory + "\\" + files_folder_name; } }
    }

    //internal sealed class ProjectSettingsDelegates
    //{
    //    public get_string_value_f gen_name = ProjectSettingsDelegatesDefault.gen_name;
    //}



    internal sealed class ProjectSettings
    {
        public ProjectSettings()
        {
            _transit_project_settings = new transit_project_settings_t("", "", "", TripProject.CameraDirections.Left);
        }

        public static string lastFilesDirectory { get { return Properties.Settings.Default.lastIRBDir; } set { Properties.Settings.Default.lastIRBDir = value; } }
        public static string lastProjectParentDirectory { get { return Properties.Settings.Default.lastProjDir; } set { Properties.Settings.Default.lastProjDir = value; } }

        public string Name { get { return _transit_project_settings.name; } set { _transit_project_settings.name = value; } }
        public string ProjectDirectory { get { return _transit_project_settings.project_folder; } set { _transit_project_settings.project_folder = value; } }
        public string FilesDirectory { get { return _transit_project_settings.files_folder; } set { _transit_project_settings.files_folder = value; } }

        public bool CreateFolders() { return _transit_project_settings.CreateFolders(); }

        private transit_project_settings_t _transit_project_settings;
    }


}
