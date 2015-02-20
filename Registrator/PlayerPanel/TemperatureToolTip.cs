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
    public partial class TemperatureToolTip : Form
    {

        private LabelControl _temperatureToolTip;

        public TemperatureToolTip()
        {
            InitializeComponent();

            _temperatureToolTip = new LabelControl();

            elementHost1.Child = _temperatureToolTip;

        }

        public string Text { get { return _temperatureToolTip.Text; } set { _temperatureToolTip.Text = value; } }


    }
}
