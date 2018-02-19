using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Windows;
using WeifenLuo.WinFormsUI.Docking;
using DrawToolsLib;
using System.Media;
using System.Reflection;


namespace Registrator
{
    public partial class AreasPanel : DockContent
    {

        public event EventHandler<AreaToolEvent> newAreaEventHandler;
        public event EventHandler<AreasChangedEvent> areasChangedEventHandler;
        public event EventHandler<AreasDeletedInEditorEvent> AreasDeletedInEditorEventHandler;
        public event EventHandler<AreasTemplateSelectedEvent> AreasTemplateSelectedEventHandler;

        private AreasTemplate m_template = null;

        private SoundPlayer _alarm_player;

        public AreasPanel()
        {
            InitializeComponent();
            try_create_alarm_palyer();
        }

        private void try_create_alarm_palyer()
        {
            _alarm_player = null;
            try
            {
                _alarm_player = new SoundPlayer(System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location) + "\\beep.wav");
            }
            catch(System.IO.FileNotFoundException)
            {
                _alarm_player = null;
            }

        }
        public AreasTemplate Template
        {
            get
            {
                return m_template;
            }

            set
            {
                m_template = value;
            }

        }

        public delegate void InitListDelegate();

        public void InitList()
        {

            if(InvokeRequired)
            {
                BeginInvoke(new InitListDelegate(InitList), new object[] { });
                return;
            }

            if (m_template == null)
                return;

            listView1.Items.Clear();

            for (int i = 0; i < m_template.Areas.Count; i++)
            {
                Area area = (Area)m_template.Areas[i];
                ListViewItem item = new ListViewItem(area.Name);
                item.SubItems.Add(area.MinTemp.ToString());
                item.SubItems.Add(area.MaxTemp.ToString());
                item.SubItems.Add(area.AvrgTemp.ToString());
                listView1.Items.Add(item);
                
                item.SubItems[0].BackColor = Color.White;
                item.SubItems[1].BackColor = Color.White;
                item.SubItems[2].BackColor = Color.White;

                bool is_alarm = false;
                if (area.MinTemp > area.MinTempBorder)
                {
                    is_alarm = true;
                    item.SubItems[0].BackColor = Color.Red;
                }

                if (area.MaxTemp > area.MaxTempBorder)
                {
                    is_alarm = true;
                    item.SubItems[1].BackColor = Color.Red;
                }

                if (area.AvrgTemp > area.AvrgTempBorder)
                {
                    is_alarm = true;
                    item.SubItems[2].BackColor = Color.Red;
                }

                if (is_alarm && _alarm_player != null)
                {
                    try
                    {
                        _alarm_player.Play();
                    }
                    catch (System.IO.FileNotFoundException)
                    {
                        _alarm_player = null;
                    }
                }

            }
        }

        public delegate void UpdateAreasRowsDelegate(List<int> results_ids);

        public void UpdateAreasRows(List<int> results_ids)
        {

            if(InvokeRequired)
            {
                BeginInvoke(new UpdateAreasRowsDelegate(UpdateAreasRows), new object[] { results_ids });
                return;
            }

            int counter = 0;

            foreach (int i in results_ids)
            {
                if (i > m_template.Areas.Count)
                    continue;

                Area area = (Area)m_template.Areas[i];
                ListViewItem item = listView1.Items[i];

                if (item.SubItems.Count < 2)
                {
                    item.SubItems.Add(area.MinTemp.ToString());
                    item.SubItems.Add(area.MaxTemp.ToString());
                    item.SubItems.Add(area.AvrgTemp.ToString());
                }
                else
                {
                    item.SubItems[1].Text = area.MinTemp.ToString();
                    item.SubItems[2].Text = area.MaxTemp.ToString();
                    item.SubItems[3].Text = area.AvrgTemp.ToString();
                }

                item.SubItems[0].BackColor = Color.White;
                //item.SubItems[2].BackColor = Color.White;
                //item.SubItems[3].BackColor = Color.White;

                bool is_alarm = false;
                if (area.MinTemp > area.MinTempBorder)
                {
                    is_alarm = true;
                    item.SubItems[0].BackColor = Color.Red;
                }

                if (area.MaxTemp > area.MaxTempBorder)
                {
                    is_alarm = true;
                    item.SubItems[0].BackColor = Color.Red;
                }

                if (area.AvrgTemp > area.AvrgTempBorder)
                {
                    is_alarm = true;
                    item.SubItems[0].BackColor = Color.Red;
                }

                if (is_alarm && _alarm_player != null)
                {
                    try
                    {
                        _alarm_player.Play();
                    }
                    catch(System.IO.FileNotFoundException)
                    {
                        _alarm_player = null;
                    }
                }

                if (is_alarm)
                    counter++;

            }
        }

