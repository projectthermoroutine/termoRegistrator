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
#include <position_detector_common\position_detector_packet.h>
#include <position_detector_common\details\position_detector_packet_details.h>

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


		class coordinate_calculator final : public icoordinate_calculator
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


		using time_span_t = std::pair<time_t, time_t>;
		using counter_span_t = std::pair<synchronization::counter_t, synchronization::counter_t>;

		class manager_track_traits
		{
		public:
			manager_track_traits() :
				coordinate0(0),
				counter_size(0),
				direction(1),
				direction0(1),
				counter0(0)
			{}

			position_detector::counter_t counter0;
			coordinate_t coordinate0;
			uint32_t counter_size;
			nonstandard_kms_t positive_nonstandard_kms;
			nonstandard_kms_t negative_nonstandard_kms;
			int32_t direction;
			int32_t direction0;
			counter_span_t counter_span;

			path_info_ptr_t _path_info;
		};

		template<typename TEvent>
		using proccess_event_packet_func_t = std::function<bool(const TEvent&)>;

		class packets_manager_controller : public events::event_info
		{
		public:
			manager_track_traits track_traits;
			proccess_event_packet_func_t<StartCommandEvent_packet> proccess_start_event_paket_func;
			proccess_event_packet_func_t<CoordinateCorrected_packet> proccess_coordinate_event_paket_func;
			proccess_event_packet_func_t<PassportChangedEvent_packet> proccess_passport_event_paket_func;
			proccess_event_packet_func_t<ReverseEvent_packet> proccess_reverse_event_paket_func;
			proccess_event_packet_func_t<StopCommandEvent_packet> proccess_stop_event_paket_func;
		protected:
			virtual bool get_info(const StartCommandEvent_packet& packet);
			virtual bool get_info(const CoordinateCorrected_packet& packet);
			virtual bool get_info(const PassportChangedEvent_packet& packet);
			virtual bool get_info(const ReverseEvent_packet& packet);
			virtual bool get_info(const StopCommandEvent_packet& packet);
		};


		bool retrieve_start_point_info(const StartCommandEvent_packet & event, manager_track_traits& track_traits);
		bool retrieve_change_point_info(const PassportChangedEvent_packet & packet, manager_track_traits& track_traits);
		bool retrieve_reverse_point_info(const ReverseEvent_packet & packet, manager_track_traits& track_traits);
		bool retrieve_corrected_point_info(const CoordinateCorrected_packet & event, manager_track_traits& track_traits);


		template<typename TEventPacket, typename TManagerTrackTraits>
		using retrieve_point_info_func_t2 = std::function<bool(const TEventPacket *, TManagerTrackTraits&)>;

		template<typename TEvent, typename TManagerTrackTraits>
		using retrieve_point_info_func_t = std::function<bool(const TEvent&, TManagerTrackTraits&)>;

		template<typename TEventPacket, typename TManagerTrackTraits, typename TEvent>
		retrieve_point_info_func_t2<TEventPacket, TManagerTrackTraits> create_retrieve_point_info_func(const retrieve_point_info_func_t<TEvent, TManagerTrackTraits> & functor)
		{
			return[&](const TEventPacket *packet, TManagerTrackTraits& track_traits) -> bool
			{
				return functor(*(reinterpret_cast<const TEventPacket *>(packet)), track_traits);
			}
		}

	}//namespace packets_manager_ns
}//namespace position_detector