using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using WeifenLuo.WinFormsUI.Docking;

namespace Registrator
{
    public class EquTreeNode : TreeNode
    {
        EquDbObject m_ObjectDB;
        object m_object;
        ToolStripDropDown m_menu;
        Equipment.Properties m_Properties;

        public void ShowMenu(Control parent, Point p)
        {
            if(m_menu!=null)
                m_menu.Show(parent, p);
        }
        public void ShowProperties(DockPanel dockPanel, DockState dockState)
        {
            m_Properties.setProperties(this);
            m_Properties.Show(dockPanel, dockState);
        }

        public EquTreeNode(ToolStripDropDown menu, EquDbObject ObjectDb, Equipment.Properties Properties)
            : base(ObjectDb.Name)
        {
            m_menu = menu;
            m_ObjectDB = ObjectDb;
            m_Properties = Properties;
        }
        public EquTreeNode(ToolStripDropDown menu, Equipment.Properties Properties)
            : base()
        {
            m_menu = menu;
            m_ObjectDB = null;
            m_Properties = Properties;
        }
        public EquTreeNode(EquDbObject ObjectDb)
            : base(ObjectDb.Name)
        {
            m_menu = null;
            m_ObjectDB = ObjectDb;
            m_Properties = null;
        }
        public EquDbObject ObjectDB 
        { 
            get { return m_ObjectDB; } 
            set 
            { 
                m_ObjectDB = value; 
                this.Text = m_ObjectDB.Name; 
            } 
        }
        public EquTreeNode()
            : base()
        {
            
        }

        public EquTreeNode DeepCopy()
        {
            EquTreeNode other = (EquTreeNode)this.MemberwiseClone();
            other.MenuDropDown = this.MenuDropDown;
            other.MenuProperties = this.MenuProperties;
            
            return other;
        }

        public ToolStripDropDown MenuDropDown { set { m_menu = value; } get { return m_menu; } }
        public Equipment.Properties MenuProperties { set { m_Properties = value; } get { return m_Properties; } }
    }
}
