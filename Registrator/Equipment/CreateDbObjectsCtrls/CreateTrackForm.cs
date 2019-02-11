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
    
    public partial class CreateTrackForm : Form
    {
        public event EventHandler<DbObjectEventArg> TrackAddedEvent;

        void TrackAdded(EquDbObject db_object)
        {
            TrackAddedEvent?.Invoke(this, new DbObjectEventArg(db_object));
        }

        DB.metro_db_edit_controller _db_controller;
        EquClass equClass;
        EquGroup equGroup;
        EquLine equLine;
        int defaultPicketLength;

        public CreateTrackForm( DB.metro_db_controller db_controller, EquDbObject parent)
        {
            InitializeComponent();

            _db_controller = new DB.metro_db_edit_controller(db_controller);
            
            button2.Enabled = false;
            defaultPicketLength = Registrator.Properties.Settings.Default.DefaultPicketLength;
            equLine  = parent as EquLine;
            equGroup = equLine.Parent as EquGroup;
            equClass = equGroup.Parent as EquClass;
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
                    EquPath addedTrack = null;
                    string error_str = "";
                    Exception exception = null;
                    using (var dbTransaction = _db_controller.dbContext.Database.BeginTransaction())
                    {
                        try
                        {
                            int addedTrackID = 0;
                            if (_db_controller.dbContext.Tracks.Count() > 0)
                                addedTrackID = _db_controller.dbContext.Tracks.Max(t => t.ID);

                            _db_controller.dbContext.Tracks.Add(new Track { ID = ++addedTrackID, Name = trackName, LineId = equLine.Code });
                            _db_controller.dbContext.SaveChanges();

                            addedTrack = new EquPath(addedTrackID, trackName, equLine);

                            addRangePickets(addedTrack);
                            dbTransaction.Commit();

                        }
                        catch (System.Data.Entity.Infrastructure.DbUpdateConcurrencyException exc)
                        {
                            exception = exc;
                        }
                        catch (System.Data.Entity.Infrastructure.DbUpdateException exc)
                        {
                            exception = exc;
                        }
                        catch (System.Data.Entity.Validation.DbEntityValidationException exc)
                        {
                            exception = exc;
                        }
                        catch (System.NotSupportedException exc)
                        {
                            exception = exc;
                        }
                        catch (System.InvalidOperationException exc)
                        {
                            exception = exc;
                        }
                        catch (Exception exc)
                        {
                            exception = exc;
                        }

                        if (exception != null)
                        {
                            dbTransaction.Rollback();
                            error_str = exception.Message;
                            MessageBox.Show(exception.Message, "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                        else
                            TrackAdded(addedTrack);
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

            int count_pickets = (int)(numUpDownTo.Value - numUpDownFrom.Value) + 1;

            if (numUpDownFrom.Value < 0 && numUpDownTo.Value >= 0)
                count_pickets++;

            string displayId = numUpDownFrom.Value.ToString();

            _db_controller.AddPickets(
                    first_picket: new Picket { Npiketa = displayId, Dlina = defaultPicketLength * 10, path = track_object.Code },
                    count: count_pickets,
                    add_to_left: false
                    );
        }
    }
}
