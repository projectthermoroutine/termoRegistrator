﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ThermoRoutineLib;
using System.Drawing;
using System.Windows.Forms;

namespace Registrator.Equipment
{
    public class FrameChangedEventNEW : EventArgs
    {
        private int m_index;
        private bool m_displayNewObject;
        private ulong m_coord = 0;
        public int duration=0;

        public FrameChangedEventNEW(int index, bool displayNewObjectArg, ulong coord, int duration_arg)
            : base()
        {
            m_index = index;
            m_displayNewObject = displayNewObjectArg;
            m_coord = coord;
            duration = duration_arg;
        }

        public int Index { get { return m_index; } set { m_index = value; } }
        public bool displayNewObject { get { return m_displayNewObject; } set { m_displayNewObject = value; } }
        public ulong Coord { get { return m_coord; } set { m_coord = value; } }
    }

    public class lineLengthEvent : EventArgs
    {
        public ulong lineLength = 0;

        public lineLengthEvent(ulong LineLength_arg)
            : base()
        {
            lineLength = LineLength_arg;
        }
    }

    public class ResultLayouts
    {
        public int Code;
    }

    public class ProcessEquipment
    {
        public DB.DataBaseHelper DBHelper = null;
        public int curLine = 0;

        public ulong sampling_frequencies = 5000;    //30000 mm - 3000 cm - 30 m
        //public int NEAR_DISTANCE = 3;              //meters

        public ulong lastCoordinate = 0;
        public ulong lastCoordinate_viewSector = 0;
        private IEnumerable<Registrator.DB.ResultEquipCode> subquery;
        public  IEnumerable<Registrator.DB.ResultEquipCodeFrame> subqueryFrame;
      
        public int xPoint;
        public int yPoint;
        public int StartCoordinatX = 30;
        public int StartCoordinatY = 30;
        
        public ulong mmCoordinate = 0;
        public int tempCounter1 = 0;

        private DataGridView dataGridView_;
        //private Registrator.EquipmentMonitor.MyDelegateFrameProcess d;
        private Registrator.EquipmentMonitor.MyDelegateFrameProcessDataGrid dDataGrid;
        private Registrator.EquipmentMonitor.MyDelegateFrameProcessDataGridClearAll dDataGridClearAll;
        private Registrator.EquipmentMonitor.MyDelegateFrameProcessDataGridClear dDataGridClear;
        private IAsyncResult result;
        public IEnumerable<ResultLayouts> subqueryLayouts;

        private int curMaxtemperature = 20;

        bool displayNewObject = false;

        private ulong LineLength = 0;
        public int duration = 1;

        public ProcessEquipment(ref DB.DataBaseHelper dbHelperArg)
        {
            DBHelper = dbHelperArg;
           
        }
        public void setLine(int line)
        {
            curLine = line;

            DBHelper.fill_Equip_Filter_Object();
            DBHelper.getLineObjects(line);

            subqueryLayouts = (from r in DBHelper.dataTable_LayoutTable.AsEnumerable() where r.Line == line select new ResultLayouts { Code = r.Code });  // calc line length 

            var resStartCoordLine = (from r in DBHelper.dataTable_Lines.AsEnumerable() where r.LineNum == line select new { r.StartCoordinate });

            LineLength += (ulong)resStartCoordLine.First().StartCoordinate;

            foreach (var item in subqueryLayouts)
            {
                var resPicketLength = (from r in DBHelper.dataTable_PicketsTable.AsEnumerable() where r.Peregon == item.Code select new { r.Dlina });

                foreach (var itemDlina in resPicketLength)
                    LineLength += (ulong)itemDlina.Dlina;
            }

            FireSetLineLength(new lineLengthEvent(LineLength));
        }
        public void setDataGrid(ref DataGridView dgArg)
        {
            dataGridView_ = dgArg;
        }
        public void setDelegat(/*ref Registrator.EquipmentMonitor.MyDelegateFrameProcess dArg,*/ ref Registrator.EquipmentMonitor.MyDelegateFrameProcessDataGrid dArg1, ref Registrator.EquipmentMonitor.MyDelegateFrameProcessDataGridClear dDataGridClearArg, Registrator.EquipmentMonitor.MyDelegateFrameProcessDataGridClearAll dDataGridClearAllArg)
        {
            //d = dArg;
            dDataGrid = dArg1;
            dDataGridClear = dDataGridClearArg;
            dDataGridClearAll = dDataGridClearAllArg;
        }


        public void refresh()      
        {
            if (dataGridView_ != null)
            {
                lastCoordinate = 0;
                dataGridView_.BeginInvoke(dDataGridClearAll);
            }
        }
      
