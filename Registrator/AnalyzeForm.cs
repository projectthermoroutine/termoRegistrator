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

        const long max_frame_distance_cm = 500;
        private MovieTransit m_movieTransit = null;
        DB.metro_db_controller _db_controller;
        string _transit_name = "";

        public AnalyzeForm()
        {
            InitializeComponent();
        }

        public AnalyzeForm(MovieTransit movieTransit, string transit_name,DB.metro_db_controller db_ctrl)
            : this()
        {
            _transit_name = transit_name;
            m_movieTransit = movieTransit;
            _db_controller = new DB.metro_db_controller(db_ctrl);

            equipmentTableAdapter1.Fill(teplovizorDataSet1.equipment);
            shotsTableAdapter1.Fill(teplovizorDataSet1.shots);

        }


        class LIST_ITEM
        {
            public LIST_ITEM()
            { objectId = 0; object_coordinate = 0; nearest_frame_index = 0; frame_coordinate = 0; frame_timestamp = 0.0; }
            public LIST_ITEM(int objId, long obj_coordinate):this()
            {
                objectId = objId; object_coordinate = obj_coordinate;
            }
            public int objectId;
            public long object_coordinate;
            public Int32 nearest_frame_index;
            public long frame_coordinate;
            public double frame_timestamp;
        }

        List<LIST_ITEM> _processing_objects;
        List<LIST_ITEM> _processed_objects;


        List<LIST_ITEM> get_objects_by_coordinate(long coordinate, long max_offset_in_cm)
        {
            List<LIST_ITEM> result = new List<LIST_ITEM>();

            var objects = _db_controller.get_objects_by_coordinate(coordinate / 10, max_offset_in_cm);
            foreach (var db_object in objects)
            {
                result.Add(new LIST_ITEM (db_object.Code, db_object.shiftLine));
            }

            return result;
        }

        private void Analyze(BackgroundWorker worker)
        {
            var number_frames = m_movieTransit.FramesCount();

            _processing_objects = new List<LIST_ITEM>();
            _processed_objects = new List<LIST_ITEM>();

            for (int i = 0; i < number_frames; i++)
            {
                if (worker.CancellationPending)
                    break;

                double frame_data_time = 0;
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


                //TODO: устанавливаем линию и путь в контроллер БД

                var objects = get_objects_by_coordinate(coordinate.coordinate, max_frame_distance_cm);
                if (objects.Count == 0)
                {
                    _processed_objects.Clear();
                    save_objects_termogrammes(_processing_objects);
                    _processed_objects = _processing_objects;
                    _processing_objects.Clear();
                }
                else 
                {
                    process_objects(objects, coordinate.coordinate, i, frame_data_time);
                }

                worker.ReportProgress(100 * (i + 1) / number_frames);
            }
        }

        void process_objects(List<LIST_ITEM> objects, long frame_coordinate, int frame_index, double frame_timestamp)
        {
            List<LIST_ITEM> not_processed_objects = new List<LIST_ITEM>();

            foreach (var db_object in objects) 
            {
                bool processed = false;
                foreach(var processed_object in _processed_objects)
                {
                    if (processed_object.objectId == db_object.objectId)
                    {
                        processed = true;
                        break;
                    }
                }
                if (!processed)
                {
                    db_object.frame_coordinate = frame_coordinate;
                    db_object.nearest_frame_index = frame_index;
                    db_object.frame_timestamp = frame_timestamp;
                    not_processed_objects.Add(db_object);
                }

            }

            List<LIST_ITEM> objects_for_save = new List<LIST_ITEM>();
            List<LIST_ITEM> new_objects = new List<LIST_ITEM>();

            foreach (var db_object in not_processed_objects)
            {
                LIST_ITEM nearest_frame = null;
                bool add_frame = true;
                foreach (var processing_object in _processing_objects)
                {
                    if (processing_object.objectId == db_object.objectId)
                    {
                        add_frame = false;
                        var prev_distance = calc_frame_object_distance(processing_object.frame_coordinate, processing_object.object_coordinate);
                        var current_distance = calc_frame_object_distance(db_object.frame_coordinate, db_object.object_coordinate);

                        if (prev_distance < current_distance)
                        {
                            nearest_frame = processing_object;
                        }
                        else
                        {
                            processing_object.frame_coordinate = db_object.frame_coordinate;
                            processing_object.nearest_frame_index = db_object.nearest_frame_index;
                            processing_object.frame_timestamp = db_object.frame_timestamp;
                        }

                        break;
                    }
                }
                if (nearest_frame != null)
                    objects_for_save.Add(nearest_frame);

                if (add_frame)
                {
                    new_objects.Add(db_object);
                }
            }

            delete_items_from_list(_processed_objects, delegate(LIST_ITEM processed_object)
            {
                var current_distance = calc_frame_object_distance(frame_coordinate, processed_object.object_coordinate);
                return max_frame_distance_cm * 10 < current_distance ? true : false;
            });

            _processed_objects.AddRange(new_objects);

            save_objects_termogrammes(objects_for_save);

            delete_items_from_list(_processed_objects, delegate(LIST_ITEM processing_object)
            {
                int index_for_delete = -1;
                for (int i = 0; i < objects_for_save.Count; i++)
                {
                    var saved_object = objects_for_save[i];
                    if(processing_object.objectId == saved_object.objectId)
                    {
                        index_for_delete = i;
                        break;
                    }
                }

                if(index_for_delete >= 0){
                    objects_for_save.RemoveAt(index_for_delete);
                    return true;
                }

                return false;
            });

        }

        void delete_items_from_list<T>(List<T> list_values,Predicate<T> match)
        {
            if (list_values.Count > 0)
            {
                List<int> indxs_for_delete = new List<int>();
                for (int i = 0; i < list_values.Count; i++)
                {
                    var value = list_values[i];
                    if(match(list_values[i]))
                    {
                        indxs_for_delete.Add(i);
                    }
                }

                if (indxs_for_delete.Count > 0)
                {
                    int count_deletes = 0;
                    foreach (var index_for_delete in indxs_for_delete)
                    {
                        list_values.RemoveAt(index_for_delete - count_deletes);
                        count_deletes++;
                    }

                }
            }

        }

        long calc_frame_object_distance(long frame_coord,long object_coord)
        {
            return frame_coord <= object_coord ? object_coord - frame_coord : frame_coord - object_coord;
        }

        void save_objects_termogrammes(List<LIST_ITEM> objects)
        {
            foreach(var db_object in objects)
            {
                var termogramm_name = generate_termogramm_name(db_object);
                //TODO: сохраняем данные в БД и/или на диск
            }
        }

        string generate_termogramm_name(LIST_ITEM info)
        {
            //TODO: генерация либо имя термограммы для БД, либо имя файла с термограммой
            return "";  

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
