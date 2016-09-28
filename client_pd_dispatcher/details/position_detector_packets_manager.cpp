#include <Windows.h>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <exception>
#include <loglib\log.h>
#include <memory>
#include <vector>

#include <map>
#include <mutex>
#include <common\sync_helpers.h>
#include <common\on_exit.h>
#include <common\delegates.h>
#include <queue>
#include <thread>
#include <atomic>
#include <unordered_map>

#ifndef TIMESTAMP_SYNCH_PACKET_ON
#include <list>
#endif

#include <array>

#include <position_detector_common\position_detector_packet.h>
#include <position_detector_common\details\position_detector_packet_details.h>
#include "position_detector_packets_manager.h"
#include "packets_manager_helpers.h"
#include "track_points_info_manager.h"
#include "pd_packets_manager_details.h"


namespace position_detector
{
	struct device_event_traits
	{
		coordinate_t coordinate;
		path_info_ptr_t _path_info;
		bool operator==(const struct device_event_traits & other) const
		{
			if (!_path_info || !other._path_info)
				return false;

			return (_path_info.get() == other._path_info.get() ||
				*_path_info == *other._path_info);
		}

	};

}

namespace std
{
	template<>
	struct hash<position_detector::device_event_traits>
	{
		typedef position_detector::device_event_traits argument_type;
		typedef std::size_t result_type;

		result_type operator()(argument_type const& s) const
		{
			return hash<decltype(s._path_info->line)>()(s._path_info->line_path);
		}
	};

	template<>
	struct equal_to<position_detector::device_event_traits>
	{
		typedef position_detector::device_event_traits argument_type;
		bool operator()(const argument_type &lhs, const argument_type &rhs) const
		{
			return lhs == rhs;
		}
	};
}


namespace position_detector
{
	using namespace events;
	using namespace synchronization;
	using namespace packets_manager_ns;

	static const unsigned int default_counter_size = 10;//mm

	using time_span_t = std::pair<time_t, time_t>;
	using counter_span_t = std::pair<synchronization::counter_t, synchronization::counter_t>;

	using event_guid_t = std::string;

	template<typename TEvent>
	using priv_retrieve_point_info_func_t = std::function<bool(const TEvent&, manager_track_traits&)>;

	template<typename TEvent>
	retrieve_point_info_func_t2<event_packet, manager_track_traits> priv_create_retrieve_point_info_func(const priv_retrieve_point_info_func_t<TEvent> & functor)
	{
		return create_retrieve_point_info_func<event_packet, manager_track_traits, TEvent>(functor);
	}


	using sync_packet_shared_ptr_t = std::shared_ptr<synchro_packet_t>;

#define CHANGE_COORDINATE_ARGS synchronization::counter_t,const icoordinate_calculator&

	static coordinate_t default_item_length = static_cast<coordinate_t>(CoordType::METRO);
	struct packets_manager::Impl
	{
	private:
		enum class RETRIEVE_POINT_INFO_FUNC_INDEX
		{
			START,
			CHANGE_PASSPORT,
			REVERSE
		};

		enum class State{
			ProcessSyncroPackets,
			RetriveStartPoint,
			RetriveStopPoint,
			ProcessReverseEvent,
			ProcessChangePassportEvent,
			ProcessCorrectedCoordinateEvent
		};
	public:
		Impl(uint8_t _counter_size, coordinate_t _device_offset, DEVICE_LAYOUT _device_lyaout, uint32_t _container_limit, CoordType coord_type) :container_limit(_container_limit),
			is_track_settings_set(false),
			coordinate0(0),
			_stop_requested(false),
			_last_found_time(0),
			_state(State::ProcessSyncroPackets),
			device_layout(_device_lyaout),
			direction(1),
			direction0(1),
			counter0(0),
			prev_counter(0),
			_track_points_info_counter(0),
			_coords_type(coord_type),
			device_offset(_device_offset),
			_next_deferred_counter(0),
			_next_device_deferred_counter(0),
			valid_counter0_span(100),
			device_ahead_start_counter(0),
			device_offset_sign(1),
			orientation0(1),
			device_ahead0(true)
		{

			LOG_STACK();

			//if (valid_counter0_span > 0)
			//	valid_counter0_span /= counter_size;

			set_counter_size((uint32_t)_counter_size);

			_retrieve_point_info_funcs.emplace_back(priv_create_retrieve_point_info_func<StartCommandEvent_packet>(&retrieve_start_point_info));
			_retrieve_point_info_funcs.emplace_back(priv_create_retrieve_point_info_func<PassportChangedEvent_packet>(&retrieve_change_point_info));
			_retrieve_point_info_funcs.emplace_back(priv_create_retrieve_point_info_func<ReverseEvent_packet>(&retrieve_reverse_point_info));

			sync_packet_semaphore = sync_helpers::create_basic_semaphore_object(0);

			synchronizer_packets_ctrl.proccess_start_event_paket_func = [this](const StartCommandEvent_packet& packet){return this->get_info(packet); };
			synchronizer_packets_ctrl.proccess_coordinate_event_paket_func = [this](const CoordinateCorrected_packet& packet){return this->get_info(packet); };
			synchronizer_packets_ctrl.proccess_passport_event_paket_func = [this](const PassportChangedEvent_packet& packet){return this->get_info(packet); };
			synchronizer_packets_ctrl.proccess_reverse_event_paket_func = [this](const ReverseEvent_packet& packet){return this->get_info(packet); };
			synchronizer_packets_ctrl.proccess_stop_event_paket_func = [this](const StopCommandEvent_packet& packet){return this->get_info(packet); };

			//device_packets_ctrl.proccess_start_event_paket_func = [this](const StartCommandEvent_packet& packet){return this->get_info(packet); };
			device_packets_ctrl.proccess_coordinate_event_paket_func = [this](const CoordinateCorrected_packet& packet){return this->get_info_for_device(packet); };
			device_packets_ctrl.proccess_passport_event_paket_func = [this](const PassportChangedEvent_packet& packet){return this->get_info(packet); };


			process_sync_packets_thread = std::thread([this](){this->process_sync_packets_loop(); });
		}

		~Impl()
		{
			is_track_settings_set = false;
			_stop_requested = true;
			sync_helpers::release_semaphore(sync_packet_semaphore);
			if (process_sync_packets_thread.joinable())
			{
				process_sync_packets_thread.join();
			}

		}
	private:
		using events_queue = std::map<synchronization::counter_t, event_packet_ptr_t>;
		using device_events_queue_t = std::unordered_map <device_event_traits, event_packet_ptr_t >;


