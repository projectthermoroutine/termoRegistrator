// ThermoRegistratorCOMServer.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "PD_COMServer_i.h"
//#include "PD_COMServer_i.c"

#include <thread>

#include <loglib\log.h>

using namespace ATL;


//class CPD_COMServerModule : public ATL::CAtlExeModuleT< CPD_COMServerModule >
//{
//public :
//	DECLARE_LIBID(LIBID_PD_COMServerLib)
//	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_PD_COMSERVER, "{612157F5-ECBB-4C35-A2B3-1A59E3A4DFC1}")
//	};
//
//CPD_COMServerModule _AtlModule;
//
//
//
////
//extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
//								LPTSTR /*lpCmdLine*/, int nShowCmd)
//{
//	return _AtlModule.WinMain(nShowCmd);
//}

class CATLCOMServiceModule : public CAtlServiceModuleT< CATLCOMServiceModule, IDS_SERVICENAME >
{
public:
	DECLARE_LIBID(LIBID_PD_COMServerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_PD_COMSERVER, "{612157F5-ECBB-4C35-A2B3-1A59E3A4DFC1}")
	HRESULT InitializeSecurity() throw()
	{
		LOG_STACK();

		CoInitializeSecurity(NULL, -1, NULL, NULL,
			RPC_C_AUTHN_LEVEL_NONE, RPC_C_IMP_LEVEL_IDENTIFY,
			NULL, EOAC_NONE, NULL);

		// TODO : Call CoInitializeSecurity and provide the appropriate 
		// security settings for your service
		// Suggested - PKT Level Authentication, 
		// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY 
		// and an appropriate Non NULL Security Descriptor.

		return S_OK;
	}
	// Called when the service is started
	void ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);

	HRESULT PreMessageLoop(_In_ int nShowCmd)
	{
		auto result = CAtlServiceModuleT<CATLCOMServiceModule, IDS_SERVICENAME>::PreMessageLoop(nShowCmd);

		if (SUCCEEDED(result))
		{
			result = CoGetClassObject(CLSID_ProxyPD_Dispatcher, CLSCTX_LOCAL_SERVER | CLSCTX_ACTIVATE_64_BIT_SERVER, NULL, IID_IClassFactory, (void**)&pICF);

			if (FAILED(result))
				return result;

			result = pICF->CreateInstance(NULL, IID_IProxyPD_Dispatcher, (void**)&pIProxy_pd_dispatcher);

			if (FAILED(result)){
				pICF->Release();
			}
		}

		return result;
	}

	HRESULT PostMessageLoop()
	{
		pIProxy_pd_dispatcher->Release();
		pICF->Release();
		return CAtlServiceModuleT<CATLCOMServiceModule, IDS_SERVICENAME>::PostMessageLoop();
	}
	private:
		IClassFactory* pICF;
		IProxyPD_Dispatcher* pIProxy_pd_dispatcher;

};

CATLCOMServiceModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
	LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}


void CATLCOMServiceModule::ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	pICF = nullptr;
	pIProxy_pd_dispatcher = nullptr;
	CAtlServiceModuleT<CATLCOMServiceModule, IDS_SERVICENAME>::ServiceMain(dwArgc, lpszArgv);
}
