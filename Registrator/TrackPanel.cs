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

        public IEnumerable<Registrator.DB.ResultEquipCodeFrame> Objects { set { m_trackControlNew.Objects = value; } }

      // TrackControl m_trackControl = new TrackControl();
        TrackControlNew m_trackControlNew;
        TrackOptions    Toptions_Form;

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
        private delegate void SetCoordDelegateNEW(Equipment.FrameChangedEventNEW data);
        private delegate void SetLineLengthDelegate(ulong LineLength);

        public void RefreshTrackControl()
        {
            
            m_trackControlNew.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new TrackControlNew.RefreshDelegate(m_trackControlNew.Refresh)); //.Refresh();
        }


        public void setCoordinatNEW(Equipment.FrameChangedEventNEW data)
        {
            if (InvokeRequired)
            {
                BeginInvoke(new SetCoordDelegateNEW(setCoordinatNEW), new object[] { data });
            }
            else
            {
                m_trackControlNew.trackPanelHeight = this.Height;
                m_trackControlNew.trackPanelWidth = this.Width;

                m_trackControlNew.m_curCoord = data.Coord;
                m_trackControlNew.direction = data.direction;
                m_trackControlNew.displayNewObject = data.displayNewObject;
                m_trackControlNew.Objects = data.objects;
                RefreshTrackControl();
            }
        }
        public void setLineLength( ulong lineLength)
        {
            if (InvokeRequired)
                BeginInvoke(new SetLineLengthDelegate(setLineLength), new object[] { lineLength });
            else
                m_trackControlNew.lineLength = lineLength;
        }

        private void toolStripSplitButton1_ButtonClick(object sender, EventArgs e)
        {

        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            TrackOptions Toptions_Form = new TrackOptions();
            Toptions_Form.ShowDialog();

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
