﻿#include "tvcrack.h"

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

std::vector<irb_frames_indexes_span_t> init_indexes_spans(
	irb_files_list_t& irb_frames_streams_list
	)
{
	std::vector<irb_frames_indexes_span_t> result;
	uint32_t first_value = 0;
	uint32_t last_value = 0;
	for (auto stream_iter = irb_frames_streams_list.begin(); stream_iter != irb_frames_streams_list.end();)
	{
		stream_iter->get()->open();
		auto num_frames = stream_iter->get()->count_frames();
		stream_iter->get()->close();

		if (num_frames == 0){
			stream_iter = irb_frames_streams_list.erase(stream_iter);
			continue;
		}
		last_value = first_value + num_frames - 1;
		result.push_back({ first_value, last_value });
		first_value = last_value + 1;
		++stream_iter;

	}

	return result;
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
