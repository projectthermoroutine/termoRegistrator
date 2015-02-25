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
        int m_hOffset = 1000;
        public int m_curCoord = 0;
        public int last_coordinat = 0;
        public bool displayNewObject = false;
        DB.DataBaseHelper dbHelper = null;
        public int trackPanelWidth = 0;
        public int trackPanelHeight = 0;

        public DB.DataBaseHelper DB_Helper { set { dbHelper = value; } }

        public TrackControlNew()
        {
            InitializeComponent();
            penTunnel = new Pen(new SolidColorBrush(Colors.Black), 3.0d);
            penCamera = new Pen(new SolidColorBrush(Colors.Yellow), 5.0d);
            brush = new SolidColorBrush(Colors.Yellow);
            pen = new Pen(brush, 5.0d);
            

            drawingVisual = new DrawingVisual();
            drawingContext = drawingVisual.RenderOpen();
        }

        private DrawingVisual drawingVisual;
        private DrawingContext drawingContext;
        private RenderTargetBitmap bmp;
        private Brush brush;
        private Pen pen;
        private Pen penTunnel;
        private Pen penCamera;
        private object lockerDB = new object();
        private TranslateTransform trans;
        private int lastTransform = 0;
        private int scale = 3;
       
        public void Refresh()
        {
            if (dbHelper == null)
                return;

            scale = trackPanelWidth / 60;

            lastTransform -= 1;

            int w1 = (int)ImageEquip.Width;

            if (displayNewObject)
            {
                drawingVisual = new DrawingVisual();
                drawingContext = drawingVisual.RenderOpen();
                
                lock (lockerDB)
                {
                    foreach (var item in dbHelper.subqueryFrame)
                    {
                        brush = new SolidColorBrush((Color)ColorConverter.ConvertFromString(item.Color));
                        pen.Brush = brush;
                        drawingContext.DrawEllipse(brush, pen, new Point((item.shiftLine - m_curCoord) * trackPanelWidth / 60, (trackPanelHeight / 7)), trackPanelHeight / 19, trackPanelHeight / 19);
                        lastTransform = 0;
                    }
                }

                drawingContext.Close();
                bmp = new RenderTargetBitmap(trackPanelWidth, trackPanelHeight/2, 0, 0, PixelFormats.Pbgra32);
                bmp.Render(drawingVisual);
                ImageEquip.Source = bmp;
            }
            else
            {
                if (lastTransform != -60)
                {
                    trans = new TranslateTransform(lastTransform * scale, trackPanelHeight / 7);
                    ImageEquip.RenderTransform = trans;
                }
            }
        }

        public delegate void RefreshDelegate();
    }
    
}
