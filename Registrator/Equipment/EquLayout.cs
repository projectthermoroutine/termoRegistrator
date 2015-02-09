using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
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
