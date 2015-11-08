using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    public class EquLine : EquDbObject
    {
        long m_offsetLineCoordinate;
        string  m_LineCode;

        public EquLine(int code, String name, string lineCode, long offsetLineCoordinate)
        : base(code, name)
        {
            m_LineCode = lineCode;
            m_offsetLineCoordinate = offsetLineCoordinate;
        }
        public string LineCode { get { return m_LineCode; } set { m_LineCode = value; } }
        public long OffsetLineCoordinate {  get { return m_offsetLineCoordinate; }  set { m_offsetLineCoordinate = value; }  }
    }
}
