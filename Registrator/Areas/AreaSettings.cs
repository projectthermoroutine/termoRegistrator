using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Registrator
{
    public partial class AreaSettings : Form
    {

        private Area m_area;

        public AreaSettings()
        {
            InitializeComponent();
        }

        public AreaSettings(Area area)
            : this()
        {
            Area = area;
        }

        public Area Area
        {
            get
            {
                return m_area;
            }

            set
            {
                m_area = value;

                areaName.Text = m_area.Name;
                
                //displayMin.Checked = m_area.MinTempVisibility;
                //displayMax.Checked = m_area.MaxTempVisibility;
                //displayAvrg.Checked = m_area.AvrgTempVisibility;

                alarmMin.Checked = m_area.MinTempAlarmed;
                alarmMax.Checked = m_area.MaxTempAlarmed;
                alarmAvrg.Checked = m_area.AvrgTempAlarmed;

                borderMin.Value = (decimal)m_area.MinTempBorder;
                borderMax.Value = (decimal)m_area.MaxTempBorder;
                borderAvrg.Value = (decimal)m_area.AvrgTempBorder;


            }
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            Close();
            Dispose();
        }

        private void saveButton_Click(object sender, EventArgs e)
        {
            
            m_area.Name = areaName.Text;
            
            //m_area.MinTempVisibility = displayMin.Checked;
            //m_area.MaxTempVisibility = displayMax.Checked;
            //m_area.AvrgTempVisibility = displayAvrg.Checked;

            m_area.MinTempAlarmed = alarmMin.Checked;
            m_area.MaxTempAlarmed = alarmMax.Checked;
            m_area.AvrgTempAlarmed = alarmAvrg.Checked;

            m_area.MinTempBorder = (float)borderMin.Value;
            m_area.MaxTempBorder = (float)borderMax.Value;
            m_area.AvrgTempBorder = (float)borderAvrg.Value;

            Close();
            Dispose();
        }

    }
}
