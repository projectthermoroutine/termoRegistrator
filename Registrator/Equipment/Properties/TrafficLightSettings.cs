using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Registrator.Equipment.Properties
{
    public class TrafficLightSettings: EquipmentInPicketCommonSettings
    {
        public TrafficLightSettings(DB.metro_db_controller db_controller)
            :base(db_controller)
        {

        }
    }
}
