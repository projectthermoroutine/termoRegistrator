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
    /// Interaction logic for RangeSlider.xaml
    /// </summary>
    public partial class RangeSlider : UserControl
    {

        public event EventHandler<LimitsChangedEvent> LimitsChangedEventHandler;

        public RangeSlider()
        {
            InitializeComponent();
            
            this.Loaded += Slider_Loaded;
        }

        void Slider_Loaded(object sender, RoutedEventArgs e)
        {
            LowerSlider.ValueChanged += LowerSlider_ValueChanged;
            UpperSlider.ValueChanged += UpperSlider_ValueChanged;
        }

        private void LowerSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            UpperSlider.Value = Math.Max(UpperSlider.Value, LowerSlider.Value);
        }

        private void UpperSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            LowerSlider.Value = Math.Min(UpperSlider.Value, LowerSlider.Value);
        }

        public double Minimum
        {
            get 
            { 
                return (double)GetValue(MinimumProperty); 
            }

            set 
            { 
                SetValue(MinimumProperty, value); 
            }
        }

        public static readonly DependencyProperty MinimumProperty =
            DependencyProperty.Register("Minimum", typeof(double), typeof(RangeSlider), new UIPropertyMetadata(0d));

        public double LowerValue
        {
            get 
            { 
                return (double)GetValue(LowerValueProperty); 
            }

            set 
            { 
                SetValue(LowerValueProperty, value); 
            }
        }

        public static readonly DependencyProperty LowerValueProperty =
            DependencyProperty.Register("LowerValue", typeof(double), typeof(RangeSlider), new UIPropertyMetadata(0d));

        public double UpperValue
        {
            get 
            { 
                return (double)GetValue(UpperValueProperty); 
            }

            set 
            { 
                SetValue(UpperValueProperty, value); 
            }
        }

        public static readonly DependencyProperty UpperValueProperty =
            DependencyProperty.Register("UpperValue", typeof(double), typeof(RangeSlider), new UIPropertyMetadata(0d));

        public double Maximum
        {
            get
            { 
                return (double)GetValue(MaximumProperty); 
            }

            set 
            { 
                SetValue(MaximumProperty, value); 
            }
        }

        public static readonly DependencyProperty MaximumProperty =
            DependencyProperty.Register("Maximum", typeof(double), typeof(RangeSlider), new UIPropertyMetadata(1d));

        public double SelectionLower
        {
            get 
            { 
                return (double)GetValue(SelectionLowerProperty); 
            }

            set 
            { 
                SetValue(SelectionLowerProperty, value); 
            }
        }

        public static readonly DependencyProperty SelectionLowerProperty =
            DependencyProperty.Register("SelectionLower", typeof(double), typeof(RangeSlider), new UIPropertyMetadata(0d));

        public double SelectionUpper
        {
            get 
            { 
                return (double)GetValue(SelectionUpperProperty); 
            }

            set 
            { 
                SetValue(SelectionUpperProperty, value); 
            }
        }

        public static readonly DependencyProperty SelectionUpperProperty =
            DependencyProperty.Register("SelectionUpper", typeof(double), typeof(RangeSlider), new UIPropertyMetadata(0d));

        public double TickFrequency
        {
            get 
            { 
                return (double)GetValue(TickFrequencyProperty); 
            }

            set 
            { 
                SetValue(TickFrequencyProperty, value); 
            }
        }

        public static readonly DependencyProperty TickFrequencyProperty =
            DependencyProperty.Register("TickFrequency", typeof(double), typeof(RangeSlider), new UIPropertyMetadata(1d));

        private void LowerSlider_ValueChanged_1(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            FireLimitsChangedEvent(new LimitsChangedEvent(LowerValue, UpperValue));
        }

        private void UpperSlider_ValueChanged_1(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            FireLimitsChangedEvent(new LimitsChangedEvent(LowerValue, UpperValue));
        }

        public void FireLimitsChangedEvent(LimitsChangedEvent e)
        {
            EventHandler<LimitsChangedEvent> handler = LimitsChangedEventHandler;
            if (handler != null)
                handler(this, e);
        }

    }

    public class LimitsChangedEvent : EventArgs
    {

        double m_min = 0;
        double m_max = 0;

        public LimitsChangedEvent()
            : base()
        {

        }

        public LimitsChangedEvent(double min, double max)
            : this()
        {
            m_min = min;
            m_max = max;
        }

        public double Minimum
        {
            get
            {
                return m_min;
            }

            set
            {
                m_min = value;
            }
        }

        public double Maximum
        {
            get
            {
                return m_max;
            }

            set
            {
                m_max = value;
            }
        }
    }
}
