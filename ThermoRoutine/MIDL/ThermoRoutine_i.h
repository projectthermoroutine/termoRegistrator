

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Thu Feb 26 22:54:15 2015
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

#ifndef __ThermoRoutine_i_h__
#define __ThermoRoutine_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IComponentRegistrar_FWD_DEFINED__
#define __IComponentRegistrar_FWD_DEFINED__
typedef interface IComponentRegistrar IComponentRegistrar;

#endif 	/* __IComponentRegistrar_FWD_DEFINED__ */


#ifndef __IPallete_FWD_DEFINED__
#define __IPallete_FWD_DEFINED__
typedef interface IPallete IPallete;

#endif 	/* __IPallete_FWD_DEFINED__ */


#ifndef __IArea_FWD_DEFINED__
#define __IArea_FWD_DEFINED__
typedef interface IArea IArea;

#endif 	/* __IArea_FWD_DEFINED__ */


#ifndef __ITRWrapper_FWD_DEFINED__
#define __ITRWrapper_FWD_DEFINED__
typedef interface ITRWrapper ITRWrapper;

#endif 	/* __ITRWrapper_FWD_DEFINED__ */


#ifndef __IMovieTransit_FWD_DEFINED__
#define __IMovieTransit_FWD_DEFINED__
typedef interface IMovieTransit IMovieTransit;

#endif 	/* __IMovieTransit_FWD_DEFINED__ */


#ifndef __CompReg_FWD_DEFINED__
#define __CompReg_FWD_DEFINED__

#ifdef __cplusplus
typedef class CompReg CompReg;
#else
typedef struct CompReg CompReg;
#endif /* __cplusplus */

#endif 	/* __CompReg_FWD_DEFINED__ */


#ifndef ___ITRWrapperEvents_FWD_DEFINED__
#define ___ITRWrapperEvents_FWD_DEFINED__
typedef interface _ITRWrapperEvents _ITRWrapperEvents;

#endif 	/* ___ITRWrapperEvents_FWD_DEFINED__ */


#ifndef ___IThermoLibErrorEvents_FWD_DEFINED__
#define ___IThermoLibErrorEvents_FWD_DEFINED__
typedef interface _IThermoLibErrorEvents _IThermoLibErrorEvents;

#endif 	/* ___IThermoLibErrorEvents_FWD_DEFINED__ */


#ifndef ___IThermoLibComponentsEvents_FWD_DEFINED__
#define ___IThermoLibComponentsEvents_FWD_DEFINED__
typedef interface _IThermoLibComponentsEvents _IThermoLibComponentsEvents;

#endif 	/* ___IThermoLibComponentsEvents_FWD_DEFINED__ */


#ifndef __TRWrapper_FWD_DEFINED__
#define __TRWrapper_FWD_DEFINED__

#ifdef __cplusplus
typedef class TRWrapper TRWrapper;
#else
typedef struct TRWrapper TRWrapper;
#endif /* __cplusplus */

#endif 	/* __TRWrapper_FWD_DEFINED__ */


#ifndef __MovieTransit_FWD_DEFINED__
#define __MovieTransit_FWD_DEFINED__

#ifdef __cplusplus
typedef class MovieTransit MovieTransit;
#else
typedef struct MovieTransit MovieTransit;
#endif /* __cplusplus */

#endif 	/* __MovieTransit_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IComponentRegistrar_INTERFACE_DEFINED__
#define __IComponentRegistrar_INTERFACE_DEFINED__

/* interface IComponentRegistrar */
/* [unique][dual][uuid][object] */ 


