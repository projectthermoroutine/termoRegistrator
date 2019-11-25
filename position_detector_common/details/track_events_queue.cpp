#define NOMINMAX

#include <assert.h>

#include "track_events_queue.h"

#define _SIMPLE_ALG

namespace position_detector::packets_manager_ns
{
	using namespace std::string_view_literals;

	namespace
	{
		constexpr bool new_transit_flag{ true };
		constexpr bool not_new_transit_flag{ false };

		constexpr bool device_ahead{ true };
		constexpr bool device_behind{ true };

		constexpr std::size_t new_coordinate_idx{ 0 };
		constexpr std::size_t from_coordinate_idx{ 1 };

	}


#define INDEX_CAST(_queue_direction_) static_cast<std::size_t>(_queue_direction_)
#define COUNTER_SPAN(_coordinate1, _coordinate0, _counter_size) static_cast<position_detector::counter32_t>(std::abs(_coordinate1 - _coordinate0) / _counter_size)

	inline std::size_t index_from_direction(const direction_t & direction)
	{
		return direction == 1 ? 1 : 0;
	}

	inline void reverse_queue_direction(track_events_queue_t::queue_direction_t & direction)
	{
		direction = direction == track_events_queue_t::queue_direction_t::device_ahead ? track_events_queue_t::queue_direction_t::device_behind : track_events_queue_t::queue_direction_t::device_ahead;
	}

	inline std::int8_t sign_direction_from_queue_direction(track_events_queue_t::queue_direction_t direction)
	{
		return direction == track_events_queue_t::queue_direction_t::device_behind ? -1 : 1;
	}


	std::wstring to_wstring(track_events_queue_t::queue_direction_t queue_direction)
	{
		return queue_direction == track_events_queue_t::queue_direction_t::device_ahead ? L"device_ahead" : L"device_behind";
	}



	void track_events_queue_t::set_begin_path_info(manager_track_traits && track_traits, bool new_transit, bool cutter_behind)
	{
		LOG_STACK();

		if (!device_events_queue_.empty())
			return;

		_current_track_traits = track_traits;

		device_events_queue_.push_back({ track_traits.coordinate0, std::move(track_traits), { { invalid_coordinate, invalid_coordinate } }, {}, std::list<path_events_t::path_event_t>{} });

		auto & first_path_info = device_events_queue_.front();

		p_current_path_events_ = &first_path_info.events;
		p_current_path_ = &first_path_info;
		current_path_iter_ = device_events_queue_.begin();
		next_event_[INDEX_CAST(queue_direction_t::device_behind)] = next_event_[INDEX_CAST(queue_direction_t::device_ahead)] = p_current_path_events_->end();
		_cutter_direction = first_path_info.start_path_track_traits.direction;

		_queue_direction = cutter_behind ? queue_direction_t::device_ahead : queue_direction_t::device_behind;

		if (new_transit)
			_start_track_traits = first_path_info.start_path_track_traits;

	}

