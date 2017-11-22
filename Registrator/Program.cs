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
            bool simulator_mode = false;

            if (args.Length == 1 || args.Length == 2)
            {
                if (args[0] == "-auto")
                {
                    autostart = true;
                    if (args.Length == 2 && args[1] == "-simulator")
                        simulator_mode = true;
                }
            }
            Application.Run(new MainWindow(autostart, simulator_mode));
        }
    }
}
