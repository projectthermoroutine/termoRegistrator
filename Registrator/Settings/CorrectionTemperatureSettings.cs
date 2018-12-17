using System;
using System.ComponentModel;
using PropertyGridUtils;

namespace Registrator
{
    /// <summary>
    /// Данные для редактирования в PropertyGrid
    /// </summary>
    [TypeConverter(typeof(PropertySorter))]
    public class CorrectionTemperatureSettings : FilterablePropertyBase, ICloneable
    {

        public object Clone()
        {
            return new CorrectionTemperatureSettings() as object;
        }
        /// <summary>
        /// Конструктор
        /// </summary>
        public CorrectionTemperatureSettings()
        {
            CorrectionFactor = Properties.Settings.Default.CorrectionTemperatureFactor;
            CorrectionOffset = Properties.Settings.Default.CorrectionTemperatureOffset;
            CorrectionEnable = Properties.Settings.Default.UseCorrectionTemperature;
        }

        public static bool operator !=(CorrectionTemperatureSettings a, CorrectionTemperatureSettings b)
        {
            return !(a == b);
        }

        public static bool operator ==(CorrectionTemperatureSettings a, CorrectionTemperatureSettings b)
        {
            return a.CorrectionEnable == b.CorrectionEnable && 
                    a.CorrectionFactor == b.CorrectionFactor &&
                    a.CorrectionOffset == b.CorrectionOffset;
        }

        public override bool Equals(object obj)
        {
            return this == (CorrectionTemperatureSettings)obj;
        }

        public override int GetHashCode()
        {
            return CorrectionEnable ? Convert.ToInt32(CorrectionFactor + CorrectionOffset) : 0;
        }


        /// <summary>
        /// Включить коррекцию температуры
        /// </summary>
        [DisplayName("Включить коррекцию")]
        [Description("Включить коррекцию")]
        [Category("Параметры коррекции")]
        public bool CorrectionEnable { get; set; }

        /// <summary>
        /// Коэффициент коррекции температуры
        /// </summary>
        [DisplayName("Коэффициент коррекции:")]
        [Description("Коэффициент коррекции")]
        [Category("Параметры коррекции")]
        public float CorrectionFactor { get; set; }

        /// <summary>
        /// Постоянная смещения температуры
        /// </summary>
        [DisplayName("Постоянная смещения:")]
        [Description("Постоянная смещения")]
        [Category("Параметры коррекции")]
        public float CorrectionOffset { get; set; }

    }

}
