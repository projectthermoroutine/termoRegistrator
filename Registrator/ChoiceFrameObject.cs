using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{

    public class SaveObjectFrameProcessEvent : EventArgs
    {
        private int _objectId;
        private uint _frame_index;
        private long _frame_coord;
        private double _frame_timestamp;

        public SaveObjectFrameProcessEvent(
                    int objectId, 
                    uint frame_index, 
                    long frame_coord, 
                    double frame_timestamp
        )
            : base()
        {
            _objectId = objectId;
            _frame_index = frame_index;
            _frame_coord = frame_coord;
            _frame_timestamp = frame_timestamp;
        }

        public int ObjectId { get { return _objectId; }}
        public uint FrameIndex { get { return _frame_index; }}
        public long FrameCoord { get { return _frame_coord; } }
        public double FrameTimeStamp { get { return _frame_timestamp; } }
    }

    public class ChoiceFrameObject : IDisposable
    {

        public event EventHandler<SaveObjectFrameProcessEvent> SaveObjectFrameProcessHandler;

        private void SaveObjectFrameProcess(LIST_ITEM item)
        {
            EventHandler<SaveObjectFrameProcessEvent> handler = SaveObjectFrameProcessHandler;
            if (handler != null)
                handler(this, new SaveObjectFrameProcessEvent(item.objectId, item.nearest_frame_index, item.frame_coordinate, item.frame_timestamp));
        }



        const long max_frame_distance_cm = 500;

        public ChoiceFrameObject()
        {
            _processing_objects = new List<LIST_ITEM>();
            _processed_objects = new List<LIST_ITEM>();
        }

        class LIST_ITEM
        {
            public LIST_ITEM()
            { objectId = 0; object_coordinate = 0; nearest_frame_index = 0; frame_coordinate = 0; frame_timestamp = 0.0; }
            public LIST_ITEM(int objId, long obj_coordinate)
                : this()
            {
                objectId = objId; object_coordinate = obj_coordinate;
            }
            public int objectId;
            public long object_coordinate;
            public UInt32 nearest_frame_index;
            public long frame_coordinate;
            public double frame_timestamp;
        }

        List<LIST_ITEM> _processing_objects;
        List<LIST_ITEM> _processed_objects;

        public delegate void getObjectInfoAction<in T>(T obj,out int objId,out long obj_coord);

        public void process_objects<T>(List<T> objects, getObjectInfoAction<T> getObjectInfo, long frame_coordinate, uint frame_index, double frame_timestamp)
        {

            List<LIST_ITEM> objects_info = new List<LIST_ITEM>();
            foreach(var obj in objects)
            {
                LIST_ITEM new_item = new LIST_ITEM();
                getObjectInfo(obj, out new_item.objectId, out new_item.object_coordinate);
                var current_distance = calc_frame_object_distance(frame_coordinate, new_item.object_coordinate);
                if(max_frame_distance_cm * 10 >= current_distance) 
                    objects_info.Add(new_item);

            }

            process_objects(objects_info, frame_coordinate, frame_index, frame_timestamp);
        }

        void process_objects(List<LIST_ITEM> objects, long frame_coordinate, uint frame_index, double frame_timestamp)
        {
            _processed_objects.RemoveAll(delegate(LIST_ITEM processed_object)
            {
                var current_distance = calc_frame_object_distance(frame_coordinate, processed_object.object_coordinate);
                return max_frame_distance_cm * 10 < current_distance ? true : false;
            });

            List<LIST_ITEM> not_processed_objects = new List<LIST_ITEM>();

            foreach (var db_object in objects)
            {
                bool processed = false;
                foreach (var processed_object in _processed_objects)
                {
                    if (processed_object.objectId == db_object.objectId)
                    {
                        processed = true;
                        break;
                    }
                }
                if (!processed)
                {
                    db_object.frame_coordinate = frame_coordinate;
                    db_object.nearest_frame_index = frame_index;
                    db_object.frame_timestamp = frame_timestamp;
                    not_processed_objects.Add(db_object);
                }

            }

            List<LIST_ITEM> objects_for_save = new List<LIST_ITEM>();
            List<LIST_ITEM> new_objects = new List<LIST_ITEM>();

            foreach (var db_object in not_processed_objects)
            {
                LIST_ITEM nearest_frame = null;
                bool add_frame = true;
                foreach (var processing_object in _processing_objects)
                {
                    if (processing_object.objectId == db_object.objectId)
                    {
                        add_frame = false;
                        var prev_distance = calc_frame_object_distance(processing_object.frame_coordinate, processing_object.object_coordinate);
                        var current_distance = calc_frame_object_distance(db_object.frame_coordinate, db_object.object_coordinate);

                        if (prev_distance < current_distance)
                        {
                            nearest_frame = processing_object;
                        }
                        else
                        {
                            processing_object.frame_coordinate = db_object.frame_coordinate;
                            processing_object.nearest_frame_index = db_object.nearest_frame_index;
                            processing_object.frame_timestamp = db_object.frame_timestamp;
                        }

                        break;
                    }
                }
                if (nearest_frame != null)
                    objects_for_save.Add(nearest_frame);

                if (add_frame)
                {
                    new_objects.Add(db_object);
                }
            }

            save_objects_termogrammes(objects_for_save);
            _processed_objects.AddRange(objects_for_save);

            _processing_objects.RemoveAll(delegate(LIST_ITEM processing_object)
            {
                int index_for_delete = -1;
                for (int i = 0; i < objects_for_save.Count; i++)
                {
                    var saved_object = objects_for_save[i];
                    if (processing_object.objectId == saved_object.objectId)
                    {
                        index_for_delete = i;
                        break;
                    }
                }

                if (index_for_delete >= 0)
                {
                    objects_for_save.RemoveAt(index_for_delete);
                    return true;
                }

                return false;
            });

            _processing_objects.AddRange(new_objects);
        }

        long calc_frame_object_distance(long frame_coord, long object_coord)
        {
            var distance = frame_coord <= object_coord ? object_coord - frame_coord : frame_coord - object_coord;
            return distance >= 0 ? distance : -distance;
        }

        void save_objects_termogrammes(List<LIST_ITEM> objects)
        {
            foreach (var db_object in objects)
            {
                SaveObjectFrameProcess(db_object);
            }
        }

        public void close()
        {
            save_objects_termogrammes(_processing_objects);
            _processing_objects.Clear();
        }
        public void Dispose()
        {
            close();
        }
    }
}
