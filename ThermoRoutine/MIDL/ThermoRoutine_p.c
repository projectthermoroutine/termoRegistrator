

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Mon May 11 01:03:21 2015
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

#if !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif /* __RPCPROXY_H_VERSION__ */


#include "ThermoRoutine_i.h"

#define TYPE_FORMAT_STRING_SIZE   1253                              
#define PROC_FORMAT_STRING_SIZE   3127                              
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   3            

typedef struct _ThermoRoutine_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } ThermoRoutine_MIDL_TYPE_FORMAT_STRING;

typedef struct _ThermoRoutine_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } ThermoRoutine_MIDL_PROC_FORMAT_STRING;

typedef struct _ThermoRoutine_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } ThermoRoutine_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const ThermoRoutine_MIDL_TYPE_FORMAT_STRING ThermoRoutine__MIDL_TypeFormatString;
extern const ThermoRoutine_MIDL_PROC_FORMAT_STRING ThermoRoutine__MIDL_ProcFormatString;
extern const ThermoRoutine_MIDL_EXPR_FORMAT_STRING ThermoRoutine__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IComponentRegistrar_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IComponentRegistrar_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPallete_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPallete_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IArea_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IArea_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ITRWrapper_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ITRWrapper_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IMovieTransit_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IMovieTransit_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const ThermoRoutine_MIDL_PROC_FORMAT_STRING ThermoRoutine__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure SetPallete */


	/* Procedure Attach */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 16 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x1 ),	/* 1 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter palleteFileName */


	/* Parameter bstrPath */

/* 24 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */


	/* Return value */

/* 30 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 32 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 34 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetDefaultPallete */


	/* Procedure RegisterAll */

/* 36 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 38 */	NdrFcLong( 0x0 ),	/* 0 */
/* 42 */	NdrFcShort( 0x8 ),	/* 8 */
/* 44 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 46 */	NdrFcShort( 0x0 ),	/* 0 */
/* 48 */	NdrFcShort( 0x8 ),	/* 8 */
/* 50 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 52 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x0 ),	/* 0 */
/* 58 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */


	/* Return value */

/* 60 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 62 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 64 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure StartRecord */


	/* Procedure UnregisterAll */

/* 66 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 68 */	NdrFcLong( 0x0 ),	/* 0 */
/* 72 */	NdrFcShort( 0x9 ),	/* 9 */
/* 74 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 76 */	NdrFcShort( 0x0 ),	/* 0 */
/* 78 */	NdrFcShort( 0x8 ),	/* 8 */
/* 80 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 82 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 84 */	NdrFcShort( 0x0 ),	/* 0 */
/* 86 */	NdrFcShort( 0x0 ),	/* 0 */
/* 88 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */


	/* Return value */

/* 90 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 92 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 94 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetComponents */

/* 96 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 98 */	NdrFcLong( 0x0 ),	/* 0 */
/* 102 */	NdrFcShort( 0xa ),	/* 10 */
/* 104 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 106 */	NdrFcShort( 0x0 ),	/* 0 */
/* 108 */	NdrFcShort( 0x8 ),	/* 8 */
/* 110 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 112 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 114 */	NdrFcShort( 0x1 ),	/* 1 */
/* 116 */	NdrFcShort( 0x0 ),	/* 0 */
/* 118 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pbstrCLSIDs */

/* 120 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 122 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 124 */	NdrFcShort( 0x41e ),	/* Type Offset=1054 */

	/* Parameter pbstrDescriptions */

/* 126 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 128 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 130 */	NdrFcShort( 0x41e ),	/* Type Offset=1054 */

	/* Return value */

/* 132 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 134 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 136 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RegisterComponent */

/* 138 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 140 */	NdrFcLong( 0x0 ),	/* 0 */
/* 144 */	NdrFcShort( 0xb ),	/* 11 */
/* 146 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 148 */	NdrFcShort( 0x0 ),	/* 0 */
/* 150 */	NdrFcShort( 0x8 ),	/* 8 */
/* 152 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 154 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 156 */	NdrFcShort( 0x0 ),	/* 0 */
/* 158 */	NdrFcShort( 0x1 ),	/* 1 */
/* 160 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrCLSID */

/* 162 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 164 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 166 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 168 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 170 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 172 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UnregisterComponent */

/* 174 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 176 */	NdrFcLong( 0x0 ),	/* 0 */
/* 180 */	NdrFcShort( 0xc ),	/* 12 */
/* 182 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 184 */	NdrFcShort( 0x0 ),	/* 0 */
/* 186 */	NdrFcShort( 0x8 ),	/* 8 */
/* 188 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 190 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 192 */	NdrFcShort( 0x0 ),	/* 0 */
/* 194 */	NdrFcShort( 0x1 ),	/* 1 */
/* 196 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrCLSID */

/* 198 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 200 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 202 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 204 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 206 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 208 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetPallete */

/* 210 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 212 */	NdrFcLong( 0x0 ),	/* 0 */
/* 216 */	NdrFcShort( 0x9 ),	/* 9 */
/* 218 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 220 */	NdrFcShort( 0x0 ),	/* 0 */
/* 222 */	NdrFcShort( 0x8 ),	/* 8 */
/* 224 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 226 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 228 */	NdrFcShort( 0x1 ),	/* 1 */
/* 230 */	NdrFcShort( 0x1 ),	/* 1 */
/* 232 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pallete */

/* 234 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 236 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 238 */	NdrFcShort( 0x430 ),	/* Type Offset=1072 */

	/* Return value */

/* 240 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 242 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 244 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetPalleteLength */

/* 246 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 248 */	NdrFcLong( 0x0 ),	/* 0 */
/* 252 */	NdrFcShort( 0xa ),	/* 10 */
/* 254 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 256 */	NdrFcShort( 0x0 ),	/* 0 */
/* 258 */	NdrFcShort( 0x22 ),	/* 34 */
/* 260 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 262 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 264 */	NdrFcShort( 0x0 ),	/* 0 */
/* 266 */	NdrFcShort( 0x0 ),	/* 0 */
/* 268 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter len */

/* 270 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 272 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 274 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 276 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 278 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 280 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddArea */

/* 282 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 284 */	NdrFcLong( 0x0 ),	/* 0 */
/* 288 */	NdrFcShort( 0x7 ),	/* 7 */
/* 290 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 292 */	NdrFcShort( 0x6 ),	/* 6 */
/* 294 */	NdrFcShort( 0x8 ),	/* 8 */
/* 296 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 298 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 300 */	NdrFcShort( 0x0 ),	/* 0 */
/* 302 */	NdrFcShort( 0x0 ),	/* 0 */
/* 304 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter id */

/* 306 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 308 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 310 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter area */

/* 312 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 314 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 316 */	NdrFcShort( 0x442 ),	/* Type Offset=1090 */

	/* Return value */

/* 318 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 320 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 322 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AreaChanged */

/* 324 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 326 */	NdrFcLong( 0x0 ),	/* 0 */
/* 330 */	NdrFcShort( 0x8 ),	/* 8 */
/* 332 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 334 */	NdrFcShort( 0x6 ),	/* 6 */
/* 336 */	NdrFcShort( 0x8 ),	/* 8 */
/* 338 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 340 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 342 */	NdrFcShort( 0x0 ),	/* 0 */
/* 344 */	NdrFcShort( 0x0 ),	/* 0 */
/* 346 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter id */

/* 348 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 350 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 352 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter area */

/* 354 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 356 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 358 */	NdrFcShort( 0x442 ),	/* Type Offset=1090 */

	/* Return value */

/* 360 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 362 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 364 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RemoveArea */

/* 366 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 368 */	NdrFcLong( 0x0 ),	/* 0 */
/* 372 */	NdrFcShort( 0x9 ),	/* 9 */
/* 374 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 376 */	NdrFcShort( 0x6 ),	/* 6 */
/* 378 */	NdrFcShort( 0x22 ),	/* 34 */
/* 380 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 382 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 384 */	NdrFcShort( 0x0 ),	/* 0 */
/* 386 */	NdrFcShort( 0x0 ),	/* 0 */
/* 388 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter id */

/* 390 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 392 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 394 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter type */

/* 396 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 398 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 400 */	NdrFcShort( 0x450 ),	/* Type Offset=1104 */

	/* Return value */

/* 402 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 404 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 406 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetAreasInfo */

/* 408 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 410 */	NdrFcLong( 0x0 ),	/* 0 */
/* 414 */	NdrFcShort( 0xa ),	/* 10 */
/* 416 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 418 */	NdrFcShort( 0x64 ),	/* 100 */
/* 420 */	NdrFcShort( 0x64 ),	/* 100 */
/* 422 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 424 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 426 */	NdrFcShort( 0x0 ),	/* 0 */
/* 428 */	NdrFcShort( 0x0 ),	/* 0 */
/* 430 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter results */

/* 432 */	NdrFcShort( 0x201a ),	/* Flags:  must free, in, out, srv alloc size=8 */
/* 434 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 436 */	NdrFcShort( 0x454 ),	/* Type Offset=1108 */

	/* Parameter size */

/* 438 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 440 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 442 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 444 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 446 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 448 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetAreaInfo */

/* 450 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 452 */	NdrFcLong( 0x0 ),	/* 0 */
/* 456 */	NdrFcShort( 0xb ),	/* 11 */
/* 458 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 460 */	NdrFcShort( 0x8 ),	/* 8 */
/* 462 */	NdrFcShort( 0x52 ),	/* 82 */
/* 464 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 466 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 468 */	NdrFcShort( 0x0 ),	/* 0 */
/* 470 */	NdrFcShort( 0x0 ),	/* 0 */
/* 472 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter aid */

/* 474 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 476 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 478 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter measure */

