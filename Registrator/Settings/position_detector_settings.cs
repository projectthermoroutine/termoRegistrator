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
    public class position_detector_settings : FilterablePropertyBase, ICloneable
    {
        public object Clone()
        {
            return new position_detector_settings();
        }

        /// <summary>
        /// Конструктор
        /// </summary>
        public position_detector_settings()
        {
            _pd_ip = Properties.Settings.Default.pd_ip;
            _pd_i_ip = Properties.Settings.Default.pd_i_ip;
            _pd_port = Properties.Settings.Default.pd_port;
            _pd_events_ip = Properties.Settings.Default.pd_events_ip;
            _pd_i_events_ip = Properties.Settings.Default.pd_i_events_ip;
            _pd_events_port = Properties.Settings.Default.pd_events_port;
        }

        public static bool operator !=(position_detector_settings a, position_detector_settings b)
        {
            return !(a == b);
        }


        public static bool operator ==(position_detector_settings a, position_detector_settings b)
        {
            return a._pd_ip == b._pd_ip &&
                a._pd_port == b._pd_port &&
                a._pd_events_ip == b._pd_events_ip &&
                a._pd_events_port == b._pd_events_port &&
                a._pd_i_ip == b._pd_i_ip &&
                a._pd_i_events_ip == b._pd_i_events_ip;
        }

        private string _pd_ip;
        private string _pd_i_ip;
        private ushort _pd_port;
        private string _pd_events_ip;
        private string _pd_i_events_ip;
        private ushort _pd_events_port;

        /// <summary>
        /// IP адрес компьютера рабочего места
        /// </summary>
        [DisplayName("IP порт sync")]
        [Description("IP порт рассылки пакетов синхронизации")]
        [PropertyOrder(11)]
        [Category("Настройки приема координат")]
        public ushort pd_port
        {
            get { return _pd_port; }
            set { _pd_port = value; }
        }

        /// <summary>
        /// IP адрес компьютера рабочего места
        /// </summary>
        [DisplayName("IP адрес sync")]
        [Description("IP адрес рассылки пакетов синхронизации")]
        [PropertyOrder(10)]
        [Category("Настройки приема координат")]
        public string pd_ip
        {
            get { return _pd_ip; }
            set { _pd_ip = value; }
        }

        /// <summary>
        /// IP адрес компьютера рабочего места
        /// </summary>
        [DisplayName("IP адрес интерфейса sync")]
        [Description("IP адрес интерфейса, принимающего пакеты синхронизации")]
        [PropertyOrder(12)]
        [Category("Настройки приема координат")]
        [TypeConverter(typeof(IpInterfaceConverter))]
        public string pd_i_ip
        {
            get { return _pd_i_ip; }
            set { _pd_i_ip = value; }
        }



        /// <summary>
        /// IP адрес компьютера рабочего места
        /// </summary>
        [DisplayName("IP порт events")]
        [Description("IP порт рассылки пакетов событий проезда")]
        [PropertyOrder(21)]
        [Category("Настройки приема координат")]
        public ushort pd_events_port
        {
            get { return _pd_events_port; }
            set { _pd_events_port = value; }
        }


        /// <summary>
        /// IP адрес компьютера рабочего места
        /// </summary>
        [DisplayName("IP адрес events")]
        [Description("IP адрес рассылки пакетов событий проезда")]
        [PropertyOrder(20)]
        [Category("Настройки приема координат")]
        public string pd_events_ip
        {
            get { return _pd_events_ip; }
            set { _pd_events_ip = value; }
        }

        /// <summary>
        /// IP адрес компьютера рабочего места
        /// </summary>
        [DisplayName("IP адрес интерфейса events")]
        [Description("IP адрес интерфейса, принимающего пакеты событий проезда")]
        [Category("Настройки приема координат")]
        [PropertyOrder(22)]
        [TypeConverter(typeof(IpInterfaceConverter))]
        public string pd_i_events_ip
        {
            get { return _pd_i_events_ip; }
            set { _pd_i_events_ip = value; }
        }

    }

}
