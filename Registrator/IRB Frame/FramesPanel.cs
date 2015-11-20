using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Registrator
{
    public partial class FramesPanel : ToolWindow
    {
        private bool m_checkAllState = true;
        private DB.metro_db_controller _db_controller;
        PointsInfoManager _points_info_manager;
        
        public FramesPanel(PointsInfoManager points_info_manager, DB.metro_db_controller db_controller)
        {
            InitializeComponent();
            if (db_controller != null)
                _db_controller = new DB.metro_db_controller(db_controller);

            _points_info_manager = points_info_manager;

            foreach (var item in _points_info_manager.PointsInfoList())
            {
                var view_item = createListViewItem(item);
                AddShotToList(view_item);
            }

            points_info_manager.AddPointInfoEventHandler += AddPointInfoEventHandler;
        }
        void AddPointInfoEventHandler(object sender, PointInfoEvent e)
        {
            var point_info = e.PointInfo;
            var view_item = createListViewItem(point_info);
            AddShotToList(view_item);
        }
        const int columns_number = 5;
        ListViewItem createListViewItem(point_info point_info)
        {
            var view_data = retrieve_view_item_data(point_info);
            ListViewItem item = new ListViewItem("", 0);
            for (int i = 0; i < columns_number; i++)
            {
                item.SubItems.Add(view_data.view_data[i]);
            }
            return item;
        }

        private string get_data_time_str(double unixTimeStamp)
        {
            System.DateTime dtDateTime = new DateTime(1970, 1, 1, 0, 0, 0, 0, System.DateTimeKind.Utc);
            dtDateTime = dtDateTime.AddSeconds(unixTimeStamp).ToLocalTime();
            return dtDateTime.ToString("yy/MM/dd ff:mm:ss");
        }

        enum ColumnIndex
        {
            DATA_TIME,
            LINE,
            PATH,
            COORDINATE,
            OBJECTS
        }

        view_point_info retrieve_view_item_data(point_info point_info)
        {
            view_point_info result_data = new view_point_info();
            result_data.view_data[(int)ColumnIndex.DATA_TIME] = get_data_time_str(point_info.timestamp);
            result_data.view_data[(int)ColumnIndex.LINE] = point_info.frame_info._frame_coordinate.line;
            result_data.view_data[(int)ColumnIndex.PATH] = point_info.frame_info._frame_coordinate.path;
            result_data.view_data[(int)ColumnIndex.COORDINATE] = point_info.frame_info._frame_coordinate.picket.ToString() + " пк " +
                                                                (point_info.frame_info._frame_coordinate.offset / (100 * 10)).ToString() + " м";
            result_data.view_data[(int)ColumnIndex.OBJECTS] = retrieve_objects_names(point_info);

            return result_data;
        }

        string retrieve_objects_names(point_info point_info)
        {
            _db_controller.setLineAndPath(point_info.frame_info._frame_coordinate.line, point_info.frame_info._frame_coordinate.path);
            var objects = _db_controller.get_objects_by_coordinate(point_info.frame_info._frame_coordinate.coordinate + point_info.frame_info._frame_coordinate.camera_offset, 50);

            string res = "";
            foreach (var cur_object in objects)
            {
                res += cur_object.name + ";";
            }

            return res;
        }

        class view_point_info
        {
            const int count_data_items = 5;
            public string [] view_data = new string[count_data_items];
        }

        public delegate void AddShotToListDelegate(ListViewItem item);

        public void AddShotToList(ListViewItem item)
        {
            if (InvokeRequired)
                BeginInvoke(new AddShotToListDelegate(AddShotToList), new object[] { item });
            else
                shotsList.Items.Add(item);
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {

            bool checkedPresence = false;

            for (int i = shotsList.Items.Count - 1 ; i > -1  ; i-- )
            {
                if (shotsList.Items[i].Checked)
                {
                    shotsList.Items.RemoveAt(i);
                    checkedPresence = true;
                }

            }

            if (!checkedPresence && shotsList.SelectedIndices.Count > 0)
                shotsList.Items.RemoveAt(shotsList.SelectedIndices[0]);
        
        }

        private void timeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            shotsList.Columns[1].Width = (timeToolStripMenuItem.Checked) ? 89 : 0;
        }

        private void lineToolStripMenuItem_Click(object sender, EventArgs e)
        {
            shotsList.Columns[2].Width = (lineToolStripMenuItem.Checked) ? 60 : 0;
        }

        private void pathToolStripMenuItem_Click(object sender, EventArgs e)
        {
            shotsList.Columns[3].Width = (pathToolStripMenuItem.Checked) ? 60 : 0;
        }
        
        private void piketToolStripMenuItem_Click(object sender, EventArgs e)
        {
            shotsList.Columns[4].Width = (pNoToolStripMenuItem.Checked) ? 62 : 0;
        }

        private void objToolStripMenuItem_Click(object sender, EventArgs e)
        {
            shotsList.Columns[6].Width = (objToolStripMenuItem.Checked) ? 112 : 0;
        }

        private void checkAllButton_Click(object sender, EventArgs e)
        {

            if (!m_checkAllState)
                while (shotsList.CheckedItems.Count > 0)
                    shotsList.CheckedItems[0].Checked = false;
            else
                for (int i = 0; i < shotsList.Items.Count; i++)
                    shotsList.Items[i].Checked = true;

            m_checkAllState = !m_checkAllState;
            //checkAllButton.checkAllButton.Text = (m_checkAllState) ? "Выбрать все" : "";
            checkAllButton.Image = ((System.Drawing.Image)((m_checkAllState) ? global::Registrator.Properties.Resources.iconCheckAll : global::Registrator.Properties.Resources.iconUnCheckAll));
        }

        private void FramesPanel_FormClosed(object sender, FormClosedEventArgs e)
        {
            _points_info_manager.AddPointInfoEventHandler += AddPointInfoEventHandler;
        }
    }
}
