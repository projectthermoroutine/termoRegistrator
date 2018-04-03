namespace Registrator.Equipment.CreateDbObjectsCtrls
{
    partial class CreateStrelkaTypeForm
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
            this.label6 = new System.Windows.Forms.Label();
            this.numUpDown_Lenght = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.tableLayoutPanel6 = new System.Windows.Forms.TableLayoutPanel();
            this.cmbBox_strelka = new System.Windows.Forms.ComboBox();
            this.lbl_strelka = new System.Windows.Forms.Label();
            this.tableLayoutPanel7 = new System.Windows.Forms.TableLayoutPanel();
            this.Cancel = new System.Windows.Forms.Button();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.OK = new System.Windows.Forms.Button();
            this.lnewEquipName = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.txtBxName = new System.Windows.Forms.TextBox();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            ((System.ComponentModel.ISupportInitialize)(this.numUpDown_Lenght)).BeginInit();
            this.tableLayoutPanel6.SuspendLayout();
            this.tableLayoutPanel7.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.panel1.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // label6
            // 
            this.label6.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.label6.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label6.Location = new System.Drawing.Point(3, 137);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(363, 2);
            this.label6.TabIndex = 18;
            // 
            // numUpDown_Lenght
            // 
            this.numUpDown_Lenght.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.numUpDown_Lenght.Location = new System.Drawing.Point(3, 15);
            this.numUpDown_Lenght.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.numUpDown_Lenght.Name = "numUpDown_Lenght";
            this.numUpDown_Lenght.Size = new System.Drawing.Size(140, 20);
            this.numUpDown_Lenght.TabIndex = 0;
            // 
            // label4
            // 
            this.label4.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(149, 18);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(21, 13);
            this.label4.TabIndex = 1;
            this.label4.Text = "см";
            // 
            // label5
            // 
            this.label5.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(251, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(37, 13);
            this.label5.TabIndex = 15;
            this.label5.Text = "длина";
            // 
            // tableLayoutPanel6
            // 
            this.tableLayoutPanel6.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.tableLayoutPanel6.ColumnCount = 2;
            this.tableLayoutPanel6.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel6.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 101F));
            this.tableLayoutPanel6.Controls.Add(this.numUpDown_Lenght, 0, 0);
            this.tableLayoutPanel6.Controls.Add(this.label4, 1, 0);
            this.tableLayoutPanel6.Location = new System.Drawing.Point(183, 22);
            this.tableLayoutPanel6.Name = "tableLayoutPanel6";
            this.tableLayoutPanel6.RowCount = 1;
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel6.Size = new System.Drawing.Size(174, 50);
            this.tableLayoutPanel6.TabIndex = 14;
            // 
            // cmbBox_strelka
            // 
            this.cmbBox_strelka.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.cmbBox_strelka.FormattingEnabled = true;
            this.cmbBox_strelka.Items.AddRange(new object[] {
            "левая",
            "правая"});
            this.cmbBox_strelka.Location = new System.Drawing.Point(3, 14);
            this.cmbBox_strelka.Name = "cmbBox_strelka";
            this.cmbBox_strelka.Size = new System.Drawing.Size(140, 21);
            this.cmbBox_strelka.TabIndex = 1;
            // 
            // lbl_strelka
            // 
            this.lbl_strelka.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.lbl_strelka.AutoSize = true;
            this.lbl_strelka.Location = new System.Drawing.Point(51, 0);
            this.lbl_strelka.Name = "lbl_strelka";
            this.lbl_strelka.Size = new System.Drawing.Size(78, 13);
            this.lbl_strelka.TabIndex = 16;
            this.lbl_strelka.Text = "левая/правая";
            // 
            // tableLayoutPanel7
            // 
            this.tableLayoutPanel7.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.tableLayoutPanel7.ColumnCount = 2;
            this.tableLayoutPanel7.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel7.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 21F));
            this.tableLayoutPanel7.Controls.Add(this.cmbBox_strelka, 0, 0);
            this.tableLayoutPanel7.Location = new System.Drawing.Point(6, 22);
            this.tableLayoutPanel7.Name = "tableLayoutPanel7";
            this.tableLayoutPanel7.RowCount = 1;
            this.tableLayoutPanel7.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel7.Size = new System.Drawing.Size(167, 50);
            this.tableLayoutPanel7.TabIndex = 17;
            // 
            // Cancel
            // 
            this.Cancel.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.Cancel.AutoSize = true;
            this.Cancel.Location = new System.Drawing.Point(234, 6);
            this.Cancel.Name = "Cancel";
            this.Cancel.Size = new System.Drawing.Size(75, 23);
            this.Cancel.TabIndex = 0;
            this.Cancel.Text = "Отмена";
            this.Cancel.UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.tableLayoutPanel2.ColumnCount = 2;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 15F));
            this.tableLayoutPanel2.Controls.Add(this.lbl_strelka, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.tableLayoutPanel7, 0, 1);
            this.tableLayoutPanel2.Controls.Add(this.label5, 1, 0);
            this.tableLayoutPanel2.Controls.Add(this.tableLayoutPanel6, 1, 1);
            this.tableLayoutPanel2.Location = new System.Drawing.Point(4, 52);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 2;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.Size = new System.Drawing.Size(360, 82);
            this.tableLayoutPanel2.TabIndex = 16;
            // 
            // OK
            // 
            this.OK.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.OK.Location = new System.Drawing.Point(146, 6);
            this.OK.Name = "OK";
            this.OK.Size = new System.Drawing.Size(82, 22);
            this.OK.TabIndex = 2;
            this.OK.Text = "Применить";
            this.OK.UseVisualStyleBackColor = true;
            this.OK.Click += new System.EventHandler(this.OK_Click);
            // 
            // lnewEquipName
            // 
            this.lnewEquipName.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.lnewEquipName.Location = new System.Drawing.Point(3, 0);
            this.lnewEquipName.Name = "lnewEquipName";
            this.lnewEquipName.Size = new System.Drawing.Size(363, 23);
            this.lnewEquipName.TabIndex = 7;
            this.lnewEquipName.Text = "Введите имя";
            this.lnewEquipName.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // panel1
            // 
            this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.panel1.Controls.Add(this.Cancel);
            this.panel1.Controls.Add(this.OK);
            this.panel1.Location = new System.Drawing.Point(48, 142);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(318, 32);
            this.panel1.TabIndex = 17;
            // 
            // txtBxName
            // 
            this.txtBxName.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtBxName.Location = new System.Drawing.Point(8, 26);
            this.txtBxName.Margin = new System.Windows.Forms.Padding(8, 3, 8, 3);
            this.txtBxName.Name = "txtBxName";
            this.txtBxName.Size = new System.Drawing.Size(353, 20);
            this.txtBxName.TabIndex = 3;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.txtBxName, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.panel1, 0, 6);
            this.tableLayoutPanel1.Controls.Add(this.label6, 0, 5);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel2, 0, 4);
            this.tableLayoutPanel1.Controls.Add(this.lnewEquipName, 0, 2);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 7;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.Size = new System.Drawing.Size(369, 176);
            this.tableLayoutPanel1.TabIndex = 17;
            // 
            // CreateStrelkaTypeForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(369, 176);
            this.Controls.Add(this.tableLayoutPanel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Margin = new System.Windows.Forms.Padding(2);
            this.Name = "CreateStrelkaTypeForm";
            this.Text = "Добавить стрелку";
            ((System.ComponentModel.ISupportInitialize)(this.numUpDown_Lenght)).EndInit();
            this.tableLayoutPanel6.ResumeLayout(false);
            this.tableLayoutPanel6.PerformLayout();
            this.tableLayoutPanel7.ResumeLayout(false);
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.NumericUpDown numUpDown_Lenght;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel6;
        private System.Windows.Forms.ComboBox cmbBox_strelka;
        private System.Windows.Forms.Label lbl_strelka;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel7;
        private System.Windows.Forms.Button Cancel;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.Button OK;
        private System.Windows.Forms.Label lnewEquipName;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TextBox txtBxName;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
    }
}