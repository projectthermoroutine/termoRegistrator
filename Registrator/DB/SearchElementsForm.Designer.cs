namespace Registrator
{
    partial class SearchElementsForm
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
            this.components = new System.ComponentModel.Container();
            this.label1 = new System.Windows.Forms.Label();
            this.searchBox = new System.Windows.Forms.TextBox();
            this.searchButton = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.closeButton = new System.Windows.Forms.Button();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.allEquipmentTableBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.metrocardDataSet1BindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.classesComboBox = new System.Windows.Forms.ComboBox();
            this.groupsComboBox = new System.Windows.Forms.ComboBox();
            this.linesComboBox = new System.Windows.Forms.ComboBox();
            this.peregonComboBox = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.picketUpDown = new System.Windows.Forms.NumericUpDown();
            this.offsUpDown = new System.Windows.Forms.NumericUpDown();
            this.label8 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.pathsComboBox = new System.Windows.Forms.ComboBox();
            this.picketCb = new System.Windows.Forms.CheckBox();
            this.offsCb = new System.Windows.Forms.CheckBox();
            this.ObjName = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.equipID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ClassId = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.GroupId = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.LayoutId = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ObjClass = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ObjGroup = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Line = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Path = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Span = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Picket = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Offs = new System.Windows.Forms.DataGridViewTextBoxColumn();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.allEquipmentTableBindingSource)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.metrocardDataSet1BindingSource)).BeginInit();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picketUpDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.offsUpDown)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(128, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Наименование объекта";
            // 
            // searchBox
            // 
            this.searchBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.searchBox.Location = new System.Drawing.Point(15, 25);
            this.searchBox.Name = "searchBox";
            this.searchBox.Size = new System.Drawing.Size(580, 20);
            this.searchBox.TabIndex = 1;
            this.searchBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.searchBox_KeyPress);
            // 
            // searchButton
            // 
            this.searchButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.searchButton.Location = new System.Drawing.Point(598, 23);
            this.searchButton.Name = "searchButton";
            this.searchButton.Size = new System.Drawing.Size(75, 23);
            this.searchButton.TabIndex = 2;
            this.searchButton.Text = "Поиск";
            this.searchButton.UseVisualStyleBackColor = true;
            this.searchButton.Click += new System.EventHandler(this.searchButton_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 136);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(106, 13);
            this.label2.TabIndex = 17;
            this.label2.Text = "Результаты поиска";
            // 
            // closeButton
            // 
            this.closeButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.closeButton.Location = new System.Drawing.Point(598, 389);
            this.closeButton.Name = "closeButton";
            this.closeButton.Size = new System.Drawing.Size(75, 23);
            this.closeButton.TabIndex = 19;
            this.closeButton.Text = "Закрыть";
            this.closeButton.UseVisualStyleBackColor = true;
            this.closeButton.Click += new System.EventHandler(this.closeButton_Click);
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.AllowUserToDeleteRows = false;
            this.dataGridView1.AllowUserToResizeRows = false;
            this.dataGridView1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.ObjName,
            this.equipID,
            this.ClassId,
            this.GroupId,
            this.LayoutId,
            this.ObjClass,
            this.ObjGroup,
            this.Line,
            this.Path,
            this.Span,
            this.Picket,
            this.Offs});
            this.dataGridView1.EditMode = System.Windows.Forms.DataGridViewEditMode.EditProgrammatically;
            this.dataGridView1.Location = new System.Drawing.Point(15, 152);
            this.dataGridView1.MultiSelect = false;
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.ReadOnly = true;
            this.dataGridView1.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridView1.Size = new System.Drawing.Size(658, 231);
            this.dataGridView1.TabIndex = 20;
            this.dataGridView1.DoubleClick += new System.EventHandler(this.dataGridView1_DoubleClick);
            // 
            // allEquipmentTableBindingSource
            // 
            this.allEquipmentTableBindingSource.DataSource = this.metrocardDataSet1BindingSource;
            // 
            // classesComboBox
            // 
            this.classesComboBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.classesComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.classesComboBox.Items.AddRange(new object[] {
            "не определено"});
            this.classesComboBox.Location = new System.Drawing.Point(49, 14);
            this.classesComboBox.MaxDropDownItems = 20;
            this.classesComboBox.Name = "classesComboBox";
            this.classesComboBox.Size = new System.Drawing.Size(200, 21);
            this.classesComboBox.TabIndex = 21;
            this.classesComboBox.SelectedIndexChanged += new System.EventHandler(this.classesComboBox_SelectedIndexChanged);
            // 
            // groupsComboBox
            // 
            this.groupsComboBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupsComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.groupsComboBox.Items.AddRange(new object[] {
            "не определено"});
            this.groupsComboBox.Location = new System.Drawing.Point(49, 41);
            this.groupsComboBox.MaxDropDownItems = 20;
            this.groupsComboBox.Name = "groupsComboBox";
            this.groupsComboBox.Size = new System.Drawing.Size(200, 21);
            this.groupsComboBox.TabIndex = 22;
            this.groupsComboBox.SelectedIndexChanged += new System.EventHandler(this.groupsComboBox_SelectedIndexChanged);
            // 
            // linesComboBox
            // 
            this.linesComboBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.linesComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.linesComboBox.Items.AddRange(new object[] {
            "не определено"});
            this.linesComboBox.Location = new System.Drawing.Point(300, 13);
            this.linesComboBox.MaxDropDownItems = 20;
            this.linesComboBox.Name = "linesComboBox";
            this.linesComboBox.Size = new System.Drawing.Size(62, 21);
            this.linesComboBox.TabIndex = 23;
            this.linesComboBox.SelectedIndexChanged += new System.EventHandler(this.linesComboBox_SelectedIndexChanged);
            // 
            // peregonComboBox
            // 
            this.peregonComboBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.peregonComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.peregonComboBox.Items.AddRange(new object[] {
            "не определено"});
            this.peregonComboBox.Location = new System.Drawing.Point(434, 13);
            this.peregonComboBox.MaxDropDownItems = 20;
            this.peregonComboBox.Name = "peregonComboBox";
            this.peregonComboBox.Size = new System.Drawing.Size(215, 21);
            this.peregonComboBox.TabIndex = 25;
            this.peregonComboBox.SelectedIndexChanged += new System.EventHandler(this.peregonComboBox_SelectedIndexChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(1, 17);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(45, 13);
            this.label3.TabIndex = 30;
            this.label3.Text = "Служба";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(4, 44);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(42, 13);
            this.label4.TabIndex = 31;
            this.label4.Text = "Группа";
            // 
            // label5
            // 
            this.label5.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(255, 17);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(39, 13);
            this.label5.TabIndex = 32;
            this.label5.Text = "Линия";
            // 
            // label7
            // 
            this.label7.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(375, 17);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(50, 13);
            this.label7.TabIndex = 34;
            this.label7.Text = "Перегон";
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.picketUpDown);
            this.groupBox1.Controls.Add(this.offsUpDown);
            this.groupBox1.Controls.Add(this.classesComboBox);
            this.groupBox1.Controls.Add(this.label8);
            this.groupBox1.Controls.Add(this.groupsComboBox);
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.linesComboBox);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.pathsComboBox);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.peregonComboBox);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.picketCb);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.offsCb);
            this.groupBox1.Location = new System.Drawing.Point(15, 51);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(658, 73);
            this.groupBox1.TabIndex = 36;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Фильтр";
            // 
            // picketUpDown
            // 
            this.picketUpDown.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.picketUpDown.Enabled = false;
            this.picketUpDown.Location = new System.Drawing.Point(434, 42);
            this.picketUpDown.Maximum = new decimal(new int[] {
            3000,
            0,
            0,
            0});
            this.picketUpDown.Minimum = new decimal(new int[] {
            3000,
            0,
            0,
            -2147483648});
            this.picketUpDown.Name = "picketUpDown";
            this.picketUpDown.Size = new System.Drawing.Size(55, 20);
            this.picketUpDown.TabIndex = 40;
            // 
            // offsUpDown
            // 
            this.offsUpDown.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.offsUpDown.Enabled = false;
            this.offsUpDown.Location = new System.Drawing.Point(572, 42);
            this.offsUpDown.Maximum = new decimal(new int[] {
            2000,
            0,
            0,
            0});
            this.offsUpDown.Minimum = new decimal(new int[] {
            2000,
            0,
            0,
            -2147483648});
            this.offsUpDown.Name = "offsUpDown";
            this.offsUpDown.Size = new System.Drawing.Size(56, 20);
            this.offsUpDown.TabIndex = 39;
            // 
            // label8
            // 
            this.label8.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(634, 44);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(15, 13);
            this.label8.TabIndex = 35;
            this.label8.Text = "м";
            // 
            // label6
            // 
            this.label6.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(263, 44);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(31, 13);
            this.label6.TabIndex = 33;
            this.label6.Text = "Путь";
            // 
            // pathsComboBox
            // 
            this.pathsComboBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.pathsComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.pathsComboBox.Items.AddRange(new object[] {
            "не определено"});
            this.pathsComboBox.Location = new System.Drawing.Point(300, 41);
            this.pathsComboBox.MaxDropDownItems = 20;
            this.pathsComboBox.Name = "pathsComboBox";
            this.pathsComboBox.Size = new System.Drawing.Size(62, 21);
            this.pathsComboBox.TabIndex = 24;
            this.pathsComboBox.SelectedIndexChanged += new System.EventHandler(this.pathsComboBox_SelectedIndexChanged);
            // 
            // picketCb
            // 
            this.picketCb.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.picketCb.AutoSize = true;
            this.picketCb.Enabled = false;
            this.picketCb.Location = new System.Drawing.Point(372, 44);
            this.picketCb.Name = "picketCb";
            this.picketCb.Size = new System.Drawing.Size(57, 17);
            this.picketCb.TabIndex = 26;
            this.picketCb.Text = "Пикет";
            this.picketCb.UseVisualStyleBackColor = true;
            this.picketCb.CheckedChanged += new System.EventHandler(this.picketCb_CheckedChanged);
            // 
            // offsCb
            // 
            this.offsCb.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.offsCb.AutoSize = true;
            this.offsCb.Enabled = false;
            this.offsCb.Location = new System.Drawing.Point(495, 43);
            this.offsCb.Name = "offsCb";
            this.offsCb.Size = new System.Drawing.Size(80, 17);
            this.offsCb.TabIndex = 28;
            this.offsCb.Text = "Смещение";
            this.offsCb.UseVisualStyleBackColor = true;
            this.offsCb.CheckedChanged += new System.EventHandler(this.offsCb_CheckedChanged);
            // 
            // ObjName
            // 
            this.ObjName.DataPropertyName = "ObjName";
            this.ObjName.HeaderText = "Наименование";
            this.ObjName.Name = "ObjName";
            this.ObjName.ReadOnly = true;
            // 
            // equipID
            // 
            this.equipID.HeaderText = "equipId";
            this.equipID.Name = "equipID";
            this.equipID.ReadOnly = true;
            this.equipID.Visible = false;
            // 
            // ClassId
            // 
            this.ClassId.HeaderText = "ClassId";
            this.ClassId.Name = "ClassId";
            this.ClassId.ReadOnly = true;
            this.ClassId.Visible = false;
            // 
            // GroupId
            // 
            this.GroupId.HeaderText = "GroupId";
            this.GroupId.Name = "GroupId";
            this.GroupId.ReadOnly = true;
            this.GroupId.Visible = false;
            // 
            // LayoutId
            // 
            this.LayoutId.HeaderText = "LayoutId";
            this.LayoutId.Name = "LayoutId";
            this.LayoutId.ReadOnly = true;
            this.LayoutId.Visible = false;
            // 
            // ObjClass
            // 
            this.ObjClass.DataPropertyName = "ClsName";
            this.ObjClass.HeaderText = "Служба";
            this.ObjClass.Name = "ObjClass";
            this.ObjClass.ReadOnly = true;
            // 
            // ObjGroup
            // 
            this.ObjGroup.DataPropertyName = "GrpName";
            this.ObjGroup.HeaderText = "Группа";
            this.ObjGroup.Name = "ObjGroup";
            this.ObjGroup.ReadOnly = true;
            // 
            // Line
            // 
            this.Line.DataPropertyName = "Line";
            this.Line.HeaderText = "Линия";
            this.Line.Name = "Line";
            this.Line.ReadOnly = true;
            // 
            // Path
            // 
            this.Path.DataPropertyName = "Track";
            this.Path.HeaderText = "Путь";
            this.Path.Name = "Path";
            this.Path.ReadOnly = true;
            // 
            // Span
            // 
            this.Span.DataPropertyName = "LtName";
            this.Span.HeaderText = "Перегон";
            this.Span.Name = "Span";
            this.Span.ReadOnly = true;
            // 
            // Picket
            // 
            this.Picket.DataPropertyName = "Point";
            this.Picket.HeaderText = "Пикет";
            this.Picket.Name = "Picket";
            this.Picket.ReadOnly = true;
            // 
            // Offs
            // 
            this.Offs.DataPropertyName = "Displacement";
            this.Offs.HeaderText = "Смещение";
            this.Offs.Name = "Offs";
            this.Offs.ReadOnly = true;
            // 
            // SearchElementsForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(686, 426);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.closeButton);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.searchButton);
            this.Controls.Add(this.searchBox);
            this.Controls.Add(this.label1);
            this.MinimumSize = new System.Drawing.Size(694, 453);
            this.Name = "SearchElementsForm";
            this.Text = "Поиск";
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.allEquipmentTableBindingSource)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.metrocardDataSet1BindingSource)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picketUpDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.offsUpDown)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox searchBox;
        private System.Windows.Forms.Button searchButton;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button closeButton;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.BindingSource allEquipmentTableBindingSource;
        private System.Windows.Forms.BindingSource metrocardDataSet1BindingSource;
        private System.Windows.Forms.ComboBox classesComboBox;
        private System.Windows.Forms.ComboBox groupsComboBox;
        private System.Windows.Forms.ComboBox linesComboBox;
        private System.Windows.Forms.ComboBox peregonComboBox;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.NumericUpDown picketUpDown;
        private System.Windows.Forms.NumericUpDown offsUpDown;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ComboBox pathsComboBox;
        private System.Windows.Forms.CheckBox picketCb;
        private System.Windows.Forms.CheckBox offsCb;
        private System.Windows.Forms.DataGridViewTextBoxColumn ObjName;
        private System.Windows.Forms.DataGridViewTextBoxColumn equipID;
        private System.Windows.Forms.DataGridViewTextBoxColumn ClassId;
        private System.Windows.Forms.DataGridViewTextBoxColumn GroupId;
        private System.Windows.Forms.DataGridViewTextBoxColumn LayoutId;
        private System.Windows.Forms.DataGridViewTextBoxColumn ObjClass;
        private System.Windows.Forms.DataGridViewTextBoxColumn ObjGroup;
        private System.Windows.Forms.DataGridViewTextBoxColumn Line;
        private System.Windows.Forms.DataGridViewTextBoxColumn Path;
        private System.Windows.Forms.DataGridViewTextBoxColumn Span;
        private System.Windows.Forms.DataGridViewTextBoxColumn Picket;
        private System.Windows.Forms.DataGridViewTextBoxColumn Offs;

    }
}