using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;
using System.Collections;
using System.Windows.Forms.Integration;
using System.Windows.Threading;

namespace Registrator
{
    using map_objects_list = List<measure_object>;
    
    public partial class TrackPanel : ToolWindow
    {
        public class TrackScaleEventArgs : EventArgs
        {
            private double ZoomCoefficient_;
            public TrackScaleEventArgs(double ZoomCoefficient) { ZoomCoefficient_ = ZoomCoefficient; }
            public double ZoomCoefficient { get { return ZoomCoefficient_; } }
        }

        public IEnumerable<Registrator.DB.ResultEquipCodeFrame> Objects { set { m_trackControlNew.setObjects(value); } }

        TrackControlNew         m_trackControlNew;
        DB.metro_db_controller  DBController;

        public void setDBController( DB.metro_db_controller db_controller)
        {
            m_trackControlNew.setDBController( db_controller);
            DBController = db_controller;
        }

        public TrackPanel()
        {
            InitializeComponent();
            
            elementHost1.Dock = DockStyle.Fill;

            m_trackControlNew = new TrackControlNew();
            
            elementHost1.Child = m_trackControlNew;

            this.panel1.Controls.Add(elementHost1);
            RefreshDelegateObj = new UpdateDelegate(m_trackControlNew.UpdateTrack);
            TransformDelegateObj = new TransformDelegate(m_trackControlNew.Transform);
            SetTrackLengthDelegateObj = new SetTrackLengthDelegate(m_trackControlNew.setTrackLength);
        }

        private delegate void SetCoordDelegate(int x, int y);
        private delegate void SetCoordDelegateNEW(Equipment.RefreshEquip data);
        private delegate void delegate_callTransformTrack(Equipment.TrasformTrackEvent data);
        private delegate void SetTrackLengthDelegate(double e);

        public delegate void UpdateDelegate();
        public delegate void TransformDelegate();
        UpdateDelegate RefreshDelegateObj;
        TransformDelegate TransformDelegateObj;
        SetTrackLengthDelegate SetTrackLengthDelegateObj;

        public void RefreshTrack()
        {
            m_trackControlNew.Dispatcher.BeginInvoke(DispatcherPriority.Normal, RefreshDelegateObj); 
        }
        public void TransformTrack()
        {
            m_trackControlNew.Dispatcher.BeginInvoke(DispatcherPriority.Normal, TransformDelegateObj);
        }

        long mmCoordinate;
        long OldmmCoordinate;
        double TrackLength = 0;

        public void setCoordinatNEW(Equipment.RefreshEquip data)
        {
            if (InvokeRequired){
                BeginInvoke(new SetCoordDelegateNEW(setCoordinatNEW), new object[] { data });
            }
            else
            {
                mmCoordinate = data.mmCoordinate;
                OldmmCoordinate = data.mmCoordinate;
                m_trackControlNew.setNextRefreshFrameData(data);
                RefreshTrack();
            }
        }

        public void callTransformTrack(Equipment.TrasformTrackEvent data)
        {
            if (InvokeRequired) 
                BeginInvoke(new delegate_callTransformTrack(callTransformTrack), new object[] { data }); 
            else
            {
                mmCoordinate = data.Coord;
                m_trackControlNew.setNextTransformFrameData(data);
                TransformTrack();
            }
        }

        public event EventHandler<TrackScaleEventArgs> trackScaleEventHandler;

        public virtual void FireTrackScaleEvent(TrackScaleEventArgs e)
        {
            if (trackScaleEventHandler != null) { trackScaleEventHandler(this, e); }
        }

        private void ButtonZoomOut_Click(object sender, EventArgs e)
        {

            //DBController.set_objects_by_coordinate(OldmmCoordinate, (long)TrackLength);

            DispatcherOperation DOperation =  m_trackControlNew.Dispatcher.BeginInvoke(DispatcherPriority.Normal, SetTrackLengthDelegateObj,1.5);
            DOperation.Completed += DOperationIn_Completed;
            
        }
       
        void DOperationIn_Completed(object sender, EventArgs e)
        {
            FireTrackScaleEvent(new TrackScaleEventArgs(1.5));
        }

        private void ButtonZoomIn_Click(object sender, EventArgs e)
        {
            DispatcherOperation DOperationOut = m_trackControlNew.Dispatcher.BeginInvoke(DispatcherPriority.Normal, SetTrackLengthDelegateObj, 0.5);
            DOperationOut.Completed += DOperationOut_Completed;
        }
        void DOperationOut_Completed(object sender, EventArgs e)
        {
            FireTrackScaleEvent(new TrackScaleEventArgs(0.5));
        }
        //public void MapObjectsLoadedEventFired(object sender, MapObjectsLoadedEvent e)
        //{
        //    if (m_trackControl == null) 
        //        return;
        //    m_trackControl.MapObjects = e.MapObjects;
        //    DrawMap();
        //    RefreshTrackControl();
        //}

        //public delegate void RefreshDelegate();

        //public void RefreshTrackControl()
        //{
        //    m_trackControl.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new TrackControl.RefreshDelegate(m_trackControl.Refresh));//.Refresh();
        //}

        //public delegate void DrawMapDelegate();

        //public void DrawMap()
        //{
        //    m_trackControl.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new DrawMapDelegate(m_trackControl.DrawMap));
        //}

        //private delegate void SetCoordDelegate(int coord);

        //public void SetCoord(int coord)
        //{
        //    if (InvokeRequired)
        //    {
        //        BeginInvoke(new SetCoordDelegate(SetCoord), new object[] { coord });
        //    }
        //    else
        //    {
        //        m_trackControl.CurCoord = coord / m_trackControl.Factor;
        //        RefreshTrackControl();
        //    }
        //}

        //public delegate void RefreshControlDelegate();

        //public void RefreshControl()
        //{
        //    if (InvokeRequired)
        //    {
        //        BeginInvoke(new RefreshControlDelegate(RefreshControl));
        //    }
        //    else
        //    {
        //        m_trackControl.Refresh();
        //    }
        //}

    }
}
