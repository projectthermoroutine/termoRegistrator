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
            
            numericUpDown_trackLength.Value = Properties.Settings.Default.TrackHalfVeiwSector*2;
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

            Properties.Settings.Default.TrackUpdateFrequency = (long)numUpDown_lengthOfUpdateTrack.Value;
            Properties.Settings.Default.TrackHalfVeiwSector = (long)numericUpDown_trackLength.Value;

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
            Properties.Settings.Default.TrackHalfVeiwSector = (long)numericUpDown_trackLength.Value/2;
        }

        private void numericUpDown_trackLength_ValueChanged(object sender, EventArgs e)
        {
    
        }

        private void TrackOptions_Load(object sender, EventArgs e)
        {

        }
    }
}
