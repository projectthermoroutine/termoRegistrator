using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Windows.Documents;
using System.Collections;

namespace Registrator
{
    public partial class OrdersListForm : Form
    {

        EquObject m_object = null;

        ArrayList m_orders = new ArrayList();

        DB.teplovizorDataSet.EquipmentOrdersDataTable dt = null;
      //  MetrocardDataSet.AllEquipmentTableDataTable edt = null;

        public OrdersListForm(ref DB.DataBaseHelper dbHelperArg)
        {
            InitializeComponent();

            addButton.Visible = false;
            delButton.Visible = false;
            toolStripSeparator1.Visible = false;

            InitForm();
            dbHelper = dbHelperArg;

        }

        public OrdersListForm(EquObject obj, ref DB.DataBaseHelper dbHelperArg)
        {

            InitializeComponent();

            m_object = obj;
            dbHelper = dbHelperArg;
            InitForm();
        }

        public EquObject Object
        {
            get
            {
                return m_object;
            }

            set
            {
                m_object = value;
            }
        }

        public DB.DataBaseHelper dbHelper;

        void InitObjectOrders(int objId)
        {

            dbHelper.TblAdapter_Orders.Fill(dbHelper.dataTable_Orders);

            var empData1 = (from r in dbHelper.dataTable_Orders.AsEnumerable() where r.id_equipment == m_object.Code select new { r.ID,r.CreationDate,r.Desc,r.FinishDate,r.FirstDate,r.Person,r.State,r.id_equipment });

            EquObject obj = new EquObject(  
                                            objId,
                                            /*Convert.ToInt32(edt.Rows[0].ItemArray[0])*/ m_object.Code,
                                            m_object.Name,
                                            m_object.Group, 
                                            m_object.Layout, 
                                            m_object.Path, 
                                            m_object.Picket, 
                                            m_object.Offset
                                         );

            int i=0;

            foreach(var item in empData1)
            {
                m_orders.Add(new EquOrder(obj));

                m_orders.Add(new EquOrder(obj));

                ((EquOrder)m_orders[i]).equipId = item.id_equipment;
                ((EquOrder)m_orders[i]).ID = item.ID;
                ((EquOrder)m_orders[i]).Person = item.Person;
                ((EquOrder)m_orders[i]).Desc = item.Desc;
                ((EquOrder)m_orders[i]).CreationDate = item.CreationDate;
                ((EquOrder)m_orders[i]).FirstDate = item.FinishDate;
                ((EquOrder)m_orders[i]).FinishDate = item.FinishDate;

                ((EquOrder)m_orders[i]).State = EquOrder.OrderState.ORDER_CREATED;

                if(item.State == 2)
                    ((EquOrder)m_orders[i]).State = EquOrder.OrderState.ORDER_ACCEPTED;

                if(item.State == 3)
                    ((EquOrder)m_orders[i]).State = EquOrder.OrderState.ORDER_FULLFILLED;


                ListViewItem itemLV = new ListViewItem(new String[]{
                    "",
                    /*(i + 1).ToString()*/ item.ID.ToString(),
                    ((EquOrder)m_orders[i]).CreationDate.ToShortDateString() ,
                    (((EquOrder)m_orders[i]).State == EquOrder.OrderState.ORDER_ACCEPTED || ((EquOrder)m_orders[i]).State == EquOrder.OrderState.ORDER_FULLFILLED)?((EquOrder)m_orders[i]).FirstDate.ToShortDateString():"",
                    (((EquOrder)m_orders[i]).State == EquOrder.OrderState.ORDER_FULLFILLED)?((EquOrder)m_orders[i]).FinishDate.ToShortDateString():"",
                    ((EquOrder)m_orders[i]).Object.Line.ToString(),
                    ((EquOrder)m_orders[i]).Object.Path.ToString(),
                    ((EquOrder)m_orders[i]).Object.Layout.Name,
                    String.Concat(new string[]{ ((EquOrder)m_orders[i]).Object.Picket.ToString(), " + ", ((EquOrder)m_orders[i]).Object.Offset.ToString() }),
                    ((EquOrder)m_orders[i]).Object.Group.Name,
                    ((EquOrder)m_orders[i]).Object.Name,
                    ((EquOrder)m_orders[i]).Object.Group.Class.Name,
                    ((EquOrder)m_orders[i]).Person
                });

                listView1.Items.Add(itemLV);
                i++;

            }

            //equipmentOrdersAdapter1.FillByElement(teplovizorDataSet1.EquipmentOrders, objId);
            //dt = equipmentOrdersAdapter1.GetDataByElement(m_object.ID);

            //if (dt.Rows.Count < 1)
            //    return;

            //allEquipmentTableAdapter1.FillByObject(
            //    metrocardDataSet1.AllEquipmentTable,
            //    Convert.ToInt32(dt.Rows[0].ItemArray[8]),
            //    Convert.ToInt32(dt.Rows[0].ItemArray[9]),
            //    Convert.ToInt16(dt.Rows[0].ItemArray[10]),
            //    Convert.ToInt16(dt.Rows[0].ItemArray[11]),
            //    Convert.ToDecimal(dt.Rows[0].ItemArray[12])
            //    );
            //edt = allEquipmentTableAdapter1.GetDataByObject(
            //    Convert.ToInt32(dt.Rows[0].ItemArray[8]),
            //    Convert.ToInt32(dt.Rows[0].ItemArray[9]),
            //    Convert.ToInt16(dt.Rows[0].ItemArray[10]),
            //    Convert.ToInt16(dt.Rows[0].ItemArray[11]),
            //    Convert.ToDecimal(dt.Rows[0].ItemArray[12])
            //    );

            //if (edt.Rows.Count < 1)
            //    return;

             
            //EquClass equClass = new EquClass(Convert.ToInt32(edt.Rows[0].ItemArray[16]), (String)edt.Rows[0].ItemArray[12]);
            //EquGroup equGroup = new EquGroup(Convert.ToInt32(edt.Rows[0].ItemArray[15]), (String)edt.Rows[0].ItemArray[11]);
            //equGroup.Class = equClass;
            
            //EquLayout equLayout = new EquLayout(Convert.ToInt32(edt.Rows[0].ItemArray[1]), (String)edt.Rows[0].ItemArray[13]);
                
            //EquObject obj = new EquObject(
            //                                    objId,
            //                                    Convert.ToInt32(edt.Rows[0].ItemArray[0]),
            //                                    (String)edt.Rows[0].ItemArray[10],
            //                                    equGroup, 
            //                                    equLayout, 
            //                                    Convert.ToInt32(edt.Rows[0].ItemArray[2]), 
            //                                    Convert.ToInt32(edt.Rows[0].ItemArray[6]), 
            //                                    Convert.ToSingle(edt.Rows[0].ItemArray[7])
            //                                );

            //for (int i = 0; i < dt.Rows.Count; i++ )
            //{

            //    object[] items = dt.Rows[i].ItemArray;

            //    m_orders.Add(new EquOrder(obj));
            //    ((EquOrder)m_orders[i]).ID = Convert.ToInt32(items[0]);
            //    ((EquOrder)m_orders[i]).Person = Convert.ToString(items[1]);
            //    ((EquOrder)m_orders[i]).Desc = Convert.ToString(items[2]);
            //    ((EquOrder)m_orders[i]).CreationDate = Convert.ToDateTime(items[3]);
            //    ((EquOrder)m_orders[i]).FirstDate = Convert.ToDateTime(items[4]);
            //    ((EquOrder)m_orders[i]).FinishDate = Convert.ToDateTime(items[5]);

            //    ((EquOrder)m_orders[i]).State = EquOrder.OrderState.ORDER_CREATED;

            //    if(Convert.ToInt32(items[6]) == 2)
            //        ((EquOrder)m_orders[i]).State = EquOrder.OrderState.ORDER_ACCEPTED;

            //    if (Convert.ToInt32(items[6]) == 3)
            //        ((EquOrder)m_orders[i]).State = EquOrder.OrderState.ORDER_FULLFILLED;

            //    ListViewItem item = new ListViewItem(new String[]{
            //        "",
            //        (i + 1).ToString(),
            //        ((EquOrder)m_orders[i]).CreationDate.ToShortDateString(),
            //        (((EquOrder)m_orders[i]).State == EquOrder.OrderState.ORDER_ACCEPTED || ((EquOrder)m_orders[i]).State == EquOrder.OrderState.ORDER_FULLFILLED)?((EquOrder)m_orders[i]).FirstDate.ToShortDateString():"",
            //        (((EquOrder)m_orders[i]).State == EquOrder.OrderState.ORDER_FULLFILLED)?((EquOrder)m_orders[i]).FinishDate.ToShortDateString():"",
            //        ((EquOrder)m_orders[i]).Object.Line.ToString(),
            //        ((EquOrder)m_orders[i]).Object.Path.ToString(),
            //        ((EquOrder)m_orders[i]).Object.Layout.Name,
            //        String.Concat(new string[]{ ((EquOrder)m_orders[i]).Object.Picket.ToString(), " + ", ((EquOrder)m_orders[i]).Object.Offset.ToString() }),
            //        ((EquOrder)m_orders[i]).Object.Group.Name,
            //        ((EquOrder)m_orders[i]).Object.Name,
            //        ((EquOrder)m_orders[i]).Object.Group.Class.Name,
            //        ((EquOrder)m_orders[i]).Person
            //    });

            //    listView1.Items.Add(item);

            //}

        }

