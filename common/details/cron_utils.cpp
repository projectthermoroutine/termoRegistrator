/////////////////////////////////////
// (C) 2014 ООО "Код Безопасности"
// Проект: SNES-AV eset
// Автор: Зайцев Роман
// Создан: 02.10.2014
// Краткое описание: 
/////////////////////////////////////

#include <common\cron_utils.h>
#include <regex>
#include <cwctype>
#include <loglib\log.h>
#include <numeric>
#include <set>

namespace cron_utils
{
	namespace details
	{
		const int expression_token_count = 5;
		const int expression_minimum_size = expression_token_count*2-1;	//  '* * * * *' - is the shortest form
		const int expression_maximum_size = 10000;

		template<int range_begin, int range_end>
		struct pattern_cell
		{
			static const int min = range_begin;
			static const int max = range_end;

			static_assert(max > min, "max must be greater than min");

			std::vector<int> values;

			void fill_full_range()
			{
				for (int index = min; index <= max; index += 1)
					values.push_back(index);
			}

			int distance(int start, int end, bool wrap, int current_max = max)
			{
				if (start <= end && !wrap)
					return end-start;
				else
					return current_max - start + end - min + 1;
			}

			void fill_range_and_wrap(int start, int ticks)
			{
				if (ticks)
				{
					for (int i = 0; i <= ticks; ++i)
						values.push_back((start - min + i) % (max - min + 1) + min);
					std::sort(std::begin(values), std::end(values));
					values.erase(std::unique(std::begin(values), std::end(values)), std::end(values));
				}
				else
					values.push_back(start);
			}
		};

		using minutes_t = pattern_cell < 0, 59 > ;
		using hours_t = pattern_cell < 0, 23 > ;
		using day_of_month_t = pattern_cell < 1, 31 > ;
		using month_t = pattern_cell < 1, 12 > ;
		using day_of_week_t = pattern_cell < 0, 6 > ;

		struct expression
		{
			minutes_t minutes;
			hours_t hours;
			day_of_month_t day_of_month;
			month_t month;
			day_of_week_t day_of_week;
		};

		template<int range_begin, int range_end>
		void parse_token(const std::wstring& str, pattern_cell<range_begin, range_end>& cell)
		{
			if (!cell.values.empty())
				throw std::logic_error("Cell is not empty");

			if (str != L"*")
			{
				int step = 1;
				const auto slash_iter = std::find(std::cbegin(str), std::cend(str), '/');
				if (slash_iter != std::cend(str))
				{
					step = std::stoi(str.substr(slash_iter + 1 - std::begin(str)));
					if (step < 1)
						throw std::invalid_argument("Invalid step item in cron expression");
				}
				
				{
					const auto range_substr = str.substr(0, slash_iter - std::begin(str));
					const auto regex_str = std::wregex(L"[^,]+");	//  anything between commas
					const auto list_begin = std::wsregex_iterator(std::cbegin(range_substr), std::cend(range_substr), regex_str);
					const auto list_end = std::wsregex_iterator();

					for (auto regex_iter = list_begin; regex_iter != list_end; ++regex_iter)
					{
						const auto list_item = regex_iter->str();

						if (list_item.empty())
							throw std::logic_error("Regex is not well-formed (list item is empty)");

						if (list_item == L"*")
						{
							for (int index = cell.min; index <= cell.max; index += step)
								cell.values.push_back(index);
						}
						else
						{
							const auto checked_value = [&](int value) -> int
							{
								if (value < cell.min || value > cell.max)
									throw std::invalid_argument("Invalid value in cron expression");
								return value;
							};

							const auto hyphen_count = std::count(std::cbegin(list_item), std::cend(list_item), '-');
							if (hyphen_count == 0)
							{
								const auto value = checked_value(std::stoi(list_item));
								cell.values.push_back(value);	//  regular integer
							}
							else
								if (hyphen_count == 1)	//  this is a range
								{
									const auto hypen_iter = std::find(std::cbegin(list_item), std::cend(list_item), '-');
									if (hypen_iter == std::cbegin(list_item) || hypen_iter == std::cend(list_item) - 1)
										throw std::invalid_argument("Invalid list item in cron expression");

									const int range_start = checked_value(std::stoi(list_item.substr(0, hypen_iter - std::begin(list_item))));
									const int range_end = checked_value(std::stoi(list_item.substr(hypen_iter + 1 - std::begin(list_item))));

									for (int i = range_start; i <= range_end; i += step)
										cell.values.push_back(i);
								}
								else
									throw std::invalid_argument("Invalid list item in cron expression");
						}
					}
				}
				std::sort(std::begin(cell.values), std::end(cell.values));
				cell.values.erase(std::unique(std::begin(cell.values), std::end(cell.values)), std::end(cell.values));
			}
			else	//  if '*'
				cell.fill_full_range();
		}

