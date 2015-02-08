// dllmain.cpp: реализация DllMain.

#include "stdafx.h"
#include "resource.h"
#include "ThermoRoutine_i.h"
#include "dllmain.h"
#include "compreg.h"
#include "xdlldata.h"

CThermoRoutineModule _AtlModule;

class CThermoRoutineApp : public CWinApp
{
public:

// Переопределение
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CThermoRoutineApp, CWinApp)
END_MESSAGE_MAP()

CThermoRoutineApp theApp;

BOOL CThermoRoutineApp::InitInstance()
{
#ifdef _MERGE_PROXYSTUB
	if (!PrxDllMain(m_hInstance, DLL_PROCESS_ATTACH, NULL))
		return FALSE;
#endif
	return CWinApp::InitInstance();
}

int CThermoRoutineApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
