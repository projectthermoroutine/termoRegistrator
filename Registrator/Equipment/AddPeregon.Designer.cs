namespace Registrator.Equipment
{
    partial class AddPeregon
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
            this.listBox1 = new CustomControls.ListBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.TxtBx_GroupName = new System.Windows.Forms.TextBox();
            this.button2 = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.lstBx_peregon = new System.Windows.Forms.ListBox();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label3 = new System.Windows.Forms.Label();
            this.lbl_PeregonLength = new System.Windows.Forms.Label();
            this.num_PeregonLength = new System.Windows.Forms.NumericUpDown();
            this.tableLayoutPanel1.SuspendLayout();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.num_PeregonLength)).BeginInit();
            this.SuspendLayout();
            // 
            // listBox1
            // 
            this.listBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listBox1.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.listBox1.FormattingEnabled = true;
            this.listBox1.Location = new System.Drawing.Point(5, 26);
            this.listBox1.Margin = new System.Windows.Forms.Padding(5, 3, 3, 3);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new System.Drawing.Size(368, 71);
            this.listBox1.TabIndex = 8;
            this.listBox1.SelectedIndexChanged += new System.EventHandler(this.listBox1_SelectedIndexChanged);
            // 
            // label2
            // 
            this.label2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label2.Location = new System.Drawing.Point(3, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(370, 23);
            this.label2.TabIndex = 11;
            this.label2.Text = "Созданные перегоны";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label1
            // 
            this.label1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label1.Location = new System.Drawing.Point(3, 200);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(370, 20);
            this.label1.TabIndex = 10;
            this.label1.Text = "Введите название нового перегона";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // TxtBx_GroupName
            // 
            this.TxtBx_GroupName.Dock = System.Windows.Forms.DockStyle.Fill;
            this.TxtBx_GroupName.Location = new System.Drawing.Point(3, 223);
            this.TxtBx_GroupName.Name = "TxtBx_GroupName";
            this.TxtBx_GroupName.Size = new System.Drawing.Size(370, 20);
            this.TxtBx_GroupName.TabIndex = 9;
            this.TxtBx_GroupName.TextChanged += new System.EventHandler(this.TxtBx_GroupName_TextChanged_1);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(42, 3);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 7;
            this.button2.Text = "OK";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click_1);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(123, 3);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 6;
            this.button1.Text = "Cancel";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click_1);
            // 
            // lstBx_peregon
            // 
            this.lstBx_peregon.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lstBx_peregon.FormattingEnabled = true;
            this.lstBx_peregon.Location = new System.Drawing.Point(3, 126);
            this.lstBx_peregon.Name = "lstBx_peregon";
            this.lstBx_peregon.Size = new System.Drawing.Size(370, 71);
            this.lstBx_peregon.TabIndex = 12;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.panel1, 0, 8);
            this.tableLayoutPanel1.Controls.Add(this.label3, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.TxtBx_GroupName, 0, 5);
            this.tableLayoutPanel1.Controls.Add(this.label1, 0, 4);
            this.tableLayoutPanel1.Controls.Add(this.label2, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.lstBx_peregon, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.listBox1, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.lbl_PeregonLength, 0, 6);
            this.tableLayoutPanel1.Controls.Add(this.num_PeregonLength, 0, 7);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 9;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 23F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 23F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.Size = new System.Drawing.Size(376, 328);
            this.tableLayoutPanel1.TabIndex = 13;
            // 
            // panel1
            // 
            this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.panel1.Controls.Add(this.button2);
            this.panel1.Controls.Add(this.button1);
            this.panel1.Location = new System.Drawing.Point(171, 295);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(202, 30);
            this.panel1.TabIndex = 14;
            // 
            // label3
            // 
            this.label3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label3.Location = new System.Drawing.Point(3, 100);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(370, 23);
            this.label3.TabIndex = 14;
            this.label3.Text = "Выберите при необходимости второй перегон";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lbl_PeregonLength
            // 
            this.lbl_PeregonLength.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.lbl_PeregonLength.AutoSize = true;
            this.lbl_PeregonLength.Location = new System.Drawing.Point(111, 251);
            this.lbl_PeregonLength.Name = "lbl_PeregonLength";
            this.lbl_PeregonLength.Size = new System.Drawing.Size(154, 13);
            this.lbl_PeregonLength.TabIndex = 15;
            this.lbl_PeregonLength.Text = "Введите длину перегона (см)";
            // 
            // num_PeregonLength
            // 
            this.num_PeregonLength.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.num_PeregonLength.Location = new System.Drawing.Point(128, 272);
            this.num_PeregonLength.Maximum = new decimal(new int[] {
            900000,
            0,
            0,
            0});
            this.num_PeregonLength.Name = "num_PeregonLength";
            this.num_PeregonLength.Size = new System.Drawing.Size(120, 20);
            this.num_PeregonLength.TabIndex = 16;
            // 
            // AddPeregon
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(376, 328);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Name = "AddPeregon";
            this.Text = "Добавление перегона";
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.panel1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.num_PeregonLength)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private CustomControls.ListBox listBox1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox TxtBx_GroupName;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.ListBox lstBx_peregon;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label lbl_PeregonLength;
        private System.Windows.Forms.NumericUpDown num_PeregonLength;
    }
}