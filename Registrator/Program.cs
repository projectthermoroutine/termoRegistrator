using System;
using System.Collections.Generic;
//using System.Linq;
using System.Windows.Forms;
using System.Threading;
using Microsoft.Win32;

namespace Registrator
{
    static class Program
    {

        public static MainWindow MainForm = null;
        /// <summary>
        /// Главная точка входа для приложения.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            //bool first = false;
            //using (var m = new Mutex(true, "Global\\" + appGuid, out first))
            //{
            //    if (!first)
            //    {
            //        MessageBox.Show("Application" + " " + Application.ProductName.ToString() + " " + "already running.");
            //        return;
            //    }

            //    try
            //    {
                    Application.EnableVisualStyles();
                    Application.SetCompatibleTextRenderingDefault(false);
                    StartupParams startupParams = new StartupParams { auto = false, simulator = false, write = false };

                    foreach (var arg in args)
                    {
                        if (arg == "-auto")
                            startupParams.auto = true;

                        if (arg == "-simulator")
                            startupParams.simulator = true;

                        if (arg == "-save")
                            startupParams.write = true;
                    }

                    //SystemEvents.SessionEnding += SystemEvents_SessionEnding;
                    //SystemEvents.SessionEnded += SystemEvents_SessionEnded;

                    MainForm = new MainWindow(startupParams);
                    Application.Run(MainForm);
            //    }
            //    finally
            //    {
            //        m.ReleaseMutex();
            //    }
            //}
        }

        private static void SystemEvents_SessionEnded(object sender, SessionEndedEventArgs e)
        {
            NLog.LogManager.GetCurrentClassLogger().Debug("SessionEnded");
            Application.Exit();
        }

        private static void SystemEvents_SessionEnding(object sender, SessionEndingEventArgs e)
        {
            NLog.LogManager.GetCurrentClassLogger().Debug("SessionEnding");
            e.Cancel = false;
            MainForm?.Close();
            Application.Exit();
        }

        private static string appGuid = "E5ECE9AA-F8C3-4A84-A47D-992AF62409D9";

    }
}
