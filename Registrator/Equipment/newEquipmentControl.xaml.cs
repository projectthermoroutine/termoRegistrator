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
        void clickableEllipse_MouseUp(object sender, MouseButtonEventArgs e)
        {
            p = e.GetPosition(grid1);

            equip1.Visibility = System.Windows.Visibility.Visible;
            posTransform = new TranslateTransform();
            posTransform.X = p.X-127;
            posTransform.Y = p.Y-127;
            equip1.RenderTransform = posTransform;


            delegate1((int)posTransform.X,(int)p.Y);
        }

        private void equip1_MouseMove(object sender, MouseEventArgs e)
        {

        }

        private void grid1_MouseMove(object sender, MouseEventArgs e)
        {
            grid1.UpdateLayout();
        }
    }
}
