#include <Windows.h>
#include <vector>
#include <array>
#include <memory>
#include <thread>
#include <algorithm>
#include <future>
#include <queue>

#include <common\sync_helpers.h>
#include <common\on_exit.h>
#include <common\log_and_throw.h>

#include "variocam_grabber.h"
#include "hirbgrabdyn.h"
#include "defines.h"

namespace video_grabber
{
	const UI32 send_camera_command_timeout = 100;

	HMODULE load_library(const char * library_name)
	{
		HMODULE library = 0;
		__try
		{
			library = LoadLibraryA(library_name);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			auto last_error = GetLastError();
			if (last_error == 0)
				last_error = GetExceptionCode();

			throw std::system_error(last_error, std::system_category());
		}

		if ( library < reinterpret_cast<HMODULE>(32))
		{
			auto last_error = GetLastError();
			throw std::system_error(last_error, std::system_category());
		}

		return library;
	}
	void free_library(HMODULE library)
	{
		if (library < reinterpret_cast<HMODULE>(32))
		{
			return;
		}
		__try
		{
			FreeLibrary(library);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
		}
	}


	void * get_proc_address(HMODULE library, const char * proc_name)
	{
		if (library == 0)
			throw std::invalid_argument("The passed argument library can't be equal to nullptr");

		if (proc_name == nullptr)
			throw std::invalid_argument("The passed argument proc_name can't be equal to nullptr");

		auto proc = GetProcAddress(library, proc_name);
		if (proc == 0)
		{
			auto last_error = GetLastError();
			throw std::system_error(last_error, std::system_category());
		}

		return proc;
	}
	static const char * IRBDLL_NAME = "irbgrab.dll";
	bool retrieve_full_path_grabber_library_name(char* file_name,int size)
	{
		if (size < sizeof(IRBDLL_NAME))
			throw std::out_of_range("Invalide size of the file name destination buffer.");

		char FileName[256];
		strcpy_s(FileName, sizeof(FileName), GetCommandLineA());

		int ii = (int)strlen(FileName);
		if (ii > 4)
		{
			strcpy_s(&FileName[ii - 5], 4, "ini");
		}

		char DLLPath[256] = { 0 };
		char DLLName[256];

		ii = sizeof(DLLName);

		char *reg_key = "Software\\Wow6432Node\\InfraTec\\IRBSDK_v3";

		LONG  regres = RegGetValueA(HKEY_LOCAL_MACHINE, reg_key, "path", RRF_RT_ANY, NULL, DLLPath, (LPDWORD)&ii);

		if (regres != ERROR_SUCCESS)
		{
			reg_key = "Software\\InfraTec\\IRBSDK_v3";
			regres = RegGetValueA(HKEY_LOCAL_MACHINE, reg_key, "path", RRF_RT_ANY, NULL, DLLPath, (LPDWORD)&ii);
		}

		if ((regres != ERROR_SUCCESS) || (strlen(DLLPath) == 0))
		{
			strcpy_s(DLLPath, sizeof(DLLPath), &FileName[1]);
			ii = (int)strlen(DLLPath) - 1;
			while ((ii >= 0) && (DLLPath[ii] != '\\'))
				ii--;
			DLLPath[ii + 1] = 0;
		}
		else
		{
			strcpy_s(DLLPath, sizeof(DLLPath), (char*)(DLLPath + 1));
		}


		GetPrivateProfileStringA("Settings", "DLLPath", DLLPath, DLLPath, sizeof(DLLPath), &FileName[1]);
		ii = (int)strlen(DLLPath);
		if ((ii > 0) && (DLLPath[ii - 1] != '\\'))
		{
			DLLPath[ii - 1] = '\\';
			DLLPath[ii + 0] = 0;
		}

		GetPrivateProfileStringA("Settings", "DLLName", "irbgrab.dll", DLLName, sizeof(DLLName), &FileName[1]);

		strcpy_s(file_name, size, DLLPath);
		ii = (int)strlen(file_name);
		strcpy_s(&file_name[ii], size - ii, DLLName);

		return true;

	}

#define SET_ROUTINE_PTR(_name) functions.##_name = reinterpret_cast<decltype(functions.##_name)>(##_name)
#define CALL_ROUTINE(_name) functions.##_name

	class grabber_api final
	{

	private:

