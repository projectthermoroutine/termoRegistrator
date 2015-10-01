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
using System.IO;
using System.Collections;
using IRControls;
using DrawToolsLib;
using System.Threading;

namespace Registrator
{
    /// <summary>
    /// Логика взаимодействия для PlayerControl.xaml
    /// </summary>
    public partial class PlayerControl : UserControl
    {

        public enum ToolMode
        {
            FrameToolMode,
            ObjectToolMode,
            DeltaToolMode,
            AreasToolMode
        }

        public event EventHandler<ToolModeChangedEvent> ToolModeChangedEventHandler;
        public event EventHandler<KeyPressed> KeyPressedEventHandler;

        public event EventHandler<LimitsChangedEvent> LimitsChangedEventHandler;
        public event EventHandler<LimitsModeChangedEvent> LimitsModeChangedEventHandler;

        ToolMode m_toolMode = ToolMode.FrameToolMode;

        float minT = 0;
        float maxT = 0;
        float avrT = 0;
        float maxminT = 0;

        int frCount = 0;
        int curFrame = 0;

        bool m_blockImgUpdate = false;

        public PlayerControl(bool bottomVisibility = true)
        {
            InitializeComponent();

            termoScale.LimitsChangedEventHandler += LimitsChangedEventFired;

            filmProgress.Value = 0;
            TopVisible = !bottomVisibility;
            SubTopVisible = bottomVisibility;
            BottomPanelVisible = bottomVisibility;
            coordinatesGrid.Visibility = Visibility.Collapsed;
            lineGrid.Visibility = Visibility.Visible;
            picketGrid.Visibility = Visibility.Visible;
        }

        public long Position
        {
            set
            {
                var cm = value / 10;
                var m = value / (100 * 10);
                var km = value / (1000 * 100 * 10);

                cm -= m * 100;
                m -=  km * 1000;

                coordinate_label.Content = km.ToString() + " km " + m.ToString() + " m " + cm.ToString() + " cm";
            }
        }

        public string Line
        {
            set
            {
                string _value = value;
                if (_value == "")
                    _value = "---";
                line_label.Content = "Линия: " + _value;
            }
        }

        public void setPositionByPicket(Int32 picket, Int32 offset)
        {
            var m = offset / (100 * 10);
            picket_label.Content = picket.ToString() + " пк " + m.ToString() + " m" ;
        }


        public void ShowTemperatureToolTip(int x,int y,string message)
        {

            //drawingCanvas.

        }

        public bool BlockImgUpdate { get { return m_blockImgUpdate; } set { m_blockImgUpdate = value; } }
        public void ResetImage(bool blockImgUpdt = false)
        {
            m_blockImgUpdate = blockImgUpdt;
            imageBackground.Source = null;
        }

        public void SetImage(byte[] raster, int width, int height)
        {

         //   if (m_blockImgUpdate)
          //      return;

            PixelFormat pf = PixelFormats.Bgr32;

            int rawStride = (width * pf.BitsPerPixel + 7) / 8;
      
            try
            {
                BitmapSource bs = BitmapSource.Create(width, height, 96, 96, pf, null, raster, rawStride);
                imageBackground.Source = bs;
            }
            catch (OutOfMemoryException)
            {

                return;
            }

            

        }

        public void SetPlayerMode(byte mode)
        {
            switch(mode)
            {
                case (0):
                    {
                        playerMode.Content = "MOVIE";
                        BottomPanelVisible = true;
                        break;
                    }
                case (1):
                    {
                        playerMode.Content = "CAMERA";
                        BottomPanelVisible = false;
                        break;
                    }
                case (2):
                    {
                        playerMode.Content = "RECORD";
                        break;
                    }
            }
        }

        public double ActualScale
        {
            get
            {
                return drawingCanvas.ActualScale;
            }

            set
            {
                frameContainer.LayoutTransform = new ScaleTransform(value, value);
                drawingCanvas.ActualScale = value;
            }
        }

        public float MinT
        {
            get
            {
                return minT;
            }

            set
            {
                minT = value;
                if (subtop.Visibility == Visibility.Visible)
                    t_min.Content = String.Concat(new string[] { "T мин = ", minT.ToString("f1"), " \u00B0", "C" });
                else
                    t_min1.Content = String.Concat(new string[] { (modeSelection.SelectedIndex != 2) ? "T мин = " : "\u2206T мин = ", minT.ToString("f1"), " \u00B0", "C" });
            }
        }

