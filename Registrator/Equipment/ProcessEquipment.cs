using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ThermoRoutineLib;
using System.Drawing;
using System.Windows.Forms;
using NLog;

namespace Registrator.Equipment
{
    public class FrameChangedEventNEW : EventArgs
    {
        public FrameChangedEventNEW(int index, bool displayNewObjectArg, ulong coord, int duration_arg, IEnumerable<Registrator.DB.ResultEquipCodeFrame> objects)
            : base()
        {
            Index = index;
            displayNewObject = displayNewObjectArg;
            Coord = coord;
            direction = duration_arg;
            this.objects = objects;
        }

        public int Index { get; set; }
        public bool displayNewObject { get; set; }
        public ulong Coord { get; set; }
        public int direction { get; set; }
        public IEnumerable<Registrator.DB.ResultEquipCodeFrame> objects { get; set; }

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
        private DB.metro_db_controller _db_controller = null;
        public int curLine = -1;

        public ulong sampling_frequencies = 5000;    //30000 mm - 3000 cm - 30 m
        //public int NEAR_DISTANCE = 3;              //meters

        public ulong lastCoordinate = 0;
        public ulong lastCoordinate_viewSector = 0;
      
        public int xPoint;
        public int yPoint;
        public int StartCoordinatX = 30;
        public int StartCoordinatY = 30;
        
        public ulong mmCoordinate = 0;
        public int tempCounter1 = 0;
        static readonly Logger Log = LogManager.GetCurrentClassLogger();
        
        //private DataGridView dataGridView_;
       
        private IAsyncResult result;
        public IEnumerable<ResultLayouts> subqueryLayouts;

        private int curMaxtemperature = 20;

        bool displayNewObject = false;

        //private ulong LineLength = 0;
        public int direction = 1;
        public ulong updateFreq = 5;
        public string curlineCode = "";

        public int cameraOffset = 0;
        public bool apply_or_not = false;
        private bool path_detected = false;

        public ProcessEquipment(DB.metro_db_controller db_controller)
        {
            _db_controller = db_controller;
            objects = new List<Registrator.DB.ResultEquipCodeFrame>();
          
        }
        public void setLine(int line, int path)
        {
            curLine = line;
            _db_controller.retrieve_groups();
            _db_controller.get_objects(line, path);
        }
        public int getLineNumber(string lineCode)
        {
            var line_number = _db_controller.get_line_number(lineCode);
            if (line_number != -1)
                curlineCode = lineCode;

            return line_number;
        }
        public void refresh()      
        {
            lastCoordinate = 0;
            FireDataGridClear(new dataGridClearEvent());
        }

        public void track_process(ref _irb_frame_info frameInfo)
        {
            int curline=0;
#if DEBUG
            if (curlineCode != "красн")
            {
                curline = getLineNumber("красн");

                if (curline != -1)
                {
                    setLine(curline, 1);
                    direction = frameInfo.coordinate.direction;

                    //------------------------------------------------------- PROCESS EQUIPMENT ------------------------------------------------------------
                    process(ref frameInfo);
                    //--------------------------------------------------------------------------------------------------------------------------------------
                }
            }
            else
                process(ref frameInfo);

#else

            if (curlineCode != frameInfo.coordinate.line && path_detected == true)
            {
                curline = getLineNumber(frameInfo.coordinate.line);

                if (curline != -1)
                {
                    try
                    {
                        setLine(curline, Convert.ToInt32(frameInfo.coordinate.path));
                        path_detected = true;
                        direction = frameInfo.coordinate.direction;
                        //------------------------------------------------------- PROCESS EQUIPMENT ------------------------------------------------------------
                        process(ref frameInfo);
                        //--------------------------------------------------------------------------------------------------------------------------------------
                    }
                    catch (FormatException e)
                    {
                        path_detected = false;
                        Log.Warn("could not be detect path number\n");
                        return;
                    }
                }
            }
            else
              process(ref frameInfo);
#endif
        }

        IEnumerable<Registrator.DB.ResultEquipCodeFrame> objects;
        public void process(ref _irb_frame_info frameInfo)
        {

            displayNewObject = false;

#if DEBUG    // SET COORDINATE
            mmCoordinate += 50;
#else
            if(apply_or_not)
                mmCoordinate = (ulong)((long)frameInfo.coordinate.coordinate + cameraOffset);
            else
                mmCoordinate = (ulong)((long)frameInfo.coordinate.coordinate + frameInfo.coordinate.camera_offset);
#endif      

                if (lastCoordinate < mmCoordinate)
                {
                    lastCoordinate = mmCoordinate + sampling_frequencies * 4;

                    objects = _db_controller.get_objects_by_coordinate(mmCoordinate / 10, sampling_frequencies / 10);
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
                 foreach (var item in objects)
                    {
                        if (item.shiftLine < item.shiftLine + sampling_frequencies / 10 && item.shiftLine > item.shiftLine - sampling_frequencies / 10)
                        {
                            //  SET equip to DATAGRID 
                            FireDataGridDataChange(new dataGridDataChange(item.name, mmCoordinate, item.Npicket, curMaxtemperature, item.maxTemperature, item.shiftFromPicket));
                            //  INSERT MAX TEMPERATURE (for cur equip)
                            _db_controller.save_object_temperature(item.Code, (float)item.curTemperature);
                        }
                    }
                }

                // DRAW equip ON TRACK CONTROL NEW
                FireFrameChangedEventNEW(new FrameChangedEventNEW(0, displayNewObject, mmCoordinate, 0, objects));
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
