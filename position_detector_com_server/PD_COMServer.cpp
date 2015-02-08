// ThermoRegistratorCOMServer.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "PD_COMServer_i.h"


using namespace ATL;


class CPD_COMServerModule : public ATL::CAtlExeModuleT< CPD_COMServerModule >
{
public :
	DECLARE_LIBID(LIBID_PD_COMServerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_PD_COMSERVER, "{612157F5-ECBB-4C35-A2B3-1A59E3A4DFC1}")
	};

CPD_COMServerModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

