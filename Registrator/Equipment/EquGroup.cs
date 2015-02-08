using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    public class EquGroup : EquDbObject
    {

        private EquClass m_class;


        public EquGroup()
            : base()
        {

        }

        public EquGroup(int code, String name)
            : base(code, name)
        {

        }

        public EquClass Class
        {
            get
            {
                return m_class;
            }

            set
            {
                m_class = value;
            }
        }

    }
}
