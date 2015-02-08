using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.IO;


namespace DrawToolsLib
{
    /// <summary>
    /// Rectangle tool
    /// </summary>
    class ToolRectangle : ToolRectangleBase
    {
        public ToolRectangle()
        {
            MemoryStream stream = new MemoryStream(Properties.Resources.Rectangle);
            ToolCursor = new Cursor(stream);
        }

        /// <summary>
        /// Create new rectangle
        /// </summary>
        public override void OnMouseDown(DrawingCanvas drawingCanvas, MouseButtonEventArgs e)
        {
            Point p = e.GetPosition(drawingCanvas);

            GraphicsRectangle rect = new GraphicsRectangle(
                p.X,
                p.Y,
                p.X + 1,
                p.Y + 1,
                drawingCanvas.LineWidth,
                drawingCanvas.ObjectColor,
                drawingCanvas.ActualScale);

            AddNewObject(drawingCanvas, rect);

            rect.AreaChangedEventHandler += drawingCanvas.AreaChangedEventFired;

            drawingCanvas.FireAreaAddedEvent(new AreaAddedEvent(rect.Id, ToolType.Rectangle, p.X, p.Y, p.X + 1, p.Y + 1));
        }

    }
}
