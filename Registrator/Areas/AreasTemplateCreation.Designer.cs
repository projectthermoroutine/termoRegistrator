namespace Registrator
{
    partial class AreasTemplateCreation
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
            this.label1 = new System.Windows.Forms.Label();
            this.selectedOnly = new System.Windows.Forms.CheckBox();
            this.cancelButton = new System.Windows.Forms.Button();
            this.saveButton = new System.Windows.Forms.Button();
            this.templateName = new System.Windows.Forms.ComboBox();
            this.templatesTableAdapter1 = new Registrator.DB.teplovizorDataSetTableAdapters.templatesTableAdapter();
            this.areasTableAdapter1 = new Registrator.DB.teplovizorDataSetTableAdapters.areasTableAdapter();
            this.templateAreasTableAdapter1 = new Registrator.DB.teplovizorDataSetTableAdapters.TemplateAreasTableAdapter();
            this.template_areasTableAdapter1 = new Registrator.DB.teplovizorDataSetTableAdapters.template_areasTableAdapter();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(130, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Наименование шаблона";
            // 
            // selectedOnly
            // 
            this.selectedOnly.AutoSize = true;
            this.selectedOnly.Location = new System.Drawing.Point(15, 51);
            this.selectedOnly.Name = "selectedOnly";
            this.selectedOnly.Size = new System.Drawing.Size(174, 17);
            this.selectedOnly.TabIndex = 2;
            this.selectedOnly.Text = "Только выделенные области";
            this.selectedOnly.UseVisualStyleBackColor = true;
            this.selectedOnly.CheckedChanged += new System.EventHandler(this.selectedOnly_CheckedChanged);
            // 
            // cancelButton
            // 
            this.cancelButton.Location = new System.Drawing.Point(278, 84);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 3;
            this.cancelButton.Text = "Отмена";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // saveButton
            // 
            this.saveButton.Location = new System.Drawing.Point(197, 84);
            this.saveButton.Name = "saveButton";
            this.saveButton.Size = new System.Drawing.Size(75, 23);
            this.saveButton.TabIndex = 4;
            this.saveButton.Text = "Сохранить";
            this.saveButton.UseVisualStyleBackColor = true;
            this.saveButton.Click += new System.EventHandler(this.saveButton_Click);
            // 
            // templateName
            // 
            this.templateName.FormattingEnabled = true;
            this.templateName.Items.AddRange(new object[] {
            "Новый шаблон"});
            this.templateName.Location = new System.Drawing.Point(15, 25);
            this.templateName.Name = "templateName";
            this.templateName.Size = new System.Drawing.Size(338, 21);
            this.templateName.TabIndex = 5;
            // 
            // templatesTableAdapter1
            // 
            this.templatesTableAdapter1.ClearBeforeFill = true;
            // 
            // areasTableAdapter1
            // 
            this.areasTableAdapter1.ClearBeforeFill = true;
            // 
            // templateAreasTableAdapter1
            // 
            this.templateAreasTableAdapter1.ClearBeforeFill = true;
            // 
            // template_areasTableAdapter1
            // 
            this.template_areasTableAdapter1.ClearBeforeFill = true;
            // 
            // AreasTemplateCreation
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(365, 120);
            this.Controls.Add(this.templateName);
            this.Controls.Add(this.saveButton);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.selectedOnly);
            this.Controls.Add(this.label1);
            this.MaximumSize = new System.Drawing.Size(373, 154);
            this.MinimumSize = new System.Drawing.Size(373, 154);
            this.Name = "AreasTemplateCreation";
            this.Text = "Создание шаблона областей";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox selectedOnly;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.Button saveButton;
        private System.Windows.Forms.ComboBox templateName;
        private DB.teplovizorDataSetTableAdapters.templatesTableAdapter templatesTableAdapter1;
        private DB.teplovizorDataSetTableAdapters.areasTableAdapter areasTableAdapter1;
        private DB.teplovizorDataSetTableAdapters.TemplateAreasTableAdapter templateAreasTableAdapter1;
        private DB.teplovizorDataSetTableAdapters.template_areasTableAdapter template_areasTableAdapter1;
    }
}