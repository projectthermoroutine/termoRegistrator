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
    public partial class ReportTuning : Form
    {
        public event EventHandler<ReportTunedEvent> ReportTunedHandler;

        public ReportTuning(DateTime DateFrom, DateTime DateTo)
        {
            InitializeComponent();

            dateFrom.Value = DateFrom;
            dateTo.Value = DateTo;
        }
        public ReportTuning()
        {
            InitializeComponent();

            //dateFrom.Value = null;
            //dateTo.Value = null;
        }
        public void ObjectNotSelected(bool notSelected = true)
        {
            obj.Checked = !notSelected;
            obj.Enabled = !notSelected;
        }

        public void DeltaNotSelected(bool notSelected = true)
        {
            delta.Checked = !notSelected;
            delta.Enabled = !notSelected;
        }

        public void SetDateFrom(DateTime from)
        {
            dateFrom.Value = from;
        }

        public void SetDateTo(DateTime to)
        {
            dateTo.Value = to;
        }

        private void fromAvailable_CheckedChanged(object sender, EventArgs e)
        {
            dateFrom.Enabled = fromAvailable.Checked;
            dateTo.Enabled = fromAvailable.Checked;
            toAvailable.Enabled = fromAvailable.Checked;
        }

        private void toAvailable_CheckedChanged(object sender, EventArgs e)
        {
            dateTo.Enabled = toAvailable.Checked;
        }

        public void AcceptButtonActualize()
        {
            acceptButton.Enabled = (frame.Checked || obj.Checked || delta.Checked || state.Checked) && (chart.Checked || table.Checked);
            chart.Enabled = true;
            if (!(frame.Checked || obj.Checked || delta.Checked) && state.Checked)
            {
                chart.Checked = false;
                chart.Enabled = false;
                table.Checked = true;
            }
        }

        private void frame_CheckedChanged(object sender, EventArgs e)
        {
            AcceptButtonActualize();
        }

        private void obj_CheckedChanged(object sender, EventArgs e)
        {
            AcceptButtonActualize();
        }

        private void delta_CheckedChanged(object sender, EventArgs e)
        {
            AcceptButtonActualize();
        }

        private void chart_CheckedChanged(object sender, EventArgs e)
        {
            AcceptButtonActualize();
        }

        private void table_CheckedChanged(object sender, EventArgs e)
        {
            AcceptButtonActualize();
        }

        private void acceptButton_Click(object sender, EventArgs e)
        {
            if (!((frame.Checked || obj.Checked || delta.Checked || state.Checked) && (chart.Checked || table.Checked)))
                return;

            ReportTunedEvent rte = new ReportTunedEvent();

            rte.FromDateNeeded = fromAvailable.Checked;
            rte.ToDateNeeded   = toAvailable.Checked;
            
            if (rte.FromDateNeeded)
                rte.DateFrom = dateFrom.Value;
            if (rte.ToDateNeeded)
                rte.DateTo = dateTo.Value;

            rte.FrameIsNeeded = frame.Checked;
            rte.ObjectIsNeeded = obj.Checked;
            rte.DeltaIsNeeded = delta.Checked;
            rte.ChartIsNeeded = chart.Checked;
            rte.TableIsNeeded = table.Checked;
            rte.StatesNeeded  = state.Checked;

            FireReportTunedEvent(rte);

            Close();
            Dispose();

        }

        public virtual void FireReportTunedEvent(ReportTunedEvent e)
        {
            EventHandler<ReportTunedEvent> handler = ReportTunedHandler;

            if (handler != null)
            {
                handler(this, e);
            }
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            Close();
            Dispose();
        }

        private void state_CheckedChanged(object sender, EventArgs e)
        {
            AcceptButtonActualize();
        }
    }

    public class ReportTunedEvent : EventArgs
    {
        DateTime m_dateFrom;
        DateTime m_dateTo;

        bool m_needFrom = false;
        bool m_needTo = false;

        bool m_needFrame = false;
        bool m_needObj = false;
        bool m_needDelta = false;
        bool m_needState = false;

        bool m_needChart = false;
        bool m_needTable = false;

        public ReportTunedEvent()
            : base()
        {   }

        public bool FromDateNeeded { get { return m_needFrom; } set { m_needFrom = value; } }
        public bool ToDateNeeded { get { return m_needTo; } set { m_needTo = value; } }
        public DateTime DateFrom { get { return m_dateFrom; } set { m_dateFrom = value; } }
        public DateTime DateTo { get { return m_dateTo; } set { m_dateTo = value; } }
        public bool FrameIsNeeded { get { return m_needFrame; } set { m_needFrame = value; } }
        public bool ObjectIsNeeded { get { return m_needObj; } set { m_needObj = value; } }
        public bool DeltaIsNeeded { get { return m_needDelta; } set { m_needDelta = value; } }
        public bool ChartIsNeeded { get { return m_needChart; } set { m_needChart = value; } }
        public bool TableIsNeeded { get { return m_needTable; } set { m_needTable = value; } }
        public bool StatesNeeded { get { return m_needState; } set { m_needState = value; } }
    }
}
