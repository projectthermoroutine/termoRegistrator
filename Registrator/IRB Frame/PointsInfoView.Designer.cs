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
            this.PointsInfoViewCtrl = new System.Windows.Forms.ListView();
            this.typeColumn = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.timeColumn = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.lineColumn = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.pathColumn = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.pNoColumn = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.objColumn = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.SuspendLayout();
            // 
            // PointsInfoViewCtrl
            // 
            this.PointsInfoViewCtrl.AllowColumnReorder = true;
            this.PointsInfoViewCtrl.CheckBoxes = true;
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
            this.PointsInfoViewCtrl.TabIndex = 2;
            this.PointsInfoViewCtrl.UseCompatibleStateImageBehavior = false;
            this.PointsInfoViewCtrl.View = System.Windows.Forms.View.Details;
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
    }
}
