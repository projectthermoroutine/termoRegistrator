#include <Windows.h>
#include <vector>
#include <memory>
#include <forward_list>

#include "irb_files_patcher.h"
#include "irb_file_helper.h"
#include <common\on_exit.h>

namespace irb_files_patcher
{

	using counters_interval_t = irb_frame_delegates::counter_interval_t;
	using file_name_t = std::wstring;
	using coordinate_calculator = position_detector::coordinate_calculator_ptr_t;
	const position_detector::counter32_t invalid_counter{ 0 };
	const std::pair<position_detector::counter32_t, position_detector::counter32_t> invalid_counters_interval(invalid_counter, invalid_counter);

	const uint32_t max_count_buffered_irb_frames = 200;


	namespace details
	{
		using namespace position_detector;
		using namespace irb_frame_helper;
		path_info_ptr_t retrieve_path_info(const FrameCoord & frame_coord)
		{
			path_info_ptr_t path_info_ = std::make_shared<path_info>();

			path_info_->line = frame_coord.line;
			path_info_->railway = frame_coord.railway;
			path_info_->path = frame_coord.path;
			path_info_->path_name = frame_coord.path_name;
			path_info_->path_type = static_cast<decltype(path_info_->path_type)>(frame_coord.path_type);

			return path_info_;
		}

		void retrieve_path_info(FrameCoord & frame_coord, const track_point_info& info)
		{
			frame_coord.coordinate = info.coordinate;
			frame_coord.railway = info._path_info->railway;
			frame_coord.line = info._path_info->line;
			frame_coord.path = info._path_info->path;
			frame_coord.path_name = info._path_info->path_name;
			frame_coord.path_type = static_cast<decltype(frame_coord.path_type)>(info._path_info->path_type);
			frame_coord.direction = info._path_info->direction;
			frame_coord.counter = info.counter;
			frame_coord.picket = info.picket;
			frame_coord.offset = info.offset;
			frame_coord.counter_size = info.counter_size;
		}

		void trim_invalid_frames_infos(irb_frame_delegates::irb_frames_infos_t& info)
		{

			for (auto iter = info.begin(); iter != info.end();)
			{

				if (iter->key == invalid_counter){
					iter = info.erase(iter);
					continue;
				}
				++iter;

			}

		}

	}//namespace details

	struct irb_files_patcher::Impl
	{

		std::atomic<char> busy;

		Impl() :busy(0)
		{}

		~Impl()
		{
			if (busy.load() > 0){
				LOG_FATAL() << L"Try to remove the object class when performing other operations with them.";
				std::terminate();
			}

		}


		struct recalc_info_t
		{
			counters_interval_t counters;
			counters_interval_t processed_counters;
			coordinate_calculator calculator;

			recalc_info_t(const recalc_info_t&) = delete;
			recalc_info_t(recalc_info_t&& other) :counters(std::move(other.counters)), processed_counters(std::move(other.processed_counters)), calculator(std::move(other.calculator))
			{}
			recalc_info_t(counters_interval_t&& _counters, coordinate_calculator&& _calculator) :counters(std::move(_counters)), processed_counters(invalid_counters_interval), calculator(std::move(_calculator))
			{}
		};


		std::list<recalc_info_t> _recalc_info_list;
		std::map<file_name_t, counters_interval_t> map_file_to_frames_info;
		using file_info_t = std::pair<file_name_t, irb_frame_delegates::irb_frames_infos_t>;
		
		std::map<counters_interval_t, file_info_t> map_counters_to_file;
		std::mutex _recalc_info_guard;
		std::mutex _guard;

