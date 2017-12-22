#define NOMINMAX

#include "track_events_queue.h"

namespace position_detector
{
	namespace packets_manager_ns
	{

		namespace
		{
			class reset_reverse_events_exception : public std::exception
			{};
		}


		const coordinate_t invalid_coordinate{ std::numeric_limits<coordinate_t>::max() };
		const position_detector::counter32_t invalid_counter{ 0 };

#define INDEX_CAST(_queue_direction_) static_cast<std::size_t>(_queue_direction_)

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

		void track_events_queue_t::change_cutter_path(const manager_track_traits & track_traits)
		{
			LOG_STACK();

			if (device_events_queue_.empty() || _queue_direction == queue_direction_t::device_behind)
				return;

			auto & cutter_path_info = device_events_queue_.front();
			
			if (!cutter_path_info.start_path_track_traits.compare_path_info(track_traits))
				cutter_path_info.start_path_track_traits = track_traits;

			cutter_path_info.path_interval[INDEX_CAST(queue_direction_t::device_ahead)] = invalid_coordinate;

			for (auto path_info_iter = ++device_events_queue_.begin(); path_info_iter != device_events_queue_.end(); ++path_info_iter)
			{
				cutter_path_info.reverse_events.splice(cutter_path_info.reverse_events.cend(), std::move(path_info_iter->reverse_events));
			}

			auto cutter_path_info_buffer = device_events_queue_.front();
			device_events_queue_.clear();

			device_events_queue_.push_front(std::move(cutter_path_info_buffer));

			auto & first_path_info = device_events_queue_.front();

			p_current_path_events_ = &first_path_info.events;
			p_current_path_ = &first_path_info;
			current_path_iter_ = device_events_queue_.begin();
			next_event_[INDEX_CAST(queue_direction_t::device_behind)] = next_event_[INDEX_CAST(queue_direction_t::device_ahead)] = p_current_path_events_->end();

		}


		bool track_events_queue_t::process_track(const coordinate_t & coordinate, const synchronization::counter_t & counter, manager_track_traits & track_traits, const coordinate_t & cutter_coordinate, packets_manager_ns::direction_t cutter_direction)
		{
			LOG_STACK();

			LOG_DEBUG() << L"Current queue direction: " << to_wstring(_queue_direction);

			if (device_events_queue_.empty() && p_current_path_events_ == nullptr)
				return false;

			bool result{ false };
			const auto direction = track_traits.direction;
			auto events_limit_iter = _queue_direction == queue_direction_t::device_ahead ? p_current_path_events_->end() : p_current_path_events_->begin();

			if (next_event_[INDEX_CAST(_queue_direction)] != events_limit_iter)
			{//in current movment direction there are coordinate correction events 
				decltype(p_current_path_events_->end()) event_iter{ next_event_[INDEX_CAST(_queue_direction)] };

				const auto & event_coordinate_limit = event_iter->coordinate[INDEX_CAST(_queue_direction)];

				if (event_coordinate_limit != invalid_coordinate && event_coordinate_limit*direction > coordinate*direction)
				{// change next event

					if (_queue_direction == queue_direction_t::device_ahead){
						++next_event_[INDEX_CAST(queue_direction_t::device_behind)]; ++next_event_[INDEX_CAST(queue_direction_t::device_ahead)];
					}
					else{
						--next_event_[INDEX_CAST(queue_direction_t::device_behind)]; --next_event_[INDEX_CAST(queue_direction_t::device_ahead)];
					}

					event_iter->event_counters[INDEX_CAST(_queue_direction)].counters.push_back(counter);

					track_traits = event_iter->new_track_traits;
					track_traits._path_info->counter0 = track_traits.counter0 = counter;

					result = true;
				}
			}

			if (!result)
			{
				const auto & path_coordinate_limit = p_current_path_->path_interval[INDEX_CAST(_queue_direction)];

				if (path_coordinate_limit != invalid_coordinate && path_coordinate_limit*direction <= coordinate*direction)
				{// change current path

					const auto sign_direction = sign_direction_from_queue_direction(_queue_direction);
					std::advance(current_path_iter_, sign_direction * 1);

					auto & current_path_info = *current_path_iter_;

					p_current_path_events_ = &current_path_info.events;
					p_current_path_ = &current_path_info;
					auto events_limit_iter = _queue_direction == queue_direction_t::device_ahead ? p_current_path_events_->begin() : p_current_path_events_->end();

					next_event_[INDEX_CAST(queue_direction_t::device_behind)] = next_event_[INDEX_CAST(queue_direction_t::device_ahead)] = events_limit_iter;
					if (!p_current_path_events_->empty())
						--next_event_[INDEX_CAST(queue_direction_t::device_ahead)];

					p_current_path_->event_counters[INDEX_CAST(_queue_direction)].counters.push_back(counter);

					track_traits = p_current_path_->start_path_track_traits;
					track_traits._path_info->counter0 = track_traits.counter0 = counter;

					if (p_current_path_->path_interval[INDEX_CAST(queue_direction_t::device_behind)] != invalid_coordinate)
					{
						const decltype(track_traits.direction) k = static_cast<decltype(track_traits.direction)>(sign_direction);
						const decltype(track_traits.direction) new_direction = k*(p_current_path_->path_interval[1] - p_current_path_->path_interval[0]) > 0 ? 1 : -1;
						track_traits.direction = new_direction;
					}

					result = true;
				}
			}

			if (_queue_direction == queue_direction_t::device_ahead)
			{// cut reverse events
				auto & cutter_path_info = device_events_queue_.front();
				while (!cutter_path_info.reverse_events.empty())
				{
					auto first_reverse_event_iter = cutter_direction == 1 ? cutter_path_info.reverse_events.begin() : --cutter_path_info.reverse_events.end();
					if ((*first_reverse_event_iter)->new_track_traits.coordinate0*cutter_direction < cutter_coordinate*cutter_direction)
					{// cut reverse event

						LOG_DEBUG() << L"Cut reverse event. Reverse event coordinate: "
							<< (*first_reverse_event_iter)->new_track_traits.coordinate0
							<< L" cutter coordinate: " << cutter_coordinate
							<< L", cutter direction: " << cutter_direction
							<< L", _cutter direction: " << _cutter_direction;
						_reverse_events_list.erase(*first_reverse_event_iter);
						cutter_path_info.reverse_events.erase(first_reverse_event_iter);

						LOG_DEBUG() << L"Cutter path reverse events count: " << cutter_path_info.reverse_events.size();

						continue;
					}

					LOG_DEBUG() << L"Reverse events count: " << _reverse_events_list.size() << L". Cutter path reverse events count: " << cutter_path_info.reverse_events.size();

					break;
				}
			}

			return result;
		}

