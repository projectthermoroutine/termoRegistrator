#include <Windows.h>
#include <vector>
#include <memory>
#include <thread>
#include <algorithm>

#include <common\sync_helpers.h>

#include "variocam_grabber.h"
#include "hirbgrabdyn.h"
#include "defines.h"

namespace video_grabber
{
	HMODULE load_library(const char * library_name)
	{
		HMODULE library = 0;
		__try
		{
			library = LoadLibraryA(library_name);
		}
		__except (GetExceptionCode())
		{
			auto last_error = GetLastError();
			throw std::system_error(last_error, std::system_category());
		}

		if ((int)library < 32)
		{
			auto last_error = GetLastError();
			throw std::system_error(last_error, std::system_category());
		}

		return library;
	}
	void free_library(HMODULE library)
	{
		if ((int)library < 32)
		{
			return;
		}
		__try
		{
			FreeLibrary(library);
		}
		__except (GetExceptionCode())
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

		char DLLPath[256];
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
	public:
		grabber_api() :is_api_initialized(false), IRBGrabDLLHandle(0)
		{
			is_api_initialized = InitIrbGrabDLL();
		}
		~grabber_api()
		{
			FreeIrbGrabDLL();
		}
		void Close()
		{
			FreeIrbGrabDLL();
		}
	public:
		inline int GetSources(char* pCharBuffer, UI32* SrcCnt)
		{
			return CALL_ROUTINE(GetSources)(pCharBuffer, SrcCnt);
		}
		inline int  InitSource(const UI32 SrcID)
		{
			return CALL_ROUTINE(InitSource)(SrcID);
		}
		inline int CloseSource(const UI32 SrcID)
		{
			return CALL_ROUTINE(CloseSource)(SrcID);
		}
		inline int AcqInterval_uSecs(const UI32 uSecs)
		{
			return CALL_ROUTINE(AcqInterval_uSecs)(uSecs);
		}
		inline int SendCommand(const UI32 SrcID, const char* cmd, char*  answer, const UI32 timeout)
		{
			return CALL_ROUTINE(SendCommand)(SrcID, cmd, answer, timeout);
		}
		inline int ShowWindow(const UI32 SrcID, const UI32 Mode)
		{
			return CALL_ROUTINE(ShowWindow)(SrcID, Mode);
		}
		inline int GrabIRBLUT(const UI32 SrcID, void* pbuf)
		{
			return CALL_ROUTINE(GrabIRBLUT)(SrcID, pbuf);
		}
		inline int RegisterWndMsgNewPict(const UI32 SrcID, const THandle WndHandle, const DWORD NewPictMsg)
		{
			return CALL_ROUTINE(RegisterWndMsgNewPict)(SrcID, WndHandle, NewPictMsg);
		}
		inline int Grab(TGrabInfoIn& FrameInfoIn, TGrabInfoOut& FrameInfoOut)
		{
			return CALL_ROUTINE(Grab)(FrameInfoIn, FrameInfoOut);
		}
	private:
		bool init_functions()
		{

			auto prevErrorMode = SetErrorMode(SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);

			char dll_name[256];
			auto res = retrieve_full_path_grabber_library_name(dll_name, 256);
			if (!res)
				return false;

			auto library = load_library(dll_name);

			SetErrorMode(prevErrorMode);

			auto GetSources = get_proc_address(library, "irbg_GetSources");
			auto InitSource = get_proc_address(library, "irbg_InitSource");
			auto CloseSource = get_proc_address(library, "irbg_CloseSource");
			auto Grab = get_proc_address(library, "irbg_Grab");
			auto SendCommand = get_proc_address(library, "irbg_SendCommand");
			auto AcqInterval_uSecs = get_proc_address(library, "irbg_AcqInterval_uSecs");
			auto ShowWindow = get_proc_address(library, "irbg_showWindow");
			auto GrabIRBLUT = get_proc_address(library, "irbg_GrabIRBLUT");
			auto RegisterWndMsgNewPict = get_proc_address(library, "irbg_RegisterWndMsgNewPict");

			SET_ROUTINE_PTR(GetSources);
			SET_ROUTINE_PTR(InitSource);
			SET_ROUTINE_PTR(CloseSource);
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
			if (IRBGrabDLLHandle != NULL){
				free_library(IRBGrabDLLHandle);
				IRBGrabDLLHandle = NULL;
			}
			std::memset(&functions, 0, sizeof(TirbgrabFunctions));
		}
		bool InitIrbGrabDLL()
		{
			FreeIrbGrabDLL();
			return init_functions();
		}

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

		Impl() :current_source(-1), FGrabStarted(false), b_stop_grabbing(false), state(IRB_GRABBER_STATE::NONE), _is_connection_active(false)
		{
		}
		~Impl()
		{
			if (_is_connection_active && current_source >= 0)
			{
				api.CloseSource(current_source);
			}
		}

		bool InitializeConection(int SrcId)
		{
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
			stop();
			if (_is_connection_active)
			{
				_is_connection_active = false;
				api.CloseSource(current_source);
				current_source = -1;
			}

			return true;
		}


		void GrabFrames(process_grabbed_frame_func_t process_grabbed_frame_func, active_grab_state_callback_func_t active_grab_state_callback_func)
		{
			active_grab_state_callback_func(false);

			if (!_is_connection_active && !InitializeConection(current_source))
				return;

			TGrabInfoIn FInfoIn;
			TGrabInfoOut FInfoOut;
			FInfoIn.SrcID = current_source;
			FInfoIn.Buf = nullptr;
			FInfoIn.Bufsize = 0;
			FInfoIn.Options = 1;
			FInfoIn.Timeout = 0;
			FInfoIn.DataType = DATATYPE_IRBFRAME;
			int windowCounter = 0;
			// first Grab with FInfoIn.Buf == NULL to get memory size


			if (api.Grab(FInfoIn, FInfoOut) == IRBDLL_RET_ERROR)
			{
				return;
			}

			active_grab_state_callback_func(true);

			std::unique_ptr<BYTE[]> in_buffer(new BYTE[FInfoOut.Bufsize]);
			FInfoIn.Buf = in_buffer.get();
			FInfoIn.Bufsize = FInfoOut.Bufsize;
			while (!b_stop_grabbing)
			{
				// now we grab frames
				auto grabResult = api.Grab(FInfoIn, FInfoOut);

				switch (grabResult)
				{
				case IRBDLL_NO_ERROR:
				{
										process_grabbed_frame_func(FInfoOut.Buf, FInfoOut.Bufsize, reinterpret_cast<irb_spec*>(&FInfoOut.IRBmin), IRB_DATA_TYPE::IRBFRAME);
										break;
				}
				case IRBDLL_RET_ERROR:
				{
										 break;
				}
				case IRBDLL_NO_IMAGE:
				{
										break;
				}

				};

			}//while (!b_stop_grabbing)

			active_grab_state_callback_func(false);
		}
		int StartPreview(process_grabbed_frame_func_t process_grabbed_frame_func, active_grab_state_callback_func_t active_grab_state_callback_func)
		{
			if (_processing_loop_thread.joinable())
			{
				stop();
			}
			b_stop_grabbing = false;
			std::thread processing_loop_thread(
				[this, process_grabbed_frame_func, active_grab_state_callback_func]()
			{ this->GrabFrames(process_grabbed_frame_func, active_grab_state_callback_func); }
			);

			_processing_loop_thread.swap(processing_loop_thread);

			return 1;
		}

		void stop()
		{
			if (b_stop_grabbing)
				return;
			b_stop_grabbing = true;
			if (_processing_loop_thread.joinable())
			{
#ifdef WAIT_THREAD_TERMINATING_ON
				_processing_loop_thread.join();
				//_processing_loop_thread.detach();
#else
				_processing_loop_thread.detach();
#endif
			}


		}
		void Close()
		{
			if (!b_stop_grabbing)
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
			UI32 srccnt;
			int len = api.GetSources(NULL, &srccnt);

			if (len == 0)
			{
				std::vector<std::string>();
			}
			std::unique_ptr<char[]> str_buffer(std::make_unique<char[]>(len + 1));
			char * str = str_buffer.get();

			len = api.GetSources(str, &srccnt);
			if (len == 0)
			{
				std::vector<std::string>();
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
				ii++;
			}
			return res;
		}

	};
	variocam_grabber::variocam_grabber() 
	{
		_p_impl = std::make_unique<Impl>();
	}
	variocam_grabber::~variocam_grabber()
	{
		if (!_p_impl->b_stop_grabbing)
		{
			_p_impl->stop();
		}
		_p_impl->Close();
	}

	int variocam_grabber::Start(process_grabbed_frame_func_t process_grabbed_frame_func, active_grab_state_callback_func_t active_grab_state_callback_func)
	{
		return _p_impl->StartPreview(process_grabbed_frame_func, active_grab_state_callback_func);
	}
	int variocam_grabber::StartPreview(process_grabbed_frame_func_t process_grabbed_frame_func, active_grab_state_callback_func_t active_grab_state_callback_func)
	{
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


	void variocam_grabber::ShowSettings(bool visible)
	{
		_p_impl->api.ShowWindow(_p_impl->current_source, visible ? 1:0);
	}
	void variocam_grabber::Stop(bool unload)
	{
		_p_impl->stop();
		if (unload)
			_p_impl->Close();
	}

	std::vector<std::string> variocam_grabber::get_sources() const
	{
		return _p_impl->get_sources();
	}


	IRB_GRABBER_STATE variocam_grabber::state() const
	{
		return _p_impl->state;
	}
	const std::string& variocam_grabber::last_error() const
	{
		return _p_impl->last_error;
	}


}