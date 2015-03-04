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

        public DB.DataBaseHelper DB_Helper { set { m_trackControlNew.DB_Helper = value; } }

      // TrackControl m_trackControl = new TrackControl();
        TrackControlNew m_trackControlNew;
        public TrackPanel()
        {
            InitializeComponent();
            
            elementHost1.Dock = DockStyle.Fill;

            m_trackControlNew = new TrackControlNew();
            elementHost1.Child = m_trackControlNew;

            this.panel1.Controls.Add(elementHost1);

            //this.Paint += TrackPanel_Paint; 

        }

        void TrackPanel_Paint(object sender, PaintEventArgs e)
        {
                
        }



        private delegate void SetCoordDelegate(int x, int y);
        private delegate void SetCoordDelegateNEW(bool displayNewObjects, ulong coord);
        //public void DrawEquipment(int x, int y)
        //{
        //    if (InvokeRequired)
        //    {
        //        BeginInvoke(new SetCoordDelegate(DrawEquipment), new object[] { x,y });
        //    }
        //    else
        //    {
        //        // m_trackControl.CurCoord = coord / m_trackControl.Factor;
        //        RefreshTrackControl();
        //    }
        //}
      
        public void RefreshTrackControl()
        {
            
            m_trackControlNew.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new TrackControlNew.RefreshDelegate(m_trackControlNew.Refresh)); //.Refresh();
        }


        public void setCoordinatNEW(bool displayNewObjects, ulong coord)
        {
            if (InvokeRequired)
            {
                BeginInvoke(new SetCoordDelegateNEW(setCoordinatNEW), new object[] { displayNewObjects, coord });
            }
            else
            {
                m_trackControlNew.trackPanelHeight = this.Height;
                m_trackControlNew.trackPanelWidth = this.Width;

                m_trackControlNew.m_curCoord = coord;
                m_trackControlNew.displayNewObject = displayNewObjects;
                // m_trackControl.CurCoord = coord / m_trackControl.Factor;
                RefreshTrackControl();
            }
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
