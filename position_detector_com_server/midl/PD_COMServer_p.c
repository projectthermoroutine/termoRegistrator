

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Sun Mar 08 16:33:46 2015
 */
/* Compiler settings for ..\PD_COMServer.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif /* __RPCPROXY_H_VERSION__ */


#include "PD_COMServer_i.h"

#define TYPE_FORMAT_STRING_SIZE   1031                              
#define PROC_FORMAT_STRING_SIZE   177                               
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   2            

typedef struct _PD_COMServer_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } PD_COMServer_MIDL_TYPE_FORMAT_STRING;

typedef struct _PD_COMServer_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } PD_COMServer_MIDL_PROC_FORMAT_STRING;

typedef struct _PD_COMServer_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } PD_COMServer_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const PD_COMServer_MIDL_TYPE_FORMAT_STRING PD_COMServer__MIDL_TypeFormatString;
extern const PD_COMServer_MIDL_PROC_FORMAT_STRING PD_COMServer__MIDL_ProcFormatString;
extern const PD_COMServer_MIDL_EXPR_FORMAT_STRING PD_COMServer__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IProxyPD_Dispatcher_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IProxyPD_Dispatcher_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif

static const PD_COMServer_MIDL_PROC_FORMAT_STRING PD_COMServer__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure getConfig */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x24 ),	/* 36 */
/* 14 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x4,		/* 4 */
/* 16 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 18 */	NdrFcShort( 0x1 ),	/* 1 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */
/* 24 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter syncSettings */