	bool
		track_events_queue_t::process_track(const coordinate_t & coordinate,
			const synchronization::counter_t & counter,
			manager_track_traits & track_traits,
			const coordinate_t & cutter_coordinate,
			packets_manager_ns::direction_t cutter_direction
		)
	{
		LOG_STACK();

		LOG_DEBUG() << L"Current queue direction: "sv << to_wstring(_queue_direction);

		_processed_counter = static_cast<decltype(_processed_counter)>(counter);

		if (device_events_queue_.empty() && p_current_path_events_ == nullptr)
			return false;

		bool result{ false };
		const auto direction = track_traits.direction;
		auto events_limit_iter = p_current_path_events_->end();

		if (_queue_direction == queue_direction_t::device_behind &&
			next_event_[INDEX_CAST(_queue_direction)] != events_limit_iter
		)
		{//in current movment direction there are coordinate correction events 
			decltype(p_current_path_events_->end()) event_iter{ next_event_[INDEX_CAST(_queue_direction)] };

			const auto & event_coordinate_limit = event_iter->coordinate[from_coordinate_idx];

			if (event_coordinate_limit != invalid_coordinate && event_coordinate_limit*direction <= coordinate * direction)
			{// change next event

				LOG_DEBUG() << L"Apply correct coordinate event. Coordinate: "sv << coordinate << L" mm, counter: "sv << counter;

				bool erase_event = true;
				if (_queue_direction == queue_direction_t::device_ahead)
				{
					next_event_[INDEX_CAST(queue_direction_t::device_behind)] = next_event_[INDEX_CAST(queue_direction_t::device_ahead)];
					if (next_event_[INDEX_CAST(queue_direction_t::device_ahead)] == p_current_path_events_->begin())
						next_event_[INDEX_CAST(queue_direction_t::device_ahead)] = p_current_path_events_->end();
					else
						--next_event_[INDEX_CAST(queue_direction_t::device_ahead)];
				}
				else
				{
#ifdef _SIMPLE_ALG
					//remove event
					++next_event_[INDEX_CAST(queue_direction_t::device_behind)];
					/*next_event_[INDEX_CAST(queue_direction_t::device_ahead)] = p_current_path_events_->end();*/
#else
					next_event_[INDEX_CAST(queue_direction_t::device_ahead)] = next_event_[INDEX_CAST(queue_direction_t::device_behind)];
					++next_event_[INDEX_CAST(queue_direction_t::device_behind)];
#endif
				}

				track_traits = event_iter->new_track_traits;

				if(erase_event)
					p_current_path_events_->erase(event_iter);
				else
					event_iter->event_counters[INDEX_CAST(_queue_direction)].counters.push_back(counter);

				track_traits._path_info->counter0 = track_traits.counter0 = counter - static_cast<decltype(counter)>(std::abs(coordinate - event_coordinate_limit) / track_traits.counter_size);

				_current_track_traits = track_traits;

				result = true;
			}
		}

		if (!result)
		{
			const auto & path_coordinate_limit = p_current_path_->path_interval[INDEX_CAST(_queue_direction)];

			if (path_coordinate_limit != invalid_coordinate && path_coordinate_limit*direction <= coordinate * direction)
			{// change current path

				const auto sign_direction = sign_direction_from_queue_direction(_queue_direction);
				std::advance(current_path_iter_, sign_direction * 1);

				auto & current_path_info = *current_path_iter_;

				p_current_path_events_ = &current_path_info.events;
				p_current_path_ = &current_path_info;

				next_event_[INDEX_CAST(queue_direction_t::device_behind)] = next_event_[INDEX_CAST(queue_direction_t::device_ahead)] = p_current_path_events_->end();

				if (_queue_direction == queue_direction_t::device_ahead)
				{
					if (!p_current_path_events_->empty())
						--next_event_[INDEX_CAST(queue_direction_t::device_ahead)];
				}
				else
				{
					next_event_[INDEX_CAST(queue_direction_t::device_behind)] = p_current_path_events_->begin();
				}

				p_current_path_->event_counters[INDEX_CAST(_queue_direction)].counters.push_back(counter);

				track_traits = p_current_path_->start_path_track_traits;
				track_traits._path_info->counter0 = track_traits.counter0 = counter - static_cast<decltype(counter)>(std::abs(coordinate - path_coordinate_limit) / track_traits.counter_size);

				if (p_current_path_->path_interval[INDEX_CAST(queue_direction_t::device_behind)] != invalid_coordinate)
				{
					const decltype(track_traits.direction) k = static_cast<decltype(track_traits.direction)>(sign_direction);
					const decltype(track_traits.direction) new_direction = k * (p_current_path_->path_interval[1] - p_current_path_->path_interval[0]) > 0 ? 1 : -1;
					track_traits.direction = new_direction;
				}

				_current_track_traits = track_traits;

				result = true;
			}
		}

		if (_queue_direction == queue_direction_t::device_ahead)
		{
			const auto current_coordinate_mod = cutter_coordinate * cutter_direction;

			auto & cutter_path_info = device_events_queue_.front();
			// cut events
			while (!cutter_path_info.events.empty())
			{
				auto last_event_iter = --cutter_path_info.events.end();
				if (last_event_iter->new_track_traits.coordinate0*cutter_direction < current_coordinate_mod)
				{// cut event

					LOG_DEBUG() << L"Cut event. Event coordinate: "sv
						<< last_event_iter->new_track_traits.coordinate0
						<< L" cutter coordinate: "sv << cutter_coordinate
						<< L", cutter direction: "sv << cutter_direction
						<< L", _cutter direction: "sv << _cutter_direction;

					if (&cutter_path_info == p_current_path_)
					{
						if (next_event_[INDEX_CAST(queue_direction_t::device_behind)] == last_event_iter)
							next_event_[INDEX_CAST(queue_direction_t::device_behind)] = cutter_path_info.events.end();
					}

					cutter_path_info.events.erase(last_event_iter);

					LOG_DEBUG() << L"Cutter path events count: "sv << cutter_path_info.events.size();

					continue;
				}

				break;
			}

			// cut reverse events
			while (!cutter_path_info.reverse_events.empty())
			{
				auto first_reverse_event_iter = cutter_direction == 1 ? cutter_path_info.reverse_events.begin() : --cutter_path_info.reverse_events.end();
				if ((*first_reverse_event_iter)->new_track_traits.coordinate0*cutter_direction < current_coordinate_mod)
				{// cut reverse event

					LOG_DEBUG() << L"Cut reverse event. Reverse event coordinate: "sv
						<< (*first_reverse_event_iter)->new_track_traits.coordinate0
						<< L" cutter coordinate: "sv << cutter_coordinate
						<< L", cutter direction: "sv << cutter_direction
						<< L", _cutter direction: "sv << _cutter_direction;
					_reverse_events_list.erase(*first_reverse_event_iter);
					cutter_path_info.reverse_events.erase(first_reverse_event_iter);

					LOG_DEBUG() << L"Cutter path reverse events count: "sv << cutter_path_info.reverse_events.size();

					continue;
				}

				LOG_DEBUG() << L"Reverse events count: "sv << _reverse_events_list.size() << L". Cutter path reverse events count: "sv << cutter_path_info.reverse_events.size();

				break;
			}
		}//if (_queue_direction == queue_direction_t::device_ahead)

		return result;
	}

