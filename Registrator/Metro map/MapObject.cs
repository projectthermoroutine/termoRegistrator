using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    public enum MAPMEAS_TYPE
    {
        MAPMS_PICK,       // 1811, 1812 пикеты   !!!! =Firsr in this enum
        MAPMS_STATION,    // 1620       станции
        MAPMS_RZ,         // 1511       рельсовые цепи
        MAPMS_PKO,        // 1814       Пикетоотметчики
        MAPMS_GSTAT,      // 1621       Граница станции
        MAPMS_STOP1,      // 2510       Остановка 1-го вагона
        MAPMS_STRELKA,    // 1820       Стрелка
        MAPMS_SPATH,      // 1622       Станционный путь
        MAPMS_PPATH,      // 1623       Перегонный путь
        MAPMS_SEMAFOR,    // 1512       Светофор
        MAPMS_PICKBORDER, // 1813       Граница пикетажа
        MAPMS_STARTPOINT, // НАЧАЛО ПРОЕЗДА
        MAPMS_MEASPOINT,  // Начало измерений
        MAPMS_MEASFINISH, // Конец измерений
        MAPMS_LR,         // 1410 ЛР
        MAPMS_KP,         // 1420 КП
        MAPMS_PJA,        // 1440 ПЯ
        MAPMS_DT,         // 1501 ДТ
        MAPMS_NUM
    }

    public class MapObject
    {
        private MAPMEAS_TYPE m_type;  // тип объекта
        private int m_line;  // линия
        private int m_path;  // путь
        private int m_pick;  // пикет 
        private int m_offs;  // смещение (см)
        private int m_coord;     // смещение от начала маршрута, см
        private int m_code;      // код объекта по БД
        private int m_len;       // Длина, см
        private String m_name;   // название объекта
        private String m_shortName;
        private int m_layout;    // Положение (код перегона)
        private int m_objData;   // Данные объекта
        private int m_ptData;
        private int m_dir;                     //12 или 21
        private int m_sec = -1;     // относительное время прохождения этого объекта; не пройден = -1
        private String m_time;  // абсолютное время прохождения этого объекта
        private bool m_forward;    //вперед головой или хвостом

        
        public MapObject()
        {

        }

        public MapObject(String[] map)
        {

            if(map.Length < 17)
                return;

            m_type = (MAPMEAS_TYPE)Int32.Parse(map[0]);
            m_line = Int32.Parse(map[1]);
            m_path = Int32.Parse(map[2]);
            m_pick = Int32.Parse(map[3]); 
            m_offs = Int32.Parse(map[4]);
            m_coord = Int32.Parse(map[5]);
            m_code = Int32.Parse(map[6]);
            m_len = Int32.Parse(map[7]);
            m_name = map[8];
            m_shortName = map[9];
            m_layout = Int32.Parse(map[10]);
            m_objData = Int32.Parse(map[11]);
            m_ptData = Int32.Parse(map[12]);
            m_dir = Int32.Parse(map[13]);                     
            m_sec = Int32.Parse(map[14]);     
            m_time = map[15];
            m_forward = (Int32.Parse(map[16])==1) ? true : false;
        }

        public MAPMEAS_TYPE Type
        {
            get
            {
                return m_type; ;
            }

            set
            {
                m_type = value;
            }
        }

        public int Line
        {
            get
            {
                return m_line;
            }

            set
            {
                m_line = value;
            }

        }

        public int Path
        {
            get
            {
                return m_path;
            }

            set
            {
                m_path = value;
            }

        }

        public int Pick
        {
            get
            {
                return m_pick;
            }

            set
            {
                m_pick = value;
            }

        }

        public int Offset
        {
            get
            {
                return m_offs;
            }

            set
            {
                m_offs = value;
            }

        }

        public int Coord
        {
            get
            {
                return m_coord;
            }

            set
            {
                m_coord = value;
            }

        }

        public int Code
        {
            get
            {
                return m_code;
            }

            set
            {
                m_code = value;
            }

        }

        public int Len
        {
            get
            {
                return m_len;
            }

            set
            {
                m_len = value;
            }

        }

        public String Name
        {
            get
            {
                return m_name;
            }

            set
            {
                m_name = value;
            }

        }

        public String ShortName
        {
            get
            {
                return m_shortName;
            }

            set
            {
                m_shortName = value;
            }

        }

        public int Layout
        {
            get
            {
                return m_layout;
            }

            set
            {
                m_layout = value;
            }

        }

        public int ObjData
        {
            get
            {
                return m_objData;
            }

            set
            {
                m_objData = value;
            }

        }

        public int PtData
        {
            get
            {
                return m_ptData;
            }

            set
            {
                m_ptData = value;
            }

        }

        public int Dir
        {
            get
            {
                return m_dir;
            }

            set
            {
                m_dir = value;
            }

        }

        public int Sec
        {
            get
            {
                return m_sec;
            }

            set
            {
                m_sec = value;
            }

        }

        public String Time
        {
            get
            {
                return m_time;
            }

            set
            {
                m_time = value;
            }

        }

        public bool Forward
        {
            get
            {
                return m_forward;
            }

            set
            {
                m_forward = value;
            }

        }

    }
}
