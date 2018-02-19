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
      
        public RefreshEquip(long mmCoordinate, long LengthOfViewedTrack,sbyte direction_) 
        {
            m_mmCoordinate = mmCoordinate;
            m_LengthOfViewedTrack = LengthOfViewedTrack;
            direction = direction_;
        }
        public RefreshEquip(long mmCoordinate, long LengthOfViewedTrack)
        {
            m_mmCoordinate = mmCoordinate;
            m_LengthOfViewedTrack = LengthOfViewedTrack;
        }
    
        public RefreshEquip() {}
        public string Name { get { return m_name; } set { m_name = value; } }
        public long mmCoordinate { get { return m_mmCoordinate; } set { m_mmCoordinate = value; } }
        public string Npicket { get { return Convert.ToString(m_Npicket); } }
        public string curMaxTemperature { get { return Convert.ToString(m_curMaxTemperature); } }
        public string maxTemperature { get { return Convert.ToString(m_maxTemperature); } }
        public string shiftFromPicket { get { return Convert.ToString(m_shiftFromPicket); } }
        public sbyte direction { get; set; }
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

        long m_cameraOffset = 0;
        bool applyCameraOffsetManual = false;

        public void SetCameraOffset(long cameraOffset) { m_cameraOffset = cameraOffset; applyCameraOffsetManual = true;  }
        public void ResetCameraOffset(){    applyCameraOffsetManual = false;  }

        bool zoomOccure = false;

        public void updateLengthOfViewedTrack(double TrackLenght)
        {
            LengthOfViewedTrack = (long)TrackLenght;
            zoomOccure = true;
        }
        public ProcessEquipment(DB.metro_db_controller db_controller)
        {
            _db_controller = db_controller;
            NonUpdateIntervalCoordinate = new Interval(0, 0);
            objects = new List<Registrator.DB.ResultEquipCode>();
            LengthOfViewedTrack = Registrator.Properties.Settings.Default.TrackHalfVeiwSector;
            
            if (LengthOfViewedTrack == 0)
                LengthOfViewedTrack = 10000;//mm(10m)

            //_db_controller.ChangePath += _db_controller_ChangePath;
#if DEBUG1
            mmCoordinate = 0;
            direction = 0;
#endif
        }

     
        public void track_process(_irb_frame_info frameInfo)
        {
#if DEBUG1
            _db_controller.setLineAndPath("Зелёная1", "Бакинский");
            //------------------------------------------------------- PROCESS EQUIPMENT ------------------------------------------------------------
            process(ref frameInfo);
            //--------------------------------------------------------------------------------------------------------------------------------------
#else
            try
            {
                pathChanged = _db_controller.setLineAndPath(frameInfo.coordinate.line,frameInfo.coordinate.path);
                direction = frameInfo.coordinate.direction;
                //------------------------------------------------------- PROCESS EQUIPMENT ------------------------------------------------------------
                process(frameInfo);
                //--------------------------------------------------------------------------------------------------------------------------------------
            }
            catch (FormatException e)
            {
                Log.Warn("could not be detect path number\n");
                return;
            }
#endif
        }

        IEnumerable<Registrator.DB.ResultEquipCode> objects;
        long mmCoordinateBefore = 0;
        bool pathChanged = false;
        public void process(_irb_frame_info frameInfo)
        {
           
           
#if DEBUG1   // SET COORDINATE
            mmCoordinate += 20;
#else
            if (applyCameraOffsetManual)
                mmCoordinate = (long)frameInfo.coordinate.coordinate + (m_cameraOffset - frameInfo.coordinate.camera_offset);
            else
                mmCoordinate = (long)frameInfo.coordinate.coordinate;// + frameInfo.coordinate.camera_offset;
            
#endif      

            if (NonUpdateIntervalCoordinate.max < mmCoordinate ||
                NonUpdateIntervalCoordinate.min > mmCoordinate || 
                pathChanged ||
                zoomOccure)
            {
                NonUpdateIntervalCoordinate.max = mmCoordinate + LengthOfViewedTrack; 
                NonUpdateIntervalCoordinate.min = mmCoordinate - LengthOfViewedTrack;
                
                FireDrawTrackControl(new RefreshEquip(mmCoordinate,LengthOfViewedTrack, frameInfo.coordinate.direction));
                FireDataGridDataRefreshChange(new RefreshEquip(mmCoordinate,LengthOfViewedTrack*2));

                mmCoordinateBefore = mmCoordinate;
                pathChanged = false;
                zoomOccure = false;
            }
            
            TrasformTrackEvent_.Coord = mmCoordinate;
            //TrasformTrackEvent_.direction = (int)frameInfo.coordinate.direction;
            FireTransformTrack(TrasformTrackEvent_);
            
            RefreshEquip_.mmCoordinate = mmCoordinate;
            RefreshEquip_.LengthOfViewedTrack = LengthOfViewedTrack;
            RefreshEquip_.direction = frameInfo.coordinate.direction;
            FireDataGridDataChange(RefreshEquip_);
        }

        RefreshEquip RefreshEquip_ = new RefreshEquip();
        TrasformTrackEvent TrasformTrackEvent_ = new TrasformTrackEvent(0,0,0);
        public event EventHandler<RefreshEquip>         DrawTrackControlEventHandler;
        public event EventHandler<RefreshEquip>         ZoomTrackControl;
        public event EventHandler<TrasformTrackEvent>   TrasformTrackHandler;
        public event EventHandler<RefreshEquip>         DataGridHandler;
        public event EventHandler<RefreshEquip>         DataGridRefreshHandler;

        public virtual void FireZoomTrackControlEvent(RefreshEquip e)
        {
            if (ZoomTrackControl != null) { ZoomTrackControl(this, e); }
        }

        public virtual void FireDrawTrackControl(RefreshEquip e)
        {
            if (DrawTrackControlEventHandler != null) { DrawTrackControlEventHandler(this, e); }
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
