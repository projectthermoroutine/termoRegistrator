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
using System.Threading.Tasks;


namespace Registrator
{
    public partial class TrackControlNew : UserControl
    {
        public class TrackOptionsParams
        {
            public Color color = Colors.Green;
            public Color trailMarkerColor = Colors.Yellow;
            public bool showEquipment = true;
            public bool showTrafficLight = true;

            public TrackOptionsParams()
            {
                color = Properties.Settings.Default.TrackPanel_BackGroungColor;
                trailMarkerColor = Properties.Settings.Default.TrackPanel_TrailMarkerColor;
                showEquipment = Properties.Settings.Default.TrackPanel_VisibleEquipment;
            }
        }
        
        class Strelka
        {
            Line LineBegin { get; set; }
            Line LineEnd { get; set; }

            public Strelka(long shif_Line, int objLenght)
            {
                shiftLine = shif_Line;
                Lenght = objLenght;
            
                minimalStrelkaLenght = 15;
                LineEnd = new Line();
                LineBegin = new Line();
                LineEnd.Stroke = Brushes.Black;
                LineEnd.StrokeThickness = 1;
                LineBegin.Stroke = Brushes.Black;
                LineBegin.StrokeThickness = 1;
            }

            public void setCoordinate(double X, double Y1, double Y2, double Scale)
            {
                LineBegin.Y1 = Y1;
                LineBegin.Y2 = Y2;
                LineBegin.X1 = X;

                LineEnd.Y1 = Y1;
                LineEnd.Y2 = Y2;
                
                if (shiftLine < 0)
                {
                    if (Lenght * Scale < minimalStrelkaLenght)
                        LineEnd.X1 = X - minimalStrelkaLenght;
                    else
                        LineEnd.X1 = X - Lenght * Scale;

                    LineEnd.X2 = LineEnd.X1 + 5;
                    LineBegin.X2 = X - 5;
                }
                else
                {
                    if (Lenght * Scale < minimalStrelkaLenght)
                        LineEnd.X1 = X + minimalStrelkaLenght;
                    else
                        LineEnd.X1 = X + Lenght * Scale;

                    LineEnd.X2 = LineEnd.X1 - 5;
                    LineBegin.X2 = X + 5;
                }
            }

            public List<UIElement> Pack()
            {
                return new List<UIElement>() { LineBegin, LineEnd };
            }

            double minimalStrelkaLenght {get; set;}
            long shiftLine { get; set; }
            int Lenght { get; set; }
        }

        long CurCoord = 0;
        long visible_track_length = 0;


        List<Registrator.DB.EFClasses.AllEquipment> _objects = null;
        List<Registrator.DB.EFClasses.AllEquipment> Objects { set { _objects = value; } }
        IEnumerable<DB.EFClasses.Picket> Pickets;
        DB.metro_db_controller db_controller;
        Uri _Uri;
        bool DrawEquip;

        DrawingVisual drawingVisual;
        DrawingContext drawingContext;
        Brush brush;
        Brush CanvasBrush;
        Pen pen;
        TranslateTransform trans;
        double Scale;
        Ellipse e;
        SolidColorBrush mySolidColorBrush;
        double TrackLength = 0;
        long PreviousTransformCoordinate = 0;
        double x;
        
        public TrackControlNew(DB.metro_db_controller db)
        {
            db_controller = db;
            InitializeComponent();

            brush = new SolidColorBrush(Colors.Yellow);
            CanvasBrush = new SolidColorBrush(Color.FromArgb(20, 0, 255, 0));
            pen = new Pen(brush, 5.0d);
            trans = new TranslateTransform { X = 0, Y = 0 };
            drawingVisual = new DrawingVisual();
            drawingContext = drawingVisual.RenderOpen();
            mySolidColorBrush = new SolidColorBrush();
            TrackLength = (double)Properties.Settings.Default.TrackHalfVeiwSector;
            _Uri = new Uri("pack://application:,,,/Registrator;component/Resources/TraficLight.png");

            traffic_light_image = new BitmapImage(_Uri);

            TrackOptionsParams TrackParams = new TrackOptionsParams();
            canvasBackground = new Rectangle
            {
                Fill = new SolidColorBrush(TrackParams.color)
            };
            DrawEquip = TrackParams.showEquipment;
            trailMarkerColor = TrackParams.trailMarkerColor;
            LTrainPosition.Stroke = new SolidColorBrush(trailMarkerColor);
           // this.SizeChanged += TrackControlNew_SizeChanged;
    
        }

