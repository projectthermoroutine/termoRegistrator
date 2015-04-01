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

        public ProjectProperties()
        {
            InitializeComponent();
            UpdateProjectName();
            UpdateIRBFolderText();
        }

        private void UpdateIRBFolderText()
        {
            IRBFolderBox.Text = ProjectFolderBox.Text + "\\Files";
            Properties.Settings.Default.lastIRBDir = IRBFolderBox.Text;
        }

        private void UpdateProjectName()
        {
            ProjectNameTextBox.Text = "Проезд_" + ProjectDatePicker.Value.ToShortDateString();

        }

        private void ProjectNameTextBox_TextChanged(object sender, EventArgs e)
        {
            UpdateProjectFolderText();
        }

        private void UpdateProjectFolderText()
        {
            string dir = Properties.Settings.Default.lastProjDir;
            dir += "\\" + ProjectNameTextBox.Text;
            ProjectFolderBox.Text = dir;
            UpdateIRBFolderText();
        }

        private void ProjectDatePicker_ValueChanged(object sender, EventArgs e)
        {
            UpdateProjectName();
        }

        private void ChangeProjectFolderButton_Click(object sender, EventArgs e)
        {
            folderBrowserDialog1.SelectedPath = Properties.Settings.Default.lastProjDir;
            DialogResult res = folderBrowserDialog1.ShowDialog();
            if (res == System.Windows.Forms.DialogResult.OK)
            {
                Properties.Settings.Default.lastProjDir = folderBrowserDialog1.SelectedPath;
                UpdateProjectFolderText();
            }
        }

        private void LeftRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            if (LeftRadioButton.Checked) RightRadioButton.Checked = false;
        }

        private void RightRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            if (RightRadioButton.Checked) LeftRadioButton.Checked = false;
        }

        private void IRBFolderButton_Click(object sender, EventArgs e)
        {
            folderBrowserDialog1.SelectedPath = Properties.Settings.Default.lastIRBDir;
            DialogResult res = folderBrowserDialog1.ShowDialog();
            if (res == System.Windows.Forms.DialogResult.OK)
            {
                Properties.Settings.Default.lastIRBDir = folderBrowserDialog1.SelectedPath;
                IRBFolderBox.Text = folderBrowserDialog1.SelectedPath;
            }
        }

        public string ProjectName
        {
            get
            {
                return ProjectNameTextBox.Text;
            }
        }

        public string ProjectFolder
        {
            get
            {
                return ProjectFolderBox.Text;
            }
        }

        public string IRBFolder
        {
            get
            {
                return IRBFolderBox.Text;
            }
        }

        public bool CameraTurnedToLeftSide
        {
            get
            {
                if (LeftRadioButton.Checked) return true;
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
            if (!Directory.Exists(ProjectFolder))
            {
                DirectoryInfo info;
                try
                {
                    info = Directory.CreateDirectory(ProjectFolder);
                    if (!info.Exists)
                    {
                        MessageBox.Show("Невозможно создать каталог:\n", "Ошибка при создании каталога", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        wasErrors = true;
                        return;
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Невозможно создать каталог:\n" + ProjectFolder + "\nТекст ошибки: " + ex.ToString(), "Ошибка при создании каталога", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    wasErrors = true;
                    return;
                }
                try
                {
                    if (!Directory.Exists(IRBFolder))
                    {
                        info = Directory.CreateDirectory(IRBFolder);
                        if (!info.Exists)
                        {
                            MessageBox.Show("Невозможно создать каталог:\n", "Ошибка при создании каталога", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            wasErrors = true;
                            return;
                        }
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Невозможно создать каталог:\n" + IRBFolder + "\nТекст ошибки: " + ex.ToString(), "Ошибка при создании каталога", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    wasErrors = true;
                    return;
                }
            }
            DialogResult = System.Windows.Forms.DialogResult.OK;
            Hide();
        }
     }
}
