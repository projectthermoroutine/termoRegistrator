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
    public partial class AreasTemplateCreation : Form
    {

        List<Area> m_areas = new List<Area>();
        List<AreasTemplate> m_templates = new List<AreasTemplate>();

        int[] m_selectedIndexes = null;

        public AreasTemplateCreation()
        {
            InitializeComponent();
            InitForm();
            templateName.SelectedIndex = 0;
        }

        public void InitForm()
        {

           // Registrator.DB.teplovizorDataSet.templatesDataTable tdt = templatesTableAdapter1.GetData();

            //for (int i = 0 ; i < tdt.Rows.Count ; i++ )
            //{
                
            //    AreasTemplate curTemplate = new AreasTemplate();
            //    curTemplate.DbId = Convert.ToInt32(tdt.Rows[i].ItemArray[0]);
            //    curTemplate.Name = Convert.ToString(tdt.Rows[i].ItemArray[1]);
            //    m_templates.Add(curTemplate);

            //    templateName.Items.Add(curTemplate.Name);

            //}

        }

        public void SelectTemplateWithId(int id)
        {
            for(int i = 0 ; i < m_templates.Count ; i++)
            {
                AreasTemplate template = m_templates[i];
                if (template == null || template.DbId != id)
                    continue;
                templateName.SelectedIndex = i + 1;
                return;
            }
        }

        public List<Area> Areas
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

        public int[] SelectedIndexes
        {
            get
            {
                return m_selectedIndexes;
            }

            set
            {
                m_selectedIndexes = value;
            }
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            Close();
            Dispose();
        }

        private void saveButton_Click(object sender, EventArgs e)
        {

            if (templateName.SelectedIndex > 0)
                if (MessageBox.Show("Вы пытаетесь изменить сущесвующий шаблон областей. Содержимое шаблона будет изменено. Продолжить? ", "Изменение шаблона областей", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation) != DialogResult.Yes)
                {
                    Close();
                    Dispose();
                    return;
                }
            List<Area> areas = new List<Area>();

            if (selectedOnly.Checked)
            {
                for (int i = 0; i < m_selectedIndexes.Length; i++)
                {
                    if (m_selectedIndexes[i] >= m_areas.Count)
                        continue;
                    Area area = m_areas[m_selectedIndexes[i]];
                    if (area == null)
                        continue;

                    areas.Add(area);

                }
            }
            else
                areas = m_areas;
          //  SaveTemplate(areas);

            Close();
            Dispose();

        }

       // private AreasTemplate SaveTemplate(List<Area> areas)
       // {

            //AreasTemplate template = new AreasTemplate(templateName.Text as String);
            //if (templateName.SelectedIndex > 0 && (templateName.SelectedIndex - 1) < m_templates.Count)
            //    template = m_templates[templateName.SelectedIndex - 1] as AreasTemplate;

            //if (template == null)
            //    return template;

            //template.Areas.Clear();

            //if (template.DbId == -1)
            //{
            //   // int rowsCount = templatesTableAdapter1.Insert(template.Name);
            //    //Object tid = templatesTableAdapter1.LastIdentity();
            //    //if (tid == null)
            //    //    return null;
            //    //template.DbId = Convert.ToInt32(tid);
            //    //throw new Exception("Need to get template id from DB");
            //}
            //else
            //{
            //    template.Name = templateName.SelectedValue as String;
            //    //templatesTableAdapter1.Update(template.Name, (int)template.DbId);

            //    //template_areasTableAdapter1.DeleteTemplateAreas((int)template.DbId);

            //}
            
            //for (int i = 0; i < areas.Count; i++ )
            //{

            //    Area area = areas[i] as Area;
                
            //    if (area == null)
            //        continue;

                //System.Nullable<int> f = templateAreasTableAdapter1.IsAreaInOtherTemplates((int)area.DbId, (int)template.DbId);//проверить, присутствует ли область в других шаблонах

                //bool inOtherTemplates = (f == null || f < 1) ? false : true;

                //if (area.DbId == -1 || inOtherTemplates /*новая область или область changed и присутствует в других шаблонах*/)
                //{
                //    if (area.Type == Area.AreaType.AREA_RECT)
                //    {
                //        //areasTableAdapter1.Insert(area.Name
                        //                     , area.MinTempBorder
                        //                     , area.MinTempBorder
                        //                     , area.MaxTempBorder
                        //                     , area.MaxTempBorder
                        //                     , area.AvrgTempBorder
                        //                     , area.AvrgTempBorder
                        //                     , ""
                        //                     , (int)area.Width
                        //                     , (int)area.Height
                        //                     , (int)area.X
                        //                     , (int)area.Y
                        //                     , 0
                        //                     );
            //        }
            //        else if(area.Type == Area.AreaType.AREA_ELLIPS)
            //        {
            //            //areasTableAdapter1.Insert(area.Name
            //            //                     , area.MinTempBorder
            //            //                     , area.MinTempBorder
            //            //                     , area.MaxTempBorder
            //            //                     , area.MaxTempBorder
            //            //                     , area.AvrgTempBorder
            //            //                     , area.AvrgTempBorder
            //            //                     , ""
            //            //                     , (int)area.Width
            //            //                     , (int)area.Height
            //            //                     , (int)area.X
            //            //                     , (int)area.Y
            //            //                     , 1
            //            //                     );
            //        }
            //        else if (area.Type == Area.AreaType.AREA_FREE)
            //        {
            //            throw new Exception("Saving template for free area need to be realized");
            //        }

            //        //throw new Exception("Need to get area id from DB");

            //       // object aid = areasTableAdapter1.LastIdentity();
            //        //if (aid == null)
            //        //    return null;
            //        //area.ProgID = Convert.ToInt32(aid);

            //    }

            //    template.Areas.Add(area);

            //  //  template_areasTableAdapter1.Insert((int)template.DbId, area.ProgID);

            //}

            //return template;

      //  }

        private void selectedOnly_CheckedChanged(object sender, EventArgs e)
        {
            saveButton.Enabled = !selectedOnly.Checked || (m_selectedIndexes != null && m_selectedIndexes.Length > 0) ;
        }



    }
}
