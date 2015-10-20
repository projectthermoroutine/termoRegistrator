using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace Registrator.DB
{
    public class metro_db_controller
    {
        public metro_db_controller(metro_db_controller controller)
        {
            if (controller == null)
            {
                _db = new DB.DataBaseHelper();

                //queriesAdapter.insertRowInPassageTable("Table_3", 0 , 0,"",0,0, DateTime.Now);

                if (LoadingProgressChanged != null)
                    LoadingProgressChanged(this, new LoadingProgressEvent(0, "start tables loading"));

                _db.TrackTblAdapter.Fill(_db.Track);
                if (LoadingProgressChanged != null)
                    LoadingProgressChanged(this, new LoadingProgressEvent(5, "tables loading"));
                _db.TblAdapter_ProcessEquipment.Fill(_db.processEquipmentDataTable);
                if (LoadingProgressChanged != null)
                    LoadingProgressChanged(this, new LoadingProgressEvent(10, "tables loading"));
                _db.TblAdapter_Pickets.Fill(_db.Pickets);
                if (LoadingProgressChanged != null)
                    LoadingProgressChanged(this, new LoadingProgressEvent(20, "tables loading"));
                _db.TblAdapter_AllEquipment.Fill(_db.EquipmentAll);
                if (LoadingProgressChanged != null)
                    LoadingProgressChanged(this, new LoadingProgressEvent(30, "tables loading"));
                _db.TblAdapter_Class.Fill(_db.Class);
                if (LoadingProgressChanged != null)
                    LoadingProgressChanged(this, new LoadingProgressEvent(40, "tables loading"));
                _db.TblAdapter_Group.Fill(_db.Group);
                if (LoadingProgressChanged != null)
                    LoadingProgressChanged(this, new LoadingProgressEvent(50, "tables loading"));
                _db.TblAdapter_Layout.Fill(_db.Layout);
                if (LoadingProgressChanged != null)
                    LoadingProgressChanged(this, new LoadingProgressEvent(60, "tables loading"));
                _db.TblAdapter_Main.Fill(_db.Main);
                if (LoadingProgressChanged != null)
                    LoadingProgressChanged(this, new LoadingProgressEvent(70, "tables loading"));
                _db.TblAdapter_Objects.Fill(_db.Objects);
                if (LoadingProgressChanged != null)
                    LoadingProgressChanged(this, new LoadingProgressEvent(80, "tables loading"));
                _db.TblAdapter_Lines.Fill(_db.Lines);
                if (LoadingProgressChanged != null)
                    LoadingProgressChanged(this, new LoadingProgressEvent(90, "tables loading"));
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
        public void addObjectTermogramme(int id,string path,long coordinate, DateTime dt)
        {
            queriesAdapter.insertRowInPassageTable(id, path, coordinate, dt);
        }
        public string getDBFilePath()
        {
            object obj =  queriesAdapter.databaseFiles(); 
            string str = obj as string;
            str = str.Remove(str.LastIndexOf("\\"));
            //TODO error check
            return str;
        }

        static public event EventHandler<LoadingProgressEvent> LoadingProgressChanged;

        private DataBaseHelper _db;

        public int current_line_ID = -1;
        public int current_path_ID = -1;
        public string current_path = "";
        private int coordinatPlusNearDistance = 0;
        private int coordinat = 0;
        private int NEAR_DISTANCE = 0;
        //private int sampling_frequencies = 0;
        private IEnumerable<Registrator.DB.ResultEquipCode> _line_path_objects = null;
        
        private IEnumerable<Registrator.DB.ResultEquipCode>[] _line_path_objects_array = null;

        public int get_line_ID(string line_code)
        {
            var rows = (from r in _db.Lines.AsEnumerable() where r.LineCode == line_code select new { r.LineNum });
            if (rows.Count() != 0)
                return rows.First().LineNum;
            return -1;
        }
        public int get_track_ID(string track_name)
        {
            var rows = (from r in _db.Track.AsEnumerable() where r.Track == track_name select new { r.ID });
            
            if (rows.Count() != 0)
                return rows.First().ID;
            
            return -1;

        }
        public MetrocardDataSet.LinesRow get_line(int line_number)
        {
            var rows = (from r in _db.Lines.AsEnumerable() where r.LineNum == line_number select r);
            if (rows.Count() != 0)
                return rows.First();
            return null;
        }

        public IEnumerable<MetrocardDataSet.ClassRow> get_classes()
        {
            return from r in _db.Class.AsEnumerable() select r;
        }

        public IEnumerable<MetrocardDataSet.EquipmentFilter_TblRow> get_filters()
        {
            return from r in _db.EquipmentFilter_Tbl.AsEnumerable() orderby r.filter_id select r;
        }


        private System.Data.DataTable get_table(string table_name)
        {
            return (System.Data.DataTable)_db.Tables[table_name];
        }


        public MetrocardDataSet.processEquipmentDataTableDataTable process_equipment_table { get { return (MetrocardDataSet.processEquipmentDataTableDataTable)get_table("processEquipmentDataTable"); } }
        public MetrocardDataSet.OrdersTableAdapterDataTable orders_table { get { return (MetrocardDataSet.OrdersTableAdapterDataTable)get_table("OrdersTableAdapter"); } }
        public MetrocardDataSet.ObjectsDataTable objects_table { get { return (MetrocardDataSet.ObjectsDataTable)get_table("Objects"); } }
        public MetrocardDataSet.ClassDataTable classes_table { get { return (MetrocardDataSet.ClassDataTable)get_table("Class"); } }
        public MetrocardDataSet.LinesDataTable lines_table { get { return (MetrocardDataSet.LinesDataTable)get_table("Lines"); } }
        public MetrocardDataSet.LayoutDataTable layout_table { get { return (MetrocardDataSet.LayoutDataTable)get_table("Layout"); } }
        public MetrocardDataSet.PicketsDataTable pickets_table { get { return (MetrocardDataSet.PicketsDataTable)get_table("Pickets"); } }
        public MetrocardDataSet.EquipmentAllDataTable all_equipment_table { get { return (MetrocardDataSet.EquipmentAllDataTable)get_table("EquipmentAll"); } }
        public MetrocardDataSet.GroupDataTable groups_table { get { return (MetrocardDataSet.GroupDataTable)get_table("Group"); } }
        public MetrocardDataSet.EquipmentFilter_TblDataTable equipment_filter_table { get { return (MetrocardDataSet.EquipmentFilter_TblDataTable)get_table("EquipmentFilter_Tbl"); } }
        public MetrocardDataSet.PassagesDataTable passagesTable { get { return (MetrocardDataSet.PassagesDataTable)get_table("Passages"); } }
        public MetrocardDataSet.TrackDataTable trackTable { get { return (MetrocardDataSet.TrackDataTable)get_table("Track"); } }
       

        private System.ComponentModel.Component get_table_adapter(table_index index)
        {
            return _db._adapters[(int)index];
        }

        public MetrocardDataSetTableAdapters.processEquipmentDataTableTableAdapter process_equipment_adapter { get { return (MetrocardDataSetTableAdapters.processEquipmentDataTableTableAdapter)get_table_adapter(table_index.ProcessEquipment); } }
        public MetrocardDataSetTableAdapters.OrdersTableAdapter orders_adapter { get { return (MetrocardDataSetTableAdapters.OrdersTableAdapter)get_table_adapter(table_index.Orders); } }
        public MetrocardDataSetTableAdapters.ObjectsTableAdapter objects_adapter { get { return (MetrocardDataSetTableAdapters.ObjectsTableAdapter)get_table_adapter(table_index.Objects); } }
        public MetrocardDataSetTableAdapters.ClassTableAdapter classes_adapter { get { return (MetrocardDataSetTableAdapters.ClassTableAdapter)get_table_adapter(table_index.Classes); } }
        public MetrocardDataSetTableAdapters.LinesTableAdapter lines_adapter { get { return (MetrocardDataSetTableAdapters.LinesTableAdapter)get_table_adapter(table_index.Lines); } }
        public MetrocardDataSetTableAdapters.LayoutTableAdapter layout_adapter { get { return (MetrocardDataSetTableAdapters.LayoutTableAdapter)get_table_adapter(table_index.Layout); } }
        public MetrocardDataSetTableAdapters.PicketsTableAdapter pickets_adapter { get { return (MetrocardDataSetTableAdapters.PicketsTableAdapter)get_table_adapter(table_index.Pickets); } }
        public MetrocardDataSetTableAdapters.EquipmentAllTableAdapter all_equipment_adapter { get { return (MetrocardDataSetTableAdapters.EquipmentAllTableAdapter)get_table_adapter(table_index.AllEquipment); } }
        public MetrocardDataSetTableAdapters.GroupTableAdapter groups_adapter { get { return (MetrocardDataSetTableAdapters.GroupTableAdapter)get_table_adapter(table_index.Groups); } }
        public MetrocardDataSetTableAdapters.EquipmentFilter_TblTableAdapter equipment_filter_adapter { get { return (MetrocardDataSetTableAdapters.EquipmentFilter_TblTableAdapter)get_table_adapter(table_index.EquipmentFilter); } }
        public MetrocardDataSetTableAdapters.PassagesTableAdapter passagesAdapter { get { return (MetrocardDataSetTableAdapters.PassagesTableAdapter)get_table_adapter(table_index.Passages); } }
        public MetrocardDataSetTableAdapters.QueriesTableAdapter queriesAdapter { get { return (MetrocardDataSetTableAdapters.QueriesTableAdapter)get_table_adapter(table_index.Queries); } }
        public MetrocardDataSetTableAdapters.TrackTableAdapter trackAdapter {get{return(MetrocardDataSetTableAdapters.TrackTableAdapter)get_table_adapter(table_index.Track);} }


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

        public void save_object_temperature(int object_id, float temperature)
        {
            _db.TblAdapter_ProcessEquipment.insertEquipTemperature(object_id, (int)temperature);
        }

        public IEnumerable<Registrator.DB.ResultEquipCode> get_objects(int line, int path)
        {
            /*if (line == current_line_ID &&
                path == current_path &&
                _line_path_objects != null)
                return _line_path_objects;

            current_line_ID = line;
            current_path = path;*/

            if (groupsNumbers.Count == 0) // filters disable
                _line_path_objects = from r in _db.processEquipmentDataTable.AsEnumerable()
                                     where r.LineNum == line && r.Track == path && r.Code != 0
                                     select new ResultEquipCode { Code = r.Code, name = r.Object, shiftLine = r.shiftLine, X = r.x, Y = r.y, curTemperature = r.curTemperature, maxTemperature = r.maxTemperature, shiftFromPicket = r.shiftFromPicket, Npicket = r.Npicket, GroupCode = r.GroupNum, Color = r.Color };
            else
                _line_path_objects = from r in _db.processEquipmentDataTable.AsEnumerable()
                                     where r.LineNum == line && r.Track == path && r.Code != 0 && groupsNumbers.Contains(r.GroupNum)
                                     select new ResultEquipCode { Code = r.Code, name = r.Object, shiftLine = r.shiftLine, X = r.x, Y = r.y, curTemperature = r.curTemperature, maxTemperature = r.maxTemperature, shiftFromPicket = r.shiftFromPicket, Npicket = r.Npicket, GroupCode = r.GroupNum, Color = r.Color };

            return _line_path_objects;
        }

        public IEnumerable<Registrator.DB.ResultEquipCodeFrame> get_objects_by_coordinate(long coordinate, long camera_range_view)
        {
            if (_line_path_objects == null)
                return new List<Registrator.DB.ResultEquipCodeFrame>();

            long max_line_offset = coordinate + camera_range_view;
            long min_line_offset = coordinate - camera_range_view;

            var objects = from r in _line_path_objects
                          where r.shiftLine < max_line_offset && r.shiftLine > min_line_offset
                          select new ResultEquipCodeFrame { Code = r.Code, name = r.name, shiftLine = r.shiftLine, X = r.X, Y = r.Y, curTemperature = r.curTemperature, maxTemperature = r.maxTemperature, shiftFromPicket = r.shiftFromPicket, Npicket = r.Npicket, Color = r.Color };


            IEnumerable<Registrator.DB.ResultEquipCodeFrame> res = objects as IEnumerable<Registrator.DB.ResultEquipCodeFrame>;

            return res;
        }

        public IEnumerable<Registrator.DB.ResultEquipCodeFrame> getCoordinateObjectsDuration(long coordinate, long camera_range_view, long LineLen)
        {
            if (_line_path_objects == null)
                return new List<Registrator.DB.ResultEquipCodeFrame>();

            long max_line_offset = coordinate < camera_range_view ? LineLen : LineLen - coordinate + camera_range_view;
            long min_line_offset = LineLen < coordinate + camera_range_view * 5 ? 0 : LineLen - coordinate - camera_range_view * 5;

            var objects = from r in _line_path_objects
                          where r.shiftLine < max_line_offset && r.shiftLine > min_line_offset
                          select new ResultEquipCodeFrame { Code = r.Code, name = r.name, shiftLine = r.shiftLine, X = r.X, Y = r.Y, curTemperature = r.curTemperature, maxTemperature = r.maxTemperature, shiftFromPicket = r.shiftFromPicket, Npicket = r.Npicket, Color = r.Color };

            return objects;
        }


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

        public bool getPicketAndPicketOffset(ShotDesc desc, ref int picket, ref uint offsetFromPicket)
        {
            var resPickets = from r in _db.Pickets.AsEnumerable() where r.line == desc.Line && r.path != desc.Path select r;
            var resLineStartCoordinate = from r in _db.Lines.AsEnumerable() where r.LineNum == desc.Line select new { r.StartCoordinate };

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

            coordinate = (long)resLineStartCoordinate.First().StartCoordinate + (long)beginPicketNum;

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
        private int     currentPath = -1;

        public void clearCurrentPathANDLineValues()
        {
            currentLine  = "";
            current_path = "";
        }

        public void setLineAndPath(string line, string path)
        {
            if(line != currentLine || path != current_path)
            {
                currentLine = line;
                current_path = path;

                int lineNumber = get_line_ID(line);
                int trackID = get_track_ID(path);

                if(lineNumber != -1 && trackID != -1)
                {
                    retrieve_groups();
                    get_objects(lineNumber, trackID);
                }
                else
                {
                    //error
                }
            }
        }
    }
}

