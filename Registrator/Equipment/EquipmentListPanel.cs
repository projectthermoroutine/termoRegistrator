using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;
using System.Threading;


namespace Registrator
{
    public partial class EquipmentListPanel : DockContent
    {
        private int m_prevSelected = 0;

        public event EventHandler<ItemSelectedEvent> ItemSelectedHandler;
        public event EventHandler<ObjectFilterSetEvent> ObjectFilterSetEventHandler;

        UInt32 m_objFilter = 0xFFFFFFFF;

        public EquipmentListPanel()
        {
            InitializeComponent();
            EnableColumns();
        }

        public delegate void SetListDelegate<T>(IEnumerable<T> list);

        public void SetList<T>(IEnumerable<T> list)
        {

            if(InvokeRequired)
            {
                BeginInvoke(new SetListDelegate<T>(SetList), new object[]{list});
                return;
            }

            m_prevSelected = 0;

            listView2.Items.Clear();

            foreach(var map_object in list)
            {
                listView2.Items.Add(new ListViewItem());
            }

            listView2.Refresh();

        }

        public int PrevSelected { get { return m_prevSelected; } set { m_prevSelected = value; } }
        public UInt32 ObjectFilter { get { return m_objFilter; } set { m_objFilter = value; } }

        private delegate void ActivateDelegate(int index, int distance);

        public void Activate(int index, int distance)
        {
            if (listView2.InvokeRequired)
            {
                listView2.Invoke(new ActivateDelegate(Activate), new object[]{index, distance});
            }
            else
            {
                if (listView2.Items.Count < 1 || m_prevSelected >= listView2.Items.Count - 1)
                    return; 

                listView2.Items[m_prevSelected].SubItems[2].Text = "";
                
                m_prevSelected = index;
                if (index >= listView2.Items.Count)
                    index = listView2.Items.Count - 1;
                listView2.Items[index].SubItems[2].Text = distance.ToString();
                listView2.Items[index].Selected = true;
                listView2.Items[index].Focused = true;
            }

        }

        private void FireItemSelectedEvent(ItemSelectedEvent e)
        {
            EventHandler<ItemSelectedEvent> handler = ItemSelectedHandler;
            if (handler != null)
            {
                handler(this, e);
            }
        }

        private void listView2_DoubleClick(object sender, EventArgs e)
        {
            if(listView2.SelectedIndices.Count > 0) 
                FireItemSelectedEvent(new ItemSelectedEvent(listView2.SelectedIndices[0]));
        }

        private void piketToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //piketToolStripMenuItem.Checked = !piketToolStripMenuItem.Checked;
            listView2.Columns[1].Width = (piketToolStripMenuItem.Checked) ? 110 : 0;
        }

        private void distanceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //distanceToolStripMenuItem.Checked = !distanceToolStripMenuItem.Checked;
            listView2.Columns[2].Width = (distanceToolStripMenuItem.Checked) ? 107 : 0;
        }

        private void timeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //timeToolStripMenuItem.Checked = !timeToolStripMenuItem.Checked;
            listView2.Columns[3].Width = (timeToolStripMenuItem.Checked) ? 60 : 0;
        }

        private void typeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //typeToolStripMenuItem.Checked = !typeToolStripMenuItem.Checked;
            listView2.Columns[4].Width = (typeToolStripMenuItem.Checked) ? 92 : 0;
        }

        private void lineNpathToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //lineNpathToolStripMenuItem.Checked = !lineNpathToolStripMenuItem.Checked;
            listView2.Columns[5].Width = (lineNpathToolStripMenuItem.Checked) ? 87 : 0;
        }

        private void offsetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //offsetToolStripMenuItem.Checked = !offsetToolStripMenuItem.Checked;
            listView2.Columns[6].Width = (offsetToolStripMenuItem.Checked) ? 97 : 0;
        }

        private void codeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //codeToolStripMenuItem.Checked = !codeToolStripMenuItem.Checked;
            listView2.Columns[7].Width = (codeToolStripMenuItem.Checked) ? 60 : 0;
        }

        private void lengthToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //lengthToolStripMenuItem.Checked = !lengthToolStripMenuItem.Checked;
            listView2.Columns[8].Width = (lengthToolStripMenuItem.Checked) ? 60 : 0;
        }

        private void layoutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //layoutToolStripMenuItem.Checked = !layoutToolStripMenuItem.Checked;
            listView2.Columns[9].Width = (layoutToolStripMenuItem.Checked) ? 85 : 0;
        }

        private void dataToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //dataToolStripMenuItem.Checked = !dataToolStripMenuItem.Checked;
            listView2.Columns[10].Width = (dataToolStripMenuItem.Checked) ? 70 : 0;
        }

        private void EnableColumns()
        {
            listView2.Columns[1].Width = (piketToolStripMenuItem.Checked) ? 110 : 0;
            listView2.Columns[2].Width = (distanceToolStripMenuItem.Checked) ? 107 : 0;
            listView2.Columns[3].Width = (timeToolStripMenuItem.Checked) ? 60 : 0;
            listView2.Columns[4].Width = (typeToolStripMenuItem.Checked) ? 92 : 0;
            listView2.Columns[5].Width = (lineNpathToolStripMenuItem.Checked) ? 87 : 0;
            listView2.Columns[6].Width = (offsetToolStripMenuItem.Checked) ? 97 : 0;
            listView2.Columns[7].Width = (codeToolStripMenuItem.Checked) ? 60 : 0;
            listView2.Columns[8].Width = (lengthToolStripMenuItem.Checked) ? 60 : 0;
            listView2.Columns[9].Width = (layoutToolStripMenuItem.Checked) ? 85 : 0;
            listView2.Columns[10].Width = (dataToolStripMenuItem.Checked) ? 70 : 0;
        }

        public void ObjectFilterSetEventFired(object sender, ObjectFilterSetEvent e)
        {
            m_objFilter = e.ObjectFilter;
            m_prevSelected = 0;
            FireObjectFilterSet(e);
        }

        public void FireObjectFilterSet(ObjectFilterSetEvent e)
        {
            EventHandler<ObjectFilterSetEvent> handler = ObjectFilterSetEventHandler;
            if (handler != null)
                handler(this, e);
        }

        private void toolStripSplitButton1_ButtonClick(object sender, EventArgs e)
        {
            
            EquipmentFilterTool eft = new EquipmentFilterTool();
            eft.ObjectFilterSetEventHandler += ObjectFilterSetEventFired;
            eft.ShowDialog(this);
            
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            EquipmentFilterTool eft = new EquipmentFilterTool(m_objFilter);
            eft.ObjectFilterSetEventHandler += ObjectFilterSetEventFired;
            eft.ShowDialog(this);
        }
        
    }

    public class ItemSelectedEvent : EventArgs
    {

        private int m_index = -1;

        public ItemSelectedEvent(int index) 
            : base()
        {
            m_index = index;
        }

        public int Index
        {
            get
            {
                return m_index;
            }

            set
            {
                m_index = value;
            }
        }

    }

}