		void check_token_format(const std::wstring& token)
		{
			if (token.empty())
				throw std::logic_error("Regex is not well-formed (token is empty)");

			const auto slash_count = std::count(std::cbegin(token), std::cend(token), '/');
			const auto minus_count = std::count(std::cbegin(token), std::cend(token), '-');
			const auto star_count = std::count(std::cbegin(token), std::cend(token), '*');
			const auto comma_count = std::count(std::cbegin(token), std::cend(token), ',');

			if (slash_count > 1 || minus_count > 60 || star_count > 1 || comma_count > 60)
				throw std::invalid_argument("Invalid cron expression token format");

			if (token[0] == ',' || token[0] == '/' || token[0] == '-')
				throw std::invalid_argument("Invalid cron expression token format");

			const auto last_symbol = token[token.size() - 1];
			if (last_symbol == ',' || last_symbol == '/' || last_symbol == '-')
				throw std::invalid_argument("Invalid cron expression token format");

			for (const auto& ch : token)
			{
				if (!std::iswdigit(ch) && ch != '/' && ch != '-' && ch != '*' && ch != ',') 
					throw std::invalid_argument("Invalid character was found in cron expression");
			}

			if (comma_count && slash_count)
				throw std::invalid_argument("Can't parse cron expression");

			if (star_count)
			{
				if (token.size() > 1)
					if (token[1] != '/')
						throw std::invalid_argument("Invalid step in cron expression");
				if (comma_count > 0)
					throw std::invalid_argument("Invalid step in cron expression");
			}
		}

		expression parse_expression(const std::wstring& str)
		{
			if (str.size() < expression_minimum_size)
				throw std::invalid_argument("Cron expression is too short");

			std::vector<std::wstring> tokens;

			std::wregex tokens_regex(L"\\S+");									//  anything between whitespaces

			const auto begin = std::wsregex_iterator(std::cbegin(str), std::cend(str), tokens_regex);
			const auto end = std::wsregex_iterator();

			for (auto i = begin; i != end; ++i)
			{
				auto token = i->str();
				for (auto& ch : token)
					ch = std::towlower(ch);
								 
				check_token_format(token);
				tokens.push_back(std::move(token));
			}

			if (tokens.size() != expression_token_count)
				throw std::invalid_argument("Invalid token count in cron expression");

			expression result;

			parse_token(tokens[0], result.minutes);
			parse_token(tokens[1], result.hours);
			parse_token(tokens[2], result.day_of_month);
			parse_token(tokens[3], result.month);
			parse_token(tokens[4], result.day_of_week);

			return result;
		}

		int last_day_of_month(int month, int year)
		{
			if (month == 0 || month == 2 || month == 4 || month == 6 || month == 7 || month == 9 || month == 11)
				return 31;
			else if (month == 3 || month == 5 || month == 8 || month == 10)
				return 30;
			else
			{
				if (year % 4 == 0)
				{
					if (year % 100 == 0)
					{
						if (year % 400 == 0)
							return 29;
						return 28;
					}
					return 29;
				}
				return 28;
			}
		}

		void fill_range_and_wrap_month(day_of_month_t& day_of_month_active, int start, int ticks, int month, int year)
		{
			if (ticks)
			{
				int current_max = last_day_of_month(month, year);
				int current_day = start;
				for (int i = 0; i <= ticks; ++i)
				{
					day_of_month_active.values.push_back(current_day);
					if (current_day == current_max)
					{
						++month;
						current_max = last_day_of_month(month, year);
						current_day = day_of_month_active.min;
					}
					else
						++current_day;
				}
				std::sort(std::begin(day_of_month_active.values), std::end(day_of_month_active.values));
				day_of_month_active.values.erase(std::unique(std::begin(day_of_month_active.values), std::end(day_of_month_active.values)), std::end(day_of_month_active.values));
			}
			else
				day_of_month_active.values.push_back(start);
		}
	}

	bool check_time_interval(const time_interval_t& interval)
	{
		LOG_STACK();

		const auto elapsed_seconds = std::difftime(interval.end, interval.begin);
		return elapsed_seconds >= 0;
	}

