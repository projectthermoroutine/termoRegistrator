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
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.cameraToolbarCtrl = new System.Windows.Forms.ToolStrip();
            this.changePlayerModeBtn = new System.Windows.Forms.ToolStripButton();
            this.cameraSrcComboBox = new System.Windows.Forms.ToolStripComboBox();
            this.connectCameraBtn = new System.Windows.Forms.ToolStripButton();
            this.recordButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator6 = new System.Windows.Forms.ToolStripSeparator();
            this.previewCameraBtn = new System.Windows.Forms.ToolStripButton();
            this.recordPreviewBtn = new System.Windows.Forms.ToolStripButton();
            this.stopCameraPreviewBtn = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
            this.cameraPalleteSelectionCtrl = new System.Windows.Forms.ToolStripComboBox();
            this.zoomInBtn = new System.Windows.Forms.ToolStripButton();
            this.zoomOutBtn = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator8 = new System.Windows.Forms.ToolStripSeparator();
            this.cameraTermoScaleBtn = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator10 = new System.Windows.Forms.ToolStripSeparator();
            this.cameraShotButton = new System.Windows.Forms.ToolStripButton();
            this.cameraSettingsButton = new System.Windows.Forms.ToolStripButton();
            this.cameraOffsetBtn = new System.Windows.Forms.ToolStripButton();
            this.playerToolBarCtrl = new System.Windows.Forms.ToolStrip();
            this.previewModeButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.playPauseButton = new System.Windows.Forms.ToolStripButton();
            this.pauseButton = new System.Windows.Forms.ToolStripButton();
            this.stopButton = new System.Windows.Forms.ToolStripButton();
            this.slowPlayBtn = new System.Windows.Forms.ToolStripButton();
            this.speedLabel = new System.Windows.Forms.ToolStripLabel();
            this.fastPlayBtn = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.zoomIn = new System.Windows.Forms.ToolStripButton();
            this.zoomOut = new System.Windows.Forms.ToolStripButton();
            this.fitSize = new System.Windows.Forms.ToolStripButton();
            this.fitDisplay = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.framesFilter = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.termoScaleBtn = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.palleteSelectionCtrl = new System.Windows.Forms.ToolStripComboBox();
            this.shotButton = new System.Windows.Forms.ToolStripButton();
            this.ts_cameraOffset = new System.Windows.Forms.ToolStripButton();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.MoviePane.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.cameraToolbarCtrl.SuspendLayout();
            this.playerToolBarCtrl.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // MoviePane
            // 
            this.MoviePane.AutoScroll = true;
            this.MoviePane.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.MoviePane.BackColor = System.Drawing.SystemColors.Control;
            this.MoviePane.Controls.Add(this.elementHost1);
            this.MoviePane.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MoviePane.ForeColor = System.Drawing.SystemColors.ControlText;
            this.MoviePane.Location = new System.Drawing.Point(0, 50);
            this.MoviePane.Margin = new System.Windows.Forms.Padding(0);
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
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.AutoSize = true;
            this.tableLayoutPanel1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel1.Controls.Add(this.cameraToolbarCtrl, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.playerToolBarCtrl, 0, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.Size = new System.Drawing.Size(912, 50);
            this.tableLayoutPanel1.TabIndex = 9;
            // 
            // cameraToolbarCtrl
            // 
            this.cameraToolbarCtrl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cameraToolbarCtrl.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.changePlayerModeBtn,
            this.cameraSrcComboBox,
            this.connectCameraBtn,
            this.recordButton,
            this.toolStripSeparator6,
            this.previewCameraBtn,
            this.recordPreviewBtn,
            this.stopCameraPreviewBtn,
            this.toolStripSeparator7,
            this.cameraPalleteSelectionCtrl,
            this.zoomInBtn,
            this.zoomOutBtn,
            this.toolStripSeparator8,
            this.cameraTermoScaleBtn,
            this.toolStripSeparator10,
            this.cameraShotButton,
            this.cameraSettingsButton,
            this.cameraOffsetBtn});
            this.cameraToolbarCtrl.Location = new System.Drawing.Point(0, 0);
            this.cameraToolbarCtrl.Name = "cameraToolbarCtrl";
            this.cameraToolbarCtrl.Size = new System.Drawing.Size(912, 25);
            this.cameraToolbarCtrl.TabIndex = 13;
            this.cameraToolbarCtrl.Text = "playerToolBar";
            // 
            // changePlayerModeBtn
            // 
            this.changePlayerModeBtn.CheckOnClick = true;
            this.changePlayerModeBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.changePlayerModeBtn.Image = ((System.Drawing.Image)(resources.GetObject("changePlayerModeBtn.Image")));
            this.changePlayerModeBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.changePlayerModeBtn.Name = "changePlayerModeBtn";
            this.changePlayerModeBtn.Size = new System.Drawing.Size(23, 22);
            this.changePlayerModeBtn.Text = "Камера/Просмотр";
            this.changePlayerModeBtn.Click += new System.EventHandler(this.cameraOnOff_Btn_Click);
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
            // toolStripSeparator6
            // 
            this.toolStripSeparator6.Name = "toolStripSeparator6";
            this.toolStripSeparator6.Size = new System.Drawing.Size(6, 25);
            // 
            // previewCameraBtn
            // 
            this.previewCameraBtn.CheckOnClick = true;
            this.previewCameraBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.previewCameraBtn.Image = ((System.Drawing.Image)(resources.GetObject("previewCameraBtn.Image")));
            this.previewCameraBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.previewCameraBtn.Name = "previewCameraBtn";
            this.previewCameraBtn.Size = new System.Drawing.Size(23, 22);
            this.previewCameraBtn.Text = "Воспроизвести";
            this.previewCameraBtn.Click += new System.EventHandler(this.previewCameraBtn_Click);
            // 
            // recordPreviewBtn
            // 
            this.recordPreviewBtn.CheckOnClick = true;
            this.recordPreviewBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.recordPreviewBtn.Image = ((System.Drawing.Image)(resources.GetObject("recordPreviewBtn.Image")));
            this.recordPreviewBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.recordPreviewBtn.Name = "recordPreviewBtn";
            this.recordPreviewBtn.Size = new System.Drawing.Size(23, 22);
            this.recordPreviewBtn.Text = "Режим просмотра записи";
            this.recordPreviewBtn.Click += new System.EventHandler(this.previewRecordBtn_Click);
            // 
            // stopCameraPreviewBtn
            // 
            this.stopCameraPreviewBtn.CheckOnClick = true;
            this.stopCameraPreviewBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.stopCameraPreviewBtn.Image = ((System.Drawing.Image)(resources.GetObject("stopCameraPreviewBtn.Image")));
            this.stopCameraPreviewBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.stopCameraPreviewBtn.Name = "stopCameraPreviewBtn";
            this.stopCameraPreviewBtn.Size = new System.Drawing.Size(23, 22);
            this.stopCameraPreviewBtn.Text = "Стоп";
            this.stopCameraPreviewBtn.Click += new System.EventHandler(this.stopCameraGrabBtn_Click);
            // 
            // toolStripSeparator7
            // 
            this.toolStripSeparator7.Name = "toolStripSeparator7";
            this.toolStripSeparator7.Size = new System.Drawing.Size(6, 25);
            // 
            // cameraPalleteSelectionCtrl
            // 
            this.cameraPalleteSelectionCtrl.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cameraPalleteSelectionCtrl.Items.AddRange(new object[] {
            "Черно-белая",
            "Полноцветная",
            "Феррум"});
            this.cameraPalleteSelectionCtrl.Name = "cameraPalleteSelectionCtrl";
            this.cameraPalleteSelectionCtrl.Size = new System.Drawing.Size(121, 25);
            this.cameraPalleteSelectionCtrl.Tag = "1";
            this.cameraPalleteSelectionCtrl.SelectedIndexChanged += new System.EventHandler(this.palleteSelectionCtrl_SelectedIndexChanged);
            // 
            // zoomInBtn
            // 
            this.zoomInBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.zoomInBtn.Image = global::Registrator.Properties.Resources.zoomIn;
            this.zoomInBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.zoomInBtn.Name = "zoomInBtn";
            this.zoomInBtn.Size = new System.Drawing.Size(23, 22);
            this.zoomInBtn.Text = "Увеличить";
            this.zoomInBtn.Click += new System.EventHandler(this.zoomInBtn_Click);
            // 
            // zoomOutBtn
            // 
            this.zoomOutBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.zoomOutBtn.Image = global::Registrator.Properties.Resources.zoomOut1;
            this.zoomOutBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.zoomOutBtn.Name = "zoomOutBtn";
            this.zoomOutBtn.Size = new System.Drawing.Size(23, 22);
            this.zoomOutBtn.Text = "Уменьшить";
            this.zoomOutBtn.Click += new System.EventHandler(this.zoomOutBtn_Click);
            // 
            // toolStripSeparator8
            // 
            this.toolStripSeparator8.Name = "toolStripSeparator8";
            this.toolStripSeparator8.Size = new System.Drawing.Size(6, 25);
            // 
            // cameraTermoScaleBtn
            // 
            this.cameraTermoScaleBtn.CheckOnClick = true;
            this.cameraTermoScaleBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.cameraTermoScaleBtn.Image = ((System.Drawing.Image)(resources.GetObject("cameraTermoScaleBtn.Image")));
            this.cameraTermoScaleBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.cameraTermoScaleBtn.Name = "cameraTermoScaleBtn";
            this.cameraTermoScaleBtn.Size = new System.Drawing.Size(23, 22);
            this.cameraTermoScaleBtn.Text = "Температурная шкала";
            this.cameraTermoScaleBtn.Click += new System.EventHandler(this.TermoScaleBtn_Click);
            // 
            // toolStripSeparator10
            // 
            this.toolStripSeparator10.Name = "toolStripSeparator10";
            this.toolStripSeparator10.Size = new System.Drawing.Size(6, 25);
            // 
            // cameraShotButton
            // 
            this.cameraShotButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.cameraShotButton.Image = global::Registrator.Properties.Resources.bar_savesnapshot;
            this.cameraShotButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.cameraShotButton.Name = "cameraShotButton";
            this.cameraShotButton.Size = new System.Drawing.Size(23, 22);
            this.cameraShotButton.Text = "Стоп-кадр";
            this.cameraShotButton.Click += new System.EventHandler(this.shotButton_Click);
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
            // cameraOffsetBtn
            // 
            this.cameraOffsetBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.cameraOffsetBtn.Image = ((System.Drawing.Image)(resources.GetObject("cameraOffsetBtn.Image")));
            this.cameraOffsetBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.cameraOffsetBtn.Name = "cameraOffsetBtn";
            this.cameraOffsetBtn.Size = new System.Drawing.Size(23, 22);
            this.cameraOffsetBtn.Text = "настройка смещения камеры";
            this.cameraOffsetBtn.Click += new System.EventHandler(this.ts_cameraOffset_Click);
            // 
            // playerToolBarCtrl
            // 
            this.playerToolBarCtrl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.playerToolBarCtrl.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.previewModeButton,
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
            this.ts_cameraOffset});
            this.playerToolBarCtrl.Location = new System.Drawing.Point(0, 25);
            this.playerToolBarCtrl.Name = "playerToolBarCtrl";
            this.playerToolBarCtrl.Size = new System.Drawing.Size(912, 25);
            this.playerToolBarCtrl.TabIndex = 9;
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
            this.previewModeButton.Click += new System.EventHandler(this.cameraOnOff_Btn_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
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
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
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
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 25);
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
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(6, 25);
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
            this.palleteSelectionCtrl.Tag = "0";
            this.palleteSelectionCtrl.SelectedIndexChanged += new System.EventHandler(this.palleteSelectionCtrl_SelectedIndexChanged);
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
            // ts_cameraOffset
            // 
            this.ts_cameraOffset.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.ts_cameraOffset.Image = ((System.Drawing.Image)(resources.GetObject("ts_cameraOffset.Image")));
            this.ts_cameraOffset.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ts_cameraOffset.Name = "ts_cameraOffset";
            this.ts_cameraOffset.Size = new System.Drawing.Size(23, 22);
            this.ts_cameraOffset.Text = "настройка смещения камеры";
            this.ts_cameraOffset.Click += new System.EventHandler(this.ts_cameraOffset_Click);
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.AutoSize = true;
            this.tableLayoutPanel2.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.tableLayoutPanel2.ColumnCount = 1;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel2.Controls.Add(this.tableLayoutPanel1, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.MoviePane, 0, 1);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel2.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 2;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.Size = new System.Drawing.Size(912, 290);
            this.tableLayoutPanel2.TabIndex = 3;
            // 
            // PlayerPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.ClientSize = new System.Drawing.Size(912, 290);
            this.Controls.Add(this.tableLayoutPanel2);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.KeyPreview = true;
            this.Name = "PlayerPanel";
            this.Text = "Просмотр";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.PlayerPanel_FormClosing);
            this.MoviePane.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.cameraToolbarCtrl.ResumeLayout(false);
            this.cameraToolbarCtrl.PerformLayout();
            this.playerToolBarCtrl.ResumeLayout(false);
            this.playerToolBarCtrl.PerformLayout();
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel MoviePane;
        private System.Windows.Forms.Integration.ElementHost elementHost1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.ToolStrip playerToolBarCtrl;
        private System.Windows.Forms.ToolStripButton previewModeButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton playPauseButton;
        private System.Windows.Forms.ToolStripButton pauseButton;
        private System.Windows.Forms.ToolStripButton stopButton;
        private System.Windows.Forms.ToolStripButton slowPlayBtn;
        private System.Windows.Forms.ToolStripLabel speedLabel;
        private System.Windows.Forms.ToolStripButton fastPlayBtn;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton zoomIn;
        private System.Windows.Forms.ToolStripButton zoomOut;
        private System.Windows.Forms.ToolStripButton fitSize;
        private System.Windows.Forms.ToolStripButton fitDisplay;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripButton framesFilter;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripButton termoScaleBtn;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripComboBox palleteSelectionCtrl;
        private System.Windows.Forms.ToolStripButton shotButton;
        private System.Windows.Forms.ToolStripButton ts_cameraOffset;
        private System.Windows.Forms.ToolStrip cameraToolbarCtrl;
        private System.Windows.Forms.ToolStripButton changePlayerModeBtn;
        private System.Windows.Forms.ToolStripComboBox cameraSrcComboBox;
        private System.Windows.Forms.ToolStripButton connectCameraBtn;
        private System.Windows.Forms.ToolStripButton recordButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator6;
        private System.Windows.Forms.ToolStripButton previewCameraBtn;
        private System.Windows.Forms.ToolStripButton recordPreviewBtn;
        private System.Windows.Forms.ToolStripButton stopCameraPreviewBtn;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator7;
        private System.Windows.Forms.ToolStripComboBox cameraPalleteSelectionCtrl;
        private System.Windows.Forms.ToolStripButton zoomInBtn;
        private System.Windows.Forms.ToolStripButton zoomOutBtn;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator8;
        private System.Windows.Forms.ToolStripButton cameraTermoScaleBtn;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator10;
        private System.Windows.Forms.ToolStripButton cameraShotButton;
        private System.Windows.Forms.ToolStripButton cameraSettingsButton;
        private System.Windows.Forms.ToolStripButton cameraOffsetBtn;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
    }
}