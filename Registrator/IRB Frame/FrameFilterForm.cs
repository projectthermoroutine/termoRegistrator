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
    public partial class FrameFilterForm : Form
    {

        public event EventHandler<FilteredEvent> FilteredEventHandler;

        protected MovieTransit m_tvHandler = null;

        int m_framesViewed = 0;
        int m_framesSelected = 0;

        byte[] m_filterMask = null;

        bool m_stopFlag = false;

        public FrameFilterForm()
        {
            InitializeComponent();
        }

        public FrameFilterForm(MovieTransit tvHandler)
        {
            InitializeComponent();
            m_tvHandler = tvHandler;
            InitFilter();
        }

        public void InitFilter()
        {

            if (m_tvHandler == null)
                return;

            float timeFromVal = 0f;
            float timeToVal = 0f; 
            float tempObjVal = 0f;
            float tempAreaVal = 0f;
            short pickFromVal = 0; 
            short pickToVal = 0;
            Int32 flags = 0x00;
            bool checkedOnly = false;

            bool res = m_tvHandler.InitFrameFilter(out timeFromVal
                                                  , out timeToVal
                                                  , out tempObjVal
                                                  , out tempAreaVal
                                                  , out pickFromVal
                                                  , out pickToVal
                                                  , out flags
                                                  , out checkedOnly);

            short hF = (short)(timeFromVal / 60);
            short mF = (short)(timeFromVal % 60);

            short hT = (short)(timeToVal / 60);
            short mT = (short)(timeToVal % 60);

            timeFrom.Value = Convert.ToDateTime(hF + " : " + mF);
            timeTill.Value = Convert.ToDateTime(hT + " : " + mT);

            objectTopMargin.Value = new Decimal(tempObjVal);
            areaTopMargin.Value = new Decimal(tempAreaVal);

            picketFrom.Value = new Decimal(pickFromVal);
            picketTill.Value = new Decimal(pickToVal);

            timeFromFlag.Checked = (flags & 0x01) == 0x01;
            timeToFlag.Checked = (flags & 0x02) == 0x02;

            tempFrameFlag.Checked = (flags & 0x08) == 0x08;
            tempAreaFlag.Checked = (flags & 0x10) == 0x10;

            picketFromFlag.Checked = (flags & 0x20) == 0x20;
            picketToFlag.Checked = (flags & 0x40) == 0x40;

            onlyCheckedFlag.Checked = checkedOnly;

        }

        private void closeButton_Click(object sender, EventArgs e)
        {
            Close();
            Dispose();
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

        public int ViewedFrames
        {
            get
            {
                return m_framesViewed;
            }
        }

        public int SelectedFrames
        {
            get
            {
                return m_framesSelected;
            }
        }

        private void Filter()
        {

            if (m_tvHandler == null)
               return;

            Int32 flags = 0x00;
            flags |= ((timeFromFlag.Checked) ? 0x01 : 0x00);
            flags |= ((timeToFlag.Checked) ? 0x02 : 0x00);
            flags |= ((onlyCheckedFlag.Checked) ? 0x04 : 0x00);
            flags |= ((tempFrameFlag.Checked) ? 0x08 : 0x00);
            flags |= ((tempAreaFlag.Checked) ? 0x10 : 0x00);
            flags |= ((picketFromFlag.Checked) ? 0x20 : 0x00);
            flags |= ((picketToFlag.Checked) ? 0x40 : 0x00);
            flags |= ((timeFromFlag.Checked) ? 0x01 : 0x00);

            try
            {
                m_tvHandler.SetFrameFilter(timeFrom.Value.Hour * 60 + timeFrom.Value.Minute
                                          , timeTill.Value.Hour * 60 + timeTill.Value.Minute
                                          , Decimal.ToSingle(objectTopMargin.Value)
                                          , Decimal.ToSingle(areaTopMargin.Value)
                                          , Decimal.ToInt16(picketFrom.Value)
                                          , Decimal.ToInt16(picketTill.Value)
                                          , flags);

                int framesNum = m_tvHandler.FramesCount();

                m_filterMask = new byte[framesNum];

                m_framesViewed = 0;

                m_framesSelected = 0;

                for (short i = 0; i < framesNum; i++)
                {
                    if (m_stopFlag)
                    {
                        SetViewedLabelText("0");
                        SetSelectedLabelText("0");
                        filterBgWorker.ReportProgress(0);
                        SetProgressLabelText("0");
                        return;
                    }

                    bool res = m_tvHandler.IsFrameMeetFilter(i);

                    m_framesViewed++;

                    m_filterMask[i] = 0;

                    if (res)
                    {
                        m_filterMask[i] = 1;
                        m_framesSelected++;
                    }
                    SetViewedLabelText(m_framesViewed.ToString());
                    SetSelectedLabelText(m_framesSelected.ToString());

                    int progress = m_framesViewed * 100 / framesNum;

                    filterBgWorker.ReportProgress(progress);

                    SetProgressLabelText(progress.ToString());

                }
            }
            catch (Exception exc)
            {
                var error = exc.Message;
            }

            FireFilteredEvent(new FilteredEvent(m_framesSelected, m_filterMask));

        }

        public delegate void SetViewedLabelTextDelegate(String txt);

        public void SetViewedLabelText(String txt)
        {
            if(InvokeRequired)
                BeginInvoke(new SetViewedLabelTextDelegate(SetViewedLabelText), new object[]{txt});
            else
                viewedLabel.Text = txt;
        }

        public delegate void SetSelectedLabelTextDelegate(String txt);

        public void SetSelectedLabelText(String txt)
        {
            if (InvokeRequired)
                BeginInvoke(new SetSelectedLabelTextDelegate(SetSelectedLabelText), new object[] { txt });
            else
                selectedLabel.Text = txt;
        }

        public delegate void SetProgressLabelTextDelegate(String txt);

        public void SetProgressLabelText(String txt)
        {
            if (InvokeRequired)
                BeginInvoke(new SetProgressLabelTextDelegate(SetProgressLabelText), new object[] { txt });
            else
                progressLabel.Text = txt+"%";
        }

        private void filterBgWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            if (m_tvHandler != null )
            {
                Filter();
            }
        }

        public void SetButtonsEnabled()
        {
            filterButton.Enabled = true;
            cancelButton.Enabled = false;
            resetButton.Enabled = true;
            closeButton.Enabled = true;
        }

        public void SetButtonsDisabled()
        {
            filterButton.Enabled = false;
            cancelButton.Enabled = true;
            resetButton.Enabled = false;
            closeButton.Enabled = false;
        }

        private void filterBgWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            progressBar.Value = e.ProgressPercentage;
        }

        private void filterBgWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            SetButtonsEnabled();
        }

        private void filterButton_Click(object sender, EventArgs e)
        {
            m_stopFlag = false;
            SetButtonsDisabled();
            filterBgWorker.RunWorkerAsync();
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            m_stopFlag = true;
        }

        public void ResetFilterParameters()
        {
            timeFromFlag.Checked = false;
            timeToFlag.Checked = false;
            tempFrameFlag.Checked = false;
            tempAreaFlag.Checked = false;
            picketFromFlag.Checked = false;
            picketToFlag.Checked = false;
        }

        public void SetAllFilterMask(byte flag = 1)
        {
            if (m_filterMask == null)
                return;
            for (int i = 0; i < m_filterMask.Length; i++)
                m_filterMask[i] = flag;

        }

        public void ResetFilter()
        {
            ResetFilterParameters();
            
            if (m_tvHandler == null)
                return;

            int framesNum = m_tvHandler.FramesCount();

            if (framesNum < 1)
                return;

            m_filterMask = new byte[framesNum];

            SetAllFilterMask(1);

            FireFilteredEvent(new FilteredEvent(framesNum, m_filterMask));

        }

        private void resetButton_Click(object sender, EventArgs e)
        {
            if(DialogResult.Yes == MessageBox.Show("Будут сброшены параметры фильтра и результаты фильтрации!\nПродолжить?", "Внимание!", MessageBoxButtons.YesNo, MessageBoxIcon.Warning))
                ResetFilterParameters();
        }

        private void timeFromFlag_CheckedChanged(object sender, EventArgs e)
        {
            timeFrom.Enabled = timeFromFlag.Checked;
        }

        private void timeToFlag_CheckedChanged(object sender, EventArgs e)
        {
            timeTill.Enabled = timeToFlag.Checked;
        }

        private void tempFrameFlag_CheckedChanged(object sender, EventArgs e)
        {
            objectTopMargin.Enabled = tempFrameFlag.Checked;
        }

        private void tempAreaFlag_CheckedChanged(object sender, EventArgs e)
        {
            areaTopMargin.Enabled = tempAreaFlag.Checked;
        }

        private void picketFromFlag_CheckedChanged(object sender, EventArgs e)
        {
            picketFrom.Enabled = picketFromFlag.Checked;
        }

        private void picketToFlag_CheckedChanged(object sender, EventArgs e)
        {
            picketTill.Enabled = picketToFlag.Checked;
        }

        private void FireFilteredEvent(FilteredEvent e)
        {
            EventHandler<FilteredEvent> handler = FilteredEventHandler;
            if (handler != null)
                handler(this, e);
        }

    }

    public class FilteredEvent : EventArgs
    {

        int m_framesToDisplay = 0;
        byte[] m_filterMask = null;

        public FilteredEvent()
            : base()
        {

        }

        public FilteredEvent(int framesToDisplay, byte[] filterMask)
            : this()
        {
            m_framesToDisplay = framesToDisplay;
            m_filterMask = filterMask;
        }

        public byte[] FilterMask
        {
            get
            {
                return m_filterMask;
            }

            set
            {
                m_filterMask = value;
            }
        }

        public int FramesToDisplay
        {
            get
            {
                return m_framesToDisplay;
            }

            set
            {
                m_framesToDisplay = value;
            }
        }

    }

}