        public void process(ref _irb_frame_info frameInfo)
        {
            displayNewObject = false;

#if DEBUG    // SET COORDINATE
            mmCoordinate+=50;
#else
               mmCoordinate =  frameInfo.coordinate.coordinate / 10;
#endif      
            if (duration == 0) // Train should be from coordinate begin
            {
                if (lastCoordinate < mmCoordinate)
                {
                    lastCoordinate = mmCoordinate + sampling_frequencies * 4;

                    DBHelper.getCoordinateObjects(mmCoordinate / 10, sampling_frequencies / 10);
                    displayNewObject = true;

                }

                if (lastCoordinate_viewSector < mmCoordinate)
                {
                    lastCoordinate_viewSector = mmCoordinate + sampling_frequencies/5;

                    dataGridView_.BeginInvoke(dDataGridClearAll); // CLEAR DATAGRID
#if DEBUG       // SET TEMPERATURE
                    if (curMaxtemperature > 50) curMaxtemperature = 20;
                    curMaxtemperature++;
#else
                 curMaxtemperature = (int)frameInfo.measure.tmax;
#endif
                    foreach (var item in DBHelper.subqueryFrame)
                    {
                        if (item.shiftLine < item.shiftLine + sampling_frequencies / 10 && item.shiftLine > item.shiftLine - sampling_frequencies / 10)
                        {
                            //  SET equip to DATAGRID 
                            dataGridView_.BeginInvoke(dDataGrid, item.name, Convert.ToString(mmCoordinate), Convert.ToString(item.Npicket), Convert.ToString(curMaxtemperature), Convert.ToString(item.maxTemperature), Convert.ToString(item.shiftFromPicket));
                            //  INSERT MAX TEMPERATURE (for cur equip)
                            DBHelper.TblAdapter_ProcessEquipment.insertEquipTemperature(item.Code, item.curTemperature);
                        }
                    }
                }


                // DRAW equip ON TRACK CONTROL NEW
                FireFrameChangedEventNEW(new FrameChangedEventNEW(0, displayNewObject, mmCoordinate, 0));
            }
            else
            {
                if (lastCoordinate < mmCoordinate)
                {
                    lastCoordinate = mmCoordinate + sampling_frequencies * 4;

                    DBHelper.getCoordinateObjectsDuration((mmCoordinate / 10), sampling_frequencies / 10, LineLength);
                    displayNewObject = true;

                }

                if (lastCoordinate_viewSector < mmCoordinate)
                {
                    lastCoordinate_viewSector = mmCoordinate + sampling_frequencies/5;

                    dataGridView_.BeginInvoke(dDataGridClearAll); // CLEAR DATAGRID
#if DEBUG       // SET TEMPERATURE
                    if (curMaxtemperature > 50) curMaxtemperature = 20;
                    curMaxtemperature++;
#else
                 curMaxtemperature = (int)frameInfo.measure.tmax;
#endif
                    tmp_coord = LineLength - (mmCoordinate / 10);

                    foreach (var item in DBHelper.subqueryFrame)
                    {
                        if (item.shiftLine < tmp_coord + sampling_frequencies / 10 && item.shiftLine > tmp_coord - sampling_frequencies / 10)
                        {
                            //  SET equip to DATAGRID 
                            dataGridView_.BeginInvoke(dDataGrid, item.name, Convert.ToString(mmCoordinate), Convert.ToString(item.Npicket), Convert.ToString(curMaxtemperature), Convert.ToString(item.maxTemperature), Convert.ToString(item.shiftFromPicket));
                            //  INSERT MAX TEMPERATURE (for cur equip)
                            DBHelper.TblAdapter_ProcessEquipment.insertEquipTemperature(item.Code, item.curTemperature);
                        }
                    }
                }

                // DRAW equip ON TRACK CONTROL NEW
               // tmp_coord = LineLength*10 - mmCoordinate;
                FireFrameChangedEventNEW(new FrameChangedEventNEW(0, displayNewObject, mmCoordinate, 1));
            }
        }
        public ulong tmp_coord = 0;
        public event EventHandler<FrameChangedEventNEW> FrameChangedHandlerNEW;
        public event EventHandler<lineLengthEvent> lineLengthHandler;
        public virtual void FireFrameChangedEventNEW(FrameChangedEventNEW e)
        {
            EventHandler<FrameChangedEventNEW> handler = FrameChangedHandlerNEW;

            if (handler != null)
            {
                handler(this, e);
            }
        }
        public virtual void FireSetLineLength(lineLengthEvent e)
        {
            EventHandler<lineLengthEvent> handler = lineLengthHandler;

            if (handler != null)
            {
                handler(this, e);
            }
        }
    }
}
