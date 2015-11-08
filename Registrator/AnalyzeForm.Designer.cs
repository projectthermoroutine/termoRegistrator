namespace Registrator
{
    partial class AnalyzeForm
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
            this.label1 = new System.Windows.Forms.Label();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.analyzeButton = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            this.analyzeBgWorker = new System.ComponentModel.BackgroundWorker();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(87, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Выполнено: 0 %";
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(12, 31);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(564, 23);
            this.progressBar1.TabIndex = 1;
            // 
            // analyzeButton
            // 
            this.analyzeButton.Location = new System.Drawing.Point(211, 68);
            this.analyzeButton.Name = "analyzeButton";
            this.analyzeButton.Size = new System.Drawing.Size(75, 23);
            this.analyzeButton.TabIndex = 2;
            this.analyzeButton.Text = "Анализ";
            this.analyzeButton.UseVisualStyleBackColor = true;
            this.analyzeButton.Click += new System.EventHandler(this.analyzeButton_Click);
            // 
            // cancelButton
            // 
            this.cancelButton.Location = new System.Drawing.Point(300, 68);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 3;
            this.cancelButton.Text = "Закрыть";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // analyzeBgWorker
            // 
            this.analyzeBgWorker.WorkerReportsProgress = true;
            this.analyzeBgWorker.WorkerSupportsCancellation = true;
            this.analyzeBgWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.analyzeBgWorker_DoWork);
            this.analyzeBgWorker.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.analyzeBgWorker_ProgressChanged);
            this.analyzeBgWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.analyzeBgWorker_RunWorkerCompleted);
            // 
            // teplovizorDataSet1
            // 
            // AnalyzeForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(588, 100);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.analyzeButton);
            this.Controls.Add(this.progressBar1);
            this.Controls.Add(this.label1);
            this.Name = "AnalyzeForm";
            this.Text = "Автоматический анализ измерений";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.Button analyzeButton;
        private System.Windows.Forms.Button cancelButton;
        private System.ComponentModel.BackgroundWorker analyzeBgWorker;
        //private DB.teplovizorDataSet teplovizorDataSet1;
        //private DB.teplovizorDataSetTableAdapters.equipmentTableAdapter equipmentTableAdapter1;
        //private DB.teplovizorDataSetTableAdapters.equipment1TableAdapter equipment1TableAdapter1;
        //private DB.teplovizorDataSetTableAdapters.shotsTableAdapter shotsTableAdapter1;
    }
}