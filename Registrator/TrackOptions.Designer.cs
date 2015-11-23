namespace Registrator
{
    partial class TrackOptions
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
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabBackGround = new System.Windows.Forms.TabPage();
            this.tableLayoutPanelColors = new System.Windows.Forms.TableLayoutPanel();
            this.elementHost2 = new System.Windows.Forms.Integration.ElementHost();
            this.groupColorSetUserControl2 = new Registrator.Equipment.GroupColorSetUserControl();
            this.label_length = new System.Windows.Forms.Label();
            this.elementHost1 = new System.Windows.Forms.Integration.ElementHost();
            this.groupColorSetUserControl1 = new Registrator.Equipment.GroupColorSetUserControl();
            this.label2 = new System.Windows.Forms.Label();
            this.ObjectsToDisplay = new System.Windows.Forms.TabPage();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.EquipmentVisibilityCheckBox = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.button_Cancel = new System.Windows.Forms.Button();
            this.button_OK = new System.Windows.Forms.Button();
            this.panel2 = new System.Windows.Forms.Panel();
            this.tableLayoutPanel1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabBackGround.SuspendLayout();
            this.tableLayoutPanelColors.SuspendLayout();
            this.ObjectsToDisplay.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.tabControl1, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.panel1, 0, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(355, 201);
            this.tableLayoutPanel1.TabIndex = 4;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabBackGround);
            this.tabControl1.Controls.Add(this.ObjectsToDisplay);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(3, 3);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(349, 155);
            this.tabControl1.TabIndex = 10;
            // 
            // tabBackGround
            // 
            this.tabBackGround.Controls.Add(this.tableLayoutPanelColors);
            this.tabBackGround.Location = new System.Drawing.Point(4, 22);
            this.tabBackGround.Name = "tabBackGround";
            this.tabBackGround.Padding = new System.Windows.Forms.Padding(3);
            this.tabBackGround.Size = new System.Drawing.Size(341, 129);
            this.tabBackGround.TabIndex = 1;
            this.tabBackGround.Tag = "Colors";
            this.tabBackGround.Text = "Настройки";
            this.tabBackGround.UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanelColors
            // 
            this.tableLayoutPanelColors.ColumnCount = 2;
            this.tableLayoutPanelColors.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanelColors.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanelColors.Controls.Add(this.elementHost2, 1, 1);
            this.tableLayoutPanelColors.Controls.Add(this.label_length, 0, 0);
            this.tableLayoutPanelColors.Controls.Add(this.elementHost1, 1, 0);
            this.tableLayoutPanelColors.Controls.Add(this.label2, 0, 1);
            this.tableLayoutPanelColors.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanelColors.Location = new System.Drawing.Point(3, 3);
            this.tableLayoutPanelColors.Name = "tableLayoutPanelColors";
            this.tableLayoutPanelColors.RowCount = 2;
            this.tableLayoutPanelColors.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 48.78049F));
            this.tableLayoutPanelColors.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 51.21951F));
            this.tableLayoutPanelColors.Size = new System.Drawing.Size(335, 123);
            this.tableLayoutPanelColors.TabIndex = 10;
            // 
            // elementHost2
            // 
            this.elementHost2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.elementHost2.Location = new System.Drawing.Point(169, 61);
            this.elementHost2.Margin = new System.Windows.Forms.Padding(2);
            this.elementHost2.Name = "elementHost2";
            this.elementHost2.Size = new System.Drawing.Size(164, 60);
            this.elementHost2.TabIndex = 10;
            this.elementHost2.Text = "elementHost2";
            this.elementHost2.Child = this.groupColorSetUserControl2;
            // 
            // label_length
            // 
            this.label_length.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label_length.AutoSize = true;
            this.label_length.Location = new System.Drawing.Point(53, 23);
            this.label_length.Name = "label_length";
            this.label_length.Size = new System.Drawing.Size(61, 13);
            this.label_length.TabIndex = 7;
            this.label_length.Text = "Цвет фона";
            // 
            // elementHost1
            // 
            this.elementHost1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.elementHost1.Location = new System.Drawing.Point(169, 2);
            this.elementHost1.Margin = new System.Windows.Forms.Padding(2);
            this.elementHost1.Name = "elementHost1";
            this.elementHost1.Size = new System.Drawing.Size(164, 55);
            this.elementHost1.TabIndex = 8;
            this.elementHost1.Text = "elementHost1";
            this.elementHost1.Child = this.groupColorSetUserControl1;
            // 
            // label2
            // 
            this.label2.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(22, 84);
            this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(123, 13);
            this.label2.TabIndex = 9;
            this.label2.Text = "Цвет маркера состава";
            // 
            // ObjectsToDisplay
            // 
            this.ObjectsToDisplay.Controls.Add(this.tableLayoutPanel2);
            this.ObjectsToDisplay.Location = new System.Drawing.Point(4, 22);
            this.ObjectsToDisplay.Margin = new System.Windows.Forms.Padding(2);
            this.ObjectsToDisplay.Name = "ObjectsToDisplay";
            this.ObjectsToDisplay.Size = new System.Drawing.Size(341, 129);
            this.ObjectsToDisplay.TabIndex = 2;
            this.ObjectsToDisplay.Tag = "Display";
            this.ObjectsToDisplay.Text = "Фильтр";
            this.ObjectsToDisplay.UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 1;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel2.Controls.Add(this.EquipmentVisibilityCheckBox, 0, 1);
            this.tableLayoutPanel2.Controls.Add(this.label1, 0, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel2.Margin = new System.Windows.Forms.Padding(2);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 2;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 63F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(341, 129);
            this.tableLayoutPanel2.TabIndex = 0;
            // 
            // EquipmentVisibilityCheckBox
            // 
            this.EquipmentVisibilityCheckBox.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.EquipmentVisibilityCheckBox.AutoSize = true;
            this.EquipmentVisibilityCheckBox.Location = new System.Drawing.Point(121, 89);
            this.EquipmentVisibilityCheckBox.Margin = new System.Windows.Forms.Padding(2);
            this.EquipmentVisibilityCheckBox.Name = "EquipmentVisibilityCheckBox";
            this.EquipmentVisibilityCheckBox.Size = new System.Drawing.Size(99, 17);
            this.EquipmentVisibilityCheckBox.TabIndex = 0;
            this.EquipmentVisibilityCheckBox.Text = "Оборудование";
            this.EquipmentVisibilityCheckBox.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(136, 26);
            this.label1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(69, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Отображать";
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.button_Cancel);
            this.panel1.Controls.Add(this.button_OK);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(3, 164);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(349, 34);
            this.panel1.TabIndex = 4;
            // 
            // button_Cancel
            // 
            this.button_Cancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.button_Cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.button_Cancel.Location = new System.Drawing.Point(279, 11);
            this.button_Cancel.Name = "button_Cancel";
            this.button_Cancel.Size = new System.Drawing.Size(70, 23);
            this.button_Cancel.TabIndex = 0;
            this.button_Cancel.Text = "Отмена";
            this.button_Cancel.UseVisualStyleBackColor = true;
            this.button_Cancel.Click += new System.EventHandler(this.button_Cancel_Click);
            // 
            // button_OK
            // 
            this.button_OK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.button_OK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.button_OK.Location = new System.Drawing.Point(198, 11);
            this.button_OK.Name = "button_OK";
            this.button_OK.Size = new System.Drawing.Size(75, 23);
            this.button_OK.TabIndex = 1;
            this.button_OK.Text = "OK";
            this.button_OK.UseVisualStyleBackColor = true;
            this.button_OK.Click += new System.EventHandler(this.button_OK_Click);
            // 
            // panel2
            // 
            this.panel2.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.panel2.AutoSize = true;
            this.panel2.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.panel2.Location = new System.Drawing.Point(441, 239);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(0, 0);
            this.panel2.TabIndex = 9;
            // 
            // TrackOptions
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(355, 201);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Name = "TrackOptions";
            this.Text = "настройки панели  пути";
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tabControl1.ResumeLayout(false);
            this.tabBackGround.ResumeLayout(false);
            this.tableLayoutPanelColors.ResumeLayout(false);
            this.tableLayoutPanelColors.PerformLayout();
            this.ObjectsToDisplay.ResumeLayout(false);
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Label label_length;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button button_OK;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanelColors;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabBackGround;
        private System.Windows.Forms.Integration.ElementHost elementHost1;
        private Equipment.GroupColorSetUserControl groupColorSetUserControl1;
        private System.Windows.Forms.TabPage ObjectsToDisplay;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.Button button_Cancel;
        private System.Windows.Forms.CheckBox EquipmentVisibilityCheckBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Integration.ElementHost elementHost2;
        private Equipment.GroupColorSetUserControl groupColorSetUserControl2;
        private System.Windows.Forms.Label label2;
    }
}