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

        public DB.DataBaseHelper dbHelper;
        private EquLayout equLayoutNew;
        
        public Properties(string str, DB.DataBaseHelper dbHelper_Arg, EquDbObject equDBObj)
        {
            InitializeComponent();

            dbHelper = dbHelper_Arg;

            switch (str)
            {
                case "Peregon":
                    peregonSettings = new PeregonProperties(dbHelper_Arg, (EquLayout)equDBObj);
                    propertyGrid1.SelectedObject = peregonSettings;
                    break;

                case "Line":
                    lineSettings = new LineSettings(dbHelper_Arg, (EquLine)equDBObj);
                    propertyGrid1.SelectedObject = lineSettings;
                    break;

                case "Picket":
                    picketSettings = new PicketSettings(dbHelper_Arg, (Picket)equDBObj);
                    propertyGrid1.SelectedObject = picketSettings;
                    break;

                case "Equipment":
                    if(((EquObject)equDBObj).typeEquip==0)
                    {
                        if (((EquObject)equDBObj).shiftFromEndPicket == -1)
                        {
                            equipSettings = new EquipmentSettings(dbHelper_Arg, (EquObject)equDBObj);
                            propertyGrid1.SelectedObject = equipSettings;
                        }
                        else
                        {
                            equipExtSettings = new EquipmentExtendedSettings(dbHelper_Arg, (EquObject)equDBObj);
                            propertyGrid1.SelectedObject = equipExtSettings;
                        }
                    }
                    if (((EquObject)equDBObj).typeEquip == 1)
                    {
                        strelkaSettings = new strelkaSettings(dbHelper_Arg, (EquObject)equDBObj);
                        propertyGrid1.SelectedObject = strelkaSettings;
                    }
                    break;
            }
        }
    }
}
