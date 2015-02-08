using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using ThermoRoutineLib;


namespace Registrator
{
    public partial class SynchroDialog : Form
    {

        public event EventHandler<SynchronizationCompletedEvent> SynchronizationCompletedEventHandler;

        protected MovieTransit m_tvHandler = null;

        protected String m_rzFilePath = null;

        public SynchroDialog()
        {
            InitializeComponent();
        }

        public SynchroDialog(MovieTransit tvHandler, String rzFilePath)
        {
            InitializeComponent();
            m_tvHandler = tvHandler;
            if (m_tvHandler != null)
            {
               // m_tvHandler.SynchroProgress += new _ITRWrapperEvents_SynchroProgressEventHandler(SynchroProgressFired); ;//  new _ITRWrapperEvents_SynchroProgressEventHandler(SynchroProgressFired);
               // startDateTime.Text = m_tvHandler.GetDateTimeString();
            } 
            m_rzFilePath = rzFilePath;
            if (m_rzFilePath != null)
            {
                //String startDateTime = null;
                //String startPoint = null;
                //String correction = null;
                //m_tvHandler.SynchroInit(m_rzFilePath, out startDateTime, out startPoint, out correction);
                //startDT.Text = (startDateTime != null) ? startDateTime : "";
                //startP.Text = (startPoint != null) ? startPoint : "";
                //crt.Text = (correction != null) ? correction : "";
            }
        }

        public void SynchroProgressFired(int curProgress, int fullProgress)
        {
            //syncThread.ReportProgress((int)(((float)curProgress / fullProgress) * 100));
            progressLabel.Text = (int)(((float)curProgress / fullProgress) * 100) + "%";// (int)(curProgress / fullProgress) * 100 + "%";
            progressLabel.Refresh();
            progressBar1.Value = (int)(((float)curProgress / fullProgress) * 100);// (int)(curProgress / fullProgress) * 100;
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
                if (m_rzFilePath != null)
                {
                    //String startDateTime = null;
                    //String startPoint = null;
                    //String correction = null;
                    //m_tvHandler.SynchroInit(m_rzFilePath, out startDateTime, out startPoint, out correction);
                    //startDT.Text = (startDateTime != null) ? startDateTime : "";
                    //startP.Text = (startPoint != null) ? startPoint : "";
                    //crt.Text = (correction != null) ? correction : "";
                }
            }
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
                if (m_tvHandler != null)
                {
                 //   m_tvHandler.SynchroProgress += new _ITRWrapperEvents_SynchroProgressEventHandler(SynchroProgressFired);//new _ITRWrapperEvents_SynchroProgressEventHandler(SynchroProgressFired);
                  //  startDateTime.Text = m_tvHandler.GetDateTimeString();
                } 
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            syncThread.CancelAsync();
            Close();
            Dispose();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            //if (m_tvHandler != null && m_rzFilePath != null)
            //{
            //    //Convert.ToSingle(textBox5.Text)
            //    m_tvHandler.Synchronize(m_rzFilePath, 0F, (byte)((checkBox1.Checked) ? 1 : 0));
            //}
            syncButton.Enabled = false;
            closeButton.Enabled = false;
            cancelButton.Enabled = true;
            syncThread.RunWorkerAsync();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            syncThread.CancelAsync();
            closeButton.Enabled = true;
            syncButton.Enabled = true;
        }

        private void syncThread_DoWork(object sender, DoWorkEventArgs e)
        {
            if (m_tvHandler != null && m_rzFilePath != null)
            {
                //Convert.ToSingle(textBox5.Text)
               // m_tvHandler.Synchronize(m_rzFilePath, Convert.ToSingle(cameraOffset.Value), (byte)((checkBox1.Checked) ? 1 : 0));
            }
        }

        private void syncThread_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            FireSynchronizationCompletedEvent(new SynchronizationCompletedEvent());
            closeButton.Enabled = true;
            cancelButton.Enabled = false;
            syncButton.Enabled = true;
        }

        private void FireSynchronizationCompletedEvent(SynchronizationCompletedEvent e)
        {
            EventHandler<SynchronizationCompletedEvent> handler = SynchronizationCompletedEventHandler;
            if (handler != null)
                handler(this, e);
        }

        private void syncThread_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            progressLabel.Text = e.ProgressPercentage+"%";//(int)(curProgress / fullProgress )*100 + "%";
            progressBar1.Value = e.ProgressPercentage;//(int)(curProgress / fullProgress)*100;
        }

    }

    public class SynchronizationCompletedEvent : EventArgs
    {

        public SynchronizationCompletedEvent()
            : base()
        {

        }

    }

}
