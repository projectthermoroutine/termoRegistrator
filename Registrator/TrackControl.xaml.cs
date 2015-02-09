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


namespace Registrator
{
    /// <summary>
    /// Логика взаимодействия для TrackControl.xaml
    /// </summary>
    /// 
    using map_objects_list = List<measure_object>; 
    public partial class TrackControl : UserControl
    {

        int m_imgHeight = 200;
        int m_curCoord = 0;
        int m_factor = 100;

        map_objects_list m_mapObjects = null;
        int m_pathLength = 0;
        int m_hOffset = 1000;
        int m_deltaOffset = 0;
        int m_prevEndCoord = 0;

        DrawingVisual drawingVisual = new DrawingVisual();
        DrawingContext drawingContext;

        enum SEMAPHOR_TYPE
        {
            SEMAPHOR_WHITE,
            SEMAPHOR_RED,
            SEMAPHOR_YELLOW,
            SEMAPHOR_GREEN,
            SEMAPHOR_BLUE
        }

        public TrackControl()
        {
            InitializeComponent();
        }

        public TrackControl(map_objects_list mapObjects, int pathLength)
            : this()
        {

            m_mapObjects = mapObjects;
            m_pathLength = pathLength;
            FormatSetting(m_mapObjects);
            m_hOffset /= m_factor;
            DrawMap();

        }

        public delegate void RefreshDelegate();

        public void Refresh()
        {
            if (m_hOffset == 500)
                return;
            TranslateTransform trans = new TranslateTransform(m_hOffset - 500, 0);
            image1.RenderTransform = trans;
        }

        public int PathLength { get { return m_pathLength; } set { m_pathLength = value; } }
        public int CurCoord { get { return m_curCoord; } set { HOffset -= (value - m_curCoord); m_curCoord = value; } }
        public map_objects_list MapObjects { get { return m_mapObjects; } set { m_mapObjects = value; FormatSetting(m_mapObjects); } }

        public void FormatSetting(map_objects_list objs)
        {
            if (objs.Count == 0)
                return;

            int min = 0;
            int max = 0;

            foreach(var map_object in objs)
            {
                if (map_object.coord < min)
                    min = (int)map_object.coord;
                if (map_object.coord > max)
                    max = (int)map_object.coord;
            }

            m_pathLength = (max - min) / m_factor + 1000;
            m_hOffset = 500;
        }

        public int Factor { get { return m_factor; } }
        public int HOffset { get { return m_hOffset; } set { m_hOffset = value; } }

        public void DrawMap()
        {

            m_pathLength = 200;
            drawingContext = drawingVisual.RenderOpen();
            //DrawChain("123", 12, 129);
            //DrawPicket("name"/*obj.Name*/, 2, 5, true);
            //DrawPicket("name1"/*obj.Name*/, 222, 51, true);
            //DrawPicket("name1"/*obj.Name*/, 22, 51, false);


            if (m_mapObjects.Count == 0 || m_pathLength == 0)
                return;

            bool is_first_object = true;

            //PathLength = 50;

            //

            foreach (var obj in m_mapObjects)
            {
                switch ((MAPMEAS_TYPE)obj.type)
                {
                    case (MAPMEAS_TYPE.MAPMS_PICK):
                        {
                            int coord = (int)obj.coord;

                            if (!is_first_object)
                            {
                                if ((int)obj.coord > m_prevEndCoord)
                                    m_deltaOffset += ((int)obj.coord - m_prevEndCoord);
                            }

                            m_prevEndCoord = (int)obj.coord + obj.length;

                            DrawPicket(obj.picket.ToString()/*obj.Name*/, ((int)obj.coord - m_deltaOffset) / m_factor + m_hOffset, obj.length / m_factor, (obj.picket % 2) == 0);
                            break;
                        }
                    case (MAPMEAS_TYPE.MAPMS_STATION):

                        break;
                    case (MAPMEAS_TYPE.MAPMS_RZ):
                        DrawChain(obj.name, ((int)obj.coord - m_deltaOffset) / m_factor + m_hOffset, obj.length / m_factor);
                        break;
                    case (MAPMEAS_TYPE.MAPMS_PKO):

                        break;
                    case (MAPMEAS_TYPE.MAPMS_GSTAT):

                        break;
                    case (MAPMEAS_TYPE.MAPMS_STOP1):
                        DrawFirstCarrageStop(((int)obj.coord - m_deltaOffset) / m_factor + m_hOffset);
                        break;
                    case (MAPMEAS_TYPE.MAPMS_STRELKA):
                        DrawStrelka(((int)obj.coord - m_deltaOffset) / m_factor + m_hOffset);
                        break;
                    case (MAPMEAS_TYPE.MAPMS_SPATH):

                        break;
                    case (MAPMEAS_TYPE.MAPMS_PPATH):

                        break;
                    case (MAPMEAS_TYPE.MAPMS_SEMAFOR):
                        DrawSemaphor(obj.short_name, obj, ((int)obj.coord - m_deltaOffset) / m_factor + m_hOffset);
                        break;
                    case (MAPMEAS_TYPE.MAPMS_PICKBORDER):

                        break;
                    case (MAPMEAS_TYPE.MAPMS_STARTPOINT):

                        break;
                    case (MAPMEAS_TYPE.MAPMS_MEASPOINT):

                        break;
                    case (MAPMEAS_TYPE.MAPMS_MEASFINISH):

                        break;
                    case (MAPMEAS_TYPE.MAPMS_LR):

                        break;
                    case (MAPMEAS_TYPE.MAPMS_KP):

                        break;
                    case (MAPMEAS_TYPE.MAPMS_PJA):

                        break;
                    case (MAPMEAS_TYPE.MAPMS_DT):

                        break;
                }

                is_first_object = true;

            }

            drawingContext.Close();
            

            RenderTargetBitmap bmp = new RenderTargetBitmap(m_pathLength, m_imgHeight, 96, 96, PixelFormats.Pbgra32);
            bmp.Render(drawingVisual);
            image1.Height = m_imgHeight;
            image1.Source = bmp;


            //-----------------
            DrawingVisual drawingVisual1 = new DrawingVisual();
            DrawingContext drawingContext1 = drawingVisual1.RenderOpen(); 

            Brush brush = new SolidColorBrush(Colors.Yellow);
            Pen pen = new Pen(brush, 5.0d);
            drawingContext1.DrawLine(pen, new Point(500, 0), new Point(500, 1000));
            drawingContext1.Close();

            bmp = new RenderTargetBitmap(m_pathLength, m_imgHeight, 96, 96, PixelFormats.Pbgra32);
            bmp.Render(drawingVisual1);
            image2.Height = m_imgHeight;
            image2.Source = bmp;

        }