/* 480 */	NdrFcShort( 0x4112 ),	/* Flags:  must free, out, simple ref, srv alloc size=16 */
/* 482 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 484 */	NdrFcShort( 0x45c ),	/* Type Offset=1116 */

	/* Parameter result */

/* 486 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 488 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 490 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 492 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 494 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 496 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RemoveAllAreas */

/* 498 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 500 */	NdrFcLong( 0x0 ),	/* 0 */
/* 504 */	NdrFcShort( 0xc ),	/* 12 */
/* 506 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 508 */	NdrFcShort( 0x0 ),	/* 0 */
/* 510 */	NdrFcShort( 0x8 ),	/* 8 */
/* 512 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 514 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 516 */	NdrFcShort( 0x0 ),	/* 0 */
/* 518 */	NdrFcShort( 0x0 ),	/* 0 */
/* 520 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 522 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 524 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 526 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ClearGrabbingCache */

/* 528 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 530 */	NdrFcLong( 0x0 ),	/* 0 */
/* 534 */	NdrFcShort( 0x7 ),	/* 7 */
/* 536 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 538 */	NdrFcShort( 0x0 ),	/* 0 */
/* 540 */	NdrFcShort( 0x8 ),	/* 8 */
/* 542 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 544 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 546 */	NdrFcShort( 0x0 ),	/* 0 */
/* 548 */	NdrFcShort( 0x0 ),	/* 0 */
/* 550 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 552 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 554 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 556 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure StopGrabbing */

/* 558 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 560 */	NdrFcLong( 0x0 ),	/* 0 */
/* 564 */	NdrFcShort( 0x8 ),	/* 8 */
/* 566 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 568 */	NdrFcShort( 0xa ),	/* 10 */
/* 570 */	NdrFcShort( 0x8 ),	/* 8 */
/* 572 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 574 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 576 */	NdrFcShort( 0x0 ),	/* 0 */
/* 578 */	NdrFcShort( 0x0 ),	/* 0 */
/* 580 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter unload */

/* 582 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 584 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 586 */	0x1,		/* FC_BYTE */
			0x0,		/* 0 */

	/* Parameter save */

/* 588 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 590 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 592 */	0x1,		/* FC_BYTE */
			0x0,		/* 0 */

	/* Return value */

/* 594 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 596 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 598 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure StopRecord */

/* 600 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 602 */	NdrFcLong( 0x0 ),	/* 0 */
/* 606 */	NdrFcShort( 0xa ),	/* 10 */
/* 608 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 610 */	NdrFcShort( 0xa ),	/* 10 */
/* 612 */	NdrFcShort( 0x8 ),	/* 8 */
/* 614 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 616 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 618 */	NdrFcShort( 0x0 ),	/* 0 */
/* 620 */	NdrFcShort( 0x0 ),	/* 0 */
/* 622 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter unload */

/* 624 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 626 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 628 */	0x1,		/* FC_BYTE */
			0x0,		/* 0 */

	/* Parameter save */

/* 630 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 632 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 634 */	0x1,		/* FC_BYTE */
			0x0,		/* 0 */

	/* Return value */

/* 636 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 638 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 640 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetBlockCamFrame */

/* 642 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 644 */	NdrFcLong( 0x0 ),	/* 0 */
/* 648 */	NdrFcShort( 0xb ),	/* 11 */
/* 650 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 652 */	NdrFcShort( 0x5 ),	/* 5 */
/* 654 */	NdrFcShort( 0x8 ),	/* 8 */
/* 656 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 658 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 660 */	NdrFcShort( 0x0 ),	/* 0 */
/* 662 */	NdrFcShort( 0x0 ),	/* 0 */
/* 664 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter blockFlag */

/* 666 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 668 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 670 */	0x1,		/* FC_BYTE */
			0x0,		/* 0 */

	/* Return value */

/* 672 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 674 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 676 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure StartRecieveCoordinates */

/* 678 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 680 */	NdrFcLong( 0x0 ),	/* 0 */
/* 684 */	NdrFcShort( 0xc ),	/* 12 */
/* 686 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 688 */	NdrFcShort( 0xc ),	/* 12 */
/* 690 */	NdrFcShort( 0x8 ),	/* 8 */
/* 692 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 694 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 696 */	NdrFcShort( 0x0 ),	/* 0 */
/* 698 */	NdrFcShort( 0x1 ),	/* 1 */
/* 700 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pd_ip */

/* 702 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 704 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 706 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter pd_i_ip */

/* 708 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 710 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 712 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter pd_port */

/* 714 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 716 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 718 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter events_pd_ip */

/* 720 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 722 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 724 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter events_i_pd_ip */

/* 726 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 728 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 730 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter events_pd_port */

/* 732 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 734 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 736 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 738 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 740 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 742 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Close */


	/* Procedure StopRecieveCoordinates */

/* 744 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 746 */	NdrFcLong( 0x0 ),	/* 0 */
/* 750 */	NdrFcShort( 0xd ),	/* 13 */
/* 752 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 754 */	NdrFcShort( 0x0 ),	/* 0 */
/* 756 */	NdrFcShort( 0x8 ),	/* 8 */
/* 758 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 760 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 762 */	NdrFcShort( 0x0 ),	/* 0 */
/* 764 */	NdrFcShort( 0x0 ),	/* 0 */
/* 766 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */


	/* Return value */

/* 768 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 770 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 772 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure StartGrabbing */

/* 774 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 776 */	NdrFcLong( 0x0 ),	/* 0 */
/* 780 */	NdrFcShort( 0xe ),	/* 14 */
/* 782 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 784 */	NdrFcShort( 0x0 ),	/* 0 */
/* 786 */	NdrFcShort( 0x8 ),	/* 8 */
/* 788 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 790 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 792 */	NdrFcShort( 0x0 ),	/* 0 */
/* 794 */	NdrFcShort( 0x0 ),	/* 0 */
/* 796 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 798 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 800 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 802 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetGrabberPath */

/* 804 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 806 */	NdrFcLong( 0x0 ),	/* 0 */
/* 810 */	NdrFcShort( 0xf ),	/* 15 */
/* 812 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 814 */	NdrFcShort( 0x0 ),	/* 0 */
/* 816 */	NdrFcShort( 0x8 ),	/* 8 */
/* 818 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 820 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 822 */	NdrFcShort( 0x0 ),	/* 0 */
/* 824 */	NdrFcShort( 0x1 ),	/* 1 */
/* 826 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter grabberPath */

/* 828 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 830 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 832 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 834 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 836 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 838 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ShowCamSettings */

/* 840 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 842 */	NdrFcLong( 0x0 ),	/* 0 */
/* 846 */	NdrFcShort( 0x10 ),	/* 16 */
/* 848 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 850 */	NdrFcShort( 0x0 ),	/* 0 */
/* 852 */	NdrFcShort( 0x8 ),	/* 8 */
/* 854 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 856 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 858 */	NdrFcShort( 0x0 ),	/* 0 */
/* 860 */	NdrFcShort( 0x0 ),	/* 0 */
/* 862 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 864 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 866 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 868 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure HideCamSettings */

/* 870 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 872 */	NdrFcLong( 0x0 ),	/* 0 */
/* 876 */	NdrFcShort( 0x11 ),	/* 17 */
/* 878 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 880 */	NdrFcShort( 0x0 ),	/* 0 */
/* 882 */	NdrFcShort( 0x8 ),	/* 8 */
/* 884 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 886 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 888 */	NdrFcShort( 0x0 ),	/* 0 */
/* 890 */	NdrFcShort( 0x0 ),	/* 0 */
/* 892 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 894 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 896 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 898 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetGrabberSources */

/* 900 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 902 */	NdrFcLong( 0x0 ),	/* 0 */
/* 906 */	NdrFcShort( 0x12 ),	/* 18 */
/* 908 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 910 */	NdrFcShort( 0x0 ),	/* 0 */
/* 912 */	NdrFcShort( 0x8 ),	/* 8 */
/* 914 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 916 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 918 */	NdrFcShort( 0x1 ),	/* 1 */
/* 920 */	NdrFcShort( 0x0 ),	/* 0 */
/* 922 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sourcesList */

/* 924 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 926 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 928 */	NdrFcShort( 0x41e ),	/* Type Offset=1054 */

	/* Return value */

/* 930 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 932 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 934 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetRealTimeFrameRaster */

/* 936 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 938 */	NdrFcLong( 0x0 ),	/* 0 */
/* 942 */	NdrFcShort( 0x13 ),	/* 19 */
/* 944 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 946 */	NdrFcShort( 0x8 ),	/* 8 */
/* 948 */	NdrFcShort( 0x22 ),	/* 34 */
/* 950 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 952 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 954 */	NdrFcShort( 0x1 ),	/* 1 */
/* 956 */	NdrFcShort( 0x1 ),	/* 1 */
/* 958 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter frameId */

/* 960 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 962 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 964 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter frame_info */

/* 966 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
/* 968 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 970 */	NdrFcShort( 0x4ac ),	/* Type Offset=1196 */

	/* Parameter pixels */

/* 972 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 974 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 976 */	NdrFcShort( 0x430 ),	/* Type Offset=1072 */

	/* Parameter res */

/* 978 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 980 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 982 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 984 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 986 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 988 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetNextRealTimeFrameRaster */

/* 990 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 992 */	NdrFcLong( 0x0 ),	/* 0 */
/* 996 */	NdrFcShort( 0x14 ),	/* 20 */
/* 998 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1000 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1002 */	NdrFcShort( 0x3e ),	/* 62 */
/* 1004 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 1006 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 1008 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1010 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1012 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter frameId */

/* 1014 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1016 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1018 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter frame_info */