		void irb_file_changed(irb_frame_delegates::irb_frames_infos_t&& info, const std::wstring & filename)
		{
			LOG_STACK();

			++busy;
			ON_EXIT_OF_SCOPE([&]{--busy; });

			using namespace irb_frame_delegates;
			if (filename.empty())
				return;

			details::trim_invalid_frames_infos(info);

			if (!std::is_sorted(info.cbegin(), info.cend()))
			{
				LOG_WARN() << "Couldn't process irb frames infos for the file, infos not sorted. File name " << filename <<
					", infos size after trim: " << std::to_wstring(info.size());

				return;
			}


			counters_interval_t new_interval{ info.front().key, info.back().key };
			irb_frames_infos_t new_info{ std::move(info) };


			auto iter = map_file_to_frames_info.find(filename);
			if (iter == map_file_to_frames_info.end()){
				map_file_to_frames_info.emplace(filename, new_interval);
			}
			else{
				const auto & prev_counters_interval = iter->second;

				if (prev_counters_interval >= new_interval){
					LOG_WARN() << "Couldn't process irb frames infos for the file, infos counters invalid. File name " << filename <<
						", prev counters interval: [" << std::to_wstring(prev_counters_interval.first) <<
						", " << std::to_wstring(prev_counters_interval.second) <<
						"], additional counters interval: [" << std::to_wstring(new_interval.first) <<
						", " << std::to_wstring(new_interval.second) << "]";
					return;

				}

				new_interval.first = prev_counters_interval.first;

				{
					std::lock_guard<decltype(_guard)> lock_map(_guard);
					irb_frames_infos_t & prev_info = map_counters_to_file[iter->second].second;
					prev_info.splice(prev_info.end(), std::move(new_info));
					new_info = std::move(prev_info);

					map_counters_to_file.erase(iter->second);
				}
				iter->second = new_interval;
			}

			{
				std::lock_guard<decltype(_guard)> lock_map(_guard);
				map_counters_to_file.emplace(new_interval, std::make_pair(filename, std::move(new_info)));
			}
	
			recalc();
		}

		void recalc_coordinate(const position_detector::counter32_t& start_counter, const position_detector::counter32_t& end_counter, coordinate_calculator&& calculator_ptr)
		{
			LOG_STACK();

			++busy;
			ON_EXIT_OF_SCOPE([&]{--busy; });

			if (!calculator_ptr)
				return;

			if (std::make_pair(start_counter, end_counter) == invalid_counters_interval)
				return;
			{
				std::lock_guard<decltype(_recalc_info_guard)> lock(_recalc_info_guard);
				_recalc_info_list.emplace_back(counters_interval_t{ start_counter, end_counter }, std::move(calculator_ptr));
			}

			recalc();
		}

		void recalc()
		{
			LOG_STACK();

			std::lock_guard<decltype(_recalc_info_guard)> lock(_recalc_info_guard);

			for (auto iter = _recalc_info_list.begin(); iter != _recalc_info_list.end();)
			{
				auto & processed_interval = iter->processed_counters;
				auto search_interval = iter->counters;
				if (processed_interval != invalid_counters_interval)
				{
					search_interval.first = processed_interval.second + 1;
					search_interval.second = iter->counters.second;
				}

				auto search_iter = map_counters_to_file.lower_bound({ search_interval.first, search_interval.first });
				if (search_iter == map_counters_to_file.end())
					continue;

				auto processing_interval = search_interval;

				for (auto interval_iter = search_iter; interval_iter != map_counters_to_file.end(); ++interval_iter)
				{
					if (interval_iter->first.second < search_interval.second)
						processing_interval.second = interval_iter->first.second;

					const auto last_processed_counter = recalc(processing_interval, interval_iter->second, iter->calculator);
					//					if (last_processed_counter != invalid_counter)
					{
						if (processed_interval.first == invalid_counter)
							processed_interval.first = processing_interval.first;

						processed_interval.second = last_processed_counter;

						if (last_processed_counter < processing_interval.second)
						{
							LOG_WARN() << "Couldn't find irb frame in the file. File name " << interval_iter->second.first <<
								", frame counters interval: [" << std::to_wstring(interval_iter->first.first) <<
								", " << std::to_wstring(interval_iter->first.second) <<
								"], processed interval: [" << std::to_wstring(processed_interval.first) <<
								", " << std::to_wstring(processed_interval.second) << "]";

							processed_interval.second = processing_interval.second;

						}

						if (iter->processed_counters == invalid_counters_interval){
							iter->processed_counters = processed_interval;
						}

						iter->processed_counters.second = processed_interval.second;
						if (processed_interval.second == search_interval.second)
							break;

						processing_interval.first = processed_interval.second + 1;
						processing_interval.second = search_interval.second;

					}


					if (iter->counters == iter->processed_counters){

						iter = _recalc_info_list.erase(iter);
						continue;
					}
					++iter;
				}//for (auto interval_iter = search_iter; interval_iter != map_counters_to_file.end(); ++interval_iter)
			}//for (auto iter = recalc_info_list.begin(); iter != recalc_info_list.end();)


		}

