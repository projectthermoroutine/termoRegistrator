#pragma once
#include <Windows.h>
#include <memory>
#include <functional>
#include "variocam_grabber.h"
#include "irb_frame_helper.h"
#include "irb_frame_delegates.h"

namespace irb_grab_frames_dispatcher
{
	using namespace video_grabber;

	using irb_frame_shared_ptr_t = std::shared_ptr<irb_frame_helper::IRBFrame>;
	using grabbed_frame_delegate_t = std::function<bool(const irb_grab_frames_dispatcher::irb_frame_shared_ptr_t&)>;

	using grabbing_state_func_t = std::function<void(bool)>;

	class frames_dispatcher final
	{
	public:
		frames_dispatcher();
		~frames_dispatcher();

	public:
		bool init_grabber_connection(int src_id);
		bool close_grabber_connection();
		int start_grabbing(grabbing_state_func_t grabing_state_func);
		int stop_grabbing(bool unload = false);
		void show_settigs(bool visible);
		bool send_camera_command(const std::string& command);
		frames_dispatcher & operator+=(const grabbed_frame_delegate_t& delegate);
//		frames_dispatcher & operator-=(const grabbed_frame_delegate_t& delegate);

		std::vector<std::string> get_grabber_sources() const;


		bool grabber_activity() const;
		std::string last_error() const { return _last_error; }

	private:
		struct Impl;
		std::unique_ptr<Impl> _p_impl;

		mutable std::string _last_error;

	};
}