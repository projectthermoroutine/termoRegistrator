using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator.Equipment
{
    public enum MessageBoxResult
    {
	    Yes,
	    No,
        Error
    };


    public static class CustomMessageBox
    {
        public static MessageBoxResult Show(string title, string message)
        {
            using (WarningForm form = new WarningForm(title, message))
            {
                form.ShowDialog();
                return form.Apply;
            }
        }

    }
}
