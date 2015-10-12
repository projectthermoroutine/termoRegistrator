#pragma once

#include "position_detector_packets_manager.h"
#include <mutex>

namespace position_detector
{

	class track_points_info_manager
	{
	public:
		track_points_info_manager() :_empty(true){}
		~track_points_info_manager() = default;

		void append_point_info(const track_point_info& info) { _locker.lock();  _last_point_info = info; _empty = false; _locker.unlock(); }
		void get_last_point_info(track_point_info& info) const { _locker.lock();  info = _last_point_info;  _locker.unlock(); }
		const track_point_info& back() const { return _last_point_info; }
		track_point_info& back() { return _last_point_info; }

		void lock() const { _locker.lock(); }
		void unlock() const { _locker.unlock(); }

		void clear() { _empty = true; }
		bool empty() const { return _empty; }
	private:

		mutable std::mutex _locker;
		track_point_info _last_point_info;
		bool _empty;
};

}//namespace position_detector