		struct path_events_t
		{
			coordinate_t coordinate0;
			manager_track_traits start_path_track_traits;
			std::map<coordinate_t, manager_track_traits> events;
		};

		using device_events_queue_t2 = std::list<path_events_t>;

		using calc_device_counter_func_t = synchronization::counter_t(*)(synchronization::counter_t, synchronization::counter_t);

		calc_device_counter_func_t calc_device_counter_func;

		packets_manager_controller synchronizer_packets_ctrl;
		manager_track_traits synchronizer_track_traits;

		packets_manager_controller device_packets_ctrl;
		manager_track_traits device_track_traits;

		device_events_queue_t2 device_events_queue;

		std::map<coordinate_t, manager_track_traits> _current_deffed_device_path_events;

		using get_point_info_func_t = std::function<bool(const event_packet*, manager_track_traits&)>;


		std::vector<get_point_info_func_t> _retrieve_point_info_funcs;

	public:

		std::function<void(CHANGE_COORDINATE_ARGS)> passportChanged;
		std::function<void(CHANGE_COORDINATE_ARGS)> coordinateCorrected;

		//signals::event<void(CHANGE_COORDINATE_ARGS)> passportChanged;
		//signals::event<void(CHANGE_COORDINATE_ARGS)> coordinateCorrected;

	public:

		std::map<event_guid_t, event_packet_ptr_t> _event_packets_container;
		std::map<synchronization::counter_t, sync_packet_shared_ptr_t> _synchro_packets_container;
		std::mutex _event_packets_mtx;
		std::mutex _synchro_packets_mtx;

		events_queue deferred_event_packet_queue;
		device_events_queue_t device_deferred_event_packet_queue;

		unsigned int container_limit;
		uint32_t counter_size;
		coordinate_t device_offset;
		DEVICE_LAYOUT device_layout;
		synchronization::counter_t device_offset_in_counter;
		bool device_ahead;
		synchronization::counter_t device_ahead_start_counter;

		std::map<synchronization::counter_t, sync_packet_ptr_t> _synchro_packets;

		volatile bool is_track_settings_set;

		std::queue<sync_packet_shared_ptr_t> sync_packet_queue;
		std::mutex _synchro_packets_queue_mtx;

		handle_holder sync_packet_semaphore;
		std::thread process_sync_packets_thread;



	private:

		volatile bool _stop_requested;
		void process_sync_packets_loop()
		{

			for (;;)
			{
				if (!sync_helpers::wait(sync_packet_semaphore))
				{
					break;
				}
				if (_stop_requested)
					break;

				_synchro_packets_queue_mtx.lock();
				if (sync_packet_queue.empty())
				{
					_synchro_packets_queue_mtx.unlock();
					if (_stop_requested)
						break;
					continue;
				}

				auto data = sync_packet_queue.front();
				sync_packet_queue.pop();
				_synchro_packets_queue_mtx.unlock();

				if (counter_span.first <= data->counter)
					dispatch_synchro_packet(*data);
			}
		}


		static synchronization::counter_t calc_device_counter_no_offset(synchronization::counter_t counter, synchronization::counter_t)
		{
			return counter;
		}
		static synchronization::counter_t calc_device_counter_ahead(synchronization::counter_t counter, synchronization::counter_t device_offset_counter)
		{
			return counter + device_offset_counter;
		}
		static synchronization::counter_t calc_device_counter_behind(synchronization::counter_t counter, synchronization::counter_t device_offset_counter)
		{
			return device_offset_counter <= counter ? counter - device_offset_counter : 0;
		}

		void _set_device_offset(coordinate_t _device_offset, bool lock)
		{
			if (lock){
				device_calculation_mtx.lock();
				synchronizer_calculation_mtx.lock();
			}
			device_offset = _device_offset;
			coordinate_t sign = device_offset >= 0 ? 1 : -1;

			device_offset_in_counter = static_cast<synchronization::counter_t>(sign*device_offset*counter_size);
			calc_device_counter_func = calc_device_counter_no_offset;
			if (device_offset != 0){
				if (device_offset < 0)
					calc_device_counter_func = calc_device_counter_behind;
				else
					calc_device_counter_func = calc_device_counter_ahead;
			}

			device_ahead = true;
			if (device_offset > 0 && direction0 < 0)
					device_ahead = false;
			else if (device_offset < 0 && direction0 > 0)
					device_ahead = false;


			if (lock){
				device_calculation_mtx.unlock();
				synchronizer_calculation_mtx.unlock();
			}
		}

		void dispatch_deffered_event_packets(synchronization::counter_t & next_deffered_counter, synchronization::counter_t counter, events_queue& deffered_events_queue)
		{
			if (next_deffered_counter > 0 && next_deffered_counter <= counter){
				auto deferred_event_packet = deffered_events_queue.begin()->second;
				deffered_events_queue.erase(next_deffered_counter);
				next_deffered_counter = 0;
				if (!deffered_events_queue.empty())
					next_deffered_counter = deffered_events_queue.cbegin()->first;

				dispatch_event_packet(deferred_event_packet);
			}
		}

		bool process_deffered_path_events(const coordinate_t & coordinate, const synchronization::counter_t & counter)
		{

			auto & first_path_info = device_events_queue.front();

			manager_track_traits track_traits;
			{
				std::lock_guard<decltype(device_calculation_mtx)> lock_device(device_calculation_mtx);
				track_traits = device_track_traits;
			}

			bool change_path = false;
			if (track_traits.direction*coordinate >= track_traits.direction*first_path_info.coordinate0)
			{
				_current_deffed_device_path_events = std::move(first_path_info.events);

				std::lock_guard<decltype(device_calculation_mtx)> lock_device(device_calculation_mtx);
				device_track_traits = std::move(track_traits);
				device_track_traits.counter0 = counter;
				device_events_queue.pop_front();
				return true;
			}


			auto iter = _current_deffed_device_path_events.lower_bound(track_traits.direction*coordinate);
			if (iter != _current_deffed_device_path_events.end())
			{
				manager_track_traits track_traits(std::move(iter->second));
				_current_deffed_device_path_events.erase(iter);
				std::lock_guard<decltype(device_calculation_mtx)> lock_device(device_calculation_mtx);
				device_track_traits = std::move(track_traits);
				device_track_traits.counter0 = counter;
				return true;
		}

			return false;
		}


