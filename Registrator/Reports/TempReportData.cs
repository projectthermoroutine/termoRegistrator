using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Drawing;
using System.Windows.Media.Imaging;
using System.Windows.Media;

namespace Registrator
{
    public class TempReportData
    {
        int m_number = 1;
        DateTime m_date;
        float m_tempMin = 0;
        float m_tempAvr = 0;
        float m_tempMax = 0;
        MemoryStream ms = new MemoryStream();
        String m_state = "Исправно";

        public TempReportData() 
            : base()
        {        }

        //public int Number { get { return m_number; } set { m_number = value; } }
        public DateTime Date { get { return m_date; } set { m_date = value; } }
        public float TempMin { get { return m_tempMin; } set { m_tempMin = value; }}
        public double DeltaTemperature { get; set;  }
        public float TempAvr { get{ return m_tempAvr; } set { m_tempAvr = value; } }
        public float TempMax { get { return m_tempMax; } set {m_tempMax = value; }}
        public String State { get { return m_state; } set { m_state = value;  } }
        byte[] m_raster;
        public byte[] Picture 
        { 
             get { return m_raster; }
             set {
                    int width  = 640;
                    int height = 480;
                             
                    PixelFormat pf = PixelFormats.Bgr32;

                    int rawStride = (width * pf.BitsPerPixel + 7) / 8;
           
                    BitmapSource bs = BitmapSource.Create(width, height, 96, 96, pf, null, value, rawStride);

                    using (var memStream = new MemoryStream())
                    {
                        BitmapEncoder encoder = new PngBitmapEncoder();
                        encoder.Frames.Add(BitmapFrame.Create(bs));
                        encoder.Save(memStream);
                        m_raster = memStream.ToArray();
                    }
             }
        }
    }
}