	bool check_event_has_occurred(const std::wstring& str, const time_interval_t& time_interval)
	{
		LOG_STACK();

		if (!check_time_interval(time_interval))
			throw std::invalid_argument("Check time interval failed");

		if (str.empty())
			throw std::invalid_argument("Cron expression is empty");
		if (str.size() > details::expression_maximum_size)
			throw std::invalid_argument("Cron expression is too long");

		const details::expression plan = details::parse_expression(str);

		std::tm time_begin, time_end;
		{
			const auto time_begin_converted = localtime_s(&time_begin, &time_interval.begin) == 0;
			if (!time_begin_converted)
				throw std::invalid_argument("Failed to convert time begin to std::tm");
			const auto time_end_converted = localtime_s(&time_end, &time_interval.end) == 0;
			if (!time_end_converted)
				throw std::invalid_argument("Failed to convert time end to std::tm");
		}

		//  adjust month to CRON format
		time_begin.tm_mon += details::month_t::min;
		time_end.tm_mon += details::month_t::min;

		details::minutes_t minutes_active;
		details::hours_t hours_active;
		details::day_of_week_t day_of_week_active;
		details::day_of_month_t day_of_month_active;
		details::month_t month_active;

		if (time_end.tm_year - time_begin.tm_year >= 2)
		{
			//  if past more than year, then all month, days, etc. past
			month_active.fill_full_range();
			day_of_month_active.fill_full_range();
			day_of_week_active.fill_full_range();
			hours_active.fill_full_range();
			minutes_active.fill_full_range();
		}
		else
		{
			const auto month_dist = month_active.distance(time_begin.tm_mon, time_end.tm_mon, time_end.tm_year - time_begin.tm_year == 1);
			month_active.fill_range_and_wrap(time_begin.tm_mon, month_dist);
			if (month_dist >= 2)
			{
				//  if past more than month, then all days, hours, etc. past
				day_of_month_active.fill_full_range();
				day_of_week_active.fill_full_range();
				hours_active.fill_full_range();
				minutes_active.fill_full_range();
			}
			else
			{	
				const auto last_day_of_month = details::last_day_of_month(time_begin.tm_mon - 1, time_begin.tm_year + 1900);	//  tm_mon is in CRON format now
				const auto day_of_month_dist = day_of_month_active.distance(time_begin.tm_mday, time_end.tm_mday, month_dist == 1, last_day_of_month);

				fill_range_and_wrap_month(day_of_month_active, time_begin.tm_mday, day_of_month_dist, time_begin.tm_mon - 1, time_begin.tm_year + 1900);

				if (day_of_month_dist >= 7)
				{
					day_of_week_active.fill_full_range();
				}
				else
				{
					const auto day_of_week_dist = day_of_week_active.distance(time_begin.tm_wday, time_end.tm_wday, false);
					day_of_week_active.fill_range_and_wrap(time_begin.tm_wday, day_of_week_dist);
				}

				if (day_of_month_dist >= 2)
				{
					hours_active.fill_full_range();
					minutes_active.fill_full_range();
				}
				else
				{
					const auto hours_dist = hours_active.distance(time_begin.tm_hour, time_end.tm_hour, day_of_month_dist == 1);
					hours_active.fill_range_and_wrap(time_begin.tm_hour, hours_dist);

					if (hours_dist >= 2)
					{
						minutes_active.fill_full_range();
					}
					else
					{
						const auto minutes_dist = minutes_active.distance(time_begin.tm_min, time_end.tm_min, hours_dist == 1);
						minutes_active.fill_range_and_wrap(time_begin.tm_min, minutes_dist);
					}
				}
			}
		}

		std::vector<int> minutes_inters, hours_inters, day_of_month_inters, day_of_week_inters, month_inters;
		std::set_intersection(std::cbegin(plan.minutes.values), std::cend(plan.minutes.values), std::cbegin(minutes_active.values), std::cend(minutes_active.values), std::back_inserter(minutes_inters));
		std::set_intersection(std::cbegin(plan.hours.values), std::cend(plan.hours.values), std::cbegin(hours_active.values), std::cend(hours_active.values), std::back_inserter(hours_inters));
		std::set_intersection(std::cbegin(plan.day_of_month.values), std::cend(plan.day_of_month.values), std::cbegin(day_of_month_active.values), std::cend(day_of_month_active.values), std::back_inserter(day_of_month_inters));
		std::set_intersection(std::cbegin(plan.day_of_week.values), std::cend(plan.day_of_week.values), std::cbegin(day_of_week_active.values), std::cend(day_of_week_active.values), std::back_inserter(day_of_week_inters));
		std::set_intersection(std::cbegin(plan.month.values), std::cend(plan.month.values), std::cbegin(month_active.values), std::cend(month_active.values), std::back_inserter(month_inters));

		return !minutes_inters.empty() && !hours_inters.empty() && !day_of_month_inters.empty() && !day_of_week_inters.empty() && !month_inters.empty();
	}

}