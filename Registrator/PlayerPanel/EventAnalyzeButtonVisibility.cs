using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    public class EventAnalyzeButtonVisibility: EventArgs
    {
        private bool m_analyzeButtonEnable;
        public EventAnalyzeButtonVisibility(bool analyzeButtonEnable)
        {
            m_analyzeButtonEnable = analyzeButtonEnable;
        }
        public bool analyzeButtonEnable { get { return m_analyzeButtonEnable; } }
    }
}
