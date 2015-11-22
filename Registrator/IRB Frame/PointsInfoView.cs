using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Registrator
{
    public partial class PointsInfoView : UserControl
    {
        private DB.metro_db_controller _db_controller;
        public PointsInfoView(DB.metro_db_controller db_controller)
        {
            InitializeComponent();
            if (db_controller != null)
                _db_controller = new DB.metro_db_controller(db_controller);
        }

        const int columns_number = 5;

        public void AddPointInfo(point_info point_info)
        {
            AddShotToList(createListViewItem(point_info));
        }

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
            return dtDateTime.ToString("yyyy/MM/dd HH:mm:ss");
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
            public string[] view_data = new string[count_data_items];
        }

        public delegate void AddShotToListDelegate(ListViewItem item);

        void AddShotToList(ListViewItem item)
        {
            if (InvokeRequired)
                BeginInvoke(new AddShotToListDelegate(AddShotToList), new object[] { item });
            else
                PointsInfoViewCtrl.Items.Add(item);
        }


    }
}
