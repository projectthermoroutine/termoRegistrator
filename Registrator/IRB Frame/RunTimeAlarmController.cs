using System;
using System.Collections.Generic;
using System.Linq;
using ThermoRoutineLib;
using System.IO;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;
using System.Drawing.Text;

using System.Threading.Tasks;

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
        public delegate void ProcessAlarmFrame(object sender, AlarmFrameEvent e);

        public ProcessAlarmFrame processAlarmFrame;

        const long max_frame_distance_mm = 10000;
        public sealed class settings
        {
            public settings()
            {
                filter_frame = Properties.Settings.Default.AS_frame;
                filter_objects = Properties.Settings.Default.AS_objects;

                object_traits.is_manual = Properties.Settings.Default.AS_objects_manual_T;
                object_traits.maxT = object_traits.is_manual ? (float)Properties.Settings.Default.AS_objects_max_T : float.MaxValue;

                frame_traits.is_manual = true;
                frame_traits.maxT = (float)Properties.Settings.Default.AS_frame_max_T;

            }

            public void Save()
            {
                Properties.Settings.Default.AS_frame = filter_frame;
                Properties.Settings.Default.AS_objects = filter_objects;

                Properties.Settings.Default.AS_objects_manual_T = object_traits.is_manual;
                Properties.Settings.Default.AS_objects_max_T = (int)object_traits.maxT;

                Properties.Settings.Default.AS_frame_max_T = (int)frame_traits.maxT;
            }

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
        object _settings_mtx = new object();
        bool _flush = false;
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

        public settings Settings { get => _settings;
            set
            {
                lock (_settings_mtx)
                {
                    _settings = value;
                    if (!_settings.object_traits.is_manual) _settings.object_traits.maxT = float.MaxValue;

                    _flush = true;
                }
            }
        }
        public void SetFrameRawDataDelegate(get_frame_raw_data_func get_frame_raw_data_func) { get_frame_raw_data = get_frame_raw_data_func; }

        List<db_object_info> get_objects_by_coordinate(_frame_coordinate frame_coordinate)
        {
            return _db_controller.get_objects_by_coordinate(frame_coordinate.line, frame_coordinate.path, frame_coordinate.coordinate, max_frame_distance_mm).ToList();
        }

        ChoiceFrameObject choice_frames;

        _irb_frame_info _processing_frame_info;

        List<db_object_info> _emptyObjectList = new List<db_object_info>();

        string line;
        string path;
        byte path_type;
        bool line_path_set;

        public void ProcessIRBFrame(uint frame_id, _irb_frame_info frame_info, _area_temperature_measure objects_area_measure, _point Area_MaxT_Point)
        {
            if (get_frame_raw_data == null)
                return;

            bool coordinate_valid = frame_info.coordinate.path != "" && frame_info.coordinate.line != "";

            settings settings_cache;

            lock (_settings_mtx)
            {
                settings_cache = _settings;
            }

            if (_flush)
            {
                _flush = false;
                choice_frames.Flush(false);
            }

            if (settings_cache.filter_frame)
            {
                if (coordinate_valid && frame_info.measure.tmax >= settings_cache.frame_traits.maxT)
                {
                    process_alarm_frame_termogramme(frame_id, frame_info, settings_cache.frame_traits.maxT);
                }
            }

            if (settings_cache.filter_objects)
            {
                _point MaxT_Point = frame_info.max_T_point;
                float maxT = frame_info.measure.tmax;
                if (!float.IsNaN(objects_area_measure.max))
                {
                    maxT = objects_area_measure.max;
                    MaxT_Point = Area_MaxT_Point;
                }

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


                float limitMaxT = settings_cache.object_traits.maxT;
                List<db_object_info> objects = _emptyObjectList;

                if (settings_cache.object_traits.is_manual)
                {
                    if(maxT >= limitMaxT)
                        objects = get_objects_by_coordinate(frame_info.coordinate);
                }
                else
                {
                    objects = get_objects_by_coordinate(frame_info.coordinate).Where((db_object) => db_object.maxTemperature <= maxT).ToList();
                    limitMaxT = float.NaN;
                }

                _processing_frame_info = frame_info;
                choice_frames.process_objects(objects,
                                                getObjectInfo: delegate (db_object_info obj)
                                                {
                                                    return new FrameObjectInfo(obj.Code, obj.shiftLine, maxT, limitMaxT, MaxT_Point);
                                                },
                                                frame_coordinate: frame_info.coordinate.coordinate,
                                                frame_index: frame_id,
                                                frame_timestamp: frame_info.timestamp);
            }//if (_settings.filter_objects)

        }

        class FrameObjectInfo : FrameObjectBase
        {

            public FrameObjectInfo(int id, long coordinate, float temperat, float maxT, _point max_T_point)
            {
                Id = id; Coordinate = coordinate;
                Temperature = temperat;
                MaxT = maxT;
                MaxT_Point = max_T_point;
            }
            public float Temperature { get; }
            public float MaxT { get; }
            public _point MaxT_Point { get; }
        }

        void process_object_termogramme(object sender, SaveObjectFrameProcessEvent arg)
        {
            var frame_raw_data = get_frame_raw_data((int)arg.FrameIndex);

            _frame_coordinate object_coordinate = _processing_frame_info.coordinate;

            object_coordinate.coordinate = arg.FrameCoord;

            float MaxT = ((FrameObjectInfo)arg.FrameObject).MaxT;
            if (float.IsNaN(MaxT))
            {
                try
                {
                    var db_object = _db_controller.GetObjectById(arg.FrameObject.Id);
                    if (db_object != null)
                    {
                        MaxT = db_object.maxTemperature;
                    }
                }
                catch (DB.DBRegistratorException /*exc*/)
                { }
            }

            alarm_termogramm_ctx ctx = new alarm_termogramm_ctx
            {
                frame_info = _processing_frame_info,
                alarm_traits = new AlarmTraits { maxT = MaxT },
                termorgramm_data_raw = frame_raw_data,
                objectId = arg.FrameObject.Id,
                maxT = ((FrameObjectInfo)arg.FrameObject).Temperature,
                maxT_point = ((FrameObjectInfo)arg.FrameObject).MaxT_Point
            };

            ctx.frame_info.coordinate = object_coordinate;

            process_alarm_termogramme(ctx);

        }


        void process_alarm_frame_termogramme(uint frame_id, _irb_frame_info _frame_info, float maxTemperature)
        {
            var frame_raw_data = get_frame_raw_data((int)frame_id);

            alarm_termogramm_ctx ctx = new alarm_termogramm_ctx
            {
                frame_info = _frame_info,
                alarm_traits = new AlarmTraits { maxT = maxTemperature },
                termorgramm_data_raw = frame_raw_data,
                objectId = -1,
                maxT = _frame_info.measure.tmax,
                maxT_point = _frame_info.max_T_point
            };

            process_alarm_termogramme(ctx);
        }

        void process_alarm_termogramme(alarm_termogramm_ctx ctx)
        {
            //EventHandler<AlarmFrameEvent> handler = AlarmFrameHandler;
            //if (handler != null)
            //    handler.BeginInvoke(this, new AlarmFrameEvent(ctx), null, null);

            var e = new AlarmFrameEvent(ctx);

            if (processAlarmFrame != null)
                Task.Factory.StartNew(() =>
                {
                    processAlarmFrame(this, e);
                });
        }

    }

    public class alarm_termogramm_ctx
    {
        public _irb_frame_info frame_info;
        public AlarmTraits alarm_traits;
        public Array termorgramm_data_raw;
        public int objectId;
        public float maxT;
        public _point maxT_point;
    }


    public class AlarmFrameEvent : EventArgs
    {

        public AlarmFrameEvent(alarm_termogramm_ctx Ctx)
            : base()
        {
            this.Ctx = Ctx;
        }
        public alarm_termogramm_ctx Ctx { get; }
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

        bool _offset_positive = false;

        UInt32 _file_name_postfix;
        string _last_dir_name;

        bool _drawMetaInfo;
        public AlarmFrameWriter(RunTimeAlarmController controller/*, DB.metro_db_controller db_controller*/, string termogramm_dir_path)
        {
            _termogramm_dir_path = termogramm_dir_path;
            _file_name_postfix = 0;

            //if (db_controller != null)
                _db_controller = new DB.metro_db_controller(null/*db_controller*/);

            _irb_frame_saver = new MovieTransit();

            if (controller != null)
            {
                controller.processAlarmFrame = write_alarm_frame;
                //controller.AlarmFrameHandler += write_alarm_frame;
            }

            _offset_positive = Properties.Settings.Default.AS_offset_positive;
            _drawMetaInfo = Properties.Settings.Default.AS_draw_meta;

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

        private void AddVisualInfoToFrameImage(Bitmap image, _point maxT_point, float point_T)
        {
            //Bitmap tempBMP = new Bitmap(image.Width, image.Height);
            //Graphics g = Graphics.FromImage(tempBMP);
            Graphics g = Graphics.FromImage(image);

            Pen colorPen = new Pen(Color.Red, 3);
            // Create rectangle for ellipse.
            Rectangle rect = new Rectangle(maxT_point.x - 20, maxT_point.y - 20, 40, 40);
            g.DrawEllipse(colorPen, rect);
            g.DrawRectangle(new Pen(Color.FromArgb(0xFF, 05, 0xE9, 0xF3)), maxT_point.x, maxT_point.y, 1, 1);

            //g.CompositingMode = CompositingMode.SourceCopy; //CompositingMode.SourceOver;

            if (_drawMetaInfo)
            {
                // Create string to draw.
                var drawString = $"{point_T} C. [x: {maxT_point.x}, y: {maxT_point.y}]";

                // Create font and brush.
                Font drawFont = new Font("Arial", 14);

                // Create rectangle for drawing.
                float width = g.MeasureString(drawString, drawFont).Width;
                float height = 25.0F;
                RectangleF drawRect = new RectangleF(maxT_point.x - 20, maxT_point.y - 20 - height, width, height);

                // Draw rectangle to screen.
                Pen blackPen = new Pen(Color.Black);
                g.DrawRectangle(blackPen, maxT_point.x - 20, maxT_point.y - 20 - height, width, height);

                g.SmoothingMode = SmoothingMode.AntiAlias;
                g.InterpolationMode = InterpolationMode.HighQualityBicubic;
                g.PixelOffsetMode = PixelOffsetMode.HighQuality;
                g.TextRenderingHint = TextRenderingHint.SingleBitPerPixelGridFit;
                // Draw string to screen.
                g.DrawString(drawString, drawFont, Brushes.Red, drawRect);
            }

            //g.DrawImage(image, new PointF(0, 0));
            //image = tempBMP;
        }


        void write_alarm_frame(object sender, AlarmFrameEvent ev)
        {
            var Ctx = ev.Ctx;

            string object_name = "";
            int picket = Ctx.frame_info.coordinate.picket;
            string picket_name = picket.ToString();
            int offset = Ctx.frame_info.coordinate.offset;
            if (picket == 0 && offset < 0)
                picket_name = "-" + picket_name;

            if (Ctx.objectId != -1)
            {
                try
                {
                    var db_object = _db_controller.GetObjectById(Ctx.objectId);
                    if (db_object != null)
                    {
                        object_name = db_object.Name;
                        var picket_obj = _db_controller.GetPicketInfo(db_object.Picket);
                        picket_name = picket_obj.Npiketa;
                        picket = Convert.ToInt32(picket_name);
                        offset = db_object.shiftFromPicket;
                        if (_offset_positive && offset < 0)
                            offset += picket_obj.Dlina;
                    }
                }
                catch (DB.DBRegistratorException /*exc*/)
                { }

            }

            string dir_path = create_files_directory(Ctx.frame_info);

            try
            {

                object raster = new byte[Ctx.frame_info.image_info.width * Ctx.frame_info.image_info.height * 4];

                string pallete_filename = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location) + "\\PAL\\RAIN.PAL";
                if (_irb_frame_saver.GetFrameRasterFromRawData(Ctx.termorgramm_data_raw, pallete_filename, ref raster))
                {
                    using (Bitmap image = CopyRasterToBitmap((byte[])raster, Ctx.frame_info.image_info.width, Ctx.frame_info.image_info.height))
                    {
                        AddVisualInfoToFrameImage(image, Ctx.maxT_point, Ctx.maxT);
                        image.Save(Path.Combine(dir_path, raster_file_name), ImageFormat.Jpeg);
                    }
                }

                using (StreamWriter description_file = File.CreateText(Path.Combine(dir_path, info_file_name)))
                {

                    if (Ctx.frame_info.coordinate.coordinate != 0)
                    {
                        description_file.WriteLine("{0};{1};{2}пк;{3}см", Ctx.frame_info.coordinate.line, Ctx.frame_info.coordinate.path, picket_name, offset / 10);
                    }
                    else
                        description_file.WriteLine();


                    string temperature_data_str = "Измеренная темп.: " + Ctx.maxT + " C. Макс. предел темп.: " + Ctx.alarm_traits.maxT + " C.";

                    description_file.WriteLine(temperature_data_str);
                    if (object_name != "")
                        description_file.WriteLine(object_name);
                }

                if (_irb_frame_saver.SaveFrameFromRawDataEx(Ctx.termorgramm_data_raw, object_name, picket, offset, Path.Combine(dir_path, termogramm_file_name)))
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
            string dir_name = irb_frame_time_helper.build_time_string_from_unixtime(frame_info.timestamp, "yyMMddHHmmssfff") + "-" + frame_info.coordinate.counter;

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
