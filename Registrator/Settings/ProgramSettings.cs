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
    public partial class ProgramSettings : Form
    {

        public event PD_SettingsEvent PdSettingsChanged;
 
        private position_detector_settings _position_detector_settings;
        public position_detector_settings pd_settings { get { return _position_detector_settings; } set { _position_detector_settings = value; } }

        public ProgramSettings()
        {
            InitializeComponent();

            _position_detector_settings = new position_detector_settings();

  
        }

        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            if(e.Node.Name == "PositionDetector_Node")
            {
                //устанавливаем редактируемый объект
                propertyGrid1.SelectedObject = _position_detector_settings;
            }

        }

        private void ProgramSettings_FormClosed(object sender, FormClosedEventArgs e)
        {
            Properties.Settings.Default.pd_ip = _position_detector_settings.pd_ip;
            Properties.Settings.Default.pd_port = _position_detector_settings.pd_port;
            Properties.Settings.Default.pd_events_ip = _position_detector_settings.pd_events_ip;
            Properties.Settings.Default.pd_events_port = _position_detector_settings.pd_events_port;

            Properties.Settings.Default.Save();
         
            if(PdSettingsChanged != null)
                PdSettingsChanged(_position_detector_settings);

        }

    }

     public delegate void PD_SettingsEvent(position_detector_settings pd_settings);

}
