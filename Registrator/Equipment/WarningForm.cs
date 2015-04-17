using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Registrator.Equipment
{
    public partial class WarningForm : Form
    {
        public MessageBoxResult Apply = MessageBoxResult.No;
        public WarningForm(string title, string message)
        {
            InitializeComponent();

            this.Text = title;
            this.lbl_warning.Text = message;

        }

        private void btn_apply_Click(object sender, EventArgs e)
        {
            Apply = MessageBoxResult.Yes;

            Dispose();
            Close();
        }

        private void btn_cancel_Click(object sender, EventArgs e)
        {
            Dispose();
            Close();
        }
    }
}
