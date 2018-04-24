using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Registrator.Equipment.CreateDbObjectsCtrls
{
    public class IDbObjectCreation
    {
        public event EventHandler<DbObjectEventArg> DbObjectAddedEvent;
        public void DbObjectAdded(EquDbObject db_object)
        {
            EventHandler<DbObjectEventArg> handler = DbObjectAddedEvent;
            if (handler != null)
                handler(this, new DbObjectEventArg(db_object));
        }
    }

    public class DbObjectEventArg : EventArgs
    {
        public DbObjectEventArg(EquDbObject db_object)
        {
            DbObject = db_object;
        }
        public DbObjectEventArg(EquDbObject db_object, EquDbObject[] db_objects)
        {
            DbObject = db_object;
            DbObjects = db_objects;
        }
        public DbObjectEventArg(EquDbObject[] db_objects, bool left_picket)
        {
            DbObjects = db_objects;
            leftPicket = left_picket;
        }
        public EquDbObject DbObject { get; private set; }
        public EquDbObject[] DbObjects { get; private set; }
        public bool leftPicket { get; private set; }
    }

}
