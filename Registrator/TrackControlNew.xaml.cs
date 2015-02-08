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
    /// Interaction logic for TrackControlNew.xaml
    /// </summary>
    public partial class TrackControlNew : UserControl
    {
        public TrackControlNew()
        {
            InitializeComponent();
        }
        public void DrawEquipment()
        {
            DrawingVisual drawingVisual = new DrawingVisual();
            DrawingContext drawingContext = drawingVisual.RenderOpen();
            //drawingContext.DrawText(text, new Point(2, 2));
            Brush brush = new SolidColorBrush(Colors.Yellow);
            Pen pen = new Pen(brush, 5.0d);
            drawingContext.DrawEllipse(brush, pen, new Point(30, 30), 12, 12);
            drawingContext.Close();
            
            RenderTargetBitmap bmp = new RenderTargetBitmap(180, 180, 120, 96, PixelFormats.Pbgra32);
            bmp.Render(drawingVisual);
            ImageEquip.Source = bmp;

        }
    }
}
