// TRWrapper.h: объявление CTRWrapper

#pragma once
#include "resource.h"       // основные символы
#include "ThermoRoutine_i.h"
#include "_ITRWrapperEvents_CP.h"

#include <client_pd_dispatcher\client_pd_dispatcher.h>
#include "irb_frame_helper.h"

#include "irb_grab_frames_dispatcher.h"
#include "irb_frame_image_dispatcher.h"
#include "irb_frames_cache.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Однопотоковые COM-объекты не поддерживаются должным образом платформой Windows CE, например платформами Windows Mobile, в которых не предусмотрена полная поддержка DCOM. Определите _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA, чтобы принудить ATL поддерживать создание однопотоковых COM-объектов и разрешить использование его реализаций однопотоковых COM-объектов. Для потоковой модели в вашем rgs-файле задано значение 'Free', поскольку это единственная потоковая модель, поддерживаемая не-DCOM платформами Windows CE."
#endif

using namespace ATL;


// CTRWrapper
#if !defined TRWRAPPER_INCLUDED

#define TRWRAPPER_INCLUDED
#endif

class ATL_NO_VTABLE CTRWrapper :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CTRWrapper, &CLSID_TRWrapper>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CTRWrapper>,
	public CProxy_ITRWrapperEvents<CTRWrapper>,
	public IDispatchImpl<ITRWrapper, &IID_ITRWrapper, &LIBID_ThermoRoutineLib, /*wMajor =*/ 2, /*wMinor =*/ 4>
{
public:
	CTRWrapper();

	~CTRWrapper();


public:
	BYTE m_tvStatus;
	BYTE grabberReady;

private:
	std::unique_ptr<irb_grab_frames_dispatcher::frames_dispatcher> _grab_frames_dispatcher;
	std::unique_ptr<irb_frame_delegates::irb_frames_cache> _irb_frames_cache;
	irb_frames_cache::irb_frames_cache<3> _extern_irb_frames_cache;
	irb_frame_image_dispatcher::image_dispatcher _image_dispatcher;

	std::vector<irb_frame_helper::frame_id_t> _cached_frame_ids;
	std::wstring _grab_frames_dir;
	std::wstring _grab_frames_file_pattern;
private:
	std::unique_ptr<position_detector::client_pd_dispatcher> _client_pd_dispatcher;
	position_detector::packets_manager_ptr_t _coordinates_manager;
	thread_exception_handler_ptr _thread_exception_handler;
	exception_queue_ptr_t _exception_queue;

	uint16_t _notify_grab_frame_span;
	uint16_t _notify_grab_frame_counter;
	int32_t _camera_offset;
	uint32_t counterSize;

	std::shared_ptr<irb_frame_delegates::irb_frames_writer> _frames_writer;

private:
	void client_pd_dispatcher_error_handler(const std::exception_ptr &exc_ptr);
	void pd_proxy_error_handler(const std::string &error);
	bool process_grabbed_frame(const irb_grab_frames_dispatcher::irb_frame_shared_ptr_t& frame);
	void init_grabber_dispatcher();
	void grabbing_state(bool active);
	void process_new_frame(irb_frame_helper::frame_id_t frame_id);
	void init_pd_objects();
	void close_pd_objects();
	void new_irb_file(const std::wstring & filename);
private:
	bool disable_events;
	bool _is_grabbing;
	video_grabber::IRB_GRABBER_STATE grabber_state;

	irb_frame_helper::frame_id_t _cur_frame_id;


public:
DECLARE_REGISTRY_RESOURCEID(IDR_TRWRAPPER)


BEGIN_COM_MAP(CTRWrapper)
	COM_INTERFACE_ENTRY(ITRWrapper)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CTRWrapper)
	CONNECTION_POINT_ENTRY(__uuidof(_ITRWrapperEvents))
END_CONNECTION_POINT_MAP()

	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	DECLARE_CLASSFACTORY_SINGLETON(CTRWrapper)


