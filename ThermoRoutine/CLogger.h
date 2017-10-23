#pragma once
#include "resource.h"       // основные символы
#include "ThermoRoutine_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Однопотоковые COM-объекты не поддерживаются должным образом платформой Windows CE, например платформами Windows Mobile, в которых не предусмотрена полная поддержка DCOM. Определите _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA, чтобы принудить ATL поддерживать создание однопотоковых COM-объектов и разрешить использование его реализаций однопотоковых COM-объектов. Для потоковой модели в вашем rgs-файле задано значение 'Free', поскольку это единственная потоковая модель, поддерживаемая не-DCOM платформами Windows CE."
#endif

using namespace ATL;

// CTRWrapper
#if !defined LOGGER_INCLUDED

#define LOGGER_INCLUDED
#endif

class ATL_NO_VTABLE CLogger :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLogger, &CLSID_Logger>,
	public IDispatchImpl<ILogger, &IID_ILogger, &LIBID_ThermoRoutineLib, /*wMajor =*/ 2, /*wMinor =*/ 4>
{
public:
	CLogger();
	~CLogger();

public:
	DECLARE_REGISTRY_RESOURCEID(IDR_LOGGER)

	BEGIN_COM_MAP(CLogger)
		COM_INTERFACE_ENTRY(ILogger)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	DECLARE_CLASSFACTORY_SINGLETON(CLogger)

public:

	STDMETHOD(Close)(void);
	STDMETHOD(InitializeLogger)(BSTR log_config_full_filename, BSTR logs_path, BSTR log_file_name);
};

OBJECT_ENTRY_AUTO(__uuidof(Logger), CLogger)


