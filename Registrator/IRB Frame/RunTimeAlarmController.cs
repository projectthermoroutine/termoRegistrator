using System;
using System.Collections.Generic;
using System.Linq;
using ThermoRoutineLib;
using System.IO;

namespace Registrator.IRB_Frame
{
    using db_object_info = DB.ResultEquipCode;

    public sealed class AlarmTraits
    {
        public float maxT = float.MaxValue;
    }

    public sealed class RunTimeAlarmController
    {
        public event EventHandler<AlarmFrameEvent> AlarmFrameHandler;

        const long max_frame_distance_mm = 10000;
        public sealed class settings
        {
            public sealed class object_settings
            {
                public float maxT = float.MaxValue;
                public bool  is_manual = false;
            }

            public bool filter_objects = false;
            public bool filter_frame = false;

            public object_settings object_traits = new object_settings();
            public object_settings frame_traits = new object_settings();
        }

        DB.metro_db_controller _db_controller;
        settings _settings = new settings();
        get_frame_raw_data_func get_frame_raw_data = null;


        public RunTimeAlarmController(DB.metro_db_controller db_controller)
        {
            line_path_set = false;
            if (db_controller != null)
                _db_controller = new DB.metro_db_controller(db_controller);

            choice_frames = new ChoiceFrameObject();
            choice_frames.SaveObjectFrameProcessHandler += process_object_termogramme;
        }

        public settings Settings { set { _settings = value; } }
        public void SetFrameRawDataDelegate(get_frame_raw_data_func get_frame_raw_data_func) { get_frame_raw_data = get_frame_raw_data_func; }

        List<db_object_info> get_objects_by_coordinate(_frame_coordinate frame_coordinate)
        {
            return _db_controller.get_objects_by_coordinate(frame_coordinate.line, frame_coordinate.path, frame_coordinate.coordinate, max_frame_distance_mm).ToList();
        }

        ChoiceFrameObject choice_frames;

        _irb_frame_info _processing_frame_info;

        string line;
        string path;
        byte path_type;
        bool line_path_set;

        public void ProcessIRBFrame(uint frame_id, _irb_frame_info frame_info)
        {
            if (get_frame_raw_data == null)
                return;

            bool coordinate_valid = frame_info.coordinate.path != "" && frame_info.coordinate.line != "";

            settings settings_cache = _settings;

            if (settings_cache.filter_frame)
            {
                if (coordinate_valid && frame_info.measure.tmax >= settings_cache.frame_traits.maxT)
                {
                    process_alarm_frame_termogramme(frame_id, frame_info);
                }
            }

            if (settings_cache.filter_objects)
            {
                if (!settings_cache.object_traits.is_manual || frame_info.measure.tmax >= settings_cache.object_traits.maxT)
                {
                    if (line_path_set)
                    {
                        if (coordinate_valid)
                        {
                            if (path != frame_info.coordinate.path || path_type != frame_info.coordinate.path_type || line != frame_info.coordinate.line)
                            {
                                line = frame_info.coordinate.line;
                                path = frame_info.coordinate.path;
                                path_type = frame_info.coordinate.path_type;
                                choice_frames.Flush();
                            }
                        }
                        else
                        {
                            line_path_set = false;
                            choice_frames.Flush();
                            return;
                        }
                    }
                    else
                    {
                        if (coordinate_valid)
                        {
                            line_path_set = true;
                            line = frame_info.coordinate.line;
                            path = frame_info.coordinate.path;
                            path_type = frame_info.coordinate.path_type;
                        }
                        else
                            return;
                    }
                }


                var objects = get_objects_by_coordinate(frame_info.coordinate);
                if (!settings_cache.object_traits.is_manual)
                {
                   objects = objects.Where((db_object) => db_object.maxTemperature <= frame_info.measure.tmax).ToList();
                }

                _processing_frame_info = frame_info;
                choice_frames.process_objects(objects,
                                                delegate(DB.ResultEquipCode obj, out int objId, out long obj_coord)
                                                {
                                                    objId = obj.Code;
                                                    obj_coord = obj.shiftLine;
                                                },
                                                frame_info.coordinate.coordinate,
                                                frame_id,
                                                frame_info.timestamp);

            }//if (_settings.filter_objects)

        }

        protected sealed class alarm_termogramm_ctx
        {
            public _irb_frame_info frame_info;
            public AlarmTraits alarm_traits;
            public Array termorgramm_data_raw;
            public int objectId;
        }



        void process_object_termogramme(object sender, SaveObjectFrameProcessEvent arg)
        {
            var frame_raw_data = get_frame_raw_data((int)arg.FrameIndex);

            _frame_coordinate object_coordinate = _processing_frame_info.coordinate;

            object_coordinate.coordinate = arg.FrameCoord;
            alarm_termogramm_ctx ctx = new alarm_termogramm_ctx { frame_info = _processing_frame_info, termorgramm_data_raw = frame_raw_data, objectId = arg.ObjectId };
            ctx.frame_info.coordinate = object_coordinate;

            process_alarm_termogramme(ctx);

        }