/* 26 */	NdrFcShort( 0x6113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=24 */
/* 28 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 30 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter eventSettings */

/* 32 */	NdrFcShort( 0x6113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=24 */
/* 34 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 36 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter clientId */

/* 38 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 40 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 42 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 44 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 46 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 48 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure setConfig */

/* 50 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 52 */	NdrFcLong( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x8 ),	/* 8 */
/* 58 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 60 */	NdrFcShort( 0x0 ),	/* 0 */
/* 62 */	NdrFcShort( 0x8 ),	/* 8 */
/* 64 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 66 */	0xa,		/* 10 */
			0x85,		/* Ext Flags:  new corr desc, srv corr check, has big amd64 byval param */
/* 68 */	NdrFcShort( 0x0 ),	/* 0 */
/* 70 */	NdrFcShort( 0x1 ),	/* 1 */
/* 72 */	NdrFcShort( 0x0 ),	/* 0 */
/* 74 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter Arr */

/* 76 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 78 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 80 */	NdrFcShort( 0x3fc ),	/* Type Offset=1020 */

	/* Return value */

/* 82 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 84 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 86 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure connectToErrorsStream */

/* 88 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 90 */	NdrFcLong( 0x0 ),	/* 0 */
/* 94 */	NdrFcShort( 0x9 ),	/* 9 */
/* 96 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 98 */	NdrFcShort( 0x0 ),	/* 0 */
/* 100 */	NdrFcShort( 0x24 ),	/* 36 */
/* 102 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 104 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 106 */	NdrFcShort( 0x1 ),	/* 1 */
/* 108 */	NdrFcShort( 0x0 ),	/* 0 */
/* 110 */	NdrFcShort( 0x0 ),	/* 0 */
/* 112 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter errStream */

/* 114 */	NdrFcShort( 0x6113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=24 */
/* 116 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 118 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Parameter clientId */

/* 120 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 122 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 124 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 126 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 128 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 130 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure disconnectClient */

/* 132 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 134 */	NdrFcLong( 0x0 ),	/* 0 */
/* 138 */	NdrFcShort( 0xa ),	/* 10 */
/* 140 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 142 */	NdrFcShort( 0x10 ),	/* 16 */
/* 144 */	NdrFcShort( 0x8 ),	/* 8 */
/* 146 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 148 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 150 */	NdrFcShort( 0x0 ),	/* 0 */
/* 152 */	NdrFcShort( 0x0 ),	/* 0 */
/* 154 */	NdrFcShort( 0x0 ),	/* 0 */
/* 156 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter clientId */

/* 158 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 160 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 162 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter errorsClientId */

/* 164 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 166 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 168 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 170 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 172 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 174 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const PD_COMServer_MIDL_TYPE_FORMAT_STRING PD_COMServer__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/*  4 */	NdrFcShort( 0x26 ),	/* Offset= 38 (42) */
/*  6 */	
			0x13, 0x0,	/* FC_OP */
/*  8 */	NdrFcShort( 0xe ),	/* Offset= 14 (22) */
/* 10 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 12 */	NdrFcShort( 0x2 ),	/* 2 */
/* 14 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 16 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 18 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 20 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 22 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 24 */	NdrFcShort( 0x8 ),	/* 8 */
/* 26 */	NdrFcShort( 0xfff0 ),	/* Offset= -16 (10) */
/* 28 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 30 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 32 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 34 */	NdrFcShort( 0x0 ),	/* 0 */
/* 36 */	NdrFcShort( 0x8 ),	/* 8 */
/* 38 */	NdrFcShort( 0x0 ),	/* 0 */
/* 40 */	NdrFcShort( 0xffde ),	/* Offset= -34 (6) */
/* 42 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 44 */	NdrFcShort( 0x18 ),	/* 24 */
/* 46 */	NdrFcShort( 0x0 ),	/* 0 */
/* 48 */	NdrFcShort( 0x0 ),	/* Offset= 0 (48) */
/* 50 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 52 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 54 */	NdrFcShort( 0xffea ),	/* Offset= -22 (32) */
/* 56 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 58 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (32) */
/* 60 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 62 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 64 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 66 */	
			0x11, 0x0,	/* FC_RP */
/* 68 */	NdrFcShort( 0x3b8 ),	/* Offset= 952 (1020) */
/* 70 */	
			0x12, 0x0,	/* FC_UP */
/* 72 */	NdrFcShort( 0x3a0 ),	/* Offset= 928 (1000) */
/* 74 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 76 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 78 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 80 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 82 */	NdrFcShort( 0x2 ),	/* Offset= 2 (84) */
/* 84 */	NdrFcShort( 0x10 ),	/* 16 */
/* 86 */	NdrFcShort( 0x2f ),	/* 47 */
/* 88 */	NdrFcLong( 0x14 ),	/* 20 */
/* 92 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 94 */	NdrFcLong( 0x3 ),	/* 3 */
/* 98 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 100 */	NdrFcLong( 0x11 ),	/* 17 */
/* 104 */	NdrFcShort( 0x8001 ),	/* Simple arm type: FC_BYTE */
/* 106 */	NdrFcLong( 0x2 ),	/* 2 */
/* 110 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 112 */	NdrFcLong( 0x4 ),	/* 4 */
/* 116 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 118 */	NdrFcLong( 0x5 ),	/* 5 */
/* 122 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 124 */	NdrFcLong( 0xb ),	/* 11 */
/* 128 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 130 */	NdrFcLong( 0xa ),	/* 10 */
/* 134 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 136 */	NdrFcLong( 0x6 ),	/* 6 */
/* 140 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (372) */
/* 142 */	NdrFcLong( 0x7 ),	/* 7 */
/* 146 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 148 */	NdrFcLong( 0x8 ),	/* 8 */
/* 152 */	NdrFcShort( 0xe2 ),	/* Offset= 226 (378) */
/* 154 */	NdrFcLong( 0xd ),	/* 13 */
/* 158 */	NdrFcShort( 0xe0 ),	/* Offset= 224 (382) */
/* 160 */	NdrFcLong( 0x9 ),	/* 9 */
/* 164 */	NdrFcShort( 0xec ),	/* Offset= 236 (400) */
/* 166 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 170 */	NdrFcShort( 0xf8 ),	/* Offset= 248 (418) */
/* 172 */	NdrFcLong( 0x24 ),	/* 36 */
/* 176 */	NdrFcShort( 0x2ee ),	/* Offset= 750 (926) */
/* 178 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 182 */	NdrFcShort( 0x2e8 ),	/* Offset= 744 (926) */
/* 184 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 188 */	NdrFcShort( 0x2e6 ),	/* Offset= 742 (930) */
/* 190 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 194 */	NdrFcShort( 0x2e4 ),	/* Offset= 740 (934) */
/* 196 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 200 */	NdrFcShort( 0x2e2 ),	/* Offset= 738 (938) */
/* 202 */	NdrFcLong( 0x4014 ),	/* 16404 */
/* 206 */	NdrFcShort( 0x2e0 ),	/* Offset= 736 (942) */
/* 208 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 212 */	NdrFcShort( 0x2de ),	/* Offset= 734 (946) */
/* 214 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 218 */	NdrFcShort( 0x2dc ),	/* Offset= 732 (950) */
/* 220 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 224 */	NdrFcShort( 0x2c6 ),	/* Offset= 710 (934) */
/* 226 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 230 */	NdrFcShort( 0x2c4 ),	/* Offset= 708 (938) */
/* 232 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 236 */	NdrFcShort( 0x2ce ),	/* Offset= 718 (954) */
/* 238 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 242 */	NdrFcShort( 0x2c4 ),	/* Offset= 708 (950) */
/* 244 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 248 */	NdrFcShort( 0x2c6 ),	/* Offset= 710 (958) */
/* 250 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 254 */	NdrFcShort( 0x2c4 ),	/* Offset= 708 (962) */
/* 256 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 260 */	NdrFcShort( 0x2c2 ),	/* Offset= 706 (966) */
/* 262 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 266 */	NdrFcShort( 0x2c0 ),	/* Offset= 704 (970) */
/* 268 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 272 */	NdrFcShort( 0x2be ),	/* Offset= 702 (974) */
/* 274 */	NdrFcLong( 0x10 ),	/* 16 */
/* 278 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 280 */	NdrFcLong( 0x12 ),	/* 18 */
/* 284 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 286 */	NdrFcLong( 0x13 ),	/* 19 */
/* 290 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 292 */	NdrFcLong( 0x15 ),	/* 21 */
/* 296 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 298 */	NdrFcLong( 0x16 ),	/* 22 */
/* 302 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 304 */	NdrFcLong( 0x17 ),	/* 23 */
/* 308 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 310 */	NdrFcLong( 0xe ),	/* 14 */
/* 314 */	NdrFcShort( 0x29c ),	/* Offset= 668 (982) */
/* 316 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 320 */	NdrFcShort( 0x2a0 ),	/* Offset= 672 (992) */
/* 322 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 326 */	NdrFcShort( 0x29e ),	/* Offset= 670 (996) */
/* 328 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 332 */	NdrFcShort( 0x25a ),	/* Offset= 602 (934) */
/* 334 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 338 */	NdrFcShort( 0x258 ),	/* Offset= 600 (938) */
/* 340 */	NdrFcLong( 0x4015 ),	/* 16405 */
/* 344 */	NdrFcShort( 0x256 ),	/* Offset= 598 (942) */
/* 346 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 350 */	NdrFcShort( 0x24c ),	/* Offset= 588 (938) */
/* 352 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 356 */	NdrFcShort( 0x246 ),	/* Offset= 582 (938) */
/* 358 */	NdrFcLong( 0x0 ),	/* 0 */
/* 362 */	NdrFcShort( 0x0 ),	/* Offset= 0 (362) */
/* 364 */	NdrFcLong( 0x1 ),	/* 1 */
/* 368 */	NdrFcShort( 0x0 ),	/* Offset= 0 (368) */
/* 370 */	NdrFcShort( 0xffff ),	/* Offset= -1 (369) */
/* 372 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 374 */	NdrFcShort( 0x8 ),	/* 8 */
/* 376 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 378 */	
			0x12, 0x0,	/* FC_UP */
/* 380 */	NdrFcShort( 0xfe9a ),	/* Offset= -358 (22) */
/* 382 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 384 */	NdrFcLong( 0x0 ),	/* 0 */
/* 388 */	NdrFcShort( 0x0 ),	/* 0 */
/* 390 */	NdrFcShort( 0x0 ),	/* 0 */
/* 392 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 394 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 396 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 398 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 400 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 402 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 406 */	NdrFcShort( 0x0 ),	/* 0 */
/* 408 */	NdrFcShort( 0x0 ),	/* 0 */
/* 410 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 412 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 414 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 416 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 418 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 420 */	NdrFcShort( 0x2 ),	/* Offset= 2 (422) */
/* 422 */	
			0x12, 0x0,	/* FC_UP */
/* 424 */	NdrFcShort( 0x1e4 ),	/* Offset= 484 (908) */
/* 426 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x89,		/* 137 */
/* 428 */	NdrFcShort( 0x20 ),	/* 32 */
/* 430 */	NdrFcShort( 0xa ),	/* 10 */
/* 432 */	NdrFcLong( 0x8 ),	/* 8 */
/* 436 */	NdrFcShort( 0x50 ),	/* Offset= 80 (516) */
/* 438 */	NdrFcLong( 0xd ),	/* 13 */
/* 442 */	NdrFcShort( 0x70 ),	/* Offset= 112 (554) */
/* 444 */	NdrFcLong( 0x9 ),	/* 9 */
/* 448 */	NdrFcShort( 0x90 ),	/* Offset= 144 (592) */
/* 450 */	NdrFcLong( 0xc ),	/* 12 */
/* 454 */	NdrFcShort( 0xb0 ),	/* Offset= 176 (630) */
/* 456 */	NdrFcLong( 0x24 ),	/* 36 */
/* 460 */	NdrFcShort( 0x102 ),	/* Offset= 258 (718) */
/* 462 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 466 */	NdrFcShort( 0x11e ),	/* Offset= 286 (752) */
/* 468 */	NdrFcLong( 0x10 ),	/* 16 */
/* 472 */	NdrFcShort( 0x138 ),	/* Offset= 312 (784) */
/* 474 */	NdrFcLong( 0x2 ),	/* 2 */
/* 478 */	NdrFcShort( 0x14e ),	/* Offset= 334 (812) */
/* 480 */	NdrFcLong( 0x3 ),	/* 3 */
/* 484 */	NdrFcShort( 0x164 ),	/* Offset= 356 (840) */
/* 486 */	NdrFcLong( 0x14 ),	/* 20 */
/* 490 */	NdrFcShort( 0x17a ),	/* Offset= 378 (868) */
/* 492 */	NdrFcShort( 0xffff ),	/* Offset= -1 (491) */
/* 494 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 496 */	NdrFcShort( 0x0 ),	/* 0 */
/* 498 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 500 */	NdrFcShort( 0x0 ),	/* 0 */
/* 502 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 504 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 508 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 510 */	
			0x12, 0x0,	/* FC_UP */
/* 512 */	NdrFcShort( 0xfe16 ),	/* Offset= -490 (22) */
/* 514 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 516 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 518 */	NdrFcShort( 0x10 ),	/* 16 */
/* 520 */	NdrFcShort( 0x0 ),	/* 0 */
/* 522 */	NdrFcShort( 0x6 ),	/* Offset= 6 (528) */
/* 524 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 526 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 528 */	
			0x11, 0x0,	/* FC_RP */
/* 530 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (494) */
/* 532 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 534 */	NdrFcShort( 0x0 ),	/* 0 */
/* 536 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 538 */	NdrFcShort( 0x0 ),	/* 0 */
/* 540 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 542 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 546 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 548 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 550 */	NdrFcShort( 0xff58 ),	/* Offset= -168 (382) */
/* 552 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 554 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 556 */	NdrFcShort( 0x10 ),	/* 16 */
/* 558 */	NdrFcShort( 0x0 ),	/* 0 */
/* 560 */	NdrFcShort( 0x6 ),	/* Offset= 6 (566) */
/* 562 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 564 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 566 */	
			0x11, 0x0,	/* FC_RP */
/* 568 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (532) */
/* 570 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 572 */	NdrFcShort( 0x0 ),	/* 0 */
/* 574 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 576 */	NdrFcShort( 0x0 ),	/* 0 */
/* 578 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 580 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 584 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 586 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 588 */	NdrFcShort( 0xff44 ),	/* Offset= -188 (400) */
/* 590 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 592 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 594 */	NdrFcShort( 0x10 ),	/* 16 */
/* 596 */	NdrFcShort( 0x0 ),	/* 0 */
/* 598 */	NdrFcShort( 0x6 ),	/* Offset= 6 (604) */
/* 600 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 602 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 604 */	
			0x11, 0x0,	/* FC_RP */
/* 606 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (570) */
/* 608 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 610 */	NdrFcShort( 0x0 ),	/* 0 */
/* 612 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 614 */	NdrFcShort( 0x0 ),	/* 0 */
/* 616 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 618 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 622 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 624 */	
			0x12, 0x0,	/* FC_UP */
/* 626 */	NdrFcShort( 0x176 ),	/* Offset= 374 (1000) */
/* 628 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 630 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 632 */	NdrFcShort( 0x10 ),	/* 16 */
/* 634 */	NdrFcShort( 0x0 ),	/* 0 */
/* 636 */	NdrFcShort( 0x6 ),	/* Offset= 6 (642) */
/* 638 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 640 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 642 */	
			0x11, 0x0,	/* FC_RP */
/* 644 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (608) */
/* 646 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 648 */	NdrFcLong( 0x2f ),	/* 47 */
/* 652 */	NdrFcShort( 0x0 ),	/* 0 */
/* 654 */	NdrFcShort( 0x0 ),	/* 0 */
/* 656 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 658 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 660 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 662 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 664 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 666 */	NdrFcShort( 0x1 ),	/* 1 */
/* 668 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 670 */	NdrFcShort( 0x4 ),	/* 4 */
/* 672 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 674 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 676 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 678 */	NdrFcShort( 0x18 ),	/* 24 */
/* 680 */	NdrFcShort( 0x0 ),	/* 0 */
/* 682 */	NdrFcShort( 0xa ),	/* Offset= 10 (692) */
/* 684 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 686 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 688 */	NdrFcShort( 0xffd6 ),	/* Offset= -42 (646) */
/* 690 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 692 */	
			0x12, 0x0,	/* FC_UP */
/* 694 */	NdrFcShort( 0xffe2 ),	/* Offset= -30 (664) */
/* 696 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 698 */	NdrFcShort( 0x0 ),	/* 0 */
/* 700 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 702 */	NdrFcShort( 0x0 ),	/* 0 */
/* 704 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 706 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 710 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 712 */	
			0x12, 0x0,	/* FC_UP */
/* 714 */	NdrFcShort( 0xffda ),	/* Offset= -38 (676) */
/* 716 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 718 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 720 */	NdrFcShort( 0x10 ),	/* 16 */
/* 722 */	NdrFcShort( 0x0 ),	/* 0 */
/* 724 */	NdrFcShort( 0x6 ),	/* Offset= 6 (730) */
/* 726 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 728 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 730 */	
			0x11, 0x0,	/* FC_RP */
/* 732 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (696) */
/* 734 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 736 */	NdrFcShort( 0x8 ),	/* 8 */
/* 738 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 740 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 742 */	NdrFcShort( 0x10 ),	/* 16 */
/* 744 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 746 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 748 */	0x0,		/* 0 */
			NdrFcShort( 0xfff1 ),	/* Offset= -15 (734) */
			0x5b,		/* FC_END */
/* 752 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 754 */	NdrFcShort( 0x20 ),	/* 32 */
/* 756 */	NdrFcShort( 0x0 ),	/* 0 */
/* 758 */	NdrFcShort( 0xa ),	/* Offset= 10 (768) */
/* 760 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 762 */	0x36,		/* FC_POINTER */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 764 */	0x0,		/* 0 */
			NdrFcShort( 0xffe7 ),	/* Offset= -25 (740) */
			0x5b,		/* FC_END */
/* 768 */	
			0x11, 0x0,	/* FC_RP */
/* 770 */	NdrFcShort( 0xff12 ),	/* Offset= -238 (532) */
/* 772 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 774 */	NdrFcShort( 0x1 ),	/* 1 */
/* 776 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 778 */	NdrFcShort( 0x0 ),	/* 0 */
/* 780 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 782 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 784 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 786 */	NdrFcShort( 0x10 ),	/* 16 */
/* 788 */	NdrFcShort( 0x0 ),	/* 0 */
/* 790 */	NdrFcShort( 0x6 ),	/* Offset= 6 (796) */
/* 792 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 794 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 796 */	
			0x12, 0x0,	/* FC_UP */
/* 798 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (772) */
/* 800 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 802 */	NdrFcShort( 0x2 ),	/* 2 */
/* 804 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 806 */	NdrFcShort( 0x0 ),	/* 0 */
/* 808 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 810 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 812 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 814 */	NdrFcShort( 0x10 ),	/* 16 */
/* 816 */	NdrFcShort( 0x0 ),	/* 0 */
/* 818 */	NdrFcShort( 0x6 ),	/* Offset= 6 (824) */
/* 820 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 822 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 824 */	
			0x12, 0x0,	/* FC_UP */
/* 826 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (800) */
/* 828 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 830 */	NdrFcShort( 0x4 ),	/* 4 */
/* 832 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 834 */	NdrFcShort( 0x0 ),	/* 0 */
/* 836 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 838 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 840 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 842 */	NdrFcShort( 0x10 ),	/* 16 */
/* 844 */	NdrFcShort( 0x0 ),	/* 0 */
/* 846 */	NdrFcShort( 0x6 ),	/* Offset= 6 (852) */
/* 848 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 850 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 852 */	
			0x12, 0x0,	/* FC_UP */
/* 854 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (828) */
/* 856 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 858 */	NdrFcShort( 0x8 ),	/* 8 */
/* 860 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 862 */	NdrFcShort( 0x0 ),	/* 0 */
/* 864 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 866 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 868 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 870 */	NdrFcShort( 0x10 ),	/* 16 */
/* 872 */	NdrFcShort( 0x0 ),	/* 0 */
/* 874 */	NdrFcShort( 0x6 ),	/* Offset= 6 (880) */
/* 876 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 878 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 880 */	
			0x12, 0x0,	/* FC_UP */
/* 882 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (856) */
/* 884 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 886 */	NdrFcShort( 0x8 ),	/* 8 */
/* 888 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 890 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 892 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 894 */	NdrFcShort( 0x8 ),	/* 8 */
/* 896 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 898 */	NdrFcShort( 0xffc8 ),	/* -56 */
/* 900 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 902 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 904 */	NdrFcShort( 0xffec ),	/* Offset= -20 (884) */
/* 906 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 908 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 910 */	NdrFcShort( 0x38 ),	/* 56 */
/* 912 */	NdrFcShort( 0xffec ),	/* Offset= -20 (892) */
/* 914 */	NdrFcShort( 0x0 ),	/* Offset= 0 (914) */
/* 916 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 918 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 920 */	0x40,		/* FC_STRUCTPAD4 */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 922 */	0x0,		/* 0 */
			NdrFcShort( 0xfe0f ),	/* Offset= -497 (426) */
			0x5b,		/* FC_END */
/* 926 */	
			0x12, 0x0,	/* FC_UP */
/* 928 */	NdrFcShort( 0xff04 ),	/* Offset= -252 (676) */
/* 930 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 932 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 934 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 936 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 938 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 940 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 942 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 944 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 946 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 948 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 950 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 952 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 954 */	
			0x12, 0x0,	/* FC_UP */
/* 956 */	NdrFcShort( 0xfdb8 ),	/* Offset= -584 (372) */
/* 958 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 960 */	NdrFcShort( 0xfdba ),	/* Offset= -582 (378) */
/* 962 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 964 */	NdrFcShort( 0xfdba ),	/* Offset= -582 (382) */
/* 966 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 968 */	NdrFcShort( 0xfdc8 ),	/* Offset= -568 (400) */
/* 970 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 972 */	NdrFcShort( 0xfdd6 ),	/* Offset= -554 (418) */
/* 974 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 976 */	NdrFcShort( 0x2 ),	/* Offset= 2 (978) */
/* 978 */	
			0x12, 0x0,	/* FC_UP */
/* 980 */	NdrFcShort( 0x14 ),	/* Offset= 20 (1000) */
/* 982 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 984 */	NdrFcShort( 0x10 ),	/* 16 */
/* 986 */	0x6,		/* FC_SHORT */
			0x1,		/* FC_BYTE */
/* 988 */	0x1,		/* FC_BYTE */
			0x8,		/* FC_LONG */
/* 990 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 992 */	
			0x12, 0x0,	/* FC_UP */
/* 994 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (982) */
/* 996 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 998 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 1000 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 1002 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1004 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1006 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1006) */
/* 1008 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1010 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1012 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1014 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1016 */	NdrFcShort( 0xfc52 ),	/* Offset= -942 (74) */
/* 1018 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1020 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1022 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1024 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1026 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1028 */	NdrFcShort( 0xfc42 ),	/* Offset= -958 (70) */

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
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            }

        };



/* Standard interface: __MIDL_itf_PD_COMServer_0000_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IProxyPD_Dispatcher, ver. 0.0,
   GUID={0xBF9611C9,0x957F,0x4A19,{0x9D,0xA1,0x72,0x95,0xB0,0xCE,0xE2,0xBC}} */

#pragma code_seg(".orpc")
static const unsigned short IProxyPD_Dispatcher_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    50,
    88,
    132
    };

