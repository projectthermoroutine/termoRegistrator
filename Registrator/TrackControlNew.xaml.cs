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

        
        IEnumerable<Registrator.DB.EFClasses.AllEquipment> _objects = null;
        IEnumerable<Registrator.DB.EFClasses.AllEquipment> Objects { set { _objects = value; } }
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
        
        public void setDBController( DB.metro_db_controller db_controllerArg) 
        {
            db_controller = db_controllerArg;
        }

        public TrackControlNew()
        {
            InitializeComponent();
            brush = new SolidColorBrush(Colors.Yellow);
            CanvasBrush = new SolidColorBrush(Color.FromArgb(20, 0, 255, 0));
            pen = new Pen(brush, 5.0d);
            trans = new TranslateTransform();
            trans.X = 0;
            trans.Y = 0;
            drawingVisual = new DrawingVisual();
            drawingContext = drawingVisual.RenderOpen();
            mySolidColorBrush = new SolidColorBrush();
            TrackLength = (double)Properties.Settings.Default.TrackHalfVeiwSector;
            _Uri = new Uri("pack://application:,,,/Registrator;component/Resources/TraficLight.png");

            TrackOptionsParams TrackParams = new TrackOptionsParams();
            canvasBackground = new Rectangle();

            canvasBackground.Fill = new SolidColorBrush(TrackParams.color);
            DrawEquip = TrackParams.showEquipment;
            trailMarkerColor = TrackParams.trailMarkerColor;
            LTrainPosition.Stroke = new SolidColorBrush(trailMarkerColor);
            this.SizeChanged += TrackControlNew_SizeChanged;
    
        }

        void TrackControlNew_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            if (Pickets != null)
                DrawTrack(trans.X, previousUpdateTrackCoordinate);
        }
       

        public void setTrackOptions(TrackOptionsParams TrackParams)
        {
            canvasBackground.Fill = new SolidColorBrush(TrackParams.color);
            DrawEquip = TrackParams.showEquipment;
            trailMarkerColor = TrackParams.trailMarkerColor;
            LTrainPosition.Stroke = new SolidColorBrush(trailMarkerColor);
            
            if(Pickets != null)
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

            if (Pickets != null)
                DrawTrack(0, CurCoord);
        }

        public void UpdateTrackLength(long track_length)
        {
            visible_track_length = track_length;
        }
        
        public void UpdateTrack(Equipment.RefreshEquip data)
        {
            CurCoord = data.mmCoordinate;
            visible_track_length = data.LengthOfViewedTrack;
            PreviousTransformCoordinate = data.mmCoordinate;
            UpdateTrack();
        }
        
        void DrawTrack(double XCanvas, long coordinate)
        {
            setCanvas(XCanvas);
            if (!DrawPickets(coordinate))
                return;

            SetPositionLine();
            DrawEquipments(coordinate);
            canvas1.UpdateLayout();
        }

        double standartPicketLenght = 100000;
        long previousUpdateTrackCoordinate = 0;
        double countPartsCanvas = 3;
        Rectangle canvasBackground;
        Color trailMarkerColor;

        void setCanvas(double XCanvas = 0)
        {
            Scale = (double)(canvas1.ActualWidth) / (double)(TrackLength);
            ViewingHalfCanvasWidth = canvas1.ActualWidth / 2;
            trans.X = XCanvas;
            canvas1.Children.Clear();

            defaultPicketWidthInPixels = standartPicketLenght * Scale;
           
            canvasBackground.Width = canvas1.ActualWidth * countPartsCanvas;
            canvasBackground.Height = canvas1.ActualHeight;
            canvas1.Children.Add(canvasBackground);
            canvasBackground.RenderTransform = new TranslateTransform(-canvas1.ActualWidth,0);
           
            Canvas.SetZIndex(canvasBackground, -10);
        }
        
        double ViewingHalfCanvasWidth = 0;
      

        struct equipment_visible_traits
        {
            public static double width = 15.0;
            public static double height = 15.0;
        }
        
        void DrawEquipments(long _CurCoord)
        {
            if (_objects == null)
                return;

            x = ViewingHalfCanvasWidth;

            foreach (var item in _objects)
            {
                switch(item.EquipTypeID)
                {
                    case (int)Registrator.EQUIPS_TYPES.Equipment:
                        if (!DrawEquip) 
                            break;

                        e = new Ellipse();
                        e.Width = equipment_visible_traits.width;
                        e.Height = equipment_visible_traits.height;
                        mySolidColorBrush.Color = Color.FromRgb(128,128,128);//(Color)ColorConverter.ConvertFromString(""/*item.EquipID*/);
                        e.Fill = mySolidColorBrush;
                        e.StrokeThickness = 2;
                        e.Stroke = Brushes.Black;
                        canvas1.Children.Add(e);

                        x = ViewingHalfCanvasWidth + (double)(item.shiftLine - _CurCoord) * Scale;

                        var y = (EquipmentYPosition - e.Height);// - item.Y * Scale;
                        e.RenderTransform = new TranslateTransform(x - equipment_visible_traits.width / 2, y);

                        DrawObjectName(item.Name, x - equipment_visible_traits.width / 3, y - equipment_visible_traits.height / 4);

                        break;

                    case (int)Registrator.EQUIPS_TYPES.TrafficLight:

                        BitmapImage theImage = new BitmapImage(_Uri);
                        ImageBrush myImageBrush = new ImageBrush(theImage);

                        Line l = new Line();

                        Canvas myCanvas = new Canvas();
                        myCanvas.Width = TrafficLightWidth;
                        myCanvas.Height = TrafficLightHeght;
                        myCanvas.Background = myImageBrush;
                        Canvas.SetZIndex(myCanvas, 2);
                        canvas1.Children.Add(myCanvas);
                        
                        x = ViewingHalfCanvasWidth + (double)(item.shiftLine - _CurCoord) * Scale;

                        l.X1 = x + TrafficLightWidth / 2;
                        l.Y1 = LineYPosition;
                        l.Y2 = RectangleYPosition;
                        l.X2 = l.X1;
                        l.Stroke = Brushes.Black;
                        l.StrokeThickness = 1;
                        canvas1.Children.Add(l);

                        myCanvas.RenderTransform = new TranslateTransform(x, (LineYPosition - TrafficLightHeght));// - item.Y * Scale);

                        break;

                    case (int)Registrator.EQUIPS_TYPES.Strelka:

                        Strelka _Strelka = new Strelka(item.shiftLine, item.EquipLenght);// objectLenght);
                        x = ViewingHalfCanvasWidth + (item.shiftLine - _CurCoord) * Scale;

                        _Strelka.setCoordinate(x, RectangleYPosition, LineYPosition, Scale);
                        drawObj(_Strelka.Pack());
                        break;
                }
            }
        }
        
        void drawObj(List<UIElement> lst)
        {
            foreach (UIElement el in lst)
                canvas1.Children.Add(el);
        }
        
        double RectangleYPosition = 0;
        double LineYPosition = 0;
        double TrafficLightYPosition = 0;
        double TextBlockYPosition = 0;

        double defaultPicketWidthInPixels;
        double BorderHeightRectangle = 0;
        double RectangleHeght = 0;
        double MinRectangleHeight = 10;
        double TextBlockHeight = 14;
        double LineThickness = 0;
        double EquipmentYPosition = 0;
        
        double TrafficLightHeght = 50;
        double TrafficLightWidth = 12;

        bool DrawPickets(double _CurCoord)
        {
            var ViewingPickets = from r in Pickets where r.EndShiftLine > _CurCoord - TrackLength * 2 && r.StartShiftLine < _CurCoord + TrackLength * 2 select r;

            if (ViewingPickets.Count() == 0)
                return false;

            var FirstPicket = ViewingPickets.First();
            double beforePicketRigthCanvasPoint;
            double t = ((double)FirstPicket.StartShiftLine -_CurCoord) * Scale;

            beforePicketRigthCanvasPoint = ViewingHalfCanvasWidth + t;

            RectangleHeght = canvas1.ActualHeight / 30;
            if (RectangleHeght < MinRectangleHeight) RectangleHeght = MinRectangleHeight;

            RectangleYPosition = canvas1.ActualHeight / 2 + RectangleHeght;

            BorderHeightRectangle = RectangleHeght / 10;
            TextBlockYPosition = RectangleYPosition - TextBlockHeight;
            TrafficLightYPosition = RectangleYPosition - TrafficLightHeght;
            LineThickness = 1;
            LineYPosition = TextBlockYPosition - LineThickness;
            EquipmentYPosition = LineYPosition;

            //enumerra txtBlock = textBlock.GetEnumerator();
            
            foreach (var picket in ViewingPickets)
            {
                double picketWidthInPixels = defaultPicketWidthInPixels;

                if (picket.Dlina != 100000)
                    picketWidthInPixels = picket.Dlina * Scale;

                x = beforePicketRigthCanvasPoint + picketWidthInPixels;

                DrawPicketRectangle(Convert.ToInt32(picket.Npiketa), beforePicketRigthCanvasPoint, picketWidthInPixels);
                DrawPicketsNumbers(beforePicketRigthCanvasPoint, Convert.ToInt32(picket.Npiketa), TextBlockYPosition);
                beforePicketRigthCanvasPoint = x;
            }

            DrawLineAbovePicketNumber(LineYPosition);

            return true;
        }

        void DrawLineAbovePicketNumber(double _LineYPosition)
        {
            Line LBorder = new Line();
            LBorder.X1 = -canvas1.ActualWidth;
            LBorder.X2 = canvas1.ActualWidth * 2;
            LBorder.Y1 = _LineYPosition;
            LBorder.Y2 = _LineYPosition;
            LBorder.Stroke = Brushes.Gray;
            LBorder.StrokeThickness = 1;
            canvas1.Children.Add(LBorder);

        }

        void SetPositionLine(/*double _LineYPosition*/)
        {
            LTrainPosition.X1 = ViewingHalfCanvasWidth;
            LTrainPosition.X2 = ViewingHalfCanvasWidth;
        }

        void DrawPicketsNumbers(double _beforePicketRigthCanvasPoint, int picketNum, double _TextBlockYPosition)
        {
            TextBlock textBlock = new TextBlock();
            textBlock.Text = picketNum.ToString();
            textBlock.Foreground = new SolidColorBrush(Colors.Blue);
            textBlock.FontSize = 14;
            Canvas.SetZIndex(textBlock, 1);
            canvas1.Children.Add(textBlock);
            Canvas.SetLeft(textBlock, _beforePicketRigthCanvasPoint + (x - _beforePicketRigthCanvasPoint) / 2);
            Canvas.SetTop(textBlock, _TextBlockYPosition);
        }

        void DrawObjectName(string object_name, double x,  double y)
        {
            TextBlock textBlock = new TextBlock();
            textBlock.Text = object_name;
            textBlock.Foreground = new SolidColorBrush(Colors.Blue);
            textBlock.FontSize = 8;
            Canvas.SetZIndex(textBlock, 1);
            RotateTransform myRotateTransform = new RotateTransform(-90.0);
            textBlock.RenderTransform = myRotateTransform;
            canvas1.Children.Add(textBlock);
            Canvas.SetLeft(textBlock, x);
            Canvas.SetTop(textBlock, y);
        }


        void DrawPicketRectangle(int picketNum, double _beforePicketRigthCanvasPoint, double _picketWidthInPixels)
        {
            Rectangle rec = new Rectangle();
            rec.StrokeThickness = BorderHeightRectangle;
            rec.Stroke = Brushes.Black;

            if (picketNum % 2 != 0)
                rec.Fill = new SolidColorBrush(Colors.Black);
            else
                rec.Fill = new SolidColorBrush(Colors.White);

            rec.Width = _picketWidthInPixels;
            rec.Height = TextBlockHeight;

            canvas1.Children.Add(rec);
            Canvas.SetLeft(rec, _beforePicketRigthCanvasPoint);
            Canvas.SetTop(rec, RectangleYPosition);
        }
    }
}
