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
using Registrator.Equipment.CreateDbObjectsCtrls;

namespace Registrator.Equipment
{
    public partial class TunnelControl : UserControl
    {
        Point p;
       
        DelegateCoordinateEquipment ReturnCoordinateDelegate = null;
        public TunnelControl(DelegateCoordinateEquipment d)
        {
            InitializeComponent();
            ReturnCoordinateDelegate = d;
            axisBegin = new Point(grid1.ActualHeight / 2, grid1.ActualWidth / 2);
        }

        Point axisBegin;

        private void tunnel_MouseUp(object sender, MouseButtonEventArgs e)
        {
            p = e.GetPosition(grid1);

            equip1.Visibility = System.Windows.Visibility.Visible;

            equip1.RenderTransform = new TranslateTransform(p.X - 10, p.Y - 10);

            if (ReturnCoordinateDelegate != null)
                ReturnCoordinateDelegate(
                                (int)(100 * (p.X - axisBegin.X) / grid1.ActualWidth),
                                (int)(100 * (p.Y - axisBegin.Y) / grid1.ActualWidth)
                             );
        }

        public static int GetTunnelWidth() { return 100; } //in percent
    }
}
