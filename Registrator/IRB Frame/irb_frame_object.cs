using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using ThermoRoutineLib;

namespace Registrator
{

    public class irb_frame_object
    {
        public _frame_coordinate _frame_coordinate;
        public Array raw_data = null;
    }

    public class object_info
    {
        public object_info(int ObjectId_) { ObjectId = ObjectId_; }
        public int ObjectId = -1;
    }

    public class point_info
    {
        public irb_frame_object frame_info;
        public double timestamp;
        public List<object_info> objects = new List<object_info>();
    }

    public delegate Array get_frame_raw_data_func(int frameId);
    public delegate List<object_info> get_objects_by_coordinate_func(_frame_coordinate frame_coordinate);
    public class point_info_factory
    {
        public static point_info create_point_info(int frameId, _frame_coordinate frame_coordinate,double data_time, get_frame_raw_data_func get_frame_raw_data, get_objects_by_coordinate_func get_objects_by_coordinate)
        {
            point_info point_info_ = new point_info();

            point_info_.frame_info = new irb_frame_object();
            point_info_.frame_info._frame_coordinate = frame_coordinate;
            point_info_.frame_info.raw_data = get_frame_raw_data(frameId);
            point_info_.objects = get_objects_by_coordinate(frame_coordinate);
            point_info_.timestamp = data_time;

            return point_info_;
        }
    }

    public class PointsInfoManager
    {

        public event EventHandler<PointInfoEvent> AddPointInfoEventHandler;

        public PointsInfoManager()
        {
            _points_info_list = new List<point_info>();
        }

        List<point_info> _points_info_list;

        public void Add(point_info point_info) { _points_info_list.Add(point_info); PointInfoAdded(point_info); }
        private void PointInfoAdded(point_info point_info_)
        {
            EventHandler<PointInfoEvent> handler = AddPointInfoEventHandler;
            if (handler != null)
                handler(this, new PointInfoEvent(point_info_));
        }

        public void RemoveAt(int index) { _points_info_list.RemoveAt(index); }
        public ReadOnlyCollection<point_info> PointsInfoList() { return _points_info_list.AsReadOnly(); }

    }

    public class PointInfoEvent : EventArgs
    {
        private point_info _point_info;
        public PointInfoEvent(point_info point_info_)
        {
            _point_info = point_info_;
        }
        public point_info PointInfo { get { return _point_info; } }
    }


}
