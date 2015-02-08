// упаковщик dlldata.c

#ifdef _MERGE_PROXYSTUB // объединить DLL заглушки прокси

#define REGISTER_PROXY_DLL //DllRegisterServer и т. п.

#define _WIN32_WINNT 0x0500	//для WinNT 4.0 или Win95 с DCOM
#define USE_STUBLESS_PROXY	//определено только с ключом MIDL /Oicf

#pragma comment(lib, "rpcns4.lib")
#pragma comment(lib, "rpcrt4.lib")

#define ENTRY_PREFIX	Prx

#include "midl\dlldata.c"
#include "ThermoRoutine_p.c"

#endif //_MERGE_PROXYSTUB
