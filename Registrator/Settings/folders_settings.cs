using System;
using System.Collections.Generic;
using System.Windows.Forms.Design;
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
    public class folders_settings : FilterablePropertyBase//, ICloneable
    {
        public object Clone() 
        {
            return new folders_settings() as object;
        }
        /// <summary>
        /// Конструктор
        /// </summary>
        public folders_settings()
        {
            _root_dir_projects = Properties.Settings.Default.lastProjDir;
        }

        public static bool operator !=(folders_settings a, folders_settings b)
        {
            return !(a == b);
        }

        public static bool operator ==(folders_settings a, folders_settings b)
        {
            return a._root_dir_projects == b._root_dir_projects;
        }

        public override bool Equals(object obj)
        {
            return this == (folders_settings)obj;
        }

        public override int GetHashCode()
        {
            return projects_root_dir.GetHashCode();
        }

        private string _root_dir_projects;

        /// <summary>
        /// Смещение камеры относительно датчика положения (m)
        /// </summary>
        [DisplayName("Корневая папка для файлов проезда")]
        [Description("Корневая папка для файлов проезда")]
        [Category("Настройка папок")]
        [Editor(typeof(FolderNameEditor), typeof(UITypeEditor))]
        public string projects_root_dir
        {
            get { return _root_dir_projects; }
            set { _root_dir_projects = value; }
        }
    
    }

}
