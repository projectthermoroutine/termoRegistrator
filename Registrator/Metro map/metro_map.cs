using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    using map_objects_list = List<measure_object>;
    public class metro_map
    {
        public void LoadObjectsFromDb() { }
        public int GetMapObjectsCount() { return 0; }
        public int GetEquListCount() { return 0; }
        public int MaxEquNum() { return 0; }

        public bool GetEquipmentElement(short elNum, uint filter, out measure_object element)
        {
            element = new measure_object();
            return false;
        }
        public bool GetMeasureObject(short objNum, out measure_object element)
        {
            element = new measure_object();
            return false;
        }

        public map_objects_list LoadEquipmentList()
        {
            return null;
        }
        public List<measure_object> GetMapObjectsFromFile(string filePath)
        {
            return new List<measure_object>();
        }
    }


    public class map_point
    {
        public map_point()
        {
            m_Line = 0;
            m_Path = 0;
            m_Picket = 0;
            m_Offset = 0;
            m_Coord = 0;
            m_Span = 0;
        }
        public map_point(short Path, short Line, long Coord)
            : this()
        {
            m_Line = Line;
            m_Path = Path;
            m_Coord = Coord;
        }
  
        short m_Line;
        short m_Path;
        long m_Picket;
        long m_Offset;
        long m_Coord;
        ulong m_Span;
        public short line { get { return m_Line; } set { m_Line = value; } }
        public short path { get { return m_Path; } set { m_Path = value; } }
        public long picket { get { return m_Picket; } set { m_Picket = value; } }
        public long offset { get { return m_Offset; } set { m_Offset = value; } }
        public long coord { get { return m_Coord; } set { m_Coord = value; } }
        public ulong span { get { return m_Span; } set { m_Span = value; } }
    }

    public class map_point_info
    {
        public map_point map_point;
        public int distance;
        public String peregon;

        public bool get_info(map_point map_point)
        {
            this.map_point = map_point;
            distance = 0;
            peregon = this.map_point.span.ToString();

            return true;
        }
    }


    public class map_object_info : map_point_info
    {
        public String msec;
        public String objName;
 
        public bool get_info(measure_object map_object)
        {
            get_info(map_object.map_point);
            msec = "00:00:00";
            objName = map_object.name;
            return true;
        }
    }


}