        public void AddArea(Area area)
        {
            if(m_template == null)
            {
                m_template = new AreasTemplate();
            }

            m_template.Areas.Add(area);
            area.AreaAlarmedHandler += AreaAlarmedEventFired;
            ListViewItem item = new ListViewItem(area.Name);
            //item.SubItems.Add(area.MinTemp.ToString());
            //item.SubItems.Add(area.MaxTemp.ToString());
            //item.SubItems.Add(area.AvrgTemp.ToString());

            listView1.Items.Add(item); 

        }

        public void DeleteArea(int areaIndx)
        {
            ((Area)m_template.Areas[areaIndx]).AreaAlarmedHandler -= AreaAlarmedEventFired;
            m_template.Areas.RemoveAt(areaIndx);
            listView1.Items.RemoveAt(areaIndx+1);
        }

        public void AreaAlarmedEventFired(object sender, AreaAlarmedEvent e)
        {

        }

        private void settingsButton_Click(object sender, EventArgs e)
        {
            if (listView1.SelectedIndices.Count < 1 || (listView1.SelectedIndices[0] > 0 && m_template == null) )
                return;
            int indx = listView1.SelectedIndices[0];

            AreaSettings stgs = new AreaSettings((Area)m_template.Areas[indx]);
            stgs.saveButton.Click += EditorSaved;
            stgs.ShowDialog(this);

        }

        public void EditorSaved(object sender, EventArgs e)
        {
            InitList();
        }

        private void loadTemplateButton_Click(object sender, EventArgs e)
        {
            AreasTemplateSelection ats = new AreasTemplateSelection();
            ats.areaTemplateSelectedHandler += AreasTemplateSelectedFired;
            ats.ShowDialog(this);
        }

        private void saveTemplateButton_Click(object sender, EventArgs e)
        {
            if (m_template == null || m_template.Areas.Count < 1)
                return;
            AreasTemplateCreation atc = new AreasTemplateCreation();
            atc.Areas = m_template.Areas;
            atc.ShowDialog(this);
        }

        public virtual void AreaToolChange(Area.AreaType area_type)
        {
            EventHandler<AreaToolEvent> handler = newAreaEventHandler;
            if (handler != null)
                handler(this, new AreaToolEvent(area_type));
        }

        public virtual void FireAreasChangedEvent(AreasChangedEvent e)
        {

            EventHandler<AreasChangedEvent> handler = areasChangedEventHandler;

            if (handler != null)
            {

                handler(this, e);

            }

        }

        private void addRectButton_Click(object sender, EventArgs e)
        {
            AreaToolChange(Area.AreaType.AREA_RECT);
        }
        private void addEllipsButton_Click(object sender, EventArgs e)
        {
            AreaToolChange(Area.AreaType.AREA_ELLIPS);
        }
        private void addFreeButton_Click(object sender, EventArgs e)
        {
            AreaToolChange(Area.AreaType.AREA_FREE);
        }

        public void AreasTemplateSelectedFired(object sender, AreasTemplateSelectedEvent e)
        {
            m_template = e.Template;
            InitList();
            FireAreasTemplateSelectedEvent(e);
        }

        public void FireAreasTemplateSelectedEvent(AreasTemplateSelectedEvent e)
        {
            EventHandler<AreasTemplateSelectedEvent> handler = AreasTemplateSelectedEventHandler;
            if (handler != null)
                handler(this, e);
        }

        public virtual void AreaAddedEventFired(object sender, AreaAddedEvent e)
        {
            Area area = new Area();
            switch(e.Type)
            {
                case (ToolType.Rectangle):
                    {
                        area.Type = Area.AreaType.AREA_RECT;
                        break;
                    }
                case (ToolType.Ellipse):
                    {
                        area.Type = Area.AreaType.AREA_ELLIPS;
                        break;
                    }
                case (ToolType.PolyLine):
                    {
                        area.Type = Area.AreaType.AREA_FREE;
                        break;
                    }
            }

            area.ProgID = e.ID;

            area.X = e.X;
            area.Y = e.Y;

            area.Width = e.Width;
            area.Height = e.Height;

            area.Name = "Область " + e.ID.ToString();

            AddArea(area);
        }

