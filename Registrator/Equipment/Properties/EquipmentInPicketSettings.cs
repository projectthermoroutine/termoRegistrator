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

        public new void SetObjDB(EquTreeNode ObjectTreeNode)
        {
            EquObject equObject = ObjectTreeNode.ObjectDB as EquObject;
            EquPicket equPicket = (ObjectTreeNode.Parent as EquTreeNode).ObjectDB as EquPicket;

            code_equip = equObject.Code;
            PicketLenght = equPicket.lenght;

        }

    }
}
