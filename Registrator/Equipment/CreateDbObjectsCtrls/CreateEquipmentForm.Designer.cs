﻿namespace Registrator.Equipment.CreateDbObjectsCtrls
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
            this.txtBxName = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.label6 = new System.Windows.Forms.Label();
            this.selectEquip = new System.Windows.Forms.ComboBox();
            this.lnewEquipName = new System.Windows.Forms.Label();
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
            this.tableLayoutPanel1.SuspendLayout();
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
            // txtBxName
            // 
            this.txtBxName.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtBxName.Location = new System.Drawing.Point(8, 76);
            this.txtBxName.Margin = new System.Windows.Forms.Padding(8, 3, 8, 3);
            this.txtBxName.Name = "txtBxName";
            this.txtBxName.Size = new System.Drawing.Size(579, 20);
            this.txtBxName.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label1.Location = new System.Drawing.Point(3, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(589, 23);
            this.label1.TabIndex = 14;
            this.label1.Text = "Список доступного оборудования";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.label6, 0, 8);
            this.tableLayoutPanel1.Controls.Add(this.label1, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.selectEquip, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.lnewEquipName, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.txtBxName, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel4, 0, 7);
            this.tableLayoutPanel1.Controls.Add(this.panel1, 0, 8);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel2, 0, 4);
            this.tableLayoutPanel1.Controls.Add(this.label7, 0, 6);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(6, 3, 6, 3);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 9;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 7F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 333F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 36F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(595, 666);
            this.tableLayoutPanel1.TabIndex = 15;
            // 
            // label6
            // 
            this.label6.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.label6.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label6.Location = new System.Drawing.Point(3, 628);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(589, 2);
            this.label6.TabIndex = 19;
            // 
            // selectEquip
            // 
            this.selectEquip.Dock = System.Windows.Forms.DockStyle.Fill;
            this.selectEquip.FormattingEnabled = true;
            this.selectEquip.Location = new System.Drawing.Point(8, 26);
            this.selectEquip.Margin = new System.Windows.Forms.Padding(8, 3, 8, 3);
            this.selectEquip.Name = "selectEquip";
            this.selectEquip.Size = new System.Drawing.Size(579, 21);
            this.selectEquip.TabIndex = 13;
            this.selectEquip.SelectedIndexChanged += new System.EventHandler(this.cmbBx_selEquip_SelectedIndexChanged);
            // 
            // lnewEquipName
            // 
            this.lnewEquipName.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lnewEquipName.Location = new System.Drawing.Point(3, 50);
            this.lnewEquipName.Name = "lnewEquipName";
            this.lnewEquipName.Size = new System.Drawing.Size(589, 23);
            this.lnewEquipName.TabIndex = 7;
            this.lnewEquipName.Text = "Дополнительная информация";
            this.lnewEquipName.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tableLayoutPanel4
            // 
            this.tableLayoutPanel4.ColumnCount = 3;
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 260F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel4.Controls.Add(this.elementHost1, 1, 1);
            this.tableLayoutPanel4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel4.Location = new System.Drawing.Point(3, 298);
            this.tableLayoutPanel4.Name = "tableLayoutPanel4";
            this.tableLayoutPanel4.RowCount = 3;
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 263F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel4.Size = new System.Drawing.Size(589, 327);
            this.tableLayoutPanel4.TabIndex = 16;
            // 
            // elementHost1
            // 
            this.elementHost1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.elementHost1.Location = new System.Drawing.Point(167, 35);
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
            this.panel1.Location = new System.Drawing.Point(274, 633);
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
            this.tableLayoutPanel2.Location = new System.Drawing.Point(3, 102);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 3;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 31F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(589, 183);
            this.tableLayoutPanel2.TabIndex = 16;
            // 
            // lShisft
            // 
            this.lShisft.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lShisft.Location = new System.Drawing.Point(4, 1);
            this.lShisft.Name = "lShisft";
            this.lShisft.Size = new System.Drawing.Size(140, 74);
            this.lShisft.TabIndex = 9;
            this.lShisft.Text = "смещение от пикета";
            this.lShisft.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lMaxTemperature
            // 
            this.lMaxTemperature.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lMaxTemperature.Location = new System.Drawing.Point(151, 1);
            this.lMaxTemperature.Name = "lMaxTemperature";
            this.lMaxTemperature.Size = new System.Drawing.Size(140, 74);
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
            this.cmbBx_valid.Location = new System.Drawing.Point(298, 102);
            this.cmbBx_valid.Name = "cmbBx_valid";
            this.cmbBx_valid.Size = new System.Drawing.Size(140, 21);
            this.cmbBx_valid.TabIndex = 8;
            // 
            // lValid
            // 
            this.lValid.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lValid.Location = new System.Drawing.Point(298, 1);
            this.lValid.Name = "lValid";
            this.lValid.Size = new System.Drawing.Size(140, 74);
            this.lValid.TabIndex = 11;
            this.lValid.Text = "Техническое состояние";
            this.lValid.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.tableLayoutPanel3);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(151, 79);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(140, 68);
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
            this.tableLayoutPanel3.Size = new System.Drawing.Size(140, 68);
            this.tableLayoutPanel3.TabIndex = 14;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label2.Location = new System.Drawing.Point(119, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(18, 68);
            this.label2.TabIndex = 5;
            this.label2.Text = "°C";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // n_MaxTemperature
            // 
            this.n_MaxTemperature.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.n_MaxTemperature.Enabled = false;
            this.n_MaxTemperature.Location = new System.Drawing.Point(6, 24);
            this.n_MaxTemperature.Name = "n_MaxTemperature";
            this.n_MaxTemperature.Size = new System.Drawing.Size(103, 20);
            this.n_MaxTemperature.TabIndex = 6;
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.tableLayoutPanel5);
            this.panel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel3.Location = new System.Drawing.Point(4, 79);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(140, 68);
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
            this.tableLayoutPanel5.Size = new System.Drawing.Size(140, 68);
            this.tableLayoutPanel5.TabIndex = 0;
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(114, 27);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(23, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "мм";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // n_picketShift
            // 
            this.n_picketShift.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.n_picketShift.Location = new System.Drawing.Point(7, 24);
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
            this.label5.Location = new System.Drawing.Point(496, 31);
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
            this.tableLayoutPanel6.Location = new System.Drawing.Point(445, 79);
            this.tableLayoutPanel6.Name = "tableLayoutPanel6";
            this.tableLayoutPanel6.RowCount = 1;
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel6.Size = new System.Drawing.Size(140, 68);
            this.tableLayoutPanel6.TabIndex = 15;
            // 
            // numUpDown_equipLenght
            // 
            this.numUpDown_equipLenght.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.numUpDown_equipLenght.Enabled = false;
            this.numUpDown_equipLenght.Location = new System.Drawing.Point(3, 24);
            this.numUpDown_equipLenght.Maximum = new decimal(new int[] {
            900000,
            0,
            0,
            0});
            this.numUpDown_equipLenght.Name = "numUpDown_equipLenght";
            this.numUpDown_equipLenght.Size = new System.Drawing.Size(105, 20);
            this.numUpDown_equipLenght.TabIndex = 0;
            // 
            // label4
            // 
            this.label4.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(114, 27);
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
            this.longObjectCheckBox.Location = new System.Drawing.Point(445, 158);
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
            this.label7.Location = new System.Drawing.Point(3, 290);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(589, 2);
            this.label7.TabIndex = 20;
            // 
            // CreateEquipmentForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(595, 666);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Name = "CreateEquipmentForm";
            this.Text = "Добавление(редактирование) оборудования";
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.addNewEquipment_MouseMove);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
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
        private System.Windows.Forms.TextBox txtBxName;
        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel4;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.Label lShisft;
        private System.Windows.Forms.Label lMaxTemperature;
        private System.Windows.Forms.ComboBox cmbBx_valid;
        private System.Windows.Forms.Label lValid;
        private System.Windows.Forms.ComboBox selectEquip;
        private System.Windows.Forms.Label lnewEquipName;
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
    }
}