using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ThermoRoutineLib;
using System.Drawing;
using System.Windows.Forms;

namespace Registrator.Equipment
{
    public class ResultEquipCode
    {
        public int Code;
        public string name;
        public int shiftLine;
        public int X;
        public int Y;
        public int curTemperature;
        public int maxTemperature;
        public int Npicket;
        public int shiftFromPicket;
    }
    public class ResultEquipCodeFrame
    {
        public int Code;
        public string name;
        public int shiftLine;
        public int X;
        public int Y;
        public int curTemperature;
        public int maxTemperature;
        public int Npicket;
        public int shiftFromPicket;
    }
    public class ProcessEquipment
    {

        public DB.DataBaseHelper DBHelper;
        public int curLine = 0;

        public int sampling_frequencies = 30;    //
        public int NEAR_DISTANCE = 3;           //meters
        private int coordinatPlusNearDistance;

        public ulong lastCoordinate = 0;
        private IEnumerable<ResultEquipCode> subquery;
        public IEnumerable<ResultEquipCodeFrame> subqueryFrame;
        private Graphics g;
        private PictureBox pb;
        private Pen pen;
        private Brush brush;
        public int xPoint;
        public int yPoint;
        public int StartCoordinatX = 30;
        public int StartCoordinatY = 30;
        //-------------- for test ------------------------
        public int tempCounter = 0;
        public int tempCounter1 = 0;

        private DataGridView dg;
        private Registrator.EquipmentMonitor.MyDelegateFrameProcess d;
        private Registrator.EquipmentMonitor.MyDelegateFrameProcessDataGrid dDataGrid;
        private Registrator.EquipmentMonitor.MyDelegateFrameProcessDataGridClearAll dDataGridClearAll;
        private Registrator.EquipmentMonitor.MyDelegateFrameProcessDataGridClear dDataGridClear;
        private IAsyncResult result;

        private int curMaxtemperature = 20;
        public ProcessEquipment( DB.DataBaseHelper dbHelperArg)
        {
            DBHelper = dbHelperArg;
            
        }
        public void setLine(int line)
        {
            curLine = line;
            subquery = from r in DBHelper.dataTable_ProcessEquipment.AsEnumerable() where r.LineNum == curLine && r.Code!=0 select new ResultEquipCode { Code = r.Code, name=r.Object, shiftLine = r.shiftLine, X = r.x, Y = r.y , curTemperature = r.curTemperature, maxTemperature = r.maxTemperature, shiftFromPicket = r.shiftFromPicket, Npicket = r.Npicket };
        }
        public void setDataGrid(ref DataGridView dgArg)
        {
            dg = dgArg;
        }
        public void setDelegat(ref Registrator.EquipmentMonitor.MyDelegateFrameProcess dArg, ref Registrator.EquipmentMonitor.MyDelegateFrameProcessDataGrid dArg1, ref Registrator.EquipmentMonitor.MyDelegateFrameProcessDataGridClear dDataGridClearArg, Registrator.EquipmentMonitor.MyDelegateFrameProcessDataGridClearAll dDataGridClearAllArg)
        {
            d = dArg;
            dDataGrid = dArg1;
            dDataGridClear = dDataGridClearArg;
            dDataGridClearAll = dDataGridClearAllArg;
        }

        public void refresh()
        {
            if ((brush != null) && (pen != null) && (dg != null))
            {
                brush.Dispose();
                pen.Dispose();
                lastCoordinate = 0;
                dg.BeginInvoke(dDataGridClear);
            }
        }

        public void setGraphics(ref PictureBox pbArg)
        {
            g = pbArg.CreateGraphics();
            pb = pbArg;
            pen = new Pen(Color.Black, 2);
            brush = new SolidBrush(Color.Chocolate);
        }
        public void process(ref _irb_frame_info frameInfo)
        {
            tempCounter++;
           // if (lastCoordinate < frameInfo.coordinate.coordinate)
            if (lastCoordinate < (ulong)tempCounter) // temp
            {
                //lastCoordinate = frameInfo.coordinate.coordinate + (ulong)sampling_frequencies;
                lastCoordinate = (ulong)tempCounter + (ulong)sampling_frequencies; // temp
                int coordinat = (int)lastCoordinate;
                coordinatPlusNearDistance = coordinat + NEAR_DISTANCE;

                subqueryFrame = from r in subquery where r.shiftLine >= coordinatPlusNearDistance && r.shiftLine <= coordinatPlusNearDistance + sampling_frequencies select new ResultEquipCodeFrame { Code = r.Code, name = r.name, shiftLine = r.shiftLine, X = r.X, Y = r.Y, curTemperature = r.curTemperature, maxTemperature = r.maxTemperature, shiftFromPicket = r.shiftFromPicket, Npicket = r.Npicket };

                dg.BeginInvoke(dDataGridClearAll);
                //result = pb.BeginInvoke(d, 1);
                //result.AsyncWaitHandle.WaitOne();
               

                //int curMaxtemperature = (int)frameInfo.measure.max;
                //int curMaxtemperature = 10;     // FOR TEST

                //dg.Invoke(dDataGrid, "1", tempCounter.ToString(),"55", tempCounter.ToString(), tempCounter.ToString(),"1");     // FOR TEST

                //if (tempCounter1 > 40) tempCounter1 = 0;
                //xPoint = StartCoordinatX + tempCounter1++;
                //yPoint = StartCoordinatY + tempCounter1;

                ////g.FillEllipse(brush, xPoint, yPoint, 10, 10);
                //g.DrawEllipse(pen, 1, 1, 10, 10);

                if (curMaxtemperature > 50) curMaxtemperature = 20;
                curMaxtemperature++;

                if (subqueryFrame.Count() > 0)
                {
                    lock (this)
                    {
                        foreach (var item in subqueryFrame)
                        {
                            //brush.Dispose();
                            //pen.Dispose();
                            //pen = new Pen(Color.Black, 2);
                            //brush = new SolidBrush(Color.Chocolate);

                            //xPoint = item.X + StartCoordinatX;
                            //yPoint = item.Y + StartCoordinatY;

                            //g.FillEllipse(brush, xPoint, yPoint, 10, 10);
                            //g.DrawEllipse(pen, xPoint, yPoint, 10, 10);

                            dg.BeginInvoke(dDataGrid, item.name, Convert.ToString(tempCounter), Convert.ToString(item.Npicket), Convert.ToString(curMaxtemperature), Convert.ToString(item.maxTemperature), Convert.ToString(item.shiftFromPicket));

                            DBHelper.TblAdapter_ProcessEquipment.insertEquipTemperature(item.Code, item.curTemperature);            // FOR TEST
                            //DBHelper.TblAdapter_ProcessEquipment.insertEquipTemperature(item.Code, curMaxtemperature);
                        }
                    }
                }
            }
        }
    
        public void test()
        {
            //pb.
           // d(1);
            xPoint = 10 + StartCoordinatX;
            yPoint = 10 + StartCoordinatY;
            g.FillEllipse(brush, xPoint, yPoint, 10, 10);
            g.DrawEllipse(pen, xPoint, yPoint, 10, 10);
            //draw_coordinate_system();
        }
    }
}
