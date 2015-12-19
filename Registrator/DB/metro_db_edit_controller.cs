using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Registrator.DB
{
    class metro_db_edit_controller: metro_db_controller
    {
        public metro_db_edit_controller(metro_db_controller controller)
            :base(controller)
        {       }

        public bool deletePicketFromDataBase(EquPicket _EquPicket)
        {
            EquPath _EquPath   = _EquPicket.Path;
            EquLine _EquLine   = _EquPath.Line;
            EquGroup _EquGroup = _EquLine.Group;
            EquClass _EquClass = _EquGroup.Class;

            var empData1 = (from r in all_equipment_table.AsEnumerable() where r.number == _EquPicket.Code select new { r.ClassNum, r.GroupNum, r.LineNum, r.Track, r.Layout, r.number }).Distinct();

            foreach (var item in empData1)
            {
                var emp = (from r in all_equipment_table.AsEnumerable() where r.ClassNum == item.ClassNum && r.GroupNum == item.GroupNum && r.Track == item.Track && r.Layout == item.Layout select new { r.Npicket });

                int res = 0;
                int cnt = emp.Count();
                res = all_equipment_adapter.delPicket(_EquClass.Code, _EquGroup.Code, _EquLine.Code, _EquPath.Code, 0, _EquPicket.Code, cnt);
            }

            object resDelPicket = pickets_adapter.delPicketFromDB(_EquPicket.number);

            refresh();

            return true;
        }

        public bool deletePathFromLine(EquPath _EquPath)
        {
            EquLine _EquLine = _EquPath.Line;
            EquGroup _EquGroup = _EquLine.Group;
            EquClass _EquClass = _EquGroup.Class;

            var res = trackAdapter.delPath(_EquPath.Code,_EquClass.Code, _EquGroup.Code, _EquLine.Code);

            all_equipment_table.Clear();
            all_equipment_adapter.Fill(all_equipment_table);
            trackTable.Clear();
            trackAdapter.Fill(trackTable);
            pickets_table.Clear();
            pickets_adapter.Fill(pickets_table);
            objects_table.Clear();
            objects_adapter.Fill(objects_table);

            return true;
        }

        public bool deleteLine(EquLine _EquLine)
        {
            EquGroup _EquGroup = _EquLine.Group;
            EquClass _EquClass = _EquGroup.Class;

            var res = lines_adapter.delLine(_EquClass.Code, _EquGroup.Code, _EquLine.Code);

            all_equipment_table.Clear();
            all_equipment_adapter.Fill(all_equipment_table);
            lines_table.Clear();
            lines_adapter.Fill(lines_table);
            trackTable.Clear();
            trackAdapter.Fill(trackTable);
            pickets_table.Clear();
            pickets_adapter.Fill(pickets_table);
            objects_table.Clear();
            objects_adapter.Fill(objects_table);

            return true;
        }

        public bool deleteLineFromDB(EquLine _EquLine)
        {
            //EquGroup _EquGroup = _EquLine.Group;
            //EquClass _EquClass = _EquGroup.Class;

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

            groups_table.Clear();
            groups_adapter.Fill(groups_table);
            all_equipment_table.Clear();
            all_equipment_adapter.Fill(all_equipment_table);
            lines_table.Clear();
            lines_adapter.Fill(lines_table);
            trackTable.Clear();
            trackAdapter.Fill(trackTable);
            pickets_table.Clear();
            pickets_adapter.Fill(pickets_table);
            objects_table.Clear();
            objects_adapter.Fill(objects_table);

            return true;
        }
        public bool deleteClass(EquClass _EquClass)
        {
            classes_adapter.delClass(_EquClass.Code);
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