		void deffer_coordinate_correct_point_info(manager_track_traits && track_traits)
		{
			auto & current_path_info = device_events_queue.back();
			current_path_info.events.emplace(track_traits.coordinate0, std::move(track_traits));
		}

		void deffer_start_path_point_info(const coordinate_t & coordinate, manager_track_traits && track_traits)
		{
			device_events_queue.emplace_back( coordinate, std::move(track_traits));
		}

		public:

			void set_device_offset(coordinate_t _device_offset)
			{
				_set_device_offset(_device_offset,true);
			}

			void set_counter_size(uint32_t _counter_size)
			{
				std::lock_guard<decltype(device_calculation_mtx)> lock_device(device_calculation_mtx);
				std::lock_guard<decltype(synchronizer_calculation_mtx)> lock(synchronizer_calculation_mtx);
				counter_size = _counter_size;

				counter_valid_span = 100 * 100 * 10;
				if (counter_size > 0)
					counter_valid_span /= counter_size;

				_set_device_offset(device_offset,false);


			}

		void dispatch_synchro_packet(const synchro_packet_t &packet)
		{
			LOG_STACK();

			dispatch_deffered_event_packets(_next_deferred_counter, packet.counter, deferred_event_packet_queue);

			//dispatch_deffered_event_packets(_next_device_deferred_counter, device_counter, device_deferred_event_packet_queue);

			track_point_info data;
			{
				manager_track_traits track_traits;
				{
					std::lock_guard<decltype(device_calculation_mtx)> lock_device(device_calculation_mtx);
					track_traits = device_track_traits;
				}

				if (counter_span.first > packet.counter){
				return;
			}

				auto coordinate = calculate_coordinate(track_traits.coordinate0, direction*distance_from_counter(packet.counter, track_traits.counter0, track_traits.counter_size));

				if(process_deffered_path_events(coordinate, packet.counter))
					coordinate = calculate_coordinate(track_traits.coordinate0, direction*distance_from_counter(packet.counter, track_traits.counter0, track_traits.counter_size));

				data.counter = packet.counter;
				data.counter_size = track_traits.counter_size;
				data.coordinate = coordinate;
				data.timestamp = packet.timestamp;
				data.speed = packet.speed;
				data.direction = packet.direction;
			data.valid = true;
			if (prev_counter > 0 &&
					(prev_counter + counter_valid_span < packet.counter ||
					prev_counter - counter_valid_span > packet.counter)
				)
			{
				data.valid = false;
			}

			if (data.valid && is_track_settings_set)
					prev_counter = packet.counter;

				auto * actual_nonstandart_kms = &track_traits.positive_nonstandard_kms;
				if (coordinate < 0)
			{
					actual_nonstandart_kms = &track_traits.negative_nonstandard_kms;
			}
				calculate_picket_offset(coordinate, *actual_nonstandart_kms, data.picket, data.offset);
				data._path_info = track_traits._path_info;

			}
			_track_points_info.append_point_info(data);
		}

		bool check_counter(const event_packet_ptr_t &packet)
		{
			LOG_STACK();

			LOG_TRACE() << L"Checking counter: " << packet->counter << ", start counter: " << synchronizer_track_traits.counter0 << ", current counter; " << prev_counter;

			if (synchronizer_track_traits.counter0 > valid_counter0_span && packet->counter < synchronizer_track_traits.counter0 - valid_counter0_span){
				LOG_TRACE() << L"Counter of event packet less first counter of the transit";
				return false;
			}

			if (prev_counter > 0 && prev_counter < packet->counter){
				LOG_TRACE() << L"Deffer event packet";
				deferred_event_packet_queue.emplace(std::pair<synchronization::counter_t, event_packet_ptr_t>{ packet->counter, packet });
				if (_next_deferred_counter == 0)
					_next_deferred_counter = packet->counter;
				return false;
			}

			return true;
		}

		bool deffer_event_for_device(event_packet * packet)
		{
			if (device_offset != 0){
				bool device_ahead = true;
				if (device_offset > 0 && direction < 0)
					device_ahead = false;
				else if (device_offset < 0 && direction > 0)
					device_ahead = false;
				if (!device_ahead){
					LOG_TRACE() << L"Deffer event packet for device";
//					device_deferred_event_packet_queue.emplace(std::pair<synchronization::counter_t, event_packet_ptr_t>{ packet->counter, event_packet_ptr_t(packet) });
					if (_next_device_deferred_counter == 0)
						_next_device_deferred_counter = packet->counter;
					return true;
				}
			}

			return false;
		}

		void dispatch_device_event_packet(const event_packet_ptr_t &packet)
		{
			LOG_STACK();

			if (_stop_requested)
				return;

			packet->get_info(&device_packets_ctrl);
		}


	public:
		void dispatch_event_packet(const event_packet_ptr_t &packet)
		{
			LOG_STACK();

			if (_stop_requested)
				return;

			if (!check_counter(packet))
				return;

			const auto iter = _event_packets_container.find(packet->guid);
			if (iter != _event_packets_container.cend()){
				LOG_TRACE() << L"Event packet with guid: '" << packet->guid.c_str() << "' already processed.";
				return;
			}
			
			auto res = packet->get_info(&synchronizer_packets_ctrl);

			if (res)
			{
				if (container_limit == _event_packets_container.size())
				{
					_event_packets_container.clear();
				}
				_event_packets_container.emplace(packet->guid, packet);
			}
		}

	public:

