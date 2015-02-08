#pragma once
#include "resource.h"       // main symbols
#include <comutil.h>
#include <map>
#include <memory>

#include "PD_COMServer_i.h"
#include "_IProxy_PD_DispatcherEvents_CP.H"
#include "..\midl\PD_COMServer_i.h"

#pragma comment(lib, "comsuppw.lib")



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CProxyPD_Dispatcher

class ATL_NO_VTABLE CProxyPD_Dispatcher :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CProxyPD_Dispatcher, &CLSID_ProxyPD_Dispatcher>,
	public IConnectionPointContainerImpl<CProxyPD_Dispatcher>,
	public CProxy_IProxy_PD_DispatcherEvents<CProxyPD_Dispatcher>,
	public IDispatchImpl<IProxyPD_Dispatcher, &IID_IProxyPD_Dispatcher, &LIBID_PD_COMServerLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CProxyPD_Dispatcher();
	~CProxyPD_Dispatcher();
private:
	struct Impl;
	std::unique_ptr<Impl> _p_impl;

public:
	DECLARE_REGISTRY_RESOURCEID(IDR_PROXY_PD_DISPATCHER)


BEGIN_COM_MAP(CProxyPD_Dispatcher)
	COM_INTERFACE_ENTRY(IProxyPD_Dispatcher)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CProxyPD_Dispatcher)
	CONNECTION_POINT_ENTRY(__uuidof(_IProxy_PD_DispatcherEvents))
END_CONNECTION_POINT_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	DECLARE_CLASSFACTORY_SINGLETON(CProxyPD_Dispatcher)

public:

	STDMETHOD(getConfig)(ShareMemorySettings* syncSettings, ShareMemorySettings* eventSettings, ULONG32* clientId);
	STDMETHOD(setConfig)(VARIANT Arr);
	STDMETHOD(connectToErrorsStream)(ShareMemorySettings* errStream, ULONG32* clientId);
	STDMETHOD(disconnectClient)(ULONG32 clientId, ULONG32 errorsClientId);
};

OBJECT_ENTRY_AUTO(__uuidof(ProxyPD_Dispatcher), CProxyPD_Dispatcher)