        private void DrawPicket(String title, int hOffset, int length, bool isEven = false)
        {
            Brush brush = new SolidColorBrush(Colors.Black);
            Pen pen = new Pen(brush, 1.0d);
            drawingContext.DrawRectangle(((isEven) ? new SolidColorBrush(Colors.Black) : new SolidColorBrush(Colors.White)), pen, new Rect(new Point(hOffset, m_imgHeight / 2 - 2.5), new Point(hOffset + length, m_imgHeight / 2 + 2.5)));
            String[] substrs = title.Split(new char[] { ' ' });
            if (substrs.Length > 1)
                title = substrs[1];
            FormattedText text = new FormattedText(title,
                  CultureInfo.GetCultureInfo("ru-ru"),
                  FlowDirection.LeftToRight,
                  new Typeface("Verdana"),
                  10, System.Windows.Media.Brushes.Black);
            drawingContext.DrawText(
                  text,
                  new Point((hOffset + length / 2 - text.Width / 2), m_imgHeight / 2 - 15 ));
        }

        private void DrawSemaphor(String title, measure_object obj, int hOffset = 0)
        {
            char[] chars = obj.name.ToCharArray();

            if (chars.Length < 5)
                return;

            Brush brushBlack = new SolidColorBrush(Colors.Black);
            Pen penBlack = new Pen(brushBlack, 1.0d);
            drawingContext.DrawLine(penBlack, new Point((int)obj.coord + hOffset, m_imgHeight / 2 - 2.5), new Point((int)obj.coord + hOffset, m_imgHeight / 2 - 20));

            int lightsNum = Int32.Parse(chars[4].ToString());

            int vOffset = 0;

            for (int i = lightsNum - 1 ; i > -1  && i < chars.Length ; i-- )
            {
                bool crossed = false;
                SEMAPHOR_TYPE semaphor_type = SEMAPHOR_TYPE.SEMAPHOR_WHITE;
                switch(Int32.Parse(chars[5+i].ToString()))
                {
                    case(0):

                        break;
                    case (1):

                        break;
                    case (2):
                        semaphor_type = SEMAPHOR_TYPE.SEMAPHOR_WHITE; crossed = true;
                        break;
                    case (3):
                        semaphor_type = SEMAPHOR_TYPE.SEMAPHOR_WHITE;
                        break;
                    case (4):
                        semaphor_type = SEMAPHOR_TYPE.SEMAPHOR_RED;
                        break;
                    case (5):
                        semaphor_type = SEMAPHOR_TYPE.SEMAPHOR_RED; crossed = true;
                        break;
                    case (6):
                        semaphor_type = SEMAPHOR_TYPE.SEMAPHOR_YELLOW;
                        break;
                    case (7):
                        semaphor_type = SEMAPHOR_TYPE.SEMAPHOR_YELLOW; crossed = true;
                        break;
                    case (8):
                        semaphor_type = SEMAPHOR_TYPE.SEMAPHOR_GREEN;
                        break;
                    case (9):
                        semaphor_type = SEMAPHOR_TYPE.SEMAPHOR_BLUE;
                        break;
                    default:
                        continue;

                };
                vOffset = DrawLight((int)obj.coord + hOffset, vOffset, semaphor_type, crossed);
            }
        }