	public:
		grabber_api() 
			: is_api_initialized(false)
			, IRBGrabDLLHandle(0)
		{
			LOG_STACK();


			std::exception_ptr errors;

			InitIrbGrabDLL(errors);

			if (errors)
				std::rethrow_exception(errors);

			is_api_initialized = true;

		}
		~grabber_api()
		{
			LOG_STACK();

			seh_wrapper_invoke(CALL_ROUTINE(CloseDLL));
			FreeIrbGrabDLL();
		}
		void Close()
		{
		}

	public:
		inline int GetSources(char* pCharBuffer, UI32* SrcCnt)
		{
			return seh_wrapper_invoke(CALL_ROUTINE(GetSources), pCharBuffer, SrcCnt);
		}
		inline int  InitSource(const UI32 SrcID)
		{
			return seh_wrapper_invoke(CALL_ROUTINE(InitSource),SrcID);
		}
		inline int CloseSource(const UI32 SrcID)
		{
			return seh_wrapper_invoke(CALL_ROUTINE(CloseSource),SrcID);
		}
		inline int AcqInterval_uSecs(const UI32 uSecs)
		{
			return seh_wrapper_invoke(CALL_ROUTINE(AcqInterval_uSecs),uSecs);
		}
		inline int SendCommand(const UI32 SrcID, const char* cmd, char*  answer, const UI32 timeout)
		{
			return seh_wrapper_invoke(CALL_ROUTINE(SendCommand),SrcID, cmd, answer, timeout);
		}
		inline int ShowWindow(const UI32 SrcID, const UI32 Mode)
		{
			return seh_wrapper_invoke(CALL_ROUTINE(ShowWindow),SrcID, Mode);
		}
		inline int GrabIRBLUT(const UI32 SrcID, void* pbuf)
		{
			return seh_wrapper_invoke(CALL_ROUTINE(GrabIRBLUT),SrcID, pbuf);
		}
		inline int RegisterWndMsgNewPict(const UI32 SrcID, const THandle WndHandle, const DWORD NewPictMsg)
		{
			return seh_wrapper_invoke(CALL_ROUTINE(RegisterWndMsgNewPict),SrcID, WndHandle, NewPictMsg, 0);
		}
		inline int Grab(TGrabInfoIn& FrameInfoIn, TGrabInfoOut& FrameInfoOut)
		{
			return seh_wrapper_invoke(CALL_ROUTINE(Grab),FrameInfoIn, FrameInfoOut);
		}

	private:

		template<typename TCallable, typename... TArgs>
		int seh_wrapper_invoke(const TCallable& func, TArgs&&... args)
		{
			__try
			{
				return func(std::forward<TArgs>(args)...);
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				auto last_error = GetLastError();
				if (last_error == 0)
					last_error = GetExceptionCode();

				throw std::system_error(last_error, std::system_category());
			}

			__assume(false);
		}

	private:
		bool init_functions()
		{
			LOG_STACK();
			HMODULE library{};
			auto prevErrorMode = SetErrorMode(SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);

			{
				ON_EXIT_OF_SCOPE([&] {SetErrorMode(prevErrorMode); });

				char dll_name[256];
				auto res = retrieve_full_path_grabber_library_name(dll_name, 256);
				if (!res)
					return false;

				library = load_library(dll_name);
			}

			auto GetSources  = get_proc_address(library, "irbg_GetSources");
			auto InitSource  = get_proc_address(library, "irbg_InitSource");
			auto CloseSource = get_proc_address(library, "irbg_CloseSource");
			auto CloseDLL    = get_proc_address(library, "irbg_CloseDLL");
			auto Grab        = get_proc_address(library, "irbg_Grab");
			auto SendCommand = get_proc_address(library, "irbg_SendCommand");
			auto ShowWindow  = get_proc_address(library, "irbg_showWindow");
			auto GrabIRBLUT  = get_proc_address(library, "irbg_GrabIRBLUT");

			auto AcqInterval_uSecs     = get_proc_address(library, "irbg_AcqInterval_uSecs");
			auto RegisterWndMsgNewPict = get_proc_address(library, "irbg_RegisterWndMsgNewPict");

			SET_ROUTINE_PTR(GetSources);
			SET_ROUTINE_PTR(InitSource);
			SET_ROUTINE_PTR(CloseSource);
			SET_ROUTINE_PTR(CloseDLL);
			SET_ROUTINE_PTR(Grab);
			SET_ROUTINE_PTR(SendCommand);
			SET_ROUTINE_PTR(AcqInterval_uSecs);
			SET_ROUTINE_PTR(ShowWindow);
			SET_ROUTINE_PTR(GrabIRBLUT);
			SET_ROUTINE_PTR(RegisterWndMsgNewPict);

			IRBGrabDLLHandle = library;

			return true;
		}
		void FreeIrbGrabDLL(void)
		{
			LOG_STACK();
			if (IRBGrabDLLHandle != NULL)
			{
				free_library(IRBGrabDLLHandle);
				IRBGrabDLLHandle = NULL;
			}
			std::memset(&functions, 0, sizeof(TirbgrabFunctions));
		}
		void InitIrbGrabDLL(std::exception_ptr& exceptions)
		{
			LOG_STACK();
			try 
			{
				FreeIrbGrabDLL();
				init_functions();
			}
			catch (...)
			{
				exceptions = std::current_exception();
			}
		}

private:
		HMODULE IRBGrabDLLHandle;
		TirbgrabFunctions	functions;
		bool is_api_initialized;
		std::string error;

	};


	
	struct variocam_grabber::Impl
	{
		grabber_api			api;
		bool				FGrabStarted;
		int					current_source;
		volatile bool		b_stop_grabbing;

