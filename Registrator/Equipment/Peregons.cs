using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Windows.Documents;

namespace Registrator
{
    public class ResultPeregons
    {
        public string LtName;
        public int Layout;
        public int NperegonBefore;
        public int NperegonAfter;

    }
    public class Peregons
    {

        public int numberPeregon1;
        public int numberPeregon2;
        public int numberPeregon3;

        public int peregon1;
        public int peregon2;
        public int indexSelectedfistOrLastItem = 0;
        public int indexAddNewPeregonInTrackOrNextFromLine = 0;
        public bool isSelectedNewPeregon;

        public List<int> lstPeregonNumber;
        public List<string> lstPeregonNames;
        public int line;
        public int before1;
        public int after1;
        public int before2;
        public int after2; 
        public int before3;
        public int after3;
        public int layoutNumber;
        public String layoutName;
        public string layoutNameShort;
        public int PeregonMaxIndex;
        public int selectedIndexFirst;
        public int selIndexInCmbBx;
        public DB.DataBaseHelper dbHelper;


        ///public  int isFirst=0;


        public string newLayoutName;

        public Peregons(DB.DataBaseHelper dbHelperArg)
        {
            lstPeregonNumber = new List<int>();
            lstPeregonNames = new List<string>();
            dbHelper = dbHelperArg;
            isSelectedNewPeregon = false;
            PeregonMaxIndex = Convert.ToInt32(dbHelper.TblAdapter_Layout.selectLayoutMaxIndex());
        }

        public List<string> createLogicalPeregonsList(int lineNumber, int trackNumber)
        {
            if(lstPeregonNumber.Count>0){
                lstPeregonNumber.Clear();
                lstPeregonNames.Clear();
            }

            IEnumerable<ResultPeregons> subquery1 = from r in dbHelper.dataTable_LayoutTable.AsEnumerable() where r.Line == lineNumber && r.Code!=0 orderby r.Layout select new ResultPeregons { LtName = r.Layout, Layout = r.Code, NperegonBefore = r.NperegonBefore, NperegonAfter = r.NperegonAfter};

            int whileIndex = 0;
                
            
                
            if (subquery1.Count() != 0)
            {
                lstPeregonNames.Insert(0, "Создать новый перегон");
                //indexAddNewPeregonInTrackOrNextFromLine = 1;

                var g = subquery1.First();

                if (g.NperegonBefore != 0)
                {
                    while (true)
                    {
                        whileIndex = 0;
                        foreach (var item in subquery1)
                        {
                            if (g.Layout == item.NperegonAfter)
                            {
                                g = item;
                                whileIndex = 1;
                            }
                        }
                        if (g.NperegonBefore == 0 || whileIndex == 0) break;
                    }
                }
         
                lstPeregonNames.Add(Convert.ToString(g.LtName));
                lstPeregonNumber.Add(Convert.ToInt32(g.Layout));

                whileIndex = 0;
                while (true)
                {
                    whileIndex = 0;
                    foreach (var item in subquery1) {
                        if ((g.Layout == item.NperegonBefore) && g.NperegonAfter != 0)
                        {
                            g = item;
                            lstPeregonNames.Add(Convert.ToString(item.LtName));
                            lstPeregonNumber.Add(Convert.ToInt32(item.Layout));
                            whileIndex = 1;
                        }
                    }
                    if (whileIndex == 0) break;
                }

                lstPeregonNames.Add("Создать новый перегон");
                //indexAddNewPeregonInTrackOrNextFromLine += 2;
            }
            else
                lstPeregonNames.Add("Создать новый перегон");

            return lstPeregonNames;
        }

