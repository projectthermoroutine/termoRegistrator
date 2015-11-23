namespace Registrator
{
    partial class PointsInfoView
    {
        /// <summary> 
        /// Требуется переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором компонентов

        /// <summary> 
        /// Обязательный метод для поддержки конструктора - не изменяйте 
        /// содержимое данного метода при помощи редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PointsInfoView));
            this.PointsInfoViewCtrl = new System.Windows.Forms.ListView();
            this.typeColumn = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.timeColumn = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.lineColumn = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.pathColumn = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.pNoColumn = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.objColumn = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.SuspendLayout();
            // 
            // PointsInfoViewCtrl
            // 
            this.PointsInfoViewCtrl.AllowColumnReorder = true;
            this.PointsInfoViewCtrl.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.typeColumn,
            this.timeColumn,
            this.lineColumn,
            this.pathColumn,
            this.pNoColumn,
            this.objColumn});
            this.PointsInfoViewCtrl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.PointsInfoViewCtrl.FullRowSelect = true;
            this.PointsInfoViewCtrl.GridLines = true;
            this.PointsInfoViewCtrl.Location = new System.Drawing.Point(0, 0);
            this.PointsInfoViewCtrl.MultiSelect = false;
            this.PointsInfoViewCtrl.Name = "PointsInfoViewCtrl";
            this.PointsInfoViewCtrl.Size = new System.Drawing.Size(511, 112);
            this.PointsInfoViewCtrl.SmallImageList = this.imageList1;
            this.PointsInfoViewCtrl.TabIndex = 2;
            this.PointsInfoViewCtrl.UseCompatibleStateImageBehavior = false;
            this.PointsInfoViewCtrl.View = System.Windows.Forms.View.Details;
            this.PointsInfoViewCtrl.DoubleClick += new System.EventHandler(this.PointsInfoViewCtrl_DoubleClick);
            this.PointsInfoViewCtrl.KeyDown += new System.Windows.Forms.KeyEventHandler(this.PointsInfoViewCtrl_KeyDown);
            // 
            // typeColumn
            // 
            this.typeColumn.Text = "";
            this.typeColumn.Width = 40;
            // 
            // timeColumn
            // 
            this.timeColumn.Text = "Время";
            this.timeColumn.Width = 89;
            // 
            // lineColumn
            // 
            this.lineColumn.Text = "Линия";
            // 
            // pathColumn
            // 
            this.pathColumn.Text = "Путь";
            // 
            // pNoColumn
            // 
            this.pNoColumn.Text = "П+С";
            this.pNoColumn.Width = 62;
            // 
            // objColumn
            // 
            this.objColumn.Text = "Объект";
            this.objColumn.Width = 112;
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
            // PointsInfoView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.PointsInfoViewCtrl);
            this.Name = "PointsInfoView";
            this.Size = new System.Drawing.Size(511, 112);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView PointsInfoViewCtrl;
        private System.Windows.Forms.ColumnHeader typeColumn;
        private System.Windows.Forms.ColumnHeader timeColumn;
        private System.Windows.Forms.ColumnHeader lineColumn;
        private System.Windows.Forms.ColumnHeader pathColumn;
        private System.Windows.Forms.ColumnHeader pNoColumn;
        private System.Windows.Forms.ColumnHeader objColumn;
        private System.Windows.Forms.ImageList imageList1;
    }
}
