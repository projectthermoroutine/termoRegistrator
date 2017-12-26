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
        Queries,
    };

    internal class DataBaseHelper : MetrocardDataSet
    {
        public MetrocardDataSetTableAdapters.QueriesTableAdapter QueriesTblAdapter;
        public List<System.ComponentModel.Component> _adapters;

        public DataBaseHelper()
            : base()
        {
            this.InitTableAdapters();
        }

        private void InitTableAdapters()
        {
            _adapters = new List<System.ComponentModel.Component>();

            QueriesTblAdapter = new MetrocardDataSetTableAdapters.QueriesTableAdapter();
            _adapters.Add(QueriesTblAdapter);
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
