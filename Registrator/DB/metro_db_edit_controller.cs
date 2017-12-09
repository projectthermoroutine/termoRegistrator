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

        public int add_line(int class_code, int group_code, string line_name, string line_code, ref string error_msg)
        {
            try
            {
                int line_number = Convert.ToInt32(lines_adapter.selectMaxIndex());
                line_number++;

                var res = from r in lines_table.AsEnumerable() where r.LineNum != 0 && r.LineCode == line_code select new { r.LineNum };
                
                if(res.Count() > 1)
                {
                    error_msg = "В базе данных присутствуют одновременно две линии ";
                    return 0;
                }

                if (res.Count() == 0)
                    lines_adapter.add_line(line_number, line_name, 0, line_code);
                else
                    line_number = res.First().LineNum;

                //var lines_in_group = from r in all_equipment_table.AsEnumerable() where r.LineNum == line_number && r.GroupNum == group_code select new { r.LineNum };

                var qLinesInGroup = from m in dbContext.Mains where m.LineNum == line_number && m.GroupNum == group_code select new { m.LineNum };
               
                if(qLinesInGroup.Count() > 0)
                {
                    error_msg = "В выбранной группе уже присутствует добавляемая линия ";
                    return 0;

                }

                queriesAdapter.add_line_to_group(class_code, group_code, line_number);

                //all_equipment_adapter.add_line_to_group(class_code, group_code, line_number);

                //all_equipment_table.Clear();
                //all_equipment_adapter.Fill(all_equipment_table);

                layout_table.Clear();
                layout_adapter.Fill(layout_table);
                lines_table.Clear();
                lines_adapter.Fill(lines_table);

                return line_number;
            }
            catch (System.Data.SqlClient.SqlException e)
            {
                error_msg = e.Message;
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

            var res = trackAdapter.delPath(_EquPath.Code,_EquClass.Code, _EquGroup.Code, _EquLine.Code);

            //all_equipment_table.Clear();
            //all_equipment_adapter.Fill(all_equipment_table);
            //trackTable.Clear();
            //trackAdapter.Fill(trackTable);
            //pickets_table.Clear();
            //pickets_adapter.Fill(pickets_table);
            //objects_table.Clear();
            //objects_adapter.Fill(objects_table);

            return true;
        }

        public bool deleteLine(EquLine _EquLine)
        {
            EquGroup _EquGroup = _EquLine.Group;
            EquClass _EquClass = _EquGroup.Class;
           
            try
            {
                string error_msg = "";
                lines_adapter.delLine(_EquClass.Code, _EquGroup.Code, _EquLine.Code, ref error_msg);

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

            //all_equipment_table.Clear();
            //all_equipment_adapter.Fill(all_equipment_table);
            //lines_table.Clear();
            //lines_adapter.Fill(lines_table);
            //trackTable.Clear();
            //trackAdapter.Fill(trackTable);
            //pickets_table.Clear();
            //pickets_adapter.Fill(pickets_table);
            //objects_table.Clear();
            //objects_adapter.Fill(objects_table);

            return true;
        }

        public bool deleteLineFromDB(EquLine _equLine)
        {
            EquGroup _equGroup = _equLine.Group;
            EquClass _equClass = _equGroup.Class;

            string err = "";
            queriesAdapter.delLine(_equClass.Code, _equGroup.Code, _equLine.Code, ref err);
            queriesAdapter.delLineFromDB(_equLine.Code);


            //var empData1 = (from r in all_equipment_table.AsEnumerable() where r.LineNum == _EquLine.Code select new { r.GroupNum }).Distinct();

            //foreach (var item in empData1)
            //{
            //    var emp = (from r in all_equipment_table.AsEnumerable() where r.GroupNum == item.GroupNum select new { r.LineNum }).Distinct();

            //    int res = 0;
            //    int cnt = emp.Count();
            //    //res = all_equipment_adapter.delLine(_EquClass.Code, _EquGroup.Code, _EquLine.Code, cnt);
            //}

            //all_equipment_adapter.delLineFromDB(_EquLine.Code);

            //refresh();

            return true;
        }

        public bool deleteGroupFromClass(EquGroup _EquGroup)
        {
            ;
            EquClass _EquClass = _EquGroup.Class;
            
            try
            {
                string error_msg = "";
                
                groups_adapter.delGroup(_EquClass.Code, _EquGroup.Code,ref error_msg);
                
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

            //groups_table.Clear();
            //groups_adapter.Fill(groups_table);
            //all_equipment_table.Clear();
            //all_equipment_adapter.Fill(all_equipment_table);
            //lines_table.Clear();
            //lines_adapter.Fill(lines_table);
            //trackTable.Clear();
            //trackAdapter.Fill(trackTable);
            //pickets_table.Clear();
            //pickets_adapter.Fill(pickets_table);
            //objects_table.Clear();
            //objects_adapter.Fill(objects_table);

            return true;
        }
        public bool deleteClass(EquClass _EquClass)
        {
            try
            {
                string error_msg = "";
                var res = classes_adapter.delClass(_EquClass.Code, ref error_msg);
               
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

            var empData = from r in all_equipment_table.AsEnumerable() where r.ClassNum == _EquClass.Code && r.GroupNum == _EquGroup.Code && r.LineNum == _EquLine.Code && r.Track == _EquPath.Code && r.Npicket == _EquPicket.Code && r.Code == _EquObject.Code select new { r.Code };

            int cnt = empData.Count();
            int res = all_equipment_adapter.delEquip(_EquGroup.Code, _EquPath.Code, _EquPicket.Code, _EquObject.Code, cnt);

            var emp1 = from r in objects_table.AsEnumerable() where r.Object == _EquObject.Name select new { r.Code };

            if (emp1.Count() > 1)
                objects_adapter.DeleteQueryByCode(_EquObject.Code);

            return true;
        }

        public bool deleteEquipmentFromDB(EquObject _EquObject)
        {
            EquPicket _EquPicket = _EquObject.Picket;
            EquPath _EquPath = _EquPicket.Path;
            EquLine _EquLine = _EquPath.Line;
            EquGroup _EquGroup = _EquLine.Group;
            EquClass _EquClass = _EquGroup.Class;

            var resAllGroupForEquip = from r in all_equipment_table.AsEnumerable() where r.GroupNum == _EquGroup.Code && r.typeId == _EquObject.typeEquip select new { r.GroupNum };

            foreach (var item in resAllGroupForEquip)
            {
                var resAllTracksInGroup = from r in all_equipment_table.AsEnumerable() where r.GroupNum == item.GroupNum && r.typeId == _EquObject.typeEquip select new { r.Track };

                foreach (var itemTrack in resAllTracksInGroup)
                {
                    var resAllPicketInTrack = from r in all_equipment_table.AsEnumerable() where r.Track == itemTrack.Track && r.GroupNum == item.GroupNum && r.typeId == _EquObject.typeEquip select new { r.Npicket };

                    foreach (var itemPicket in resAllPicketInTrack)
                    {
                        var resAllSelectedEquipInPicket = from r in all_equipment_table.AsEnumerable() where r.Track == itemTrack.Track && r.GroupNum == item.GroupNum && r.Npicket == itemPicket.Npicket && r.typeId == _EquObject.typeEquip select new { r.Code };
                        var resAllEquipInPicket = from r in all_equipment_table.AsEnumerable() where r.Track == itemTrack.Track && r.GroupNum == item.GroupNum && r.Npicket == itemPicket.Npicket select new { r.Code };

                        if (resAllEquipInPicket.Count() > resAllSelectedEquipInPicket.Count())
                        {
                            foreach (var itemEquip in resAllSelectedEquipInPicket)
                                all_equipment_adapter.delEquip(_EquGroup.Code, _EquPath.Code, itemPicket.Npicket, itemEquip.Code, 0);

                        }
                        else
                        {
                            int i = 0;
                            foreach (var itemEquip in resAllSelectedEquipInPicket)
                            {
                                if (i == resAllSelectedEquipInPicket.Count() - 1)
                                {
                                    all_equipment_adapter.delEquip(_EquGroup.Code, _EquPath.Code, itemPicket.Npicket, itemEquip.Code, 1);
                                    break;
                                }

                                i++;
                                all_equipment_adapter.delEquip(_EquGroup.Code, _EquPath.Code, itemPicket.Npicket, itemEquip.Code, 0);
                            }
                        }
                    }
                }

            }

            refresh();

            return true;
        }

        public void saveArea(int equipID, Area area, bool saveAsDefault, DateTime dtime = new DateTime())
        {
            if(saveAsDefault)
            {
                objects_adapter.UpdateObjectArea(equipID, (int)area.X, (int)area.Y, (int)area.Height, (int)area.Width, (int)area.Type);
            }
            else
            {
                ObjectsFramesAdapter.UpdateObjectFramesArea(equipID, dtime, (int)area.X, (int)area.Y, (int)area.Height, (int)area.Width, (int)area.Type);
            }
        }

        public void updateEquipmentState(int equipID, int equipState)
        {
            objects_adapter.updateEquipState(equipID, equipState);
        }

        public void updateEquipmentPosition(int equipID, int X,int Y)
        {
            objects_adapter.updateObjectCoordinate(equipID, X, Y);
        }
   }
}