		std::thread _processing_loop_thread;

		IRB_GRABBER_STATE	state;
		std::string			last_error;

		bool _is_connection_active;

		HANDLE _h_new_pic_event;
		handle_holder _stop_event;

		Impl() 
			: current_source(-1)
			, FGrabStarted(false)
			, b_stop_grabbing(false)
			, state(IRB_GRABBER_STATE::NONE)
			, _is_connection_active(false)
			, _h_new_pic_event(INVALID_HANDLE_VALUE)
			, _stop_event(sync_helpers::create_basic_manual_reset_event())
		{
		}
		~Impl()
		{
			CloseConnection();
		}

		bool InitializeConection(int SrcId)
		{
			LOG_STACK();
			if (SrcId < 0)
				return false;
			stop();
			if (_is_connection_active && current_source >= 0)
			{
				_is_connection_active = false;
				api.CloseSource(current_source);
			}
			current_source = -1;
			char buf[200];
			int src = SrcId;
			unsigned int count;
			auto res = api.GetSources(buf, &count);
			if (count == 0 || res == 0)
			{
				return false;
			}
			res = api.InitSource(src);
			if (res >= 0)
			{
				current_source = src;
				_is_connection_active = true;
				return true;
			}
			return false;
		}

		bool CloseConnection()
		{
			LOG_STACK();
			stop();
			if (_is_connection_active)
			{
				_is_connection_active = false;
				try{
					api.CloseSource(current_source);
				}
				catch (...)
				{
					last_error = "CloseSource throws exception";
					LOG_WARN() << last_error.c_str();
					current_source = -1;
					return false;

				}
				current_source = -1;
			}

			return true;
		}

		bool send_camera_command(const std::string& command)
		{
			LOG_STACK();
			if (!_is_connection_active || current_source < 0)
			{
				return false;
			}

			LOG_DEBUG() << L"Sending command '" << command.c_str() << L"' to the camera with id: " << current_source;

			std::array<char, MaxAnswerLength + 1> answer = {0};
			const auto res = api.SendCommand(current_source, command.c_str(), answer.data(), send_camera_command_timeout);

			LOG_DEBUG() << L"SendCommand returned " << res << L". Answer: " << answer.data();

			return res > 0 ? true : false;
		}

		bool RegisterWndMsgNewPict(/*const UI32 SrcID, */const HANDLE h_new_pic_event, const HANDLE hWnd, const DWORD NewPictMsg)
		{
			if (current_source < 0 || h_new_pic_event == 0)
				return false;

			//const THandle hWnd = h_new_pic_event == INVALID_HANDLE_VALUE ? (THandle)INVALID_HANDLE_VALUE : (THandle)1;

			const bool result = api.RegisterWndMsgNewPict(current_source, (THandle)hWnd, NewPictMsg) == IRBDLL_NO_ERROR;

			LOG_DEBUG() << L"RegisterWndMsgNewPict result: " << result << L". [src id: " << current_source << L", hWnd: " << hWnd << L", msg id: " << NewPictMsg << L']';

			if (result) 
			{
				if (h_new_pic_event == INVALID_HANDLE_VALUE)
				{
					stop();
				}
				_h_new_pic_event = h_new_pic_event;
			}
			return result;
		}

