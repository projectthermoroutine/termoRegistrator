using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Text;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;
using System.Collections;
using System.Windows.Forms.Integration;
using System.Windows.Threading;
using System.Windows.Media;

namespace Registrator
{

    public partial class TrackPanel : DockContent
    {
        public class TrackScaleEventArgs : EventArgs
        {
            private double trackLen;
            public TrackScaleEventArgs(double TrackLenght) { trackLen = TrackLenght; }
            public double ZoomCoefficient { get { return trackLen; } }
        }

        TrackControlNew         m_trackControlNew;
       
        public TrackPanel(DB.metro_db_controller db)
        {

            InitializeComponent();

            trackLen = (double)Properties.Settings.Default.TrackHalfVeiwSector;
            
            elementHost1.Dock = DockStyle.Fill;
            m_trackControlNew = new TrackControlNew(db);
            elementHost1.Child = m_trackControlNew;

            this.panel1.Controls.Add(elementHost1);
        }

        private delegate void DrawTrackControlDelegate(Equipment.RefreshEquip data);
        private delegate void delegate_callTransformTrack(Equipment.TrasformTrackEvent data);

        public void DrawTrackControl(Equipment.RefreshEquip data)
        {
            if (InvokeRequired){
                BeginInvoke(new DrawTrackControlDelegate(DrawTrackControl), new object[] { data });
            }
            else
            {
                m_trackControlNew.UpdateTrack(data);
                toolStripPath.Text = data.Coordinate.path_name;
                toolStripCoordinate.Text = data.Coordinate.coordinate.ToString();
            }
        }

        public void callTransformTrack(Equipment.TrasformTrackEvent data)
        {
            if (InvokeRequired) 
                BeginInvoke(new delegate_callTransformTrack(callTransformTrack), new object[] { data }); 
            else
            {
                m_trackControlNew.TransformTrack(data);
                toolStripCoordinate.Text = (data.Coord/1000).ToString() + " m";
            }
        }

        public event EventHandler<TrackScaleEventArgs> trackScaleEventHandler;

        public virtual void FireTrackScaleEvent(TrackScaleEventArgs e)
        {
            trackScaleEventHandler?.Invoke(this, e);
        }

        double trackLen = 0;
        double standartPicketLenght = 100000;
        const int maxScaleInPickets = 20;
        const int minScaleInPickets = 1;
        bool changeZoom = true;
        private void ButtonZoomOut_Click(object sender, EventArgs e)
        {

            double _trackLenInPickets = trackLen * 1.5 / standartPicketLenght;

            if (_trackLenInPickets >= maxScaleInPickets)
                return;

            trackLen = trackLen * 1.5;
            changeZoom = true;

            FireTrackScaleEvent(new TrackScaleEventArgs(trackLen));

            if (InvokeRequired)
                BeginInvoke(new EventHandler(delegate { m_trackControlNew.UpdateTrackLength((long)trackLen); m_trackControlNew.UpdateTrack(); }));

            else
            {
                m_trackControlNew.UpdateTrackLength((long)trackLen);
                m_trackControlNew.UpdateTrack();
            }
        }

        private void ButtonZoomIn_Click(object sender, EventArgs e)
        {
            double _trackLenInPickets = trackLen * 0.5 / standartPicketLenght;
         
            trackLen = trackLen * 0.5;
            if (trackLen < standartPicketLenght)
            {
                if (!changeZoom) 
                    return;
                trackLen = standartPicketLenght;
                changeZoom = true;
            }

            FireTrackScaleEvent(new TrackScaleEventArgs(trackLen));
            if (InvokeRequired)
                BeginInvoke(new EventHandler(delegate { m_trackControlNew.UpdateTrackLength((long)trackLen); m_trackControlNew.UpdateTrack(); }));

            else
            {
                m_trackControlNew.UpdateTrackLength((long)trackLen);
                m_trackControlNew.UpdateTrack();
            }

        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            TrackOptions formTrackOptions = new TrackOptions();
            
            if(formTrackOptions.ShowDialog()==DialogResult.OK)
                m_trackControlNew.setTrackOptions(formTrackOptions.TrackParams);
        }

    }
}
