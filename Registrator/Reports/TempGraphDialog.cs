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
    public partial class TempGraphDialog : Form
    {
        private ThermoJournalRow[] rows;

        public ThermoJournalRow[] Rows
        {
            get { return rows; }
            set { rows = value; }
        }

        public TempGraphDialog()
        {
            ThermoJournalRow row1 = new ThermoJournalRow();
            row1.Object_condition = 2;
            ThermoJournalRow row2 = new ThermoJournalRow();
            row2.Object_condition = 2.2f;
            ThermoJournalRow row3 = new ThermoJournalRow();
            row3.Object_condition = 3;
            ThermoJournalRow row4 = new ThermoJournalRow();
            row4.Object_condition = 6;
            rows = new ThermoJournalRow[] { row1, row2, row3, row4 };
            
            InitializeComponent();
            bindingSource1.DataSource = Rows;
        }

        private void TempGraphDialog_Load(object sender, EventArgs e)
        {

            this.reportViewer1.RefreshReport();
        }
    }
}
