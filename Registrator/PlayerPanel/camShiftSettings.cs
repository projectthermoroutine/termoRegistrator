using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Registrator
{
    public partial class camShiftSettings : Form
    {
        public decimal cur_camera_offset;
        public decimal camera_offset;
        public camShiftSettings()
        {
            InitializeComponent();

            numericUpDown1.Enabled = false;
            b_saveToFile.Enabled = false;
            cur_camera_offset = Properties.Settings.Default.camera_offset;
            camera_offset = Properties.Settings.Default.camera_offset;
            numericUpDown2.Value = camera_offset;
            numericUpDown1.Value = cur_camera_offset;
        }

        private void b_apply_Click(object sender, EventArgs e)
        {
            Properties.Settings.Default.current_camera_offset = (long)numericUpDown1.Value;
        }

        private void b_cancel_Click(object sender, EventArgs e)
        {
            Dispose();
            Close();
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked)
            {
                numericUpDown1.Enabled = true;
                numericUpDown2.Enabled = true;
            }
            else
            {
                numericUpDown1.Enabled = false;
                numericUpDown2.Enabled = false;
            }
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDown1.Value != cur_camera_offset)
                b_apply.Enabled = true;
            else
                b_apply.Enabled = false;
        }

        private void b_saveToFile_Click(object sender, EventArgs e)
        {
            Properties.Settings.Default.camera_offset = (long)numericUpDown2.Value;
        }

        private void numericUpDown2_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDown2.Value != camera_offset)
                b_saveToFile.Enabled = true;
            else
                b_saveToFile.Enabled = false;
        }
    }
}
