using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{

    enum equTypes {Equipment=0,Strelka=2 ,TrafficLight=3 }

    public class EquDbObject
    {
        int m_code = -1;
        String m_name = "";

        public EquDbObject()
            : base()
        {}
        public EquDbObject(int code, String name)
        {
            m_code = code;
            m_name = name;
        }

        public int Code  {  get  {  return m_code;   }  set  {  m_code = value;  }  }
        public String Name { get { return m_name; } set { m_name = value; } }
    }

    public class EquClass : EquDbObject
    {
        public EquClass()
            : base()
        {}

        public EquClass(int code, String name)
            : base(code, name)
        {}
    }

    public class EquGroup : EquDbObject
    {
        private EquClass m_class;
        public EquGroup()
            : base()
        {}

        public EquGroup(int code, String name)
            : base(code, name)
        {}
        public EquClass Class { get { return m_class; } set { m_class = value; } }
    }

    public class EquLine : EquDbObject
    {
        long m_offsetLineCoordinate;
        string m_LineCode;

        public EquLine(int code, String name, string lineCode, long offsetLineCoordinate)
            : base(code, name)
        {
            m_LineCode = lineCode;
            m_offsetLineCoordinate = offsetLineCoordinate;
        }
        public string LineCode { get { return m_LineCode; } set { m_LineCode = value; } }
        public long OffsetLineCoordinate { get { return m_offsetLineCoordinate; } set { m_offsetLineCoordinate = value; } }
    }

    public class EquPath : EquDbObject
    {
        public EquPath(int code, String name)
            : base(code, name)
        { }
    }

    public enum PicketTag
    {
        New,
        Old
    }
    public class EquPicket : EquDbObject, IEquatable<EquPicket>
    {
        public int before;
        public int after;
        public int number;
        public string npicket;
        public int LeftLineShift = 0;
        public int RightLineShift = 0;
        public PicketTag picketTag = PicketTag.Old;
        public int lenght = 0;

        public EquPicket()
            : base()
        {}

        public EquPicket(int code, String name)
            : base(code, name)
        {}

        public EquPicket(String name, int code, string newPicketNum, int newPicketIndex, int _after, int _before, int LeftLineShiftArg, int RightLineShiftArg, int lenght_)
            : base(code, name)
        {
            number = newPicketIndex;
            npicket = newPicketNum;

            before = _before;
            after = _after;

            LeftLineShift = LeftLineShiftArg;
            RightLineShift = RightLineShiftArg;
            lenght = lenght_;
        }
        public bool Equals(EquPicket p)
        {
            if (p == null)
                return false;

            return number == p.number;
        }

        public override bool Equals(object obj)
        {
            return base.Equals(obj as EquPicket);
        }

        public override int GetHashCode()
        {
            return Name.GetHashCode();
        }
    }


    public class EquLayout : EquDbObject
    {
        private EquPath m_path;
        public int beforePeregon;
        public int aftrerPeregon;
        public EquLayout()
            : base()
        {

        }

        public EquLayout(int code, String name)
            : base(code, name)
        {

        }
        public EquPath Path
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
    }



}
