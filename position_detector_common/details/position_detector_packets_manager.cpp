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
#include <common\tuple_helpers.h>
#include <queue>
#include <thread>
#include <atomic>
#include <future>

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
#include "track_events_queue.h"

namespace position_detector
{
	namespace details
	{

		template<typename F, typename Tuple, bool Enough, int TotalArgs, int... N>
		struct call_impl
		{
			void static call(F f, Tuple&& t)
			{
				call_impl<F, Tuple, TotalArgs == 1 + sizeof...(N),
					TotalArgs, N..., sizeof...(N)
				>::call(f, std::forward<Tuple>(t));
			}
		};

		template<typename F, typename Tuple, int TotalArgs, int... N>
		struct call_impl<F, Tuple, true, TotalArgs, N...>
		{
			void static call(F f, Tuple&& t)
			{
				f(std::get<N>(std::forward<Tuple>(t))...);
			}
		};

		template<typename F, typename Tuple>
		void call(F f, Tuple&& t)
		{
			typedef typename std::decay<Tuple>::type type;
			call_impl<F, Tuple, 0 == std::tuple_size<type>::value,
				std::tuple_size<type>::value
			>::call(f, std::forward<Tuple>(t));
		}



		template<typename... TArgs>
		class notifier
		{
			using args_tuple_t = std::tuple<std::decay_t<TArgs>...>;

			using notify_callback_t = std::function<void(args_tuple_t&&)>;
			using queue_item_t = std::tuple<notify_callback_t, args_tuple_t>;

		public:

			notifier() :
				_processing_loop_thread([this]{ this->processing_loop(); })

			{}

			~notifier()
			{
				interrupt();

				if (_processing_loop_thread.joinable())
				{
					_processing_loop_thread.join();
				}
			}

			template<typename F>
			void operator()(const F& notify_callback, TArgs&& ...args)
			{
				{
					std::lock_guard<decltype(_queue_mtx)> lock(_queue_mtx);
					if (_interruption_point)
					{
						LOG_WARN() << L"Interrupt was requested. Notify can't process.";
						return;
					}
					_queue.emplace([=](args_tuple_t&& tuple){ call(notify_callback, std::move(tuple)); }, std::tuple<TArgs...>(std::forward<TArgs>(args)...));
				}
				_cv.notify_one();
			}

		private:
			std::queue<queue_item_t> _queue;
			std::condition_variable _cv;
			std::mutex _queue_mtx;
			bool _interruption_point;

			std::thread _processing_loop_thread;

			void interrupt()
			{
				{
					std::lock_guard<decltype(_queue_mtx)> lock(_queue_mtx);
					_interruption_point = true;
				}
				_cv.notify_one();
			}


			void processing_loop()
			{
				LOG_STACK();

				args_tuple_t ptr_result;
				notify_callback_t notify_callback;

				while (wait_and_get(notify_callback,ptr_result))
				{
					try
					{
						notify_callback(std::move(ptr_result));
					}
					catch (const std::exception &e)
					{
						LOG_DEBUG() << "Exception in notifier callback. Info: " << e.what();
					}
					catch (...)
					{
						LOG_DEBUG() << "Unknown exception in notifier callback.";
					}
				}
			}


			bool wait_and_get(notify_callback_t& notify_callback, args_tuple_t &data)
			{
				std::unique_lock<decltype(_queue_mtx)> lock(_queue_mtx);
				_cv.wait(lock, [&]
				{
					return _interruption_point || !_queue.empty();
				});

				if (_interruption_point)
				{
					if (!_queue.empty())
					{
						std::string dump_exceptions_message("Unprocessed requests");

						while (!_queue.empty())
						{
							//const auto cur_exception = _queue.front();
							_queue.pop();
						}

						LOG_WARN() << dump_exceptions_message.c_str();
					}

					return false;
				}

				auto item = std::move(_queue.front());
				_queue.pop();

				data = std::move(std::get<1>(item));
				notify_callback = std::move(std::get<0>(item));

				return true;
			}

		};
	}

