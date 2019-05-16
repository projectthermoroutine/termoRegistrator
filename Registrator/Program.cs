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
            StartupParams startupParams = new StartupParams{ auto = false, simulator = false, write = false};

            foreach (var arg in args)
            {
                if (arg == "-auto")
                    startupParams.auto = true;

                if (arg == "-simulator")
                    startupParams.simulator = true;

                if (arg == "-save")
                    startupParams.write = true;
            }

            Application.Run(new MainWindow(startupParams));
        }
    }
}
