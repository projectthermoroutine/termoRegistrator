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
    public partial class ThermoJournalReportDlg : Form
    {
        private ThermoJournalRow[] rows;

        public ThermoJournalRow[] Rows
        {
            get { return rows; }
            set { rows = value; }
        }

        

        public ThermoJournalReportDlg()
        {
            ThermoJournalRow row = new ThermoJournalRow();
            rows = new ThermoJournalRow[] { row };
            InitializeComponent();
            bindingSource1.DataSource = Rows;

            BindingSource src = new BindingSource();
            Microsoft.Reporting.WinForms.ReportDataSource reportDataSource2 = new Microsoft.Reporting.WinForms.ReportDataSource();

            //src.DataMember = "MainReportInfo";
            src.DataSource = Rows;// bindingSource1;// (ASKZClient.ReportsForm);
            reportDataSource2.Name = "DataSet1";
            reportDataSource2.Value = src;
            //reportViewer1.LocalReport.DataSources.Add(reportDataSource2);
            reportViewer1.RefreshReport();
        }

        private void ThermoJournalReportDlg_Load(object sender, EventArgs e)
        {

            this.reportViewer1.RefreshReport();
        }
    }
}
