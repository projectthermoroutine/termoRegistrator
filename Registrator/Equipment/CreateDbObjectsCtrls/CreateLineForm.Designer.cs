namespace Registrator.Equipment.CreateDbObjectsCtrls
{
    partial class CreateLineForm
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
            this.button2 = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.lbl_created = new System.Windows.Forms.Label();
            this.TxtBx_Name = new System.Windows.Forms.TextBox();
            this.lbl_Name = new System.Windows.Forms.Label();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.lbl_number = new System.Windows.Forms.Label();
            this.txtBx_line_code = new System.Windows.Forms.TextBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.name = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Code = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.tableLayoutPanel1.SuspendLayout();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.SuspendLayout();
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(36, 3);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 1;
            this.button2.Text = "Применить";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(117, 3);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 0;
            this.button1.Text = "Отмена";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // lbl_created
            // 
            this.lbl_created.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lbl_created.Location = new System.Drawing.Point(3, 0);
            this.lbl_created.Name = "lbl_created";
            this.lbl_created.Size = new System.Drawing.Size(300, 23);
            this.lbl_created.TabIndex = 5;
            this.lbl_created.Text = "Созданные линии";
            this.lbl_created.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // TxtBx_Name
            // 
            this.TxtBx_Name.Dock = System.Windows.Forms.DockStyle.Fill;
            this.TxtBx_Name.Location = new System.Drawing.Point(8, 324);
            this.TxtBx_Name.Margin = new System.Windows.Forms.Padding(8, 3, 8, 3);
            this.TxtBx_Name.Name = "TxtBx_Name";
            this.TxtBx_Name.Size = new System.Drawing.Size(290, 20);
            this.TxtBx_Name.TabIndex = 3;
            this.TxtBx_Name.TextChanged += new System.EventHandler(this.TxtBx_Name_TextChanged);
            // 
            // lbl_Name
            // 
            this.lbl_Name.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lbl_Name.Location = new System.Drawing.Point(3, 302);
            this.lbl_Name.Name = "lbl_Name";
            this.lbl_Name.Size = new System.Drawing.Size(300, 19);
            this.lbl_Name.TabIndex = 4;
            this.lbl_Name.Text = "Введите название новой линии";
            this.lbl_Name.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.lbl_created, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.lbl_number, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.txtBx_line_code, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.panel1, 0, 8);
            this.tableLayoutPanel1.Controls.Add(this.lbl_Name, 0, 4);
            this.tableLayoutPanel1.Controls.Add(this.TxtBx_Name, 0, 5);
            this.tableLayoutPanel1.Controls.Add(this.dataGridView1, 0, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 9;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.Size = new System.Drawing.Size(306, 384);
            this.tableLayoutPanel1.TabIndex = 8;
            // 
            // lbl_number
            // 
            this.lbl_number.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lbl_number.Location = new System.Drawing.Point(3, 253);
            this.lbl_number.Name = "lbl_number";
            this.lbl_number.Size = new System.Drawing.Size(300, 23);
            this.lbl_number.TabIndex = 8;
            this.lbl_number.Text = "Код линии (Интеграл)";
            this.lbl_number.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // txtBx_line_code
            // 
            this.txtBx_line_code.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.txtBx_line_code.Enabled = false;
            this.txtBx_line_code.Location = new System.Drawing.Point(80, 279);
            this.txtBx_line_code.Name = "txtBx_line_code";
            this.txtBx_line_code.Size = new System.Drawing.Size(146, 20);
            this.txtBx_line_code.TabIndex = 9;
            // 
            // panel1
            // 
            this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.panel1.Controls.Add(this.button2);
            this.panel1.Controls.Add(this.button1);
            this.panel1.Location = new System.Drawing.Point(106, 350);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(197, 31);
            this.panel1.TabIndex = 7;
            // 
            // dataGridView1
            // 
            this.dataGridView1.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.name,
            this.Code});
            this.dataGridView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridView1.Location = new System.Drawing.Point(2, 25);
            this.dataGridView1.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.dataGridView1.MultiSelect = false;
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.RowTemplate.Height = 28;
            this.dataGridView1.Size = new System.Drawing.Size(302, 226);
            this.dataGridView1.TabIndex = 10;
            this.dataGridView1.MouseClick += new System.Windows.Forms.MouseEventHandler(this.dataGridView1_MouseClick);
            // 
            // name
            // 
            this.name.HeaderText = "Название";
            this.name.MinimumWidth = 100;
            this.name.Name = "name";
            this.name.ReadOnly = true;
            // 
            // Code
            // 
            this.Code.HeaderText = "Код линии";
            this.Code.Name = "Code";
            this.Code.ReadOnly = true;
            // 
            // CreateLineForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(306, 384);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Name = "CreateLineForm";
            this.Text = "Добавление линии";
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.panel1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label lbl_created;
        private System.Windows.Forms.TextBox TxtBx_Name;
        private System.Windows.Forms.Label lbl_Name;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Label lbl_number;
        private System.Windows.Forms.TextBox txtBx_line_code;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.DataGridViewTextBoxColumn name;
        private System.Windows.Forms.DataGridViewTextBoxColumn Code;
    }
}