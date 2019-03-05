using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Windows.Documents;
using Registrator.DB.EFClasses;
using NLog;

namespace Registrator
{
    public class PicketsManager
    {
        DB.metro_db_edit_controller _db_controller;
        private static readonly Logger logger = LogManager.GetCurrentClassLogger();

        public PicketsManager(DB.metro_db_controller db_controller)
        {
            _db_controller = new DB.metro_db_edit_controller(db_controller);
        }

        public void changePicketLength(int Length, DB.EFClasses.Picket picket)
        {
            string error_message = _db_controller.UpdatePicketLength(picket, Length);
            if(error_message != "")
            {
                MessageBox.Show($"Ошибка сервера базы данных: {error_message}.", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}
