using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Linq.Expressions;

namespace Registrator.DB
{
    public class ResultEquipCode
    {
        public int Code;
        public string name;
        public long shiftLine;
        public int X;
        public int Y;
        public int curTemperature;
        public int maxTemperature;
        public int Npicket;
        public int shiftFromPicket;
        public int GroupCode;
        public string Color;
    }
    public class ResultEquipCodeFrame
    {
        public int Code;
        public string name;
        public long shiftLine;
        public int X;
        public int Y;
        public int curTemperature;
        public int maxTemperature;
        public int Npicket;
        public int shiftFromPicket;
        public string Color;
    }



        internal enum table_index
        {
            ProcessEquipment,
            Pickets,
            AllEquipment,
            Groups,
            Layout,
            Main,
            Objects,
            Classes,
            Orders,
            Lines,
            EquipmentFilter
        };

    internal class DataBaseHelper : MetrocardDataSet
    {

        public System.ComponentModel.BackgroundWorker backgroundWorker1;    //TODO

        //
        public MetrocardDataSetTableAdapters.processEquipmentDataTableTableAdapter TblAdapter_ProcessEquipment;
        //
        public MetrocardDataSetTableAdapters.PicketsTableAdapter TblAdapter_Pickets;
        //
        public MetrocardDataSetTableAdapters.EquipmentAllTableAdapter TblAdapter_AllEquipment; // AllEquipmentTable
        //
        public MetrocardDataSetTableAdapters.GroupTableAdapter TblAdapter_Group;        // Group Table
        //
        public MetrocardDataSetTableAdapters.LayoutTableAdapter TblAdapter_Layout;      // Layout Table
        //
        public MetrocardDataSetTableAdapters.MainTableAdapter TblAdapter_Main;          // Main Table
        //
        public MetrocardDataSetTableAdapters.ObjectsTableAdapter TblAdapter_Objects;    // Objects Table
        //
        public MetrocardDataSetTableAdapters.ClassTableAdapter TblAdapter_Class;        // Class Table
        //
        public MetrocardDataSetTableAdapters.OrdersTableAdapter TblAdapter_Orders;      // Orders Table
        //
        public MetrocardDataSetTableAdapters.LinesTableAdapter TblAdapter_Lines;        // Lines Table
        //
        public MetrocardDataSetTableAdapters.EquipmentFilter_TblTableAdapter TblAdapter_EquipmentFilter; // Equipment Filter  Table
        public MetrocardDataSet.EquipmentFilter_TblDataTable dataTable_EquipmentFilter;
        //

        public List<System.ComponentModel.Component> _adapters;


        public DataBaseHelper():base()
        {
            this.InitTableAdapters();
        }

        private void InitTableAdapters()
        {
            _adapters = new List<System.ComponentModel.Component>();
            //

            TblAdapter_ProcessEquipment = new MetrocardDataSetTableAdapters.processEquipmentDataTableTableAdapter();
            _adapters.Add(TblAdapter_ProcessEquipment);
            //
            TblAdapter_Pickets = new MetrocardDataSetTableAdapters.PicketsTableAdapter(); // Pickets
            _adapters.Add(TblAdapter_Pickets);
            //
            TblAdapter_AllEquipment = new MetrocardDataSetTableAdapters.EquipmentAllTableAdapter(); // AllEquipmentTable Table
            _adapters.Add(TblAdapter_AllEquipment);
            //
            TblAdapter_Group = new MetrocardDataSetTableAdapters.GroupTableAdapter();       // Group Table
            _adapters.Add(TblAdapter_Group);
            //
            TblAdapter_Layout = new MetrocardDataSetTableAdapters.LayoutTableAdapter();     // Layout Table
            _adapters.Add(TblAdapter_Layout);
            //
            TblAdapter_Main = new MetrocardDataSetTableAdapters.MainTableAdapter();         // Main Table
            _adapters.Add(TblAdapter_Main);
            //
            TblAdapter_Objects = new MetrocardDataSetTableAdapters.ObjectsTableAdapter();   // Objects Table
            _adapters.Add(TblAdapter_Objects);
            //
            TblAdapter_Class = new MetrocardDataSetTableAdapters.ClassTableAdapter();       // Class Table
            _adapters.Add(TblAdapter_Class);
            //
            TblAdapter_Orders = new MetrocardDataSetTableAdapters.OrdersTableAdapter();     // Orders Table
            _adapters.Add(TblAdapter_Orders);
            //
            TblAdapter_Lines = new MetrocardDataSetTableAdapters.LinesTableAdapter();       // Lines Table
            _adapters.Add(TblAdapter_Lines);
            //
            TblAdapter_EquipmentFilter = new MetrocardDataSetTableAdapters.EquipmentFilter_TblTableAdapter();   // Equipment Filter Table
            _adapters.Add(TblAdapter_EquipmentFilter);
            //
        }

