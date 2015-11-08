using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{

    public class EquDbObject
    {
        int m_code = -1;
        String m_name = "";

        public EquDbObject()
            : base()
        {

        }

        public EquDbObject(int code, String name)
            
        {
            m_code = code;
            m_name = name;
        }

        public int Code  {  get  {  return m_code;   }  set  {  m_code = value;  }  }
 
        public String Name
        {
            get
            {
                return m_name;
            }

            set
            {
                m_name = value;
            }
        }
    }
}
