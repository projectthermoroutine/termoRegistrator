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
        long CurCoord = 0;
        IEnumerable<Registrator.DB.ResultEquipCodeFrame> _objects = null;
        IEnumerable<Registrator.DB.ResultEquipCodeFrame> Objects { set { _objects = value; } }
        IEnumerable<Registrator.DB.Picket> Pickets;
        DB.metro_db_controller db_controller;

        public void setDBController( DB.metro_db_controller db_controllerArg) 
        {
            db_controller = db_controllerArg;
            db_controller.ChangePath += db_controller_ChangePath;
        }

        void db_controller_ChangePath(object sender, EventArgs e)
        {
            Pickets = db_controller.getPicketsForCurrentPath();
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
            //textBlock = new List<TextBlock>(100);
          
        }
        public void setNextTransformFrameData(Equipment.TrasformTrackEvent data)
        {
            CurCoord = data.Coord;
        }
        public void setNextRefreshFrameData(Equipment.RefreshEquip data)
        {
           CurCoord = data.Coord;
           Objects = db_controller.ObjectsByCurCoordinate;
        }

        public void setTrackLength(double ZoomCoefficient) 
        {
            TrackLength = TrackLength*ZoomCoefficient;
            db_controller.get_objects_by_coordinate(CurCoordPreviousUpdate, (long)TrackLength);
            ZoomTrack(trans.X);
        }
        public void setObjects(IEnumerable<Registrator.DB.ResultEquipCodeFrame> ObjectsArg) { Objects = ObjectsArg; }

        DrawingVisual drawingVisual;
        DrawingContext drawingContext;
        Brush brush;
        Brush CanvasBrush;
        Pen pen;
        object lockerDB = new object();
        TranslateTransform trans;
        double Scale;
        Ellipse e;
        SolidColorBrush mySolidColorBrush;
        double TrackLength = 0;
        long PreviousCoordinate = 0;
        double transformTemp = 0;

        public void Transform()
        {
            transformTemp = Scale * (CurCoord - PreviousCoordinate);
            trans.X = trans.X - transformTemp;
            PreviousCoordinate = CurCoord;
            canvas1.RenderTransform = trans;
        }

        double x;
        void ZoomTrack(double XCanvas)
        {

            
            setCanvas(XCanvas);
            DrawPickets(CurCoordPreviousUpdate);
            DrawEquipments(CurCoordPreviousUpdate);

            canvas1.UpdateLayout();
        }

        public void UpdateTrack()
        {
            CurCoordPreviousUpdate = CurCoord;
            setCanvas();
            DrawPickets(CurCoord);
            DrawEquipments(CurCoord);

            canvas1.UpdateLayout();
        }
        long CurCoordPreviousUpdate = 0;
        void setCanvas(double XCanvas=0)
        {
            
            Scale = (double)(canvas1.ActualWidth) / (double)(TrackLength);
            ViewingHalfCanvasWidth = canvas1.ActualWidth / 2;
            trans.X = XCanvas;
            canvas1.Children.Clear();
            
            defaultPicketWidthInPixels = 100000 * Scale;
            
            Rectangle canvasBackground = new Rectangle();
            canvasBackground.Width = canvas1.ActualWidth*2;
            canvasBackground.Height = canvas1.ActualHeight;
            canvas1.Children.Add(canvasBackground);
            canvasBackground.RenderTransform = new TranslateTransform(0,0);
            canvasBackground.Fill = new SolidColorBrush(Colors.Green);
            Canvas.SetZIndex(canvasBackground, -10);
        }
        
        double ViewingHalfCanvasWidth = 0;

        void DrawEquipments(long _CurCoord)
        {
            foreach (var item in _objects)
            {
                e = new Ellipse();
                e.Width  = 20;
                e.Height = 20;
                mySolidColorBrush.Color = (Color)ColorConverter.ConvertFromString(item.Color);
                e.Fill = mySolidColorBrush;
                e.StrokeThickness = 2;
                e.Stroke = Brushes.Black;
                canvas1.Children.Add(e);

                if (item.shiftLine > CurCoord)
                    x = ViewingHalfCanvasWidth + (double)(item.shiftLine - _CurCoord) * Scale;
                else
                    x = ViewingHalfCanvasWidth - (double)(_CurCoord - item.shiftLine) * Scale;

                e.RenderTransform = new TranslateTransform(x, (EquipmentYPosition - e.Height) - item.Y * Scale);
            }
        }

        double defaultPicketWidthInPixels;
        double BorderHeightRectangle = 0;
        double RectangleHeght = 0;
        double MinRectangleHeight = 10;
        double RectangleYPosition = 0;
        double TextBlockHeight = 0;
        double TextBlockYPosition = 0;
        double LineThickness = 0;
        double LineYPosition = 0;
        double EquipmentYPosition = 0;
        
        List<TextBlock> textBlock;
        
        class TrackObjectsMeasure
        {

        }

        void DrawPickets(double _CurCoord)
        {
            var ViewingPickets = from r in Pickets where r.RigthShiftLine > _CurCoord - TrackLength / 2 && r.LeftShiftLine < _CurCoord + TrackLength / 2 select r;

            if (ViewingPickets.Count() == 0)
                return;

            var FirstPicket = ViewingPickets.First();
            double beforePicketRigthCanvasPoint = ViewingHalfCanvasWidth - (double)FirstPicket.LeftShiftLine * Scale;

            RectangleHeght = canvas1.ActualHeight / 30;
            if (RectangleHeght < MinRectangleHeight) RectangleHeght = MinRectangleHeight;

            RectangleYPosition = canvas1.ActualHeight / 2 + RectangleHeght;

            BorderHeightRectangle = RectangleHeght / 10;
            TextBlockHeight = RectangleHeght;
            TextBlockYPosition = RectangleYPosition - TextBlockHeight;
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

                DrawPicketRectangle(picket.Num, beforePicketRigthCanvasPoint, picketWidthInPixels);
                DrawPicketsNumbers(beforePicketRigthCanvasPoint, picket.Num, TextBlockYPosition);
                beforePicketRigthCanvasPoint = x;
            }

            DrawLineAbovePicketNumber(LineYPosition);
        }
        void DrawLineAbovePicketNumber(double _LineYPosition)
        {
            Line LBorder = new Line();
            LBorder.X1 = 0;
            LBorder.X2 = canvas1.ActualWidth*2;
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
