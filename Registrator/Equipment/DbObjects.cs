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

    enum equTypes {Equipment=0,Strelka=2 ,TrafficLight=3 }

    public class EquDbObject
    {
        int m_code = -1;
        String m_name = "";

        public EquDbObject()
            : base()
        {
            Parent = null;
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
            Parent = parent;
        }

        public int Code  {  get  {  return m_code;   }  set  {  m_code = value;  }  }
        public String Name { get { return m_name; } set { m_name = value; } }
        public EquDbObject Parent { get; set; }
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

    public class EquPath : EquDbObject
    {


        public EquPath(int code, String name, EquDbObject parent)
            : base(code, "Путь " + name,parent)
        { }
        public EquLine Line { get { return (EquLine)Parent; } set { Parent = value; } }
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
        public int keyNumber;
        public string npicket;
        public int LeftLineShift = 0;
        public int RightLineShift = 0;
        public PicketTag picketTag = PicketTag.Old;
        public int lenght { get; set; }

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
            lenght = lenght_;
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
