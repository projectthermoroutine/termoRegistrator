namespace Registrator
{
    partial class AreasTemplateSelection
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
            this.templatesLabel = new System.Windows.Forms.Label();
            this.listBox1 = new System.Windows.Forms.ListBox();
            this.cancelButton = new System.Windows.Forms.Button();
            this.loadButton = new System.Windows.Forms.Button();
            this.addButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // templatesLabel
            // 
            this.templatesLabel.AutoSize = true;
            this.templatesLabel.Location = new System.Drawing.Point(12, 9);
            this.templatesLabel.Name = "templatesLabel";
            this.templatesLabel.Size = new System.Drawing.Size(54, 13);
            this.templatesLabel.TabIndex = 0;
            this.templatesLabel.Text = "Шаблоны";
            // 
            // listBox1
            // 
            this.listBox1.FormattingEnabled = true;
            this.listBox1.Location = new System.Drawing.Point(12, 25);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new System.Drawing.Size(303, 212);
            this.listBox1.TabIndex = 1;
            this.listBox1.SelectedIndexChanged += new System.EventHandler(this.listBox1_SelectedIndexChanged);
            // 
            // cancelButton
            // 
            this.cancelButton.Location = new System.Drawing.Point(240, 243);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 2;
            this.cancelButton.Text = "Отменить";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // loadButton
            // 
            this.loadButton.Enabled = false;
            this.loadButton.Location = new System.Drawing.Point(78, 243);
            this.loadButton.Name = "loadButton";
            this.loadButton.Size = new System.Drawing.Size(75, 23);
            this.loadButton.TabIndex = 3;
            this.loadButton.Text = "Загрузить";
            this.loadButton.UseVisualStyleBackColor = true;
            this.loadButton.Click += new System.EventHandler(this.loadButton_Click);
            // 
            // addButton
            // 
            this.addButton.Enabled = false;
            this.addButton.Location = new System.Drawing.Point(159, 243);
            this.addButton.Name = "addButton";
            this.addButton.Size = new System.Drawing.Size(75, 23);
            this.addButton.TabIndex = 4;
            this.addButton.Text = "Добавить";
            this.addButton.UseVisualStyleBackColor = true;
            this.addButton.Click += new System.EventHandler(this.addButton_Click);
            // 
            // AreasTemplateSelection
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(327, 273);
            this.Controls.Add(this.addButton);
            this.Controls.Add(this.loadButton);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.listBox1);
            this.Controls.Add(this.templatesLabel);
            this.MaximumSize = new System.Drawing.Size(335, 307);
            this.MinimumSize = new System.Drawing.Size(335, 307);
            this.Name = "AreasTemplateSelection";
            this.Text = "Выбор шаблона областей";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label templatesLabel;
        private System.Windows.Forms.ListBox listBox1;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.Button loadButton;
        private System.Windows.Forms.Button addButton;
    }
}