/* 1020 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
/* 1022 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1024 */	NdrFcShort( 0x4ac ),	/* Type Offset=1196 */

	/* Parameter pixels */

/* 1026 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 1028 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1030 */	NdrFcShort( 0x430 ),	/* Type Offset=1072 */

	/* Parameter res */

/* 1032 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1034 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1036 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1038 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1040 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1042 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetRealTimeFrame */

/* 1044 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1046 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1050 */	NdrFcShort( 0x15 ),	/* 21 */
/* 1052 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1054 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1056 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1058 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 1060 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 1062 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1064 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1066 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter frameId */

/* 1068 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1070 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1072 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter frame_info */

/* 1074 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
/* 1076 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1078 */	NdrFcShort( 0x4ac ),	/* Type Offset=1196 */

	/* Parameter pixels */

/* 1080 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 1082 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1084 */	NdrFcShort( 0x430 ),	/* Type Offset=1072 */

	/* Parameter temp_values */

/* 1086 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 1088 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1090 */	NdrFcShort( 0x430 ),	/* Type Offset=1072 */

	/* Parameter res */

/* 1092 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1094 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1096 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1098 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1100 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1102 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetNextRealTimeFrame */

/* 1104 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1106 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1110 */	NdrFcShort( 0x16 ),	/* 22 */
/* 1112 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1114 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1116 */	NdrFcShort( 0x3e ),	/* 62 */
/* 1118 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 1120 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 1122 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1124 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1126 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter frameId */

/* 1128 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1130 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1132 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter frame_info */

/* 1134 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
/* 1136 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1138 */	NdrFcShort( 0x4ac ),	/* Type Offset=1196 */

	/* Parameter pixels */

/* 1140 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 1142 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1144 */	NdrFcShort( 0x430 ),	/* Type Offset=1072 */

	/* Parameter temp_values */

/* 1146 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 1148 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1150 */	NdrFcShort( 0x430 ),	/* Type Offset=1072 */

	/* Parameter res */

/* 1152 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1154 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1156 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1158 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1160 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1162 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_pixel_temperature */

/* 1164 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1166 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1170 */	NdrFcShort( 0x17 ),	/* 23 */
/* 1172 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1174 */	NdrFcShort( 0x14 ),	/* 20 */
/* 1176 */	NdrFcShort( 0x3e ),	/* 62 */
/* 1178 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x6,		/* 6 */
/* 1180 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1182 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1184 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1186 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter frameId */

/* 1188 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1190 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1192 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter x */

/* 1194 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1196 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1198 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter y */

/* 1200 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1202 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1204 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter tempToReturn */

/* 1206 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1208 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1210 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter res */

/* 1212 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1214 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1216 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1218 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1220 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1222 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ConnectCamera */

/* 1224 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1226 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1230 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1232 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1234 */	NdrFcShort( 0x5 ),	/* 5 */
/* 1236 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1238 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1240 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1242 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1244 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1246 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter initMode */

/* 1248 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1250 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1252 */	0x1,		/* FC_BYTE */
			0x0,		/* 0 */

	/* Parameter res */

/* 1254 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1256 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1258 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1260 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1262 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1264 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DisconnectCamera */

/* 1266 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1268 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1272 */	NdrFcShort( 0x19 ),	/* 25 */
/* 1274 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1276 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1278 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1280 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1282 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1284 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1286 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1288 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter res */

/* 1290 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1292 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1294 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1296 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1298 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1300 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure FinishAll */

/* 1302 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1304 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1308 */	NdrFcShort( 0x1a ),	/* 26 */
/* 1310 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1312 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1314 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1316 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1318 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1320 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1322 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1324 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 1326 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1328 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1330 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetPallete */

/* 1332 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1334 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1338 */	NdrFcShort( 0x1b ),	/* 27 */
/* 1340 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1342 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1344 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1346 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1348 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1350 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1352 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1354 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter palleteFileName */

/* 1356 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1358 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1360 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 1362 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1364 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1366 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetDefaultPallete */

/* 1368 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1370 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1374 */	NdrFcShort( 0x1c ),	/* 28 */
/* 1376 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1378 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1380 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1382 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1384 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1386 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1388 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1390 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 1392 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1394 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1396 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetPallete */

/* 1398 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1400 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1404 */	NdrFcShort( 0x1d ),	/* 29 */
/* 1406 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1408 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1410 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1412 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1414 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 1416 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1418 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1420 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pallete */

/* 1422 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 1424 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1426 */	NdrFcShort( 0x430 ),	/* Type Offset=1072 */

	/* Return value */

/* 1428 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1430 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1432 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetPalleteLength */

/* 1434 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1436 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1440 */	NdrFcShort( 0x1e ),	/* 30 */
/* 1442 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1444 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1446 */	NdrFcShort( 0x3e ),	/* 62 */
/* 1448 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1450 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1452 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1454 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1456 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter number_colors */

/* 1458 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1460 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1462 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter len */

/* 1464 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1466 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1468 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1470 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1472 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1474 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetPaletteCalibrationMode */

/* 1476 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1478 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1482 */	NdrFcShort( 0x1f ),	/* 31 */
/* 1484 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1486 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1488 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1490 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1492 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1494 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1496 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1498 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter mode */

/* 1500 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1502 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1504 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */

/* 1506 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1508 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1510 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetPaletteCalibration */

/* 1512 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1514 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1518 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1520 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1522 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1524 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1526 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1528 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1530 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1532 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1534 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter min */

/* 1536 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1538 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1540 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter max */

/* 1542 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1544 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1546 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 1548 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1550 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1552 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddArea */

/* 1554 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1556 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1560 */	NdrFcShort( 0x21 ),	/* 33 */
/* 1562 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1564 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1566 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1568 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 1570 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1572 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1574 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1576 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter id */

/* 1578 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1580 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1582 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter area */

/* 1584 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1586 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1588 */	NdrFcShort( 0x442 ),	/* Type Offset=1090 */

	/* Return value */

/* 1590 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1592 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1594 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AreaChanged */

/* 1596 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1598 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1602 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1604 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1606 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1608 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1610 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 1612 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1614 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1616 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1618 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter id */

/* 1620 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1622 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1624 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter area */

/* 1626 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1628 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1630 */	NdrFcShort( 0x442 ),	/* Type Offset=1090 */

	/* Return value */

/* 1632 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1634 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1636 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RemoveArea */

/* 1638 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1640 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1644 */	NdrFcShort( 0x23 ),	/* 35 */
/* 1646 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1648 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1650 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1652 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1654 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1656 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1658 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1660 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter id */

/* 1662 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1664 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1666 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter type */

/* 1668 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 1670 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1672 */	NdrFcShort( 0x450 ),	/* Type Offset=1104 */

	/* Return value */

/* 1674 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1676 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1678 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetAreaInfo */

/* 1680 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1682 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1686 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1688 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1690 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1692 */	NdrFcShort( 0x52 ),	/* 82 */
/* 1694 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 1696 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1698 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1700 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1702 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter aid */

/* 1704 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1706 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1708 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter measure */

/* 1710 */	NdrFcShort( 0x4112 ),	/* Flags:  must free, out, simple ref, srv alloc size=16 */
/* 1712 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1714 */	NdrFcShort( 0x45c ),	/* Type Offset=1116 */

	/* Parameter result */

/* 1716 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1718 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1720 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1722 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1724 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1726 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RemoveAllAreas */

/* 1728 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1730 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1734 */	NdrFcShort( 0x25 ),	/* 37 */
/* 1736 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1738 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1740 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1742 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1744 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1746 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1748 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1750 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 1752 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1754 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1756 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetMaxFramesInIRBFile */

/* 1758 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1760 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1764 */	NdrFcShort( 0x26 ),	/* 38 */
/* 1766 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1768 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1770 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1772 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1774 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1776 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1778 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1780 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter frames_number */

/* 1782 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1784 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1786 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1788 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1790 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1792 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetCounterSize */

/* 1794 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1796 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1800 */	NdrFcShort( 0x27 ),	/* 39 */
/* 1802 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1804 */	NdrFcShort( 0x5 ),	/* 5 */
/* 1806 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1808 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1810 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1812 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1814 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1816 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter counterSize */

/* 1818 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1820 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1822 */	0x1,		/* FC_BYTE */
			0x0,		/* 0 */

	/* Return value */

/* 1824 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1826 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1828 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetCameraOffset */

/* 1830 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1832 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1836 */	NdrFcShort( 0x28 ),	/* 40 */
/* 1838 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1840 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1842 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1844 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1846 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1848 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1850 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1852 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter cameraOffset */

/* 1854 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1856 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1858 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1860 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1862 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1864 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetIRBFiles */

/* 1866 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1868 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1872 */	NdrFcShort( 0x7 ),	/* 7 */
/* 1874 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 1876 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1878 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1880 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 1882 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 1884 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1886 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1888 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter filesNames */

/* 1890 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1892 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1894 */	NdrFcShort( 0x4ce ),	/* Type Offset=1230 */

	/* Parameter errors */

/* 1896 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 1898 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1900 */	NdrFcShort( 0x41e ),	/* Type Offset=1054 */

	/* Parameter result */

/* 1902 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1904 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1906 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1908 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1910 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1912 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure FilesCount */

/* 1914 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1916 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1920 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1922 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1924 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1926 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1928 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1930 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1932 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1934 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1936 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter filesCount */

/* 1938 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1940 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1942 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1944 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1946 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1948 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure FramesCount */

/* 1950 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1952 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1956 */	NdrFcShort( 0x9 ),	/* 9 */
/* 1958 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1960 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1962 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1964 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1966 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1968 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1970 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1972 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter framesCount */

