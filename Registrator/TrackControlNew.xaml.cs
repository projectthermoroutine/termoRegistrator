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
        IEnumerable<Registrator.DB.ResultEquipCode> _objects = null;
        IEnumerable<Registrator.DB.ResultEquipCode> Objects { set { _objects = value; } }
        IEnumerable<Registrator.DB.Picket> Pickets;
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
    

            //textBlock = new List<TextBlock>(100); TODO
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
        
        public void UpdateTrack(Equipment.RefreshEquip data)
        {
            CurCoord = data.mmCoordinate;
            PreviousTransformCoordinate = data.mmCoordinate;
            Objects = db_controller.get_objects_by_coordinate(CurCoord, data.LengthOfViewedTrack*2);
            Pickets = db_controller.getPicketsForCurrentPath();
            TrackLength = data.LengthOfViewedTrack;
            previousUpdateTrackCoordinate = CurCoord;
            
            if (Pickets != null)
                DrawTrack(0, CurCoord);
        }
        
        void DrawTrack(double XCanvas, long coordinate)
        {
            setCanvas(XCanvas);
            if (!DrawPickets(coordinate))
                return;

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
      

        
        void DrawEquipments(long _CurCoord)
        {
            x = ViewingHalfCanvasWidth;

            foreach (var item in _objects)
            {
                switch(item.EquipType)
                {
                    case (int)Registrator.equTypes.Equipment:
                        if (!DrawEquip) 
                            break;

                        e = new Ellipse();
                        e.Width = 15;
                        e.Height = 15;
                        mySolidColorBrush.Color = (Color)ColorConverter.ConvertFromString(item.Color);
                        e.Fill = mySolidColorBrush;
                        e.StrokeThickness = 2;
                        e.Stroke = Brushes.Black;
                        canvas1.Children.Add(e);

                        x = ViewingHalfCanvasWidth + (double)(item.shiftLine - _CurCoord) * Scale;

                        e.RenderTransform = new TranslateTransform(x - 15, (EquipmentYPosition - e.Height) - item.Y * Scale);
                        break;

                    case (int)Registrator.equTypes.TrafficLight:

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

                        myCanvas.RenderTransform = new TranslateTransform(x, (LineYPosition - TrafficLightHeght) - item.Y * Scale);

                        break;

                    case (int)Registrator.equTypes.Strelka:

                        Strelka _Strelka = new Strelka(item.shiftLine,item.objectLenght);
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

        List<TextBlock> textBlock;
        
        bool DrawPickets(double _CurCoord)
        {
            var ViewingPickets = from r in Pickets where r.RigthShiftLine > _CurCoord - TrackLength * 2 && r.LeftShiftLine < _CurCoord + TrackLength * 2 select r;

            if (ViewingPickets.Count() == 0)
                return false;

            var FirstPicket = ViewingPickets.First();
            double beforePicketRigthCanvasPoint;
            double t = ((double)FirstPicket.LeftShiftLine-_CurCoord) * Scale;

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

                if (picket.Length != 100000)
                    picketWidthInPixels = picket.Length * Scale;

                x = beforePicketRigthCanvasPoint + picketWidthInPixels;

                DrawPicketRectangle(Convert.ToInt32(picket.Num), beforePicketRigthCanvasPoint, picketWidthInPixels);
                DrawPicketsNumbers(beforePicketRigthCanvasPoint, Convert.ToInt32(picket.Num), TextBlockYPosition);
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