static const MIDL_STUBLESS_PROXY_INFO IProxyPD_Dispatcher_ProxyInfo =
    {
    &Object_StubDesc,
    PD_COMServer__MIDL_ProcFormatString.Format,
    &IProxyPD_Dispatcher_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IProxyPD_Dispatcher_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    PD_COMServer__MIDL_ProcFormatString.Format,
    &IProxyPD_Dispatcher_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(11) _IProxyPD_DispatcherProxyVtbl = 
{
    &IProxyPD_Dispatcher_ProxyInfo,
    &IID_IProxyPD_Dispatcher,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IProxyPD_Dispatcher::getConfig */ ,
    (void *) (INT_PTR) -1 /* IProxyPD_Dispatcher::setConfig */ ,
    (void *) (INT_PTR) -1 /* IProxyPD_Dispatcher::connectToErrorsStream */ ,
    (void *) (INT_PTR) -1 /* IProxyPD_Dispatcher::disconnectClient */
};


static const PRPC_STUB_FUNCTION IProxyPD_Dispatcher_table[] =
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

CInterfaceStubVtbl _IProxyPD_DispatcherStubVtbl =
{
    &IID_IProxyPD_Dispatcher,
    &IProxyPD_Dispatcher_ServerInfo,
    11,
    &IProxyPD_Dispatcher_table[-3],
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
    PD_COMServer__MIDL_TypeFormatString.Format,
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

const CInterfaceProxyVtbl * const _PD_COMServer_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IProxyPD_DispatcherProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _PD_COMServer_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IProxyPD_DispatcherStubVtbl,
    0
};

PCInterfaceName const _PD_COMServer_InterfaceNamesList[] = 
{
    "IProxyPD_Dispatcher",
    0
};

const IID *  const _PD_COMServer_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _PD_COMServer_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _PD_COMServer, pIID, n)

int __stdcall _PD_COMServer_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_PD_COMServer_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo PD_COMServer_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _PD_COMServer_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _PD_COMServer_StubVtblList,
    (const PCInterfaceName * ) & _PD_COMServer_InterfaceNamesList,
    (const IID ** ) & _PD_COMServer_BaseIIDList,
    & _PD_COMServer_IID_Lookup, 
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* defined(_M_AMD64)*/