        public float MaxT
        {
            get
            {
                return maxT;
            }

            set
            {
                maxT = value;
                if (subtop.Visibility == Visibility.Visible)
                    t_max.Content = String.Concat(new string[] { "T макс = ", maxT.ToString("f1"), " \u00B0", "C" });
                else
                    t_max1.Content = String.Concat(new string[] { (modeSelection.SelectedIndex != 2) ? "T макс = " : "\u2206T макс = ", maxT.ToString("f1"), " \u00B0", "C" });
            }
        }

        public float AvrT
        {
            get
            {
                return avrT;
            }

            set
            {
                avrT = value;
                if(subtop.Visibility == Visibility.Visible)
                    t_avr.Content = String.Concat(new string[] { "T сред = ", avrT.ToString("f1"), " \u00B0", "C" });
                else
                    t_avr1.Content = String.Concat(new string[] { (modeSelection.SelectedIndex != 2) ? "T сред = " : "\u2206T сред = ", avrT.ToString("f1"), " \u00B0", "C" });
            }
        }

        public float Max_MinT
        {
            get
            {
                return maxminT;
            }

            set
            {
                maxminT = value;
                t_mm.Content = String.Concat(new string[] { "T макс-мин = ", maxminT.ToString("f1"), " \u00B0", "C" });
            }
        }

        public int FrameAmount
        {
            get
            {
                return frCount;
            }

            set
            {
                frCount = value;
                frame.Content = String.Concat(new string[] { curFrame.ToString(), " / ", frCount.ToString() });
                filmProgress.Maximum = (frCount < 1) ? 1.0 : (double)frCount;
            }
        }

        public int CurFrameNum
        {
            get
            {
                return curFrame;
            }

            set
            {
                curFrame = value;
                frame.Content = String.Concat(new string[] { curFrame.ToString(), " / ", frCount.ToString() });
                filmProgress.Value = (double)value;
            }
        }

        public String Time
        {
            set
            {
                time.Content = value;
            }
        }

        public bool TermoScaleVisible
        {
            get
            {
                return termoScale.IsVisible;
            }

            set
            {
                termoScale.Visibility = (value) ? Visibility.Visible : Visibility.Collapsed;
            }
        }

        public bool TopVisible
        {
            get
            {
                return top.IsVisible;
            }

            set
            {
                top.Visibility = (value) ? Visibility.Visible : Visibility.Collapsed;
            }
        }

        public bool SubTopVisible
        {
            get
            {
                return subtop.IsVisible;
            }

            set
            {
                subtop.Visibility = (value) ? Visibility.Visible : Visibility.Collapsed;//.Hidden;
            }
        }

        public bool BottomPanelVisible
        {
            get
            {
                return bottom.IsVisible;
            }

            set
            {
                bottom.Visibility = (value)?Visibility.Visible:Visibility.Hidden;//.Collapsed;//.Hidden;
            }
        }

        public void ResetImageStrech()
        {
        }

        public void InverseImageStrech()
        {
        }

        public ToolMode Mode
        {
            get
            {
                return m_toolMode;
            }

            set
            {
                m_toolMode = value;
            }
        }

        public void FireToolModeChangedEvent(ToolModeChangedEvent e)
        {
            EventHandler<ToolModeChangedEvent> handler = ToolModeChangedEventHandler;
            if (handler != null)
                handler(this, e);
        }

        public void RemoveAllAreas()
        {

            if (drawingCanvas != null)
                drawingCanvas.DeleteAllAreas();
        }

        private void comboBox1_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

            if(drawingCanvas != null)
                drawingCanvas.DeleteAllAreas();

            switch(modeSelection.SelectedIndex)
            {

                case (0):
                    {
                        m_toolMode = ToolMode.FrameToolMode;
                        break;
                    }
                case (1):
                    {
                        m_toolMode = ToolMode.ObjectToolMode;
                        break;
                    }
                case (2):
                    {
                        m_toolMode = ToolMode.DeltaToolMode;
                        break;
                    }
                case (3):
                    {
                        m_toolMode = ToolMode.AreasToolMode;
                        break;
                    }

            }



