using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using ThermoRoutineLib;

namespace Registrator
{
    public partial class FramesPanel : ToolWindow
    {
        private bool m_checkAllState = true;
        private DB.metro_db_controller _db_controller;
        PointsInfoManager _points_info_manager;
        PointsInfoView PointsInfoViewCtrl;
        
        public FramesPanel(PointsInfoManager points_info_manager, DB.metro_db_controller db_controller)
        {
            InitializeComponent();
            if (db_controller != null)
                _db_controller = new DB.metro_db_controller(db_controller);

            PointsInfoViewCtrl = new PointsInfoView(_db_controller);
            this.Controls.Add(this.PointsInfoViewCtrl);
            this.PointsInfoViewCtrl.Dock = System.Windows.Forms.DockStyle.Fill;
            PointsInfoViewCtrl.CheckBoxes = true;
            PointsInfoViewCtrl.DoubleClickItem += shotsList_DoubleClick;
            PointsInfoViewCtrl.ItemPressed += shotsList_DoubleClick;
            PointsInfoViewCtrl.ItemDeleted += shotsList_ItemDeleted;

            _points_info_manager = points_info_manager;

            foreach (var item in _points_info_manager.PointsInfoList())
            {
                PointsInfoViewCtrl.AddPointInfo(item);
            }

            points_info_manager.AddPointInfoEventHandler += AddPointInfoEventHandler;
        }
        void AddPointInfoEventHandler(object sender, PointInfoEvent e)
        {
            PointsInfoViewCtrl.AddPointInfo(e.PointInfo);
        }

        private void deleteCheckedShotsBtn_Click(object sender, EventArgs e)
        {
            for (int i = PointsInfoViewCtrl.PointInfoListView.Items.Count - 1; i > -1; i--)
            {
                if (PointsInfoViewCtrl.PointInfoListView.Items[i].Checked)
                {
                    PointsInfoViewCtrl.PointInfoListView.Items.RemoveAt(i);
                    _points_info_manager.RemoveAt(i);
                }
            }
        }

        private void ColumnSelectionMenuItem_Click(object sender, EventArgs e)
        {
            PointsInfoViewCtrl.SetColumnVisible(timeToolStripMenuItem.Checked, (int)timeToolStripMenuItem.Tag);
        }

        private void checkAllButton_Click(object sender, EventArgs e)
        {
            m_checkAllState = !m_checkAllState;
            var list_view = PointsInfoViewCtrl.PointInfoListView;
            if (!m_checkAllState)
                while (list_view.CheckedItems.Count > 0)
                    list_view.CheckedItems[0].Checked = false;
            else
                for (int i = 0; i < list_view.Items.Count; i++)
                    list_view.Items[i].Checked = true;

            //checkAllButton.checkAllButton.Text = (m_checkAllState) ? "Выбрать все" : "";
            checkAllButton.Image = ((System.Drawing.Image)((m_checkAllState) ? global::Registrator.Properties.Resources.iconCheckAll : global::Registrator.Properties.Resources.iconUnCheckAll));
        }

        private void FramesPanel_FormClosed(object sender, FormClosedEventArgs e)
        {
            _points_info_manager.AddPointInfoEventHandler += AddPointInfoEventHandler;
        }

        private void shotsList_DoubleClick(object sender, ItemEvent e)
        {
            ShotForm form = new ShotForm(e.PointInfo, _db_controller);
            form.ShowDialog();
        }

        private void shotsList_ItemDeleted(object sender, ItemEvent e)
        {
            _points_info_manager.RemoveAt(e.ItemIndex);
        }

        private void SaveCheckedShotsBtn_Click(object sender, EventArgs e)
        {
            folderBrowserDialog.SelectedPath = Properties.Settings.Default.IRBFrameShotSavePath;
            var res = folderBrowserDialog.ShowDialog();
            if (res == System.Windows.Forms.DialogResult.OK)
            {
                Properties.Settings.Default.IRBFrameShotSavePath = folderBrowserDialog.SelectedPath;
                string folder = folderBrowserDialog.SelectedPath;

                var list_view = PointsInfoViewCtrl.PointInfoListView;
                if (list_view.CheckedItems.Count > 0 && _irb_frame_saver == null)
                {
                    _irb_frame_saver = new MovieTransit();
                }
                foreach (var item in list_view.CheckedItems)
                {
                    var view_item = item as ListViewItem;
                    if(view_item.Tag == null)
                        continue;
                    SavePointInfo((point_info)view_item.Tag, folder);
                    int item_index = view_item.Index;
                    list_view.Items.RemoveAt(item_index);
                    _points_info_manager.RemoveAt(item_index);

                }
            }
        }

        MovieTransit _irb_frame_saver = null;
        private void SavePointInfo(point_info point_info,string folder_name)
        {
            string file_name;
            bool next = false;
            for (; ; ) 
            {
                file_name = folder_name + @"\" + generate_termogramm_name(point_info,next);
                if (!File.Exists(file_name))
                    break;
                next = true;
            }

            string object_name = get_objects_names(point_info);
            _irb_frame_saver.SaveFrameFromRawDataEx(point_info.frame_info.raw_data,
                                                    object_name,
                                                    point_info.frame_info._frame_coordinate.picket,
                                                    point_info.frame_info._frame_coordinate.offset, 
                                                    file_name);
        }

        string get_objects_names(point_info point_info)
        {
            string res = "";
            foreach (var object_id in point_info.objects)
            {
                res += object_id.ToString() + ";";
            }

            return res;
        }

        int _name_postfix_index = 0;
        string generate_termogramm_name(point_info point_info,bool next)
        {

            string name = "FrameShot_";
            if(point_info.timestamp == 0){
                name += _name_postfix_index.ToString();
            }
            else{
                name += get_data_time_str(point_info.timestamp);
            }

            if (next)
                name += "_" + _name_postfix_index++.ToString();

            return name + ".irb";
        }

        private string get_data_time_str(double unixTimeStamp)
        {
            return irb_frame_time_helper.build_time_string_from_unixtime(unixTimeStamp, "yyyy_MM_dd_HH_mm_ss");
        }

        private void ViewShotBtn_Click(object sender, EventArgs e)
        {
            var item = PointsInfoViewCtrl.SelectedItem;
            if (item == null)
                return;

            ShotForm form = new ShotForm(item, _db_controller);
            form.ShowDialog();
        }


    }
}