EXTERN_C const IID IID_IComponentRegistrar;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("a817e7a2-43fa-11d0-9e44-00aa00b6770a")
    IComponentRegistrar : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Attach( 
            /* [in] */ BSTR bstrPath) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RegisterAll( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE UnregisterAll( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetComponents( 
            /* [out] */ SAFEARRAY * *pbstrCLSIDs,
            /* [out] */ SAFEARRAY * *pbstrDescriptions) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RegisterComponent( 
            /* [in] */ BSTR bstrCLSID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE UnregisterComponent( 
            /* [in] */ BSTR bstrCLSID) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IComponentRegistrarVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IComponentRegistrar * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IComponentRegistrar * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IComponentRegistrar * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IComponentRegistrar * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IComponentRegistrar * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IComponentRegistrar * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IComponentRegistrar * This,
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
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Attach )( 
            IComponentRegistrar * This,
            /* [in] */ BSTR bstrPath);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RegisterAll )( 
            IComponentRegistrar * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *UnregisterAll )( 
            IComponentRegistrar * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetComponents )( 
            IComponentRegistrar * This,
            /* [out] */ SAFEARRAY * *pbstrCLSIDs,
            /* [out] */ SAFEARRAY * *pbstrDescriptions);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RegisterComponent )( 
            IComponentRegistrar * This,
            /* [in] */ BSTR bstrCLSID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *UnregisterComponent )( 
            IComponentRegistrar * This,
            /* [in] */ BSTR bstrCLSID);
        
        END_INTERFACE
    } IComponentRegistrarVtbl;

    interface IComponentRegistrar
    {
        CONST_VTBL struct IComponentRegistrarVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IComponentRegistrar_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IComponentRegistrar_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IComponentRegistrar_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IComponentRegistrar_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IComponentRegistrar_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IComponentRegistrar_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IComponentRegistrar_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IComponentRegistrar_Attach(This,bstrPath)	\
    ( (This)->lpVtbl -> Attach(This,bstrPath) ) 

#define IComponentRegistrar_RegisterAll(This)	\
    ( (This)->lpVtbl -> RegisterAll(This) ) 

#define IComponentRegistrar_UnregisterAll(This)	\
    ( (This)->lpVtbl -> UnregisterAll(This) ) 

#define IComponentRegistrar_GetComponents(This,pbstrCLSIDs,pbstrDescriptions)	\
    ( (This)->lpVtbl -> GetComponents(This,pbstrCLSIDs,pbstrDescriptions) ) 

#define IComponentRegistrar_RegisterComponent(This,bstrCLSID)	\
    ( (This)->lpVtbl -> RegisterComponent(This,bstrCLSID) ) 

#define IComponentRegistrar_UnregisterComponent(This,bstrCLSID)	\
    ( (This)->lpVtbl -> UnregisterComponent(This,bstrCLSID) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IComponentRegistrar_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_ThermoRoutine_0000_0001 */
/* [local] */ 

typedef struct _area_temperature_measure
    {
    FLOAT min;
    FLOAT max;
    FLOAT avr;
    } 	area_temperature_measure;

typedef struct _area_temperature_measure_result
    {
    ULONG area_id;
    BOOL result_is_valid;
    area_temperature_measure measure;
    } 	area_temperature_measure_result;

typedef struct _frame_coordinate
    {
    int path;
    int line;
    ULONG64 coordinate;
    } 	frame_coordinate;

typedef struct _temperature_measure
    {
    FLOAT tmin;
    FLOAT tmax;
    FLOAT tavr;
    FLOAT object_tmin;
    FLOAT object_tmax;
    FLOAT calibration_min;
    FLOAT calibration_max;
    } 	temperature_measure;

typedef struct _raster_info2
    {
    USHORT width;
    USHORT heigth;
    USHORT firstValidX;
    USHORT lastValidX;
    USHORT firstValidY;
    USHORT lastValidY;
    } 	raster_info2;

typedef struct _raster_info
    {
    USHORT width;
    USHORT height;
    } 	raster_info;

typedef struct _irb_frame_info
    {
    double timestamp;
    raster_info image_info;
    temperature_measure measure;
    frame_coordinate coordinate;
    } 	irb_frame_info;

typedef 
enum _calibration_mode
    {
        NONE	= 0,
        MIN_MAX	= ( NONE + 1 ) ,
        AVERAGE	= ( MIN_MAX + 1 ) ,
        MANUAL	= ( AVERAGE + 1 ) 
    } 	calibration_mode;

typedef 
enum _area_type
    {
        RECTANGLE	= 0,
        ELLIPSE	= ( RECTANGLE + 1 ) 
    } 	area_type;

typedef struct _area_info
    {
    area_type type;
    SHORT x0;
    SHORT y0;
    USHORT width;
    USHORT heigth;
    } 	area_info;



extern RPC_IF_HANDLE __MIDL_itf_ThermoRoutine_0000_0001_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_ThermoRoutine_0000_0001_v0_0_s_ifspec;

#ifndef __IPallete_INTERFACE_DEFINED__
#define __IPallete_INTERFACE_DEFINED__

/* interface IPallete */
/* [unique][dual][uuid][object] */ 


EXTERN_C const IID IID_IPallete;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("859E7C6E-270C-41F2-AE3A-7CBDDFFA30AF")
    IPallete : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetPallete( 
            /* [in] */ BSTR palleteFileName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetDefaultPallete( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetPallete( 
            /* [out][in] */ VARIANT *pallete) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetPalleteLength( 
            /* [retval][out] */ SHORT *len) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IPalleteVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPallete * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPallete * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPallete * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPallete * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPallete * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPallete * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPallete * This,
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
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetPallete )( 
            IPallete * This,
            /* [in] */ BSTR palleteFileName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetDefaultPallete )( 
            IPallete * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetPallete )( 
            IPallete * This,
            /* [out][in] */ VARIANT *pallete);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetPalleteLength )( 
            IPallete * This,
            /* [retval][out] */ SHORT *len);
        
        END_INTERFACE
    } IPalleteVtbl;

    interface IPallete
    {
        CONST_VTBL struct IPalleteVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPallete_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPallete_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPallete_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPallete_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IPallete_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IPallete_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IPallete_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IPallete_SetPallete(This,palleteFileName)	\
    ( (This)->lpVtbl -> SetPallete(This,palleteFileName) ) 

#define IPallete_SetDefaultPallete(This)	\
    ( (This)->lpVtbl -> SetDefaultPallete(This) ) 

#define IPallete_GetPallete(This,pallete)	\
    ( (This)->lpVtbl -> GetPallete(This,pallete) ) 

#define IPallete_GetPalleteLength(This,len)	\
    ( (This)->lpVtbl -> GetPalleteLength(This,len) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPallete_INTERFACE_DEFINED__ */


#ifndef __IArea_INTERFACE_DEFINED__
#define __IArea_INTERFACE_DEFINED__

/* interface IArea */
/* [unique][dual][uuid][object] */ 


EXTERN_C const IID IID_IArea;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("03BA3F31-A54D-443E-AE88-2C556EA196FA")
    IArea : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AddArea( 
            /* [in] */ SHORT id,
            /* [in] */ area_info *area) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AreaChanged( 
            /* [in] */ SHORT id,
            /* [in] */ area_info *area) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RemoveArea( 
            /* [in] */ SHORT id,
            /* [out] */ area_type *type) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetAreasInfo( 
            /* [out][in] */ area_temperature_measure_result **results,
            /* [in] */ ULONG size) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetAreaInfo( 
            /* [in] */ ULONG aid,
            /* [out] */ area_temperature_measure *measure,
            /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RemoveAllAreas( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IAreaVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IArea * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IArea * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IArea * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IArea * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IArea * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IArea * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IArea * This,
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
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AddArea )( 
            IArea * This,
            /* [in] */ SHORT id,
            /* [in] */ area_info *area);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AreaChanged )( 
            IArea * This,
            /* [in] */ SHORT id,
            /* [in] */ area_info *area);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RemoveArea )( 
            IArea * This,
            /* [in] */ SHORT id,
            /* [out] */ area_type *type);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetAreasInfo )( 
            IArea * This,
            /* [out][in] */ area_temperature_measure_result **results,
            /* [in] */ ULONG size);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetAreaInfo )( 
            IArea * This,
            /* [in] */ ULONG aid,
            /* [out] */ area_temperature_measure *measure,
            /* [retval][out] */ VARIANT_BOOL *result);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RemoveAllAreas )( 
            IArea * This);
        
        END_INTERFACE
    } IAreaVtbl;

    interface IArea
    {
        CONST_VTBL struct IAreaVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IArea_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IArea_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IArea_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IArea_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IArea_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IArea_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IArea_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IArea_AddArea(This,id,area)	\
    ( (This)->lpVtbl -> AddArea(This,id,area) ) 

#define IArea_AreaChanged(This,id,area)	\
    ( (This)->lpVtbl -> AreaChanged(This,id,area) ) 

#define IArea_RemoveArea(This,id,type)	\
    ( (This)->lpVtbl -> RemoveArea(This,id,type) ) 

#define IArea_GetAreasInfo(This,results,size)	\
    ( (This)->lpVtbl -> GetAreasInfo(This,results,size) ) 

#define IArea_GetAreaInfo(This,aid,measure,result)	\
    ( (This)->lpVtbl -> GetAreaInfo(This,aid,measure,result) ) 

#define IArea_RemoveAllAreas(This)	\
    ( (This)->lpVtbl -> RemoveAllAreas(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IArea_INTERFACE_DEFINED__ */


#ifndef __ITRWrapper_INTERFACE_DEFINED__
#define __ITRWrapper_INTERFACE_DEFINED__

/* interface ITRWrapper */
/* [unique][dual][uuid][object] */ 


EXTERN_C const IID IID_ITRWrapper;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1347B40A-BFBD-4A82-8A75-46A2558507BA")
    ITRWrapper : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ClearGrabbingCache( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE StopGrabbing( 
            /* [in] */ BYTE unload,
            /* [in] */ BYTE save) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE StartRecord( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE StopRecord( 
            /* [in] */ BYTE unload,
            /* [in] */ BYTE save) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetBlockCamFrame( 
            /* [in] */ BYTE blockFlag) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE StartRecieveCoordinates( 
            /* [in] */ BSTR pd_ip,
            /* [in] */ USHORT pd_port,
            /* [in] */ BSTR events_pd_ip,
            /* [in] */ USHORT events_pd_port) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE StopRecieveCoordinates( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE StartGrabbing( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetGrabberPath( 
            /* [in] */ BSTR grabberPath) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ShowCamSettings( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE HideCamSettings( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGrabberSources( 
            /* [out] */ SAFEARRAY * *sourcesList) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetRealTimeFrameRaster( 
            /* [in] */ DWORD frameId,
            /* [out] */ irb_frame_info *frame_info,
            /* [out][in] */ VARIANT *pixels,
            /* [retval][out] */ VARIANT_BOOL *res) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetNextRealTimeFrameRaster( 
            /* [out] */ DWORD *frameId,
            /* [out] */ irb_frame_info *frame_info,
            /* [out][in] */ VARIANT *pixels,
            /* [retval][out] */ VARIANT_BOOL *res) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetRealTimeFrame( 
            /* [in] */ DWORD frameId,
            /* [out] */ irb_frame_info *frame_info,
            /* [out][in] */ VARIANT *pixels,
            /* [out][in] */ VARIANT *temp_values,
            /* [retval][out] */ VARIANT_BOOL *res) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetNextRealTimeFrame( 
            /* [out] */ DWORD *frameId,
            /* [out] */ irb_frame_info *frame_info,
            /* [out][in] */ VARIANT *pixels,
            /* [out][in] */ VARIANT *temp_values,
            /* [retval][out] */ VARIANT_BOOL *res) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE get_pixel_temperature( 
            /* [in] */ DWORD frameId,
            /* [in] */ USHORT x,
            /* [in] */ USHORT y,
            /* [out] */ FLOAT *tempToReturn,
            /* [retval][out] */ VARIANT_BOOL *res) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ConnectCamera( 
            /* [in] */ BYTE initMode,
            /* [retval][out] */ VARIANT_BOOL *res) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DisconnectCamera( 
            /* [retval][out] */ VARIANT_BOOL *res) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FinishAll( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetPallete( 
            /* [in] */ BSTR palleteFileName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetDefaultPallete( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetPallete( 
            /* [out][in] */ VARIANT *pallete) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetPalleteLength( 
            /* [out] */ ULONG32 *number_colors,
            /* [retval][out] */ SHORT *len) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetPaletteCalibrationMode( 
            /* [in] */ calibration_mode mode) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetPaletteCalibration( 
            /* [in] */ float min,
            /* [in] */ float max) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AddArea( 
            /* [in] */ SHORT id,
            /* [in] */ area_info *area) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AreaChanged( 
            /* [in] */ SHORT id,
            /* [in] */ area_info *area) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RemoveArea( 
            /* [in] */ SHORT id,
            /* [out] */ area_type *type) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetAreaInfo( 
            /* [in] */ ULONG aid,
            /* [out] */ area_temperature_measure *measure,
            /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RemoveAllAreas( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetMaxFramesInIRBFile( 
            USHORT frames_number) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ITRWrapperVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITRWrapper * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITRWrapper * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITRWrapper * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITRWrapper * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITRWrapper * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITRWrapper * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITRWrapper * This,
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
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ClearGrabbingCache )( 
            ITRWrapper * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *StopGrabbing )( 
            ITRWrapper * This,
            /* [in] */ BYTE unload,
            /* [in] */ BYTE save);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *StartRecord )( 
            ITRWrapper * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *StopRecord )( 
            ITRWrapper * This,
            /* [in] */ BYTE unload,
            /* [in] */ BYTE save);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetBlockCamFrame )( 
            ITRWrapper * This,
            /* [in] */ BYTE blockFlag);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *StartRecieveCoordinates )( 
            ITRWrapper * This,
            /* [in] */ BSTR pd_ip,
            /* [in] */ USHORT pd_port,
            /* [in] */ BSTR events_pd_ip,
            /* [in] */ USHORT events_pd_port);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *StopRecieveCoordinates )( 
            ITRWrapper * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *StartGrabbing )( 
            ITRWrapper * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetGrabberPath )( 
            ITRWrapper * This,
            /* [in] */ BSTR grabberPath);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ShowCamSettings )( 
            ITRWrapper * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *HideCamSettings )( 
            ITRWrapper * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetGrabberSources )( 
            ITRWrapper * This,
            /* [out] */ SAFEARRAY * *sourcesList);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetRealTimeFrameRaster )( 
            ITRWrapper * This,
            /* [in] */ DWORD frameId,
            /* [out] */ irb_frame_info *frame_info,
            /* [out][in] */ VARIANT *pixels,
            /* [retval][out] */ VARIANT_BOOL *res);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetNextRealTimeFrameRaster )( 
            ITRWrapper * This,
            /* [out] */ DWORD *frameId,
            /* [out] */ irb_frame_info *frame_info,
            /* [out][in] */ VARIANT *pixels,
            /* [retval][out] */ VARIANT_BOOL *res);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetRealTimeFrame )( 
            ITRWrapper * This,
            /* [in] */ DWORD frameId,
            /* [out] */ irb_frame_info *frame_info,
            /* [out][in] */ VARIANT *pixels,
            /* [out][in] */ VARIANT *temp_values,
            /* [retval][out] */ VARIANT_BOOL *res);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetNextRealTimeFrame )( 
            ITRWrapper * This,
            /* [out] */ DWORD *frameId,
            /* [out] */ irb_frame_info *frame_info,
            /* [out][in] */ VARIANT *pixels,
            /* [out][in] */ VARIANT *temp_values,
            /* [retval][out] */ VARIANT_BOOL *res);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *get_pixel_temperature )( 
            ITRWrapper * This,
            /* [in] */ DWORD frameId,
            /* [in] */ USHORT x,
            /* [in] */ USHORT y,
            /* [out] */ FLOAT *tempToReturn,
            /* [retval][out] */ VARIANT_BOOL *res);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ConnectCamera )( 
            ITRWrapper * This,
            /* [in] */ BYTE initMode,
            /* [retval][out] */ VARIANT_BOOL *res);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DisconnectCamera )( 
            ITRWrapper * This,
            /* [retval][out] */ VARIANT_BOOL *res);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *FinishAll )( 
            ITRWrapper * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetPallete )( 
            ITRWrapper * This,
            /* [in] */ BSTR palleteFileName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetDefaultPallete )( 
            ITRWrapper * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetPallete )( 
            ITRWrapper * This,
            /* [out][in] */ VARIANT *pallete);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetPalleteLength )( 
            ITRWrapper * This,
            /* [out] */ ULONG32 *number_colors,
            /* [retval][out] */ SHORT *len);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetPaletteCalibrationMode )( 
            ITRWrapper * This,
            /* [in] */ calibration_mode mode);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetPaletteCalibration )( 
            ITRWrapper * This,
            /* [in] */ float min,
            /* [in] */ float max);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AddArea )( 
            ITRWrapper * This,
            /* [in] */ SHORT id,
            /* [in] */ area_info *area);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AreaChanged )( 
            ITRWrapper * This,
            /* [in] */ SHORT id,
            /* [in] */ area_info *area);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RemoveArea )( 
            ITRWrapper * This,
            /* [in] */ SHORT id,
            /* [out] */ area_type *type);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetAreaInfo )( 
            ITRWrapper * This,
            /* [in] */ ULONG aid,
            /* [out] */ area_temperature_measure *measure,
            /* [retval][out] */ VARIANT_BOOL *result);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RemoveAllAreas )( 
            ITRWrapper * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetMaxFramesInIRBFile )( 
            ITRWrapper * This,
            USHORT frames_number);
        
        END_INTERFACE
    } ITRWrapperVtbl;

    interface ITRWrapper
    {
        CONST_VTBL struct ITRWrapperVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITRWrapper_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITRWrapper_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITRWrapper_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITRWrapper_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITRWrapper_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITRWrapper_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITRWrapper_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITRWrapper_ClearGrabbingCache(This)	\
    ( (This)->lpVtbl -> ClearGrabbingCache(This) ) 

#define ITRWrapper_StopGrabbing(This,unload,save)	\
    ( (This)->lpVtbl -> StopGrabbing(This,unload,save) ) 

#define ITRWrapper_StartRecord(This)	\
    ( (This)->lpVtbl -> StartRecord(This) ) 

#define ITRWrapper_StopRecord(This,unload,save)	\
    ( (This)->lpVtbl -> StopRecord(This,unload,save) ) 

#define ITRWrapper_SetBlockCamFrame(This,blockFlag)	\
    ( (This)->lpVtbl -> SetBlockCamFrame(This,blockFlag) ) 

#define ITRWrapper_StartRecieveCoordinates(This,pd_ip,pd_port,events_pd_ip,events_pd_port)	\
    ( (This)->lpVtbl -> StartRecieveCoordinates(This,pd_ip,pd_port,events_pd_ip,events_pd_port) ) 

#define ITRWrapper_StopRecieveCoordinates(This)	\
    ( (This)->lpVtbl -> StopRecieveCoordinates(This) ) 

#define ITRWrapper_StartGrabbing(This)	\
    ( (This)->lpVtbl -> StartGrabbing(This) ) 

#define ITRWrapper_SetGrabberPath(This,grabberPath)	\
    ( (This)->lpVtbl -> SetGrabberPath(This,grabberPath) ) 

#define ITRWrapper_ShowCamSettings(This)	\
    ( (This)->lpVtbl -> ShowCamSettings(This) ) 

#define ITRWrapper_HideCamSettings(This)	\
    ( (This)->lpVtbl -> HideCamSettings(This) ) 

#define ITRWrapper_GetGrabberSources(This,sourcesList)	\
    ( (This)->lpVtbl -> GetGrabberSources(This,sourcesList) ) 

#define ITRWrapper_GetRealTimeFrameRaster(This,frameId,frame_info,pixels,res)	\
    ( (This)->lpVtbl -> GetRealTimeFrameRaster(This,frameId,frame_info,pixels,res) ) 

#define ITRWrapper_GetNextRealTimeFrameRaster(This,frameId,frame_info,pixels,res)	\
    ( (This)->lpVtbl -> GetNextRealTimeFrameRaster(This,frameId,frame_info,pixels,res) ) 

#define ITRWrapper_GetRealTimeFrame(This,frameId,frame_info,pixels,temp_values,res)	\
    ( (This)->lpVtbl -> GetRealTimeFrame(This,frameId,frame_info,pixels,temp_values,res) ) 

#define ITRWrapper_GetNextRealTimeFrame(This,frameId,frame_info,pixels,temp_values,res)	\
    ( (This)->lpVtbl -> GetNextRealTimeFrame(This,frameId,frame_info,pixels,temp_values,res) ) 

#define ITRWrapper_get_pixel_temperature(This,frameId,x,y,tempToReturn,res)	\
    ( (This)->lpVtbl -> get_pixel_temperature(This,frameId,x,y,tempToReturn,res) ) 

#define ITRWrapper_ConnectCamera(This,initMode,res)	\
    ( (This)->lpVtbl -> ConnectCamera(This,initMode,res) ) 

#define ITRWrapper_DisconnectCamera(This,res)	\
    ( (This)->lpVtbl -> DisconnectCamera(This,res) ) 

#define ITRWrapper_FinishAll(This)	\
    ( (This)->lpVtbl -> FinishAll(This) ) 

#define ITRWrapper_SetPallete(This,palleteFileName)	\
    ( (This)->lpVtbl -> SetPallete(This,palleteFileName) ) 

#define ITRWrapper_SetDefaultPallete(This)	\
    ( (This)->lpVtbl -> SetDefaultPallete(This) ) 

#define ITRWrapper_GetPallete(This,pallete)	\
    ( (This)->lpVtbl -> GetPallete(This,pallete) ) 

#define ITRWrapper_GetPalleteLength(This,number_colors,len)	\
    ( (This)->lpVtbl -> GetPalleteLength(This,number_colors,len) ) 

#define ITRWrapper_SetPaletteCalibrationMode(This,mode)	\
    ( (This)->lpVtbl -> SetPaletteCalibrationMode(This,mode) ) 

#define ITRWrapper_SetPaletteCalibration(This,min,max)	\
    ( (This)->lpVtbl -> SetPaletteCalibration(This,min,max) ) 

#define ITRWrapper_AddArea(This,id,area)	\
    ( (This)->lpVtbl -> AddArea(This,id,area) ) 

#define ITRWrapper_AreaChanged(This,id,area)	\
    ( (This)->lpVtbl -> AreaChanged(This,id,area) ) 

#define ITRWrapper_RemoveArea(This,id,type)	\
    ( (This)->lpVtbl -> RemoveArea(This,id,type) ) 

#define ITRWrapper_GetAreaInfo(This,aid,measure,result)	\
    ( (This)->lpVtbl -> GetAreaInfo(This,aid,measure,result) ) 

#define ITRWrapper_RemoveAllAreas(This)	\
    ( (This)->lpVtbl -> RemoveAllAreas(This) ) 

#define ITRWrapper_SetMaxFramesInIRBFile(This,frames_number)	\
    ( (This)->lpVtbl -> SetMaxFramesInIRBFile(This,frames_number) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITRWrapper_INTERFACE_DEFINED__ */


#ifndef __IMovieTransit_INTERFACE_DEFINED__
#define __IMovieTransit_INTERFACE_DEFINED__

/* interface IMovieTransit */
/* [unique][dual][uuid][object] */ 


EXTERN_C const IID IID_IMovieTransit;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E37EF668-25E2-4784-BFF3-642AA953FCAA")
    IMovieTransit : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetIRBFiles( 
            /* [in] */ VARIANT filesNames,
            /* [out] */ SAFEARRAY * *errors,
            /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FilesCount( 
            /* [retval][out] */ SHORT *filesCount) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FramesCount( 
            /* [retval][out] */ LONG *framesCount) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFrameRaster( 
            /* [in] */ SHORT frameIndex,
            /* [out] */ irb_frame_info *frame_info,
            /* [out][in] */ VARIANT *frameRaster,
            /* [retval][out] */ VARIANT_BOOL *res) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFrame( 
            /* [in] */ SHORT frameIndex,
            /* [out] */ irb_frame_info *frame_info,
            /* [out][in] */ VARIANT *pixels,
            /* [out][in] */ VARIANT *temp_values,
            /* [retval][out] */ VARIANT_BOOL *res) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE get_pixel_temperature( 
            /* [in] */ DWORD frameIndex,
            /* [in] */ USHORT x,
            /* [in] */ USHORT y,
            /* [out] */ FLOAT *tempToReturn,
            /* [retval][out] */ VARIANT_BOOL *res) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SaveCurrentFrame( 
            /* [in] */ BSTR path,
            /* [retval][out] */ SHORT *result) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCurFrameTemperatures( 
            /* [out] */ temperature_measure *measure) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE IsFrameMeetFilter( 
            /* [in] */ SHORT frameNum,
            /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFramePositionInfo( 
            /* [in] */ ULONG frame_id,
            /* [out] */ frame_coordinate *coordinate,
            /* [out] */ double *timestamp) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCurrentFramePositionInfo( 
            /* [out] */ frame_coordinate *coordinate,
            /* [out] */ double *timestamp) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ClearMovieTransitCache( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE InitFrameFilter( 
            /* [out] */ FLOAT *timeFrom,
            /* [out] */ FLOAT *timeTo,
            /* [out] */ FLOAT *tempObj,
            /* [out] */ FLOAT *tempArea,
            /* [out] */ SHORT *pickFrom,
            /* [out] */ SHORT *pickTo,
            /* [out] */ LONG *flags,
            /* [out] */ VARIANT_BOOL *checkedOnly,
            /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetFrameFilter( 
            /* [in] */ FLOAT timeFrom,
            /* [in] */ FLOAT timeTo,
            /* [in] */ FLOAT tempObj,
            /* [in] */ FLOAT tempArea,
            /* [in] */ SHORT picketFrom,
            /* [in] */ SHORT picketTo,
            /* [in] */ LONG flags,
            /* [retval][out] */ VARIANT_BOOL *res) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetPallete( 
            /* [in] */ BSTR palleteFileName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetDefaultPallete( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetPallete( 
            /* [out][in] */ VARIANT *pallete) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetPalleteLength( 
            /* [out] */ ULONG32 *number_colors,
            /* [retval][out] */ SHORT *len) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetPaletteCalibrationMode( 
            /* [in] */ calibration_mode mode) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetPaletteCalibration( 
            /* [in] */ float min,
            /* [in] */ float max) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AddArea( 
            /* [in] */ SHORT id,
            /* [in] */ area_info *area) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AreaChanged( 
            /* [in] */ SHORT id,
            /* [in] */ area_info *area) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RemoveArea( 
            /* [in] */ SHORT id,
            /* [out] */ area_type *type) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetAreasInfo( 
            /* [out][in] */ area_temperature_measure_result **results,
            /* [in] */ ULONG size) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetAreaInfo( 
            /* [in] */ ULONG aid,
            /* [out] */ area_temperature_measure *measure,
            /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RemoveAllAreas( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IMovieTransitVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMovieTransit * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMovieTransit * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMovieTransit * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMovieTransit * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMovieTransit * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMovieTransit * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMovieTransit * This,
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
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetIRBFiles )( 
            IMovieTransit * This,
            /* [in] */ VARIANT filesNames,
            /* [out] */ SAFEARRAY * *errors,
            /* [retval][out] */ VARIANT_BOOL *result);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *FilesCount )( 
            IMovieTransit * This,
            /* [retval][out] */ SHORT *filesCount);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *FramesCount )( 
            IMovieTransit * This,
            /* [retval][out] */ LONG *framesCount);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFrameRaster )( 
            IMovieTransit * This,
            /* [in] */ SHORT frameIndex,
            /* [out] */ irb_frame_info *frame_info,
            /* [out][in] */ VARIANT *frameRaster,
            /* [retval][out] */ VARIANT_BOOL *res);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFrame )( 
            IMovieTransit * This,
            /* [in] */ SHORT frameIndex,
            /* [out] */ irb_frame_info *frame_info,
            /* [out][in] */ VARIANT *pixels,
            /* [out][in] */ VARIANT *temp_values,
            /* [retval][out] */ VARIANT_BOOL *res);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *get_pixel_temperature )( 
            IMovieTransit * This,
            /* [in] */ DWORD frameIndex,
            /* [in] */ USHORT x,
            /* [in] */ USHORT y,
            /* [out] */ FLOAT *tempToReturn,
            /* [retval][out] */ VARIANT_BOOL *res);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SaveCurrentFrame )( 
            IMovieTransit * This,
            /* [in] */ BSTR path,
            /* [retval][out] */ SHORT *result);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Close )( 
            IMovieTransit * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCurFrameTemperatures )( 
            IMovieTransit * This,
            /* [out] */ temperature_measure *measure);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *IsFrameMeetFilter )( 
            IMovieTransit * This,
            /* [in] */ SHORT frameNum,
            /* [retval][out] */ VARIANT_BOOL *result);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFramePositionInfo )( 
            IMovieTransit * This,
            /* [in] */ ULONG frame_id,
            /* [out] */ frame_coordinate *coordinate,
            /* [out] */ double *timestamp);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCurrentFramePositionInfo )( 
            IMovieTransit * This,
            /* [out] */ frame_coordinate *coordinate,
            /* [out] */ double *timestamp);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ClearMovieTransitCache )( 
            IMovieTransit * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *InitFrameFilter )( 
            IMovieTransit * This,
            /* [out] */ FLOAT *timeFrom,
            /* [out] */ FLOAT *timeTo,
            /* [out] */ FLOAT *tempObj,
            /* [out] */ FLOAT *tempArea,
            /* [out] */ SHORT *pickFrom,
            /* [out] */ SHORT *pickTo,
            /* [out] */ LONG *flags,
            /* [out] */ VARIANT_BOOL *checkedOnly,
            /* [retval][out] */ VARIANT_BOOL *result);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetFrameFilter )( 
            IMovieTransit * This,
            /* [in] */ FLOAT timeFrom,
            /* [in] */ FLOAT timeTo,
            /* [in] */ FLOAT tempObj,
            /* [in] */ FLOAT tempArea,
            /* [in] */ SHORT picketFrom,
            /* [in] */ SHORT picketTo,
            /* [in] */ LONG flags,
            /* [retval][out] */ VARIANT_BOOL *res);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetPallete )( 
            IMovieTransit * This,
            /* [in] */ BSTR palleteFileName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetDefaultPallete )( 
            IMovieTransit * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetPallete )( 
            IMovieTransit * This,
            /* [out][in] */ VARIANT *pallete);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetPalleteLength )( 
            IMovieTransit * This,
            /* [out] */ ULONG32 *number_colors,
            /* [retval][out] */ SHORT *len);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetPaletteCalibrationMode )( 
            IMovieTransit * This,
            /* [in] */ calibration_mode mode);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetPaletteCalibration )( 
            IMovieTransit * This,
            /* [in] */ float min,
            /* [in] */ float max);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AddArea )( 
            IMovieTransit * This,
            /* [in] */ SHORT id,
            /* [in] */ area_info *area);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AreaChanged )( 
            IMovieTransit * This,
            /* [in] */ SHORT id,
            /* [in] */ area_info *area);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RemoveArea )( 
            IMovieTransit * This,
            /* [in] */ SHORT id,
            /* [out] */ area_type *type);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetAreasInfo )( 
            IMovieTransit * This,
            /* [out][in] */ area_temperature_measure_result **results,
            /* [in] */ ULONG size);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetAreaInfo )( 
            IMovieTransit * This,
            /* [in] */ ULONG aid,
            /* [out] */ area_temperature_measure *measure,
            /* [retval][out] */ VARIANT_BOOL *result);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RemoveAllAreas )( 
            IMovieTransit * This);
        
        END_INTERFACE
    } IMovieTransitVtbl;

    interface IMovieTransit
    {
        CONST_VTBL struct IMovieTransitVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMovieTransit_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMovieTransit_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMovieTransit_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMovieTransit_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMovieTransit_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMovieTransit_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMovieTransit_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMovieTransit_SetIRBFiles(This,filesNames,errors,result)	\
    ( (This)->lpVtbl -> SetIRBFiles(This,filesNames,errors,result) ) 

#define IMovieTransit_FilesCount(This,filesCount)	\
    ( (This)->lpVtbl -> FilesCount(This,filesCount) ) 

#define IMovieTransit_FramesCount(This,framesCount)	\
    ( (This)->lpVtbl -> FramesCount(This,framesCount) ) 

#define IMovieTransit_GetFrameRaster(This,frameIndex,frame_info,frameRaster,res)	\
    ( (This)->lpVtbl -> GetFrameRaster(This,frameIndex,frame_info,frameRaster,res) ) 

#define IMovieTransit_GetFrame(This,frameIndex,frame_info,pixels,temp_values,res)	\
    ( (This)->lpVtbl -> GetFrame(This,frameIndex,frame_info,pixels,temp_values,res) ) 

#define IMovieTransit_get_pixel_temperature(This,frameIndex,x,y,tempToReturn,res)	\
    ( (This)->lpVtbl -> get_pixel_temperature(This,frameIndex,x,y,tempToReturn,res) ) 

#define IMovieTransit_SaveCurrentFrame(This,path,result)	\
    ( (This)->lpVtbl -> SaveCurrentFrame(This,path,result) ) 

#define IMovieTransit_Close(This)	\
    ( (This)->lpVtbl -> Close(This) ) 

#define IMovieTransit_GetCurFrameTemperatures(This,measure)	\
    ( (This)->lpVtbl -> GetCurFrameTemperatures(This,measure) ) 

#define IMovieTransit_IsFrameMeetFilter(This,frameNum,result)	\
    ( (This)->lpVtbl -> IsFrameMeetFilter(This,frameNum,result) ) 

#define IMovieTransit_GetFramePositionInfo(This,frame_id,coordinate,timestamp)	\
    ( (This)->lpVtbl -> GetFramePositionInfo(This,frame_id,coordinate,timestamp) ) 

#define IMovieTransit_GetCurrentFramePositionInfo(This,coordinate,timestamp)	\
    ( (This)->lpVtbl -> GetCurrentFramePositionInfo(This,coordinate,timestamp) ) 

#define IMovieTransit_ClearMovieTransitCache(This)	\
    ( (This)->lpVtbl -> ClearMovieTransitCache(This) ) 

#define IMovieTransit_InitFrameFilter(This,timeFrom,timeTo,tempObj,tempArea,pickFrom,pickTo,flags,checkedOnly,result)	\
    ( (This)->lpVtbl -> InitFrameFilter(This,timeFrom,timeTo,tempObj,tempArea,pickFrom,pickTo,flags,checkedOnly,result) ) 

#define IMovieTransit_SetFrameFilter(This,timeFrom,timeTo,tempObj,tempArea,picketFrom,picketTo,flags,res)	\
    ( (This)->lpVtbl -> SetFrameFilter(This,timeFrom,timeTo,tempObj,tempArea,picketFrom,picketTo,flags,res) ) 

#define IMovieTransit_SetPallete(This,palleteFileName)	\
    ( (This)->lpVtbl -> SetPallete(This,palleteFileName) ) 

#define IMovieTransit_SetDefaultPallete(This)	\
    ( (This)->lpVtbl -> SetDefaultPallete(This) ) 

#define IMovieTransit_GetPallete(This,pallete)	\
    ( (This)->lpVtbl -> GetPallete(This,pallete) ) 

#define IMovieTransit_GetPalleteLength(This,number_colors,len)	\
    ( (This)->lpVtbl -> GetPalleteLength(This,number_colors,len) ) 

#define IMovieTransit_SetPaletteCalibrationMode(This,mode)	\
    ( (This)->lpVtbl -> SetPaletteCalibrationMode(This,mode) ) 

#define IMovieTransit_SetPaletteCalibration(This,min,max)	\
    ( (This)->lpVtbl -> SetPaletteCalibration(This,min,max) ) 

#define IMovieTransit_AddArea(This,id,area)	\
    ( (This)->lpVtbl -> AddArea(This,id,area) ) 

#define IMovieTransit_AreaChanged(This,id,area)	\
    ( (This)->lpVtbl -> AreaChanged(This,id,area) ) 

#define IMovieTransit_RemoveArea(This,id,type)	\
    ( (This)->lpVtbl -> RemoveArea(This,id,type) ) 

#define IMovieTransit_GetAreasInfo(This,results,size)	\
    ( (This)->lpVtbl -> GetAreasInfo(This,results,size) ) 

#define IMovieTransit_GetAreaInfo(This,aid,measure,result)	\
    ( (This)->lpVtbl -> GetAreaInfo(This,aid,measure,result) ) 

#define IMovieTransit_RemoveAllAreas(This)	\
    ( (This)->lpVtbl -> RemoveAllAreas(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMovieTransit_INTERFACE_DEFINED__ */



#ifndef __ThermoRoutineLib_LIBRARY_DEFINED__
#define __ThermoRoutineLib_LIBRARY_DEFINED__

/* library ThermoRoutineLib */
/* [custom][version][uuid] */ 


EXTERN_C const IID LIBID_ThermoRoutineLib;

EXTERN_C const CLSID CLSID_CompReg;

#ifdef __cplusplus

class DECLSPEC_UUID("F6ADF13C-570E-46BD-927D-E32831827AFD")
CompReg;
#endif

#ifndef ___ITRWrapperEvents_DISPINTERFACE_DEFINED__
#define ___ITRWrapperEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ITRWrapperEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__ITRWrapperEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("13D9C294-5D0B-45B3-BEB6-BA509B758712")
    _ITRWrapperEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ITRWrapperEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _ITRWrapperEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _ITRWrapperEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _ITRWrapperEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _ITRWrapperEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _ITRWrapperEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _ITRWrapperEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _ITRWrapperEvents * This,
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
    } _ITRWrapperEventsVtbl;

    interface _ITRWrapperEvents
    {
        CONST_VTBL struct _ITRWrapperEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ITRWrapperEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _ITRWrapperEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _ITRWrapperEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _ITRWrapperEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _ITRWrapperEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _ITRWrapperEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _ITRWrapperEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ITRWrapperEvents_DISPINTERFACE_DEFINED__ */


#ifndef ___IThermoLibErrorEvents_DISPINTERFACE_DEFINED__
#define ___IThermoLibErrorEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IThermoLibErrorEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__IThermoLibErrorEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("37F028AB-55D8-4838-8795-B2FED780095B")
    _IThermoLibErrorEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IThermoLibErrorEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IThermoLibErrorEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IThermoLibErrorEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IThermoLibErrorEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IThermoLibErrorEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IThermoLibErrorEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IThermoLibErrorEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IThermoLibErrorEvents * This,
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
    } _IThermoLibErrorEventsVtbl;

    interface _IThermoLibErrorEvents
    {
        CONST_VTBL struct _IThermoLibErrorEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IThermoLibErrorEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IThermoLibErrorEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IThermoLibErrorEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IThermoLibErrorEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IThermoLibErrorEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IThermoLibErrorEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IThermoLibErrorEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IThermoLibErrorEvents_DISPINTERFACE_DEFINED__ */


#ifndef ___IThermoLibComponentsEvents_DISPINTERFACE_DEFINED__
#define ___IThermoLibComponentsEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IThermoLibComponentsEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__IThermoLibComponentsEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("3DE144B6-936A-423A-9AF9-B472136D8024")
    _IThermoLibComponentsEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IThermoLibComponentsEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IThermoLibComponentsEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IThermoLibComponentsEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IThermoLibComponentsEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IThermoLibComponentsEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IThermoLibComponentsEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IThermoLibComponentsEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IThermoLibComponentsEvents * This,
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
    } _IThermoLibComponentsEventsVtbl;

    interface _IThermoLibComponentsEvents
    {
        CONST_VTBL struct _IThermoLibComponentsEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IThermoLibComponentsEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IThermoLibComponentsEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IThermoLibComponentsEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IThermoLibComponentsEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IThermoLibComponentsEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IThermoLibComponentsEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IThermoLibComponentsEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IThermoLibComponentsEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_TRWrapper;

#ifdef __cplusplus

class DECLSPEC_UUID("42D46C01-D857-481A-8B59-6E57C343D094")
TRWrapper;
#endif

EXTERN_C const CLSID CLSID_MovieTransit;

#ifdef __cplusplus

class DECLSPEC_UUID("4D7164CF-D373-41C6-850A-E73FFAC50E3A")
MovieTransit;
#endif
#endif /* __ThermoRoutineLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     unsigned long *, unsigned long            , LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserMarshal(  unsigned long *, unsigned char *, LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserUnmarshal(unsigned long *, unsigned char *, LPSAFEARRAY * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree(     unsigned long *, LPSAFEARRAY * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


