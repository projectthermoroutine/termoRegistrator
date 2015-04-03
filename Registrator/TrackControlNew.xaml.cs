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
        public ulong m_curCoord = 0;
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
            trans = new TranslateTransform();
            trans.Y = 0;
            drawingVisual = new DrawingVisual();
            drawingContext = drawingVisual.RenderOpen();
            mySolidColorBrush = new SolidColorBrush();
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
        private int tmp;
        private double mashtab;
        Ellipse e1;
        SolidColorBrush mySolidColorBrush;
        private double count_of_sectors = -1;
        public int duration;
        public ulong lineLength = 0;

        public void Refresh()
        {
            if (dbHelper == null)
                return;
            
            //scale = (int)canvas1.ActualWidth / 400;

            if (displayNewObject)
            {
                canvas1.Children.Clear();
                canvas1.RenderTransform = new TranslateTransform(0, 0);
                count_of_sectors++;

                lock (lockerDB)
                {
                    foreach (var item in dbHelper.subqueryFrame)
                    {
                        double awidth = -canvas2.ActualWidth;
                        canvas1.Margin = new Thickness(0, 0, awidth * 2, 0);
                        mashtab = (4 * 5000) / (canvas1.ActualWidth - canvas1.ActualWidth/3);
                        canvas1.UpdateLayout();
                        e1 = new Ellipse();
                        e1.Width = (int)canvas1.ActualHeight / 10;
                        e1.Height = (int)canvas1.ActualHeight / 10;
                        //mySolidColorBrush = new SolidColorBrush((Color)ColorConverter.ConvertFromString(item.Color));
                        mySolidColorBrush.Color = (Color)ColorConverter.ConvertFromString(item.Color);
                        e1.Fill = mySolidColorBrush;
                        e1.StrokeThickness = 2;
                        e1.Stroke = Brushes.Black;
                        canvas1.Children.Add(e1);

                        double sub;
                        double x;

                        if (duration == 0)
                        {
                            //canvas1.Margin = new Thickness(0, 0, awidth * 2, 0);

                            if (item.shiftLine > m_curCoord)
                            {
                                sub = (double)(item.shiftLine - (m_curCoord / 10)) / (mashtab / 10);
                                x = (canvas1.ActualWidth / 6) + /*(m_curCoord%(4*5000) - count_of_sectors * 4 * 5000)*/ +sub;
                            }
                            else
                            {
                                sub = (double)((m_curCoord / 10) - item.shiftLine) / (mashtab / 10);
                                x = (canvas1.ActualWidth / 6) + /*(m_curCoord%(4*5000) - count_of_sectors * 4 * 5000)*/ -sub;
                            }
                        }
                        else // поезд идет к началу координат
                        {
                            if (item.shiftLine > ((lineLength*10) - m_curCoord))
                            {
                                sub = (double)(item.shiftLine - (lineLength-(m_curCoord / 10))) / (mashtab / 10);
                                x = (canvas1.ActualWidth / 6) + /*(m_curCoord%(4*5000) - count_of_sectors * 4 * 5000)*/ - sub;
                            }
                            else
                            {
                                sub = (double)((lineLength - (m_curCoord / 10)) - item.shiftLine) / (mashtab / 10);
                                x = (canvas1.ActualWidth / 6) + /*(m_curCoord%(4*5000) - count_of_sectors * 4 * 5000)*/ + sub;
                            }
                        }

                        e1.RenderTransform = new TranslateTransform(x, canvas1.ActualHeight - canvas1.ActualHeight * item.Y / 100);
                        lastTransform = 0;
                    }

                    //canvas1.RenderTransform = new TranslateTransform(canvas1.ActualWidth/2 ,0);
                }
            }
            else
            {
                // if (lastTransform != -400)
                {
                    //trans.X = lastTransform * scale;
                    m_curCoord = m_curCoord/*%4*5000*/ - (ulong)count_of_sectors * 4 * 5000;
                    trans.X = -(m_curCoord / mashtab);
                    canvas1.RenderTransform = trans;
                    // lastTransform--;
                }
            }
        }

        public delegate void RefreshDelegate();
    }
    
}
