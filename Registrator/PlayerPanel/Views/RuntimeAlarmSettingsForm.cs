using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Registrator.Views
{
    public partial class RuntimeAlarmSettingsForm : Form
    {
        RuntimeAlarmSettings RuntimeAlarmSettingsCtrl;

        public IRB_Frame.RunTimeAlarmController.settings Settings { get; private set; }

        public RuntimeAlarmSettingsForm(IRB_Frame.RunTimeAlarmController.settings settings)
        {
            InitializeComponent();
            RuntimeAlarmSettingsCtrl = new RuntimeAlarmSettings();
            elementHost1.Child = RuntimeAlarmSettingsCtrl;

            Settings = settings;

            RuntimeAlarmSettingsCtrl.chkEnableFrameFilter.IsChecked = Settings.filter_frame;
            RuntimeAlarmSettingsCtrl.chkEnableObjectFilter.IsChecked = Settings.filter_objects;

            if (Settings.filter_frame)
                RuntimeAlarmSettingsCtrl.maxFrameT_NumericCtrl.Value = (decimal)Settings.frame_traits.maxT;


            if (Settings.filter_objects)
                if (Settings.object_traits.is_manual)
                {
                    RuntimeAlarmSettingsCtrl.ManualObjectTRadioBtn.IsChecked = true;
                    RuntimeAlarmSettingsCtrl.maxObjectT_NumericCtrl.Value = (decimal)Settings.object_traits.maxT;
                }
                else
                    RuntimeAlarmSettingsCtrl.MaxObjectTRadioBtn.IsChecked = true;

        }

        private void CancelBtn_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void RetrieveSettings()
        {
            Settings.filter_frame = (bool)RuntimeAlarmSettingsCtrl.chkEnableFrameFilter.IsChecked;
            Settings.filter_objects = (bool)RuntimeAlarmSettingsCtrl.chkEnableObjectFilter.IsChecked;

            if (Settings.filter_frame)
                Settings.frame_traits.maxT = (float)RuntimeAlarmSettingsCtrl.maxFrameT_NumericCtrl.Value;

            if (Settings.filter_objects)
            {
                Settings.object_traits.is_manual = (bool)RuntimeAlarmSettingsCtrl.ManualObjectTRadioBtn.IsChecked;
                if (Settings.object_traits.is_manual)
                {
                    Settings.object_traits.maxT = (float)RuntimeAlarmSettingsCtrl.maxObjectT_NumericCtrl.Value;
                }
            }
        }

        private void OkBtn_Click(object sender, EventArgs e)
        {
            RetrieveSettings();
            Close();

        }

        private void SaveBtn_Click(object sender, EventArgs e)
        {
            RetrieveSettings();
            Settings.Save();
        }
    }
}
