using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Registrator
{
    public partial class FramesPanel : ToolWindow
    {

        public event EventHandler<NeedShotEvent> NeedShotEventHandler;

        private bool m_checkAllState = true;

        public FramesPanel()
        {
            InitializeComponent();
        }

        public delegate void AddShotToListDelegate(ListViewItem item);

        public void AddShotToList(ListViewItem item)
        {
            if (InvokeRequired)
                BeginInvoke(new AddShotToListDelegate(AddShotToList), new object[] { item });
            else
                shotsList.Items.Add(item);
        }

        public void FrameShotedEventFired(object sender, FrameShotedEvent e)
        {
            ListViewItem item = new ListViewItem("", (int)e.Shot.TypeOfShot);
            //item.SubItems.Add("");
            item.SubItems.Add(e.Shot.MsecString);
            item.SubItems.Add(e.Shot.Line.ToString());
            item.SubItems.Add(e.Shot.Path.ToString());
            item.SubItems.Add(e.Shot.PicketNOffset);
            item.SubItems.Add(e.Shot.Peregon);
            item.SubItems.Add(e.Shot.ObjName);

            //shotsList.Items.Add(item);

            AddShotToList(item);

        }

        public void FireNeedShotEvent(NeedShotEvent e)
        {
            EventHandler<NeedShotEvent> handler = NeedShotEventHandler;
            if (handler != null)
                handler(this, e);
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            FireNeedShotEvent(new NeedShotEvent());
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {

            bool checkedPresence = false;

            for (int i = shotsList.Items.Count - 1 ; i > -1  ; i-- )
            {
                if (shotsList.Items[i].Checked)
                {
                    shotsList.Items.RemoveAt(i);
                    checkedPresence = true;
                }

            }

            if (!checkedPresence && shotsList.SelectedIndices.Count > 0)
                shotsList.Items.RemoveAt(shotsList.SelectedIndices[0]);
        
        }

        private void timeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            shotsList.Columns[1].Width = (timeToolStripMenuItem.Checked) ? 89 : 0;
        }

        private void lineToolStripMenuItem_Click(object sender, EventArgs e)
        {
            shotsList.Columns[2].Width = (lineToolStripMenuItem.Checked) ? 60 : 0;
        }

        private void pathToolStripMenuItem_Click(object sender, EventArgs e)
        {
            shotsList.Columns[3].Width = (pathToolStripMenuItem.Checked) ? 60 : 0;
        }
        
        private void piketToolStripMenuItem_Click(object sender, EventArgs e)
        {
            shotsList.Columns[4].Width = (pNoToolStripMenuItem.Checked) ? 62 : 0;
        }

        private void layoutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            shotsList.Columns[5].Width = (layoutToolStripMenuItem.Checked) ? 148 : 0;
        }

        private void objToolStripMenuItem_Click(object sender, EventArgs e)
        {
            shotsList.Columns[6].Width = (objToolStripMenuItem.Checked) ? 112 : 0;
        }

        private void checkAllButton_Click(object sender, EventArgs e)
        {

            if (!m_checkAllState)
                while (shotsList.CheckedItems.Count > 0)
                    shotsList.CheckedItems[0].Checked = false;
            else
                for (int i = 0; i < shotsList.Items.Count; i++)
                    shotsList.Items[i].Checked = true;

            m_checkAllState = !m_checkAllState;
            //checkAllButton.checkAllButton.Text = (m_checkAllState) ? "Выбрать все" : "";
            checkAllButton.Image = ((System.Drawing.Image)((m_checkAllState) ? global::Registrator.Properties.Resources.iconCheckAll : global::Registrator.Properties.Resources.iconUnCheckAll));
        }
           
    }

    public class NeedShotEvent : EventArgs
    {

        ShotDesc.ShotType m_shotType = ShotDesc.ShotType.SHOT_TYPE_USER;

        public NeedShotEvent(ShotDesc.ShotType shotType = ShotDesc.ShotType.SHOT_TYPE_USER)
        {
            m_shotType = shotType;
        }

        public ShotDesc.ShotType Type
        {
            get
            {
                return m_shotType;
            }

            set
            {
                m_shotType = value;
            }
        }

    }
}