public:

	STDMETHOD(ClearGrabbingCache)();

	STDMETHOD(StartRecieveCoordinates)(BSTR pd_ip,
										BSTR pd_i_ip,
										USHORT pd_port,
										BSTR events_pd_ip, 
										BSTR events_i_pd_ip,
										USHORT events_pd_port);
	STDMETHOD(StopRecieveCoordinates)();

	STDMETHOD(GetGrabberSources)(SAFEARRAY **sourcesList);

	STDMETHOD(GetRealTimeFrameRaster)(DWORD frameId,
									irb_frame_info* frame_info,
									VARIANT* pixels,
									VARIANT_BOOL* res);

	STDMETHOD(GetNextRealTimeFrameRaster)(DWORD* frameId, 
									irb_frame_info* frame_info,
									VARIANT* pixels,
									VARIANT_BOOL* res);

	STDMETHOD(GetRealTimeFrame)(DWORD frameId, irb_frame_info* frame_info, VARIANT* pixels, VARIANT* temp_values, VARIANT_BOOL* res);
	STDMETHOD(GetNextRealTimeFrame)(DWORD* frameId, irb_frame_info* frame_info, VARIANT* pixels, VARIANT* temp_values, VARIANT_BOOL* res);
	STDMETHOD(GetFramePositionInfo)(
		ULONG frame_id,
		frame_coordinate *frameCoordinate,
		double* timestamp,
		VARIANT_BOOL* result
		);

	STDMETHOD(GetFrameRawData)(ULONG32 frameId,
		SAFEARRAY** RawData,
		ULONG32* DataSize
		);


	STDMETHOD(get_pixel_temperature)(DWORD frameId, USHORT x, USHORT y, FLOAT* tempToReturn, VARIANT_BOOL* res);

	STDMETHOD(SetPallete)(BSTR palleteFileName);
	STDMETHOD(SetDefaultPallete)(void);
	STDMETHOD(GetPallete)(VARIANT* pallete);
	STDMETHOD(GetPalleteLength)(ULONG32* number_colors, SHORT* len);
	STDMETHOD(SetPaletteCalibrationMode)(calibration_mode mode);
	STDMETHOD(SetPaletteCalibration)(float min, float max);

	STDMETHOD(ConnectCamera)(BYTE initMode, VARIANT_BOOL* res);
	STDMETHOD(DisconnectCamera)(VARIANT_BOOL* res);
	STDMETHOD(StartGrabbing)(void);
	STDMETHOD(StopRecord)(BYTE unload, BYTE save);
	STDMETHOD(StopGrabbing)(BYTE unload, BYTE save);
	STDMETHOD(StartRecord)(void);
	STDMETHOD(FinishAll)(void);


	STDMETHOD(ShowCamSettings)(void);
	STDMETHOD(HideCamSettings)(void);
	STDMETHOD(SetBlockCamFrame)(BYTE blockFlag);
	STDMETHOD(SetGrabberPath)(BSTR grabberPath);


	STDMETHOD(AddArea)(SHORT id, area_info* area);
	STDMETHOD(AreaChanged)(SHORT id, area_info* area);
	STDMETHOD(RemoveArea)(SHORT id, area_type* type);
	STDMETHOD(RemoveAllAreas)(void);
	STDMETHOD(GetAreaInfo)(ULONG aid, area_temperature_measure* measure, VARIANT_BOOL* result);

	STDMETHOD(SetMaxFramesInIRBFile)(USHORT frames_number);
	STDMETHOD(SetCounterSize)(BYTE counterSize);

	STDMETHOD(SetCameraOffset)(LONG32 offset);
	STDMETHOD(FlushGrabbedFramesToTmpFile)();

	STDMETHOD(EnableBadPixelsControl)(VARIANT_BOOL enable, BSTR pixels_settings);

};

OBJECT_ENTRY_AUTO(__uuidof(TRWrapper), CTRWrapper)