	using namespace events;
	using namespace synchronization;
	using namespace packets_manager_ns;

	static const unsigned int default_counter_size = 10;//mm

	using time_span_t = std::pair<time_t, time_t>;
	using counter_span_t = std::pair<synchronization::counter_t, synchronization::counter_t>;

	using event_guid_t = std::string;

	using sync_packet_shared_ptr_t = std::shared_ptr<synchro_packet_t>;

	static coordinate_t default_item_length = static_cast<coordinate_t>(CoordType::METRO);
	struct packets_manager::Impl
	{
	private:

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
			_stop_requested(false),
			_state(State::ProcessSyncroPackets),
			device_layout(_device_lyaout),
			prev_counter(0),
			_track_points_info_counter(0),
			_coords_type(coord_type),
			device_offset(_device_offset),
			valid_counter0_span(100),
			device_offset_sign(1),
			orientation0(1),
			device_ahead0(true)
		{

			LOG_STACK();

			//if (valid_counter0_span > 0)
			//	valid_counter0_span /= counter_size;

			set_counter_size((uint32_t)_counter_size);

			sync_packet_semaphore = sync_helpers::create_basic_semaphore_object(0);

			synchronizer_packets_ctrl.proccess_start_event_paket_func = [this](const StartCommandEvent_packet& packet){return this->get_info(packet); };
			synchronizer_packets_ctrl.proccess_coordinate_event_paket_func = [this](const CoordinateCorrected_packet& packet){return this->get_info(packet); };
			synchronizer_packets_ctrl.proccess_passport_event_paket_func = [this](const PassportChangedEvent_packet& packet){return this->get_info(packet); };
			synchronizer_packets_ctrl.proccess_reverse_event_paket_func = [this](const ReverseEvent_packet& packet){return this->get_info(packet); };
			synchronizer_packets_ctrl.proccess_stop_event_paket_func = [this](const StopCommandEvent_packet& packet){return this->get_info(packet); };

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

		packets_manager_controller synchronizer_packets_ctrl;

		std::mutex synchronizer_calculation_mtx;
		manager_track_traits synchronizer_track_traits;


		std::mutex device_calculation_mtx;
		manager_track_traits device_track_traits;

		track_events_queue_t device_events_queue;


		using get_point_info_func_t = std::function<void(const event_packet*, manager_track_traits&)>;

	public:

		passport_changed_process_func_t passportChanged;
		coordinate_corrected_process_func_t coordinateCorrected;

		track_state_change_notify_func_t track_state_change_notify;

	private:

		details::notifier<const position_detector::counter32_t&, const position_detector::counter32_t&, coordinate_calculator_ptr_t&&> _notifier;

		using coordinate_calculator_shared_ptr_t = std::shared_ptr<icoordinate_calculator>;

		void passportChangedNotify(const position_detector::counter32_t& begin, const position_detector::counter32_t& end, coordinate_calculator_ptr_t&& calculator_ptr)
		{
			LOG_STACK();

			coordinatesChangedNotify(passportChanged, begin, end, std::move(calculator_ptr));
		}

		void coordinateCorrectedNotify(const position_detector::counter32_t& begin, const position_detector::counter32_t& end, coordinate_calculator_ptr_t&& calculator_ptr)
		{
			LOG_STACK();

			coordinatesChangedNotify(coordinateCorrected, begin, end, std::move(calculator_ptr));
		}

		void coordinatesChangedNotify(const change_coordinate_notify_t& functor, const position_detector::counter32_t& begin, const position_detector::counter32_t& end, coordinate_calculator_ptr_t&& calculator_ptr)
		{
			LOG_STACK();

			if (!functor)
				return;

			_notifier(functor, begin, end, std::move(calculator_ptr));

		}

	public:

		std::map<event_guid_t, event_packet_ptr_t> _event_packets_container;
		std::map<synchronization::counter_t, sync_packet_shared_ptr_t> _synchro_packets_container;
		std::mutex _event_packets_mtx;
		std::mutex _synchro_packets_mtx;

		using events_queue = std::map<synchronization::counter_t, event_packet_ptr_t>;

		events_queue deferred_event_packet_queue;

		unsigned int container_limit;
		uint32_t counter_size;
		coordinate_t device_offset;
		DEVICE_LAYOUT device_layout;
		synchronization::counter_t device_offset_in_counter;
		bool device_ahead;

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
			}
			device_offset = _device_offset;

