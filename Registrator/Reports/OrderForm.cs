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
    public partial class OrderForm : Form
    {

        EquOrder m_order = null;

        //private teplovizorDataSet.ordersDataTable odt = null;

        public EventHandler<NewOrderEvent> newOrderEventHandler;
        public EventHandler<OrderChangedEvent> orderChangedEventHandler;

        public OrderForm(/*DB.metro_db_controller _db_controller*/)
        {
            InitializeComponent();
        }

        public OrderForm(EquOrder order, DB.metro_db_controller db_controller)
            : this()
        {
            m_order = order;
            InitForm();
            _db_controller = new DB.metro_db_controller(db_controller);
        }

        private DB.metro_db_controller _db_controller;
        public void InitForm()
        {


            if (m_order == null)
                return;

            if (m_order.ID == -1)
                acceptButton.Enabled = false;

            creationDate.MinDate = DateTime.Now;
            firstDate.MinDate = DateTime.Now;
            finishDate.MinDate = DateTime.Now;

            creationRb.Enabled = true;
            startRb.Enabled = true;
            finishRb.Enabled = true;

            creationDate.Enabled = true;
            firstDate.Enabled = false;
            finishDate.Enabled = false;

            if (m_order.State == EquOrder.OrderState.ORDER_CREATED ||
                m_order.State == EquOrder.OrderState.ORDER_ACCEPTED ||
                m_order.State == EquOrder.OrderState.ORDER_FULLFILLED)
            {
                creationDate.MinDate = m_order.CreationDate;

                creationRb.Checked = true;
                startRb.Checked = false;
                finishRb.Checked = false;

            }
            if (m_order.State == EquOrder.OrderState.ORDER_ACCEPTED ||
                m_order.State == EquOrder.OrderState.ORDER_FULLFILLED)
            {
                firstDate.MinDate = m_order.CreationDate;

                creationRb.Checked = false;
                startRb.Checked = true;
                finishRb.Checked = false;

                creationRb.Enabled = true;
                startRb.Enabled = true;
                finishRb.Enabled = true;

                creationDate.Enabled = false;
                firstDate.Enabled = true;
                finishDate.Enabled = false;

            }
            if (m_order.State == EquOrder.OrderState.ORDER_FULLFILLED)
            {
                finishDate.MinDate = m_order.CreationDate;

                creationRb.Checked = false;
                startRb.Checked = false;
                finishRb.Checked = true;

                creationRb.Enabled = true;
                startRb.Enabled = true;
                finishRb.Enabled = true;

                creationDate.Enabled = false;
                firstDate.Enabled = false;
                finishDate.Enabled = true;

            }

            lineLbl.Text = m_order.Object.Line.LineCode;
            pathLbl.Text = m_order.Object.Path.Name;
            pNoffsLbl.Text = String.Concat(new String[] { m_order.Object.Picket.npicket, " + ", m_order.Object.Offset.ToString() });
            classLbl.Text = m_order.Object.Group.Class.Name;
            groupLbl.Text = m_order.Object.Group.Name;
            objLbl.Text = m_order.Object.Name;

            fio.Text = m_order.Person;
            desc.Text = m_order.Desc;



        }

        public EquOrder Order
        {
            get
            {
                return m_order;
            }

            set
            {
                m_order = value;
                InitForm();
            }
        }

        private void acceptButton_Click(object sender, EventArgs e)
        {
            Save();

        }
        private void cancelButton_Click(object sender, EventArgs e)
        {
            Close();
            Dispose();
        }

        private void saveButton_Click(object sender, EventArgs e)
        {
            Save();
            Close();
            Dispose();
        }
        private int calcNumberOrder()
        {
            var resOrderNumber = (from r in _db_controller.orders_table.AsEnumerable() orderby r.ID select new { r.ID });

            int ind = 1;

            foreach (var item in resOrderNumber)
            {
                if (ind != Convert.ToInt32(item.ID))
                    break;
                ind++;
            }

            return ind;
        }

        private void Save()
        {
            m_order.Person = fio.Text;
            m_order.Desc = desc.Text;

            if (creationRb.Checked)
            {
                m_order.State = EquOrder.OrderState.ORDER_CREATED;
                m_order.CreationDate = creationDate.Value;
            }
            else if (startRb.Checked)
            {
                m_order.State = EquOrder.OrderState.ORDER_ACCEPTED;
                m_order.FirstDate = firstDate.Value;
            }
            else if (finishRb.Checked)
            {
                m_order.State = EquOrder.OrderState.ORDER_FULLFILLED;
                m_order.FinishDate = finishDate.Value;
            }

            if (m_order.ID == -1)
            {

                // calc ID
                m_order.ID = calcNumberOrder();

                _db_controller.orders_adapter.insertOrder1(m_order.ID,
                                                     m_order.Person,
                                                     m_order.Desc,
                                                     m_order.CreationDate,
                                                     m_order.FirstDate,
                                                     m_order.FinishDate, (byte)(m_order.State), m_order.Object.Code);

                FireNewOrderEvent(new NewOrderEvent(m_order));

            }
            else
            {
                int? res2 = -1;

                _db_controller.orders_adapter.updateOrder(m_order.ID, m_order.Person, m_order.Desc, m_order.CreationDate, m_order.FirstDate, m_order.FinishDate, (byte)(m_order.State), ref res2);

                if (res2 == 1)
                    FireOrderChangedEvent(new OrderChangedEvent(m_order));
                else
                    MessageBox.Show("Изменение распоряжение не выполнено. Сервер Базы Данных не выполнил операцию по изменению распоряжения",
                    "Ошибка Базы Данных", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void creationRb_CheckedChanged(object sender, EventArgs e)
        {
            creationDate.Enabled = creationRb.Checked;
            firstDate.Enabled = startRb.Checked;
            finishDate.Enabled = finishRb.Checked;
        }

        private void FireNewOrderEvent(NewOrderEvent e)
        {

            EventHandler<NewOrderEvent> handler = newOrderEventHandler;

            if (handler != null)
            {
                handler(this, e);
            }


        }

        private void FireOrderChangedEvent(OrderChangedEvent e)
        {

            EventHandler<OrderChangedEvent> handler = orderChangedEventHandler;

            if (handler != null)
            {
                handler(this, e);
            }

        }

    }

    public class NewOrderEvent : EventArgs
    {
        EquOrder m_order = null;

        public NewOrderEvent()
            : base()
        {

        }

        public NewOrderEvent(EquOrder order)
            : this()
        {

            m_order = order;

        }

        public EquOrder Order
        {
            get
            {
                return m_order;
            }

            set
            {
                m_order = value;
            }
        }



    }

    public class OrderChangedEvent : EventArgs
    {

        EquOrder m_order = null;

        public OrderChangedEvent()
            : base()
        {

        }

        public OrderChangedEvent(EquOrder order)
            : this()
        {

            m_order = order;

        }

        public EquOrder Order
        {
            get
            {
                return m_order;
            }

            set
            {
                m_order = value;
            }
        }

    }

}
