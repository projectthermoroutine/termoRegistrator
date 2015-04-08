using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    using map_objects_list = List<measure_object>;

    public class measure_object
    {
        public enum object_type
        {
            Object,
            Equipment
        };
        const int count_items = 17;

        public map_point _map_point;

        byte m_Type;
        long m_Code;
        int m_Length;
        long m_ObjData;
        long m_PtData;
        byte m_Dir;
        long m_Sec;
        bool m_forward;
        string m_Name;
        string m_ShortName;
        long  m_Time;

        long m_frame_number;

        public map_point map_point { get { return _map_point; } set { _map_point = value; } }

        public byte type { get { return m_Type; } set { m_Type = value; } }
        public string line { get { return _map_point.line; } set { _map_point.line = value; } }
        public string path { get { return _map_point.path; } set { _map_point.path = value; } }
        public long picket { get { return _map_point.picket; } set { _map_point.picket = value; } }
        public long offset { get { return _map_point.offset; } set { _map_point.offset = value; } }
        public long coord { get { return _map_point.coord; } set { _map_point.coord = value; } }
        public long code { get { return m_Code; } set { m_Code = value; } }
        public int length { get { return m_Length; } set { m_Length = value; } }
        public ulong span { get { return _map_point.span; } set { _map_point.span = value; } }
        public long obj_data { get { return m_ObjData; } set { m_ObjData = value; } }
        public long pt_data { get { return m_PtData; } set { m_PtData = value; } }
        public byte dir { get { return m_Dir; } set { m_Dir = value; } }
        public long sec { get { return m_Sec; } set { m_Sec = value; } }
        public bool forward { get { return m_forward; } set { m_forward = value; } }
        public string name { get { return m_Name; } set { m_Name = value; } }
        public string short_name { get { return m_ShortName; } set { m_ShortName = value; } }
        public long time { get { return m_Time; } set { m_Time = value; } }

        public long frame_number { get { return m_frame_number; } set { m_frame_number = value; } }
        

        static public int number_strings(object_type type)
        {
            if (type == object_type.Equipment)
                return 14;
            if (type == object_type.Object)
                return count_items;
            return 0;
        }

        public string[] to_strings(object_type type)
        {
            string[] res = null;
            if (type == object_type.Equipment)
            {
                res = new String[14];
                res[0] = name;// mo.Name;
                res[1] = picket + "+" + 0.01 * offset;
                res[2] = (sec >= 0) ? "" : "";
                res[3] = m_Type.ToString();
                res[4] = line + "-" + path;
                res[5] = (coord * 0.01).ToString();
                res[6] = code.ToString();
                res[7] = length + " см";
                res[8] = span.ToString();
                res[9] = obj_data.ToString();
                res[10] = pt_data.ToString();
                res[11] = dir.ToString();
                res[12] = (forward) ? "1" : "0";
                res[13] = frame_number.ToString();
            }

            if (type == object_type.Object)
            {
                res = new String[count_items];
                res[0] = m_Type.ToString();
                res[1] = line.ToString();
                res[2] = path.ToString();
                res[3] = picket.ToString();
                res[4] = offset.ToString();
                res[5] = coord.ToString();
                res[6] = code.ToString();
                res[7] = length.ToString();
                res[8] = name;
                res[9] = short_name;
                res[10] = span.ToString();
                res[11] = obj_data.ToString();
                res[12] = pt_data.ToString();
                res[13] = dir.ToString();
                res[14] = sec.ToString();
                res[15] = (sec >= 0) ? "" : "";
                res[16] = (forward) ? "1" : "0";
            
            }

            return res;
        }
    }
}