	void track_events_queue_t::reverse(const coordinate_t & coordinate, manager_track_traits && track_traits)
	{
		LOG_STACK();

		if (device_events_queue_.empty())
			return;

		LOG_DEBUG() << L"Current queue direction: "sv << to_wstring(_queue_direction);

		reverse_queue_direction(_queue_direction);

		_cutter_direction *= -1;

		_current_track_traits = track_traits;

		_reverse_events_list.push_back({ _queue_direction, std::move(track_traits) });
		//if (_queue_direction == queue_direction_t::device_ahead)
		{//recalc next reverse event for cutting

			auto last_reverse_iter = _reverse_events_list.end();
			--last_reverse_iter;
			for (auto & path_info : device_events_queue_)
			{
				if (last_reverse_iter->new_track_traits.compare_path_info_weak(path_info.start_path_track_traits))
				{// reverse event occured in this path
					auto reverse_event_iter = path_info.reverse_events.begin();

					for (; reverse_event_iter != path_info.reverse_events.end(); ++reverse_event_iter)
					{
						if ((*reverse_event_iter)->new_track_traits.coordinate0 > last_reverse_iter->new_track_traits.coordinate0)
						{//все реверсы для одного пути формируются в упорядоченный список по возрастанию координаты реверса
							break;
						}
					}

					path_info.reverse_events.insert(reverse_event_iter, last_reverse_iter);

					break;
				}
			}
		}

#ifdef _SIMPLE_ALG
		if (_queue_direction == queue_direction_t::device_ahead)
		{//remove all ahead change path events
			auto path_iter = current_path_iter_;
			++path_iter;
			for (; path_iter != device_events_queue_.end();)
			{
				for (const auto & iter : path_iter->reverse_events) 
				{
					LOG_DEBUG() << L"Remove reverse event. Reverse event coordinate: "sv
						<< iter->new_track_traits.coordinate0
						<< L" [line: "sv << iter->new_track_traits._path_info->line
						<< L", path: "sv << iter->new_track_traits._path_info->path	<< L']';

					_reverse_events_list.erase(iter);
				}

				LOG_DEBUG() << L"Remove path info [line: "sv << path_iter->start_path_track_traits._path_info->line
					<< L", path: "sv << path_iter->start_path_track_traits._path_info->path << L']';

				path_iter = device_events_queue_.erase(path_iter);
			}

			p_current_path_->path_interval[INDEX_CAST(_queue_direction)] = invalid_coordinate;
		}
		else
		{//_queue_direction == queue_direction_t::device_behind
			if (!p_current_path_events_->empty())
			{
				for (auto event_iter = p_current_path_events_->begin(); event_iter != p_current_path_events_->end(); ++event_iter)
				{
					if (coordinate*track_traits.direction < event_iter->coordinate[from_coordinate_idx] * track_traits.direction)
					{
						next_event_[INDEX_CAST(queue_direction_t::device_behind)] = event_iter;
					}
				}
			}
		}

#endif // _SIMPLE_ALGD

		LOG_DEBUG() << L"Reverse events count: "sv << _reverse_events_list.size();
		LOG_DEBUG() << L"Queue direction after reverse: "sv << to_wstring(_queue_direction);

	}

	manager_track_traits
		track_events_queue_t::defer_coordinate_correct_point_info(
			const coordinate_t & coordinate,
			manager_track_traits && track_traits,
			const change_coordinate_info_t& change_coordinate_info
		)
	{
		LOG_STACK();

		if (_processed_counter != invalid_counter32 && track_traits.counter0 < _processed_counter)
		{
			if (_processed_counter - track_traits.counter0 > static_cast<decltype(_processed_counter)>(_cutter_offset_in_counter))
			{
				const auto current_coordinate = calculate_coordinate(_current_track_traits.coordinate0, _current_track_traits.direction*distance_from_counter(_processed_counter, _current_track_traits.counter0, track_traits.counter_size));

				if (current_coordinate*_current_track_traits.direction > coordinate*_current_track_traits.direction)
				{// точку коррекции координаты устройство уже преодолело, нужно скорректировать координаты для устройства

					return apply_coordinate_correct_event(
						coordinate,
						_cutter_direction,
						std::move(track_traits),
						change_coordinate_info
					);
				}
			}
		}

		auto & cutter_path_info = device_events_queue_.front();

		if (_cutter_direction != track_traits.direction)
		{
			LOG_DEBUG() << L"Coordinate correct event with change movement direction."sv;

			_cutter_direction = track_traits.direction;

			//if (!device_events_queue_.front().events.empty())
			{// событие коррекции координаты со сменой направления не первое для данного пути

				cutter_path_info.start_path_track_traits.direction = _cutter_direction;
				if (p_current_path_ == &cutter_path_info)
				{
					device_events_queue_.clear();
					_reverse_events_list.clear();

					manager_track_traits new_track_traits{ std::move(track_traits) };

					new_track_traits.coordinate0 -= new_track_traits.counter_size*_cutter_offset_in_counter*_cutter_direction;

					set_begin_path_info(std::move(new_track_traits), not_new_transit_flag, !device_ahead);
					return device_events_queue_.front().start_path_track_traits;
				}

				return{};
			}
		}

		if (!cutter_path_info.events.empty())
		{
			const auto coordinate_diff_abs = std::abs(track_traits.coordinate0 - coordinate);

			if (_coordinate_valid_span < coordinate_diff_abs)
			{
				cutter_path_info.events.clear();
				next_event_[INDEX_CAST(queue_direction_t::device_ahead)] = next_event_[INDEX_CAST(queue_direction_t::device_behind)] = cutter_path_info.events.end();
			}
		}

		cutter_path_info.events.push_back(path_events_t::path_event_t{ { { track_traits.coordinate0, coordinate } },
																		std::move(track_traits),
																		{}
			}
		);

		LOG_DEBUG() << L"Deffer coordinate correct event. Events count: "sv << cutter_path_info.events.size();

		if (&cutter_path_info == p_current_path_ && next_event_[INDEX_CAST(queue_direction_t::device_behind)] == cutter_path_info.events.end())
		{
			next_event_[INDEX_CAST(queue_direction_t::device_behind)] = (--cutter_path_info.events.end());
		}

		return{};
	}