			device_ahead = true;
			if (device_offset > 0 && device_track_traits.direction0 < 0)
					device_ahead = false;
			else if (device_offset < 0 && device_track_traits.direction0 > 0)
					device_ahead = false;

			if (lock){
				device_calculation_mtx.unlock();
			}
		}

		public:

			void set_device_offset(coordinate_t _device_offset)
			{
				_set_device_offset(_device_offset,true);
			}

			void set_counter_size(uint32_t _counter_size)
			{
				std::lock_guard<decltype(device_calculation_mtx)> lock_device(device_calculation_mtx);
				counter_size = _counter_size;
				device_track_traits.counter_size = _counter_size;
				synchronizer_track_traits.counter_size = _counter_size;

				counter_valid_span = 100 * 100 * 10;
				if (counter_size > 0)
					counter_valid_span /= counter_size;

				_set_device_offset(device_offset,false);


			}

			void dispatch_synchro_packet(const synchro_packet_t &packet)
			{
				LOG_STACK();

				if (counter_span.first > packet.counter){
					return;
				}

				track_point_info data;
				{
					coordinate_t synchronizer_coordinate;
					packets_manager_ns::direction_t synchronizer_direction;
					{
						std::lock_guard<decltype(synchronizer_calculation_mtx)> lock(synchronizer_calculation_mtx);
						synchronizer_direction = synchronizer_track_traits.direction;
						synchronizer_coordinate = calculate_coordinate(synchronizer_track_traits.coordinate0, synchronizer_track_traits.direction*distance_from_counter(packet.counter, synchronizer_track_traits.counter0, synchronizer_track_traits.counter_size));
					}


					std::lock_guard<decltype(device_calculation_mtx)> lock_device(device_calculation_mtx);
					manager_track_traits &track_traits = device_track_traits;

					auto coordinate = calculate_coordinate(track_traits.coordinate0, track_traits.direction*distance_from_counter(packet.counter, track_traits.counter0, track_traits.counter_size));

					if (device_events_queue.process_track(coordinate, packet.counter, track_traits, synchronizer_coordinate, synchronizer_direction))
						coordinate = calculate_coordinate(track_traits.coordinate0, track_traits.direction*distance_from_counter(packet.counter, track_traits.counter0, track_traits.counter_size));

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
					return false;
				}

				return true;
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

			_event_packet_holder = packet;

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


			{
				manager_track_traits track_traits;
				{
					retrieve_start_point_info(event, track_traits);
					track_traits.counter_size = counter_size;

					orientation0 = 1;
					if (event.track_settings.orientation != "Salon"){
						orientation0 = -1;
					}
					device_offset_sign = orientation0 * track_traits.direction;
					device_ahead = true;
					if (device_offset > 0 && orientation0 == -1)
						device_ahead = false;
					else
						if (device_offset < 0 && orientation0 == 1)
							device_ahead = false;


					device_ahead0 = device_ahead;

					synchronizer_track_traits = track_traits;
				}

				track_traits.coordinate0 += device_offset_sign*device_offset;
				{
					std::lock_guard<decltype(device_calculation_mtx)>  guard(device_calculation_mtx);
					device_track_traits = track_traits;
					device_events_queue.set_begin_path_info(std::move(track_traits), true, device_ahead0);
					device_events_queue.set_cutter_offset(device_offset_in_counter);
				}

			}

			LOG_TRACE() << L" Starting new transit.";
			{
				std::lock_guard<decltype(_track_points_info)>  guard(_track_points_info);
				_track_points_info.clear();
			}

			_event_packets_container.clear();

			is_track_settings_set = true;

			_start_track_event_packet = std::move(_event_packet_holder);

			if (track_state_change_notify)
				track_state_change_notify(true);

			return true;

		}

