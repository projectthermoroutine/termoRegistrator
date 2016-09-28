#pragma once
#include <Windows.h>
#include <memory>
#include <functional>
#include <vector>

namespace video_grabber
{

	enum class IRB_GRABBER_STATE
	{
		NONE = 0,
		INITED = 1, 
		SRC_CONNECTED = 2,
		GRABBING = 3,
		ERROR_ = 4
	};

	enum class IRB_DATA_TYPE
	{
		NONE = 0,
		BITMAP = 1,    // complete bitmap inclusive header (54 Byte)
		BITMAP32DATA = 2,    // bitmap pixel data (32Bit color) without header
		IRBFRAME = 3,    // IRB-Frame with header
		IRBDATA = 4,    // IRB-data (16Bit per pixel digital value ) without header
		TEMPERATURES = 5,    // temperature-Data (Float-Values)
		_8BITDATA = 6,    // 8Bit-Data
		TEMPERATUREFRAME = 7    // IRBHeader (1728 Byte) + Temperature-Data (Single-Values)
	};

#pragma pack(push,1)
	typedef struct _irb_spec{
		float IRBmin;
		float IRBavg;
		float IRBmax;
	} irb_spec;

#pragma pack(pop)
	using process_grabbed_frame_func_t = std::function<void(const void*, unsigned int, const irb_spec*,IRB_DATA_TYPE)>;
	using active_grab_state_callback_func_t = std::function<void(bool)>;

	class variocam_grabber final
	{
	public:
		variocam_grabber();
		~variocam_grabber();

	public:
		bool init_connection(int src_id);
		bool close_connection();

		int Start(process_grabbed_frame_func_t process_grabbed_frame_func, active_grab_state_callback_func_t active_grab_state_callback_func);
		int StartPreview(process_grabbed_frame_func_t process_grabbed_frame_func, active_grab_state_callback_func_t active_grab_state_callback_func);
		void ShowSettings(bool visible);
		void Stop(bool unload = false);

		bool send_camera_command(const std::string& command);
		std::vector<std::string> get_sources() const;

		IRB_GRABBER_STATE state() const;
		const std::string& last_error() const;

	private:
		struct Impl;
		std::unique_ptr<Impl> _p_impl;

	};
}