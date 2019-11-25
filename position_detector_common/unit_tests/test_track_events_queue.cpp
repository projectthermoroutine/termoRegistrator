#include <CppUnitTest.h>

#include <common\unit_tests_common.h>
#include <common\sync_helpers.h>

#include <algorithm>
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <utility>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <functional>
#include <random>
#include <type_traits>
#include <numeric>


#include <position_detector_common\details\track_events_queue.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace client_pd_dispatcher_test_project
{
	using namespace unit_tests_common;
	using namespace position_detector;
	using namespace position_detector::packets_manager_ns;

	const coordinate_t invalid_coordinate{ std::numeric_limits<coordinate_t>::max() };
	const position_detector::counter32_t invalid_counter{ 0 };


	const std::wstring railway_name = L"test_railway_0";

	struct line_path_item_t
	{
		std::wstring line_name;
		std::wstring path_name;
	};

	line_path_item_t line_path_items[] = { { L"test_line_0", L"test_path_0" }, { L"test_line_0", L"test_path_1" }, { L"test_line_1", L"test_path_0" }, { L"test_line_1", L"test_path_1" } };

	enum class line_path_code_t : std::size_t
	{
		start_line_path = 0,
		line0_path0 = 0,
		line0_path1 = 1,
		line1_path0 = 2,
		line1_path1 = 3,
	};



	enum class movment_direction_t : std::uint8_t
	{
		forward = 0,
		backward = 1
	};

	enum class orientation_t : std::int8_t
	{
		boiler = 1,
		salon = -1
	};


	inline std::int8_t sign_direction(movment_direction_t direction)
	{
		return direction == movment_direction_t::forward ? 1 : -1;
	}


	template <typename T>
	T any_of(const std::vector<T> & elements)
	{
		if (elements.empty()) return T();
		return elements.at(rand() % elements.size());
	}

	template <typename T>
	typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, bool>::value, T>::type any_from_range(const T min_value, const T max_value)
	{
		if (min_value > max_value)
			throw std::invalid_argument("An invalid argument was passed to the function 'any_from_range'");

		std::uniform_int_distribution<T> dist(min_value, max_value);
		return dist(std::random_device());
	}

	bool any_of_bool()
	{
		return (rand() & 1) != 0;
	}

	movment_direction_t any_of_movment_direction()
	{
		return any_of_bool() ? movment_direction_t::forward : movment_direction_t::backward;
	}

	orientation_t any_of_orientation()
	{
		return any_of_bool() ? orientation_t::salon : orientation_t::boiler;
	}


	template <typename T>
	typename std::enable_if<std::is_same<T, std::wstring>::value, std::wstring>::type
		generate_value()
	{
		const std::size_t sample_chars_len = 16;
		wchar_t chars[sample_chars_len + 1] = L"0123456789ABCDEF";
		const std::size_t min_len = 10;
		const std::size_t max_len = 100;
		const std::size_t len = (static_cast<std::size_t>(rand()) % (max_len - min_len)) + min_len;

		std::wstring result;
		for (std::size_t n = 0; n != len; ++n)
		{
			result += chars[rand() % sample_chars_len];
		}

		return result;
	}

	struct start_data_t
	{
		counter32_t counter0;
		coordinate_t coordinate0;
		movment_direction_t direction0;
		orientation_t orientation0;

		struct
		{
			uint32_t counter_size;
			coordinate_t device_offset;
		};
	};


	template <typename T>
	typename std::enable_if<std::is_same<T, start_data_t>::value, start_data_t>::type
		generate_value()
	{
		start_data_t result = {};
		result.counter0 = any_from_range<decltype(result.counter0)>(1, 10000);
		result.coordinate0 = any_from_range<decltype(result.coordinate0)>(-10000000, 10000000);
		result.counter_size = any_from_range<decltype(result.counter_size)>(1, 10);
		result.device_offset = any_from_range<decltype(result.device_offset)>(-50 * 100 * 10, 50 * 100 * 10);

		result.orientation0 = any_of_orientation();
		result.direction0 = any_of_movment_direction();

		return result;
	}

		std::wstring to_wstring(movment_direction_t movment_direction)
		{
			return movment_direction == movment_direction_t::forward ? L"forward" : L"backward";
		}

		std::wstring to_wstring(orientation_t orientation)
		{
			return orientation == orientation_t::salon ? L"salon" : L"boiler";
		}

	std::wstring dump_start_data(const start_data_t& data)
	{
		std::wstring result{L"start data {"};
		result += L"counter0: " + std::to_wstring(data.counter0);
		result += L"\ncoordinate0: " + std::to_wstring(data.coordinate0);
		result += L"\ncounter_size: " + std::to_wstring(data.counter_size);
		result += L"\ndevice_offset: " + std::to_wstring(data.device_offset);
		result += L"\ndirection0: " + to_wstring(data.direction0);
		result += L"\norientation0: " + to_wstring(data.orientation0);
		result += L"}";

		return result;
	}

	std::wstring dump_track_traits(const manager_track_traits& track_traits)
	{
		std::wstring result{ L"track_traits {" };
		result += L"counter0: " + std::to_wstring(track_traits.counter0);
		result += L"\ncoordinate0: " + std::to_wstring(track_traits.coordinate0);
		result += L"\ncounter_size: " + std::to_wstring(track_traits.counter_size);
		result += L"\ndirection: " + (track_traits.direction == 1 ? std::wstring(L"forward") : std::wstring(L"backward"));
		result += L"\ntrack info: " + track_traits._path_info->line + L"\\" + track_traits._path_info->path;
		result += L"}";

		return result;
	}

	manager_track_traits create_track_traits(counter32_t counter0, coordinate_t coordinate0, movment_direction_t direction, uint32_t counter_size, line_path_code_t line_path_item_index)
	{
		const line_path_item_t & line_path_item = line_path_items[static_cast<std::size_t>(line_path_item_index)];
		
		path_info_ptr_t path_info_ = std::make_shared<path_info>();

		path_info_->line = line_path_item.line_name;
		path_info_->railway = railway_name;
		path_info_->path = line_path_item.path_name;
		path_info_->path_name = line_path_item.path_name;
		path_info_->counter0 = counter0;
		path_info_->direction = static_cast<decltype(path_info_->direction)>(direction);

		manager_track_traits track_traits;

		track_traits.coordinate0 = coordinate0;
		track_traits.counter0 = counter0;
		track_traits.counter_size = counter_size;
		track_traits._path_info = std::move(path_info_);
		track_traits.direction = track_traits.direction0 = direction == movment_direction_t::forward ? 1 : -1;

		return track_traits;
	}


	manager_track_traits create_start_path_track_traits(counter32_t counter0, coordinate_t coordinate0, movment_direction_t direction, uint32_t counter_size)
	{
		return create_track_traits(counter0, coordinate0, direction, counter_size, line_path_code_t::start_line_path);
	}


	bool compare_track_traits(const manager_track_traits& left, const manager_track_traits& right)
	{
		return left.compare_path_info(right) &&
			left.counter_size == right.counter_size &&
			left.counter0 == right.counter0 &&
			left.counter_span == right.counter_span &&
			left.direction == right.direction &&
			left.direction0 == right.direction0 &&
			std::abs(left.coordinate0 - right.coordinate0) < left.counter_size;
	}


	bool is_device_ahead(const start_data_t& start_data)
	{
		bool device_ahead0 = true;
		if (start_data.device_offset > 0 && start_data.orientation0 == orientation_t::salon)
			device_ahead0 = false;
		else
			if (start_data.device_offset < 0 && start_data.orientation0 == orientation_t::boiler)
				device_ahead0 = false;

		return device_ahead0;
	}

	bool is_device_ahead(orientation_t orientation, coordinate_t device_offset)
	{
		if (device_offset > 0 && orientation == orientation_t::salon)
			return false;
		else
			if (device_offset < 0 && orientation == orientation_t::boiler)
				return false;

		return true;
	}


	void reverse_track(manager_track_traits& track_traits)
	{
		track_traits.direction = -1 * track_traits.direction;

		uint8_t _direction = 1;
		if (track_traits.direction == 1)
			_direction = 0;

		if (track_traits._path_info)
			track_traits._path_info->direction = _direction;

		track_traits.counter_span.first = track_traits.counter0;
	}



	inline position_detector::counter_t calc_cutter_offset(const start_data_t& start_data)
	{
		return static_cast<position_detector::counter_t>(std::abs(start_data.device_offset) / start_data.counter_size);
	}


	struct track_traits_info_t
	{
		manager_track_traits track_traits;
		bool cutted;
	};

	std::list<track_traits_info_t> generate_reverse_events_for_one_path(const manager_track_traits& start_track_traits, track_events_queue_t & track_events_queue, bool device_ahead0, const coordinate_t& cutter_offset)
	{
		const auto count_reverse_events = any_from_range<std::uint32_t>(1, 20); //any_from_range<std::uint32_t>(1, 10);
		const auto direction0 = start_track_traits.direction0;
		std::decay_t<decltype(direction0)> reverse_direction = direction0;
		counter32_t reverse_event_counter = start_track_traits.counter0;
		coordinate_t reverse_coordinate = start_track_traits.coordinate0;
		bool device_ahead{ device_ahead0 };

		std::list<track_traits_info_t> reverse_events;

		for (std::uint32_t reverse_event_index = 0; reverse_event_index < count_reverse_events; ++reverse_event_index)
		{
			manager_track_traits current_reverse_event{ start_track_traits };
			const counter32_t counters_elapsed = any_from_range<counter32_t>(1, 100000000);

			reverse_event_counter += counters_elapsed;
			reverse_coordinate += reverse_direction*static_cast<coordinate_t>(counters_elapsed)*start_track_traits.counter_size;
			const coordinate_t cutter_coordinate = reverse_coordinate + cutter_offset;

			if (device_ahead)
				for (auto & reverse_event : reverse_events)
				{
					if (!reverse_event.cutted && reverse_event.track_traits.coordinate0*reverse_direction < cutter_coordinate*reverse_direction)
						reverse_event.cutted = true;
				}


			current_reverse_event.coordinate0 = reverse_coordinate;
			current_reverse_event.counter0 = reverse_event_counter;

			manager_track_traits pre_reverse_event{ current_reverse_event };
			pre_reverse_event.direction = reverse_direction;

			track_events_queue.process_track(reverse_coordinate - reverse_direction*static_cast<coordinate_t>(start_track_traits.counter_size), reverse_event_counter - 1, std::move(pre_reverse_event), cutter_coordinate - reverse_direction*static_cast<coordinate_t>(start_track_traits.counter_size), reverse_direction);

			reverse_direction *= -1;
			current_reverse_event.direction = reverse_direction;

			reverse_events.push_back({ current_reverse_event, false});

			track_events_queue.reverse(reverse_coordinate, manager_track_traits{ current_reverse_event });
			track_events_queue.process_track(reverse_coordinate, reverse_event_counter, std::move(current_reverse_event), cutter_coordinate, reverse_direction);

			device_ahead = !device_ahead;
		}

		std::uint16_t index = 0;
		for (const auto & event : reverse_events){
			LOG_DEBUG() << L"Reverse event #" << (++index) 
						<< L" must cutted: " << event.cutted 
						<< L" " << dump_track_traits(event.track_traits);
		}

		return reverse_events;
	}

	TEST_CLASS(track_events_queue)
	{
	public:

		static void simple_test_one_event_after_start(const start_data_t& start_data)
		{
			LOG_STACK();

			const counter32_t counter0{ start_data.counter0 };
			const coordinate_t coordinate0{ start_data.coordinate0 };
			const movment_direction_t direction0{ start_data.direction0 };
			const uint32_t counter_size = start_data.counter_size;
			const coordinate_t device_offset = start_data.device_offset;
			const coordinate_t device_offset_abs = std::abs(start_data.device_offset);
			const position_detector::counter_t cutter_offset_in_counter = calc_cutter_offset(start_data);
			const decltype(manager_track_traits::direction0) direction0_sign = direction0 == movment_direction_t::backward ? -1 : 1;

			const orientation_t orientation0 = start_data.orientation0;
			coordinate_t sign = static_cast<coordinate_t>(orientation0) * direction0_sign;

			const manager_track_traits start_track_traits = create_start_path_track_traits(counter0, coordinate0 + sign*device_offset, direction0, counter_size);

			const bool device_ahead0 = is_device_ahead(start_data);

			track_events_queue_t device_events_queue;
			device_events_queue.set_cutter_offset(cutter_offset_in_counter);
			device_events_queue.set_begin_path_info(manager_track_traits(start_track_traits), true, device_ahead0);

			std::uint8_t step = 1;

			/*
			просто смена линии
			*/
			coordinate_t event_coordinate = coordinate0 + direction0_sign * device_offset_abs * 2;
			counter32_t event_counter = counter0 + static_cast<decltype(event_counter)>(std::abs(event_coordinate - coordinate0) / counter_size);
			int32_t cutter_direction = start_track_traits.direction;

			coordinate_t event_coordinate0 = -5000 * 100 * 10;


			const auto dump_current_state = [&]
			{
				std::wstring result{ L"[step " + std::to_wstring(step) + L" {" };
				result += L"event_coordinate: " + std::to_wstring(event_coordinate);
				result += L"\nevent_counter: " + std::to_wstring(event_counter);
				result += L"}\n" + dump_start_data(start_data);
				result += L"]";

				return result;

			};

			auto event_track_traits = create_track_traits(event_counter, event_coordinate0, movment_direction_t::forward, counter_size, line_path_code_t::line1_path0);

			std::function<position_detector::counter32_t()> right_counter_start_calc = [&]
			{
				return event_counter - static_cast<position_detector::counter32_t>(cutter_offset_in_counter);
			};

			std::function<position_detector::counter32_t()> right_counter_end_calc = [&]
			{
				return event_counter;
			};

			std::function<manager_track_traits(const position_detector::counter32_t&)> create_right_track_traits = [&](const position_detector::counter32_t& counter_start)
			{
				return create_track_traits(counter_start, event_coordinate0, movment_direction_t::forward, counter_size, line_path_code_t::line1_path0);
			};

			const auto pointsInfoChangedNotifyFunc = [&](const position_detector::counter32_t& counter_start, const position_detector::counter32_t& counter_end, const manager_track_traits & track_traits)
			{

				position_detector::counter32_t right_counter_start = right_counter_start_calc();
				Assert::AreEqual(counter_start, right_counter_start, (L"Invalid start counter. " + dump_current_state()).c_str());
				Assert::AreEqual(counter_end, right_counter_end_calc(), (L"Invalid end counter. " + dump_current_state()).c_str());

				const auto & right_track_traits = create_right_track_traits(right_counter_start);

				Assert::IsTrue(compare_track_traits(track_traits, right_track_traits));

			};

			auto new_track_traits = 
				device_events_queue.process_change_path_event(
					event_coordinate, 
					cutter_direction, 
					std::move(event_track_traits), 
					pointsInfoChangedNotifyFunc
				);
			if (device_ahead0)
				Assert::IsTrue(new_track_traits.valid(), (L"New track traits must be valid. " + dump_current_state()).c_str());
			else
				Assert::IsFalse(new_track_traits.valid(), (L"New track traits must be invalid. Device behind cutter. " + dump_current_state()).c_str());


			/*
			коррекция координаты на начальном пути без смены ориентации
			*/

			++step;// 2
			device_events_queue.reset();

			device_events_queue.set_cutter_offset(cutter_offset_in_counter);
			device_events_queue.set_begin_path_info(manager_track_traits(start_track_traits), true, device_ahead0);

			event_track_traits = create_track_traits(event_counter, event_coordinate0, direction0, counter_size, line_path_code_t::start_line_path);

			create_right_track_traits = [&](const position_detector::counter32_t& counter_start)
			{
				return create_track_traits(counter_start, event_coordinate0, direction0, counter_size, line_path_code_t::line0_path0);
			};

			new_track_traits = device_events_queue.process_coordinate_correct_event(event_coordinate, cutter_direction, std::move(event_track_traits), pointsInfoChangedNotifyFunc);

			if (device_ahead0)
				Assert::IsTrue(new_track_traits.valid(), (L"New track traits must be valid. " + dump_current_state()).c_str());
			else
				Assert::IsFalse(new_track_traits.valid(), (L"New track traits must be invalid. Device behind cutter. " + dump_current_state()).c_str());


			/*
			смена линии до стартовой позиции устройства
			*/
			++step; //3

			device_events_queue.reset();

			device_events_queue.set_cutter_offset(cutter_offset_in_counter);
			device_events_queue.set_begin_path_info(manager_track_traits(start_track_traits), true, device_ahead0);

			event_coordinate = coordinate0 + direction0_sign * device_offset_abs / 2;; //координата события вдвое меньше чем расстояние от старта проезда до устройства (т.е. устройство при старте проезда находилось за событием(коррекция либо смена паспорта))
			event_counter = counter0 + static_cast<decltype(event_counter)>(std::abs(event_coordinate - coordinate0) / counter_size);

			event_track_traits = create_track_traits(event_counter, event_coordinate0, movment_direction_t::forward, counter_size, line_path_code_t::line1_path0);

			right_counter_start_calc = [&]
			{
				return counter0;
			};


			create_right_track_traits = [&](const position_detector::counter32_t& counter_start)
			{
				return create_track_traits(counter_start, event_coordinate0, movment_direction_t::forward, counter_size, line_path_code_t::line1_path0);
			};

			new_track_traits = 
				device_events_queue.process_change_path_event(
					event_coordinate, 
					cutter_direction, 
					std::move(event_track_traits), 
					pointsInfoChangedNotifyFunc
				);

			if (device_ahead0)
				Assert::IsTrue(new_track_traits.valid(), (L"New track traits must be valid. " + dump_current_state()).c_str());
			else
				Assert::IsFalse(new_track_traits.valid(), (L"New track traits must be invalid. Device behind cutter. " + dump_current_state()).c_str());

			/*
				коррекция координаты на начальном пути без смены ориентации и стартовой позиции устройства
			*/
			++step;//4

			device_events_queue.reset();

			device_events_queue.set_cutter_offset(cutter_offset_in_counter);
			device_events_queue.set_begin_path_info(manager_track_traits(start_track_traits), true, device_ahead0);

			event_track_traits = create_track_traits(event_counter, event_coordinate0, direction0, counter_size, line_path_code_t::start_line_path);

			create_right_track_traits = [&](const position_detector::counter32_t& counter_start)
			{
				return create_track_traits(counter_start, event_coordinate0, direction0, counter_size, line_path_code_t::line0_path0);
			};

			new_track_traits = device_events_queue.process_coordinate_correct_event(event_coordinate, cutter_direction, std::move(event_track_traits), pointsInfoChangedNotifyFunc);

			if (device_ahead0)
				Assert::IsTrue(new_track_traits.valid(), (L"New track traits must be valid. " + dump_current_state()).c_str());
			else
				Assert::IsFalse(new_track_traits.valid(), (L"New track traits must be invalid. Device behind cutter. " + dump_current_state()).c_str());


		}

		TEST_METHOD(test_track_events_queue)
		{
			checked_execute([]
			{
				const std::size_t count_iterations = 100;

				for (std::size_t i = 0; i < count_iterations; ++i)
				{
					simple_test_one_event_after_start(generate_value<start_data_t>());
				}

			});
		}



		static void simple_test_some_reverse_and_1_event_after_start(const start_data_t& start_data)
		{
			LOG_STACK();

			const counter32_t counter0{ start_data.counter0 };
			const coordinate_t coordinate0{ start_data.coordinate0 };
			const movment_direction_t direction0{ start_data.direction0 };
			const uint32_t counter_size = start_data.counter_size;
			const coordinate_t device_offset = start_data.device_offset;
			const coordinate_t device_offset_abs = std::abs(start_data.device_offset);
			const position_detector::counter_t cutter_offset_in_counter = calc_cutter_offset(start_data);
			const decltype(manager_track_traits::direction0) direction0_sign = direction0 == movment_direction_t::backward ? -1 : 1;

			const orientation_t orientation0 = start_data.orientation0;
			coordinate_t sign = static_cast<coordinate_t>(orientation0)* direction0_sign;

			const manager_track_traits start_track_traits = create_start_path_track_traits(counter0, coordinate0 + sign*device_offset, direction0, counter_size);
			const coordinate_t device_coordinate0{ start_track_traits.coordinate0 };

			const bool device_ahead0 = is_device_ahead(start_data);

			track_events_queue_t device_events_queue(track_events_queue_t::race_detect_strategy_t::check_only_start_point);
			device_events_queue.set_cutter_offset(cutter_offset_in_counter);
			device_events_queue.set_begin_path_info(manager_track_traits(start_track_traits), true, device_ahead0);

			const coordinate_t cutter_offset = -1 * sign*device_offset;

			auto reverse_events = generate_reverse_events_for_one_path(start_track_traits, device_events_queue, device_ahead0, cutter_offset);

			const auto reverse_events_count = reverse_events.size();

			std::uint8_t step = 1;
			coordinate_t event_coordinate{invalid_coordinate};
			counter32_t event_counter{invalid_counter};
			coordinate_t event_coordinate0 = -5000 * 100 * 10;

			const auto dump_current_state = [&](const manager_track_traits& track_traits)
			{
				std::wstring result{ L"[step " + std::to_wstring(step) + L" {" };
				result += L"event_coordinate: " + std::to_wstring(event_coordinate);
				result += L"\nevent_counter: " + std::to_wstring(event_counter);
				result += L"\ndevice_offset: " + std::to_wstring(device_offset);
				result += L"\norientation0: " + to_wstring(orientation0);
				result += L"}\n";
				result += L"\ncount reverse events: " + std::to_wstring(reverse_events_count);
				result += L"\n" + dump_track_traits(track_traits);
				result += L"]";

				return result;

			};

			const auto dump_tracks_traits = [&](const manager_track_traits& actual_track_traits, const manager_track_traits& expected_track_traits)
			{
				std::wstring result{ L"actual:  " + dump_track_traits(actual_track_traits) };
				result += L"\nexpected: " + dump_track_traits(expected_track_traits);
				return result;
			};

			const auto & last_reverse_event = reverse_events.back().track_traits;

			if ((device_ahead0 && reverse_events_count % 2 != 0) ||
				(!device_ahead0 && reverse_events_count % 2 == 0)
				)
			{
				event_coordinate = last_reverse_event.coordinate0 + last_reverse_event.direction * device_offset_abs;
				event_counter = last_reverse_event.counter0 + static_cast<decltype(event_counter)>(std::abs(event_coordinate - last_reverse_event.coordinate0) / counter_size);
				int32_t cutter_direction = last_reverse_event.direction;

				auto event_track_traits = create_track_traits(event_counter, event_coordinate0, movment_direction_t::forward, counter_size, line_path_code_t::line1_path0);

				auto new_track_traits = 
					device_events_queue.process_change_path_event(
						event_coordinate, 
						cutter_direction,
						manager_track_traits{ event_track_traits },
						[](const position_detector::counter32_t&, const position_detector::counter32_t&, const manager_track_traits&)
				{
					Assert::Fail(L"Could only defer event.");
				});

				Assert::IsFalse(new_track_traits.valid(), (L"New track traits must be invalid. Device behind cutter. " + dump_current_state(last_reverse_event)).c_str());
				return;

			}


			const bool cut_last_reverse_event = false;// any_of_bool();
			int32_t cutter_direction = last_reverse_event.direction;

			if (cut_last_reverse_event)
			{
				event_coordinate = last_reverse_event.coordinate0 + last_reverse_event.direction * device_offset_abs;
			}
			else
			{
				event_coordinate = last_reverse_event.coordinate0 - last_reverse_event.direction * device_offset_abs / 2;
			}

			event_counter = last_reverse_event.counter0 + static_cast<decltype(event_counter)>(std::abs(event_coordinate - last_reverse_event.coordinate0) / counter_size);

			{

				bool result = device_events_queue.process_track(event_coordinate + last_reverse_event.direction * device_offset_abs, event_counter, manager_track_traits(last_reverse_event), event_coordinate, cutter_direction);
				Assert::IsFalse(result, L"No defered events for this point.");
			}

			auto event_track_traits = create_track_traits(event_counter, event_coordinate0, movment_direction_t::forward, counter_size, line_path_code_t::line1_path0);

			std::function<void(const position_detector::counter32_t&, const position_detector::counter32_t&, const manager_track_traits&)> pointsInfoChangedNotifyFunc;
			std::function<manager_track_traits(const position_detector::counter32_t&)> create_right_track_traits;
			std::function<position_detector::counter32_t()> right_counter_start_calc;
			std::function<position_detector::counter32_t()> right_counter_end_calc;


			position_detector::counter32_t right_counter_start{ invalid_counter };
			position_detector::counter32_t right_counter_end{ invalid_counter };
			coordinate_t right_event_coordinate0{ event_coordinate0 };
			bool current_device_ahead{ device_ahead0 };
			auto device_ahead_reverse_event_iter = reverse_events.end();
			auto device_behind_reverse_event_iter = reverse_events.end();
			std::uint32_t notify_counter{ 0 };


			if (cut_last_reverse_event)
			{
				/*
					последнее reverse событие должно быть обрезано, а значит считаем, что после последнего разворота пошел новый путь
				*/
				right_counter_start_calc = [&]
				{
					return event_counter - static_cast<position_detector::counter32_t>(cutter_offset_in_counter);
				};

				right_counter_end_calc = [&]
				{
					return event_counter;
				};

				create_right_track_traits = [&](const position_detector::counter32_t& counter_start)
				{
					return create_track_traits(counter_start, event_coordinate0, movment_direction_t::forward, counter_size, line_path_code_t::line1_path0);
				};

				pointsInfoChangedNotifyFunc = [&](const position_detector::counter32_t& counter_start, const position_detector::counter32_t& counter_end, const manager_track_traits & track_traits)
				{

					position_detector::counter32_t right_counter_start = right_counter_start_calc();
					Assert::AreEqual(counter_start, right_counter_start, (L"Invalid start counter. " + dump_current_state(last_reverse_event)).c_str());
					Assert::AreEqual(counter_end, right_counter_end_calc(), (L"Invalid end counter. " + dump_current_state(last_reverse_event)).c_str());

					const auto & right_track_traits = create_right_track_traits(right_counter_start);

					Assert::IsTrue(compare_track_traits(track_traits, right_track_traits), dump_tracks_traits(track_traits, right_track_traits).c_str());

				};

			}//if (cut_last_reverse_event)
			else
			{
				/*
					последнее reverse событие должно быть, а значит может быть 2 ситуации
					1 - все хорошо, все реверсы укладываются в новый путь
					2 - что-то не так, при каких то реверсах уже должен был смениться путь, но этого не произошло.
						Считаем, что после последнего разворота пошел новый путь.
				*/

				reverse_events.push_front({ start_track_traits, true });

				bool reset_events = false;
				{
					auto reverse_event_iter = reverse_events.begin();
					if (event_coordinate*cutter_direction <= reverse_event_iter->track_traits.coordinate0*cutter_direction)
					{
						reset_events = true;
					}
				}

				//for (auto reverse_event_iter = reverse_events.begin(); reverse_event_iter != --reverse_events.end(); ++reverse_event_iter)
				//{
				//	if (event_coordinate*cutter_direction <= reverse_event_iter->coordinate0*cutter_direction)
				//	{
				//		reset_events = true;
				//		break;
				//	}
				//}

				if (reset_events)
				{// ситуация 2

					create_right_track_traits = [&](const position_detector::counter32_t& counter_start)
					{
						return create_track_traits(counter_start, event_coordinate0 + (event_counter - counter_start) * counter_size, movment_direction_t::forward, counter_size, line_path_code_t::line1_path0);
					};

					pointsInfoChangedNotifyFunc = [&](const position_detector::counter32_t& counter_start, const position_detector::counter32_t& counter_end, const manager_track_traits & track_traits)
					{

						position_detector::counter32_t right_counter_start = last_reverse_event.counter0;
						Assert::AreEqual(counter_start, right_counter_start, (L"Invalid start counter. " + dump_current_state(last_reverse_event)).c_str());
						Assert::AreEqual(counter_end, event_counter, (L"Invalid end counter. " + dump_current_state(last_reverse_event)).c_str());

						const auto & right_track_traits = create_right_track_traits(right_counter_start);

						Assert::IsTrue(compare_track_traits(track_traits, right_track_traits), dump_tracks_traits(track_traits, right_track_traits).c_str());

					};


				}//if (reset_events)
				else
				{// ситуация 1

					{//добавим фиктивные события реверса в начало и конец движения
						//reverse_events.push_front(start_track_traits); добавили ранее
						reverse_events.push_back({ start_track_traits, false });

						if (device_ahead0)
						{
							reverse_events.back().track_traits.direction = -1 * direction0_sign;

						}

						reverse_events.back().track_traits.coordinate0 = event_coordinate;
						reverse_events.back().track_traits.counter0 = event_counter;
					}


					right_counter_start = invalid_counter ;
					right_counter_end = reverse_events.front().track_traits.counter0;
					right_event_coordinate0 = event_coordinate0 ;
					current_device_ahead = device_ahead0 ;


					const auto next_device_ahead_not_cutted_reverse_event = [&](const std::list<track_traits_info_t>::iterator & begin_device_ahead)
					{
						std::list<track_traits_info_t>::iterator result(begin_device_ahead);
						while (result != reverse_events.end())
						{
							if (!result->cutted)
							{
								if (!current_device_ahead)
									++result;
								break;
							}
							++result;
							current_device_ahead = !current_device_ahead;
						}

						return result;
					};

					device_ahead_reverse_event_iter = reverse_events.begin();
					device_behind_reverse_event_iter = reverse_events.begin();

					if (device_ahead0)
						++device_ahead_reverse_event_iter;
					else
						current_device_ahead = !current_device_ahead;

					device_ahead_reverse_event_iter = next_device_ahead_not_cutted_reverse_event(device_ahead_reverse_event_iter);

					const auto next_reverse_event_iter = reverse_events.begin();// next_device_ahead_not_cutted_reverse_event(reverse_events.begin());
					if (next_reverse_event_iter == reverse_events.end())
					{//все реверсы были на старом пути

						pointsInfoChangedNotifyFunc = [&](const position_detector::counter32_t& counter_start, const position_detector::counter32_t& counter_end, const manager_track_traits & track_traits)
						{
							Assert::AreEqual(notify_counter++, std::uint32_t{ 0 });

							right_counter_start = event_track_traits.counter0 - static_cast<position_detector::counter32_t>(cutter_offset_in_counter);
							right_counter_end = event_track_traits.counter0;
							right_event_coordinate0 = event_coordinate0;

							Assert::AreEqual(counter_start, right_counter_start, (L"Invalid start counter. " + dump_current_state(last_reverse_event)).c_str());
							Assert::AreEqual(counter_end, right_counter_end, (L"Invalid end counter. " + dump_current_state(last_reverse_event)).c_str());

							const auto & right_track_traits = create_track_traits(right_counter_start, right_event_coordinate0, movment_direction_t::forward, counter_size, line_path_code_t::line1_path0);;

							Assert::IsTrue(compare_track_traits(track_traits, right_track_traits), dump_tracks_traits(track_traits, right_track_traits).c_str());

						};
					}
					else
					{
						notify_counter = 0;
						pointsInfoChangedNotifyFunc = [&, next_device_ahead_not_cutted_reverse_event](const position_detector::counter32_t& counter_start, const position_detector::counter32_t& counter_end, const manager_track_traits & track_traits)
						{
							Assert::IsFalse(device_behind_reverse_event_iter == reverse_events.end());

							LOG_DEBUG() << L"Notify counter:" << ++notify_counter;

							if (current_device_ahead)
							{
								device_behind_reverse_event_iter = device_ahead_reverse_event_iter;
								--device_behind_reverse_event_iter;

								if (!device_behind_reverse_event_iter->cutted)
								{//реверс при движении устройством позади попадает в новый путь
									right_counter_start = device_behind_reverse_event_iter->track_traits.counter0;
									right_counter_end = device_ahead_reverse_event_iter->track_traits.counter0;
									right_event_coordinate0 = event_coordinate0 + event_track_traits.direction*std::abs(device_behind_reverse_event_iter->track_traits.coordinate0 - event_coordinate);
								}
								else
								{
									right_counter_start = device_ahead_reverse_event_iter->track_traits.counter0 - static_cast<position_detector::counter32_t>(std::abs(device_ahead_reverse_event_iter->track_traits.coordinate0 - event_coordinate) / counter_size);
									right_counter_end = device_ahead_reverse_event_iter->track_traits.counter0;
									right_event_coordinate0 = event_coordinate0;
								}
							}
							else
							{
								device_behind_reverse_event_iter = device_ahead_reverse_event_iter;
								++device_behind_reverse_event_iter;
								if (!device_behind_reverse_event_iter->cutted)
								{//реверс при движении устройством позади попадает в новый путь
									right_counter_end = device_behind_reverse_event_iter->track_traits.counter0;
								}
								else
								{
									right_counter_end = device_ahead_reverse_event_iter->track_traits.counter0 + static_cast<position_detector::counter32_t>(std::abs(device_ahead_reverse_event_iter->track_traits.coordinate0 - event_coordinate) / counter_size);
								}

								right_counter_start = device_ahead_reverse_event_iter->track_traits.counter0;
								right_event_coordinate0 = event_coordinate0 + event_track_traits.direction*std::abs(device_ahead_reverse_event_iter->track_traits.coordinate0 - event_coordinate);
							}

							Assert::AreEqual(counter_start, right_counter_start, (L"Invalid start counter. " + dump_current_state(last_reverse_event)).c_str());
							Assert::AreEqual(counter_end, right_counter_end, (L"Invalid end counter. " + dump_current_state(last_reverse_event)).c_str());

							const auto right_movment_direction = current_device_ahead ? movment_direction_t::forward : movment_direction_t::backward;
							auto right_track_traits = create_track_traits(right_counter_start, right_event_coordinate0, movment_direction_t::forward, counter_size, line_path_code_t::line1_path0);
							right_track_traits.direction = current_device_ahead ? 1 : -1;

							Assert::IsTrue(compare_track_traits(track_traits, right_track_traits), dump_tracks_traits(track_traits, right_track_traits).c_str());

							if (current_device_ahead)
								current_device_ahead = !current_device_ahead;
							else
							{
								if (++device_ahead_reverse_event_iter != reverse_events.end())
								{
									++device_ahead_reverse_event_iter;
								}

								current_device_ahead = !current_device_ahead;
								device_ahead_reverse_event_iter = next_device_ahead_not_cutted_reverse_event(device_ahead_reverse_event_iter);
							}

						};

					}//else if (next_reverse_event_iter == reverse_events.end())
				}//else if (reset_events) ситуация 1
			}//else if (cut_last_reverse_event)


			auto new_track_traits = 
				device_events_queue.process_change_path_event(
					event_coordinate, 
					cutter_direction, 
					manager_track_traits{ event_track_traits },
					pointsInfoChangedNotifyFunc
				);
			if (device_ahead0)
				Assert::IsTrue(new_track_traits.valid(), (L"New track traits must be valid. " + dump_current_state(event_track_traits)).c_str());

		}


		TEST_METHOD(test_track_events_queue_some_reverse_events)
		{
			checked_execute([]
			{
				const std::size_t count_iterations = 100;

				for (std::size_t i = 0; i < count_iterations; ++i)
				{
					const auto start_data = generate_value<start_data_t>();

					for (std::size_t j = 0; j < count_iterations; ++j)
					{
						simple_test_some_reverse_and_1_event_after_start(start_data);
					}
				}

			});
		}


		static void complex_test_1(const start_data_t& start_data)
		{
			LOG_STACK();

			const counter32_t counter0{ start_data.counter0 };
			const coordinate_t coordinate0{ start_data.coordinate0 };
			const movment_direction_t direction0{ start_data.direction0 };
			const uint32_t counter_size = start_data.counter_size;
			const coordinate_t device_offset = start_data.device_offset;
			const coordinate_t device_offset_abs = std::abs(start_data.device_offset);
			const position_detector::counter_t cutter_offset_in_counter = calc_cutter_offset(start_data);
			const decltype(manager_track_traits::direction0) direction0_sign = direction0 == movment_direction_t::backward ? -1 : 1;

			const orientation_t orientation0 = start_data.orientation0;
			coordinate_t sign = static_cast<coordinate_t>(orientation0)* direction0_sign;

			const manager_track_traits start_track_traits = create_start_path_track_traits(counter0, coordinate0 + sign*device_offset, direction0, counter_size);

			const bool device_ahead0 = is_device_ahead(start_data);

			track_events_queue_t device_events_queue;
			device_events_queue.set_cutter_offset(cutter_offset_in_counter);
			device_events_queue.set_begin_path_info(manager_track_traits(start_track_traits), true, device_ahead0);

			std::uint8_t step = 1;

			/*
			просто смена линии
			*/
			coordinate_t event_coordinate = coordinate0 + direction0_sign * device_offset_abs * 2;
			counter32_t event_counter = counter0 + static_cast<decltype(event_counter)>(std::abs(event_coordinate - coordinate0) / counter_size);
			int32_t cutter_direction = start_track_traits.direction;

			coordinate_t event_coordinate0 = -5000 * 100 * 10;

			const coordinate_t line0_to_line1_coordinate = event_coordinate;


			const auto dump_current_state = [&]
			{
				std::wstring result{ L"[step " + std::to_wstring(step) + L" {" };
				result += L"event_coordinate: " + std::to_wstring(event_coordinate);
				result += L"\nevent_counter: " + std::to_wstring(event_counter);
				result += L"}\n" + dump_start_data(start_data);
				result += L"]";

				return result;

			};

			auto event_track_traits = create_track_traits(event_counter, event_coordinate0, movment_direction_t::forward, counter_size, line_path_code_t::line1_path0);

			std::function<position_detector::counter32_t()> right_counter_start_calc = [&]
			{
				return event_counter - static_cast<position_detector::counter32_t>(cutter_offset_in_counter);
			};

			std::function<position_detector::counter32_t()> right_counter_end_calc = [&]
			{
				return event_counter;
			};

			std::function<manager_track_traits(const position_detector::counter32_t&)> create_right_track_traits = [&](const position_detector::counter32_t& counter_start)
			{
				return create_track_traits(counter_start, event_coordinate0, movment_direction_t::forward, counter_size, line_path_code_t::line1_path0);
			};

			const auto pointsInfoChangedNotifyFunc = [&](const position_detector::counter32_t& counter_start, const position_detector::counter32_t& counter_end, const manager_track_traits & track_traits)
			{

				position_detector::counter32_t right_counter_start = right_counter_start_calc();
				Assert::AreEqual(counter_start, right_counter_start, (L"Invalid start counter. " + dump_current_state()).c_str());
				Assert::AreEqual(counter_end, right_counter_end_calc(), (L"Invalid end counter. " + dump_current_state()).c_str());

				const auto & right_track_traits = create_right_track_traits(right_counter_start);

				Assert::IsTrue(compare_track_traits(track_traits, right_track_traits));

			};


			auto new_track_traits = 
				device_events_queue.process_change_path_event(
					event_coordinate, 
					cutter_direction, 
					manager_track_traits{ event_track_traits }, 
					pointsInfoChangedNotifyFunc
				);
			
			if (device_ahead0)
				Assert::IsTrue(new_track_traits.valid(), (L"New track traits must be valid. " + dump_current_state()).c_str());
			else
				Assert::IsFalse(new_track_traits.valid(), (L"New track traits must be invalid. " + dump_current_state()).c_str());


			/*
			коррекция координаты на новой линии со сменой направления движения
			*/


			++step;// 2

			auto movment_span = device_offset_abs / 2;
			event_coordinate = event_coordinate0;
			event_coordinate0 -= movment_span;
			event_coordinate += movment_span;

			event_counter += static_cast<decltype(event_counter)>(std::abs(event_coordinate - event_track_traits.coordinate0) / counter_size);

			event_track_traits = create_track_traits(event_counter, event_coordinate0, movment_direction_t::backward, counter_size, line_path_code_t::line1_path0);

			create_right_track_traits = [&](const position_detector::counter32_t& counter_start)
			{
				return create_track_traits(counter_start, event_coordinate0, movment_direction_t::backward, counter_size, line_path_code_t::line1_path0);
			};


			new_track_traits = 
				device_events_queue.process_coordinate_correct_event(
					event_coordinate, 
					cutter_direction, 
					manager_track_traits{ event_track_traits },
					pointsInfoChangedNotifyFunc
				);
			
			if (device_ahead0)
			{
				Assert::IsTrue(new_track_traits.valid(), (L"New track traits must be valid. " + dump_current_state()).c_str());
			}
			else
			{
				Assert::IsFalse(new_track_traits.valid(), (L"New track traits must be invalid. " + dump_current_state()).c_str());

				const counter32_t line0_to_line1_counter = event_counter + (counter32_t)(cutter_offset_in_counter / 2);

				const auto cutter_coordinate = calculate_coordinate(event_track_traits.coordinate0, event_track_traits.direction*distance_from_counter(line0_to_line1_counter, event_track_traits.counter0, event_track_traits.counter_size));


				Assert::IsTrue(device_events_queue.process_track(line0_to_line1_coordinate, line0_to_line1_counter, new_track_traits, cutter_coordinate, -1), L"Could change line for device at this point");
				Assert::IsTrue(new_track_traits.valid(), (L"New track traits must be valid. " + dump_current_state()).c_str());
				Assert::IsTrue(new_track_traits.compare_path_info(event_track_traits), (L"New track traits must be valid. " + dump_current_state()).c_str());

			}

		}


		TEST_METHOD(test_track_events_queue_complex_test)
		{
			checked_execute([]
			{
				const std::size_t count_iterations = 100;

				for (std::size_t i = 0; i < count_iterations; ++i)
				{
					const auto start_data = generate_value<start_data_t>();

					for (std::size_t j = 0; j < count_iterations; ++j)
					{
						complex_test_1(start_data);
					}
				}

			});
		}


		static void particular_test_1()
		{
			LOG_STACK();

			const counter32_t counter0{ 100000 };
			const coordinate_t coordinate0{ 0 };
			const movment_direction_t direction0{ movment_direction_t::forward };
			const uint32_t counter_size = 1;
			const coordinate_t device_offset = 41000;
			const coordinate_t device_offset_abs = std::abs(device_offset);
			const position_detector::counter_t cutter_offset_in_counter = device_offset_abs / counter_size;
			const decltype(manager_track_traits::direction0) direction0_sign = sign_direction(direction0);

			const orientation_t orientation0 = orientation_t::boiler;
			coordinate_t sign = static_cast<coordinate_t>(orientation0)* direction0_sign;

			const manager_track_traits start_track_traits = create_track_traits(counter0, coordinate0 + sign * device_offset, direction0, counter_size, line_path_code_t::line0_path0);

			const bool device_ahead0 = is_device_ahead(orientation0, device_offset);

			track_events_queue_t device_events_queue;
			device_events_queue.set_cutter_offset(cutter_offset_in_counter);
			device_events_queue.set_begin_path_info(manager_track_traits(start_track_traits), true, device_ahead0);

			std::uint8_t step = 1;

			manager_track_traits device_track_traits = start_track_traits;
			manager_track_traits null_point_track_traits = start_track_traits;
			null_point_track_traits.coordinate0 = coordinate0;

			/*
				реверс
			*/
			counter32_t counters_elapsed = 1000000;

			{
				auto reverse_direction = direction0;
				decltype(manager_track_traits::direction0) reverse_direction_sign = sign_direction(reverse_direction);

				manager_track_traits current_reverse_event{ start_track_traits };

				current_reverse_event.coordinate0 += reverse_direction_sign * static_cast<coordinate_t>(counters_elapsed)*counter_size;
				current_reverse_event.counter0 += counters_elapsed;

				reverse_track(current_reverse_event);

				const coordinate_t cutter_coordinate = current_reverse_event.coordinate0 - sign * device_offset_abs;

				device_events_queue.reverse(current_reverse_event.coordinate0, manager_track_traits{ current_reverse_event });

				null_point_track_traits = device_track_traits = current_reverse_event;
				null_point_track_traits.coordinate0 = cutter_coordinate;

				device_events_queue.process_track(current_reverse_event.coordinate0, current_reverse_event.counter0, current_reverse_event, cutter_coordinate, null_point_track_traits.direction);
			}

			/*
				смена паспорта
			*/

			counters_elapsed = 4000;

			coordinate_t event_coordinate = null_point_track_traits.coordinate0 + null_point_track_traits.direction * static_cast<coordinate_t>(counters_elapsed)*counter_size;
			counter32_t event_counter = null_point_track_traits.counter0 + counters_elapsed;
			int32_t cutter_direction = null_point_track_traits.direction;

			coordinate_t event_coordinate0 = 5000 * 100 * 10;

			const coordinate_t path0_to_path1_coordinate = event_coordinate;

			auto event_track_traits = create_track_traits(event_counter, event_coordinate0, movment_direction_t::backward, counter_size, line_path_code_t::line0_path1);

			auto new_track_traits =
				device_events_queue.process_change_path_event(
					event_coordinate,
					cutter_direction,
					manager_track_traits{ event_track_traits },
					{}
				);


			null_point_track_traits = event_track_traits;

			device_events_queue.process_track(
											event_coordinate, 
											event_counter + (counter32_t)cutter_offset_in_counter,
											device_track_traits, 
											null_point_track_traits.coordinate0 + null_point_track_traits.direction*device_offset_abs,
											null_point_track_traits.direction
			);


			/*
				реверс
			*/

			{
				counters_elapsed = 1000000;
				decltype(manager_track_traits::direction0) reverse_direction_sign = null_point_track_traits.direction;

				manager_track_traits current_reverse_event{ device_track_traits };

				current_reverse_event.coordinate0 += reverse_direction_sign * static_cast<coordinate_t>(counters_elapsed)*counter_size;
				current_reverse_event.counter0 += counters_elapsed;

				reverse_track(current_reverse_event);

				const coordinate_t cutter_coordinate = current_reverse_event.coordinate0 - sign * device_offset_abs;

				device_events_queue.reverse(current_reverse_event.coordinate0, manager_track_traits{ current_reverse_event });

				null_point_track_traits = device_track_traits = current_reverse_event;
				null_point_track_traits.coordinate0 = cutter_coordinate;

				device_events_queue.process_track(current_reverse_event.coordinate0, current_reverse_event.counter0, current_reverse_event, cutter_coordinate, current_reverse_event.direction);
			}



			/*
				смена паспорта
			*/

			counters_elapsed = 2000;

			event_coordinate = null_point_track_traits.coordinate0 + null_point_track_traits.direction * static_cast<coordinate_t>(counters_elapsed)*counter_size;
			event_counter = null_point_track_traits.counter0 + counters_elapsed;
			cutter_direction = null_point_track_traits.direction;

			event_coordinate0 = -1500 * 100 * 10;

			//const coordinate_t path0_to_path1_coordinate = event_coordinate;

			event_track_traits = create_track_traits(event_counter, event_coordinate0, movment_direction_t::forward, counter_size, line_path_code_t::line0_path0);

			const auto pointsInfoChangedNotifyFunc = [&](const position_detector::counter32_t& counter_start, const position_detector::counter32_t& counter_end, const manager_track_traits & /*track_traits*/)
			{

				position_detector::counter32_t right_counter_start = (counter32_t)event_counter - (counter32_t)counters_elapsed;
				Assert::AreEqual(counter_start, right_counter_start, (L"Invalid start counter. "));
				Assert::AreEqual(counter_end, event_counter, (L"Invalid end counter. "));

				//const auto & right_track_traits = create_right_track_traits(right_counter_start);
				//Assert::IsTrue(compare_track_traits(track_traits, right_track_traits));

			};


			new_track_traits =
				device_events_queue.process_change_path_event(
					event_coordinate,
					cutter_direction,
					manager_track_traits{ event_track_traits },
					pointsInfoChangedNotifyFunc
			);


			null_point_track_traits = event_track_traits;

			Assert::IsTrue(new_track_traits.valid());

			device_track_traits = new_track_traits;

			auto res = device_events_queue.process_track(
				event_coordinate0 + device_track_traits.direction * device_offset,
				(position_detector::counter_t)event_counter,
				device_track_traits,
				null_point_track_traits.coordinate0,
				null_point_track_traits.direction
			);


			Assert::IsFalse(res);

		}


		TEST_METHOD(test_track_events_queue_particular_test)
		{
			checked_execute([]
			{
				particular_test_1();
			});
		}

	};
}