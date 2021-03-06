#pragma once
#include "resource.h"       // �������� �������
#include "ThermoRoutine_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "������������� COM-������� �� �������������� ������� ������� ���������� Windows CE, �������� ����������� Windows Mobile, � ������� �� ������������� ������ ��������� DCOM. ���������� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA, ����� ��������� ATL ������������ �������� ������������� COM-�������� � ��������� ������������� ��� ���������� ������������� COM-��������. ��� ��������� ������ � ����� rgs-����� ������ �������� 'Free', ��������� ��� ������������ ��������� ������, �������������� ��-DCOM ����������� Windows CE."
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
	STDMETHOD(InitializeLogger)(BSTR log_config_data, BSTR logs_path, BSTR log_file_name);
};

OBJECT_ENTRY_AUTO(__uuidof(Logger), CLogger)


