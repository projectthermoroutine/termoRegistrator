using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    public class FrameObject
    {
        int m_width = 0;
        int m_height = 0;
        byte[] m_pixels = null;

        public FrameObject()
            : base()
        {

        }

        public int Width
        {

            get
            {
                return m_width;
            }

            set
            {
                m_width = value;
            }

        }

        public int Height
        {

            get
            {
                return m_height;
            }

            set
            {
                m_height = value;
            }

        }

        public byte[] Pixels
        {

            get
            {
                return m_pixels;
            }

            set
            {
                m_pixels = value;
            }

        }

    }
}
