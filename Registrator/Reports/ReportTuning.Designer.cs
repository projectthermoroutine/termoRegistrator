namespace Registrator
{
    partial class ReportTuning
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
            this.dateFrom = new System.Windows.Forms.DateTimePicker();
            this.fromAvailable = new System.Windows.Forms.CheckBox();
            this.toAvailable = new System.Windows.Forms.CheckBox();
            this.dateTo = new System.Windows.Forms.DateTimePicker();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.chart = new System.Windows.Forms.CheckBox();
            this.table = new System.Windows.Forms.CheckBox();
            this.frame = new System.Windows.Forms.CheckBox();
            this.obj = new System.Windows.Forms.CheckBox();
            this.delta = new System.Windows.Forms.CheckBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.state = new System.Windows.Forms.CheckBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.cancelButton = new System.Windows.Forms.Button();
            this.acceptButton = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // dateFrom
            // 
            this.dateFrom.Enabled = false;
            this.dateFrom.Location = new System.Drawing.Point(180, 20);
            this.dateFrom.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.dateFrom.Name = "dateFrom";
            this.dateFrom.Size = new System.Drawing.Size(194, 26);
            this.dateFrom.TabIndex = 1;
            // 
            // fromAvailable
            // 
            this.fromAvailable.AutoSize = true;
            this.fromAvailable.Location = new System.Drawing.Point(9, 29);
            this.fromAvailable.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.fromAvailable.Name = "fromAvailable";
            this.fromAvailable.Size = new System.Drawing.Size(132, 24);
            this.fromAvailable.TabIndex = 6;
            this.fromAvailable.Text = "Дата начала";
            this.fromAvailable.UseVisualStyleBackColor = true;
            this.fromAvailable.CheckedChanged += new System.EventHandler(this.fromAvailable_CheckedChanged);
            // 
            // toAvailable
            // 
            this.toAvailable.AutoSize = true;
            this.toAvailable.Enabled = false;
            this.toAvailable.Location = new System.Drawing.Point(9, 65);
            this.toAvailable.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.toAvailable.Name = "toAvailable";
            this.toAvailable.Size = new System.Drawing.Size(157, 24);
            this.toAvailable.TabIndex = 8;
            this.toAvailable.Text = "Дата окончания";
            this.toAvailable.UseVisualStyleBackColor = true;
            this.toAvailable.CheckedChanged += new System.EventHandler(this.toAvailable_CheckedChanged);
            // 
            // dateTo
            // 
            this.dateTo.Enabled = false;
            this.dateTo.Location = new System.Drawing.Point(180, 60);
            this.dateTo.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.dateTo.Name = "dateTo";
            this.dateTo.Size = new System.Drawing.Size(194, 26);
            this.dateTo.TabIndex = 7;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.fromAvailable);
            this.groupBox1.Controls.Add(this.toAvailable);
            this.groupBox1.Controls.Add(this.dateFrom);
            this.groupBox1.Controls.Add(this.dateTo);
            this.groupBox1.Location = new System.Drawing.Point(15, 18);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox1.Size = new System.Drawing.Size(396, 102);
            this.groupBox1.TabIndex = 9;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Период";
            // 
            // chart
            // 
            this.chart.AutoSize = true;
            this.chart.Location = new System.Drawing.Point(9, 29);
            this.chart.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.chart.Name = "chart";
            this.chart.Size = new System.Drawing.Size(93, 24);
            this.chart.TabIndex = 10;
            this.chart.Text = "График";
            this.chart.UseVisualStyleBackColor = true;
            this.chart.CheckedChanged += new System.EventHandler(this.chart_CheckedChanged);
            // 
            // table
            // 
            this.table.AutoSize = true;
            this.table.Location = new System.Drawing.Point(172, 29);
            this.table.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.table.Name = "table";
            this.table.Size = new System.Drawing.Size(98, 24);
            this.table.TabIndex = 11;
            this.table.Text = "Таблица";
            this.table.UseVisualStyleBackColor = true;
            this.table.CheckedChanged += new System.EventHandler(this.table_CheckedChanged);
            // 
            // frame
            // 
            this.frame.AutoSize = true;
            this.frame.Location = new System.Drawing.Point(9, 29);
            this.frame.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.frame.Name = "frame";
            this.frame.Size = new System.Drawing.Size(111, 24);
            this.frame.TabIndex = 13;
            this.frame.Text = "По снимку";
            this.frame.UseVisualStyleBackColor = true;
            this.frame.CheckedChanged += new System.EventHandler(this.frame_CheckedChanged);
            // 
            // obj
            // 
            this.obj.AutoSize = true;
            this.obj.Location = new System.Drawing.Point(9, 65);
            this.obj.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.obj.Name = "obj";
            this.obj.Size = new System.Drawing.Size(121, 24);
            this.obj.TabIndex = 14;
            this.obj.Text = "По объекту";
            this.obj.UseVisualStyleBackColor = true;
            this.obj.CheckedChanged += new System.EventHandler(this.obj_CheckedChanged);
            // 
            // delta
            // 
            this.delta.AutoSize = true;
            this.delta.Location = new System.Drawing.Point(172, 29);
            this.delta.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.delta.Name = "delta";
            this.delta.Size = new System.Drawing.Size(199, 24);
            this.delta.TabIndex = 15;
            this.delta.Text = "Разность температур";
            this.delta.UseVisualStyleBackColor = true;
            this.delta.Visible = false;
            this.delta.CheckedChanged += new System.EventHandler(this.delta_CheckedChanged);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.state);
            this.groupBox2.Controls.Add(this.frame);
            this.groupBox2.Controls.Add(this.delta);
            this.groupBox2.Controls.Add(this.obj);
            this.groupBox2.Location = new System.Drawing.Point(15, 122);
            this.groupBox2.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox2.Size = new System.Drawing.Size(396, 100);
            this.groupBox2.TabIndex = 16;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Данные в отчет";
            // 
            // state
            // 
            this.state.AutoSize = true;
            this.state.Location = new System.Drawing.Point(172, 60);
            this.state.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.state.Name = "state";
            this.state.Size = new System.Drawing.Size(142, 24);
            this.state.TabIndex = 16;
            this.state.Text = "Изменение ТС";
            this.state.UseVisualStyleBackColor = true;
            this.state.Visible = false;
            this.state.CheckedChanged += new System.EventHandler(this.state_CheckedChanged);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.chart);
            this.groupBox3.Controls.Add(this.table);
            this.groupBox3.Location = new System.Drawing.Point(15, 222);
            this.groupBox3.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox3.Size = new System.Drawing.Size(394, 74);
            this.groupBox3.TabIndex = 17;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Представление";
            // 
            // cancelButton
            // 
            this.cancelButton.Location = new System.Drawing.Point(297, 315);
            this.cancelButton.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(112, 35);
            this.cancelButton.TabIndex = 18;
            this.cancelButton.Text = "Отмена";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // acceptButton
            // 
            this.acceptButton.Enabled = false;
            this.acceptButton.Location = new System.Drawing.Point(177, 315);
            this.acceptButton.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.acceptButton.Name = "acceptButton";
            this.acceptButton.Size = new System.Drawing.Size(112, 35);
            this.acceptButton.TabIndex = 19;
            this.acceptButton.Text = "Применить";
            this.acceptButton.UseVisualStyleBackColor = true;
            this.acceptButton.Click += new System.EventHandler(this.acceptButton_Click);
            // 
            // ReportTuning
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(416, 351);
            this.Controls.Add(this.acceptButton);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.MaximizeBox = false;
            this.MaximumSize = new System.Drawing.Size(432, 403);
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(432, 373);
            this.Name = "ReportTuning";
            this.ShowIcon = false;
            this.Text = "Настройка отчета";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DateTimePicker dateFrom;
        private System.Windows.Forms.CheckBox fromAvailable;
        private System.Windows.Forms.CheckBox toAvailable;
        private System.Windows.Forms.DateTimePicker dateTo;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckBox chart;
        private System.Windows.Forms.CheckBox table;
        private System.Windows.Forms.CheckBox frame;
        private System.Windows.Forms.CheckBox obj;
        private System.Windows.Forms.CheckBox delta;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.Button acceptButton;
        private System.Windows.Forms.CheckBox state;
    }
}