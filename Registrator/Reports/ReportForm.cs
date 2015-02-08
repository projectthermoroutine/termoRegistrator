using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Collections;

namespace Registrator
{
    public partial class ReportForm : Form
    {

        //TempReportDataSet m_dataSet = null;

        ArrayList m_rows = new ArrayList();

        public ReportForm()
        {
            InitializeComponent();
        }

        public ReportForm(ArrayList rows) 
        {
            InitializeComponent();

            m_rows = rows;

        }

        private void ReportForm_Load(object sender, EventArgs e)
        {
            TempReportDataBindingSource.DataSource = m_rows;
            this.reportViewer1.RefreshReport();
        }

    }
}