        private int DrawLight(int hOffset, int vOffset = 0, SEMAPHOR_TYPE type = SEMAPHOR_TYPE.SEMAPHOR_RED, bool crossed = false )
        {
            
            Color color = Colors.Red;

            switch(type)
            {
                case(SEMAPHOR_TYPE.SEMAPHOR_WHITE):
                    color = Colors.White;
                    break;
                case(SEMAPHOR_TYPE.SEMAPHOR_RED):
                    color = Colors.Red;
                    break;
                case(SEMAPHOR_TYPE.SEMAPHOR_YELLOW):
                    color = Colors.Yellow;
                    break;
                case (SEMAPHOR_TYPE.SEMAPHOR_GREEN):
                    color = Colors.Green;
                    break;
                case (SEMAPHOR_TYPE.SEMAPHOR_BLUE):
                    color = Colors.Blue;
                    break;
            
            }

            Brush brush = new SolidColorBrush(color);
            Pen pen = new Pen(brush, 1.0d);
            drawingContext.DrawEllipse(brush, pen, new Point(hOffset, m_imgHeight / 2 - 20 - vOffset), 5, 5);

            if (crossed)
            {
                pen = new Pen(brush, 1.0d);
                drawingContext.DrawLine(pen, new Point(hOffset - 7, m_imgHeight / 2 - 20 - vOffset - 7), new Point(hOffset + 7, m_imgHeight / 2 - 20 - vOffset + 7));
                drawingContext.DrawLine(pen, new Point(hOffset - 7, m_imgHeight / 2 - 20 - vOffset + 7), new Point(hOffset + 7, m_imgHeight / 2 - 20 - vOffset - 7));
            }

            return vOffset+10;
        }

        private void DrawChain(String title, int hOffset, int length)
        {
            Brush brush = new SolidColorBrush(Colors.Black);
            Pen pen = new Pen(brush, 1.0d);
            drawingContext.DrawRectangle(null, pen, new Rect(new Point(hOffset, m_imgHeight / 2 + 2.5), new Point(hOffset + length, m_imgHeight / 2 + 20)));
            FormattedText text = new FormattedText(title,
                  CultureInfo.GetCultureInfo("ru-ru"),
                  FlowDirection.LeftToRight,
                  new Typeface("Verdana"),
                  10, System.Windows.Media.Brushes.Aqua);
            drawingContext.DrawText(
                  text,
                  new Point((hOffset + length / 2 - text.Width / 2), m_imgHeight / 2 + 7));
        }

        private void DrawStrelka(int hOffset, bool right = true)
        {
            Brush brush = new SolidColorBrush(Colors.Black);
            Pen pen = new Pen(brush, 1.0d);
            if(right)
                drawingContext.DrawLine(pen, new Point(hOffset, m_imgHeight / 2 + 2.5), new Point(hOffset - 12.5, m_imgHeight / 2 + 12.5));
            else
                drawingContext.DrawLine(pen, new Point(hOffset, m_imgHeight / 2 - 2.5), new Point(hOffset - 12.5, m_imgHeight / 2 - 12.5));
        }

        private void DrawFirstCarrageStop(int hOffset)
        {
            Brush brushW = new SolidColorBrush(Colors.White);
            Pen penW = new Pen(brushW, 1.0d);
            Brush brushB = new SolidColorBrush(Colors.Black);
            Pen penB = new Pen(brushB, 1.0d);
            drawingContext.DrawRectangle(brushW, penW, new Rect(new Point(hOffset - 2.5, m_imgHeight / 2 - 2.5), new Point(hOffset + 2.5, m_imgHeight / 2 - 7.5)));
            drawingContext.DrawRectangle(brushB, penB, new Rect(new Point(hOffset - 2.5, m_imgHeight / 2 - 7.5), new Point(hOffset + 2.5, m_imgHeight / 2 - 12.5)));
            drawingContext.DrawRectangle(brushW, penW, new Rect(new Point(hOffset - 2.5, m_imgHeight / 2 - 12.5), new Point(hOffset + 2.5, m_imgHeight / 2 - 17.5)));
            drawingContext.DrawRectangle(brushB, penB, new Rect(new Point(hOffset - 2.5, m_imgHeight / 2 - 17.5), new Point(hOffset + 2.5, m_imgHeight / 2 - 22.5)));
            drawingContext.DrawRectangle(brushW, penW, new Rect(new Point(hOffset - 2.5, m_imgHeight / 2 - 22.5), new Point(hOffset + 2.5, m_imgHeight / 2 - 27.5)));
            drawingContext.DrawRectangle(brushB, penB, new Rect(new Point(hOffset - 2.5, m_imgHeight / 2 - 27.5), new Point(hOffset + 2.5, m_imgHeight / 2 - 32.5)));
        
        }

        private void image1_Initialized(object sender, EventArgs e)
        {
            DrawMap();
        }

        private void UserControl_SizeChanged(object sender, SizeChangedEventArgs e)
        {

        }


    }
}