		position_detector::counter32_t recalc(const counters_interval_t& interval, const file_info_t& file_info, const coordinate_calculator& calculator)
		{
			LOG_STACK();

			using namespace irb_file_helper;

			std::unique_ptr<IRBFile> irb_file;
			const auto & file_name = file_info.first;

			try{
				irb_file = std::make_unique<IRBFile>(file_name);
				irb_file->open();
			}
			catch (const irb_file_exception& exc)
			{
				LOG_WARN() << "Couldn't open irb file: " << file_name << ". Error: " << exc.what();
				return invalid_counter;
			}


			struct frame_info_t
			{
				uint32_t index;
				irb_frame_shared_ptr_t frame;
			};

			std::vector<frame_info_t> processed_frames;

			const auto write_frames_functor = [&]
			{
				bool write_error = false;
				for (const auto & item : processed_frames)
				{
					try{
						irb_file->write_frame_by_index(item.index, *item.frame.get());
					}
					catch (const irb_file_exception& exc)
					{
						LOG_WARN() << "Couldn't write irb frame to the file. File name " << file_name << ", frame index: " << std::to_wstring(item.index) << ". Error: " << exc.what();
						if (write_error){

							LOG_WARN() << "Couldn't write irb frames to the file. File name " << file_name << ". Error: " << exc.what();
							break;
						}
						write_error = true;
					}
				}

				processed_frames.clear();
			};


			const auto frames_count = irb_file->count_frames();
			auto current_counter = invalid_counter;
			for (const auto & item : file_info.second)
			{
				const auto &frame_index = item.index;
				const auto &current_frame_counter = item.key;

				if (current_frame_counter == invalid_counter)
					continue;

				if (current_frame_counter >= interval.first &&
					current_frame_counter <= interval.second)
				{

					auto frame = irb_file->read_frame_by_index(frame_index);

					if (frame->coords.counter != current_frame_counter){

						LOG_WARN() << "Couldn't find irb frame in the file. File name " << file_name <<
							", frame index: " << std::to_wstring(frame_index) <<
							", expected frame counter: " << std::to_wstring(current_frame_counter) <<
							", actual frame counter: " << std::to_wstring(frame->coords.counter);

						continue;
					}

					current_counter = current_frame_counter;

					auto & frame_coordinate = frame->coords;

					using namespace position_detector;
					track_point_info data;

					data.counter = frame_coordinate.counter;
					data.counter_size = frame_coordinate.counter_size;
					data.coordinate = frame_coordinate.coordinate;
					data.valid = true;

					data._path_info = details::retrieve_path_info(frame_coordinate);

					calculator->calculate(data);

					details::retrieve_path_info(frame_coordinate, data);

					//frame->coords = frame_coordinate;

					processed_frames.push_back({ frame_index, irb_frame_shared_ptr_t(std::move(frame)) });

					if (processed_frames.size() >= max_count_buffered_irb_frames)
					{
						write_frames_functor();
					}
				}

				if (current_frame_counter > interval.second)
					break;
			}

			write_frames_functor();

			return current_counter;
		}

	};


	irb_files_patcher::irb_files_patcher() :
		_p_impl(std::make_unique<Impl>())
	{
		LOG_STACK();
	}
	irb_files_patcher::~irb_files_patcher()
	{
		LOG_STACK();
	}

	void irb_files_patcher::new_irb_file(const std::wstring & /*filename*/)
	{
		LOG_STACK();
	}
	void irb_files_patcher::irb_file_changed(irb_frame_delegates::irb_frames_infos_t&& info, const std::wstring & filename)
	{
		LOG_STACK();

		_p_impl->irb_file_changed(std::move(info), filename);
	}

	void irb_files_patcher::recalc_coordinate(const position_detector::counter32_t& start, const position_detector::counter32_t& end, position_detector::coordinate_calculator_ptr_t&& calculator)
	{
		LOG_STACK();
		_p_impl->recalc_coordinate(start, end, std::move(calculator));
	}


}
