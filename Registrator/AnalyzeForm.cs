using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ThermoRoutineLib;
using System.Data.OleDb;
using Registrator.DB.teplovizorDataSetTableAdapters;
using System.IO;

namespace Registrator
{
    using map_objects_list = List<measure_object>;
    public partial class AnalyzeForm : Form
    {

        private MovieTransit m_tvHandler = null;
        private map_objects_list _equipment_list = null;
        private metro_map _metro_map;


        public AnalyzeForm()
        {
            InitializeComponent();
        }

        public AnalyzeForm(MovieTransit tvHandler, metro_map metro_map, map_objects_list equList)
            : this()
        {
             m_tvHandler = tvHandler;
            _equipment_list = equList;
            _metro_map = metro_map;

            equipmentTableAdapter1.Fill(teplovizorDataSet1.equipment);
            shotsTableAdapter1.Fill(teplovizorDataSet1.shots);

        }

        public MovieTransit TvHandler { get { return m_tvHandler; } set { m_tvHandler = value; } }
        public map_objects_list EquList { get { return _equipment_list; } set { _equipment_list = value; } }
        private void Analyze(BackgroundWorker worker)
        {

            int maxProgress = _metro_map.MaxEquNum();

            DateTime dateTime = DateTime.Now;

            String dStr = "";// = m_tvHandler.GetTimeString();

            if (dStr != null)
                dateTime = Convert.ToDateTime(dStr);

            for (int i = 0; i < maxProgress; i++)
            {
                String baseFilePath = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetEntryAssembly().Location) + "\\db\\";
                string path = String.Concat(new object[] { baseFilePath, dateTime.ToString("yyyyMMdd"), "\\"});
                if(!Directory.Exists(path))
                    Directory.CreateDirectory(path);

                path = String.Concat(new object[] { path, DateTime.Now.ToString("yyyyMMddHHmmssffff"), i, ".irb" });

                var map_object = _equipment_list[i];
                int result = 0;// m_tvHandler.SaveEquipmentIRB((int)map_object.frame_number, path);
                if (result == 0)
                {

                    string[] lineNpath = {map_object.line.ToString(),map_object.path.ToString()};
                    string[] pickNoffs = { map_object.picket.ToString(), (map_object.offset * 0.01).ToString() };

                    Registrator.DB.teplovizorDataSet.equipment1DataTable dt = equipment1TableAdapter1.GetData(
                                                                                        (int)map_object.code,
                                                                                        (int)map_object.span,
                                                                                        (int)map_object.path,
                                                                                        (int)map_object.picket,
                                                                                        (float)map_object.offset
                                                                                        );

                    if (dt.Rows.Count < 1)
                    {
                        int r1 = equipmentTableAdapter1.InsertAutoQuery(
                                                    (int)map_object.code,
                                                    (int)map_object.span,
                                                    (int)map_object.path,
                                                    (int)map_object.picket,
                                                    (float)map_object.offset
                                            );
                        try
                        {
                            int r4 = equipmentTableAdapter1.Update(teplovizorDataSet1.equipment);
                        }
                        catch (Exception)
                        {
                            MessageBox.Show("1 failed");
                        }

                    }

                    dt = equipment1TableAdapter1.GetData(
                                                        (int)map_object.code,
                                                        (int)map_object.span,
                                                        (int)map_object.path,
                                                        (int)map_object.picket,
                                                        (float)map_object.offset
                                                        );
                    
                    if (dt.Rows.Count < 1)
                    {
                        return;
                    }

                    int r3 = shotsTableAdapter1.InsertShotQuery(dateTime, path, (int)dt.Rows[0].ItemArray[0]);

                }
                else
                    MessageBox.Show("frame hasn't been written !!!");

                worker.ReportProgress(100 * (i+1) / maxProgress);
            }

            try
            {
                int r4 = equipmentTableAdapter1.Update(teplovizorDataSet1.equipment);
            }
            catch (Exception)
            {
                MessageBox.Show("1 failed");
            }

            try
            {
                int r5 = shotsTableAdapter1.Update(teplovizorDataSet1.shots);
            }
            catch (Exception)
            {
                MessageBox.Show("2 failed");
            }
        }

        private void Stop()
        {
            analyzeBgWorker.CancelAsync();
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            Stop();
            Close();
            Dispose();
        }

        private void analyzeButton_Click(object sender, EventArgs e)
        {
            analyzeBgWorker.RunWorkerAsync();
        }

        private void analyzeBgWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            BackgroundWorker worker = sender as BackgroundWorker;

            Analyze(worker);
        }

        private void analyzeBgWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            this.progressBar1.Value = e.ProgressPercentage;
            label1.Text = String.Concat(new object[] { "Выполнено: ", e.ProgressPercentage.ToString(), " %" });
        }

        private void analyzeBgWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
        }



    }
}
