using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    public class EquLine : EquDbObject
    {
        private EquGroup m_group;

        public string LineCode;

        public EquLine()
        : base()
        {

        }

        public EquLine(int code, String name)
        : base(code, name)
        {
            
        }
        public EquGroup Line
        {
            get
            {
                return m_group;
            }

            set
            {
                m_group = value;
            }
        }
    }
}
