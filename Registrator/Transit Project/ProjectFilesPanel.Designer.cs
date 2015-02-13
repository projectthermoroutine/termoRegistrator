namespace Registrator
{
    partial class ProjectFilesPanel
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ProjectFilesPanel));
            System.Windows.Forms.TreeNode treeNode1 = new System.Windows.Forms.TreeNode("Файлы тепловизора");
            this.toolBarCtrl = new System.Windows.Forms.ToolStrip();
            this.addButton = new System.Windows.Forms.ToolStripButton();
            this.delButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButton3 = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton4 = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.analizeButton = new System.Windows.Forms.ToolStripButton();
            this.fileTreeView = new System.Windows.Forms.TreeView();
            this.toolBarCtrl.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolBarCtrl
            // 
            this.toolBarCtrl.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.toolBarCtrl.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addButton,
            this.delButton,
            this.toolStripSeparator1,
            this.toolStripButton3,
            this.toolStripButton4,
            this.toolStripSeparator2,
            this.analizeButton});
            this.toolBarCtrl.Location = new System.Drawing.Point(0, 0);
            this.toolBarCtrl.Name = "toolBarCtrl";
            this.toolBarCtrl.Size = new System.Drawing.Size(292, 31);
            this.toolBarCtrl.TabIndex = 0;
            this.toolBarCtrl.Text = "toolStrip1";
            // 
            // addButton
            // 
            this.addButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.addButton.Image = ((System.Drawing.Image)(resources.GetObject("addButton.Image")));
            this.addButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.addButton.Name = "addButton";
            this.addButton.Size = new System.Drawing.Size(28, 28);
            this.addButton.Text = "Добавить файл";
            this.addButton.Click += new System.EventHandler(this.addButton_Click);
            // 
            // delButton
            // 
            this.delButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.delButton.Enabled = false;
            this.delButton.Image = ((System.Drawing.Image)(resources.GetObject("delButton.Image")));
            this.delButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.delButton.Name = "delButton";
            this.delButton.Size = new System.Drawing.Size(28, 28);
            this.delButton.Text = "Удалить файл";
            this.delButton.Click += new System.EventHandler(this.delButton_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 31);
            // 
            // toolStripButton3
            // 
            this.toolStripButton3.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton3.Enabled = false;
            this.toolStripButton3.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton3.Image")));
            this.toolStripButton3.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton3.Name = "toolStripButton3";
            this.toolStripButton3.Size = new System.Drawing.Size(28, 28);
            this.toolStripButton3.Text = "Синхронизировать";
            this.toolStripButton3.Visible = false;
            this.toolStripButton3.Click += new System.EventHandler(this.toolStripButton3_Click);
            // 
            // toolStripButton4
            // 
            this.toolStripButton4.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton4.Enabled = false;
            this.toolStripButton4.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton4.Image")));
            this.toolStripButton4.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton4.Name = "toolStripButton4";
            this.toolStripButton4.Size = new System.Drawing.Size(28, 28);
            this.toolStripButton4.Text = "Десинхронизировать";
            this.toolStripButton4.Visible = false;
            this.toolStripButton4.Click += new System.EventHandler(this.toolStripButton4_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 31);
            this.toolStripSeparator2.Visible = false;
            // 
            // analizeButton
            // 
            this.analizeButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.analizeButton.Enabled = false;
            this.analizeButton.Image = ((System.Drawing.Image)(resources.GetObject("analizeButton.Image")));
            this.analizeButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.analizeButton.Name = "analizeButton";
            this.analizeButton.Size = new System.Drawing.Size(51, 28);
            this.analizeButton.Text = "Анализ";
            this.analizeButton.Visible = false;
            this.analizeButton.Click += new System.EventHandler(this.analizeButton_Click);
            // 
            // fileTreeView
            // 
            this.fileTreeView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.fileTreeView.FullRowSelect = true;
            this.fileTreeView.Location = new System.Drawing.Point(0, 31);
            this.fileTreeView.Name = "fileTreeView";
            treeNode1.BackColor = System.Drawing.Color.Transparent;
            treeNode1.Name = "thermoFiles";
            treeNode1.Text = "Файлы тепловизора";
            this.fileTreeView.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode1});
            this.fileTreeView.ShowLines = false;
            this.fileTreeView.ShowNodeToolTips = true;
            this.fileTreeView.Size = new System.Drawing.Size(292, 235);
            this.fileTreeView.TabIndex = 1;
            this.fileTreeView.Click += new System.EventHandler(this.fileTreeView_Click);
            // 
            // ProjectFilesPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 266);
            this.Controls.Add(this.fileTreeView);
            this.Controls.Add(this.toolBarCtrl);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.Name = "ProjectFilesPanel";
            this.TabText = "Файлы проекта";
            this.Text = "Файлы проекта";
            this.toolBarCtrl.ResumeLayout(false);
            this.toolBarCtrl.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ToolStrip toolBarCtrl;
        private System.Windows.Forms.TreeView fileTreeView;
        private System.Windows.Forms.ToolStripButton addButton;
        private System.Windows.Forms.ToolStripButton delButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton toolStripButton3;
        private System.Windows.Forms.ToolStripButton toolStripButton4;
        private System.Windows.Forms.ToolStripButton analizeButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
    }
}