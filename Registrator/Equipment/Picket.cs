using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
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
        public int LeftLineShift   = 0;
        public int RightLineShift  = 0;
        public PicketTag picketTag = PicketTag.Old;

        public EquPicket() : base()
        {

        }

        public EquPicket(int code, String name) : base(code, name)
        {

        }

        public EquPicket( String name, int code, string newPicketNum, int newPicketIndex, int _after, int _before, int LeftLineShiftArg, int RightLineShiftArg)
            : base(code, name)
        {
            number = newPicketIndex;
            npicket = newPicketNum;

            before = _before;
            after  = _after;

            LeftLineShift = LeftLineShiftArg;
            RightLineShift = RightLineShiftArg;
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
}