        void TrackControlNew_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            if (Pickets != null && Pickets.Count() != 0)
                DrawTrack(trans.X, previousUpdateTrackCoordinate);
        }
       

        public void setTrackOptions(TrackOptionsParams TrackParams)
        {
            canvasBackground.Fill = new SolidColorBrush(TrackParams.color);
            DrawEquip = TrackParams.showEquipment;
            trailMarkerColor = TrackParams.trailMarkerColor;
            LTrainPosition.Stroke = new SolidColorBrush(trailMarkerColor);
            
            if(Pickets != null && Pickets.Count() != 0)
                DrawTrack(trans.X, previousUpdateTrackCoordinate);
        }
        
        public void TransformTrack(Equipment.TrasformTrackEvent data)
        {
            CurCoord = data.Coord;

            trans.X = trans.X - Scale * (double)(CurCoord - PreviousTransformCoordinate);
            PreviousTransformCoordinate = CurCoord;
            canvas1.RenderTransform = trans;
        }

        public void TransformTrack()
        {
            canvas1.RenderTransform = trans;
        }

        public void UpdateTrack()
        {
            Objects = db_controller.get_objects_by_coordinate(CurCoord, visible_track_length * 2);
            Pickets = db_controller.getPicketsForCurrentPath();
            TrackLength = visible_track_length;
            previousUpdateTrackCoordinate = CurCoord;

            canvas1.Children.Clear();

            if (Pickets != null && Pickets.Count() != 0)
                DrawTrack(0, CurCoord);

            canvas1.UpdateLayout();

        }

        public void UpdateTrackLength(long track_length)
        {
            visible_track_length = track_length;
        }
        
        public void UpdateTrack(Equipment.RefreshEquip data)
        {
            CurCoord = data.Coordinate.coordinate;
            visible_track_length = data.LengthOfViewedTrack;
            PreviousTransformCoordinate = data.Coordinate.coordinate;
            UpdateTrack();
        }
        
        void DrawTrack(double XCanvas, long coordinate)
        {
            setCanvas(XCanvas);
            if (!DrawPickets(coordinate))
                return;

            SetPositionLine();
            DrawEquipments(coordinate);
        }

        double standartPicketLength = 100000;
        long previousUpdateTrackCoordinate = 0;
        double countPartsCanvas = 3;
        Rectangle canvasBackground;
        Color trailMarkerColor;

        BitmapImage traffic_light_image;


        void setCanvas(double XCanvas = 0)
        {
            Scale = (double)(canvas1.ActualWidth) / (double)(TrackLength);
            ViewingHalfCanvasWidth = canvas1.ActualWidth / 2;
            trans.X = XCanvas;

            defaultPicketWidthInPixels = standartPicketLength * Scale;
           
            canvasBackground.Width = canvas1.ActualWidth * countPartsCanvas;
            canvasBackground.Height = canvas1.ActualHeight;
            canvas1.Children.Add(canvasBackground);
            canvasBackground.RenderTransform = new TranslateTransform(-canvas1.ActualWidth,0);
           
            Canvas.SetZIndex(canvasBackground, -10);
        }
        
        double ViewingHalfCanvasWidth = 0;
      

        static class Shape
        {
            public static class General
            {

                public const double width = 15.0;
                public const double height = 15.0;

                public static class Font
                {
                    public const double text_size = 8.0;
                    public const double numbers_size = 8.0;
                }
            }


            public static class Equipment
            {

                public const double width = 15.0;
                public const double height = 15.0;

                public static class Font
                {
                    public const double text_size = 8.0;
                    public const double numbers_size = 8.0;
                }
            }

            public static class Picket
            {

