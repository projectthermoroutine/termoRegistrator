using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Registrator.Equipment.Properties
{

    public class EquipmentSettings: EquipmentsCommonSettings
    {
        public EquipmentSettings(DB.metro_db_controller db_controller)
            : base(db_controller)
        {

        }

       
    }


    public abstract class EquipmentSetObject
    {
        protected int code_equip = 0;
        public void SetObjDB(EquTreeNode ObjectTreeNode)
        {
            EquDbObject equObject = ObjectTreeNode.ObjectDB as EquDbObject;

            code_equip = equObject.Code;
        }
    }
}
