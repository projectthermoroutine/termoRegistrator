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
    public class TrasformTrackEvent : EventArgs
    {
        public TrasformTrackEvent(int index, long coord, int duration_arg)
            : base()
        {
            Coord = coord;
        }

        public long Coord { get; set; }
    }
    public class RefreshEquip : EventArgs
    {
        private string m_name="";
        private long  m_mmCoordinate=0;
        private int m_Npicket=0;
        private int m_curMaxTemperature=0;
        private int m_maxTemperature=0;
        private int m_shiftFromPicket=0;
        private long m_LengthOfViewedTrack=0;
        

        public RefreshEquip(string name, long mmCoordinate, int Npicket, int curMaxTemperature, int maxTemperature, int shiftFromPicket)
            : base()
        {
            m_name = name;
            m_mmCoordinate = mmCoordinate;
            m_Npicket = Npicket;
            m_curMaxTemperature = curMaxTemperature;
            m_maxTemperature = maxTemperature;
            m_shiftFromPicket = shiftFromPicket;
        }
        public RefreshEquip(long coord, int duration_arg)
        {
            Coord = coord;
            direction = duration_arg;
        }
        public RefreshEquip(long mmCoordinate, long LengthOfViewedTrack) 
        {
            m_mmCoordinate = mmCoordinate;
            m_LengthOfViewedTrack = LengthOfViewedTrack;
        }
        public RefreshEquip( long LengthOfViewedTrack)
        {
            m_LengthOfViewedTrack = LengthOfViewedTrack;
        }
        public RefreshEquip() {}
        public string Name { get { return m_name; } set { m_name = value; } }
        public long mmCoordinate { get { return m_mmCoordinate; } set { m_mmCoordinate = value; } }
        public string Npicket { get { return Convert.ToString(m_Npicket); } }
        public string curMaxTemperature { get { return Convert.ToString(m_curMaxTemperature); } }
        public string maxTemperature { get { return Convert.ToString(m_maxTemperature); } }
        public string shiftFromPicket { get { return Convert.ToString(m_shiftFromPicket); } }
        public long Coord { get; set; }
        public int direction { get; set; }
        public long LengthOfViewedTrack { get { return m_LengthOfViewedTrack; } set { m_LengthOfViewedTrack = value; } }
    }

    public class dataGridClearEvent:EventArgs
    {
        public dataGridClearEvent()
        { }
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

        public long mmCoordinate = 0;
        static readonly NLog.Logger Log = LogManager.GetCurrentClassLogger();
       
        private int curMaxtemperature = 20;

        public int direction = 1;
        public long LengthOfViewedTrack;

        public int cameraOffset = 0;
        public bool apply_or_not = false;

        public void updateLengthOfViewedTrack(double ZoomCoefficient)
        {
            LengthOfViewedTrack = (long)(((double)LengthOfViewedTrack)*ZoomCoefficient);

            NonUpdateIntervalCoordinate.max = mmCoordinateBefore + LengthOfViewedTrack;
            NonUpdateIntervalCoordinate.min = mmCoordinateBefore - LengthOfViewedTrack;

            FireDataGridDataRefreshChange(new RefreshEquip(mmCoordinate, LengthOfViewedTrack));

        }
        public ProcessEquipment(DB.metro_db_controller db_controller)
        {
            _db_controller = db_controller;
            NonUpdateIntervalCoordinate = new Interval(0, 0);
            objects = new List<Registrator.DB.ResultEquipCodeFrame>();
        }

        public ProcessEquipment(ref DB.metro_db_controller db_controllerArg)
        {
            _db_controller = db_controllerArg;
            NonUpdateIntervalCoordinate = new Interval(0, 0);
            objects = new List<Registrator.DB.ResultEquipCodeFrame>();
        }

        public void track_process(ref _irb_frame_info frameInfo)
        {
#if DEBUG
            _db_controller.setLineAndPath("Зелёная", "Бакинский");
            //------------------------------------------------------- PROCESS EQUIPMENT ------------------------------------------------------------
            process(ref frameInfo);
            //--------------------------------------------------------------------------------------------------------------------------------------
#else

            try
            {
                _db_controller.setLineAndPath(frameInfo.coordinate.line,frameInfo.coordinate.path);
                direction = frameInfo.coordinate.direction;
                //------------------------------------------------------- PROCESS EQUIPMENT ------------------------------------------------------------
                process(ref frameInfo);
                //--------------------------------------------------------------------------------------------------------------------------------------
            }
            catch (FormatException e)
            {
                Log.Warn("could not be detect path number\n");
                return;
            }
#endif
        }

        IEnumerable<Registrator.DB.ResultEquipCodeFrame> objects;
        long mmCoordinateBefore = 0;
        public void process(ref _irb_frame_info frameInfo)
        {


#if DEBUG   // SET COORDINATE
            mmCoordinate -= 20;
#else
            if(apply_or_not)
                mmCoordinate = (long)frameInfo.coordinate.coordinate + cameraOffset;
            else
                mmCoordinate = (long)frameInfo.coordinate.coordinate + frameInfo.coordinate.camera_offset;
#endif      

            if (NonUpdateIntervalCoordinate.max < mmCoordinate ||
                NonUpdateIntervalCoordinate.min > mmCoordinate)
            {
                NonUpdateIntervalCoordinate.max = mmCoordinate + LengthOfViewedTrack; 
                NonUpdateIntervalCoordinate.min = mmCoordinate - LengthOfViewedTrack;
                
                _db_controller.get_objects_by_coordinate(mmCoordinate, LengthOfViewedTrack);

                FireFrameChangedEventNEW(new RefreshEquip(mmCoordinate, (int)frameInfo.coordinate.direction));
                FireDataGridDataRefreshChange(new RefreshEquip(mmCoordinate,LengthOfViewedTrack));
#if DEBUG       // SET TEMPERATURE
//              if (curMaxtemperature > 50) curMaxtemperature = 20;
//               curMaxtemperature++;
//#else
//               curMaxtemperature = (int)frameInfo.measure.tmax;
#endif
                mmCoordinateBefore = mmCoordinate;
            }
            
            TrasformTrackEvent_.Coord = mmCoordinate;
            //TrasformTrackEvent_.direction = (int)frameInfo.coordinate.direction;
            FireTransformTrack(TrasformTrackEvent_);
            
            RefreshEquip_.mmCoordinate = mmCoordinate;
            RefreshEquip_.LengthOfViewedTrack = LengthOfViewedTrack;
            FireDataGridDataChange(RefreshEquip_);
        }

        RefreshEquip RefreshEquip_ = new RefreshEquip();
        TrasformTrackEvent TrasformTrackEvent_ = new TrasformTrackEvent(0,0,0);
        public event EventHandler<RefreshEquip>         FrameChangedHandlerNEW;
        public event EventHandler<RefreshEquip>         ZoomTrackControl;
        public event EventHandler<TrasformTrackEvent>   TrasformTrackHandler;
        public event EventHandler<RefreshEquip>         DataGridHandler;
        public event EventHandler<RefreshEquip>         DataGridRefreshHandler;
        public virtual void FireZoomTrackControlEvent(RefreshEquip e)
        {
            if (ZoomTrackControl != null) { ZoomTrackControl(this, e); }
        }

        public virtual void FireFrameChangedEventNEW(RefreshEquip e)
        {
            if (FrameChangedHandlerNEW != null) { FrameChangedHandlerNEW(this, e); }
        }
        public virtual void FireTransformTrack(TrasformTrackEvent e) {
            if (TrasformTrackHandler != null) { TrasformTrackHandler(this, e); }
        }

        public virtual void FireDataGridDataChange(RefreshEquip e) {
            if (DataGridHandler != null)  { DataGridHandler(this, e); }
        }
        public virtual void FireDataGridDataRefreshChange(RefreshEquip e){
            if (DataGridHandler != null) { DataGridRefreshHandler(this, e); }
        }
    }
}
