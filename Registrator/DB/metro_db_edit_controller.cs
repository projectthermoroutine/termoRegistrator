using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Registrator.DB
{
    using db_object_info = DB.EFClasses.AllEquipment;

    public class metro_db_edit_controller: metro_db_controller
    {
        public metro_db_edit_controller(metro_db_controller controller)
            :base(controller)
        {       }

        public void deletePicket(int picket_id)
        {
            var equipments = dbContext.AllEquipments.Where(e => e.Picket == picket_id);
            dbContext.AllEquipments.RemoveRange(equipments);

            var picket = dbContext.Pickets.Where(p => p.number == picket_id).FirstOrDefault();

            if (picket.number != 0)
            {
                dbContext.Pickets.Remove(picket);
            }

            dbContext.SaveChanges();
        }

        public void DeletePath(int path_id)
        {
            var equipments = dbContext.AllEquipments.Where(e => e.Path == path_id);
            dbContext.AllEquipments.RemoveRange(equipments);

            var pickets = dbContext.Pickets.Where(p => p.path == path_id);
            dbContext.Pickets.RemoveRange(pickets);

            var path = dbContext.Tracks.Where(p => p.ID == path_id).FirstOrDefault();

            if(path.ID != 0)
            {
                dbContext.Tracks.Remove(path);
            }

            dbContext.SaveChanges();

        }

        public void deleteLine(int line_id)
        {
            var equipments = dbContext.AllEquipments.Where(e => e.Line == line_id);
            dbContext.AllEquipments.RemoveRange(equipments);

            var paths = dbContext.Tracks.Where(p => p.LineId == line_id);

            foreach (var path in paths)
            {
                var pickets = dbContext.Pickets.Where(p => p.path == path.ID);
                dbContext.Pickets.RemoveRange(pickets);
            }

            dbContext.Tracks.RemoveRange(paths);

            var line = dbContext.Lines.Where(l => l.LineNum == line_id).FirstOrDefault();

            if (line.LineNum != 0)
            {
                dbContext.Lines.Remove(line);
            }

            dbContext.SaveChanges();
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

        public bool deleteEquipmentFromPicket(int obj_id)
        {
            DB.EFClasses.AllEquipment equip = dbContext.AllEquipments.Where(eq => eq.Code == obj_id).Distinct().FirstOrDefault();
            dbContext.AllEquipments.Remove(equip);
            dbContext.SaveChanges();

            return true;
        }

        public bool deleteEquipmentAsType(int obj_id)
        {
            queriesAdapter.delEquipAsType(obj_id);

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

        public string UpdatePicketLength(DB.EFClasses.Picket picket, int length)
        {
            var picket_objects_pushed_out = dbContext.AllEquipments.Where(eq => eq.Picket == picket.number && Math.Abs(eq.shiftFromPicket) > length).ToList();

            if (picket_objects_pushed_out.Count > 0)
            {
                string error_message = "Невозможно изменить длину пикета, т.к. есть оборудование смещение которого больше, чем задаваемое значение длины пикета:\n";
                string objects_info = "";
                foreach (var db_object in picket_objects_pushed_out)
                {
                    objects_info += $"объект: {db_object.Name}, смещение: {db_object.shiftFromPicket / (10 * 100)} м, группа: {db_object.GroupObject.Name}, класс: {db_object.GroupObject.Class.Name}\n";
                }

                return error_message + objects_info;
            }

            bool positive_picket = picket.StartShiftLine >= 0;

            List<EFClasses.Picket> changing_pickets;
            if (positive_picket)
                changing_pickets = dbContext.Pickets.Where(pkt => pkt.path == picket.path && pkt.StartShiftLine >= picket.StartShiftLine).OrderBy(i => i.StartShiftLine).ToList();
            else
                changing_pickets = dbContext.Pickets.Where(pkt => pkt.path == picket.path && pkt.StartShiftLine <= picket.StartShiftLine).OrderByDescending(i => i.StartShiftLine).ToList();

            string error_str = "";
            Exception exception = null;
            using (var dbTransaction = dbContext.Database.BeginTransaction())
            {
                bool first_picket = true;
                int right_end_coordinate = 0;
                int left_end_coordinate = 0;
                foreach (var update_picket in changing_pickets)
                {
                    if (first_picket)
                    {
                        first_picket = false;
                        update_picket.Dlina = length;
                    }

                    if(positive_picket)
                    {
                        right_end_coordinate = update_picket.StartShiftLine + length;
                        left_end_coordinate = update_picket.StartShiftLine;
                    }
                    else
                    {
                        right_end_coordinate = update_picket.EndShiftLine;
                        left_end_coordinate = update_picket.EndShiftLine - length;
                    }

                    update_picket.EndShiftLine = right_end_coordinate;
                    update_picket.StartShiftLine = left_end_coordinate;

                    dbContext.Pickets.Attach(update_picket);
                    dbContext.Entry(update_picket).State = System.Data.Entity.EntityState.Modified;

                    update_objects_coordinates(update_picket);
                }

                try
                {
                    dbContext.SaveChanges();
                    dbTransaction.Commit();
                }
                catch (System.Data.Entity.Infrastructure.DbUpdateConcurrencyException exc)
                {
                    exception = exc;
                }
                catch(System.Data.Entity.Infrastructure.DbUpdateException exc)
                {
                    exception = exc;
                }
                catch (System.Data.Entity.Validation.DbEntityValidationException exc)
                {
                    exception = exc;
                }
                catch (System.NotSupportedException exc)
                {
                    exception = exc;
                }
                catch (System.InvalidOperationException exc)
                {
                    exception = exc;
                }

                if(exception != null)
                {
                    dbTransaction.Rollback();
                    error_str = exception.Message;
                }

            }
            return error_str;
        }

        private string update_objects_coordinates(DB.EFClasses.Picket picket)
        {
            foreach (var equipment in dbContext.AllEquipments.Where(eq => eq.Picket == picket.number))
            {
                if(picket.StartShiftLine > 0)
                    equipment.shiftLine = picket.StartShiftLine + equipment.shiftFromPicket;
                else
                    equipment.shiftLine = picket.EndShiftLine + equipment.shiftFromPicket;

                dbContext.AllEquipments.Attach(equipment);
                dbContext.Entry(equipment).State = System.Data.Entity.EntityState.Modified;

            }

            return "";

        }

        public static class PicketTraits
        {
            public const int DefaultLengthMm = 100000;
        }


        public void AddPickets(DB.EFClasses.Picket first_picket, int count, bool add_to_left)
        {
            int new_picket_id = dbContext.Pickets.Count() > 0 ? dbContext.Pickets.Max(p => p.number) : 0;
            new_picket_id++;

            string str_picket_number = "";

            EFClasses.Picket picket_at_end = null;
            {
                var pickets_at_path = add_to_left ?
                                    dbContext.Pickets.Where(pkt => pkt.path == first_picket.path).OrderBy(p => p.StartShiftLine)
                                    : dbContext.Pickets.Where(pkt => pkt.path == first_picket.path).OrderByDescending(p => p.StartShiftLine);

                if (pickets_at_path.Count() > 0)
                    picket_at_end = pickets_at_path.First();
            }

            if(picket_at_end == null)
            {
                picket_at_end = new EFClasses.Picket();

                int first_picket_number = first_picket.Npiketa != "" ? Convert.ToInt32(first_picket.Npiketa) : 0;
                picket_at_end.StartShiftLine = picket_at_end.EndShiftLine = first_picket_number * PicketTraits.DefaultLengthMm;
                str_picket_number = first_picket.Npiketa == "" ? "0" : first_picket.Npiketa;

            }
            else
            {
                str_picket_number = EFClasses.Picket.NextNumber(picket_at_end.Npiketa, add_to_left);
            }

            int picket_length = first_picket.Dlina;

            int i = 0;
            do {

                EFClasses.Picket picket = new EFClasses.Picket
                {
                    number = new_picket_id++,
                    Npiketa = str_picket_number,
                    NpicketBefore = add_to_left ? 0 : picket_at_end.number,
                    NpicketAfter = add_to_left ? picket_at_end.number : 0,
                    Dlina = picket_length,
                    path = first_picket.path,
                    StartShiftLine = add_to_left ? picket_at_end.StartShiftLine - picket_length : picket_at_end.EndShiftLine,
                    EndShiftLine = add_to_left ? picket_at_end.StartShiftLine : picket_at_end.EndShiftLine + picket_length

                };

                dbContext.Pickets.Add(picket);

                if (picket_at_end.number != 0)
                {
                    if (add_to_left)
                        picket_at_end.NpicketBefore = picket.number;
                    else
                        picket_at_end.NpicketAfter = picket.number;

                    if (i == 0)
                    {
                        dbContext.Pickets.Attach(picket_at_end);
                        dbContext.Entry(picket_at_end).State = System.Data.Entity.EntityState.Modified;
                    }

                }

                picket_at_end = picket;
                str_picket_number = EFClasses.Picket.NextNumber(picket_at_end.Npiketa, add_to_left);
                picket_length = PicketTraits.DefaultLengthMm;

            }
            while (++i < count);

            dbContext.SaveChanges();

        }
    }
}
