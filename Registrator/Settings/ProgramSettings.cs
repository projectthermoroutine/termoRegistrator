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
        public event Sync_SettingsEvent SyncSettingsChanged;

        private position_detector_settings _position_detector_settings;
        private synchronizer_settings _synchronizer_settings;
        public position_detector_settings pd_settings { get { return _position_detector_settings; } set { _position_detector_settings = value; } }
        public synchronizer_settings sync_settings { get { return _synchronizer_settings; } set { _synchronizer_settings = value; } }

        public ProgramSettings()
        {
            InitializeComponent();

            _position_detector_settings = new position_detector_settings();
            _synchronizer_settings = new synchronizer_settings();

  
        }

        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            if(e.Node.Name == "PositionDetector_Node")
            {
                //устанавливаем редактируемый объект
                propertyGrid1.SelectedObject = _position_detector_settings;
            }
            if (e.Node.Name == "synchronizer_node")
            {
                //устанавливаем редактируемый объект
                propertyGrid1.SelectedObject = _synchronizer_settings;
            }

        }

        private void ProgramSettings_FormClosed(object sender, FormClosedEventArgs e)
        {
            Properties.Settings.Default.pd_ip = _position_detector_settings.pd_ip;
            Properties.Settings.Default.pd_i_ip = _position_detector_settings.pd_i_ip;
            Properties.Settings.Default.pd_port = _position_detector_settings.pd_port;
            Properties.Settings.Default.pd_events_ip = _position_detector_settings.pd_events_ip;
            Properties.Settings.Default.pd_i_events_ip = _position_detector_settings.pd_i_events_ip;
            Properties.Settings.Default.pd_events_port = _position_detector_settings.pd_events_port;

            Properties.Settings.Default.synchronizer_counter_size = _synchronizer_settings.counter_size;

            Properties.Settings.Default.Save();
         
            if(PdSettingsChanged != null)
                PdSettingsChanged(_position_detector_settings);

            if (SyncSettingsChanged != null)
                SyncSettingsChanged(_synchronizer_settings);

        }

    }

     public delegate void PD_SettingsEvent(position_detector_settings pd_settings);
     public delegate void Sync_SettingsEvent(synchronizer_settings sync_settings);

}