		bool retrieve_start_point_info(const event_packet * packet)
		{
			LOG_STACK();

			const StartCommandEvent_packet * event = reinterpret_cast<const StartCommandEvent_packet *>(packet);

			auto path_info_ = packets_manager_helpers::retrieve_path_info(*event);

			std::lock_guard<decltype(calculation_mtx)>  guard(calculation_mtx);
			counter0 = event->counter;
			path_info_->direction = 0;
			direction = 1;
			if (event->track_settings.movement_direction != "Forward"){
				direction = -1;
				path_info_->direction = 1;
			}
			
			direction0 = direction;

			auto positive_nonstandard_kms_tmp = event->track_settings.kms.positive_kms;
			auto negative_nonstandard_kms_tmp = event->track_settings.kms.negative_kms;
			prepare_nonstandart_kms(positive_nonstandard_kms_tmp);
			prepare_nonstandart_kms(negative_nonstandard_kms_tmp);

			positive_nonstandard_kms.swap(positive_nonstandard_kms_tmp);
			negative_nonstandard_kms.swap(negative_nonstandard_kms_tmp);


			auto * actual_nonstandart_kms = &positive_nonstandard_kms;
			if (event->track_settings.user_start_item.coordinate_item.km < 0 ||
				event->track_settings.user_start_item.coordinate_item.m < 0 ||
				event->track_settings.user_start_item.coordinate_item.mm < 0)
			{
				actual_nonstandart_kms = &negative_nonstandard_kms;
			}
			coordinate0 = calculate_coordinate0(event->track_settings.user_start_item.coordinate_item, *actual_nonstandart_kms);

			//time_span.first = sync_packet->timestamp;
			counter_span.first = event->counter;
			
			device_ahead = true;
			if (device_offset > 0 && direction < 0)
				device_ahead = false;
			else if (device_offset < 0 && direction > 0)
				device_ahead = false;


			if(device_ahead)
				device_ahead_start_counter = counter0;

			_path_info.swap(path_info_);

			return true;

		}

public:
		bool retrieve_change_point_info(
			const event_packet * event
			)
		{
			LOG_STACK();

			const PassportChangedEvent_packet * packet = reinterpret_cast<const PassportChangedEvent_packet *>(event);

			auto path_info_ = packets_manager_helpers::retrieve_path_info(*packet);

			std::lock_guard<decltype(calculation_mtx)>  guard(calculation_mtx);
			counter0 = packet->counter;

			auto positive_nonstandard_kms_tmp = packet->change_passport_point_direction.kms.positive_kms;
			auto negative_nonstandard_kms_tmp = packet->change_passport_point_direction.kms.negative_kms;
			prepare_nonstandart_kms(positive_nonstandard_kms_tmp);
			prepare_nonstandart_kms(negative_nonstandard_kms_tmp);

			positive_nonstandard_kms.swap(positive_nonstandard_kms_tmp);
			negative_nonstandard_kms.swap(negative_nonstandard_kms_tmp);


			auto * actual_nonstandart_kms = &positive_nonstandard_kms;
			if (packet->change_passport_point_direction.start_item.coordinate_item.km < 0 ||
				packet->change_passport_point_direction.start_item.coordinate_item.m < 0 ||
				packet->change_passport_point_direction.start_item.coordinate_item.mm < 0)
			{
				actual_nonstandart_kms = &negative_nonstandard_kms;
			}
			coordinate0 = calculate_coordinate0(packet->change_passport_point_direction.start_item.coordinate_item, *actual_nonstandart_kms);

			counter_span.first = counter0;

			if (path_info_->railway.empty())
				path_info_->railway = _path_info->railway;
			if (path_info_->line.empty())
				path_info_->line = _path_info->line;
			if (path_info_->path_name.empty())
				path_info_->path_name = _path_info->path_name;

			_path_info.swap(path_info_);

			if (device_ahead)
				device_ahead_start_counter = counter0;

			//if (device_offset != 0){
			//	bool device_ahead = true;
			//	if (device_offset > 0 && direction < 0)
			//		device_ahead = false;
			//	else if (device_offset < 0 && direction > 0)
			//		device_ahead = false;
			//	if (device_ahead)
			//		passportChanged(counter0, coordinate_calculator({ counter0, coordinate0, counter_size, positive_nonstandard_kms, negative_nonstandard_kms, direction0 }, _path_info));
			//}
			return true;
		}

		bool retrieve_reverse_point_info(
			const event_packet * event
			)
		{
			LOG_STACK();

			const ReverseEvent_packet * packet = reinterpret_cast<const ReverseEvent_packet *>(event);
			std::lock_guard<decltype(calculation_mtx)>  guard(calculation_mtx);

			coordinate0 = calculate_coordinate(coordinate0, direction*distance_from_counter(packet->counter, counter0, counter_size));

			direction = direction0;

			if (packet->is_reverse)	{
				direction = -1 * direction0;
			}

			uint8_t _direction = 1;
			if (direction == 1)
				_direction = 0;


			auto prev_device_ahead = device_ahead;
			device_ahead = true;
			if (device_offset > 0 && direction < 0)
				device_ahead = false;
			else if (device_offset < 0 && direction > 0)
				device_ahead = false;

			if (!prev_device_ahead && device_ahead)
				device_ahead_start_counter = counter0;


			return true;

		}

		bool retrieve_corrected_point_info(
			const event_packet * packet
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
			std::lock_guard<decltype(calculation_mtx)>  guard(calculation_mtx);

			auto * actual_nonstandart_kms = &positive_nonstandard_kms;
			if (event->correct_direction.coordinate_item.km < 0 ||
				event->correct_direction.coordinate_item.m < 0 ||
				event->correct_direction.coordinate_item.mm < 0)
			{
				actual_nonstandart_kms = &negative_nonstandard_kms;
			}
			counter0 = event->correct_direction.counter;
			counter_span.first = counter0;
			direction = _direction;

			uint8_t direction_ = 1;
			if (direction == 1)
				direction_ = 0;

			if (_path_info)
				_path_info->direction = direction_;

			coordinate0 = calculate_coordinate0(event->correct_direction.coordinate_item, *actual_nonstandart_kms);

			auto prev_device_ahead = device_ahead;
			device_ahead = true;
			if (device_offset > 0 && direction < 0)
				device_ahead = false;
			else if (device_offset < 0 && direction > 0)
				device_ahead = false;
			if (!prev_device_ahead && device_ahead)
				device_ahead_start_counter = counter0;

			//if (device_offset != 0){
			//	bool device_ahead = true;
			//	if (device_offset > 0 && direction < 0)
			//		device_ahead = false;
			//	else if (device_offset < 0 && direction > 0)
			//		device_ahead = false;
			//	if (device_ahead)
			//		coordinateCorrected(counter0, coordinate_calculator({ counter0, coordinate0, counter_size, positive_nonstandard_kms, negative_nonstandard_kms, direction0 }, _path_info));
			//}
			//else
			//if (prev_counter > counter0)
			//	coordinateCorrected(counter0, coordinate_calculator({ counter0, coordinate0, counter_size, positive_nonstandard_kms, negative_nonstandard_kms, direction0 }, _path_info));

			return true;

		}

