
//#include "Winreg.h"
//#include "Windows.h"
#include "shlwapi.h"

inline BOOL SetRegStrValue(HKEY hkey, char * name, char * value)
{
	return ERROR_SUCCESS == RegSetValueEx(
								hkey,
								name,
								0,
								REG_SZ,
								(const BYTE*)value,
								strlen(value)
							 );
}

inline BOOL GetRegStrValue(HKEY hkey, char * name, char * resValue)
{
	unsigned long ds = MAX_PATH;

	LONG fff = RegQueryValueEx( hkey, name, NULL, NULL, (LPBYTE) resValue, &ds);

	return ERROR_SUCCESS == fff;

}

inline void SetKeyValue(char * valName, char * value)
{
	HKEY mK, hK;

	RegOpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE", &mK);
	RegCreateKey(mK, "MetroIR", &hK);

	SetRegStrValue(hK, valName, value);
}
