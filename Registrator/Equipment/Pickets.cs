﻿using System;
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
    public class Pickets
    {
        public int picketNumber = -1;
        public string newPcketName;
        public int indexAddNewPicketOrSelectNextFromPeregon; // TODO
        public int indexSelectedfistOrLastItem;
        public DB.metro_db_controller _db_controller;

        public List<string> lstPicketsNumber;
        public List<string> lstPicketsNumberDislay;
        public int PicketMaxIndex;

        public bool isSelectedNewPicket;
        public int peregon;
        public int before1;
        public int after1;
        public int before2;
        public int after2;
        public int layoutNumber;
        public String layoutName;
        public int selectedIndexFirst;
        public int typeOfPicketCreation;
        public int newPicketIndex;
        public int isFirst=0;

        public Pickets(DB.metro_db_controller db_controller)
        {
            lstPicketsNumber = new List<string>();
            lstPicketsNumberDislay = new List<string>();
            _db_controller = new DB.metro_db_controller(db_controller);
            isSelectedNewPicket = false;
            
        }

        public List<string> createLogicalPicketList(int picketNumber, int path, int line)
        {
            if (lstPicketsNumber.Count>0)
                lstPicketsNumber.Clear();

            if (lstPicketsNumberDislay.Count > 0)
                lstPicketsNumberDislay.Clear();

            if (picketNumber != -1)
            {
                isFirst = 0;
                var empData = (from r in _db_controller.pickets_table.AsEnumerable() where /*r.Peregon == picketNumber &&*/ r.Npiketa != 0 && r.line == line && r.path == path orderby r.Npiketa select new { r.Npiketa,/* r.Peregon,*/ r.NpicketBefore, r.NpicketAfter, r.number }).Distinct();

                if (empData.Count() != 0)
                {
                    var val1 = empData.First();

                    lstPicketsNumber.Add("Новый пикет");
                    lstPicketsNumberDislay.Add("Новый пикет");
                    //find first picket
                    int whileIndex = 0;
                    while (true)
                    {
                        whileIndex = 0;
                        foreach (var item in empData)
                        {
                            if (val1.number == item.NpicketAfter)
                            {
                                val1 = item;
                                whileIndex = 1;
                            }
                        }
                        if (val1.NpicketBefore == 0 || whileIndex == 0) break;
                    }
                    //
                    // create logical list of pickets
                    lstPicketsNumber.Add(Convert.ToString(val1.number));
                    lstPicketsNumberDislay.Add(Convert.ToString(val1.Npiketa));

                    while (true)
                    {
                        whileIndex = 0;
                        foreach (var item in empData)
                        {
                            if (val1.number == item.NpicketBefore)
                            {
                                val1 = item;
                                lstPicketsNumber.Add(Convert.ToString(item.number));
                                lstPicketsNumberDislay.Add(Convert.ToString(val1.Npiketa));
                                whileIndex = 1;
                            }
                        }
                        if (whileIndex == 0) break;
                    }
                    //
                    lstPicketsNumber.Add("Новый пикет");
                    lstPicketsNumberDislay.Add("Новый пикет");
                }
                else
                {
                    isFirst = 1;
                    lstPicketsNumber.Add("Новый пикет");
                    lstPicketsNumberDislay.Add("Новый пикет");
                }
            }
            else
            {
                lstPicketsNumber.Add("Новый пикет");
                lstPicketsNumberDislay.Add("Новый пикет");
            }

            //return lstPicketsNumber;
            return lstPicketsNumberDislay;
        }

        public void calcNewPicketNumber(int nePicketNum)
        {
            int PicketMaxIndexTMP = nePicketNum;
            int ind1 = lstPicketsNumber.Count - 1;

            if (selectedIndexFirst == 0)
            {
                if (isFirst == 1)
                {
                    typeOfPicketCreation = 3;
                    before1 = 0;
                    after1 = 0;
                    newPicketIndex = PicketMaxIndexTMP;
                }
                else
                {
                    before1 = 0;
                    after1 = Convert.ToInt32(lstPicketsNumber[1]);
                    before2 = PicketMaxIndexTMP;
                    typeOfPicketCreation = 0;
                    newPicketIndex = before2;
                }
            }
            else
            {
                if (selectedIndexFirst == (lstPicketsNumber.Count - 1))
                {
                    after1 = PicketMaxIndexTMP;

                    before2 = Convert.ToInt32(lstPicketsNumber[lstPicketsNumber.Count - 2]);
                    after2 = 0;
                    typeOfPicketCreation = 1;
                    newPicketIndex = after1;
                }
            }
        }

        public void processListBoxSelect(int selectedIndex)
        {
            //indexSelectedfistOrLastItem = 0;
            //peregon2 = -1;

            selectedIndexFirst = selectedIndex;
            //peregon1 = selectedIndexFirst;

            //if (selectedIndexFirst == 0)
            //{
            //    indexSelectedfistOrLastItem = 1; // выбрано создание нового первого перегона в линии
            //    TxtBx_GroupName.Enabled = true;
            //    return;
            //}
            //if (selectedIndexFirst == (lstPicketsNumber.Count - 1))
            //{
            //    indexSelectedfistOrLastItem = 2; // выбрано созданте нового последнего перегона в линии
            //    TxtBx_GroupName.Enabled = true;
            //    return;
            //}
        }

        public void processSecondSelectionListBox(int listBoxSelectedIndex)
        {
            if (listBoxSelectedIndex == 0) indexSelectedfistOrLastItem = 7;
            else indexSelectedfistOrLastItem = 6;
        }
        public void clear()
        {
            lstPicketsNumber.Clear();

        }
    }
}
