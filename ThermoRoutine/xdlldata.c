// ��������� dlldata.c

#ifdef _MERGE_PROXYSTUB // ���������� DLL �������� ������

#define REGISTER_PROXY_DLL //DllRegisterServer � �. �.

#define _WIN32_WINNT 0x0500	//��� WinNT 4.0 ��� Win95 � DCOM
#define USE_STUBLESS_PROXY	//���������� ������ � ������ MIDL /Oicf

#pragma comment(lib, "rpcns4.lib")
#pragma comment(lib, "rpcrt4.lib")

#define ENTRY_PREFIX	Prx

#include "midl\dlldata.c"
#include "ThermoRoutine_p.c"

#endif //_MERGE_PROXYSTUB
