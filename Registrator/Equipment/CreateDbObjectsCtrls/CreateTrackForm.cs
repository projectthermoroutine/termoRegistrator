using System;
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
        public DbObjectEventArg(EquDbObject db_object, EquDbObject[] db_objects)
        {
            DbObject = db_object;
            DbObjects = db_objects;
        }
        public DbObjectEventArg(EquDbObject[] db_objects, bool left_picket)
        {
            DbObjects = db_objects;
            leftPicket = left_picket;
        }
        public EquDbObject DbObject {get;private set; }
        public EquDbObject[] DbObjects { get; private set; }
        public bool leftPicket { get; private set; }
    }
   
    public partial class CreateTrackForm : Form
    {
        public event EventHandler<DbObjectEventArg> TrackAddedEvent;
        List<EquDbObject> Pickets;

        void TrackAdded(EquDbObject db_object, EquDbObject[] db_objects)
        {
            EventHandler<DbObjectEventArg> handler = TrackAddedEvent;
            if (handler != null)
                handler(this, new DbObjectEventArg(db_object, db_objects));
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
            Pickets           = new List<EquDbObject>();
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
                    int  addedTrackID = _db_controller.dbContext.Tracks.Max(t => t.ID);

                    _db_controller.queriesAdapter.add_track(++addedTrackID, trackName, equLine.Code);

                    var addedTrack = new EquPath(addedTrackID, trackName, equLine);

                    addRangePickets(addedTrack);
                    TrackAdded(addedTrack, Pickets.ToArray());

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

                    if (_db_controller.dbContext.Pickets.Where(pk => pk.number == addedPicketID && pk.path == track_object.Code).Distinct().Count() == 0)
                    {
                        Pickets.Add(p1);
                    }
                    else
                    {
                        MessageBox.Show("Пикет с таким номером уже присутствует на пути", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                        return;
                    }

                    addedPicketID++;
                }

                EquPicket p = _PicketsManager.AddPicketToDB(i.ToString(), equClass.Code, equGroup.Code, equLine.Code, track_object.Code, addedPicketID, defaultPicketLength * 10);

                if (_db_controller.dbContext.Pickets.Where(pk => pk.number == addedPicketID && pk.path == track_object.Code).Distinct().Count() == 0)
                {
                    Pickets.Add(p);
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