		void GrabFrames(
			process_grabbed_frame_func_t process_grabbed_frame_func, 
			active_grab_state_callback_func_t active_grab_state_callback_func,
			std::promise<void> & promise
		)
		{
			LOG_STACK();
			std::unique_ptr<BYTE[]> in_buffer;
			TGrabInfoIn FInfoIn;
			TGrabInfoOut FInfoOut;

			try{

				if (!_is_connection_active && !InitializeConection(current_source)){
					LOG_DEBUG() << L"No connection camera.";
					throw std::runtime_error("No connection camera.");
				}

				FInfoIn.SrcID = current_source;
				FInfoIn.Buf = nullptr;
				FInfoIn.Bufsize = 0;
				FInfoIn.Options = 1;
				FInfoIn.Timeout = 0;
				FInfoIn.DataType = DATATYPE_IRBFRAME;
				//int windowCounter = 0;
				// first Grab with FInfoIn.Buf == NULL to get memory size
				if (api.Grab(FInfoIn, FInfoOut) == IRBDLL_RET_ERROR)
				{
					LOG_DEBUG() << L"Grab function failed.";
					throw std::runtime_error("Grab function failed.");
				}


				in_buffer.reset(new BYTE[FInfoOut.Bufsize]);
			}
			catch (...)
			{
				promise.set_exception(std::current_exception());
				return;
			}

			promise.set_value();

			ON_EXIT_OF_SCOPE([&]{ active_grab_state_callback_func(false); });

			active_grab_state_callback_func(true);

			FInfoIn.Buf = in_buffer.get();
			FInfoIn.Bufsize = FInfoOut.Bufsize;
			bool no_image_flag = false;
			std::chrono::steady_clock::time_point start;
			int64_t all_elapsed_sec_no_image = 0;

			const bool use_new_pic_event = _h_new_pic_event != 0 && _h_new_pic_event != INVALID_HANDLE_VALUE;

			while ((!use_new_pic_event && !b_stop_grabbing) || (use_new_pic_event && sync_helpers::wait_any<INFINITE>(_stop_event.get(), _h_new_pic_event) == 1))
			{
				int grabResult;
				try
				{
					grabResult = api.Grab(FInfoIn, FInfoOut);
				}
				catch (...)
				{
					logger::log_current_exception(logger::level::error, L"Frame grabbing failed.");
					b_stop_grabbing = true;
					break;
				}

				switch (grabResult)
				{
				case IRBDLL_NO_ERROR:
				{
					process_grabbed_frame_func(FInfoOut.Buf, FInfoOut.Bufsize, reinterpret_cast<irb_spec*>(&FInfoOut.IRBmin), IRB_DATA_TYPE::IRBFRAME);
					no_image_flag = false;
					break;
				}
				case IRBDLL_NO_IMAGE:
				{
										if (no_image_flag)
										{
											const auto end = std::chrono::steady_clock::now();
											const auto elapsed_sec = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
											if (elapsed_sec >= 1){
												start = end;
												all_elapsed_sec_no_image += elapsed_sec;
												LOG_TRACE() << L"Irb frame grabbing function return NO IMAGE already " << all_elapsed_sec_no_image << L" sec";
											}

										}
										else
										{
											start = std::chrono::steady_clock::now();
											no_image_flag = true;
											all_elapsed_sec_no_image = 0;
											//LOG_TRACE() << L"Irb frame grabbing function return NO IMAGE";
										}

										break;
				}
				default:
				{
					no_image_flag = false;
					LOG_TRACE() << L"Irb frame grabbing function return: " << grabResult;
					break;
				}

				};

			}//while (!b_stop_grabbing)
		}
		int StartPreview(
			process_grabbed_frame_func_t process_grabbed_frame_func,
			active_grab_state_callback_func_t active_grab_state_callback_func
		)
		{
			LOG_STACK();
			stop();
			b_stop_grabbing = false;
			sync_helpers::reset_event(_stop_event);

			std::promise<void> promise;
			auto result = promise.get_future();
			std::thread processing_loop_thread(
				[&]()
			{ this->GrabFrames(process_grabbed_frame_func, active_grab_state_callback_func, promise); }
			);

			_processing_loop_thread.swap(processing_loop_thread);

			try{
				result.get();
			}
			catch (...)
			{
				return 0;
			}

			return 1;
		}