		bool get_info(const CoordinateCorrected_packet& event)
		{
			LOG_STACK();

			LOG_TRACE() << event;

			if (is_track_settings_set)
			{
				if (event.correct_direction.counter < synchronizer_track_traits.counter0)
					return true;

				if (!set_state(State::ProcessCorrectedCoordinateEvent))
					return false;

				utils::on_exit exit_guard([this](){ reset_state(); });

				is_track_settings_set = false;
				{
					coordinate_t coordinate{};
					if (device_offset != 0){
						std::lock_guard<decltype(synchronizer_calculation_mtx)>  guard(synchronizer_calculation_mtx);
						coordinate = calculate_coordinate(synchronizer_track_traits.coordinate0, synchronizer_track_traits.direction*distance_from_counter(event.counter, synchronizer_track_traits.counter0, synchronizer_track_traits.counter_size));
					}

					packets_manager_ns::direction_t synchronizer_direction;
					manager_track_traits prev_track_traits;
					manager_track_traits track_traits;
					{
						std::lock_guard<decltype(synchronizer_calculation_mtx)>  guard(synchronizer_calculation_mtx);
						synchronizer_direction = synchronizer_track_traits.direction;
						prev_track_traits = synchronizer_track_traits;
						retrieve_corrected_point_info(event, synchronizer_track_traits);
						track_traits = synchronizer_track_traits;
					}

					//bool direction_changed = track_traits.direction != synchronizer_direction;
					//device_offset_sign = orientation0 * track_traits.direction;

					{
						if (device_offset != 0)
						{
							manager_track_traits new_track_traits;
							std::lock_guard<decltype(device_calculation_mtx)>  guard(device_calculation_mtx);

							if (device_ahead)
							{
								const auto coordinateCorrectedNotifyFunc = [&](const position_detector::counter32_t& counter_start, const position_detector::counter32_t& counter_end, const manager_track_traits & track_traits)
								{
									coordinateCorrectedNotify(counter_start, counter_end, std::make_unique<coordinate_calculator>(track_traits));
								};

								new_track_traits = device_events_queue.process_coordinate_correct_event(coordinate, synchronizer_direction, std::move(track_traits), coordinateCorrectedNotifyFunc);

							}//if (device_ahead)
							else
							{
								new_track_traits = device_events_queue.defer_coordinate_correct_point_info(coordinate, std::move(track_traits));
							}

							if (new_track_traits.valid())
							{
								device_track_traits = std::move(new_track_traits);
							}


						}//if (device_offset != 0)
						else
						{
							device_track_traits = std::move(track_traits);
						}
					}
				}

				is_track_settings_set = true;

				_coordinate_correct_event_packet = std::move(_event_packet_holder);

			}
			return true;
		}

