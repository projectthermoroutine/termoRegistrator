

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Wed Feb 18 16:01:45 2015
 */
/* Compiler settings for ..\PD_COMServer.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IProxyPD_Dispatcher,0xBF9611C9,0x957F,0x4A19,0x9D,0xA1,0x72,0x95,0xB0,0xCE,0xE2,0xBC);


MIDL_DEFINE_GUID(IID, LIBID_PD_COMServerLib,0x782EA07C,0xE477,0x48D1,0x89,0xE5,0x53,0x51,0x4D,0xE6,0xDF,0xE1);


MIDL_DEFINE_GUID(IID, DIID__IProxy_PD_DispatcherEvents,0x6D25FBF8,0x4CE6,0x4931,0xA8,0xC7,0x94,0x4C,0x0F,0xA1,0xE5,0x0A);


MIDL_DEFINE_GUID(CLSID, CLSID_ProxyPD_Dispatcher,0x5EC93852,0x7FDE,0x4E1B,0x9C,0xB8,0x2E,0xA9,0x14,0x72,0xE1,0xAC);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