        public void fillDataTables()
        {
            TblAdapter_ProcessEquipment.Fill(processEquipmentDataTable);
            TblAdapter_Pickets.Fill(Pickets);
            TblAdapter_AllEquipment.Fill(EquipmentAll);
            TblAdapter_Class.Fill(Class);
            TblAdapter_Group.Fill(Group);
            TblAdapter_Layout.Fill(Layout);
            TblAdapter_Main.Fill(Main);
            TblAdapter_Objects.Fill(Objects);
            TblAdapter_Lines.Fill(Lines);
            TblAdapter_EquipmentFilter.Fill(EquipmentFilter_Tbl);
        }
        public void refresh()
        {
            processEquipmentDataTable.Clear();
            Pickets.Clear();
            EquipmentAll.Clear();
            Class.Clear();
            Group.Clear();
            Layout.Clear();
            Main.Clear();
            Objects.Clear();
            Lines.Clear();
            EquipmentFilter_Tbl.Clear();

            TblAdapter_ProcessEquipment.Fill(processEquipmentDataTable);
            TblAdapter_Pickets.Fill(Pickets);
            TblAdapter_AllEquipment.Fill(EquipmentAll);
            TblAdapter_Class.Fill(Class);
            TblAdapter_Group.Fill(Group);
            TblAdapter_Layout.Fill(Layout);
            TblAdapter_Main.Fill(Main);
            TblAdapter_Objects.Fill(Objects);
            TblAdapter_Lines.Fill(Lines);
            TblAdapter_EquipmentFilter.Fill(EquipmentFilter_Tbl);
        }
    }


    public class LoadingProgressEvent : EventArgs
    {
        public LoadingProgressEvent(int percent, string desc)
            : base()
        {
            this.percent = percent;
            this.description = desc;
        }
        public int percent {get;set;}
        public string description {get;set;}
    }


    public class metro_db_controller
    {
        public metro_db_controller(metro_db_controller controller)
        {
            if (controller == null)
            {
                _db = new DB.DataBaseHelper();

                if (LoadingProgressChanged != null)
                    LoadingProgressChanged(this, new LoadingProgressEvent(0, "start tables loading"));

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
                    LoadingProgressChanged(this, new LoadingProgressEvent(100, "tables loading"));
            }
            else
            {
                _db = controller._db;
            }
        }

        static public event EventHandler<LoadingProgressEvent> LoadingProgressChanged;

        private DataBaseHelper _db;

        public int current_line = -1;
        public int current_path = -1;
        private int coordinatPlusNearDistance = 0;
        private int coordinat = 0;
        private int NEAR_DISTANCE = 0;
        private int sampling_frequencies = 0;
        private IEnumerable<Registrator.DB.ResultEquipCode> _line_path_objects = null;


        public int get_line_number(string line_code)
        {
            var rows = (from r in _db.Lines.AsEnumerable() where r.LineCode == line_code select new { r.LineNum });
            if (rows.Count() != 0)
                return rows.First().LineNum;
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
            return from r in _db.dataTable_EquipmentFilter.AsEnumerable() orderby r.filter_id select r;
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
            if (line == current_line &&
                path == current_path &&
                _line_path_objects != null)
                return _line_path_objects;

            current_line = line;
            current_path = path;
            
            if (groupsNumbers.Count == 0) // filters disable
                _line_path_objects = from r in _db.processEquipmentDataTable.AsEnumerable() 
                                      where r.LineNum == current_line && r.Track == current_path && r.Code != 0 
                                      select new ResultEquipCode { Code = r.Code, name = r.Object, shiftLine = r.shiftLine, X = r.x, Y = r.y, curTemperature = r.curTemperature, maxTemperature = r.maxTemperature, shiftFromPicket = r.shiftFromPicket, Npicket = r.Npicket, GroupCode = r.GroupNum, Color = r.Color };
            else
                _line_path_objects = from r in _db.processEquipmentDataTable.AsEnumerable() 
                                      where r.LineNum == current_line && r.Track == current_path && r.Code != 0 && groupsNumbers.Contains(r.GroupNum) 
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
           
            return objects;
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


    }


}
