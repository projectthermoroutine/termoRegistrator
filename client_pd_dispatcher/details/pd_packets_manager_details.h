#pragma once
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

namespace position_detector
{
	namespace packets_manager_ns
	{

		using namespace events;
		using namespace synchronization;


		inline
			coordinate_t
			distance_from_counter(
			synchronization::counter_t counter,
			synchronization::counter_t counter0,
			uint32_t counter_size
			)
		{
			return (counter - counter0)*counter_size;
		}

		inline
			coordinate_t
			calculate_coordinate(
			coordinate_t coordinate0,
			coordinate_t distance
			)
		{
			return coordinate0 + distance;
		}

#define STANDART_PICKET_SIZE_M 100L

		void
			prepare_nonstandart_kms(
			nonstandard_kms_t & nonstandard_kms
			);

		void
			calculate_picket_offset(
			coordinate_t coordinate,
			const nonstandard_kms_map_t & nonstandard_kms,
			picket_t & picket,
			offset_t & offset
			);

		coordinate_t 
			calculate_coordinate0(
			const coordinate_item_t& coordinate_item, 
			const nonstandard_kms_t &nonstandard_kms
			);


		struct track_traits
		{
			position_detector::counter_t counter0;
			coordinate_t coordinate0;
			uint32_t counter_size;
			nonstandard_kms_t positive_nonstandard_kms;
			nonstandard_kms_t negative_nonstandard_kms;

			int32_t direction;
		};


		class coordinate_calculator final : icoordinate_calculator
		{
		public:
			coordinate_calculator(const track_traits& track_traits,
				const path_info_ptr_t & path_info
				) :_track_traits(track_traits),
				_path_info(path_info)
			{}

			virtual void calculate(position_detector::counter_t counter, int32_t direction, track_point_info& info)
			{
				auto coordinate = calculate_coordinate(_track_traits.coordinate0, direction*distance_from_counter(counter, _track_traits.counter0, _track_traits.counter_size));

				info.counter = counter;
				info.coordinate = coordinate;

				auto * actual_nonstandart_kms = &_track_traits.positive_nonstandard_kms;
				if (coordinate < 0)
				{
					actual_nonstandart_kms = &_track_traits.negative_nonstandard_kms;
				}
				calculate_picket_offset(coordinate, *actual_nonstandart_kms, info.picket, info.offset);
				info._path_info = _path_info;

			}
		private:
			track_traits _track_traits;
			path_info_ptr_t _path_info;

		};

	}//namespace packets_manager
}//namespace position_detector