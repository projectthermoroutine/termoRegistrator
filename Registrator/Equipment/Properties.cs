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

        public DB.DataBaseHelper dbHelper;
        private EquLayout equLayoutNew;
        
        public Properties(DB.DataBaseHelper dbHelper_Arg)
        {
            InitializeComponent();

            dbHelper = dbHelper_Arg;
            peregonSettings = new PeregonProperties(dbHelper);
            lineSettings = new LineSettings(dbHelper);
            picketSettings = new PicketSettings(dbHelper);
            equipSettings = new EquipmentSettings(dbHelper);
            equipExtSettings = new EquipmentExtendedSettings(dbHelper);
            strelkaSettings = new strelkaSettings(dbHelper);
            groupSettings = new groupSettings(dbHelper);
            classSettings = new ClassSettings(dbHelper);
        }

        public void setProperties(string str, EquDbObject equDBObj)
        {
            switch (str)
            {
                case "Peregon":
                    peregonSettings.setObjDb((EquLayout)equDBObj);
                    propertyGrid1.SelectedObject = peregonSettings;
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
            }
        }
    }
}
