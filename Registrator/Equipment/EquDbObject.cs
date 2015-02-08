using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{

    public class EquDbObject : EquTreeNode
    {

        int m_code = -1;
        String m_name = "";
        String m_sname = "";
        private EquGroup m_group;
        
        public EquGroup Group
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
        
        public EquDbObject()
            : base()
        {

        }

        public EquDbObject(int code, String name)
            : base(name)
        {
            m_code = code;
            m_name = name;
            m_sname = name;
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

        public String Name
        {

            get
            {
                return m_name;
            }

            set
            {
                m_name = value;
                m_sname = m_name;
            }

        }

        public String ShortName
        {

            get
            {
                return m_sname;
            }

            set
            {
                m_sname = value;
            }

        }

    }
}
