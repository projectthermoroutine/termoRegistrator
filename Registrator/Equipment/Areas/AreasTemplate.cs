using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;

namespace Registrator
{
    public class AreasTemplate : DbObject
    {
        private String m_name;
        private List<Area> m_areas;
        private bool m_changed;

        public AreasTemplate() 
            : base()
        {
            m_areas = new List<Area>();
            m_changed = false;
        }

        public AreasTemplate(String name)
            : this()
        {
            m_name = name;
        }

        public String Name { get { return m_name; } set { m_name = value; } }
        public List<Area> Areas { get { return m_areas; } set { m_areas = value; } }
        public bool Changed { get { return m_changed; } set { m_changed = value; } }
 
    }
}
