using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{

    public class db_object_helper
    {
        public static T Parent<T>(EquDbObject db_object) where T:EquDbObject
        {
            return db_object.Parent == null ? null : (T)db_object.Parent;
        }
    }

    public enum EQUIPS_TYPES {Equipment=0, Strelka=2, TrafficLight=3 }
    enum EQUIPS_WORKS_STATE {off=0, on }
    enum STRELKA_DIRECT {Left=0,Right }
    enum AREA_TYPE {trafficLight=0 }

    public class EquDbObject
    {
        protected int m_code = -1;
        protected String m_name = "empty";

        EquDbObject _parent;

        public EquDbObject()
            : base()
        {
            _parent = null;
        }
        public EquDbObject(int code, String name):
            this()
        {
            m_code = code;
            m_name = name;
        }
        public EquDbObject(int code, String name, EquDbObject parent) :
            this(code,name)
        {
            _parent = parent;
        }

        public int Code  {  get  {  return m_code;   }  set  {  m_code = value;  }  }
        public String Name { get { return m_name; } set { m_name = value; } }
        public EquDbObject Parent { get { if (_parent == null) { return new EquDbObject(); } else { return _parent; } } set { _parent = value; } }
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
        public EquGroup()
            : base()
        {}

        public EquGroup(int code, String name, EquDbObject parent)
            : base(code, name,parent)
        { }
        public EquClass Class { get { return (EquClass)Parent; } set { Parent = value; } }
    }

    public class EquLineNULL : EquLine
    {
        public EquLineNULL()
            : base(-1,"empty", new EquDbObject())
        {

        }
    }


    public class EquLine : EquDbObject
    {
        string m_LineCode;

        public EquLine(int code, String name, EquDbObject parent)
            : base(code, "Линия " + name, parent)
        {
            m_LineCode = name;
        }
      
        public string LineCode { get { return m_LineCode; } set { m_LineCode = value; } }
        public EquGroup Group { get { return (EquGroup)Parent; } set { Parent = value; } }
    }

    public class EquPathNULL : EquPath
    {
        public EquPathNULL()
            : base(0, "empty", new EquDbObject())
        {
            
        }

    }



    public class EquPath : EquDbObject
    {
        public EquPath(int code, String name, EquDbObject parent)
            : base(code, "Путь " + name,parent)
        {

        }

        public EquLine Line { get { return (EquLine)Parent; } set { Parent = value; } }
    }

    public enum PicketTag
    {
        New,
        Old
    }

    public class EquPicketNULL : EquPicket
    {
        public EquPicketNULL()
            :base()
        { }
    }


    public class EquPicket : EquDbObject, IEquatable<EquPicket>
    {
        public int before;
        public int after;
        public int keyNumber;
        public string npicket;
        public int LeftLineShift = 0;
        public int RightLineShift = 0;
        public PicketTag picketTag = PicketTag.Old;
        public int Length { get; set; }

        public EquPicket()
            : base()
        {}

        public EquPicket(int code, String name, EquDbObject parent)
            : base(code, name,parent)
        { }

        public EquPicket(
            string newPicketNum, int newPicketIndex,
            int _after, int _before,
            int LeftLineShiftArg, int RightLineShiftArg,
            int lenght_,
            EquDbObject parent
            )
            : base(newPicketIndex, "Пикет " + newPicketNum, parent)
        {
            init(newPicketNum, newPicketIndex, _after, _before, LeftLineShiftArg, RightLineShiftArg, lenght_);
        }
        private void init(string newPicketNum, int newPicketIndex, int _after, int _before, int LeftLineShiftArg, int RightLineShiftArg, int lenght_)
        {
            keyNumber = newPicketIndex;
            npicket = newPicketNum;

            before = _before;
            after = _after;

            LeftLineShift = LeftLineShiftArg;
            RightLineShift = RightLineShiftArg;
            Length = lenght_;
        }
        public EquPath Path { get { return (EquPath)Parent; } set { Parent = value; } }

        public bool Equals(EquPicket p)
        {
            if (p == null)
                return false;

            return keyNumber == p.keyNumber;
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
        public EquLayout()
            : base()
        {}

        public EquLayout(int code, String name)
            : base(code, name)
        {}
        public EquLayout(int code, String name, EquDbObject parent)
            : base(code, name,parent)
        { }
        public EquPath Path { get { return (EquPath)Parent; } set { Parent = value; } }
    }



}
