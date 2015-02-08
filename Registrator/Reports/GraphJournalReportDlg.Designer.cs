namespace Registrator
{
    partial class GraphJournalReportDlg
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            Microsoft.Reporting.WinForms.ReportDataSource reportDataSource1 = new Microsoft.Reporting.WinForms.ReportDataSource();
            this.reportViewer1 = new Microsoft.Reporting.WinForms.ReportViewer();
            this.bindingSource1 = new System.Windows.Forms.BindingSource(this.components);
            this.ThermoJournalReportDlgBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.GraphJournalReportDlgBindingSource = new System.Windows.Forms.BindingSource(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.bindingSource1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.ThermoJournalReportDlgBindingSource)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.GraphJournalReportDlgBindingSource)).BeginInit();
            this.SuspendLayout();
            // 
            // reportViewer1
            // 
            this.reportViewer1.Dock = System.Windows.Forms.DockStyle.Fill;
            reportDataSource1.Name = "DataSet1";
            reportDataSource1.Value = this.bindingSource1;
            this.reportViewer1.LocalReport.DataSources.Add(reportDataSource1);
            this.reportViewer1.LocalReport.ReportEmbeddedResource = "Registrator.Report3.rdlc";
            this.reportViewer1.Location = new System.Drawing.Point(0, 0);
            this.reportViewer1.Name = "reportViewer1";
            this.reportViewer1.Size = new System.Drawing.Size(849, 467);
            this.reportViewer1.TabIndex = 0;
            // 
            // ThermoJournalReportDlgBindingSource
            // 
            this.ThermoJournalReportDlgBindingSource.DataMember = "Rows";
            this.ThermoJournalReportDlgBindingSource.DataSource = typeof(Registrator.ThermoJournalReportDlg);
            // 
            // GraphJournalReportDlgBindingSource
            // 
            this.GraphJournalReportDlgBindingSource.DataMember = "Rows";
            this.GraphJournalReportDlgBindingSource.DataSource = typeof(Registrator.GraphJournalReportDlg);
            // 
            // GraphJournalReportDlg
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(849, 467);
            this.Controls.Add(this.reportViewer1);
            this.Name = "GraphJournalReportDlg";
            this.Text = "График устранения дефектов электрооборудования";
            this.Load += new System.EventHandler(this.GraphJournalReportDlg_Load);
            ((System.ComponentModel.ISupportInitialize)(this.bindingSource1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.ThermoJournalReportDlgBindingSource)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.GraphJournalReportDlgBindingSource)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Microsoft.Reporting.WinForms.ReportViewer reportViewer1;
        private System.Windows.Forms.BindingSource ThermoJournalReportDlgBindingSource;
        private System.Windows.Forms.BindingSource bindingSource1;
        private System.Windows.Forms.BindingSource GraphJournalReportDlgBindingSource;
    }
}