		bool process_event_packet(State state, const std::function<void()> &functor)
		{
			LOG_STACK();

			if (!set_state(state))
				return false;

			is_track_settings_set = false;

			utils::on_exit exit_guard([this](){ reset_state(); });

			functor();

			is_track_settings_set = true;
			
			return true;
		}
		void rebuild_track_point_info_container(const event_packet * event, const get_point_info_func_t& retrieve_point_info_func, manager_track_traits& track_traits)
		{
			LOG_STACK();
			auto start_counter = event->counter;
			bool has_info = false;
			std::unique_lock<decltype(_track_points_info)> track_points_info_guard(_track_points_info);

			if (!_track_points_info.empty()){
				const auto& last_point = _track_points_info.back();
				has_info = last_point.counter >= start_counter;
			}

			if (has_info)
			{
				retrieve_point_info_func(event, track_traits);

				auto & res = _track_points_info.back();
				auto coordinate = calculate_coordinate(track_traits.coordinate0, track_traits.direction*distance_from_counter(res.counter, track_traits.counter0, track_traits.counter_size));
				res._path_info = track_traits._path_info;
				res.coordinate = coordinate;

				auto * actual_nonstandart_kms = &track_traits.positive_nonstandard_kms;
				if (coordinate < 0)
				{
					actual_nonstandart_kms = &track_traits.negative_nonstandard_kms;
				}
				calculate_picket_offset(coordinate, *actual_nonstandart_kms, res.picket, res.offset);

			}
			else
			{
				track_points_info_guard.unlock();

				std::lock_guard<decltype(_synchro_packets_mtx)> synchro_packets_guard(_synchro_packets_mtx);

				retrieve_point_info_func(event, track_traits);

				auto iter = _synchro_packets_container.lower_bound(event->counter);
				if (iter == _synchro_packets_container.end())
				{
					LOG_TRACE() << L"Container synchro packets does not contain packets which counter greater or equal " << event->counter;
					LOG_TRACE() << L"Container synchro packets size: " << _synchro_packets_container.size();
					_synchro_packets_container.clear();
					return;
				}

				uint32_t count = 0;
				for (; iter != _synchro_packets_container.end(); ++iter, ++count){
					if (track_traits.counter_span.first <= iter->first){
						std::lock_guard<decltype(_synchro_packets_queue_mtx)> lock(_synchro_packets_queue_mtx);
						sync_packet_queue.push(iter->second);
					}
				}
				if (count > 0)
					sync_helpers::release_semaphore(sync_packet_semaphore, count);
				_synchro_packets_container.clear();

			}
		}

