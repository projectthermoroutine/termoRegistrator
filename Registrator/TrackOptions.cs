using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Registrator
{
    public partial class TrackOptions : Form
    {
        public TrackControlNew.TrackOptionsParams TrackParams;
        public TrackOptions()
        {
            InitializeComponent();
            TrackParams = new TrackControlNew.TrackOptionsParams();
            groupColorSetUserControl1.colorPicker.SelectedColor = TrackParams.color;
            groupColorSetUserControl2.colorPicker.SelectedColor = TrackParams.trailMarkerColor;
            EquipmentVisibilityCheckBox.Checked = TrackParams.showEquipment;

        }
     
        public long TrackLength= 0;
        private void button_Cancel_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void button_OK_Click(object sender, EventArgs e)
        {
            Properties.Settings.Default.TrackPanel_BackGroungColor = (System.Windows.Media.Color)groupColorSetUserControl1.colorPicker.SelectedColor;
            Properties.Settings.Default.TrackPanel_TrailMarkerColor = (System.Windows.Media.Color)groupColorSetUserControl2.colorPicker.SelectedColor;
            Properties.Settings.Default.TrackPanel_VisibleEquipment = EquipmentVisibilityCheckBox.Checked;
            Properties.Settings.Default.Save();

            TrackParams = new TrackControlNew.TrackOptionsParams();

            Close();
        }
    }
}