                public const double width = 15.0;

                public static class Rectangle
                {
                    public const double MinHeight = 10.0;
                    public const double HeightScale = 30.0;
                    public const double StrokeThicknessScale = 10.0;
                }

                public static class Number
                {
                    public const double height = 14.0;
                }

                public static class Line
                {
                    public const double Thickness = 1.0;
                }

                
                public static class Font
                {
                    public const double number_size = 14.0;
                }
            }

            public static class TrafficLight
            {

                public const double width = 12.0;
                public const double height = 50.0;

                public static class Font
                {
                    public const double number_size = 14.0;
                }
            }

        }

        internal sealed class Point
        {
            public double X { get; set; }
            public double Y { get; set; }
        }

        internal sealed class NamesInfo
        {
            public UInt32 Count { get; set; } = 0;
            public Dictionary<string, int> Info { get; set; } = new Dictionary<string, int>();
            public TextBlock NamesView { get; set; } = null;
        }



        void DrawEquipments(long _CurCoord)
        {
            if (_objects == null)
                return;

            x = ViewingHalfCanvasWidth;

            Dictionary<long, NamesInfo> displyed_name = new Dictionary<long, NamesInfo>();
            Dictionary<long, Point> objects_points = new Dictionary<long, Point>();

            foreach (var item in _objects)
            {
                switch(item.EquipTypeID)
                {
                    case (int)Registrator.EQUIPS_TYPES.Equipment:

                        if (!DrawEquip) 
                            break;

                        x = ViewingHalfCanvasWidth + (double)(item.shiftLine - _CurCoord) * Scale;
                        var y = (EquipmentYPosition - Shape.Equipment.height);// - item.Y * Scale;

                        NamesInfo names;

                        long key = item.shiftLine / 1000;
                        if (!displyed_name.ContainsKey(key))
                        {
                            mySolidColorBrush.Color = Color.FromRgb(128,128,128);//(Color)ColorConverter.ConvertFromString(""/*item.EquipID*/);

                            e = new Ellipse
                            {
                                Width = Shape.Equipment.width,
                                Height = Shape.Equipment.height,
                                Fill = mySolidColorBrush,
                                StrokeThickness = 0.5,
                                Stroke = Brushes.Black,
                                RenderTransform = new TranslateTransform(x - Shape.Equipment.width / 2, y)
                            };

                            canvas1.Children.Add(e);

                            displyed_name[key] = new NamesInfo();
                            objects_points[key] = new Point
                            {
                                X = x - Shape.Equipment.width / 4 + Shape.Equipment.width / 8,
                                Y = y + Shape.Equipment.height / 4 - Shape.Equipment.height / 8
                            };

                        }

                        names = displyed_name[key];

                        if (!names.Info.ContainsKey(item.Name))
                        {
                            if (names.Info.Count == 0) {
                                names.NamesView = DrawObjectName(item.Name, x, y);
                            }
                            else {

                                if (names.Info.Count == 1)
                                    names.NamesView.Text += "...";
                            }

                            names.Info[item.Name] = 1;
                        }
                        else
                        {
                            names.Info[item.Name] += 1;
                        }

                        names.Count += 1;

                        break;

                    case (int)Registrator.EQUIPS_TYPES.TrafficLight:

                        //BitmapImage theImage = new BitmapImage(_Uri);

                        Canvas myCanvas = new Canvas
                        {
                            Width = Shape.TrafficLight.width,
                            Height = Shape.TrafficLight.height,
                            Background = new ImageBrush(traffic_light_image)
                        };

                        Canvas.SetZIndex(myCanvas, 0);
                        canvas1.Children.Add(myCanvas);
                        
                        x = ViewingHalfCanvasWidth + (double)(item.shiftLine - _CurCoord) * Scale;

                        Line l = new Line
                        {
                            X1 = x + Shape.TrafficLight.width / 2,
                            X2 = x + Shape.TrafficLight.width / 2,
                            Y1 = picket_view_data.Line.Y0,
                            Y2 = picket_view_data.Rectangle.Y0,
                            Stroke = Brushes.Black,
                            StrokeThickness = 1
                        };

                        canvas1.Children.Add(l);

                        myCanvas.RenderTransform = new TranslateTransform(x, (picket_view_data.Line.Y0 - Shape.TrafficLight.height));

                        break;

                    case (int)Registrator.EQUIPS_TYPES.Strelka:

                        Strelka _Strelka = new Strelka(item.shiftLine, item.EquipLenght);// objectLenght);
                        x = ViewingHalfCanvasWidth + (item.shiftLine - _CurCoord) * Scale;

                        _Strelka.setCoordinate(x, picket_view_data.Rectangle.Y0, picket_view_data.Line.Y0, Scale);
                        drawObj(_Strelka.Pack());
                        break;
                }
            }//foreach (var item in _objects)

            double font_size = Shape.Equipment.Font.numbers_size;
            foreach (var item in objects_points)
            {
                NamesInfo info = displyed_name[item.Key];

                if (info.Count > 1)
                {
                    Point point = item.Value;
                    string count_str = info.Count.ToString();

                    DrawText(count_str, point.X, point.Y, font_size);

                    ToolTip ttpTextBlock = new ToolTip
                    {
                        Content = String.Join("\n", info.Info.Keys)
                    };
                    info.NamesView.ToolTip = (ttpTextBlock);

                }
            }

        }

