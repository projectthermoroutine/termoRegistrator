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
using System.IO;

namespace Registrator
{
    using map_objects_list = List<measure_object>;
    public partial class AnalyzeForm : Form
    {
        const long max_frame_distance_mm = 5000;
        private MovieTransit m_movieTransit = null;
        DB.metro_db_controller _db_controller;

        public AnalyzeForm()
        {
            InitializeComponent();
        }

        public AnalyzeForm(MovieTransit movieTransit ,DB.metro_db_controller db_ctrl)
            : this()
        {
            m_movieTransit = movieTransit;
            _db_controller = new DB.metro_db_controller(db_ctrl);

        }

        List<Registrator.DB.ResultEquipCode> get_objects_by_coordinate(_frame_coordinate coordinate, long max_offset_in_mm)
        {
            _db_controller.setLineAndPath(coordinate.line, coordinate.path);
            return _db_controller.get_objects_by_coordinate(coordinate.coordinate, max_offset_in_mm).ToList();
        }

        public string pathDBFiles;

        private void Analyze(BackgroundWorker worker)
        {
            var number_frames = m_movieTransit.FramesCount();
            pathDBFiles = _db_controller.getDBFilePath();
            _db_controller.clearCurrentPathANDLineValues();
            using (ChoiceFrameObject choice_frames = new ChoiceFrameObject())
            {
                choice_frames.SaveObjectFrameProcessHandler += save_object_termogramme;
                double frame_data_time = 0;

                for (int i = 0; i < number_frames; i++)
                {
                    if (worker.CancellationPending)
                        break;
                    _frame_coordinate coordinate = new _frame_coordinate();

                    var result = m_movieTransit.GetFramePositionInfo((uint)i,
                                                    out coordinate,
                                                    out frame_data_time);

                    if (!result)
                    {
                        worker.ReportProgress(100 * (i + 1) / number_frames);
                        continue;
                    }

                    if (frame_data_time == 0.0 || coordinate.coordinate == 0)
                    {
                        worker.ReportProgress(100 * (i + 1) / number_frames);
                        continue;
                    }

                    var objects = get_objects_by_coordinate(coordinate, max_frame_distance_mm);

                    choice_frames.process_objects(objects,
                                                    delegate(Registrator.DB.ResultEquipCode obj, out int objId, out long obj_coord)
                                                    {
                                                        objId = obj.Code;
                                                        obj_coord = obj.shiftLine;
                                                    },
                                                    coordinate.coordinate,
                                                    (uint)i,
                                                    frame_data_time);

                    worker.ReportProgress(100 * (i + 1) / number_frames);
                }
            }
        }

        void save_object_termogramme(object sender, SaveObjectFrameProcessEvent arg)
        {
            string termogramm_namePath = generate_termogramm_name(arg.ObjectId, arg.FrameCoord, arg.FrameTimeStamp);

            if (!m_movieTransit.SaveOneFrame((uint)arg.FrameIndex, termogramm_namePath))
            {
                // ERROR
            }
            else
            {
                _db_controller.addObjectTermogramme(arg.ObjectId, termogramm_namePath, arg.FrameCoord, irb_frame_time_helper.date_time_from_unixtime(arg.FrameTimeStamp));
            }
        }

        string generate_termogramm_name(int objectId,
                                        long frame_coord,
                                        double timestamp)
        {
            return pathDBFiles + "\\TermogrammObject_" + objectId.ToString() + "_" + frame_coord.ToString() + "_" + irb_frame_time_helper.build_time_string_from_unixtime(timestamp,"yyyy_MM_dd_HH_mm_ss") + ".irb";
        }
        private DateTime UnixTimeStampToDateTime(double unixTimeStamp)
        {
            // Unix timestamp is seconds past epoch
            System.DateTime dtDateTime = new DateTime(1970, 1, 1, 0, 0, 0, 0, System.DateTimeKind.Utc);
            dtDateTime = dtDateTime.AddSeconds(unixTimeStamp).ToLocalTime();
            return dtDateTime;
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
