using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Registrator.DB
{
    public class metro_db_edit_controller: metro_db_controller
    {
        public metro_db_edit_controller(metro_db_controller controller)
            :base(controller)
        {       }

        public int AddLine( string lineName, string lineCode, ref string errorMsg)
        {
            try
            {
                int line_number = dbContext.Lines.Max(l => l.LineNum);

                if ((from l in dbContext.Lines where l.LineNum == line_number select l.LineName).Count() > 0) ///TODO
                {
                    //error_msg = "В базе данных присутствуют  ";
                    //return 0;
                }

                dbContext.Lines.Add(new EFClasses.Line {LineNum = line_number, LineName = lineName, StartCoordinate = 0, LineCode = lineCode });

                return line_number;
            }
            catch (System.Data.SqlClient.SqlException e)
            {
                errorMsg = e.Message;
                return 0;
            }
        }

        public bool deletePicketFromDataBase(EquPicket _equPicket)
        {
            EquPath _equPath   = _equPicket.Path;
            EquLine _equLine   = _equPath.Line;
            EquGroup _equGroup = _equLine.Group;
            EquClass _equClass = _equGroup.Class;

            //var empData1 = (from r in all_equipment_table.AsEnumerable() where r.number == _EquPicket.Code select new { r.ClassNum, r.GroupNum, r.LineNum, r.Track, r.Layout, r.number }).Distinct();

            //foreach (var item in empData1)
            //{
            //    var emp = (from r in all_equipment_table.AsEnumerable() where r.ClassNum == item.ClassNum && r.GroupNum == item.GroupNum && r.Track == item.Track && r.Layout == item.Layout select new { r.Npicket });

            //    int res = 0;
            //    int cnt = emp.Count();
            //    res = all_equipment_adapter.delPicket(_EquClass.Code, _EquGroup.Code, _EquLine.Code, _EquPath.Code, 0, _EquPicket.Code, cnt);
            //}

            //var query = (from m in dbContext.Mains where m.Npicket == _equPicket.Code select new { m.ClassNum, m.GroupNum, m.LineNum, m.Track, m.Layout , m.Npicket }).Distinct();

            //foreach (var item in query)
            //{
                object result = queriesAdapter.delPicket(_equClass.Code, _equGroup.Code, _equLine.Code, _equPath.Code, 0, _equPicket.keyNumber);
            //}

            object resDelPicket = pickets_adapter.delPicketFromDB(_equPicket.keyNumber);

            refresh();

            return true;
        }

        public bool deletePathFromLine(EquPath _EquPath)
        {
            EquLine _EquLine = _EquPath.Line;
            EquGroup _EquGroup = _EquLine.Group;
            EquClass _EquClass = _EquGroup.Class;

            var res = queriesAdapter.DeleteTrack(_EquPath.Code);

            return true;
        }

        public bool deleteLine(EquLine equLine)
        {
            EquGroup _equGroup = equLine.Group;
            EquClass _equClass = _equGroup.Class;

            //queriesAdapter.deleteLine(_equClass.Code, _equGroup.Code, equLine.Code);

            return true;
        }

        //public bool deleteLineFromDB(EquLine _equLine)
        //{
        //    EquGroup _equGroup = _equLine.Group;
        //    EquClass _equClass = _equGroup.Class;

        //    queriesAdapter.deleteLine(_equClass.Code, _equGroup.Code, _equLine.Code);
            
        //    return true;
        //}

        public bool deleteGroupFromClass(EquGroup _EquGroup)
        {
            EquClass _equClass = _EquGroup.Class;
            
            try
            {
                string error_msg = "";

                queriesAdapter.DeleteGroup(_EquGroup.Code, ref error_msg);
                
                if (error_msg != "")
                {
                    MessageBox.Show("Произошла ошибка при выполнении сервером базы данных запроса . Операция отменена. Ошибка: " + "\n " + error_msg, "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
            }
            catch (System.Data.SqlClient.SqlException e)
            {
                MessageBox.Show("Ошибка базы данных. Операция отменена. Код ошибки: " + e.ErrorCode + "\n " + e.Message, "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            return true;
        }
        public bool deleteClass(EquClass _EquClass)
        {
            try
            {
                string error_msg = "";
                var res = queriesAdapter.DeleteClass(_EquClass.Code, ref error_msg);
                //var res = classes_adapter.delClass(_EquClass.Code, ref error_msg);
               
                if (error_msg != "")
                {
                    MessageBox.Show("Произошла ошибка при выполнении сервером базы данных запроса . Операция отменена. Ошибка: " + "\n " + error_msg, "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
            }
            catch (System.Data.SqlClient.SqlException e)
            {
                MessageBox.Show("Ошибка базы данных. Операция отменена. Код ошибки: " + e.ErrorCode + "\n " + e.Message, "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            refresh();

            return true;
        }

        public bool deleteEquipmentFromPicket(EquObject _EquObject)
        {
            EquPicket _EquPicket = _EquObject.Picket;
            EquPath   _EquPath   = _EquPicket.Path;
            EquLine   _EquLine   = _EquPath.Line;
            EquGroup _EquGroup   = _EquLine.Group;
            EquClass _EquClass   = _EquGroup.Class;

            var query = queriesAdapter.deleteEquipment(_EquObject.Code);

            return true;
        }

        public bool deleteEquipmentAsType(EquObject _EquObject)
        {
            EquPicket _EquPicket = _EquObject.Picket;
            EquPath _EquPath = _EquPicket.Path;
            EquLine _EquLine = _EquPath.Line;
            EquGroup _EquGroup = _EquLine.Group;
            EquClass _EquClass = _EquGroup.Class;

            queriesAdapter.delEquipAsType(_EquObject.Code);

            ///TODO refresh

            return true;
        }

        public void saveArea(int equipID, Area area, bool saveAsDefault, DateTime dtime = new DateTime())
        {
            if(saveAsDefault)
            {
                queriesAdapter.UpdateObjectArea(equipID, (int)area.X, (int)area.Y, (int)area.Height, (int)area.Width, (int)area.Type);
            }
            else
            {
                ObjectsFramesAdapter.UpdateObjectFramesArea(equipID, dtime, (int)area.X, (int)area.Y, (int)area.Height, (int)area.Width, (int)area.Type);
            }
        }

        public void updateEquipmentState(int equipID, int equipState)
        {
            queriesAdapter.updateEquipState(equipID, equipState);
        }

        public void updateEquipmentPosition(int equipID, int X,int Y)
        {
            queriesAdapter.updateObjectCoordinate(equipID, X, Y);
        }
   }
}
