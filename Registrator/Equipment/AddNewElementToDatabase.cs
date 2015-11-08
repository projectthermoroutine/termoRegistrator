using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
//using System.Windows;

namespace Registrator
{
    public delegate void DisplayTheAddedObject(string name, string Tag);
    public partial class AddNewElementToDatabase : Form
    {
        private AddObjectOnTreeView addObjectOnTreeView;
        public DB.metro_db_controller _db_controller;
        int PathNumber;
        List<int> codesOfStations;
        List<int> codesOfPickets;
        List<int> codesOfEquipment;

        Peregons peregonsObj;
        PicketsManager  picketsManager;

        EquGroup  equGroup;
        EquClass  equClass;
        EquLine   equLine;
        EquPath   equPath;
        
        string tag;

        public AddNewElementToDatabase( PicketsManager PicketsManager,
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
            peregonsObj = new Peregons(_db_controller);
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
                case "Peregon":
                    groupBox2.Text = "Добавляемый перегон";
                    codesOfStations.Clear();
                    peregonsObj.clear();

                    string[] str = peregonsObj.createLogicalPeregonsList(equLine.Code, equPath.Code).Skip(1).Take(peregonsObj.lstPeregonNames.Count - 2).ToArray();
                    CmbBx.Items.AddRange(str);
                    codesOfStations.AddRange(peregonsObj.lstPeregonNumber.ToArray());
                    break;

                case "Picket":
                    this.Text = "Добавление пикета";
                    groupBox2.Text = "Добавляемый пикет";
                    label6.Text = "Список пикетов";

                    peregonsObj.isSelectedNewPeregon = false;

                    peregonsObj.layoutNumber = equLine.Code;
                  
                    picketsManager.createLogicalPicketList(equPath.Code, equLine.Code/*, equLayout.Code*/);

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
       
        private void create_Click(object sender, EventArgs e)
        {
            switch (tag)
            {
                case "Peregon":
                    Equipment.AddPeregon formGroup = new Equipment.AddPeregon(_db_controller, new DisplayTheAddedObject(processDataFromChildForm));
                    formGroup.peregons(equLine.Code, PathNumber, ref peregonsObj);
                    formGroup.ShowDialog();
                    break;
                
            }
        }
        private int SelectedIndexChangedOneTime = 0;
        void processDataFromChildForm(string newObjectName, string typeOfObject)
        {
            SelectedIndexChangedOneTime = 0;
            switch (typeOfObject)
            {
                case "Peregon":
                    _db_controller.layout_table.Clear();
                    _db_controller.layout_adapter.Fill(_db_controller.layout_table);

                    codesOfStations.Clear();
                    string[] str = peregonsObj.createLogicalPeregonsList(equLine.Code, equPath.Code).Skip(1).Take(peregonsObj.lstPeregonNames.Count - 2).ToArray();
                    CmbBx.Items.AddRange(str);
                    codesOfStations.AddRange(peregonsObj.lstPeregonNumber.ToArray());

                    CmbBx.Items.Clear();
                    CmbBx.Items.AddRange(str);
                    
                    if(peregonsObj.indexSelectedfistOrLastItem == 1)
                            CmbBx.SelectedIndex = 0;
                    else
                    {
                        if (peregonsObj.indexSelectedfistOrLastItem == 2)
                            CmbBx.SelectedIndex = CmbBx.Items.Count - 1;
                        else
                            CmbBx.SelectedIndex = peregonsObj.selIndexInCmbBx;
                    }

                    listBoxCreatedObjects.Items.Add(newObjectName);
                    peregonsObj.isSelectedNewPeregon = true;
                    break;

            }
        }

        private void CmbBx_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (SelectedIndexChangedOneTime == 1)
                return;

            SelectedIndexChangedOneTime++;
             if (CmbBx.SelectedIndex != -1)
             {
                int match;
                
                switch(tag)
                {
                    case "Peregon":
                        match = codesOfStations[CmbBx.SelectedIndex];
                        var res4 = from r in _db_controller.all_equipment_table.AsEnumerable() where r.Layout == match && r.Track == equPath.Code && r.LineNum == equLine.Code && r.GroupNum == equGroup.Code && r.ClassNum == equClass.Code select new { r.Npicket};  // check name duplicate
                        
                        if (res4.Count() == 0)
                        {
                            listBoxCreatedObjects.Items.Add(CmbBx.SelectedItem.ToString());
                            peregonsObj.isSelectedNewPeregon = false;
                            OK.Enabled = true;
                        }
                        else
                        {
                            MessageBox.Show("Выбранный перегон уже присутствует в пути", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                            OK.Enabled = false;
                        }
                        break;
                }
            }
        }
    }
}