	private:
		std::map<synchronization::counter_t, sync_packet_ptr_t> _synchro_packets_tmp;
	public:
		bool get_info(const StartCommandEvent_packet& event)
		{
			LOG_STACK();

			if (!set_state(State::RetriveStartPoint))
				return false;

			utils::on_exit exit_guard([this](){ reset_state(); });

			LOG_TRACE() << event;

			if (is_track_settings_set){
				LOG_TRACE() << L"Transit already started.";
				return false;
			}
			auto & retrieve_point_info_func = _retrieve_point_info_funcs[(int)RETRIEVE_POINT_INFO_FUNC_INDEX::START];

			auto res = rebuild_track_point_info_container(&event, retrieve_point_info_func);
			if (res){
				LOG_TRACE() << L" Starting new transit.";
				_track_points_info.lock();
				_track_points_info.clear();
				_track_points_info.unlock();
				is_track_settings_set = true;
				//queue_sync_packets(_synchro_packets_container);
				_event_packets_container.clear();

			}
			return res;

		}
		template<typename TContainer>
		void queue_sync_packets(TContainer& container)
		{
			int32_t count = 0;
			_synchro_packets_mtx.lock();
			for (auto iter = container.begin(); iter != container.end(); count++, iter++){
				if (synchronizer_track_traits.counter_span.first <= iter->first){
					_synchro_packets_queue_mtx.lock();
					sync_packet_queue.push(iter->second);
					_synchro_packets_queue_mtx.unlock();
				}
			}
			_synchro_packets_mtx.unlock();
			if (count > 0)
				sync_helpers::release_semaphore(sync_packet_semaphore, count);

		}

		bool get_info(const CoordinateCorrected_packet& event)
		{
			LOG_STACK();

			LOG_TRACE() << event;

			bool res = true;
			if (is_track_settings_set)
			{
				if (event.correct_direction.counter < synchronizer_track_traits.counter0)
					return true;

				if (!set_state(State::ProcessCorrectedCoordinateEvent))
					return false;

				//if (deffer_event_for_device(&event));
				//	return true;

				is_track_settings_set = false;
				int32_t prev_direction;
				{
					if (!device_ahead)
					{
						manager_track_traits track_traits;
						{
							std::lock_guard<decltype(synchronizer_calculation_mtx)>  guard(synchronizer_calculation_mtx);
							prev_direction = synchronizer_track_traits.direction;
							res = retrieve_corrected_point_info(event, synchronizer_track_traits);
							track_traits = synchronizer_track_traits;
						}
					}



				}

				if (res){
					if (prev_direction != synchronizer_track_traits.direction){
						CoordinateCorrected_packet* corrected_packet = const_cast<CoordinateCorrected_packet*>(&event);
						int32_t device_offset_km = static_cast<int32_t>(device_offset / (10 * 100 * default_item_length));
						int32_t orig_km = corrected_packet->correct_direction.coordinate_item.km;
						int32_t orig_m = corrected_packet->correct_direction.coordinate_item.m;
						corrected_packet->correct_direction.coordinate_item.km += device_offset_km;
						corrected_packet->correct_direction.coordinate_item.m += (int32_t)device_offset / (10 * 100) - device_offset_km * (int32_t)default_item_length;
						dispatch_device_event_packet(event_packet_ptr_t(corrected_packet));
						corrected_packet->correct_direction.coordinate_item.km = orig_km;
						corrected_packet->correct_direction.coordinate_item.m = orig_m;
					}
					else
					{
						if (device_offset != 0){
							if (device_ahead){
								//	coordinateCorrected(counter0, coordinate_calculator({ counter0, coordinate0, counter_size, positive_nonstandard_kms, negative_nonstandard_kms, direction0 }, _path_info));
							}
							else
								deffer_event_for_device(const_cast<CoordinateCorrected_packet*>(&event));
						}
						else{
							dispatch_device_event_packet(event_packet_ptr_t(const_cast<CoordinateCorrected_packet*>(&event)));
						}
					}
				}

				is_track_settings_set = true;
				reset_state();
			}
			return res;
		}

		bool get_info_for_device(const CoordinateCorrected_packet& event)
		{
			LOG_STACK();

			LOG_TRACE() << event;

			bool res = true;
			{
				std::lock_guard<decltype(device_calculation_mtx)>  guard(device_calculation_mtx);
				res = retrieve_corrected_point_info(event, device_track_traits);
			}

			auto prev_device_ahead = device_ahead;
			device_ahead = true;
			if (device_offset > 0 && device_track_traits.direction < 0)
				device_ahead = false;
			else if (device_offset < 0 && device_track_traits.direction > 0)
				device_ahead = false;
			if (!prev_device_ahead && device_ahead)
				device_ahead_start_counter = device_track_traits.counter0;

			//if (device_offset != 0){
			//	bool device_ahead = true;
			//	if (device_offset > 0 && direction < 0)
			//		device_ahead = false;
			//	else if (device_offset < 0 && direction > 0)
			//		device_ahead = false;
			//	if (device_ahead)
			//		coordinateCorrected(counter0, coordinate_calculator({ counter0, coordinate0, counter_size, positive_nonstandard_kms, negative_nonstandard_kms, direction0 }, _path_info));
			//}
			//else
			//if (prev_counter > counter0)
			//	coordinateCorrected(counter0, coordinate_calculator({ counter0, coordinate0, counter_size, positive_nonstandard_kms, negative_nonstandard_kms, direction0 }, _path_info));

			return res;
		}


