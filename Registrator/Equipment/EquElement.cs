using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{

    


    public class EquElement
    {

        int m_id;

        String m_name;
        String m_short;
        String m_class;
        String m_group;
        String m_line;
        String m_path;
        

        public EquElement()
        {

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

        public String ShortName
        {

            get
            {
                return m_short;
            }

            set
            {
                m_short = value;
            }
        }

        public String Line
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

        public String Path
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

        public String Class
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

        public String Group
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

        public int Id
        {

            get
            {
                return m_id;
            }

            set
            {
                m_id = value;
            }

        }

    }
}
