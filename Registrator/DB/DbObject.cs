using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    public class DbObject
    {
        private long m_dbId = -1;
        public long DbId { get { return m_dbId; } set { m_dbId = value; } }
 
    }
}
