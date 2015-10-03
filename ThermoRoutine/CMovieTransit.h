// TRWrapper.h: объявление CTRWrapper

#pragma once
#include "resource.h"       // основные символы
#include "ThermoRoutine_i.h"
#include "_IThermoLibErrorEvents_CP.h"

#include "movie_transit.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Однопотоковые COM-объекты не поддерживаются должным образом платформой Windows CE, например платформами Windows Mobile, в которых не предусмотрена полная поддержка DCOM. Определите _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA, чтобы принудить ATL поддерживать создание однопотоковых COM-объектов и разрешить использование его реализаций однопотоковых COM-объектов. Для потоковой модели в вашем rgs-файле задано значение 'Free', поскольку это единственная потоковая модель, поддерживаемая не-DCOM платформами Windows CE."
#endif

using namespace ATL;

// CTRWrapper
#if !defined MOVIETRANSIT_INCLUDED

#define MOVIETRANSIT_INCLUDED
#endif

class ATL_NO_VTABLE CMovieTransit :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMovieTransit, &CLSID_MovieTransit>,
	public IConnectionPointContainerImpl<CMovieTransit>,
	public CProxy_IThermoLibErrorEvents<CMovieTransit>,
	public IDispatchImpl<IMovieTransit, &IID_IMovieTransit, &LIBID_ThermoRoutineLib, /*wMajor =*/ 2, /*wMinor =*/ 3>
{
public:
	CMovieTransit();
	~CMovieTransit();

private:
	std::unique_ptr<movie_transit_ns::movie_transit> _movie_transit;

private:
	bool disable_events;

	irb_frame_helper::frame_id_t _cur_frame_id;
	irb_frame_helper::camera_offset_t _camera_offset;
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_MOVIETRANSIT)


	BEGIN_COM_MAP(CMovieTransit)
		COM_INTERFACE_ENTRY(IMovieTransit)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CMovieTransit)
		CONNECTION_POINT_ENTRY(__uuidof(_IThermoLibErrorEvents))
	END_CONNECTION_POINT_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

	STDMETHOD(GetFramePositionInfo)(ULONG frame_id, frame_coordinate *frameCoordinate, double* timestamp, VARIANT_BOOL* result);
	STDMETHOD(GetCurrentFramePositionInfo)(frame_coordinate *frameCoordinate, double* timestamp);

	STDMETHOD(GetAreasInfo)(area_temperature_measure_result **results, ULONG size);

	STDMETHOD(GetAreaInfo)(ULONG aid, area_temperature_measure* measure, VARIANT_BOOL* result);

	STDMETHOD(ClearMovieTransitCache)();

	STDMETHOD(SetIRBFiles)(VARIANT filesNames, SAFEARRAY **errors, VARIANT_BOOL* result);
	STDMETHOD(FilesCount)(SHORT* filesCount);
	STDMETHOD(SetPallete)(BSTR palleteFileName);
	STDMETHOD(SetDefaultPallete)(void);
	STDMETHOD(GetPallete)(VARIANT* pallete);
	STDMETHOD(GetPalleteLength)(ULONG32* number_colors, SHORT* len);
	STDMETHOD(SetPaletteCalibrationMode)(calibration_mode mode);
	STDMETHOD(SetPaletteCalibration)(float min, float max);




	STDMETHOD(SaveCurrentFrame)(BSTR path, VARIANT_BOOL* result);
	STDMETHOD(SaveFrame)(ULONG index, BSTR deviceName, ULONG picket, ULONG offset, BSTR filename, VARIANT_BOOL* result);
	STDMETHOD(SaveIrbFrames)(VARIANT framesIndexes, BSTR fileNamePattern, USHORT framesPerFile, VARIANT_BOOL* result);

	STDMETHOD(Close)(void);

	STDMETHOD(AddArea)(SHORT id, area_info* area);
	STDMETHOD(AreaChanged)(SHORT id, area_info* area);
	STDMETHOD(RemoveArea)(SHORT id, area_type* type);
	STDMETHOD(RemoveAllAreas)(void);

	STDMETHOD(GetCurFrameTemperatures)(temperature_measure* measure);

	STDMETHOD(GetTimeString)(BSTR* timeString);
	STDMETHOD(GetDateTimeString)(BSTR* dateTimeString);

	STDMETHOD(InitFrameFilter)(FLOAT* timeFrom, FLOAT* timeTo, FLOAT* tempObj, FLOAT* tempArea, SHORT* pickFrom, SHORT* pickTo, LONG* flags, VARIANT_BOOL* checkedOnly, VARIANT_BOOL* result);
	STDMETHOD(SetFrameFilter)(FLOAT timeFrom, FLOAT timeTo, FLOAT tempObj, FLOAT tempArea, SHORT picketFrom, SHORT picketTo, LONG flags, VARIANT_BOOL* res);
	STDMETHOD(IsFrameMeetFilter)(ULONG32 frameNum, VARIANT_BOOL* result);

	STDMETHOD(FramesCount)(LONG* framesCount);
	STDMETHOD(GetCurrentFrameRaster)(VARIANT* raster, irb_frame_info* frame_info, VARIANT_BOOL* res);
	STDMETHOD(GetFrameRaster)(ULONG32 frameNum,
							irb_frame_info* frame_info,
							VARIANT* frameRaster,
							VARIANT_BOOL* res
							);
	STDMETHOD(GetFrame)(ULONG32 frameNum,
						irb_frame_info* frame_info,
						VARIANT* frameRaster,
						VARIANT* temp_values, 
						VARIANT_BOOL* res
						);

	STDMETHOD(get_pixel_temperature)(DWORD frameIndex, USHORT x, USHORT y, FLOAT* tempToReturn, VARIANT_BOOL* res);


	STDMETHOD(WriteCameraOffset)(LONG32 offset);

	STDMETHOD(InitializeLogger)(BSTR log_config_data, VARIANT_BOOL developers_log, ULONG max_log_buffer_size, BSTR logs_path, BSTR log_file_name);
};

OBJECT_ENTRY_AUTO(__uuidof(MovieTransit), CMovieTransit)


