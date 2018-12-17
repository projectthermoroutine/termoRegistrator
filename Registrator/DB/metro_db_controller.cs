using NLog;
using Registrator.DB.EFClasses;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace Registrator.DB
{
    using db_object_info = DB.EFClasses.AllEquipment;

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
        private static readonly Logger logger = LogManager.GetCurrentClassLogger();

        public metro_db_controller(metro_db_controller controller)
        {
            if (controller == null)
            {
                try
                {
                    _db = new DB.DataBaseHelper();
                    _dbContext = new ContextMetroCard();

                    //_dbContext.Database.Connection.Open();
                }
                catch (Exception e)
                {

                    logger.ErrorException("Exception in metro_db_controller: ", e);

                }
            }
            else
            {
                _db = controller._db;
                _dbContext = controller._dbContext;
            }
        }

        public List<EFClasses.ObjectsFrame> getObjMeasurements(int ObjID)  { return _dbContext.ObjectsFrames.Where(fr => fr.ObjID == ObjID).Distinct().ToList(); }

        public void addObjectTermogramme(int id, string path, long coordinate, DateTime dt)
        {
            try
            {
                queriesAdapter.insertRowInPassageTable(id, path, coordinate, dt);
            }
            catch (System.Data.SqlClient.SqlException e) {
                String str = e.Message;
            }
        }
        public string GetDBFilePath()
        {
            object obj = queriesAdapter.databaseFiles();
            string str = obj as string;
            str = str.Remove(str.LastIndexOf("\\"));
            //TODO error check
            return str;
        }

        private DataBaseHelper _db;
        private ContextMetroCard _dbContext;
        public ContextMetroCard dbContext { get { return _dbContext; } }

        //private int sampling_frequencies = 0;
        //private IEnumerable<Registrator.DB.ResultEquipCode> _line_path_objects = null;

        private List<db_object_info> _line_path_objects = null;

        public int? GetLineID(string line_code)
        {
           return _dbContext.Lines.Where(l => l.LineCode == line_code).FirstOrDefault()?.LineNum;
        }

        public int? GetTrackID(int line_id, string track_name)
        {
            return _dbContext.Tracks.Where(t => t.Track1 == track_name && t.LineId == line_id).FirstOrDefault()?.ID;
        }

        public IEnumerable<DB.EFClasses.EquipmentFilter_Tbl> GetFilters()
        {
            return _dbContext.EquipmentFilter_Tbl.OrderBy(e => e.filter_id);
        }

        private System.Data.DataTable get_table(string table_name)
        {
            return (System.Data.DataTable)_db.Tables[table_name];
        }

        private System.ComponentModel.Component get_table_adapter(table_index index)
        {
            return _db._adapters[(int)index];
        }

        public MetrocardDataSetTableAdapters.QueriesTableAdapter queriesAdapter { get { return (MetrocardDataSetTableAdapters.QueriesTableAdapter)get_table_adapter(table_index.Queries); } }

        public void SaveObjectTemperature(int object_id, float temperature)
        {
            queriesAdapter.insertEquipTemperature(object_id, (int)temperature);
        }

        public List<db_object_info> get_objects(int line, int path)
        {
            if (groupsNumbers.Count == 0) // filters disable
            {
                _line_path_objects = _dbContext.AllEquipments.Where(e => e.Line == line && e.Path == path).ToList();
            }
            else
            {
                //var query = (from m in _dbContext.Mains where groupsNumbers.Contains(m.GroupId) select m.EquipmentId).Distinct();
                var query = (from m in _dbContext.AllEquipments where groupsNumbers.Contains(m.Group) select m.Code).Distinct();

                _line_path_objects = _dbContext.AllEquipments.Where(e => e.Line == line && e.Path == path)
                                                          .Where(e => query.Contains(e.Code)).ToList();
            }

            return _line_path_objects;
        }

        long beforeCoordinate = Int64.MaxValue;
        long beforeCoordinateRangeLeft = 0;
        long beforeCoordinateRangeRight = 0;

        public List<db_object_info> get_objects_by_coordinate(long coordinate, long span = 0)
        {
            return get_objects_by_coordinate_(coordinate, span, span).ToList();
        }
        public List<db_object_info> get_objects_by_coordinate(string line, string path, long coordinate, long span)
        {
            setLineAndPath(line, path);
            return get_objects_by_coordinate_(coordinate, span, span).ToList();
        }

        public IEnumerable<db_object_info> get_objects_by_coordinate_(long coordinate, long leftRange, long rightRange)
        {
            if (leftRange == 0 && rightRange == 0)
                return _line_path_objects ?? new List<db_object_info>();

            if (beforeCoordinate != coordinate ||
                leftRange != beforeCoordinateRangeLeft ||
                rightRange!= beforeCoordinateRangeRight ||
                beforeCoordinate == Int64.MaxValue
                )
            {
                if (_line_path_objects == null)
                    return new List<db_object_info>();

                long max_line_offset = coordinate + rightRange;
                long min_line_offset = coordinate - leftRange;


                var objects = from r in _line_path_objects
                              where r.shiftLine < max_line_offset && r.shiftLine > min_line_offset
                              select r;
                              //select new ResultEquipCode { Code = r.Code, name = r.name, shiftLine = r.shiftLine, X = r.X, Y = r.Y, curTemperature = r.curTemperature, maxTemperature = r.maxTemperature, shiftFromPicket = r.shiftFromPicket, Npicket = r.Npicket, picket = r.picket, Color = r.Color, EquipType = r.EquipType, objectLenght = r.objectLenght };


                m_objects_by_coordinate = objects.ToList();
                beforeCoordinate = coordinate;
                beforeCoordinateRangeLeft = leftRange;
                beforeCoordinateRangeRight = rightRange;
            }

            return m_objects_by_coordinate;
        }

        public db_object_info GetObjectById(int id)
        {
            return (from e in _dbContext.AllEquipments where e.Code == id select e).Distinct().First();
        }


        List<db_object_info> m_objects_by_coordinate = new List<db_object_info>();

        public List<int> groupsNumbers = new List<int>();

        public void retrieve_groups()
        {
            groupsNumbers.Clear();

            var filters = _dbContext.EquipmentFilter_Tbl.Where(e => e.apply == 1);

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

        public Picket GetPicketInfo(int picket_id)
        {
            return _dbContext.Pickets.Find(picket_id);

        }
        public async Task<Picket> GetPicketInfoAsync(int picket_id)
        {
            return await _dbContext.Pickets.FindAsync(picket_id);

        }

        public bool GetPicketAndPicketOffset(ShotDesc desc, ref int picket, ref uint offsetFromPicket)
        {
            IQueryable<DB.EFClasses.Picket> resPickets = _dbContext.Pickets
                .Where(pk=> _dbContext.Tracks.Where(tr => tr.LineId == desc.Line).Select(tr => tr.ID)
                .Distinct()
                .Contains(pk.path));

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

        string currentLine  = "";
        string current_path_Tag = "";

        int line_id = -1;
        int path_id = -1;

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

                int lineNumber = GetLineID(line) ?? -1;
                if (lineNumber == -1)
                    return false;

                int trackID = GetTrackID(lineNumber, path) ?? -1;
                if (trackID == -1)
                    return false;

                line_id = lineNumber;
                path_id = trackID;

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

        public List<EFClasses.Picket> getPicketsForCurrentPath()
        {
            if (m_pickets.Count == 0)
                m_pickets = _dbContext.Pickets.Where(pk => pk.path == path_id).ToList();

            return m_pickets;
        }

        public string GetCurrentPath()
        {
            return current_path_Tag;
        }

        List<EFClasses.Picket> m_pickets = new List<EFClasses.Picket>();

        /// <summary>
        /// Returns area object constructed from database data.
        /// </summary>
        /// <param name="object_id"> equipment UID </param>
        /// <param name="dtime">Time of snapshot. The coordinates of the area are saved with each snapshot</param>
        /// <param name="loadDefault"> If true it trying to load area parameters from 'AllEquipment' table. If false from 'ObjectsFrames' </param>
        /// <returns></returns>
        public Area LoadArea(int object_id, DateTime dtime=new DateTime(), bool loadDefault=true)
        {
            try
            {
                if (loadDefault)
                {
                    AllEquipment allEquip = (from r in _dbContext.AllEquipments where r.Code == object_id select r).Distinct().FirstOrDefault();

                    return new Area(allEquip.Code, (Area.AreaType)allEquip.Area_Type, allEquip.Area_Height, allEquip.Area_Width, allEquip.Area_X, allEquip.Area_Y); 
                            
                }
                else
                {
                    return _dbContext.ObjectsFrames
                        .Where(e => e.ObjID == object_id)
                        .Select(e => new Area(object_id, (Area.AreaType)e.Area_Type, (double)e.Area_Height, (double)e.Area_Width, (double)e.Area_X, (double)e.Area_Y))
                        .Distinct()
                        .FirstOrDefault();
                }
            }
            catch(System.Data.StrongTypingException)
            {
                throw new Exception("LoadArea exception");
            }
        }
    }
}

