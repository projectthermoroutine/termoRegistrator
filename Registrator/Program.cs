using System;
using System.Collections.Generic;
//using System.Linq;
using System.Windows.Forms;

namespace Registrator
{
    static class Program
    {
        /// <summary>
        /// Главная точка входа для приложения.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            bool autostart = false;
            if (args.Length == 1 && args[0] == "-auto")
                autostart = true;

            Application.Run(new MainWindow(autostart));
        }
    }
}
