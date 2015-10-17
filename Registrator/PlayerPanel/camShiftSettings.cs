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
        private decimal cur_camera_offset;
        private decimal camera_offset;
        private bool chkbox = false;

        public camShiftSettings(int camOffset_Arg, bool chkbox_Arg)
        {
            InitializeComponent();
            chkbox = chkbox_Arg;
            checkBox1.Checked = chkbox_Arg;
            numericUpDown1.Enabled = false;
            b_saveToFile.Enabled = false;
            cur_camera_offset = camOffset_Arg;
            camera_offset = Properties.Settings.Default.camera_offset / 1000;
            numericUpDown1.Value = cur_camera_offset;

            groupBox1.Controls.Add(checkBox1);
            checkBox1.Location = new Point(groupBox1.Left + 15, groupBox1.Top + 1);
            checkBox1.BringToFront();

        }

        private void b_apply_Click(object sender, EventArgs e)
        {
            FireCamShiftChange(new eventCameraOffset((int)numericUpDown1.Value, true));
            cur_camera_offset = numericUpDown1.Value;
            b_apply.Enabled = false;
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
                
                if (numericUpDown1.Value != cur_camera_offset)
                {
                    b_apply.Enabled = true;
                    b_saveToFile.Enabled = true;
                }
                else
                {
                    b_apply.Enabled = false;
                    b_saveToFile.Enabled = false;
                }
                //numericUpDown1.Value = cur_camera_offset;
            }
            else
            {
                //numericUpDown1.Value = camera_offset;
                numericUpDown1.Enabled = false;
                FireCameraShiftCheckedChange(new eventCameraOffset(false));
                b_apply.Enabled = false;
                b_saveToFile.Enabled = false;
            }
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            // apply
            if (numericUpDown1.Value != cur_camera_offset)
            {
                b_apply.Enabled = true;
                b_saveToFile.Enabled = true;
            }
            else
            {
                b_apply.Enabled = false;
                b_saveToFile.Enabled = false;
            }
        }
        private void b_saveToFile_Click_1(object sender, EventArgs e)
        {
            //Properties.Settings.Default.camera_offset = (long)numericUpDown1.Value;
            FireCamShiftChange(new eventCameraOffset((int)numericUpDown1.Value, false));
            b_saveToFile.Enabled = false;
        }

        public event EventHandler<eventCameraOffset> camShiftSetHandler;
        public event EventHandler<eventCameraOffset> cameraOffsetCheckedSetHandler;
        public virtual void FireCamShiftChange(eventCameraOffset e)
        {
            EventHandler<eventCameraOffset> handler = camShiftSetHandler;

            if (handler != null)
            {
                handler(this, e);
            }
        }
        public virtual void FireCameraShiftCheckedChange(eventCameraOffset e)
        {
            EventHandler<eventCameraOffset> handler = cameraOffsetCheckedSetHandler;

            if (handler != null)
            {
                handler(this, e);
            }
        }
        private void cancel_Click(object sender, EventArgs e)
        {
            numericUpDown1.Value = cur_camera_offset;
        }

        private void camShiftSettings_ResizeEnd(object sender, EventArgs e)
        {
            checkBox1.Location = new Point(groupBox1.Left + 15, groupBox1.Top);
        }
    }

}
