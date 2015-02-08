namespace Registrator
{
    partial class FrameFilterForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrameFilterForm));
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.timeToFlag = new System.Windows.Forms.CheckBox();
            this.timeFromFlag = new System.Windows.Forms.CheckBox();
            this.timeTill = new System.Windows.Forms.DateTimePicker();
            this.timeFrom = new System.Windows.Forms.DateTimePicker();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.tempAreaFlag = new System.Windows.Forms.CheckBox();
            this.tempFrameFlag = new System.Windows.Forms.CheckBox();
            this.label7 = new System.Windows.Forms.Label();
            this.areaTopMargin = new System.Windows.Forms.NumericUpDown();
            this.label6 = new System.Windows.Forms.Label();
            this.objectTopMargin = new System.Windows.Forms.NumericUpDown();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.picketToFlag = new System.Windows.Forms.CheckBox();
            this.picketFromFlag = new System.Windows.Forms.CheckBox();
            this.picketTill = new System.Windows.Forms.NumericUpDown();
            this.picketFrom = new System.Windows.Forms.NumericUpDown();
            this.onlyCheckedFlag = new System.Windows.Forms.CheckBox();
            this.progressBar = new System.Windows.Forms.ProgressBar();
            this.label9 = new System.Windows.Forms.Label();
            this.progressLabel = new System.Windows.Forms.Label();
            this.viewedLabel = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.selectedLabel = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.closeButton = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            this.filterButton = new System.Windows.Forms.Button();
            this.resetButton = new System.Windows.Forms.Button();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.filterBgWorker = new System.ComponentModel.BackgroundWorker();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            this.pictureBox4 = new System.Windows.Forms.PictureBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.areaTopMargin)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.objectTopMargin)).BeginInit();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picketTill)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.picketFrom)).BeginInit();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.pictureBox1);
            this.groupBox1.Controls.Add(this.timeToFlag);
            this.groupBox1.Controls.Add(this.timeFromFlag);
            this.groupBox1.Controls.Add(this.timeTill);
            this.groupBox1.Controls.Add(this.timeFrom);
            this.groupBox1.Location = new System.Drawing.Point(6, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(220, 84);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Время";
            // 
            // timeToFlag
            // 
            this.timeToFlag.AutoSize = true;
            this.timeToFlag.Location = new System.Drawing.Point(87, 53);
            this.timeToFlag.Name = "timeToFlag";
            this.timeToFlag.Size = new System.Drawing.Size(38, 17);
            this.timeToFlag.TabIndex = 7;
            this.timeToFlag.Text = "до";
            this.timeToFlag.UseVisualStyleBackColor = true;
            this.timeToFlag.CheckedChanged += new System.EventHandler(this.timeToFlag_CheckedChanged);
            // 
            // timeFromFlag
            // 
            this.timeFromFlag.AutoSize = true;
            this.timeFromFlag.Location = new System.Drawing.Point(88, 23);
            this.timeFromFlag.Name = "timeFromFlag";
            this.timeFromFlag.Size = new System.Drawing.Size(37, 17);
            this.timeFromFlag.TabIndex = 6;
            this.timeFromFlag.Text = "от";
            this.timeFromFlag.UseVisualStyleBackColor = true;
            this.timeFromFlag.CheckedChanged += new System.EventHandler(this.timeFromFlag_CheckedChanged);
            // 
            // timeTill
            // 
            this.timeTill.CustomFormat = "HH : mm";
            this.timeTill.Enabled = false;
            this.timeTill.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.timeTill.Location = new System.Drawing.Point(141, 50);
            this.timeTill.Name = "timeTill";
            this.timeTill.ShowUpDown = true;
            this.timeTill.Size = new System.Drawing.Size(70, 20);
            this.timeTill.TabIndex = 5;
            // 
            // timeFrom
            // 
            this.timeFrom.CustomFormat = "HH : mm";
            this.timeFrom.Enabled = false;
            this.timeFrom.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.timeFrom.Location = new System.Drawing.Point(141, 19);
            this.timeFrom.Name = "timeFrom";
            this.timeFrom.ShowUpDown = true;
            this.timeFrom.Size = new System.Drawing.Size(70, 20);
            this.timeFrom.TabIndex = 4;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.pictureBox2);
            this.groupBox2.Controls.Add(this.tempAreaFlag);
            this.groupBox2.Controls.Add(this.tempFrameFlag);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.areaTopMargin);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.objectTopMargin);
            this.groupBox2.Location = new System.Drawing.Point(232, 3);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(308, 84);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Отклонение от средней температуры";
            // 
            // tempAreaFlag
            // 
            this.tempAreaFlag.AutoSize = true;
            this.tempAreaFlag.Location = new System.Drawing.Point(88, 52);
            this.tempAreaFlag.Name = "tempAreaFlag";
            this.tempAreaFlag.Size = new System.Drawing.Size(117, 17);
            this.tempAreaFlag.TabIndex = 15;
            this.tempAreaFlag.Text = "По области более";
            this.tempAreaFlag.UseVisualStyleBackColor = true;
            this.tempAreaFlag.CheckedChanged += new System.EventHandler(this.tempAreaFlag_CheckedChanged);
            // 
            // tempFrameFlag
            // 
            this.tempFrameFlag.AutoSize = true;
            this.tempFrameFlag.Location = new System.Drawing.Point(88, 22);
            this.tempFrameFlag.Name = "tempFrameFlag";
            this.tempFrameFlag.Size = new System.Drawing.Size(105, 17);
            this.tempFrameFlag.TabIndex = 14;
            this.tempFrameFlag.Text = "По кадру более";
            this.tempFrameFlag.UseVisualStyleBackColor = true;
            this.tempFrameFlag.CheckedChanged += new System.EventHandler(this.tempFrameFlag_CheckedChanged);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(286, 54);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(14, 13);
            this.label7.TabIndex = 11;
            this.label7.Text = "C";
            // 
            // areaTopMargin
            // 
            this.areaTopMargin.Enabled = false;
            this.areaTopMargin.Location = new System.Drawing.Point(211, 51);
            this.areaTopMargin.Name = "areaTopMargin";
            this.areaTopMargin.Size = new System.Drawing.Size(69, 20);
            this.areaTopMargin.TabIndex = 13;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(286, 24);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(14, 13);
            this.label6.TabIndex = 6;
            this.label6.Text = "С";
            // 
            // objectTopMargin
            // 
            this.objectTopMargin.Enabled = false;
            this.objectTopMargin.Location = new System.Drawing.Point(211, 21);
            this.objectTopMargin.Name = "objectTopMargin";
            this.objectTopMargin.Size = new System.Drawing.Size(69, 20);
            this.objectTopMargin.TabIndex = 10;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.pictureBox4);
            this.groupBox3.Controls.Add(this.picketToFlag);
            this.groupBox3.Controls.Add(this.picketFromFlag);
            this.groupBox3.Controls.Add(this.picketTill);
            this.groupBox3.Controls.Add(this.picketFrom);
            this.groupBox3.Location = new System.Drawing.Point(6, 89);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(220, 82);
            this.groupBox3.TabIndex = 2;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Пикеты";
            // 
            // picketToFlag
            // 
            this.picketToFlag.AutoSize = true;
            this.picketToFlag.Location = new System.Drawing.Point(87, 51);
            this.picketToFlag.Name = "picketToFlag";
            this.picketToFlag.Size = new System.Drawing.Size(38, 17);
            this.picketToFlag.TabIndex = 9;
            this.picketToFlag.Text = "до";
            this.picketToFlag.UseVisualStyleBackColor = true;
            this.picketToFlag.CheckedChanged += new System.EventHandler(this.picketToFlag_CheckedChanged);
            // 
            // picketFromFlag
            // 
            this.picketFromFlag.AutoSize = true;
            this.picketFromFlag.Location = new System.Drawing.Point(87, 20);
            this.picketFromFlag.Name = "picketFromFlag";
            this.picketFromFlag.Size = new System.Drawing.Size(37, 17);
            this.picketFromFlag.TabIndex = 8;
            this.picketFromFlag.Text = "от";
            this.picketFromFlag.UseVisualStyleBackColor = true;
            this.picketFromFlag.CheckedChanged += new System.EventHandler(this.picketFromFlag_CheckedChanged);
            // 
            // picketTill
            // 
            this.picketTill.Enabled = false;
            this.picketTill.Location = new System.Drawing.Point(141, 50);
            this.picketTill.Name = "picketTill";
            this.picketTill.Size = new System.Drawing.Size(69, 20);
            this.picketTill.TabIndex = 9;
            // 
            // picketFrom
            // 
            this.picketFrom.Enabled = false;
            this.picketFrom.Location = new System.Drawing.Point(141, 19);
            this.picketFrom.Name = "picketFrom";
            this.picketFrom.Size = new System.Drawing.Size(69, 20);
            this.picketFrom.TabIndex = 8;
            // 
            // onlyCheckedFlag
            // 
            this.onlyCheckedFlag.AutoSize = true;
            this.onlyCheckedFlag.Location = new System.Drawing.Point(88, 50);
            this.onlyCheckedFlag.Name = "onlyCheckedFlag";
            this.onlyCheckedFlag.Size = new System.Drawing.Size(192, 17);
            this.onlyCheckedFlag.TabIndex = 3;
            this.onlyCheckedFlag.Text = "Показывать только отмеченные";
            this.onlyCheckedFlag.UseVisualStyleBackColor = true;
            // 
            // progressBar
            // 
            this.progressBar.Location = new System.Drawing.Point(6, 193);
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(534, 23);
            this.progressBar.TabIndex = 4;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(5, 177);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(67, 13);
            this.label9.TabIndex = 14;
            this.label9.Text = "Выполнено:";
            // 
            // progressLabel
            // 
            this.progressLabel.AutoSize = true;
            this.progressLabel.Location = new System.Drawing.Point(78, 177);
            this.progressLabel.Name = "progressLabel";
            this.progressLabel.Size = new System.Drawing.Size(21, 13);
            this.progressLabel.TabIndex = 14;
            this.progressLabel.Text = "0%";
            // 
            // viewedLabel
            // 
            this.viewedLabel.AutoSize = true;
            this.viewedLabel.Location = new System.Drawing.Point(84, 222);
            this.viewedLabel.Name = "viewedLabel";
            this.viewedLabel.Size = new System.Drawing.Size(13, 13);
            this.viewedLabel.TabIndex = 16;
            this.viewedLabel.Text = "0";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(5, 222);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(79, 13);
            this.label12.TabIndex = 15;
            this.label12.Text = "Просмотрено:";
            // 
            // selectedLabel
            // 
            this.selectedLabel.AutoSize = true;
            this.selectedLabel.Location = new System.Drawing.Point(84, 238);
            this.selectedLabel.Name = "selectedLabel";
            this.selectedLabel.Size = new System.Drawing.Size(13, 13);
            this.selectedLabel.TabIndex = 18;
            this.selectedLabel.Text = "0";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(25, 238);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(59, 13);
            this.label14.TabIndex = 17;
            this.label14.Text = "Отобрано:";
            // 
            // closeButton
            // 
            this.closeButton.Location = new System.Drawing.Point(465, 269);
            this.closeButton.Name = "closeButton";
            this.closeButton.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.closeButton.Size = new System.Drawing.Size(75, 23);
            this.closeButton.TabIndex = 19;
            this.closeButton.Text = "Закрыть";
            this.closeButton.UseVisualStyleBackColor = true;
            this.closeButton.Click += new System.EventHandler(this.closeButton_Click);
            // 
            // cancelButton
            // 
            this.cancelButton.Enabled = false;
            this.cancelButton.Location = new System.Drawing.Point(304, 269);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 20;
            this.cancelButton.Text = "Отмена";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // filterButton
            // 
            this.filterButton.Location = new System.Drawing.Point(209, 269);
            this.filterButton.Name = "filterButton";
            this.filterButton.Size = new System.Drawing.Size(89, 23);
            this.filterButton.TabIndex = 21;
            this.filterButton.Text = "Фильтровать";
            this.filterButton.UseVisualStyleBackColor = true;
            this.filterButton.Click += new System.EventHandler(this.filterButton_Click);
            // 
            // resetButton
            // 
            this.resetButton.Location = new System.Drawing.Point(385, 269);
            this.resetButton.Name = "resetButton";
            this.resetButton.Size = new System.Drawing.Size(75, 23);
            this.resetButton.TabIndex = 22;
            this.resetButton.Text = "Сбросить";
            this.resetButton.UseVisualStyleBackColor = true;
            this.resetButton.Click += new System.EventHandler(this.resetButton_Click);
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.pictureBox3);
            this.groupBox4.Controls.Add(this.onlyCheckedFlag);
            this.groupBox4.Location = new System.Drawing.Point(232, 89);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(308, 82);
            this.groupBox4.TabIndex = 23;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Метки";
            // 
            // filterBgWorker
            // 
            this.filterBgWorker.WorkerReportsProgress = true;
            this.filterBgWorker.WorkerSupportsCancellation = true;
            this.filterBgWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.filterBgWorker_DoWork);
            this.filterBgWorker.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.filterBgWorker_ProgressChanged);
            this.filterBgWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.filterBgWorker_RunWorkerCompleted);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox1.Image")));
            this.pictureBox1.Location = new System.Drawing.Point(22, 19);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(50, 50);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 8;
            this.pictureBox1.TabStop = false;
            // 
            // pictureBox2
            // 
            this.pictureBox2.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox2.Image")));
            this.pictureBox2.Location = new System.Drawing.Point(19, 21);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(50, 50);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox2.TabIndex = 16;
            this.pictureBox2.TabStop = false;
            // 
            // pictureBox3
            // 
            this.pictureBox3.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox3.Image")));
            this.pictureBox3.Location = new System.Drawing.Point(19, 20);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(50, 50);
            this.pictureBox3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox3.TabIndex = 17;
            this.pictureBox3.TabStop = false;
            // 
            // pictureBox4
            // 
            this.pictureBox4.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox4.Image")));
            this.pictureBox4.Location = new System.Drawing.Point(22, 20);
            this.pictureBox4.Name = "pictureBox4";
            this.pictureBox4.Size = new System.Drawing.Size(50, 50);
            this.pictureBox4.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox4.TabIndex = 18;
            this.pictureBox4.TabStop = false;
            // 
            // FrameFilterForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(546, 301);
            this.ControlBox = false;
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.resetButton);
            this.Controls.Add(this.filterButton);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.closeButton);
            this.Controls.Add(this.selectedLabel);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.viewedLabel);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.progressLabel);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.progressBar);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "FrameFilterForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Фильтрация кадров фильма";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.areaTopMargin)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.objectTopMargin)).EndInit();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picketTill)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.picketFrom)).EndInit();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.CheckBox onlyCheckedFlag;
        private System.Windows.Forms.DateTimePicker timeTill;
        private System.Windows.Forms.DateTimePicker timeFrom;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.NumericUpDown objectTopMargin;
        private System.Windows.Forms.NumericUpDown picketTill;
        private System.Windows.Forms.NumericUpDown picketFrom;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.NumericUpDown areaTopMargin;
        private System.Windows.Forms.ProgressBar progressBar;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label progressLabel;
        private System.Windows.Forms.Label viewedLabel;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label selectedLabel;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Button closeButton;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.Button filterButton;
        private System.Windows.Forms.Button resetButton;
        private System.Windows.Forms.CheckBox timeToFlag;
        private System.Windows.Forms.CheckBox timeFromFlag;
        private System.Windows.Forms.CheckBox tempAreaFlag;
        private System.Windows.Forms.CheckBox tempFrameFlag;
        private System.Windows.Forms.CheckBox picketToFlag;
        private System.Windows.Forms.CheckBox picketFromFlag;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.ComponentModel.BackgroundWorker filterBgWorker;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.PictureBox pictureBox4;
        private System.Windows.Forms.PictureBox pictureBox3;
    }
}