using System;
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
        public int direction=0;

        public FrameChangedEventNEW(int index, bool displayNewObjectArg, ulong coord, int duration_arg)
            : base()
        {
            m_index = index;
            m_displayNewObject = displayNewObjectArg;
            m_coord = coord;
            direction = duration_arg;
        }

        public int Index { get { return m_index; } set { m_index = value; } }
        public bool displayNewObject { get { return m_displayNewObject; } set { m_displayNewObject = value; } }
        public ulong Coord { get { return m_coord; } set { m_coord = value; } }
    }

    public class dataGridDataChange : EventArgs
    {
        private string m_name;
        private ulong  m_mmCoordinate;
        private int m_Npicket;
        private int m_curMaxTemperature;
        private int m_maxTemperature;
        private int m_shiftFromPicket;

        public dataGridDataChange(string name, ulong mmCoordinate, int Npicket, int curMaxTemperature, int maxTemperature, int shiftFromPicket)
            : base()
        {
            m_name = name;
            m_mmCoordinate = mmCoordinate;
            m_Npicket = Npicket;
            m_curMaxTemperature = curMaxTemperature;
            m_maxTemperature = maxTemperature;
            m_shiftFromPicket = shiftFromPicket;
        }

        public string Name { get { return m_name; } set { m_name = value; } }
        public string mmCoordinate { get { return Convert.ToString(m_mmCoordinate); } }
        public string Npicket { get { return Convert.ToString(m_Npicket); } }
        public string curMaxTemperature { get { return Convert.ToString(m_curMaxTemperature); } }
        public string maxTemperature { get { return Convert.ToString(m_maxTemperature); } }
        public string shiftFromPicket { get { return Convert.ToString(m_shiftFromPicket); } }
    }

    public class dataGridClearEvent:EventArgs
    {
        public dataGridClearEvent()
        { }
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
        public int curLine = -1;

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
       
        private IAsyncResult result;
        public IEnumerable<ResultLayouts> subqueryLayouts;

        private int curMaxtemperature = 20;

        bool displayNewObject = false;

        private ulong LineLength = 0;
        public int direction = 1;
        public ulong updateFreq = 5;
        public string curlineCode = "";

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
        public int getLineNumber(string lineCode)
        {
            
            var resStartCoordLine = (from r in DBHelper.dataTable_Lines.AsEnumerable() where r.LineCode == lineCode select new { r.LineNum});

            if (resStartCoordLine.Count() != 0)
            {
                curlineCode = lineCode;
                return resStartCoordLine.First().LineNum;
            }

            return -1;
        }
        public void refresh()      
        {
            lastCoordinate = 0;
            FireDataGridClear(new dataGridClearEvent());
        }
      
        public void process(ref _irb_frame_info frameInfo)
        {
            displayNewObject = false;

#if DEBUG    // SET COORDINATE
            mmCoordinate+=50;
#else
            mmCoordinate =  frameInfo.coordinate.coordinate;
#endif      
            if (direction == 0) // Train should be from coordinate begin
            {
                if (lastCoordinate < mmCoordinate)
                {
                    lastCoordinate = mmCoordinate + sampling_frequencies * 4;

                    DBHelper.getCoordinateObjects(mmCoordinate / 10, sampling_frequencies / 10);
                    displayNewObject = true;
                }

                if (lastCoordinate_viewSector < mmCoordinate)
                {
                    lastCoordinate_viewSector = mmCoordinate + sampling_frequencies / updateFreq;
                    
                    FireDataGridClear(new dataGridClearEvent());
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
                            FireDataGridDataChange(new dataGridDataChange(item.name, mmCoordinate, item.Npicket, curMaxtemperature, item.maxTemperature, item.shiftFromPicket));
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
                    lastCoordinate_viewSector = mmCoordinate + sampling_frequencies / updateFreq;
                    FireDataGridClear(new dataGridClearEvent());
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
                            FireDataGridDataChange(new dataGridDataChange(item.name,mmCoordinate,item.Npicket,curMaxtemperature,item.maxTemperature,item.shiftFromPicket));
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
        public event EventHandler<dataGridDataChange> DataGridHandler;
        public event EventHandler<dataGridClearEvent> DataGridClearHandler;
     
        public virtual void FireDataGridClear(dataGridClearEvent e)
        {
            EventHandler<dataGridClearEvent> handler = DataGridClearHandler;

            if(handler != null)
            {
                handler(this, e);
            }
        }

        public virtual void FireFrameChangedEventNEW(FrameChangedEventNEW e)
        {
            EventHandler<FrameChangedEventNEW> handler = FrameChangedHandlerNEW;
           
            if (handler != null)
            {
                handler(this, e);
            }
        }

        public virtual void FireDataGridDataChange(dataGridDataChange e)
        {
            EventHandler<dataGridDataChange> handler = DataGridHandler;
            
            if(handler != null)
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
