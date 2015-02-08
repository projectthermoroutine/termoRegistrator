namespace Registrator
{
    partial class AreaSettings
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
            this.areaName = new System.Windows.Forms.TextBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label4 = new System.Windows.Forms.Label();
            this.borderMin = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.borderAvrg = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.borderMax = new System.Windows.Forms.NumericUpDown();
            this.alarmMax = new System.Windows.Forms.CheckBox();
            this.alarmAvrg = new System.Windows.Forms.CheckBox();
            this.alarmMin = new System.Windows.Forms.CheckBox();
            this.cancelButton = new System.Windows.Forms.Button();
            this.saveButton = new System.Windows.Forms.Button();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.borderMin)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.borderAvrg)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.borderMax)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(83, 13);
            this.label1.TabIndex = 9;
            this.label1.Text = "Наименование";
            // 
            // areaName
            // 
            this.areaName.Location = new System.Drawing.Point(15, 25);
            this.areaName.Name = "areaName";
            this.areaName.Size = new System.Drawing.Size(284, 20);
            this.areaName.TabIndex = 10;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.borderMin);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.borderAvrg);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.borderMax);
            this.groupBox2.Controls.Add(this.alarmMax);
            this.groupBox2.Controls.Add(this.alarmAvrg);
            this.groupBox2.Controls.Add(this.alarmMin);
            this.groupBox2.Location = new System.Drawing.Point(15, 51);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(284, 95);
            this.groupBox2.TabIndex = 9;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Оповещение";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(234, 19);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(43, 13);
            this.label4.TabIndex = 15;
            this.label4.Text = "град. С";
            // 
            // borderMin
            // 
            this.borderMin.DecimalPlaces = 1;
            this.borderMin.Location = new System.Drawing.Point(183, 16);
            this.borderMin.Maximum = new decimal(new int[] {
            200,
            0,
            0,
            0});
            this.borderMin.Minimum = new decimal(new int[] {
            273,
            0,
            0,
            -2147483648});
            this.borderMin.Name = "borderMin";
            this.borderMin.Size = new System.Drawing.Size(44, 20);
            this.borderMin.TabIndex = 14;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(234, 68);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(43, 13);
            this.label3.TabIndex = 13;
            this.label3.Text = "град. С";
            // 
            // borderAvrg
            // 
            this.borderAvrg.DecimalPlaces = 1;
            this.borderAvrg.Location = new System.Drawing.Point(183, 65);
            this.borderAvrg.Maximum = new decimal(new int[] {
            200,
            0,
            0,
            0});
            this.borderAvrg.Minimum = new decimal(new int[] {
            273,
            0,
            0,
            -2147483648});
            this.borderAvrg.Name = "borderAvrg";
            this.borderAvrg.Size = new System.Drawing.Size(44, 20);
            this.borderAvrg.TabIndex = 12;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(234, 43);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(43, 13);
            this.label2.TabIndex = 11;
            this.label2.Text = "град. С";
            // 
            // borderMax
            // 
            this.borderMax.DecimalPlaces = 1;
            this.borderMax.Location = new System.Drawing.Point(183, 40);
            this.borderMax.Maximum = new decimal(new int[] {
            200,
            0,
            0,
            0});
            this.borderMax.Minimum = new decimal(new int[] {
            273,
            0,
            0,
            -2147483648});
            this.borderMax.Name = "borderMax";
            this.borderMax.Size = new System.Drawing.Size(44, 20);
            this.borderMax.TabIndex = 3;
            // 
            // alarmMax
            // 
            this.alarmMax.AutoSize = true;
            this.alarmMax.Location = new System.Drawing.Point(6, 42);
            this.alarmMax.Name = "alarmMax";
            this.alarmMax.Size = new System.Drawing.Size(171, 17);
            this.alarmMax.TabIndex = 2;
            this.alarmMax.Text = "Максимальная температура";
            this.alarmMax.UseVisualStyleBackColor = true;
            // 
            // alarmAvrg
            // 
            this.alarmAvrg.AutoSize = true;
            this.alarmAvrg.Location = new System.Drawing.Point(6, 65);
            this.alarmAvrg.Name = "alarmAvrg";
            this.alarmAvrg.Size = new System.Drawing.Size(137, 17);
            this.alarmAvrg.TabIndex = 1;
            this.alarmAvrg.Text = "Средняя температура";
            this.alarmAvrg.UseVisualStyleBackColor = true;
            // 
            // alarmMin
            // 
            this.alarmMin.AutoSize = true;
            this.alarmMin.Location = new System.Drawing.Point(6, 19);
            this.alarmMin.Name = "alarmMin";
            this.alarmMin.Size = new System.Drawing.Size(165, 17);
            this.alarmMin.TabIndex = 0;
            this.alarmMin.Text = "Минимальная температура";
            this.alarmMin.UseVisualStyleBackColor = true;
            // 
            // cancelButton
            // 
            this.cancelButton.Location = new System.Drawing.Point(224, 152);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 11;
            this.cancelButton.Text = "Отмена";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // saveButton
            // 
            this.saveButton.Location = new System.Drawing.Point(143, 152);
            this.saveButton.Name = "saveButton";
            this.saveButton.Size = new System.Drawing.Size(75, 23);
            this.saveButton.TabIndex = 12;
            this.saveButton.Text = "Сохранить";
            this.saveButton.UseVisualStyleBackColor = true;
            this.saveButton.Click += new System.EventHandler(this.saveButton_Click);
            // 
            // AreaSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(310, 183);
            this.Controls.Add(this.saveButton);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.areaName);
            this.Controls.Add(this.label1);
            this.MaximizeBox = false;
            this.MaximumSize = new System.Drawing.Size(318, 217);
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(318, 217);
            this.Name = "AreaSettings";
            this.ShowIcon = false;
            this.Text = "Настройка области";
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.borderMin)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.borderAvrg)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.borderMax)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox areaName;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown borderMin;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown borderAvrg;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown borderMax;
        private System.Windows.Forms.CheckBox alarmMax;
        private System.Windows.Forms.CheckBox alarmAvrg;
        private System.Windows.Forms.CheckBox alarmMin;
        private System.Windows.Forms.Button cancelButton;
        public System.Windows.Forms.Button saveButton;

    }
}