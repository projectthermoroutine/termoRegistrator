namespace Registrator
{
    partial class AreasPanel
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AreasPanel));
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.loadTemplateButton = new System.Windows.Forms.ToolStripButton();
            this.saveTemplateButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.addRectButton = new System.Windows.Forms.ToolStripButton();
            this.addEllipsButton = new System.Windows.Forms.ToolStripButton();
            this.addFreeButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.deleteButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.settingsButton = new System.Windows.Forms.ToolStripButton();
            this.listView1 = new System.Windows.Forms.ListView();
            this.areaName = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.minTemp = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.maxTemp = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.avrgTemp = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.alarmRules = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolStrip1
            // 
            this.toolStrip1.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadTemplateButton,
            this.saveTemplateButton,
            this.toolStripSeparator2,
            this.addRectButton,
            this.addEllipsButton,
            this.addFreeButton,
            this.toolStripSeparator3,
            this.deleteButton,
            this.toolStripSeparator4,
            this.settingsButton});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(755, 31);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // loadTemplateButton
            // 
            this.loadTemplateButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.loadTemplateButton.Image = ((System.Drawing.Image)(resources.GetObject("loadTemplateButton.Image")));
            this.loadTemplateButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.loadTemplateButton.Name = "loadTemplateButton";
            this.loadTemplateButton.Size = new System.Drawing.Size(28, 28);
            this.loadTemplateButton.Text = "Загрузить шаблон";
            this.loadTemplateButton.Click += new System.EventHandler(this.loadTemplateButton_Click);
            // 
            // saveTemplateButton
            // 
            this.saveTemplateButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.saveTemplateButton.Image = ((System.Drawing.Image)(resources.GetObject("saveTemplateButton.Image")));
            this.saveTemplateButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.saveTemplateButton.Name = "saveTemplateButton";
            this.saveTemplateButton.Size = new System.Drawing.Size(28, 28);
            this.saveTemplateButton.Text = "Сохранить шаблон";
            this.saveTemplateButton.Click += new System.EventHandler(this.saveTemplateButton_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 31);
            // 
            // addRectButton
            // 
            this.addRectButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.addRectButton.Image = ((System.Drawing.Image)(resources.GetObject("addRectButton.Image")));
            this.addRectButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.addRectButton.Name = "addRectButton";
            this.addRectButton.Size = new System.Drawing.Size(28, 28);
            this.addRectButton.Text = "Прямоугольная область";
            this.addRectButton.Click += new System.EventHandler(this.addRectButton_Click);
            // 
            // addEllipsButton
            // 
            this.addEllipsButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.addEllipsButton.Image = ((System.Drawing.Image)(resources.GetObject("addEllipsButton.Image")));
            this.addEllipsButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.addEllipsButton.Name = "addEllipsButton";
            this.addEllipsButton.Size = new System.Drawing.Size(28, 28);
            this.addEllipsButton.Text = "Эллиптическая область";
            this.addEllipsButton.Click += new System.EventHandler(this.addEllipsButton_Click);
            // 
            // addFreeButton
            // 
            this.addFreeButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.addFreeButton.Image = ((System.Drawing.Image)(resources.GetObject("addFreeButton.Image")));
            this.addFreeButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.addFreeButton.Name = "addFreeButton";
            this.addFreeButton.Size = new System.Drawing.Size(28, 28);
            this.addFreeButton.Text = "Произвольная область";
            this.addFreeButton.Click += new System.EventHandler(this.addFreeButton_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 31);
            // 
            // deleteButton
            // 
            this.deleteButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.deleteButton.Enabled = false;
            this.deleteButton.Image = ((System.Drawing.Image)(resources.GetObject("deleteButton.Image")));
            this.deleteButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.deleteButton.Name = "deleteButton";
            this.deleteButton.Size = new System.Drawing.Size(28, 28);
            this.deleteButton.Text = "Удалить область";
            this.deleteButton.Click += new System.EventHandler(this.deleteButton_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(6, 31);
            // 
            // settingsButton
            // 
            this.settingsButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.settingsButton.Enabled = false;
            this.settingsButton.Image = ((System.Drawing.Image)(resources.GetObject("settingsButton.Image")));
            this.settingsButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.settingsButton.Name = "settingsButton";
            this.settingsButton.Size = new System.Drawing.Size(28, 28);
            this.settingsButton.Text = "Настроить область";
            this.settingsButton.Click += new System.EventHandler(this.settingsButton_Click);
            // 
            // listView1
            // 
            this.listView1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.areaName,
            this.minTemp,
            this.maxTemp,
            this.avrgTemp,
            this.alarmRules});
            this.listView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listView1.FullRowSelect = true;
            this.listView1.GridLines = true;
            this.listView1.Location = new System.Drawing.Point(0, 31);
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(755, 235);
            this.listView1.TabIndex = 1;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.Details;
            this.listView1.SelectedIndexChanged += new System.EventHandler(this.listView1_SelectedIndexChanged);
            this.listView1.DoubleClick += new System.EventHandler(this.listView1_DoubleClick);
            // 
            // areaName
            // 
            this.areaName.Text = "Область";
            this.areaName.Width = 168;
            // 
            // minTemp
            // 
            this.minTemp.Text = "Мин. Т, град. С";
            this.minTemp.Width = 95;
            // 
            // maxTemp
            // 
            this.maxTemp.Text = "Макс. Т, град. С";
            this.maxTemp.Width = 99;
            // 
            // avrgTemp
            // 
            this.avrgTemp.Text = "Сред. Т, град. С";
            this.avrgTemp.Width = 100;
            // 
            // alarmRules
            // 
            this.alarmRules.Text = "Оповещения";
            this.alarmRules.Width = 191;
            // 
            // AreasPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(755, 266);
            this.Controls.Add(this.listView1);
            this.Controls.Add(this.toolStrip1);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.Name = "AreasPanel";
            this.Text = "Области";
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton addEllipsButton;
        private System.Windows.Forms.ToolStripButton addFreeButton;
        private System.Windows.Forms.ToolStripButton settingsButton;
        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.ColumnHeader areaName;
        private System.Windows.Forms.ColumnHeader minTemp;
        private System.Windows.Forms.ColumnHeader maxTemp;
        private System.Windows.Forms.ColumnHeader avrgTemp;
        private System.Windows.Forms.ColumnHeader alarmRules;
        private System.Windows.Forms.ToolStripButton loadTemplateButton;
        private System.Windows.Forms.ToolStripButton saveTemplateButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton addRectButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripButton deleteButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
    }
}