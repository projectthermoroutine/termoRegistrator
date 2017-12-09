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
        public string picket;
        public int Npicket;
        public int shiftFromPicket;
        public int EquipType;
        public string Color;
        public int objectLenght = 0;
    }

    public class PicketContainer
    {
        public string Num;
        public int RigthShiftLine;
        public int LeftShiftLine;
        public int Length;
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
        EquipmentFilter,
        Passages,
        Queries,
        Track,
        ObjectsFrames

    };

    internal class DataBaseHelper : MetrocardDataSet
    {
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
        //
        public MetrocardDataSetTableAdapters.PassagesTableAdapter TblAdapter_Passages;

        public MetrocardDataSetTableAdapters.QueriesTableAdapter QueriesTblAdapter;

        public MetrocardDataSetTableAdapters.TrackTableAdapter TrackTblAdapter;

        public MetrocardDataSetTableAdapters.ObjectsFramesTableAdapter ObjectsFramesTblAdapter;

        public List<System.ComponentModel.Component> _adapters;
        

        public DataBaseHelper()
            : base()
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
            TblAdapter_Passages = new MetrocardDataSetTableAdapters.PassagesTableAdapter();
            _adapters.Add(TblAdapter_Passages);

            QueriesTblAdapter = new MetrocardDataSetTableAdapters.QueriesTableAdapter();
            _adapters.Add(QueriesTblAdapter);

            TrackTblAdapter = new MetrocardDataSetTableAdapters.TrackTableAdapter();
            _adapters.Add(TrackTblAdapter);

            ObjectsFramesTblAdapter = new MetrocardDataSetTableAdapters.ObjectsFramesTableAdapter();
            _adapters.Add(ObjectsFramesTblAdapter);
        }

        public void refresh_passages_table()
        {
            Passages.Clear();
            TblAdapter_Passages.Fill(Passages);
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
            Track.Clear();

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
            TrackTblAdapter.Fill(Track);

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
        public int percent { get; set; }
        public string description { get; set; }
    }

}
