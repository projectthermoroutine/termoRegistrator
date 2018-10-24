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

        public bool deletePicketFromDataBase(EquPicket _equPicket)
        {
            object resDelPicket = queriesAdapter.delPicketFromDB(_equPicket.keyNumber);

            return true;
        }

        public int DeletePathFromLine(EquPath _EquPath)
        {
            EquLine _EquLine = _EquPath.Line;
            EquGroup _EquGroup = _EquLine.Group;
            EquClass _EquClass = _EquGroup.Class;

            int? er = 0;
            queriesAdapter.DeleteTrackTransaction(_EquPath.Code, ref er);

            //var res = queriesAdapter.GetReturnValue(39);

            return (int)er;
        }

        public bool deleteLine(EquLine equLine)
        {
            EquGroup _equGroup = equLine.Group;
            EquClass _equClass = _equGroup.Class;

            queriesAdapter.DeleteLineSQL(/*_equClass.Code, _equGroup.Code, */equLine.Code);

            return true;
        }

        public bool deleteGroupFromClass(EquGroup _EquGroup)
        {
            EquClass _equClass = _EquGroup.Class;
            
            try
            {
                int? error_msg = 0;

                queriesAdapter.DeleteGroup(_EquGroup.Code, ref error_msg);
                
                if (error_msg != 0)
                {
                    MessageBox.Show("Произошла ошибка при выполнении хранимой процедуры. Операция отменена. Ошибка: " + "\n " + error_msg, "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
            }
            catch (System.Data.SqlClient.SqlException e)
            {
                MessageBox.Show("Ошибка базы данных. Операция отменена. Код исключения: " + e.ErrorCode + "\n " + e.Message, "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
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

            return true;
        }

        public bool deleteEquipmentFromPicket(EquObject _EquObject)
        {
            EquPicket _EquPicket = _EquObject.Picket;
            EquPath   _EquPath   = _EquPicket.Path;
            EquLine   _EquLine   = _EquPath.Line;
            EquGroup _EquGroup   = _EquLine.Group;
            EquClass _EquClass   = _EquGroup.Class;

            DB.EFClasses.AllEquipment equip = dbContext.AllEquipments.Where(eq => eq.Code == _EquObject.Code).Distinct().FirstOrDefault();
            dbContext.AllEquipments.Remove(equip);
            dbContext.SaveChanges();

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
                queriesAdapter.UpdateObjectFramesArea(equipID, dtime, (int)area.X, (int)area.Y, (int)area.Height, (int)area.Width, (int)area.Type);
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
