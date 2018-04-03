using Registrator.Equipment.CreateDbObjectsCtrls;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Registrator.Equipment.Properties.Classes
{
    public class TrafficLightClassesSettings : EquipmentsClassesCommonSettings
    {
        public TrafficLightClassesSettings(DB.metro_db_controller controller)
            :base(controller)
        {

        }
    }
}
