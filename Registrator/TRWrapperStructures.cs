using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;


namespace Registrator
{
    [StructLayout(LayoutKind.Sequential)]
    public struct area_temperature_measure
    {
        public float min;
        public float max;
        public float avr;
    };

    [StructLayout(LayoutKind.Sequential)]
    public struct area_temperature_measure_result
    {
        public uint area_id;
        public bool result_is_valid;
        public area_temperature_measure measure;
    };


}
