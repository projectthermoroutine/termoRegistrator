using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using Microsoft.Reporting.WinForms;

namespace Registrator.Reports
{
    public partial class ReportForm : Form
    {
        TempReportDataSet m_rows = new TempReportDataSet();
        ReportViewer rv;

        public ReportForm()
        {
            InitializeComponent();
        }

        public ReportForm(TempReportDataSet rows)
        {
            InitializeComponent();

            m_rows = rows;
        }
        
        public class TempReportData_
        {
            Int16 m_number = 1;

            public TempReportData_() 
                : base()
            {   }

            public Int16 Code {  get { return m_number; } set { m_number = value; } }
        }

        private void ReportForm_Load_1(object sender, EventArgs e)
        {
            ReportDataSource RDS = new ReportDataSource();
            //TempReportData_ tmp = new TempReportData_();
            //tmp.Code = 9;
            //Array ar = new TempReportData_[1] { tmp };

            RDS.Value = m_rows.Rows;
            RDS.Name = "DataSet1";

            reportViewer1.LocalReport.DataSources.Add(RDS);

            this.reportViewer1.RefreshReport();
            this.reportViewer1.RefreshReport();
        }
    }
}
