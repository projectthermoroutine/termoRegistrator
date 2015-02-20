using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Registrator
{
    public partial class TemperatureToolTip : ToolTip
    {
        public TemperatureToolTip()
        {
            InitializeComponent();
            this.OwnerDraw = true;
            this.BackColor = Color.Yellow;
            this.Draw += TemperatureToolTip_Draw;
        }

        void TemperatureToolTip_Draw(object sender, DrawToolTipEventArgs e)
        {
            e.DrawBackground();
            e.DrawBorder();
            e.DrawText();
        }
    }
}
