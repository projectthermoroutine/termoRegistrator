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
        public DB.DataBaseHelper dbHelper;
        int lineNumber;
        int PathNumber;
        List<int> codesOfStations;
        List<int> codesOfPickets;
        List<int> codesOfEquipment;
        String peregonNameNew1 = "";
        int peregonNumberNew1 = 0;

        Peregons peregonsObj;
        Pickets picketsObj;

        EquGroup  equGroup;
        EquClass  equClass;
        EquLine   equLine;
        EquPath   equPath;
        EquLayout equLayout;
        Picket equPicket;
        private string tag;
        string[] displayPicketNumbers;

        public AddNewElementToDatabase(AddObjectOnTreeView sender, string tagArg, DB.DataBaseHelper dbHelperArg, EquClass equClassArg, EquGroup equGroupArg, EquLine equLineArg, EquPath equPathArg, EquLayout equLayoutArg, Picket equPicketArg, EquObject equObjectArg)
        {
            dbHelper = dbHelperArg;
            
            InitializeComponent();
            
            tag = tagArg;
            peregonsObj = new Peregons(dbHelper);
            picketsObj = new Pickets(dbHelper);

            codesOfStations = new List<int>();
            codesOfPickets = new List<int>();
            codesOfEquipment = new List<int>();

            equLine  = equLineArg;
            equGroup = equGroupArg;
            equClass = equClassArg;
            equPath = equPathArg;
            equLayout = equLayoutArg;
            equPicket = equPicketArg;

            addObjectOnTreeView = sender;

            OK.Enabled = false;

           
            switch(tag)
            {
                case "Peregon":
                    lineNumber = equLine.Code;
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

                    lineNumber = equLine.Code;
                    peregonsObj.isSelectedNewPeregon = false;
                    peregonNumberNew1 = equLayout.Code;
                    peregonNameNew1 = equLayout.Name;

                    peregonsObj.layoutNumber = equLine.Code;
                    int codeSelectedPeregon;

                    picketsObj.clear();

                    codeSelectedPeregon = equLayout.Code;

                    displayPicketNumbers = picketsObj.createLogicalPicketList(codeSelectedPeregon, equPath.Code, equLine.Code).Skip(1).Take(picketsObj.lstPicketsNumber.Count - 2).ToArray();
                    CmbBx.Items.AddRange(displayPicketNumbers);
                    codesOfPickets.AddRange(picketsObj.lstPicketsNumber.Skip(1).Take(picketsObj.lstPicketsNumber.Count - 2).ToArray().Select(c => int.Parse(c.ToString())).ToArray());
                    OK.Enabled = true;
                    break;
            }
        }
        //--- OK button --- 
        private void button1_Click(object sender, EventArgs e)
        {
            int result;
            //var eq = (from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Group == equGroup.Code  select new{r.Code});
            //var eqIndex = eq.First();
            //----- Group ----------------------------------------------------------------------------
            string groupName = equGroup.Name; //TxtBx_GroupName.Text;
            int GroupIndex = equGroup.Code; //Convert.ToInt32(dbHelper.TblAdapter_Group.selectGroupMaxIndex());
            //----- Line -----------------------------------------------------------------------------
            if (equLine != null)
                lineNumber = equLine.Code;
            //----- Peregons -------------------------------------------------------------------------
            PathNumber = equPath.Code;
            switch (tag)
            {
                case "Peregon":
                        peregonNumberNew1 = codesOfStations[CmbBx.SelectedIndex];
                        result = dbHelper.TblAdapter_AllEquipment.insertStoredProcedure3(   equClass.Code,
                                                                                            equGroup.Code, lineNumber,
                                                                                            PathNumber,
                                                                                            peregonNumberNew1
                                                                                        );

                        addObjectOnTreeView(peregonNumberNew1, TxtBx.Text + ";" + Convert.ToString(CmbBx.SelectedIndex), "Peregon");
                    break;
                
                case "Picket":
                    int picketInd = codesOfPickets[CmbBx.SelectedIndex];
                    string dispNumber = displayPicketNumbers[CmbBx.SelectedIndex];
                    var empData = from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.number == picketInd && r.Npicket != 0 && r.LineNum == equLine.Code && r.Track == equPath.Code select new { r.number };

                    if (empData.Count() == 0)
                    {
                        result = dbHelper.TblAdapter_AllEquipment.PicketAdd(equClass.Code, equGroup.Code, lineNumber, PathNumber, equLayout.Code, picketInd);
                        addObjectOnTreeView(picketInd, Convert.ToString(dispNumber), "Picket");
                       
                    }
                    else
                    {
                        //OK.Enabled = false;
                        MessageBox.Show("Пикет с таким номером уже присутствует на пути", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
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
                    Equipment.AddPeregon formGroup = new Equipment.AddPeregon(dbHelper, new DisplayTheAddedObject(processDataFromChildForm));
                    formGroup.peregons(lineNumber, PathNumber,ref peregonsObj);
                    formGroup.ShowDialog();
                    break;
                case "Picket":
                    Equipment.AddPicket formPicket = new Equipment.AddPicket(dbHelper, new DisplayTheAddedObject(processDataFromChildForm));
                    formPicket.Pickets(equLayout.Code, ref picketsObj, equPath, equLine);
                    formPicket.ShowDialog();
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
                    dbHelper.dataTable_LayoutTable.Clear();
                    dbHelper.TblAdapter_Layout.Fill(dbHelper.dataTable_LayoutTable);

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

                    this.TxtBx.Text = newObjectName;
                    peregonsObj.isSelectedNewPeregon = true;
                    break;

                case "Pickets":
                    dbHelper.dataTable_PicketsTable.Clear();
                    dbHelper.TblAdapter_Pickets.Fill(dbHelper.dataTable_PicketsTable);
                    displayPicketNumbers = picketsObj.createLogicalPicketList(equLayout.Code, equPath.Code, equLine.Code).Skip(1).Take(picketsObj.lstPicketsNumber.Count - 2).ToArray();
                    codesOfPickets.Clear();
                    codesOfPickets.AddRange(picketsObj.lstPicketsNumber.Skip(1).Take(picketsObj.lstPicketsNumber.Count - 2).ToArray().Select(c => int.Parse(c.ToString())).ToArray());
                    CmbBx.Items.Clear();
                    CmbBx.Items.AddRange(displayPicketNumbers);

                    if (picketsObj.typeOfPicketCreation == 0)
                    {
                        CmbBx.SelectedIndex = 0;
                    }
                    else
                    {
                        CmbBx.SelectedIndex = CmbBx.Items.Count - 1;
                    }

                    this.TxtBx.Text = newObjectName;
                    picketsObj.isSelectedNewPicket = true;
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
                        var res4 = from r in dbHelper.dataTable_AllEquipment.AsEnumerable() where r.Layout == match && r.Track == equPath.Code && r.LineNum == equLine.Code && r.GroupNum == equGroup.Code && r.ClassNum == equClass.Code select new { r.Npicket};  // check name duplicate
                        
                        if (res4.Count() == 0)
                        {
                            TxtBx.Text = CmbBx.SelectedItem.ToString();
                            peregonsObj.isSelectedNewPeregon = false;
                            OK.Enabled = true;
                        }
                        else
                        {
                            MessageBox.Show("Выбранный перегон уже присутствует в пути", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                            OK.Enabled = false;
                        }
                        break;

                    case "Picket":
                        TxtBx.Text = CmbBx.SelectedItem.ToString();
                   
                        break;
                }
            }
        }
    }
}
