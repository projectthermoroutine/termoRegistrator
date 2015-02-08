using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Registrator
{
    public partial class EquipmentFilterTool : Form
    {

        enum Objects
        {
            MAPMS_PICK,       
            MAPMS_STATION,    
            MAPMS_RZ,         
            MAPMS_PKO,        
            MAPMS_GSTAT,      
            MAPMS_STOP1,      
            MAPMS_STRELKA,    
            MAPMS_SPATH,      
            MAPMS_PPATH,      
            MAPMS_SEMAFOR,     
            MAPMS_PICKBORDER, 
            MAPMS_STARTPOINT, 
            MAPMS_MEASPOINT,   
            MAPMS_MEASFINISH,   
            MAPMS_LR,          
            MAPMS_KP,          
            MAPMS_PJA,         
            MAPMS_DT,          
            MAPMS_NUM
        }

        UInt32 m_objFilter = 0;

        public event EventHandler<ObjectFilterSetEvent> ObjectFilterSetEventHandler;

        public EquipmentFilterTool()
        {
            InitializeComponent();
        }

        public EquipmentFilterTool(UInt32 objFilter)
        {
            m_objFilter = objFilter;
            InitializeComponent();
            InitFilter();
        }

        public void InitFilter()
        {

            picketCheckBox.Checked = (m_objFilter & (1 << (int)Objects.MAPMS_PICK)) == (1 << (int)Objects.MAPMS_PICK);
            stationCheckBox.Checked = (m_objFilter & (1 << (int)Objects.MAPMS_STATION)) == (1 << (int)Objects.MAPMS_STATION);
            picketCheckerCheckBox.Checked = (m_objFilter & (1 << (int)Objects.MAPMS_PKO)) == (1 << (int)Objects.MAPMS_PKO);
            stationBorderCheckBox.Checked = (m_objFilter & (1 << (int)Objects.MAPMS_GSTAT)) == (1 << (int)Objects.MAPMS_GSTAT);
            headStopCheckBox.Checked = (m_objFilter & (1 << (int)Objects.MAPMS_STOP1)) == (1 << (int)Objects.MAPMS_STOP1);
            stationPathCheckBox.Checked = (m_objFilter & (1 << (int)Objects.MAPMS_SPATH)) == (1 << (int)Objects.MAPMS_SPATH);
            pathCheckBox.Checked = (m_objFilter & (1 << (int)Objects.MAPMS_PPATH)) == (1 << (int)Objects.MAPMS_PPATH);
            picketBorderCheckBox.Checked = (m_objFilter & (1 << (int)Objects.MAPMS_PICKBORDER)) == (1 << (int)Objects.MAPMS_PICKBORDER);
            //электрооборудование
            lrCheckBox.Checked = (m_objFilter & (1 << (int)Objects.MAPMS_LR)) == (1 << (int)Objects.MAPMS_LR);
            kpCheckBox.Checked = (m_objFilter & (1 << (int)Objects.MAPMS_KP)) == (1 << (int)Objects.MAPMS_KP);
            pyaCheckBox.Checked = (m_objFilter & (1 << (int)Objects.MAPMS_PJA)) == (1 << (int)Objects.MAPMS_PJA);
            dtCheckBox.Checked = (m_objFilter & (1 << (int)Objects.MAPMS_DT)) == (1 << (int)Objects.MAPMS_DT);
            rcCheckBox.Checked = (m_objFilter & (1 << (int)Objects.MAPMS_RZ)) == (1 << (int)Objects.MAPMS_RZ);
            strelkaCheckBox.Checked = (m_objFilter & (1 << (int)Objects.MAPMS_STRELKA)) == (1 << (int)Objects.MAPMS_STRELKA);
            lightsCheckBox.Checked = (m_objFilter & (1 << (int)Objects.MAPMS_SEMAFOR)) == (1 << (int)Objects.MAPMS_SEMAFOR);
            //контрольные точки
            pathStartCheckBox.Checked = (m_objFilter & (1 << (int)Objects.MAPMS_STARTPOINT)) == (1 << (int)Objects.MAPMS_STARTPOINT);
            measureStartCheckBox.Checked = (m_objFilter & (1 << (int)Objects.MAPMS_MEASPOINT)) == (1 << (int)Objects.MAPMS_MEASPOINT);
            measureStopCheckBox.Checked = (m_objFilter & (1 << (int)Objects.MAPMS_MEASFINISH)) == (1 << (int)Objects.MAPMS_MEASFINISH);

        }


        private void cancelButton_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            Close();
            Dispose();
        }

        private void acceptButton_Click(object sender, EventArgs e)
        {
            SetFilter();
            if (m_objFilter == 0)
            {
                MessageBox.Show("Не выбрана ни одна группа объектов!", "Внимание!", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
            Close();
            this.DialogResult = DialogResult.OK;
            FireObjectFilterSet(new ObjectFilterSetEvent(m_objFilter));
            //Dispose();
        }

        public UInt32 ObjectFilter
        {
            get
            {
                return m_objFilter;
            }

            set
            {
                m_objFilter = value;
                InitFilter();
            }
        }

        private void SetFilter()
        {

            m_objFilter = 0;
            
            //объекты пути
            if (picketCheckBox.Checked)
                m_objFilter |= (1 << (int)Objects.MAPMS_PICK);
            if (stationCheckBox.Checked)
                m_objFilter |= (1 << (int)Objects.MAPMS_STATION);
            if (picketCheckerCheckBox.Checked)
                m_objFilter |= (1 << (int)Objects.MAPMS_PKO);
            if (stationBorderCheckBox.Checked)
                m_objFilter |= (1 << (int)Objects.MAPMS_GSTAT);
            if (headStopCheckBox.Checked)
                m_objFilter |= (1 << (int)Objects.MAPMS_STOP1);
            if (stationPathCheckBox.Checked)
                m_objFilter |= (1 << (int)Objects.MAPMS_SPATH);
            if (pathCheckBox.Checked)
                m_objFilter |= (1 << (int)Objects.MAPMS_PPATH);
            if (picketBorderCheckBox.Checked)
                m_objFilter |= (1 << (int)Objects.MAPMS_PICKBORDER);
            //электрооборудование
            if (lrCheckBox.Checked)
                m_objFilter |= (1 << (int)Objects.MAPMS_LR);
            if (kpCheckBox.Checked)
                m_objFilter |= (1 << (int)Objects.MAPMS_KP);
            if (pyaCheckBox.Checked)
                m_objFilter |= (1 << (int)Objects.MAPMS_PJA);
            if (dtCheckBox.Checked)
                m_objFilter |= (1 << (int)Objects.MAPMS_DT);
            if (rcCheckBox.Checked)
                m_objFilter |= (1 << (int)Objects.MAPMS_RZ);
            if (strelkaCheckBox.Checked)
                m_objFilter |= (1 << (int)Objects.MAPMS_STRELKA);
            if (lightsCheckBox.Checked)
                m_objFilter |= (1 << (int)Objects.MAPMS_SEMAFOR);
            //контрольные точки
            if (pathStartCheckBox.Checked)
                m_objFilter |= (1 << (int)Objects.MAPMS_STARTPOINT);
            if (measureStartCheckBox.Checked)
                m_objFilter |= (1 << (int)Objects.MAPMS_MEASPOINT);
            if (measureStopCheckBox.Checked)
                m_objFilter |= (1 << (int)Objects.MAPMS_MEASFINISH);
        }

        public void FireObjectFilterSet(ObjectFilterSetEvent e)
        {
            EventHandler<ObjectFilterSetEvent> handler = ObjectFilterSetEventHandler;
            if (handler != null)
                handler(this, e);
        }

    }

    public class ObjectFilterSetEvent : EventArgs
    {

        UInt32 m_objFilter = 0;

        public ObjectFilterSetEvent() 
            : base()
        {

        }

        public ObjectFilterSetEvent(UInt32 objFilter)
            : base()
        {
            m_objFilter = objFilter;
        }

        public UInt32 ObjectFilter
        {
            get
            {
                return m_objFilter;
            }

            set
            {
                m_objFilter = value;
            }
        }

    }
}