	manager_track_traits
		track_events_queue_t::defer_new_path_point_info(
			const coordinate_t & coordinate,
			manager_track_traits && track_traits,
			const change_coordinate_info_t& change_coordinate_info
		)
	{
		LOG_STACK();

		auto last_path_iter = device_events_queue_.begin();
		last_path_iter->path_interval[INDEX_CAST(queue_direction_t::device_behind)] = coordinate;


		if (_processed_counter != invalid_counter32 && track_traits.counter0 < _processed_counter)
		{
			if (_processed_counter - track_traits.counter0 > static_cast<decltype(_processed_counter)>(_cutter_offset_in_counter))
			{
				const auto current_coordinate = calculate_coordinate(_current_track_traits.coordinate0, _current_track_traits.direction*distance_from_counter(_processed_counter, _current_track_traits.counter0, track_traits.counter_size));

				if (current_coordinate*_current_track_traits.direction > coordinate*_current_track_traits.direction)
				{// точку коррекции координаты устройство уже преодолело, нужно скорректировать координаты для устройства

					return apply_change_path_event(
						coordinate,
						_cutter_direction,
						std::move(track_traits),
						change_coordinate_info
					);
				}
			}
		}

		_cutter_direction = track_traits.direction;
		const auto coordinate0 = track_traits.coordinate0;
		device_events_queue_.push_front({ coordinate0, std::move(track_traits), { { invalid_coordinate, coordinate0 } }, {}, std::list<path_events_t::path_event_t>{} });

		return{};
	}


	void track_events_queue_t::reset()
	{
		LOG_STACK();

		device_events_queue_.clear();
		_reverse_events_list.clear();
	}

	track_events_queue_t::nearest_event_info_t
		track_events_queue_t::get_nearest_event_info(
			coordinate_t event_coordinate
		)
	{
		LOG_STACK();

		assert(device_events_queue_.size() > 1);

		auto & cutter_path_info = device_events_queue_.front();

		std::array<path_events_t::event_counters_t*, 2> nearest_event_counters = { nullptr, nullptr };
		coordinate_t nearest_event_coordinate{ 0 };
		position_detector::counter32_t nearest_event_offset_in_counter{ invalid_counter32 };


		bool use_next_path = false;


		if (_queue_direction == queue_direction_t::device_behind)
		{
			use_next_path = true;
		}
		else
		{
			if (cutter_path_info.events.empty())
			{
				use_next_path = true;
			}
			else
			{
				auto next_cutter_event_iter = cutter_path_info.events.rbegin();

				nearest_event_counters[INDEX_CAST(queue_direction_t::device_ahead)] = &next_cutter_event_iter->event_counters[INDEX_CAST(_queue_direction)];
				nearest_event_counters[INDEX_CAST(queue_direction_t::device_behind)] = &next_cutter_event_iter->event_counters[INDEX_CAST(queue_direction_t::device_behind)];
				nearest_event_coordinate = next_cutter_event_iter->coordinate[new_coordinate_idx];
			}
		}

		if (use_next_path)
		{
			if (_queue_direction == queue_direction_t::device_behind)
			{
				auto next_path_iter = current_path_iter_;
				++next_path_iter;
				if (next_path_iter != device_events_queue_.end())
				{
					nearest_event_counters[INDEX_CAST(queue_direction_t::device_ahead)] = &cutter_path_info.event_counters[INDEX_CAST(queue_direction_t::device_ahead)];
					nearest_event_counters[INDEX_CAST(queue_direction_t::device_behind)] = &next_path_iter->event_counters[INDEX_CAST(queue_direction_t::device_behind)];
					nearest_event_coordinate = cutter_path_info.path_interval[INDEX_CAST(queue_direction_t::device_ahead)];
				}
				else
					return {};
			}
			else
			{

				nearest_event_counters[INDEX_CAST(queue_direction_t::device_behind)] = &(++device_events_queue_.begin())->event_counters[INDEX_CAST(queue_direction_t::device_behind)];
				nearest_event_counters[INDEX_CAST(queue_direction_t::device_ahead)] = &cutter_path_info.event_counters[INDEX_CAST(_queue_direction)];
				nearest_event_coordinate = cutter_path_info.path_interval[INDEX_CAST(_queue_direction)];
			}
		}

		nearest_event_offset_in_counter = COUNTER_SPAN(nearest_event_coordinate, event_coordinate, _start_track_traits.counter_size);


		return { true, std::move(nearest_event_counters), nearest_event_coordinate, nearest_event_offset_in_counter };
	}


