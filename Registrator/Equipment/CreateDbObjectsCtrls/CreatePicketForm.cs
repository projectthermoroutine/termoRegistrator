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
    public partial class CreatePicketForm : Form
    {
        public event EventHandler<DbObjectEventArg> PicketAddedEvent;
        void PicketAdded(EquDbObject path)
        {
            PicketAddedEvent?.Invoke(this, new DbObjectEventArg(path));
        }

        DB.metro_db_edit_controller _db_controller;
        //
        EquClass equClass;
        EquGroup equGroup;
        EquLine equLine;
        EquPath equPath;
        ContextMetroCard _dbContext;
        int _pathId = 0;

        IQueryable<Picket> _picketsOnPath;
        Picket _first_picket = null;
        Picket _last_picket = null;

        bool _pathHasNoPickets = false;

        public CreatePicketForm(DB.metro_db_controller db_controller, EquDbObject parent, ContextMetroCard dbContext)
        {
            _dbContext = dbContext;
            _db_controller = new DB.metro_db_edit_controller(db_controller);

            InitializeComponent();

            numUpDownSingleLength.Value = (decimal)Registrator.Properties.Settings.Default.DefaultPicketLength;
            
            equPath = parent as EquPath;
            equLine = equPath.Parent as EquLine;
            equGroup = equLine.Parent as EquGroup;
            equClass = equGroup.Parent as EquClass;

            _pathId = equPath.Code;

            _picketsOnPath = _db_controller.PicketsByPath(_pathId);

            _pathHasNoPickets = _picketsOnPath.Count() == 0;

            if(!_pathHasNoPickets)
            {
                var picket_list = _picketsOnPath.ToList();
                _first_picket = picket_list.First();
                _last_picket = picket_list.Last();

                NextPicketNumberTextBox.Text = Picket.NextNumber(_last_picket.Npiketa, false);
                NextPicketNumberTextBox.Enabled = false;

            }

        }

        bool AddToLeft = false;

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            Dispose();
            Close();
        }

        private void ButtonAddClick(object sender, EventArgs e)
        {
            addRangePickets();
            PicketAdded(equPath);

            Close();
            Dispose();
        }
        void addRangePickets()
        {
            string first_picket_number = NextPicketNumberTextBox.Text;

            try
            {

                _db_controller.AddPickets(
                        first_picket: new Picket { Npiketa = first_picket_number, Dlina = (int)numUpDownSingleLength.Value * 10, path = equPath.Code },
                        count: (int)CountPicketsCtrl.Value,
                        add_to_left: AddToLeft
                        );
            }
            catch(Exception exc)
            {
                MessageBox.Show(exc.Message, "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void buttonApplyDefault_Click(object sender, EventArgs e)
        {
             Registrator.Properties.Settings.Default.DefaultPicketLength = (int)numUpDownSingleLength.Value;
        }

        private void chBxNonDefault_CheckedChanged(object sender, EventArgs e)
        {
            if (chBxNonDefault.Checked)
                numUpDownSingleLength.Enabled = true;
            else
                numUpDownSingleLength.Enabled = false;
        }

        private void radioButtonLeft_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButtonLeft.Enabled)
            {
                AddToLeft = true;

                if (_first_picket != null)
                    NextPicketNumberTextBox.Text = Picket.NextNumber(_first_picket.Npiketa, true);
            }
        }

        private void radioButtonRight_CheckedChanged(object sender, EventArgs e)
        {
            if(radioButtonRight.Enabled)
            {
                AddToLeft = false;

                if (_last_picket != null)
                    NextPicketNumberTextBox.Text = Picket.NextNumber(_last_picket.Npiketa, false);
            }
        }
    }
}
