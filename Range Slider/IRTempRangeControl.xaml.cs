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

namespace IRControls
{
    /// <summary>
    /// Логика взаимодействия для IRTempRangeControl.xaml
    /// </summary>
    public partial class IRTempRangeControl : UserControl
    {

        public event EventHandler<LimitsChangedEvent> LimitsChangedEventHandler;

        //private Color[] palette = new Color[256];

        //public Color[] Palette
        //{
        //    set
        //    {
        //        palette = value;
        //        rebuildScale();
        //    }
        //    get
        //    {
        //        return palette;
        //    }
        //}

        private void rebuildScale()
        {
            Path p = new Path();

            byte[] pixls = new byte[20 * Palette.Length * 4];
            for (int i = 0; i < Palette.Length; i++)
            {
                int ptr=i*20*4;
                byte r = Palette[Palette.Length - i - 1].R;
                byte g = Palette[Palette.Length - i - 1].G;
                byte b = Palette[Palette.Length - i - 1].B;
                for (int j = 0; j < 20; j++)
                {
                    pixls[ptr++] = r;
                    pixls[ptr++] = g;
                    pixls[ptr++] = b;
                    pixls[ptr++] = 0xFF;
                }
            }
            //Image im = new Image();
            PixelFormat pf=PixelFormats.Bgr32;
            int rawStride = (20 * pf.BitsPerPixel + 7) / 8;
            
            BitmapSource bms = BitmapSource.Create(20, Palette.Length, 96, 96, pf, null, pixls, rawStride);
            image1.Source = bms;
            //if(colorsGrid.Children.Count>0) colorsGrid.Children.RemoveRange(0, colorsGrid.Children.Count);
            //Grid.SetColumn(im, 0);
            //im.Width = 20;
            //im.Margin = new Thickness(0, 10, 0, 10);
            //colorsGrid.Children.Add(im);
            
        }

        public IRTempRangeControl()
        {
            InitializeComponent();
            
            rangeSlider1.LimitsChangedEventHandler += LimitsChangedEventFired;
            
            initPalette();
            
            rebuildScale();
        }

        private void initPalette()
        {
            Palette = new Color[256];
            for (int i = 0; i < 256; i++)
            {
                Palette[i] = Color.FromRgb((byte)i, (byte)i, (byte)i);
            }
        }

        public double ScaleTopValue
        {
            get 
            { 
                return (double)GetValue(ScaleTopValueProperty); 
            }

            set 
            { 
                SetValue(ScaleTopValueProperty, value); 
            }
        }

        public static readonly DependencyProperty ScaleTopValueProperty =
            DependencyProperty.Register("ScaleTopValue", typeof(double), typeof(IRTempRangeControl), new UIPropertyMetadata(120d));

        public double ScaleLowerValue
        {
            get 
            { 
                return (double)GetValue(ScaleLowerValueProperty); 
            }

            set
            {
                SetValue(ScaleLowerValueProperty, value);
                this.InvalidateVisual();
            }
        }

        public static readonly DependencyProperty ScaleLowerValueProperty =
            DependencyProperty.Register("ScaleLowerValue", typeof(double), typeof(IRTempRangeControl), new UIPropertyMetadata(-40d));

        public double CalibrationTopValue
        {
            get 
            { 
                return (double)GetValue(CalibrationTopValueProperty); 
            }
            
            set 
            { 
                SetValue(CalibrationTopValueProperty, value); 
            }
        }

        public static readonly DependencyProperty CalibrationTopValueProperty =
            DependencyProperty.Register("CalibrationTopValue", typeof(double), typeof(IRTempRangeControl), new UIPropertyMetadata(120d));

        public double CalibrationLowerValue
        {
            get 
            { 
                return (double)GetValue(CalibrationLowerValueProperty); 
            }
            set
            {
                SetValue(CalibrationLowerValueProperty, value);
                this.InvalidateVisual();
            }
        }

        public static readonly DependencyProperty CalibrationLowerValueProperty =
            DependencyProperty.Register("CalibrationLowerValue", typeof(double), typeof(IRTempRangeControl), new UIPropertyMetadata(-40d));

        public Color[] Palette
        {
            get { return (Color[])GetValue(PaletteProperty); }
            set
            {
                SetValue(PaletteProperty, value);
                rebuildScale();
                this.InvalidateVisual();
                
            }
        }

        public static readonly DependencyProperty PaletteProperty =
            DependencyProperty.Register("Palette", typeof(Color[]), typeof(IRTempRangeControl), new UIPropertyMetadata(new Color[0]));

        public double ObjectTopValue
        {
            get 
            { 
                return (double)GetValue(ObjectTopValueProperty); 
            }
            set 
            { 
                SetValue(ObjectTopValueProperty, value); 
            }
        }

        public static readonly DependencyProperty ObjectTopValueProperty =
            DependencyProperty.Register("ObjectTopValue", typeof(double), typeof(IRTempRangeControl), new UIPropertyMetadata(50d));

        public double ObjectLowerValue
        {
            get 
            { 
                return (double)GetValue(ObjectLowerValueProperty); 
            }
            set
            {
                SetValue(ObjectLowerValueProperty, value);
            }
        }

        public static readonly DependencyProperty ObjectLowerValueProperty =
            DependencyProperty.Register("ObjectLowerValue", typeof(double), typeof(IRTempRangeControl), new UIPropertyMetadata(10d));

        public void LimitsChangedEventFired(object sender, LimitsChangedEvent e)
        {
            FireLimitsChangedEvent(e);
        }

        public void FireLimitsChangedEvent(LimitsChangedEvent e)
        {
            EventHandler<LimitsChangedEvent> handler = LimitsChangedEventHandler;
            if (handler != null)
                handler(this, e);
        }

    }
}
