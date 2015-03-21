namespace Registrator.Equipment
{
    partial class EquipmentFilterNew
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
            this.lstBx_Classes = new System.Windows.Forms.ListBox();
            this.lstBx_Group = new System.Windows.Forms.ListBox();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.btn_Apply = new System.Windows.Forms.Button();
            this.lbl_Classes = new System.Windows.Forms.Label();
            this.lbl_Groups = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.Close = new System.Windows.Forms.Button();
            this.apply = new System.Windows.Forms.Button();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.panel1 = new System.Windows.Forms.Panel();
            this.number = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.classNumber = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.status = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.groups_id = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Class = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Groups = new System.Windows.Forms.DataGridViewTextBoxColumn();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.tableLayoutPanel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // lstBx_Classes
            // 
            this.lstBx_Classes.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lstBx_Classes.FormattingEnabled = true;
            this.lstBx_Classes.Location = new System.Drawing.Point(3, 16);
            this.lstBx_Classes.Name = "lstBx_Classes";
            this.lstBx_Classes.Size = new System.Drawing.Size(279, 226);
            this.lstBx_Classes.TabIndex = 2;
            this.lstBx_Classes.SelectedIndexChanged += new System.EventHandler(this.lstBx_Classes_SelectedIndexChanged);
            // 
            // lstBx_Group
            // 
            this.lstBx_Group.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lstBx_Group.FormattingEnabled = true;
            this.lstBx_Group.Location = new System.Drawing.Point(288, 16);
            this.lstBx_Group.Name = "lstBx_Group";
            this.lstBx_Group.Size = new System.Drawing.Size(280, 226);
            this.lstBx_Group.TabIndex = 3;
            this.lstBx_Group.SelectedIndexChanged += new System.EventHandler(this.lstBx_Group_SelectedIndexChanged);
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.number,
            this.classNumber,
            this.status,
            this.groups_id,
            this.Class,
            this.Groups});
            this.dataGridView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridView1.Location = new System.Drawing.Point(3, 298);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.Size = new System.Drawing.Size(571, 260);
            this.dataGridView1.TabIndex = 4;
            // 
            // btn_Apply
            // 
            this.btn_Apply.Location = new System.Drawing.Point(3, 3);
            this.btn_Apply.Name = "btn_Apply";
            this.btn_Apply.Size = new System.Drawing.Size(129, 31);
            this.btn_Apply.TabIndex = 5;
            this.btn_Apply.Text = "Добавить фильтр";
            this.btn_Apply.UseVisualStyleBackColor = true;
            this.btn_Apply.Click += new System.EventHandler(this.btn_Apply_Click);
            // 
            // lbl_Classes
            // 
            this.lbl_Classes.AutoSize = true;
            this.lbl_Classes.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lbl_Classes.Location = new System.Drawing.Point(3, 0);
            this.lbl_Classes.Name = "lbl_Classes";
            this.lbl_Classes.Size = new System.Drawing.Size(279, 13);
            this.lbl_Classes.TabIndex = 6;
            this.lbl_Classes.Text = "Список классов";
            this.lbl_Classes.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lbl_Groups
            // 
            this.lbl_Groups.AutoSize = true;
            this.lbl_Groups.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lbl_Groups.Location = new System.Drawing.Point(288, 0);
            this.lbl_Groups.Name = "lbl_Groups";
            this.lbl_Groups.Size = new System.Drawing.Size(280, 13);
            this.lbl_Groups.TabIndex = 7;
            this.lbl_Groups.Text = "Список групп";
            this.lbl_Groups.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(138, 3);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(107, 31);
            this.button1.TabIndex = 8;
            this.button1.Text = "Удалить фильтр";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // Close
            // 
            this.Close.Location = new System.Drawing.Point(94, 3);
            this.Close.Name = "Close";
            this.Close.Size = new System.Drawing.Size(75, 23);
            this.Close.TabIndex = 9;
            this.Close.Text = "Закрыть";
            this.Close.UseVisualStyleBackColor = true;
            this.Close.Click += new System.EventHandler(this.Close_Click);
            // 
            // apply
            // 
            this.apply.Location = new System.Drawing.Point(3, 3);
            this.apply.Name = "apply";
            this.apply.Size = new System.Drawing.Size(85, 23);
            this.apply.TabIndex = 10;
            this.apply.Text = "Применить";
            this.apply.UseVisualStyleBackColor = true;
            this.apply.Click += new System.EventHandler(this.button2_Click);
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.panel2, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.dataGridView1, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel2, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.panel1, 0, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 4;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 48.46743F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 51.53257F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.Size = new System.Drawing.Size(577, 596);
            this.tableLayoutPanel1.TabIndex = 11;
            // 
            // panel2
            // 
            this.panel2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.panel2.Controls.Add(this.apply);
            this.panel2.Controls.Add(this.Close);
            this.panel2.Location = new System.Drawing.Point(403, 564);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(171, 28);
            this.panel2.TabIndex = 12;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 2;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel2.Controls.Add(this.lbl_Groups, 1, 0);
            this.tableLayoutPanel2.Controls.Add(this.lstBx_Classes, 0, 1);
            this.tableLayoutPanel2.Controls.Add(this.lbl_Classes, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.lstBx_Group, 1, 1);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(3, 3);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 2;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(571, 245);
            this.tableLayoutPanel2.TabIndex = 12;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.button1);
            this.panel1.Controls.Add(this.btn_Apply);
            this.panel1.Location = new System.Drawing.Point(3, 254);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(257, 38);
            this.panel1.TabIndex = 12;
            // 
            // number
            // 
            this.number.HeaderText = "№";
            this.number.Name = "number";
            this.number.Width = 30;
            // 
            // classNumber
            // 
            this.classNumber.HeaderText = "номер класса";
            this.classNumber.Name = "classNumber";
            this.classNumber.Visible = false;
            // 
            // status
            // 
            this.status.HeaderText = "Статус";
            this.status.Name = "status";
            this.status.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.status.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Automatic;
            this.status.Width = 50;
            // 
            // groups_id
            // 
            this.groups_id.HeaderText = "номера групп";
            this.groups_id.Name = "groups_id";
            this.groups_id.Visible = false;
            // 
            // Class
            // 
            this.Class.HeaderText = "Класс";
            this.Class.Name = "Class";
            this.Class.ReadOnly = true;
            // 
            // Groups
            // 
            this.Groups.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.Groups.HeaderText = "Группы";
            this.Groups.Name = "Groups";
            this.Groups.ReadOnly = true;
            // 
            // EquipmentFilterNew
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(577, 596);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Name = "EquipmentFilterNew";
            this.Text = "Редактор фильтров";
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox lstBx_Classes;
        private System.Windows.Forms.ListBox lstBx_Group;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.Button btn_Apply;
        private System.Windows.Forms.Label lbl_Classes;
        private System.Windows.Forms.Label lbl_Groups;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button Close;
        private System.Windows.Forms.Button apply;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.DataGridViewTextBoxColumn number;
        private System.Windows.Forms.DataGridViewTextBoxColumn classNumber;
        private System.Windows.Forms.DataGridViewCheckBoxColumn status;
        private System.Windows.Forms.DataGridViewTextBoxColumn groups_id;
        private System.Windows.Forms.DataGridViewTextBoxColumn Class;
        private System.Windows.Forms.DataGridViewTextBoxColumn Groups;
    }
}