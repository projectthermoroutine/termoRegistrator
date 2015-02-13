using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ThermoRoutineLib;
using System.Drawing;
using System.Windows.Forms;

namespace Registrator.Equipment
{
    public class FrameChangedEventNEW : EventArgs
    {
        private int m_index;
        private bool m_displayNewObject;
        private int m_coord = 0;

        public FrameChangedEventNEW(int index, bool displayNewObjectArg, int coord)
            : base()
        {
            m_index = index;
            m_displayNewObject = displayNewObjectArg;
            m_coord = coord;
        }

        public int Index { get { return m_index; } set { m_index = value; } }
        public bool displayNewObject { get { return m_displayNewObject; } set { m_displayNewObject = value; } }
        public int Coord { get { return m_coord; } set { m_coord = value; } }
    }

    public class ProcessEquipment
    {

        public DB.DataBaseHelper DBHelper=null;
        public int curLine = 0;

        public int sampling_frequencies = 60;    //
        public int NEAR_DISTANCE = 3;            //meters
        private int coordinatPlusNearDistance;

        public int lastCoordinate = 0;
        private IEnumerable<Registrator.DB.ResultEquipCode> subquery;
        public  IEnumerable<Registrator.DB.ResultEquipCodeFrame> subqueryFrame;
      
        public int xPoint;
        public int yPoint;
        public int StartCoordinatX = 30;
        public int StartCoordinatY = 30;
        
        public int metrCoordinate = 0;
        public int tempCounter1 = 0;

        private DataGridView dataGridView_;
        //private Registrator.EquipmentMonitor.MyDelegateFrameProcess d;
        private Registrator.EquipmentMonitor.MyDelegateFrameProcessDataGrid dDataGrid;
        private Registrator.EquipmentMonitor.MyDelegateFrameProcessDataGridClearAll dDataGridClearAll;
        private Registrator.EquipmentMonitor.MyDelegateFrameProcessDataGridClear dDataGridClear;
        private IAsyncResult result;

        private int curMaxtemperature = 20;

        bool displayNewObject = false;

        public ProcessEquipment(ref DB.DataBaseHelper dbHelperArg)
        {
            DBHelper = dbHelperArg;
           
        }
        public void setLine(int line)
        {
            DBHelper.getLineObjects(line); 
            //subquery = from r in DBHelper.dataTable_ProcessEquipment.AsEnumerable() where r.LineNum == curLine && r.Code!=0 select new DB.ResultEquipCode { Code = r.Code, name=r.Object, shiftLine = r.shiftLine, X = r.x, Y = r.y , curTemperature = r.curTemperature, maxTemperature = r.maxTemperature, shiftFromPicket = r.shiftFromPicket, Npicket = r.Npicket };
        }
        public void setDataGrid(ref DataGridView dgArg)
        {
            dataGridView_ = dgArg;
        }
        public void setDelegat(/*ref Registrator.EquipmentMonitor.MyDelegateFrameProcess dArg,*/ ref Registrator.EquipmentMonitor.MyDelegateFrameProcessDataGrid dArg1, ref Registrator.EquipmentMonitor.MyDelegateFrameProcessDataGridClear dDataGridClearArg, Registrator.EquipmentMonitor.MyDelegateFrameProcessDataGridClearAll dDataGridClearAllArg)
        {
            //d = dArg;
            dDataGrid = dArg1;
            dDataGridClear = dDataGridClearArg;
            dDataGridClearAll = dDataGridClearAllArg;
        }


        public void refresh()      
        {
            if (dataGridView_ != null)
            {
                lastCoordinate = 0;
                dataGridView_.BeginInvoke(dDataGridClearAll);
            }
        }

      
        public void process(ref _irb_frame_info frameInfo)
        {

            displayNewObject = false;

#if DEBUG    // SET COORDINATE

            tempCounter1++;
            if (tempCounter1 > 3)
            {
                metrCoordinate++;
                tempCounter1 = 0;
            }
#else
            ulong ULONG_metrCoordinate = frameInfo.coordinate.coordinate / 1000;
            metrCoordinate = (int)ULONG_metrCoordinate;
#endif    

            if (lastCoordinate < metrCoordinate) 
            {
                lastCoordinate = metrCoordinate + sampling_frequencies; 

                DBHelper.getCoordinateObjects((int)lastCoordinate);

                dataGridView_.BeginInvoke(dDataGridClearAll); // CLEAR DATAGRID
                //result = dataGridView_.BeginInvoke(d, 1);
                //result.AsyncWaitHandle.WaitOne();
                
#if DEBUG       // SET TEMPERATURE
                if (curMaxtemperature > 50) curMaxtemperature = 20;
                curMaxtemperature++;
#else
                 curMaxtemperature = (int)frameInfo.measure.tmax;
#endif
                //  GET EQUIPMENT (for current coordinate sector)
                if (DBHelper.subqueryFrame.Count() > 0)
                {
                    lock (this)
                    {
                        foreach (var item in DBHelper.subqueryFrame)
                        {
                            //  SET equip to DATAGRID 
                            dataGridView_.BeginInvoke(dDataGrid, item.name, Convert.ToString(metrCoordinate), Convert.ToString(item.Npicket), Convert.ToString(curMaxtemperature), Convert.ToString(item.maxTemperature), Convert.ToString(item.shiftFromPicket));
                            //  INSERT MAX TEMPERATURE (for cur equip)
                            DBHelper.TblAdapter_ProcessEquipment.insertEquipTemperature(item.Code, item.curTemperature);            
                            displayNewObject = true;
                        }
                    }
                }
            }
            // DRAW equip ON TRACK CONTROL NEW
            FireFrameChangedEventNEW(new FrameChangedEventNEW(0, displayNewObject, metrCoordinate));
        }

        public event EventHandler<FrameChangedEventNEW> FrameChangedHandlerNEW;
        public virtual void FireFrameChangedEventNEW(FrameChangedEventNEW e)
        {
            EventHandler<FrameChangedEventNEW> handler = FrameChangedHandlerNEW;

            if (handler != null)
            {
                handler(this, e);
            }
        }
    }
}