        public Area SearchInTemplate(int progId)
        {
            if(m_template == null)
                return null;

            for (int i = 0; i < m_template.Areas.Count; i++)
            {
                if (((Area)m_template.Areas[i]).ProgID == progId)
                    return (Area)m_template.Areas[i];
            }

            return null;
        }

        public virtual void AreasDeletedEventFired(object sender, AreasDeletedEvent e)
        {

            if(m_template == null)
                return;

            List<Area> areas = new List<Area>();

            int count = m_template.Areas.Count;

            for (int i = 0; i < count; i++ ) 
            {
                Area area = m_template.Areas[i];
                areas.Add(area);

                for (int k = 0; k < e.Areas.Length; k++)
                {
                    if (area.ProgID == e.Areas[k])
                    {
                        areas.RemoveAt(areas.Count-1);
                        break;
                    }
                }
            }

            m_template.Areas = areas;

            InitList();

        }

        public virtual void AreaChangedEventFired(object sender, AreaChangedEvent e)
        {
            Area area = SearchInTemplate(e.Id);

            if (area == null)
                return;

            area.X = e.X;
            area.Y = e.Y;

            area.Width = e.Width;
            area.Height = e.Height;

            InitList();//UpdateAreasRows();
        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {
            settingsButton.Enabled = (listView1.SelectedIndices.Count > 0);
            deleteButton.Enabled = (listView1.SelectedIndices.Count > 0);
        }

        private void FireAreasDeletedInEditorEvent(AreasDeletedInEditorEvent e)
        {
            EventHandler<AreasDeletedInEditorEvent> handler = AreasDeletedInEditorEventHandler;

            if (handler != null)
                handler(this, e);

        }

        private void deleteButton_Click(object sender, EventArgs e)
        {

            int[] areaIDs = new int[listView1.SelectedIndices.Count];

            List<Area> areas = new List<Area>();

            int count = m_template.Areas.Count;

            int counter = 0;

            for (int i = 0; i < m_template.Areas.Count; i++)
            {
                Area area = m_template.Areas[i];
                areas.Add(area);

                for (int k = 0; k < listView1.SelectedIndices.Count; k++)
                {
                    if (i == listView1.SelectedIndices[k])
                    {
                        areaIDs[counter] = area.ProgID;
                        areas.RemoveAt(areas.Count - 1);
                        listView1.Items.RemoveAt(k);
                        counter++;
                        m_template.Areas.RemoveAt(i);
                        i--;
                        break;
                    }
                }
            }

            

            m_template.Areas = areas;

            FireAreasDeletedInEditorEvent(new AreasDeletedInEditorEvent(areaIDs));

 //           UpdateAreasRows();
        }

        private void listView1_DoubleClick(object sender, EventArgs e)
        {

            if (m_template == null || m_template.Areas.Count < 1 || listView1.SelectedIndices.Count < 1)
                return;

            if (listView1.SelectedIndices.Count < 1 || (listView1.SelectedIndices[0] > 0 && m_template == null))
                return;
            int indx = listView1.SelectedIndices[0];

            AreaSettings stgs = new AreaSettings((Area)m_template.Areas[indx]);
            stgs.saveButton.Click += EditorSaved;
            stgs.ShowDialog(this);
        }

        public void AnalyzeArea(int index, float min, float max, float avr)
        {
            if (m_template == null || m_template.Areas == null || m_template.Areas.Count <= index)
                return;

            Area area = m_template.Areas[index] as Area;

            if (area == null)
                return;

            area.MinTemp = min;
            area.MinTemp = max;
            area.AvrgTemp = avr;

            ActualizeArea(area);

        }

        private void ActualizeArea(Area area)
        {
            throw new Exception("ActualizeArea need to be realized");
        }


    }

    

    public class AreaToolEvent : EventArgs
    {
        public Area.AreaType AreaType {get; private set;}
        public AreaToolEvent(Area.AreaType _area_type)
        {
            AreaType = _area_type;
        }
    }

    public class AreasChangedEvent : EventArgs
    {

        ArrayList m_areas;

        public AreasChangedEvent()
            : base()
        {

        }

        public AreasChangedEvent(ArrayList areas)
            : this()
        {
            m_areas = areas;
        }

        public ArrayList Areas
        {
            get
            {
                return m_areas;
            }

            set
            {
                m_areas = value;
            }
        }

    }

}