		bool process_event_packet(const event_packet * event,State state,RETRIEVE_POINT_INFO_FUNC_INDEX index)
		{
			LOG_STACK();

			if (!set_state(state))
				return false;

			is_track_settings_set = false;

			utils::on_exit exit_guard([this](){ reset_state(); });

			auto & retrieve_point_info_func = _retrieve_point_info_funcs[(int)index];

			auto res = rebuild_track_point_info_container(event, retrieve_point_info_func);
			is_track_settings_set = true;
		/*	if (res)
				queue_sync_packets(_synchro_packets_container);*/
			
			return res;
		}
		bool rebuild_track_point_info_container(const event_packet * event, const get_point_info_func_t& retrieve_point_info_func)
		{
			LOG_STACK();
			auto start_counter = event->counter;
			bool has_info = false;
			_track_points_info.lock();
			if (!_track_points_info.empty()){
				const auto& last_point = _track_points_info.back();
				if (last_point.counter >= start_counter)
					has_info = true;
			}

			if (!has_info)
			{
				_track_points_info.unlock();

				//_synchro_packets_tmp.clear();
				_synchro_packets_mtx.lock();
				/*_synchro_packets_tmp.swap(_synchro_packets_container);
				_synchro_packets_mtx.unlock();*/

				{
					std::lock_guard<decltype(synchronizer_calculation_mtx)>  guard(synchronizer_calculation_mtx);
					retrieve_point_info_func(event, synchronizer_track_traits);
				}

				auto iter = _synchro_packets_container.lower_bound(event->counter);
				if (iter == _synchro_packets_container.end())
				{
					LOG_TRACE() << L"Container synchro packets does not contain packets which counter greater or equal " << event->counter;
					LOG_TRACE() << L"Container synchro packets size: " << _synchro_packets_container.size();
					_synchro_packets_container.clear();
					_synchro_packets_mtx.unlock();
					return true;
				}

				uint32_t count = 0;
				for (; iter != _synchro_packets_container.end(); ++iter, ++count){
					if (synchronizer_track_traits.counter_span.first <= iter->first){
						std::lock_guard<decltype(_synchro_packets_queue_mtx)> lock(_synchro_packets_queue_mtx);
						sync_packet_queue.push(iter->second);
					}
				}
				if (count > 0)
					sync_helpers::release_semaphore(sync_packet_semaphore, count);
				_synchro_packets_container.clear();
				_synchro_packets_mtx.unlock();
			}
			else
			{
				{
					std::lock_guard<decltype(synchronizer_calculation_mtx)>  guard(synchronizer_calculation_mtx);
					retrieve_point_info_func(event, synchronizer_track_traits);
				}
				auto & res = _track_points_info.back();
				auto coordinate = calculate_coordinate(synchronizer_track_traits.coordinate0, synchronizer_track_traits.direction*distance_from_counter(res.counter, synchronizer_track_traits.counter0, synchronizer_track_traits.counter_size));
				res._path_info = synchronizer_track_traits._path_info;
				res.coordinate = coordinate;

				auto * actual_nonstandart_kms = &synchronizer_track_traits.positive_nonstandard_kms;
				if ((coordinate + device_offset) < 0)
				{
					actual_nonstandart_kms = &synchronizer_track_traits.negative_nonstandard_kms;
				}
				calculate_picket_offset(coordinate + device_offset, *actual_nonstandart_kms, res.picket, res.offset);

				_track_points_info.unlock();
			}
			return true;
		}

		bool get_info(const PassportChangedEvent_packet& event)
		{
			LOG_STACK();
			LOG_TRACE() << event;

			bool res = true;
			if (is_track_settings_set)
			{
				if (synchronizer_track_traits.counter_span.first > event.counter)
					return true;

			/*	if (deffer_event_for_device(&event));
					return true;*/

				auto res = process_event_packet(&event, State::ProcessChangePassportEvent, RETRIEVE_POINT_INFO_FUNC_INDEX::CHANGE_PASSPORT);

				if (res){
					if (device_offset != 0){
						if (device_ahead){

							passportChanged(counter0, coordinate_calculator(
								{ counter0, coordinate0, synchronizer_track_traits.counter_size, positive_nonstandard_kms, negative_nonstandard_kms, direction0 }, _path_info));
			}
						else
							deffer_event_for_device(const_cast<PassportChangedEvent_packet*>(&event));
					}
					else{
						dispatch_device_event_packet(event_packet_ptr_t(const_cast<PassportChangedEvent_packet*>(&event)));
					}
				}
			}
			return res;

		}
		bool get_info(const ReverseEvent_packet& event)
		{
			LOG_STACK();
			LOG_TRACE() << event;

			if (is_track_settings_set)
			{
				if (synchronizer_track_traits.counter_span.first > valid_counter0_span && event.counter < synchronizer_track_traits.counter_span.first - valid_counter0_span)
					return true;

				auto res = process_event_packet(&event, State::ProcessReverseEvent, RETRIEVE_POINT_INFO_FUNC_INDEX::REVERSE);
				if (res)
					proccess_reverse_event_paket_func(event);
				return res;
			}
			return true;
		}

		void proccess_reverse_event_paket_func(const ReverseEvent_packet& event)
		{
			std::lock_guard<decltype(device_calculation_mtx)>  guard(device_calculation_mtx);
			retrieve_reverse_point_info(event, device_track_traits);

			auto prev_device_ahead = device_ahead;
			device_ahead = true;
			if (device_offset > 0 && device_track_traits.direction < 0)
				device_ahead = false;
			else if (device_offset < 0 && device_track_traits.direction > 0)
				device_ahead = false;

			if (!prev_device_ahead && device_ahead)
				device_ahead_start_counter = device_track_traits.counter0;
		}

		bool get_info(const StopCommandEvent_packet& event)
		{
			LOG_STACK();
			LOG_TRACE() << event;

			if (!is_track_settings_set){
				LOG_TRACE() << "Transit was already stoped.";
				return true;
			}
			if(!set_state(State::RetriveStopPoint))
				return false;

			is_track_settings_set = false;

			auto start_counter = event.counter;
			bool stoped = false;
			_track_points_info.lock();
			if (!_track_points_info.empty()){
				const auto& last_point = _track_points_info.back();
				if (last_point.counter >= start_counter)
					stoped = true;
			}
			if (!stoped)
			{
				_track_points_info.unlock();
				is_track_settings_set = true;

				queue_sync_packets(_synchro_packets_container);
				reset_state();
				return false;
			}

			_track_points_info.unlock();


			synchronizer_track_traits.counter_span.second = event.counter;

			synchronizer_calculation_mtx.lock();
			synchronizer_track_traits.counter0 = 0;
			synchronizer_calculation_mtx.unlock();
			device_calculation_mtx.lock();
			prev_counter = 0;
			device_track_traits.counter0 = 0;
			device_calculation_mtx.unlock();


			LOG_TRACE() << "Transit successfully stoped.";

			reset_state();
			return true;
		}

		void clear()
		{
			LOG_STACK();

			{
				std::lock_guard<decltype(_synchro_packets_mtx)> lock(_synchro_packets_mtx);
			_synchro_packets_container.clear();
			}

			decltype(sync_packet_queue) _tmp_queue;
			{	
				std::lock_guard<decltype(_synchro_packets_queue_mtx)> lock(_synchro_packets_queue_mtx);
			_tmp_queue.swap(sync_packet_queue);
			}

		//	_event_packets_container.clear();

		}

	private:
		track_points_info_manager _track_points_info;
		mutable uint32_t _track_points_info_counter;

		track_point_info _currrent_track_settings;

