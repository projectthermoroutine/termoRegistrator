using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace Registrator
{

    public class TripProjectChangedEvent_old : EventArgs
    {

        private TripProject m_tripProject = null;
        private int m_eventType = -1;
        
        public const int TRIP_PROJECT_NO_EVENT = -1;
        public const int TRIP_PROJECT_THERMO_FILES_ADDED = 0;
        public const int TRIP_PROJECT_SYNC_FILE_CHANGED = 1;
        public const int TRIP_PROJECT_BOTH_CHANGED = 2;

        public TripProjectChangedEvent_old(TripProject tripProject, int eventType)
        {
            m_tripProject = tripProject;
            m_eventType = eventType;
        }

        public TripProject TripProject { get { return m_tripProject; } set { m_tripProject = value; } }
        public int EventType { get { return m_eventType; } set { m_eventType = value; } }

    }

    [Serializable]
    public class TripProject
    {

        public event EventHandler FilesLoadedHandler;
        public event EventHandler TripProjectChangedHandler;
        public enum CameraDirections
        {
            Left, 
            Right
        }

        protected List<string> m_projectFiles = new List<string>();
        protected String m_trackFile = null;
        protected bool m_saved = false;
        protected String m_filePath = "";

        private CameraDirections camDir = CameraDirections.Left;

        public CameraDirections CameraDirection { get { return camDir; } set { camDir = value; } }

        private string m_IRBFilesPath = "";

        public string IRBFilesPath { get { return m_IRBFilesPath; } set { m_IRBFilesPath = value; } }

       
        void FireTripProjectChangedEvent()
        {
            EventHandler handler = TripProjectChangedHandler;
            if(handler != null)
            {
                handler(this, new EventArgs());
            }
        }

       void FilesLoaded()
        {
            EventHandler handler = FilesLoadedHandler;
            if (handler != null)
            {
                handler(this, new EventArgs());
            }
        }


        bool _is_files_loaded;
        List<string> _status_list;
        public void files_loaded(List<string> status_list)
        {
            if (status_list.Count != m_projectFiles.Count)
                return;

            _is_files_loaded = true;
            _status_list = status_list;
            FilesLoaded();
        }

        public List<string> filesLoadedStatus { get { return _status_list; } }

        object _mtx;
        public TripProject()
        {
            _mtx = new object();
            _is_files_loaded = false;
            _status_list = new List<string>();
        }

        public TripProject(String tripProjectFilepath) : this()
        {
            LoadProject(tripProjectFilepath);
        }

        public String FilePath { get { return m_filePath; } set { m_filePath = value; } }

        public List<string> Files { get { return m_projectFiles; } set { m_projectFiles = value; FireTripProjectChangedEvent(); } }

        public String TrackFile { get { return m_trackFile; } set { m_trackFile = value; } }
        public bool Saved { get { return m_saved; } }
        public bool LoadProject(String path)
        {
            FileStream fs;
            try
            {
                fs = new FileStream(path, FileMode.Open);
            }
            catch (Exception )
            {
                return false;
            }

            try
            {
                BinaryFormatter bf = new BinaryFormatter();

                m_projectFiles = (List<string>) bf.Deserialize(fs);
                m_filePath = (string) bf.Deserialize(fs);
                camDir = (CameraDirections) bf.Deserialize(fs);
                m_IRBFilesPath = (string) bf.Deserialize(fs);
            }
            catch (Exception )
            {
                return false;
            }
            fs.Close();
            m_saved = true;
            return true;
        }

        public void LoadProject(List<string> termoFiles, String trackFile)
        {
            for (int i = 0; i < termoFiles.Count; i++)
            {
                m_projectFiles.Add(termoFiles[i]);
            }

            m_trackFile = trackFile;

        }

        public void addTermoFiles(String[] termoFiles)
        {
            List<string> new_file_names = new List<string>();
            for (int i = 0; i < termoFiles.Length; i++)
            {
                var file_name = termoFiles[i];
                if (!m_projectFiles.Contains(file_name))
                    new_file_names.Add(file_name);
            }

            if (new_file_names.Count > 0)
            {
                lock (_mtx)
                {
                    m_projectFiles.AddRange(new_file_names);
                }
                FireTripProjectChangedEvent();
            }
        }

        public void clearTermoFiles()
        {
            lock (_mtx)
            {
                m_projectFiles.Clear();
            }
            FireTripProjectChangedEvent();
        }

        public void deleteTermoFiles(String[] termoFiles)
        {
            bool removed = false;
            lock (_mtx)
            {

                for (int i = 0; i < termoFiles.Length; i++)
                {
                    var file_name = termoFiles[i];
                    if (m_projectFiles.Remove(file_name))
                        removed = true;
                }
            }
            if (removed)
            {
                FireTripProjectChangedEvent();
            }
        }

        public void removeFiles(Predicate<string> where)
        {
            lock (_mtx)
            {
                List<string> _tmp_project_files = new List<string>(m_projectFiles);
                m_projectFiles.Clear();

                FireTripProjectChangedEvent();

                List<string> files_for_removing = new List<string>();
                foreach (var file_name in _tmp_project_files)
                    if (where(file_name))
                        files_for_removing.Add(file_name);

                foreach (var file_name in files_for_removing)
                {
                    try
                    {
                        File.Delete(file_name);
                        _tmp_project_files.Remove(file_name);
                    }
                    catch (System.IO.IOException)
                    {

                    }
                }

                m_projectFiles.AddRange(_tmp_project_files);
            }
            FireTripProjectChangedEvent();
        }

        public bool SaveProject(String tripProjectFilepath)
        {
            FileStream fs;
            try
            {
                fs = new FileStream(tripProjectFilepath, FileMode.Create);
            }
            catch (Exception )
            {
                return false;
            }

            try
            {
                BinaryFormatter bf = new BinaryFormatter();

                bf.Serialize(fs, m_projectFiles);
                bf.Serialize(fs, m_filePath);
                bf.Serialize(fs, camDir);
                bf.Serialize(fs, m_IRBFilesPath);
            }
            catch (Exception )
            {
                fs.Close();
                return false;
            }

            fs.Close();

            m_saved = true;
            return true;
        }
    }
}
