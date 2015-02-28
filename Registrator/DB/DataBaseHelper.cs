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
        public int shiftLine;
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
        public int shiftLine;
        public int X;
        public int Y;
        public int curTemperature;
        public int maxTemperature;
        public int Npicket;
        public int shiftFromPicket;
        public string Color;
    }

    public class DataBaseHelper
    {
        public System.ComponentModel.BackgroundWorker backgroundWorker1;    //TODO

        public MetrocardDataSet MCDS;
        //
        public MetrocardDataSetTableAdapters.processEquipmentDataTableTableAdapter TblAdapter_ProcessEquipment;
        public MetrocardDataSet.processEquipmentDataTableDataTable dataTable_ProcessEquipment;
        //
        public MetrocardDataSetTableAdapters.PicketsTableAdapter TblAdapter_Pickets;
        public MetrocardDataSet.PicketsDataTable dataTable_PicketsTable;
        //
        public MetrocardDataSetTableAdapters.EquipmentAllTableAdapter TblAdapter_AllEquipment; // AllEquipmentTable
        public MetrocardDataSet.EquipmentAllDataTable dataTable_AllEquipment;
        //
        public MetrocardDataSetTableAdapters.GroupTableAdapter TblAdapter_Group;        // Group Table
        public MetrocardDataSet.GroupDataTable dataTable_GroupTable;
        //
        public MetrocardDataSetTableAdapters.LayoutTableAdapter TblAdapter_Layout;      // Layout Table
        public MetrocardDataSet.LayoutDataTable dataTable_LayoutTable;
        //
        public MetrocardDataSetTableAdapters.MainTableAdapter TblAdapter_Main;          // Main Table
        public MetrocardDataSet.MainDataTable dataTable_Main;
        //
        public MetrocardDataSetTableAdapters.ObjectsTableAdapter TblAdapter_Objects;    // Objects Table
        public MetrocardDataSet.ObjectsDataTable dataTable_Objects;
        //
        public MetrocardDataSetTableAdapters.ClassTableAdapter TblAdapter_Class;        // Class Table
        public MetrocardDataSet.ClassDataTable dataTable_Class;
        //
        public MetrocardDataSetTableAdapters.OrdersTableAdapter TblAdapter_Orders;      // Orders Table
        public MetrocardDataSet.OrdersTableAdapterDataTable dataTable_Orders;
        //
        public MetrocardDataSetTableAdapters.LinesTableAdapter TblAdapter_Lines;        // Lines Table
        public MetrocardDataSet.LinesDataTable dataTable_Lines;
        //
        public MetrocardDataSetTableAdapters.EquipmentFilter_TblTableAdapter TblAdapter_EquipmentFilter; // Equipment Filter  Table
        public MetrocardDataSet.EquipmentFilter_TblDataTable dataTable_EquipmentFilter;
        //

        public void InitTableAdaptersAndDataTables()
        {
            MCDS = new MetrocardDataSet();
            //

            TblAdapter_ProcessEquipment = new MetrocardDataSetTableAdapters.processEquipmentDataTableTableAdapter(); 
            dataTable_ProcessEquipment = new MetrocardDataSet.processEquipmentDataTableDataTable();
            //
            TblAdapter_Pickets = new MetrocardDataSetTableAdapters.PicketsTableAdapter(); // Pickets
            dataTable_PicketsTable = new MetrocardDataSet.PicketsDataTable();
            //
            TblAdapter_AllEquipment = new MetrocardDataSetTableAdapters.EquipmentAllTableAdapter(); // AllEquipmentTable Table
            dataTable_AllEquipment = new MetrocardDataSet.EquipmentAllDataTable();
            //
            TblAdapter_Class = new MetrocardDataSetTableAdapters.ClassTableAdapter();       // Class Table
            dataTable_Class = new MetrocardDataSet.ClassDataTable();
            //
            TblAdapter_Group = new MetrocardDataSetTableAdapters.GroupTableAdapter();       // Group Table
            dataTable_GroupTable = new MetrocardDataSet.GroupDataTable();
            //
            TblAdapter_Layout = new MetrocardDataSetTableAdapters.LayoutTableAdapter();     // Layout Table
            dataTable_LayoutTable = new MetrocardDataSet.LayoutDataTable();
            //
            TblAdapter_Main = new MetrocardDataSetTableAdapters.MainTableAdapter();         // Main Table
            dataTable_Main = new MetrocardDataSet.MainDataTable();
            //
            TblAdapter_Objects = new MetrocardDataSetTableAdapters.ObjectsTableAdapter();   // Objects Table
            dataTable_Objects = new MetrocardDataSet.ObjectsDataTable();
            //
            TblAdapter_Orders = new MetrocardDataSetTableAdapters.OrdersTableAdapter();     // Orders Table
            dataTable_Orders = new MetrocardDataSet.OrdersTableAdapterDataTable();
            //
            TblAdapter_Lines = new MetrocardDataSetTableAdapters.LinesTableAdapter();       // Lines Table
            dataTable_Lines = new MetrocardDataSet.LinesDataTable();
            //
            TblAdapter_EquipmentFilter = new MetrocardDataSetTableAdapters.EquipmentFilter_TblTableAdapter();   // Equipment Filter Table
            dataTable_EquipmentFilter = new MetrocardDataSet.EquipmentFilter_TblDataTable();
            //
        }

        public void fillDataTables()
        {
            TblAdapter_ProcessEquipment.Fill(dataTable_ProcessEquipment);
            TblAdapter_Pickets.Fill(dataTable_PicketsTable);
            TblAdapter_AllEquipment.Fill(dataTable_AllEquipment);
            TblAdapter_Class.Fill(dataTable_Class);
            TblAdapter_Group.Fill(dataTable_GroupTable);
            TblAdapter_Layout.Fill(dataTable_LayoutTable);
            TblAdapter_Main.Fill(dataTable_Main);
            TblAdapter_Objects.Fill(dataTable_Objects);
            TblAdapter_Lines.Fill(dataTable_Lines);
            TblAdapter_EquipmentFilter.Fill(dataTable_EquipmentFilter);
        }
        public void refresh()
        {
            dataTable_ProcessEquipment.Clear();
            dataTable_PicketsTable.Clear();
            dataTable_AllEquipment.Clear();
            dataTable_Class.Clear();
            dataTable_GroupTable.Clear();
            dataTable_LayoutTable.Clear();
            dataTable_Main.Clear();
            dataTable_Objects.Clear();
            dataTable_Lines.Clear();
            dataTable_EquipmentFilter.Clear();

            TblAdapter_ProcessEquipment.Fill(dataTable_ProcessEquipment);
            TblAdapter_Pickets.Fill(dataTable_PicketsTable);
            TblAdapter_AllEquipment.Fill(dataTable_AllEquipment);
            TblAdapter_Class.Fill(dataTable_Class);
            TblAdapter_Group.Fill(dataTable_GroupTable);
            TblAdapter_Layout.Fill(dataTable_LayoutTable);
            TblAdapter_Main.Fill(dataTable_Main);
            TblAdapter_Objects.Fill(dataTable_Objects);
            TblAdapter_Lines.Fill(dataTable_Lines);
            TblAdapter_EquipmentFilter.Fill(dataTable_EquipmentFilter);
        }

        public int curLine = 0;
        public IEnumerable<Registrator.DB.ResultEquipCode> subquery;
        public IEnumerable<Registrator.DB.ResultEquipCodeFrame> subqueryFrame;
        private int coordinatPlusNearDistance = 0;
        private int coordinat = 0;
        private int NEAR_DISTANCE = 0;
        private int sampling_frequencies = 0;

        public void setWorkingAreaEquipmentMonitor()
        {
            coordinatPlusNearDistance = coordinat + NEAR_DISTANCE;

        }

        public void getLineObjects(int line)
        {
            curLine = line;

            if (groupsNumbers.Count == 0) // filters disable
                subquery = (from r in dataTable_ProcessEquipment.AsEnumerable() where r.LineNum == curLine && r.Code != 0  select new ResultEquipCode { Code = r.Code, name = r.Object, shiftLine = r.shiftLine, X = r.x, Y = r.y, curTemperature = r.curTemperature, maxTemperature = r.maxTemperature, shiftFromPicket = r.shiftFromPicket, Npicket = r.Npicket, GroupCode = r.GroupNum, Color=r.Color });
            else
                subquery = (from r in dataTable_ProcessEquipment.AsEnumerable() where r.LineNum == curLine && r.Code != 0 && groupsNumbers.Contains(r.GroupNum) select new ResultEquipCode { Code = r.Code, name = r.Object, shiftLine = r.shiftLine, X = r.x, Y = r.y, curTemperature = r.curTemperature, maxTemperature = r.maxTemperature, shiftFromPicket = r.shiftFromPicket, Npicket = r.Npicket, GroupCode = r.GroupNum, Color = r.Color });
        }

        public void getCoordinateObjects(int coordinate)
        {
            int tmp = coordinate + 30;
            subqueryFrame = from r in subquery where r.shiftLine >= coordinate - 30 && r.shiftLine <= coordinate+30 select new ResultEquipCodeFrame { Code = r.Code, name = r.name, shiftLine = r.shiftLine, X = r.X, Y = r.Y, curTemperature = r.curTemperature, maxTemperature = r.maxTemperature, shiftFromPicket = r.shiftFromPicket, Npicket = r.Npicket, Color=r.Color };
        }
     
        // TODO
       // class EquipFilter_Helper
       // {
        public List<int> groupsNumbers = new List<int>();

        public void fill_Equip_Filter_Object()
        {
            groupsNumbers.Clear();

            var resFiltesrs = from r in dataTable_EquipmentFilter.AsEnumerable() where r.apply == 1  select r;

            string str = null;

            foreach(var item in resFiltesrs)
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
       // }
    }
}