        void process_alarm_frame_termogramme(uint frame_id, _irb_frame_info frame_info)
        {
            var frame_raw_data = get_frame_raw_data((int)frame_id);

            alarm_termogramm_ctx ctx = new alarm_termogramm_ctx { frame_info = _processing_frame_info, termorgramm_data_raw = frame_raw_data, objectId = -1 };
            process_alarm_termogramme(ctx);
        }

        void process_alarm_termogramme(alarm_termogramm_ctx ctx)
        {

            EventHandler<AlarmFrameEvent> handler = AlarmFrameHandler;
            if (handler != null)
                handler.BeginInvoke(this, new AlarmFrameEvent(ctx.frame_info, ctx.alarm_traits, ctx.objectId, ctx.termorgramm_data_raw), null, null);

        }

    }

    public class AlarmFrameEvent : EventArgs
    {

        public AlarmFrameEvent(_irb_frame_info frame_info, AlarmTraits alarm_traits, int objectId, Array termogramm_data)
            : base()
        {
            this.frame_info = frame_info;
            this.alarm_traits = alarm_traits;
            this.objectId = objectId;
            this.frame_data = termogramm_data;
        }

        public _irb_frame_info frame_info { get; set; }
        public AlarmTraits alarm_traits { get; set; }
        public int objectId { get; set; }
        public Array frame_data { get; set; }
    }



    internal sealed class AlarmFrameWriter
    {
        const string termogramm_file_name_prefix = "alarm_termogramm_";

        string _termogramm_dir_path;

        MovieTransit _irb_frame_saver;
        DB.metro_db_controller _db_controller;

        UInt32 _file_name_postfix;
        long _last_frame_coordinate;
        public AlarmFrameWriter(RunTimeAlarmController controller, DB.metro_db_controller db_controller, string termogramm_dir_path)
        {
            _termogramm_dir_path = termogramm_dir_path;
            _file_name_postfix = 0;
            _last_frame_coordinate = 0;

            if (db_controller != null)
                _db_controller = new DB.metro_db_controller(db_controller);

            _irb_frame_saver = new MovieTransit();

            if (controller != null)
                controller.AlarmFrameHandler += write_alarm_frame;

        }


        //[Serializable]
        //internal class FrameBlobData : ISerializable
        //{
        //    public string TestProperty { get; set; }

        //    public void GetObjectData(SerializationInfo info,
        //                              StreamingContext context)
        //    {
        //        info.AddValue("TestProperty", this.TestProperty);
        //    }
        //}



        void write_alarm_frame(object sender, AlarmFrameEvent ev)
        {
            string object_name = "";
            int picket = ev.frame_info.coordinate.picket;
            int offset = ev.frame_info.coordinate.offset;
            if (ev.objectId != -1)
            {
                try
                {
                    var db_object = _db_controller.GetObjectById(ev.objectId);
                    if (db_object != null)
                    {
                        object_name = db_object.Name;
                        picket = db_object.Picket;
                        offset = db_object.shiftFromPicket;
                    }
                }
                catch (DB.DBRegistratorException /*exc*/)
                { }

            }

            string file_path = generate_termogramm_file_path(ev.objectId, ev.frame_info.coordinate.coordinate);

            if (!_irb_frame_saver.SaveFrameFromRawDataEx(ev.frame_data, object_name, picket, offset, file_path))
            {
                return;
            }


            StreamWriter description_file = File.CreateText(Path.ChangeExtension(file_path, "txt"));

            string temperature_data_str = "Measured temperature: " + ev.frame_info.measure.tmax + "C. Max temperature limit: " + ev.alarm_traits.maxT + "C.";

            description_file.WriteLine(temperature_data_str);
            if(object_name != "")
                description_file.WriteLine(object_name);

            //TODO generate pdf

        }

        string generate_termogramm_file_path(int objectId,
                                        long frame_coord
                                        )
        {
            string postfix = frame_coord.ToString();

            if (_last_frame_coordinate == frame_coord)
            {
                postfix += _file_name_postfix.ToString(); _file_name_postfix++;
            }
            else
                _file_name_postfix = 0;

            _last_frame_coordinate = frame_coord;

            return Path.Combine(_termogramm_dir_path,
                                    termogramm_file_name_prefix +
                                    postfix +
                                    /*"_" + irb_frame_time_helper.build_time_string_from_unixtime(timestamp, "yyyy_MM_dd_HH_mm_ss") +*/
                                    ".irb"
                                );
        }


    }

}