		path_info_ptr_t _path_info;
		synchronization::counter_t counter0;
		synchronization::counter_t counter0_device;
		synchronization::counter_t valid_counter0_span;

		synchronization::counter_t prev_counter;
		synchronization::counter_t _next_deferred_counter;
		synchronization::counter_t _next_device_deferred_counter;

	public:
		synchronization::counter_t counter_valid_span;
	private:

		std::mutex synchronizer_calculation_mtx;
		std::mutex device_calculation_mtx;

		coordinate_t coordinate0;
		coordinate_t coordinate0_device;
		int32_t direction;
		int32_t direction0;

		int32_t orientation0; // 1 - Salon , -1 - Boiler
		int32_t device_offset_sign;
		bool device_ahead0;

		nonstandard_kms_map_t positive_nonstandard_kms;
		nonstandard_kms_map_t negative_nonstandard_kms;

		CoordType _coords_type;


		position_detector::time_span_t time_span;
		position_detector::counter_span_t counter_span;


		inline bool set_state(State state)
		{
			auto prev_state = _InterlockedCompareExchange8((char*)(&_state), (char)state, (char)State::ProcessSyncroPackets);
			return prev_state == (char)State::ProcessSyncroPackets;
		}
		inline void reset_state(){ _InterlockedExchange8((char*)(&_state),(char)State::ProcessSyncroPackets); }

		volatile State _state;

public:
	bool get_last_point_info(track_point_info& info) const
	{
		if (!is_track_settings_set)
			return false;

		if (_track_points_info.empty())
			return false;

		_track_points_info.get_last_point_info(info);
		return true;
	}

private:
	time_t _last_found_time;
	};


	packets_manager::packets_manager(uint8_t counter_size, coordinate_t device_offset, DEVICE_LAYOUT device_lyaout, unsigned int container_limit, CoordType coord_type) :
_p_impl(std::make_unique<packets_manager::Impl>(counter_size, device_offset, container_limit, coord_type))
	{
	}
	
	packets_manager::~packets_manager() = default;

	template<>
	void packets_manager::add_packet(const sync_packet_ptr_t & packet)
	{
		LOG_STACK();

		if (_p_impl->is_track_settings_set)
		{
			_p_impl->dispatch_synchro_packet(*packet);

			//_p_impl->_synchro_packets_queue_mtx.lock();
			//	_p_impl->sync_packet_queue.push(packet);
			//_p_impl->_synchro_packets_queue_mtx.unlock();

			//sync_helpers::release_semaphore(_p_impl->sync_packet_semaphore);
		}
		else
		{
			std::lock_guard<std::mutex> guard(_p_impl->_synchro_packets_mtx);

			if (_p_impl->container_limit == _p_impl->_synchro_packets_container.size())
			{
				_p_impl->_synchro_packets_container.clear();
			}
			_p_impl->_synchro_packets_container.emplace(packet->counter, packet);
		}
	}

	void packets_manager::set_device_offset(coordinate_t device_offset)
	{
		_p_impl->set_device_offset(device_offset);
	}

	void packets_manager::set_counter_size(uint8_t counter_size)
	{
		_p_impl->set_counter_size((uint32_t)counter_size);
	}
	template<>
	void packets_manager::add_packet(const event_packet_ptr_t & packet)
	{
		_p_impl->dispatch_event_packet(packet);

	}
#ifdef TIMESTAMP_SYNCH_PACKET_ON
	bool packets_manager::get_point_info_by_time(const time_t &time, track_point_info& info)
	{
		return _p_impl->get_point_info_by_time(time, info);
	}
#else
	bool packets_manager::get_last_point_info(track_point_info& info) const
	{
		return _p_impl->get_last_point_info(info);
	}
#endif

	void packets_manager::add_passport_changed_process_func(const passport_changed_process_func_t& func)
	{
		_p_impl->passportChanged = func;
		//_p_impl->passportChanged += func;
	}
	void packets_manager::add_coordinate_corrected_process_func(const coordinate_corrected_process_func_t& func)
	{
		_p_impl->coordinateCorrected = func;
		//_p_impl->coordinateCorrected += func;
	}
	void packets_manager::remove_passport_changed_process_func(const passport_changed_process_func_t&)
	{
		_p_impl->passportChanged = std::function<void(CHANGE_COORDINATE_ARGS)>();
		//_p_impl->passportChanged -= func;
	}
	void packets_manager::remove_coordinate_corrected_process_func(const coordinate_corrected_process_func_t&)
	{
		_p_impl->coordinateCorrected = std::function<void(CHANGE_COORDINATE_ARGS)>();
		//_p_impl->coordinateCorrected -= func;
	}



}//namespace position_detector

//bool retrieve_start_point_info(const event_packet * packet)
//{
//	LOG_STACK();
//
//	const StartCommandEvent_packet * event = reinterpret_cast<const StartCommandEvent_packet *>(packet);
//
//	auto path_info_ = packets_manager_helpers::retrieve_path_info(*event);
//
//	std::lock_guard<decltype(calculation_mtx)>  guard(calculation_mtx);
//	counter0 = event->counter;
//	path_info_->direction = 0;
//	direction = 1;
//	if (event->track_settings.movement_direction != "Forward"){
//		direction = -1;
//		path_info_->direction = 1;
//	}
//
//	direction0 = direction;
//
//	auto positive_nonstandard_kms_tmp = event->track_settings.kms.positive_kms;
//	auto negative_nonstandard_kms_tmp = event->track_settings.kms.negative_kms;
//	prepare_nonstandart_kms(positive_nonstandard_kms_tmp);
//	prepare_nonstandart_kms(negative_nonstandard_kms_tmp);
//
//	positive_nonstandard_kms.swap(positive_nonstandard_kms_tmp);
//	negative_nonstandard_kms.swap(negative_nonstandard_kms_tmp);
//
//
//	auto * actual_nonstandart_kms = &positive_nonstandard_kms;
//	if (event->track_settings.user_start_item.coordinate_item.km < 0 ||
//		event->track_settings.user_start_item.coordinate_item.m < 0 ||
//		event->track_settings.user_start_item.coordinate_item.mm < 0)
//	{
//		actual_nonstandart_kms = &negative_nonstandard_kms;
//	}
//	coordinate0 = calculate_coordinate0(event->track_settings.user_start_item.coordinate_item, *actual_nonstandart_kms);
//
//	//time_span.first = sync_packet->timestamp;
//	counter_span.first = event->counter;
//
//	device_ahead = true;
//	if (device_offset > 0 && direction < 0)
//		device_ahead = false;
//	else if (device_offset < 0 && direction > 0)
//		device_ahead = false;
//
//
//	if (device_ahead)
//		device_ahead_start_counter = counter0;
//
//	_path_info.swap(path_info_);
//
//	return true;
//
//}