		void track_events_queue_t::reverse(const coordinate_t & /*coordinate*/, manager_track_traits && track_traits)
		{
			LOG_STACK();

			if (device_events_queue_.empty())
				return;

			LOG_DEBUG() << L"Current queue direction: " << to_wstring(_queue_direction);

			reverse_queue_direction(_queue_direction);

			_cutter_direction *= -1;

			_reverse_events_list.push_back({ _queue_direction, std::move(track_traits) });
			//if (_queue_direction == queue_direction_t::device_ahead)
			{//recalc next reverse event for cutting

				auto last_reverse_iter = _reverse_events_list.end();
				--last_reverse_iter;
				for (auto & path_info : device_events_queue_)
				{
					if (last_reverse_iter->new_track_traits.compare_path_info(path_info.start_path_track_traits))
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

			LOG_DEBUG() << L"Reverse events count: " << _reverse_events_list.size();
			LOG_DEBUG() << L"Queue direction after reverse: " << to_wstring(_queue_direction);

		}

		manager_track_traits track_events_queue_t::defer_coordinate_correct_point_info(const coordinate_t & coordinate, manager_track_traits && track_traits)
		{
			LOG_STACK();

			if (device_events_queue_.empty() || _queue_direction == queue_direction_t::device_ahead)
				return{};

			if (_cutter_direction != track_traits.direction)
			{
				_cutter_direction = track_traits.direction;

				//if (!device_events_queue_.front().events.empty())
				{// событие коррекции координаты со сменой направления не первое для данного пути

					auto & cutter_path_info = device_events_queue_.front();
					cutter_path_info.start_path_track_traits.direction = _cutter_direction;
					if (p_current_path_ == &cutter_path_info)
					{
						device_events_queue_.clear();
						_reverse_events_list.clear();

						manager_track_traits new_track_traits{ track_traits };

						new_track_traits.coordinate0 -= new_track_traits.counter_size*_cutter_offset_in_counter*_cutter_direction;

						set_begin_path_info(std::move(new_track_traits), false, false);
						return device_events_queue_.front().start_path_track_traits;
					}

					return{};
				}

			}

			device_events_queue_.front().events.push_front({ { { track_traits.coordinate0, coordinate } }, std::move(track_traits), {} });
			return{};
		}

		manager_track_traits track_events_queue_t::defer_new_path_point_info(const coordinate_t & coordinate, manager_track_traits && track_traits)
		{
			LOG_STACK();

			if (device_events_queue_.empty() || _queue_direction == queue_direction_t::device_ahead)
				return{};

			auto last_path_iter = device_events_queue_.begin();
			last_path_iter->path_interval[INDEX_CAST(queue_direction_t::device_behind)] = coordinate;

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


		manager_track_traits track_events_queue_t::recalc_track_intervals(const coordinate_t & cutter_coordinate, packets_manager_ns::direction_t /*cutter_direction*/, const manager_track_traits & track_traits, const change_coordinate_info_t& change_coordinate_info)
		{
			LOG_STACK();

			auto & cutter_path_info = device_events_queue_.front();

			position_detector::counter32_t counter_start{ invalid_counter };
			manager_track_traits recalc_track_traits{ track_traits };

			if (!_reverse_events_list.empty())
			{
				std::array<path_events_t::event_counters_t*, 2> nearest_event_counters = { nullptr, nullptr };
				coordinate_t nearest_event_coordinate{ 0 };
				position_detector::counter32_t nearest_event_offset_in_counter{ 0 };

				const bool use_nearest_event_counters = !(device_events_queue_.size() == 1 || p_current_path_->start_path_track_traits.compare_path_info(track_traits));
				if (use_nearest_event_counters)
				{
					bool use_next_path = false;
					if (cutter_path_info.events.empty())
					{
						use_next_path = true;
					}
					else
					{
						auto next_cutter_event_iter = cutter_path_info.events.begin();

						while (next_cutter_event_iter != cutter_path_info.events.end())
						{
							if (cutter_coordinate * _cutter_direction > next_cutter_event_iter->coordinate[INDEX_CAST(queue_direction_t::device_behind)] * _cutter_direction)
							{//try get next cutter path event
								++next_cutter_event_iter;
								continue;
							}
							break;
						}

						if (next_cutter_event_iter == cutter_path_info.events.end()){
							use_next_path = true;
						}
						else{

							nearest_event_counters[INDEX_CAST(queue_direction_t::device_ahead)] = &next_cutter_event_iter->event_counters[INDEX_CAST(_queue_direction)];
							nearest_event_counters[INDEX_CAST(queue_direction_t::device_behind)] = &next_cutter_event_iter->event_counters[INDEX_CAST(queue_direction_t::device_behind)];
							nearest_event_coordinate = next_cutter_event_iter->coordinate[0];
						}
					}

					if (use_next_path)
					{
						nearest_event_counters[INDEX_CAST(queue_direction_t::device_behind)] = &(++device_events_queue_.begin())->event_counters[INDEX_CAST(queue_direction_t::device_behind)];
						nearest_event_counters[INDEX_CAST(queue_direction_t::device_ahead)] = &cutter_path_info.event_counters[INDEX_CAST(_queue_direction)];
						nearest_event_coordinate = cutter_path_info.path_interval[INDEX_CAST(_queue_direction)];
					}

					nearest_event_offset_in_counter = static_cast<position_detector::counter32_t>(std::abs(nearest_event_coordinate - cutter_coordinate) / track_traits.counter_size);

				}//if (use_nearest_event_counters)

				queue_direction_t queue_direction{ queue_direction_t::device_ahead };
				bool recalc_counter_start{ true };
				bool recalc_coordinate{ true };
				bool last_iter{ false };
				//coordinate_t sign{ _cutter_direction };

				position_detector::counter32_t counter_end = 0;
				coordinate_t coordinate0{ track_traits.coordinate0 };
				manager_track_traits start_path_track_traits{ track_traits };

				const auto & cutter_track_traits = cutter_path_info.start_path_track_traits;

				position_detector::counter32_t event_offset_in_counter{ 0 };
				bool use_nearest_event_counter{ false };

				{// initialize counter_start if need
					const auto & first_reverse_event = _reverse_events_list.front();
					if (first_reverse_event.queue_direction == queue_direction_t::device_behind)
					{
						if (_start_track_traits.counter0 + _cutter_offset_in_counter >= first_reverse_event.new_track_traits.counter0)
						{
							recalc_counter_start = false;
							counter_start = recalc_track_traits.counter0 = _start_track_traits.counter0;
						}
					}
					else
					{
						queue_direction = queue_direction_t::device_behind;
						counter_start = recalc_track_traits.counter0 = _start_track_traits.counter0;
						recalc_counter_start = false;
					}

				}

				std::uint32_t iteration = 0;
				for (auto reverse_event_iter = _reverse_events_list.begin(); reverse_event_iter != _reverse_events_list.end();)
				{
					manager_track_traits * _track_traits = &reverse_event_iter->new_track_traits;
					counter_end = _track_traits->counter0;
					coordinate_t old_coordinate0 = reverse_event_iter->new_track_traits.coordinate0;

					switch (queue_direction)
					{
						case  queue_direction_t::device_ahead:
						{
							if (recalc_counter_start)
							{
								recalc_counter_start = false;
								position_detector::counter32_t nearest_counter{ counter_end };

								if (use_nearest_event_counters)
								{
									use_nearest_event_counter = false;
									if (cutter_track_traits.compare_path_info(*_track_traits) &&
										nearest_event_coordinate * _cutter_direction > _track_traits->coordinate0 * _cutter_direction
										)
									{
										event_offset_in_counter = static_cast<decltype(counter_start)>(std::abs(old_coordinate0 - cutter_coordinate) / track_traits.counter_size);
									}
									else
									{
										use_nearest_event_counter = true;
										auto & nearest_counters = *nearest_event_counters[static_cast<std::size_t>(queue_direction)];
										nearest_counter = nearest_counters.counters[nearest_counters.cur_selection++];
										event_offset_in_counter = nearest_event_offset_in_counter;
									}
								}
								else
								{
									event_offset_in_counter = static_cast<decltype(counter_start)>(std::abs(old_coordinate0 - cutter_coordinate) / track_traits.counter_size);
								}

								counter_start = recalc_track_traits.counter0 = nearest_counter - event_offset_in_counter;
							}//if (recalc_counter_start)

							*_track_traits = recalc_track_traits;
							_track_traits->counter0 = counter_end;
							_track_traits->direction *= -1;

							if (++reverse_event_iter == _reverse_events_list.end()){
								--reverse_event_iter;
								last_iter = true;
							}
							break;
						}

						case  queue_direction_t::device_behind:
						{
							if (reverse_event_iter->queue_direction == queue_direction_t::device_behind)
							{
								_track_traits = &start_path_track_traits;

								position_detector::counter32_t nearest_counter{ counter_start };

								if (use_nearest_event_counter)
								{
									auto & nearest_counters = *nearest_event_counters[0];
									nearest_counter = nearest_counters.counters[nearest_counters.cur_selection++];

								}

								counter_end = counter_start + static_cast<decltype(counter_start)>(std::abs(coordinate0 - _track_traits->coordinate0) / track_traits.counter_size);

								recalc_counter_start = true;
								recalc_coordinate = false;
								if (last_iter)
									++reverse_event_iter;
							}
							else
							{
								*_track_traits = track_traits;
								_track_traits->counter0 = counter_end;

								if (use_nearest_event_counter)
								{
									auto & nearest_counters = *nearest_event_counters[0];
									const auto & nearest_counter = nearest_counters.counters[nearest_counters.cur_selection];
									if (nearest_counter <= counter_end)
										++nearest_counters.cur_selection;

								}

								++reverse_event_iter;
							}

						}
					};//switch (queue_direction)

					if (recalc_coordinate){
						coordinate0 = _track_traits->coordinate0 = calculate_coordinate(coordinate0, recalc_track_traits.direction*distance_from_counter(counter_end, counter_start, _track_traits->counter_size));
					}
					else
					{
						coordinate0 = track_traits.coordinate0;
						recalc_coordinate = true;
					}

					LOG_DEBUG() << L"Iteration " << ++iteration 
								<< L". Queue direction: " << to_wstring(queue_direction) 
								<< L". Counters interval for recalc [" << std::to_wstring(counter_start) << L", " << std::to_wstring(counter_end) << L"]"
								<< L" recalc event coordinate0: " << recalc_track_traits.coordinate0
								<< L" next event coordinate0: " << coordinate0;

					change_coordinate_info(counter_start, counter_end, recalc_track_traits);
					counter_start = counter_end;
					recalc_track_traits = *_track_traits;

					reverse_queue_direction(queue_direction);

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

				if (use_nearest_event_counters)
				{
					nearest_event_counters[0]->cur_selection = nearest_event_counters[1]->cur_selection = 0;
				}


			}//if (!_reverse_events_list.empty())
			else
			{
				position_detector::counter_t res_counter = (position_detector::counter_t)track_traits.counter0 - _cutter_offset_in_counter;
				if (_start_track_traits.counter0 + _cutter_offset_in_counter >= track_traits.counter0)
					res_counter = _start_track_traits.counter0;

				recalc_track_traits.counter0 = counter_start = static_cast<decltype(track_traits.counter0)>(res_counter);
			}

			LOG_DEBUG() << L"Last counters interval for recalc [" << std::to_wstring(counter_start) << L", " << std::to_wstring(track_traits.counter0) << L"]";

			change_coordinate_info(counter_start, track_traits.counter0, recalc_track_traits);

			return recalc_track_traits;

		}

		manager_track_traits track_events_queue_t::process_coordinate_correct_event(const coordinate_t & cutter_coordinate, packets_manager_ns::direction_t cutter_direction, manager_track_traits && track_traits, const change_coordinate_info_t& change_coordinate_info)
		{
			LOG_STACK();

			LOG_DEBUG() << L"Current queue direction: " << to_wstring(_queue_direction);

			if (device_events_queue_.empty() || _queue_direction == queue_direction_t::device_behind)
				return{};

			auto & cutter_path_info = device_events_queue_.front();
			if (_cutter_direction == track_traits.direction)
			{
				if (!cutter_path_info.events.empty())
				{
					auto & next_cutter_event = cutter_path_info.events.front();

					if ((cutter_coordinate - _coordinate_valid_span) < next_cutter_event.coordinate[0] &&
						(cutter_coordinate + _coordinate_valid_span) > next_cutter_event.coordinate[0] &&
						(track_traits.coordinate0 - _coordinate_valid_span) < next_cutter_event.coordinate[1] &&
						(track_traits.coordinate0 + _coordinate_valid_span) > next_cutter_event.coordinate[1]
						)
					{
						cutter_path_info.events.pop_front();
						return{};
					}
				}
			}//if (_cutter_direction == track_traits.direction)

			bool clear_all_events = true;
			if (device_events_queue_.size() == 1/*cutter_path_info.path_interval[_queue_direction] == invalid_coordinate*/)
			{//возможно через эту точку корекции координаты состав не проезжал в обратном направлении к текущему направлению движения
				const auto cutter_coordinate_mod = cutter_coordinate*_cutter_direction;

				if (_reverse_events_list.empty())
				{
					if (cutter_path_info.start_path_track_traits.coordinate0 * _cutter_direction <= cutter_coordinate_mod)
					{
						clear_all_events = false;
					}
				}
				else
				{
					clear_all_events = false;
					for (const auto & reverse_event : _reverse_events_list)
					{
						if (reverse_event.new_track_traits.coordinate0*_cutter_direction > cutter_coordinate_mod)
						{
							clear_all_events = true;
							break;
						}

					}

				}//if (_reverse_events_list.empty())
				
			}//if (device_events_queue_.size() == 1)
			/*else поезд точно проезжал эту точку корекции координаты в обратном направлении к текущему направлению движения*/

			if (clear_all_events)
			{
				device_events_queue_.clear();
				_reverse_events_list.clear();

				manager_track_traits new_track_traits{ track_traits };
				new_track_traits.counter0 -= static_cast<decltype(track_traits.counter0)>(_cutter_offset_in_counter);
				set_begin_path_info(std::move(new_track_traits), false, true);
			}


			const auto & recalc_track_traits = recalc_track_intervals(cutter_coordinate, cutter_direction, track_traits, change_coordinate_info);

			if (_cutter_direction != track_traits.direction)
			{
				_cutter_direction = track_traits.direction;
				change_cutter_path(track_traits);
			}


			return recalc_track_traits;

		}

		bool track_events_queue_t::check_need_recalc_track(const coordinate_t & cutter_coordinate, packets_manager_ns::direction_t cutter_direction, const manager_track_traits & track_traits)
		{
			LOG_STACK();

			if (device_events_queue_.size() <= 1)
			{
				if (!_reverse_events_list.empty())
				{
					if (_reverse_events_list.back().queue_direction == queue_direction_t::device_behind)
					{
						device_events_queue_.clear();
						_reverse_events_list.clear();
						manager_track_traits new_track_traits{ track_traits };
						new_track_traits.counter0 -= static_cast<decltype(track_traits.counter0)>(_cutter_offset_in_counter);
						set_begin_path_info(std::move(new_track_traits), false, true);

					}
					else
					{
						(void)(cutter_direction);
						(void)(cutter_coordinate);
						bool reset_events = false;
						//coordinate_t device_offset_abs = _cutter_offset_in_counter * _start_track_traits.counter_size;

						if (p_current_path_->start_path_track_traits.coordinate0*cutter_direction >= cutter_coordinate*cutter_direction)
						{
							reset_events = true;
						}
						else {
							if (_race_detect_strategy == race_detect_strategy_t::check_all_reverses)
								for (auto reverse_event_iter = _reverse_events_list.begin(); reverse_event_iter != --_reverse_events_list.end(); ++reverse_event_iter)
								{
									if (reverse_event_iter->queue_direction == queue_direction_t::device_behind)
									{
										if (reverse_event_iter->new_track_traits.coordinate0*cutter_direction >= cutter_coordinate*cutter_direction)
										{
											reset_events = true;
											break;
										}
									}
								}
						}

						if (reset_events)
						{
							auto last_reverse_event = _reverse_events_list.back();
							_reverse_events_list.clear();
							_reverse_events_list.push_back(std::move(last_reverse_event));
							const auto last_reverse_event_iter = _reverse_events_list.begin();
							auto & path_info = device_events_queue_.front();
							path_info.reverse_events.clear();
							path_info.reverse_events.push_back(last_reverse_event_iter);

							throw reset_reverse_events_exception();

						}
					}
				}

				return true;
			}

			auto & next_cutter_path_info = *(++device_events_queue_.begin());

			if (track_traits.compare_path_info(next_cutter_path_info.start_path_track_traits))
			{
				// need compare direction

				decltype(track_traits.direction) new_direction;
				if (next_cutter_path_info.path_interval[1] == invalid_coordinate)
				{
					new_direction = next_cutter_path_info.start_path_track_traits.direction;
				}
				else
				{
					new_direction = (next_cutter_path_info.path_interval[1] - next_cutter_path_info.path_interval[0]) > 0 ? 1 : -1;
				}

				if (new_direction == cutter_direction)
				{
					device_events_queue_.pop_front();
					next_cutter_path_info.path_interval[static_cast<std::size_t>(queue_direction_t::device_behind)] = invalid_coordinate;

					return false;
				}
			}

			device_events_queue_.clear();
			_reverse_events_list.clear();

			set_begin_path_info(manager_track_traits(track_traits), false, true);

			return true;
		}


		manager_track_traits track_events_queue_t::process_change_path_event(const coordinate_t & cutter_coordinate, packets_manager_ns::direction_t cutter_direction, const manager_track_traits & track_traits, const change_coordinate_info_t& change_coordinate_info)
		{
			LOG_STACK();

			LOG_DEBUG() << L"Current queue direction: " << to_wstring(_queue_direction);

			if (device_events_queue_.empty() || _queue_direction == queue_direction_t::device_behind)
				return{};

			if (track_traits.compare_path_info(device_events_queue_.front().start_path_track_traits))
				return{};

			bool need_recalc{ false };

			try{
				need_recalc = check_need_recalc_track(cutter_coordinate, cutter_direction, track_traits);

			}
			catch (const reset_reverse_events_exception&)
			{
				auto & last_reverse_event = _reverse_events_list.back();
				auto recalc_track_traits = track_traits;
				recalc_track_traits.counter0 = last_reverse_event.new_track_traits.counter0;
				recalc_track_traits.coordinate0 += track_traits.direction*(track_traits.counter0 - recalc_track_traits.counter0)*track_traits.counter_size;
				change_coordinate_info(recalc_track_traits.counter0, track_traits.counter0, recalc_track_traits);

				return recalc_track_traits;
			}

			if (need_recalc)
			{
				const auto & recalc_track_traits = recalc_track_intervals(cutter_coordinate, cutter_direction, track_traits, change_coordinate_info);

				if (!device_events_queue_.front().start_path_track_traits.compare_path_info(track_traits))
				{
					change_cutter_path(track_traits);
				}

				return recalc_track_traits;
			}//if (check_need_recalc_track(cutter_coordinate, cutter_direction, track_traits))


			return{};
		}

	}//namespace packets_manager_ns
}//namespace position_detector