/* 1974 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1976 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1978 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1980 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1982 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1984 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetFrameRaster */

/* 1986 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1988 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1992 */	NdrFcShort( 0xa ),	/* 10 */
/* 1994 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1996 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1998 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2000 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 2002 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 2004 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2006 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2008 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter frameIndex */

/* 2010 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2012 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2014 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter frame_info */

/* 2016 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
/* 2018 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2020 */	NdrFcShort( 0x4ac ),	/* Type Offset=1196 */

	/* Parameter frameRaster */

/* 2022 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 2024 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2026 */	NdrFcShort( 0x430 ),	/* Type Offset=1072 */

	/* Parameter res */

/* 2028 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2030 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2032 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2034 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2036 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2038 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetFrame */

/* 2040 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2042 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2046 */	NdrFcShort( 0xb ),	/* 11 */
/* 2048 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2050 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2052 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2054 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 2056 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 2058 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2060 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2062 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter frameIndex */

/* 2064 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2066 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2068 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter frame_info */

/* 2070 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
/* 2072 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2074 */	NdrFcShort( 0x4ac ),	/* Type Offset=1196 */

	/* Parameter pixels */

/* 2076 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 2078 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2080 */	NdrFcShort( 0x430 ),	/* Type Offset=1072 */

	/* Parameter temp_values */

/* 2082 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 2084 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2086 */	NdrFcShort( 0x430 ),	/* Type Offset=1072 */

	/* Parameter res */

/* 2088 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2090 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2092 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2094 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2096 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2098 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_pixel_temperature */

/* 2100 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2102 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2106 */	NdrFcShort( 0xc ),	/* 12 */
/* 2108 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2110 */	NdrFcShort( 0x14 ),	/* 20 */
/* 2112 */	NdrFcShort( 0x3e ),	/* 62 */
/* 2114 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x6,		/* 6 */
/* 2116 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2118 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2120 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2122 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter frameIndex */

/* 2124 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2126 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2128 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter x */

/* 2130 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2132 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2134 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter y */

/* 2136 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2138 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2140 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter tempToReturn */

/* 2142 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2144 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2146 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter res */

/* 2148 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2150 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2152 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2154 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2156 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2158 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SaveCurrentFrame */

/* 2160 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2162 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2166 */	NdrFcShort( 0xe ),	/* 14 */
/* 2168 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2170 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2172 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2174 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 2176 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2178 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2180 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2182 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter filename */

/* 2184 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2186 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2188 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter result */

/* 2190 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2192 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2194 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2196 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2198 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2200 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SaveFrame */

/* 2202 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2204 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2208 */	NdrFcShort( 0xf ),	/* 15 */
/* 2210 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2212 */	NdrFcShort( 0x18 ),	/* 24 */
/* 2214 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2216 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 2218 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2220 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2222 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2224 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 2226 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2228 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2230 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter picket */

/* 2232 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2234 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2236 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter offset */

/* 2238 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2240 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2242 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter filename */

/* 2244 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2246 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2248 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter result */

/* 2250 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2252 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2254 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2256 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2258 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2260 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetCurFrameTemperatures */

/* 2262 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2264 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2268 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2270 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2272 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2274 */	NdrFcShort( 0x48 ),	/* 72 */
/* 2276 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2278 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2280 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2282 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2284 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter measure */

/* 2286 */	NdrFcShort( 0x8112 ),	/* Flags:  must free, out, simple ref, srv alloc size=32 */
/* 2288 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2290 */	NdrFcShort( 0x480 ),	/* Type Offset=1152 */

	/* Return value */

/* 2292 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2294 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2296 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure IsFrameMeetFilter */

/* 2298 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2300 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2304 */	NdrFcShort( 0x11 ),	/* 17 */
/* 2306 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2308 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2310 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2312 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 2314 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2316 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2318 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2320 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter frameNum */

/* 2322 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2324 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2326 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter result */

/* 2328 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2330 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2332 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2334 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2336 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2338 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetFramePositionInfo */

/* 2340 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2342 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2346 */	NdrFcShort( 0x12 ),	/* 18 */
/* 2348 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2350 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2352 */	NdrFcShort( 0x2c ),	/* 44 */
/* 2354 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x4,		/* 4 */
/* 2356 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 2358 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2360 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2362 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter frame_id */

/* 2364 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2366 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2368 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter coordinate */