            ToolModeChangedEvent te = new ToolModeChangedEvent(m_toolMode);
            FireToolModeChangedEvent(te);
        }

        public void DrawArea(Area area)
        {

            if (area == null)
                return;

            GraphicsBase areaGraphics = null;

            if(area.Type == Area.AreaType.AREA_RECT)
            {
                areaGraphics = new GraphicsRectangle( area.X
                    , area.Y
                    , area.X + area.Width
                    , area.Y + area.Height
                    , 2
                    , Color.FromRgb(0, 0, 0)
                    , 1);

                (areaGraphics as GraphicsRectangle).Id = area.ProgID;

                (areaGraphics as GraphicsRectangle).AreaChangedEventHandler += drawingCanvas.AreaChangedEventFired;

            }
            else if (area.Type == Area.AreaType.AREA_ELLIPS)
            {
                areaGraphics = new GraphicsEllipse(area.X
                    , area.Y
                    , area.X + area.Width
                    , area.Y + area.Height
                    , 2
                    , Color.FromRgb(0, 0, 0)
                    , 1);

                (areaGraphics as GraphicsEllipse).Id = area.ProgID;

                (areaGraphics as GraphicsEllipse).AreaChangedEventHandler += drawingCanvas.AreaChangedEventFired;

            }

            if (areaGraphics != null)
                drawingCanvas.AddArea(areaGraphics);

        }

        private void UserControl_KeyUp(object sender, KeyEventArgs e)
        {
        }

        private void FireKeyPressedEvent(KeyPressed e)
        {
            EventHandler<KeyPressed> handler = KeyPressedEventHandler;
            if (handler != null)
                handler(this, e);

        }

        public void LimitsChangedEventFired(object sender, LimitsChangedEvent e)
        {
            FireLimitsChangedEvent(e);
        }

        public void SetFrameLimits(float bottom_value,float top_value)
        {
            termoScale.ScaleTopValue = top_value;
            termoScale.ScaleLowerValue = bottom_value;
        }
        public void SetCalibrationLimits(float top_value,float bottom_value)
        {
            termoScale.CalibrationTopValue = Math.Round(top_value);
            termoScale.CalibrationLowerValue = Math.Round(bottom_value);
        }

        public void SetObjectLimits(float bottom_value, float top_value)
        {
            termoScale.ObjectTopValue = top_value;
            termoScale.ObjectLowerValue = bottom_value;
        }


        public void SetTopObjectLimit(float value)
        {
            termoScale.ObjectTopValue = value;
        }

        public void SetBottomObjectLimit(float value)
        {
            termoScale.ObjectLowerValue = value;
        }

        public void FireLimitsChangedEvent(LimitsChangedEvent e)
        {
            EventHandler<LimitsChangedEvent> handler = LimitsChangedEventHandler;
            if (handler != null)
                handler(this, e);
        }

        public void FireLimitsModeChangedEvent(LimitsModeChangedEvent e)
        {
            EventHandler<LimitsModeChangedEvent> handler = LimitsModeChangedEventHandler;
            if (handler != null)
                handler(this, e);
        }

        private void UserControl_KeyDown(object sender, KeyEventArgs e)
        {
          //  FireKeyPressedEvent(new KeyPressed(e.Key));
        }

        private void termoScale_LimitsModeChangedEventHandler(object sender, LimitsModeChangedEvent e)
        {
            FireLimitsModeChangedEvent(e);
        }

        private void UserControl_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            FireKeyPressedEvent(new KeyPressed(e.Key));
        }
    }

    public class KeyPressed : EventArgs
    {

        Key m_key;
        
        public KeyPressed(Key key)
            : base()
        {
            m_key = key;
        }

        public Key PressedKey
        {
            get
            {
                return m_key;
            }

            set
            {
                m_key = value;
            }

        }

    }

    public class ToolModeChangedEvent : EventArgs
    {

        PlayerControl.ToolMode m_mode = PlayerControl.ToolMode.FrameToolMode;

        public ToolModeChangedEvent()
            : base()
        {

        }

        public ToolModeChangedEvent(PlayerControl.ToolMode mode)
            : this()
        {
            m_mode = mode;
        }

        public PlayerControl.ToolMode Mode
        {
            get
            {
                return m_mode;
            }

            set
            {
                m_mode = value;
            }
        }

    }

}
