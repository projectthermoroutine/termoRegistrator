#define RASTER_FROM_TEMP_VALS_ON

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;



namespace Registrator
{
    public class CPalette
    {
		public Int32[] image;
		public uint numI;
    }

	public enum IMAGE_CALIBRATION_TYPE
	{
		NONE,
		MIN_MAX,
		AVERAGE,
		MANUAL
	};


    public sealed class temp_interval
    {
        public temp_interval(float min, float max)
        {
            this.min = min; this.max = max;
        }
        public float min;
        public float max;
    }

    public class irb_frame_image_helper
    {
        public irb_frame_image_helper()
        {
            _calibration_type = IMAGE_CALIBRATION_TYPE.MIN_MAX;
            _width = 0; _height = 0;
            allocate_temp_vals(1024, 768);
            _calibration_interval = new temp_interval(0, 50);
            _temperature_span = new temp_interval(0, 0);
            _real_span = new temp_interval(0, 0);
        }
        void allocate_temp_vals(ushort width, ushort height)
        {
            if (_width != width || _height != height)
            {
                _width = width; _height = height;
                _temp_vals = new float[_width * _height];
            }
        }


        float[] _temp_vals;
        ushort _width;
        ushort _height;

        IMAGE_CALIBRATION_TYPE _calibration_type;
        public void set_calibration_type(IMAGE_CALIBRATION_TYPE type) { _calibration_type = type; }

        CPalette _palette = new CPalette();

        public CPalette Palette { get { return _palette; } set { _palette = value; } }

        temp_interval _calibration_interval;
        temp_interval _temperature_span;
        temp_interval _real_span;

        public void SetPaletteCalibration(float Minimum, float Maximum)
        {
            _calibration_interval.min = Minimum;
            _calibration_interval.max = Maximum;
        }
        void get_calibration_interval(irb_frame_helper frame, temp_interval temperature_span,out float scale,out int offset)
        {
            offset = 0;
            if (!frame.is_temperature_span_calculated())
                frame.Extremum(_temp_vals);
            switch (_calibration_type)
            {
                case IMAGE_CALIBRATION_TYPE.NONE:
                    temperature_span.min = frame.header.calibration_min;
                    temperature_span.max = frame.header.calibration_max;
                    scale = (float)_palette.numI / (temperature_span.max - temperature_span.min);
                    return;
                //break;
                case IMAGE_CALIBRATION_TYPE.MIN_MAX:


                    temperature_span.min = frame.min_temperature;
                    temperature_span.max = frame.max_temperature;
                    scale = (float)_palette.numI / (temperature_span.max - temperature_span.min);
                    return;

//                    break;
                case IMAGE_CALIBRATION_TYPE.AVERAGE:
                    {
                        var high_temp_delta = frame.max_temperature - frame.avr_temperature;
                        var low_temp_delta = frame.avr_temperature - frame.min_temperature;
                        var delta_delta = low_temp_delta - high_temp_delta;
                        var min_delta = high_temp_delta;
                        if (high_temp_delta > low_temp_delta)
                        {
                            delta_delta = high_temp_delta - low_temp_delta;
                            min_delta = low_temp_delta;
                        }
                        if (delta_delta > 2 * min_delta)
                        {
                            if (high_temp_delta > low_temp_delta)
                            {
                                temperature_span.min = frame.min_temperature;
                                temperature_span.max = frame.avr_temperature + low_temp_delta;
                            }
                            else
                            {
                                temperature_span.min = frame.avr_temperature - high_temp_delta;
                                temperature_span.max = frame.max_temperature;
                            }

                            break;
                        }

                        temperature_span.min = frame.min_temperature;
                        temperature_span.max = frame.max_temperature;
                        break;
                    }
                case IMAGE_CALIBRATION_TYPE.MANUAL:
                    temperature_span = _calibration_interval;
                    scale = (float)_palette.numI / (temperature_span.max - temperature_span.min);
                    return;

                    //break;
                default:
                    break;
            }

		scale = 0.0f;
		var span = temperature_span.max - temperature_span.min;
		if (span == 0)
			return;
		var coeff = (float)_palette.numI / span;
		const float max_coeff = 8.0f;
		if (coeff > max_coeff)
		{
			scale = max_coeff;
			offset = (int)((frame.avr_temperature - temperature_span.min)*(coeff - max_coeff));
		}
		else
		{
			scale = coeff;
		}

        }

