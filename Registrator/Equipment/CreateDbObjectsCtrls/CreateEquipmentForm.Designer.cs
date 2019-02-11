namespace Registrator.Equipment.CreateDbObjectsCtrls
{
    partial class CreateEquipmentForm
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
            this.Cancel = new System.Windows.Forms.Button();
            this.OK = new System.Windows.Forms.Button();
            this.EquipmentInfoTextBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.mainTableLayoutPanel = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel7 = new System.Windows.Forms.TableLayoutPanel();
            this.equipmentNameLabel = new System.Windows.Forms.Label();
            this.EquipmentInfoLabel = new System.Windows.Forms.Label();
            this.EquipmentNameTextBox = new System.Windows.Forms.TextBox();
            this.selectEquip = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.tableLayoutPanel4 = new System.Windows.Forms.TableLayoutPanel();
            this.elementHost1 = new System.Windows.Forms.Integration.ElementHost();
            this.panel1 = new System.Windows.Forms.Panel();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.lShisft = new System.Windows.Forms.Label();
            this.lMaxTemperature = new System.Windows.Forms.Label();
            this.cmbBx_valid = new System.Windows.Forms.ComboBox();
            this.lValid = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
            this.label2 = new System.Windows.Forms.Label();
            this.n_MaxTemperature = new System.Windows.Forms.NumericUpDown();
            this.panel3 = new System.Windows.Forms.Panel();
            this.tableLayoutPanel5 = new System.Windows.Forms.TableLayoutPanel();
            this.label3 = new System.Windows.Forms.Label();
            this.n_picketShift = new System.Windows.Forms.NumericUpDown();
            this.label5 = new System.Windows.Forms.Label();
            this.tableLayoutPanel6 = new System.Windows.Forms.TableLayoutPanel();
            this.numUpDown_equipLenght = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.longObjectCheckBox = new System.Windows.Forms.CheckBox();
            this.label7 = new System.Windows.Forms.Label();
            this.mainTableLayoutPanel.SuspendLayout();
            this.tableLayoutPanel7.SuspendLayout();
            this.tableLayoutPanel4.SuspendLayout();
            this.panel1.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.panel2.SuspendLayout();
            this.tableLayoutPanel3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.n_MaxTemperature)).BeginInit();
            this.panel3.SuspendLayout();
            this.tableLayoutPanel5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.n_picketShift)).BeginInit();
            this.tableLayoutPanel6.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numUpDown_equipLenght)).BeginInit();
            this.SuspendLayout();
            // 
            // Cancel
            // 
            this.Cancel.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.Cancel.AutoSize = true;
            this.Cancel.Location = new System.Drawing.Point(224, 5);
            this.Cancel.Name = "Cancel";
            this.Cancel.Size = new System.Drawing.Size(78, 23);
            this.Cancel.TabIndex = 0;
            this.Cancel.Text = "Отмена";
            this.Cancel.UseVisualStyleBackColor = true;
            this.Cancel.Click += new System.EventHandler(this.Cancel_Click);
            // 
            // OK
            // 
            this.OK.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.OK.Enabled = false;
            this.OK.Location = new System.Drawing.Point(141, 5);
            this.OK.Name = "OK";
            this.OK.Size = new System.Drawing.Size(77, 23);
            this.OK.TabIndex = 2;
            this.OK.Text = "Применить";
            this.OK.UseVisualStyleBackColor = true;
            this.OK.Click += new System.EventHandler(this.OK_Click);
            // 
            // EquipmentInfoTextBox
            // 
            this.EquipmentInfoTextBox.Location = new System.Drawing.Point(375, 56);
            this.EquipmentInfoTextBox.Name = "EquipmentInfoTextBox";
            this.EquipmentInfoTextBox.Size = new System.Drawing.Size(283, 20);
            this.EquipmentInfoTextBox.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label1.Location = new System.Drawing.Point(3, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(366, 27);
            this.label1.TabIndex = 14;
            this.label1.Text = "Список доступного оборудования";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // mainTableLayoutPanel
            // 
            this.mainTableLayoutPanel.ColumnCount = 1;
            this.mainTableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.mainTableLayoutPanel.Controls.Add(this.tableLayoutPanel7, 0, 0);
            this.mainTableLayoutPanel.Controls.Add(this.label6, 0, 5);
            this.mainTableLayoutPanel.Controls.Add(this.tableLayoutPanel4, 0, 4);
            this.mainTableLayoutPanel.Controls.Add(this.panel1, 0, 5);
            this.mainTableLayoutPanel.Controls.Add(this.tableLayoutPanel2, 0, 1);
            this.mainTableLayoutPanel.Controls.Add(this.label7, 0, 3);
            this.mainTableLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainTableLayoutPanel.Location = new System.Drawing.Point(0, 0);
            this.mainTableLayoutPanel.Margin = new System.Windows.Forms.Padding(6, 3, 6, 3);
            this.mainTableLayoutPanel.Name = "mainTableLayoutPanel";
            this.mainTableLayoutPanel.RowCount = 6;
            this.mainTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.mainTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.mainTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.mainTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 7F));
            this.mainTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 333F));
            this.mainTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.mainTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 36F));
            this.mainTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.mainTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.mainTableLayoutPanel.Size = new System.Drawing.Size(750, 666);
            this.mainTableLayoutPanel.TabIndex = 15;
            // 
            // tableLayoutPanel7
            // 
            this.tableLayoutPanel7.AutoSize = true;
            this.tableLayoutPanel7.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.tableLayoutPanel7.ColumnCount = 2;
            this.tableLayoutPanel7.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel7.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel7.Controls.Add(this.label1, 0, 0);
            this.tableLayoutPanel7.Controls.Add(this.equipmentNameLabel, 0, 1);
            this.tableLayoutPanel7.Controls.Add(this.EquipmentInfoTextBox, 1, 2);
            this.tableLayoutPanel7.Controls.Add(this.EquipmentInfoLabel, 0, 2);
            this.tableLayoutPanel7.Controls.Add(this.EquipmentNameTextBox, 1, 1);
            this.tableLayoutPanel7.Controls.Add(this.selectEquip, 1, 0);
            this.tableLayoutPanel7.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel7.Location = new System.Drawing.Point(3, 3);
            this.tableLayoutPanel7.Name = "tableLayoutPanel7";
            this.tableLayoutPanel7.RowCount = 3;
            this.tableLayoutPanel7.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel7.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel7.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel7.Size = new System.Drawing.Size(744, 79);
            this.tableLayoutPanel7.TabIndex = 16;
            // 
            // equipmentNameLabel
            // 
            this.equipmentNameLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.equipmentNameLabel.Location = new System.Drawing.Point(3, 27);
            this.equipmentNameLabel.Name = "equipmentNameLabel";
            this.equipmentNameLabel.Size = new System.Drawing.Size(366, 26);
            this.equipmentNameLabel.TabIndex = 15;
            this.equipmentNameLabel.Text = "Название оборудования";
            this.equipmentNameLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // EquipmentInfoLabel
            // 
            this.EquipmentInfoLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.EquipmentInfoLabel.Location = new System.Drawing.Point(3, 53);
            this.EquipmentInfoLabel.Name = "EquipmentInfoLabel";
            this.EquipmentInfoLabel.Size = new System.Drawing.Size(366, 26);
            this.EquipmentInfoLabel.TabIndex = 7;
            this.EquipmentInfoLabel.Text = "Дополнительная информация";
            this.EquipmentInfoLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // EquipmentNameTextBox
            // 
            this.EquipmentNameTextBox.Location = new System.Drawing.Point(375, 30);
            this.EquipmentNameTextBox.Name = "EquipmentNameTextBox";
            this.EquipmentNameTextBox.Size = new System.Drawing.Size(283, 20);
            this.EquipmentNameTextBox.TabIndex = 16;
            // 
            // selectEquip
            // 
            this.selectEquip.FormattingEnabled = true;
            this.selectEquip.Location = new System.Drawing.Point(375, 3);
            this.selectEquip.Name = "selectEquip";
            this.selectEquip.Size = new System.Drawing.Size(283, 21);
            this.selectEquip.TabIndex = 13;
            this.selectEquip.SelectedIndexChanged += new System.EventHandler(this.cmbBx_selEquip_SelectedIndexChanged);
            // 
            // label6
            // 
            this.label6.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.label6.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label6.Location = new System.Drawing.Point(3, 647);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(744, 2);
            this.label6.TabIndex = 19;
            // 
            // tableLayoutPanel4
            // 
            this.tableLayoutPanel4.ColumnCount = 3;
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 260F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel4.Controls.Add(this.elementHost1, 1, 1);
            this.tableLayoutPanel4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel4.Location = new System.Drawing.Point(3, 265);
            this.tableLayoutPanel4.Name = "tableLayoutPanel4";
            this.tableLayoutPanel4.RowCount = 3;
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 263F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel4.Size = new System.Drawing.Size(744, 327);
            this.tableLayoutPanel4.TabIndex = 16;
            // 
            // elementHost1
            // 
            this.elementHost1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.elementHost1.Location = new System.Drawing.Point(245, 35);
            this.elementHost1.Name = "elementHost1";
            this.elementHost1.Size = new System.Drawing.Size(254, 257);
            this.elementHost1.TabIndex = 13;
            this.elementHost1.Text = "elementHost1";
            this.elementHost1.Child = null;
            // 
            // panel1
            // 
            this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.panel1.Controls.Add(this.Cancel);
            this.panel1.Controls.Add(this.OK);
            this.panel1.Location = new System.Drawing.Point(429, 598);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(318, 29);
            this.panel1.TabIndex = 17;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Single;
            this.tableLayoutPanel2.ColumnCount = 4;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel2.Controls.Add(this.lShisft, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.lMaxTemperature, 1, 0);
            this.tableLayoutPanel2.Controls.Add(this.cmbBx_valid, 2, 1);
            this.tableLayoutPanel2.Controls.Add(this.lValid, 2, 0);
            this.tableLayoutPanel2.Controls.Add(this.panel2, 1, 1);
            this.tableLayoutPanel2.Controls.Add(this.panel3, 0, 1);
            this.tableLayoutPanel2.Controls.Add(this.label5, 3, 0);
            this.tableLayoutPanel2.Controls.Add(this.tableLayoutPanel6, 3, 1);
            this.tableLayoutPanel2.Controls.Add(this.longObjectCheckBox, 3, 2);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(3, 88);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 3;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 31F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(744, 164);
            this.tableLayoutPanel2.TabIndex = 16;
            // 
            // lShisft
            // 
            this.lShisft.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lShisft.Location = new System.Drawing.Point(4, 1);
            this.lShisft.Name = "lShisft";
            this.lShisft.Size = new System.Drawing.Size(178, 64);
            this.lShisft.TabIndex = 9;
            this.lShisft.Text = "смещение от пикета";
            this.lShisft.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lMaxTemperature
            // 
            this.lMaxTemperature.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lMaxTemperature.Location = new System.Drawing.Point(189, 1);
            this.lMaxTemperature.Name = "lMaxTemperature";
            this.lMaxTemperature.Size = new System.Drawing.Size(178, 64);
            this.lMaxTemperature.TabIndex = 10;
            this.lMaxTemperature.Text = "Температура (макс.)";
            this.lMaxTemperature.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // cmbBx_valid
            // 
            this.cmbBx_valid.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.cmbBx_valid.FormattingEnabled = true;
            this.cmbBx_valid.Items.AddRange(new object[] {
            "исправно",
            "неисправно"});
            this.cmbBx_valid.Location = new System.Drawing.Point(393, 87);
            this.cmbBx_valid.Name = "cmbBx_valid";
            this.cmbBx_valid.Size = new System.Drawing.Size(140, 21);
            this.cmbBx_valid.TabIndex = 8;
            // 
            // lValid
            // 
            this.lValid.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lValid.Location = new System.Drawing.Point(374, 1);
            this.lValid.Name = "lValid";
            this.lValid.Size = new System.Drawing.Size(178, 64);
            this.lValid.TabIndex = 11;
            this.lValid.Text = "Техническое состояние";
            this.lValid.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.tableLayoutPanel3);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(189, 69);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(178, 58);
            this.panel2.TabIndex = 12;
            // 
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.ColumnCount = 2;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel3.Controls.Add(this.label2, 1, 0);
            this.tableLayoutPanel3.Controls.Add(this.n_MaxTemperature, 0, 0);
            this.tableLayoutPanel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel3.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 1;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.Size = new System.Drawing.Size(178, 58);
            this.tableLayoutPanel3.TabIndex = 14;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label2.Location = new System.Drawing.Point(157, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(18, 58);
            this.label2.TabIndex = 5;
            this.label2.Text = "°C";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // n_MaxTemperature
            // 
            this.n_MaxTemperature.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.n_MaxTemperature.Enabled = false;
            this.n_MaxTemperature.Location = new System.Drawing.Point(25, 19);
            this.n_MaxTemperature.Name = "n_MaxTemperature";
            this.n_MaxTemperature.Size = new System.Drawing.Size(103, 20);
            this.n_MaxTemperature.TabIndex = 6;
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.tableLayoutPanel5);
            this.panel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel3.Location = new System.Drawing.Point(4, 69);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(178, 58);
            this.panel3.TabIndex = 13;
            // 
            // tableLayoutPanel5
            // 
            this.tableLayoutPanel5.ColumnCount = 2;
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel5.Controls.Add(this.label3, 1, 0);
            this.tableLayoutPanel5.Controls.Add(this.n_picketShift, 0, 0);
            this.tableLayoutPanel5.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel5.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel5.Name = "tableLayoutPanel5";
            this.tableLayoutPanel5.RowCount = 1;
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel5.Size = new System.Drawing.Size(178, 58);
            this.tableLayoutPanel5.TabIndex = 0;
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(152, 22);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(23, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "мм";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // n_picketShift
            // 
            this.n_picketShift.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.n_picketShift.Location = new System.Drawing.Point(26, 19);
            this.n_picketShift.Maximum = new decimal(new int[] {
            900000,
            0,
            0,
            0});
            this.n_picketShift.Name = "n_picketShift";
            this.n_picketShift.Size = new System.Drawing.Size(97, 20);
            this.n_picketShift.TabIndex = 14;
            // 
            // label5
            // 
            this.label5.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(631, 26);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(37, 13);
            this.label5.TabIndex = 16;
            this.label5.Text = "длина";
            // 
            // tableLayoutPanel6
            // 
            this.tableLayoutPanel6.ColumnCount = 2;
            this.tableLayoutPanel6.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel6.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel6.Controls.Add(this.numUpDown_equipLenght, 0, 0);
            this.tableLayoutPanel6.Controls.Add(this.label4, 1, 0);
            this.tableLayoutPanel6.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel6.Location = new System.Drawing.Point(559, 69);
            this.tableLayoutPanel6.Name = "tableLayoutPanel6";
            this.tableLayoutPanel6.RowCount = 1;
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel6.Size = new System.Drawing.Size(181, 58);
            this.tableLayoutPanel6.TabIndex = 15;
            // 
            // numUpDown_equipLenght
            // 
            this.numUpDown_equipLenght.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.numUpDown_equipLenght.Enabled = false;
            this.numUpDown_equipLenght.Location = new System.Drawing.Point(3, 19);
            this.numUpDown_equipLenght.Maximum = new decimal(new int[] {
            900000,
            0,
            0,
            0});
            this.numUpDown_equipLenght.Name = "numUpDown_equipLenght";
            this.numUpDown_equipLenght.Size = new System.Drawing.Size(146, 20);
            this.numUpDown_equipLenght.TabIndex = 0;
            // 
            // label4
            // 
            this.label4.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(155, 22);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(23, 13);
            this.label4.TabIndex = 1;
            this.label4.Text = "мм";
            // 
            // longObjectCheckBox
            // 
            this.longObjectCheckBox.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.longObjectCheckBox.AutoSize = true;
            this.longObjectCheckBox.Enabled = false;
            this.longObjectCheckBox.Location = new System.Drawing.Point(559, 138);
            this.longObjectCheckBox.Name = "longObjectCheckBox";
            this.longObjectCheckBox.Size = new System.Drawing.Size(134, 17);
            this.longObjectCheckBox.TabIndex = 17;
            this.longObjectCheckBox.Text = "протяженный объект";
            this.longObjectCheckBox.UseVisualStyleBackColor = true;
            // 
            // label7
            // 
            this.label7.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.label7.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label7.Location = new System.Drawing.Point(3, 257);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(744, 2);
            this.label7.TabIndex = 20;
            // 
            // CreateEquipmentForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(750, 666);
            this.Controls.Add(this.mainTableLayoutPanel);
            this.Name = "CreateEquipmentForm";
            this.Text = "Добавление(редактирование) оборудования";
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.addNewEquipment_MouseMove);
            this.mainTableLayoutPanel.ResumeLayout(false);
            this.mainTableLayoutPanel.PerformLayout();
            this.tableLayoutPanel7.ResumeLayout(false);
            this.tableLayoutPanel7.PerformLayout();
            this.tableLayoutPanel4.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.tableLayoutPanel3.ResumeLayout(false);
            this.tableLayoutPanel3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.n_MaxTemperature)).EndInit();
            this.panel3.ResumeLayout(false);
            this.tableLayoutPanel5.ResumeLayout(false);
            this.tableLayoutPanel5.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.n_picketShift)).EndInit();
            this.tableLayoutPanel6.ResumeLayout(false);
            this.tableLayoutPanel6.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numUpDown_equipLenght)).EndInit();
            this.ResumeLayout(false);

        }
        private System.Windows.Forms.Button Cancel;
        private System.Windows.Forms.Button OK;
        private System.Windows.Forms.TextBox EquipmentInfoTextBox;
        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TableLayoutPanel mainTableLayoutPanel;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel4;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.Label lShisft;
        private System.Windows.Forms.Label lMaxTemperature;
        private System.Windows.Forms.ComboBox cmbBx_valid;
        private System.Windows.Forms.Label lValid;
        private System.Windows.Forms.ComboBox selectEquip;
        private System.Windows.Forms.Label EquipmentInfoLabel;
        private System.Windows.Forms.Integration.ElementHost elementHost1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel5;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown n_picketShift;
        private System.Windows.Forms.NumericUpDown n_MaxTemperature;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel6;
        private System.Windows.Forms.NumericUpDown numUpDown_equipLenght;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.CheckBox longObjectCheckBox;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel7;
        private System.Windows.Forms.Label equipmentNameLabel;
        private System.Windows.Forms.TextBox EquipmentNameTextBox;
    }
}