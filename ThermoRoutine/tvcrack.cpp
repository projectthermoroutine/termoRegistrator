#include "tvcrack.h"

#include <vector>
#include <memory>
#include <algorithm>
#include <utility>


void init_key_spans_and_keys_data(
	const irb_files_list_t& irb_frames_streams_list,
	std::vector<irb_frames_key_span_t> & span_key_list,
	std::vector<irb_frames_key_list_t> & key_list
	)
{
	span_key_list.clear();
	key_list.clear();

	for (auto & stream : irb_frames_streams_list)
	{
		span_key_list.emplace_back(stream->get_frames_key_span());
		key_list.emplace_back(stream->get_frames_key_list());
	}

	std::sort(key_list.begin(), key_list.end(),
		[](const irb_frames_key_list_t& el1, const irb_frames_key_list_t& el2)
	{
		if (el1.size() == 0)
			return false;
		if (el2.size() == 0)
			return true;
		if (el1.back().id < el2.front().id)
			return true;
		return false;
	});
}

void init_indexes_spans(
	irb_files_list_t& irb_frames_streams_list,
	std::vector<irb_frames_indexes_span_t> & span_list
	)
{
	span_list.clear();

	uint32_t first_value = 0;
	uint32_t last_value = 0;
	std::vector<int> delete_indexes;
	int current_index = 0;
	for (auto & stream : irb_frames_streams_list)
	{
		auto num_frames = stream->count_frames();
		if (num_frames == 0){
			delete_indexes.push_back(current_index++);
			continue;
		}
		last_value = first_value + stream->count_frames() - 1;
		span_list.emplace_back(std::make_pair(first_value, last_value));
		first_value = last_value + 1;
		current_index++;
	}

	current_index = 0;
	for (auto index : delete_indexes)
	{
		irb_frames_streams_list.erase(irb_frames_streams_list.begin() + index - current_index++);
	}
}



uint64_t get_key_item_id(const irb_frame_key& key)
{
	return (uint64_t)key.id;
}


uint64_t get_key_item_coordinate(const irb_frame_key& key)
{
	return key.coordinate;
}

double get_key_item_time(const irb_frame_key& key)
{
	return key.time;
}

uint32_t get_key_item_index(const uint32_t & index)
{
	return index;
}
