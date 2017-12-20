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
        public MetrocardDataSetTableAdapters.PicketsTableAdapter TblAdapter_Pickets;
        //
        //public MetrocardDataSetTableAdapters.EquipmentAllTableAdapter TblAdapter_AllEquipment; // AllEquipmentTable
        //
        public MetrocardDataSetTableAdapters.OrdersTableAdapter TblAdapter_Orders;      // Orders Table
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
            TblAdapter_Pickets = new MetrocardDataSetTableAdapters.PicketsTableAdapter(); // Pickets
            _adapters.Add(TblAdapter_Pickets);
            //
            TblAdapter_Orders = new MetrocardDataSetTableAdapters.OrdersTableAdapter();     // Orders Table
            _adapters.Add(TblAdapter_Orders);
            //
            TblAdapter_EquipmentFilter = new MetrocardDataSetTableAdapters.EquipmentFilter_TblTableAdapter();   // Equipment Filter Table
            _adapters.Add(TblAdapter_EquipmentFilter);
            //
            TblAdapter_Passages = new MetrocardDataSetTableAdapters.PassagesTableAdapter();
            _adapters.Add(TblAdapter_Passages);

            QueriesTblAdapter = new MetrocardDataSetTableAdapters.QueriesTableAdapter();
            _adapters.Add(QueriesTblAdapter);

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
            Pickets.Clear();
            EquipmentFilter_Tbl.Clear();

            TblAdapter_Pickets.Fill(Pickets);
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
        public int percent { get; set; }
        public string description { get; set; }
    }

}
