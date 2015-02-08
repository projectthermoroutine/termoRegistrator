using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    public class EquOrder
    {

        public enum OrderState
        {
            ORDER_NOTCREATED,
            ORDER_CREATED,
            ORDER_ACCEPTED,
            ORDER_FULLFILLED
        }

        int m_id = -1;
        int m_equip = -1;

        EquObject m_object;
        String m_person = "";
        String m_desc = "";

        DateTime m_creationDate = DateTime.Now;
        DateTime m_firstDate = DateTime.Now;
        DateTime m_finishDate = DateTime.Now;

        OrderState m_state = OrderState.ORDER_NOTCREATED;

        public EquOrder() 
            : base()
        {

        }

        public EquOrder(EquObject obj)
            : this()
        {

            m_object = obj;

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
        public int equipId
        {
            get
            {
                return m_equip;
            }

            set
            {
                m_equip = value;
            }
        }
        public int ID
        {
            get
            {
                return m_id;
            }

            set
            {
                m_id = value;
            }
        }

        public String Person
        {
            get
            {
                return m_person;
            }

            set
            {
                m_person = value;
            }
        }

        public String Desc
        {
            get
            {
                return m_desc;
            }

            set
            {
                m_desc = value;
            }
        }

        public DateTime CreationDate
        {
            get
            {
                return m_creationDate;
            }

            set
            {
                m_creationDate = value;
            }
        }

        public DateTime FirstDate
        {
            get
            {
                return m_firstDate;
            }

            set
            {
                m_firstDate = value;
            }
        }

        public DateTime FinishDate
        {
            get
            {
                return m_finishDate;
            }

            set
            {
                m_finishDate = value;
            }
        }

        public OrderState State
        {
            get
            {
                return m_state;
            }

            set
            {
                m_state = value;
            }
        }

    }
}
