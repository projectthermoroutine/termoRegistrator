namespace Registrator
{
    partial class PlayerPanel
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PlayerPanel));
            this.MoviePane = new System.Windows.Forms.Panel();
            this.elementHost1 = new System.Windows.Forms.Integration.ElementHost();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.fitSize = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.palleteSelectionCtrl = new System.Windows.Forms.ToolStripComboBox();
            this.playerToolBarCtrl = new System.Windows.Forms.ToolStrip();
            this.previewModeButton = new System.Windows.Forms.ToolStripButton();
            this.cameraSrcComboBox = new System.Windows.Forms.ToolStripComboBox();
            this.connectCameraBtn = new System.Windows.Forms.ToolStripButton();
            this.recordButton = new System.Windows.Forms.ToolStripButton();
            this.playPauseButton = new System.Windows.Forms.ToolStripButton();
            this.pauseButton = new System.Windows.Forms.ToolStripButton();
            this.stopButton = new System.Windows.Forms.ToolStripButton();
            this.slowPlayBtn = new System.Windows.Forms.ToolStripButton();
            this.speedLabel = new System.Windows.Forms.ToolStripLabel();
            this.fastPlayBtn = new System.Windows.Forms.ToolStripButton();
            this.zoomIn = new System.Windows.Forms.ToolStripButton();
            this.zoomOut = new System.Windows.Forms.ToolStripButton();
            this.fitDisplay = new System.Windows.Forms.ToolStripButton();
            this.framesFilter = new System.Windows.Forms.ToolStripButton();
            this.termoScaleBtn = new System.Windows.Forms.ToolStripButton();
            this.shotButton = new System.Windows.Forms.ToolStripButton();
            this.cameraSettingsButton = new System.Windows.Forms.ToolStripButton();
            this.elementAreasTableAdapter1 = new Registrator.DB.teplovizorDataSetTableAdapters.ElementAreasTableAdapter();
            this.MoviePane.SuspendLayout();
            this.playerToolBarCtrl.SuspendLayout();
            this.SuspendLayout();
            // 
            // MoviePane
            // 
            this.MoviePane.AutoScroll = true;
            this.MoviePane.BackColor = System.Drawing.SystemColors.Control;
            this.MoviePane.Controls.Add(this.elementHost1);
            this.MoviePane.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MoviePane.ForeColor = System.Drawing.SystemColors.ControlText;
            this.MoviePane.Location = new System.Drawing.Point(0, 25);
            this.MoviePane.Name = "MoviePane";
            this.MoviePane.Size = new System.Drawing.Size(912, 265);
            this.MoviePane.TabIndex = 5;
            // 
            // elementHost1
            // 
            this.elementHost1.BackColor = System.Drawing.SystemColors.ControlText;
            this.elementHost1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.elementHost1.Location = new System.Drawing.Point(0, 0);
            this.elementHost1.Name = "elementHost1";
            this.elementHost1.Size = new System.Drawing.Size(912, 265);
            this.elementHost1.TabIndex = 2;
            this.elementHost1.Text = "elementHost1";
            this.elementHost1.Child = null;
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // fitSize
            // 
            this.fitSize.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.fitSize.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.fitSize.Name = "fitSize";
            this.fitSize.Size = new System.Drawing.Size(85, 22);
            this.fitSize.Text = "Размер кадра";
            this.fitSize.Visible = false;
            this.fitSize.Click += new System.EventHandler(this.fitToFrameSizeBtn_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(6, 25);
            // 
            // palleteSelectionCtrl
            // 
            this.palleteSelectionCtrl.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.palleteSelectionCtrl.Items.AddRange(new object[] {
            "Черно-белая",
            "Полноцветная",
            "Феррум"});
            this.palleteSelectionCtrl.Name = "palleteSelectionCtrl";
            this.palleteSelectionCtrl.Size = new System.Drawing.Size(121, 25);
            this.palleteSelectionCtrl.SelectedIndexChanged += new System.EventHandler(this.palleteSelectionCtrl_SelectedIndexChanged);
            // 
            // playerToolBarCtrl
            // 
            this.playerToolBarCtrl.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.previewModeButton,
            this.cameraSrcComboBox,
            this.connectCameraBtn,
            this.recordButton,
            this.toolStripSeparator1,
            this.playPauseButton,
            this.pauseButton,
            this.stopButton,
            this.slowPlayBtn,
            this.speedLabel,
            this.fastPlayBtn,
            this.toolStripSeparator2,
            this.zoomIn,
            this.zoomOut,
            this.fitSize,
            this.fitDisplay,
            this.toolStripSeparator3,
            this.framesFilter,
            this.toolStripSeparator4,
            this.termoScaleBtn,
            this.toolStripSeparator5,
            this.palleteSelectionCtrl,
            this.shotButton,
            this.cameraSettingsButton});
            this.playerToolBarCtrl.Location = new System.Drawing.Point(0, 0);
            this.playerToolBarCtrl.Name = "playerToolBarCtrl";
            this.playerToolBarCtrl.Size = new System.Drawing.Size(912, 25);
            this.playerToolBarCtrl.TabIndex = 4;
            this.playerToolBarCtrl.Text = "playerToolBar";
            // 
            // previewModeButton
            // 
            this.previewModeButton.CheckOnClick = true;
            this.previewModeButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.previewModeButton.Image = ((System.Drawing.Image)(resources.GetObject("previewModeButton.Image")));
            this.previewModeButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.previewModeButton.Name = "previewModeButton";
            this.previewModeButton.Size = new System.Drawing.Size(23, 22);
            this.previewModeButton.Text = "Камера/запись";
            this.previewModeButton.CheckedChanged += new System.EventHandler(this.recModeButton_CheckedChanged);
            this.previewModeButton.CheckStateChanged += new System.EventHandler(this.recModeButton_CheckStateChanged);
            this.previewModeButton.Click += new System.EventHandler(this.cameraOnOff_Btn_Click);
            // 
            // cameraSrcComboBox
            // 
            this.cameraSrcComboBox.Name = "cameraSrcComboBox";
            this.cameraSrcComboBox.Size = new System.Drawing.Size(121, 25);
            this.cameraSrcComboBox.Visible = false;
            // 
            // connectCameraBtn
            // 
            this.connectCameraBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.connectCameraBtn.Image = ((System.Drawing.Image)(resources.GetObject("connectCameraBtn.Image")));
            this.connectCameraBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.connectCameraBtn.Name = "connectCameraBtn";
            this.connectCameraBtn.Size = new System.Drawing.Size(54, 22);
            this.connectCameraBtn.Text = "connect";
            this.connectCameraBtn.Click += new System.EventHandler(this.connectCameraBtn_Click);
            // 
            // recordButton
            // 
            this.recordButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.recordButton.Enabled = false;
            this.recordButton.Image = ((System.Drawing.Image)(resources.GetObject("recordButton.Image")));
            this.recordButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.recordButton.Name = "recordButton";
            this.recordButton.Size = new System.Drawing.Size(23, 22);
            this.recordButton.Text = "Запись";
            this.recordButton.Click += new System.EventHandler(this.recordBtn_Click);
            // 
            // playPauseButton
            // 
            this.playPauseButton.CheckOnClick = true;
            this.playPauseButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.playPauseButton.Image = ((System.Drawing.Image)(resources.GetObject("playPauseButton.Image")));
            this.playPauseButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.playPauseButton.Name = "playPauseButton";
            this.playPauseButton.Size = new System.Drawing.Size(23, 22);
            this.playPauseButton.Text = "Воспроизвести";
            this.playPauseButton.Click += new System.EventHandler(this.playBtn_Click);
            // 
            // pauseButton
            // 
            this.pauseButton.CheckOnClick = true;
            this.pauseButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.pauseButton.Image = ((System.Drawing.Image)(resources.GetObject("pauseButton.Image")));
            this.pauseButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.pauseButton.Name = "pauseButton";
            this.pauseButton.Size = new System.Drawing.Size(23, 22);
            this.pauseButton.Text = "Пауза";
            this.pauseButton.Click += new System.EventHandler(this.pauseBtn_Click);
            // 
            // stopButton
            // 
            this.stopButton.CheckOnClick = true;
            this.stopButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.stopButton.Image = ((System.Drawing.Image)(resources.GetObject("stopButton.Image")));
            this.stopButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.stopButton.Name = "stopButton";
            this.stopButton.Size = new System.Drawing.Size(23, 22);
            this.stopButton.Text = "Стоп";
            this.stopButton.Click += new System.EventHandler(this.stopBtn_Click);
            // 
            // slowPlayBtn
            // 
            this.slowPlayBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.slowPlayBtn.Image = ((System.Drawing.Image)(resources.GetObject("slowPlayBtn.Image")));
            this.slowPlayBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.slowPlayBtn.Name = "slowPlayBtn";
            this.slowPlayBtn.Size = new System.Drawing.Size(23, 22);
            this.slowPlayBtn.Text = "Назад";
            this.slowPlayBtn.Click += new System.EventHandler(this.slowPlayBtn_Click);
            // 
            // speedLabel
            // 
            this.speedLabel.AutoSize = false;
            this.speedLabel.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.speedLabel.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.speedLabel.Name = "speedLabel";
            this.speedLabel.Size = new System.Drawing.Size(30, 22);
            this.speedLabel.Text = "x1";
            // 
            // fastPlayBtn
            // 
            this.fastPlayBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.fastPlayBtn.Image = ((System.Drawing.Image)(resources.GetObject("fastPlayBtn.Image")));
            this.fastPlayBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.fastPlayBtn.Name = "fastPlayBtn";
            this.fastPlayBtn.Size = new System.Drawing.Size(23, 22);
            this.fastPlayBtn.Text = "Вперед";
            this.fastPlayBtn.Click += new System.EventHandler(this.fastPlayBtn_Click);
            // 
            // zoomIn
            // 
            this.zoomIn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.zoomIn.Image = global::Registrator.Properties.Resources.zoomIn;
            this.zoomIn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.zoomIn.Name = "zoomIn";
            this.zoomIn.Size = new System.Drawing.Size(23, 22);
            this.zoomIn.Text = "Увеличить";
            this.zoomIn.Click += new System.EventHandler(this.zoomInBtn_Click);
            // 
            // zoomOut
            // 
            this.zoomOut.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.zoomOut.Image = global::Registrator.Properties.Resources.zoomOut1;
            this.zoomOut.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.zoomOut.Name = "zoomOut";
            this.zoomOut.Size = new System.Drawing.Size(23, 22);
            this.zoomOut.Text = "Уменьшить";
            this.zoomOut.Click += new System.EventHandler(this.zoomOutBtn_Click);
            // 
            // fitDisplay
            // 
            this.fitDisplay.CheckOnClick = true;
            this.fitDisplay.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.fitDisplay.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.fitDisplay.Name = "fitDisplay";
            this.fitDisplay.Size = new System.Drawing.Size(90, 22);
            this.fitDisplay.Text = "В размер окна";
            this.fitDisplay.Visible = false;
            this.fitDisplay.Click += new System.EventHandler(this.fitToDisplayBtn_Click);
            // 
            // framesFilter
            // 
            this.framesFilter.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.framesFilter.Image = global::Registrator.Properties.Resources.filter1;
            this.framesFilter.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.framesFilter.Name = "framesFilter";
            this.framesFilter.Size = new System.Drawing.Size(23, 22);
            this.framesFilter.Text = "Фильтр кадров";
            this.framesFilter.Click += new System.EventHandler(this.framesFilterBtn_Click);
            // 
            // termoScaleBtn
            // 
            this.termoScaleBtn.CheckOnClick = true;
            this.termoScaleBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.termoScaleBtn.Image = ((System.Drawing.Image)(resources.GetObject("termoScaleBtn.Image")));
            this.termoScaleBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.termoScaleBtn.Name = "termoScaleBtn";
            this.termoScaleBtn.Size = new System.Drawing.Size(23, 22);
            this.termoScaleBtn.Text = "toolStripButton13";
            this.termoScaleBtn.Click += new System.EventHandler(this.TermoScaleBtn_Click);
            // 
            // shotButton
            // 
            this.shotButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.shotButton.Image = global::Registrator.Properties.Resources.bar_savesnapshot;
            this.shotButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.shotButton.Name = "shotButton";
            this.shotButton.Size = new System.Drawing.Size(23, 22);
            this.shotButton.Text = "Стоп-кадр";
            this.shotButton.Click += new System.EventHandler(this.shotButton_Click);
            // 
            // cameraSettingsButton
            // 
            this.cameraSettingsButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.cameraSettingsButton.Enabled = false;
            this.cameraSettingsButton.Image = global::Registrator.Properties.Resources.camctl_options;
            this.cameraSettingsButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.cameraSettingsButton.Name = "cameraSettingsButton";
            this.cameraSettingsButton.Size = new System.Drawing.Size(23, 22);
            this.cameraSettingsButton.Text = "camera settings";
            this.cameraSettingsButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.cameraSettingsButton.Click += new System.EventHandler(this.cameraSettingsBtn_Click);
            // 
            // elementAreasTableAdapter1
            // 
            this.elementAreasTableAdapter1.ClearBeforeFill = true;
            // 
            // PlayerPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.ClientSize = new System.Drawing.Size(912, 290);
            this.Controls.Add(this.MoviePane);
            this.Controls.Add(this.playerToolBarCtrl);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.KeyPreview = true;
            this.Name = "PlayerPanel";
            this.Text = "Просмотр";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.PlayerPanel_FormClosing);
            this.MoviePane.ResumeLayout(false);
            this.playerToolBarCtrl.ResumeLayout(false);
            this.playerToolBarCtrl.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel MoviePane;
        //private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Integration.ElementHost elementHost1;
        private System.Windows.Forms.ToolStripButton previewModeButton;
        private System.Windows.Forms.ToolStripButton recordButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton playPauseButton;
        private System.Windows.Forms.ToolStripButton pauseButton;
        private System.Windows.Forms.ToolStripButton stopButton;
        private System.Windows.Forms.ToolStripButton slowPlayBtn;
        private System.Windows.Forms.ToolStripButton fastPlayBtn;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton zoomIn;
        private System.Windows.Forms.ToolStripButton zoomOut;
        private System.Windows.Forms.ToolStripButton fitSize;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripButton framesFilter;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripButton termoScaleBtn;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripComboBox palleteSelectionCtrl;
        private System.Windows.Forms.ToolStripButton shotButton;
        private System.Windows.Forms.ToolStrip playerToolBarCtrl;
        private System.Windows.Forms.ToolStripButton fitDisplay;
        private System.Windows.Forms.ToolStripLabel speedLabel;
        private System.Windows.Forms.ToolStripButton cameraSettingsButton;
        private DB.teplovizorDataSetTableAdapters.ElementAreasTableAdapter elementAreasTableAdapter1;
        private System.Windows.Forms.ToolStripComboBox cameraSrcComboBox;
        private System.Windows.Forms.ToolStripButton connectCameraBtn;
    }
}