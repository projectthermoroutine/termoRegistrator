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
		using namespace events;
		using namespace synchronization;

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


		bool packets_manager_controller::get_info(const StartCommandEvent_packet& packet)
		{
			if (proccess_start_event_paket_func)
				return proccess_start_event_paket_func(packet);
			return false;
		}
		bool packets_manager_controller::get_info(const CoordinateCorrected_packet& packet)
		{
			if (proccess_coordinate_event_paket_func)
				return proccess_coordinate_event_paket_func(packet);
			return false;
		}
		bool packets_manager_controller::get_info(const PassportChangedEvent_packet& packet)
		{
			if (proccess_passport_event_paket_func)
				return proccess_passport_event_paket_func(packet);
			return false;
		}
		bool packets_manager_controller::get_info(const ReverseEvent_packet& packet)
		{
			if (proccess_reverse_event_paket_func)
				return proccess_reverse_event_paket_func(packet);
			return false;
		}
		bool packets_manager_controller::get_info(const StopCommandEvent_packet& packet)
		{
			if (proccess_stop_event_paket_func)
				return proccess_stop_event_paket_func(packet);
			return false;
		}

		template<typename TEvent>
		using retrieve_point_info_func_t = std::function<bool(const TEvent&, manager_track_traits&)>;
		template<typename TBaseEvent> class retrieve_point_info
		{
		public:
			template<typename TEvent,class TFunc> retrieve_point_info(const retrieve_point_info_func_t<TEvent>& func)
			{

			}

			bool operator()(const TBaseEvent& event)
			{

			}

		private:
			retrieve_point_info_func_t _func;

		};
		bool retrieve_start_point_info(const event_packet * packet, manager_track_traits& track_traits)
		{
			LOG_STACK();

			const StartCommandEvent_packet * event = reinterpret_cast<const StartCommandEvent_packet *>(packet);

			auto path_info_ = packets_manager_helpers::retrieve_path_info(*event);

			track_traits.counter0 = event->counter;
			path_info_->direction = 0;
			track_traits.direction = 1;
			if (event->track_settings.movement_direction != "Forward"){
				track_traits.direction = -1;
				path_info_->direction = 1;
			}

			track_traits.direction0 = track_traits.direction;

			auto positive_nonstandard_kms_tmp = event->track_settings.kms.positive_kms;
			auto negative_nonstandard_kms_tmp = event->track_settings.kms.negative_kms;
			prepare_nonstandart_kms(positive_nonstandard_kms_tmp);
			prepare_nonstandart_kms(negative_nonstandard_kms_tmp);

			track_traits.positive_nonstandard_kms.swap(positive_nonstandard_kms_tmp);
			track_traits.negative_nonstandard_kms.swap(negative_nonstandard_kms_tmp);


			auto * actual_nonstandart_kms = &track_traits.positive_nonstandard_kms;
			if (event->track_settings.user_start_item.coordinate_item.km < 0 ||
				event->track_settings.user_start_item.coordinate_item.m < 0 ||
				event->track_settings.user_start_item.coordinate_item.mm < 0)
			{
				actual_nonstandart_kms = &track_traits.negative_nonstandard_kms;
			}
			track_traits.coordinate0 = calculate_coordinate0(event->track_settings.user_start_item.coordinate_item, *actual_nonstandart_kms);

			//track_traits.time_span.first = sync_packet->timestamp;
			track_traits.counter_span.first = event->counter;

			track_traits._path_info.swap(path_info_);

			return true;

		}

	bool retrieve_change_point_info(
	const event_packet * event, manager_track_traits& track_traits
		)
	{
		LOG_STACK();

		const PassportChangedEvent_packet * packet = reinterpret_cast<const PassportChangedEvent_packet *>(event);

		auto path_info_ = packets_manager_helpers::retrieve_path_info(*packet);

		track_traits.counter0 = packet->counter;

		auto positive_nonstandard_kms_tmp = packet->change_passport_point_direction.kms.positive_kms;
		auto negative_nonstandard_kms_tmp = packet->change_passport_point_direction.kms.negative_kms;
		prepare_nonstandart_kms(positive_nonstandard_kms_tmp);
		prepare_nonstandart_kms(negative_nonstandard_kms_tmp);

		track_traits.positive_nonstandard_kms.swap(positive_nonstandard_kms_tmp);
		track_traits.negative_nonstandard_kms.swap(negative_nonstandard_kms_tmp);


		auto * actual_nonstandart_kms = &track_traits.positive_nonstandard_kms;
		if (packet->change_passport_point_direction.start_item.coordinate_item.km < 0 ||
			packet->change_passport_point_direction.start_item.coordinate_item.m < 0 ||
			packet->change_passport_point_direction.start_item.coordinate_item.mm < 0)
		{
			actual_nonstandart_kms = &track_traits.negative_nonstandard_kms;
		}
		track_traits.coordinate0 = calculate_coordinate0(packet->change_passport_point_direction.start_item.coordinate_item, *actual_nonstandart_kms);

		track_traits.counter_span.first = track_traits.counter0;

		if (path_info_->railway.empty())
			path_info_->railway = track_traits._path_info->railway;
		if (path_info_->line.empty())
			path_info_->line = track_traits._path_info->line;
		if (path_info_->path_name.empty())
			path_info_->path_name = track_traits._path_info->path_name;

		track_traits._path_info.swap(path_info_);

		return true;
	}
	bool retrieve_reverse_point_info(
		const event_packet * event, manager_track_traits& track_traits
		)
	{
		LOG_STACK();

		const ReverseEvent_packet * packet = reinterpret_cast<const ReverseEvent_packet *>(event);

		track_traits.coordinate0 = calculate_coordinate(track_traits.coordinate0, track_traits.direction*distance_from_counter(packet->counter, track_traits.counter0, track_traits.counter_size));

		track_traits.direction = track_traits.direction0;

		if (packet->is_reverse)	{
			track_traits.direction = -1 * track_traits.direction0;
		}

		uint8_t _direction = 1;
		if (track_traits.direction == 1)
			_direction = 0;

		if (track_traits._path_info)
			track_traits._path_info->direction = _direction;

		track_traits.counter0 = packet->counter;
		track_traits.counter_span.first = track_traits.counter0;

		return true;

	}
	bool retrieve_corrected_point_info(
		const event_packet * packet, manager_track_traits& track_traits
		)
	{
		LOG_STACK();
		const CoordinateCorrected_packet * event = reinterpret_cast<const CoordinateCorrected_packet *>(packet);

		int32_t _direction = 1;
		if (event->correct_direction.direction != "Forward"){
			_direction = -1;
		}

		//if (_direction != direction)
		//	return false;

		auto * actual_nonstandart_kms = &track_traits.positive_nonstandard_kms;
		if (event->correct_direction.coordinate_item.km < 0 ||
			event->correct_direction.coordinate_item.m < 0 ||
			event->correct_direction.coordinate_item.mm < 0)
		{
			actual_nonstandart_kms = &track_traits.negative_nonstandard_kms;
		}
		track_traits.counter0 = event->correct_direction.counter;
		track_traits.counter_span.first = track_traits.counter0;
		track_traits.direction = _direction;

		uint8_t direction_ = 1;
		if (track_traits.direction == 1)
			direction_ = 0;

		if (track_traits._path_info)
			track_traits._path_info->direction = direction_;

		track_traits.coordinate0 = calculate_coordinate0(event->correct_direction.coordinate_item, *actual_nonstandart_kms);

		return true;

	}


	}
}