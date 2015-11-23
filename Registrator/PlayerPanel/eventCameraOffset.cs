using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    public class eventCameraOffset : EventArgs
    {
        private int m_offset;
        private bool m_save;
        private bool m_checked;
        public eventCameraOffset(int shift, bool save)
        {
            m_offset = shift;
            m_save = save;
        }

        public eventCameraOffset(bool checked_arg)
        {
            m_checked = checked_arg;
        }

        public bool _checked
        {
            get { return m_checked; }
            set { m_checked = value; }
        }

        public int offset 
        {
            get { return m_offset; }
            set { m_offset = value; }
        }
        public bool save
        {
            get { return m_save; }
            set { m_save = value; }
        }
    }
}