	std::tuple< position_detector::counter32_t,
		track_events_queue_t::interval_direction_t/*,
		std::list<track_events_queue_t::reverse_event_t>::iterator*/
	>
		track_events_queue_t::get_first_recalc_interval_info(
			coordinate_t event_coordinate,
			nearest_event_info_t & nearest_event_info
		)
	{
		LOG_STACK();

		auto & cutter_path_info = device_events_queue_.front();
		const auto & first_reverse_event = _reverse_events_list.front();

		switch (_queue_direction)
		{
		case  queue_direction_t::device_ahead:
		{
			if (first_reverse_event.queue_direction == queue_direction_t::device_behind)
			{
				if (_start_track_traits.counter0 + _cutter_offset_in_counter >= first_reverse_event.new_track_traits.counter0)
				{
					return { _start_track_traits.counter0, _queue_direction };
				}
				else
				{
					if (first_reverse_event.new_track_traits.compare_path_info(cutter_path_info.start_path_track_traits))
					{
						return { first_reverse_event.new_track_traits.counter0 - COUNTER_SPAN(first_reverse_event.new_track_traits.coordinate0, event_coordinate, _start_track_traits.counter_size),
							_queue_direction
						};
					}
					else
					{
						if (nearest_event_info.valid)
						{
							auto & nearest_counters = *nearest_event_info.counters[INDEX_CAST(queue_direction_t::device_ahead)];
							const auto nearest_counter = nearest_counters.counters[nearest_counters.cur_selection++];
							return { nearest_counter - nearest_event_info.offset_in_counter, _queue_direction };
						}
						else
						{
							//TODO: throw exception
						}
					}
				}
			}
			else
			{
				return { _start_track_traits.counter0, queue_direction_t::device_behind };
			}
			break;
		}
		case  queue_direction_t::device_behind:
		{
			if (first_reverse_event.queue_direction == queue_direction_t::device_ahead)
			{
				return { first_reverse_event.new_track_traits.counter0 - COUNTER_SPAN(first_reverse_event.new_track_traits.coordinate0, event_coordinate, _start_track_traits.counter_size),
					_queue_direction
				};
			}
			else
			{
				const auto first_direction = first_reverse_event.new_track_traits.direction;

				if (first_reverse_event.new_track_traits.compare_path_info(cutter_path_info.start_path_track_traits))
				{
					if (first_reverse_event.new_track_traits.coordinate0 * first_direction >= event_coordinate * first_direction)
					{
						return { first_reverse_event.new_track_traits.counter0, _queue_direction };
					}
					else
					{
						return { first_reverse_event.new_track_traits.counter0 + COUNTER_SPAN(event_coordinate, first_reverse_event.new_track_traits.coordinate0, _start_track_traits.counter_size),
							_queue_direction
						};
					}

				}
				else
				{
					if (nearest_event_info.valid)
					{
						auto & nearest_counters = *nearest_event_info.counters[INDEX_CAST(queue_direction_t::device_behind)];
						const auto nearest_counter = nearest_counters.counters[nearest_counters.cur_selection++];
						return { nearest_counter + nearest_event_info.offset_in_counter, _queue_direction };
					}
					else
					{
						//TODO: throw exception
					}
				}
			}

			break;
		}
		};

		return { invalid_counter32, _queue_direction };


	}


