﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Registrator.Equipment.CreateDbObjectsCtrls
{
    public class IDbObjectCreation
    {
        public event EventHandler<DbObjectEventArg> DbObjectAddedEvent;
        public void DbObjectAdded(EquDbObject db_object)
        {
            EventHandler<DbObjectEventArg> handler = DbObjectAddedEvent;
            if (handler != null)
                handler(this, new DbObjectEventArg(db_object));
        }
    }

    public class DbObjectEventArg : EventArgs
    {
        public DbObjectEventArg(EquDbObject db_object){
            DbObject = db_object;
        }
        public EquDbObject DbObject {get;private set;}
    }

    public partial class CreateTrackForm : Form
    {
        public event EventHandler<DbObjectEventArg> TrackAddedEvent;
        void TrackAdded(EquDbObject db_object)
        {
            EventHandler<DbObjectEventArg> handler = TrackAddedEvent;
            if (handler != null)
                handler(this, new DbObjectEventArg(db_object));
        }

        DB.metro_db_controller _db_controller;
        EquClass equClass;
        EquGroup equGroup;
        EquLine equLine;
        PicketsManager _PicketsManager;
        int defaultPicketLength;

        public CreateTrackForm( DB.metro_db_controller db_controller,EquDbObject parent)
        {
            InitializeComponent();

            _db_controller = new DB.metro_db_controller(db_controller);
            
            button2.Enabled = false;
            defaultPicketLength = Registrator.Properties.Settings.Default.DefaultPicketLength;
            equLine  = parent as EquLine;
            equGroup = equLine.Parent as EquGroup;
            equClass = equGroup.Parent as EquClass;

            _PicketsManager   = new PicketsManager(_db_controller);
        }
    
        private void ApplyBtn_Click(object sender, EventArgs e)
        {
            string trackName = txtBx_number.Text.Trim();

            if (trackName.Length > 49)
            {
                MessageBox.Show("Слишком длинное название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            if (trackName.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*' }) == -1)
            {
                if (trackName.Length != 0)
                {

                    

                    var resMatch = from r in _db_controller.trackTable.AsEnumerable() where r.Track == trackName select new { r.Track };

                    if (resMatch.Count() == 0)
                    {
                        _db_controller.trackAdapter.Insert1(trackName);
                    }
                      
                    _db_controller.trackTable.Clear();
                    _db_controller.trackAdapter.Fill(_db_controller.trackTable);

                    var resID = from r in _db_controller.trackTable.AsEnumerable() 
                                where r.Track == trackName 
                                select new {r.ID, r.Track };

                    int TrackID = resID.First().ID;
                    var resLineTrack = from r in _db_controller.all_equipment_table.AsEnumerable() where r.Track == TrackID && r.LineNum == equLine.Code && r.GroupNum == equGroup.Code select r;

                    if(resLineTrack.Count()>0)
                    {
                        MessageBox.Show("Путь с таким именем уже присутствует", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                        return;
                    }
                   

                    _db_controller.all_equipment_adapter.Path1(equClass.Code, equGroup.Code, equLine.Code, resID.First().ID);

                    var new_track = new EquPath(resID.First().ID, trackName, equLine);

                    addRangePickets(new_track);

                    _db_controller.pickets_table.Clear();
                    _db_controller.pickets_adapter.Fill(_db_controller.pickets_table);
                    _db_controller.all_equipment_table.Clear();
                    _db_controller.all_equipment_adapter.Fill(_db_controller.all_equipment_table);

                    TrackAdded(new_track);

                    Close();
                    Dispose();
                }
                else
                    MessageBox.Show("Название не должно быть пустым", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                MessageBox.Show("Некорректный символ: '@','.', ',', '!', ... ",
                    "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }


        private void txtBx_number_TextChanged(object sender, EventArgs e)
        {
            if(txtBx_number.Text.Length > 0)
                button2.Enabled = true;
        }

        private void CancelBtn_Click(object sender, EventArgs e)
        {
            Close();
            Dispose();
        }

        void addRangePickets(EquPath track_object)
        {
            if (numUpDownFrom.Value > numUpDownTo.Value)
            {
                MessageBox.Show("Диапазон пикетов задан не верно", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            int addedPicketID = Convert.ToInt32(_db_controller.pickets_adapter.selectMaxNumberIndex());

            bool flagMinus0 = false;
            if (numUpDownFrom.Value < 0)
                flagMinus0 = true;

            for (int i = (int)numUpDownFrom.Value; i <= (int)numUpDownTo.Value; i++)
            {
                addedPicketID++;

                if (flagMinus0 && i == 0)
                {
                    EquPicket p1 = _PicketsManager.AddPicketToDB("-0", equClass.Code, equGroup.Code, equLine.Code, track_object.Code, addedPicketID, defaultPicketLength * 10);

                    var empData1 = from r in _db_controller.all_equipment_table.AsEnumerable() where r.number == addedPicketID && r.number != 0 && r.LineNum == equLine.Code && r.Track == track_object.Code select new { r.number };

                    if (empData1.Count() == 0)
                    {
                        _db_controller.all_equipment_adapter.PicketAdd(equClass.Code, equGroup.Code, equLine.Code, track_object.Code, 0, addedPicketID);
                    }
                    else
                    {
                        MessageBox.Show("Пикет с таким номером уже присутствует на пути", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                        return;
                    }

                    addedPicketID++;
                }

                EquPicket p = _PicketsManager.AddPicketToDB(i.ToString(), equClass.Code, equGroup.Code, equLine.Code, track_object.Code, addedPicketID, defaultPicketLength * 10);

                var empData = from r in _db_controller.all_equipment_table.AsEnumerable() where r.number == addedPicketID && r.number != 0 && r.LineNum == equLine.Code && r.Track == track_object.Code select new { r.number };

                if (empData.Count() == 0)
                {
                    _db_controller.all_equipment_adapter.PicketAdd(equClass.Code, equGroup.Code, equLine.Code, track_object.Code, 0, addedPicketID);
                }
                else
                {
                    MessageBox.Show("Пикет с таким номером уже присутствует на пути", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                }
            }
        }
    }
}
