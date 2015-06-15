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
        public Equipment.PeregonProperties peregonSettings;
        public Equipment.LineSettings lineSettings;
        public Equipment.EquipmentSettings equipSettings;
        public Equipment.EquipmentExtendedSettings equipExtSettings;
        public Equipment.strelkaSettings strelkaSettings;
        public Equipment.PicketSettings picketSettings;
        public Equipment.groupSettings groupSettings;
        public Equipment.ClassSettings classSettings;
        public Equipment.PathSettings pathSettings;

        public DB.metro_db_controller _db_controller;
        private EquLayout equLayoutNew;
        
        public Properties(DB.metro_db_controller db_controller)
        {
            InitializeComponent();

            _db_controller = new DB.metro_db_controller(db_controller);
            peregonSettings = new PeregonProperties(_db_controller);
            lineSettings = new LineSettings(_db_controller);
            picketSettings = new PicketSettings(_db_controller);
            equipSettings = new EquipmentSettings(_db_controller);
            equipExtSettings = new EquipmentExtendedSettings(_db_controller);
            strelkaSettings = new strelkaSettings(_db_controller);
            groupSettings = new groupSettings(_db_controller);
            classSettings = new ClassSettings(_db_controller);
            pathSettings = new PathSettings(_db_controller);
        }

        public void setProperties(string str, EquDbObject equDBObj)
        {
            switch (str)
            {
                case "Peregon":
                    peregonSettings.setObjDb((EquLayout)equDBObj);
                    propertyGrid1.SelectedObject = peregonSettings;
                    break;
                case "Path":
                    pathSettings.setObjDB((EquPath)equDBObj);
                    propertyGrid1.SelectedObject = pathSettings;
                    break;
                case "Line":
                    lineSettings.setObjDB((EquLine)equDBObj);
                    propertyGrid1.SelectedObject = lineSettings;
                    break;

                case "Picket":
                    picketSettings.setObjDB((Picket)equDBObj);
                    propertyGrid1.SelectedObject = picketSettings;
                    break;

                case "Equipment":
                    if (((EquObject)equDBObj).typeEquip == 0)
                    {
                        if (((EquObject)equDBObj).shiftFromEndPicket == -1)
                        {
                            equipSettings.setObjDB((EquObject)equDBObj);
                            propertyGrid1.SelectedObject = equipSettings;
                        }
                        else
                        {
                            equipExtSettings.setObjDB((EquObject)equDBObj);
                            propertyGrid1.SelectedObject = equipExtSettings;
                        }
                    }
                    if (((EquObject)equDBObj).typeEquip == 2)
                    {
                        strelkaSettings.setObjDB((EquObject)equDBObj);
                        propertyGrid1.SelectedObject = strelkaSettings;
                    }
                    break;

                case "Group":
                    groupSettings.setObjDB((EquGroup)equDBObj);
                    propertyGrid1.SelectedObject = groupSettings;

                    break;

                case "Class":
                    classSettings.setObjDB((EquClass)equDBObj);
                    propertyGrid1.SelectedObject = classSettings;

                    break;
            }
        }
    }
}