	manager_track_traits
		track_events_queue_t::recalc_track_intervals(
			coordinate_t event_coordinate,
			packets_manager_ns::direction_t /*cutter_direction*/,
			const manager_track_traits & track_traits,
			const change_coordinate_info_t& change_coordinate_info
		)
	{
		LOG_STACK();

		auto & cutter_path_info = device_events_queue_.front();

		manager_track_traits recalc_track_traits{ track_traits };
		position_detector::counter32_t counter_start{ invalid_counter32 };
		if (!_reverse_events_list.empty())
		{
			nearest_event_info_t nearest_event_info{};

			if (device_events_queue_.size() != 1
				&& p_current_path_ != &cutter_path_info
				)
			{
				nearest_event_info = get_nearest_event_info(event_coordinate);
			}

			interval_direction_t interval_direction{ _queue_direction };
			bool recalc_counter_start{ false };
			bool recalc_coordinate{ true };
			bool last_iter{ false };
			//coordinate_t sign{ _cutter_direction };

			position_detector::counter32_t counter_end = 0;
			coordinate_t coordinate0{ track_traits.coordinate0 };
			manager_track_traits start_path_track_traits{ track_traits };

			const auto & cutter_track_traits = cutter_path_info.start_path_track_traits;

			bool use_nearest_event_counter{ false };

			std::tie(counter_start, interval_direction) = get_first_recalc_interval_info(event_coordinate, nearest_event_info);

			if (counter_start != invalid_counter32)
				recalc_track_traits.counter0 = counter_start;

			std::uint32_t iteration = 0;
			for (auto reverse_event_iter = _reverse_events_list.begin(); reverse_event_iter != _reverse_events_list.end();)
			{
				manager_track_traits * p_cur_track_traits = &reverse_event_iter->new_track_traits;
				counter_end = p_cur_track_traits->counter0;
				coordinate_t old_coordinate0 = reverse_event_iter->new_track_traits.coordinate0;

				const auto recalc_start_interval = [&]
				{
					recalc_counter_start = false;
					position_detector::counter32_t nearest_counter{ counter_end };
					position_detector::counter32_t event_offset_in_counter{ 0 };

					if (nearest_event_info.valid)
					{
						use_nearest_event_counter = false;
						if (cutter_track_traits.compare_path_info_weak(*p_cur_track_traits) &&
							nearest_event_info.coordinate * _cutter_direction > p_cur_track_traits->coordinate0 * _cutter_direction
							)
						{
							event_offset_in_counter = COUNTER_SPAN(old_coordinate0, event_coordinate, track_traits.counter_size);
						}
						else
						{
							use_nearest_event_counter = true;
							auto & nearest_counters = *nearest_event_info.counters[INDEX_CAST(interval_direction)];
							nearest_counter = nearest_counters.counters[nearest_counters.cur_selection++];
							event_offset_in_counter = nearest_event_info.offset_in_counter;
						}
					}
					else
					{
						event_offset_in_counter = COUNTER_SPAN(old_coordinate0, event_coordinate, track_traits.counter_size);
					}

					return nearest_counter - event_offset_in_counter;

				};

				switch (interval_direction)
				{
				case  interval_direction_t::device_ahead:
				{
					if (recalc_counter_start)
						counter_start = recalc_track_traits.counter0 = recalc_start_interval();

					*p_cur_track_traits = recalc_track_traits;
					p_cur_track_traits->counter0 = counter_end;
					p_cur_track_traits->direction *= -1;

					if (++reverse_event_iter == _reverse_events_list.end()) {
						--reverse_event_iter;
						last_iter = true;
					}
					break;
				}

				case  interval_direction_t::device_behind:
				{
					if (reverse_event_iter->queue_direction == queue_direction_t::device_behind)
					{
						p_cur_track_traits = &start_path_track_traits;

						position_detector::counter32_t nearest_counter{ counter_start };

						if (use_nearest_event_counter)
						{
							auto & nearest_counters = *nearest_event_info.counters[INDEX_CAST(queue_direction_t::device_behind)];
							nearest_counter = nearest_counters.counters[nearest_counters.cur_selection++];

						}

						counter_end = nearest_counter + COUNTER_SPAN(coordinate0, p_cur_track_traits->coordinate0, track_traits.counter_size);

						recalc_counter_start = true;
						recalc_coordinate = false;
						if (last_iter)
							++reverse_event_iter;
					}
					else
					{
						*p_cur_track_traits = track_traits;
						p_cur_track_traits->counter0 = counter_end;

						if (use_nearest_event_counter)
						{
							auto & nearest_counters = *nearest_event_info.counters[INDEX_CAST(queue_direction_t::device_behind)];
							const auto & nearest_counter = nearest_counters.counters[nearest_counters.cur_selection];
							if (nearest_counter <= counter_end)
								++nearest_counters.cur_selection;

						}

						++reverse_event_iter;
					}

				}
				};//switch (queue_direction)

				if (counter_start != invalid_counter32 && counter_end != invalid_counter32)
				{
					if (recalc_coordinate) 
					{
						coordinate0 = p_cur_track_traits->coordinate0 = calculate_coordinate(coordinate0, recalc_track_traits.direction*distance_from_counter(counter_end, counter_start, p_cur_track_traits->counter_size));
					}
					else
					{
						coordinate0 = track_traits.coordinate0;
						recalc_coordinate = true;
					}

					LOG_DEBUG() << L"Iteration "sv << ++iteration
						<< L". Queue direction: "sv << to_wstring(interval_direction)
						<< L". Counters interval for recalc ["sv << counter_start << L", "sv << counter_end << L']'
						<< L" recalc event coordinate0: "sv << recalc_track_traits.coordinate0
						<< L" next event coordinate0: "sv << coordinate0;

					change_coordinate_info(counter_start, counter_end, recalc_track_traits);
					counter_start = counter_end;
				}

				recalc_track_traits = *p_cur_track_traits;

				reverse_queue_direction(interval_direction);

			}//for (auto reverse_event_iter = _reverse_events_list.begin(); reverse_event_iter != _reverse_events_list.end();)

			if (_reverse_events_list.back().queue_direction == queue_direction_t::device_ahead)
			{
				coordinate0 = recalc_track_traits.coordinate0;
				counter_start = recalc_track_traits.counter0;
				recalc_track_traits = track_traits;
				recalc_track_traits.counter0 = counter_start;
				recalc_track_traits.coordinate0 = coordinate0;
			}
			else
			{
				recalc_track_traits = start_path_track_traits;

				if (_start_track_traits.counter0 + _cutter_offset_in_counter >= track_traits.counter0)
					recalc_track_traits.counter0 = counter_start = _start_track_traits.counter0;
				else
					recalc_track_traits.counter0 = counter_start = track_traits.counter0 - static_cast<decltype(counter_start)>(_cutter_offset_in_counter);

			}

			if (nearest_event_info.valid)
			{
				nearest_event_info.counters[0]->cur_selection = nearest_event_info.counters[1]->cur_selection = 0;
			}


		}//if (!_reverse_events_list.empty())
		else
		{
			position_detector::counter_t res_counter = (position_detector::counter_t)track_traits.counter0 - _cutter_offset_in_counter * sign_direction_from_queue_direction(_queue_direction);
			if (sign_direction_from_queue_direction(_queue_direction) > 0 && _start_track_traits.counter0 + _cutter_offset_in_counter >= track_traits.counter0)
				res_counter = _start_track_traits.counter0;

			recalc_track_traits.counter0 = counter_start = static_cast<decltype(track_traits.counter0)>(res_counter);
		}

		LOG_DEBUG() << L"Last counters interval for recalc ["sv << counter_start << L", "sv << track_traits.counter0 << L']';

		change_coordinate_info(counter_start, track_traits.counter0, recalc_track_traits);

		return recalc_track_traits;

	}

