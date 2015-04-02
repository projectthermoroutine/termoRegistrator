

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Thu Apr 02 20:49:13 2015
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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __PD_COMServer_i_h__
#define __PD_COMServer_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IProxyPD_Dispatcher_FWD_DEFINED__
#define __IProxyPD_Dispatcher_FWD_DEFINED__
typedef interface IProxyPD_Dispatcher IProxyPD_Dispatcher;

#endif 	/* __IProxyPD_Dispatcher_FWD_DEFINED__ */


#ifndef ___IProxy_PD_DispatcherEvents_FWD_DEFINED__
#define ___IProxy_PD_DispatcherEvents_FWD_DEFINED__
typedef interface _IProxy_PD_DispatcherEvents _IProxy_PD_DispatcherEvents;

#endif 	/* ___IProxy_PD_DispatcherEvents_FWD_DEFINED__ */


#ifndef __ProxyPD_Dispatcher_FWD_DEFINED__
#define __ProxyPD_Dispatcher_FWD_DEFINED__

#ifdef __cplusplus
typedef class ProxyPD_Dispatcher ProxyPD_Dispatcher;
#else
typedef struct ProxyPD_Dispatcher ProxyPD_Dispatcher;
#endif /* __cplusplus */

#endif 	/* __ProxyPD_Dispatcher_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_PD_COMServer_0000_0000 */
/* [local] */ 

typedef struct _ShareMemorySettings
    {
    long share_memory_size;
    BSTR share_memory_name;
    BSTR read_event_name;
    } 	ShareMemorySettings;

typedef 
enum _errorSource
    {
        dispatch_error	= 0,
        connection_error	= ( dispatch_error + 1 ) ,
        runtime_error	= ( connection_error + 1 ) 
    } 	errorSource;

typedef struct _errorInfo
    {
    errorSource err_source;
    unsigned char err_str;
    } 	errorInfo;



extern RPC_IF_HANDLE __MIDL_itf_PD_COMServer_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_PD_COMServer_0000_0000_v0_0_s_ifspec;

#ifndef __IProxyPD_Dispatcher_INTERFACE_DEFINED__
#define __IProxyPD_Dispatcher_INTERFACE_DEFINED__

/* interface IProxyPD_Dispatcher */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IProxyPD_Dispatcher;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BF9611C9-957F-4A19-9DA1-7295B0CEE2BC")
    IProxyPD_Dispatcher : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getConfig( 
            /* [out] */ ShareMemorySettings *syncSettings,
            /* [out] */ ShareMemorySettings *eventSettings,
            /* [out] */ ULONG32 *clientId) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE setConfig( 
            /* [in] */ VARIANT Arr) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE connectToErrorsStream( 
            /* [out] */ ShareMemorySettings *errStream,
            /* [out] */ ULONG32 *clientId) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE disconnectClient( 
            /* [in] */ ULONG32 clientId,
            /* [in] */ ULONG32 errorsClientId) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IProxyPD_DispatcherVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IProxyPD_Dispatcher * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IProxyPD_Dispatcher * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IProxyPD_Dispatcher * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IProxyPD_Dispatcher * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IProxyPD_Dispatcher * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IProxyPD_Dispatcher * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IProxyPD_Dispatcher * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *getConfig )( 
            IProxyPD_Dispatcher * This,
            /* [out] */ ShareMemorySettings *syncSettings,
            /* [out] */ ShareMemorySettings *eventSettings,
            /* [out] */ ULONG32 *clientId);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *setConfig )( 
            IProxyPD_Dispatcher * This,
            /* [in] */ VARIANT Arr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *connectToErrorsStream )( 
            IProxyPD_Dispatcher * This,
            /* [out] */ ShareMemorySettings *errStream,
            /* [out] */ ULONG32 *clientId);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *disconnectClient )( 
            IProxyPD_Dispatcher * This,
            /* [in] */ ULONG32 clientId,
            /* [in] */ ULONG32 errorsClientId);
        
        END_INTERFACE
    } IProxyPD_DispatcherVtbl;

    interface IProxyPD_Dispatcher
    {
        CONST_VTBL struct IProxyPD_DispatcherVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IProxyPD_Dispatcher_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IProxyPD_Dispatcher_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IProxyPD_Dispatcher_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IProxyPD_Dispatcher_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IProxyPD_Dispatcher_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IProxyPD_Dispatcher_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IProxyPD_Dispatcher_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IProxyPD_Dispatcher_getConfig(This,syncSettings,eventSettings,clientId)	\
    ( (This)->lpVtbl -> getConfig(This,syncSettings,eventSettings,clientId) ) 

#define IProxyPD_Dispatcher_setConfig(This,Arr)	\
    ( (This)->lpVtbl -> setConfig(This,Arr) ) 

#define IProxyPD_Dispatcher_connectToErrorsStream(This,errStream,clientId)	\
    ( (This)->lpVtbl -> connectToErrorsStream(This,errStream,clientId) ) 

#define IProxyPD_Dispatcher_disconnectClient(This,clientId,errorsClientId)	\
    ( (This)->lpVtbl -> disconnectClient(This,clientId,errorsClientId) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IProxyPD_Dispatcher_INTERFACE_DEFINED__ */



#ifndef __PD_COMServerLib_LIBRARY_DEFINED__
#define __PD_COMServerLib_LIBRARY_DEFINED__

/* library PD_COMServerLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_PD_COMServerLib;

#ifndef ___IProxy_PD_DispatcherEvents_DISPINTERFACE_DEFINED__
#define ___IProxy_PD_DispatcherEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IProxy_PD_DispatcherEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__IProxy_PD_DispatcherEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("6D25FBF8-4CE6-4931-A8C7-944C0FA1E50A")
    _IProxy_PD_DispatcherEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IProxy_PD_DispatcherEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IProxy_PD_DispatcherEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IProxy_PD_DispatcherEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IProxy_PD_DispatcherEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IProxy_PD_DispatcherEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IProxy_PD_DispatcherEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IProxy_PD_DispatcherEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IProxy_PD_DispatcherEvents * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } _IProxy_PD_DispatcherEventsVtbl;

    interface _IProxy_PD_DispatcherEvents
    {
        CONST_VTBL struct _IProxy_PD_DispatcherEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IProxy_PD_DispatcherEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IProxy_PD_DispatcherEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IProxy_PD_DispatcherEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IProxy_PD_DispatcherEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IProxy_PD_DispatcherEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IProxy_PD_DispatcherEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IProxy_PD_DispatcherEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IProxy_PD_DispatcherEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ProxyPD_Dispatcher;

#ifdef __cplusplus

class DECLSPEC_UUID("5EC93852-7FDE-4E1B-9CB8-2EA91472E1AC")
ProxyPD_Dispatcher;
#endif
#endif /* __PD_COMServerLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