        public void InitForm()
        {
            listView1.Items.Clear();

            if (m_object != null)
            {
                
                InitObjectOrders(m_object.ID);

            }
        }

        private void addButton_Click(object sender, EventArgs e)
        {
            if (m_object == null)
                return;

            OrderForm of = new OrderForm(new EquOrder(m_object), ref dbHelper);
            of.newOrderEventHandler += NewOrderEventFired;
            of.orderChangedEventHandler += OrderChangedEventFired;
            of.ShowDialog();
        }

        private void delButton_Click(object sender, EventArgs e)
        {
            if (m_orders == null || m_orders.Count < 1 || listView1.SelectedIndices.Count < 1)
                return;

            equipmentOrdersAdapter1.DeleteOrderQuery(((EquOrder)m_orders[listView1.SelectedIndices[0]]).ID);

            InitForm();

        }

        private void showButton_Click(object sender, EventArgs e)
        {

            if (m_orders == null || m_orders.Count < 1 || listView1.SelectedIndices.Count < 1)
                return;

            OrderForm of = new OrderForm((EquOrder)m_orders[listView1.SelectedIndices[0]],ref dbHelper);
            of.newOrderEventHandler += NewOrderEventFired;
            of.orderChangedEventHandler += OrderChangedEventFired;
            of.ShowDialog();
        }

        private void UpdateDb()
        {
            if (dt == null)
                return;
        }

        public virtual void NewOrderEventFired(object sender, NewOrderEvent e)
        {
            EquOrder order = e.Order;
            m_orders.Add(order);
            InitForm();

        }

        public virtual void OrderChangedEventFired(object sender, OrderChangedEvent e)
        {
            if (dt == null)
                return;
            InitForm();
        }

        private void OrdersListForm_VisibleChanged(object sender, EventArgs e)
        {
            if (!Visible)
                Dispose();
        }

        private void listView1_DoubleClick(object sender, EventArgs e)
        {
            if (m_orders == null || m_orders.Count < 1 || listView1.SelectedIndices.Count < 1)
                return;

            OrderForm of = new OrderForm((EquOrder)m_orders[listView1.SelectedIndices[0]],ref dbHelper);
            of.newOrderEventHandler += NewOrderEventFired;
            of.orderChangedEventHandler += OrderChangedEventFired;
            of.ShowDialog();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Close();
            Dispose();
        }

    }
}
