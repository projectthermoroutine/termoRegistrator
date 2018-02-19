using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.SqlServer.Dts.Runtime;
using System.Xml;
using Microsoft.SqlServer.Management.Smo;
using System.Data;

namespace CreateDatabase
{
    class Program
    {
        static void Main(string[] args)
        {

            XMLHelper xml = new XMLHelper("config.xml");

            Config cfg = xml.LoadConfig("config.xml");

            MigrateMetroDB migrate = new MigrateMetroDB();

            migrate.Process(cfg);

            Console.ReadKey();

        }
    }
    class MyEventListener : DefaultEvents
    {
        public override bool OnError(DtsObject source, int errorCode, string subComponent,
          string description, string helpFile, int helpContext, string idofInterfaceWithError)
        {
            // Add application-specific diagnostics here.  
            Console.WriteLine("Error in {0}/{1} : {2}", source, subComponent, description);
            return false;
        }

        public override void OnTaskFailed(TaskHost taskHost)
        {
            Console.WriteLine("Task failed");
        }

        public override void OnPreExecute(Executable exec, ref bool fireAgain)
        {
            Console.WriteLine("OnPreExecute");
        }

    }

    public class XMLHelper
    {
        private string _xmlPath;

        public XMLHelper(string xmlPath)
        {
            _xmlPath = xmlPath;

        }


        //private void LoadConnections(String server, String dbname, XmlNode attr)
        //{


        //}

        public Config LoadConfig(string xmlPath)
        {
            Config cfg = new Config();
            XmlDocument xDoc;

            try
            {
                xDoc = new XmlDocument();
                xDoc.Load(xmlPath);

                // получим корневой элемент
                XmlElement xRoot = xDoc.DocumentElement;

                foreach (XmlNode n in xRoot.SelectNodes("//connection"))
                {
                    XmlNode attr = n.Attributes.GetNamedItem("name");

                    if (attr != null)
                    {
                        if (attr.Value == "target")
                        {
                            XmlNodeList nodeList = n.ChildNodes;

                            foreach (XmlNode node in nodeList)
                            {
                                if (node.Name == "sqlServer")
                                {
                                    cfg.ServerTarget = node.InnerText;
                                }

                                if (node.Name == "databaseName")
                                {
                                    cfg.DatabaseTarget = node.InnerText;
                                }
                            }

                            //cfg.ServerTarget = 
                        }

                        if (attr.Value == "source")
                        {
                            XmlNodeList nodeList = n.ChildNodes;

                            foreach (XmlNode node in nodeList)
                            {
                                if (node.Name == "sqlServer")
                                {
                                    cfg.ServerSource = node.InnerText;
                                }

                                if (node.Name == "databaseName")
                                {
                                    cfg.DatabaseSource = node.InnerText;
                                }
                            }
                        }
                    }
                    else
                        throw new Exception("xml is invalid");
                }


                cfg.MigratePackagePath      = SetProperty(xRoot, "//ssis_package", "path");
                cfg.BackupDatabaseFile      = SetProperty(xRoot, "//backup_database_file", "path");
                cfg.RestoreDatabasePath     = SetProperty(xRoot, "//restore_database_path", "path_database");
                cfg.RestoreDatabaseLogPath  = SetProperty(xRoot, "//restore_database_path", "path_database_log");

                return cfg;
            }
            catch (Exception e)
            {
                return new Config();
            }

        }

        private string SetProperty(XmlElement xRoot, string node_name, string tag)
        {
            string res = "";
            XmlNodeList packagesXml = xRoot.SelectNodes(node_name); // "//ssis_package");

            if (packagesXml.Count == 1)
            {
                foreach (XmlNode xmlNode in packagesXml[0].ChildNodes)

                    if (xmlNode.Name == tag)
                    {
                        res = xmlNode.InnerText;
                    }
            }
            else
                throw new Exception("invalid config");

            return res;
        }
    }


    public class Config
    {
        private string server_target = "";
        public string ServerTarget { get { return server_target; } set { server_target = value; } }

        private string db_target = "";
        public string DatabaseTarget { get { return db_target; } set { db_target = value; } }

        private string server_source = "";
        public string ServerSource { get { return server_source; } set { server_source = value; } }

        private string db_source = "";
        public string DatabaseSource { get { return db_source; } set { db_source = value; } }

        private string migrate_package_path = "";
        public string MigratePackagePath { get { return migrate_package_path; } set { migrate_package_path = value; } }

        private string backup_database_file = "";
        public string BackupDatabaseFile { get { return backup_database_file; } set { backup_database_file = value; } }

        private string restore_database_path = "";
        public string RestoreDatabasePath { get { return restore_database_path; } set { restore_database_path = value; } }

        private string restore_database_log_path = "";
        public string RestoreDatabaseLogPath {  get { return restore_database_log_path; }  set { restore_database_log_path = value; } }
    }


    public class MigrateMetroDB
    {
        private string xmlPath = "";

        public MigrateMetroDB()
        {
            //this.xmlPath = xmlPath;

            //LoadConfig(this.xmlPath);
        }

