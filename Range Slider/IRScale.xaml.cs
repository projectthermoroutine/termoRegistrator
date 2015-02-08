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
using System.Globalization;

namespace IRControls
{
    /// <summary>
    /// Логика взаимодействия для IRScale.xaml
    /// </summary>
    public partial class IRScale : UserControl
    {

        public IRScale()
        {
            InitializeComponent();
        } 

        protected override void OnRender(DrawingContext dc)
        {
            base.OnRender(dc);
            
            int cC = 1;

            double xFinish = base.ActualWidth - 2;//.getWidth();
            double yFinish = base.ActualHeight - 10;
            double xStart = xFinish - base.ActualWidth / 5;
            double yStart = 10;

            //dc.setColor(Color.BLACK);
            
            double[] intervals = getIntervals();

            Pen pen=new Pen(Brushes.Black, 1);
//*
            for (int i = 0; i < intervals.Length; i++)
            {
                double xS = xStart - 5, xF = xS + 5, yS = 0, yF = 0;

                double relativeY = yFinish - yStart;

                yS = ((relativeY * (intervals[i] - LowerValue) / (TopValue-LowerValue)));

                yS = yFinish - yS;

                yF = yS;

                if (yS >= yStart && yS <= yFinish)
                {
                    string pnt=string.Format("{0:F1}", intervals[i]);
                    //String pnt = String.format("%10." + cC + "f", intervals[i]);

                    dc.DrawLine(pen, new Point(xS, yS), new Point(xF, yF));

                    FormattedText txt=new FormattedText(pnt, 
                        CultureInfo.GetCultureInfo("ru-RU"),
                        FlowDirection.LeftToRight,
                        new Typeface("Verdana"),
                        10, System.Windows.Media.Brushes.Black);
                    dc.DrawText(txt, new Point(xS-35, yS-6));
                    //int slen = g.getFontMetrics().stringWidth(pnt) + xStart - 50;
                    //while (slen > xStart - 7)
                    //{
                    //    Font f = g.getFont();
                    //    int fs = g.getFont().getSize();

                    //    f = f.deriveFont((float)(fs - 1));
                    //    g.setFont(f);
                    //    slen = g.getFontMetrics().stringWidth(pnt) + xStart - 50;
                    //}

                    //g.drawString(pnt, xS - 45, yS + 5);
                }

            }
             //*/
        }

        public double[] getIntervals()
        {
            double[] ret = new double[11];

            int cC = 1;

            double step = 0.1F;

            int multiple = -1;

            double start = 0;

            double t = this.TopValue, b = this.LowerValue;

            double dt = t - b;
            if (dt == 0)
            {
                return new double[0];
            }

            step = dt / 10;

            ret = new double[(int)(dt / step) + 1];

            if (Math.Abs((t)) >= Math.Abs((b)))
            {
                if (Math.Abs(t) > 1)
                {
                    t /= 10;
                    multiple++;
                }
                while (Math.Abs(t) > 1)
                {
                    multiple++;
                    t /= 10;
                }



                start = TopValue;//(int)(t*100)/10;//this.top - this.top%step;//t*10;

                while ((start + step) <= TopValue) start += step;

                for (int i = ret.Length - 1; i >= 0; ret[i] = start - i * step, i--) ;

            }

            if (Math.Abs((t)) < Math.Abs((b)))
            {
                if (Math.Abs(b) > 1)
                {
                    b /= 10;
                    multiple++;
                }
                while (Math.Abs(b) > 1)
                {
                    multiple++;
                    b /= 10;
                }

                start = LowerValue;//(int)(b*100)/10;//this.bottom + this.bottom%step;//b*10;

                while ((start - step) >= LowerValue) start -= step;

                for (int i = 0; i < ret.Length; ret[i] = start + i * step, i++) ;

            }

            for (int i = 1; i < ret.Length; i++)
            {
                while ((int)ret[i - 1] == (int)ret[i] && (int)(ret[i - 1] * Math.Pow(10, cC)) == (int)(ret[i] * Math.Pow(10, cC)))
                {
                    cC++;
                }
            }

            return ret;
        }

        public double TopValue
        {
            get { return (double)GetValue(TopValueProperty); }
            set { SetValue(TopValueProperty, value); }
        }

        public static readonly DependencyProperty TopValueProperty =
            DependencyProperty.Register("TopValue", typeof(double), typeof(IRScale),
            new FrameworkPropertyMetadata(120d,
                FrameworkPropertyMetadataOptions.AffectsRender,
                null));

        public double LowerValue
        {
            get { return (double)GetValue(LowerValueProperty); }
            set { 
                SetValue(LowerValueProperty, value);
                this.InvalidateVisual();
            }
        }

        public static readonly DependencyProperty LowerValueProperty =
            DependencyProperty.Register("LowerValue", typeof(double), typeof(IRScale), 
                new FrameworkPropertyMetadata(-40d,
                FrameworkPropertyMetadataOptions.AffectsRender, 
                null));

    }

  
}
