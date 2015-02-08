using System;
using System.Collections.Generic;
using System.Text;

namespace Registrator
{
    class EquipmentState
    {
        
        private int m_type = 0;
        private String m_name;

        public EquipmentState()
        {

        }

        public int Type
        {

            get
            {
                return m_type;
            }

            set
            {
                m_type = value;
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
            }

        }


    }
}
