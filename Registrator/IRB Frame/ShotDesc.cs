using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{

    public class irb_frame_time_helper
    {
        const string time_format = @"hh\:mm\:ss\.fff";
        public static string build_msec_to_string(UInt64 msec)
        {
            TimeSpan time_span = TimeSpan.FromMilliseconds((double)msec);
            return time_span.ToString(time_format);
        }
        public static string build_time_string_from_time(double timestamp)
        {
            ulong days = (ulong)timestamp;
            ulong time = (ulong)((timestamp - days) * 86400 * 1000);

            TimeSpan time_span = TimeSpan.FromMilliseconds((double)time);
            return time_span.ToString(time_format);
        }
    }

    public class ShotDesc
    {

        public enum ShotType
        {
            SHOT_TYPE_USER,
            SHOT_TYPE_OBJ,
            SHOT_TYPE_AREA
        }

        ShotType m_shotType = ShotType.SHOT_TYPE_USER;

        int m_fileNum = -1;
        int m_fileFrameNum = -1;
        int m_frameNum = -1;

        DateTimeOffset m_timeOffset;

        byte m_type = 0;

        String m_tempPath = "";

        map_object_info _map_object_info;
        map_point_info _map_point_info;

        double m_dist = 0.0;
        UInt64 _msec = 0;
        Int16 m_line = 0;
        Int16 m_path = 0;
        String m_obj = "";
        String m_pNo = "";
        String m_peregon = "";
        
        public ShotType TypeOfShot { get { return m_shotType; } set { m_shotType = value; } }
        public map_object_info map_object_info { get { return _map_object_info; } set { _map_object_info = value; } }
        public map_point_info map_point_info { get { return _map_point_info; } set { _map_point_info = value; } }
        public double Distance { get { return m_dist; } set { m_dist = value; } }
        public String MsecString { get { return irb_frame_time_helper.build_msec_to_string(_msec); } }
        public UInt64 Msec { get { return _msec; } set { _msec = value; } }
        public Int16 Line { get { return m_line; } set { m_line = value; } }
        public Int16 Path { get { return m_path; } set { m_path = value; } }
        public String ObjName { get { return m_obj; } set { m_obj = value; } }
        public String PicketNOffset { get { return m_pNo; } set { m_pNo = value; } }
        public String Peregon { get { return m_peregon; } set { m_peregon = value; } }
        public int FileNum { get { return m_fileNum; } set { m_fileNum = value; } }
        public int FileFrameNum { get { return m_fileFrameNum; } set { m_fileFrameNum = value; } }
        public int FrameNum { get { return m_frameNum; } set { m_frameNum = value; } }
        public DateTimeOffset TimeOffset { get { return m_timeOffset; } set { m_timeOffset = value; } }
        public DateTimeOffset DateOf { get { return m_timeOffset; } set { m_timeOffset = value; } }
        public byte Type { get { return m_type; } set { m_type = value; } }
        public String TempPath { get { return m_tempPath; } set { m_tempPath = value; } }
    }
}
