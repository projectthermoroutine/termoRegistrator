using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Registrator
{
    public class EquTreeNode : TreeNode
    {

        object m_object;

        public EquTreeNode()
            : base()
        {

        }

        public EquTreeNode(String name)
            : base(name)
        {

        }

        public Object UserObject
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

    }
}
