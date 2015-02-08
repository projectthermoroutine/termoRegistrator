using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{

     public class irb_frame_header
    {
        public ushort width = 0;
        public ushort height = 0;
        public ushort firstValidX = 0;
        public ushort lastValidX = 0;
        public ushort firstValidY = 0;
        public ushort lastValidY = 0;

        public float calibration_min = 0;
        public float calibration_max = 0;

    }

    public class irb_frame_helper
    {
        public irb_frame_header header = new irb_frame_header();
        public CTemperatureMeasure temperature_measure = new CTemperatureMeasure();
        public double timestamp = 0.0;
        public UInt32 id = 0;

        public ushort[] pixels = null;
        public float[] temp_values = null;

        public bool is_position_valid(ushort x,ushort y)
        {
            return header.firstValidX <= x &&
                   header.lastValidX >= x &&
                   header.firstValidY <= y &&
                   header.lastValidY >= y;
        }

        public bool get_pixel_temperature(ushort x, ushort y, out float tempToReturn)
        {
            tempToReturn = 0;
            if (x >= header.width || y >= header.height) return false;

            ushort pixel = pixels[header.width * y + x];

            byte hiByte = (byte)(pixel >> 8);
            byte loByte = (byte)(pixel & 0xFF);

            float Temp1 = temp_values[hiByte];
            float Temp2 = temp_values[hiByte + 1];

            tempToReturn = (Temp1 + ((Temp2 - Temp1) * (float)loByte / 256));

            return true;
        }
        public float get_pixel_temperature(ushort pixel)
        {
            byte hiByte = (byte)(pixel >> 8);
            byte loByte = (byte)(pixel & 0xFF);

            float Temp1 = temp_values[hiByte];
            float Temp2 = temp_values[hiByte + 1];

            return (Temp1 + ((Temp2 - Temp1) * (float)loByte / 256));
        }


        ushort _max_temperature_pixel;
        ushort _min_temperature_pixel;
        public float min_temperature, max_temperature, avr_temperature;
		bool _temperature_span_calculated = false;
        public void reset_measure() { _temperature_span_calculated = false; }

		public bool is_temperature_span_calculated() { return _temperature_span_calculated; }
        public ushort get_min_temperature_pixel() { return _min_temperature_pixel; }
        public ushort get_max_temperature_pixel() { return _max_temperature_pixel; }

        public bool Extremum(float [] temp_vals = null)
        {

            ushort maxw = 0;
            ushort minw = 65535;
            float avg_temp = 0;

            var firstY = header.firstValidY;
            var lastY = header.lastValidY;
            var firstX = header.firstValidX;
            var lastX = header.lastValidX;

            for (var y = firstY; y <= lastY; y++)
            {
                for (int x = firstX; x <= lastX; x++)
                {
                    var pixel = pixels[header.width * y + x];
                    if (maxw < pixel)
                    {
                        maxw = pixel;
                        _max_temperature_pixel = pixel;
                    }
                    if (pixel < minw)
                    {
                        minw = pixel;
                        _min_temperature_pixel = pixel;
                    }

                    if (temp_vals != null)
                    {
                        var pixel_temp = get_pixel_temperature(pixel);
                        temp_vals[header.width * y + x] = pixel_temp;
                        avg_temp += pixel_temp;
                    }

                }
            }

            if (temp_vals != null)
            {
                avr_temperature = (float)(avg_temp / ((lastX - firstX + 1) * (lastY - firstY + 1))) - 273.15f;
            }


            byte hiByte = (byte)(maxw >> 8);
            byte loByte = (byte)(maxw & 0xFF);

            float Temp1 = temp_values[hiByte];
            float Temp2 = temp_values[hiByte + 1];

            float dTemp = Temp2 - Temp1;
            float up1 = dTemp * (float)loByte;

            float temp = Temp1 + up1 / 256 - (float)273.15;
            max_temperature = temp;

            hiByte = (byte)(minw >> 8);
            loByte = (byte)(minw & 0xFF);

            Temp1 = temp_values[hiByte];
            Temp2 = temp_values[hiByte + 1];

            dTemp = Temp2 - Temp1;
            up1 = dTemp * (float)loByte;

            temp = Temp1 + up1 / 256 - (float)273.15;
            min_temperature = temp;

            _temperature_span_calculated = true;
            return true;
        }


    }
}
