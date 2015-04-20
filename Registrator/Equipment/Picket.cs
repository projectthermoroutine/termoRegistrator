using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    public class Picket  : EquDbObject
    {
        //private EquLayout m_layout;
        public int before;
        public int after;
        public int number;
        public Picket() : base()
        {

        }

        public Picket(int code, String name) : base(code, name)
        {

        }
        //public EquLayout Layout
        //{
        //    get
        //    {
        //        return m_layout;
        //    }

        //    set
        //    {
        //        m_layout = value;
        //    }
        //}
    }
}
