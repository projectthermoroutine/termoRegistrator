using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Registrator
{
    public partial class FramesPanel : ToolWindow
    {
        private bool m_checkAllState = true;
        private DB.metro_db_controller _db_controller;
        PointsInfoManager _points_info_manager;
        PointsInfoView PointsInfoViewCtrl;
        
        public FramesPanel(PointsInfoManager points_info_manager, DB.metro_db_controller db_controller)
        {
            InitializeComponent();
            if (db_controller != null)
                _db_controller = new DB.metro_db_controller(db_controller);

            PointsInfoViewCtrl = new PointsInfoView(_db_controller);
            this.Controls.Add(this.PointsInfoViewCtrl);
            this.PointsInfoViewCtrl.Dock = System.Windows.Forms.DockStyle.Fill;
            PointsInfoViewCtrl.CheckBoxes = true;
            PointsInfoViewCtrl.DoubleClickItem += shotsList_DoubleClick;
            PointsInfoViewCtrl.ItemPressed += shotsList_DoubleClick;
            PointsInfoViewCtrl.ItemDeleted += shotsList_ItemDeleted;

            _points_info_manager = points_info_manager;

            foreach (var item in _points_info_manager.PointsInfoList())
            {
                PointsInfoViewCtrl.AddPointInfo(item);
            }

            points_info_manager.AddPointInfoEventHandler += AddPointInfoEventHandler;
        }
        void AddPointInfoEventHandler(object sender, PointInfoEvent e)
        {
            PointsInfoViewCtrl.AddPointInfo(e.PointInfo);
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {

            bool checkedPresence = false;

            for (int i = PointsInfoViewCtrl.PointInfoListView.Items.Count - 1; i > -1; i--)
            {
                if (PointsInfoViewCtrl.PointInfoListView.Items[i].Checked)
                {
                    PointsInfoViewCtrl.PointInfoListView.Items.RemoveAt(i);
                    checkedPresence = true;
                }

            }

            if (!checkedPresence && PointsInfoViewCtrl.PointInfoListView.SelectedIndices.Count > 0)
                PointsInfoViewCtrl.PointInfoListView.Items.RemoveAt(PointsInfoViewCtrl.PointInfoListView.SelectedIndices[0]);
        
        }

        private void timeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            PointsInfoViewCtrl.PointInfoListView.Columns[1].Width = (timeToolStripMenuItem.Checked) ? 89 : 0;
        }

        private void lineToolStripMenuItem_Click(object sender, EventArgs e)
        {
            PointsInfoViewCtrl.PointInfoListView.Columns[2].Width = (lineToolStripMenuItem.Checked) ? 60 : 0;
        }

        private void pathToolStripMenuItem_Click(object sender, EventArgs e)
        {
            PointsInfoViewCtrl.PointInfoListView.Columns[3].Width = (pathToolStripMenuItem.Checked) ? 60 : 0;
        }
        
        private void piketToolStripMenuItem_Click(object sender, EventArgs e)
        {
            PointsInfoViewCtrl.PointInfoListView.Columns[4].Width = (pNoToolStripMenuItem.Checked) ? 62 : 0;
        }

        private void objToolStripMenuItem_Click(object sender, EventArgs e)
        {
            PointsInfoViewCtrl.PointInfoListView.Columns[6].Width = (objToolStripMenuItem.Checked) ? 112 : 0;
        }

        private void checkAllButton_Click(object sender, EventArgs e)
        {
            m_checkAllState = !m_checkAllState;
            var list_view = PointsInfoViewCtrl.PointInfoListView;
            if (!m_checkAllState)
                while (list_view.CheckedItems.Count > 0)
                    list_view.CheckedItems[0].Checked = false;
            else
                for (int i = 0; i < list_view.Items.Count; i++)
                    list_view.Items[i].Checked = true;

            //checkAllButton.checkAllButton.Text = (m_checkAllState) ? "Выбрать все" : "";
            checkAllButton.Image = ((System.Drawing.Image)((m_checkAllState) ? global::Registrator.Properties.Resources.iconCheckAll : global::Registrator.Properties.Resources.iconUnCheckAll));
        }

        private void FramesPanel_FormClosed(object sender, FormClosedEventArgs e)
        {
            _points_info_manager.AddPointInfoEventHandler += AddPointInfoEventHandler;
        }

        private void shotsList_DoubleClick(object sender, ItemEvent e)
        {
            ShotForm form = new ShotForm(e.PointInfo, _db_controller);
            form.ShowDialog();
        }

        private void shotsList_ItemDeleted(object sender, ItemEvent e)
        {
            _points_info_manager.RemoveAt(e.ItemIndex);
        }
    
    }
}
