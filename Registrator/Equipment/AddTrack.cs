using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Registrator.Equipment
{
    public partial class AddTrack : Form
    {
        private AddObjectOnTreeView addObjectOnTreeView;
        public DB.metro_db_controller _db_controller;
        public string newGroupName;
        private string setDataTable;
        public int lineNumer;
        public int Track;
        //
        public Peregons peregonObj;
        public Pickets PicketsObj;
        public EquClass equClass;
        public EquGroup equGroup;
        public EquLine equLine;
        public EquPath equPath;
        public equipment equipObj;
        public int peregonNumber;
        //
        public AddTrack(DB.metro_db_controller db_controller, AddObjectOnTreeView sender, string setDataTableArg)
        {
            InitializeComponent();

            _db_controller = new DB.metro_db_controller(db_controller);
            
            button2.Enabled = false;

            setDataTable = setDataTableArg;

            addObjectOnTreeView = sender;
        }
        public void Line(ref EquLine LineArg, ref EquGroup GroupArg, ref EquClass EquClassArg)
        {
            equLine = LineArg;
            equGroup = GroupArg;
            equClass = EquClassArg;
        }
        public void Path(ref EquPath PathArg, ref EquLine LineArg, ref EquGroup GroupArg, ref EquClass EquClassArg)
        {
            equPath = PathArg;
            equLine = LineArg;
            equGroup = GroupArg;
            equClass = EquClassArg;
        }
        private void button2_Click(object sender, EventArgs e)
        {
            string inputTrackName = txtBx_number.Text.Trim();
            //string newName = TxtBx_Name.Text.Trim();

            //if (newName.Length>49)
            //{
            //    MessageBox.Show("Слишком длинное название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            //    return;
            //}


            if (inputTrackName.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*' }) == -1)
            {
                if (inputTrackName.Length != 0)
                {
                    switch (setDataTable)
                    {
                        case "Track":
                            var resMatch = from r in _db_controller.trackTable.AsEnumerable() where r.Track == inputTrackName select new { r.Track };
                                        
                            if (resMatch.Count() == 0)
                            {
                                _db_controller.trackAdapter.Insert1(inputTrackName);

                                _db_controller.trackTable.Clear();
                                _db_controller.trackAdapter.Fill(_db_controller.trackTable);

                                var resID = from r in _db_controller.trackTable.AsEnumerable() where r.Track == inputTrackName select new {r.ID, r.Track };
                                _db_controller.all_equipment_adapter.Path1(equClass.Code, equGroup.Code, equLine.Code, resID.First().ID);

                                addObjectOnTreeView(resID.First().ID, inputTrackName, "Track");

                                Close();
                                Dispose();
                            }
                            else
                                MessageBox.Show("Путь с таким именем уже существует", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                    
                            break;
                    }
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
            {
                //TxtBx_Name.Enabled = true;
                button2.Enabled = true;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Close();
            Dispose();
        }

        private void TxtBx_Name_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
