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

#include <position_detector_common\position_detector_packets_manager.h>
#include <position_detector_common\details\packets_manager_helpers.h>
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

			manager_track_traits(manager_track_traits&& other)
			{
				swap(other);
			}

			manager_track_traits(const manager_track_traits& other) :
				counter0(other.counter0),
				coordinate0(other.coordinate0),
				direction(other.direction),
				direction0(other.direction0),
				counter_size(other.counter_size),
				positive_nonstandard_kms(other.positive_nonstandard_kms),
				negative_nonstandard_kms(other.negative_nonstandard_kms),
				_path_info(other._path_info),
				counter_span(other.counter_span)
			{		
			}

			manager_track_traits & operator=(manager_track_traits other) 
			{
				swap(other);
				return *this;
			}

			void lock() const { _mtx.lock(); }
			void unlock() const { _mtx.unlock(); }

			bool valid() const { return counter0 > 0; }

			position_detector::counter32_t counter0;
			coordinate_t coordinate0;
			uint32_t counter_size;
			nonstandard_kms_t positive_nonstandard_kms;
			nonstandard_kms_t negative_nonstandard_kms;
			int32_t direction;// текущее направление движения (-1 - на уменьшение координат; 1 - на увеличение координат)
			int32_t direction0;// начальное направление движения (-1 - на уменьшение координат; 1 - на увеличение координат)
			counter_span_t counter_span;

			path_info_ptr_t _path_info;

			void swap(manager_track_traits& b) 
			{
				using std::swap;
				swap(counter0, b.counter0);
				swap(coordinate0, b.coordinate0);
				swap(direction, b.direction);
				swap(direction0, b.direction0);
				swap(counter_size, b.counter_size);
				swap(positive_nonstandard_kms, b.positive_nonstandard_kms);
				swap(negative_nonstandard_kms, b.negative_nonstandard_kms);
				swap(_path_info, b._path_info);
				swap(counter_span, b.counter_span);
			}

			bool compare_path_info(const manager_track_traits &other) const
			{
				if (!_path_info || !other._path_info)
					return false;

				return  _path_info == other._path_info || *_path_info == *other._path_info;
			}

			bool compare_path_info_strong(const manager_track_traits &other) const
			{
				if (!_path_info || !other._path_info)
					return false;

				const bool result = _path_info == other._path_info || *_path_info == *other._path_info;
				
				return result ? counter0 == other.counter0 && coordinate0 == other.coordinate0 : false;
			}

		private:
			mutable std::mutex _mtx;
		};



		class coordinate_calculator final : public icoordinate_calculator
		{
		public:
			coordinate_calculator(const manager_track_traits& track_traits
				) :_track_traits(track_traits)
			{}

			virtual void calculate(track_point_info& info)
			{
				auto coordinate = calculate_coordinate(_track_traits.coordinate0, _track_traits.direction*distance_from_counter(info.counter, _track_traits.counter0, _track_traits.counter_size));

				info.coordinate = coordinate;
				info.counter_size = _track_traits.counter_size;

				auto * actual_nonstandart_kms = &_track_traits.positive_nonstandard_kms;
				if (coordinate < 0)
				{
					actual_nonstandart_kms = &_track_traits.negative_nonstandard_kms;
				}
				calculate_picket_offset(coordinate, *actual_nonstandart_kms, info.picket, info.offset);
				info._path_info = _track_traits._path_info;

			}
		private:
			manager_track_traits _track_traits;

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


		void retrieve_start_point_info(const StartCommandEvent_packet & event, manager_track_traits& track_traits);
		void retrieve_change_point_info(const PassportChangedEvent_packet & packet, manager_track_traits& track_traits);
		void retrieve_reverse_point_info(const ReverseEvent_packet & packet, manager_track_traits& track_traits);
		void retrieve_corrected_point_info(const CoordinateCorrected_packet & event, manager_track_traits& track_traits);


	}//namespace packets_manager_ns
}//namespace position_detector