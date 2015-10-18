using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    public class EventPlayerChangeMode: EventArgs
    {
        private PlayerPanel.PlayerMode m_mode;
        public EventPlayerChangeMode(PlayerPanel.PlayerMode mode)
        {
            m_mode = mode;
        }
        public PlayerPanel.PlayerMode Mode { get { return m_mode; } }
    }
}
