using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Registrator.Equipment
{
    public partial class Properties : ToolWindow
    {
        public Equipment.LineSettings lineSettings;
        public Equipment.EquipmentSettings equipSettings;
        public Equipment.EquipmentExtendedSettings equipExtSettings;
        public Equipment.strelkaSettings strelkaSettings;
        public Equipment.PicketSettings picketSettings;
        public Equipment.groupSettings groupSettings;
        public Equipment.ClassSettings classSettings;
        public Equipment.PathSettings pathSettings;

        public DB.metro_db_controller _db_controller;
        
        public Properties(DB.metro_db_controller db_controller)
        {
            InitializeComponent();

            _db_controller = new DB.metro_db_controller(db_controller);
            lineSettings = new LineSettings(_db_controller);
            picketSettings = new PicketSettings(_db_controller);
            equipSettings = new EquipmentSettings(_db_controller);
            equipExtSettings = new EquipmentExtendedSettings(_db_controller);
            strelkaSettings = new strelkaSettings(_db_controller);
            groupSettings = new groupSettings(_db_controller);
            classSettings = new ClassSettings(_db_controller);
            pathSettings = new PathSettings(_db_controller);
        }
        

        public bool setProperties( EquTreeNode equDBObj)
        {
            switch (equDBObj.ObjectDB.GetType().ToString())
            {
                //case "Peregon":
                //    peregonSettings.setObjDb((EquLayout)equDBObj);
                //    propertyGrid1.SelectedObject = peregonSettings;
                //    break;

                case "Registrator.EquPath":
                    pathSettings.setObjDB(equDBObj.ObjectDB);
                    propertyGrid1.SelectedObject = pathSettings;
                    
                    break;
                case "Registrator.EquLine":
                    lineSettings.setObjDB(equDBObj);
                    propertyGrid1.SelectedObject = lineSettings;
                    break;

                case "Registrator.EquPicket":
                    picketSettings.setObjDB(equDBObj);
                    propertyGrid1.SelectedObject = picketSettings;
                    break;

                case "Registrator.EquObject":
                    if ((equDBObj.ObjectDB as EquObject).typeEquip == 0)
                    {
                        if ((equDBObj.ObjectDB as EquObject).ObjectLenght == 0)
                        {
                            equipSettings.setObjDB(equDBObj);
                            propertyGrid1.SelectedObject = equipSettings;
                        }
                        else
                        {
                            equipExtSettings.setObjDB(equDBObj);
                            propertyGrid1.SelectedObject = equipExtSettings;
                        }
                    }
                    if ((equDBObj.ObjectDB as EquObject).typeEquip == 2)
                    {
                        strelkaSettings.setObjDB(equDBObj);
                        propertyGrid1.SelectedObject = strelkaSettings;
                    }

                    break;

                case "Registrator.EquGroup":
                    groupSettings.setObjDB(equDBObj);
                    propertyGrid1.SelectedObject = groupSettings;

                    break;

                case "Registrator.EquClass":
                    classSettings.setObjDB(equDBObj);
                    propertyGrid1.SelectedObject = classSettings;

                    break;
                default:
                    return false;
            }

            return true;
        }

    }
}
