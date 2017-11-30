#pragma once

#include <position_detector_common\position_detector_packet.h>
#include <position_detector_common\details\position_detector_packet_details.h>
#include <client_pd_dispatcher\position_detector_packets_manager.h>
#include <client_pd_dispatcher\details\pd_packets_manager_details.h>

#include <array>
#include <list>


namespace position_detector
{
	namespace packets_manager_ns
	{
		using direction_t = int32_t;

		using change_coordinate_info_t = std::function<void(const position_detector::counter32_t&, const position_detector::counter32_t&, const manager_track_traits & track_traits)>;

		class track_events_queue_t
		{
		public:

			enum class race_detect_strategy_t : std::uint8_t
			{
				check_all_reverses = 0,
				check_only_start_point = 1
			};


			enum class queue_direction_t : std::size_t
			{
				device_behind = 0,
				device_ahead = 1
			};


			struct reverse_event_t
			{
				queue_direction_t queue_direction;
				manager_track_traits new_track_traits;
			};


			struct path_events_t
			{
				coordinate_t coordinate0;
				manager_track_traits start_path_track_traits;

				std::array<coordinate_t, 2> path_interval;

				struct event_counters_t
				{
					std::vector<synchronization::counter_t> counters;
					uint32_t cur_selection;// = 0;
				};
				std::array<event_counters_t, 2> event_counters;

				struct path_event_t
				{
					std::array<coordinate_t, 2> coordinate;
					manager_track_traits new_track_traits;
					std::array<event_counters_t, 2> event_counters;
				};

				std::list<path_event_t> events;
				std::list<std::list<reverse_event_t>::iterator> reverse_events;
			};

			const static coordinate_t coordinate_valid_span_default = 5 * 10 * 100; //mm (5m)

		public:
			track_events_queue_t(race_detect_strategy_t race_detect_strategy = race_detect_strategy_t::check_all_reverses, const coordinate_t & coordinate_valid_span = coordinate_valid_span_default) :
				_race_detect_strategy(race_detect_strategy),
				p_current_path_events_(nullptr),
				p_current_path_(nullptr), 
				current_path_iter_(device_events_queue_.end()),
				_next_reverse_event_for_cut(_reverse_events_list.end()),
				_cutter_offset_in_counter(0),
				_coordinate_valid_span(coordinate_valid_span),
				_cutter_direction(0),
				_queue_direction(queue_direction_t::device_ahead)
			{}

			void set_cutter_offset(const position_detector::counter_t &cutter_offset_in_counter) { _cutter_offset_in_counter = cutter_offset_in_counter; }
		public:

			void set_begin_path_info(manager_track_traits && track_traits, bool new_transit, bool cutter_behind);

		public:
			bool process_track(const coordinate_t & coordinate, const synchronization::counter_t & counter, manager_track_traits & track_traits, const coordinate_t & cutter_coordinate, packets_manager_ns::direction_t cutter_direction);

		public:
			void reverse(const coordinate_t & coordinate, manager_track_traits && track_traits);

			/* функции обработки событий проезда в случае движения устройством позади*/
			manager_track_traits defer_new_path_point_info(const coordinate_t & coordinate, manager_track_traits && track_traits);
			manager_track_traits defer_coordinate_correct_point_info(const coordinate_t & coordinate, manager_track_traits && track_traits);

			/* функции обработки событий проезда в случае движения устройством впереди*/
			manager_track_traits process_coordinate_correct_event(const coordinate_t & coordinate, packets_manager_ns::direction_t cutter_direction, manager_track_traits && track_traits, const change_coordinate_info_t& change_coordinate_info);
			manager_track_traits process_change_path_event(const coordinate_t & cutter_coordinate, packets_manager_ns::direction_t cutter_direction, const manager_track_traits & track_traits, const change_coordinate_info_t&);

		public:
			void reset();

		private:

			bool check_need_recalc_track(const coordinate_t & cutter_coordinate, packets_manager_ns::direction_t cutter_direction, const manager_track_traits & track_traits);
			void change_cutter_path(const manager_track_traits & track_traits);

			manager_track_traits recalc_track_intervals(const coordinate_t & cutter_coordinate, packets_manager_ns::direction_t cutter_direction, const manager_track_traits & track_traits, const change_coordinate_info_t&);

		private:

			using events_queue_t = std::list<path_events_t>;

			manager_track_traits _start_track_traits;

			events_queue_t device_events_queue_;
			std::list<path_events_t::path_event_t> * p_current_path_events_;
			path_events_t * p_current_path_;
			events_queue_t::iterator current_path_iter_;
			std::array<std::list<path_events_t::path_event_t>::iterator,2> next_event_;

			std::list<reverse_event_t> _reverse_events_list;
			std::list<reverse_event_t>::iterator _next_reverse_event_for_cut;

			queue_direction_t _queue_direction;
			position_detector::counter_t _cutter_offset_in_counter;

			coordinate_t _coordinate_valid_span;

			packets_manager_ns::direction_t _cutter_direction;// текущее направление движения (-1 - на уменьшение координат; 1 - на увеличение координат)

		private:
			race_detect_strategy_t _race_detect_strategy;

		};
	}//namespace packets_manager_ns
}//namespace position_detector