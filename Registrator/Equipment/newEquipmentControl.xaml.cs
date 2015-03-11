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

namespace Registrator.Equipment
{
    /// <summary>
    /// Interaction logic for newEquipmentControl.xaml
    /// </summary>
    public partial class newEquipmentControl : UserControl
    {
        TranslateTransform posTransform;
        public Point p;
        DelegateCoordinateEquipmrnt delegate1;
        public newEquipmentControl(DelegateCoordinateEquipmrnt d)
        {
            InitializeComponent();
            delegate1 = d;
        }

        private void grid1_MouseMove(object sender, MouseEventArgs e)
        {
            this.UpdateLayout();
        }

        private void tunnel_MouseUp(object sender, MouseButtonEventArgs e)
        {
            p = e.GetPosition(grid1);

            equip1.Visibility = System.Windows.Visibility.Visible;
            posTransform = new TranslateTransform();
            posTransform.X = p.X-10;
            posTransform.Y = p.Y-10;
            equip1.RenderTransform = posTransform;


            delegate1((int)posTransform.X, (int)p.Y);
        }
    }
}
