using Registrator.DB.EFClasses;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace Registrator.DB
{
    public class DBRegistratorException : Exception
    {
        public DBRegistratorException() { }

        public DBRegistratorException(string message)
            : base(message)
        { }

        public DBRegistratorException(string message, Exception inner)
            : base(message, inner)
        { }
    }

    public class metro_db_controller
    {
        public metro_db_controller(metro_db_controller controller)
        {
            if (controller == null)
            {
                _db = new DB.DataBaseHelper();
                _dbContext = new ContextMetroCard();

                if (LoadingProgressChanged != null)
                    LoadingProgressChanged(this, new LoadingProgressEvent(0, "start tables loading"));

                _db.ObjectsFramesTblAdapter.Fill(_db.ObjectsFrames);
                if (LoadingProgressChanged != null)
                    LoadingProgressChanged(this, new LoadingProgressEvent(2, "tables loading"));

                _db.TblAdapter_Pickets.Fill(_db.Pickets);
               if (LoadingProgressChanged != null)
                    LoadingProgressChanged(this, new LoadingProgressEvent(30, "tables loading"));
                _db.TblAdapter_EquipmentFilter.Fill(_db.EquipmentFilter_Tbl);
                if (LoadingProgressChanged != null)
                    LoadingProgressChanged(this, new LoadingProgressEvent(95, "tables loading"));
                _db.TblAdapter_Passages.Fill(_db.Passages);
                if (LoadingProgressChanged != null)
                    LoadingProgressChanged(this, new LoadingProgressEvent(100, "tables loading"));

            }
            else
            {
                _db = controller._db;
            }
        }

        public List<MetrocardDataSet.ObjectsFramesRow> getObjMeasurements(int ObjID)
        {
            //TODO transfer request from LINQ in sql
            return (from r in _db.ObjectsFrames.AsEnumerable() where r.ObjID == ObjID orderby r.Time select r).ToList();
        }


        public void addObjectTermogramme(int id, string path, long coordinate, DateTime dt)
        {
            try {
                queriesAdapter.insertRowInPassageTable(id, path, coordinate, dt);
            }
            catch (System.Data.SqlClient.SqlException e) {
                String str = e.Message;
            }

        }
        public string getDBFilePath()
        {
            object obj = queriesAdapter.databaseFiles();
            string str = obj as string;
            str = str.Remove(str.LastIndexOf("\\"));
            //TODO error check
            return str;
        }

        static public event EventHandler<LoadingProgressEvent> LoadingProgressChanged;

        private DataBaseHelper _db;
        private ContextMetroCard _dbContext;
        public ContextMetroCard dbContext { private set {} get { return _dbContext; } }

        public int current_line_ID = -1;
        public int current_path_ID = -1;
        string current_path_Tag = "";
        private int coordinatPlusNearDistance = 0;
        private int coordinat = 0;
        private int NEAR_DISTANCE = 0;
        //private int sampling_frequencies = 0;
        //private IEnumerable<Registrator.DB.ResultEquipCode> _line_path_objects = null;

        private IEnumerable<EFClasses.Equipment> _line_path_objects = null;

        private IEnumerable<Registrator.DB.ResultEquipCode>[] _line_path_objects_array = null;

        public int GetLineID(string line_code)
        {
           return _dbContext.Lines.Where(l => l.LineCode == line_code).FirstOrDefault().LineNum;
        }

        public int GetTrackID(int line_id, string track_name)
        {
            return _dbContext.Tracks.Where(t => t.Track1 == track_name && t.LineId == line_id).FirstOrDefault().ID;
        }

        public IEnumerable<MetrocardDataSet.EquipmentFilter_TblRow> GetFilters()
        {
            return from r in _db.EquipmentFilter_Tbl.AsEnumerable() orderby r.filter_id select r;
        }

        private System.Data.DataTable get_table(string table_name)
        {
            return (System.Data.DataTable)_db.Tables[table_name];
        }

        public MetrocardDataSet.OrdersTableAdapterDataTable orders_table { get { return (MetrocardDataSet.OrdersTableAdapterDataTable)get_table("OrdersTableAdapter"); } }
        public MetrocardDataSet.PicketsDataTable pickets_table { get { return (MetrocardDataSet.PicketsDataTable)get_table("Pickets"); } }
        public MetrocardDataSet.EquipmentFilter_TblDataTable equipment_filter_table { get { return (MetrocardDataSet.EquipmentFilter_TblDataTable)get_table("EquipmentFilter_Tbl"); } }
        public MetrocardDataSet.ObjectsFramesDataTable objectsFramesDataTable { get { return (MetrocardDataSet.ObjectsFramesDataTable)get_table("ObjectsFrames"); } }

        private System.ComponentModel.Component get_table_adapter(table_index index)
        {
            return _db._adapters[(int)index];
        }

        public MetrocardDataSetTableAdapters.OrdersTableAdapter orders_adapter { get { return (MetrocardDataSetTableAdapters.OrdersTableAdapter)get_table_adapter(table_index.Orders); } }
        public MetrocardDataSetTableAdapters.PicketsTableAdapter pickets_adapter { get { return (MetrocardDataSetTableAdapters.PicketsTableAdapter)get_table_adapter(table_index.Pickets); } }
        public MetrocardDataSetTableAdapters.EquipmentFilter_TblTableAdapter equipment_filter_adapter { get { return (MetrocardDataSetTableAdapters.EquipmentFilter_TblTableAdapter)get_table_adapter(table_index.EquipmentFilter); } }
        public MetrocardDataSetTableAdapters.PassagesTableAdapter passagesAdapter { get { return (MetrocardDataSetTableAdapters.PassagesTableAdapter)get_table_adapter(table_index.Passages); } }
        public MetrocardDataSetTableAdapters.QueriesTableAdapter queriesAdapter { get { return (MetrocardDataSetTableAdapters.QueriesTableAdapter)get_table_adapter(table_index.Queries); } }
        public MetrocardDataSetTableAdapters.ObjectsFramesTableAdapter ObjectsFramesAdapter { get { return (MetrocardDataSetTableAdapters.ObjectsFramesTableAdapter)get_table_adapter(table_index.ObjectsFrames); } }

        public void refresh()
        {
            _db.refresh();
        }

        public void refresh_process_eqipment_table()
        {
            var adapter = equipment_filter_adapter;
            var table = equipment_filter_table;
            adapter.DeleteAll();
            table.Clear();
            adapter.Fill((MetrocardDataSet.EquipmentFilter_TblDataTable)table);
        }


        public void setWorkingAreaEquipmentMonitor()
        {
            coordinatPlusNearDistance = coordinat + NEAR_DISTANCE;

        }

        public void SaveObjectTemperature(int object_id, float temperature)
        {
            queriesAdapter.insertEquipTemperature(object_id, (int)temperature);
            //_db.TblAdapter_ProcessEquipment.insertEquipTemperature();
        }

        public IEnumerable<EFClasses.Equipment> get_objects(int line, int path)
        {
            if (groupsNumbers.Count == 0) // filters disable
            {
                _line_path_objects = _dbContext.Equipments.Where(e => e.Line == line && e.Path == path);
            }
            else
            {
                var query = (from m in _dbContext.Mains where groupsNumbers.Contains(m.GroupId) select m.EquipmentId).Distinct();
                _line_path_objects = _dbContext.Equipments.Where(e => e.Line == line && e.Path == path)
                                                          .Where(e => query.Contains(e.Code));
            }

            return _line_path_objects;
        }

        long beforeCoordinate = Int64.MaxValue;
        long beforeCoordinateRangeLeft = 0;
        long beforeCoordinateRangeRight = 0;

        public IEnumerable<Registrator.DB.ResultEquipCode> get_objects_by_coordinate(long coordinate, long span)
        {
            return get_objects_by_coordinate_(coordinate, span, span);
        }
        public IEnumerable<Registrator.DB.ResultEquipCode> get_objects_by_coordinate(string line, string path, long coordinate, long span)
        {
            setLineAndPath(line, path);
            return get_objects_by_coordinate_(coordinate, span, span);
        }

        public IEnumerable<Registrator.DB.ResultEquipCode> get_objects_by_coordinate_(long coordinate, long leftRange, long rightRange)
        {
            if (beforeCoordinate != coordinate ||
                leftRange != beforeCoordinateRangeLeft ||
                rightRange!= beforeCoordinateRangeRight ||
                beforeCoordinate == Int64.MaxValue
                )
            {
                if (_line_path_objects == null)
                    return new List<Registrator.DB.ResultEquipCode>();

                long max_line_offset = coordinate + rightRange;
                long min_line_offset = coordinate - leftRange;


                var objects = from r in _line_path_objects
                              where r.shiftLine < max_line_offset && r.shiftLine > min_line_offset
                              select r;
                              //select new ResultEquipCode { Code = r.Code, name = r.name, shiftLine = r.shiftLine, X = r.X, Y = r.Y, curTemperature = r.curTemperature, maxTemperature = r.maxTemperature, shiftFromPicket = r.shiftFromPicket, Npicket = r.Npicket, picket = r.picket, Color = r.Color, EquipType = r.EquipType, objectLenght = r.objectLenght };


                m_objects_by_coordinate = (objects as IEnumerable<Registrator.DB.ResultEquipCode>);
                beforeCoordinate = coordinate;
                beforeCoordinateRangeLeft = leftRange;
                beforeCoordinateRangeRight = rightRange;
            }

            return m_objects_by_coordinate;
        }

        public EFClasses.Equipment GetObjectById(int id)
        {
            return (from e in _dbContext.Equipments where e.Code == id select e).Distinct().First();
        }


        IEnumerable<Registrator.DB.ResultEquipCode> m_objects_by_coordinate = new List<Registrator.DB.ResultEquipCode>();

        public List<int> groupsNumbers = new List<int>();

        public void retrieve_groups()
        {
            groupsNumbers.Clear();
            var filters = from r in _db.EquipmentFilter_Tbl.AsEnumerable() where r.apply == 1 select r;
            string str = null;

            foreach (var item in filters)
            {
                str += Convert.ToString(item.groups_Numbers);

                string[] strSplit = str.Split(';');

                foreach (string itemGroupNumber in strSplit)
                {
                    if (itemGroupNumber == "")
                        break;

                    int itemInt = Convert.ToInt32(itemGroupNumber);

                    groupsNumbers.Add(itemInt);
                }
            }
        }

        public bool GetPicketAndPicketOffset(ShotDesc desc, ref int picket, ref uint offsetFromPicket)
        {
            var resPickets = from r in _db.Pickets.AsEnumerable() where r.line == desc.Line && r.path != desc.Path select r;
            long lineOffset = _dbContext.Lines.Where(l => l.LineNum == desc.Line).Select(l=>l.StartCoordinate).FirstOrDefault();

            int beginPicketDlina = 0;
            int after = 0;
            int beginPicketNum = 0;

            foreach (var item in resPickets)
            {
                if (item.NpicketBefore == 0)
                {
                    beginPicketDlina = item.Dlina;
                    beginPicketNum = item.number;
                    after = item.NpicketAfter;
                }
            }

            long coordinate;

            coordinate = lineOffset + (long)beginPicketNum;

            if (coordinate >= desc.Distance && coordinate <= desc.Distance)
            {
                picket = beginPicketNum;
                coordinate -= beginPicketDlina;
                offsetFromPicket = (uint)(desc.Distance - coordinate);

                return true;
            }

            for (int i = 0; i < resPickets.Count(); i++)
            {
                foreach (var item in resPickets)
                {
                    if (item.NpicketAfter == after)
                    {
                        coordinate += item.Dlina;
                        after = item.NpicketAfter;

                        if (after == 0)
                        {
                            // ERROR picket not found
                            return false;
                        }

                        if (coordinate >= desc.Distance && coordinate <= desc.Distance)
                        {
                            picket = beginPicketNum;

                            coordinate -= item.Dlina;
                            offsetFromPicket = (uint)(desc.Distance - coordinate);

                            return true;
                        }
                    }
                }
            }
            return false;
        }

        private string  currentLine  = "";

        int mCurLineNum = -1;
        int mCurTrackNum = -1;

        public void clearCurrentPathANDLineValues()
        {
            currentLine  = "";
            current_path_Tag = "";
        }

        public bool setLineAndPath(string line, string path)
        {
            if(line != currentLine || path != current_path_Tag)
            {
                currentLine = line;
                current_path_Tag = path;

                int lineNumber = GetLineID(line);
                int trackID = GetTrackID(lineNumber,path);
                
                mCurLineNum = lineNumber;
                mCurTrackNum = trackID;

                m_pickets.Clear();

                if(lineNumber != -1 && trackID != -1)
                {
                    retrieve_groups();
                    get_objects(lineNumber, trackID);
                    
                }
                else
                {
                    //error
                }

                return true;
            }
            return false;
        }

        public List<Registrator.DB.PicketContainer> getPicketsForCurrentPath()
        {
            if (m_pickets.Count == 0)
            {
                var res = from r in _db.Pickets.AsEnumerable()
                          where r.line == mCurLineNum && r.path == mCurTrackNum
                          select new PicketContainer { Num = r.Npiketa, RigthShiftLine = r.EndShiftLine, LeftShiftLine = r.StartShiftLine, Length = r.Dlina };

                m_pickets = res.ToList();
            }
            return m_pickets;
        }

        public string GetCurrentPath()
        {
            return current_path_Tag;
        }

        List<Registrator.DB.PicketContainer> m_pickets = new List<Registrator.DB.PicketContainer>();

        public Area LoadArea(int object_id, DateTime dtime=new DateTime(), bool loadDefault=true)
        {
            try
            {
                if (loadDefault)
                {
                    return (from r in _dbContext.Equipments
                              where r.Code == object_id
                              select new Area(object_id, (Area.AreaType)r.Area_Type, r.Area_Height, r.Area_Width, r.Area_X, r.Area_Y)).Distinct().FirstOrDefault();
                }
                else
                    return (from r in _db.ObjectsFrames.AsEnumerable() ///TODO that is ObjectFrames table?
                              where r.ObjID == object_id
                              select new Area(object_id, (Area.AreaType)r.Area_Type, r.Area_Height, r.Area_Width, r.Area_X, r.Area_Y)).Distinct().FirstOrDefault();
            }
            catch(System.Data.StrongTypingException e)
            {
                throw new Exception("LoadArea exception");
            }
        }
    }
}

