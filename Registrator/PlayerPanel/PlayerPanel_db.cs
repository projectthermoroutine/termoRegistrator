using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    using map_objects_list = List<measure_object>;

    public partial class PlayerPanel
    {

        protected map_objects_list _equipment_list = null;
        protected metro_map _metro_map;

        public void LoadObjectsFromDb()
        {
            _metro_map.LoadObjectsFromDb();
        }
        public map_objects_list get_equipment_list()
        {
            int objCount = _metro_map.GetEquListCount();
            var list = new map_objects_list();
            for (short i = 0; i < objCount; i++)
            {
                measure_object measure_object;

                if (!_metro_map.GetEquipmentElement(i, 0xFFFFFFFF, out measure_object))
                    continue;
                measure_object.frame_number = i;
                list.Add(measure_object);
            }

            return list;
        }

        public map_objects_list get_map_objects_list()
        {
            int objCount = _metro_map.GetMapObjectsCount();
            var list = new map_objects_list();
            for (short i = 0; i < objCount; i++)
            {
                measure_object measure_object;

                if (!_metro_map.GetMeasureObject(i, out measure_object))
                    continue;
                list.Add(measure_object);
            }

            return list;
        }

        public map_objects_list filter_equipment_list(map_objects_list equList)
        {
            var list = new map_objects_list();
            foreach (var map_object in equList)
            {
                var type = (int)map_object.type;
                if (((1 << type) & m_objFilter) == (1 << type))
                {
                    list.Add(map_object);
                }
            }

            return list;
        }

    }
}
