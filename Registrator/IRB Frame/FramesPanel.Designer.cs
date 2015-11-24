namespace Registrator
{
    partial class FramesPanel
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FramesPanel));
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.checkAllButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.ViewShotBtn = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.SaveCheckedShotsBtn = new System.Windows.Forms.ToolStripButton();
            this.deleteCheckedShotsBtn = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.columnsSelection = new System.Windows.Forms.ToolStripSplitButton();
            this.timeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lineToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.pathToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.pNoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.objToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.folderBrowserDialog = new System.Windows.Forms.FolderBrowserDialog();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.checkAllButton,
            this.toolStripSeparator2,
            this.ViewShotBtn,
            this.toolStripSeparator1,
            this.toolStripSeparator4,
            this.SaveCheckedShotsBtn,
            this.deleteCheckedShotsBtn,
            this.toolStripSeparator3,
            this.columnsSelection});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(579, 25);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // checkAllButton
            // 
            this.checkAllButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.checkAllButton.Image = global::Registrator.Properties.Resources.iconCheckAll;
            this.checkAllButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.checkAllButton.Name = "checkAllButton";
            this.checkAllButton.Size = new System.Drawing.Size(23, 22);
            this.checkAllButton.Text = "Отметить все";
            this.checkAllButton.Click += new System.EventHandler(this.checkAllButton_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // ViewShotBtn
            // 
            this.ViewShotBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.ViewShotBtn.Image = ((System.Drawing.Image)(resources.GetObject("ViewShotBtn.Image")));
            this.ViewShotBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ViewShotBtn.Name = "ViewShotBtn";
            this.ViewShotBtn.Size = new System.Drawing.Size(23, 22);
            this.ViewShotBtn.Text = "Просмотреть кадр";
            this.ViewShotBtn.Click += new System.EventHandler(this.ViewShotBtn_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(6, 25);
            // 
            // SaveCheckedShotsBtn
            // 
            this.SaveCheckedShotsBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.SaveCheckedShotsBtn.Image = ((System.Drawing.Image)(resources.GetObject("SaveCheckedShotsBtn.Image")));
            this.SaveCheckedShotsBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.SaveCheckedShotsBtn.Name = "SaveCheckedShotsBtn";
            this.SaveCheckedShotsBtn.Size = new System.Drawing.Size(23, 22);
            this.SaveCheckedShotsBtn.Text = "Сохранить";
            this.SaveCheckedShotsBtn.Click += new System.EventHandler(this.SaveCheckedShotsBtn_Click);
            // 
            // deleteCheckedShotsBtn
            // 
            this.deleteCheckedShotsBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.deleteCheckedShotsBtn.Image = ((System.Drawing.Image)(resources.GetObject("deleteCheckedShotsBtn.Image")));
            this.deleteCheckedShotsBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.deleteCheckedShotsBtn.Name = "deleteCheckedShotsBtn";
            this.deleteCheckedShotsBtn.Size = new System.Drawing.Size(23, 22);
            this.deleteCheckedShotsBtn.Text = "Удалить";
            this.deleteCheckedShotsBtn.Click += new System.EventHandler(this.deleteCheckedShotsBtn_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 25);
            // 
            // columnsSelection
            // 
            this.columnsSelection.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.columnsSelection.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.timeToolStripMenuItem,
            this.lineToolStripMenuItem,
            this.pathToolStripMenuItem,
            this.pNoToolStripMenuItem,
            this.objToolStripMenuItem});
            this.columnsSelection.Image = ((System.Drawing.Image)(resources.GetObject("columnsSelection.Image")));
            this.columnsSelection.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.columnsSelection.Name = "columnsSelection";
            this.columnsSelection.Size = new System.Drawing.Size(32, 22);
            this.columnsSelection.Text = "Колонки";
            // 
            // timeToolStripMenuItem
            // 
            this.timeToolStripMenuItem.Checked = true;
            this.timeToolStripMenuItem.CheckOnClick = true;
            this.timeToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.timeToolStripMenuItem.Name = "timeToolStripMenuItem";
            this.timeToolStripMenuItem.Size = new System.Drawing.Size(175, 22);
            this.timeToolStripMenuItem.Tag = "1";
            this.timeToolStripMenuItem.Text = "Время";
            this.timeToolStripMenuItem.Click += new System.EventHandler(this.ColumnSelectionMenuItem_Click);
            // 
            // lineToolStripMenuItem
            // 
            this.lineToolStripMenuItem.Checked = true;
            this.lineToolStripMenuItem.CheckOnClick = true;
            this.lineToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.lineToolStripMenuItem.Name = "lineToolStripMenuItem";
            this.lineToolStripMenuItem.Size = new System.Drawing.Size(175, 22);
            this.lineToolStripMenuItem.Tag = "2";
            this.lineToolStripMenuItem.Text = "Линия";
            this.lineToolStripMenuItem.Click += new System.EventHandler(this.ColumnSelectionMenuItem_Click);
            // 
            // pathToolStripMenuItem
            // 
            this.pathToolStripMenuItem.Checked = true;
            this.pathToolStripMenuItem.CheckOnClick = true;
            this.pathToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.pathToolStripMenuItem.Name = "pathToolStripMenuItem";
            this.pathToolStripMenuItem.Size = new System.Drawing.Size(175, 22);
            this.pathToolStripMenuItem.Tag = "3";
            this.pathToolStripMenuItem.Text = "Путь";
            this.pathToolStripMenuItem.Click += new System.EventHandler(this.ColumnSelectionMenuItem_Click);
            // 
            // pNoToolStripMenuItem
            // 
            this.pNoToolStripMenuItem.Checked = true;
            this.pNoToolStripMenuItem.CheckOnClick = true;
            this.pNoToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.pNoToolStripMenuItem.Name = "pNoToolStripMenuItem";
            this.pNoToolStripMenuItem.Size = new System.Drawing.Size(175, 22);
            this.pNoToolStripMenuItem.Tag = "4";
            this.pNoToolStripMenuItem.Text = "Пикет+Смещение";
            this.pNoToolStripMenuItem.Click += new System.EventHandler(this.ColumnSelectionMenuItem_Click);
            // 
            // objToolStripMenuItem
            // 
            this.objToolStripMenuItem.Checked = true;
            this.objToolStripMenuItem.CheckOnClick = true;
            this.objToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.objToolStripMenuItem.Name = "objToolStripMenuItem";
            this.objToolStripMenuItem.Size = new System.Drawing.Size(175, 22);
            this.objToolStripMenuItem.Tag = "5";
            this.objToolStripMenuItem.Text = "Объект";
            this.objToolStripMenuItem.Click += new System.EventHandler(this.ColumnSelectionMenuItem_Click);
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "w_g.png");
            this.imageList1.Images.SetKeyName(1, "w_r.png");
            this.imageList1.Images.SetKeyName(2, "w_y.png");
            this.imageList1.Images.SetKeyName(3, "w_b.png");
            // 
            // FramesPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(579, 270);
            this.Controls.Add(this.toolStrip1);
            this.Name = "FramesPanel";
            this.TabText = "Снимки для отчета";
            this.Text = "Снимки для отчета";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.FramesPanel_FormClosed);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton deleteCheckedShotsBtn;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton SaveCheckedShotsBtn;
        private System.Windows.Forms.ImageList imageList1;
        private System.Windows.Forms.ToolStripButton ViewShotBtn;
        private System.Windows.Forms.ToolStripSplitButton columnsSelection;
        private System.Windows.Forms.ToolStripMenuItem timeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem lineToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem pathToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem pNoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem objToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripButton checkAllButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog;
    }
}