

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Fri Mar 27 14:36:54 2015
 */
/* Compiler settings for ..\ThermoRoutine.idl:
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

MIDL_DEFINE_GUID(IID, IID_IComponentRegistrar,0xa817e7a2,0x43fa,0x11d0,0x9e,0x44,0x00,0xaa,0x00,0xb6,0x77,0x0a);


MIDL_DEFINE_GUID(IID, IID_IPallete,0x859E7C6E,0x270C,0x41F2,0xAE,0x3A,0x7C,0xBD,0xDF,0xFA,0x30,0xAF);


MIDL_DEFINE_GUID(IID, IID_IArea,0x03BA3F31,0xA54D,0x443E,0xAE,0x88,0x2C,0x55,0x6E,0xA1,0x96,0xFA);


MIDL_DEFINE_GUID(IID, IID_ITRWrapper,0x1347B40A,0xBFBD,0x4A82,0x8A,0x75,0x46,0xA2,0x55,0x85,0x07,0xBA);


MIDL_DEFINE_GUID(IID, IID_IMovieTransit,0xE37EF668,0x25E2,0x4784,0xBF,0xF3,0x64,0x2A,0xA9,0x53,0xFC,0xAA);


MIDL_DEFINE_GUID(IID, LIBID_ThermoRoutineLib,0x58DCE02F,0x2B74,0x4B1D,0xAE,0xCE,0x63,0x51,0xED,0x4A,0xD7,0x92);


MIDL_DEFINE_GUID(CLSID, CLSID_CompReg,0xF6ADF13C,0x570E,0x46BD,0x92,0x7D,0xE3,0x28,0x31,0x82,0x7A,0xFD);


MIDL_DEFINE_GUID(IID, DIID__ITRWrapperEvents,0x13D9C294,0x5D0B,0x45B3,0xBE,0xB6,0xBA,0x50,0x9B,0x75,0x87,0x12);


MIDL_DEFINE_GUID(IID, DIID__IThermoLibErrorEvents,0x37F028AB,0x55D8,0x4838,0x87,0x95,0xB2,0xFE,0xD7,0x80,0x09,0x5B);


MIDL_DEFINE_GUID(IID, DIID__IThermoLibComponentsEvents,0x3DE144B6,0x936A,0x423A,0x9A,0xF9,0xB4,0x72,0x13,0x6D,0x80,0x24);


MIDL_DEFINE_GUID(CLSID, CLSID_TRWrapper,0x42D46C01,0xD857,0x481A,0x8B,0x59,0x6E,0x57,0xC3,0x43,0xD0,0x94);


MIDL_DEFINE_GUID(CLSID, CLSID_MovieTransit,0x4D7164CF,0xD373,0x41C6,0x85,0x0A,0xE7,0x3F,0xFA,0xC5,0x0E,0x3A);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



