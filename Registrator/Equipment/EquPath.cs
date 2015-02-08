using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    public class EquPath : EquDbObject
    {
        private EquLine m_line;
        public EquPath()
            : base()
        {

        }

        public EquPath(int code, String name)
            : base(code, name)
        {

        }
        public EquLine Line
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

    }
}
