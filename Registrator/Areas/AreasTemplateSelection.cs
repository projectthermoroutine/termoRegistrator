using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Collections;

namespace Registrator
{
    public partial class AreasTemplateSelection : Form
    {

        ArrayList m_templates = new ArrayList();

        public EventHandler<AreasTemplateSelectedEvent> areaTemplateSelectedHandler;

        public AreasTemplateSelection()
        {
            InitializeComponent();
            InitForm();
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            Close();
            Dispose();
        }

        public virtual void FireAreasTemplateSelectedEvent(AreasTemplateSelectedEvent e)
        {

            EventHandler<AreasTemplateSelectedEvent> handler = areaTemplateSelectedHandler;

            if(handler != null)
            {
                handler(this, e);
            }

        }

        private void loadButton_Click(object sender, EventArgs e)
        {

            if (m_templates.Count > 0 && listBox1.SelectedIndex > -1)
            {
                AreasTemplate template = m_templates[listBox1.SelectedIndex] as AreasTemplate;
                if(template != null)
                    FireAreasTemplateSelectedEvent(new AreasTemplateSelectedEvent(template));
            }
            Close();
            Dispose();
        }

        private void addButton_Click(object sender, EventArgs e)
        {
            //FireAreasTemplateSelectedEvent(new AreasTemplateSelectedEvent());
            Close();
            Dispose();
        }

        public void InitForm()
        {

            Registrator.DB.teplovizorDataSet.TemplateAreasDataTable tadt = templateAreasTableAdapter1.GetData();

            AreasTemplate curTemplate = null;
            for (int i = 0; i < tadt.Rows.Count; i++)
            {
                if (curTemplate == null || curTemplate.DbId != Convert.ToInt32(tadt.Rows[i].ItemArray[0]))
                {
                    curTemplate = new AreasTemplate();
                    curTemplate.DbId = Convert.ToInt32(tadt.Rows[i].ItemArray[0]);
                    curTemplate.Name = Convert.ToString(tadt.Rows[i].ItemArray[1]);
                    m_templates.Add(curTemplate);
                    listBox1.Items.Add(curTemplate.Name);
                }

                Area area = new Area();

                area.ProgID = Convert.ToInt32(tadt.Rows[i].ItemArray[15]);
                area.Name = Convert.ToString(tadt.Rows[i].ItemArray[16]);

                area.MinTempBorder = Convert.ToSingle(tadt.Rows[i].ItemArray[2]);
                area.MaxTempBorder = Convert.ToSingle(tadt.Rows[i].ItemArray[4]);
                area.AvrgTempBorder = Convert.ToSingle(tadt.Rows[i].ItemArray[6]);

                area.X = Convert.ToInt32(tadt.Rows[i].ItemArray[11]);
                area.Y = Convert.ToInt32(tadt.Rows[i].ItemArray[12]);
                area.Width = Convert.ToInt32(tadt.Rows[i].ItemArray[9]);
                area.Height = Convert.ToInt32(tadt.Rows[i].ItemArray[10]);
                area.Type = (Convert.ToInt32(tadt.Rows[i].ItemArray[17]) == 0) ? Area.AreaType.AREA_RECT : ((Convert.ToInt32(tadt.Rows[i].ItemArray[17]) == 1) ? Area.AreaType.AREA_ELLIPS : Area.AreaType.AREA_FREE);

                curTemplate.Areas.Add(area);

                if (area.Type == Area.AreaType.AREA_FREE)
                {
                    throw new Exception("Need to obtaint free area from DB");
                }

            }

        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            loadButton.Enabled = listBox1.SelectedIndices.Count > 0;
        }

    }

    public class AreasTemplateSelectedEvent : EventArgs
    {

        public enum SelectionType
        {
            SELECTION_REPLACE,
            SELECTION_ADD
        }

        private SelectionType m_selectionType;
        private AreasTemplate m_template;

        public AreasTemplateSelectedEvent()
            : base()
        {

        }

        public AreasTemplateSelectedEvent(AreasTemplate template, SelectionType type = SelectionType.SELECTION_REPLACE)
            : this()
        {
            m_template = template;
            m_selectionType = type;
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

        public SelectionType Type
        {
            get
            {
                return m_selectionType;
            }

            set
            {
                m_selectionType = value;
            }
        }


    }

}
