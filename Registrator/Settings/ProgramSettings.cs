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

        public event SettingsEvent<position_detector_settings> PdSettingsChanged;
        public event SettingsEvent<synchronizer_settings> SyncSettingsChanged;
        public event SettingsEvent<common_settings> CommonSettingsChanged;
        public event SettingsEvent<CorrectionTemperatureSettings> CorrectionTemperatureSettingsChanged;


        private common_settings _common_settings;
        private position_detector_settings _position_detector_settings;
        private synchronizer_settings _synchronizer_settings;
        private folders_settings _folders_settings;
        public position_detector_settings pd_settings { get { return _position_detector_settings; } set { _position_detector_settings = value; } }
        public synchronizer_settings sync_settings { get { return _synchronizer_settings; } set { _synchronizer_settings = value; } }
        public common_settings common_settings { get { return _common_settings; } set { _common_settings = value; } }
        public folders_settings folder_settings { get { return _folders_settings; } set { _folders_settings = value; } }

        public CorrectionTemperatureSettings CorrectionTemperatureSettings { get; set; } = new CorrectionTemperatureSettings();

        List<object> _objects_for_checking;

        public ProgramSettings()
        {
            InitializeComponent();

            _position_detector_settings = new position_detector_settings();
            _synchronizer_settings = new synchronizer_settings();
            _common_settings = new common_settings();
            folder_settings = new folders_settings();

            _objects_for_checking = new List<object>();

        }

        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            object _object = null;
            if(e.Node.Name == "PositionDetector_Node")
            {
                //устанавливаем редактируемый объект
                _object = _position_detector_settings.Clone();
            }
            if (e.Node.Name == "synchronizer_node")
            {
                //устанавливаем редактируемый объект
                _object = _synchronizer_settings.Clone();
            }

            if (e.Node.Name == "common_node")
            {
                //устанавливаем редактируемый объект
                _object = _common_settings.Clone();
            }
            if (e.Node.Name == "dirNode")
            {
                //устанавливаем редактируемый объект
                _object = _folders_settings.Clone();
            }

            if (e.Node.Name == "CorrectionTemperatureNode")
            {
                //устанавливаем редактируемый объект
                _object = CorrectionTemperatureSettings.Clone();
            }
            


            if (_object != null)
            {
                var type = _object.GetType();
                foreach (var settings in _objects_for_checking)
                {
                    if (type == settings.GetType())
                    {
                        propertyGrid1.SelectedObject = settings;
                        return;
                    }
                }
                _objects_for_checking.Add(_object);
                propertyGrid1.SelectedObject = _object;
            }

        }

        private void ProgramSettings_FormClosed(object sender, FormClosedEventArgs e)
        {

            foreach(var settings in _objects_for_checking)
            {
                var type = settings.GetType();
                if (type == typeof(position_detector_settings))
                {
                    if ((position_detector_settings)settings != _position_detector_settings)
                    {
                        position_detector_settings _new_settings = settings as position_detector_settings;
                        Properties.Settings.Default.pd_ip = _new_settings.pd_ip;
                        Properties.Settings.Default.pd_i_ip = _new_settings.pd_i_ip;
                        Properties.Settings.Default.pd_port = _new_settings.pd_port;
                        Properties.Settings.Default.pd_events_ip = _new_settings.pd_events_ip;
                        Properties.Settings.Default.pd_i_events_ip = _new_settings.pd_i_events_ip;
                        Properties.Settings.Default.pd_events_port = _new_settings.pd_events_port;

                        Properties.Settings.Default.Save();

                        PdSettingsChanged?.Invoke(_new_settings);
                    }
                }

                if (type == typeof(synchronizer_settings))
                {
                    if ((synchronizer_settings)settings != _synchronizer_settings)
                    {
                        synchronizer_settings _new_settings = settings as synchronizer_settings;
                        Properties.Settings.Default.synchronizer_counter_size = _new_settings.counter_size;

                        Properties.Settings.Default.Save();

                        SyncSettingsChanged?.Invoke(_new_settings);
                    }
                }

                if (type == typeof(common_settings))
                {
                    if ((common_settings)settings != _common_settings)
                    {
                        common_settings _new_settings = settings as common_settings;
                        Properties.Settings.Default.maxFramesInFile = _new_settings.max_frames_in_file;
                        Properties.Settings.Default.camera_offset = _new_settings.camera_offset;

                        Properties.Settings.Default.Save();

                        CommonSettingsChanged?.Invoke(_new_settings);
                    }

                }

                if (type == typeof(folders_settings))
                {
                    if ((folders_settings)settings != _folders_settings)
                    {
                        folders_settings _new_settings = settings as folders_settings;
                        Properties.Settings.Default.lastProjDir = _new_settings.projects_root_dir;
                        Properties.Settings.Default.Save();
                    }
                }

                if (type == typeof(CorrectionTemperatureSettings))
                {
                    if ((CorrectionTemperatureSettings)settings != CorrectionTemperatureSettings)
                    {
                        CorrectionTemperatureSettings _new_settings = settings as CorrectionTemperatureSettings;
                        Properties.Settings.Default.CorrectionTemperatureFactor = _new_settings.CorrectionFactor;
                        Properties.Settings.Default.CorrectionTemperatureOffset = _new_settings.CorrectionOffset;
                        Properties.Settings.Default.UseCorrectionTemperature = _new_settings.CorrectionEnable;

                        Properties.Settings.Default.Save();

                        CorrectionTemperatureSettingsChanged?.Invoke(_new_settings);
                    }

                }

            }

        }

    }

     public delegate void SettingsEvent<T>(T settings);


}
