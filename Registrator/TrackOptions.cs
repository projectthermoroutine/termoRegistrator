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
    public partial class TrackOptions : Form
    {
        public TrackOptions()
        {
            InitializeComponent();
            long l = Properties.Settings.Default.TrackHalfVeiwSector;
            numericUpDown_trackLength.Value = Properties.Settings.Default.TrackHalfVeiwSector;
        }
     
        public long TrackLength= 0;
        private void button_OK_Click(object sender, EventArgs e)
        {
            Properties.Settings.Default.TrackHalfVeiwSector = (long)numericUpDown_trackLength.Value;
            TrackLength = (long)numericUpDown_trackLength.Value;
        }
        public void setTrackLengthHandler(EventHandler e)
        {
            button_OK.Click += e;
        }
        private void button_Cancel_Click(object sender, EventArgs e)
        {
            Dispose();
            Close();
        }
    }
}