        public void Process(Config cfg)
        {
            Server mySourceServer = new Server(cfg.ServerTarget);
            //string db_name = "MetrocardWithEquips";
            //mySourceServer.ConnectionContext.Connect();

            Console.WriteLine("Creating backup ...");
            if (!CreateBackup(cfg.DatabaseTarget, cfg.BackupDatabaseFile, mySourceServer))
            {
                Console.WriteLine("Create database was failed");
                return;
            }

            Console.WriteLine("Restoring database ...");
            if(!RestoreDatabase(cfg.DatabaseSource, cfg.BackupDatabaseFile, cfg.RestoreDatabasePath, cfg.RestoreDatabaseLogPath, mySourceServer))
            {
                Console.WriteLine("Database restore was failed");
                return;
            }

            string pkgLocation;
            Package pkg;
            Application app;
            DTSExecResult pkgResults;
            MyEventListener eventListener = new MyEventListener();

            pkgLocation = cfg.MigratePackagePath;

            if (pkgLocation.Length > 0)
            {
                app = new Application();

                try
                {
                    Console.WriteLine("Loading " + pkgLocation + " package ...");
                    pkg = app.LoadPackage(pkgLocation, eventListener);

                    Console.WriteLine("Executing package ...");
                    pkgResults = pkg.Execute();

                    Console.WriteLine(pkgResults.ToString());
                }
                catch(Exception e)
                {
                    Console.WriteLine(e.Message);
                }
            }
        }

        bool CreateBackup(string databaseName, string backup, Server server)
        {
            Backup bkpDBFull = new Backup();
            /* Specify whether you want to back up database or files or log */
            bkpDBFull.Action = BackupActionType.Database;
            /* Specify the name of the database to back up */
            bkpDBFull.Database = databaseName;
            /* You can take backup on several media type (disk or tape), here I am
             * using File type and storing backup on the file system */
            bkpDBFull.Devices.AddDevice(backup, DeviceType.File);
            bkpDBFull.BackupSetName = "metro database Backup";
            bkpDBFull.BackupSetDescription = "metro database - Full Backup";
            /* You can specify the expiration date for your backup data
             * after that date backup data would not be relevant */
            //bkpDBFull.ExpirationDate = DateTime.Today.AddDays(10);
            //bkpDBFull.ExpirationDate =
            /* You can specify Initialize = false (default) to create a new 
             * backup set which will be appended as last backup set on the media. You
             * can specify Initialize = true to make the backup as first set on the
             * medium and to overwrite any other existing backup sets if the all the
             * backup sets have expired and specified backup set name matches with
             * the name on the medium */
            bkpDBFull.Initialize = true;

            /* Wiring up events for progress monitoring */
            // bkpDBFull.PercentComplete += CompletionStatusInPercent;
            // bkpDBFull.Complete += Backup_Completed;

            /* SqlBackup method starts to take back up
             * You can also use SqlBackupAsync method to perform the backup 
             * operation asynchronously */


            try
            {
                bkpDBFull.SqlBackup(server);
                return true;
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);

                return false;
            }

        }

        bool RestoreDatabase(string databaseName, string backupPath, string relocate_mdf_path, string relocate_log_path, Server server)
        {

            Database db = server.Databases[databaseName];


            if (db == null)
            {
                try
                {
                    Restore restoreDB = new Restore();

                    /* Specify whether you want to restore database, files or log */
                    restoreDB.Action = RestoreActionType.Database;
                    restoreDB.Devices.AddDevice(backupPath, DeviceType.File);

                    DataTable dtFileList = restoreDB.ReadFileList(server);
                    string dbLogicalName = dtFileList.Rows[0][0].ToString();
                    string dbPhysicalName = dtFileList.Rows[0][1].ToString();
                    string logLogicalName = dtFileList.Rows[1][0].ToString();
                    string logPhysicalName = dtFileList.Rows[1][1].ToString();

                    RelocateFile DataFile = new RelocateFile();
                    string MDF = restoreDB.ReadFileList(server).Rows[0][1].ToString();
                    DataFile.LogicalFileName = dbLogicalName; //databaseName; //restoreDB.ReadFileList(server).Rows[0][0].ToString();
                    DataFile.PhysicalFileName = relocate_mdf_path; //server.Databases[databaseName].FileGroups[0].Files[0].FileName;

                    RelocateFile LogFile = new RelocateFile();
                    string LDF = restoreDB.ReadFileList(server).Rows[1][1].ToString();
                    LogFile.LogicalFileName = logLogicalName;  //restoreDB.ReadFileList(server).Rows[1][0].ToString();
                    LogFile.PhysicalFileName = relocate_log_path; //server.Databases[databaseName].LogFiles[0].FileName;

                    restoreDB.RelocateFiles.Add(DataFile);
                    restoreDB.RelocateFiles.Add(LogFile);

                    /* You can specify ReplaceDatabase = false (default) to not create a new
                     * database, the specified database must exist on SQL Server
                     * instance. If you can specify ReplaceDatabase = true to create new
                     * database image regardless of the existence of specified database with
                     * the same name */
                    restoreDB.ReplaceDatabase = true;

                    /* If you have a differential or log restore after the current restore,
                     * you would need to specify NoRecovery = true, this will ensure no
                     * recovery performed and subsequent restores are allowed. It means it
                     * the database will be in a restoring state. */
                    //restoreDB.NoRecovery = true;

                    /* Wiring up events for progress monitoring */
                    //restoreDB.PercentComplete += CompletionStatusInPercent;
                    //restoreDB.Complete += Restore_Completed;

                    restoreDB.Database = databaseName;

                    /* SqlRestore method starts to restore the database
                     * You can also use SqlRestoreAsync method to perform restore 
                     * operation asynchronously */
                    restoreDB.SqlRestore(server);

                    Database dbCheck = server.Databases[databaseName];
                    dbCheck.SetOnline();
                    server.Refresh();

                    return true;
                }
                catch(Exception e)
                {
                    Console.WriteLine(e.Message);
                }
            }
            else
                Console.WriteLine("Database " + db.Name + " already exist!");

            return false;
        }
    }

}
