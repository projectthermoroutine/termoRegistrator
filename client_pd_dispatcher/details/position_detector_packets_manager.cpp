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
#include <queue>
#include <thread>
#include <atomic>

#ifndef TIMESTAMP_SYNCH_PACKET_ON
#include <list>
#endif


#include <array>

#include <position_detector_common\position_detector_packet.h>
#include <position_detector_common\details\position_detector_packet_details.h>
#include "position_detector_packets_manager.h"
#include "packets_manager_helpers.h"
#include "track_points_info_manager.h"

namespace position_detector
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

			if (current_m >= position_m_temp){
				int32_t popravka = 0;
				if (position_m_temp > current_m - item.second && 
					last_non_standart_km != item.first &&
					current_picket != -1){
					popravka = 1;
				}
				auto delta = (position_m_temp - last_calculated_m) / STANDART_PICKET_SIZE_M -popravka;
				picket = znak*(current_picket + 1 + delta);
				offset = znak*static_cast<picket_t>(coordinate_temp - (static_cast<coordinate_t>(last_calculated_m + delta * STANDART_PICKET_SIZE_M) * 10 * 100));
				return;
			}
			current_picket = static_cast<picket_t>(item.first);
			last_calculated_m = current_m;
			last_non_standart_km = item.first + 1;
		}

		auto delta = (position_m_temp - last_calculated_m) / STANDART_PICKET_SIZE_M;
		picket = znak*(current_picket + delta);
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

	static const unsigned int default_counter_size = 10;//mm

	using time_span_t = std::pair<time_t, time_t>;
	using counter_span_t = std::pair<synchronization::counter_t, synchronization::counter_t>;

	using event_guid_t = std::string;

	using retrive_point_info_func_t = std::function<bool(const event_packet *)>;

	struct packets_manager::Impl : public events::event_info
	{
		friend class event_parser;

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
		Impl(uint8_t _counter_size, coordinate_t _device_offset ,uint32_t _container_limit, CoordType coord_type) :container_limit(_container_limit),
			is_track_settings_set(false),
			counter_size((uint32_t)_counter_size),
			coordinate0(0),
			_stop_requested(false),
			_last_found_time(0),
			_state(State::ProcessSyncroPackets),
			direction(1),
			direction0(1),
			counter0(0),
			prev_counter(0),
			_track_points_info_counter(0),
			_coords_type(coord_type),
			device_offset(_device_offset)
		{

			LOG_STACK();

			counter_valid_span = 100 * 100 * 10;
			if (counter_size > 0)
				counter_valid_span /= counter_size;

			_retrieve_point_info_funcs.emplace_back(std::bind(&packets_manager::Impl::retrieve_start_point_info, this, std::placeholders::_1));
			_retrieve_point_info_funcs.emplace_back(std::bind(&packets_manager::Impl::retrieve_change_point_info, this, std::placeholders::_1));
			_retrieve_point_info_funcs.emplace_back(std::bind(&packets_manager::Impl::retrieve_reverse_point_info, this, std::placeholders::_1));
			//_retrieve_point_info_funcs[RETRIEVE_POINT_INFO_FUNC_INDEX::REVERSE] = std::bind(&packets_manager::Impl::retrieve_change_point_info, this, std::placeholders::_1);

			sync_packet_semaphore = sync_helpers::create_basic_semaphore_object(0);

			process_sync_packets_thread = std::thread([this](){this->process_sync_packets_loop(); });
		}

		virtual ~Impl() override
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
		std::vector<retrive_point_info_func_t> _retrieve_point_info_funcs;


	public:

		std::map<event_guid_t, event_packet_ptr_t> _event_packets_container;
		std::map<synchronization::counter_t, sync_packet_ptr_t> _synchro_packets_container;
		std::mutex _event_packets_mtx;
		std::mutex _synchro_packets_mtx;

		unsigned int container_limit;
		uint32_t counter_size;
		coordinate_t device_offset;

		std::map<synchronization::counter_t, sync_packet_ptr_t> _synchro_packets;

		volatile bool is_track_settings_set;

		std::queue<sync_packet_ptr_t> sync_packet_queue;
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
					dispatch_synchro_packet(data);
			}
		}


		void dispatch_synchro_packet(const sync_packet_ptr_t &packet)
		{

			calculation_mtx.lock();
			auto coordinate = calculate_coordinate(coordinate0, direction*distance_from_counter(packet->counter, counter0, counter_size));

			track_point_info data;

			data.counter = packet->counter;
			data.coordinate = coordinate;
			data.timestamp = packet->timestamp;
			data.speed = packet->speed;
			data.direction = packet->direction;
			data.valid = true;
			if (prev_counter > 0 &&
				(prev_counter + counter_valid_span < data.counter ||
				prev_counter - counter_valid_span > data.counter)
				){
				data.valid = false;
			}

			if (data.valid)
				prev_counter = data.counter;

			auto * actual_nonstandart_kms = &positive_nonstandard_kms;
			if ((coordinate + device_offset) < 0)
			{
				actual_nonstandart_kms = &negative_nonstandard_kms;
			}
			calculate_picket_offset(coordinate + device_offset, *actual_nonstandart_kms, data.picket, data.offset);
			data._path_info = _path_info;

			calculation_mtx.unlock();

			_track_points_info.append_point_info(data);
		}
	public:
		void dispatch_event_packet(const event_packet_ptr_t &packet)
		{
			LOG_STACK();

			if (_stop_requested)
				return;

			const auto iter = _event_packets_container.find(packet->guid);
			if (iter != _event_packets_container.cend())
				return;
			
			auto res = packet->get_info(this);

			if (res){
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

			if (_path_info)
				_path_info->direction = _direction;

			counter0 = packet->counter;
			counter_span.first = counter0;
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

			if (_direction != direction)
				return false;
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

			coordinate0 = calculate_coordinate0(event->correct_direction.coordinate_item, *actual_nonstandart_kms);

			return true;

		}


	private:
		std::map<synchronization::counter_t, sync_packet_ptr_t> _synchro_packets_tmp;
	protected:
		virtual bool get_info(const StartCommandEvent_packet& event)
		{
			LOG_STACK();

			if (!set_state(State::RetriveStartPoint))
				return false;

			utils::on_exit exit_guard([this](){ reset_state(); });

			LOG_TRACE() << event;

			if (is_track_settings_set)
				return false;
			//is_track_settings_set = false;


			auto & retrieve_point_info_func = _retrieve_point_info_funcs[(int)RETRIEVE_POINT_INFO_FUNC_INDEX::START];

			auto res = rebuild_track_point_info_container(&event, retrieve_point_info_func);
			if (res){
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
				if (counter_span.first <= iter->first){
					_synchro_packets_queue_mtx.lock();
					sync_packet_queue.push(iter->second);
					_synchro_packets_queue_mtx.unlock();
				}
			}
			_synchro_packets_mtx.unlock();
			if (count > 0)
				sync_helpers::release_semaphore(sync_packet_semaphore, count);

		}

		virtual bool get_info(const CoordinateCorrected_packet& event)
		{
			LOG_STACK();

			LOG_TRACE() << event;

			bool res = true;
			if (is_track_settings_set)
			{
				if (event.correct_direction.counter < counter0)
					return true;

				if (!set_state(State::ProcessCorrectedCoordinateEvent))
					return false;

				is_track_settings_set = false;

				res = retrieve_corrected_point_info(&event);

				is_track_settings_set = true;
				reset_state();
			}
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
		bool rebuild_track_point_info_container(const event_packet * event, const retrive_point_info_func_t& retrieve_point_info_func)
		{
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

				auto iter = _synchro_packets_container.lower_bound(event->counter);
				if (iter == _synchro_packets_container.end())
				{
					_synchro_packets_container.clear();
					_synchro_packets_mtx.unlock();
					return false;
				}
				retrieve_point_info_func(event);

				uint32_t count = 0;
				for (; iter != _synchro_packets_container.end(); ++iter, count++){
					if (counter_span.first <= iter->first){
						_synchro_packets_queue_mtx.lock();
						sync_packet_queue.push(iter->second);
						_synchro_packets_queue_mtx.unlock();
					}
				}
				if (count > 0)
					sync_helpers::release_semaphore(sync_packet_semaphore, count);
				_synchro_packets_container.clear();
				_synchro_packets_mtx.unlock();
			}
			else
			{
				retrieve_point_info_func(event);
				auto & res = _track_points_info.back();
				auto coordinate = calculate_coordinate(coordinate0, direction*distance_from_counter(res.counter, counter0, counter_size));
				res._path_info = _path_info;
				res.coordinate = coordinate;

				auto * actual_nonstandart_kms = &positive_nonstandard_kms;
				if ((coordinate + device_offset) < 0)
				{
					actual_nonstandart_kms = &negative_nonstandard_kms;
				}
				calculate_picket_offset(coordinate + device_offset, *actual_nonstandart_kms, res.picket, res.offset);

				_track_points_info.unlock();
			}
			return true;
		}

		virtual bool get_info(const PassportChangedEvent_packet& event)
		{
			LOG_STACK();
			LOG_TRACE() << event;

			if (is_track_settings_set)
			{
				if (counter_span.first > event.counter)
					return true;
				return process_event_packet(&event, State::ProcessChangePassportEvent, RETRIEVE_POINT_INFO_FUNC_INDEX::CHANGE_PASSPORT);
			}
			return true;

		}
		virtual bool get_info(const ReverseEvent_packet& event)
		{
			LOG_STACK();
			LOG_TRACE() << event;

			if (is_track_settings_set)
			{
				if (counter_span.first > event.counter)
					return true;

				return process_event_packet(&event, State::ProcessReverseEvent, RETRIEVE_POINT_INFO_FUNC_INDEX::REVERSE);
			}
			return true;
		}
		virtual bool get_info(const StopCommandEvent_packet& event)
		{
			LOG_STACK();
			LOG_TRACE() << event;

			if (!is_track_settings_set)
				return true;
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

			counter_span.second = event.counter;

			//clear();

			reset_state();
			return true;
		}

		void clear()
		{
			LOG_STACK();

			_synchro_packets_mtx.lock();
			_synchro_packets_container.clear();
			_synchro_packets_mtx.unlock();

			std::queue<sync_packet_ptr_t> _tmp_queue;
			_synchro_packets_queue_mtx.lock();
			_tmp_queue.swap(sync_packet_queue);
			_synchro_packets_queue_mtx.unlock();

		//	_event_packets_container.clear();

		}

	private:
		track_points_info_manager _track_points_info;
		mutable uint32_t _track_points_info_counter;

		track_point_info _currrent_track_settings;

		path_info_ptr_t _path_info;
		synchronization::counter_t counter0;
		synchronization::counter_t prev_counter;

	public:
		synchronization::counter_t counter_valid_span;
	private:

		std::mutex calculation_mtx;

		coordinate_t coordinate0;
		int32_t direction;
		int32_t direction0;

		nonstandard_kms_map_t positive_nonstandard_kms;
		nonstandard_kms_map_t negative_nonstandard_kms;

		CoordType _coords_type;


		time_span_t time_span;
		counter_span_t counter_span;


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


packets_manager::packets_manager(uint8_t counter_size, coordinate_t device_offset, unsigned int container_limit, CoordType coord_type) :
_p_impl(std::make_unique<packets_manager::Impl>(counter_size, device_offset, container_limit, coord_type))
	{
	}
	
	packets_manager::~packets_manager() = default;

	template<>
	void packets_manager::add_packet(const sync_packet_ptr_t & packet)
	{
		if (_p_impl->is_track_settings_set)
		{
			_p_impl->_synchro_packets_queue_mtx.lock();
				_p_impl->sync_packet_queue.push(packet);
			_p_impl->_synchro_packets_queue_mtx.unlock();

			sync_helpers::release_semaphore(_p_impl->sync_packet_semaphore);
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
		_p_impl->device_offset = device_offset;
	}


	void packets_manager::set_counter_size(uint8_t counter_size)
	{
		_p_impl->counter_size = (uint32_t)counter_size;
		
		if (counter_size > 0)
			_p_impl->counter_valid_span = 100 * 100 * 10 / counter_size;

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

}//namespace position_detector
