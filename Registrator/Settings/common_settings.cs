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
    public class common_settings : FilterablePropertyBase, ICloneable
    {

        public object Clone() 
        {
            return new common_settings() as object;
        }
        /// <summary>
        /// Конструктор
        /// </summary>
        public common_settings()
        {
            _camera_offset = Properties.Settings.Default.camera_offset;
            _max_frames_in_file = Properties.Settings.Default.maxFramesInFile;
        }

        public static bool operator !=(common_settings a, common_settings b)
        {
            return !(a == b);
        }

        public static bool operator ==(common_settings a, common_settings b)
        {
            return a._camera_offset == b._camera_offset &&
                    a._max_frames_in_file == b._max_frames_in_file;
        }

        private long _camera_offset;

        /// <summary>
        /// Смещение камеры относительно датчика положения (mm)
        /// </summary>
        [DisplayName("Смещение камеры (м):")]
        [Description("Смещение камеры относительно датчика положения")]
        [Category("Параметры оборудования")]
        public long camera_offset
        {
            get { return _camera_offset; }
            set { _camera_offset = value; }
        }

        private ushort _max_frames_in_file;
        /// <summary>
        /// Максимальное количество фреймов в файле при записи проезда
        /// </summary>
        [DisplayName("Количество фреймов в файле:")]
        [Description("Максимальное количество фреймов в файле при записи проезда")]
        [Category("Параметры записи")]
        public ushort max_frames_in_file
        {
            get { return _max_frames_in_file; }
            set { _max_frames_in_file = value; }
        }
    
    
    }

}
