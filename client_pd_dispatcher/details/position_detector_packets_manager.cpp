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
			CHANGE_PASSPORT,
			REVERSE
		};

		enum class State{
			ProcessSyncroPackets,
			RetriveStartPoint,
			RetriveStopPoint,
			ProcessReverseEvent,
			ProcessChangePassportEvent
		};
	public:
		Impl(uint8_t _counter_size, uint32_t _container_limit, CoordType coord_type) :container_limit(_container_limit),
			is_track_settings_set(false),
			counter_size((uint32_t)_counter_size),
			coordinate0(0),
			_stop_requested(false),
			_last_found_time(0),
			_state(State::ProcessSyncroPackets),
			direction(1),
			_track_points_info_counter(0),
			_coords_type(coord_type)
		{

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

				dispatch_synchro_packet(data);
			}
		}


		void dispatch_synchro_packet(const sync_packet_ptr_t &packet)
		{
			auto coordinate = calculate_coordinate(coordinate0, direction*distance_from_counter(packet->counter, counter0, counter_size));

			track_point_info data;

			data.counter = packet->counter;
			data.coordinate = coordinate;
			data.timestamp = packet->timestamp;
			data.speed = packet->speed;
			data.direction = packet->direction;

			data._path_info = _path_info;

			track_points_lock.lock(true);
#ifdef TIMESTAMP_SYNCH_PACKET_ON
			_track_points_info.emplace(data._movment_info.timestamp, data);
#else
			if (_track_points_info_counter++ == container_limit){
				_track_points_info.pop_front();
				_track_points_info_counter--;
			}
			_track_points_info.push_back(data);
#endif
			track_points_lock.unlock(true);

		}
	public:
		void dispatch_event_packet(const event_packet_ptr_t &packet)
		{
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

	private:
		coordinate_t calculate_coordinate0(const coordinate_item_t& coordinate_item, const nonstandard_kms_item_t &nonstandard_kms)
		{

			coordinate_t _coord0 = coordinate_item.km * static_cast<decltype(coordinate0)>(_coords_type)* 100 * 10 + coordinate_item.m * 100 * 10 + coordinate_item.mm;
			coordinate_t _delta = 0;
			coordinate_t default_item_length = static_cast<coordinate_t>(_coords_type);
			if (_coords_type == CoordType::METRO)
			{
				if (!nonstandard_kms.empty())
				{
					for each (auto & item in nonstandard_kms)
					{
						if ((coordinate_t)coordinate_item.km > item.first)
						{
							_delta += item.second - default_item_length;
						}
					}
				}
			}

			return _coord0 + _delta;
		}

		bool retrieve_start_point_info(const StartCommandEvent_packet& event, const sync_packet_ptr_t& sync_packet)
		{
			auto path_info_ = packets_manager_helpers::retrieve_path_info(event);

			counter0 = event.counter;
			path_info_->direction = 0;
			direction = 1;
			if (event.track_settings.movement_direction != "Forward"){
				direction = -1;
				path_info_->direction = 1;
			}

			coordinate0 = calculate_coordinate0(event.track_settings.user_start_item.coordinate_item, event.track_settings.kms);

			time_span.first = sync_packet->timestamp;
			counter_span.first = event.counter;
			
			_path_info.swap(path_info_);

			return true;

		}

public:
		bool retrieve_change_point_info(
			const event_packet * event
			)
		{
			const PassportChangedEvent_packet * packet = reinterpret_cast<const PassportChangedEvent_packet *>(event);

			auto path_info_ = packets_manager_helpers::retrieve_path_info(*packet);

			counter0 = packet->counter;

			coordinate0 = calculate_coordinate0(packet->change_passport_point_direction.start_item.coordinate_item, packet->change_passport_point_direction.kms);

			counter_span.first = counter0;

			_path_info.swap(path_info_);
			return true;
		}

		bool retrieve_reverse_point_info(
			const event_packet * event
			)
		{
			const ReverseEvent_packet * packet = reinterpret_cast<const ReverseEvent_packet *>(event);

			coordinate0 = calculate_coordinate(coordinate0, direction*distance_from_counter(packet->counter, counter0, counter_size));

			direction = 1;

			if (packet->is_reverse)	{
				direction = -1;
				if (_path_info)
					_path_info->direction = 1;
			}
			else
			if (_path_info)
				_path_info->direction = 0;


			counter0 = packet->counter;

			counter_span.first = counter0;
			return true;

		}



	private:
		std::map<synchronization::counter_t, sync_packet_ptr_t> _synchro_packets_tmp;
	protected:
		virtual bool get_info(const StartCommandEvent_packet& event)
		{
			if (!set_state(State::RetriveStartPoint))
				return false;

			is_track_settings_set = false;

			_synchro_packets_tmp.clear();
			_synchro_packets_mtx.lock();
			_synchro_packets_tmp.swap(_synchro_packets_container);
			_synchro_packets_mtx.unlock();

			auto iter = _synchro_packets_tmp.lower_bound(event.counter);
			if (iter == _synchro_packets_tmp.end())
			{
				_synchro_packets_tmp.clear();
				reset_state();
				return false;
			}

			const auto & syncro_packet = iter->second;
			auto res = retrieve_start_point_info(event, syncro_packet);
			if (!res)
			{
				_synchro_packets_tmp.clear();
				reset_state();
				return false;
			}

			uint32_t count = 0;
			for (; iter != _synchro_packets_tmp.end(); ++iter, count++){
				sync_packet_queue.push(iter->second);
			}
			sync_helpers::release_semaphore(sync_packet_semaphore, count);
			_synchro_packets_tmp.clear();

			is_track_settings_set = true;

			count = 0;
			_synchro_packets_mtx.lock();
			for (iter = _synchro_packets_container.begin(); iter != _synchro_packets_container.end(); count++, iter++){
				_synchro_packets_queue_mtx.lock();
					sync_packet_queue.push(iter->second);
				_synchro_packets_queue_mtx.unlock();
			}
			_synchro_packets_mtx.unlock();

			if (count > 0)
				sync_helpers::release_semaphore(sync_packet_semaphore, count);

			_event_packets_container.clear();

			reset_state();

			return true;

		}
		virtual bool get_info(const CoordinateCorrected_packet& )
		{
			return false;
		}

		bool process_event_packet(const event_packet * event,State state,RETRIEVE_POINT_INFO_FUNC_INDEX index)
		{
			if (!set_state(state))
				return false;

			is_track_settings_set = false;

			auto & retrieve_point_info_func = _retrieve_point_info_funcs[(int)index];

			auto start_counter = event->counter;
			track_points_lock.lock(true);

			auto res = std::find_if(_track_points_info.begin(), _track_points_info.end(), [start_counter](const track_point_info & item){return start_counter >= item.counter; });

			if (res == _track_points_info.end())
			{
				track_points_lock.unlock(true);

				_synchro_packets_tmp.clear();
				_synchro_packets_mtx.lock();
				_synchro_packets_tmp.swap(_synchro_packets_container);
				_synchro_packets_mtx.unlock();

				auto iter = _synchro_packets_tmp.lower_bound(event->counter);
				if (iter == _synchro_packets_tmp.end())
				{
					_synchro_packets_tmp.clear();
					is_track_settings_set = true;
					reset_state();
					return false;
				}
				retrieve_point_info_func(event);

				uint32_t count = 0;
				for (; iter != _synchro_packets_tmp.end(); ++iter, count++){
					sync_packet_queue.push(iter->second);
				}
				sync_helpers::release_semaphore(sync_packet_semaphore, count);
				_synchro_packets_tmp.clear();
			}
			else
			{
				retrieve_point_info_func(event);
				for (; res != _track_points_info.end(); res++)
				{
					auto coordinate = calculate_coordinate(coordinate0, direction*distance_from_counter(res->counter, counter0, counter_size));
					res->_path_info = _path_info;
					res->coordinate = coordinate;
				}
				track_points_lock.unlock(true);
			}

			is_track_settings_set = true;
			uint32_t count = 0;
			_synchro_packets_mtx.lock();
			for (auto iter = _synchro_packets_container.begin(); iter != _synchro_packets_container.end(); count++, iter++){
				_synchro_packets_queue_mtx.lock();
				sync_packet_queue.push(iter->second);
				_synchro_packets_queue_mtx.unlock();
			}
			_synchro_packets_mtx.unlock();
			if (count > 0)
				sync_helpers::release_semaphore(sync_packet_semaphore, count);

			reset_state();
			return true;

		}

		virtual bool get_info(const PassportChangedEvent_packet& event)
		{
			if (is_track_settings_set)
				return process_event_packet(&event, State::ProcessChangePassportEvent, RETRIEVE_POINT_INFO_FUNC_INDEX::CHANGE_PASSPORT);
			return false;

		}
		virtual bool get_info(const ReverseEvent_packet& event)
		{
			if (is_track_settings_set)
				return process_event_packet(&event, State::ProcessReverseEvent, RETRIEVE_POINT_INFO_FUNC_INDEX::REVERSE);
			return false;
		}
		virtual bool get_info(const StopCommandEvent_packet& event)
		{
			if (!is_track_settings_set || !set_state(State::RetriveStopPoint))
				return false;

			is_track_settings_set = false;

			auto start_counter = event.counter;
			track_points_lock.lock(true);

			auto res = std::find_if(_track_points_info.begin(), _track_points_info.end(), [start_counter](const track_point_info & item){return start_counter >= item.counter; });

			if (res == _track_points_info.end())
			{
				track_points_lock.unlock(true);
				is_track_settings_set = true;
				reset_state();
				return false;
			}

			track_points_lock.unlock(true);

			counter_span.second = event.counter;

			clear();

			reset_state();
			return true;
		}

		void clear()
		{
			_synchro_packets_mtx.lock();
			_synchro_packets_container.clear();
			_synchro_packets_mtx.unlock();

			std::queue<sync_packet_ptr_t> _tmp_queue;
			_synchro_packets_queue_mtx.lock();
			_tmp_queue.swap(sync_packet_queue);
			_synchro_packets_queue_mtx.unlock();

			sync_helpers::rw_lock_guard_exclusive guard(track_points_lock);
			_track_points_info.clear();

			_event_packets_container.clear();

		}

	private:
#ifdef TIMESTAMP_SYNCH_PACKET_ON
		using track_points_container_t = std::map<time_t, track_point_info>;
#else
		using track_points_container_t = std::list<track_point_info>;
#endif
		mutable sync_helpers::rw_lock track_points_lock;

		track_points_container_t _track_points_info;
		mutable uint32_t _track_points_info_counter;

		track_point_info _currrent_track_settings;

		path_info_ptr_t _path_info;
		//path_info _path_info;
		synchronization::counter_t counter0;
		coordinate_t coordinate0;
		int32_t direction;

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

		using track_points_value_t = std::map<time_t, track_point_info>::value_type;
public:
#ifdef TIMESTAMP_SYNCH_PACKET_ON
	bool get_point_info_by_time(const time_t &time, track_point_info& info)
	{

		sync_helpers::rw_lock_guard_shared guard(track_points_lock);
		if (time < time_span.first)
			return false;


		const auto res = _track_points_info.lower_bound(time);
		if (res == _track_points_info.cend())
			return false;
		info = res->second;

		return true;
	}
#else
	bool get_last_point_info(track_point_info& info) const
	{
		if (!is_track_settings_set)
			return false;
		sync_helpers::rw_lock_guard_shared guard(track_points_lock);
		if (_track_points_info.empty())
			return false;
		info = _track_points_info.back();
		return true;
	}
#endif
private:
	time_t _last_found_time;
	};


	
	//class event_parser : public events::event_info
	//{
	//private:
	//	enum class RETRIEVE_POINT_INFO_FUNC_INDEX
	//	{
	//		CHANGE_PASSPORT,
	//		REVERSE
	//	};

	//	std::vector<retrive_point_info_func_t> _retrieve_point_info_funcs;

	//	struct packets_manager::Impl *_container;

	//	enum class State{
	//		ProcessSyncroPackets,
	//		RetriveStartPoint,
	//		RetriveStopPoint,
	//		ProcessReverseEvent,
	//		ProcessChangePassportEvent
	//	};

	//	inline bool set_state(State state)
	//	{
	//		auto prev_state = _InterlockedCompareExchange8((char*)(&_state), (char)state, (char)State::ProcessSyncroPackets);
	//		return prev_state == (char)State::ProcessSyncroPackets;
	//	}
	//	inline void reset_state(){ _InterlockedExchange8((char*)(&_state), (char)State::ProcessSyncroPackets); }

	//	volatile State _state;


	//public:
	//	event_parser(struct packets_manager::Impl *container):
	//		_state(State::ProcessSyncroPackets)
	//	{
	//		_container = container;
	//		_retrieve_point_info_funcs.emplace_back(std::bind(&packets_manager::Impl::retrieve_change_point_info, _container, std::placeholders::_1));
	//		_retrieve_point_info_funcs.emplace_back(std::bind(&packets_manager::Impl::retrieve_reverse_point_info, _container, std::placeholders::_1));

	//	}
	//protected:
	//	virtual bool get_info(const StartCommandEvent_packet& event)
	//	{
	//		if (!set_state(State::RetriveStartPoint))
	//			return;

	//		_container->is_track_settings_set = false;

	//		_container->_synchro_packets_tmp.clear();
	//		_container->_synchro_packets_mtx.lock();
	//		_container->_synchro_packets_tmp.swap(_container->_synchro_packets_container);
	//		_container->_synchro_packets_mtx.unlock();

	//		auto iter = _container->_synchro_packets_tmp.find(event.counter);
	//		if (iter == _container->_synchro_packets_tmp.end())
	//		{
	//			_container->_synchro_packets_tmp.clear();
	//			reset_state();
	//			return;
	//		}

	//		const auto & syncro_packet = iter->second;
	//		auto res = _container->retrieve_start_point_info(event, syncro_packet);
	//		if (!res)
	//		{
	//			_container->_synchro_packets_tmp.clear();
	//			reset_state();
	//			return;
	//		}

	//		uint32_t count = 0;
	//		for (; iter != _container->_synchro_packets_tmp.end(); ++iter, count++){
	//			_container->sync_packet_queue.push(iter->second);
	//		}
	//		sync_helpers::release_semaphore(_container->sync_packet_semaphore, count);
	//		_container->_synchro_packets_tmp.clear();

	//		_container->is_track_settings_set = true;

	//		_container->_synchro_packets_mtx.lock();
	//		for (iter = _container->_synchro_packets_container.begin(); iter != _container->_synchro_packets_container.end(); iter++){
	//			_container->_synchro_packets_queue_mtx.lock();
	//			_container->sync_packet_queue.push(iter->second);
	//			_container->_synchro_packets_queue_mtx.unlock();
	//		}
	//		_container->_synchro_packets_mtx.unlock();
	//		sync_helpers::release_semaphore(_container->sync_packet_semaphore, (uint32_t)_container->sync_packet_queue.size());

	//		reset_state();

	//	}
	//	virtual bool get_info(const CoordinateCorrected_packet&)
	//	{
	//	}
	//	void process_event_packet(const event_packet * event, State state, RETRIEVE_POINT_INFO_FUNC_INDEX index)
	//	{
	//		if (!set_state(state))
	//			return;

	//		_container->is_track_settings_set = false;

	//		auto & retrieve_point_info_func = _retrieve_point_info_funcs[(int)index];

	//		auto start_counter = event->counter;
	//		_container->track_points_lock.lock(true);

	//		auto res = std::find_if(_container->_track_points_info.begin(), _container->_track_points_info.end(), [start_counter](const track_point_info & item){return start_counter == item.counter; });

	//		if (res == _container->_track_points_info.end())
	//		{
	//			_container->track_points_lock.unlock(true);

	//			_container->_synchro_packets_tmp.clear();
	//			_container->_synchro_packets_mtx.lock();
	//			_container->_synchro_packets_tmp.swap(_container->_synchro_packets_container);
	//			_container->_synchro_packets_mtx.unlock();

	//			auto iter = _container->_synchro_packets_tmp.find(event->counter);
	//			if (iter == _container->_synchro_packets_tmp.end())
	//			{
	//				_container->_synchro_packets_tmp.clear();
	//				reset_state();
	//				return;
	//			}
	//			retrieve_point_info_func(event);

	//			uint32_t count = 0;
	//			for (; iter != _container->_synchro_packets_tmp.end(); ++iter, count++){
	//				_container->sync_packet_queue.push(iter->second);
	//			}
	//			sync_helpers::release_semaphore(_container->sync_packet_semaphore, count);
	//			_container->_synchro_packets_tmp.clear();
	//		}
	//		else
	//		{
	//			retrieve_point_info_func(event);
	//			for (; res != _container->_track_points_info.end(); res++)
	//			{
	//				auto coordinate = calculate_coordinate(_container->coordinate0, _container->direction*distance_from_counter(res->counter, _container->counter0, _container->counter_size));
	//				//res->_path_info = _container->_path_info;
	//				res->coordinate = coordinate;
	//			}
	//			_container->track_points_lock.unlock(true);
	//		}

	//		_container->is_track_settings_set = true;

	//		_container->_synchro_packets_mtx.lock();
	//		for (auto iter = _container->_synchro_packets_container.begin(); iter != _container->_synchro_packets_container.end(); iter++){
	//			_container->_synchro_packets_queue_mtx.lock();
	//			_container->sync_packet_queue.push(iter->second);
	//			_container->_synchro_packets_queue_mtx.unlock();
	//		}
	//		_container->_synchro_packets_mtx.unlock();
	//		sync_helpers::release_semaphore(_container->sync_packet_semaphore, (uint32_t)_container->sync_packet_queue.size());

	//		reset_state();

	//	}

	//	virtual bool get_info(const PassportChangedEvent_packet& event)
	//	{
	//		process_event_packet(&event, State::ProcessChangePassportEvent, RETRIEVE_POINT_INFO_FUNC_INDEX::CHANGE_PASSPORT);

	//	}
	//	virtual void get_info(const ReverseEvent_packet& event)
	//	{
	//		process_event_packet(&event, State::ProcessReverseEvent, RETRIEVE_POINT_INFO_FUNC_INDEX::REVERSE);
	//	}
	//	virtual void get_info(const StopCommandEvent_packet& event)
	//	{
	//		if (!set_state(State::RetriveStopPoint))
	//			return;

	//		_container->is_track_settings_set = false;

	//		_container->_synchro_packets_mtx.lock();

	//		const auto & iter = _container->_synchro_packets_container.find(event.counter);
	//		if (iter == _container->_synchro_packets_container.cend())
	//		{
	//			_container->_synchro_packets_mtx.unlock();
	//			return;
	//		}
	//		_container->counter_span.second = event.counter;
	//		_container->_synchro_packets_mtx.unlock();

	//		reset_state();
	//	}

	//};



packets_manager::packets_manager(uint8_t counter_size, unsigned int container_limit, CoordType coord_type) :
	_p_impl(std::make_unique<packets_manager::Impl>(counter_size, container_limit, coord_type))
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

	void packets_manager::set_counter_size(uint8_t counter_size)
	{
		_p_impl->counter_size = (uint32_t)counter_size;
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
