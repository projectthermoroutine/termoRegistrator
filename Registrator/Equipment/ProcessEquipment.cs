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
        public TrasformTrackEvent()
            : base()
        { }

        public long Coord { get; set; } = 0;
    }
    public class RefreshEquip : EventArgs
    {
        public _frame_coordinate Coordinate { get; }
        public long LengthOfViewedTrack { get; } = 0;
      
        public RefreshEquip(_frame_coordinate frame_Coordinate, long lengthOfViewedTrack, sbyte direction_) 
        {
            Coordinate = frame_Coordinate;
            LengthOfViewedTrack = lengthOfViewedTrack;
            direction = direction_;
        }
        public RefreshEquip(_frame_coordinate frame_Coordinate, long lengthOfViewedTrack)
        {
            Coordinate = frame_Coordinate;
            LengthOfViewedTrack = lengthOfViewedTrack;
        }
    
        public RefreshEquip() {}
        public sbyte direction { get; } = 0;
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
        public int curLine = -1;

        public long mmCoordinate = 0;
        static readonly NLog.Logger Log = LogManager.GetCurrentClassLogger();
       
        public int direction = 1;
        public long LengthOfViewedTrack;

        long m_cameraOffset = 0;
        bool applyCameraOffsetManual = false;

        public void SetCameraOffset(long cameraOffset) { m_cameraOffset = cameraOffset; applyCameraOffsetManual = true;  }
        public void ResetCameraOffset(){    applyCameraOffsetManual = false;  }

        bool zoomOccured = false;

        public void updateLengthOfViewedTrack(double TrackLength)
        {
            LengthOfViewedTrack = (long)TrackLength;
            zoomOccured = true;
        }

        public DB.metro_db_controller DbController { get; } = new DB.metro_db_controller(null);

        public ProcessEquipment()
        {
            NonUpdateIntervalCoordinate = new Interval(0, 0);
            LengthOfViewedTrack = Registrator.Properties.Settings.Default.TrackHalfVeiwSector;
            
            if (LengthOfViewedTrack == 0)
                LengthOfViewedTrack = 10000;//mm(10m)
        }

        public bool ProcessFrame(_irb_frame_info frameInfo)
        {
            var path_changed = DbController.setLineAndPath(frameInfo.coordinate.line, frameInfo.coordinate.path);

            try
            {
                pathChanged = path_changed;
                direction = frameInfo.coordinate.direction;
                process(frameInfo);
            }
            catch (FormatException)
            {
                Log.Warn("could not be detect path number\n");
            }

            return path_changed;
        }

        long mmCoordinateBefore = 0;
        bool pathChanged = false;
        public void process(_irb_frame_info frameInfo)
        {
           
            if (applyCameraOffsetManual)
                mmCoordinate = (long)frameInfo.coordinate.coordinate + (m_cameraOffset - frameInfo.coordinate.camera_offset);
            else
                mmCoordinate = (long)frameInfo.coordinate.coordinate;
            

            if ( NonUpdateIntervalCoordinate.max < mmCoordinate ||
                 NonUpdateIntervalCoordinate.min > mmCoordinate || 
                 pathChanged ||
                 zoomOccured
                )
            {
                NonUpdateIntervalCoordinate.max = mmCoordinate + LengthOfViewedTrack; 
                NonUpdateIntervalCoordinate.min = mmCoordinate - LengthOfViewedTrack;
                
                FireDrawTrackControl(new RefreshEquip(frameInfo.coordinate, LengthOfViewedTrack, frameInfo.coordinate.direction));

                mmCoordinateBefore = mmCoordinate;
                pathChanged = false;
                zoomOccured = false;
            }
            
            TrasformTrackEvent_.Coord = mmCoordinate;
            FireTransformTrack(TrasformTrackEvent_);
            
        }

        RefreshEquip RefreshEquip_ = new RefreshEquip();
        TrasformTrackEvent TrasformTrackEvent_ = new TrasformTrackEvent();
        public event EventHandler<RefreshEquip>         DrawTrackControlEventHandler;
        public event EventHandler<RefreshEquip>         ZoomTrackControl;
        public event EventHandler<TrasformTrackEvent>   TrasformTrackHandler;
        public event EventHandler<RefreshEquip>         DataGridHandler;
        public event EventHandler<RefreshEquip>         DataGridRefreshHandler;

        public virtual void FireZoomTrackControlEvent(RefreshEquip e)
        {
            ZoomTrackControl?.Invoke(this, e);
        }

        public virtual void FireDrawTrackControl(RefreshEquip e)
        {
            DrawTrackControlEventHandler?.Invoke(this, e);
        }
        public virtual void FireTransformTrack(TrasformTrackEvent e) {
            TrasformTrackHandler?.Invoke(this, e);
        }

        public virtual void FireDataGridDataChange(RefreshEquip e) {
            DataGridHandler?.Invoke(this, e);
        }
        public virtual void FireDataGridDataRefreshChange(RefreshEquip e){
            if (DataGridHandler != null) { DataGridRefreshHandler(this, e); }
        }
    }
}
