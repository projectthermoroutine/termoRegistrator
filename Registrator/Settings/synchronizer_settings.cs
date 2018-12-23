using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;
using System.Drawing.Design;
using PropertyGridUtils;
using System.Diagnostics;
using System.Collections.Specialized;

namespace Registrator
{
    /// <summary>
    /// Данные для редактирования в PropertyGrid
    /// </summary>
    [TypeConverter(typeof(PropertySorter))]
    public class synchronizer_settings : FilterablePropertyBase, ICloneable
    {
        public object Clone()
        {
            return new synchronizer_settings();
        }

        /// <summary>
        /// Конструктор
        /// </summary>
        public synchronizer_settings()
        {
            _counter_size = Properties.Settings.Default.synchronizer_counter_size;
        }

        public static bool operator !=(synchronizer_settings a, synchronizer_settings b)
        {
            return !(a == b);
        }

        public static bool operator ==(synchronizer_settings a, synchronizer_settings b)
        {
            return a._counter_size == b._counter_size;
        }

        public override bool Equals(object obj)
        {
            return this == (synchronizer_settings)obj;
        }

        public override int GetHashCode()
        {
            return _counter_size;
        }

        private byte _counter_size;

        /// <summary>
        /// IP адрес компьютера рабочего места
        /// </summary>
        [DisplayName("Разрешение отсчетов синхронизатора (мм):")]
        [Description("Разрешение отсчетов синхронизатора")]
        [Category("Настройки синхронизатора")]
        public byte counter_size
        {
            get { return _counter_size; }
            set { _counter_size = value; }
        }
    }

}
