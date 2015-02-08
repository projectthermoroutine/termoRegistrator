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

        //TrackControl m_trackControl = new TrackControl();
        TrackControlNew m_trackControlNew = new TrackControlNew();

        public TrackPanel()
        {
            InitializeComponent();
            
            elementHost1.Dock = DockStyle.Fill;

            //if (m_trackControl == null)
            //    m_trackControl = new TrackControl();
            //elementHost1.Child = m_trackControl;
            elementHost1.Child = m_trackControlNew;

            this.panel1.Controls.Add(elementHost1);

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
