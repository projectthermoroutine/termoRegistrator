namespace Registrator
{
    partial class ProjectProperties
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ProjectProperties));
            this.ProjectNameTextBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.IRBFolderBox = new System.Windows.Forms.TextBox();
            this.IRBFolderButton = new System.Windows.Forms.Button();
            this.CancelBtn = new System.Windows.Forms.Button();
            this.CreateButton = new System.Windows.Forms.Button();
            this.ProjectDatePicker = new System.Windows.Forms.DateTimePicker();
            this.label3 = new System.Windows.Forms.Label();
            this.ChangeProjectFolderButton = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.ProjectFolderBox = new System.Windows.Forms.TextBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.numericUpDown1 = new System.Windows.Forms.NumericUpDown();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).BeginInit();
            this.SuspendLayout();
            // 
            // ProjectNameTextBox
            // 
            this.ProjectNameTextBox.Location = new System.Drawing.Point(15, 25);
            this.ProjectNameTextBox.Name = "ProjectNameTextBox";
            this.ProjectNameTextBox.Size = new System.Drawing.Size(341, 20);
            this.ProjectNameTextBox.TabIndex = 0;
            this.ProjectNameTextBox.TextChanged += new System.EventHandler(this.ProjectNameTextBox_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(83, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Наименование";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 26);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(48, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Каталог";
            // 
            // IRBFolderBox
            // 
            this.IRBFolderBox.Location = new System.Drawing.Point(60, 23);
            this.IRBFolderBox.Name = "IRBFolderBox";
            this.IRBFolderBox.ReadOnly = true;
            this.IRBFolderBox.Size = new System.Drawing.Size(357, 20);
            this.IRBFolderBox.TabIndex = 2;
            // 
            // IRBFolderButton
            // 
            this.IRBFolderButton.Location = new System.Drawing.Point(423, 21);
            this.IRBFolderButton.Name = "IRBFolderButton";
            this.IRBFolderButton.Size = new System.Drawing.Size(25, 23);
            this.IRBFolderButton.TabIndex = 7;
            this.IRBFolderButton.Text = "...";
            this.IRBFolderButton.UseVisualStyleBackColor = true;
            this.IRBFolderButton.Click += new System.EventHandler(this.IRBFolderButton_Click);
            // 
            // CancelBtn
            // 
            this.CancelBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.CancelBtn.Location = new System.Drawing.Point(398, 192);
            this.CancelBtn.Name = "CancelBtn";
            this.CancelBtn.Size = new System.Drawing.Size(75, 23);
            this.CancelBtn.TabIndex = 9;
            this.CancelBtn.Text = "Отмена";
            this.CancelBtn.UseVisualStyleBackColor = true;
            // 
            // CreateButton
            // 
            this.CreateButton.Location = new System.Drawing.Point(312, 192);
            this.CreateButton.Name = "CreateButton";
            this.CreateButton.Size = new System.Drawing.Size(75, 23);
            this.CreateButton.TabIndex = 10;
            this.CreateButton.Text = "Создать";
            this.CreateButton.UseVisualStyleBackColor = true;
            this.CreateButton.Click += new System.EventHandler(this.CreateButton_Click);
            // 
            // ProjectDatePicker
            // 
            this.ProjectDatePicker.Format = System.Windows.Forms.DateTimePickerFormat.Short;
            this.ProjectDatePicker.Location = new System.Drawing.Point(367, 25);
            this.ProjectDatePicker.Name = "ProjectDatePicker";
            this.ProjectDatePicker.Size = new System.Drawing.Size(106, 20);
            this.ProjectDatePicker.TabIndex = 14;
            this.ProjectDatePicker.ValueChanged += new System.EventHandler(this.ProjectDatePicker_ValueChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(364, 9);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(33, 13);
            this.label3.TabIndex = 15;
            this.label3.Text = "Дата";
            // 
            // ChangeProjectFolderButton
            // 
            this.ChangeProjectFolderButton.Location = new System.Drawing.Point(277, 75);
            this.ChangeProjectFolderButton.Name = "ChangeProjectFolderButton";
            this.ChangeProjectFolderButton.Size = new System.Drawing.Size(25, 23);
            this.ChangeProjectFolderButton.TabIndex = 19;
            this.ChangeProjectFolderButton.Text = "...";
            this.ChangeProjectFolderButton.UseVisualStyleBackColor = true;
            this.ChangeProjectFolderButton.Click += new System.EventHandler(this.ChangeProjectFolderButton_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 61);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(92, 13);
            this.label4.TabIndex = 18;
            this.label4.Text = "Каталог проекта";
            // 
            // ProjectFolderBox
            // 
            this.ProjectFolderBox.Location = new System.Drawing.Point(15, 77);
            this.ProjectFolderBox.Name = "ProjectFolderBox";
            this.ProjectFolderBox.ReadOnly = true;
            this.ProjectFolderBox.Size = new System.Drawing.Size(256, 20);
            this.ProjectFolderBox.TabIndex = 17;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.IRBFolderBox);
            this.groupBox2.Controls.Add(this.IRBFolderButton);
            this.groupBox2.Location = new System.Drawing.Point(12, 131);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(461, 55);
            this.groupBox2.TabIndex = 20;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Размещение файлов тепловизора";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.checkBox1);
            this.groupBox3.Controls.Add(this.numericUpDown1);
            this.groupBox3.Location = new System.Drawing.Point(308, 51);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(165, 74);
            this.groupBox3.TabIndex = 21;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "текущее cмещение камеры";
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Location = new System.Drawing.Point(46, 51);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(75, 17);
            this.checkBox1.TabIndex = 1;
            this.checkBox1.Text = "изменить";
            this.checkBox1.UseVisualStyleBackColor = true;
            this.checkBox1.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // numericUpDown1
            // 
            this.numericUpDown1.Enabled = false;
            this.numericUpDown1.Location = new System.Drawing.Point(46, 25);
            this.numericUpDown1.Name = "numericUpDown1";
            this.numericUpDown1.Size = new System.Drawing.Size(75, 20);
            this.numericUpDown1.TabIndex = 0;
            // 
            // ProjectProperties
            // 
            this.AcceptButton = this.CreateButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(481, 227);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.ChangeProjectFolderButton);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.ProjectFolderBox);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.ProjectDatePicker);
            this.Controls.Add(this.CreateButton);
            this.Controls.Add(this.CancelBtn);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.ProjectNameTextBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(480, 235);
            this.Name = "ProjectProperties";
            this.Text = "Новый проект";
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox ProjectNameTextBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox IRBFolderBox;
        private System.Windows.Forms.Button IRBFolderButton;
        private System.Windows.Forms.Button CancelBtn;
        private System.Windows.Forms.Button CreateButton;
        private System.Windows.Forms.DateTimePicker ProjectDatePicker;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button ChangeProjectFolderButton;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox ProjectFolderBox;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.NumericUpDown numericUpDown1;
    }
}