	manager_track_traits
		track_events_queue_t::process_coordinate_correct_event(
			const coordinate_t & cutter_coordinate,
			packets_manager_ns::direction_t cutter_direction,
			manager_track_traits && track_traits,
			const change_coordinate_info_t& change_coordinate_info
		)

	{
		LOG_STACK();

		LOG_DEBUG() << L"Current queue direction: "sv << to_wstring(_queue_direction);

		if (device_events_queue_.empty())
			return{};

		if (_queue_direction == queue_direction_t::device_behind)
			return defer_coordinate_correct_point_info(cutter_coordinate, std::move(track_traits), change_coordinate_info);

		return apply_coordinate_correct_event(cutter_coordinate, cutter_direction, std::move(track_traits), change_coordinate_info);

	}

	manager_track_traits
		track_events_queue_t::apply_coordinate_correct_event(
			const coordinate_t & cutter_coordinate,
			packets_manager_ns::direction_t cutter_direction,
			manager_track_traits && track_traits,
			const change_coordinate_info_t& change_coordinate_info
		)
	{
		LOG_STACK();

		bool clear_all_events = false;

		auto & cutter_path_info = device_events_queue_.front();
		if (_queue_direction == queue_direction_t::device_behind && !_reverse_events_list.empty())
		{
			clear_all_events = true;
			auto & last_reverse_event = _reverse_events_list.back();

			if (last_reverse_event.queue_direction != _queue_direction)
			{// последнее реверс событие не совпадает по направлению с текущим направлением движения, что-то не так
				track_traits.counter0 += static_cast<decltype(track_traits.counter0)>(_cutter_offset_in_counter);
			}
			else
			{
				if (!last_reverse_event.new_track_traits.compare_path_info_weak(cutter_path_info.start_path_track_traits) ||
					cutter_coordinate * _cutter_direction > last_reverse_event.new_track_traits.coordinate0 * _cutter_direction
					)
				{
					track_traits.counter0 += static_cast<decltype(track_traits.counter0)>(_cutter_offset_in_counter);
				}
				else
				{
					track_traits.counter0 = last_reverse_event.new_track_traits.counter0;
					track_traits.coordinate0 += track_traits.direction*std::abs(last_reverse_event.new_track_traits.coordinate0 - cutter_coordinate);
				}
			}

		}// if (_queue_direction == queue_direction_t::device_ahead)

		if (clear_all_events)
		{
			device_events_queue_.clear();
			_reverse_events_list.clear();
			manager_track_traits new_track_traits{ track_traits };
			set_begin_path_info(std::move(new_track_traits), not_new_transit_flag, _queue_direction == queue_direction_t::device_ahead ? device_ahead : !device_ahead);
		}


		const auto & recalc_track_traits = recalc_track_intervals(cutter_coordinate, cutter_direction, track_traits, change_coordinate_info);

		//if (_cutter_direction != track_traits.direction)
		//{
		//	_cutter_direction = track_traits.direction;
		//	change_cutter_path(track_traits);
		//}

		return recalc_track_traits;
	}

