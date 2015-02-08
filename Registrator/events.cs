using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    public class MapReceivedEvent : EventArgs
    {
        String m_file;
        public MapReceivedEvent(String file)
            : base()
        {
            m_file = file;
        }

        public String File { get { return m_file; } set { m_file = value; } }
    }

    public class CoordEvent : EventArgs
    {
        float m_km = 0;
        uint m_msec = 0;
        public CoordEvent(float km, uint msec)
            : base()
        {
            m_km = km;
            m_msec = msec;
        }

        public float Km { get { return m_km; } set { m_km = value; } }
        public uint Msec { get { return m_msec; } set { m_msec = value; } }
    }

    public class StartStopEvent : EventArgs
    {
    }
}