        //Dictionary<int, double> _map_number_to_width_px = new Dictionary<int, double>();

        void drawObj(List<UIElement> lst)
        {
            foreach (UIElement el in lst)
                canvas1.Children.Add(el);
        }
        
        double EquipmentYPosition = 0;

        double defaultPicketWidthInPixels;

        PicketViewData picket_view_data;

        internal struct PicketViewData
        {
            public struct RectangleData
            {
                public double Height;
                //public double Width;
                public double X0;
                public double Y0;
            }
            public struct NumberBlockData
            {
                //public double Height;
                //public double Width;
                //public double X0;
                public double Y0;
            }

            public struct LineData
            {
                //public double Height;
                //public double Width;
                //public double X0;
                public double Y0;
            }

            public RectangleData Rectangle;
            public NumberBlockData NumberBlock;
            public LineData Line;
        }

        bool DrawPickets(double _CurCoord)
        {
            var ViewingPickets = 
                from r in Pickets
                where r.EndShiftLine > _CurCoord - TrackLength * 2 && r.StartShiftLine < _CurCoord + TrackLength * 2
                orderby r.StartShiftLine ascending
                select r;

            if (ViewingPickets.Count() == 0)
                return false;

            var FirstPicket = ViewingPickets.First();

            picket_view_data.Rectangle.X0 = ViewingHalfCanvasWidth + (FirstPicket.StartShiftLine - _CurCoord) * Scale;
            picket_view_data.Rectangle.Height = Math.Max(canvas1.ActualHeight / Shape.Picket.Rectangle.HeightScale, Shape.Picket.Rectangle.MinHeight);

            picket_view_data.Rectangle.Y0 = canvas1.ActualHeight / 2 + picket_view_data.Rectangle.Height;

            picket_view_data.NumberBlock.Y0 = picket_view_data.Rectangle.Y0 - Shape.Picket.Number.height;

            picket_view_data.Line.Y0 = picket_view_data.NumberBlock.Y0 - Shape.Picket.Line.Thickness;

            EquipmentYPosition = picket_view_data.Line.Y0;

            foreach (var picket in ViewingPickets)
            {
                double picketWidthInPixels = defaultPicketWidthInPixels;

                if (picket.Dlina != 100000)
                    picketWidthInPixels = picket.Dlina * Scale;

                x = picket_view_data.Rectangle.X0 + picketWidthInPixels;

                DrawPicketRectangle(
                    picketNum: Convert.ToInt32(picket.Npiketa), 
                    x0: picket_view_data.Rectangle.X0, y0: picket_view_data.Rectangle.Y0,
                    width: picketWidthInPixels, height: picket_view_data.Rectangle.Height
                    );

                DrawPicketsNumbers(picket_view_data.Rectangle.X0, picket.Npiketa, picket_view_data.NumberBlock.Y0);
                picket_view_data.Rectangle.X0 = x;
            }

            DrawLineAbovePicketNumber(picket_view_data.Line.Y0);

            return true;
        }

