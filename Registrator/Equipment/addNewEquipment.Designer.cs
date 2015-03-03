namespace Registrator.Equipment
{
    partial class addNewEquipment
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
            this.lstBxAllEquip = new System.Windows.Forms.ListBox();
            this.OK = new System.Windows.Forms.Button();
            this.txtBxName = new System.Windows.Forms.TextBox();
            this.lEquipList = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.cmbBx_selEquip = new System.Windows.Forms.ComboBox();
            this.lnewEquipName = new System.Windows.Forms.Label();
            this.tableLayoutPanel4 = new System.Windows.Forms.TableLayoutPanel();
            this.elementHost1 = new System.Windows.Forms.Integration.ElementHost();
            this.panel1 = new System.Windows.Forms.Panel();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.lShisft = new System.Windows.Forms.Label();
            this.lMaxTemperature = new System.Windows.Forms.Label();
            this.cmbBx_valid = new System.Windows.Forms.ComboBox();
            this.txtBx_MaxTemperature = new System.Windows.Forms.TextBox();
            this.lValid = new System.Windows.Forms.Label();
            this.txtBxShift = new System.Windows.Forms.TextBox();
            this.tableLayoutPanel1.SuspendLayout();
            this.tableLayoutPanel4.SuspendLayout();
            this.panel1.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // Cancel
            // 
            this.Cancel.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.Cancel.AutoSize = true;
            this.Cancel.Location = new System.Drawing.Point(224, 7);
            this.Cancel.Name = "Cancel";
            this.Cancel.Size = new System.Drawing.Size(75, 23);
            this.Cancel.TabIndex = 0;
            this.Cancel.Text = "Cancel";
            this.Cancel.UseVisualStyleBackColor = true;
            this.Cancel.Click += new System.EventHandler(this.Cancel_Click);
            // 
            // lstBxAllEquip
            // 
            this.lstBxAllEquip.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lstBxAllEquip.FormattingEnabled = true;
            this.lstBxAllEquip.Location = new System.Drawing.Point(8, 36);
            this.lstBxAllEquip.Margin = new System.Windows.Forms.Padding(8, 3, 8, 3);
            this.lstBxAllEquip.Name = "lstBxAllEquip";
            this.lstBxAllEquip.Size = new System.Drawing.Size(559, 53);
            this.lstBxAllEquip.TabIndex = 1;
            // 
            // OK
            // 
            this.OK.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.OK.Location = new System.Drawing.Point(147, 7);
            this.OK.Name = "OK";
            this.OK.Size = new System.Drawing.Size(71, 21);
            this.OK.TabIndex = 2;
            this.OK.Text = "OK";
            this.OK.UseVisualStyleBackColor = true;
            this.OK.Click += new System.EventHandler(this.OK_Click);
            // 
            // txtBxName
            // 
            this.txtBxName.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtBxName.Location = new System.Drawing.Point(8, 168);
            this.txtBxName.Margin = new System.Windows.Forms.Padding(8, 3, 8, 3);
            this.txtBxName.Name = "txtBxName";
            this.txtBxName.Size = new System.Drawing.Size(559, 20);
            this.txtBxName.TabIndex = 3;
            // 
            // lEquipList
            // 
            this.lEquipList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lEquipList.Location = new System.Drawing.Point(3, 0);
            this.lEquipList.Name = "lEquipList";
            this.lEquipList.Size = new System.Drawing.Size(569, 33);
            this.lEquipList.TabIndex = 5;
            this.lEquipList.Text = "Список всего оборудования ";
            this.lEquipList.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label1
            // 
            this.label1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label1.Location = new System.Drawing.Point(3, 92);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(569, 23);
            this.label1.TabIndex = 14;
            this.label1.Text = "Список доступного оборудования";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.lEquipList, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.lstBxAllEquip, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.label1, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.cmbBx_selEquip, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.lnewEquipName, 0, 4);
            this.tableLayoutPanel1.Controls.Add(this.txtBxName, 0, 5);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel4, 0, 8);
            this.tableLayoutPanel1.Controls.Add(this.panel1, 0, 9);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel2, 0, 6);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 10;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 300F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.Size = new System.Drawing.Size(575, 588);
            this.tableLayoutPanel1.TabIndex = 15;
            // 
            // cmbBx_selEquip
            // 
            this.cmbBx_selEquip.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cmbBx_selEquip.FormattingEnabled = true;
            this.cmbBx_selEquip.Location = new System.Drawing.Point(8, 118);
            this.cmbBx_selEquip.Margin = new System.Windows.Forms.Padding(8, 3, 8, 3);
            this.cmbBx_selEquip.Name = "cmbBx_selEquip";
            this.cmbBx_selEquip.Size = new System.Drawing.Size(559, 21);
            this.cmbBx_selEquip.TabIndex = 13;
            this.cmbBx_selEquip.SelectedIndexChanged += new System.EventHandler(this.cmbBx_selEquip_SelectedIndexChanged);
            // 
            // lnewEquipName
            // 
            this.lnewEquipName.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lnewEquipName.Location = new System.Drawing.Point(3, 142);
            this.lnewEquipName.Name = "lnewEquipName";
            this.lnewEquipName.Size = new System.Drawing.Size(569, 23);
            this.lnewEquipName.TabIndex = 7;
            this.lnewEquipName.Text = "Введите имя";
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
            this.tableLayoutPanel4.Location = new System.Drawing.Point(3, 253);
            this.tableLayoutPanel4.Name = "tableLayoutPanel4";
            this.tableLayoutPanel4.RowCount = 3;
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 263F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel4.Size = new System.Drawing.Size(569, 294);
            this.tableLayoutPanel4.TabIndex = 16;
            // 
            // elementHost1
            // 
            this.elementHost1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.elementHost1.Location = new System.Drawing.Point(157, 18);
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
            this.panel1.Location = new System.Drawing.Point(254, 553);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(318, 32);
            this.panel1.TabIndex = 17;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 3;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.81295F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 32.85371F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.tableLayoutPanel2.Controls.Add(this.lShisft, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.lMaxTemperature, 1, 0);
            this.tableLayoutPanel2.Controls.Add(this.cmbBx_valid, 2, 1);
            this.tableLayoutPanel2.Controls.Add(this.txtBx_MaxTemperature, 1, 1);
            this.tableLayoutPanel2.Controls.Add(this.lValid, 2, 0);
            this.tableLayoutPanel2.Controls.Add(this.txtBxShift, 0, 1);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(3, 194);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 2;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(569, 53);
            this.tableLayoutPanel2.TabIndex = 16;
            // 
            // lShisft
            // 
            this.lShisft.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lShisft.Location = new System.Drawing.Point(3, 0);
            this.lShisft.Name = "lShisft";
            this.lShisft.Size = new System.Drawing.Size(186, 26);
            this.lShisft.TabIndex = 9;
            this.lShisft.Text = "смещение от пикета";
            this.lShisft.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lMaxTemperature
            // 
            this.lMaxTemperature.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lMaxTemperature.Location = new System.Drawing.Point(195, 0);
            this.lMaxTemperature.Name = "lMaxTemperature";
            this.lMaxTemperature.Size = new System.Drawing.Size(180, 26);
            this.lMaxTemperature.TabIndex = 10;
            this.lMaxTemperature.Text = "Температура (макс.)";
            this.lMaxTemperature.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // cmbBx_valid
            // 
            this.cmbBx_valid.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cmbBx_valid.FormattingEnabled = true;
            this.cmbBx_valid.Items.AddRange(new object[] {
            "неисправно",
            "исправно"});
            this.cmbBx_valid.Location = new System.Drawing.Point(381, 29);
            this.cmbBx_valid.Name = "cmbBx_valid";
            this.cmbBx_valid.Size = new System.Drawing.Size(185, 21);
            this.cmbBx_valid.TabIndex = 8;
            // 
            // txtBx_MaxTemperature
            // 
            this.txtBx_MaxTemperature.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtBx_MaxTemperature.Location = new System.Drawing.Point(195, 29);
            this.txtBx_MaxTemperature.Name = "txtBx_MaxTemperature";
            this.txtBx_MaxTemperature.Size = new System.Drawing.Size(180, 20);
            this.txtBx_MaxTemperature.TabIndex = 4;
            // 
            // lValid
            // 
            this.lValid.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lValid.Location = new System.Drawing.Point(381, 0);
            this.lValid.Name = "lValid";
            this.lValid.Size = new System.Drawing.Size(185, 26);
            this.lValid.TabIndex = 11;
            this.lValid.Text = "Техническое состояние";
            this.lValid.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // txtBxShift
            // 
            this.txtBxShift.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtBxShift.Location = new System.Drawing.Point(3, 29);
            this.txtBxShift.Name = "txtBxShift";
            this.txtBxShift.Size = new System.Drawing.Size(186, 20);
            this.txtBxShift.TabIndex = 6;
            // 
            // addNewEquipment
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(575, 588);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Name = "addNewEquipment";
            this.Text = "Добавление(редактирование) оборудования";
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.tableLayoutPanel4.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
            this.ResumeLayout(false);

        }
        private System.Windows.Forms.Button Cancel;
        private System.Windows.Forms.ListBox lstBxAllEquip;
        private System.Windows.Forms.Button OK;
        private System.Windows.Forms.TextBox txtBxName;
        private System.Windows.Forms.Label lEquipList;
        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel4;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.Label lShisft;
        private System.Windows.Forms.Label lMaxTemperature;
        private System.Windows.Forms.ComboBox cmbBx_valid;
        private System.Windows.Forms.TextBox txtBx_MaxTemperature;
        private System.Windows.Forms.Label lValid;
        private System.Windows.Forms.TextBox txtBxShift;
        private System.Windows.Forms.ComboBox cmbBx_selEquip;
        private System.Windows.Forms.Label lnewEquipName;
        private System.Windows.Forms.Integration.ElementHost elementHost1;
    }
}