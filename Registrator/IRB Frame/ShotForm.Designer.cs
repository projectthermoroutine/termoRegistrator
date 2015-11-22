namespace Registrator
{
    partial class ShotForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ShotForm));
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.shotToolBarCtrl = new System.Windows.Forms.ToolStrip();
            this.zoomIn = new System.Windows.Forms.ToolStripButton();
            this.zoomOut = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.termoScaleBtn = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.palleteSelectionCtrl = new System.Windows.Forms.ToolStripComboBox();
            this.elementHost1 = new System.Windows.Forms.Integration.ElementHost();
            this.tableLayoutPanel1.SuspendLayout();
            this.shotToolBarCtrl.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.shotToolBarCtrl, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.elementHost1, 0, 2);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 3;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 33F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 22F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(564, 542);
            this.tableLayoutPanel1.TabIndex = 4;
            // 
            // shotToolBarCtrl
            // 
            this.shotToolBarCtrl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.shotToolBarCtrl.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.zoomIn,
            this.zoomOut,
            this.toolStripSeparator1,
            this.toolStripSeparator2,
            this.termoScaleBtn,
            this.toolStripSeparator3,
            this.palleteSelectionCtrl});
            this.shotToolBarCtrl.Location = new System.Drawing.Point(0, 33);
            this.shotToolBarCtrl.Name = "shotToolBarCtrl";
            this.shotToolBarCtrl.Size = new System.Drawing.Size(564, 22);
            this.shotToolBarCtrl.TabIndex = 9;
            this.shotToolBarCtrl.Text = "playerToolBar";
            // 
            // zoomIn
            // 
            this.zoomIn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.zoomIn.Image = global::Registrator.Properties.Resources.zoomIn;
            this.zoomIn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.zoomIn.Name = "zoomIn";
            this.zoomIn.Size = new System.Drawing.Size(23, 19);
            this.zoomIn.Text = "Увеличить";
            this.zoomIn.Click += new System.EventHandler(this.zoomInButton_Click);
            // 
            // zoomOut
            // 
            this.zoomOut.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.zoomOut.Image = global::Registrator.Properties.Resources.zoomOut1;
            this.zoomOut.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.zoomOut.Name = "zoomOut";
            this.zoomOut.Size = new System.Drawing.Size(23, 19);
            this.zoomOut.Text = "Уменьшить";
            this.zoomOut.Click += new System.EventHandler(this.zoomOutButton_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 22);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 22);
            // 
            // termoScaleBtn
            // 
            this.termoScaleBtn.CheckOnClick = true;
            this.termoScaleBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.termoScaleBtn.Image = ((System.Drawing.Image)(resources.GetObject("termoScaleBtn.Image")));
            this.termoScaleBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.termoScaleBtn.Name = "termoScaleBtn";
            this.termoScaleBtn.Size = new System.Drawing.Size(23, 19);
            this.termoScaleBtn.Text = "toolStripButton13";
            this.termoScaleBtn.Click += new System.EventHandler(this.TermoScaleBtn_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 22);
            // 
            // palleteSelectionCtrl
            // 
            this.palleteSelectionCtrl.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.palleteSelectionCtrl.Items.AddRange(new object[] {
            "Черно-белая",
            "Полноцветная",
            "Феррум"});
            this.palleteSelectionCtrl.Name = "palleteSelectionCtrl";
            this.palleteSelectionCtrl.Size = new System.Drawing.Size(121, 22);
            this.palleteSelectionCtrl.Tag = "0";
            this.palleteSelectionCtrl.SelectedIndexChanged += new System.EventHandler(this.palleteSelectionCtrl_SelectedIndexChanged);
            // 
            // elementHost1
            // 
            this.elementHost1.BackColor = System.Drawing.SystemColors.ControlText;
            this.elementHost1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.elementHost1.Location = new System.Drawing.Point(3, 58);
            this.elementHost1.Name = "elementHost1";
            this.elementHost1.Size = new System.Drawing.Size(558, 481);
            this.elementHost1.TabIndex = 1;
            this.elementHost1.Text = "elementHost1";
            this.elementHost1.Child = null;
            // 
            // ShotForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.ClientSize = new System.Drawing.Size(564, 542);
            this.Controls.Add(this.tableLayoutPanel1);
            this.MinimumSize = new System.Drawing.Size(580, 580);
            this.Name = "ShotForm";
            this.Text = "Просмотр снимка";
            this.Load += new System.EventHandler(this.ShotForm_Load);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.shotToolBarCtrl.ResumeLayout(false);
            this.shotToolBarCtrl.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Integration.ElementHost elementHost1;
        private System.Windows.Forms.ToolStrip shotToolBarCtrl;
        private System.Windows.Forms.ToolStripButton zoomIn;
        private System.Windows.Forms.ToolStripButton zoomOut;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton termoScaleBtn;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripComboBox palleteSelectionCtrl;
        
    
    }
}