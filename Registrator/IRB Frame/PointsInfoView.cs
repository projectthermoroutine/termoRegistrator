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

        enum ColumnIndex
        {
            DATA_TIME,
            LINE,
            PATH,
            COORDINATE,
            OBJECTS
        }

        public event EventHandler<ItemEvent> DoubleClickItem;
        public event EventHandler<ItemEvent> ItemDeleted;
        public event EventHandler<ItemEvent> ItemPressed;

        private DB.metro_db_controller _db_controller = new DB.metro_db_controller(null);
        public PointsInfoView()
        {
            AutoCheckNewItem = false;
            InitializeComponent();
        }

        public void SetColumnVisible(bool visible, int index) { if ((uint)index > columns_number) return; int width = visible ? 60 : 0; PointsInfoViewCtrl.Columns[index].Width = width; }

        public ListView PointInfoListView { get{ return PointsInfoViewCtrl;}}

        public bool CheckBoxes{ set{ this.PointsInfoViewCtrl.CheckBoxes = value;}}
        public point_info SelectedItem
        {
            get
            {
                if (PointsInfoViewCtrl.SelectedItems.Count == 0)
                    return null;
                var item = PointsInfoViewCtrl.SelectedItems[0];
                return (point_info)item.Tag;
            }
        }

        const int columns_number = 5;

        public bool AutoCheckNewItem { get; set; }

        public void AddPointInfo(point_info point_info)
        {
            AddShotToList(createListViewItem(point_info));
        }

        ListViewItem createListViewItem(point_info point_info)
        {
            var view_data = retrieve_view_item_data(point_info);
            ListViewItem item = new ListViewItem("", 0);
            item.Tag = point_info;
            for (int i = 0; i < columns_number; i++)
            {
                item.SubItems.Add(view_data.view_data[i]);
            }
            return item;
        }

        private string get_data_time_str(double unixTimeStamp)
        {
            return irb_frame_time_helper.build_time_string_from_unixtime(unixTimeStamp, "yyyy/MM/dd HH:mm:ss");
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
            string res = "";
            foreach (var object_info in point_info.objects)
            {
                try
                {
                    var db_object = _db_controller.GetObjectById(object_info.ObjectId);
                    if (db_object != null)
                        res += db_object.Name + ";";
                }
                catch (DB.DBRegistratorException exc)
                {
                    res = exc.Message;
                    break;
                }
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
            {
                PointsInfoViewCtrl.Items.Add(item);
                if (AutoCheckNewItem)
                    item.Checked = true;
            }
        }

        private void PointsInfoViewCtrl_DoubleClick(object sender, EventArgs e)
        {
            if (PointsInfoViewCtrl.SelectedItems.Count == 0)
                return;

            var item = PointsInfoViewCtrl.SelectedItems[0];
            if (item.Tag == null)
                return;

            EventHandler<ItemEvent> handler = DoubleClickItem;
            if (handler != null)
                handler(this, new ItemEvent((point_info)item.Tag, item.Index));

        }

        private void PointsInfoViewCtrl_KeyDown(object sender, KeyEventArgs e)
        {
            if (PointsInfoViewCtrl.SelectedItems.Count == 0)
                return;

            var item = PointsInfoViewCtrl.SelectedItems[0];
            if (item.Tag == null)
                return;

            if (e.KeyCode == Keys.Delete)
            {
                EventHandler<ItemEvent> handler = ItemDeleted;
                if (handler != null)
                    handler(this, new ItemEvent((point_info)item.Tag, item.Index));

                PointsInfoViewCtrl.Items.RemoveAt(item.Index);
            }
            if (e.KeyCode == Keys.Enter)
            {
                EventHandler<ItemEvent> handler = ItemPressed;
                if (handler != null)
                    handler(this, new ItemEvent((point_info)item.Tag, item.Index));

            }
        }

    }

    public class ItemEvent : EventArgs
    {
        public point_info PointInfo { get; private set; }
        public int ItemIndex { get; private set; }

        public ItemEvent(point_info point_info,int index)
            : base()
        {
            PointInfo = point_info;
            ItemIndex = index;
        }
    }


}
