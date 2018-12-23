using System;
using System.Collections.Generic;
using System.Linq;
using ThermoRoutineLib;
using System.IO;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Drawing.Imaging;

namespace Registrator.IRB_Frame
{
    using db_object_info = DB.EFClasses.AllEquipment;

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

        public settings Settings { get => _settings; set => _settings = value; }
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

        public void ProcessIRBFrame(uint frame_id, _irb_frame_info frame_info, _area_temperature_measure objects_area_measure)
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
                float maxT = objects_area_measure.max == float.NaN ? frame_info.measure.tmax : objects_area_measure.max;

                if (!settings_cache.object_traits.is_manual || maxT >= settings_cache.object_traits.maxT)
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
                   objects = objects.Where((db_object) => db_object.maxTemperature <= maxT).ToList();
                }


                _processing_frame_info = frame_info;
                choice_frames.process_objects(objects,
                                                getObjectInfo: delegate (db_object_info obj)
                                                {
                                                    return new FrameObjectInfo(obj.Code, obj.shiftLine, maxT);
                                                },
                                                frame_coordinate: frame_info.coordinate.coordinate,
                                                frame_index: frame_id,
                                                frame_timestamp: frame_info.timestamp);

            }//if (_settings.filter_objects)

        }

        class FrameObjectInfo : FrameObjectBase
        {

            public FrameObjectInfo(int id, long coordinate, float temperat)
            {
                Id = id; Coordinate = coordinate;
                Temperature = temperat;
            }
            public float Temperature { get; }
        }

        sealed class alarm_termogramm_ctx
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
            alarm_termogramm_ctx ctx = new alarm_termogramm_ctx { frame_info = _processing_frame_info, alarm_traits = new AlarmTraits { maxT = ((FrameObjectInfo)arg.FrameObject).Temperature }, termorgramm_data_raw = frame_raw_data, objectId = arg.FrameObject.Id };
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
        const string termogramm_file_name = "termo.irb";
        const string raster_file_name = "termopic.jpg";
        const string info_file_name = "info.txt";

        const string complite_save_file_name = "tcompleted";

        string _termogramm_dir_path;

        MovieTransit _irb_frame_saver;
        DB.metro_db_controller _db_controller;

        UInt32 _file_name_postfix;
        string _last_dir_name;
        public AlarmFrameWriter(RunTimeAlarmController controller, DB.metro_db_controller db_controller, string termogramm_dir_path)
        {
            _termogramm_dir_path = termogramm_dir_path;
            _file_name_postfix = 0;

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


        private Bitmap CopyRasterToBitmap(byte[] data, int width, int height)
        {
            //Here create the Bitmap to the know height, width and format
            Bitmap bmp = new Bitmap(width, height, PixelFormat.Format32bppArgb);

            //Create a BitmapData and Lock all pixels to be written 
            BitmapData bmpData = bmp.LockBits(
                                 new Rectangle(0, 0, bmp.Width, bmp.Height),
                                 ImageLockMode.WriteOnly, bmp.PixelFormat);


            //Copy the data from the byte array into BitmapData.Scan0
            Marshal.Copy(data, 0, bmpData.Scan0, data.Length);


            //Unlock the pixels
            bmp.UnlockBits(bmpData);


            //Return the bitmap 
            return bmp;
        }

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

            string dir_path = create_files_directory(ev.frame_info);

            try
            {

                object raster = new byte[ev.frame_info.image_info.width * ev.frame_info.image_info.height * 4];

                string pallete_filename = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location) + "\\PAL\\RAIN.PAL";
                if (_irb_frame_saver.GetFrameRasterFromRawData(ev.frame_data, pallete_filename, ref raster))
                {
                    Bitmap image = CopyRasterToBitmap((byte[])raster, ev.frame_info.image_info.width, ev.frame_info.image_info.height);

                    image.Save(Path.Combine(dir_path, raster_file_name), ImageFormat.Jpeg);
                }

                StreamWriter description_file = File.CreateText(Path.Combine(dir_path, info_file_name));


                if (ev.frame_info.coordinate.coordinate != 0)
                {
                    description_file.WriteLine("{0};{1};{2}пк;{3}см", ev.frame_info.coordinate.line, ev.frame_info.coordinate.path, picket, offset * 10);
                }
                else
                    description_file.WriteLine();


                string temperature_data_str = "Measured temperature: " + ev.frame_info.measure.tmax + "C. Max temperature limit: " + ev.alarm_traits.maxT + "C.";

                description_file.WriteLine(temperature_data_str);
                if (object_name != "")
                    description_file.WriteLine(object_name);


                if (_irb_frame_saver.SaveFrameFromRawDataEx(ev.frame_data, object_name, picket, offset, Path.Combine(dir_path, termogramm_file_name)))
                {
                    File.Create(Path.Combine(dir_path, complite_save_file_name));
                }
                else
                    Directory.Delete(dir_path, true);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);

                if (Directory.Exists(dir_path))
                    Directory.Delete(dir_path, true);
            }

        }

        string create_files_directory(_irb_frame_info frame_info)
        {
            string dir_name = irb_frame_time_helper.build_time_string_from_unixtime(frame_info.timestamp, "YYMMDDhhmmssfff") + "-" + frame_info.coordinate.counter;

            string dir_name_postfix = "-";

            if (_last_dir_name == dir_name)
            {
                _file_name_postfix++;
                dir_name_postfix += _file_name_postfix.ToString();
            }
            else
            {
                _file_name_postfix = 0;
                _last_dir_name = dir_name;
            }

            var dir_path = Path.Combine(_termogramm_dir_path, dir_name);
            var dir_path_full = dir_name_postfix == "-" ? dir_path : dir_path + dir_name_postfix;
            uint postfix_tmp = _file_name_postfix;
            while (Directory.Exists(dir_path_full))
            {
                dir_path_full = dir_path + "-" + dir_name_postfix.ToString();
            }

            Directory.CreateDirectory(dir_path_full);

            return dir_path_full;
        }


    }

}
