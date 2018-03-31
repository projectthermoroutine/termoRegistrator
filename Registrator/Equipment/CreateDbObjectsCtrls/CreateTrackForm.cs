using Registrator.DB.EFClasses;
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

            int incorrect_symbol_index = trackName.IndexOfAny(RegistratorFormStrings.incorrect_symbols);

            if (incorrect_symbol_index == -1)
            {
                if (trackName.Length != 0)
                {
                    try
                    {
                        int addedTrackID = 0;
                        if (_db_controller.dbContext.Tracks.Count() > 0)
                            addedTrackID = _db_controller.dbContext.Tracks.Max(t => t.ID);

                        _db_controller.dbContext.Tracks.Add(new Track { ID = ++addedTrackID, Track1 = trackName, LineId = equLine.Code });
                        _db_controller.dbContext.SaveChanges();

                        var addedTrack = new EquPath(addedTrackID, trackName, equLine);
                       

                        addRangePickets(addedTrack);
                        TrackAdded(addedTrack, Pickets.ToArray());
                    }
                    catch (Exception exc)
                    {
                        MessageBox.Show(exc.Message, "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }

                    Close();
                    Dispose();
                }
                else
                    MessageBox.Show("Название не должно быть пустым", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                MessageBox.Show("Некорректный символ: " + trackName[incorrect_symbol_index],
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

            int addedPicketID = 0;

            if(_db_controller.dbContext.Pickets.Count() > 0)
                addedPicketID = _db_controller.dbContext.Pickets.Max(pkt => pkt.number);

            bool flagMinus0 = false;

            if (numUpDownFrom.Value < 0)
                flagMinus0 = true;

            Picket p = null;
            string displayId = "";

            for (int i = (int)numUpDownFrom.Value; i <= (int)numUpDownTo.Value; i++)
            {
                addedPicketID++;

                if (flagMinus0 && i == 0)
                    displayId = "-0";
                else
                    displayId = i.ToString();

                if (_db_controller.dbContext.Pickets.Where(pk => pk.number == addedPicketID && pk.path == track_object.Code).Distinct().Count() == 0)
                {
                    p = _PicketsManager.AddPicketToDB(displayId, equClass.Code, equGroup.Code, equLine.Code, track_object.Code, addedPicketID, defaultPicketLength * 10);
                    Pickets.Add(new EquPicket
                    {
                        after = p.NpicketAfter,
                        before = p.NpicketBefore,
                        keyNumber = p.number,
                        LeftLineShift = p.StartShiftLine,
                        RightLineShift = p.EndShiftLine,
                        npicket = p.Npiketa,
                        Code = p.number,
                        lenght = p.Dlina,
                        Name = p.Npiketa,
                        Path = track_object,
                        Parent = track_object.Parent
                    });
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