//public:
//	bool retrieve_change_point_info(
//		const event_packet * event
//		)
//	{
//		LOG_STACK();
//
//		const PassportChangedEvent_packet * packet = reinterpret_cast<const PassportChangedEvent_packet *>(event);
//
//		auto path_info_ = packets_manager_helpers::retrieve_path_info(*packet);
//
//		std::lock_guard<decltype(calculation_mtx)>  guard(calculation_mtx);
//		counter0 = packet->counter;
//
//		auto positive_nonstandard_kms_tmp = packet->change_passport_point_direction.kms.positive_kms;
//		auto negative_nonstandard_kms_tmp = packet->change_passport_point_direction.kms.negative_kms;
//		prepare_nonstandart_kms(positive_nonstandard_kms_tmp);
//		prepare_nonstandart_kms(negative_nonstandard_kms_tmp);
//
//		positive_nonstandard_kms.swap(positive_nonstandard_kms_tmp);
//		negative_nonstandard_kms.swap(negative_nonstandard_kms_tmp);
//
//
//		auto * actual_nonstandart_kms = &positive_nonstandard_kms;
//		if (packet->change_passport_point_direction.start_item.coordinate_item.km < 0 ||
//			packet->change_passport_point_direction.start_item.coordinate_item.m < 0 ||
//			packet->change_passport_point_direction.start_item.coordinate_item.mm < 0)
//		{
//			actual_nonstandart_kms = &negative_nonstandard_kms;
//		}
//		coordinate0 = calculate_coordinate0(packet->change_passport_point_direction.start_item.coordinate_item, *actual_nonstandart_kms);
//
//		counter_span.first = counter0;
//
//		if (path_info_->railway.empty())
//			path_info_->railway = _path_info->railway;
//		if (path_info_->line.empty())
//			path_info_->line = _path_info->line;
//		if (path_info_->path_name.empty())
//			path_info_->path_name = _path_info->path_name;
//
//		_path_info.swap(path_info_);
//
//		if (device_ahead)
//			device_ahead_start_counter = counter0;
//
//		//if (device_offset != 0){
//		//	bool device_ahead = true;
//		//	if (device_offset > 0 && direction < 0)
//		//		device_ahead = false;
//		//	else if (device_offset < 0 && direction > 0)
//		//		device_ahead = false;
//		//	if (device_ahead)
//		//		passportChanged(counter0, coordinate_calculator({ counter0, coordinate0, counter_size, positive_nonstandard_kms, negative_nonstandard_kms, direction0 }, _path_info));
//		//}
//		return true;
//	}
//
//	bool retrieve_reverse_point_info(
//		const event_packet * event
//		)
//	{
//		LOG_STACK();
//
//		const ReverseEvent_packet * packet = reinterpret_cast<const ReverseEvent_packet *>(event);
//		std::lock_guard<decltype(calculation_mtx)>  guard(calculation_mtx);
//
//		coordinate0 = calculate_coordinate(coordinate0, direction*distance_from_counter(packet->counter, counter0, counter_size));
//
//		direction = direction0;
//
//		if (packet->is_reverse)	{
//			direction = -1 * direction0;
//		}
//
//		uint8_t _direction = 1;
//		if (direction == 1)
//			_direction = 0;
//
//		if (_path_info)
//			_path_info->direction = _direction;
//
//		counter0 = packet->counter;
//		counter_span.first = counter0;
//
//		auto prev_device_ahead = device_ahead;
//		device_ahead = true;
//		if (device_offset > 0 && direction < 0)
//			device_ahead = false;
//		else if (device_offset < 0 && direction > 0)
//			device_ahead = false;
//
//		if (!prev_device_ahead && device_ahead)
//			device_ahead_start_counter = counter0;
//
//
//		return true;
//
//	}
//
//	bool retrieve_corrected_point_info(
//		const event_packet * packet
//		)
//	{
//		LOG_STACK();
//		const CoordinateCorrected_packet * event = reinterpret_cast<const CoordinateCorrected_packet *>(packet);
//
//		int32_t _direction = 1;
//		if (event->correct_direction.direction != "Forward"){
//			_direction = -1;
//		}
//
//		//if (_direction != direction)
//		//	return false;
//		std::lock_guard<decltype(calculation_mtx)>  guard(calculation_mtx);
//
//		auto * actual_nonstandart_kms = &positive_nonstandard_kms;
//		if (event->correct_direction.coordinate_item.km < 0 ||
//			event->correct_direction.coordinate_item.m < 0 ||
//			event->correct_direction.coordinate_item.mm < 0)
//		{
//			actual_nonstandart_kms = &negative_nonstandard_kms;
//		}
//		counter0 = event->correct_direction.counter;
//		counter_span.first = counter0;
//		direction = _direction;
//
//		uint8_t direction_ = 1;
//		if (direction == 1)
//			direction_ = 0;
//
//		if (_path_info)
//			_path_info->direction = direction_;
//
//		coordinate0 = calculate_coordinate0(event->correct_direction.coordinate_item, *actual_nonstandart_kms);
//
//		auto prev_device_ahead = device_ahead;
//		device_ahead = true;
//		if (device_offset > 0 && direction < 0)
//			device_ahead = false;
//		else if (device_offset < 0 && direction > 0)
//			device_ahead = false;
//		if (!prev_device_ahead && device_ahead)
//			device_ahead_start_counter = counter0;
//
//		//if (device_offset != 0){
//		//	bool device_ahead = true;
//		//	if (device_offset > 0 && direction < 0)
//		//		device_ahead = false;
//		//	else if (device_offset < 0 && direction > 0)
//		//		device_ahead = false;
//		//	if (device_ahead)
//		//		coordinateCorrected(counter0, coordinate_calculator({ counter0, coordinate0, counter_size, positive_nonstandard_kms, negative_nonstandard_kms, direction0 }, _path_info));
//		//}
//		//else
//		//if (prev_counter > counter0)
//		//	coordinateCorrected(counter0, coordinate_calculator({ counter0, coordinate0, counter_size, positive_nonstandard_kms, negative_nonstandard_kms, direction0 }, _path_info));
//
//		return true;
//
//	}

