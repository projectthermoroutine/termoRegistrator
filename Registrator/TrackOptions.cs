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
            
            label_trackBar.Text = Convert.ToString(Properties.Settings.Default.TrackUpdateFrequency);
            trackBar_updateFreq.Value = (int)Properties.Settings.Default.TrackUpdateFrequency;
            numericUpDown_trackLength.Value = Properties.Settings.Default.TrackHalfVeiwSector*2/10;
        }
        public class TrackEventArgs : EventArgs
        {
            private ulong sample_frequencies;

            public TrackEventArgs(ulong sample_frequenciesArg)
            {
                sample_frequencies = sample_frequenciesArg;
            }
            public ulong sampleFreq
            {
                get { return sample_frequencies; }
            }
        }

        public event EventHandler<TrackEventArgs> trackEventHandler;

        public virtual void FireTrackEvent(TrackEventArgs e)
        {
            EventHandler<TrackEventArgs> handler = trackEventHandler;

            if(handler!=null)
            {
                handler(this, e);
            }
        }

        private void button_OK_Click(object sender, EventArgs e)
        {

            Properties.Settings.Default.TrackUpdateFrequency = (ulong)trackBar_updateFreq.Value;
            Properties.Settings.Default.TrackHalfVeiwSector = (ulong)numericUpDown_trackLength.Value*10;

            Dispose();
            Close();
        }

        private void button_Cancel_Click(object sender, EventArgs e)
        {
            Dispose();
            Close();
        }

        private void trackBar_updateFreq_Scroll(object sender, EventArgs e)
        {
            label_trackBar.Text = Convert.ToString(trackBar_updateFreq.Value);
            Properties.Settings.Default.TrackHalfVeiwSector = (ulong)numericUpDown_trackLength.Value*10/2;
        }

        private void numericUpDown_trackLength_ValueChanged(object sender, EventArgs e)
        {
    
        }

        private void TrackOptions_Load(object sender, EventArgs e)
        {

        }
    }
}
