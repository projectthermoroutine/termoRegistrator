#include <algorithm>
#include <functional>
#include <stdexcept>
#include <exception>
#include <loglib\log.h>
#include <memory>
#include <vector>

#include <map>
#include <mutex>
#include <common\on_exit.h>
#include <queue>

#include "position_detector_packets_manager.h"
#include "packets_manager_helpers.h"
#include  "pd_packets_manager_details.h"

namespace position_detector
{

	namespace packets_manager_ns
	{
		void
			prepare_nonstandart_kms(
			nonstandard_kms_t & nonstandard_kms
			)
		{
			std::vector<nonstandard_kms_t::key_type> keys_for_delete;
			for each(auto & item in nonstandard_kms)
			{
				if (item.second == STANDART_PICKET_SIZE_M){
					keys_for_delete.emplace_back(item.first);
				}
			}

			for each(auto & key in keys_for_delete)
			{
				nonstandard_kms.erase(key);
			}
		}


		void
			calculate_picket_offset(
			coordinate_t coordinate,
			const nonstandard_kms_map_t & nonstandard_kms,
			picket_t & picket,
			offset_t & offset
			)
		{

			LOG_STACK();

			coordinate_t znak_big = 1;
			int32_t znak = 1;
			if (coordinate < 0){
				znak_big = -1; znak = -1;
			}
			int32_t position_m = static_cast<int32_t>(coordinate / (10 * 100));

			if (nonstandard_kms.empty())
			{
				picket = static_cast<picket_t>(position_m / STANDART_PICKET_SIZE_M);
				offset = static_cast<picket_t>(coordinate - static_cast<coordinate_t>(picket)* STANDART_PICKET_SIZE_M * 10 * 100);
				return;
			}

			int32_t current_m = 0;
			int32_t last_non_standart_km = 0;
			int32_t last_calculated_m = 0;
			picket_t current_picket = -1;

			auto coordinate_temp = znak_big*coordinate;
			auto position_m_temp = znak*position_m;

			for each (auto & item in nonstandard_kms)
			{
				current_m += (item.first - last_non_standart_km) * STANDART_PICKET_SIZE_M;
				current_m += item.second;

				if (current_m >= position_m_temp)
				{
					if (current_m == position_m_temp){
						picket = znak*(item.first + 1);
						offset = znak*static_cast<picket_t>(coordinate_temp - (static_cast<coordinate_t>(position_m_temp * 10 * 100)));
						return;
					}

					if (position_m_temp > current_m - item.second &&
						last_non_standart_km != item.first &&
						current_picket != -1)
					{
						position_m_temp = current_m - item.second;
					}
					auto delta = (position_m_temp - last_calculated_m) / STANDART_PICKET_SIZE_M;
					picket = znak*(current_picket + 1 + delta);
					offset = znak*static_cast<picket_t>(coordinate_temp - (static_cast<coordinate_t>(last_calculated_m + delta * STANDART_PICKET_SIZE_M) * 10 * 100));
					return;
				}
				current_picket = static_cast<picket_t>(item.first);
				last_calculated_m = current_m;
				last_non_standart_km = item.first + 1;
			}

			auto delta = (position_m_temp - last_calculated_m) / STANDART_PICKET_SIZE_M;
			picket = znak*(last_non_standart_km + delta);
			offset = znak*static_cast<picket_t>(coordinate_temp - (static_cast<coordinate_t>(last_calculated_m + delta * STANDART_PICKET_SIZE_M) * 10 * 100));

		}

		static coordinate_t default_item_length = static_cast<coordinate_t>(CoordType::METRO);

		coordinate_t calculate_coordinate0(const coordinate_item_t& coordinate_item, const nonstandard_kms_t &nonstandard_kms)
		{
			LOG_STACK();

			coordinate_t _coord0 = coordinate_item.km * default_item_length * 100 * 10 + coordinate_item.m * 100 * 10 + coordinate_item.mm;
			coordinate_t _delta = 0;
			coordinate_t znak = 1;
			if (default_item_length == static_cast<coordinate_t>(CoordType::METRO))
			{
				if (!nonstandard_kms.empty())
				{
					if (_coord0 < 0)
						znak = -1;
					for each (auto & item in nonstandard_kms)
					{
						if (item.first >= (znak*coordinate_item.km))
							break;
						_delta += item.second - default_item_length;
					}
				}
			}

			return _coord0 + znak*_delta * 100 * 10;
		}
	}
}