/* 2370 */	NdrFcShort( 0x6113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=24 */
/* 2372 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2374 */	NdrFcShort( 0x496 ),	/* Type Offset=1174 */

	/* Parameter timestamp */

/* 2376 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2378 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2380 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 2382 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2384 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2386 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetCurrentFramePositionInfo */

/* 2388 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2390 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2394 */	NdrFcShort( 0x13 ),	/* 19 */
/* 2396 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2398 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2400 */	NdrFcShort( 0x2c ),	/* 44 */
/* 2402 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 2404 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 2406 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2408 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2410 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter coordinate */

/* 2412 */	NdrFcShort( 0x6113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=24 */
/* 2414 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2416 */	NdrFcShort( 0x496 ),	/* Type Offset=1174 */

	/* Parameter timestamp */

/* 2418 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2420 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2422 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 2424 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2426 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2428 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ClearMovieTransitCache */

/* 2430 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2432 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2436 */	NdrFcShort( 0x14 ),	/* 20 */
/* 2438 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2440 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2442 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2444 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 2446 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2448 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2450 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2452 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 2454 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2456 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2458 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure InitFrameFilter */

/* 2460 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2462 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2466 */	NdrFcShort( 0x15 ),	/* 21 */
/* 2468 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
/* 2470 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2472 */	NdrFcShort( 0xfc ),	/* 252 */
/* 2474 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0xa,		/* 10 */
/* 2476 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2478 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2480 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2482 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter timeFrom */

/* 2484 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2486 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2488 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter timeTo */

/* 2490 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2492 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2494 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter tempObj */

/* 2496 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2498 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2500 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter tempArea */

/* 2502 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2504 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2506 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter pickFrom */

/* 2508 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2510 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2512 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter pickTo */

/* 2514 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2516 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2518 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter flags */

/* 2520 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2522 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2524 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter checkedOnly */

/* 2526 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2528 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 2530 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter result */

/* 2532 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2534 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 2536 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2538 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2540 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 2542 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetFrameFilter */

/* 2544 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2546 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2550 */	NdrFcShort( 0x16 ),	/* 22 */
/* 2552 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 2554 */	NdrFcShort( 0x34 ),	/* 52 */
/* 2556 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2558 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x9,		/* 9 */
/* 2560 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2562 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2564 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2566 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter timeFrom */

/* 2568 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2570 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2572 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter timeTo */

/* 2574 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2576 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2578 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter tempObj */

/* 2580 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2582 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2584 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter tempArea */

/* 2586 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2588 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2590 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter picketFrom */

/* 2592 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2594 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2596 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter picketTo */

/* 2598 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2600 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2602 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter flags */

/* 2604 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2606 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2608 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter res */

/* 2610 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2612 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 2614 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2616 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2618 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 2620 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetPallete */

/* 2622 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2624 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2628 */	NdrFcShort( 0x17 ),	/* 23 */
/* 2630 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2632 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2634 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2636 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2638 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2640 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2642 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2644 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter palleteFileName */

/* 2646 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2648 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2650 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 2652 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2654 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2656 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetDefaultPallete */

/* 2658 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2660 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2664 */	NdrFcShort( 0x18 ),	/* 24 */
/* 2666 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2668 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2670 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2672 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 2674 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2676 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2678 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2680 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 2682 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2684 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2686 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetPallete */

/* 2688 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2690 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2694 */	NdrFcShort( 0x19 ),	/* 25 */
/* 2696 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2698 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2700 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2702 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2704 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 2706 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2708 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2710 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pallete */

/* 2712 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 2714 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2716 */	NdrFcShort( 0x430 ),	/* Type Offset=1072 */

	/* Return value */

/* 2718 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2720 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2722 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetPalleteLength */

/* 2724 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2726 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2730 */	NdrFcShort( 0x1a ),	/* 26 */
/* 2732 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2734 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2736 */	NdrFcShort( 0x3e ),	/* 62 */
/* 2738 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 2740 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2742 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2744 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2746 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter number_colors */

/* 2748 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2750 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2752 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter len */

/* 2754 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2756 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2758 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2760 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2762 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2764 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetPaletteCalibrationMode */

/* 2766 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2768 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2772 */	NdrFcShort( 0x1b ),	/* 27 */
/* 2774 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2776 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2778 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2780 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2782 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2784 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2786 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2788 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter mode */

/* 2790 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2792 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2794 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */

/* 2796 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2798 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2800 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetPaletteCalibration */

/* 2802 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2804 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2808 */	NdrFcShort( 0x1c ),	/* 28 */
/* 2810 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2812 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2814 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2816 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 2818 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2820 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2822 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2824 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter min */

/* 2826 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2828 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2830 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter max */

/* 2832 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2834 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2836 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 2838 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2840 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2842 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddArea */

/* 2844 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2846 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2850 */	NdrFcShort( 0x1d ),	/* 29 */
/* 2852 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2854 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2856 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2858 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 2860 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2862 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2864 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2866 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter id */

/* 2868 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2870 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2872 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter area */

/* 2874 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2876 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2878 */	NdrFcShort( 0x442 ),	/* Type Offset=1090 */

	/* Return value */

/* 2880 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2882 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2884 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AreaChanged */

/* 2886 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2888 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2892 */	NdrFcShort( 0x1e ),	/* 30 */
/* 2894 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2896 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2898 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2900 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 2902 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2904 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2906 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2908 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter id */

/* 2910 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2912 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2914 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter area */

/* 2916 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2918 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2920 */	NdrFcShort( 0x442 ),	/* Type Offset=1090 */

	/* Return value */

/* 2922 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2924 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2926 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RemoveArea */

/* 2928 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2930 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2934 */	NdrFcShort( 0x1f ),	/* 31 */
/* 2936 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2938 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2940 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2942 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 2944 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2946 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2948 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2950 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter id */

/* 2952 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2954 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2956 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter type */

/* 2958 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 2960 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2962 */	NdrFcShort( 0x450 ),	/* Type Offset=1104 */

	/* Return value */

/* 2964 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2966 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2968 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetAreasInfo */

/* 2970 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2972 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2976 */	NdrFcShort( 0x20 ),	/* 32 */
/* 2978 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2980 */	NdrFcShort( 0x64 ),	/* 100 */
/* 2982 */	NdrFcShort( 0x64 ),	/* 100 */
/* 2984 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 2986 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2988 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2990 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2992 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter results */

/* 2994 */	NdrFcShort( 0x201a ),	/* Flags:  must free, in, out, srv alloc size=8 */
/* 2996 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2998 */	NdrFcShort( 0x454 ),	/* Type Offset=1108 */

	/* Parameter size */

/* 3000 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3002 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3004 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3006 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3008 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3010 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetAreaInfo */

/* 3012 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3014 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3018 */	NdrFcShort( 0x21 ),	/* 33 */
/* 3020 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3022 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3024 */	NdrFcShort( 0x52 ),	/* 82 */
/* 3026 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 3028 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3030 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3032 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3034 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter aid */

/* 3036 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3038 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3040 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter measure */

/* 3042 */	NdrFcShort( 0x4112 ),	/* Flags:  must free, out, simple ref, srv alloc size=16 */
/* 3044 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3046 */	NdrFcShort( 0x45c ),	/* Type Offset=1116 */

	/* Parameter result */

/* 3048 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 3050 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3052 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 3054 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3056 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3058 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RemoveAllAreas */

/* 3060 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3062 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3066 */	NdrFcShort( 0x22 ),	/* 34 */
/* 3068 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3070 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3072 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3074 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 3076 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3078 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3080 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3082 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 3084 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3086 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3088 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure WriteCameraOffset */

/* 3090 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3092 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3096 */	NdrFcShort( 0x23 ),	/* 35 */
/* 3098 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3100 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3102 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3104 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 3106 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3108 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3110 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3112 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter offset */

/* 3114 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3116 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3118 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3120 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3122 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3124 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const ThermoRoutine_MIDL_TYPE_FORMAT_STRING ThermoRoutine__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x12, 0x0,	/* FC_UP */
/*  4 */	NdrFcShort( 0xe ),	/* Offset= 14 (18) */
/*  6 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/*  8 */	NdrFcShort( 0x2 ),	/* 2 */
/* 10 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 12 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 14 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 16 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 18 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 20 */	NdrFcShort( 0x8 ),	/* 8 */
/* 22 */	NdrFcShort( 0xfff0 ),	/* Offset= -16 (6) */
/* 24 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 26 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 28 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 30 */	NdrFcShort( 0x0 ),	/* 0 */
/* 32 */	NdrFcShort( 0x4 ),	/* 4 */
/* 34 */	NdrFcShort( 0x0 ),	/* 0 */
/* 36 */	NdrFcShort( 0xffde ),	/* Offset= -34 (2) */
/* 38 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 40 */	NdrFcShort( 0x3f6 ),	/* Offset= 1014 (1054) */
/* 42 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 44 */	NdrFcShort( 0x2 ),	/* Offset= 2 (46) */
/* 46 */	
			0x13, 0x0,	/* FC_OP */
/* 48 */	NdrFcShort( 0x3dc ),	/* Offset= 988 (1036) */
/* 50 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 52 */	NdrFcShort( 0x18 ),	/* 24 */
/* 54 */	NdrFcShort( 0xa ),	/* 10 */
/* 56 */	NdrFcLong( 0x8 ),	/* 8 */
/* 60 */	NdrFcShort( 0x5a ),	/* Offset= 90 (150) */
/* 62 */	NdrFcLong( 0xd ),	/* 13 */
/* 66 */	NdrFcShort( 0x90 ),	/* Offset= 144 (210) */
/* 68 */	NdrFcLong( 0x9 ),	/* 9 */
/* 72 */	NdrFcShort( 0xc2 ),	/* Offset= 194 (266) */
/* 74 */	NdrFcLong( 0xc ),	/* 12 */
/* 78 */	NdrFcShort( 0x2c0 ),	/* Offset= 704 (782) */
/* 80 */	NdrFcLong( 0x24 ),	/* 36 */
/* 84 */	NdrFcShort( 0x2ea ),	/* Offset= 746 (830) */
/* 86 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 90 */	NdrFcShort( 0x306 ),	/* Offset= 774 (864) */
/* 92 */	NdrFcLong( 0x10 ),	/* 16 */
/* 96 */	NdrFcShort( 0x320 ),	/* Offset= 800 (896) */
/* 98 */	NdrFcLong( 0x2 ),	/* 2 */
/* 102 */	NdrFcShort( 0x33a ),	/* Offset= 826 (928) */
/* 104 */	NdrFcLong( 0x3 ),	/* 3 */
/* 108 */	NdrFcShort( 0x354 ),	/* Offset= 852 (960) */
/* 110 */	NdrFcLong( 0x14 ),	/* 20 */
/* 114 */	NdrFcShort( 0x36e ),	/* Offset= 878 (992) */
/* 116 */	NdrFcShort( 0xffff ),	/* Offset= -1 (115) */
/* 118 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 120 */	NdrFcShort( 0x4 ),	/* 4 */
/* 122 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 124 */	NdrFcShort( 0x0 ),	/* 0 */
/* 126 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 128 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 130 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 132 */	NdrFcShort( 0x4 ),	/* 4 */
/* 134 */	NdrFcShort( 0x0 ),	/* 0 */
/* 136 */	NdrFcShort( 0x1 ),	/* 1 */
/* 138 */	NdrFcShort( 0x0 ),	/* 0 */
/* 140 */	NdrFcShort( 0x0 ),	/* 0 */
/* 142 */	0x13, 0x0,	/* FC_OP */
/* 144 */	NdrFcShort( 0xff82 ),	/* Offset= -126 (18) */
/* 146 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 148 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 150 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 152 */	NdrFcShort( 0x8 ),	/* 8 */
/* 154 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 156 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 158 */	NdrFcShort( 0x4 ),	/* 4 */
/* 160 */	NdrFcShort( 0x4 ),	/* 4 */
/* 162 */	0x11, 0x0,	/* FC_RP */
/* 164 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (118) */
/* 166 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 168 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 170 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 172 */	NdrFcLong( 0x0 ),	/* 0 */
/* 176 */	NdrFcShort( 0x0 ),	/* 0 */
/* 178 */	NdrFcShort( 0x0 ),	/* 0 */
/* 180 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 182 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 184 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 186 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 188 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 190 */	NdrFcShort( 0x0 ),	/* 0 */
/* 192 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 194 */	NdrFcShort( 0x0 ),	/* 0 */
/* 196 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 198 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 202 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 204 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 206 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (170) */
/* 208 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 210 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 212 */	NdrFcShort( 0x8 ),	/* 8 */
/* 214 */	NdrFcShort( 0x0 ),	/* 0 */
/* 216 */	NdrFcShort( 0x6 ),	/* Offset= 6 (222) */
/* 218 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 220 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 222 */	
			0x11, 0x0,	/* FC_RP */
/* 224 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (188) */
/* 226 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 228 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 232 */	NdrFcShort( 0x0 ),	/* 0 */
/* 234 */	NdrFcShort( 0x0 ),	/* 0 */
/* 236 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 238 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 240 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 242 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 244 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 246 */	NdrFcShort( 0x0 ),	/* 0 */
/* 248 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 250 */	NdrFcShort( 0x0 ),	/* 0 */
/* 252 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 254 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 258 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 260 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 262 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (226) */
/* 264 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 266 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 268 */	NdrFcShort( 0x8 ),	/* 8 */
/* 270 */	NdrFcShort( 0x0 ),	/* 0 */
/* 272 */	NdrFcShort( 0x6 ),	/* Offset= 6 (278) */
/* 274 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 276 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 278 */	
			0x11, 0x0,	/* FC_RP */
/* 280 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (244) */
/* 282 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 284 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 286 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 288 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 290 */	NdrFcShort( 0x2 ),	/* Offset= 2 (292) */
/* 292 */	NdrFcShort( 0x10 ),	/* 16 */
/* 294 */	NdrFcShort( 0x2f ),	/* 47 */
/* 296 */	NdrFcLong( 0x14 ),	/* 20 */
/* 300 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 302 */	NdrFcLong( 0x3 ),	/* 3 */
/* 306 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 308 */	NdrFcLong( 0x11 ),	/* 17 */
/* 312 */	NdrFcShort( 0x8001 ),	/* Simple arm type: FC_BYTE */
/* 314 */	NdrFcLong( 0x2 ),	/* 2 */
/* 318 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 320 */	NdrFcLong( 0x4 ),	/* 4 */
/* 324 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 326 */	NdrFcLong( 0x5 ),	/* 5 */
/* 330 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 332 */	NdrFcLong( 0xb ),	/* 11 */
/* 336 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 338 */	NdrFcLong( 0xa ),	/* 10 */
/* 342 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 344 */	NdrFcLong( 0x6 ),	/* 6 */
/* 348 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (580) */
/* 350 */	NdrFcLong( 0x7 ),	/* 7 */
/* 354 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 356 */	NdrFcLong( 0x8 ),	/* 8 */
/* 360 */	NdrFcShort( 0xe2 ),	/* Offset= 226 (586) */
/* 362 */	NdrFcLong( 0xd ),	/* 13 */
/* 366 */	NdrFcShort( 0xff3c ),	/* Offset= -196 (170) */
/* 368 */	NdrFcLong( 0x9 ),	/* 9 */
/* 372 */	NdrFcShort( 0xff6e ),	/* Offset= -146 (226) */
/* 374 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 378 */	NdrFcShort( 0xd4 ),	/* Offset= 212 (590) */
/* 380 */	NdrFcLong( 0x24 ),	/* 36 */
/* 384 */	NdrFcShort( 0xd6 ),	/* Offset= 214 (598) */
/* 386 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 390 */	NdrFcShort( 0xd0 ),	/* Offset= 208 (598) */
/* 392 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 396 */	NdrFcShort( 0x100 ),	/* Offset= 256 (652) */
/* 398 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 402 */	NdrFcShort( 0xfe ),	/* Offset= 254 (656) */
/* 404 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 408 */	NdrFcShort( 0xfc ),	/* Offset= 252 (660) */
/* 410 */	NdrFcLong( 0x4014 ),	/* 16404 */
/* 414 */	NdrFcShort( 0xfa ),	/* Offset= 250 (664) */
/* 416 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 420 */	NdrFcShort( 0xf8 ),	/* Offset= 248 (668) */
/* 422 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 426 */	NdrFcShort( 0xf6 ),	/* Offset= 246 (672) */
/* 428 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 432 */	NdrFcShort( 0xe0 ),	/* Offset= 224 (656) */
/* 434 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 438 */	NdrFcShort( 0xde ),	/* Offset= 222 (660) */
/* 440 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 444 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (676) */
/* 446 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 450 */	NdrFcShort( 0xde ),	/* Offset= 222 (672) */
/* 452 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 456 */	NdrFcShort( 0xe0 ),	/* Offset= 224 (680) */
/* 458 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 462 */	NdrFcShort( 0xde ),	/* Offset= 222 (684) */
/* 464 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 468 */	NdrFcShort( 0xdc ),	/* Offset= 220 (688) */
/* 470 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 474 */	NdrFcShort( 0xda ),	/* Offset= 218 (692) */
/* 476 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 480 */	NdrFcShort( 0xe0 ),	/* Offset= 224 (704) */
/* 482 */	NdrFcLong( 0x10 ),	/* 16 */
/* 486 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 488 */	NdrFcLong( 0x12 ),	/* 18 */
/* 492 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 494 */	NdrFcLong( 0x13 ),	/* 19 */
/* 498 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 500 */	NdrFcLong( 0x15 ),	/* 21 */
/* 504 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 506 */	NdrFcLong( 0x16 ),	/* 22 */
/* 510 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 512 */	NdrFcLong( 0x17 ),	/* 23 */
/* 516 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 518 */	NdrFcLong( 0xe ),	/* 14 */
/* 522 */	NdrFcShort( 0xbe ),	/* Offset= 190 (712) */
/* 524 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 528 */	NdrFcShort( 0xc2 ),	/* Offset= 194 (722) */
/* 530 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 534 */	NdrFcShort( 0xc0 ),	/* Offset= 192 (726) */
/* 536 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 540 */	NdrFcShort( 0x74 ),	/* Offset= 116 (656) */
/* 542 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 546 */	NdrFcShort( 0x72 ),	/* Offset= 114 (660) */
/* 548 */	NdrFcLong( 0x4015 ),	/* 16405 */
/* 552 */	NdrFcShort( 0x70 ),	/* Offset= 112 (664) */
/* 554 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 558 */	NdrFcShort( 0x66 ),	/* Offset= 102 (660) */
/* 560 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 564 */	NdrFcShort( 0x60 ),	/* Offset= 96 (660) */
/* 566 */	NdrFcLong( 0x0 ),	/* 0 */
/* 570 */	NdrFcShort( 0x0 ),	/* Offset= 0 (570) */
/* 572 */	NdrFcLong( 0x1 ),	/* 1 */
/* 576 */	NdrFcShort( 0x0 ),	/* Offset= 0 (576) */
/* 578 */	NdrFcShort( 0xffff ),	/* Offset= -1 (577) */
/* 580 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 582 */	NdrFcShort( 0x8 ),	/* 8 */
/* 584 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 586 */	
			0x13, 0x0,	/* FC_OP */
/* 588 */	NdrFcShort( 0xfdc6 ),	/* Offset= -570 (18) */
/* 590 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 592 */	NdrFcShort( 0x2 ),	/* Offset= 2 (594) */
/* 594 */	
			0x13, 0x0,	/* FC_OP */
/* 596 */	NdrFcShort( 0x1b8 ),	/* Offset= 440 (1036) */
/* 598 */	
			0x13, 0x0,	/* FC_OP */
/* 600 */	NdrFcShort( 0x20 ),	/* Offset= 32 (632) */
/* 602 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 604 */	NdrFcLong( 0x2f ),	/* 47 */
/* 608 */	NdrFcShort( 0x0 ),	/* 0 */
/* 610 */	NdrFcShort( 0x0 ),	/* 0 */
/* 612 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 614 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 616 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 618 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 620 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 622 */	NdrFcShort( 0x1 ),	/* 1 */
/* 624 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 626 */	NdrFcShort( 0x4 ),	/* 4 */
/* 628 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 630 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 632 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 634 */	NdrFcShort( 0x10 ),	/* 16 */
/* 636 */	NdrFcShort( 0x0 ),	/* 0 */
/* 638 */	NdrFcShort( 0xa ),	/* Offset= 10 (648) */
/* 640 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 642 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 644 */	NdrFcShort( 0xffd6 ),	/* Offset= -42 (602) */
/* 646 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 648 */	
			0x13, 0x0,	/* FC_OP */
/* 650 */	NdrFcShort( 0xffe2 ),	/* Offset= -30 (620) */
/* 652 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 654 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 656 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 658 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 660 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 662 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 664 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 666 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 668 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 670 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 672 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 674 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 676 */	
			0x13, 0x0,	/* FC_OP */
/* 678 */	NdrFcShort( 0xff9e ),	/* Offset= -98 (580) */
/* 680 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 682 */	NdrFcShort( 0xffa0 ),	/* Offset= -96 (586) */
/* 684 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 686 */	NdrFcShort( 0xfdfc ),	/* Offset= -516 (170) */
/* 688 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 690 */	NdrFcShort( 0xfe30 ),	/* Offset= -464 (226) */
/* 692 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 694 */	NdrFcShort( 0x2 ),	/* Offset= 2 (696) */
/* 696 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 698 */	NdrFcShort( 0x2 ),	/* Offset= 2 (700) */
/* 700 */	
			0x13, 0x0,	/* FC_OP */
/* 702 */	NdrFcShort( 0x14e ),	/* Offset= 334 (1036) */
/* 704 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 706 */	NdrFcShort( 0x2 ),	/* Offset= 2 (708) */
/* 708 */	
			0x13, 0x0,	/* FC_OP */
/* 710 */	NdrFcShort( 0x14 ),	/* Offset= 20 (730) */
/* 712 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 714 */	NdrFcShort( 0x10 ),	/* 16 */
/* 716 */	0x6,		/* FC_SHORT */
			0x1,		/* FC_BYTE */
/* 718 */	0x1,		/* FC_BYTE */
			0x8,		/* FC_LONG */
/* 720 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 722 */	
			0x13, 0x0,	/* FC_OP */
/* 724 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (712) */
/* 726 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 728 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 730 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 732 */	NdrFcShort( 0x20 ),	/* 32 */
/* 734 */	NdrFcShort( 0x0 ),	/* 0 */
/* 736 */	NdrFcShort( 0x0 ),	/* Offset= 0 (736) */
/* 738 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 740 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 742 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 744 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 746 */	NdrFcShort( 0xfe30 ),	/* Offset= -464 (282) */
/* 748 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 750 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 752 */	NdrFcShort( 0x4 ),	/* 4 */
/* 754 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 756 */	NdrFcShort( 0x0 ),	/* 0 */
/* 758 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 760 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 762 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 764 */	NdrFcShort( 0x4 ),	/* 4 */
/* 766 */	NdrFcShort( 0x0 ),	/* 0 */
/* 768 */	NdrFcShort( 0x1 ),	/* 1 */
/* 770 */	NdrFcShort( 0x0 ),	/* 0 */
/* 772 */	NdrFcShort( 0x0 ),	/* 0 */
/* 774 */	0x13, 0x0,	/* FC_OP */
/* 776 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (730) */
/* 778 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 780 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 782 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 784 */	NdrFcShort( 0x8 ),	/* 8 */
/* 786 */	NdrFcShort( 0x0 ),	/* 0 */
/* 788 */	NdrFcShort( 0x6 ),	/* Offset= 6 (794) */
/* 790 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 792 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 794 */	
			0x11, 0x0,	/* FC_RP */
/* 796 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (750) */
/* 798 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 800 */	NdrFcShort( 0x4 ),	/* 4 */
/* 802 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 804 */	NdrFcShort( 0x0 ),	/* 0 */
/* 806 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 808 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 810 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 812 */	NdrFcShort( 0x4 ),	/* 4 */
/* 814 */	NdrFcShort( 0x0 ),	/* 0 */
/* 816 */	NdrFcShort( 0x1 ),	/* 1 */
/* 818 */	NdrFcShort( 0x0 ),	/* 0 */
/* 820 */	NdrFcShort( 0x0 ),	/* 0 */
/* 822 */	0x13, 0x0,	/* FC_OP */
/* 824 */	NdrFcShort( 0xff40 ),	/* Offset= -192 (632) */
/* 826 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 828 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 830 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 832 */	NdrFcShort( 0x8 ),	/* 8 */
/* 834 */	NdrFcShort( 0x0 ),	/* 0 */
/* 836 */	NdrFcShort( 0x6 ),	/* Offset= 6 (842) */
/* 838 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 840 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 842 */	
			0x11, 0x0,	/* FC_RP */
/* 844 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (798) */
/* 846 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 848 */	NdrFcShort( 0x8 ),	/* 8 */
/* 850 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 852 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 854 */	NdrFcShort( 0x10 ),	/* 16 */
/* 856 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 858 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 860 */	0x0,		/* 0 */
			NdrFcShort( 0xfff1 ),	/* Offset= -15 (846) */
			0x5b,		/* FC_END */
/* 864 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 866 */	NdrFcShort( 0x18 ),	/* 24 */
/* 868 */	NdrFcShort( 0x0 ),	/* 0 */
/* 870 */	NdrFcShort( 0xa ),	/* Offset= 10 (880) */
/* 872 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 874 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 876 */	NdrFcShort( 0xffe8 ),	/* Offset= -24 (852) */
/* 878 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 880 */	
			0x11, 0x0,	/* FC_RP */
/* 882 */	NdrFcShort( 0xfd4a ),	/* Offset= -694 (188) */
/* 884 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 886 */	NdrFcShort( 0x1 ),	/* 1 */
/* 888 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 890 */	NdrFcShort( 0x0 ),	/* 0 */
/* 892 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 894 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 896 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 898 */	NdrFcShort( 0x8 ),	/* 8 */
/* 900 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 902 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 904 */	NdrFcShort( 0x4 ),	/* 4 */
/* 906 */	NdrFcShort( 0x4 ),	/* 4 */
/* 908 */	0x13, 0x0,	/* FC_OP */
/* 910 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (884) */
/* 912 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 914 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 916 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 918 */	NdrFcShort( 0x2 ),	/* 2 */
/* 920 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 922 */	NdrFcShort( 0x0 ),	/* 0 */
/* 924 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 926 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 928 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 930 */	NdrFcShort( 0x8 ),	/* 8 */
/* 932 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 934 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 936 */	NdrFcShort( 0x4 ),	/* 4 */
/* 938 */	NdrFcShort( 0x4 ),	/* 4 */
/* 940 */	0x13, 0x0,	/* FC_OP */
/* 942 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (916) */
/* 944 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 946 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 948 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 950 */	NdrFcShort( 0x4 ),	/* 4 */
/* 952 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 954 */	NdrFcShort( 0x0 ),	/* 0 */
/* 956 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 958 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 960 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 962 */	NdrFcShort( 0x8 ),	/* 8 */
/* 964 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 966 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 968 */	NdrFcShort( 0x4 ),	/* 4 */
/* 970 */	NdrFcShort( 0x4 ),	/* 4 */
/* 972 */	0x13, 0x0,	/* FC_OP */
/* 974 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (948) */
/* 976 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 978 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 980 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 982 */	NdrFcShort( 0x8 ),	/* 8 */
/* 984 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 986 */	NdrFcShort( 0x0 ),	/* 0 */
/* 988 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 990 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 992 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 994 */	NdrFcShort( 0x8 ),	/* 8 */
/* 996 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 998 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 1000 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1002 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1004 */	0x13, 0x0,	/* FC_OP */
/* 1006 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (980) */
/* 1008 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1010 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1012 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 1014 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1016 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1018 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1020 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1022 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1024 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 1026 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 1028 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1030 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1032 */	NdrFcShort( 0xffec ),	/* Offset= -20 (1012) */
/* 1034 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1036 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1038 */	NdrFcShort( 0x28 ),	/* 40 */
/* 1040 */	NdrFcShort( 0xffec ),	/* Offset= -20 (1020) */
/* 1042 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1042) */
/* 1044 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1046 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1048 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1050 */	NdrFcShort( 0xfc18 ),	/* Offset= -1000 (50) */
/* 1052 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1054 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1056 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1058 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1060 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1062 */	NdrFcShort( 0xfc04 ),	/* Offset= -1020 (42) */
/* 1064 */	
			0x11, 0x0,	/* FC_RP */
/* 1066 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1072) */
/* 1068 */	
			0x13, 0x0,	/* FC_OP */
/* 1070 */	NdrFcShort( 0xfeac ),	/* Offset= -340 (730) */
/* 1072 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1074 */	NdrFcShort( 0x2 ),	/* 2 */
/* 1076 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1078 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1080 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (1068) */
/* 1082 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 1084 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 1086 */	
			0x11, 0x0,	/* FC_RP */
/* 1088 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1090) */
/* 1090 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x1,		/* 1 */
/* 1092 */	NdrFcShort( 0xc ),	/* 12 */
/* 1094 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1096 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1096) */
/* 1098 */	0xd,		/* FC_ENUM16 */
			0x6,		/* FC_SHORT */
/* 1100 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1102 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 1104 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 1106 */	0xd,		/* FC_ENUM16 */
			0x5c,		/* FC_PAD */
/* 1108 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 1110 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1112) */
/* 1112 */	
			0x13, 0x0,	/* FC_OP */
/* 1114 */	NdrFcShort( 0xa ),	/* Offset= 10 (1124) */
/* 1116 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 1118 */	NdrFcShort( 0xc ),	/* 12 */
/* 1120 */	0xa,		/* FC_FLOAT */
			0xa,		/* FC_FLOAT */
/* 1122 */	0xa,		/* FC_FLOAT */
			0x5b,		/* FC_END */
/* 1124 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 1126 */	NdrFcShort( 0x14 ),	/* 20 */
/* 1128 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1130 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1132 */	NdrFcShort( 0xfff0 ),	/* Offset= -16 (1116) */
/* 1134 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1136 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 1138 */	NdrFcShort( 0xffea ),	/* Offset= -22 (1116) */
/* 1140 */	
			0x11, 0x0,	/* FC_RP */
/* 1142 */	NdrFcShort( 0x36 ),	/* Offset= 54 (1196) */
/* 1144 */	
			0x15,		/* FC_STRUCT */
			0x1,		/* 1 */
/* 1146 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1148 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1150 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1152 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 1154 */	NdrFcShort( 0x1c ),	/* 28 */
/* 1156 */	0xa,		/* FC_FLOAT */
			0xa,		/* FC_FLOAT */
/* 1158 */	0xa,		/* FC_FLOAT */
			0xa,		/* FC_FLOAT */
/* 1160 */	0xa,		/* FC_FLOAT */
			0xa,		/* FC_FLOAT */
/* 1162 */	0xa,		/* FC_FLOAT */
			0x5b,		/* FC_END */
/* 1164 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1166 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1168 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1170 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1172 */	NdrFcShort( 0xfdb6 ),	/* Offset= -586 (586) */
/* 1174 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 1176 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1178 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1180 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1180) */
/* 1182 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1184 */	NdrFcShort( 0xffec ),	/* Offset= -20 (1164) */
/* 1186 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1188 */	NdrFcShort( 0xffe8 ),	/* Offset= -24 (1164) */
/* 1190 */	0x2,		/* FC_CHAR */
			0x3f,		/* FC_STRUCTPAD3 */
/* 1192 */	0x8,		/* FC_LONG */
			0xb,		/* FC_HYPER */
/* 1194 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1196 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 1198 */	NdrFcShort( 0x40 ),	/* 64 */
/* 1200 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1202 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1202) */
/* 1204 */	0xc,		/* FC_DOUBLE */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 1206 */	0x0,		/* 0 */
			NdrFcShort( 0xffc1 ),	/* Offset= -63 (1144) */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 1210 */	0x0,		/* 0 */
			NdrFcShort( 0xffc5 ),	/* Offset= -59 (1152) */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 1214 */	0x0,		/* 0 */
			NdrFcShort( 0xffd7 ),	/* Offset= -41 (1174) */
			0x5b,		/* FC_END */
/* 1218 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 1220 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 1222 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 1224 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 1226 */	
			0x12, 0x0,	/* FC_UP */
/* 1228 */	NdrFcShort( 0xfe0e ),	/* Offset= -498 (730) */
/* 1230 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1232 */	NdrFcShort( 0x2 ),	/* 2 */
/* 1234 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1236 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1238 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (1226) */
/* 1240 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 1242 */	NdrFcShort( 0xffa6 ),	/* Offset= -90 (1152) */
/* 1244 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 1246 */	NdrFcShort( 0xffb8 ),	/* Offset= -72 (1174) */
/* 1248 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 1250 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            },
            {
            LPSAFEARRAY_UserSize
            ,LPSAFEARRAY_UserMarshal
            ,LPSAFEARRAY_UserUnmarshal
            ,LPSAFEARRAY_UserFree
            },
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            }

        };



/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IComponentRegistrar, ver. 0.0,
   GUID={0xa817e7a2,0x43fa,0x11d0,{0x9e,0x44,0x00,0xaa,0x00,0xb6,0x77,0x0a}} */

#pragma code_seg(".orpc")
static const unsigned short IComponentRegistrar_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    36,
    66,
    96,
    138,
    174
    };

static const MIDL_STUBLESS_PROXY_INFO IComponentRegistrar_ProxyInfo =
    {
    &Object_StubDesc,
    ThermoRoutine__MIDL_ProcFormatString.Format,
    &IComponentRegistrar_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IComponentRegistrar_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    ThermoRoutine__MIDL_ProcFormatString.Format,
    &IComponentRegistrar_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(13) _IComponentRegistrarProxyVtbl = 
{
    &IComponentRegistrar_ProxyInfo,
    &IID_IComponentRegistrar,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IComponentRegistrar::Attach */ ,
    (void *) (INT_PTR) -1 /* IComponentRegistrar::RegisterAll */ ,
    (void *) (INT_PTR) -1 /* IComponentRegistrar::UnregisterAll */ ,
    (void *) (INT_PTR) -1 /* IComponentRegistrar::GetComponents */ ,
    (void *) (INT_PTR) -1 /* IComponentRegistrar::RegisterComponent */ ,
    (void *) (INT_PTR) -1 /* IComponentRegistrar::UnregisterComponent */
};


static const PRPC_STUB_FUNCTION IComponentRegistrar_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IComponentRegistrarStubVtbl =
{
    &IID_IComponentRegistrar,
    &IComponentRegistrar_ServerInfo,
    13,
    &IComponentRegistrar_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_ThermoRoutine_0000_0001, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IPallete, ver. 0.0,
   GUID={0x859E7C6E,0x270C,0x41F2,{0xAE,0x3A,0x7C,0xBD,0xDF,0xFA,0x30,0xAF}} */

#pragma code_seg(".orpc")
static const unsigned short IPallete_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    36,
    210,
    246
    };

static const MIDL_STUBLESS_PROXY_INFO IPallete_ProxyInfo =
    {
    &Object_StubDesc,
    ThermoRoutine__MIDL_ProcFormatString.Format,
    &IPallete_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPallete_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    ThermoRoutine__MIDL_ProcFormatString.Format,
    &IPallete_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(11) _IPalleteProxyVtbl = 
{
    &IPallete_ProxyInfo,
    &IID_IPallete,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IPallete::SetPallete */ ,
    (void *) (INT_PTR) -1 /* IPallete::SetDefaultPallete */ ,
    (void *) (INT_PTR) -1 /* IPallete::GetPallete */ ,
    (void *) (INT_PTR) -1 /* IPallete::GetPalleteLength */
};


static const PRPC_STUB_FUNCTION IPallete_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPalleteStubVtbl =
{
    &IID_IPallete,
    &IPallete_ServerInfo,
    11,
    &IPallete_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IArea, ver. 0.0,
   GUID={0x03BA3F31,0xA54D,0x443E,{0xAE,0x88,0x2C,0x55,0x6E,0xA1,0x96,0xFA}} */

#pragma code_seg(".orpc")
static const unsigned short IArea_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    282,
    324,
    366,
    408,
    450,
    498
    };

static const MIDL_STUBLESS_PROXY_INFO IArea_ProxyInfo =
    {
    &Object_StubDesc,
    ThermoRoutine__MIDL_ProcFormatString.Format,
    &IArea_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IArea_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    ThermoRoutine__MIDL_ProcFormatString.Format,
    &IArea_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(13) _IAreaProxyVtbl = 
{
    &IArea_ProxyInfo,
    &IID_IArea,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IArea::AddArea */ ,
    (void *) (INT_PTR) -1 /* IArea::AreaChanged */ ,
    (void *) (INT_PTR) -1 /* IArea::RemoveArea */ ,
    (void *) (INT_PTR) -1 /* IArea::GetAreasInfo */ ,
    (void *) (INT_PTR) -1 /* IArea::GetAreaInfo */ ,
    (void *) (INT_PTR) -1 /* IArea::RemoveAllAreas */
};


static const PRPC_STUB_FUNCTION IArea_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IAreaStubVtbl =
{
    &IID_IArea,
    &IArea_ServerInfo,
    13,
    &IArea_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: ITRWrapper, ver. 0.0,
   GUID={0x1347B40A,0xBFBD,0x4A82,{0x8A,0x75,0x46,0xA2,0x55,0x85,0x07,0xBA}} */

#pragma code_seg(".orpc")
static const unsigned short ITRWrapper_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    528,
    558,
    66,
    600,
    642,
    678,
    744,
    774,
    804,
    840,
    870,
    900,
    936,
    990,
    1044,
    1104,
    1164,
    1224,
    1266,
    1302,
    1332,
    1368,
    1398,
    1434,
    1476,
    1512,
    1554,
    1596,
    1638,
    1680,
    1728,
    1758,
    1794,
    1830
    };

static const MIDL_STUBLESS_PROXY_INFO ITRWrapper_ProxyInfo =
    {
    &Object_StubDesc,
    ThermoRoutine__MIDL_ProcFormatString.Format,
    &ITRWrapper_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO ITRWrapper_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    ThermoRoutine__MIDL_ProcFormatString.Format,
    &ITRWrapper_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(41) _ITRWrapperProxyVtbl = 
{
    &ITRWrapper_ProxyInfo,
    &IID_ITRWrapper,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::ClearGrabbingCache */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::StopGrabbing */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::StartRecord */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::StopRecord */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::SetBlockCamFrame */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::StartRecieveCoordinates */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::StopRecieveCoordinates */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::StartGrabbing */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::SetGrabberPath */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::ShowCamSettings */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::HideCamSettings */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::GetGrabberSources */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::GetRealTimeFrameRaster */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::GetNextRealTimeFrameRaster */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::GetRealTimeFrame */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::GetNextRealTimeFrame */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::get_pixel_temperature */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::ConnectCamera */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::DisconnectCamera */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::FinishAll */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::SetPallete */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::SetDefaultPallete */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::GetPallete */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::GetPalleteLength */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::SetPaletteCalibrationMode */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::SetPaletteCalibration */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::AddArea */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::AreaChanged */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::RemoveArea */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::GetAreaInfo */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::RemoveAllAreas */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::SetMaxFramesInIRBFile */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::SetCounterSize */ ,
    (void *) (INT_PTR) -1 /* ITRWrapper::SetCameraOffset */
};


static const PRPC_STUB_FUNCTION ITRWrapper_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _ITRWrapperStubVtbl =
{
    &IID_ITRWrapper,
    &ITRWrapper_ServerInfo,
    41,
    &ITRWrapper_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IMovieTransit, ver. 0.0,
   GUID={0xE37EF668,0x25E2,0x4784,{0xBF,0xF3,0x64,0x2A,0xA9,0x53,0xFC,0xAA}} */

#pragma code_seg(".orpc")
static const unsigned short IMovieTransit_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1866,
    1914,
    1950,
    1986,
    2040,
    2100,
    744,
    2160,
    2202,
    2262,
    2298,
    2340,
    2388,
    2430,
    2460,
    2544,
    2622,
    2658,
    2688,
    2724,
    2766,
    2802,
    2844,
    2886,
    2928,
    2970,
    3012,
    3060,
    3090
    };

static const MIDL_STUBLESS_PROXY_INFO IMovieTransit_ProxyInfo =
    {
    &Object_StubDesc,
    ThermoRoutine__MIDL_ProcFormatString.Format,
    &IMovieTransit_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IMovieTransit_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    ThermoRoutine__MIDL_ProcFormatString.Format,
    &IMovieTransit_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(36) _IMovieTransitProxyVtbl = 
{
    &IMovieTransit_ProxyInfo,
    &IID_IMovieTransit,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::SetIRBFiles */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::FilesCount */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::FramesCount */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::GetFrameRaster */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::GetFrame */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::get_pixel_temperature */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::Close */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::SaveCurrentFrame */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::SaveFrame */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::GetCurFrameTemperatures */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::IsFrameMeetFilter */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::GetFramePositionInfo */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::GetCurrentFramePositionInfo */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::ClearMovieTransitCache */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::InitFrameFilter */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::SetFrameFilter */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::SetPallete */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::SetDefaultPallete */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::GetPallete */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::GetPalleteLength */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::SetPaletteCalibrationMode */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::SetPaletteCalibration */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::AddArea */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::AreaChanged */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::RemoveArea */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::GetAreasInfo */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::GetAreaInfo */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::RemoveAllAreas */ ,
    (void *) (INT_PTR) -1 /* IMovieTransit::WriteCameraOffset */
};


static const PRPC_STUB_FUNCTION IMovieTransit_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IMovieTransitStubVtbl =
{
    &IID_IMovieTransit,
    &IMovieTransit_ServerInfo,
    36,
    &IMovieTransit_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    ThermoRoutine__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x800025b, /* MIDL Version 8.0.603 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * const _ThermoRoutine_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_ITRWrapperProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IAreaProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IMovieTransitProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPalleteProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IComponentRegistrarProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _ThermoRoutine_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_ITRWrapperStubVtbl,
    ( CInterfaceStubVtbl *) &_IAreaStubVtbl,
    ( CInterfaceStubVtbl *) &_IMovieTransitStubVtbl,
    ( CInterfaceStubVtbl *) &_IPalleteStubVtbl,
    ( CInterfaceStubVtbl *) &_IComponentRegistrarStubVtbl,
    0
};

PCInterfaceName const _ThermoRoutine_InterfaceNamesList[] = 
{
    "ITRWrapper",
    "IArea",
    "IMovieTransit",
    "IPallete",
    "IComponentRegistrar",
    0
};

const IID *  const _ThermoRoutine_BaseIIDList[] = 
{
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    0
};


#define _ThermoRoutine_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _ThermoRoutine, pIID, n)

int __stdcall _ThermoRoutine_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _ThermoRoutine, 5, 4 )
    IID_BS_LOOKUP_NEXT_TEST( _ThermoRoutine, 2 )
    IID_BS_LOOKUP_NEXT_TEST( _ThermoRoutine, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _ThermoRoutine, 5, *pIndex )
    
}

const ExtendedProxyFileInfo ThermoRoutine_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _ThermoRoutine_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _ThermoRoutine_StubVtblList,
    (const PCInterfaceName * ) & _ThermoRoutine_InterfaceNamesList,
    (const IID ** ) & _ThermoRoutine_BaseIIDList,
    & _ThermoRoutine_IID_Lookup, 
    5,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_) */

