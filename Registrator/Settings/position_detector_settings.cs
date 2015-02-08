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
    public class position_detector_settings : FilterablePropertyBase
    {
        /// <summary>
        /// Конструктор
        /// </summary>
        public position_detector_settings()
        {
            _pd_ip = Properties.Settings.Default.pd_ip;
            _pd_port = Properties.Settings.Default.pd_port;
            _pd_events_ip = Properties.Settings.Default.pd_events_ip;
            _pd_events_port = Properties.Settings.Default.pd_events_port;
        }

        public override bool Equals(object obj)
        {
            position_detector_settings other = (position_detector_settings)obj;
            return _pd_ip == other._pd_ip &&
                _pd_port == other._pd_port &&
                _pd_events_ip == other._pd_events_ip &&
                _pd_events_port == other._pd_events_port;
        }

        private string _pd_ip;
        private ushort _pd_port;
        private string _pd_events_ip;
        private ushort _pd_events_port;

        /// <summary>
        /// IP адрес компьютера рабочего места
        /// </summary>
        [DisplayName("IP порт")]
        [Description("IP порт датчика положения")]
        [Category("IP порт датчика положения")]
        public ushort pd_port
        {
            get { return _pd_port; }
            set { _pd_port = value; }
        }

        /// <summary>
        /// IP адрес компьютера рабочего места
        /// </summary>
        [DisplayName("IP адрес")]
        [Description("IP адрес датчика положения")]
        [Category("IP адрес датчика положения")]
        public string pd_ip
        {
            get { return _pd_ip; }
            set { _pd_ip = value; }
        }


        /// <summary>
        /// IP адрес компьютера рабочего места
        /// </summary>
        [DisplayName("IP порт")]
        [Description("IP порт событий датчика положения")]
        [Category("IP порт событий датчика положения")]
        public ushort pd_events_port
        {
            get { return _pd_events_port; }
            set { _pd_events_port = value; }
        }


        /// <summary>
        /// IP адрес компьютера рабочего места
        /// </summary>
        [DisplayName("IP адрес")]
        [Description("IP адрес событий датчика положения")]
        [Category("IP адрес событий датчика положения")]
        public string pd_events_ip
        {
            get { return _pd_events_ip; }
            set { _pd_events_ip = value; }
        }

    }

}
