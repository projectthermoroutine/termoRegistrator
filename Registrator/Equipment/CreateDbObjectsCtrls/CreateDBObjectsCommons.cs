using Registrator.DB.EFClasses;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Registrator.Equipment.CreateDbObjectsCtrls
{
    public class CreateDBObjectsCommons
    {
        public static long CalcCoordinate(long shift, ContextMetroCard dbContext)
        {
            long ObjectCoordinate = 0;

            //var Picket = dbContext.Pickets.Where(p => p.number == equPicket.keyNumber && p.path == equPath.Code).Select(e => new { e.EndShiftLine, e.StartShiftLine });

            //if (Picket.Count() != 1)
            //    throw new Exception("Cannot find picket needed for calculate line offset");

            //if (equPicket.npicket[0] == '-')
            //    ObjectCoordinate = Picket.First().EndShiftLine + shift;
            //else
            //    ObjectCoordinate = Picket.First().StartShiftLine + shift;

            return ObjectCoordinate;
        }

    }
}