        public bool get_formated_frame_raster(irb_frame_helper frame, 
                                            areas_dispatcher areas_dispatcher,
                                            temp_interval calibration_interval,
                                            out byte[] raster
            )
        {

            float pallete_color_coefficient;
            int index_offset;
            get_calibration_interval(frame, _temperature_span,out pallete_color_coefficient,out index_offset);

            calibration_interval.max = _temperature_span.max;
            calibration_interval.min = _temperature_span.min;

#if RASTER_FROM_TEMP_VALS_ON
            //var temp_span = _temperature_span.max - _temperature_span.min;
            //if (temp_span != 0)
            //{
            //    pallete_color_coefficient = (float)_palette.numI / temp_span;
            //}
#else
            ushort fromw = frame.get_min_temperature_pixel();
            ushort tow = frame.get_max_temperature_pixel();
		var pixels_span = tow - fromw;
		float dttDASHdw = 0.0f;
		if (pixels_span != 0){
			dttDASHdw = (_temperature_span.max - _temperature_span.min) / pixels_span;
			pallete_color_coefficient = (float)_pallete.numI / pixels_span;
		}
#endif

            var firstY = frame.header.firstValidY;
            var lastY = frame.header.lastValidY;
            var firstX = frame.header.firstValidX;
            var lastX = frame.header.lastValidX;
            uint number_valid_pixels = (uint)((lastX - firstX + 1) * (lastY - firstY + 1));

            int offset = 0;
            double avrw = 0;

            var data = new byte[frame.header.width * frame.header.height * 4];
            if (areas_dispatcher != null)
                areas_dispatcher.set_default_areas();


            for (int y = firstY; y <= lastY; y++)
            {
                for (int x = firstX; x <= lastX; x++)
                {
#if RASTER_FROM_TEMP_VALS_ON
                    var pixel_temp = _temp_vals[frame.header.width * y + x];
                    float curTemp = pixel_temp - 273.15f;

                    float temp_for_index = curTemp;
                    if (temp_for_index > _temperature_span.max)
                        temp_for_index = _temperature_span.max;
                    else if (temp_for_index < _temperature_span.min)
                        temp_for_index = _temperature_span.min;


                    int pallete_color_index = (int)(pallete_color_coefficient * (temp_for_index - _temperature_span.min)) + index_offset;

#else
                var pixel = frame.pixels[frame.header.width * y + x];
				unsigned int dt = pixel - fromw;
				int pallete_color_index = (int)(pallete_color_coefficient * dt);
				float curTemp = dt * (float)dttDASHdw + temperature_span.min;
#endif

                    if (areas_dispatcher != null && !areas_dispatcher.Empty())
                    {
                        var area = areas_dispatcher.getArea((short)x, (short)y);
                        if (area != null)
                        {
                            area.SetTemp(curTemp);
                        }
                    }


                    avrw += curTemp;// - 273.15;

                    if (pallete_color_index > _palette.numI - 1)
                        pallete_color_index = (int)_palette.numI - 1;

                    if (pallete_color_index < 0)
                        pallete_color_index = 0;

                    var color = _palette.image[pallete_color_index];
                    data[offset] = (byte)(color);
                    data[offset + 1] = (byte)(color >> 8);
                    data[offset + 2] = (byte)(color >> 16);
                    data[offset + 3] = (byte)(color >> 24);
                    offset += 4;
                }
            }

            if (number_valid_pixels > 0)
                frame.avr_temperature = (float)(avrw / number_valid_pixels);

            raster = (byte[])data;

            return true;
        }

    }


}
