using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ThermoRoutineLib;
using System.Runtime.InteropServices;

namespace Registrator
{

    public partial class DesynchroDialog : Form
    {

        protected MovieTransit m_tvHandler = null;
        public event EventHandler<DesynchroCompletedEvent> DesynchroCompletedEventHandler;

        public DesynchroDialog()
        {
            InitializeComponent();
        }

        public DesynchroDialog(MovieTransit tvHandler)
            : this()
        {
            m_tvHandler = tvHandler;
            Start();
        }

        public MovieTransit TvHandler
        {
            get
            {
                return m_tvHandler;
            }

            set
            {
                m_tvHandler = value;
            }
        }

        public void Start(MovieTransit tvHandler = null)
        {

            if (tvHandler != null)
                m_tvHandler = tvHandler;

            if (m_tvHandler == null)
                return;

            desynchroThread.RunWorkerAsync();

        }

        private void desynchroThread_DoWork(object sender, DoWorkEventArgs e)
        {
            //int counter = 0;
            //byte moreFramesAvailable = 0;
            //short percentage = 0;
            //do
            //{
            //    m_tvHandler.DesyncFrame(ref counter, out moreFramesAvailable, out percentage);
            //    desynchroThread.ReportProgress(percentage);
            //}
            //while( moreFramesAvailable == 1);

        }

        private void desynchroThread_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {

            progressBar1.Value = e.ProgressPercentage;
            progressLabel.Text = e.ProgressPercentage + "%";

        }

        private void FireDesynchroCompletedEvent(DesynchroCompletedEvent e)
        {
            EventHandler<DesynchroCompletedEvent> handler = DesynchroCompletedEventHandler;
            if (handler != null)
                handler(this, e);
        }

        private void DesynchroDialog_Shown(object sender, EventArgs e)
        {
            Start();
        }

        private void closeButton_Click(object sender, EventArgs e)
        {
            Close();
            Dispose();
        }

        private void desynchroThread_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            closeButton.Enabled = true;
            FireDesynchroCompletedEvent(new DesynchroCompletedEvent());
        }

        private void DesynchroDialog_Load(object sender, EventArgs e)
        {

        }

    }

    public class DesynchroCompletedEvent : EventArgs
    {

        public DesynchroCompletedEvent()
            : base()
        {

        }

    }

}