        public void processListBoxSelect(int selectedIndex, ref ListBox lstBx_peregon, ref TextBox TxtBx_GroupName)
        {
            lstBx_peregon.Items.Clear();

            indexSelectedfistOrLastItem = 0;
            peregon2 = -1;

            selectedIndexFirst = selectedIndex;
            peregon1 = selectedIndexFirst;
   
                if (selectedIndexFirst == 1 || (selectedIndexFirst > 1 && selectedIndexFirst < lstPeregonNames.Count - 2))
                {
                    lstBx_peregon.Items.Add(lstPeregonNames[selectedIndexFirst + 1]);
                    selIndexInCmbBx = selectedIndexFirst + 1;
                    indexSelectedfistOrLastItem = 6; // выбрано добавление перегона между первыми перегонами на пути
                    TxtBx_GroupName.Enabled = false;
                    return;
                }
                if (selectedIndexFirst == (lstPeregonNames.Count - 2))
                {
                    lstBx_peregon.Items.Add(lstPeregonNames[selectedIndexFirst - 1]);
                    selIndexInCmbBx = selectedIndexFirst - 1;
                    indexSelectedfistOrLastItem = 7; // выбрано добавление перегона между крайними перегодами на пути
                    TxtBx_GroupName.Enabled = false;
                    return;
                }
          
                if (selectedIndexFirst == 0 /*&& indexAddNewPeregonInTrackOrNextFromLine >= 0*/)
                {
                    if (lstPeregonNumber.Count == 0)
                        indexSelectedfistOrLastItem = 9;
                    else
                    {
                        indexSelectedfistOrLastItem = 1; // выбрано создание нового первого перегона в линии
                        selIndexInCmbBx = 1;
                    }
                    TxtBx_GroupName.Enabled = true;
                    return;
                }
           
                if (selectedIndexFirst == (lstPeregonNames.Count - 1) /*&& indexAddNewPeregonInTrackOrNextFromLine >= 2*/)
                {
                    indexSelectedfistOrLastItem = 2; // выбрано созданте нового последнего перегона в линии
                    selIndexInCmbBx = 2;
                    TxtBx_GroupName.Enabled = true;
                    return;
                }
        }

        public void processSecondSelectionListBox(int listBoxSelectedIndex)
        {
            if (listBoxSelectedIndex == 0) indexSelectedfistOrLastItem = 7;
            else indexSelectedfistOrLastItem = 6;
        }

        public void calcNewPregonNumber()
        {
            int PeregonMaxIndexTMP = PeregonMaxIndex;
            switch (indexSelectedfistOrLastItem)
            {
                case 9:
                    before1 = 0;
                    after1 = 0;
                    layoutNumber = ++PeregonMaxIndexTMP;
                    layoutName = newLayoutName;
                    break;
                //case 8:// выбрано добавление перегона между не крайними перегонами на пути
                //    layoutName = newLayoutName;
                //    layoutNumber = ++PeregonMaxIndex;
                //    after1 = layoutNumber;
                //    before2 = 
                //    break;
                case 7:// выбрано создание перегона между крайними перегодами на пути
                    layoutName = newLayoutName;
                    layoutNumber = ++PeregonMaxIndexTMP;
                    after1 = layoutNumber;
                    numberPeregon1 = lstPeregonNumber[selectedIndexFirst-1];
                    before2 = lstPeregonNumber[selectedIndexFirst-1];
                    after2 = lstPeregonNumber[selectedIndexFirst];
                    numberPeregon2 = layoutNumber;
                    before3 = layoutNumber;
                    numberPeregon3 = lstPeregonNumber[selectedIndexFirst];
                    break;
                case 6:// выбрано создание перегона между первыми перегонами на пути
                    layoutName = newLayoutName;
                    layoutNumber = ++PeregonMaxIndexTMP;
                    after1 = layoutNumber;
                    numberPeregon1 = lstPeregonNumber[selectedIndexFirst];
                    before2 = lstPeregonNumber[selectedIndexFirst];
                    after2 = lstPeregonNumber[selectedIndexFirst+1];
                    numberPeregon2 = layoutNumber;
                    before3 = layoutNumber;
                    numberPeregon3 = lstPeregonNumber[selectedIndexFirst + 1];
                    break;
               
                case 2: // выбрано создание нового последнего перегона в линии
                    before2 = lstPeregonNumber.Last();
                    after2 = 0;
                    layoutName = newLayoutName;
                    layoutNumber = ++PeregonMaxIndexTMP;
                    after1 = layoutNumber;
                    numberPeregon3 = layoutNumber;
                    break;
                case 1: // выбрано создание нового первого перегона в линии
                    before1 = 0;
                    after1 = lstPeregonNumber.First();
                    layoutNumber = ++PeregonMaxIndexTMP;
                    layoutName = newLayoutName;
                    before2 = layoutNumber;
                    break;
            }
        }
        public void clear()
        {
            lstPeregonNumber.Clear();
            lstPeregonNames.Clear();
            indexSelectedfistOrLastItem = 0;
        }
    }
}