        void DrawLineAbovePicketNumber(double _LineYPosition)
        {
            Line LBorder = new Line
            {
                X1 = -canvas1.ActualWidth,
                X2 = canvas1.ActualWidth * 2,
                Y1 = _LineYPosition,
                Y2 = _LineYPosition,
                Stroke = Brushes.Gray,
                StrokeThickness = 1
            };
            canvas1.Children.Add(LBorder);

        }

        void SetPositionLine(/*double _LineYPosition*/)
        {
            LTrainPosition.X1 = ViewingHalfCanvasWidth;
            LTrainPosition.X2 = ViewingHalfCanvasWidth;
        }

        void DrawPicketsNumbers(double _beforePicketRigthCanvasPoint, string picketNum, double _TextBlockYPosition)
        {
            TextBlock textBlock = new TextBlock
            {
                Text = picketNum,
                Foreground = new SolidColorBrush(Colors.Blue),
                FontSize = Shape.Picket.Font.number_size
            };
            Canvas.SetZIndex(textBlock, 1);
            canvas1.Children.Add(textBlock);
            Canvas.SetLeft(textBlock, _beforePicketRigthCanvasPoint + (x - _beforePicketRigthCanvasPoint) / 2);
            Canvas.SetTop(textBlock, _TextBlockYPosition);
        }

        void DrawText(string text, double x, double y, double font_size = 8.0)
        {
            TextBlock textBlock = new TextBlock
            {
                Text = text,
                Foreground = new SolidColorBrush(Colors.Blue),
                FontSize = font_size
            };

            textBlock.Measure(new Size(Double.PositiveInfinity, Double.PositiveInfinity));
            textBlock.Arrange(new Rect(textBlock.DesiredSize));

            Canvas.SetZIndex(textBlock, 1);
            canvas1.Children.Add(textBlock);

            x -= textBlock.ActualWidth / 2 - 1.5;
            y += (Shape.Equipment.height - textBlock.ActualHeight) / 2;

            Canvas.SetLeft(textBlock, x);
            Canvas.SetTop(textBlock, y);
        }

        TextBlock DrawObjectName(string object_name, double x,  double y)
        {
            TextBlock textBlock = new TextBlock
            {
                Text = object_name,
                Foreground = new SolidColorBrush(Colors.Blue),
                FontSize = Shape.Equipment.Font.text_size
            };

            Canvas.SetZIndex(textBlock, 1);

            textBlock.Measure(new Size(Double.PositiveInfinity, Double.PositiveInfinity));
            textBlock.Arrange(new Rect(textBlock.DesiredSize));

            if (object_name.Length > 6)
            {
                textBlock.RenderTransform = _rotation_90_degree;
                x -= Shape.Equipment.width / 3;
                y -= Shape.Equipment.height / 4;
            }
            else
            {
                x -= textBlock.ActualWidth / 2;
                y -= Shape.Equipment.height / 2 + 4;
            }

            canvas1.Children.Add(textBlock);
            Canvas.SetLeft(textBlock, x);
            Canvas.SetTop(textBlock, y);

            return textBlock;

        }

        RotateTransform _rotation_90_degree = new RotateTransform(-90.0);
        SolidColorBrush _black_brush = new SolidColorBrush(Colors.Black);
        SolidColorBrush _white_brush = new SolidColorBrush(Colors.White);

        void DrawPicketRectangle(int picketNum, double x0, double y0, double width, double height)
        {
            Rectangle rec = new Rectangle
            {
                StrokeThickness = height / Shape.Picket.Rectangle.StrokeThicknessScale,
                Stroke = Brushes.Black,
                Width = width,
                Height = height,

                Fill = (picketNum % 2 != 0) ? _black_brush : _white_brush
            };

            canvas1.Children.Add(rec);
            Canvas.SetLeft(rec, x0);
            Canvas.SetTop(rec, y0);
        }
    }
}