		bool get_info(const PassportChangedEvent_packet& event)
		{
			LOG_STACK();
			LOG_TRACE() << event;

			return process_event_packet(State::ProcessChangePassportEvent, [&]
			{
				if (synchronizer_track_traits.counter_span.first > event.counter)
					return;

				//if (synchronizer_track_traits.counter_span.first > valid_counter0_span && event.counter < synchronizer_track_traits.counter_span.first - valid_counter0_span)
				//	return;

				coordinate_t coordinate{};
				if (device_offset != 0){
					std::lock_guard<decltype(synchronizer_calculation_mtx)>  guard(synchronizer_calculation_mtx);
					coordinate = calculate_coordinate(synchronizer_track_traits.coordinate0, synchronizer_track_traits.direction*distance_from_counter(event.counter, synchronizer_track_traits.counter0, synchronizer_track_traits.counter_size));
				}

				packets_manager_ns::direction_t synchronizer_direction;
				manager_track_traits prev_track_traits;
				manager_track_traits track_traits;
				{
					std::lock_guard<decltype(synchronizer_calculation_mtx)>  guard(synchronizer_calculation_mtx);
					synchronizer_direction = synchronizer_track_traits.direction;
					prev_track_traits = synchronizer_track_traits;
					retrieve_change_point_info(event, synchronizer_track_traits);
					track_traits = synchronizer_track_traits;
				}

				if (device_offset != 0)
				{
					manager_track_traits new_track_traits;
					std::lock_guard<decltype(device_calculation_mtx)>  guard(device_calculation_mtx);

					if (device_ahead)
					{
						const auto passportChangedNotifyFunc = [&](const position_detector::counter32_t& counter_start, const position_detector::counter32_t& counter_end, const manager_track_traits & track_traits)
						{
							passportChangedNotify(counter_start, counter_end, std::make_unique<coordinate_calculator>(track_traits));
						};

						new_track_traits = device_events_queue.process_change_path_event(coordinate, synchronizer_direction, track_traits, passportChangedNotifyFunc);

					}//if (device_ahead)
					else
					{
						device_events_queue.defer_new_path_point_info(coordinate, std::move(track_traits));
					}

					if (new_track_traits.valid())
					{
						device_track_traits = std::move(new_track_traits);
					}

				}//if (device_offset != 0)
				else
				{
					device_track_traits = std::move(track_traits);
				}

				_passport_change_event_packet = std::move(_event_packet_holder);

			});
		}
		bool get_info(const ReverseEvent_packet& event)
		{
			LOG_STACK();
			LOG_TRACE() << event;

			return process_event_packet(State::ProcessReverseEvent, [&]
			{
				if (synchronizer_track_traits.counter_span.first > valid_counter0_span && event.counter < synchronizer_track_traits.counter_span.first - valid_counter0_span)
					return;

				{
					std::lock_guard<decltype(synchronizer_calculation_mtx)>  guard(synchronizer_calculation_mtx);
					retrieve_reverse_point_info(event, synchronizer_track_traits);
				}

				device_ahead = event.is_reverse ? !device_ahead0 : device_ahead0;

				const auto retrieve_point_info_func = [&](const event_packet* /*event_ptr*/, manager_track_traits& /*track_traits*/)
				{
					std::lock_guard<decltype(device_calculation_mtx)>  guard(device_calculation_mtx);
					const auto & coordinate = calculate_coordinate(device_track_traits.coordinate0, device_track_traits.direction*distance_from_counter(event.counter, device_track_traits.counter0, device_track_traits.counter_size));
					retrieve_reverse_point_info(event, device_track_traits);
					device_events_queue.reverse(coordinate, manager_track_traits{ device_track_traits });
				};

				rebuild_track_point_info_container(&event, retrieve_point_info_func, device_track_traits);

				_reverse_event_packet = std::move(_event_packet_holder);
				_coordinate_correct_event_packet.reset();
			});
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

			if (track_state_change_notify)
				track_state_change_notify(false);

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

			synchronizer_track_traits.counter0 = 0;
			device_calculation_mtx.lock();
			prev_counter = 0;
			device_track_traits.counter0 = 0;
			device_calculation_mtx.unlock();

			_start_track_event_packet.reset();
			_passport_change_event_packet.reset();
			_reverse_event_packet.reset();
			_coordinate_correct_event_packet.reset();
			_event_packet_holder.reset();

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

		synchronization::counter_t valid_counter0_span;

		synchronization::counter_t prev_counter;

	public:
		synchronization::counter_t counter_valid_span;
	private:

		int32_t orientation0; // 1 - Salon , -1 - Boiler
		int32_t device_offset_sign;
		bool device_ahead0;

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

	private:
		event_packet_ptr_t _start_track_event_packet;
		event_packet_ptr_t _passport_change_event_packet;
		event_packet_ptr_t _reverse_event_packet;
		event_packet_ptr_t _coordinate_correct_event_packet;
		event_packet_ptr_t _event_packet_holder;

	public:
		event_packet_ptr_t get_start_event_packet() const { return _start_track_event_packet; }
		std::vector<events::event_packet_ptr_t> get_actual_pd_event_packets() const
		{
			if (!is_track_settings_set)
				return{};

			std::vector<events::event_packet_ptr_t> result = { _start_track_event_packet };

			if (_passport_change_event_packet)
				result.emplace_back(_passport_change_event_packet);

			if (_reverse_event_packet)
				result.emplace_back(_reverse_event_packet);

			if (_coordinate_correct_event_packet)
				result.emplace_back(_coordinate_correct_event_packet);

			return result;
		}
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

	};


	packets_manager::packets_manager(uint8_t counter_size, coordinate_t device_offset, DEVICE_LAYOUT device_lyaout, unsigned int container_limit, CoordType coord_type) :
		_p_impl(std::make_unique<packets_manager::Impl>(counter_size, device_offset, device_lyaout, container_limit, coord_type))
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
		_p_impl->passportChanged = {};
		//_p_impl->passportChanged -= func;
	}
	void packets_manager::remove_coordinate_corrected_process_func(const coordinate_corrected_process_func_t&)
	{
		_p_impl->coordinateCorrected = {};
		//_p_impl->coordinateCorrected -= func;
	}

	void packets_manager::set_track_state_change_notify(const track_state_change_notify_func_t& func)
	{
		_p_impl->track_state_change_notify = func;
	}

	event_packet_ptr_t packets_manager::get_start_event_packet() const
	{
		return _p_impl->get_start_event_packet();
	}

	std::vector<events::event_packet_ptr_t> packets_manager::get_actual_pd_event_packets() const
	{
		return _p_impl->get_actual_pd_event_packets();
	}
}//namespace position_detector