using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    public class CTemperatureMeasure
    {
        public CTemperatureMeasure()
        {
            min = 0; max = 0; avg = 0; calibration_min = 0; calibration_max = 0;
            objTmin = 0; objTmax = 0; objTavg = 0;
        }
        public CTemperatureMeasure(float min,float max,float avg,float cal_min,float cal_max)
        {
            this.min = min; this.max = max; this.avg = avg; calibration_min = cal_min; calibration_max = cal_max;
            objTmin = min; objTmax = max; objTavg = avg;
        }
        public CTemperatureMeasure(
            float min, float max, float avg,
            float objTmin, float objTmax, float objTavg,
            float cal_min, float cal_max
            )
        {
            this.min = min; this.max = max; this.avg = avg; calibration_min = cal_min; calibration_max = cal_max;
            this.objTmin = objTmin; this.objTmax = objTmax; this.objTavg = objTavg;
        }

        public float min;
        public float max;
        public float avg;

        public float objTmin;
        public float objTmax;
        public float objTavg;

        public float calibration_min;
        public float calibration_max;
    }
}
