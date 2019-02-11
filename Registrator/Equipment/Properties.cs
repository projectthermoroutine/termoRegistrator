using Registrator.Equipment.Properties;
using Registrator.Equipment.Properties.Classes;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;

namespace Registrator.Equipment
{
    public partial class DBProperties : DockContent
    {
        public Equipment.LineSettings lineSettings;
        public Equipment.EquipmentInPicketSettings equipInPicketSettings;
        public Equipment.EquipmentExtendedSettings equipExtSettings;
        public Equipment.StrelkaSettings strelkaSettings;
        public Equipment.PicketSettings picketSettings;
        public Equipment.groupSettings groupSettings;
        public Equipment.ClassSettings classSettings;
        public Equipment.PathSettings pathSettings;
        public EquipmentSettings equipSettings;
        public EquipmentsClassesSettings equipmentsClassesSettings;
        public StrelkaClassSettings strelkaClassSettings;
        public TrafficLightClassesSettings trafficLightClassesSettings;
        public TrafficLightSettings trafficLightSettings;

        public DB.metro_db_controller _db_controller;
        
        public DBProperties(DB.metro_db_controller db_controller)
        {
            InitializeComponent();

            _db_controller = new DB.metro_db_controller(db_controller);
            lineSettings = new LineSettings(_db_controller);
            picketSettings = new PicketSettings(_db_controller);
            equipInPicketSettings = new EquipmentInPicketSettings(_db_controller);
            equipExtSettings = new EquipmentExtendedSettings(_db_controller);
            strelkaSettings = new StrelkaSettings(_db_controller);
            groupSettings = new groupSettings(_db_controller);
            classSettings = new ClassSettings(_db_controller);
            pathSettings = new PathSettings(_db_controller);
            equipmentsClassesSettings = new EquipmentsClassesSettings(_db_controller);
            strelkaClassSettings = new StrelkaClassSettings(_db_controller);
            equipSettings = new EquipmentSettings(_db_controller);
            trafficLightClassesSettings = new TrafficLightClassesSettings(_db_controller);
            trafficLightSettings = new TrafficLightSettings(_db_controller);
        }

        public bool SetProperties(EquTreeNode equDBObj)
        {

            if (equDBObj.ObjectDB == null)
                return false;

            EquDbObject dbObject = equDBObj.ObjectDB as EquDbObject;
            object selectedObject = null;

            switch (equDBObj.ObjectDB.GetType().ToString())
            {
                //case "Peregon":
                //    peregonSettings.setObjDb((EquLayout)equDBObj);
                //    propertyGrid1.SelectedObject = peregonSettings;
                //    break;

                case "Registrator.EquipmentObject":
                    EquipmentObject equipObject = equDBObj.ObjectDB as EquipmentObject;

                    if (equipObject.Tag == "strelka")
                    {
                        strelkaClassSettings.SetObjDB(equDBObj.ObjectDB);
                        selectedObject = strelkaClassSettings;
                    }

                    if (equipObject.Tag == "equip")
                    {
                        equipmentsClassesSettings.SetObjDB(equDBObj.ObjectDB);
                        selectedObject = equipmentsClassesSettings;
                    }

                    if (equipObject.Tag == "traffic_light")
                    {
                        trafficLightClassesSettings.SetObjDB(equDBObj.ObjectDB);
                        selectedObject = trafficLightClassesSettings;
                    }

                    break;

                case "Registrator.EquPath":
                    pathSettings.SetObjDB(equDBObj.ObjectDB);
                    selectedObject = pathSettings;

                    break;
                case "Registrator.EquLine":
                    lineSettings.SetObjDB(equDBObj.ObjectDB);
                    selectedObject = lineSettings;
                    break;

                case "Registrator.EquPicket":
                    picketSettings.SetObjDB(equDBObj.ObjectDB, equDBObj);
                    selectedObject = picketSettings;
                    break;

                case "Registrator.EquObject":

                    EQUIPS_TYPES t = (equDBObj.ObjectDB as EquObject).typeEquip;
                    switch (t)
                    {
                        case EQUIPS_TYPES.Equipment:
                            if ((equDBObj.ObjectDB as EquObject).ObjectLenght == 0)
                            {
                                equipInPicketSettings.SetObjDB(equDBObj.ObjectDB);
                                selectedObject = equipInPicketSettings;
                            }
                            else
                            {
                                equipExtSettings.SetObjDB(equDBObj.ObjectDB);
                                selectedObject = equipExtSettings;
                            }
                            break;

                        case EQUIPS_TYPES.Strelka:
                            strelkaSettings.SetObjDB(equDBObj.ObjectDB);
                            selectedObject = strelkaSettings;
                            break;

                        case EQUIPS_TYPES.TrafficLight:
                            trafficLightSettings.SetObjDB(equDBObj.ObjectDB);
                            selectedObject = trafficLightSettings;
                            break;
                    }

                    break;

                case "Registrator.EquGroup":
                    groupSettings.SetObjDB(equDBObj.ObjectDB);
                    selectedObject = groupSettings;

                    break;

                case "Registrator.EquClass":
                    classSettings.setObjDB(equDBObj);
                    selectedObject = classSettings;
                    break;
                default:
                    return false;
            }

            if (selectedObject != null)
                propertyGrid1.SelectedObject = selectedObject;

            return true;
        }

    }
}
