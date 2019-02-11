using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;
using System.Globalization;
using Registrator.Equipment.CreateDbObjectsCtrls;
using Registrator.Equipment.Properties;

namespace Registrator.Equipment
{
    public class EquipmentInPicketSettings : TemperatureSetting
    {
        public EquipmentInPicketSettings(DB.metro_db_controller db_controller)
            : base(db_controller)
        {
        }
    }
}
