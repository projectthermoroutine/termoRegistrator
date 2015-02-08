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
    using map_objects_list = List<measure_object>;

    public partial class EquipmentListPanel : ToolWindow
    {
        public class map_object_info
        {
            public static string[] get_info(measure_object map_object)
            {
                return map_object_info.to_strings(map_object);
            }
            private static string[] to_strings(measure_object map_object)
            {
                var res = new String[13];
                res[0] = map_object.picket + "+" + 0.01 * map_object.offset;
                res[1] = (map_object.sec >= 0) ? "" : "";
                res[2] = map_object.name;// mo.Name;
                res[3] = map_object.type.ToString();
                res[4] = map_object.line + "-" + map_object.path;
                res[5] = (map_object.coord * 0.01).ToString();
                res[6] = map_object.code.ToString();
                res[7] = map_object.length + " см";
                res[8] = map_object.span.ToString();
                res[9] = map_object.obj_data.ToString();
                res[10] = map_object.pt_data.ToString();
                res[11] = map_object.dir.ToString();
                res[12] = (map_object.forward) ? "1" : "0";
                //res[13] = map_object.frame_number.ToString();
                return res;
            }

        };


        private int m_prevSelected = 0;

        public event EventHandler<ItemSelectedEvent> ItemSelectedHandler;
        public event EventHandler<ObjectFilterSetEvent> ObjectFilterSetEventHandler;

        UInt32 m_objFilter = 0xFFFFFFFF;

        public EquipmentListPanel()
        {
            InitializeComponent();
            EnableColumns();
        }

        public delegate void SetListDelegate(map_objects_list list);

        public void SetList(map_objects_list list)
        {

            if(InvokeRequired)
            {
                BeginInvoke(new SetListDelegate(SetList), new object[]{list});
                return;
            }

            m_prevSelected = 0;

            listView2.Items.Clear();

            foreach(var map_object in list)
            {
                var map_object_info = EquipmentListPanel.map_object_info.get_info(map_object);
                listView2.Items.Add(new ListViewItem(map_object_info));
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
