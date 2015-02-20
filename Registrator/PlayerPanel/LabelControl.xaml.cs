using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Registrator
{
    /// <summary>
    /// Interaction logic for LabelControl.xaml
    /// </summary>
    public partial class LabelControl : UserControl
    {
        public LabelControl()
        {
            InitializeComponent();
        }

        public string Text { get { return (string)label.Content; } set { label.Content = (object)value; } }
    }
}
