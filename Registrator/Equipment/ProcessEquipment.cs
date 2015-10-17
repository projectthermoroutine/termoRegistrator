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
        public FrameChangedEventNEW(int index, long coord, int duration_arg, IEnumerable<Registrator.DB.ResultEquipCodeFrame> objects)
            : base()
        {
            Index = index;
            Coord = coord;
            direction = duration_arg;
            this.objects = objects;
        }

        public int Index { get; set; }
        public long Coord { get; set; }
        public int direction { get; set; }
        public IEnumerable<Registrator.DB.ResultEquipCodeFrame> objects { get; set; }

    }
    public class TrasformTrackEvent : EventArgs
    {
        public TrasformTrackEvent(int index, long coord, int duration_arg)
            : base()
        {
            Index = index;
            Coord = coord;
            direction = duration_arg;
        }

        public int Index { get; set; }
        public long Coord { get; set; }
        public int direction { get; set; }

    }
    public class dataGridDataChange : EventArgs
    {
        private string m_name;
        private long  m_mmCoordinate;
        private int m_Npicket;
        private int m_curMaxTemperature;
        private int m_maxTemperature;
        private int m_shiftFromPicket;

        public dataGridDataChange(string name, long mmCoordinate, int Npicket, int curMaxTemperature, int maxTemperature, int shiftFromPicket)
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
        public struct Interval
        {
            public Interval(long min, long max)
            {
                this.min = min; this.max = max;
            }
            public long min;
            public long max;
        }

        Interval NonUpdateIntervalCoordinate;
        private DB.metro_db_controller _db_controller = null;
        public int curLine = -1;

        public long lastCoordinate_viewSector = 0;
      
        public int StartCoordinatX = 30;
        public int StartCoordinatY = 30;
        
        public long mmCoordinate = 0;
        static readonly Logger Log = LogManager.GetCurrentClassLogger();
        
       
        public IEnumerable<ResultLayouts> subqueryLayouts;

        private int curMaxtemperature = 20;

        public int direction = 1;
        public long halfLengthOfViewedTrack;
        public long updatefrequency=0;

        public int cameraOffset = 0;
        public bool apply_or_not = false;
        
        public ProcessEquipment(DB.metro_db_controller db_controller)
        {
            _db_controller = db_controller;
            NonUpdateIntervalCoordinate = new Interval(0, 0);
            objects = new List<Registrator.DB.ResultEquipCodeFrame>();
        }
     
        public void refresh()      
        {
            FireDataGridClear(new dataGridClearEvent());
        }

        public void track_process(ref _irb_frame_info frameInfo)
        {
#if DEBUG
            _db_controller.setLineAndPath("красн", "1");
            //------------------------------------------------------- PROCESS EQUIPMENT ------------------------------------------------------------
            process(ref frameInfo);
            //--------------------------------------------------------------------------------------------------------------------------------------
#else

            try
            {
                _db_controller.setLineAndPath(frameInfo.coordinate.line, Convert.ToInt32(frameInfo.coordinate.path));
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
#endif
        }

        IEnumerable<Registrator.DB.ResultEquipCodeFrame> objects;

        public void process(ref _irb_frame_info frameInfo)
        {


#if DEBUG    // SET COORDINATE
            mmCoordinate += 50;
#else
            if(apply_or_not)
                mmCoordinate = (long)frameInfo.coordinate.coordinate + cameraOffset;
            else
                mmCoordinate = (long)frameInfo.coordinate.coordinate + frameInfo.coordinate.camera_offset;
#endif      

            if (NonUpdateIntervalCoordinate.max < mmCoordinate ||
                NonUpdateIntervalCoordinate.min > mmCoordinate)
                {
                    NonUpdateIntervalCoordinate.max = mmCoordinate + updatefrequency;
                    NonUpdateIntervalCoordinate.min = mmCoordinate - updatefrequency;

                    objects = _db_controller.get_objects_by_coordinate(mmCoordinate, halfLengthOfViewedTrack);
                    
                    FireFrameChangedEventNEW(new FrameChangedEventNEW(0, mmCoordinate, (int)frameInfo.coordinate.direction, objects));

                    FireDataGridClear(new dataGridClearEvent());
#if DEBUG           // SET TEMPERATURE
                    if (curMaxtemperature > 50) curMaxtemperature = 20;
                    curMaxtemperature++;
#else
                    curMaxtemperature = (int)frameInfo.measure.tmax;
#endif
                    foreach (var item in objects)
                    {
                        if (item.shiftLine < item.shiftLine + halfLengthOfViewedTrack && item.shiftLine > item.shiftLine - halfLengthOfViewedTrack)
                        {
                            //  SET equip to DATAGRID 
                            FireDataGridDataChange(new dataGridDataChange(item.name, mmCoordinate, item.Npicket, curMaxtemperature, item.maxTemperature, item.shiftFromPicket));
                            //  INSERT MAX TEMPERATURE (for cur equip)
                            _db_controller.save_object_temperature(item.Code, (float)item.curTemperature);
                        }
                    }
                }
            
            // DRAW equip ON TRACK CONTROL NEW
                FireTransformTrack(new TrasformTrackEvent(0, mmCoordinate, (int)frameInfo.coordinate.direction));
                
        }

        public ulong tmp_coord = 0;
        public event EventHandler<FrameChangedEventNEW> FrameChangedHandlerNEW;
        public event EventHandler<TrasformTrackEvent>   TrasformTrackHandler;
        public event EventHandler<lineLengthEvent>      lineLengthHandler;
        public event EventHandler<dataGridDataChange>   DataGridHandler;
        public event EventHandler<dataGridClearEvent>   DataGridClearHandler;
     
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
        public virtual void FireTransformTrack(TrasformTrackEvent e)
        {
            EventHandler<TrasformTrackEvent> handler = TrasformTrackHandler;

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
