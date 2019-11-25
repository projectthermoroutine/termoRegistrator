#include <Windows.h>
#include <memory>
#include <functional>
#include <mutex>
#include "irb_grab_frames_dispatcher.h"
#include "variocam_grabber.h"
#include "irb_frame_helper.h"
#include <common\sync_helpers.h>
#include <atomic>


namespace irb_grab_frames_dispatcher
{
	using namespace video_grabber;
	using namespace irb_frame_helper;

	using delegate_list_t = std::vector<grabbed_frame_delegate_t>;
	struct frames_dispatcher::Impl
	{
	public:
		Impl(): _enable_correction(false), _correction_factor(0.0), _correction_offset(0.0)
		{
			is_grabber_started = false;
			_state = 0;
		}
	public:
		variocam_grabber grabber;
		delegate_list_t delegates;
		std::mutex _lock;
		std::atomic_bool is_grabber_started;
		grabbing_state_func_t grabbing_state_func;
		volatile long _state;

	private:
		bool _enable_correction;
		float _correction_factor;
		float _correction_offset;

	public:

		void set_correction_temperature_settings(bool enable, float factor, float offset)
		{
			if (enable)
			{
				_correction_factor = factor;
				_correction_offset = offset;
				_enable_correction = enable;
			}
			else
			{
				_enable_correction = enable;
				_correction_factor = factor;
				_correction_offset = offset;
			}
		}

		void active_state_callback(bool state)
		{
			LOG_STACK();
			is_grabber_started.exchange(state);
			grabbing_state_func(state);

		}
		void process_data(const void* data, unsigned int size, const irb_spec * p_irb_spec, IRB_DATA_TYPE type)
		{
			LOG_STACK();

			if (data == nullptr || size == 0)
				return;
			if (type != IRB_DATA_TYPE::IRBFRAME || p_irb_spec == nullptr)
				return;

			if (!_lock.try_lock())
				return;

			std::lock_guard<decltype(_lock)> lk(_lock, std::adopt_lock);

			if (delegates.empty())
				return;

			irb_frame_shared_ptr_t frame;

			try
			{
				frame = std::make_shared<IRBFrame>();

				std::memcpy(&frame->header, data, sizeof(IRBFrameHeader));
				auto pixels_size = frame->get_pixels_count();

				irb_frame_pixels_t pixels(std::make_unique<irb_pixel_t[]>(pixels_size));
				std::memcpy(pixels.get(), (char*)data + sizeof(IRBFrameHeader), pixels_size*sizeof(irb_pixel_t));
				frame->set_pixels(pixels);
			}
			catch (...)
			{
				return;
			}

			IRBSpec irb_spec(p_irb_spec->IRBmin, p_irb_spec->IRBmax, p_irb_spec->IRBavg);

			if(_enable_correction)
			{
				irb_spec.IRBmin = irb_spec.IRBmin * _correction_factor + _correction_offset;
				irb_spec.IRBmax = irb_spec.IRBmax * _correction_factor + _correction_offset;
				irb_spec.IRBavg = irb_spec.IRBavg * _correction_factor + _correction_offset;

				frame->set_correction_temperature_settings(true, _correction_factor, _correction_offset);
			}

			frame->set_spec(irb_spec);

			for (auto & delegate : delegates)
			{
				auto res = delegate(frame);
				if (!res)
					break;
			}
		}

	};

	frames_dispatcher::frames_dispatcher()
	{
		try{
			_p_impl = std::make_unique<Impl>();
		}
		catch (const std::system_error & exc)
		{
			_last_error = exc.code().message();
		}
		catch (const std::exception & exc)
		{
			_last_error = exc.what();
		}
		catch (...)
		{
			_last_error = "Can't initialize grabber api";
		}

	}
	frames_dispatcher::~frames_dispatcher() = default;

	int frames_dispatcher::start_grabbing(grabbing_state_func_t grabbing_state_func, HANDLE h_new_frame_event, const HANDLE hWnd, std::uint32_t new_pic_msg_id)
	{
		LOG_STACK();

		if (!_p_impl)
			return 0;
		if (_p_impl->is_grabber_started == true)
			return 1;

		_p_impl->grabbing_state_func = grabbing_state_func;

		_p_impl->grabber.RegisterWndMsgNewPict(h_new_frame_event, hWnd, new_pic_msg_id);

		_p_impl->grabber.Start(
			std::bind(&frames_dispatcher::Impl::process_data, _p_impl.get(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
			std::bind(&frames_dispatcher::Impl::active_state_callback, _p_impl.get(), std::placeholders::_1)
			);

		return 1;
	}
	int frames_dispatcher::stop_grabbing(bool unload)
	{
		LOG_STACK();

		if (!_p_impl)
			return 0;

		_p_impl->grabber.RegisterWndMsgNewPict(INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE, 0);
		_p_impl->grabber.Stop(unload);
		//_p_impl->grabbing_state_func = nullptr;
		return 1;
	}

	void frames_dispatcher::show_settigs(bool visible)
	{
		LOG_STACK();

		if (!_p_impl)
			return;
		_p_impl->grabber.ShowSettings(visible);
	}

	frames_dispatcher & frames_dispatcher::operator+=(const grabbed_frame_delegate_t& delegate)
	{
		LOG_STACK();

		if (!_p_impl)
			return *this;
		std::lock_guard<decltype(_p_impl->_lock)> lock(_p_impl->_lock);
		_p_impl->delegates.emplace_back(delegate);
		return *this;
	}

	bool frames_dispatcher::grabber_activity() const
	{
		LOG_STACK();

		if (!_p_impl)
			return false;
		return true;
	}

	bool frames_dispatcher::init_grabber_connection(int src_id)
	{
		LOG_STACK();

		if (!_p_impl)
			return false;
		auto res = _p_impl->grabber.init_connection(src_id);
		if (!res){
			_last_error = _p_impl->grabber.last_error();
		}
		return res;
	}
	bool frames_dispatcher::close_grabber_connection()
	{
		LOG_STACK();
		if (!_p_impl)
			return false;
		auto res = _p_impl->grabber.close_connection();
		if (!res){
			_last_error = _p_impl->grabber.last_error();
		}
		return res;
	}
	bool frames_dispatcher::send_camera_command(const std::string& command)
	{
		LOG_STACK();
		if (!_p_impl)
			return false;
		auto res = _p_impl->grabber.send_camera_command(command);
		if (!res){
			_last_error = _p_impl->grabber.last_error();
		}
		return res;
	}


	std::vector<std::string> frames_dispatcher::get_grabber_sources() const
	{
		LOG_STACK();
		if (!_p_impl)
			return std::vector<std::string>();

		auto res = _p_impl->grabber.get_sources();
		if (res.empty())
		{
			_last_error = _p_impl->grabber.last_error();
		}
			
		return res;
	}


	void frames_dispatcher::set_correction_temperature_settings(bool enable, float factor, float offset)
	{
		if (_p_impl)
			_p_impl->set_correction_temperature_settings(enable, factor, offset);
	}


}