	std::pair<bool, manager_track_traits> 
		track_events_queue_t::check_need_recalc_track(
			const coordinate_t & event_coordinate,
			packets_manager_ns::direction_t cutter_direction, 
			const manager_track_traits & track_traits
		)
	{
		LOG_STACK();

		if (!_reverse_events_list.empty())
		{
			if (_reverse_events_list.back().queue_direction != _queue_direction)
			{// последнее реверс событие не совпадает по направлению с текущим направлением движения, что-то не так
				const auto last_reverse = _reverse_events_list.back();
				_reverse_events_list.pop_back();

				for (auto & path_info : device_events_queue_)
				{
					if (last_reverse.new_track_traits.compare_path_info_weak(path_info.start_path_track_traits))
					{// reverse event occured in this path
						for (const auto iter : path_info.reverse_events)
							_reverse_events_list.erase(iter);

						path_info.reverse_events.clear();
					}
				}

				return { true, {} };
			}

			bool reset_events = _queue_direction == queue_direction_t::device_behind;

			if(_queue_direction == queue_direction_t::device_ahead)
			{
				if (device_events_queue_.size() <= 1)
				{
					if (p_current_path_->start_path_track_traits.coordinate0*cutter_direction >= event_coordinate * cutter_direction)
					{
						reset_events = true;
					}
					else
					{
						//reset_events = _race_detect_strategy == race_detect_strategy_t::check_all_reverses;
						if (_race_detect_strategy == race_detect_strategy_t::check_all_reverses)
						{
							const auto cutter_offset_mm = _cutter_offset_in_counter * track_traits.counter_size * cutter_direction;
							for (auto reverse_event_iter = _reverse_events_list.begin(); reverse_event_iter != --_reverse_events_list.end(); ++reverse_event_iter)
							{
								if (reverse_event_iter->queue_direction == queue_direction_t::device_behind &&
									reverse_event_iter->new_track_traits.coordinate0*cutter_direction <= (event_coordinate + cutter_offset_mm) * cutter_direction
									)
								{
									reset_events = true;
									break;
								}
							}
						}
					}
				}//if (device_events_queue_.size() <= 1)
			}

			if (reset_events)
			{
				auto recalc_track_traits = track_traits;

				auto & cutter_path_info = device_events_queue_.front();
				auto & last_reverse_event = _reverse_events_list.back();
				if (_queue_direction == queue_direction_t::device_behind &&
					(!last_reverse_event.new_track_traits.compare_path_info_weak(cutter_path_info.start_path_track_traits) ||
						event_coordinate * _cutter_direction > last_reverse_event.new_track_traits.coordinate0 * _cutter_direction)
					)
				{
					recalc_track_traits.counter0 += static_cast<decltype(recalc_track_traits.counter0)>(_cutter_offset_in_counter);
				}
				else
				{
					recalc_track_traits.counter0 = last_reverse_event.new_track_traits.counter0;
					recalc_track_traits.coordinate0 += track_traits.direction*std::abs(last_reverse_event.new_track_traits.coordinate0 - event_coordinate);
				}

				return { false, recalc_track_traits };
			}
			//else if (_queue_direction == queue_direction_t::device_ahead)
			//	return { true, {} };

		}//if (!_reverse_events_list.empty())

		return { true, {} };
	}


	manager_track_traits
		track_events_queue_t::process_change_path_event(
			const coordinate_t & cutter_coordinate,
			packets_manager_ns::direction_t cutter_direction,
			manager_track_traits && track_traits,
			const change_coordinate_info_t& change_coordinate_info
		)
	{

		LOG_STACK();

		LOG_DEBUG() << L"Current queue direction: "sv << to_wstring(_queue_direction);

		if (device_events_queue_.empty())
			return{};

		if (_queue_direction == queue_direction_t::device_behind)
			return defer_new_path_point_info(cutter_coordinate, std::move(track_traits), change_coordinate_info);

		return apply_change_path_event(cutter_coordinate, cutter_direction, std::move(track_traits), change_coordinate_info);

	}

	manager_track_traits
		track_events_queue_t::apply_change_path_event(
			const coordinate_t & event_coordinate,
			packets_manager_ns::direction_t cutter_direction,
			manager_track_traits && track_traits,
			const change_coordinate_info_t& change_coordinate_info
		)
	{
		LOG_STACK();

		if (track_traits.compare_path_info(device_events_queue_.front().start_path_track_traits))
			return{};

		if (p_current_path_ != &device_events_queue_.front() &&
			track_traits.compare_path_info(p_current_path_->start_path_track_traits)
			)
			return{};

		auto [need_recalc, recalc_track_traits] = check_need_recalc_track(event_coordinate, cutter_direction, track_traits);

		if (need_recalc)
		{
			recalc_track_traits = recalc_track_intervals(event_coordinate, cutter_direction, track_traits, change_coordinate_info);
		}
		else
			if(recalc_track_traits.valid())
				change_coordinate_info(recalc_track_traits.counter0, track_traits.counter0, recalc_track_traits);


		if (recalc_track_traits.valid()) 
		{
			device_events_queue_.clear();
			_reverse_events_list.clear();

			set_begin_path_info(manager_track_traits(recalc_track_traits), not_new_transit_flag, _queue_direction == queue_direction_t::device_ahead ? device_ahead : !device_ahead);
		}

		return recalc_track_traits;
	}

}//namespace position_detector::packets_manager_ns