		void stop()
		{
			//if (b_stop_grabbing)
			//	return;
			b_stop_grabbing = true;
			sync_helpers::set_event(_stop_event);

			if (_processing_loop_thread.joinable())
			{
				_processing_loop_thread.join();
			}
		}
		void Close()
		{
			LOG_STACK();
			stop();

			if (_is_connection_active && current_source >= 0)
			{
				api.CloseSource(current_source);
				_is_connection_active = false;
				current_source = -1;
			}

			api.Close();
		}
		std::vector<std::string> get_sources()
		{
			LOG_STACK();
			UI32 srccnt{};
			int len = api.GetSources(NULL, &srccnt);

			if (len == 0)
			{
				LOG_DEBUG() << L"GetSources(NULL, &srccnt) returned 0. [srccnt: " << srccnt << L"]";
				LOG_DEBUG() << L"Try get sources after small delay. [delay: 1s";

				std::this_thread::sleep_for(std::chrono::seconds(1));

				len = api.GetSources(NULL, &srccnt);
				if (len == 0)
				{
					LOG_DEBUG() << L"GetSources(NULL, &srccnt) returned 0. [srccnt: " << srccnt << L"]";
					return{};
				}
			}
			std::unique_ptr<char[]> str_buffer(std::make_unique<char[]>(len + 1));
			char * str = str_buffer.get();

			len = api.GetSources(str, &srccnt);
			if (len == 0)
			{
				LOG_DEBUG() << L"GetSources(str, &srccnt) returned 0. [srccnt: " << srccnt << L"]";
				return{};
			}

			int ii = 0;
			int kk = 0;
			std::vector<std::string> res;
			for (;;)
			{
				if (*(str + ii) == 13)
				{
					*(str + ii) = 0;
					res.push_back(str + kk);
				}
				else if (*(str + ii) == 10)
				{
					kk = ii + 1;
				}
				else if (*(str + ii) == 0)
				{
					res.push_back(str + kk);
					break;
				}
				++ii;
			}

			for(const auto & source : res)
				LOG_INFO() << L"Source: " << source;

			return res;
		}

	};
	variocam_grabber::variocam_grabber() 
	{
		LOG_STACK();
		_p_impl = std::make_unique<Impl>();
	}
	variocam_grabber::~variocam_grabber()
	{
		LOG_STACK();
		if (!_p_impl->b_stop_grabbing)
		{
			_p_impl->stop();
		}
		_p_impl->Close();
	}

	int variocam_grabber::Start(process_grabbed_frame_func_t process_grabbed_frame_func, active_grab_state_callback_func_t active_grab_state_callback_func)
	{
		LOG_STACK();
		return _p_impl->StartPreview(process_grabbed_frame_func, active_grab_state_callback_func);
	}
	int variocam_grabber::StartPreview(process_grabbed_frame_func_t process_grabbed_frame_func, active_grab_state_callback_func_t active_grab_state_callback_func)
	{
		LOG_STACK();
		return _p_impl->StartPreview(process_grabbed_frame_func, active_grab_state_callback_func);
	}
	bool variocam_grabber::init_connection(int src_id)
	{
		return _p_impl->InitializeConection(src_id);
	}
	bool variocam_grabber::close_connection()
	{
		return _p_impl->CloseConnection();
	}
	
	bool variocam_grabber::send_camera_command(const std::string& command)
	{
		return _p_impl->send_camera_command(command);
		}

	void variocam_grabber::ShowSettings(bool visible)
	{
		LOG_STACK();
		_p_impl->api.ShowWindow(_p_impl->current_source, visible ? 1:0);
	}
	void variocam_grabber::Stop(bool unload)
	{
		LOG_STACK();
		_p_impl->stop();
		if (unload)
			_p_impl->Close();
	}

	std::vector<std::string> variocam_grabber::get_sources() const
	{
		LOG_STACK();
		return _p_impl->get_sources();
	}

	bool variocam_grabber::RegisterWndMsgNewPict(/*const std::uint32_t SrcID, */const HANDLE h_new_pic_event, const HANDLE hWnd, const DWORD NewPictMsg)
	{
		LOG_STACK();
		return _p_impl->RegisterWndMsgNewPict(/*SrcID, */h_new_pic_event, hWnd, NewPictMsg);
	}

	IRB_GRABBER_STATE variocam_grabber::state() const
	{
		LOG_STACK();
		return _p_impl->state;
	}
	std::string variocam_grabber::last_error() const
	{
		LOG_STACK();
		return _p_impl->last_error;
	}


}