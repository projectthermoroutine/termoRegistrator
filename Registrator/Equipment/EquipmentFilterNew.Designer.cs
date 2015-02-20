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
            this.apply = new System.Windows.Forms.Button();
            this.Cancel = new System.Windows.Forms.Button();
            this.lstBx_Classes = new System.Windows.Forms.ListBox();
            this.lstBx_Group = new System.Windows.Forms.ListBox();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.btn_Apply = new System.Windows.Forms.Button();
            this.lbl_Classes = new System.Windows.Forms.Label();
            this.lbl_Groups = new System.Windows.Forms.Label();
            this.status = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.Class = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Groups = new System.Windows.Forms.DataGridViewTextBoxColumn();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.SuspendLayout();
            // 
            // apply
            // 
            this.apply.Location = new System.Drawing.Point(405, 505);
            this.apply.Name = "apply";
            this.apply.Size = new System.Drawing.Size(86, 27);
            this.apply.TabIndex = 0;
            this.apply.Text = "Применить";
            this.apply.UseVisualStyleBackColor = true;
            this.apply.Click += new System.EventHandler(this.apply_Click);
            // 
            // Cancel
            // 
            this.Cancel.Location = new System.Drawing.Point(507, 505);
            this.Cancel.Name = "Cancel";
            this.Cancel.Size = new System.Drawing.Size(87, 27);
            this.Cancel.TabIndex = 1;
            this.Cancel.Text = "Отмена";
            this.Cancel.UseVisualStyleBackColor = true;
            this.Cancel.Click += new System.EventHandler(this.Cancel_Click);
            // 
            // lstBx_Classes
            // 
            this.lstBx_Classes.FormattingEnabled = true;
            this.lstBx_Classes.Location = new System.Drawing.Point(23, 52);
            this.lstBx_Classes.Name = "lstBx_Classes";
            this.lstBx_Classes.Size = new System.Drawing.Size(242, 147);
            this.lstBx_Classes.TabIndex = 2;
            this.lstBx_Classes.SelectedIndexChanged += new System.EventHandler(this.lstBx_Classes_SelectedIndexChanged);
            // 
            // lstBx_Group
            // 
            this.lstBx_Group.FormattingEnabled = true;
            this.lstBx_Group.Location = new System.Drawing.Point(312, 52);
            this.lstBx_Group.Name = "lstBx_Group";
            this.lstBx_Group.Size = new System.Drawing.Size(283, 147);
            this.lstBx_Group.TabIndex = 3;
            this.lstBx_Group.SelectedIndexChanged += new System.EventHandler(this.lstBx_Group_SelectedIndexChanged);
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.status,
            this.Class,
            this.Groups});
            this.dataGridView1.Location = new System.Drawing.Point(12, 380);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.ReadOnly = true;
            this.dataGridView1.Size = new System.Drawing.Size(583, 106);
            this.dataGridView1.TabIndex = 4;
            // 
            // btn_Apply
            // 
            this.btn_Apply.Location = new System.Drawing.Point(23, 328);
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
            this.lbl_Classes.Location = new System.Drawing.Point(81, 27);
            this.lbl_Classes.Name = "lbl_Classes";
            this.lbl_Classes.Size = new System.Drawing.Size(89, 13);
            this.lbl_Classes.TabIndex = 6;
            this.lbl_Classes.Text = "Список классов";
            // 
            // lbl_Groups
            // 
            this.lbl_Groups.AutoSize = true;
            this.lbl_Groups.Location = new System.Drawing.Point(416, 27);
            this.lbl_Groups.Name = "lbl_Groups";
            this.lbl_Groups.Size = new System.Drawing.Size(75, 13);
            this.lbl_Groups.TabIndex = 7;
            this.lbl_Groups.Text = "Список групп";
            // 
            // status
            // 
            this.status.HeaderText = "Статус";
            this.status.Name = "status";
            this.status.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.status.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Automatic;
            // 
            // Class
            // 
            this.Class.HeaderText = "Класс";
            this.Class.Name = "Class";
            this.Class.ReadOnly = true;
            // 
            // Groups
            // 
            this.Groups.HeaderText = "Группы";
            this.Groups.Name = "Groups";
            this.Groups.ReadOnly = true;
            // 
            // EquipmentFilterNew
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(607, 544);
            this.Controls.Add(this.lbl_Groups);
            this.Controls.Add(this.lbl_Classes);
            this.Controls.Add(this.btn_Apply);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.lstBx_Group);
            this.Controls.Add(this.lstBx_Classes);
            this.Controls.Add(this.Cancel);
            this.Controls.Add(this.apply);
            this.Name = "EquipmentFilterNew";
            this.Text = "Редактор фильтров";
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button apply;
        private System.Windows.Forms.Button Cancel;
        private System.Windows.Forms.ListBox lstBx_Classes;
        private System.Windows.Forms.ListBox lstBx_Group;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.Button btn_Apply;
        private System.Windows.Forms.Label lbl_Classes;
        private System.Windows.Forms.Label lbl_Groups;
        private System.Windows.Forms.DataGridViewCheckBoxColumn status;
        private System.Windows.Forms.DataGridViewTextBoxColumn Class;
        private System.Windows.Forms.DataGridViewTextBoxColumn Groups;
    }
}