using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
//using System.Windows;

namespace Registrator.Equipment.CreateDbObjectsCtrls
{
    public delegate void DisplayTheAddedObject(string name, string Tag);
    public partial class CreateDbElement : Form
    {
        private AddObjectOnTreeView addObjectOnTreeView;
        public DB.metro_db_controller _db_controller;
        int PathNumber;
        List<int> codesOfStations;
        List<int> codesOfPickets;
        List<int> codesOfEquipment;

        PicketsManager  picketsManager;

        EquGroup  equGroup;
        EquClass  equClass;
        EquLine   equLine;
        EquPath   equPath;
        
        string tag;

        public CreateDbElement( PicketsManager PicketsManager,
                                        AddObjectOnTreeView sender,
                                        string tagArg,
                                        DB.metro_db_controller db_controller,
                                        EquClass equClassArg,
                                        EquGroup equGroupArg,
                                        EquLine equLineArg,
                                        EquPath equPathArg
                                        )
        {
            _db_controller = new DB.metro_db_controller(db_controller);
            
            InitializeComponent();
            
            tag = tagArg;
            picketsManager = PicketsManager;

            codesOfStations = new List<int>();
            codesOfPickets = new List<int>();
            codesOfEquipment = new List<int>();

            equLine  = equLineArg;
            equGroup = equGroupArg;
            equClass = equClassArg;
            equPath = equPathArg;

            addObjectOnTreeView = sender;

            OK.Enabled = false;
           
            switch(tag)
            {
                case "Picket":
                    this.Text = "Добавление пикета";
                    groupBox2.Text = "Добавляемый пикет";
                    label6.Text = "Список пикетов";

                    //picketsManager.createLogicalPicketList(equPath.Code, equLine.Code, equGroup.Code, equClass.Code);

                    foreach (EquPicket p in picketsManager.PicketsList)
                        CmbBx.Items.Add(p.npicket);

                    OK.Enabled = true;
                    break;
            }
        }
        //--- OK button --- 
        private void button1_Click(object sender, EventArgs e)
        {
            int result;
            string groupName = equGroup.Name; 
            int GroupIndex = equGroup.Code; 


            PathNumber = equPath.Code;
            switch (tag)
            {
                case "Peregon":
                        int peregonNumberNew = codesOfStations[CmbBx.SelectedIndex];
                        result = _db_controller.all_equipment_adapter.insertStoredProcedure3(   equClass.Code,
                                                                                            equGroup.Code, equLine.Code,
                                                                                            PathNumber,
                                                                                            peregonNumberNew
                                                                                        );

                        addObjectOnTreeView(peregonNumberNew, listBoxCreatedObjects.Text + ";" + Convert.ToString(CmbBx.SelectedIndex), "Peregon");
                    break;
                
                
            }

            Close();
            Dispose();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Close();
            Dispose();
        }
    }
}
