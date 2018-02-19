#include <error_lib/win32_error.h>
#include <error_lib/com_error_codes.h>
#include <common/string_utils.h>

//#include <resources/event_messages.h>
//#include <message_localization_lib/message.h>

#include <algorithm>

namespace win32
{

	namespace
	{
		using table_item_t = std::pair<com_errc, const char*>;
		const table_item_t code_to_text_table[] =
		{
			{ com_errc::e_unexpected, "Catastrophic failure. Code: 0x8000FFFF" },	//	E_UNEXPECTED
			{ com_errc::e_notimpl, "Not implemented. Code: 0x80004001" },	//	E_NOTIMPL
			{ com_errc::e_outofmemory, "Ran out of memory. Code: 0x8007000E" },	//	E_OUTOFMEMORY
			{ com_errc::e_invalidarg, "One or more arguments are invalid. Code: 0x80070057" },	//	E_INVALIDARG
			{ com_errc::e_nointerface, "No such interface supported. Code: 0x80004002" },	//	E_NOINTERFACE
			{ com_errc::e_pointer, "Invalid pointer. Code: 0x80004003" },	//	E_POINTER
			{ com_errc::e_handle, "Invalid handle. Code: 0x80070006" },	//	E_HANDLE
			{ com_errc::e_abort, "Operation aborted. Code: 0x80004004" },	//	E_ABORT
			{ com_errc::e_fail, "Unspecified error. Code: 0x80004005" },	//	E_FAIL
			{ com_errc::e_accessdenied, "General access denied error. Code: 0x80070005" },	//	E_ACCESSDENIED
			{ com_errc::e_pending, "The data necessary to complete this operation is not yet available. Code: 0x8000000A" },	//	E_PENDING
			{ com_errc::e_bounds, "The operation attempted to access data outside the valid range. Code: 0x8000000B" },	//	E_BOUNDS
			{ com_errc::e_changed_state, "A concurrent or interleaved operation changed the state of the object, invalidating this operation. Code: 0x8000000C" },	//	E_CHANGED_STATE
			{ com_errc::e_illegal_state_change, "An illegal state change was requested. Code: 0x8000000D" },	//	E_ILLEGAL_STATE_CHANGE
			{ com_errc::e_illegal_method_call, "A method was called at an unexpected time. Code: 0x8000000E" },	//	E_ILLEGAL_METHOD_CALL
			{ com_errc::ro_e_metadata_name_not_found, "Typename or Namespace was not found in metadata file. Code: 0x8000000F" },	//	RO_E_METADATA_NAME_NOT_FOUND
			{ com_errc::ro_e_metadata_name_is_namespace, "Name is an existing namespace rather than a typename. Code: 0x80000010" },	//	RO_E_METADATA_NAME_IS_NAMESPACE
			{ com_errc::ro_e_metadata_invalid_type_format, "Typename has an invalid format. Code: 0x80000011" },	//	RO_E_METADATA_INVALID_TYPE_FORMAT
			{ com_errc::ro_e_invalid_metadata_file, "Metadata file is invalid or corrupted. Code: 0x80000012" },	//	RO_E_INVALID_METADATA_FILE
			{ com_errc::ro_e_closed, "The object has been closed. Code: 0x80000013" },	//	RO_E_CLOSED
			{ com_errc::ro_e_exclusive_write, "Only one thread may access the object during a write operation. Code: 0x80000014" },	//	RO_E_EXCLUSIVE_WRITE
			{ com_errc::ro_e_change_notification_in_progress, "Operation is prohibited during change notification. Code: 0x80000015" },	//	RO_E_CHANGE_NOTIFICATION_IN_PROGRESS
			{ com_errc::ro_e_error_string_not_found, "The text associated with this error code could not be found. Code: 0x80000016" },	//	RO_E_ERROR_STRING_NOT_FOUND
			{ com_errc::e_string_not_null_terminated, "String not null terminated. Code: 0x80000017" },	//	E_STRING_NOT_NULL_TERMINATED
			{ com_errc::e_illegal_delegate_assignment, "A delegate was assigned when not allowed. Code: 0x80000018" },	//	E_ILLEGAL_DELEGATE_ASSIGNMENT
			{ com_errc::e_async_operation_not_started, "An async operation was not properly started. Code: 0x80000019" },	//	E_ASYNC_OPERATION_NOT_STARTED
			{ com_errc::e_application_exiting, "The application is exiting and cannot service this request. Code: 0x8000001A" },	//	E_APPLICATION_EXITING
			{ com_errc::e_application_view_exiting, "The application view is exiting and cannot service this request. Code: 0x8000001B" },	//	E_APPLICATION_VIEW_EXITING
			{ com_errc::ro_e_must_be_agile, "The object must support the IAgileObject interface. Code: 0x8000001C" },	//	RO_E_MUST_BE_AGILE
			{ com_errc::ro_e_unsupported_from_mta, "Activating a single-threaded class from MTA is not supported. Code: 0x8000001D" },	//	RO_E_UNSUPPORTED_FROM_MTA
			{ com_errc::ro_e_committed, "The object has been committed. Code: 0x8000001E" },	//	RO_E_COMMITTED
			{ com_errc::ro_e_blocked_cross_asta_call, "A COM call to an ASTA was blocked because the call chain originated in or passed through another ASTA. This call pattern is deadlock-prone and disallowed by apartment call control. Code: 0x8000001F" },	//	RO_E_BLOCKED_CROSS_ASTA_CALL
			{ com_errc::co_e_init_tls, "Thread local storage failure. Code: 0x80004006" },	//	CO_E_INIT_TLS
			{ com_errc::co_e_init_shared_allocator, "Get shared memory allocator failure. Code: 0x80004007" },	//	CO_E_INIT_SHARED_ALLOCATOR
			{ com_errc::co_e_init_memory_allocator, "Get memory allocator failure. Code: 0x80004008" },	//	CO_E_INIT_MEMORY_ALLOCATOR
			{ com_errc::co_e_init_class_cache, "Unable to initialize class cache. Code: 0x80004009" },	//	CO_E_INIT_CLASS_CACHE
			{ com_errc::co_e_init_rpc_channel, "Unable to initialize RPC services. Code: 0x8000400A" },	//	CO_E_INIT_RPC_CHANNEL
			{ com_errc::co_e_init_tls_set_channel_control, "Cannot set thread local storage channel control. Code: 0x8000400B" },	//	CO_E_INIT_TLS_SET_CHANNEL_CONTROL
			{ com_errc::co_e_init_tls_channel_control, "Could not allocate thread local storage channel control. Code: 0x8000400C" },	//	CO_E_INIT_TLS_CHANNEL_CONTROL
			{ com_errc::co_e_init_unaccepted_user_allocator, "The user supplied memory allocator is unacceptable. Code: 0x8000400D" },	//	CO_E_INIT_UNACCEPTED_USER_ALLOCATOR
			{ com_errc::co_e_init_scm_mutex_exists, "The OLE service mutex already exists. Code: 0x8000400E" },	//	CO_E_INIT_SCM_MUTEX_EXISTS
			{ com_errc::co_e_init_scm_file_mapping_exists, "The OLE service file mapping already exists. Code: 0x8000400F" },	//	CO_E_INIT_SCM_FILE_MAPPING_EXISTS
			{ com_errc::co_e_init_scm_map_view_of_file, "Unable to map view of file for OLE service. Code: 0x80004010" },	//	CO_E_INIT_SCM_MAP_VIEW_OF_FILE
			{ com_errc::co_e_init_scm_exec_failure, "Failure attempting to launch OLE service. Code: 0x80004011" },	//	CO_E_INIT_SCM_EXEC_FAILURE
			{ com_errc::co_e_init_only_single_threaded, "There was an attempt to call CoInitialize a second time while single threaded. Code: 0x80004012" },	//	CO_E_INIT_ONLY_SINGLE_THREADED
			{ com_errc::co_e_cant_remote, "A Remote activation was necessary but was not allowed. Code: 0x80004013" },	//	CO_E_CANT_REMOTE
			{ com_errc::co_e_bad_server_name, "A Remote activation was necessary but the server name provided was invalid. Code: 0x80004014" },	//	CO_E_BAD_SERVER_NAME
			{ com_errc::co_e_wrong_server_identity, "The class is configured to run as a security id different from the caller. Code: 0x80004015" },	//	CO_E_WRONG_SERVER_IDENTITY
			{ com_errc::co_e_ole1dde_disabled, "Use of Ole1 services requiring DDE windows is disabled. Code: 0x80004016" },	//	CO_E_OLE1DDE_DISABLED
			{ com_errc::co_e_runas_syntax, "A RunAs specification must be <domain name>\\<user name> or simply <user name>. Code: 0x80004017" },	//	CO_E_RUNAS_SYNTAX
			{ com_errc::co_e_createprocess_failure, "The server process could not be started. The pathname may be incorrect. Code: 0x80004018" },	//	CO_E_CREATEPROCESS_FAILURE
			{ com_errc::co_e_runas_createprocess_failure, "The server process could not be started as the configured identity. The pathname may be incorrect or unavailable. Code: 0x80004019" },	//	CO_E_RUNAS_CREATEPROCESS_FAILURE
			{ com_errc::co_e_runas_logon_failure, "The server process could not be started because the configured identity is incorrect. Check the username and password. Code: 0x8000401A" },	//	CO_E_RUNAS_LOGON_FAILURE
			{ com_errc::co_e_launch_permssion_denied, "The client is not allowed to launch this server. Code: 0x8000401B" },	//	CO_E_LAUNCH_PERMSSION_DENIED
			{ com_errc::co_e_start_service_failure, "The service providing this server could not be started. Code: 0x8000401C" },	//	CO_E_START_SERVICE_FAILURE
			{ com_errc::co_e_remote_communication_failure, "This computer was unable to communicate with the computer providing the server. Code: 0x8000401D" },	//	CO_E_REMOTE_COMMUNICATION_FAILURE
			{ com_errc::co_e_server_start_timeout, "The server did not respond after being launched. Code: 0x8000401E" },	//	CO_E_SERVER_START_TIMEOUT
			{ com_errc::co_e_clsreg_inconsistent, "The registration information for this server is inconsistent or incomplete. Code: 0x8000401F" },	//	CO_E_CLSREG_INCONSISTENT
			{ com_errc::co_e_iidreg_inconsistent, "The registration information for this interface is inconsistent or incomplete. Code: 0x80004020" },	//	CO_E_IIDREG_INCONSISTENT
			{ com_errc::co_e_not_supported, "The operation attempted is not supported. Code: 0x80004021" },	//	CO_E_NOT_SUPPORTED
			{ com_errc::co_e_reload_dll, "A dll must be loaded. Code: 0x80004022" },	//	CO_E_RELOAD_DLL
			{ com_errc::co_e_msi_error, "A Microsoft Software Installer error was encountered. Code: 0x80004023" },	//	CO_E_MSI_ERROR
			{ com_errc::co_e_attempt_to_create_outside_client_context, "The specified activation could not occur in the client context as specified. Code: 0x80004024" },	//	CO_E_ATTEMPT_TO_CREATE_OUTSIDE_CLIENT_CONTEXT
			{ com_errc::co_e_server_paused, "Activations on the server are paused. Code: 0x80004025" },	//	CO_E_SERVER_PAUSED
			{ com_errc::co_e_server_not_paused, "Activations on the server are not paused. Code: 0x80004026" },	//	CO_E_SERVER_NOT_PAUSED
			{ com_errc::co_e_class_disabled, "The component or application containing the component has been disabled. Code: 0x80004027" },	//	CO_E_CLASS_DISABLED
			{ com_errc::co_e_clrnotavailable, "The common language runtime is not available. Code: 0x80004028" },	//	CO_E_CLRNOTAVAILABLE
			{ com_errc::co_e_async_work_rejected, "The thread-pool rejected the submitted asynchronous work. Code: 0x80004029" },	//	CO_E_ASYNC_WORK_REJECTED
			{ com_errc::co_e_server_init_timeout, "The server started, but did not finish initializing in a timely fashion. Code: 0x8000402A" },	//	CO_E_SERVER_INIT_TIMEOUT
			{ com_errc::co_e_no_secctx_in_activate, "Unable to complete the call since there is no COM+ security context inside IObjectControl.Activate. Code: 0x8000402B" },	//	CO_E_NO_SECCTX_IN_ACTIVATE
			{ com_errc::co_e_tracker_config, "The provided tracker configuration is invalid. Code: 0x80004030" },	//	CO_E_TRACKER_CONFIG
			{ com_errc::co_e_threadpool_config, "The provided thread pool configuration is invalid. Code: 0x80004031" },	//	CO_E_THREADPOOL_CONFIG
			{ com_errc::co_e_sxs_config, "The provided side-by-side configuration is invalid. Code: 0x80004032" },	//	CO_E_SXS_CONFIG
			{ com_errc::co_e_malformed_spn, "The server principal name (SPN) obtained during security negotiation is malformed. Code: 0x80004033" },	//	CO_E_MALFORMED_SPN
			{ com_errc::co_e_unrevoked_registration_on_apartment_shutdown, "The caller failed to revoke a per-apartment registration before apartment shutdown. Code: 0x80004034" },	//	CO_E_UNREVOKED_REGISTRATION_ON_APARTMENT_SHUTDOWN
			{ com_errc::co_e_premature_stub_rundown, "The object has been rundown by the stub manager while there are external clients. Code: 0x80004035" },	//	CO_E_PREMATURE_STUB_RUNDOWN
			{ com_errc::ole_e_oleverb, "Invalid OLEVERB structure. Code: 0x80040000" },	//	OLE_E_OLEVERB
			{ com_errc::ole_e_advf, "Invalid advise flags. Code: 0x80040001" },	//	OLE_E_ADVF
			{ com_errc::ole_e_enum_nomore, "Can't enumerate any more, because the associated data is missing. Code: 0x80040002" },	//	OLE_E_ENUM_NOMORE
			{ com_errc::ole_e_advisenotsupported, "This implementation doesn't take advises. Code: 0x80040003" },	//	OLE_E_ADVISENOTSUPPORTED
			{ com_errc::ole_e_noconnection, "There is no connection for this connection ID. Code: 0x80040004" },	//	OLE_E_NOCONNECTION
			{ com_errc::ole_e_notrunning, "Need to run the object to perform this operation. Code: 0x80040005" },	//	OLE_E_NOTRUNNING
			{ com_errc::ole_e_nocache, "There is no cache to operate on. Code: 0x80040006" },	//	OLE_E_NOCACHE
			{ com_errc::ole_e_blank, "Uninitialized object. Code: 0x80040007" },	//	OLE_E_BLANK
			{ com_errc::ole_e_classdiff, "Linked object's source class has changed. Code: 0x80040008" },	//	OLE_E_CLASSDIFF
			{ com_errc::ole_e_cant_getmoniker, "Not able to get the moniker of the object. Code: 0x80040009" },	//	OLE_E_CANT_GETMONIKER
			{ com_errc::ole_e_cant_bindtosource, "Not able to bind to the source. Code: 0x8004000A" },	//	OLE_E_CANT_BINDTOSOURCE
			{ com_errc::ole_e_static, "Object is static; operation not allowed. Code: 0x8004000B" },	//	OLE_E_STATIC
			{ com_errc::ole_e_promptsavecancelled, "User canceled out of save dialog. Code: 0x8004000C" },	//	OLE_E_PROMPTSAVECANCELLED
			{ com_errc::ole_e_invalidrect, "Invalid rectangle. Code: 0x8004000D" },	//	OLE_E_INVALIDRECT
			{ com_errc::ole_e_wrongcompobj, "compobj.dll is too old for the ole2.dll initialized. Code: 0x8004000E" },	//	OLE_E_WRONGCOMPOBJ
			{ com_errc::ole_e_invalidhwnd, "Invalid window handle. Code: 0x8004000F" },	//	OLE_E_INVALIDHWND
			{ com_errc::ole_e_not_inplaceactive, "Object is not in any of the inplace active states. Code: 0x80040010" },	//	OLE_E_NOT_INPLACEACTIVE
			{ com_errc::ole_e_cantconvert, "Not able to convert object. Code: 0x80040011" },	//	OLE_E_CANTCONVERT
			{ com_errc::ole_e_nostorage, "Not able to perform the operation because object is not given storage yet. Code: 0x80040012" },	//	OLE_E_NOSTORAGE
			{ com_errc::dv_e_formatetc, "Invalid FORMATETC structure. Code: 0x80040064" },	//	DV_E_FORMATETC
			{ com_errc::dv_e_dvtargetdevice, "Invalid DVTARGETDEVICE structure. Code: 0x80040065" },	//	DV_E_DVTARGETDEVICE
			{ com_errc::dv_e_stgmedium, "Invalid STDGMEDIUM structure. Code: 0x80040066" },	//	DV_E_STGMEDIUM
			{ com_errc::dv_e_statdata, "Invalid STATDATA structure. Code: 0x80040067" },	//	DV_E_STATDATA
			{ com_errc::dv_e_lindex, "Invalid lindex. Code: 0x80040068" },	//	DV_E_LINDEX
			{ com_errc::dv_e_tymed, "Invalid tymed. Code: 0x80040069" },	//	DV_E_TYMED
			{ com_errc::dv_e_clipformat, "Invalid clipboard format. Code: 0x8004006A" },	//	DV_E_CLIPFORMAT
			{ com_errc::dv_e_dvaspect, "Invalid aspect(s). Code: 0x8004006B" },	//	DV_E_DVASPECT
			{ com_errc::dv_e_dvtargetdevice_size, "tdSize parameter of the DVTARGETDEVICE structure is invalid. Code: 0x8004006C" },	//	DV_E_DVTARGETDEVICE_SIZE
			{ com_errc::dv_e_noiviewobject, "Object doesn't support IViewObject interface. Code: 0x8004006D" },	//	DV_E_NOIVIEWOBJECT
			{ com_errc::dragdrop_e_notregistered, "Trying to revoke a drop target that has not been registered. Code: 0x80040100" },	//	DRAGDROP_E_NOTREGISTERED
			{ com_errc::dragdrop_e_alreadyregistered, "This window has already been registered as a drop target. Code: 0x80040101" },	//	DRAGDROP_E_ALREADYREGISTERED
			{ com_errc::dragdrop_e_invalidhwnd, "Invalid window handle. Code: 0x80040102" },	//	DRAGDROP_E_INVALIDHWND
			{ com_errc::dragdrop_e_concurrent_drag_attempted, "A drag operation is already in progress. Code: 0x80040103" },	//	DRAGDROP_E_CONCURRENT_DRAG_ATTEMPTED
			{ com_errc::class_e_noaggregation, "Class does not support aggregation (or class object is remote). Code: 0x80040110" },	//	CLASS_E_NOAGGREGATION
			{ com_errc::class_e_classnotavailable, "ClassFactory cannot supply requested class. Code: 0x80040111" },	//	CLASS_E_CLASSNOTAVAILABLE
			{ com_errc::class_e_notlicensed, "Class is not licensed for use. Code: 0x80040112" },	//	CLASS_E_NOTLICENSED
			{ com_errc::view_e_draw, "Error drawing view. Code: 0x80040140" },	//	VIEW_E_DRAW
			{ com_errc::regdb_e_readregdb, "Could not read key from registry. Code: 0x80040150" },	//	REGDB_E_READREGDB
			{ com_errc::regdb_e_writeregdb, "Could not write key to registry. Code: 0x80040151" },	//	REGDB_E_WRITEREGDB
			{ com_errc::regdb_e_keymissing, "Could not find the key in the registry. Code: 0x80040152" },	//	REGDB_E_KEYMISSING
			{ com_errc::regdb_e_invalidvalue, "Invalid value for registry. Code: 0x80040153" },	//	REGDB_E_INVALIDVALUE
			{ com_errc::regdb_e_classnotreg, "Class not registered. Code: 0x80040154" },	//	REGDB_E_CLASSNOTREG
			{ com_errc::regdb_e_iidnotreg, "Interface not registered. Code: 0x80040155" },	//	REGDB_E_IIDNOTREG
			{ com_errc::regdb_e_badthreadingmodel, "Threading model entry is not valid. Code: 0x80040156" },	//	REGDB_E_BADTHREADINGMODEL
			{ com_errc::cat_e_catidnoexist, "CATID does not exist. Code: 0x80040160" },	//	CAT_E_CATIDNOEXIST
			{ com_errc::cat_e_nodescription, "Description not found. Code: 0x80040161" },	//	CAT_E_NODESCRIPTION
			{ com_errc::cs_e_package_notfound, "No package in the software installation data in the Active Directory meets this criteria. Code: 0x80040164" },	//	CS_E_PACKAGE_NOTFOUND
			{ com_errc::cs_e_not_deletable, "Deleting this will break the referential integrity of the software installation data in the Active Directory. Code: 0x80040165" },	//	CS_E_NOT_DELETABLE
			{ com_errc::cs_e_class_notfound, "The CLSID was not found in the software installation data in the Active Directory. Code: 0x80040166" },	//	CS_E_CLASS_NOTFOUND
			{ com_errc::cs_e_invalid_version, "The software installation data in the Active Directory is corrupt. Code: 0x80040167" },	//	CS_E_INVALID_VERSION
			{ com_errc::cs_e_no_classstore, "There is no software installation data in the Active Directory. Code: 0x80040168" },	//	CS_E_NO_CLASSSTORE
			{ com_errc::cs_e_object_notfound, "There is no software installation data object in the Active Directory. Code: 0x80040169" },	//	CS_E_OBJECT_NOTFOUND
			{ com_errc::cs_e_object_already_exists, "The software installation data object in the Active Directory already exists. Code: 0x8004016A" },	//	CS_E_OBJECT_ALREADY_EXISTS
			{ com_errc::cs_e_invalid_path, "The path to the software installation data in the Active Directory is not correct. Code: 0x8004016B" },	//	CS_E_INVALID_PATH
			{ com_errc::cs_e_network_error, "A network error interrupted the operation. Code: 0x8004016C" },	//	CS_E_NETWORK_ERROR
			{ com_errc::cs_e_admin_limit_exceeded, "The size of this object exceeds the maximum size set by the Administrator. Code: 0x8004016D" },	//	CS_E_ADMIN_LIMIT_EXCEEDED
			{ com_errc::cs_e_schema_mismatch, "The schema for the software installation data in the Active Directory does not match the required schema. Code: 0x8004016E" },	//	CS_E_SCHEMA_MISMATCH
			{ com_errc::cs_e_internal_error, "An error occurred in the software installation data in the Active Directory. Code: 0x8004016F" },	//	CS_E_INTERNAL_ERROR
			{ com_errc::cache_e_nocache_updated, "Cache not updated. Code: 0x80040170" },	//	CACHE_E_NOCACHE_UPDATED
			{ com_errc::oleobj_e_noverbs, "No verbs for OLE object. Code: 0x80040180" },	//	OLEOBJ_E_NOVERBS
			{ com_errc::oleobj_e_invalidverb, "Invalid verb for OLE object. Code: 0x80040181" },	//	OLEOBJ_E_INVALIDVERB
			{ com_errc::inplace_e_notundoable, "Undo is not available. Code: 0x800401A0" },	//	INPLACE_E_NOTUNDOABLE
			{ com_errc::inplace_e_notoolspace, "Space for tools is not available. Code: 0x800401A1" },	//	INPLACE_E_NOTOOLSPACE
			{ com_errc::convert10_e_olestream_get, "OLESTREAM Get method failed. Code: 0x800401C0" },	//	CONVERT10_E_OLESTREAM_GET
			{ com_errc::convert10_e_olestream_put, "OLESTREAM Put method failed. Code: 0x800401C1" },	//	CONVERT10_E_OLESTREAM_PUT
			{ com_errc::convert10_e_olestream_fmt, "Contents of the OLESTREAM not in correct format. Code: 0x800401C2" },	//	CONVERT10_E_OLESTREAM_FMT
			{ com_errc::convert10_e_olestream_bitmap_to_dib, "There was an error in a Windows GDI call while converting the bitmap to a DIB. Code: 0x800401C3" },	//	CONVERT10_E_OLESTREAM_BITMAP_TO_DIB
			{ com_errc::convert10_e_stg_fmt, "Contents of the IStorage not in correct format. Code: 0x800401C4" },	//	CONVERT10_E_STG_FMT
			{ com_errc::convert10_e_stg_no_std_stream, "Contents of IStorage is missing one of the standard streams. Code: 0x800401C5" },	//	CONVERT10_E_STG_NO_STD_STREAM
			{ com_errc::convert10_e_stg_dib_to_bitmap, "There was an error in a Windows GDI call while converting the DIB to a bitmap. Code: 0x800401C6" },	//	CONVERT10_E_STG_DIB_TO_BITMAP
			{ com_errc::clipbrd_e_cant_open, "OpenClipboard Failed. Code: 0x800401D0" },	//	CLIPBRD_E_CANT_OPEN
			{ com_errc::clipbrd_e_cant_empty, "EmptyClipboard Failed. Code: 0x800401D1" },	//	CLIPBRD_E_CANT_EMPTY
			{ com_errc::clipbrd_e_cant_set, "SetClipboard Failed. Code: 0x800401D2" },	//	CLIPBRD_E_CANT_SET
			{ com_errc::clipbrd_e_bad_data, "Data on clipboard is invalid. Code: 0x800401D3" },	//	CLIPBRD_E_BAD_DATA
			{ com_errc::clipbrd_e_cant_close, "CloseClipboard Failed. Code: 0x800401D4" },	//	CLIPBRD_E_CANT_CLOSE
			{ com_errc::mk_e_connectmanually, "Moniker needs to be connected manually. Code: 0x800401E0" },	//	MK_E_CONNECTMANUALLY
			{ com_errc::mk_e_exceededdeadline, "Operation exceeded deadline. Code: 0x800401E1" },	//	MK_E_EXCEEDEDDEADLINE
			{ com_errc::mk_e_needgeneric, "Moniker needs to be generic. Code: 0x800401E2" },	//	MK_E_NEEDGENERIC
			{ com_errc::mk_e_unavailable, "Operation unavailable. Code: 0x800401E3" },	//	MK_E_UNAVAILABLE
			{ com_errc::mk_e_syntax, "Invalid syntax. Code: 0x800401E4" },	//	MK_E_SYNTAX
			{ com_errc::mk_e_noobject, "No object for moniker. Code: 0x800401E5" },	//	MK_E_NOOBJECT
			{ com_errc::mk_e_invalidextension, "Bad extension for file. Code: 0x800401E6" },	//	MK_E_INVALIDEXTENSION
			{ com_errc::mk_e_intermediateinterfacenotsupported, "Intermediate operation failed. Code: 0x800401E7" },	//	MK_E_INTERMEDIATEINTERFACENOTSUPPORTED
			{ com_errc::mk_e_notbindable, "Moniker is not bindable. Code: 0x800401E8" },	//	MK_E_NOTBINDABLE
			{ com_errc::mk_e_notbound, "Moniker is not bound. Code: 0x800401E9" },	//	MK_E_NOTBOUND
			{ com_errc::mk_e_cantopenfile, "Moniker cannot open file. Code: 0x800401EA" },	//	MK_E_CANTOPENFILE
			{ com_errc::mk_e_mustbotheruser, "User input required for operation to succeed. Code: 0x800401EB" },	//	MK_E_MUSTBOTHERUSER
			{ com_errc::mk_e_noinverse, "Moniker class has no inverse. Code: 0x800401EC" },	//	MK_E_NOINVERSE
			{ com_errc::mk_e_nostorage, "Moniker does not refer to storage. Code: 0x800401ED" },	//	MK_E_NOSTORAGE
			{ com_errc::mk_e_noprefix, "No common prefix. Code: 0x800401EE" },	//	MK_E_NOPREFIX
			{ com_errc::mk_e_enumeration_failed, "Moniker could not be enumerated. Code: 0x800401EF" },	//	MK_E_ENUMERATION_FAILED
			{ com_errc::co_e_notinitialized, "CoInitialize has not been called. Code: 0x800401F0" },	//	CO_E_NOTINITIALIZED
			{ com_errc::co_e_alreadyinitialized, "CoInitialize has already been called. Code: 0x800401F1" },	//	CO_E_ALREADYINITIALIZED
			{ com_errc::co_e_cantdetermineclass, "Class of object cannot be determined. Code: 0x800401F2" },	//	CO_E_CANTDETERMINECLASS
			{ com_errc::co_e_classstring, "Invalid class string. Code: 0x800401F3" },	//	CO_E_CLASSSTRING
			{ com_errc::co_e_iidstring, "Invalid interface string. Code: 0x800401F4" },	//	CO_E_IIDSTRING
			{ com_errc::co_e_appnotfound, "Application not found. Code: 0x800401F5" },	//	CO_E_APPNOTFOUND
			{ com_errc::co_e_appsingleuse, "Application cannot be run more than once. Code: 0x800401F6" },	//	CO_E_APPSINGLEUSE
			{ com_errc::co_e_errorinapp, "Some error in application program. Code: 0x800401F7" },	//	CO_E_ERRORINAPP
			{ com_errc::co_e_dllnotfound, "DLL for class not found. Code: 0x800401F8" },	//	CO_E_DLLNOTFOUND
			{ com_errc::co_e_errorindll, "Error in the DLL. Code: 0x800401F9" },	//	CO_E_ERRORINDLL
			{ com_errc::co_e_wrongosforapp, "Wrong OS or OS version for application. Code: 0x800401FA" },	//	CO_E_WRONGOSFORAPP
			{ com_errc::co_e_objnotreg, "Object is not registered. Code: 0x800401FB" },	//	CO_E_OBJNOTREG
			{ com_errc::co_e_objisreg, "Object is already registered. Code: 0x800401FC" },	//	CO_E_OBJISREG
			{ com_errc::co_e_objnotconnected, "Object is not connected to server. Code: 0x800401FD" },	//	CO_E_OBJNOTCONNECTED
			{ com_errc::co_e_appdidntreg, "Application was launched but it didn't register a class factory. Code: 0x800401FE" },	//	CO_E_APPDIDNTREG
			{ com_errc::co_e_released, "Object has been released. Code: 0x800401FF" },	//	CO_E_RELEASED
			{ com_errc::event_s_some_subscribers_failed, "An event was able to invoke some but not all of the subscribers. Code: 0x00040200" },	//	EVENT_S_SOME_SUBSCRIBERS_FAILED
			{ com_errc::event_e_all_subscribers_failed, "An event was unable to invoke any of the subscribers. Code: 0x80040201" },	//	EVENT_E_ALL_SUBSCRIBERS_FAILED
			{ com_errc::event_s_nosubscribers, "An event was delivered but there were no subscribers. Code: 0x00040202" },	//	EVENT_S_NOSUBSCRIBERS
			{ com_errc::event_e_querysyntax, "A syntax error occurred trying to evaluate a query string. Code: 0x80040203" },	//	EVENT_E_QUERYSYNTAX
			{ com_errc::event_e_queryfield, "An invalid field name was used in a query string. Code: 0x80040204" },	//	EVENT_E_QUERYFIELD
			{ com_errc::event_e_internalexception, "An unexpected exception was raised. Code: 0x80040205" },	//	EVENT_E_INTERNALEXCEPTION
			{ com_errc::event_e_internalerror, "An unexpected internal error was detected. Code: 0x80040206" },	//	EVENT_E_INTERNALERROR
			{ com_errc::event_e_invalid_per_user_sid, "The owner SID on a per-user subscription doesn't exist. Code: 0x80040207" },	//	EVENT_E_INVALID_PER_USER_SID
			{ com_errc::event_e_user_exception, "A user-supplied component or subscriber raised an exception. Code: 0x80040208" },	//	EVENT_E_USER_EXCEPTION
			{ com_errc::event_e_too_many_methods, "An interface has too many methods to fire events from. Code: 0x80040209" },	//	EVENT_E_TOO_MANY_METHODS
			{ com_errc::event_e_missing_eventclass, "A subscription cannot be stored unless its event class already exists. Code: 0x8004020A" },	//	EVENT_E_MISSING_EVENTCLASS
			{ com_errc::event_e_not_all_removed, "Not all the objects requested could be removed. Code: 0x8004020B" },	//	EVENT_E_NOT_ALL_REMOVED
			{ com_errc::event_e_complus_not_installed, "COM+ is required for this operation, but is not installed. Code: 0x8004020C" },	//	EVENT_E_COMPLUS_NOT_INSTALLED
			{ com_errc::event_e_cant_modify_or_delete_unconfigured_object, "Cannot modify or delete an object that was not added using the COM+ Admin SDK. Code: 0x8004020D" },	//	EVENT_E_CANT_MODIFY_OR_DELETE_UNCONFIGURED_OBJECT
			{ com_errc::event_e_cant_modify_or_delete_configured_object, "Cannot modify or delete an object that was added using the COM+ Admin SDK. Code: 0x8004020E" },	//	EVENT_E_CANT_MODIFY_OR_DELETE_CONFIGURED_OBJECT
			{ com_errc::event_e_invalid_event_class_partition, "The event class for this subscription is in an invalid partition. Code: 0x8004020F" },	//	EVENT_E_INVALID_EVENT_CLASS_PARTITION
			{ com_errc::event_e_per_user_sid_not_logged_on, "The owner of the PerUser subscription is not logged on to the system specified. Code: 0x80040210" },	//	EVENT_E_PER_USER_SID_NOT_LOGGED_ON
			{ com_errc::tpc_e_invalid_property, "TabletPC inking error code. The property was not found, or supported by the recognizer. Code: 0x80040241" },	//	TPC_E_INVALID_PROPERTY
			{ com_errc::tpc_e_no_default_tablet, "TabletPC inking error code. No default tablet. Code: 0x80040212" },	//	TPC_E_NO_DEFAULT_TABLET
			{ com_errc::tpc_e_unknown_property, "TabletPC inking error code. Unknown property specified. Code: 0x8004021B" },	//	TPC_E_UNKNOWN_PROPERTY
			{ com_errc::tpc_e_invalid_input_rect, "TabletPC inking error code. An invalid input rectangle was specified. Code: 0x80040219" },	//	TPC_E_INVALID_INPUT_RECT
			{ com_errc::tpc_e_invalid_stroke, "TabletPC inking error code. The stroke object was deleted. Code: 0x80040222" },	//	TPC_E_INVALID_STROKE
			{ com_errc::tpc_e_initialize_fail, "TabletPC inking error code. Initialization failure. Code: 0x80040223" },	//	TPC_E_INITIALIZE_FAIL
			{ com_errc::tpc_e_not_relevant, "TabletPC inking error code. The data required for the operation was not supplied. Code: 0x80040232" },	//	TPC_E_NOT_RELEVANT
			{ com_errc::tpc_e_invalid_packet_description, "TabletPC inking error code. Invalid packet description. Code: 0x80040233" },	//	TPC_E_INVALID_PACKET_DESCRIPTION
			{ com_errc::tpc_e_recognizer_not_registered, "TabletPC inking error code. There are no handwriting recognizers registered. Code: 0x80040235" },	//	TPC_E_RECOGNIZER_NOT_REGISTERED
			{ com_errc::tpc_e_invalid_rights, "TabletPC inking error code. User does not have the necessary rights to read recognizer information. Code: 0x80040236" },	//	TPC_E_INVALID_RIGHTS
			{ com_errc::tpc_e_out_of_order_call, "TabletPC inking error code. API calls were made in an incorrect order. Code: 0x80040237" },	//	TPC_E_OUT_OF_ORDER_CALL
			{ com_errc::tpc_e_queue_full, "TabletPC inking error code. Queue is full. Code: 0x80040238" },	//	TPC_E_QUEUE_FULL
			{ com_errc::tpc_e_invalid_configuration, "TabletPC inking error code. RtpEnabled called multiple times. Code: 0x80040239" },	//	TPC_E_INVALID_CONFIGURATION
			{ com_errc::tpc_e_invalid_data_from_recognizer, "TabletPC inking error code. A recognizer returned invalid data. Code: 0x8004023A" },	//	TPC_E_INVALID_DATA_FROM_RECOGNIZER
			{ com_errc::tpc_s_truncated, "TabletPC inking error code. String was truncated. Code: 0x00040252" },	//	TPC_S_TRUNCATED
			{ com_errc::tpc_s_interrupted, "TabletPC inking error code. Recognition or training was interrupted. Code: 0x00040253" },	//	TPC_S_INTERRUPTED
			{ com_errc::tpc_s_no_data_to_process, "TabletPC inking error code. No personalization update to the recognizer because no training data found. Code: 0x00040254" },	//	TPC_S_NO_DATA_TO_PROCESS
			{ com_errc::xact_e_alreadyothersinglephase, "Another single phase resource manager has already been enlisted in this transaction. Code: 0x8004D000" },	//	XACT_E_ALREADYOTHERSINGLEPHASE
			{ com_errc::xact_e_cantretain, "A retaining commit or abort is not supported. Code: 0x8004D001" },	//	XACT_E_CANTRETAIN
			{ com_errc::xact_e_commitfailed, "The transaction failed to commit for an unknown reason. The transaction was aborted. Code: 0x8004D002" },	//	XACT_E_COMMITFAILED
			{ com_errc::xact_e_commitprevented, "Cannot call commit on this transaction object because the calling application did not initiate the transaction. Code: 0x8004D003" },	//	XACT_E_COMMITPREVENTED
			{ com_errc::xact_e_heuristicabort, "Instead of committing, the resource heuristically aborted. Code: 0x8004D004" },	//	XACT_E_HEURISTICABORT
			{ com_errc::xact_e_heuristiccommit, "Instead of aborting, the resource heuristically committed. Code: 0x8004D005" },	//	XACT_E_HEURISTICCOMMIT
			{ com_errc::xact_e_heuristicdamage, "Some of the states of the resource were committed while others were aborted, likely because of heuristic decisions. Code: 0x8004D006" },	//	XACT_E_HEURISTICDAMAGE
			{ com_errc::xact_e_heuristicdanger, "Some of the states of the resource may have been committed while others may have been aborted, likely because of heuristic decisions. Code: 0x8004D007" },	//	XACT_E_HEURISTICDANGER
			{ com_errc::xact_e_isolationlevel, "The requested isolation level is not valid or supported. Code: 0x8004D008" },	//	XACT_E_ISOLATIONLEVEL
			{ com_errc::xact_e_noasync, "The transaction manager doesn't support an asynchronous operation for this method. Code: 0x8004D009" },	//	XACT_E_NOASYNC
			{ com_errc::xact_e_noenlist, "Unable to enlist in the transaction. Code: 0x8004D00A" },	//	XACT_E_NOENLIST
			{ com_errc::xact_e_noisoretain, "The requested semantics of retention of isolation across retaining commit and abort boundaries cannot be supported by this transaction implementation, or isoFlags was not equal to zero. Code: 0x8004D00B" },	//	XACT_E_NOISORETAIN
			{ com_errc::xact_e_noresource, "There is no resource presently associated with this enlistment. Code: 0x8004D00C" },	//	XACT_E_NORESOURCE
			{ com_errc::xact_e_notcurrent, "The transaction failed to commit due to the failure of optimistic concurrency control in at least one of the resource managers. Code: 0x8004D00D" },	//	XACT_E_NOTCURRENT
			{ com_errc::xact_e_notransaction, "The transaction has already been implicitly or explicitly committed or aborted. Code: 0x8004D00E" },	//	XACT_E_NOTRANSACTION
			{ com_errc::xact_e_notsupported, "An invalid combination of flags was specified. Code: 0x8004D00F" },	//	XACT_E_NOTSUPPORTED
			{ com_errc::xact_e_unknownrmgrid, "The resource manager id is not associated with this transaction or the transaction manager. Code: 0x8004D010" },	//	XACT_E_UNKNOWNRMGRID
			{ com_errc::xact_e_wrongstate, "This method was called in the wrong state. Code: 0x8004D011" },	//	XACT_E_WRONGSTATE
			{ com_errc::xact_e_wronguow, "The indicated unit of work does not match the unit of work expected by the resource manager. Code: 0x8004D012" },	//	XACT_E_WRONGUOW
			{ com_errc::xact_e_xtionexists, "An enlistment in a transaction already exists. Code: 0x8004D013" },	//	XACT_E_XTIONEXISTS
			{ com_errc::xact_e_noimportobject, "An import object for the transaction could not be found. Code: 0x8004D014" },	//	XACT_E_NOIMPORTOBJECT
			{ com_errc::xact_e_invalidcookie, "The transaction cookie is invalid. Code: 0x8004D015" },	//	XACT_E_INVALIDCOOKIE
			{ com_errc::xact_e_indoubt, "The transaction status is in doubt. A communication failure occurred, or a transaction manager or resource manager has failed. Code: 0x8004D016" },	//	XACT_E_INDOUBT
			{ com_errc::xact_e_notimeout, "A time-out was specified, but time-outs are not supported. Code: 0x8004D017" },	//	XACT_E_NOTIMEOUT
			{ com_errc::xact_e_alreadyinprogress, "The requested operation is already in progress for the transaction. Code: 0x8004D018" },	//	XACT_E_ALREADYINPROGRESS
			{ com_errc::xact_e_aborted, "The transaction has already been aborted. Code: 0x8004D019" },	//	XACT_E_ABORTED
			{ com_errc::xact_e_logfull, "The Transaction Manager returned a log full error. Code: 0x8004D01A" },	//	XACT_E_LOGFULL
			{ com_errc::xact_e_tmnotavailable, "The Transaction Manager is not available. Code: 0x8004D01B" },	//	XACT_E_TMNOTAVAILABLE
			{ com_errc::xact_e_connection_down, "A connection with the transaction manager was lost. Code: 0x8004D01C" },	//	XACT_E_CONNECTION_DOWN
			{ com_errc::xact_e_connection_denied, "A request to establish a connection with the transaction manager was denied. Code: 0x8004D01D" },	//	XACT_E_CONNECTION_DENIED
			{ com_errc::xact_e_reenlisttimeout, "Resource manager reenlistment to determine transaction status timed out. Code: 0x8004D01E" },	//	XACT_E_REENLISTTIMEOUT
			{ com_errc::xact_e_tip_connect_failed, "This transaction manager failed to establish a connection with another TIP transaction manager. Code: 0x8004D01F" },	//	XACT_E_TIP_CONNECT_FAILED
			{ com_errc::xact_e_tip_protocol_error, "This transaction manager encountered a protocol error with another TIP transaction manager. Code: 0x8004D020" },	//	XACT_E_TIP_PROTOCOL_ERROR
			{ com_errc::xact_e_tip_pull_failed, "This transaction manager could not propagate a transaction from another TIP transaction manager. Code: 0x8004D021" },	//	XACT_E_TIP_PULL_FAILED
			{ com_errc::xact_e_dest_tmnotavailable, "The Transaction Manager on the destination machine is not available. Code: 0x8004D022" },	//	XACT_E_DEST_TMNOTAVAILABLE
			{ com_errc::xact_e_tip_disabled, "The Transaction Manager has disabled its support for TIP. Code: 0x8004D023" },	//	XACT_E_TIP_DISABLED
			{ com_errc::xact_e_network_tx_disabled, "The transaction manager has disabled its support for remote/network transactions. Code: 0x8004D024" },	//	XACT_E_NETWORK_TX_DISABLED
			{ com_errc::xact_e_partner_network_tx_disabled, "The partner transaction manager has disabled its support for remote/network transactions. Code: 0x8004D025" },	//	XACT_E_PARTNER_NETWORK_TX_DISABLED
			{ com_errc::xact_e_xa_tx_disabled, "The transaction manager has disabled its support for XA transactions. Code: 0x8004D026" },	//	XACT_E_XA_TX_DISABLED
			{ com_errc::xact_e_unable_to_read_dtc_config, "MSDTC was unable to read its configuration information. Code: 0x8004D027" },	//	XACT_E_UNABLE_TO_READ_DTC_CONFIG
			{ com_errc::xact_e_unable_to_load_dtc_proxy, "MSDTC was unable to load the dtc proxy dll. Code: 0x8004D028" },	//	XACT_E_UNABLE_TO_LOAD_DTC_PROXY
			{ com_errc::xact_e_aborting, "The local transaction has aborted. Code: 0x8004D029" },	//	XACT_E_ABORTING
			{ com_errc::xact_e_push_comm_failure, "The MSDTC transaction manager was unable to push the transaction to the destination transaction manager due to communication problems. Possible causes are: a firewall is present and it doesn't have an exception for the MSDTC process, the two machines cannot find each other by their NetBIOS names, or the support for network transactions is not enabled for one of the two transaction managers. Code: 0x8004D02A" },	//	XACT_E_PUSH_COMM_FAILURE
			{ com_errc::xact_e_pull_comm_failure, "The MSDTC transaction manager was unable to pull the transaction from the source transaction manager due to communication problems. Possible causes are: a firewall is present and it doesn't have an exception for the MSDTC process, the two machines cannot find each other by their NetBIOS names, or the support for network transactions is not enabled for one of the two transaction managers. Code: 0x8004D02B" },	//	XACT_E_PULL_COMM_FAILURE
			{ com_errc::xact_e_lu_tx_disabled, "The MSDTC transaction manager has disabled its support for SNA LU 6.2 transactions. Code: 0x8004D02C" },	//	XACT_E_LU_TX_DISABLED
			{ com_errc::xact_e_clerknotfound, "XACT_E_CLERKNOTFOUND. Code: 0x8004D080" },	//	XACT_E_CLERKNOTFOUND
			{ com_errc::xact_e_clerkexists, "XACT_E_CLERKEXISTS. Code: 0x8004D081" },	//	XACT_E_CLERKEXISTS
			{ com_errc::xact_e_recoveryinprogress, "XACT_E_RECOVERYINPROGRESS. Code: 0x8004D082" },	//	XACT_E_RECOVERYINPROGRESS
			{ com_errc::xact_e_transactionclosed, "XACT_E_TRANSACTIONCLOSED. Code: 0x8004D083" },	//	XACT_E_TRANSACTIONCLOSED
			{ com_errc::xact_e_invalidlsn, "XACT_E_INVALIDLSN. Code: 0x8004D084" },	//	XACT_E_INVALIDLSN
			{ com_errc::xact_e_replayrequest, "XACT_E_REPLAYREQUEST. Code: 0x8004D085" },	//	XACT_E_REPLAYREQUEST
			{ com_errc::xact_s_async, "An asynchronous operation was specified. The operation has begun, but its outcome is not known yet. Code: 0x0004D000" },	//	XACT_S_ASYNC
			{ com_errc::xact_s_defect, "XACT_S_DEFECT. Code: 0x0004D001" },	//	XACT_S_DEFECT
			{ com_errc::xact_s_readonly, "The method call succeeded because the transaction was read-only. Code: 0x0004D002" },	//	XACT_S_READONLY
			{ com_errc::xact_s_somenoretain, "The transaction was successfully aborted. However, this is a coordinated transaction, and some number of enlisted resources were aborted outright because they could not support abort-retaining semantics. Code: 0x0004D003" },	//	XACT_S_SOMENORETAIN
			{ com_errc::xact_s_okinform, "No changes were made during this call, but the sink wants another chance to look if any other sinks make further changes. Code: 0x0004D004" },	//	XACT_S_OKINFORM
			{ com_errc::xact_s_madechangescontent, "The sink is content and wishes the transaction to proceed. Changes were made to one or more resources during this call. Code: 0x0004D005" },	//	XACT_S_MADECHANGESCONTENT
			{ com_errc::xact_s_madechangesinform, "The sink is for the moment and wishes the transaction to proceed, but if other changes are made following this return by other event sinks then this sink wants another chance to look. Code: 0x0004D006" },	//	XACT_S_MADECHANGESINFORM
			{ com_errc::xact_s_allnoretain, "The transaction was successfully aborted. However, the abort was non-retaining. Code: 0x0004D007" },	//	XACT_S_ALLNORETAIN
			{ com_errc::xact_s_aborting, "An abort operation was already in progress. Code: 0x0004D008" },	//	XACT_S_ABORTING
			{ com_errc::xact_s_singlephase, "The resource manager has performed a single-phase commit of the transaction. Code: 0x0004D009" },	//	XACT_S_SINGLEPHASE
			{ com_errc::xact_s_locally_ok, "The local transaction has not aborted. Code: 0x0004D00A" },	//	XACT_S_LOCALLY_OK
			{ com_errc::xact_s_lastresourcemanager, "The resource manager has requested to be the coordinator (last resource manager) for the transaction. Code: 0x0004D010" },	//	XACT_S_LASTRESOURCEMANAGER
			{ com_errc::context_e_aborted, "The root transaction wanted to commit, but transaction aborted. Code: 0x8004E002" },	//	CONTEXT_E_ABORTED
			{ com_errc::context_e_aborting, "You made a method call on a COM+ component that has a transaction that has already aborted or in the process of aborting. Code: 0x8004E003" },	//	CONTEXT_E_ABORTING
			{ com_errc::context_e_nocontext, "There is no MTS object context. Code: 0x8004E004" },	//	CONTEXT_E_NOCONTEXT
			{ com_errc::context_e_would_deadlock, "The component is configured to use synchronization and this method call would cause a deadlock to occur. Code: 0x8004E005" },	//	CONTEXT_E_WOULD_DEADLOCK
			{ com_errc::context_e_synch_timeout, "The component is configured to use synchronization and a thread has timed out waiting to enter the context. Code: 0x8004E006" },	//	CONTEXT_E_SYNCH_TIMEOUT
			{ com_errc::context_e_oldref, "You made a method call on a COM+ component that has a transaction that has already committed or aborted. Code: 0x8004E007" },	//	CONTEXT_E_OLDREF
			{ com_errc::context_e_rolenotfound, "The specified role was not configured for the application. Code: 0x8004E00C" },	//	CONTEXT_E_ROLENOTFOUND
			{ com_errc::context_e_tmnotavailable, "COM+ was unable to talk to the Microsoft Distributed Transaction Coordinator. Code: 0x8004E00F" },	//	CONTEXT_E_TMNOTAVAILABLE
			{ com_errc::co_e_activationfailed, "An unexpected error occurred during COM+ Activation. Code: 0x8004E021" },	//	CO_E_ACTIVATIONFAILED
			{ com_errc::co_e_activationfailed_eventlogged, "COM+ Activation failed. Check the event log for more information. Code: 0x8004E022" },	//	CO_E_ACTIVATIONFAILED_EVENTLOGGED
			{ com_errc::co_e_activationfailed_catalogerror, "COM+ Activation failed due to a catalog or configuration error. Code: 0x8004E023" },	//	CO_E_ACTIVATIONFAILED_CATALOGERROR
			{ com_errc::co_e_activationfailed_timeout, "COM+ activation failed because the activation could not be completed in the specified amount of time. Code: 0x8004E024" },	//	CO_E_ACTIVATIONFAILED_TIMEOUT
			{ com_errc::co_e_initializationfailed, "COM+ Activation failed because an initialization function failed. Check the event log for more information. Code: 0x8004E025" },	//	CO_E_INITIALIZATIONFAILED
			{ com_errc::context_e_nojit, "The requested operation requires that JIT be in the current context and it is not. Code: 0x8004E026" },	//	CONTEXT_E_NOJIT
			{ com_errc::context_e_notransaction, "The requested operation requires that the current context have a Transaction, and it does not. Code: 0x8004E027" },	//	CONTEXT_E_NOTRANSACTION
			{ com_errc::co_e_threadingmodel_changed, "The components threading model has changed after install into a COM+ Application. Please re-install component. Code: 0x8004E028" },	//	CO_E_THREADINGMODEL_CHANGED
			{ com_errc::co_e_noiisintrinsics, "IIS intrinsics not available. Start your work with IIS. Code: 0x8004E029" },	//	CO_E_NOIISINTRINSICS
			{ com_errc::co_e_nocookies, "An attempt to write a cookie failed. Code: 0x8004E02A" },	//	CO_E_NOCOOKIES
			{ com_errc::co_e_dberror, "An attempt to use a database generated a database specific error. Code: 0x8004E02B" },	//	CO_E_DBERROR
			{ com_errc::co_e_notpooled, "The COM+ component you created must use object pooling to work. Code: 0x8004E02C" },	//	CO_E_NOTPOOLED
			{ com_errc::co_e_notconstructed, "The COM+ component you created must use object construction to work correctly. Code: 0x8004E02D" },	//	CO_E_NOTCONSTRUCTED
			{ com_errc::co_e_nosynchronization, "The COM+ component requires synchronization, and it is not configured for it. Code: 0x8004E02E" },	//	CO_E_NOSYNCHRONIZATION
			{ com_errc::co_e_isolevelmismatch, "The TxIsolation Level property for the COM+ component being created is stronger than the TxIsolationLevel for the \"root\" component for the transaction. The creation failed. Code: 0x8004E02F" },	//	CO_E_ISOLEVELMISMATCH
			{ com_errc::co_e_call_out_of_tx_scope_not_allowed, "The component attempted to make a cross-context call between invocations of EnterTransactionScopeand ExitTransactionScope. This is not allowed. Cross-context calls cannot be made while inside of a transaction scope. Code: 0x8004E030" },	//	CO_E_CALL_OUT_OF_TX_SCOPE_NOT_ALLOWED
			{ com_errc::co_e_exit_transaction_scope_not_called, "The component made a call to EnterTransactionScope, but did not make a corresponding call to ExitTransactionScope before returning. Code: 0x8004E031" },	//	CO_E_EXIT_TRANSACTION_SCOPE_NOT_CALLED
			{ com_errc::ole_s_usereg, "Use the registry database to provide the requested information. Code: 0x00040000" },	//	OLE_S_USEREG
			{ com_errc::ole_s_static, "Success, but static. Code: 0x00040001" },	//	OLE_S_STATIC
			{ com_errc::ole_s_mac_clipformat, "Macintosh clipboard format. Code: 0x00040002" },	//	OLE_S_MAC_CLIPFORMAT
			{ com_errc::dragdrop_s_drop, "Successful drop took place. Code: 0x00040100" },	//	DRAGDROP_S_DROP
			{ com_errc::dragdrop_s_cancel, "Drag-drop operation canceled. Code: 0x00040101" },	//	DRAGDROP_S_CANCEL
			{ com_errc::dragdrop_s_usedefaultcursors, "Use the default cursor. Code: 0x00040102" },	//	DRAGDROP_S_USEDEFAULTCURSORS
			{ com_errc::data_s_sameformatetc, "Data has same FORMATETC. Code: 0x00040130" },	//	DATA_S_SAMEFORMATETC
			{ com_errc::view_s_already_frozen, "View is already frozen. Code: 0x00040140" },	//	VIEW_S_ALREADY_FROZEN
			{ com_errc::cache_s_formatetc_notsupported, "FORMATETC not supported. Code: 0x00040170" },	//	CACHE_S_FORMATETC_NOTSUPPORTED
			{ com_errc::cache_s_samecache, "Same cache. Code: 0x00040171" },	//	CACHE_S_SAMECACHE
			{ com_errc::cache_s_somecaches_notupdated, "Some cache(s) not updated. Code: 0x00040172" },	//	CACHE_S_SOMECACHES_NOTUPDATED
			{ com_errc::oleobj_s_invalidverb, "Invalid verb for OLE object. Code: 0x00040180" },	//	OLEOBJ_S_INVALIDVERB
			{ com_errc::oleobj_s_cannot_doverb_now, "Verb number is valid but verb cannot be done now. Code: 0x00040181" },	//	OLEOBJ_S_CANNOT_DOVERB_NOW
			{ com_errc::oleobj_s_invalidhwnd, "Invalid window handle passed. Code: 0x00040182" },	//	OLEOBJ_S_INVALIDHWND
			{ com_errc::inplace_s_truncated, "Message is too long; some of it had to be truncated before displaying. Code: 0x000401A0" },	//	INPLACE_S_TRUNCATED
			{ com_errc::convert10_s_no_presentation, "Unable to convert OLESTREAM to IStorage. Code: 0x000401C0" },	//	CONVERT10_S_NO_PRESENTATION
			{ com_errc::mk_s_reduced_to_self, "Moniker reduced to itself. Code: 0x000401E2" },	//	MK_S_REDUCED_TO_SELF
			{ com_errc::mk_s_me, "Common prefix is this moniker. Code: 0x000401E4" },	//	MK_S_ME
			{ com_errc::mk_s_him, "Common prefix is input moniker. Code: 0x000401E5" },	//	MK_S_HIM
			{ com_errc::mk_s_us, "Common prefix is both monikers. Code: 0x000401E6" },	//	MK_S_US
			{ com_errc::mk_s_monikeralreadyregistered, "Moniker is already registered in running object table. Code: 0x000401E7" },	//	MK_S_MONIKERALREADYREGISTERED
			{ com_errc::sched_s_task_ready, "The task is ready to run at its next scheduled time. Code: 0x00041300" },	//	SCHED_S_TASK_READY
			{ com_errc::sched_s_task_running, "The task is currently running. Code: 0x00041301" },	//	SCHED_S_TASK_RUNNING
			{ com_errc::sched_s_task_disabled, "The task will not run at the scheduled times because it has been disabled. Code: 0x00041302" },	//	SCHED_S_TASK_DISABLED
			{ com_errc::sched_s_task_has_not_run, "The task has not yet run. Code: 0x00041303" },	//	SCHED_S_TASK_HAS_NOT_RUN
			{ com_errc::sched_s_task_no_more_runs, "There are no more runs scheduled for this task. Code: 0x00041304" },	//	SCHED_S_TASK_NO_MORE_RUNS
			{ com_errc::sched_s_task_not_scheduled, "One or more of the properties that are needed to run this task on a schedule have not been set. Code: 0x00041305" },	//	SCHED_S_TASK_NOT_SCHEDULED
			{ com_errc::sched_s_task_terminated, "The last run of the task was terminated by the user. Code: 0x00041306" },	//	SCHED_S_TASK_TERMINATED
			{ com_errc::sched_s_task_no_valid_triggers, "Either the task has no triggers or the existing triggers are disabled or not set. Code: 0x00041307" },	//	SCHED_S_TASK_NO_VALID_TRIGGERS
			{ com_errc::sched_s_event_trigger, "Event triggers don't have set run times. Code: 0x00041308" },	//	SCHED_S_EVENT_TRIGGER
			{ com_errc::sched_e_trigger_not_found, "Trigger not found. Code: 0x80041309" },	//	SCHED_E_TRIGGER_NOT_FOUND
			{ com_errc::sched_e_task_not_ready, "One or more of the properties that are needed to run this task have not been set. Code: 0x8004130A" },	//	SCHED_E_TASK_NOT_READY
			{ com_errc::sched_e_task_not_running, "There is no running instance of the task. Code: 0x8004130B" },	//	SCHED_E_TASK_NOT_RUNNING
			{ com_errc::sched_e_service_not_installed, "The Task Scheduler Service is not installed on this computer. Code: 0x8004130C" },	//	SCHED_E_SERVICE_NOT_INSTALLED
			{ com_errc::sched_e_cannot_open_task, "The task object could not be opened. Code: 0x8004130D" },	//	SCHED_E_CANNOT_OPEN_TASK
			{ com_errc::sched_e_invalid_task, "The object is either an invalid task object or is not a task object. Code: 0x8004130E" },	//	SCHED_E_INVALID_TASK
			{ com_errc::sched_e_account_information_not_set, "No account information could be found in the Task Scheduler security database for the task indicated. Code: 0x8004130F" },	//	SCHED_E_ACCOUNT_INFORMATION_NOT_SET
			{ com_errc::sched_e_account_name_not_found, "Unable to establish existence of the account specified. Code: 0x80041310" },	//	SCHED_E_ACCOUNT_NAME_NOT_FOUND
			{ com_errc::sched_e_account_dbase_corrupt, "Corruption was detected in the Task Scheduler security database; the database has been reset. Code: 0x80041311" },	//	SCHED_E_ACCOUNT_DBASE_CORRUPT
			{ com_errc::sched_e_no_security_services, "Task Scheduler security services are available only on Windows NT. Code: 0x80041312" },	//	SCHED_E_NO_SECURITY_SERVICES
			{ com_errc::sched_e_unknown_object_version, "The task object version is either unsupported or invalid. Code: 0x80041313" },	//	SCHED_E_UNKNOWN_OBJECT_VERSION
			{ com_errc::sched_e_unsupported_account_option, "The task has been configured with an unsupported combination of account settings and run time options. Code: 0x80041314" },	//	SCHED_E_UNSUPPORTED_ACCOUNT_OPTION
			{ com_errc::sched_e_service_not_running, "The Task Scheduler Service is not running. Code: 0x80041315" },	//	SCHED_E_SERVICE_NOT_RUNNING
			{ com_errc::sched_e_unexpectednode, "The task XML contains an unexpected node. Code: 0x80041316" },	//	SCHED_E_UNEXPECTEDNODE
			{ com_errc::sched_e_namespace, "The task XML contains an element or attribute from an unexpected namespace. Code: 0x80041317" },	//	SCHED_E_NAMESPACE
			{ com_errc::sched_e_invalidvalue, "The task XML contains a value which is incorrectly formatted or out of range. Code: 0x80041318" },	//	SCHED_E_INVALIDVALUE
			{ com_errc::sched_e_missingnode, "The task XML is missing a required element or attribute. Code: 0x80041319" },	//	SCHED_E_MISSINGNODE
			{ com_errc::sched_e_malformedxml, "The task XML is malformed. Code: 0x8004131A" },	//	SCHED_E_MALFORMEDXML
			{ com_errc::sched_s_some_triggers_failed, "The task is registered, but not all specified triggers will start the task, check task scheduler event log for detailed information. Code: 0x0004131B" },	//	SCHED_S_SOME_TRIGGERS_FAILED
			{ com_errc::sched_s_batch_logon_problem, "The task is registered, but may fail to start. Batch logon privilege needs to be enabled for the task principal. Code: 0x0004131C" },	//	SCHED_S_BATCH_LOGON_PROBLEM
			{ com_errc::sched_e_too_many_nodes, "The task XML contains too many nodes of the same type. Code: 0x8004131D" },	//	SCHED_E_TOO_MANY_NODES
			{ com_errc::sched_e_past_end_boundary, "The task cannot be started after the trigger's end boundary. Code: 0x8004131E" },	//	SCHED_E_PAST_END_BOUNDARY
			{ com_errc::sched_e_already_running, "An instance of this task is already running. Code: 0x8004131F" },	//	SCHED_E_ALREADY_RUNNING
			{ com_errc::sched_e_user_not_logged_on, "The task will not run because the user is not logged on. Code: 0x80041320" },	//	SCHED_E_USER_NOT_LOGGED_ON
			{ com_errc::sched_e_invalid_task_hash, "The task image is corrupt or has been tampered with. Code: 0x80041321" },	//	SCHED_E_INVALID_TASK_HASH
			{ com_errc::sched_e_service_not_available, "The Task Scheduler service is not available. Code: 0x80041322" },	//	SCHED_E_SERVICE_NOT_AVAILABLE
			{ com_errc::sched_e_service_too_busy, "The Task Scheduler service is too busy to handle your request. Please try again later. Code: 0x80041323" },	//	SCHED_E_SERVICE_TOO_BUSY
			{ com_errc::sched_e_task_attempted, "The Task Scheduler service attempted to run the task, but the task did not run due to one of the constraints in the task definition. Code: 0x80041324" },	//	SCHED_E_TASK_ATTEMPTED
			{ com_errc::sched_s_task_queued, "The Task Scheduler service has asked the task to run. Code: 0x00041325" },	//	SCHED_S_TASK_QUEUED
			{ com_errc::sched_e_task_disabled, "The task is disabled. Code: 0x80041326" },	//	SCHED_E_TASK_DISABLED
			{ com_errc::sched_e_task_not_v1_compat, "The task has properties that are not compatible with previous versions of Windows. Code: 0x80041327" },	//	SCHED_E_TASK_NOT_V1_COMPAT
			{ com_errc::sched_e_start_on_demand, "The task settings do not allow the task to start on demand. Code: 0x80041328" },	//	SCHED_E_START_ON_DEMAND
			{ com_errc::sched_e_task_not_ubpm_compat, "The combination of properties that task is using is not compatible with the scheduling engine. Code: 0x80041329" },	//	SCHED_E_TASK_NOT_UBPM_COMPAT
			{ com_errc::sched_e_deprecated_feature_used, "The task definition uses a deprecated feature. Code: 0x80041330" },	//	SCHED_E_DEPRECATED_FEATURE_USED
			{ com_errc::co_e_class_create_failed, "Attempt to create a class object failed. Code: 0x80080001" },	//	CO_E_CLASS_CREATE_FAILED
			{ com_errc::co_e_scm_error, "OLE service could not bind object. Code: 0x80080002" },	//	CO_E_SCM_ERROR
			{ com_errc::co_e_scm_rpc_failure, "RPC communication failed with OLE service. Code: 0x80080003" },	//	CO_E_SCM_RPC_FAILURE
			{ com_errc::co_e_bad_path, "Bad path to object. Code: 0x80080004" },	//	CO_E_BAD_PATH
			{ com_errc::co_e_server_exec_failure, "Server execution failed. Code: 0x80080005" },	//	CO_E_SERVER_EXEC_FAILURE
			{ com_errc::co_e_objsrv_rpc_failure, "OLE service could not communicate with the object server. Code: 0x80080006" },	//	CO_E_OBJSRV_RPC_FAILURE
			{ com_errc::mk_e_no_normalized, "Moniker path could not be normalized. Code: 0x80080007" },	//	MK_E_NO_NORMALIZED
			{ com_errc::co_e_server_stopping, "Object server is stopping when OLE service contacts it. Code: 0x80080008" },	//	CO_E_SERVER_STOPPING
			{ com_errc::mem_e_invalid_root, "An invalid root block pointer was specified. Code: 0x80080009" },	//	MEM_E_INVALID_ROOT
			{ com_errc::mem_e_invalid_link, "An allocation chain contained an invalid link pointer. Code: 0x80080010" },	//	MEM_E_INVALID_LINK
			{ com_errc::mem_e_invalid_size, "The requested allocation size was too large. Code: 0x80080011" },	//	MEM_E_INVALID_SIZE
			{ com_errc::co_s_notallinterfaces, "Not all the requested interfaces were available. Code: 0x00080012" },	//	CO_S_NOTALLINTERFACES
			{ com_errc::co_s_machinenamenotfound, "The specified machine name was not found in the cache. Code: 0x00080013" },	//	CO_S_MACHINENAMENOTFOUND
			{ com_errc::co_e_missing_displayname, "The activation requires a display name to be present under the CLSID key. Code: 0x80080015" },	//	CO_E_MISSING_DISPLAYNAME
			{ com_errc::co_e_runas_value_must_be_aaa, "The activation requires that the RunAs value for the application is Activate As Activator. Code: 0x80080016" },	//	CO_E_RUNAS_VALUE_MUST_BE_AAA
			{ com_errc::co_e_elevation_disabled, "The class is not configured to support Elevated activation. Code: 0x80080017" },	//	CO_E_ELEVATION_DISABLED
			{ com_errc::appx_e_packaging_internal, "Appx packaging API has encountered an internal error. Code: 0x80080200" },	//	APPX_E_PACKAGING_INTERNAL
			{ com_errc::appx_e_interleaving_not_allowed, "The file is not a valid Appx package because its contents are interleaved. Code: 0x80080201" },	//	APPX_E_INTERLEAVING_NOT_ALLOWED
			{ com_errc::appx_e_relationships_not_allowed, "The file is not a valid Appx package because it contains OPC relationships. Code: 0x80080202" },	//	APPX_E_RELATIONSHIPS_NOT_ALLOWED
			{ com_errc::appx_e_missing_required_file, "The file is not a valid Appx package because it is missing a manifest or block map, or missing a signature file when the code integrity file is present. Code: 0x80080203" },	//	APPX_E_MISSING_REQUIRED_FILE
			{ com_errc::appx_e_invalid_manifest, "The Appx package's manifest is invalid. Code: 0x80080204" },	//	APPX_E_INVALID_MANIFEST
			{ com_errc::appx_e_invalid_blockmap, "The Appx package's block map is invalid. Code: 0x80080205" },	//	APPX_E_INVALID_BLOCKMAP
			{ com_errc::appx_e_corrupt_content, "The Appx package's content cannot be read because it is corrupt. Code: 0x80080206" },	//	APPX_E_CORRUPT_CONTENT
			{ com_errc::appx_e_block_hash_invalid, "The computed hash value of the block does not match the one stored in the block map. Code: 0x80080207" },	//	APPX_E_BLOCK_HASH_INVALID
			{ com_errc::appx_e_requested_range_too_large, "The requested byte range is over 4GB when translated to byte range of blocks. Code: 0x80080208" },	//	APPX_E_REQUESTED_RANGE_TOO_LARGE
			{ com_errc::appx_e_invalid_sip_client_data, "The SIP_SUBJECTINFO structure used to sign the package didn't contain the required data. Code: 0x80080209" },	//	APPX_E_INVALID_SIP_CLIENT_DATA
			{ com_errc::bt_e_spurious_activation, "The background task activation is spurious. Code: 0x80080300" },	//	BT_E_SPURIOUS_ACTIVATION
			{ com_errc::disp_e_unknowninterface, "Unknown interface. Code: 0x80020001" },	//	DISP_E_UNKNOWNINTERFACE
			{ com_errc::disp_e_membernotfound, "Member not found. Code: 0x80020003" },	//	DISP_E_MEMBERNOTFOUND
			{ com_errc::disp_e_paramnotfound, "Parameter not found. Code: 0x80020004" },	//	DISP_E_PARAMNOTFOUND
			{ com_errc::disp_e_typemismatch, "Type mismatch. Code: 0x80020005" },	//	DISP_E_TYPEMISMATCH
			{ com_errc::disp_e_unknownname, "Unknown name. Code: 0x80020006" },	//	DISP_E_UNKNOWNNAME
			{ com_errc::disp_e_nonamedargs, "No named arguments. Code: 0x80020007" },	//	DISP_E_NONAMEDARGS
			{ com_errc::disp_e_badvartype, "Bad variable type. Code: 0x80020008" },	//	DISP_E_BADVARTYPE
			{ com_errc::disp_e_exception, "Exception occurred. Code: 0x80020009" },	//	DISP_E_EXCEPTION
			{ com_errc::disp_e_overflow, "Out of present range. Code: 0x8002000A" },	//	DISP_E_OVERFLOW
			{ com_errc::disp_e_badindex, "Invalid index. Code: 0x8002000B" },	//	DISP_E_BADINDEX
			{ com_errc::disp_e_unknownlcid, "Unknown language. Code: 0x8002000C" },	//	DISP_E_UNKNOWNLCID
			{ com_errc::disp_e_arrayislocked, "Memory is locked. Code: 0x8002000D" },	//	DISP_E_ARRAYISLOCKED
			{ com_errc::disp_e_badparamcount, "Invalid number of parameters. Code: 0x8002000E" },	//	DISP_E_BADPARAMCOUNT
			{ com_errc::disp_e_paramnotoptional, "Parameter not optional. Code: 0x8002000F" },	//	DISP_E_PARAMNOTOPTIONAL
			{ com_errc::disp_e_badcallee, "Invalid callee. Code: 0x80020010" },	//	DISP_E_BADCALLEE
			{ com_errc::disp_e_notacollection, "Does not support a collection. Code: 0x80020011" },	//	DISP_E_NOTACOLLECTION
			{ com_errc::disp_e_divbyzero, "Division by zero. Code: 0x80020012" },	//	DISP_E_DIVBYZERO
			{ com_errc::disp_e_buffertoosmall, "Buffer too small. Code: 0x80020013" },	//	DISP_E_BUFFERTOOSMALL
			{ com_errc::type_e_buffertoosmall, "Buffer too small. Code: 0x80028016" },	//	TYPE_E_BUFFERTOOSMALL
			{ com_errc::type_e_fieldnotfound, "Field name not defined in the record. Code: 0x80028017" },	//	TYPE_E_FIELDNOTFOUND
			{ com_errc::type_e_invdataread, "Old format or invalid type library. Code: 0x80028018" },	//	TYPE_E_INVDATAREAD
			{ com_errc::type_e_unsupformat, "Old format or invalid type library. Code: 0x80028019" },	//	TYPE_E_UNSUPFORMAT
			{ com_errc::type_e_registryaccess, "Error accessing the OLE registry. Code: 0x8002801C" },	//	TYPE_E_REGISTRYACCESS
			{ com_errc::type_e_libnotregistered, "Library not registered. Code: 0x8002801D" },	//	TYPE_E_LIBNOTREGISTERED
			{ com_errc::type_e_undefinedtype, "Bound to unknown type. Code: 0x80028027" },	//	TYPE_E_UNDEFINEDTYPE
			{ com_errc::type_e_qualifiednamedisallowed, "Qualified name disallowed. Code: 0x80028028" },	//	TYPE_E_QUALIFIEDNAMEDISALLOWED
			{ com_errc::type_e_invalidstate, "Invalid forward reference, or reference to uncompiled type. Code: 0x80028029" },	//	TYPE_E_INVALIDSTATE
			{ com_errc::type_e_wrongtypekind, "Type mismatch. Code: 0x8002802A" },	//	TYPE_E_WRONGTYPEKIND
			{ com_errc::type_e_elementnotfound, "Element not found. Code: 0x8002802B" },	//	TYPE_E_ELEMENTNOTFOUND
			{ com_errc::type_e_ambiguousname, "Ambiguous name. Code: 0x8002802C" },	//	TYPE_E_AMBIGUOUSNAME
			{ com_errc::type_e_nameconflict, "Name already exists in the library. Code: 0x8002802D" },	//	TYPE_E_NAMECONFLICT
			{ com_errc::type_e_unknownlcid, "Unknown LCID. Code: 0x8002802E" },	//	TYPE_E_UNKNOWNLCID
			{ com_errc::type_e_dllfunctionnotfound, "Function not defined in specified DLL. Code: 0x8002802F" },	//	TYPE_E_DLLFUNCTIONNOTFOUND
			{ com_errc::type_e_badmodulekind, "Wrong module kind for the operation. Code: 0x800288BD" },	//	TYPE_E_BADMODULEKIND
			{ com_errc::type_e_sizetoobig, "Size may not exceed 64K. Code: 0x800288C5" },	//	TYPE_E_SIZETOOBIG
			{ com_errc::type_e_duplicateid, "Duplicate ID in inheritance hierarchy. Code: 0x800288C6" },	//	TYPE_E_DUPLICATEID
			{ com_errc::type_e_invalidid, "Incorrect inheritance depth in standard OLE hmember. Code: 0x800288CF" },	//	TYPE_E_INVALIDID
			{ com_errc::type_e_typemismatch, "Type mismatch. Code: 0x80028CA0" },	//	TYPE_E_TYPEMISMATCH
			{ com_errc::type_e_outofbounds, "Invalid number of arguments. Code: 0x80028CA1" },	//	TYPE_E_OUTOFBOUNDS
			{ com_errc::type_e_ioerror, "I/O Error. Code: 0x80028CA2" },	//	TYPE_E_IOERROR
			{ com_errc::type_e_cantcreatetmpfile, "Error creating unique tmp file. Code: 0x80028CA3" },	//	TYPE_E_CANTCREATETMPFILE
			{ com_errc::type_e_cantloadlibrary, "Error loading type library/DLL. Code: 0x80029C4A" },	//	TYPE_E_CANTLOADLIBRARY
			{ com_errc::type_e_inconsistentpropfuncs, "Inconsistent property functions. Code: 0x80029C83" },	//	TYPE_E_INCONSISTENTPROPFUNCS
			{ com_errc::type_e_circulartype, "Circular dependency between types/modules. Code: 0x80029C84" },	//	TYPE_E_CIRCULARTYPE
			{ com_errc::stg_e_invalidfunction, "Unable to perform requested operation. Code: 0x80030001" },	//	STG_E_INVALIDFUNCTION
			{ com_errc::stg_e_filenotfound, "%1 could not be found. Code: 0x80030002" },	//	STG_E_FILENOTFOUND
			{ com_errc::stg_e_pathnotfound, "The path %1 could not be found. Code: 0x80030003" },	//	STG_E_PATHNOTFOUND
			{ com_errc::stg_e_toomanyopenfiles, "There are insufficient resources to open another file. Code: 0x80030004" },	//	STG_E_TOOMANYOPENFILES
			{ com_errc::stg_e_accessdenied, "Access Denied. Code: 0x80030005" },	//	STG_E_ACCESSDENIED
			{ com_errc::stg_e_invalidhandle, "Attempted an operation on an invalid object. Code: 0x80030006" },	//	STG_E_INVALIDHANDLE
			{ com_errc::stg_e_insufficientmemory, "There is insufficient memory available to complete operation. Code: 0x80030008" },	//	STG_E_INSUFFICIENTMEMORY
			{ com_errc::stg_e_invalidpointer, "Invalid pointer error. Code: 0x80030009" },	//	STG_E_INVALIDPOINTER
			{ com_errc::stg_e_nomorefiles, "There are no more entries to return. Code: 0x80030012" },	//	STG_E_NOMOREFILES
			{ com_errc::stg_e_diskiswriteprotected, "Disk is write-protected. Code: 0x80030013" },	//	STG_E_DISKISWRITEPROTECTED
			{ com_errc::stg_e_seekerror, "An error occurred during a seek operation. Code: 0x80030019" },	//	STG_E_SEEKERROR
			{ com_errc::stg_e_writefault, "A disk error occurred during a write operation. Code: 0x8003001D" },	//	STG_E_WRITEFAULT
			{ com_errc::stg_e_readfault, "A disk error occurred during a read operation. Code: 0x8003001E" },	//	STG_E_READFAULT
			{ com_errc::stg_e_shareviolation, "A share violation has occurred. Code: 0x80030020" },	//	STG_E_SHAREVIOLATION
			{ com_errc::stg_e_lockviolation, "A lock violation has occurred. Code: 0x80030021" },	//	STG_E_LOCKVIOLATION
			{ com_errc::stg_e_filealreadyexists, "%1 already exists. Code: 0x80030050" },	//	STG_E_FILEALREADYEXISTS
			{ com_errc::stg_e_invalidparameter, "Invalid parameter error. Code: 0x80030057" },	//	STG_E_INVALIDPARAMETER
			{ com_errc::stg_e_mediumfull, "There is insufficient disk space to complete operation. Code: 0x80030070" },	//	STG_E_MEDIUMFULL
			{ com_errc::stg_e_propsetmismatched, "Illegal write of non-simple property to simple property set. Code: 0x800300F0" },	//	STG_E_PROPSETMISMATCHED
			{ com_errc::stg_e_abnormalapiexit, "An API call exited abnormally. Code: 0x800300FA" },	//	STG_E_ABNORMALAPIEXIT
			{ com_errc::stg_e_invalidheader, "The file %1 is not a valid compound file. Code: 0x800300FB" },	//	STG_E_INVALIDHEADER
			{ com_errc::stg_e_invalidname, "The name %1 is not valid. Code: 0x800300FC" },	//	STG_E_INVALIDNAME
			{ com_errc::stg_e_unknown, "An unexpected error occurred. Code: 0x800300FD" },	//	STG_E_UNKNOWN
			{ com_errc::stg_e_unimplementedfunction, "That function is not implemented. Code: 0x800300FE" },	//	STG_E_UNIMPLEMENTEDFUNCTION
			{ com_errc::stg_e_invalidflag, "Invalid flag error. Code: 0x800300FF" },	//	STG_E_INVALIDFLAG
			{ com_errc::stg_e_inuse, "Attempted to use an object that is busy. Code: 0x80030100" },	//	STG_E_INUSE
			{ com_errc::stg_e_notcurrent, "The storage has been changed since the last commit. Code: 0x80030101" },	//	STG_E_NOTCURRENT
			{ com_errc::stg_e_reverted, "Attempted to use an object that has ceased to exist. Code: 0x80030102" },	//	STG_E_REVERTED
			{ com_errc::stg_e_cantsave, "Can't save. Code: 0x80030103" },	//	STG_E_CANTSAVE
			{ com_errc::stg_e_oldformat, "The compound file %1 was produced with an incompatible version of storage. Code: 0x80030104" },	//	STG_E_OLDFORMAT
			{ com_errc::stg_e_olddll, "The compound file %1 was produced with a newer version of storage. Code: 0x80030105" },	//	STG_E_OLDDLL
			{ com_errc::stg_e_sharerequired, "Share.exe or equivalent is required for operation. Code: 0x80030106" },	//	STG_E_SHAREREQUIRED
			{ com_errc::stg_e_notfilebasedstorage, "Illegal operation called on non-file based storage. Code: 0x80030107" },	//	STG_E_NOTFILEBASEDSTORAGE
			{ com_errc::stg_e_extantmarshallings, "Illegal operation called on object with extant marshallings. Code: 0x80030108" },	//	STG_E_EXTANTMARSHALLINGS
			{ com_errc::stg_e_docfilecorrupt, "The docfile has been corrupted. Code: 0x80030109" },	//	STG_E_DOCFILECORRUPT
			{ com_errc::stg_e_badbaseaddress, "OLE32.DLL has been loaded at the wrong address. Code: 0x80030110" },	//	STG_E_BADBASEADDRESS
			{ com_errc::stg_e_docfiletoolarge, "The compound file is too large for the current implementation. Code: 0x80030111" },	//	STG_E_DOCFILETOOLARGE
			{ com_errc::stg_e_notsimpleformat, "The compound file was not created with the STGM_SIMPLE flag. Code: 0x80030112" },	//	STG_E_NOTSIMPLEFORMAT
			{ com_errc::stg_e_incomplete, "The file download was aborted abnormally. The file is incomplete. Code: 0x80030201" },	//	STG_E_INCOMPLETE
			{ com_errc::stg_e_terminated, "The file download has been terminated. Code: 0x80030202" },	//	STG_E_TERMINATED
			{ com_errc::stg_s_converted, "The underlying file was converted to compound file format. Code: 0x00030200" },	//	STG_S_CONVERTED
			{ com_errc::stg_s_block, "The storage operation should block until more data is available. Code: 0x00030201" },	//	STG_S_BLOCK
			{ com_errc::stg_s_retrynow, "The storage operation should retry immediately. Code: 0x00030202" },	//	STG_S_RETRYNOW
			{ com_errc::stg_s_monitoring, "The notified event sink will not influence the storage operation. Code: 0x00030203" },	//	STG_S_MONITORING
			{ com_errc::stg_s_multipleopens, "Multiple opens prevent consolidated. (commit succeeded). Code: 0x00030204" },	//	STG_S_MULTIPLEOPENS
			{ com_errc::stg_s_consolidationfailed, "Consolidation of the storage file failed. (commit succeeded). Code: 0x00030205" },	//	STG_S_CONSOLIDATIONFAILED
			{ com_errc::stg_s_cannotconsolidate, "Consolidation of the storage file is inappropriate. (commit succeeded). Code: 0x00030206" },	//	STG_S_CANNOTCONSOLIDATE
			{ com_errc::stg_e_status_copy_protection_failure, "Generic Copy Protection Error. Code: 0x80030305" },	//	STG_E_STATUS_COPY_PROTECTION_FAILURE
			{ com_errc::stg_e_css_authentication_failure, "Copy Protection Error - DVD CSS Authentication failed. Code: 0x80030306" },	//	STG_E_CSS_AUTHENTICATION_FAILURE
			{ com_errc::stg_e_css_key_not_present, "Copy Protection Error - The given sector does not have a valid CSS key. Code: 0x80030307" },	//	STG_E_CSS_KEY_NOT_PRESENT
			{ com_errc::stg_e_css_key_not_established, "Copy Protection Error - DVD session key not established. Code: 0x80030308" },	//	STG_E_CSS_KEY_NOT_ESTABLISHED
			{ com_errc::stg_e_css_scrambled_sector, "Copy Protection Error - The read failed because the sector is encrypted. Code: 0x80030309" },	//	STG_E_CSS_SCRAMBLED_SECTOR
			{ com_errc::stg_e_css_region_mismatch, "Copy Protection Error - The current DVD's region does not correspond to the region setting of the drive. Code: 0x8003030A" },	//	STG_E_CSS_REGION_MISMATCH
			{ com_errc::stg_e_resets_exhausted, "Copy Protection Error - The drive's region setting may be permanent or the number of user resets has been exhausted. Code: 0x8003030B" },	//	STG_E_RESETS_EXHAUSTED
			{ com_errc::rpc_e_call_rejected, "Call was rejected by callee. Code: 0x80010001" },	//	RPC_E_CALL_REJECTED
			{ com_errc::rpc_e_call_canceled, "Call was canceled by the message filter. Code: 0x80010002" },	//	RPC_E_CALL_CANCELED
			{ com_errc::rpc_e_cantpost_insendcall, "The caller is dispatching an intertask SendMessage call and cannot call out via PostMessage. Code: 0x80010003" },	//	RPC_E_CANTPOST_INSENDCALL
			{ com_errc::rpc_e_cantcallout_inasynccall, "The caller is dispatching an asynchronous call and cannot make an outgoing call on behalf of this call. Code: 0x80010004" },	//	RPC_E_CANTCALLOUT_INASYNCCALL
			{ com_errc::rpc_e_cantcallout_inexternalcall, "It is illegal to call out while inside message filter. Code: 0x80010005" },	//	RPC_E_CANTCALLOUT_INEXTERNALCALL
			{ com_errc::rpc_e_connection_terminated, "The connection terminated or is in a bogus state and cannot be used any more. Other connections are still valid. Code: 0x80010006" },	//	RPC_E_CONNECTION_TERMINATED
			{ com_errc::rpc_e_server_died, "The callee (server [not server application]) is not available and disappeared; all connections are invalid. The call may have executed. Code: 0x80010007" },	//	RPC_E_SERVER_DIED
			{ com_errc::rpc_e_client_died, "The caller (client) disappeared while the callee (server) was processing a call. Code: 0x80010008" },	//	RPC_E_CLIENT_DIED
			{ com_errc::rpc_e_invalid_datapacket, "The data packet with the marshalled parameter data is incorrect. Code: 0x80010009" },	//	RPC_E_INVALID_DATAPACKET
			{ com_errc::rpc_e_canttransmit_call, "The call was not transmitted properly; the message queue was full and was not emptied after yielding. Code: 0x8001000A" },	//	RPC_E_CANTTRANSMIT_CALL
			{ com_errc::rpc_e_client_cantmarshal_data, "The client (caller) cannot marshall the parameter data - low memory, etc. Code: 0x8001000B" },	//	RPC_E_CLIENT_CANTMARSHAL_DATA
			{ com_errc::rpc_e_client_cantunmarshal_data, "The client (caller) cannot unmarshall the return data - low memory, etc. Code: 0x8001000C" },	//	RPC_E_CLIENT_CANTUNMARSHAL_DATA
			{ com_errc::rpc_e_server_cantmarshal_data, "The server (callee) cannot marshall the return data - low memory, etc. Code: 0x8001000D" },	//	RPC_E_SERVER_CANTMARSHAL_DATA
			{ com_errc::rpc_e_server_cantunmarshal_data, "The server (callee) cannot unmarshall the parameter data - low memory, etc. Code: 0x8001000E" },	//	RPC_E_SERVER_CANTUNMARSHAL_DATA
			{ com_errc::rpc_e_invalid_data, "Received data is invalid; could be server or client data. Code: 0x8001000F" },	//	RPC_E_INVALID_DATA
			{ com_errc::rpc_e_invalid_parameter, "A particular parameter is invalid and cannot be (un)marshalled. Code: 0x80010010" },	//	RPC_E_INVALID_PARAMETER
			{ com_errc::rpc_e_cantcallout_again, "There is no second outgoing call on same channel in DDE conversation. Code: 0x80010011" },	//	RPC_E_CANTCALLOUT_AGAIN
			{ com_errc::rpc_e_server_died_dne, "The callee (server [not server application]) is not available and disappeared; all connections are invalid. The call did not execute. Code: 0x80010012" },	//	RPC_E_SERVER_DIED_DNE
			{ com_errc::rpc_e_sys_call_failed, "System call failed. Code: 0x80010100" },	//	RPC_E_SYS_CALL_FAILED
			{ com_errc::rpc_e_out_of_resources, "Could not allocate some required resource (memory, events, ...). Code: 0x80010101" },	//	RPC_E_OUT_OF_RESOURCES
			{ com_errc::rpc_e_attempted_multithread, "Attempted to make calls on more than one thread in single threaded mode. Code: 0x80010102" },	//	RPC_E_ATTEMPTED_MULTITHREAD
			{ com_errc::rpc_e_not_registered, "The requested interface is not registered on the server object. Code: 0x80010103" },	//	RPC_E_NOT_REGISTERED
			{ com_errc::rpc_e_fault, "RPC could not call the server or could not return the results of calling the server. Code: 0x80010104" },	//	RPC_E_FAULT
			{ com_errc::rpc_e_serverfault, "The server threw an exception. Code: 0x80010105" },	//	RPC_E_SERVERFAULT
			{ com_errc::rpc_e_changed_mode, "Cannot change thread mode after it is set. Code: 0x80010106" },	//	RPC_E_CHANGED_MODE
			{ com_errc::rpc_e_invalidmethod, "The method called does not exist on the server. Code: 0x80010107" },	//	RPC_E_INVALIDMETHOD
			{ com_errc::rpc_e_disconnected, "The object invoked has disconnected from its clients. Code: 0x80010108" },	//	RPC_E_DISCONNECTED
			{ com_errc::rpc_e_retry, "The object invoked chose not to process the call now. Try again later. Code: 0x80010109" },	//	RPC_E_RETRY
			{ com_errc::rpc_e_servercall_retrylater, "The message filter indicated that the application is busy. Code: 0x8001010A" },	//	RPC_E_SERVERCALL_RETRYLATER
			{ com_errc::rpc_e_servercall_rejected, "The message filter rejected the call. Code: 0x8001010B" },	//	RPC_E_SERVERCALL_REJECTED
			{ com_errc::rpc_e_invalid_calldata, "A call control interfaces was called with invalid data. Code: 0x8001010C" },	//	RPC_E_INVALID_CALLDATA
			{ com_errc::rpc_e_cantcallout_ininputsynccall, "An outgoing call cannot be made since the application is dispatching an input-synchronous call. Code: 0x8001010D" },	//	RPC_E_CANTCALLOUT_ININPUTSYNCCALL
			{ com_errc::rpc_e_wrong_thread, "The application called an interface that was marshalled for a different thread. Code: 0x8001010E" },	//	RPC_E_WRONG_THREAD
			{ com_errc::rpc_e_thread_not_init, "CoInitialize has not been called on the current thread. Code: 0x8001010F" },	//	RPC_E_THREAD_NOT_INIT
			{ com_errc::rpc_e_version_mismatch, "The version of OLE on the client and server machines does not match. Code: 0x80010110" },	//	RPC_E_VERSION_MISMATCH
			{ com_errc::rpc_e_invalid_header, "OLE received a packet with an invalid header. Code: 0x80010111" },	//	RPC_E_INVALID_HEADER
			{ com_errc::rpc_e_invalid_extension, "OLE received a packet with an invalid extension. Code: 0x80010112" },	//	RPC_E_INVALID_EXTENSION
			{ com_errc::rpc_e_invalid_ipid, "The requested object or interface does not exist. Code: 0x80010113" },	//	RPC_E_INVALID_IPID
			{ com_errc::rpc_e_invalid_object, "The requested object does not exist. Code: 0x80010114" },	//	RPC_E_INVALID_OBJECT
			{ com_errc::rpc_s_callpending, "OLE has sent a request and is waiting for a reply. Code: 0x80010115" },	//	RPC_S_CALLPENDING
			{ com_errc::rpc_s_waitontimer, "OLE is waiting before retrying a request. Code: 0x80010116" },	//	RPC_S_WAITONTIMER
			{ com_errc::rpc_e_call_complete, "Call context cannot be accessed after call completed. Code: 0x80010117" },	//	RPC_E_CALL_COMPLETE
			{ com_errc::rpc_e_unsecure_call, "Impersonate on unsecure calls is not supported. Code: 0x80010118" },	//	RPC_E_UNSECURE_CALL
			{ com_errc::rpc_e_too_late, "Security must be initialized before any interfaces are marshalled or unmarshalled. It cannot be changed once initialized. Code: 0x80010119" },	//	RPC_E_TOO_LATE
			{ com_errc::rpc_e_no_good_security_packages, "No security packages are installed on this machine or the user is not logged on or there are no compatible security packages between the client and server. Code: 0x8001011A" },	//	RPC_E_NO_GOOD_SECURITY_PACKAGES
			{ com_errc::rpc_e_access_denied, "Access is denied. Code: 0x8001011B" },	//	RPC_E_ACCESS_DENIED
			{ com_errc::rpc_e_remote_disabled, "Remote calls are not allowed for this process. Code: 0x8001011C" },	//	RPC_E_REMOTE_DISABLED
			{ com_errc::rpc_e_invalid_objref, "The marshaled interface data packet (OBJREF) has an invalid or unknown format. Code: 0x8001011D" },	//	RPC_E_INVALID_OBJREF
			{ com_errc::rpc_e_no_context, "No context is associated with this call. This happens for some custom marshalled calls and on the client side of the call. Code: 0x8001011E" },	//	RPC_E_NO_CONTEXT
			{ com_errc::rpc_e_timeout, "This operation returned because the timeout period expired. Code: 0x8001011F" },	//	RPC_E_TIMEOUT
			{ com_errc::rpc_e_no_sync, "There are no synchronize objects to wait on. Code: 0x80010120" },	//	RPC_E_NO_SYNC
			{ com_errc::rpc_e_fullsic_required, "Full subject issuer chain SSL principal name expected from the server. Code: 0x80010121" },	//	RPC_E_FULLSIC_REQUIRED
			{ com_errc::rpc_e_invalid_std_name, "Principal name is not a valid MSSTD name. Code: 0x80010122" },	//	RPC_E_INVALID_STD_NAME
			{ com_errc::co_e_failedtoimpersonate, "Unable to impersonate DCOM client. Code: 0x80010123" },	//	CO_E_FAILEDTOIMPERSONATE
			{ com_errc::co_e_failedtogetsecctx, "Unable to obtain server's security context. Code: 0x80010124" },	//	CO_E_FAILEDTOGETSECCTX
			{ com_errc::co_e_failedtoopenthreadtoken, "Unable to open the access token of the current thread. Code: 0x80010125" },	//	CO_E_FAILEDTOOPENTHREADTOKEN
			{ com_errc::co_e_failedtogettokeninfo, "Unable to obtain user info from an access token. Code: 0x80010126" },	//	CO_E_FAILEDTOGETTOKENINFO
			{ com_errc::co_e_trusteedoesntmatchclient, "The client who called IAccessControl::IsAccessPermitted was not the trustee provided to the method. Code: 0x80010127" },	//	CO_E_TRUSTEEDOESNTMATCHCLIENT
			{ com_errc::co_e_failedtoqueryclientblanket, "Unable to obtain the client's security blanket. Code: 0x80010128" },	//	CO_E_FAILEDTOQUERYCLIENTBLANKET
			{ com_errc::co_e_failedtosetdacl, "Unable to set a discretionary ACL into a security descriptor. Code: 0x80010129" },	//	CO_E_FAILEDTOSETDACL
			{ com_errc::co_e_accesscheckfailed, "The system function, AccessCheck, returned false. Code: 0x8001012A" },	//	CO_E_ACCESSCHECKFAILED
			{ com_errc::co_e_netaccessapifailed, "Either NetAccessDel or NetAccessAdd returned an error code. Code: 0x8001012B" },	//	CO_E_NETACCESSAPIFAILED
			{ com_errc::co_e_wrongtrusteenamesyntax, "One of the trustee strings provided by the user did not conform to the <Domain>\\<Name> syntax and it was not the \"*\" string. Code: 0x8001012C" },	//	CO_E_WRONGTRUSTEENAMESYNTAX
			{ com_errc::co_e_invalidsid, "One of the security identifiers provided by the user was invalid. Code: 0x8001012D" },	//	CO_E_INVALIDSID
			{ com_errc::co_e_conversionfailed, "Unable to convert a wide character trustee string to a multibyte trustee string. Code: 0x8001012E" },	//	CO_E_CONVERSIONFAILED
			{ com_errc::co_e_nomatchingsidfound, "Unable to find a security identifier that corresponds to a trustee string provided by the user. Code: 0x8001012F" },	//	CO_E_NOMATCHINGSIDFOUND
			{ com_errc::co_e_lookupaccsidfailed, "The system function, LookupAccountSID, failed. Code: 0x80010130" },	//	CO_E_LOOKUPACCSIDFAILED
			{ com_errc::co_e_nomatchingnamefound, "Unable to find a trustee name that corresponds to a security identifier provided by the user. Code: 0x80010131" },	//	CO_E_NOMATCHINGNAMEFOUND
			{ com_errc::co_e_lookupaccnamefailed, "The system function, LookupAccountName, failed. Code: 0x80010132" },	//	CO_E_LOOKUPACCNAMEFAILED
			{ com_errc::co_e_setserlhndlfailed, "Unable to set or reset a serialization handle. Code: 0x80010133" },	//	CO_E_SETSERLHNDLFAILED
			{ com_errc::co_e_failedtogetwindir, "Unable to obtain the Windows directory. Code: 0x80010134" },	//	CO_E_FAILEDTOGETWINDIR
			{ com_errc::co_e_pathtoolong, "Path too long. Code: 0x80010135" },	//	CO_E_PATHTOOLONG
			{ com_errc::co_e_failedtogenuuid, "Unable to generate a uuid. Code: 0x80010136" },	//	CO_E_FAILEDTOGENUUID
			{ com_errc::co_e_failedtocreatefile, "Unable to create file. Code: 0x80010137" },	//	CO_E_FAILEDTOCREATEFILE
			{ com_errc::co_e_failedtoclosehandle, "Unable to close a serialization handle or a file handle. Code: 0x80010138" },	//	CO_E_FAILEDTOCLOSEHANDLE
			{ com_errc::co_e_exceedsysacllimit, "The number of ACEs in an ACL exceeds the system limit. Code: 0x80010139" },	//	CO_E_EXCEEDSYSACLLIMIT
			{ com_errc::co_e_acesinwrongorder, "Not all the DENY_ACCESS ACEs are arranged in front of the GRANT_ACCESS ACEs in the stream. Code: 0x8001013A" },	//	CO_E_ACESINWRONGORDER
			{ com_errc::co_e_incompatiblestreamversion, "The version of ACL format in the stream is not supported by this implementation of IAccessControl. Code: 0x8001013B" },	//	CO_E_INCOMPATIBLESTREAMVERSION
			{ com_errc::co_e_failedtoopenprocesstoken, "Unable to open the access token of the server process. Code: 0x8001013C" },	//	CO_E_FAILEDTOOPENPROCESSTOKEN
			{ com_errc::co_e_decodefailed, "Unable to decode the ACL in the stream provided by the user. Code: 0x8001013D" },	//	CO_E_DECODEFAILED
			{ com_errc::co_e_acnotinitialized, "The COM IAccessControl object is not initialized. Code: 0x8001013F" },	//	CO_E_ACNOTINITIALIZED
			{ com_errc::co_e_cancel_disabled, "Call Cancellation is disabled. Code: 0x80010140" },	//	CO_E_CANCEL_DISABLED
			{ com_errc::rpc_e_unexpected, "An internal error occurred. Code: 0x8001FFFF" },	//	RPC_E_UNEXPECTED
			{ com_errc::error_auditing_disabled, "The specified event is currently not being audited. Code: 0xC0090001" },	//	ERROR_AUDITING_DISABLED
			{ com_errc::error_all_sids_filtered, "The SID filtering operation removed all SIDs. Code: 0xC0090002" },	//	ERROR_ALL_SIDS_FILTERED
			{ com_errc::error_bizrules_not_enabled, "Business rule scripts are disabled for the calling application. Code: 0xC0090003" },	//	ERROR_BIZRULES_NOT_ENABLED
			{ com_errc::nte_bad_uid, "Bad UID. Code: 0x80090001" },	//	NTE_BAD_UID
			{ com_errc::nte_bad_hash, "Bad Hash. Code: 0x80090002" },	//	NTE_BAD_HASH
			{ com_errc::nte_bad_key, "Bad Key. Code: 0x80090003" },	//	NTE_BAD_KEY
			{ com_errc::nte_bad_len, "Bad Length. Code: 0x80090004" },	//	NTE_BAD_LEN
			{ com_errc::nte_bad_data, "Bad Data. Code: 0x80090005" },	//	NTE_BAD_DATA
			{ com_errc::nte_bad_signature, "Invalid Signature. Code: 0x80090006" },	//	NTE_BAD_SIGNATURE
			{ com_errc::nte_bad_ver, "Bad Version of provider. Code: 0x80090007" },	//	NTE_BAD_VER
			{ com_errc::nte_bad_algid, "Invalid algorithm specified. Code: 0x80090008" },	//	NTE_BAD_ALGID
			{ com_errc::nte_bad_flags, "Invalid flags specified. Code: 0x80090009" },	//	NTE_BAD_FLAGS
			{ com_errc::nte_bad_type, "Invalid type specified. Code: 0x8009000A" },	//	NTE_BAD_TYPE
			{ com_errc::nte_bad_key_state, "Key not valid for use in specified state. Code: 0x8009000B" },	//	NTE_BAD_KEY_STATE
			{ com_errc::nte_bad_hash_state, "Hash not valid for use in specified state. Code: 0x8009000C" },	//	NTE_BAD_HASH_STATE
			{ com_errc::nte_no_key, "Key does not exist. Code: 0x8009000D" },	//	NTE_NO_KEY
			{ com_errc::nte_no_memory, "Insufficient memory available for the operation. Code: 0x8009000E" },	//	NTE_NO_MEMORY
			{ com_errc::nte_exists, "Object already exists. Code: 0x8009000F" },	//	NTE_EXISTS
			{ com_errc::nte_perm, "Access denied. Code: 0x80090010" },	//	NTE_PERM
			{ com_errc::nte_not_found, "Object was not found. Code: 0x80090011" },	//	NTE_NOT_FOUND
			{ com_errc::nte_double_encrypt, "Data already encrypted. Code: 0x80090012" },	//	NTE_DOUBLE_ENCRYPT
			{ com_errc::nte_bad_provider, "Invalid provider specified. Code: 0x80090013" },	//	NTE_BAD_PROVIDER
			{ com_errc::nte_bad_prov_type, "Invalid provider type specified. Code: 0x80090014" },	//	NTE_BAD_PROV_TYPE
			{ com_errc::nte_bad_public_key, "Provider's public key is invalid. Code: 0x80090015" },	//	NTE_BAD_PUBLIC_KEY
			{ com_errc::nte_bad_keyset, "Keyset does not exist. Code: 0x80090016" },	//	NTE_BAD_KEYSET
			{ com_errc::nte_prov_type_not_def, "Provider type not defined. Code: 0x80090017" },	//	NTE_PROV_TYPE_NOT_DEF
			{ com_errc::nte_prov_type_entry_bad, "Provider type as registered is invalid. Code: 0x80090018" },	//	NTE_PROV_TYPE_ENTRY_BAD
			{ com_errc::nte_keyset_not_def, "The keyset is not defined. Code: 0x80090019" },	//	NTE_KEYSET_NOT_DEF
			{ com_errc::nte_keyset_entry_bad, "Keyset as registered is invalid. Code: 0x8009001A" },	//	NTE_KEYSET_ENTRY_BAD
			{ com_errc::nte_prov_type_no_match, "Provider type does not match registered value. Code: 0x8009001B" },	//	NTE_PROV_TYPE_NO_MATCH
			{ com_errc::nte_signature_file_bad, "The digital signature file is corrupt. Code: 0x8009001C" },	//	NTE_SIGNATURE_FILE_BAD
			{ com_errc::nte_provider_dll_fail, "Provider DLL failed to initialize correctly. Code: 0x8009001D" },	//	NTE_PROVIDER_DLL_FAIL
			{ com_errc::nte_prov_dll_not_found, "Provider DLL could not be found. Code: 0x8009001E" },	//	NTE_PROV_DLL_NOT_FOUND
			{ com_errc::nte_bad_keyset_param, "The Keyset parameter is invalid. Code: 0x8009001F" },	//	NTE_BAD_KEYSET_PARAM
			{ com_errc::nte_fail, "An internal error occurred. Code: 0x80090020" },	//	NTE_FAIL
			{ com_errc::nte_sys_err, "A base error occurred. Code: 0x80090021" },	//	NTE_SYS_ERR
			{ com_errc::nte_silent_context, "Provider could not perform the action since the context was acquired as silent. Code: 0x80090022" },	//	NTE_SILENT_CONTEXT
			{ com_errc::nte_token_keyset_storage_full, "The security token does not have storage space available for an additional container. Code: 0x80090023" },	//	NTE_TOKEN_KEYSET_STORAGE_FULL
			{ com_errc::nte_temporary_profile, "The profile for the user is a temporary profile. Code: 0x80090024" },	//	NTE_TEMPORARY_PROFILE
			{ com_errc::nte_fixedparameter, "The key parameters could not be set because the CSP uses fixed parameters. Code: 0x80090025" },	//	NTE_FIXEDPARAMETER
			{ com_errc::nte_invalid_handle, "The supplied handle is invalid. Code: 0x80090026" },	//	NTE_INVALID_HANDLE
			{ com_errc::nte_invalid_parameter, "The parameter is incorrect. Code: 0x80090027" },	//	NTE_INVALID_PARAMETER
			{ com_errc::nte_buffer_too_small, "The buffer supplied to a function was too small. Code: 0x80090028" },	//	NTE_BUFFER_TOO_SMALL
			{ com_errc::nte_not_supported, "The requested operation is not supported. Code: 0x80090029" },	//	NTE_NOT_SUPPORTED
			{ com_errc::nte_no_more_items, "No more data is available. Code: 0x8009002A" },	//	NTE_NO_MORE_ITEMS
			{ com_errc::nte_buffers_overlap, "The supplied buffers overlap incorrectly. Code: 0x8009002B" },	//	NTE_BUFFERS_OVERLAP
			{ com_errc::nte_decryption_failure, "The specified data could not be decrypted. Code: 0x8009002C" },	//	NTE_DECRYPTION_FAILURE
			{ com_errc::nte_internal_error, "An internal consistency check failed. Code: 0x8009002D" },	//	NTE_INTERNAL_ERROR
			{ com_errc::nte_ui_required, "This operation requires input from the user. Code: 0x8009002E" },	//	NTE_UI_REQUIRED
			{ com_errc::nte_hmac_not_supported, "The cryptographic provider does not support HMAC. Code: 0x8009002F" },	//	NTE_HMAC_NOT_SUPPORTED
			{ com_errc::nte_device_not_ready, "The device that is required by this cryptographic provider is not ready for use. Code: 0x80090030" },	//	NTE_DEVICE_NOT_READY
			{ com_errc::nte_authentication_ignored, "The dictionary attack mitigation is triggered and the provided authorization was ignored by the provider. Code: 0x80090031" },	//	NTE_AUTHENTICATION_IGNORED
			{ com_errc::nte_validation_failed, "The validation of the provided data failed the integrity or signature validation. Code: 0x80090032" },	//	NTE_VALIDATION_FAILED
			{ com_errc::nte_incorrect_password, "Incorrect password. Code: 0x80090033" },	//	NTE_INCORRECT_PASSWORD
			{ com_errc::nte_encryption_failure, "Encryption failed. Code: 0x80090034" },	//	NTE_ENCRYPTION_FAILURE
			{ com_errc::nte_device_not_found, "The device that is required by this cryptographic provider is not found on this platform. Code: 0x80090035" },	//	NTE_DEVICE_NOT_FOUND
			{ com_errc::sec_e_insufficient_memory, "Not enough memory is available to complete this request. Code: 0x80090300" },	//	SEC_E_INSUFFICIENT_MEMORY
			{ com_errc::sec_e_invalid_handle, "The handle specified is invalid. Code: 0x80090301" },	//	SEC_E_INVALID_HANDLE
			{ com_errc::sec_e_unsupported_function, "The function requested is not supported. Code: 0x80090302" },	//	SEC_E_UNSUPPORTED_FUNCTION
			{ com_errc::sec_e_target_unknown, "The specified target is unknown or unreachable. Code: 0x80090303" },	//	SEC_E_TARGET_UNKNOWN
			{ com_errc::sec_e_internal_error, "The Local Security Authority cannot be contacted. Code: 0x80090304" },	//	SEC_E_INTERNAL_ERROR
			{ com_errc::sec_e_secpkg_not_found, "The requested security package does not exist. Code: 0x80090305" },	//	SEC_E_SECPKG_NOT_FOUND
			{ com_errc::sec_e_not_owner, "The caller is not the owner of the desired credentials. Code: 0x80090306" },	//	SEC_E_NOT_OWNER
			{ com_errc::sec_e_cannot_install, "The security package failed to initialize, and cannot be installed. Code: 0x80090307" },	//	SEC_E_CANNOT_INSTALL
			{ com_errc::sec_e_invalid_token, "The token supplied to the function is invalid. Code: 0x80090308" },	//	SEC_E_INVALID_TOKEN
			{ com_errc::sec_e_cannot_pack, "The security package is not able to marshall the logon buffer, so the logon attempt has failed. Code: 0x80090309" },	//	SEC_E_CANNOT_PACK
			{ com_errc::sec_e_qop_not_supported, "The per-message Quality of Protection is not supported by the security package. Code: 0x8009030A" },	//	SEC_E_QOP_NOT_SUPPORTED
			{ com_errc::sec_e_no_impersonation, "The security context does not allow impersonation of the client. Code: 0x8009030B" },	//	SEC_E_NO_IMPERSONATION
			{ com_errc::sec_e_logon_denied, "The logon attempt failed. Code: 0x8009030C" },	//	SEC_E_LOGON_DENIED
			{ com_errc::sec_e_unknown_credentials, "The credentials supplied to the package were not recognized. Code: 0x8009030D" },	//	SEC_E_UNKNOWN_CREDENTIALS
			{ com_errc::sec_e_no_credentials, "No credentials are available in the security package. Code: 0x8009030E" },	//	SEC_E_NO_CREDENTIALS
			{ com_errc::sec_e_message_altered, "The message or signature supplied for verification has been altered. Code: 0x8009030F" },	//	SEC_E_MESSAGE_ALTERED
			{ com_errc::sec_e_out_of_sequence, "The message supplied for verification is out of sequence. Code: 0x80090310" },	//	SEC_E_OUT_OF_SEQUENCE
			{ com_errc::sec_e_no_authenticating_authority, "No authority could be contacted for authentication. Code: 0x80090311" },	//	SEC_E_NO_AUTHENTICATING_AUTHORITY
			{ com_errc::sec_i_continue_needed, "The function completed successfully, but must be called again to complete the context. Code: 0x00090312" },	//	SEC_I_CONTINUE_NEEDED
			{ com_errc::sec_i_complete_needed, "The function completed successfully, but CompleteToken must be called. Code: 0x00090313" },	//	SEC_I_COMPLETE_NEEDED
			{ com_errc::sec_i_complete_and_continue, "The function completed successfully, but both CompleteToken and this function must be called to complete the context. Code: 0x00090314" },	//	SEC_I_COMPLETE_AND_CONTINUE
			{ com_errc::sec_i_local_logon, "The logon was completed, but no network authority was available. The logon was made using locally known information. Code: 0x00090315" },	//	SEC_I_LOCAL_LOGON
			{ com_errc::sec_e_bad_pkgid, "The requested security package does not exist. Code: 0x80090316" },	//	SEC_E_BAD_PKGID
			{ com_errc::sec_e_context_expired, "The context has expired and can no longer be used. Code: 0x80090317" },	//	SEC_E_CONTEXT_EXPIRED
			{ com_errc::sec_i_context_expired, "The context has expired and can no longer be used. Code: 0x00090317" },	//	SEC_I_CONTEXT_EXPIRED
			{ com_errc::sec_e_incomplete_message, "The supplied message is incomplete. The signature was not verified. Code: 0x80090318" },	//	SEC_E_INCOMPLETE_MESSAGE
			{ com_errc::sec_e_incomplete_credentials, "The credentials supplied were not complete, and could not be verified. The context could not be initialized. Code: 0x80090320" },	//	SEC_E_INCOMPLETE_CREDENTIALS
			{ com_errc::sec_e_buffer_too_small, "The buffers supplied to a function was too small. Code: 0x80090321" },	//	SEC_E_BUFFER_TOO_SMALL
			{ com_errc::sec_i_incomplete_credentials, "The credentials supplied were not complete, and could not be verified. Additional information can be returned from the context. Code: 0x00090320" },	//	SEC_I_INCOMPLETE_CREDENTIALS
			{ com_errc::sec_i_renegotiate, "The context data must be renegotiated with the peer. Code: 0x00090321" },	//	SEC_I_RENEGOTIATE
			{ com_errc::sec_e_wrong_principal, "The target principal name is incorrect. Code: 0x80090322" },	//	SEC_E_WRONG_PRINCIPAL
			{ com_errc::sec_i_no_lsa_context, "There is no LSA mode context associated with this context. Code: 0x00090323" },	//	SEC_I_NO_LSA_CONTEXT
			{ com_errc::sec_e_time_skew, "The clocks on the client and server machines are skewed. Code: 0x80090324" },	//	SEC_E_TIME_SKEW
			{ com_errc::sec_e_untrusted_root, "The certificate chain was issued by an authority that is not trusted. Code: 0x80090325" },	//	SEC_E_UNTRUSTED_ROOT
			{ com_errc::sec_e_illegal_message, "The message received was unexpected or badly formatted. Code: 0x80090326" },	//	SEC_E_ILLEGAL_MESSAGE
			{ com_errc::sec_e_cert_unknown, "An unknown error occurred while processing the certificate. Code: 0x80090327" },	//	SEC_E_CERT_UNKNOWN
			{ com_errc::sec_e_cert_expired, "The received certificate has expired. Code: 0x80090328" },	//	SEC_E_CERT_EXPIRED
			{ com_errc::sec_e_encrypt_failure, "The specified data could not be encrypted. Code: 0x80090329" },	//	SEC_E_ENCRYPT_FAILURE
			{ com_errc::sec_e_decrypt_failure, "The specified data could not be decrypted. . Code: 0x80090330" },	//	SEC_E_DECRYPT_FAILURE
			{ com_errc::sec_e_algorithm_mismatch, "The client and server cannot communicate, because they do not possess a common algorithm. Code: 0x80090331" },	//	SEC_E_ALGORITHM_MISMATCH
			{ com_errc::sec_e_security_qos_failed, "The security context could not be established due to a failure in the requested quality of service (e.g. mutual authentication or delegation). Code: 0x80090332" },	//	SEC_E_SECURITY_QOS_FAILED
			{ com_errc::sec_e_unfinished_context_deleted, "A security context was deleted before the context was completed. This is considered a logon failure. Code: 0x80090333" },	//	SEC_E_UNFINISHED_CONTEXT_DELETED
			{ com_errc::sec_e_no_tgt_reply, "The client is trying to negotiate a context and the server requires user-to-user but didn't send a TGT reply. Code: 0x80090334" },	//	SEC_E_NO_TGT_REPLY
			{ com_errc::sec_e_no_ip_addresses, "Unable to accomplish the requested task because the local machine does not have any IP addresses. Code: 0x80090335" },	//	SEC_E_NO_IP_ADDRESSES
			{ com_errc::sec_e_wrong_credential_handle, "The supplied credential handle does not match the credential associated with the security context. Code: 0x80090336" },	//	SEC_E_WRONG_CREDENTIAL_HANDLE
			{ com_errc::sec_e_crypto_system_invalid, "The crypto system or checksum function is invalid because a required function is unavailable. Code: 0x80090337" },	//	SEC_E_CRYPTO_SYSTEM_INVALID
			{ com_errc::sec_e_max_referrals_exceeded, "The number of maximum ticket referrals has been exceeded. Code: 0x80090338" },	//	SEC_E_MAX_REFERRALS_EXCEEDED
			{ com_errc::sec_e_must_be_kdc, "The local machine must be a Kerberos KDC (domain controller) and it is not. Code: 0x80090339" },	//	SEC_E_MUST_BE_KDC
			{ com_errc::sec_e_strong_crypto_not_supported, "The other end of the security negotiation is requires strong crypto but it is not supported on the local machine. Code: 0x8009033A" },	//	SEC_E_STRONG_CRYPTO_NOT_SUPPORTED
			{ com_errc::sec_e_too_many_principals, "The KDC reply contained more than one principal name. Code: 0x8009033B" },	//	SEC_E_TOO_MANY_PRINCIPALS
			{ com_errc::sec_e_no_pa_data, "Expected to find PA data for a hint of what etype to use, but it was not found. Code: 0x8009033C" },	//	SEC_E_NO_PA_DATA
			{ com_errc::sec_e_pkinit_name_mismatch, "The client certificate does not contain a valid UPN, or does not match the client name in the logon request. Please contact your administrator. Code: 0x8009033D" },	//	SEC_E_PKINIT_NAME_MISMATCH
			{ com_errc::sec_e_smartcard_logon_required, "Smartcard logon is required and was not used. Code: 0x8009033E" },	//	SEC_E_SMARTCARD_LOGON_REQUIRED
			{ com_errc::sec_e_shutdown_in_progress, "A system shutdown is in progress. Code: 0x8009033F" },	//	SEC_E_SHUTDOWN_IN_PROGRESS
			{ com_errc::sec_e_kdc_invalid_request, "An invalid request was sent to the KDC. Code: 0x80090340" },	//	SEC_E_KDC_INVALID_REQUEST
			{ com_errc::sec_e_kdc_unable_to_refer, "The KDC was unable to generate a referral for the service requested. Code: 0x80090341" },	//	SEC_E_KDC_UNABLE_TO_REFER
			{ com_errc::sec_e_kdc_unknown_etype, "The encryption type requested is not supported by the KDC. Code: 0x80090342" },	//	SEC_E_KDC_UNKNOWN_ETYPE
			{ com_errc::sec_e_unsupported_preauth, "An unsupported preauthentication mechanism was presented to the Kerberos package. Code: 0x80090343" },	//	SEC_E_UNSUPPORTED_PREAUTH
			{ com_errc::sec_e_delegation_required, "The requested operation cannot be completed. The computer must be trusted for delegation and the current user account must be configured to allow delegation. Code: 0x80090345" },	//	SEC_E_DELEGATION_REQUIRED
			{ com_errc::sec_e_bad_bindings, "Client's supplied SSPI channel bindings were incorrect. Code: 0x80090346" },	//	SEC_E_BAD_BINDINGS
			{ com_errc::sec_e_multiple_accounts, "The received certificate was mapped to multiple accounts. Code: 0x80090347" },	//	SEC_E_MULTIPLE_ACCOUNTS
			{ com_errc::sec_e_no_kerb_key, "SEC_E_NO_KERB_KEY. Code: 0x80090348" },	//	SEC_E_NO_KERB_KEY
			{ com_errc::sec_e_cert_wrong_usage, "The certificate is not valid for the requested usage. Code: 0x80090349" },	//	SEC_E_CERT_WRONG_USAGE
			{ com_errc::sec_e_downgrade_detected, "The system cannot contact a domain controller to service the authentication request. Please try again later. Code: 0x80090350" },	//	SEC_E_DOWNGRADE_DETECTED
			{ com_errc::sec_e_smartcard_cert_revoked, "The smartcard certificate used for authentication has been revoked. Please contact your system administrator. There may be additional information in the event log. Code: 0x80090351" },	//	SEC_E_SMARTCARD_CERT_REVOKED
			{ com_errc::sec_e_issuing_ca_untrusted, "An untrusted certificate authority was detected while processing the smartcard certificate used for authentication. Please contact your system administrator. Code: 0x80090352" },	//	SEC_E_ISSUING_CA_UNTRUSTED
			{ com_errc::sec_e_revocation_offline_c, "The revocation status of the smartcard certificate used for authentication could not be determined. Please contact your system administrator. Code: 0x80090353" },	//	SEC_E_REVOCATION_OFFLINE_C
			{ com_errc::sec_e_pkinit_client_failure, "The smartcard certificate used for authentication was not trusted. Please contact your system administrator. Code: 0x80090354" },	//	SEC_E_PKINIT_CLIENT_FAILURE
			{ com_errc::sec_e_smartcard_cert_expired, "The smartcard certificate used for authentication has expired. Please contact your system administrator. Code: 0x80090355" },	//	SEC_E_SMARTCARD_CERT_EXPIRED
			{ com_errc::sec_e_no_s4u_prot_support, "The Kerberos subsystem encountered an error. A service for user protocol request was made against a domain controller which does not support service for user. Code: 0x80090356" },	//	SEC_E_NO_S4U_PROT_SUPPORT
			{ com_errc::sec_e_crossrealm_delegation_failure, "An attempt was made by this server to make a Kerberos constrained delegation request for a target outside of the server's realm. This is not supported, and indicates a misconfiguration on this server's allowed to delegate to list. Please contact your administrator. Code: 0x80090357" },	//	SEC_E_CROSSREALM_DELEGATION_FAILURE
			{ com_errc::sec_e_revocation_offline_kdc, "The revocation status of the domain controller certificate used for smartcard authentication could not be determined. There is additional information in the system event log. Please contact your system administrator. Code: 0x80090358" },	//	SEC_E_REVOCATION_OFFLINE_KDC
			{ com_errc::sec_e_issuing_ca_untrusted_kdc, "An untrusted certificate authority was detected while processing the domain controller certificate used for authentication. There is additional information in the system event log. Please contact your system administrator. Code: 0x80090359" },	//	SEC_E_ISSUING_CA_UNTRUSTED_KDC
			{ com_errc::sec_e_kdc_cert_expired, "The domain controller certificate used for smartcard logon has expired. Please contact your system administrator with the contents of your system event log. Code: 0x8009035A" },	//	SEC_E_KDC_CERT_EXPIRED
			{ com_errc::sec_e_kdc_cert_revoked, "The domain controller certificate used for smartcard logon has been revoked. Please contact your system administrator with the contents of your system event log. Code: 0x8009035B" },	//	SEC_E_KDC_CERT_REVOKED
			{ com_errc::sec_i_signature_needed, "A signature operation must be performed before the user can authenticate. Code: 0x0009035C" },	//	SEC_I_SIGNATURE_NEEDED
			{ com_errc::sec_e_invalid_parameter, "One or more of the parameters passed to the function was invalid. Code: 0x8009035D" },	//	SEC_E_INVALID_PARAMETER
			{ com_errc::sec_e_delegation_policy, "Client policy does not allow credential delegation to target server. Code: 0x8009035E" },	//	SEC_E_DELEGATION_POLICY
			{ com_errc::sec_e_policy_nltm_only, "Client policy does not allow credential delegation to target server with NLTM only authentication. Code: 0x8009035F" },	//	SEC_E_POLICY_NLTM_ONLY
			{ com_errc::sec_i_no_renegotiation, "The recipient rejected the renegotiation request. Code: 0x00090360" },	//	SEC_I_NO_RENEGOTIATION
			{ com_errc::sec_e_no_context, "The required security context does not exist. Code: 0x80090361" },	//	SEC_E_NO_CONTEXT
			{ com_errc::sec_e_pku2u_cert_failure, "The PKU2U protocol encountered an error while attempting to utilize the associated certificates. Code: 0x80090362" },	//	SEC_E_PKU2U_CERT_FAILURE
			{ com_errc::sec_e_mutual_auth_failed, "The identity of the server computer could not be verified. Code: 0x80090363" },	//	SEC_E_MUTUAL_AUTH_FAILED
			{ com_errc::sec_i_message_fragment, "The returned buffer is only a fragment of the message.  More fragments need to be returned. Code: 0x00090364" },	//	SEC_I_MESSAGE_FRAGMENT
			{ com_errc::sec_e_only_https_allowed, "Only https scheme is allowed. Code: 0x80090365" },	//	SEC_E_ONLY_HTTPS_ALLOWED
			{ com_errc::sec_i_continue_needed_message_ok, "The function completed successfully, but must be called again to complete the context.  Early start can be used. Code: 0x00090366" },	//	SEC_I_CONTINUE_NEEDED_MESSAGE_OK
			{ com_errc::sec_e_application_protocol_mismatch, "No common application protocol exists between the client and the server. Application protocol negotiation failed. Code: 0x80090367" },	//	SEC_E_APPLICATION_PROTOCOL_MISMATCH
			{ com_errc::crypt_e_msg_error, "An error occurred while performing an operation on a cryptographic message. Code: 0x80091001" },	//	CRYPT_E_MSG_ERROR
			{ com_errc::crypt_e_unknown_algo, "Unknown cryptographic algorithm. Code: 0x80091002" },	//	CRYPT_E_UNKNOWN_ALGO
			{ com_errc::crypt_e_oid_format, "The object identifier is poorly formatted. Code: 0x80091003" },	//	CRYPT_E_OID_FORMAT
			{ com_errc::crypt_e_invalid_msg_type, "Invalid cryptographic message type. Code: 0x80091004" },	//	CRYPT_E_INVALID_MSG_TYPE
			{ com_errc::crypt_e_unexpected_encoding, "Unexpected cryptographic message encoding. Code: 0x80091005" },	//	CRYPT_E_UNEXPECTED_ENCODING
			{ com_errc::crypt_e_auth_attr_missing, "The cryptographic message does not contain an expected authenticated attribute. Code: 0x80091006" },	//	CRYPT_E_AUTH_ATTR_MISSING
			{ com_errc::crypt_e_hash_value, "The hash value is not correct. Code: 0x80091007" },	//	CRYPT_E_HASH_VALUE
			{ com_errc::crypt_e_invalid_index, "The index value is not valid. Code: 0x80091008" },	//	CRYPT_E_INVALID_INDEX
			{ com_errc::crypt_e_already_decrypted, "The content of the cryptographic message has already been decrypted. Code: 0x80091009" },	//	CRYPT_E_ALREADY_DECRYPTED
			{ com_errc::crypt_e_not_decrypted, "The content of the cryptographic message has not been decrypted yet. Code: 0x8009100A" },	//	CRYPT_E_NOT_DECRYPTED
			{ com_errc::crypt_e_recipient_not_found, "The enveloped-data message does not contain the specified recipient. Code: 0x8009100B" },	//	CRYPT_E_RECIPIENT_NOT_FOUND
			{ com_errc::crypt_e_control_type, "Invalid control type. Code: 0x8009100C" },	//	CRYPT_E_CONTROL_TYPE
			{ com_errc::crypt_e_issuer_serialnumber, "Invalid issuer and/or serial number. Code: 0x8009100D" },	//	CRYPT_E_ISSUER_SERIALNUMBER
			{ com_errc::crypt_e_signer_not_found, "Cannot find the original signer. Code: 0x8009100E" },	//	CRYPT_E_SIGNER_NOT_FOUND
			{ com_errc::crypt_e_attributes_missing, "The cryptographic message does not contain all of the requested attributes. Code: 0x8009100F" },	//	CRYPT_E_ATTRIBUTES_MISSING
			{ com_errc::crypt_e_stream_msg_not_ready, "The streamed cryptographic message is not ready to return data. Code: 0x80091010" },	//	CRYPT_E_STREAM_MSG_NOT_READY
			{ com_errc::crypt_e_stream_insufficient_data, "The streamed cryptographic message requires more data to complete the decode operation. Code: 0x80091011" },	//	CRYPT_E_STREAM_INSUFFICIENT_DATA
			{ com_errc::crypt_i_new_protection_required, "The protected data needs to be re-protected. Code: 0x00091012" },	//	CRYPT_I_NEW_PROTECTION_REQUIRED
			{ com_errc::crypt_e_bad_len, "The length specified for the output data was insufficient. Code: 0x80092001" },	//	CRYPT_E_BAD_LEN
			{ com_errc::crypt_e_bad_encode, "An error occurred during encode or decode operation. Code: 0x80092002" },	//	CRYPT_E_BAD_ENCODE
			{ com_errc::crypt_e_file_error, "An error occurred while reading or writing to a file. Code: 0x80092003" },	//	CRYPT_E_FILE_ERROR
			{ com_errc::crypt_e_not_found, "Cannot find object or property. Code: 0x80092004" },	//	CRYPT_E_NOT_FOUND
			{ com_errc::crypt_e_exists, "The object or property already exists. Code: 0x80092005" },	//	CRYPT_E_EXISTS
			{ com_errc::crypt_e_no_provider, "No provider was specified for the store or object. Code: 0x80092006" },	//	CRYPT_E_NO_PROVIDER
			{ com_errc::crypt_e_self_signed, "The specified certificate is self signed. Code: 0x80092007" },	//	CRYPT_E_SELF_SIGNED
			{ com_errc::crypt_e_deleted_prev, "The previous certificate or CRL context was deleted. Code: 0x80092008" },	//	CRYPT_E_DELETED_PREV
			{ com_errc::crypt_e_no_match, "Cannot find the requested object. Code: 0x80092009" },	//	CRYPT_E_NO_MATCH
			{ com_errc::crypt_e_unexpected_msg_type, "The certificate does not have a property that references a private key. Code: 0x8009200A" },	//	CRYPT_E_UNEXPECTED_MSG_TYPE
			{ com_errc::crypt_e_no_key_property, "Cannot find the certificate and private key for decryption. Code: 0x8009200B" },	//	CRYPT_E_NO_KEY_PROPERTY
			{ com_errc::crypt_e_no_decrypt_cert, "Cannot find the certificate and private key to use for decryption. Code: 0x8009200C" },	//	CRYPT_E_NO_DECRYPT_CERT
			{ com_errc::crypt_e_bad_msg, "Not a cryptographic message or the cryptographic message is not formatted correctly. Code: 0x8009200D" },	//	CRYPT_E_BAD_MSG
			{ com_errc::crypt_e_no_signer, "The signed cryptographic message does not have a signer for the specified signer index. Code: 0x8009200E" },	//	CRYPT_E_NO_SIGNER
			{ com_errc::crypt_e_pending_close, "Final closure is pending until additional frees or closes. Code: 0x8009200F" },	//	CRYPT_E_PENDING_CLOSE
			{ com_errc::crypt_e_revoked, "The certificate is revoked. Code: 0x80092010" },	//	CRYPT_E_REVOKED
			{ com_errc::crypt_e_no_revocation_dll, "No Dll or exported function was found to verify revocation. Code: 0x80092011" },	//	CRYPT_E_NO_REVOCATION_DLL
			{ com_errc::crypt_e_no_revocation_check, "The revocation function was unable to check revocation for the certificate. Code: 0x80092012" },	//	CRYPT_E_NO_REVOCATION_CHECK
			{ com_errc::crypt_e_revocation_offline, "The revocation function was unable to check revocation because the revocation server was offline. Code: 0x80092013" },	//	CRYPT_E_REVOCATION_OFFLINE
			{ com_errc::crypt_e_not_in_revocation_database, "The certificate is not in the revocation server's database. Code: 0x80092014" },	//	CRYPT_E_NOT_IN_REVOCATION_DATABASE
			{ com_errc::crypt_e_invalid_numeric_string, "The string contains a non-numeric character. Code: 0x80092020" },	//	CRYPT_E_INVALID_NUMERIC_STRING
			{ com_errc::crypt_e_invalid_printable_string, "The string contains a non-printable character. Code: 0x80092021" },	//	CRYPT_E_INVALID_PRINTABLE_STRING
			{ com_errc::crypt_e_invalid_ia5_string, "The string contains a character not in the 7 bit ASCII character set. Code: 0x80092022" },	//	CRYPT_E_INVALID_IA5_STRING
			{ com_errc::crypt_e_invalid_x500_string, "The string contains an invalid X500 name attribute key, oid, value or delimiter. Code: 0x80092023" },	//	CRYPT_E_INVALID_X500_STRING
			{ com_errc::crypt_e_not_char_string, "The dwValueType for the CERT_NAME_VALUE is not one of the character strings. Most likely it is either a CERT_RDN_ENCODED_BLOB or CERT_RDN_OCTET_STRING. Code: 0x80092024" },	//	CRYPT_E_NOT_CHAR_STRING
			{ com_errc::crypt_e_fileresized, "The Put operation cannot continue. The file needs to be resized. However, there is already a signature present. A complete signing operation must be done. Code: 0x80092025" },	//	CRYPT_E_FILERESIZED
			{ com_errc::crypt_e_security_settings, "The cryptographic operation failed due to a local security option setting. Code: 0x80092026" },	//	CRYPT_E_SECURITY_SETTINGS
			{ com_errc::crypt_e_no_verify_usage_dll, "No DLL or exported function was found to verify subject usage. Code: 0x80092027" },	//	CRYPT_E_NO_VERIFY_USAGE_DLL
			{ com_errc::crypt_e_no_verify_usage_check, "The called function was unable to do a usage check on the subject. Code: 0x80092028" },	//	CRYPT_E_NO_VERIFY_USAGE_CHECK
			{ com_errc::crypt_e_verify_usage_offline, "Since the server was offline, the called function was unable to complete the usage check. Code: 0x80092029" },	//	CRYPT_E_VERIFY_USAGE_OFFLINE
			{ com_errc::crypt_e_not_in_ctl, "The subject was not found in a Certificate Trust List (CTL). Code: 0x8009202A" },	//	CRYPT_E_NOT_IN_CTL
			{ com_errc::crypt_e_no_trusted_signer, "None of the signers of the cryptographic message or certificate trust list is trusted. Code: 0x8009202B" },	//	CRYPT_E_NO_TRUSTED_SIGNER
			{ com_errc::crypt_e_missing_pubkey_para, "The public key's algorithm parameters are missing. Code: 0x8009202C" },	//	CRYPT_E_MISSING_PUBKEY_PARA
			{ com_errc::crypt_e_object_locator_object_not_found, "An object could not be located using the object locator infrastructure with the given name. Code: 0x8009202D" },	//	CRYPT_E_OBJECT_LOCATOR_OBJECT_NOT_FOUND
			{ com_errc::crypt_e_oss_error, "OSS Certificate encode/decode error code base  See asn1code.h for a definition of the OSS runtime errors. The OSS error values are offset by CRYPT_E_OSS_ERROR. Code: 0x80093000" },	//	CRYPT_E_OSS_ERROR
			{ com_errc::oss_more_buf, "OSS ASN.1 Error: Output Buffer is too small. Code: 0x80093001" },	//	OSS_MORE_BUF
			{ com_errc::oss_negative_uinteger, "OSS ASN.1 Error: Signed integer is encoded as a unsigned integer. Code: 0x80093002" },	//	OSS_NEGATIVE_UINTEGER
			{ com_errc::oss_pdu_range, "OSS ASN.1 Error: Unknown ASN.1 data type. Code: 0x80093003" },	//	OSS_PDU_RANGE
			{ com_errc::oss_more_input, "OSS ASN.1 Error: Output buffer is too small, the decoded data has been truncated. Code: 0x80093004" },	//	OSS_MORE_INPUT
			{ com_errc::oss_data_error, "OSS ASN.1 Error: Invalid data. Code: 0x80093005" },	//	OSS_DATA_ERROR
			{ com_errc::oss_bad_arg, "OSS ASN.1 Error: Invalid argument. Code: 0x80093006" },	//	OSS_BAD_ARG
			{ com_errc::oss_bad_version, "OSS ASN.1 Error: Encode/Decode version mismatch. Code: 0x80093007" },	//	OSS_BAD_VERSION
			{ com_errc::oss_out_memory, "OSS ASN.1 Error: Out of memory. Code: 0x80093008" },	//	OSS_OUT_MEMORY
			{ com_errc::oss_pdu_mismatch, "OSS ASN.1 Error: Encode/Decode Error. Code: 0x80093009" },	//	OSS_PDU_MISMATCH
			{ com_errc::oss_limited, "OSS ASN.1 Error: Internal Error. Code: 0x8009300A" },	//	OSS_LIMITED
			{ com_errc::oss_bad_ptr, "OSS ASN.1 Error: Invalid data. Code: 0x8009300B" },	//	OSS_BAD_PTR
			{ com_errc::oss_bad_time, "OSS ASN.1 Error: Invalid data. Code: 0x8009300C" },	//	OSS_BAD_TIME
			{ com_errc::oss_indefinite_not_supported, "OSS ASN.1 Error: Unsupported BER indefinite-length encoding. Code: 0x8009300D" },	//	OSS_INDEFINITE_NOT_SUPPORTED
			{ com_errc::oss_mem_error, "OSS ASN.1 Error: Access violation. Code: 0x8009300E" },	//	OSS_MEM_ERROR
			{ com_errc::oss_bad_table, "OSS ASN.1 Error: Invalid data. Code: 0x8009300F" },	//	OSS_BAD_TABLE
			{ com_errc::oss_too_long, "OSS ASN.1 Error: Invalid data. Code: 0x80093010" },	//	OSS_TOO_LONG
			{ com_errc::oss_constraint_violated, "OSS ASN.1 Error: Invalid data. Code: 0x80093011" },	//	OSS_CONSTRAINT_VIOLATED
			{ com_errc::oss_fatal_error, "OSS ASN.1 Error: Internal Error. Code: 0x80093012" },	//	OSS_FATAL_ERROR
			{ com_errc::oss_access_serialization_error, "OSS ASN.1 Error: Multi-threading conflict. Code: 0x80093013" },	//	OSS_ACCESS_SERIALIZATION_ERROR
			{ com_errc::oss_null_tbl, "OSS ASN.1 Error: Invalid data. Code: 0x80093014" },	//	OSS_NULL_TBL
			{ com_errc::oss_null_fcn, "OSS ASN.1 Error: Invalid data. Code: 0x80093015" },	//	OSS_NULL_FCN
			{ com_errc::oss_bad_encrules, "OSS ASN.1 Error: Invalid data. Code: 0x80093016" },	//	OSS_BAD_ENCRULES
			{ com_errc::oss_unavail_encrules, "OSS ASN.1 Error: Encode/Decode function not implemented. Code: 0x80093017" },	//	OSS_UNAVAIL_ENCRULES
			{ com_errc::oss_cant_open_trace_window, "OSS ASN.1 Error: Trace file error. Code: 0x80093018" },	//	OSS_CANT_OPEN_TRACE_WINDOW
			{ com_errc::oss_unimplemented, "OSS ASN.1 Error: Function not implemented. Code: 0x80093019" },	//	OSS_UNIMPLEMENTED
			{ com_errc::oss_oid_dll_not_linked, "OSS ASN.1 Error: Program link error. Code: 0x8009301A" },	//	OSS_OID_DLL_NOT_LINKED
			{ com_errc::oss_cant_open_trace_file, "OSS ASN.1 Error: Trace file error. Code: 0x8009301B" },	//	OSS_CANT_OPEN_TRACE_FILE
			{ com_errc::oss_trace_file_already_open, "OSS ASN.1 Error: Trace file error. Code: 0x8009301C" },	//	OSS_TRACE_FILE_ALREADY_OPEN
			{ com_errc::oss_table_mismatch, "OSS ASN.1 Error: Invalid data. Code: 0x8009301D" },	//	OSS_TABLE_MISMATCH
			{ com_errc::oss_type_not_supported, "OSS ASN.1 Error: Invalid data. Code: 0x8009301E" },	//	OSS_TYPE_NOT_SUPPORTED
			{ com_errc::oss_real_dll_not_linked, "OSS ASN.1 Error: Program link error. Code: 0x8009301F" },	//	OSS_REAL_DLL_NOT_LINKED
			{ com_errc::oss_real_code_not_linked, "OSS ASN.1 Error: Program link error. Code: 0x80093020" },	//	OSS_REAL_CODE_NOT_LINKED
			{ com_errc::oss_out_of_range, "OSS ASN.1 Error: Program link error. Code: 0x80093021" },	//	OSS_OUT_OF_RANGE
			{ com_errc::oss_copier_dll_not_linked, "OSS ASN.1 Error: Program link error. Code: 0x80093022" },	//	OSS_COPIER_DLL_NOT_LINKED
			{ com_errc::oss_constraint_dll_not_linked, "OSS ASN.1 Error: Program link error. Code: 0x80093023" },	//	OSS_CONSTRAINT_DLL_NOT_LINKED
			{ com_errc::oss_comparator_dll_not_linked, "OSS ASN.1 Error: Program link error. Code: 0x80093024" },	//	OSS_COMPARATOR_DLL_NOT_LINKED
			{ com_errc::oss_comparator_code_not_linked, "OSS ASN.1 Error: Program link error. Code: 0x80093025" },	//	OSS_COMPARATOR_CODE_NOT_LINKED
			{ com_errc::oss_mem_mgr_dll_not_linked, "OSS ASN.1 Error: Program link error. Code: 0x80093026" },	//	OSS_MEM_MGR_DLL_NOT_LINKED
			{ com_errc::oss_pdv_dll_not_linked, "OSS ASN.1 Error: Program link error. Code: 0x80093027" },	//	OSS_PDV_DLL_NOT_LINKED
			{ com_errc::oss_pdv_code_not_linked, "OSS ASN.1 Error: Program link error. Code: 0x80093028" },	//	OSS_PDV_CODE_NOT_LINKED
			{ com_errc::oss_api_dll_not_linked, "OSS ASN.1 Error: Program link error. Code: 0x80093029" },	//	OSS_API_DLL_NOT_LINKED
			{ com_errc::oss_berder_dll_not_linked, "OSS ASN.1 Error: Program link error. Code: 0x8009302A" },	//	OSS_BERDER_DLL_NOT_LINKED
			{ com_errc::oss_per_dll_not_linked, "OSS ASN.1 Error: Program link error. Code: 0x8009302B" },	//	OSS_PER_DLL_NOT_LINKED
			{ com_errc::oss_open_type_error, "OSS ASN.1 Error: Program link error. Code: 0x8009302C" },	//	OSS_OPEN_TYPE_ERROR
			{ com_errc::oss_mutex_not_created, "OSS ASN.1 Error: System resource error. Code: 0x8009302D" },	//	OSS_MUTEX_NOT_CREATED
			{ com_errc::oss_cant_close_trace_file, "OSS ASN.1 Error: Trace file error. Code: 0x8009302E" },	//	OSS_CANT_CLOSE_TRACE_FILE
			{ com_errc::crypt_e_asn1_error, "ASN1 Certificate encode/decode error code base. The ASN1 error values are offset by CRYPT_E_ASN1_ERROR. Code: 0x80093100" },	//	CRYPT_E_ASN1_ERROR
			{ com_errc::crypt_e_asn1_internal, "ASN1 internal encode or decode error. Code: 0x80093101" },	//	CRYPT_E_ASN1_INTERNAL
			{ com_errc::crypt_e_asn1_eod, "ASN1 unexpected end of data. Code: 0x80093102" },	//	CRYPT_E_ASN1_EOD
			{ com_errc::crypt_e_asn1_corrupt, "ASN1 corrupted data. Code: 0x80093103" },	//	CRYPT_E_ASN1_CORRUPT
			{ com_errc::crypt_e_asn1_large, "ASN1 value too large. Code: 0x80093104" },	//	CRYPT_E_ASN1_LARGE
			{ com_errc::crypt_e_asn1_constraint, "ASN1 constraint violated. Code: 0x80093105" },	//	CRYPT_E_ASN1_CONSTRAINT
			{ com_errc::crypt_e_asn1_memory, "ASN1 out of memory. Code: 0x80093106" },	//	CRYPT_E_ASN1_MEMORY
			{ com_errc::crypt_e_asn1_overflow, "ASN1 buffer overflow. Code: 0x80093107" },	//	CRYPT_E_ASN1_OVERFLOW
			{ com_errc::crypt_e_asn1_badpdu, "ASN1 function not supported for this PDU. Code: 0x80093108" },	//	CRYPT_E_ASN1_BADPDU
			{ com_errc::crypt_e_asn1_badargs, "ASN1 bad arguments to function call. Code: 0x80093109" },	//	CRYPT_E_ASN1_BADARGS
			{ com_errc::crypt_e_asn1_badreal, "ASN1 bad real value. Code: 0x8009310A" },	//	CRYPT_E_ASN1_BADREAL
			{ com_errc::crypt_e_asn1_badtag, "ASN1 bad tag value met. Code: 0x8009310B" },	//	CRYPT_E_ASN1_BADTAG
			{ com_errc::crypt_e_asn1_choice, "ASN1 bad choice value. Code: 0x8009310C" },	//	CRYPT_E_ASN1_CHOICE
			{ com_errc::crypt_e_asn1_rule, "ASN1 bad encoding rule. Code: 0x8009310D" },	//	CRYPT_E_ASN1_RULE
			{ com_errc::crypt_e_asn1_utf8, "ASN1 bad unicode (UTF8). Code: 0x8009310E" },	//	CRYPT_E_ASN1_UTF8
			{ com_errc::crypt_e_asn1_pdu_type, "ASN1 bad PDU type. Code: 0x80093133" },	//	CRYPT_E_ASN1_PDU_TYPE
			{ com_errc::crypt_e_asn1_nyi, "ASN1 not yet implemented. Code: 0x80093134" },	//	CRYPT_E_ASN1_NYI
			{ com_errc::crypt_e_asn1_extended, "ASN1 skipped unknown extension(s). Code: 0x80093201" },	//	CRYPT_E_ASN1_EXTENDED
			{ com_errc::crypt_e_asn1_noeod, "ASN1 end of data expected. Code: 0x80093202" },	//	CRYPT_E_ASN1_NOEOD
			{ com_errc::certsrv_e_bad_requestsubject, "The request subject name is invalid or too long. Code: 0x80094001" },	//	CERTSRV_E_BAD_REQUESTSUBJECT
			{ com_errc::certsrv_e_no_request, "The request does not exist. Code: 0x80094002" },	//	CERTSRV_E_NO_REQUEST
			{ com_errc::certsrv_e_bad_requeststatus, "The request's current status does not allow this operation. Code: 0x80094003" },	//	CERTSRV_E_BAD_REQUESTSTATUS
			{ com_errc::certsrv_e_property_empty, "The requested property value is empty. Code: 0x80094004" },	//	CERTSRV_E_PROPERTY_EMPTY
			{ com_errc::certsrv_e_invalid_ca_certificate, "The certification authority's certificate contains invalid data. Code: 0x80094005" },	//	CERTSRV_E_INVALID_CA_CERTIFICATE
			{ com_errc::certsrv_e_server_suspended, "Certificate service has been suspended for a database restore operation. Code: 0x80094006" },	//	CERTSRV_E_SERVER_SUSPENDED
			{ com_errc::certsrv_e_encoding_length, "The certificate contains an encoded length that is potentially incompatible with older enrollment software. Code: 0x80094007" },	//	CERTSRV_E_ENCODING_LENGTH
			{ com_errc::certsrv_e_roleconflict, "The operation is denied. The user has multiple roles assigned and the certification authority is configured to enforce role separation. Code: 0x80094008" },	//	CERTSRV_E_ROLECONFLICT
			{ com_errc::certsrv_e_restrictedofficer, "The operation is denied. It can only be performed by a certificate manager that is allowed to manage certificates for the current requester. Code: 0x80094009" },	//	CERTSRV_E_RESTRICTEDOFFICER
			{ com_errc::certsrv_e_key_archival_not_configured, "Cannot archive private key. The certification authority is not configured for key archival. Code: 0x8009400A" },	//	CERTSRV_E_KEY_ARCHIVAL_NOT_CONFIGURED
			{ com_errc::certsrv_e_no_valid_kra, "Cannot archive private key. The certification authority could not verify one or more key recovery certificates. Code: 0x8009400B" },	//	CERTSRV_E_NO_VALID_KRA
			{ com_errc::certsrv_e_bad_request_key_archival, "The request is incorrectly formatted. The encrypted private key must be in an unauthenticated attribute in an outermost signature. Code: 0x8009400C" },	//	CERTSRV_E_BAD_REQUEST_KEY_ARCHIVAL
			{ com_errc::certsrv_e_no_caadmin_defined, "At least one security principal must have the permission to manage this CA. Code: 0x8009400D" },	//	CERTSRV_E_NO_CAADMIN_DEFINED
			{ com_errc::certsrv_e_bad_renewal_cert_attribute, "The request contains an invalid renewal certificate attribute. Code: 0x8009400E" },	//	CERTSRV_E_BAD_RENEWAL_CERT_ATTRIBUTE
			{ com_errc::certsrv_e_no_db_sessions, "An attempt was made to open a Certification Authority database session, but there are already too many active sessions. The server may need to be configured to allow additional sessions. Code: 0x8009400F" },	//	CERTSRV_E_NO_DB_SESSIONS
			{ com_errc::certsrv_e_alignment_fault, "A memory reference caused a data alignment fault. Code: 0x80094010" },	//	CERTSRV_E_ALIGNMENT_FAULT
			{ com_errc::certsrv_e_enroll_denied, "The permissions on this certification authority do not allow the current user to enroll for certificates. Code: 0x80094011" },	//	CERTSRV_E_ENROLL_DENIED
			{ com_errc::certsrv_e_template_denied, "The permissions on the certificate template do not allow the current user to enroll for this type of certificate. Code: 0x80094012" },	//	CERTSRV_E_TEMPLATE_DENIED
			{ com_errc::certsrv_e_downlevel_dc_ssl_or_upgrade, "The contacted domain controller cannot support signed LDAP traffic. Update the domain controller or configure Certificate Services to use SSL for Active Directory access. Code: 0x80094013" },	//	CERTSRV_E_DOWNLEVEL_DC_SSL_OR_UPGRADE
			{ com_errc::certsrv_e_admin_denied_request, "The request was denied by a certificate manager or CA administrator. Code: 0x80094014" },	//	CERTSRV_E_ADMIN_DENIED_REQUEST
			{ com_errc::certsrv_e_no_policy_server, "An enrollment policy server cannot be located. Code: 0x80094015" },	//	CERTSRV_E_NO_POLICY_SERVER
			{ com_errc::certsrv_e_weak_signature_or_key, "A signature algorithm or public key length does not meet the system's minimum required strength. Code: 0x80094016" },	//	CERTSRV_E_WEAK_SIGNATURE_OR_KEY
			{ com_errc::certsrv_e_key_attestation_not_supported, "Failed to create an attested key.  This computer or the cryptographic provider may not meet the hardware requirements to support key attestation. Code: 0x80094017" },	//	CERTSRV_E_KEY_ATTESTATION_NOT_SUPPORTED
			{ com_errc::certsrv_e_encryption_cert_required, "No encryption certificate was specified. Code: 0x80094018" },	//	CERTSRV_E_ENCRYPTION_CERT_REQUIRED
			{ com_errc::certsrv_e_unsupported_cert_type, "The requested certificate template is not supported by this CA. Code: 0x80094800" },	//	CERTSRV_E_UNSUPPORTED_CERT_TYPE
			{ com_errc::certsrv_e_no_cert_type, "The request contains no certificate template information. Code: 0x80094801" },	//	CERTSRV_E_NO_CERT_TYPE
			{ com_errc::certsrv_e_template_conflict, "The request contains conflicting template information. Code: 0x80094802" },	//	CERTSRV_E_TEMPLATE_CONFLICT
			{ com_errc::certsrv_e_subject_alt_name_required, "The request is missing a required Subject Alternate name extension. Code: 0x80094803" },	//	CERTSRV_E_SUBJECT_ALT_NAME_REQUIRED
			{ com_errc::certsrv_e_archived_key_required, "The request is missing a required private key for archival by the server. Code: 0x80094804" },	//	CERTSRV_E_ARCHIVED_KEY_REQUIRED
			{ com_errc::certsrv_e_smime_required, "The request is missing a required SMIME capabilities extension. Code: 0x80094805" },	//	CERTSRV_E_SMIME_REQUIRED
			{ com_errc::certsrv_e_bad_renewal_subject, "The request was made on behalf of a subject other than the caller. The certificate template must be configured to require at least one signature to authorize the request. Code: 0x80094806" },	//	CERTSRV_E_BAD_RENEWAL_SUBJECT
			{ com_errc::certsrv_e_bad_template_version, "The request template version is newer than the supported template version. Code: 0x80094807" },	//	CERTSRV_E_BAD_TEMPLATE_VERSION
			{ com_errc::certsrv_e_template_policy_required, "The template is missing a required signature policy attribute. Code: 0x80094808" },	//	CERTSRV_E_TEMPLATE_POLICY_REQUIRED
			{ com_errc::certsrv_e_signature_policy_required, "The request is missing required signature policy information. Code: 0x80094809" },	//	CERTSRV_E_SIGNATURE_POLICY_REQUIRED
			{ com_errc::certsrv_e_signature_count, "The request is missing one or more required signatures. Code: 0x8009480A" },	//	CERTSRV_E_SIGNATURE_COUNT
			{ com_errc::certsrv_e_signature_rejected, "One or more signatures did not include the required application or issuance policies. The request is missing one or more required valid signatures. Code: 0x8009480B" },	//	CERTSRV_E_SIGNATURE_REJECTED
			{ com_errc::certsrv_e_issuance_policy_required, "The request is missing one or more required signature issuance policies. Code: 0x8009480C" },	//	CERTSRV_E_ISSUANCE_POLICY_REQUIRED
			{ com_errc::certsrv_e_subject_upn_required, "The UPN is unavailable and cannot be added to the Subject Alternate name. Code: 0x8009480D" },	//	CERTSRV_E_SUBJECT_UPN_REQUIRED
			{ com_errc::certsrv_e_subject_directory_guid_required, "The Active Directory GUID is unavailable and cannot be added to the Subject Alternate name. Code: 0x8009480E" },	//	CERTSRV_E_SUBJECT_DIRECTORY_GUID_REQUIRED
			{ com_errc::certsrv_e_subject_dns_required, "The DNS name is unavailable and cannot be added to the Subject Alternate name. Code: 0x8009480F" },	//	CERTSRV_E_SUBJECT_DNS_REQUIRED
			{ com_errc::certsrv_e_archived_key_unexpected, "The request includes a private key for archival by the server, but key archival is not enabled for the specified certificate template. Code: 0x80094810" },	//	CERTSRV_E_ARCHIVED_KEY_UNEXPECTED
			{ com_errc::certsrv_e_key_length, "The public key does not meet the minimum size required by the specified certificate template. Code: 0x80094811" },	//	CERTSRV_E_KEY_LENGTH
			{ com_errc::certsrv_e_subject_email_required, "The EMail name is unavailable and cannot be added to the Subject or Subject Alternate name. Code: 0x80094812" },	//	CERTSRV_E_SUBJECT_EMAIL_REQUIRED
			{ com_errc::certsrv_e_unknown_cert_type, "One or more certificate templates to be enabled on this certification authority could not be found. Code: 0x80094813" },	//	CERTSRV_E_UNKNOWN_CERT_TYPE
			{ com_errc::certsrv_e_cert_type_overlap, "The certificate template renewal period is longer than the certificate validity period. The template should be reconfigured or the CA certificate renewed. Code: 0x80094814" },	//	CERTSRV_E_CERT_TYPE_OVERLAP
			{ com_errc::certsrv_e_too_many_signatures, "The certificate template requires too many RA signatures. Only one RA signature is allowed. Code: 0x80094815" },	//	CERTSRV_E_TOO_MANY_SIGNATURES
			{ com_errc::certsrv_e_renewal_bad_public_key, "The certificate template requires renewal with the same public key, but the request uses a different public key. Code: 0x80094816" },	//	CERTSRV_E_RENEWAL_BAD_PUBLIC_KEY
			{ com_errc::certsrv_e_invalid_ek, "The certification authority cannot interpret or verify the endorsement key information supplied in the request, or the information is inconsistent. Code: 0x80094817" },	//	CERTSRV_E_INVALID_EK
			{ com_errc::certsrv_e_invalid_idbinding, "The certification authority cannot validate the Attestation Identity Key Id Binding. Code: 0x80094818" },	//	CERTSRV_E_INVALID_IDBINDING
			{ com_errc::certsrv_e_invalid_attestation, "The certification authority cannot validate the private key attestation data. Code: 0x80094819" },	//	CERTSRV_E_INVALID_ATTESTATION
			{ com_errc::certsrv_e_key_attestation, "The request does not support private key attestation as defined in the certificate template. Code: 0x8009481A" },	//	CERTSRV_E_KEY_ATTESTATION
			{ com_errc::certsrv_e_corrupt_key_attestation, "The request public key is not consistent with the private key attestation data. Code: 0x8009481B" },	//	CERTSRV_E_CORRUPT_KEY_ATTESTATION
			{ com_errc::certsrv_e_expired_challenge, "The private key attestation challenge cannot be validated because the encryption certificate has expired, or the certificate or key is unavailable. Code: 0x8009481C" },	//	CERTSRV_E_EXPIRED_CHALLENGE
			{ com_errc::certsrv_e_invalid_response, "The attestation response could not be validated. It is either unexpected or incorrect. Code: 0x8009481D" },	//	CERTSRV_E_INVALID_RESPONSE
			{ com_errc::certsrv_e_invalid_requestid, "A valid Request ID was not detected in the request attributes, or an invalid one was submitted. Code: 0x8009481E" },	//	CERTSRV_E_INVALID_REQUESTID
			{ com_errc::xenroll_e_key_not_exportable, "The key is not exportable. Code: 0x80095000" },	//	XENROLL_E_KEY_NOT_EXPORTABLE
			{ com_errc::xenroll_e_cannot_add_root_cert, "You cannot add the root CA certificate into your local store. Code: 0x80095001" },	//	XENROLL_E_CANNOT_ADD_ROOT_CERT
			{ com_errc::xenroll_e_response_ka_hash_not_found, "The key archival hash attribute was not found in the response. Code: 0x80095002" },	//	XENROLL_E_RESPONSE_KA_HASH_NOT_FOUND
			{ com_errc::xenroll_e_response_unexpected_ka_hash, "An unexpected key archival hash attribute was found in the response. Code: 0x80095003" },	//	XENROLL_E_RESPONSE_UNEXPECTED_KA_HASH
			{ com_errc::xenroll_e_response_ka_hash_mismatch, "There is a key archival hash mismatch between the request and the response. Code: 0x80095004" },	//	XENROLL_E_RESPONSE_KA_HASH_MISMATCH
			{ com_errc::xenroll_e_keyspec_smime_mismatch, "Signing certificate cannot include SMIME extension. Code: 0x80095005" },	//	XENROLL_E_KEYSPEC_SMIME_MISMATCH
			{ com_errc::trust_e_system_error, "A system-level error occurred while verifying trust. Code: 0x80096001" },	//	TRUST_E_SYSTEM_ERROR
			{ com_errc::trust_e_no_signer_cert, "The certificate for the signer of the message is invalid or not found. Code: 0x80096002" },	//	TRUST_E_NO_SIGNER_CERT
			{ com_errc::trust_e_counter_signer, "One of the counter signatures was invalid. Code: 0x80096003" },	//	TRUST_E_COUNTER_SIGNER
			{ com_errc::trust_e_cert_signature, "The signature of the certificate cannot be verified. Code: 0x80096004" },	//	TRUST_E_CERT_SIGNATURE
			{ com_errc::trust_e_time_stamp, "The timestamp signature and/or certificate could not be verified or is malformed. Code: 0x80096005" },	//	TRUST_E_TIME_STAMP
			{ com_errc::trust_e_bad_digest, "The digital signature of the object did not verify. Code: 0x80096010" },	//	TRUST_E_BAD_DIGEST
			{ com_errc::trust_e_basic_constraints, "A certificate's basic constraint extension has not been observed. Code: 0x80096019" },	//	TRUST_E_BASIC_CONSTRAINTS
			{ com_errc::trust_e_financial_criteria, "The certificate does not meet or contain the Authenticode(tm) financial extensions. Code: 0x8009601E" },	//	TRUST_E_FINANCIAL_CRITERIA
			{ com_errc::mssipotf_e_outofmemrange, "Tried to reference a part of the file outside the proper range. Code: 0x80097001" },	//	MSSIPOTF_E_OUTOFMEMRANGE
			{ com_errc::mssipotf_e_cantgetobject, "Could not retrieve an object from the file. Code: 0x80097002" },	//	MSSIPOTF_E_CANTGETOBJECT
			{ com_errc::mssipotf_e_noheadtable, "Could not find the head table in the file. Code: 0x80097003" },	//	MSSIPOTF_E_NOHEADTABLE
			{ com_errc::mssipotf_e_bad_magicnumber, "The magic number in the head table is incorrect. Code: 0x80097004" },	//	MSSIPOTF_E_BAD_MAGICNUMBER
			{ com_errc::mssipotf_e_bad_offset_table, "The offset table has incorrect values. Code: 0x80097005" },	//	MSSIPOTF_E_BAD_OFFSET_TABLE
			{ com_errc::mssipotf_e_table_tagorder, "Duplicate table tags or tags out of alphabetical order. Code: 0x80097006" },	//	MSSIPOTF_E_TABLE_TAGORDER
			{ com_errc::mssipotf_e_table_longword, "A table does not start on a long word boundary. Code: 0x80097007" },	//	MSSIPOTF_E_TABLE_LONGWORD
			{ com_errc::mssipotf_e_bad_first_table_placement, "First table does not appear after header information. Code: 0x80097008" },	//	MSSIPOTF_E_BAD_FIRST_TABLE_PLACEMENT
			{ com_errc::mssipotf_e_tables_overlap, "Two or more tables overlap. Code: 0x80097009" },	//	MSSIPOTF_E_TABLES_OVERLAP
			{ com_errc::mssipotf_e_table_padbytes, "Too many pad bytes between tables or pad bytes are not 0. Code: 0x8009700A" },	//	MSSIPOTF_E_TABLE_PADBYTES
			{ com_errc::mssipotf_e_filetoosmall, "File is too small to contain the last table. Code: 0x8009700B" },	//	MSSIPOTF_E_FILETOOSMALL
			{ com_errc::mssipotf_e_table_checksum, "A table checksum is incorrect. Code: 0x8009700C" },	//	MSSIPOTF_E_TABLE_CHECKSUM
			{ com_errc::mssipotf_e_file_checksum, "The file checksum is incorrect. Code: 0x8009700D" },	//	MSSIPOTF_E_FILE_CHECKSUM
			{ com_errc::mssipotf_e_failed_policy, "The signature does not have the correct attributes for the policy. Code: 0x80097010" },	//	MSSIPOTF_E_FAILED_POLICY
			{ com_errc::mssipotf_e_failed_hints_check, "The file did not pass the hints check. Code: 0x80097011" },	//	MSSIPOTF_E_FAILED_HINTS_CHECK
			{ com_errc::mssipotf_e_not_opentype, "The file is not an OpenType file. Code: 0x80097012" },	//	MSSIPOTF_E_NOT_OPENTYPE
			{ com_errc::mssipotf_e_file, "Failed on a file operation (open, map, read, write). Code: 0x80097013" },	//	MSSIPOTF_E_FILE
			{ com_errc::mssipotf_e_crypt, "A call to a CryptoAPI function failed. Code: 0x80097014" },	//	MSSIPOTF_E_CRYPT
			{ com_errc::mssipotf_e_badversion, "There is a bad version number in the file. Code: 0x80097015" },	//	MSSIPOTF_E_BADVERSION
			{ com_errc::mssipotf_e_dsig_structure, "The structure of the DSIG table is incorrect. Code: 0x80097016" },	//	MSSIPOTF_E_DSIG_STRUCTURE
			{ com_errc::mssipotf_e_pconst_check, "A check failed in a partially constant table. Code: 0x80097017" },	//	MSSIPOTF_E_PCONST_CHECK
			{ com_errc::mssipotf_e_structure, "Some kind of structural error. Code: 0x80097018" },	//	MSSIPOTF_E_STRUCTURE
			{ com_errc::error_cred_requires_confirmation, "The requested credential requires confirmation. Code: 0x80097019" },	//	ERROR_CRED_REQUIRES_CONFIRMATION
			{ com_errc::trust_e_provider_unknown, "Unknown trust provider. Code: 0x800B0001" },	//	TRUST_E_PROVIDER_UNKNOWN
			{ com_errc::trust_e_action_unknown, "The trust verification action specified is not supported by the specified trust provider. Code: 0x800B0002" },	//	TRUST_E_ACTION_UNKNOWN
			{ com_errc::trust_e_subject_form_unknown, "The form specified for the subject is not one supported or known by the specified trust provider. Code: 0x800B0003" },	//	TRUST_E_SUBJECT_FORM_UNKNOWN
			{ com_errc::trust_e_subject_not_trusted, "The subject is not trusted for the specified action. Code: 0x800B0004" },	//	TRUST_E_SUBJECT_NOT_TRUSTED
			{ com_errc::digsig_e_encode, "Error due to problem in ASN.1 encoding process. Code: 0x800B0005" },	//	DIGSIG_E_ENCODE
			{ com_errc::digsig_e_decode, "Error due to problem in ASN.1 decoding process. Code: 0x800B0006" },	//	DIGSIG_E_DECODE
			{ com_errc::digsig_e_extensibility, "Reading / writing Extensions where Attributes are appropriate, and vice versa. Code: 0x800B0007" },	//	DIGSIG_E_EXTENSIBILITY
			{ com_errc::digsig_e_crypto, "Unspecified cryptographic failure. Code: 0x800B0008" },	//	DIGSIG_E_CRYPTO
			{ com_errc::persist_e_sizedefinite, "The size of the data could not be determined. Code: 0x800B0009" },	//	PERSIST_E_SIZEDEFINITE
			{ com_errc::persist_e_sizeindefinite, "The size of the indefinite-sized data could not be determined. Code: 0x800B000A" },	//	PERSIST_E_SIZEINDEFINITE
			{ com_errc::persist_e_notselfsizing, "This object does not read and write self-sizing data. Code: 0x800B000B" },	//	PERSIST_E_NOTSELFSIZING
			{ com_errc::trust_e_nosignature, "No signature was present in the subject. Code: 0x800B0100" },	//	TRUST_E_NOSIGNATURE
			{ com_errc::cert_e_expired, "A required certificate is not within its validity period when verifying against the current system clock or the timestamp in the signed file. Code: 0x800B0101" },	//	CERT_E_EXPIRED
			{ com_errc::cert_e_validityperiodnesting, "The validity periods of the certification chain do not nest correctly. Code: 0x800B0102" },	//	CERT_E_VALIDITYPERIODNESTING
			{ com_errc::cert_e_role, "A certificate that can only be used as an end-entity is being used as a CA or vice versa. Code: 0x800B0103" },	//	CERT_E_ROLE
			{ com_errc::cert_e_pathlenconst, "A path length constraint in the certification chain has been violated. Code: 0x800B0104" },	//	CERT_E_PATHLENCONST
			{ com_errc::cert_e_critical, "A certificate contains an unknown extension that is marked 'critical'. Code: 0x800B0105" },	//	CERT_E_CRITICAL
			{ com_errc::cert_e_purpose, "A certificate being used for a purpose other than the ones specified by its CA. Code: 0x800B0106" },	//	CERT_E_PURPOSE
			{ com_errc::cert_e_issuerchaining, "A parent of a given certificate in fact did not issue that child certificate. Code: 0x800B0107" },	//	CERT_E_ISSUERCHAINING
			{ com_errc::cert_e_malformed, "A certificate is missing or has an empty value for an important field, such as a subject or issuer name. Code: 0x800B0108" },	//	CERT_E_MALFORMED
			{ com_errc::cert_e_untrustedroot, "A certificate chain processed, but terminated in a root certificate which is not trusted by the trust provider. Code: 0x800B0109" },	//	CERT_E_UNTRUSTEDROOT
			{ com_errc::cert_e_chaining, "A certificate chain could not be built to a trusted root authority. Code: 0x800B010A" },	//	CERT_E_CHAINING
			{ com_errc::trust_e_fail, "Generic trust failure. Code: 0x800B010B" },	//	TRUST_E_FAIL
			{ com_errc::cert_e_revoked, "A certificate was explicitly revoked by its issuer. Code: 0x800B010C" },	//	CERT_E_REVOKED
			{ com_errc::cert_e_untrustedtestroot, "The certification path terminates with the test root which is not trusted with the current policy settings. Code: 0x800B010D" },	//	CERT_E_UNTRUSTEDTESTROOT
			{ com_errc::cert_e_revocation_failure, "The revocation process could not continue - the certificate(s) could not be checked. Code: 0x800B010E" },	//	CERT_E_REVOCATION_FAILURE
			{ com_errc::cert_e_cn_no_match, "The certificate's CN name does not match the passed value. Code: 0x800B010F" },	//	CERT_E_CN_NO_MATCH
			{ com_errc::cert_e_wrong_usage, "The certificate is not valid for the requested usage. Code: 0x800B0110" },	//	CERT_E_WRONG_USAGE
			{ com_errc::trust_e_explicit_distrust, "The certificate was explicitly marked as untrusted by the user. Code: 0x800B0111" },	//	TRUST_E_EXPLICIT_DISTRUST
			{ com_errc::cert_e_untrustedca, "A certification chain processed correctly, but one of the CA certificates is not trusted by the policy provider. Code: 0x800B0112" },	//	CERT_E_UNTRUSTEDCA
			{ com_errc::cert_e_invalid_policy, "The certificate has invalid policy. Code: 0x800B0113" },	//	CERT_E_INVALID_POLICY
			{ com_errc::cert_e_invalid_name, "The certificate has an invalid name. The name is not included in the permitted list or is explicitly excluded. Code: 0x800B0114" },	//	CERT_E_INVALID_NAME
			{ com_errc::spapi_e_expected_section_name, "A non-empty line was encountered in the INF before the start of a section. Code: 0x800F0000" },	//	SPAPI_E_EXPECTED_SECTION_NAME
			{ com_errc::spapi_e_bad_section_name_line, "A section name marker in the INF is not complete, or does not exist on a line by itself. Code: 0x800F0001" },	//	SPAPI_E_BAD_SECTION_NAME_LINE
			{ com_errc::spapi_e_section_name_too_long, "An INF section was encountered whose name exceeds the maximum section name length. Code: 0x800F0002" },	//	SPAPI_E_SECTION_NAME_TOO_LONG
			{ com_errc::spapi_e_general_syntax, "The syntax of the INF is invalid. Code: 0x800F0003" },	//	SPAPI_E_GENERAL_SYNTAX
			{ com_errc::spapi_e_wrong_inf_style, "The style of the INF is different than what was requested. Code: 0x800F0100" },	//	SPAPI_E_WRONG_INF_STYLE
			{ com_errc::spapi_e_section_not_found, "The required section was not found in the INF. Code: 0x800F0101" },	//	SPAPI_E_SECTION_NOT_FOUND
			{ com_errc::spapi_e_line_not_found, "The required line was not found in the INF. Code: 0x800F0102" },	//	SPAPI_E_LINE_NOT_FOUND
			{ com_errc::spapi_e_no_backup, "The files affected by the installation of this file queue have not been backed up for uninstall. Code: 0x800F0103" },	//	SPAPI_E_NO_BACKUP
			{ com_errc::spapi_e_no_associated_class, "The INF or the device information set or element does not have an associated install class. Code: 0x800F0200" },	//	SPAPI_E_NO_ASSOCIATED_CLASS
			{ com_errc::spapi_e_class_mismatch, "The INF or the device information set or element does not match the specified install class. Code: 0x800F0201" },	//	SPAPI_E_CLASS_MISMATCH
			{ com_errc::spapi_e_duplicate_found, "An existing device was found that is a duplicate of the device being manually installed. Code: 0x800F0202" },	//	SPAPI_E_DUPLICATE_FOUND
			{ com_errc::spapi_e_no_driver_selected, "There is no driver selected for the device information set or element. Code: 0x800F0203" },	//	SPAPI_E_NO_DRIVER_SELECTED
			{ com_errc::spapi_e_key_does_not_exist, "The requested device registry key does not exist. Code: 0x800F0204" },	//	SPAPI_E_KEY_DOES_NOT_EXIST
			{ com_errc::spapi_e_invalid_devinst_name, "The device instance name is invalid. Code: 0x800F0205" },	//	SPAPI_E_INVALID_DEVINST_NAME
			{ com_errc::spapi_e_invalid_class, "The install class is not present or is invalid. Code: 0x800F0206" },	//	SPAPI_E_INVALID_CLASS
			{ com_errc::spapi_e_devinst_already_exists, "The device instance cannot be created because it already exists. Code: 0x800F0207" },	//	SPAPI_E_DEVINST_ALREADY_EXISTS
			{ com_errc::spapi_e_devinfo_not_registered, "The operation cannot be performed on a device information element that has not been registered. Code: 0x800F0208" },	//	SPAPI_E_DEVINFO_NOT_REGISTERED
			{ com_errc::spapi_e_invalid_reg_property, "The device property code is invalid. Code: 0x800F0209" },	//	SPAPI_E_INVALID_REG_PROPERTY
			{ com_errc::spapi_e_no_inf, "The INF from which a driver list is to be built does not exist. Code: 0x800F020A" },	//	SPAPI_E_NO_INF
			{ com_errc::spapi_e_no_such_devinst, "The device instance does not exist in the hardware tree. Code: 0x800F020B" },	//	SPAPI_E_NO_SUCH_DEVINST
			{ com_errc::spapi_e_cant_load_class_icon, "The icon representing this install class cannot be loaded. Code: 0x800F020C" },	//	SPAPI_E_CANT_LOAD_CLASS_ICON
			{ com_errc::spapi_e_invalid_class_installer, "The class installer registry entry is invalid. Code: 0x800F020D" },	//	SPAPI_E_INVALID_CLASS_INSTALLER
			{ com_errc::spapi_e_di_do_default, "The class installer has indicated that the default action should be performed for this installation request. Code: 0x800F020E" },	//	SPAPI_E_DI_DO_DEFAULT
			{ com_errc::spapi_e_di_nofilecopy, "The operation does not require any files to be copied. Code: 0x800F020F" },	//	SPAPI_E_DI_NOFILECOPY
			{ com_errc::spapi_e_invalid_hwprofile, "The specified hardware profile does not exist. Code: 0x800F0210" },	//	SPAPI_E_INVALID_HWPROFILE
			{ com_errc::spapi_e_no_device_selected, "There is no device information element currently selected for this device information set. Code: 0x800F0211" },	//	SPAPI_E_NO_DEVICE_SELECTED
			{ com_errc::spapi_e_devinfo_list_locked, "The operation cannot be performed because the device information set is locked. Code: 0x800F0212" },	//	SPAPI_E_DEVINFO_LIST_LOCKED
			{ com_errc::spapi_e_devinfo_data_locked, "The operation cannot be performed because the device information element is locked. Code: 0x800F0213" },	//	SPAPI_E_DEVINFO_DATA_LOCKED
			{ com_errc::spapi_e_di_bad_path, "The specified path does not contain any applicable device INFs. Code: 0x800F0214" },	//	SPAPI_E_DI_BAD_PATH
			{ com_errc::spapi_e_no_classinstall_params, "No class installer parameters have been set for the device information set or element. Code: 0x800F0215" },	//	SPAPI_E_NO_CLASSINSTALL_PARAMS
			{ com_errc::spapi_e_filequeue_locked, "The operation cannot be performed because the file queue is locked. Code: 0x800F0216" },	//	SPAPI_E_FILEQUEUE_LOCKED
			{ com_errc::spapi_e_bad_service_installsect, "A service installation section in this INF is invalid. Code: 0x800F0217" },	//	SPAPI_E_BAD_SERVICE_INSTALLSECT
			{ com_errc::spapi_e_no_class_driver_list, "There is no class driver list for the device information element. Code: 0x800F0218" },	//	SPAPI_E_NO_CLASS_DRIVER_LIST
			{ com_errc::spapi_e_no_associated_service, "The installation failed because a function driver was not specified for this device instance. Code: 0x800F0219" },	//	SPAPI_E_NO_ASSOCIATED_SERVICE
			{ com_errc::spapi_e_no_default_device_interface, "There is presently no default device interface designated for this interface class. Code: 0x800F021A" },	//	SPAPI_E_NO_DEFAULT_DEVICE_INTERFACE
			{ com_errc::spapi_e_device_interface_active, "The operation cannot be performed because the device interface is currently active. Code: 0x800F021B" },	//	SPAPI_E_DEVICE_INTERFACE_ACTIVE
			{ com_errc::spapi_e_device_interface_removed, "The operation cannot be performed because the device interface has been removed from the system. Code: 0x800F021C" },	//	SPAPI_E_DEVICE_INTERFACE_REMOVED
			{ com_errc::spapi_e_bad_interface_installsect, "An interface installation section in this INF is invalid. Code: 0x800F021D" },	//	SPAPI_E_BAD_INTERFACE_INSTALLSECT
			{ com_errc::spapi_e_no_such_interface_class, "This interface class does not exist in the system. Code: 0x800F021E" },	//	SPAPI_E_NO_SUCH_INTERFACE_CLASS
			{ com_errc::spapi_e_invalid_reference_string, "The reference string supplied for this interface device is invalid. Code: 0x800F021F" },	//	SPAPI_E_INVALID_REFERENCE_STRING
			{ com_errc::spapi_e_invalid_machinename, "The specified machine name does not conform to UNC naming conventions. Code: 0x800F0220" },	//	SPAPI_E_INVALID_MACHINENAME
			{ com_errc::spapi_e_remote_comm_failure, "A general remote communication error occurred. Code: 0x800F0221" },	//	SPAPI_E_REMOTE_COMM_FAILURE
			{ com_errc::spapi_e_machine_unavailable, "The machine selected for remote communication is not available at this time. Code: 0x800F0222" },	//	SPAPI_E_MACHINE_UNAVAILABLE
			{ com_errc::spapi_e_no_configmgr_services, "The Plug and Play service is not available on the remote machine. Code: 0x800F0223" },	//	SPAPI_E_NO_CONFIGMGR_SERVICES
			{ com_errc::spapi_e_invalid_proppage_provider, "The property page provider registry entry is invalid. Code: 0x800F0224" },	//	SPAPI_E_INVALID_PROPPAGE_PROVIDER
			{ com_errc::spapi_e_no_such_device_interface, "The requested device interface is not present in the system. Code: 0x800F0225" },	//	SPAPI_E_NO_SUCH_DEVICE_INTERFACE
			{ com_errc::spapi_e_di_postprocessing_required, "The device's co-installer has additional work to perform after installation is complete. Code: 0x800F0226" },	//	SPAPI_E_DI_POSTPROCESSING_REQUIRED
			{ com_errc::spapi_e_invalid_coinstaller, "The device's co-installer is invalid. Code: 0x800F0227" },	//	SPAPI_E_INVALID_COINSTALLER
			{ com_errc::spapi_e_no_compat_drivers, "There are no compatible drivers for this device. Code: 0x800F0228" },	//	SPAPI_E_NO_COMPAT_DRIVERS
			{ com_errc::spapi_e_no_device_icon, "There is no icon that represents this device or device type. Code: 0x800F0229" },	//	SPAPI_E_NO_DEVICE_ICON
			{ com_errc::spapi_e_invalid_inf_logconfig, "A logical configuration specified in this INF is invalid. Code: 0x800F022A" },	//	SPAPI_E_INVALID_INF_LOGCONFIG
			{ com_errc::spapi_e_di_dont_install, "The class installer has denied the request to install or upgrade this device. Code: 0x800F022B" },	//	SPAPI_E_DI_DONT_INSTALL
			{ com_errc::spapi_e_invalid_filter_driver, "One of the filter drivers installed for this device is invalid. Code: 0x800F022C" },	//	SPAPI_E_INVALID_FILTER_DRIVER
			{ com_errc::spapi_e_non_windows_nt_driver, "The driver selected for this device does not support this version of Windows. Code: 0x800F022D" },	//	SPAPI_E_NON_WINDOWS_NT_DRIVER
			{ com_errc::spapi_e_non_windows_driver, "The driver selected for this device does not support Windows. Code: 0x800F022E" },	//	SPAPI_E_NON_WINDOWS_DRIVER
			{ com_errc::spapi_e_no_catalog_for_oem_inf, "The third-party INF does not contain digital signature information. Code: 0x800F022F" },	//	SPAPI_E_NO_CATALOG_FOR_OEM_INF
			{ com_errc::spapi_e_devinstall_queue_nonnative, "An invalid attempt was made to use a device installation file queue for verification of digital signatures relative to other platforms. Code: 0x800F0230" },	//	SPAPI_E_DEVINSTALL_QUEUE_NONNATIVE
			{ com_errc::spapi_e_not_disableable, "The device cannot be disabled. Code: 0x800F0231" },	//	SPAPI_E_NOT_DISABLEABLE
			{ com_errc::spapi_e_cant_remove_devinst, "The device could not be dynamically removed. Code: 0x800F0232" },	//	SPAPI_E_CANT_REMOVE_DEVINST
			{ com_errc::spapi_e_invalid_target, "Cannot copy to specified target. Code: 0x800F0233" },	//	SPAPI_E_INVALID_TARGET
			{ com_errc::spapi_e_driver_nonnative, "Driver is not intended for this platform. Code: 0x800F0234" },	//	SPAPI_E_DRIVER_NONNATIVE
			{ com_errc::spapi_e_in_wow64, "Operation not allowed in WOW64. Code: 0x800F0235" },	//	SPAPI_E_IN_WOW64
			{ com_errc::spapi_e_set_system_restore_point, "The operation involving unsigned file copying was rolled back, so that a system restore point could be set. Code: 0x800F0236" },	//	SPAPI_E_SET_SYSTEM_RESTORE_POINT
			{ com_errc::spapi_e_incorrectly_copied_inf, "An INF was copied into the Windows INF directory in an improper manner. Code: 0x800F0237" },	//	SPAPI_E_INCORRECTLY_COPIED_INF
			{ com_errc::spapi_e_sce_disabled, "The Security Configuration Editor (SCE) APIs have been disabled on this Embedded product. Code: 0x800F0238" },	//	SPAPI_E_SCE_DISABLED
			{ com_errc::spapi_e_unknown_exception, "An unknown exception was encountered. Code: 0x800F0239" },	//	SPAPI_E_UNKNOWN_EXCEPTION
			{ com_errc::spapi_e_pnp_registry_error, "A problem was encountered when accessing the Plug and Play registry database. Code: 0x800F023A" },	//	SPAPI_E_PNP_REGISTRY_ERROR
			{ com_errc::spapi_e_remote_request_unsupported, "The requested operation is not supported for a remote machine. Code: 0x800F023B" },	//	SPAPI_E_REMOTE_REQUEST_UNSUPPORTED
			{ com_errc::spapi_e_not_an_installed_oem_inf, "The specified file is not an installed OEM INF. Code: 0x800F023C" },	//	SPAPI_E_NOT_AN_INSTALLED_OEM_INF
			{ com_errc::spapi_e_inf_in_use_by_devices, "One or more devices are presently installed using the specified INF. Code: 0x800F023D" },	//	SPAPI_E_INF_IN_USE_BY_DEVICES
			{ com_errc::spapi_e_di_function_obsolete, "The requested device install operation is obsolete. Code: 0x800F023E" },	//	SPAPI_E_DI_FUNCTION_OBSOLETE
			{ com_errc::spapi_e_no_authenticode_catalog, "A file could not be verified because it does not have an associated catalog signed via Authenticode(tm). Code: 0x800F023F" },	//	SPAPI_E_NO_AUTHENTICODE_CATALOG
			{ com_errc::spapi_e_authenticode_disallowed, "Authenticode(tm) signature verification is not supported for the specified INF. Code: 0x800F0240" },	//	SPAPI_E_AUTHENTICODE_DISALLOWED
			{ com_errc::spapi_e_authenticode_trusted_publisher, "The INF was signed with an Authenticode(tm) catalog from a trusted publisher. Code: 0x800F0241" },	//	SPAPI_E_AUTHENTICODE_TRUSTED_PUBLISHER
			{ com_errc::spapi_e_authenticode_trust_not_established, "The publisher of an Authenticode(tm) signed catalog has not yet been established as trusted. Code: 0x800F0242" },	//	SPAPI_E_AUTHENTICODE_TRUST_NOT_ESTABLISHED
			{ com_errc::spapi_e_authenticode_publisher_not_trusted, "The publisher of an Authenticode(tm) signed catalog was not established as trusted. Code: 0x800F0243" },	//	SPAPI_E_AUTHENTICODE_PUBLISHER_NOT_TRUSTED
			{ com_errc::spapi_e_signature_osattribute_mismatch, "The software was tested for compliance with Windows Logo requirements on a different version of Windows, and may not be compatible with this version. Code: 0x800F0244" },	//	SPAPI_E_SIGNATURE_OSATTRIBUTE_MISMATCH
			{ com_errc::spapi_e_only_validate_via_authenticode, "The file may only be validated by a catalog signed via Authenticode(tm). Code: 0x800F0245" },	//	SPAPI_E_ONLY_VALIDATE_VIA_AUTHENTICODE
			{ com_errc::spapi_e_device_installer_not_ready, "One of the installers for this device cannot perform the installation at this time. Code: 0x800F0246" },	//	SPAPI_E_DEVICE_INSTALLER_NOT_READY
			{ com_errc::spapi_e_driver_store_add_failed, "A problem was encountered while attempting to add the driver to the store. Code: 0x800F0247" },	//	SPAPI_E_DRIVER_STORE_ADD_FAILED
			{ com_errc::spapi_e_device_install_blocked, "The installation of this device is forbidden by system policy. Contact your system administrator. Code: 0x800F0248" },	//	SPAPI_E_DEVICE_INSTALL_BLOCKED
			{ com_errc::spapi_e_driver_install_blocked, "The installation of this driver is forbidden by system policy. Contact your system administrator. Code: 0x800F0249" },	//	SPAPI_E_DRIVER_INSTALL_BLOCKED
			{ com_errc::spapi_e_wrong_inf_type, "The specified INF is the wrong type for this operation. Code: 0x800F024A" },	//	SPAPI_E_WRONG_INF_TYPE
			{ com_errc::spapi_e_file_hash_not_in_catalog, "The hash for the file is not present in the specified catalog file. The file is likely corrupt or the victim of tampering. Code: 0x800F024B" },	//	SPAPI_E_FILE_HASH_NOT_IN_CATALOG
			{ com_errc::spapi_e_driver_store_delete_failed, "A problem was encountered while attempting to delete the driver from the store. Code: 0x800F024C" },	//	SPAPI_E_DRIVER_STORE_DELETE_FAILED
			{ com_errc::spapi_e_unrecoverable_stack_overflow, "An unrecoverable stack overflow was encountered. Code: 0x800F0300" },	//	SPAPI_E_UNRECOVERABLE_STACK_OVERFLOW
			{ com_errc::spapi_e_error_not_installed, "No installed components were detected. Code: 0x800F1000" },	//	SPAPI_E_ERROR_NOT_INSTALLED
			{ com_errc::scard_f_internal_error, "An internal consistency check failed. Code: 0x80100001" },	//	SCARD_F_INTERNAL_ERROR
			{ com_errc::scard_e_cancelled, "The action was cancelled by an SCardCancel request. Code: 0x80100002" },	//	SCARD_E_CANCELLED
			{ com_errc::scard_e_invalid_handle, "The supplied handle was invalid. Code: 0x80100003" },	//	SCARD_E_INVALID_HANDLE
			{ com_errc::scard_e_invalid_parameter, "One or more of the supplied parameters could not be properly interpreted. Code: 0x80100004" },	//	SCARD_E_INVALID_PARAMETER
			{ com_errc::scard_e_invalid_target, "Registry startup information is missing or invalid. Code: 0x80100005" },	//	SCARD_E_INVALID_TARGET
			{ com_errc::scard_e_no_memory, "Not enough memory available to complete this command. Code: 0x80100006" },	//	SCARD_E_NO_MEMORY
			{ com_errc::scard_f_waited_too_long, "An internal consistency timer has expired. Code: 0x80100007" },	//	SCARD_F_WAITED_TOO_LONG
			{ com_errc::scard_e_insufficient_buffer, "The data buffer to receive returned data is too small for the returned data. Code: 0x80100008" },	//	SCARD_E_INSUFFICIENT_BUFFER
			{ com_errc::scard_e_unknown_reader, "The specified reader name is not recognized. Code: 0x80100009" },	//	SCARD_E_UNKNOWN_READER
			{ com_errc::scard_e_timeout, "The user-specified timeout value has expired. Code: 0x8010000A" },	//	SCARD_E_TIMEOUT
			{ com_errc::scard_e_sharing_violation, "The smart card cannot be accessed because of other connections outstanding. Code: 0x8010000B" },	//	SCARD_E_SHARING_VIOLATION
			{ com_errc::scard_e_no_smartcard, "The operation requires a smart card, but no smart card is currently in the device. Code: 0x8010000C" },	//	SCARD_E_NO_SMARTCARD
			{ com_errc::scard_e_unknown_card, "The specified smart card name is not recognized. Code: 0x8010000D" },	//	SCARD_E_UNKNOWN_CARD
			{ com_errc::scard_e_cant_dispose, "The system could not dispose of the media in the requested manner. Code: 0x8010000E" },	//	SCARD_E_CANT_DISPOSE
			{ com_errc::scard_e_proto_mismatch, "The requested protocols are incompatible with the protocol currently in use with the smart card. Code: 0x8010000F" },	//	SCARD_E_PROTO_MISMATCH
			{ com_errc::scard_e_not_ready, "The reader or smart card is not ready to accept commands. Code: 0x80100010" },	//	SCARD_E_NOT_READY
			{ com_errc::scard_e_invalid_value, "One or more of the supplied parameters values could not be properly interpreted. Code: 0x80100011" },	//	SCARD_E_INVALID_VALUE
			{ com_errc::scard_e_system_cancelled, "The action was cancelled by the system, presumably to log off or shut down. Code: 0x80100012" },	//	SCARD_E_SYSTEM_CANCELLED
			{ com_errc::scard_f_comm_error, "An internal communications error has been detected. Code: 0x80100013" },	//	SCARD_F_COMM_ERROR
			{ com_errc::scard_f_unknown_error, "An internal error has been detected, but the source is unknown. Code: 0x80100014" },	//	SCARD_F_UNKNOWN_ERROR
			{ com_errc::scard_e_invalid_atr, "An ATR obtained from the registry is not a valid ATR string. Code: 0x80100015" },	//	SCARD_E_INVALID_ATR
			{ com_errc::scard_e_not_transacted, "An attempt was made to end a non-existent transaction. Code: 0x80100016" },	//	SCARD_E_NOT_TRANSACTED
			{ com_errc::scard_e_reader_unavailable, "The specified reader is not currently available for use. Code: 0x80100017" },	//	SCARD_E_READER_UNAVAILABLE
			{ com_errc::scard_p_shutdown, "The operation has been aborted to allow the server application to exit. Code: 0x80100018" },	//	SCARD_P_SHUTDOWN
			{ com_errc::scard_e_pci_too_small, "The PCI Receive buffer was too small. Code: 0x80100019" },	//	SCARD_E_PCI_TOO_SMALL
			{ com_errc::scard_e_reader_unsupported, "The reader driver does not meet minimal requirements for support. Code: 0x8010001A" },	//	SCARD_E_READER_UNSUPPORTED
			{ com_errc::scard_e_duplicate_reader, "The reader driver did not produce a unique reader name. Code: 0x8010001B" },	//	SCARD_E_DUPLICATE_READER
			{ com_errc::scard_e_card_unsupported, "The smart card does not meet minimal requirements for support. Code: 0x8010001C" },	//	SCARD_E_CARD_UNSUPPORTED
			{ com_errc::scard_e_no_service, "The Smart Card Resource Manager is not running. Code: 0x8010001D" },	//	SCARD_E_NO_SERVICE
			{ com_errc::scard_e_service_stopped, "The Smart Card Resource Manager has shut down. Code: 0x8010001E" },	//	SCARD_E_SERVICE_STOPPED
			{ com_errc::scard_e_unexpected, "An unexpected card error has occurred. Code: 0x8010001F" },	//	SCARD_E_UNEXPECTED
			{ com_errc::scard_e_icc_installation, "No Primary Provider can be found for the smart card. Code: 0x80100020" },	//	SCARD_E_ICC_INSTALLATION
			{ com_errc::scard_e_icc_createorder, "The requested order of object creation is not supported. Code: 0x80100021" },	//	SCARD_E_ICC_CREATEORDER
			{ com_errc::scard_e_unsupported_feature, "This smart card does not support the requested feature. Code: 0x80100022" },	//	SCARD_E_UNSUPPORTED_FEATURE
			{ com_errc::scard_e_dir_not_found, "The identified directory does not exist in the smart card. Code: 0x80100023" },	//	SCARD_E_DIR_NOT_FOUND
			{ com_errc::scard_e_file_not_found, "The identified file does not exist in the smart card. Code: 0x80100024" },	//	SCARD_E_FILE_NOT_FOUND
			{ com_errc::scard_e_no_dir, "The supplied path does not represent a smart card directory. Code: 0x80100025" },	//	SCARD_E_NO_DIR
			{ com_errc::scard_e_no_file, "The supplied path does not represent a smart card file. Code: 0x80100026" },	//	SCARD_E_NO_FILE
			{ com_errc::scard_e_no_access, "Access is denied to this file. Code: 0x80100027" },	//	SCARD_E_NO_ACCESS
			{ com_errc::scard_e_write_too_many, "The smart card does not have enough memory to store the information. Code: 0x80100028" },	//	SCARD_E_WRITE_TOO_MANY
			{ com_errc::scard_e_bad_seek, "There was an error trying to set the smart card file object pointer. Code: 0x80100029" },	//	SCARD_E_BAD_SEEK
			{ com_errc::scard_e_invalid_chv, "The supplied PIN is incorrect. Code: 0x8010002A" },	//	SCARD_E_INVALID_CHV
			{ com_errc::scard_e_unknown_res_mng, "An unrecognized error code was returned from a layered component. Code: 0x8010002B" },	//	SCARD_E_UNKNOWN_RES_MNG
			{ com_errc::scard_e_no_such_certificate, "The requested certificate does not exist. Code: 0x8010002C" },	//	SCARD_E_NO_SUCH_CERTIFICATE
			{ com_errc::scard_e_certificate_unavailable, "The requested certificate could not be obtained. Code: 0x8010002D" },	//	SCARD_E_CERTIFICATE_UNAVAILABLE
			{ com_errc::scard_e_no_readers_available, "Cannot find a smart card reader. Code: 0x8010002E" },	//	SCARD_E_NO_READERS_AVAILABLE
			{ com_errc::scard_e_comm_data_lost, "A communications error with the smart card has been detected. Retry the operation. Code: 0x8010002F" },	//	SCARD_E_COMM_DATA_LOST
			{ com_errc::scard_e_no_key_container, "The requested key container does not exist on the smart card. Code: 0x80100030" },	//	SCARD_E_NO_KEY_CONTAINER
			{ com_errc::scard_e_server_too_busy, "The Smart Card Resource Manager is too busy to complete this operation. Code: 0x80100031" },	//	SCARD_E_SERVER_TOO_BUSY
			{ com_errc::scard_e_pin_cache_expired, "The smart card PIN cache has expired. Code: 0x80100032" },	//	SCARD_E_PIN_CACHE_EXPIRED
			{ com_errc::scard_e_no_pin_cache, "The smart card PIN cannot be cached. Code: 0x80100033" },	//	SCARD_E_NO_PIN_CACHE
			{ com_errc::scard_e_read_only_card, "The smart card is read only and cannot be written to. Code: 0x80100034" },	//	SCARD_E_READ_ONLY_CARD
			{ com_errc::scard_w_unsupported_card, "The reader cannot communicate with the smart card, due to ATR configuration conflicts. Code: 0x80100065" },	//	SCARD_W_UNSUPPORTED_CARD
			{ com_errc::scard_w_unresponsive_card, "The smart card is not responding to a reset. Code: 0x80100066" },	//	SCARD_W_UNRESPONSIVE_CARD
			{ com_errc::scard_w_unpowered_card, "Power has been removed from the smart card, so that further communication is not possible. Code: 0x80100067" },	//	SCARD_W_UNPOWERED_CARD
			{ com_errc::scard_w_reset_card, "The smart card has been reset, so any shared state information is invalid. Code: 0x80100068" },	//	SCARD_W_RESET_CARD
			{ com_errc::scard_w_removed_card, "The smart card has been removed, so that further communication is not possible. Code: 0x80100069" },	//	SCARD_W_REMOVED_CARD
			{ com_errc::scard_w_security_violation, "Access was denied because of a security violation. Code: 0x8010006A" },	//	SCARD_W_SECURITY_VIOLATION
			{ com_errc::scard_w_wrong_chv, "The card cannot be accessed because the wrong PIN was presented. Code: 0x8010006B" },	//	SCARD_W_WRONG_CHV
			{ com_errc::scard_w_chv_blocked, "The card cannot be accessed because the maximum number of PIN entry attempts has been reached. Code: 0x8010006C" },	//	SCARD_W_CHV_BLOCKED
			{ com_errc::scard_w_eof, "The end of the smart card file has been reached. Code: 0x8010006D" },	//	SCARD_W_EOF
			{ com_errc::scard_w_cancelled_by_user, "The action was cancelled by the user. Code: 0x8010006E" },	//	SCARD_W_CANCELLED_BY_USER
			{ com_errc::scard_w_card_not_authenticated, "No PIN was presented to the smart card. Code: 0x8010006F" },	//	SCARD_W_CARD_NOT_AUTHENTICATED
			{ com_errc::scard_w_cache_item_not_found, "The requested item could not be found in the cache. Code: 0x80100070" },	//	SCARD_W_CACHE_ITEM_NOT_FOUND
			{ com_errc::scard_w_cache_item_stale, "The requested cache item is too old and was deleted from the cache. Code: 0x80100071" },	//	SCARD_W_CACHE_ITEM_STALE
			{ com_errc::scard_w_cache_item_too_big, "The new cache item exceeds the maximum per-item size defined for the cache. Code: 0x80100072" },	//	SCARD_W_CACHE_ITEM_TOO_BIG
			{ com_errc::comadmin_e_objecterrors, "Errors occurred accessing one or more objects - the ErrorInfo collection may have more detail. Code: 0x80110401" },	//	COMADMIN_E_OBJECTERRORS
			{ com_errc::comadmin_e_objectinvalid, "One or more of the object's properties are missing or invalid. Code: 0x80110402" },	//	COMADMIN_E_OBJECTINVALID
			{ com_errc::comadmin_e_keymissing, "The object was not found in the catalog. Code: 0x80110403" },	//	COMADMIN_E_KEYMISSING
			{ com_errc::comadmin_e_alreadyinstalled, "The object is already registered. Code: 0x80110404" },	//	COMADMIN_E_ALREADYINSTALLED
			{ com_errc::comadmin_e_app_file_writefail, "Error occurred writing to the application file. Code: 0x80110407" },	//	COMADMIN_E_APP_FILE_WRITEFAIL
			{ com_errc::comadmin_e_app_file_readfail, "Error occurred reading the application file. Code: 0x80110408" },	//	COMADMIN_E_APP_FILE_READFAIL
			{ com_errc::comadmin_e_app_file_version, "Invalid version number in application file. Code: 0x80110409" },	//	COMADMIN_E_APP_FILE_VERSION
			{ com_errc::comadmin_e_badpath, "The file path is invalid. Code: 0x8011040A" },	//	COMADMIN_E_BADPATH
			{ com_errc::comadmin_e_applicationexists, "The application is already installed. Code: 0x8011040B" },	//	COMADMIN_E_APPLICATIONEXISTS
			{ com_errc::comadmin_e_roleexists, "The role already exists. Code: 0x8011040C" },	//	COMADMIN_E_ROLEEXISTS
			{ com_errc::comadmin_e_cantcopyfile, "An error occurred copying the file. Code: 0x8011040D" },	//	COMADMIN_E_CANTCOPYFILE
			{ com_errc::comadmin_e_nouser, "One or more users are not valid. Code: 0x8011040F" },	//	COMADMIN_E_NOUSER
			{ com_errc::comadmin_e_invaliduserids, "One or more users in the application file are not valid. Code: 0x80110410" },	//	COMADMIN_E_INVALIDUSERIDS
			{ com_errc::comadmin_e_noregistryclsid, "The component's CLSID is missing or corrupt. Code: 0x80110411" },	//	COMADMIN_E_NOREGISTRYCLSID
			{ com_errc::comadmin_e_badregistryprogid, "The component's progID is missing or corrupt. Code: 0x80110412" },	//	COMADMIN_E_BADREGISTRYPROGID
			{ com_errc::comadmin_e_authenticationlevel, "Unable to set required authentication level for update request. Code: 0x80110413" },	//	COMADMIN_E_AUTHENTICATIONLEVEL
			{ com_errc::comadmin_e_userpasswdnotvalid, "The identity or password set on the application is not valid. Code: 0x80110414" },	//	COMADMIN_E_USERPASSWDNOTVALID
			{ com_errc::comadmin_e_clsidoriidmismatch, "Application file CLSIDs or IIDs do not match corresponding DLLs. Code: 0x80110418" },	//	COMADMIN_E_CLSIDORIIDMISMATCH
			{ com_errc::comadmin_e_remoteinterface, "Interface information is either missing or changed. Code: 0x80110419" },	//	COMADMIN_E_REMOTEINTERFACE
			{ com_errc::comadmin_e_dllregisterserver, "DllRegisterServer failed on component install. Code: 0x8011041A" },	//	COMADMIN_E_DLLREGISTERSERVER
			{ com_errc::comadmin_e_noservershare, "No server file share available. Code: 0x8011041B" },	//	COMADMIN_E_NOSERVERSHARE
			{ com_errc::comadmin_e_dllloadfailed, "DLL could not be loaded. Code: 0x8011041D" },	//	COMADMIN_E_DLLLOADFAILED
			{ com_errc::comadmin_e_badregistrylibid, "The registered TypeLib ID is not valid. Code: 0x8011041E" },	//	COMADMIN_E_BADREGISTRYLIBID
			{ com_errc::comadmin_e_appdirnotfound, "Application install directory not found. Code: 0x8011041F" },	//	COMADMIN_E_APPDIRNOTFOUND
			{ com_errc::comadmin_e_registrarfailed, "Errors occurred while in the component registrar. Code: 0x80110423" },	//	COMADMIN_E_REGISTRARFAILED
			{ com_errc::comadmin_e_compfile_doesnotexist, "The file does not exist. Code: 0x80110424" },	//	COMADMIN_E_COMPFILE_DOESNOTEXIST
			{ com_errc::comadmin_e_compfile_loaddllfail, "The DLL could not be loaded. Code: 0x80110425" },	//	COMADMIN_E_COMPFILE_LOADDLLFAIL
			{ com_errc::comadmin_e_compfile_getclassobj, "GetClassObject failed in the DLL. Code: 0x80110426" },	//	COMADMIN_E_COMPFILE_GETCLASSOBJ
			{ com_errc::comadmin_e_compfile_classnotavail, "The DLL does not support the components listed in the TypeLib. Code: 0x80110427" },	//	COMADMIN_E_COMPFILE_CLASSNOTAVAIL
			{ com_errc::comadmin_e_compfile_badtlb, "The TypeLib could not be loaded. Code: 0x80110428" },	//	COMADMIN_E_COMPFILE_BADTLB
			{ com_errc::comadmin_e_compfile_notinstallable, "The file does not contain components or component information. Code: 0x80110429" },	//	COMADMIN_E_COMPFILE_NOTINSTALLABLE
			{ com_errc::comadmin_e_notchangeable, "Changes to this object and its sub-objects have been disabled. Code: 0x8011042A" },	//	COMADMIN_E_NOTCHANGEABLE
			{ com_errc::comadmin_e_notdeleteable, "The delete function has been disabled for this object. Code: 0x8011042B" },	//	COMADMIN_E_NOTDELETEABLE
			{ com_errc::comadmin_e_session, "The server catalog version is not supported. Code: 0x8011042C" },	//	COMADMIN_E_SESSION
			{ com_errc::comadmin_e_comp_move_locked, "The component move was disallowed, because the source or destination application is either a system application or currently locked against changes. Code: 0x8011042D" },	//	COMADMIN_E_COMP_MOVE_LOCKED
			{ com_errc::comadmin_e_comp_move_bad_dest, "The component move failed because the destination application no longer exists. Code: 0x8011042E" },	//	COMADMIN_E_COMP_MOVE_BAD_DEST
			{ com_errc::comadmin_e_registertlb, "The system was unable to register the TypeLib. Code: 0x80110430" },	//	COMADMIN_E_REGISTERTLB
			{ com_errc::comadmin_e_systemapp, "This operation cannot be performed on the system application. Code: 0x80110433" },	//	COMADMIN_E_SYSTEMAPP
			{ com_errc::comadmin_e_compfile_noregistrar, "The component registrar referenced in this file is not available. Code: 0x80110434" },	//	COMADMIN_E_COMPFILE_NOREGISTRAR
			{ com_errc::comadmin_e_coreqcompinstalled, "A component in the same DLL is already installed. Code: 0x80110435" },	//	COMADMIN_E_COREQCOMPINSTALLED
			{ com_errc::comadmin_e_servicenotinstalled, "The service is not installed. Code: 0x80110436" },	//	COMADMIN_E_SERVICENOTINSTALLED
			{ com_errc::comadmin_e_propertysavefailed, "One or more property settings are either invalid or in conflict with each other. Code: 0x80110437" },	//	COMADMIN_E_PROPERTYSAVEFAILED
			{ com_errc::comadmin_e_objectexists, "The object you are attempting to add or rename already exists. Code: 0x80110438" },	//	COMADMIN_E_OBJECTEXISTS
			{ com_errc::comadmin_e_componentexists, "The component already exists. Code: 0x80110439" },	//	COMADMIN_E_COMPONENTEXISTS
			{ com_errc::comadmin_e_regfile_corrupt, "The registration file is corrupt. Code: 0x8011043B" },	//	COMADMIN_E_REGFILE_CORRUPT
			{ com_errc::comadmin_e_property_overflow, "The property value is too large. Code: 0x8011043C" },	//	COMADMIN_E_PROPERTY_OVERFLOW
			{ com_errc::comadmin_e_notinregistry, "Object was not found in registry. Code: 0x8011043E" },	//	COMADMIN_E_NOTINREGISTRY
			{ com_errc::comadmin_e_objectnotpoolable, "This object is not poolable. Code: 0x8011043F" },	//	COMADMIN_E_OBJECTNOTPOOLABLE
			{ com_errc::comadmin_e_applid_matches_clsid, "A CLSID with the same GUID as the new application ID is already installed on this machine. Code: 0x80110446" },	//	COMADMIN_E_APPLID_MATCHES_CLSID
			{ com_errc::comadmin_e_role_does_not_exist, "A role assigned to a component, interface, or method did not exist in the application. Code: 0x80110447" },	//	COMADMIN_E_ROLE_DOES_NOT_EXIST
			{ com_errc::comadmin_e_start_app_needs_components, "You must have components in an application in order to start the application. Code: 0x80110448" },	//	COMADMIN_E_START_APP_NEEDS_COMPONENTS
			{ com_errc::comadmin_e_requires_different_platform, "This operation is not enabled on this platform. Code: 0x80110449" },	//	COMADMIN_E_REQUIRES_DIFFERENT_PLATFORM
			{ com_errc::comadmin_e_can_not_export_app_proxy, "Application Proxy is not exportable. Code: 0x8011044A" },	//	COMADMIN_E_CAN_NOT_EXPORT_APP_PROXY
			{ com_errc::comadmin_e_can_not_start_app, "Failed to start application because it is either a library application or an application proxy. Code: 0x8011044B" },	//	COMADMIN_E_CAN_NOT_START_APP
			{ com_errc::comadmin_e_can_not_export_sys_app, "System application is not exportable. Code: 0x8011044C" },	//	COMADMIN_E_CAN_NOT_EXPORT_SYS_APP
			{ com_errc::comadmin_e_cant_subscribe_to_component, "Cannot subscribe to this component (the component may have been imported). Code: 0x8011044D" },	//	COMADMIN_E_CANT_SUBSCRIBE_TO_COMPONENT
			{ com_errc::comadmin_e_eventclass_cant_be_subscriber, "An event class cannot also be a subscriber component. Code: 0x8011044E" },	//	COMADMIN_E_EVENTCLASS_CANT_BE_SUBSCRIBER
			{ com_errc::comadmin_e_lib_app_proxy_incompatible, "Library applications and application proxies are incompatible. Code: 0x8011044F" },	//	COMADMIN_E_LIB_APP_PROXY_INCOMPATIBLE
			{ com_errc::comadmin_e_base_partition_only, "This function is valid for the base partition only. Code: 0x80110450" },	//	COMADMIN_E_BASE_PARTITION_ONLY
			{ com_errc::comadmin_e_start_app_disabled, "You cannot start an application that has been disabled. Code: 0x80110451" },	//	COMADMIN_E_START_APP_DISABLED
			{ com_errc::comadmin_e_cat_duplicate_partition_name, "The specified partition name is already in use on this computer. Code: 0x80110457" },	//	COMADMIN_E_CAT_DUPLICATE_PARTITION_NAME
			{ com_errc::comadmin_e_cat_invalid_partition_name, "The specified partition name is invalid. Check that the name contains at least one visible character. Code: 0x80110458" },	//	COMADMIN_E_CAT_INVALID_PARTITION_NAME
			{ com_errc::comadmin_e_cat_partition_in_use, "The partition cannot be deleted because it is the default partition for one or more users. Code: 0x80110459" },	//	COMADMIN_E_CAT_PARTITION_IN_USE
			{ com_errc::comadmin_e_file_partition_duplicate_files, "The partition cannot be exported, because one or more components in the partition have the same file name. Code: 0x8011045A" },	//	COMADMIN_E_FILE_PARTITION_DUPLICATE_FILES
			{ com_errc::comadmin_e_cat_imported_components_not_allowed, "Applications that contain one or more imported components cannot be installed into a non-base partition. Code: 0x8011045B" },	//	COMADMIN_E_CAT_IMPORTED_COMPONENTS_NOT_ALLOWED
			{ com_errc::comadmin_e_ambiguous_application_name, "The application name is not unique and cannot be resolved to an application id. Code: 0x8011045C" },	//	COMADMIN_E_AMBIGUOUS_APPLICATION_NAME
			{ com_errc::comadmin_e_ambiguous_partition_name, "The partition name is not unique and cannot be resolved to a partition id. Code: 0x8011045D" },	//	COMADMIN_E_AMBIGUOUS_PARTITION_NAME
			{ com_errc::comadmin_e_regdb_notinitialized, "The COM+ registry database has not been initialized. Code: 0x80110472" },	//	COMADMIN_E_REGDB_NOTINITIALIZED
			{ com_errc::comadmin_e_regdb_notopen, "The COM+ registry database is not open. Code: 0x80110473" },	//	COMADMIN_E_REGDB_NOTOPEN
			{ com_errc::comadmin_e_regdb_systemerr, "The COM+ registry database detected a system error. Code: 0x80110474" },	//	COMADMIN_E_REGDB_SYSTEMERR
			{ com_errc::comadmin_e_regdb_alreadyrunning, "The COM+ registry database is already running. Code: 0x80110475" },	//	COMADMIN_E_REGDB_ALREADYRUNNING
			{ com_errc::comadmin_e_mig_versionnotsupported, "This version of the COM+ registry database cannot be migrated. Code: 0x80110480" },	//	COMADMIN_E_MIG_VERSIONNOTSUPPORTED
			{ com_errc::comadmin_e_mig_schemanotfound, "The schema version to be migrated could not be found in the COM+ registry database. Code: 0x80110481" },	//	COMADMIN_E_MIG_SCHEMANOTFOUND
			{ com_errc::comadmin_e_cat_bitnessmismatch, "There was a type mismatch between binaries. Code: 0x80110482" },	//	COMADMIN_E_CAT_BITNESSMISMATCH
			{ com_errc::comadmin_e_cat_unacceptablebitness, "A binary of unknown or invalid type was provided. Code: 0x80110483" },	//	COMADMIN_E_CAT_UNACCEPTABLEBITNESS
			{ com_errc::comadmin_e_cat_wrongappbitness, "There was a type mismatch between a binary and an application. Code: 0x80110484" },	//	COMADMIN_E_CAT_WRONGAPPBITNESS
			{ com_errc::comadmin_e_cat_pause_resume_not_supported, "The application cannot be paused or resumed. Code: 0x80110485" },	//	COMADMIN_E_CAT_PAUSE_RESUME_NOT_SUPPORTED
			{ com_errc::comadmin_e_cat_serverfault, "The COM+ Catalog Server threw an exception during execution. Code: 0x80110486" },	//	COMADMIN_E_CAT_SERVERFAULT
			{ com_errc::comqc_e_application_not_queued, "Only COM+ Applications marked \"queued\" can be invoked using the \"queue\" moniker. Code: 0x80110600" },	//	COMQC_E_APPLICATION_NOT_QUEUED
			{ com_errc::comqc_e_no_queueable_interfaces, "At least one interface must be marked \"queued\" in order to create a queued component instance with the \"queue\" moniker. Code: 0x80110601" },	//	COMQC_E_NO_QUEUEABLE_INTERFACES
			{ com_errc::comqc_e_queuing_service_not_available, "MSMQ is required for the requested operation and is not installed. Code: 0x80110602" },	//	COMQC_E_QUEUING_SERVICE_NOT_AVAILABLE
			{ com_errc::comqc_e_no_ipersiststream, "Unable to marshal an interface that does not support IPersistStream. Code: 0x80110603" },	//	COMQC_E_NO_IPERSISTSTREAM
			{ com_errc::comqc_e_bad_message, "The message is improperly formatted or was damaged in transit. Code: 0x80110604" },	//	COMQC_E_BAD_MESSAGE
			{ com_errc::comqc_e_unauthenticated, "An unauthenticated message was received by an application that accepts only authenticated messages. Code: 0x80110605" },	//	COMQC_E_UNAUTHENTICATED
			{ com_errc::comqc_e_untrusted_enqueuer, "The message was requeued or moved by a user not in the \"QC Trusted User\" role. Code: 0x80110606" },	//	COMQC_E_UNTRUSTED_ENQUEUER
			{ com_errc::msdtc_e_duplicate_resource, "Cannot create a duplicate resource of type Distributed Transaction Coordinator. Code: 0x80110701" },	//	MSDTC_E_DUPLICATE_RESOURCE
			{ com_errc::comadmin_e_object_parent_missing, "One of the objects being inserted or updated does not belong to a valid parent collection. Code: 0x80110808" },	//	COMADMIN_E_OBJECT_PARENT_MISSING
			{ com_errc::comadmin_e_object_does_not_exist, "One of the specified objects cannot be found. Code: 0x80110809" },	//	COMADMIN_E_OBJECT_DOES_NOT_EXIST
			{ com_errc::comadmin_e_app_not_running, "The specified application is not currently running. Code: 0x8011080A" },	//	COMADMIN_E_APP_NOT_RUNNING
			{ com_errc::comadmin_e_invalid_partition, "The partition(s) specified are not valid. Code: 0x8011080B" },	//	COMADMIN_E_INVALID_PARTITION
			{ com_errc::comadmin_e_svcapp_not_poolable_or_recyclable, "COM+ applications that run as NT service may not be pooled or recycled. Code: 0x8011080D" },	//	COMADMIN_E_SVCAPP_NOT_POOLABLE_OR_RECYCLABLE
			{ com_errc::comadmin_e_user_in_set, "One or more users are already assigned to a local partition set. Code: 0x8011080E" },	//	COMADMIN_E_USER_IN_SET
			{ com_errc::comadmin_e_cantrecyclelibraryapps, "Library applications may not be recycled. Code: 0x8011080F" },	//	COMADMIN_E_CANTRECYCLELIBRARYAPPS
			{ com_errc::comadmin_e_cantrecycleserviceapps, "Applications running as NT services may not be recycled. Code: 0x80110811" },	//	COMADMIN_E_CANTRECYCLESERVICEAPPS
			{ com_errc::comadmin_e_processalreadyrecycled, "The process has already been recycled. Code: 0x80110812" },	//	COMADMIN_E_PROCESSALREADYRECYCLED
			{ com_errc::comadmin_e_pausedprocessmaynotberecycled, "A paused process may not be recycled. Code: 0x80110813" },	//	COMADMIN_E_PAUSEDPROCESSMAYNOTBERECYCLED
			{ com_errc::comadmin_e_cantmakeinprocservice, "Library applications may not be NT services. Code: 0x80110814" },	//	COMADMIN_E_CANTMAKEINPROCSERVICE
			{ com_errc::comadmin_e_progidinusebyclsid, "The ProgID provided to the copy operation is invalid. The ProgID is in use by another registered CLSID. Code: 0x80110815" },	//	COMADMIN_E_PROGIDINUSEBYCLSID
			{ com_errc::comadmin_e_default_partition_not_in_set, "The partition specified as default is not a member of the partition set. Code: 0x80110816" },	//	COMADMIN_E_DEFAULT_PARTITION_NOT_IN_SET
			{ com_errc::comadmin_e_recycledprocessmaynotbepaused, "A recycled process may not be paused. Code: 0x80110817" },	//	COMADMIN_E_RECYCLEDPROCESSMAYNOTBEPAUSED
			{ com_errc::comadmin_e_partition_accessdenied, "Access to the specified partition is denied. Code: 0x80110818" },	//	COMADMIN_E_PARTITION_ACCESSDENIED
			{ com_errc::comadmin_e_partition_msi_only, "Only Application Files (*.MSI files) can be installed into partitions. Code: 0x80110819" },	//	COMADMIN_E_PARTITION_MSI_ONLY
			{ com_errc::comadmin_e_legacycomps_not_allowed_in_1_0_format, "Applications containing one or more legacy components may not be exported to 1.0 format. Code: 0x8011081A" },	//	COMADMIN_E_LEGACYCOMPS_NOT_ALLOWED_IN_1_0_FORMAT
			{ com_errc::comadmin_e_legacycomps_not_allowed_in_nonbase_partitions, "Legacy components may not exist in non-base partitions. Code: 0x8011081B" },	//	COMADMIN_E_LEGACYCOMPS_NOT_ALLOWED_IN_NONBASE_PARTITIONS
			{ com_errc::comadmin_e_comp_move_source, "A component cannot be moved (or copied) from the System Application, an application proxy or a non-changeable application. Code: 0x8011081C" },	//	COMADMIN_E_COMP_MOVE_SOURCE
			{ com_errc::comadmin_e_comp_move_dest, "A component cannot be moved (or copied) to the System Application, an application proxy or a non-changeable application. Code: 0x8011081D" },	//	COMADMIN_E_COMP_MOVE_DEST
			{ com_errc::comadmin_e_comp_move_private, "A private component cannot be moved (or copied) to a library application or to the base partition. Code: 0x8011081E" },	//	COMADMIN_E_COMP_MOVE_PRIVATE
			{ com_errc::comadmin_e_basepartition_required_in_set, "The Base Application Partition exists in all partition sets and cannot be removed. Code: 0x8011081F" },	//	COMADMIN_E_BASEPARTITION_REQUIRED_IN_SET
			{ com_errc::comadmin_e_cannot_alias_eventclass, "Alas, Event Class components cannot be aliased. Code: 0x80110820" },	//	COMADMIN_E_CANNOT_ALIAS_EVENTCLASS
			{ com_errc::comadmin_e_private_accessdenied, "Access is denied because the component is private. Code: 0x80110821" },	//	COMADMIN_E_PRIVATE_ACCESSDENIED
			{ com_errc::comadmin_e_saferinvalid, "The specified SAFER level is invalid. Code: 0x80110822" },	//	COMADMIN_E_SAFERINVALID
			{ com_errc::comadmin_e_registry_accessdenied, "The specified user cannot write to the system registry. Code: 0x80110823" },	//	COMADMIN_E_REGISTRY_ACCESSDENIED
			{ com_errc::comadmin_e_partitions_disabled, "COM+ partitions are currently disabled. Code: 0x80110824" },	//	COMADMIN_E_PARTITIONS_DISABLED
			{ com_errc::wer_s_report_debug, "Debugger was attached. Code: 0x001B0000" },	//	WER_S_REPORT_DEBUG
			{ com_errc::wer_s_report_uploaded, "Report was uploaded. Code: 0x001B0001" },	//	WER_S_REPORT_UPLOADED
			{ com_errc::wer_s_report_queued, "Report was queued. Code: 0x001B0002" },	//	WER_S_REPORT_QUEUED
			{ com_errc::wer_s_disabled, "Reporting was disabled. Code: 0x001B0003" },	//	WER_S_DISABLED
			{ com_errc::wer_s_suspended_upload, "Reporting was temporarily suspended. Code: 0x001B0004" },	//	WER_S_SUSPENDED_UPLOAD
			{ com_errc::wer_s_disabled_queue, "Report was not queued to queueing being disabled. Code: 0x001B0005" },	//	WER_S_DISABLED_QUEUE
			{ com_errc::wer_s_disabled_archive, "Report was uploaded, but not archived due to archiving being disabled. Code: 0x001B0006" },	//	WER_S_DISABLED_ARCHIVE
			{ com_errc::wer_s_report_async, "Reporting was successfully spun off as an asynchronous operation. Code: 0x001B0007" },	//	WER_S_REPORT_ASYNC
			{ com_errc::wer_s_ignore_assert_instance, "The assertion was handled. Code: 0x001B0008" },	//	WER_S_IGNORE_ASSERT_INSTANCE
			{ com_errc::wer_s_ignore_all_asserts, "The assertion was handled and added to a permanent ignore list. Code: 0x001B0009" },	//	WER_S_IGNORE_ALL_ASSERTS
			{ com_errc::wer_s_assert_continue, "The assertion was resumed as unhandled. Code: 0x001B000A" },	//	WER_S_ASSERT_CONTINUE
			{ com_errc::wer_s_throttled, "Report was throttled. Code: 0x001B000B" },	//	WER_S_THROTTLED
			{ com_errc::wer_e_crash_failure, "Crash reporting failed. Code: 0x801B8000" },	//	WER_E_CRASH_FAILURE
			{ com_errc::wer_e_canceled, "Report aborted due to user cancelation. Code: 0x801B8001" },	//	WER_E_CANCELED
			{ com_errc::wer_e_network_failure, "Report aborted due to network failure. Code: 0x801B8002" },	//	WER_E_NETWORK_FAILURE
			{ com_errc::wer_e_not_initialized, "Report not initialized. Code: 0x801B8003" },	//	WER_E_NOT_INITIALIZED
			{ com_errc::wer_e_already_reporting, "Reporting is already in progress for the specified process. Code: 0x801B8004" },	//	WER_E_ALREADY_REPORTING
			{ com_errc::wer_e_dump_throttled, "Dump not generated due to a throttle. Code: 0x801B8005" },	//	WER_E_DUMP_THROTTLED
			{ com_errc::error_flt_io_complete, "The IO was completed by a filter. Code: 0x001F0001" },	//	ERROR_FLT_IO_COMPLETE
			{ com_errc::error_flt_no_handler_defined, "A handler was not defined by the filter for this operation. Code: 0x801F0001" },	//	ERROR_FLT_NO_HANDLER_DEFINED
			{ com_errc::error_flt_context_already_defined, "A context is already defined for this object. Code: 0x801F0002" },	//	ERROR_FLT_CONTEXT_ALREADY_DEFINED
			{ com_errc::error_flt_invalid_asynchronous_request, "Asynchronous requests are not valid for this operation. Code: 0x801F0003" },	//	ERROR_FLT_INVALID_ASYNCHRONOUS_REQUEST
			{ com_errc::error_flt_disallow_fast_io, "Disallow the Fast IO path for this operation. Code: 0x801F0004" },	//	ERROR_FLT_DISALLOW_FAST_IO
			{ com_errc::error_flt_invalid_name_request, "An invalid name request was made. The name requested cannot be retrieved at this time. Code: 0x801F0005" },	//	ERROR_FLT_INVALID_NAME_REQUEST
			{ com_errc::error_flt_not_safe_to_post_operation, "Posting this operation to a worker thread for further processing is not safe at this time because it could lead to a system deadlock. Code: 0x801F0006" },	//	ERROR_FLT_NOT_SAFE_TO_POST_OPERATION
			{ com_errc::error_flt_not_initialized, "The Filter Manager was not initialized when a filter tried to register. Make sure that the Filter Manager is getting loaded as a driver. Code: 0x801F0007" },	//	ERROR_FLT_NOT_INITIALIZED
			{ com_errc::error_flt_filter_not_ready, "The filter is not ready for attachment to volumes because it has not finished initializing (FltStartFiltering has not been called). Code: 0x801F0008" },	//	ERROR_FLT_FILTER_NOT_READY
			{ com_errc::error_flt_post_operation_cleanup, "The filter must cleanup any operation specific context at this time because it is being removed from the system before the operation is completed by the lower drivers. Code: 0x801F0009" },	//	ERROR_FLT_POST_OPERATION_CLEANUP
			{ com_errc::error_flt_internal_error, "The Filter Manager had an internal error from which it cannot recover, therefore the operation has been failed. This is usually the result of a filter returning an invalid value from a pre-operation callback. Code: 0x801F000A" },	//	ERROR_FLT_INTERNAL_ERROR
			{ com_errc::error_flt_deleting_object, "The object specified for this action is in the process of being deleted, therefore the action requested cannot be completed at this time. Code: 0x801F000B" },	//	ERROR_FLT_DELETING_OBJECT
			{ com_errc::error_flt_must_be_nonpaged_pool, "Non-paged pool must be used for this type of context. Code: 0x801F000C" },	//	ERROR_FLT_MUST_BE_NONPAGED_POOL
			{ com_errc::error_flt_duplicate_entry, "A duplicate handler definition has been provided for an operation. Code: 0x801F000D" },	//	ERROR_FLT_DUPLICATE_ENTRY
			{ com_errc::error_flt_cbdq_disabled, "The callback data queue has been disabled. Code: 0x801F000E" },	//	ERROR_FLT_CBDQ_DISABLED
			{ com_errc::error_flt_do_not_attach, "Do not attach the filter to the volume at this time. Code: 0x801F000F" },	//	ERROR_FLT_DO_NOT_ATTACH
			{ com_errc::error_flt_do_not_detach, "Do not detach the filter from the volume at this time. Code: 0x801F0010" },	//	ERROR_FLT_DO_NOT_DETACH
			{ com_errc::error_flt_instance_altitude_collision, "An instance already exists at this altitude on the volume specified. Code: 0x801F0011" },	//	ERROR_FLT_INSTANCE_ALTITUDE_COLLISION
			{ com_errc::error_flt_instance_name_collision, "An instance already exists with this name on the volume specified. Code: 0x801F0012" },	//	ERROR_FLT_INSTANCE_NAME_COLLISION
			{ com_errc::error_flt_filter_not_found, "The system could not find the filter specified. Code: 0x801F0013" },	//	ERROR_FLT_FILTER_NOT_FOUND
			{ com_errc::error_flt_volume_not_found, "The system could not find the volume specified. Code: 0x801F0014" },	//	ERROR_FLT_VOLUME_NOT_FOUND
			{ com_errc::error_flt_instance_not_found, "The system could not find the instance specified. Code: 0x801F0015" },	//	ERROR_FLT_INSTANCE_NOT_FOUND
			{ com_errc::error_flt_context_allocation_not_found, "No registered context allocation definition was found for the given request. Code: 0x801F0016" },	//	ERROR_FLT_CONTEXT_ALLOCATION_NOT_FOUND
			{ com_errc::error_flt_invalid_context_registration, "An invalid parameter was specified during context registration. Code: 0x801F0017" },	//	ERROR_FLT_INVALID_CONTEXT_REGISTRATION
			{ com_errc::error_flt_name_cache_miss, "The name requested was not found in Filter Manager's name cache and could not be retrieved from the file system. Code: 0x801F0018" },	//	ERROR_FLT_NAME_CACHE_MISS
			{ com_errc::error_flt_no_device_object, "The requested device object does not exist for the given volume. Code: 0x801F0019" },	//	ERROR_FLT_NO_DEVICE_OBJECT
			{ com_errc::error_flt_volume_already_mounted, "The specified volume is already mounted. Code: 0x801F001A" },	//	ERROR_FLT_VOLUME_ALREADY_MOUNTED
			{ com_errc::error_flt_already_enlisted, "The specified Transaction Context is already enlisted in a transaction. Code: 0x801F001B" },	//	ERROR_FLT_ALREADY_ENLISTED
			{ com_errc::error_flt_context_already_linked, "The specifiec context is already attached to another object. Code: 0x801F001C" },	//	ERROR_FLT_CONTEXT_ALREADY_LINKED
			{ com_errc::error_flt_no_waiter_for_reply, "No waiter is present for the filter's reply to this message. Code: 0x801F0020" },	//	ERROR_FLT_NO_WAITER_FOR_REPLY
			{ com_errc::error_flt_registration_busy, "The filesystem database resource is in use. Registration cannot complete at this time. Code: 0x801F0023" },	//	ERROR_FLT_REGISTRATION_BUSY
			{ com_errc::error_hung_display_driver_thread, "{Display Driver Stopped Responding} The %hs display driver has stopped working normally. Save your work and reboot the system to restore full display functionality. The next time you reboot the machine a dialog will be displayed giving you a chance to report this failure to Microsoft. Code: 0x80260001" },	//	ERROR_HUNG_DISPLAY_DRIVER_THREAD
			{ com_errc::dwm_e_compositiondisabled, "{Desktop composition is disabled} The operation could not be completed because desktop composition is disabled. Code: 0x80263001" },	//	DWM_E_COMPOSITIONDISABLED
			{ com_errc::dwm_e_remoting_not_supported, "{Some desktop composition APIs are not supported while remoting} The operation is not supported while running in a remote session. Code: 0x80263002" },	//	DWM_E_REMOTING_NOT_SUPPORTED
			{ com_errc::dwm_e_no_redirection_surface_available, "{No DWM redirection surface is available} The DWM was unable to provide a redireciton surface to complete the DirectX present. Code: 0x80263003" },	//	DWM_E_NO_REDIRECTION_SURFACE_AVAILABLE
			{ com_errc::dwm_e_not_queuing_presents, "{DWM is not queuing presents for the specified window} The window specified is not currently using queued presents. Code: 0x80263004" },	//	DWM_E_NOT_QUEUING_PRESENTS
			{ com_errc::dwm_e_adapter_not_found, "{The adapter specified by the LUID is not found} DWM can not find the adapter specified by the LUID. Code: 0x80263005" },	//	DWM_E_ADAPTER_NOT_FOUND
			{ com_errc::dwm_s_gdi_redirection_surface, "{GDI redirection surface was returned} GDI redirection surface of the top level window was returned. Code: 0x00263005" },	//	DWM_S_GDI_REDIRECTION_SURFACE
			{ com_errc::dwm_e_texture_too_large, "{Redirection surface can not be created.  The size of the surface is larger than what is supported on this machine} Redirection surface can not be created.  The size of the surface is larger than what is supported on this machine. Code: 0x80263007" },	//	DWM_E_TEXTURE_TOO_LARGE
			{ com_errc::error_monitor_no_descriptor, "Monitor descriptor could not be obtained. Code: 0x80261001" },	//	ERROR_MONITOR_NO_DESCRIPTOR
			{ com_errc::error_monitor_unknown_descriptor_format, "Format of the obtained monitor descriptor is not supported by this release. Code: 0x80261002" },	//	ERROR_MONITOR_UNKNOWN_DESCRIPTOR_FORMAT
			{ com_errc::error_monitor_invalid_descriptor_checksum, "Checksum of the obtained monitor descriptor is invalid. Code: 0xC0261003" },	//	ERROR_MONITOR_INVALID_DESCRIPTOR_CHECKSUM
			{ com_errc::error_monitor_invalid_standard_timing_block, "Monitor descriptor contains an invalid standard timing block. Code: 0xC0261004" },	//	ERROR_MONITOR_INVALID_STANDARD_TIMING_BLOCK
			{ com_errc::error_monitor_wmi_datablock_registration_failed, "WMI data block registration failed for one of the MSMonitorClass WMI subclasses. Code: 0xC0261005" },	//	ERROR_MONITOR_WMI_DATABLOCK_REGISTRATION_FAILED
			{ com_errc::error_monitor_invalid_serial_number_mondsc_block, "Provided monitor descriptor block is either corrupted or does not contain monitor's detailed serial number. Code: 0xC0261006" },	//	ERROR_MONITOR_INVALID_SERIAL_NUMBER_MONDSC_BLOCK
			{ com_errc::error_monitor_invalid_user_friendly_mondsc_block, "Provided monitor descriptor block is either corrupted or does not contain monitor's user friendly name. Code: 0xC0261007" },	//	ERROR_MONITOR_INVALID_USER_FRIENDLY_MONDSC_BLOCK
			{ com_errc::error_monitor_no_more_descriptor_data, "There is no monitor descriptor data at the specified (offset, size) region. Code: 0xC0261008" },	//	ERROR_MONITOR_NO_MORE_DESCRIPTOR_DATA
			{ com_errc::error_monitor_invalid_detailed_timing_block, "Monitor descriptor contains an invalid detailed timing block. Code: 0xC0261009" },	//	ERROR_MONITOR_INVALID_DETAILED_TIMING_BLOCK
			{ com_errc::error_monitor_invalid_manufacture_date, "Monitor descriptor contains invalid manufacture date. Code: 0xC026100A" },	//	ERROR_MONITOR_INVALID_MANUFACTURE_DATE
			{ com_errc::error_graphics_not_exclusive_mode_owner, "Exclusive mode ownership is needed to create unmanaged primary allocation. Code: 0xC0262000" },	//	ERROR_GRAPHICS_NOT_EXCLUSIVE_MODE_OWNER
			{ com_errc::error_graphics_insufficient_dma_buffer, "The driver needs more DMA buffer space in order to complete the requested operation. Code: 0xC0262001" },	//	ERROR_GRAPHICS_INSUFFICIENT_DMA_BUFFER
			{ com_errc::error_graphics_invalid_display_adapter, "Specified display adapter handle is invalid. Code: 0xC0262002" },	//	ERROR_GRAPHICS_INVALID_DISPLAY_ADAPTER
			{ com_errc::error_graphics_adapter_was_reset, "Specified display adapter and all of its state has been reset. Code: 0xC0262003" },	//	ERROR_GRAPHICS_ADAPTER_WAS_RESET
			{ com_errc::error_graphics_invalid_driver_model, "The driver stack doesn't match the expected driver model. Code: 0xC0262004" },	//	ERROR_GRAPHICS_INVALID_DRIVER_MODEL
			{ com_errc::error_graphics_present_mode_changed, "Present happened but ended up into the changed desktop mode. Code: 0xC0262005" },	//	ERROR_GRAPHICS_PRESENT_MODE_CHANGED
			{ com_errc::error_graphics_present_occluded, "Nothing to present due to desktop occlusion. Code: 0xC0262006" },	//	ERROR_GRAPHICS_PRESENT_OCCLUDED
			{ com_errc::error_graphics_present_denied, "Not able to present due to denial of desktop access. Code: 0xC0262007" },	//	ERROR_GRAPHICS_PRESENT_DENIED
			{ com_errc::error_graphics_cannotcolorconvert, "Not able to present with color convertion. Code: 0xC0262008" },	//	ERROR_GRAPHICS_CANNOTCOLORCONVERT
			{ com_errc::error_graphics_driver_mismatch, "The kernel driver detected a version mismatch between it and the user mode driver. Code: 0xC0262009" },	//	ERROR_GRAPHICS_DRIVER_MISMATCH
			{ com_errc::error_graphics_partial_data_populated, "Specified buffer is not big enough to contain entire requested dataset. Partial data populated up to the size of the buffer. Caller needs to provide buffer of size as specified in the partially populated buffer's content (interface specific). Code: 0x4026200A" },	//	ERROR_GRAPHICS_PARTIAL_DATA_POPULATED
			{ com_errc::error_graphics_present_redirection_disabled, "Present redirection is disabled (desktop windowing management subsystem is off). Code: 0xC026200B" },	//	ERROR_GRAPHICS_PRESENT_REDIRECTION_DISABLED
			{ com_errc::error_graphics_present_unoccluded, "Previous exclusive VidPn source owner has released its ownership. Code: 0xC026200C" },	//	ERROR_GRAPHICS_PRESENT_UNOCCLUDED
			{ com_errc::error_graphics_windowdc_not_available, "Window DC is not available for presentation. Code: 0xC026200D" },	//	ERROR_GRAPHICS_WINDOWDC_NOT_AVAILABLE
			{ com_errc::error_graphics_windowless_present_disabled, "Windowless present is disabled (desktop windowing management subsystem is off). Code: 0xC026200E" },	//	ERROR_GRAPHICS_WINDOWLESS_PRESENT_DISABLED
			{ com_errc::error_graphics_no_video_memory, "Not enough video memory available to complete the operation. Code: 0xC0262100" },	//	ERROR_GRAPHICS_NO_VIDEO_MEMORY
			{ com_errc::error_graphics_cant_lock_memory, "Couldn't probe and lock the underlying memory of an allocation. Code: 0xC0262101" },	//	ERROR_GRAPHICS_CANT_LOCK_MEMORY
			{ com_errc::error_graphics_allocation_busy, "The allocation is currently busy. Code: 0xC0262102" },	//	ERROR_GRAPHICS_ALLOCATION_BUSY
			{ com_errc::error_graphics_too_many_references, "An object being referenced has reach the maximum reference count already and can't be reference further. Code: 0xC0262103" },	//	ERROR_GRAPHICS_TOO_MANY_REFERENCES
			{ com_errc::error_graphics_try_again_later, "A problem couldn't be solved due to some currently existing condition. The problem should be tried again later. Code: 0xC0262104" },	//	ERROR_GRAPHICS_TRY_AGAIN_LATER
			{ com_errc::error_graphics_try_again_now, "A problem couldn't be solved due to some currently existing condition. The problem should be tried again immediately. Code: 0xC0262105" },	//	ERROR_GRAPHICS_TRY_AGAIN_NOW
			{ com_errc::error_graphics_allocation_invalid, "The allocation is invalid. Code: 0xC0262106" },	//	ERROR_GRAPHICS_ALLOCATION_INVALID
			{ com_errc::error_graphics_unswizzling_aperture_unavailable, "No more unswizzling aperture are currently available. Code: 0xC0262107" },	//	ERROR_GRAPHICS_UNSWIZZLING_APERTURE_UNAVAILABLE
			{ com_errc::error_graphics_unswizzling_aperture_unsupported, "The current allocation can't be unswizzled by an aperture. Code: 0xC0262108" },	//	ERROR_GRAPHICS_UNSWIZZLING_APERTURE_UNSUPPORTED
			{ com_errc::error_graphics_cant_evict_pinned_allocation, "The request failed because a pinned allocation can't be evicted. Code: 0xC0262109" },	//	ERROR_GRAPHICS_CANT_EVICT_PINNED_ALLOCATION
			{ com_errc::error_graphics_invalid_allocation_usage, "The allocation can't be used from its current segment location for the specified operation. Code: 0xC0262110" },	//	ERROR_GRAPHICS_INVALID_ALLOCATION_USAGE
			{ com_errc::error_graphics_cant_render_locked_allocation, "A locked allocation can't be used in the current command buffer. Code: 0xC0262111" },	//	ERROR_GRAPHICS_CANT_RENDER_LOCKED_ALLOCATION
			{ com_errc::error_graphics_allocation_closed, "The allocation being referenced has been closed permanently. Code: 0xC0262112" },	//	ERROR_GRAPHICS_ALLOCATION_CLOSED
			{ com_errc::error_graphics_invalid_allocation_instance, "An invalid allocation instance is being referenced. Code: 0xC0262113" },	//	ERROR_GRAPHICS_INVALID_ALLOCATION_INSTANCE
			{ com_errc::error_graphics_invalid_allocation_handle, "An invalid allocation handle is being referenced. Code: 0xC0262114" },	//	ERROR_GRAPHICS_INVALID_ALLOCATION_HANDLE
			{ com_errc::error_graphics_wrong_allocation_device, "The allocation being referenced doesn't belong to the current device. Code: 0xC0262115" },	//	ERROR_GRAPHICS_WRONG_ALLOCATION_DEVICE
			{ com_errc::error_graphics_allocation_content_lost, "The specified allocation lost its content. Code: 0xC0262116" },	//	ERROR_GRAPHICS_ALLOCATION_CONTENT_LOST
			{ com_errc::error_graphics_gpu_exception_on_device, "GPU exception is detected on the given device. The device is not able to be scheduled. Code: 0xC0262200" },	//	ERROR_GRAPHICS_GPU_EXCEPTION_ON_DEVICE
			{ com_errc::error_graphics_skip_allocation_preparation, "Skip preparation of allocations referenced by the DMA buffer. Code: 0x40262201" },	//	ERROR_GRAPHICS_SKIP_ALLOCATION_PREPARATION
			{ com_errc::error_graphics_invalid_vidpn_topology, "Specified VidPN topology is invalid. Code: 0xC0262300" },	//	ERROR_GRAPHICS_INVALID_VIDPN_TOPOLOGY
			{ com_errc::error_graphics_vidpn_topology_not_supported, "Specified VidPN topology is valid but is not supported by this model of the display adapter. Code: 0xC0262301" },	//	ERROR_GRAPHICS_VIDPN_TOPOLOGY_NOT_SUPPORTED
			{ com_errc::error_graphics_vidpn_topology_currently_not_supported, "Specified VidPN topology is valid but is not supported by the display adapter at this time, due to current allocation of its resources. Code: 0xC0262302" },	//	ERROR_GRAPHICS_VIDPN_TOPOLOGY_CURRENTLY_NOT_SUPPORTED
			{ com_errc::error_graphics_invalid_vidpn, "Specified VidPN handle is invalid. Code: 0xC0262303" },	//	ERROR_GRAPHICS_INVALID_VIDPN
			{ com_errc::error_graphics_invalid_video_present_source, "Specified video present source is invalid. Code: 0xC0262304" },	//	ERROR_GRAPHICS_INVALID_VIDEO_PRESENT_SOURCE
			{ com_errc::error_graphics_invalid_video_present_target, "Specified video present target is invalid. Code: 0xC0262305" },	//	ERROR_GRAPHICS_INVALID_VIDEO_PRESENT_TARGET
			{ com_errc::error_graphics_vidpn_modality_not_supported, "Specified VidPN modality is not supported (e.g. at least two of the pinned modes are not cofunctional). Code: 0xC0262306" },	//	ERROR_GRAPHICS_VIDPN_MODALITY_NOT_SUPPORTED
			{ com_errc::error_graphics_mode_not_pinned, "No mode is pinned on the specified VidPN source/target. Code: 0x00262307" },	//	ERROR_GRAPHICS_MODE_NOT_PINNED
			{ com_errc::error_graphics_invalid_vidpn_sourcemodeset, "Specified VidPN source mode set is invalid. Code: 0xC0262308" },	//	ERROR_GRAPHICS_INVALID_VIDPN_SOURCEMODESET
			{ com_errc::error_graphics_invalid_vidpn_targetmodeset, "Specified VidPN target mode set is invalid. Code: 0xC0262309" },	//	ERROR_GRAPHICS_INVALID_VIDPN_TARGETMODESET
			{ com_errc::error_graphics_invalid_frequency, "Specified video signal frequency is invalid. Code: 0xC026230A" },	//	ERROR_GRAPHICS_INVALID_FREQUENCY
			{ com_errc::error_graphics_invalid_active_region, "Specified video signal active region is invalid. Code: 0xC026230B" },	//	ERROR_GRAPHICS_INVALID_ACTIVE_REGION
			{ com_errc::error_graphics_invalid_total_region, "Specified video signal total region is invalid. Code: 0xC026230C" },	//	ERROR_GRAPHICS_INVALID_TOTAL_REGION
			{ com_errc::error_graphics_invalid_video_present_source_mode, "Specified video present source mode is invalid. Code: 0xC0262310" },	//	ERROR_GRAPHICS_INVALID_VIDEO_PRESENT_SOURCE_MODE
			{ com_errc::error_graphics_invalid_video_present_target_mode, "Specified video present target mode is invalid. Code: 0xC0262311" },	//	ERROR_GRAPHICS_INVALID_VIDEO_PRESENT_TARGET_MODE
			{ com_errc::error_graphics_pinned_mode_must_remain_in_set, "Pinned mode must remain in the set on VidPN's cofunctional modality enumeration. Code: 0xC0262312" },	//	ERROR_GRAPHICS_PINNED_MODE_MUST_REMAIN_IN_SET
			{ com_errc::error_graphics_path_already_in_topology, "Specified video present path is already in VidPN's topology. Code: 0xC0262313" },	//	ERROR_GRAPHICS_PATH_ALREADY_IN_TOPOLOGY
			{ com_errc::error_graphics_mode_already_in_modeset, "Specified mode is already in the mode set. Code: 0xC0262314" },	//	ERROR_GRAPHICS_MODE_ALREADY_IN_MODESET
			{ com_errc::error_graphics_invalid_videopresentsourceset, "Specified video present source set is invalid. Code: 0xC0262315" },	//	ERROR_GRAPHICS_INVALID_VIDEOPRESENTSOURCESET
			{ com_errc::error_graphics_invalid_videopresenttargetset, "Specified video present target set is invalid. Code: 0xC0262316" },	//	ERROR_GRAPHICS_INVALID_VIDEOPRESENTTARGETSET
			{ com_errc::error_graphics_source_already_in_set, "Specified video present source is already in the video present source set. Code: 0xC0262317" },	//	ERROR_GRAPHICS_SOURCE_ALREADY_IN_SET
			{ com_errc::error_graphics_target_already_in_set, "Specified video present target is already in the video present target set. Code: 0xC0262318" },	//	ERROR_GRAPHICS_TARGET_ALREADY_IN_SET
			{ com_errc::error_graphics_invalid_vidpn_present_path, "Specified VidPN present path is invalid. Code: 0xC0262319" },	//	ERROR_GRAPHICS_INVALID_VIDPN_PRESENT_PATH
			{ com_errc::error_graphics_no_recommended_vidpn_topology, "Miniport has no recommendation for augmentation of the specified VidPN's topology. Code: 0xC026231A" },	//	ERROR_GRAPHICS_NO_RECOMMENDED_VIDPN_TOPOLOGY
			{ com_errc::error_graphics_invalid_monitor_frequencyrangeset, "Specified monitor frequency range set is invalid. Code: 0xC026231B" },	//	ERROR_GRAPHICS_INVALID_MONITOR_FREQUENCYRANGESET
			{ com_errc::error_graphics_invalid_monitor_frequencyrange, "Specified monitor frequency range is invalid. Code: 0xC026231C" },	//	ERROR_GRAPHICS_INVALID_MONITOR_FREQUENCYRANGE
			{ com_errc::error_graphics_frequencyrange_not_in_set, "Specified frequency range is not in the specified monitor frequency range set. Code: 0xC026231D" },	//	ERROR_GRAPHICS_FREQUENCYRANGE_NOT_IN_SET
			{ com_errc::error_graphics_no_preferred_mode, "Specified mode set does not specify preference for one of its modes. Code: 0x0026231E" },	//	ERROR_GRAPHICS_NO_PREFERRED_MODE
			{ com_errc::error_graphics_frequencyrange_already_in_set, "Specified frequency range is already in the specified monitor frequency range set. Code: 0xC026231F" },	//	ERROR_GRAPHICS_FREQUENCYRANGE_ALREADY_IN_SET
			{ com_errc::error_graphics_stale_modeset, "Specified mode set is stale. Please reacquire the new mode set. Code: 0xC0262320" },	//	ERROR_GRAPHICS_STALE_MODESET
			{ com_errc::error_graphics_invalid_monitor_sourcemodeset, "Specified monitor source mode set is invalid. Code: 0xC0262321" },	//	ERROR_GRAPHICS_INVALID_MONITOR_SOURCEMODESET
			{ com_errc::error_graphics_invalid_monitor_source_mode, "Specified monitor source mode is invalid. Code: 0xC0262322" },	//	ERROR_GRAPHICS_INVALID_MONITOR_SOURCE_MODE
			{ com_errc::error_graphics_no_recommended_functional_vidpn, "Miniport does not have any recommendation regarding the request to provide a functional VidPN given the current display adapter configuration. Code: 0xC0262323" },	//	ERROR_GRAPHICS_NO_RECOMMENDED_FUNCTIONAL_VIDPN
			{ com_errc::error_graphics_mode_id_must_be_unique, "ID of the specified mode is already used by another mode in the set. Code: 0xC0262324" },	//	ERROR_GRAPHICS_MODE_ID_MUST_BE_UNIQUE
			{ com_errc::error_graphics_empty_adapter_monitor_mode_support_intersection, "System failed to determine a mode that is supported by both the display adapter and the monitor connected to it. Code: 0xC0262325" },	//	ERROR_GRAPHICS_EMPTY_ADAPTER_MONITOR_MODE_SUPPORT_INTERSECTION
			{ com_errc::error_graphics_video_present_targets_less_than_sources, "Number of video present targets must be greater than or equal to the number of video present sources. Code: 0xC0262326" },	//	ERROR_GRAPHICS_VIDEO_PRESENT_TARGETS_LESS_THAN_SOURCES
			{ com_errc::error_graphics_path_not_in_topology, "Specified present path is not in VidPN's topology. Code: 0xC0262327" },	//	ERROR_GRAPHICS_PATH_NOT_IN_TOPOLOGY
			{ com_errc::error_graphics_adapter_must_have_at_least_one_source, "Display adapter must have at least one video present source. Code: 0xC0262328" },	//	ERROR_GRAPHICS_ADAPTER_MUST_HAVE_AT_LEAST_ONE_SOURCE
			{ com_errc::error_graphics_adapter_must_have_at_least_one_target, "Display adapter must have at least one video present target. Code: 0xC0262329" },	//	ERROR_GRAPHICS_ADAPTER_MUST_HAVE_AT_LEAST_ONE_TARGET
			{ com_errc::error_graphics_invalid_monitordescriptorset, "Specified monitor descriptor set is invalid. Code: 0xC026232A" },	//	ERROR_GRAPHICS_INVALID_MONITORDESCRIPTORSET
			{ com_errc::error_graphics_invalid_monitordescriptor, "Specified monitor descriptor is invalid. Code: 0xC026232B" },	//	ERROR_GRAPHICS_INVALID_MONITORDESCRIPTOR
			{ com_errc::error_graphics_monitordescriptor_not_in_set, "Specified descriptor is not in the specified monitor descriptor set. Code: 0xC026232C" },	//	ERROR_GRAPHICS_MONITORDESCRIPTOR_NOT_IN_SET
			{ com_errc::error_graphics_monitordescriptor_already_in_set, "Specified descriptor is already in the specified monitor descriptor set. Code: 0xC026232D" },	//	ERROR_GRAPHICS_MONITORDESCRIPTOR_ALREADY_IN_SET
			{ com_errc::error_graphics_monitordescriptor_id_must_be_unique, "ID of the specified monitor descriptor is already used by another descriptor in the set. Code: 0xC026232E" },	//	ERROR_GRAPHICS_MONITORDESCRIPTOR_ID_MUST_BE_UNIQUE
			{ com_errc::error_graphics_invalid_vidpn_target_subset_type, "Specified video present target subset type is invalid. Code: 0xC026232F" },	//	ERROR_GRAPHICS_INVALID_VIDPN_TARGET_SUBSET_TYPE
			{ com_errc::error_graphics_resources_not_related, "Two or more of the specified resources are not related to each other, as defined by the interface semantics. Code: 0xC0262330" },	//	ERROR_GRAPHICS_RESOURCES_NOT_RELATED
			{ com_errc::error_graphics_source_id_must_be_unique, "ID of the specified video present source is already used by another source in the set. Code: 0xC0262331" },	//	ERROR_GRAPHICS_SOURCE_ID_MUST_BE_UNIQUE
			{ com_errc::error_graphics_target_id_must_be_unique, "ID of the specified video present target is already used by another target in the set. Code: 0xC0262332" },	//	ERROR_GRAPHICS_TARGET_ID_MUST_BE_UNIQUE
			{ com_errc::error_graphics_no_available_vidpn_target, "Specified VidPN source cannot be used because there is no available VidPN target to connect it to. Code: 0xC0262333" },	//	ERROR_GRAPHICS_NO_AVAILABLE_VIDPN_TARGET
			{ com_errc::error_graphics_monitor_could_not_be_associated_with_adapter, "Newly arrived monitor could not be associated with a display adapter. Code: 0xC0262334" },	//	ERROR_GRAPHICS_MONITOR_COULD_NOT_BE_ASSOCIATED_WITH_ADAPTER
			{ com_errc::error_graphics_no_vidpnmgr, "Display adapter in question does not have an associated VidPN manager. Code: 0xC0262335" },	//	ERROR_GRAPHICS_NO_VIDPNMGR
			{ com_errc::error_graphics_no_active_vidpn, "VidPN manager of the display adapter in question does not have an active VidPN. Code: 0xC0262336" },	//	ERROR_GRAPHICS_NO_ACTIVE_VIDPN
			{ com_errc::error_graphics_stale_vidpn_topology, "Specified VidPN topology is stale. Please reacquire the new topology. Code: 0xC0262337" },	//	ERROR_GRAPHICS_STALE_VIDPN_TOPOLOGY
			{ com_errc::error_graphics_monitor_not_connected, "There is no monitor connected on the specified video present target. Code: 0xC0262338" },	//	ERROR_GRAPHICS_MONITOR_NOT_CONNECTED
			{ com_errc::error_graphics_source_not_in_topology, "Specified source is not part of the specified VidPN's topology. Code: 0xC0262339" },	//	ERROR_GRAPHICS_SOURCE_NOT_IN_TOPOLOGY
			{ com_errc::error_graphics_invalid_primarysurface_size, "Specified primary surface size is invalid. Code: 0xC026233A" },	//	ERROR_GRAPHICS_INVALID_PRIMARYSURFACE_SIZE
			{ com_errc::error_graphics_invalid_visibleregion_size, "Specified visible region size is invalid. Code: 0xC026233B" },	//	ERROR_GRAPHICS_INVALID_VISIBLEREGION_SIZE
			{ com_errc::error_graphics_invalid_stride, "Specified stride is invalid. Code: 0xC026233C" },	//	ERROR_GRAPHICS_INVALID_STRIDE
			{ com_errc::error_graphics_invalid_pixelformat, "Specified pixel format is invalid. Code: 0xC026233D" },	//	ERROR_GRAPHICS_INVALID_PIXELFORMAT
			{ com_errc::error_graphics_invalid_colorbasis, "Specified color basis is invalid. Code: 0xC026233E" },	//	ERROR_GRAPHICS_INVALID_COLORBASIS
			{ com_errc::error_graphics_invalid_pixelvalueaccessmode, "Specified pixel value access mode is invalid. Code: 0xC026233F" },	//	ERROR_GRAPHICS_INVALID_PIXELVALUEACCESSMODE
			{ com_errc::error_graphics_target_not_in_topology, "Specified target is not part of the specified VidPN's topology. Code: 0xC0262340" },	//	ERROR_GRAPHICS_TARGET_NOT_IN_TOPOLOGY
			{ com_errc::error_graphics_no_display_mode_management_support, "Failed to acquire display mode management interface. Code: 0xC0262341" },	//	ERROR_GRAPHICS_NO_DISPLAY_MODE_MANAGEMENT_SUPPORT
			{ com_errc::error_graphics_vidpn_source_in_use, "Specified VidPN source is already owned by a DMM client and cannot be used until that client releases it. Code: 0xC0262342" },	//	ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE
			{ com_errc::error_graphics_cant_access_active_vidpn, "Specified VidPN is active and cannot be accessed. Code: 0xC0262343" },	//	ERROR_GRAPHICS_CANT_ACCESS_ACTIVE_VIDPN
			{ com_errc::error_graphics_invalid_path_importance_ordinal, "Specified VidPN present path importance ordinal is invalid. Code: 0xC0262344" },	//	ERROR_GRAPHICS_INVALID_PATH_IMPORTANCE_ORDINAL
			{ com_errc::error_graphics_invalid_path_content_geometry_transformation, "Specified VidPN present path content geometry transformation is invalid. Code: 0xC0262345" },	//	ERROR_GRAPHICS_INVALID_PATH_CONTENT_GEOMETRY_TRANSFORMATION
			{ com_errc::error_graphics_path_content_geometry_transformation_not_supported, "Specified content geometry transformation is not supported on the respective VidPN present path. Code: 0xC0262346" },	//	ERROR_GRAPHICS_PATH_CONTENT_GEOMETRY_TRANSFORMATION_NOT_SUPPORTED
			{ com_errc::error_graphics_invalid_gamma_ramp, "Specified gamma ramp is invalid. Code: 0xC0262347" },	//	ERROR_GRAPHICS_INVALID_GAMMA_RAMP
			{ com_errc::error_graphics_gamma_ramp_not_supported, "Specified gamma ramp is not supported on the respective VidPN present path. Code: 0xC0262348" },	//	ERROR_GRAPHICS_GAMMA_RAMP_NOT_SUPPORTED
			{ com_errc::error_graphics_multisampling_not_supported, "Multi-sampling is not supported on the respective VidPN present path. Code: 0xC0262349" },	//	ERROR_GRAPHICS_MULTISAMPLING_NOT_SUPPORTED
			{ com_errc::error_graphics_mode_not_in_modeset, "Specified mode is not in the specified mode set. Code: 0xC026234A" },	//	ERROR_GRAPHICS_MODE_NOT_IN_MODESET
			{ com_errc::error_graphics_dataset_is_empty, "Specified data set (e.g. mode set, frequency range set, descriptor set, topology, etc.) is empty. Code: 0x0026234B" },	//	ERROR_GRAPHICS_DATASET_IS_EMPTY
			{ com_errc::error_graphics_no_more_elements_in_dataset, "Specified data set (e.g. mode set, frequency range set, descriptor set, topology, etc.) does not contain any more elements. Code: 0x0026234C" },	//	ERROR_GRAPHICS_NO_MORE_ELEMENTS_IN_DATASET
			{ com_errc::error_graphics_invalid_vidpn_topology_recommendation_reason, "Specified VidPN topology recommendation reason is invalid. Code: 0xC026234D" },	//	ERROR_GRAPHICS_INVALID_VIDPN_TOPOLOGY_RECOMMENDATION_REASON
			{ com_errc::error_graphics_invalid_path_content_type, "Specified VidPN present path content type is invalid. Code: 0xC026234E" },	//	ERROR_GRAPHICS_INVALID_PATH_CONTENT_TYPE
			{ com_errc::error_graphics_invalid_copyprotection_type, "Specified VidPN present path copy protection type is invalid. Code: 0xC026234F" },	//	ERROR_GRAPHICS_INVALID_COPYPROTECTION_TYPE
			{ com_errc::error_graphics_unassigned_modeset_already_exists, "No more than one unassigned mode set can exist at any given time for a given VidPN source/target. Code: 0xC0262350" },	//	ERROR_GRAPHICS_UNASSIGNED_MODESET_ALREADY_EXISTS
			{ com_errc::error_graphics_path_content_geometry_transformation_not_pinned, "Specified content transformation is not pinned on the specified VidPN present path. Code: 0x00262351" },	//	ERROR_GRAPHICS_PATH_CONTENT_GEOMETRY_TRANSFORMATION_NOT_PINNED
			{ com_errc::error_graphics_invalid_scanline_ordering, "Specified scanline ordering type is invalid. Code: 0xC0262352" },	//	ERROR_GRAPHICS_INVALID_SCANLINE_ORDERING
			{ com_errc::error_graphics_topology_changes_not_allowed, "Topology changes are not allowed for the specified VidPN. Code: 0xC0262353" },	//	ERROR_GRAPHICS_TOPOLOGY_CHANGES_NOT_ALLOWED
			{ com_errc::error_graphics_no_available_importance_ordinals, "All available importance ordinals are already used in specified topology. Code: 0xC0262354" },	//	ERROR_GRAPHICS_NO_AVAILABLE_IMPORTANCE_ORDINALS
			{ com_errc::error_graphics_incompatible_private_format, "Specified primary surface has a different private format attribute than the current primary surface. Code: 0xC0262355" },	//	ERROR_GRAPHICS_INCOMPATIBLE_PRIVATE_FORMAT
			{ com_errc::error_graphics_invalid_mode_pruning_algorithm, "Specified mode pruning algorithm is invalid. Code: 0xC0262356" },	//	ERROR_GRAPHICS_INVALID_MODE_PRUNING_ALGORITHM
			{ com_errc::error_graphics_invalid_monitor_capability_origin, "Specified monitor capability origin is invalid. Code: 0xC0262357" },	//	ERROR_GRAPHICS_INVALID_MONITOR_CAPABILITY_ORIGIN
			{ com_errc::error_graphics_invalid_monitor_frequencyrange_constraint, "Specified monitor frequency range constraint is invalid. Code: 0xC0262358" },	//	ERROR_GRAPHICS_INVALID_MONITOR_FREQUENCYRANGE_CONSTRAINT
			{ com_errc::error_graphics_max_num_paths_reached, "Maximum supported number of present paths has been reached. Code: 0xC0262359" },	//	ERROR_GRAPHICS_MAX_NUM_PATHS_REACHED
			{ com_errc::error_graphics_cancel_vidpn_topology_augmentation, "Miniport requested that augmentation be cancelled for the specified source of the specified VidPN's topology. Code: 0xC026235A" },	//	ERROR_GRAPHICS_CANCEL_VIDPN_TOPOLOGY_AUGMENTATION
			{ com_errc::error_graphics_invalid_client_type, "Specified client type was not recognized. Code: 0xC026235B" },	//	ERROR_GRAPHICS_INVALID_CLIENT_TYPE
			{ com_errc::error_graphics_clientvidpn_not_set, "Client VidPN is not set on this adapter (e.g. no user mode initiated mode changes took place on this adapter yet). Code: 0xC026235C" },	//	ERROR_GRAPHICS_CLIENTVIDPN_NOT_SET
			{ com_errc::error_graphics_specified_child_already_connected, "Specified display adapter child device already has an external device connected to it. Code: 0xC0262400" },	//	ERROR_GRAPHICS_SPECIFIED_CHILD_ALREADY_CONNECTED
			{ com_errc::error_graphics_child_descriptor_not_supported, "Specified display adapter child device does not support descriptor exposure. Code: 0xC0262401" },	//	ERROR_GRAPHICS_CHILD_DESCRIPTOR_NOT_SUPPORTED
			{ com_errc::error_graphics_unknown_child_status, "Child device presence was not reliably detected. Code: 0x4026242F" },	//	ERROR_GRAPHICS_UNKNOWN_CHILD_STATUS
			{ com_errc::error_graphics_not_a_linked_adapter, "The display adapter is not linked to any other adapters. Code: 0xC0262430" },	//	ERROR_GRAPHICS_NOT_A_LINKED_ADAPTER
			{ com_errc::error_graphics_leadlink_not_enumerated, "Lead adapter in a linked configuration was not enumerated yet. Code: 0xC0262431" },	//	ERROR_GRAPHICS_LEADLINK_NOT_ENUMERATED
			{ com_errc::error_graphics_chainlinks_not_enumerated, "Some chain adapters in a linked configuration were not enumerated yet. Code: 0xC0262432" },	//	ERROR_GRAPHICS_CHAINLINKS_NOT_ENUMERATED
			{ com_errc::error_graphics_adapter_chain_not_ready, "The chain of linked adapters is not ready to start because of an unknown failure. Code: 0xC0262433" },	//	ERROR_GRAPHICS_ADAPTER_CHAIN_NOT_READY
			{ com_errc::error_graphics_chainlinks_not_started, "An attempt was made to start a lead link display adapter when the chain links were not started yet. Code: 0xC0262434" },	//	ERROR_GRAPHICS_CHAINLINKS_NOT_STARTED
			{ com_errc::error_graphics_chainlinks_not_powered_on, "An attempt was made to power up a lead link display adapter when the chain links were powered down. Code: 0xC0262435" },	//	ERROR_GRAPHICS_CHAINLINKS_NOT_POWERED_ON
			{ com_errc::error_graphics_inconsistent_device_link_state, "The adapter link was found to be in an inconsistent state. Not all adapters are in an expected PNP/Power state. Code: 0xC0262436" },	//	ERROR_GRAPHICS_INCONSISTENT_DEVICE_LINK_STATE
			{ com_errc::error_graphics_leadlink_start_deferred, "Starting the leadlink adapter has been deferred temporarily. Code: 0x40262437" },	//	ERROR_GRAPHICS_LEADLINK_START_DEFERRED
			{ com_errc::error_graphics_not_post_device_driver, "The driver trying to start is not the same as the driver for the POSTed display adapter. Code: 0xC0262438" },	//	ERROR_GRAPHICS_NOT_POST_DEVICE_DRIVER
			{ com_errc::error_graphics_polling_too_frequently, "The display adapter is being polled for children too frequently at the same polling level. Code: 0x40262439" },	//	ERROR_GRAPHICS_POLLING_TOO_FREQUENTLY
			{ com_errc::error_graphics_start_deferred, "Starting the adapter has been deferred temporarily. Code: 0x4026243A" },	//	ERROR_GRAPHICS_START_DEFERRED
			{ com_errc::error_graphics_adapter_access_not_excluded, "An operation is being attempted that requires the display adapter to be in a quiescent state. Code: 0xC026243B" },	//	ERROR_GRAPHICS_ADAPTER_ACCESS_NOT_EXCLUDED
			{ com_errc::error_graphics_opm_not_supported, "The driver does not support OPM. Code: 0xC0262500" },	//	ERROR_GRAPHICS_OPM_NOT_SUPPORTED
			{ com_errc::error_graphics_copp_not_supported, "The driver does not support COPP. Code: 0xC0262501" },	//	ERROR_GRAPHICS_COPP_NOT_SUPPORTED
			{ com_errc::error_graphics_uab_not_supported, "The driver does not support UAB. Code: 0xC0262502" },	//	ERROR_GRAPHICS_UAB_NOT_SUPPORTED
			{ com_errc::error_graphics_opm_invalid_encrypted_parameters, "The specified encrypted parameters are invalid. Code: 0xC0262503" },	//	ERROR_GRAPHICS_OPM_INVALID_ENCRYPTED_PARAMETERS
			{ com_errc::error_graphics_opm_no_video_outputs_exist, "The GDI display device passed to this function does not have any active video outputs. Code: 0xC0262505" },	//	ERROR_GRAPHICS_OPM_NO_VIDEO_OUTPUTS_EXIST
			{ com_errc::error_graphics_opm_internal_error, "An internal error caused this operation to fail. Code: 0xC026250B" },	//	ERROR_GRAPHICS_OPM_INTERNAL_ERROR
			{ com_errc::error_graphics_opm_invalid_handle, "The function failed because the caller passed in an invalid OPM user mode handle. Code: 0xC026250C" },	//	ERROR_GRAPHICS_OPM_INVALID_HANDLE
			{ com_errc::error_graphics_pvp_invalid_certificate_length, "A certificate could not be returned because the certificate buffer passed to the function was too small. Code: 0xC026250E" },	//	ERROR_GRAPHICS_PVP_INVALID_CERTIFICATE_LENGTH
			{ com_errc::error_graphics_opm_spanning_mode_enabled, "A video output could not be created because the frame buffer is in spanning mode. Code: 0xC026250F" },	//	ERROR_GRAPHICS_OPM_SPANNING_MODE_ENABLED
			{ com_errc::error_graphics_opm_theater_mode_enabled, "A video output could not be created because the frame buffer is in theater mode. Code: 0xC0262510" },	//	ERROR_GRAPHICS_OPM_THEATER_MODE_ENABLED
			{ com_errc::error_graphics_pvp_hfs_failed, "The function failed because the display adapter's Hardware Functionality Scan failed to validate the graphics hardware. Code: 0xC0262511" },	//	ERROR_GRAPHICS_PVP_HFS_FAILED
			{ com_errc::error_graphics_opm_invalid_srm, "The HDCP System Renewability Message passed to this function did not comply with section 5 of the HDCP 1.1 specification. Code: 0xC0262512" },	//	ERROR_GRAPHICS_OPM_INVALID_SRM
			{ com_errc::error_graphics_opm_output_does_not_support_hdcp, "The video output cannot enable the High-bandwidth Digital Content Protection (HDCP) System because it does not support HDCP. Code: 0xC0262513" },	//	ERROR_GRAPHICS_OPM_OUTPUT_DOES_NOT_SUPPORT_HDCP
			{ com_errc::error_graphics_opm_output_does_not_support_acp, "The video output cannot enable Analogue Copy Protection (ACP) because it does not support ACP. Code: 0xC0262514" },	//	ERROR_GRAPHICS_OPM_OUTPUT_DOES_NOT_SUPPORT_ACP
			{ com_errc::error_graphics_opm_output_does_not_support_cgmsa, "The video output cannot enable the Content Generation Management System Analogue (CGMS-A) protection technology because it does not support CGMS-A. Code: 0xC0262515" },	//	ERROR_GRAPHICS_OPM_OUTPUT_DOES_NOT_SUPPORT_CGMSA
			{ com_errc::error_graphics_opm_hdcp_srm_never_set, "The IOPMVideoOutput::GetInformation method cannot return the version of the SRM being used because the application never successfully passed an SRM to the video output. Code: 0xC0262516" },	//	ERROR_GRAPHICS_OPM_HDCP_SRM_NEVER_SET
			{ com_errc::error_graphics_opm_resolution_too_high, "The IOPMVideoOutput::Configure method cannot enable the specified output protection technology because the output's screen resolution is too high. Code: 0xC0262517" },	//	ERROR_GRAPHICS_OPM_RESOLUTION_TOO_HIGH
			{ com_errc::error_graphics_opm_all_hdcp_hardware_already_in_use, "The IOPMVideoOutput::Configure method cannot enable HDCP because the display adapter's HDCP hardware is already being used by other physical outputs. Code: 0xC0262518" },	//	ERROR_GRAPHICS_OPM_ALL_HDCP_HARDWARE_ALREADY_IN_USE
			{ com_errc::error_graphics_opm_video_output_no_longer_exists, "The operating system asynchronously destroyed this OPM video output because the operating system's state changed. This error typically occurs because the monitor PDO associated with this video output was removed, the monitor PDO associated with this video output was stopped, the video output's session became a non-console session or the video output's desktop became an inactive desktop. Code: 0xC026251A" },	//	ERROR_GRAPHICS_OPM_VIDEO_OUTPUT_NO_LONGER_EXISTS
			{ com_errc::error_graphics_opm_session_type_change_in_progress, "The method failed because the session is changing its type. No IOPMVideoOutput methods can be called when a session is changing its type. There are currently three types of sessions: console, disconnected and remote. Code: 0xC026251B" },	//	ERROR_GRAPHICS_OPM_SESSION_TYPE_CHANGE_IN_PROGRESS
			{ com_errc::error_graphics_opm_video_output_does_not_have_copp_semantics, "Either the IOPMVideoOutput::COPPCompatibleGetInformation, IOPMVideoOutput::GetInformation, or IOPMVideoOutput::Configure method failed. This error is returned when the caller tries to use a COPP specific command while the video output has OPM semantics only. Code: 0xC026251C" },	//	ERROR_GRAPHICS_OPM_VIDEO_OUTPUT_DOES_NOT_HAVE_COPP_SEMANTICS
			{ com_errc::error_graphics_opm_invalid_information_request, "The IOPMVideoOutput::GetInformation and IOPMVideoOutput::COPPCompatibleGetInformation methods return this error if the passed in sequence number is not the expected sequence number or the passed in OMAC value is invalid. Code: 0xC026251D" },	//	ERROR_GRAPHICS_OPM_INVALID_INFORMATION_REQUEST
			{ com_errc::error_graphics_opm_driver_internal_error, "The method failed because an unexpected error occurred inside of a display driver. Code: 0xC026251E" },	//	ERROR_GRAPHICS_OPM_DRIVER_INTERNAL_ERROR
			{ com_errc::error_graphics_opm_video_output_does_not_have_opm_semantics, "Either the IOPMVideoOutput::COPPCompatibleGetInformation, IOPMVideoOutput::GetInformation, or IOPMVideoOutput::Configure method failed. This error is returned when the caller tries to use an OPM specific command while the video output has COPP semantics only. Code: 0xC026251F" },	//	ERROR_GRAPHICS_OPM_VIDEO_OUTPUT_DOES_NOT_HAVE_OPM_SEMANTICS
			{ com_errc::error_graphics_opm_signaling_not_supported, "The IOPMVideoOutput::COPPCompatibleGetInformation or IOPMVideoOutput::Configure method failed because the display driver does not support the OPM_GET_ACP_AND_CGMSA_SIGNALING and OPM_SET_ACP_AND_CGMSA_SIGNALING GUIDs. Code: 0xC0262520" },	//	ERROR_GRAPHICS_OPM_SIGNALING_NOT_SUPPORTED
			{ com_errc::error_graphics_opm_invalid_configuration_request, "The IOPMVideoOutput::Configure function returns this error code if the passed in sequence number is not the expected sequence number or the passed in OMAC value is invalid. Code: 0xC0262521" },	//	ERROR_GRAPHICS_OPM_INVALID_CONFIGURATION_REQUEST
			{ com_errc::error_graphics_i2c_not_supported, "The monitor connected to the specified video output does not have an I2C bus. Code: 0xC0262580" },	//	ERROR_GRAPHICS_I2C_NOT_SUPPORTED
			{ com_errc::error_graphics_i2c_device_does_not_exist, "No device on the I2C bus has the specified address. Code: 0xC0262581" },	//	ERROR_GRAPHICS_I2C_DEVICE_DOES_NOT_EXIST
			{ com_errc::error_graphics_i2c_error_transmitting_data, "An error occurred while transmitting data to the device on the I2C bus. Code: 0xC0262582" },	//	ERROR_GRAPHICS_I2C_ERROR_TRANSMITTING_DATA
			{ com_errc::error_graphics_i2c_error_receiving_data, "An error occurred while receiving data from the device on the I2C bus. Code: 0xC0262583" },	//	ERROR_GRAPHICS_I2C_ERROR_RECEIVING_DATA
			{ com_errc::error_graphics_ddcci_vcp_not_supported, "The monitor does not support the specified VCP code. Code: 0xC0262584" },	//	ERROR_GRAPHICS_DDCCI_VCP_NOT_SUPPORTED
			{ com_errc::error_graphics_ddcci_invalid_data, "The data received from the monitor is invalid. Code: 0xC0262585" },	//	ERROR_GRAPHICS_DDCCI_INVALID_DATA
			{ com_errc::error_graphics_ddcci_monitor_returned_invalid_timing_status_byte, "The function failed because a monitor returned an invalid Timing Status byte when the operating system used the DDC/CI Get Timing Report & Timing Message command to get a timing report from a monitor. Code: 0xC0262586" },	//	ERROR_GRAPHICS_DDCCI_MONITOR_RETURNED_INVALID_TIMING_STATUS_BYTE
			{ com_errc::error_graphics_mca_invalid_capabilities_string, "The monitor returned a DDC/CI capabilities string which did not comply with the ACCESS.bus 3.0, DDC/CI 1.1, or MCCS 2 Revision 1 specification. Code: 0xC0262587" },	//	ERROR_GRAPHICS_MCA_INVALID_CAPABILITIES_STRING
			{ com_errc::error_graphics_mca_internal_error, "An internal Monitor Configuration API error occurred. Code: 0xC0262588" },	//	ERROR_GRAPHICS_MCA_INTERNAL_ERROR
			{ com_errc::error_graphics_ddcci_invalid_message_command, "An operation failed because a DDC/CI message had an invalid value in its command field. Code: 0xC0262589" },	//	ERROR_GRAPHICS_DDCCI_INVALID_MESSAGE_COMMAND
			{ com_errc::error_graphics_ddcci_invalid_message_length, "An error occurred because the field length of a DDC/CI message contained an invalid value. Code: 0xC026258A" },	//	ERROR_GRAPHICS_DDCCI_INVALID_MESSAGE_LENGTH
			{ com_errc::error_graphics_ddcci_invalid_message_checksum, "An error occurred because the checksum field in a DDC/CI message did not match the message's computed checksum value. This error implies that the data was corrupted while it was being transmitted from a monitor to a computer. Code: 0xC026258B" },	//	ERROR_GRAPHICS_DDCCI_INVALID_MESSAGE_CHECKSUM
			{ com_errc::error_graphics_invalid_physical_monitor_handle, "This function failed because an invalid monitor handle was passed to it. Code: 0xC026258C" },	//	ERROR_GRAPHICS_INVALID_PHYSICAL_MONITOR_HANDLE
			{ com_errc::error_graphics_monitor_no_longer_exists, "The operating system asynchronously destroyed the monitor which corresponds to this handle because the operating system's state changed. This error typically occurs because the monitor PDO associated with this handle was removed, the monitor PDO associated with this handle was stopped, or a display mode change occurred. A display mode change occurs when windows sends a WM_DISPLAYCHANGE windows message to applications. Code: 0xC026258D" },	//	ERROR_GRAPHICS_MONITOR_NO_LONGER_EXISTS
			{ com_errc::error_graphics_ddcci_current_current_value_greater_than_maximum_value, "A continuous VCP code's current value is greater than its maximum value. This error code indicates that a monitor returned an invalid value. Code: 0xC02625D8" },	//	ERROR_GRAPHICS_DDCCI_CURRENT_CURRENT_VALUE_GREATER_THAN_MAXIMUM_VALUE
			{ com_errc::error_graphics_mca_invalid_vcp_version, "The monitor's VCP Version (0xDF) VCP code returned an invalid version value. Code: 0xC02625D9" },	//	ERROR_GRAPHICS_MCA_INVALID_VCP_VERSION
			{ com_errc::error_graphics_mca_monitor_violates_mccs_specification, "The monitor does not comply with the MCCS specification it claims to support. Code: 0xC02625DA" },	//	ERROR_GRAPHICS_MCA_MONITOR_VIOLATES_MCCS_SPECIFICATION
			{ com_errc::error_graphics_mca_mccs_version_mismatch, "The MCCS version in a monitor's mccs_ver capability does not match the MCCS version the monitor reports when the VCP Version (0xDF) VCP code is used. Code: 0xC02625DB" },	//	ERROR_GRAPHICS_MCA_MCCS_VERSION_MISMATCH
			{ com_errc::error_graphics_mca_unsupported_mccs_version, "The Monitor Configuration API only works with monitors which support the MCCS 1.0 specification, MCCS 2.0 specification or the MCCS 2.0 Revision 1 specification. Code: 0xC02625DC" },	//	ERROR_GRAPHICS_MCA_UNSUPPORTED_MCCS_VERSION
			{ com_errc::error_graphics_mca_invalid_technology_type_returned, "The monitor returned an invalid monitor technology type. CRT, Plasma and LCD (TFT) are examples of monitor technology types. This error implies that the monitor violated the MCCS 2.0 or MCCS 2.0 Revision 1 specification. Code: 0xC02625DE" },	//	ERROR_GRAPHICS_MCA_INVALID_TECHNOLOGY_TYPE_RETURNED
			{ com_errc::error_graphics_mca_unsupported_color_temperature, "SetMonitorColorTemperature()'s caller passed a color temperature to it which the current monitor did not support. This error implies that the monitor violated the MCCS 2.0 or MCCS 2.0 Revision 1 specification. Code: 0xC02625DF" },	//	ERROR_GRAPHICS_MCA_UNSUPPORTED_COLOR_TEMPERATURE
			{ com_errc::error_graphics_only_console_session_supported, "This function can only be used if a program is running in the local console session. It cannot be used if the program is running on a remote desktop session or on a terminal server session. Code: 0xC02625E0" },	//	ERROR_GRAPHICS_ONLY_CONSOLE_SESSION_SUPPORTED
			{ com_errc::error_graphics_no_display_device_corresponds_to_name, "This function cannot find an actual GDI display device which corresponds to the specified GDI display device name. Code: 0xC02625E1" },	//	ERROR_GRAPHICS_NO_DISPLAY_DEVICE_CORRESPONDS_TO_NAME
			{ com_errc::error_graphics_display_device_not_attached_to_desktop, "The function failed because the specified GDI display device was not attached to the Windows desktop. Code: 0xC02625E2" },	//	ERROR_GRAPHICS_DISPLAY_DEVICE_NOT_ATTACHED_TO_DESKTOP
			{ com_errc::error_graphics_mirroring_devices_not_supported, "This function does not support GDI mirroring display devices because GDI mirroring display devices do not have any physical monitors associated with them. Code: 0xC02625E3" },	//	ERROR_GRAPHICS_MIRRORING_DEVICES_NOT_SUPPORTED
			{ com_errc::error_graphics_invalid_pointer, "The function failed because an invalid pointer parameter was passed to it. A pointer parameter is invalid if it is NULL, points to an invalid address, points to a kernel mode address, or is not correctly aligned. Code: 0xC02625E4" },	//	ERROR_GRAPHICS_INVALID_POINTER
			{ com_errc::error_graphics_no_monitors_correspond_to_display_device, "The function failed because the specified GDI device did not have any monitors associated with it. Code: 0xC02625E5" },	//	ERROR_GRAPHICS_NO_MONITORS_CORRESPOND_TO_DISPLAY_DEVICE
			{ com_errc::error_graphics_parameter_array_too_small, "An array passed to the function cannot hold all of the data that the function must copy into the array. Code: 0xC02625E6" },	//	ERROR_GRAPHICS_PARAMETER_ARRAY_TOO_SMALL
			{ com_errc::error_graphics_internal_error, "An internal error caused an operation to fail. Code: 0xC02625E7" },	//	ERROR_GRAPHICS_INTERNAL_ERROR
			{ com_errc::error_graphics_session_type_change_in_progress, "The function failed because the current session is changing its type. This function cannot be called when the current session is changing its type. There are currently three types of sessions: console, disconnected and remote. Code: 0xC02605E8" },	//	ERROR_GRAPHICS_SESSION_TYPE_CHANGE_IN_PROGRESS
			{ com_errc::nap_e_invalid_packet, "The NAP SoH packet is invalid. Code: 0x80270001" },	//	NAP_E_INVALID_PACKET
			{ com_errc::nap_e_missing_soh, "An SoH was missing from the NAP packet. Code: 0x80270002" },	//	NAP_E_MISSING_SOH
			{ com_errc::nap_e_conflicting_id, "The entity ID conflicts with an already registered id. Code: 0x80270003" },	//	NAP_E_CONFLICTING_ID
			{ com_errc::nap_e_no_cached_soh, "No cached SoH is present. Code: 0x80270004" },	//	NAP_E_NO_CACHED_SOH
			{ com_errc::nap_e_still_bound, "The entity is still bound to the NAP system. Code: 0x80270005" },	//	NAP_E_STILL_BOUND
			{ com_errc::nap_e_not_registered, "The entity is not registered with the NAP system. Code: 0x80270006" },	//	NAP_E_NOT_REGISTERED
			{ com_errc::nap_e_not_initialized, "The entity is not initialized with the NAP system. Code: 0x80270007" },	//	NAP_E_NOT_INITIALIZED
			{ com_errc::nap_e_mismatched_id, "The correlation id in the SoH-Request and SoH-Response do not match up. Code: 0x80270008" },	//	NAP_E_MISMATCHED_ID
			{ com_errc::nap_e_not_pending, "Completion was indicated on a request that is not currently pending. Code: 0x80270009" },	//	NAP_E_NOT_PENDING
			{ com_errc::nap_e_id_not_found, "The NAP component's id was not found. Code: 0x8027000A" },	//	NAP_E_ID_NOT_FOUND
			{ com_errc::nap_e_maxsize_too_small, "The maximum size of the connection is too small for an SoH packet. Code: 0x8027000B" },	//	NAP_E_MAXSIZE_TOO_SMALL
			{ com_errc::nap_e_service_not_running, "The NapAgent service is not running. Code: 0x8027000C" },	//	NAP_E_SERVICE_NOT_RUNNING
			{ com_errc::nap_s_cert_already_present, "A certificate is already present in the cert store. Code: 0x0027000D" },	//	NAP_S_CERT_ALREADY_PRESENT
			{ com_errc::nap_e_entity_disabled, "The entity is disabled with the NapAgent service. Code: 0x8027000E" },	//	NAP_E_ENTITY_DISABLED
			{ com_errc::nap_e_netsh_grouppolicy_error, "Group Policy is not configured. Code: 0x8027000F" },	//	NAP_E_NETSH_GROUPPOLICY_ERROR
			{ com_errc::nap_e_too_many_calls, "Too many simultaneous calls. Code: 0x80270010" },	//	NAP_E_TOO_MANY_CALLS
			{ com_errc::nap_e_shv_config_existed, "SHV configuration already existed. Code: 0x80270011" },	//	NAP_E_SHV_CONFIG_EXISTED
			{ com_errc::nap_e_shv_config_not_found, "SHV configuration is not found. Code: 0x80270012" },	//	NAP_E_SHV_CONFIG_NOT_FOUND
			{ com_errc::nap_e_shv_timeout, "SHV timed out on the request. Code: 0x80270013" },	//	NAP_E_SHV_TIMEOUT
			{ com_errc::tpm_e_error_mask, "This is an error mask to convert TPM hardware errors to win errors. Code: 0x80280000" },	//	TPM_E_ERROR_MASK
			{ com_errc::tpm_e_authfail, "Authentication failed. Code: 0x80280001" },	//	TPM_E_AUTHFAIL
			{ com_errc::tpm_e_badindex, "The index to a PCR, DIR or other register is incorrect. Code: 0x80280002" },	//	TPM_E_BADINDEX
			{ com_errc::tpm_e_bad_parameter, "One or more parameter is bad. Code: 0x80280003" },	//	TPM_E_BAD_PARAMETER
			{ com_errc::tpm_e_auditfailure, "An operation completed successfully but the auditing of that operation failed. Code: 0x80280004" },	//	TPM_E_AUDITFAILURE
			{ com_errc::tpm_e_clear_disabled, "The clear disable flag is set and all clear operations now require physical access. Code: 0x80280005" },	//	TPM_E_CLEAR_DISABLED
			{ com_errc::tpm_e_deactivated, "Activate the Trusted Platform Module (TPM). Code: 0x80280006" },	//	TPM_E_DEACTIVATED
			{ com_errc::tpm_e_disabled, "Enable the Trusted Platform Module (TPM). Code: 0x80280007" },	//	TPM_E_DISABLED
			{ com_errc::tpm_e_disabled_cmd, "The target command has been disabled. Code: 0x80280008" },	//	TPM_E_DISABLED_CMD
			{ com_errc::tpm_e_fail, "The operation failed. Code: 0x80280009" },	//	TPM_E_FAIL
			{ com_errc::tpm_e_bad_ordinal, "The ordinal was unknown or inconsistent. Code: 0x8028000A" },	//	TPM_E_BAD_ORDINAL
			{ com_errc::tpm_e_install_disabled, "The ability to install an owner is disabled. Code: 0x8028000B" },	//	TPM_E_INSTALL_DISABLED
			{ com_errc::tpm_e_invalid_keyhandle, "The key handle cannot be interpreted. Code: 0x8028000C" },	//	TPM_E_INVALID_KEYHANDLE
			{ com_errc::tpm_e_keynotfound, "The key handle points to an invalid key. Code: 0x8028000D" },	//	TPM_E_KEYNOTFOUND
			{ com_errc::tpm_e_inappropriate_enc, "Unacceptable encryption scheme. Code: 0x8028000E" },	//	TPM_E_INAPPROPRIATE_ENC
			{ com_errc::tpm_e_migratefail, "Migration authorization failed. Code: 0x8028000F" },	//	TPM_E_MIGRATEFAIL
			{ com_errc::tpm_e_invalid_pcr_info, "PCR information could not be interpreted. Code: 0x80280010" },	//	TPM_E_INVALID_PCR_INFO
			{ com_errc::tpm_e_nospace, "No room to load key. Code: 0x80280011" },	//	TPM_E_NOSPACE
			{ com_errc::tpm_e_nosrk, "There is no Storage Root Key (SRK) set. Code: 0x80280012" },	//	TPM_E_NOSRK
			{ com_errc::tpm_e_notsealed_blob, "An encrypted blob is invalid or was not created by this TPM. Code: 0x80280013" },	//	TPM_E_NOTSEALED_BLOB
			{ com_errc::tpm_e_owner_set, "The Trusted Platform Module (TPM) already has an owner. Code: 0x80280014" },	//	TPM_E_OWNER_SET
			{ com_errc::tpm_e_resources, "The TPM has insufficient internal resources to perform the requested action. Code: 0x80280015" },	//	TPM_E_RESOURCES
			{ com_errc::tpm_e_shortrandom, "A random string was too short. Code: 0x80280016" },	//	TPM_E_SHORTRANDOM
			{ com_errc::tpm_e_size, "The TPM does not have the space to perform the operation. Code: 0x80280017" },	//	TPM_E_SIZE
			{ com_errc::tpm_e_wrongpcrval, "The named PCR value does not match the current PCR value. Code: 0x80280018" },	//	TPM_E_WRONGPCRVAL
			{ com_errc::tpm_e_bad_param_size, "The paramSize argument to the command has the incorrect value . Code: 0x80280019" },	//	TPM_E_BAD_PARAM_SIZE
			{ com_errc::tpm_e_sha_thread, "There is no existing SHA-1 thread. Code: 0x8028001A" },	//	TPM_E_SHA_THREAD
			{ com_errc::tpm_e_sha_error, "The calculation is unable to proceed because the existing SHA-1 thread has already encountered an error. Code: 0x8028001B" },	//	TPM_E_SHA_ERROR
			{ com_errc::tpm_e_failedselftest, "The TPM hardware device reported a failure during its internal self test. Try restarting the computer to resolve the problem. If the problem continues, check for the latest BIOS or firmware update for your TPM hardware. Consult the computer manufacturer's documentation for instructions. Code: 0x8028001C" },	//	TPM_E_FAILEDSELFTEST
			{ com_errc::tpm_e_auth2fail, "The authorization for the second key in a 2 key function failed authorization. Code: 0x8028001D" },	//	TPM_E_AUTH2FAIL
			{ com_errc::tpm_e_badtag, "The tag value sent to for a command is invalid. Code: 0x8028001E" },	//	TPM_E_BADTAG
			{ com_errc::tpm_e_ioerror, "An IO error occurred transmitting information to the TPM. Code: 0x8028001F" },	//	TPM_E_IOERROR
			{ com_errc::tpm_e_encrypt_error, "The encryption process had a problem. Code: 0x80280020" },	//	TPM_E_ENCRYPT_ERROR
			{ com_errc::tpm_e_decrypt_error, "The decryption process did not complete. Code: 0x80280021" },	//	TPM_E_DECRYPT_ERROR
			{ com_errc::tpm_e_invalid_authhandle, "An invalid handle was used. Code: 0x80280022" },	//	TPM_E_INVALID_AUTHHANDLE
			{ com_errc::tpm_e_no_endorsement, "The TPM does not have an Endorsement Key (EK) installed. Code: 0x80280023" },	//	TPM_E_NO_ENDORSEMENT
			{ com_errc::tpm_e_invalid_keyusage, "The usage of a key is not allowed. Code: 0x80280024" },	//	TPM_E_INVALID_KEYUSAGE
			{ com_errc::tpm_e_wrong_entitytype, "The submitted entity type is not allowed. Code: 0x80280025" },	//	TPM_E_WRONG_ENTITYTYPE
			{ com_errc::tpm_e_invalid_postinit, "The command was received in the wrong sequence relative to TPM_Init and a subsequent TPM_Startup. Code: 0x80280026" },	//	TPM_E_INVALID_POSTINIT
			{ com_errc::tpm_e_inappropriate_sig, "Signed data cannot include additional DER information. Code: 0x80280027" },	//	TPM_E_INAPPROPRIATE_SIG
			{ com_errc::tpm_e_bad_key_property, "The key properties in TPM_KEY_PARMs are not supported by this TPM. Code: 0x80280028" },	//	TPM_E_BAD_KEY_PROPERTY
			{ com_errc::tpm_e_bad_migration, "The migration properties of this key are incorrect. Code: 0x80280029" },	//	TPM_E_BAD_MIGRATION
			{ com_errc::tpm_e_bad_scheme, "The signature or encryption scheme for this key is incorrect or not permitted in this situation. Code: 0x8028002A" },	//	TPM_E_BAD_SCHEME
			{ com_errc::tpm_e_bad_datasize, "The size of the data (or blob) parameter is bad or inconsistent with the referenced key. Code: 0x8028002B" },	//	TPM_E_BAD_DATASIZE
			{ com_errc::tpm_e_bad_mode, "A mode parameter is bad, such as capArea or subCapArea for TPM_GetCapability, phsicalPresence parameter for TPM_PhysicalPresence, or migrationType for TPM_CreateMigrationBlob. Code: 0x8028002C" },	//	TPM_E_BAD_MODE
			{ com_errc::tpm_e_bad_presence, "Either the physicalPresence or physicalPresenceLock bits have the wrong value. Code: 0x8028002D" },	//	TPM_E_BAD_PRESENCE
			{ com_errc::tpm_e_bad_version, "The TPM cannot perform this version of the capability. Code: 0x8028002E" },	//	TPM_E_BAD_VERSION
			{ com_errc::tpm_e_no_wrap_transport, "The TPM does not allow for wrapped transport sessions. Code: 0x8028002F" },	//	TPM_E_NO_WRAP_TRANSPORT
			{ com_errc::tpm_e_auditfail_unsuccessful, "TPM audit construction failed and the underlying command was returning a failure code also. Code: 0x80280030" },	//	TPM_E_AUDITFAIL_UNSUCCESSFUL
			{ com_errc::tpm_e_auditfail_successful, "TPM audit construction failed and the underlying command was returning success. Code: 0x80280031" },	//	TPM_E_AUDITFAIL_SUCCESSFUL
			{ com_errc::tpm_e_notresetable, "Attempt to reset a PCR register that does not have the resettable attribute. Code: 0x80280032" },	//	TPM_E_NOTRESETABLE
			{ com_errc::tpm_e_notlocal, "Attempt to reset a PCR register that requires locality and locality modifier not part of command transport. Code: 0x80280033" },	//	TPM_E_NOTLOCAL
			{ com_errc::tpm_e_bad_type, "Make identity blob not properly typed. Code: 0x80280034" },	//	TPM_E_BAD_TYPE
			{ com_errc::tpm_e_invalid_resource, "When saving context identified resource type does not match actual resource. Code: 0x80280035" },	//	TPM_E_INVALID_RESOURCE
			{ com_errc::tpm_e_notfips, "The TPM is attempting to execute a command only available when in FIPS mode. Code: 0x80280036" },	//	TPM_E_NOTFIPS
			{ com_errc::tpm_e_invalid_family, "The command is attempting to use an invalid family ID. Code: 0x80280037" },	//	TPM_E_INVALID_FAMILY
			{ com_errc::tpm_e_no_nv_permission, "The permission to manipulate the NV storage is not available. Code: 0x80280038" },	//	TPM_E_NO_NV_PERMISSION
			{ com_errc::tpm_e_requires_sign, "The operation requires a signed command. Code: 0x80280039" },	//	TPM_E_REQUIRES_SIGN
			{ com_errc::tpm_e_key_notsupported, "Wrong operation to load an NV key. Code: 0x8028003A" },	//	TPM_E_KEY_NOTSUPPORTED
			{ com_errc::tpm_e_auth_conflict, "NV_LoadKey blob requires both owner and blob authorization. Code: 0x8028003B" },	//	TPM_E_AUTH_CONFLICT
			{ com_errc::tpm_e_area_locked, "The NV area is locked and not writtable. Code: 0x8028003C" },	//	TPM_E_AREA_LOCKED
			{ com_errc::tpm_e_bad_locality, "The locality is incorrect for the attempted operation. Code: 0x8028003D" },	//	TPM_E_BAD_LOCALITY
			{ com_errc::tpm_e_read_only, "The NV area is read only and can't be written to. Code: 0x8028003E" },	//	TPM_E_READ_ONLY
			{ com_errc::tpm_e_per_nowrite, "There is no protection on the write to the NV area. Code: 0x8028003F" },	//	TPM_E_PER_NOWRITE
			{ com_errc::tpm_e_familycount, "The family count value does not match. Code: 0x80280040" },	//	TPM_E_FAMILYCOUNT
			{ com_errc::tpm_e_write_locked, "The NV area has already been written to. Code: 0x80280041" },	//	TPM_E_WRITE_LOCKED
			{ com_errc::tpm_e_bad_attributes, "The NV area attributes conflict. Code: 0x80280042" },	//	TPM_E_BAD_ATTRIBUTES
			{ com_errc::tpm_e_invalid_structure, "The structure tag and version are invalid or inconsistent. Code: 0x80280043" },	//	TPM_E_INVALID_STRUCTURE
			{ com_errc::tpm_e_key_owner_control, "The key is under control of the TPM Owner and can only be evicted by the TPM Owner. Code: 0x80280044" },	//	TPM_E_KEY_OWNER_CONTROL
			{ com_errc::tpm_e_bad_counter, "The counter handle is incorrect. Code: 0x80280045" },	//	TPM_E_BAD_COUNTER
			{ com_errc::tpm_e_not_fullwrite, "The write is not a complete write of the area. Code: 0x80280046" },	//	TPM_E_NOT_FULLWRITE
			{ com_errc::tpm_e_context_gap, "The gap between saved context counts is too large. Code: 0x80280047" },	//	TPM_E_CONTEXT_GAP
			{ com_errc::tpm_e_maxnvwrites, "The maximum number of NV writes without an owner has been exceeded. Code: 0x80280048" },	//	TPM_E_MAXNVWRITES
			{ com_errc::tpm_e_nooperator, "No operator AuthData value is set. Code: 0x80280049" },	//	TPM_E_NOOPERATOR
			{ com_errc::tpm_e_resourcemissing, "The resource pointed to by context is not loaded. Code: 0x8028004A" },	//	TPM_E_RESOURCEMISSING
			{ com_errc::tpm_e_delegate_lock, "The delegate administration is locked. Code: 0x8028004B" },	//	TPM_E_DELEGATE_LOCK
			{ com_errc::tpm_e_delegate_family, "Attempt to manage a family other then the delegated family. Code: 0x8028004C" },	//	TPM_E_DELEGATE_FAMILY
			{ com_errc::tpm_e_delegate_admin, "Delegation table management not enabled. Code: 0x8028004D" },	//	TPM_E_DELEGATE_ADMIN
			{ com_errc::tpm_e_transport_notexclusive, "There was a command executed outside of an exclusive transport session. Code: 0x8028004E" },	//	TPM_E_TRANSPORT_NOTEXCLUSIVE
			{ com_errc::tpm_e_owner_control, "Attempt to context save a owner evict controlled key. Code: 0x8028004F" },	//	TPM_E_OWNER_CONTROL
			{ com_errc::tpm_e_daa_resources, "The DAA command has no resources availble to execute the command. Code: 0x80280050" },	//	TPM_E_DAA_RESOURCES
			{ com_errc::tpm_e_daa_input_data0, "The consistency check on DAA parameter inputData0 has failed. Code: 0x80280051" },	//	TPM_E_DAA_INPUT_DATA0
			{ com_errc::tpm_e_daa_input_data1, "The consistency check on DAA parameter inputData1 has failed. Code: 0x80280052" },	//	TPM_E_DAA_INPUT_DATA1
			{ com_errc::tpm_e_daa_issuer_settings, "The consistency check on DAA_issuerSettings has failed. Code: 0x80280053" },	//	TPM_E_DAA_ISSUER_SETTINGS
			{ com_errc::tpm_e_daa_tpm_settings, "The consistency check on DAA_tpmSpecific has failed. Code: 0x80280054" },	//	TPM_E_DAA_TPM_SETTINGS
			{ com_errc::tpm_e_daa_stage, "The atomic process indicated by the submitted DAA command is not the expected process. Code: 0x80280055" },	//	TPM_E_DAA_STAGE
			{ com_errc::tpm_e_daa_issuer_validity, "The issuer's validity check has detected an inconsistency. Code: 0x80280056" },	//	TPM_E_DAA_ISSUER_VALIDITY
			{ com_errc::tpm_e_daa_wrong_w, "The consistency check on w has failed. Code: 0x80280057" },	//	TPM_E_DAA_WRONG_W
			{ com_errc::tpm_e_bad_handle, "The handle is incorrect. Code: 0x80280058" },	//	TPM_E_BAD_HANDLE
			{ com_errc::tpm_e_bad_delegate, "Delegation is not correct. Code: 0x80280059" },	//	TPM_E_BAD_DELEGATE
			{ com_errc::tpm_e_badcontext, "The context blob is invalid. Code: 0x8028005A" },	//	TPM_E_BADCONTEXT
			{ com_errc::tpm_e_toomanycontexts, "Too many contexts held by the TPM. Code: 0x8028005B" },	//	TPM_E_TOOMANYCONTEXTS
			{ com_errc::tpm_e_ma_ticket_signature, "Migration authority signature validation failure. Code: 0x8028005C" },	//	TPM_E_MA_TICKET_SIGNATURE
			{ com_errc::tpm_e_ma_destination, "Migration destination not authenticated. Code: 0x8028005D" },	//	TPM_E_MA_DESTINATION
			{ com_errc::tpm_e_ma_source, "Migration source incorrect. Code: 0x8028005E" },	//	TPM_E_MA_SOURCE
			{ com_errc::tpm_e_ma_authority, "Incorrect migration authority. Code: 0x8028005F" },	//	TPM_E_MA_AUTHORITY
			{ com_errc::tpm_e_permanentek, "Attempt to revoke the EK and the EK is not revocable. Code: 0x80280061" },	//	TPM_E_PERMANENTEK
			{ com_errc::tpm_e_bad_signature, "Bad signature of CMK ticket. Code: 0x80280062" },	//	TPM_E_BAD_SIGNATURE
			{ com_errc::tpm_e_nocontextspace, "There is no room in the context list for additional contexts. Code: 0x80280063" },	//	TPM_E_NOCONTEXTSPACE
			{ com_errc::tpm_e_command_blocked, "The command was blocked. Code: 0x80280400" },	//	TPM_E_COMMAND_BLOCKED
			{ com_errc::tpm_e_invalid_handle, "The specified handle was not found. Code: 0x80280401" },	//	TPM_E_INVALID_HANDLE
			{ com_errc::tpm_e_duplicate_vhandle, "The TPM returned a duplicate handle and the command needs to be resubmitted. Code: 0x80280402" },	//	TPM_E_DUPLICATE_VHANDLE
			{ com_errc::tpm_e_embedded_command_blocked, "The command within the transport was blocked. Code: 0x80280403" },	//	TPM_E_EMBEDDED_COMMAND_BLOCKED
			{ com_errc::tpm_e_embedded_command_unsupported, "The command within the transport is not supported. Code: 0x80280404" },	//	TPM_E_EMBEDDED_COMMAND_UNSUPPORTED
			{ com_errc::tpm_e_retry, "The TPM is too busy to respond to the command immediately, but the command could be resubmitted at a later time. Code: 0x80280800" },	//	TPM_E_RETRY
			{ com_errc::tpm_e_needs_selftest, "SelfTestFull has not been run. Code: 0x80280801" },	//	TPM_E_NEEDS_SELFTEST
			{ com_errc::tpm_e_doing_selftest, "The TPM is currently executing a full selftest. Code: 0x80280802" },	//	TPM_E_DOING_SELFTEST
			{ com_errc::tpm_e_defend_lock_running, "The TPM is defending against dictionary attacks and is in a time-out period. Code: 0x80280803" },	//	TPM_E_DEFEND_LOCK_RUNNING
			{ com_errc::tbs_e_internal_error, "An internal error has occurred within the Trusted Platform Module support program. Code: 0x80284001" },	//	TBS_E_INTERNAL_ERROR
			{ com_errc::tbs_e_bad_parameter, "One or more input parameters is bad. Code: 0x80284002" },	//	TBS_E_BAD_PARAMETER
			{ com_errc::tbs_e_invalid_output_pointer, "A specified output pointer is bad. Code: 0x80284003" },	//	TBS_E_INVALID_OUTPUT_POINTER
			{ com_errc::tbs_e_invalid_context, "The specified context handle does not refer to a valid context. Code: 0x80284004" },	//	TBS_E_INVALID_CONTEXT
			{ com_errc::tbs_e_insufficient_buffer, "A specified output buffer is too small. Code: 0x80284005" },	//	TBS_E_INSUFFICIENT_BUFFER
			{ com_errc::tbs_e_ioerror, "An error occurred while communicating with the TPM. Code: 0x80284006" },	//	TBS_E_IOERROR
			{ com_errc::tbs_e_invalid_context_param, "One or more context parameters is invalid. Code: 0x80284007" },	//	TBS_E_INVALID_CONTEXT_PARAM
			{ com_errc::tbs_e_service_not_running, "The TBS service is not running and could not be started. Code: 0x80284008" },	//	TBS_E_SERVICE_NOT_RUNNING
			{ com_errc::tbs_e_too_many_tbs_contexts, "A new context could not be created because there are too many open contexts. Code: 0x80284009" },	//	TBS_E_TOO_MANY_TBS_CONTEXTS
			{ com_errc::tbs_e_too_many_resources, "A new virtual resource could not be created because there are too many open virtual resources. Code: 0x8028400A" },	//	TBS_E_TOO_MANY_RESOURCES
			{ com_errc::tbs_e_service_start_pending, "The TBS service has been started but is not yet running. Code: 0x8028400B" },	//	TBS_E_SERVICE_START_PENDING
			{ com_errc::tbs_e_ppi_not_supported, "The physical presence interface is not supported. Code: 0x8028400C" },	//	TBS_E_PPI_NOT_SUPPORTED
			{ com_errc::tbs_e_command_canceled, "The command was canceled. Code: 0x8028400D" },	//	TBS_E_COMMAND_CANCELED
			{ com_errc::tbs_e_buffer_too_large, "The input or output buffer is too large. Code: 0x8028400E" },	//	TBS_E_BUFFER_TOO_LARGE
			{ com_errc::tbs_e_tpm_not_found, "A compatible Trusted Platform Module (TPM) Security Device cannot be found on this computer. Code: 0x8028400F" },	//	TBS_E_TPM_NOT_FOUND
			{ com_errc::tbs_e_service_disabled, "The TBS service has been disabled. Code: 0x80284010" },	//	TBS_E_SERVICE_DISABLED
			{ com_errc::tbs_e_no_event_log, "No TCG event log is available. Code: 0x80284011" },	//	TBS_E_NO_EVENT_LOG
			{ com_errc::tbs_e_access_denied, "The caller does not have the appropriate rights to perform the requested operation. Code: 0x80284012" },	//	TBS_E_ACCESS_DENIED
			{ com_errc::tbs_e_provisioning_not_allowed, "The TPM provisioning action is not allowed by the specified flags.  For provisioning to be successful, one of several actions may be required.  The TPM management console (tpm.msc) action to make the TPM Ready may help.  For further information, see the documentation for the Win32_Tpm WMI method 'Provision'.  (The actions that may be required include importing the TPM Owner Authorization value into the system, calling the Win32_Tpm WMI method for provisioning the TPM and specifying TRUE for either 'ForceClear_Allowed' or 'PhysicalPresencePrompts_Allowed' (as indicated by the value returned in the Additional Information), or enabling the TPM in the system BIOS.). Code: 0x80284013" },	//	TBS_E_PROVISIONING_NOT_ALLOWED
			{ com_errc::tbs_e_ppi_function_unsupported, "The Physical Presence Interface of this firmware does not support the requested method. Code: 0x80284014" },	//	TBS_E_PPI_FUNCTION_UNSUPPORTED
			{ com_errc::tbs_e_ownerauth_not_found, "The requested TPM OwnerAuth value was not found. Code: 0x80284015" },	//	TBS_E_OWNERAUTH_NOT_FOUND
			{ com_errc::tbs_e_provisioning_incomplete, "The TPM provisioning did not complete.  For more information on completing the provisioning, call the Win32_Tpm WMI method for provisioning the TPM ('Provision') and check the returned Information. Code: 0x80284016" },	//	TBS_E_PROVISIONING_INCOMPLETE
			{ com_errc::tpmapi_e_invalid_state, "The command buffer is not in the correct state. Code: 0x80290100" },	//	TPMAPI_E_INVALID_STATE
			{ com_errc::tpmapi_e_not_enough_data, "The command buffer does not contain enough data to satisfy the request. Code: 0x80290101" },	//	TPMAPI_E_NOT_ENOUGH_DATA
			{ com_errc::tpmapi_e_too_much_data, "The command buffer cannot contain any more data. Code: 0x80290102" },	//	TPMAPI_E_TOO_MUCH_DATA
			{ com_errc::tpmapi_e_invalid_output_pointer, "One or more output parameters was NULL or invalid. Code: 0x80290103" },	//	TPMAPI_E_INVALID_OUTPUT_POINTER
			{ com_errc::tpmapi_e_invalid_parameter, "One or more input parameters is invalid. Code: 0x80290104" },	//	TPMAPI_E_INVALID_PARAMETER
			{ com_errc::tpmapi_e_out_of_memory, "Not enough memory was available to satisfy the request. Code: 0x80290105" },	//	TPMAPI_E_OUT_OF_MEMORY
			{ com_errc::tpmapi_e_buffer_too_small, "The specified buffer was too small. Code: 0x80290106" },	//	TPMAPI_E_BUFFER_TOO_SMALL
			{ com_errc::tpmapi_e_internal_error, "An internal error was detected. Code: 0x80290107" },	//	TPMAPI_E_INTERNAL_ERROR
			{ com_errc::tpmapi_e_access_denied, "The caller does not have the appropriate rights to perform the requested operation. Code: 0x80290108" },	//	TPMAPI_E_ACCESS_DENIED
			{ com_errc::tpmapi_e_authorization_failed, "The specified authorization information was invalid. Code: 0x80290109" },	//	TPMAPI_E_AUTHORIZATION_FAILED
			{ com_errc::tpmapi_e_invalid_context_handle, "The specified context handle was not valid. Code: 0x8029010A" },	//	TPMAPI_E_INVALID_CONTEXT_HANDLE
			{ com_errc::tpmapi_e_tbs_communication_error, "An error occurred while communicating with the TBS. Code: 0x8029010B" },	//	TPMAPI_E_TBS_COMMUNICATION_ERROR
			{ com_errc::tpmapi_e_tpm_command_error, "The TPM returned an unexpected result. Code: 0x8029010C" },	//	TPMAPI_E_TPM_COMMAND_ERROR
			{ com_errc::tpmapi_e_message_too_large, "The message was too large for the encoding scheme. Code: 0x8029010D" },	//	TPMAPI_E_MESSAGE_TOO_LARGE
			{ com_errc::tpmapi_e_invalid_encoding, "The encoding in the blob was not recognized. Code: 0x8029010E" },	//	TPMAPI_E_INVALID_ENCODING
			{ com_errc::tpmapi_e_invalid_key_size, "The key size is not valid. Code: 0x8029010F" },	//	TPMAPI_E_INVALID_KEY_SIZE
			{ com_errc::tpmapi_e_encryption_failed, "The encryption operation failed. Code: 0x80290110" },	//	TPMAPI_E_ENCRYPTION_FAILED
			{ com_errc::tpmapi_e_invalid_key_params, "The key parameters structure was not valid. Code: 0x80290111" },	//	TPMAPI_E_INVALID_KEY_PARAMS
			{ com_errc::tpmapi_e_invalid_migration_authorization_blob, "The requested supplied data does not appear to be a valid migration authorization blob. Code: 0x80290112" },	//	TPMAPI_E_INVALID_MIGRATION_AUTHORIZATION_BLOB
			{ com_errc::tpmapi_e_invalid_pcr_index, "The specified PCR index was invalid. Code: 0x80290113" },	//	TPMAPI_E_INVALID_PCR_INDEX
			{ com_errc::tpmapi_e_invalid_delegate_blob, "The data given does not appear to be a valid delegate blob. Code: 0x80290114" },	//	TPMAPI_E_INVALID_DELEGATE_BLOB
			{ com_errc::tpmapi_e_invalid_context_params, "One or more of the specified context parameters was not valid. Code: 0x80290115" },	//	TPMAPI_E_INVALID_CONTEXT_PARAMS
			{ com_errc::tpmapi_e_invalid_key_blob, "The data given does not appear to be a valid key blob. Code: 0x80290116" },	//	TPMAPI_E_INVALID_KEY_BLOB
			{ com_errc::tpmapi_e_invalid_pcr_data, "The specified PCR data was invalid. Code: 0x80290117" },	//	TPMAPI_E_INVALID_PCR_DATA
			{ com_errc::tpmapi_e_invalid_owner_auth, "The format of the owner auth data was invalid. Code: 0x80290118" },	//	TPMAPI_E_INVALID_OWNER_AUTH
			{ com_errc::tpmapi_e_fips_rng_check_failed, "The random number generated did not pass FIPS RNG check. Code: 0x80290119" },	//	TPMAPI_E_FIPS_RNG_CHECK_FAILED
			{ com_errc::tpmapi_e_empty_tcg_log, "The TCG Event Log does not contain any data. Code: 0x8029011A" },	//	TPMAPI_E_EMPTY_TCG_LOG
			{ com_errc::tpmapi_e_invalid_tcg_log_entry, "An entry in the TCG Event Log was invalid. Code: 0x8029011B" },	//	TPMAPI_E_INVALID_TCG_LOG_ENTRY
			{ com_errc::tpmapi_e_tcg_separator_absent, "A TCG Separator was not found. Code: 0x8029011C" },	//	TPMAPI_E_TCG_SEPARATOR_ABSENT
			{ com_errc::tpmapi_e_tcg_invalid_digest_entry, "A digest value in a TCG Log entry did not match hashed data. Code: 0x8029011D" },	//	TPMAPI_E_TCG_INVALID_DIGEST_ENTRY
			{ com_errc::tpmapi_e_policy_denies_operation, "The requested operation was blocked by current TPM policy. Please contact your system administrator for assistance. Code: 0x8029011E" },	//	TPMAPI_E_POLICY_DENIES_OPERATION
			{ com_errc::tbsimp_e_buffer_too_small, "The specified buffer was too small. Code: 0x80290200" },	//	TBSIMP_E_BUFFER_TOO_SMALL
			{ com_errc::tbsimp_e_cleanup_failed, "The context could not be cleaned up. Code: 0x80290201" },	//	TBSIMP_E_CLEANUP_FAILED
			{ com_errc::tbsimp_e_invalid_context_handle, "The specified context handle is invalid. Code: 0x80290202" },	//	TBSIMP_E_INVALID_CONTEXT_HANDLE
			{ com_errc::tbsimp_e_invalid_context_param, "An invalid context parameter was specified. Code: 0x80290203" },	//	TBSIMP_E_INVALID_CONTEXT_PARAM
			{ com_errc::tbsimp_e_tpm_error, "An error occurred while communicating with the TPM. Code: 0x80290204" },	//	TBSIMP_E_TPM_ERROR
			{ com_errc::tbsimp_e_hash_bad_key, "No entry with the specified key was found. Code: 0x80290205" },	//	TBSIMP_E_HASH_BAD_KEY
			{ com_errc::tbsimp_e_duplicate_vhandle, "The specified virtual handle matches a virtual handle already in use. Code: 0x80290206" },	//	TBSIMP_E_DUPLICATE_VHANDLE
			{ com_errc::tbsimp_e_invalid_output_pointer, "The pointer to the returned handle location was NULL or invalid. Code: 0x80290207" },	//	TBSIMP_E_INVALID_OUTPUT_POINTER
			{ com_errc::tbsimp_e_invalid_parameter, "One or more parameters is invalid. Code: 0x80290208" },	//	TBSIMP_E_INVALID_PARAMETER
			{ com_errc::tbsimp_e_rpc_init_failed, "The RPC subsystem could not be initialized. Code: 0x80290209" },	//	TBSIMP_E_RPC_INIT_FAILED
			{ com_errc::tbsimp_e_scheduler_not_running, "The TBS scheduler is not running. Code: 0x8029020A" },	//	TBSIMP_E_SCHEDULER_NOT_RUNNING
			{ com_errc::tbsimp_e_command_canceled, "The command was canceled. Code: 0x8029020B" },	//	TBSIMP_E_COMMAND_CANCELED
			{ com_errc::tbsimp_e_out_of_memory, "There was not enough memory to fulfill the request. Code: 0x8029020C" },	//	TBSIMP_E_OUT_OF_MEMORY
			{ com_errc::tbsimp_e_list_no_more_items, "The specified list is empty, or the iteration has reached the end of the list. Code: 0x8029020D" },	//	TBSIMP_E_LIST_NO_MORE_ITEMS
			{ com_errc::tbsimp_e_list_not_found, "The specified item was not found in the list. Code: 0x8029020E" },	//	TBSIMP_E_LIST_NOT_FOUND
			{ com_errc::tbsimp_e_not_enough_space, "The TPM does not have enough space to load the requested resource. Code: 0x8029020F" },	//	TBSIMP_E_NOT_ENOUGH_SPACE
			{ com_errc::tbsimp_e_not_enough_tpm_contexts, "There are too many TPM contexts in use. Code: 0x80290210" },	//	TBSIMP_E_NOT_ENOUGH_TPM_CONTEXTS
			{ com_errc::tbsimp_e_command_failed, "The TPM command failed. Code: 0x80290211" },	//	TBSIMP_E_COMMAND_FAILED
			{ com_errc::tbsimp_e_unknown_ordinal, "The TBS does not recognize the specified ordinal. Code: 0x80290212" },	//	TBSIMP_E_UNKNOWN_ORDINAL
			{ com_errc::tbsimp_e_resource_expired, "The requested resource is no longer available. Code: 0x80290213" },	//	TBSIMP_E_RESOURCE_EXPIRED
			{ com_errc::tbsimp_e_invalid_resource, "The resource type did not match. Code: 0x80290214" },	//	TBSIMP_E_INVALID_RESOURCE
			{ com_errc::tbsimp_e_nothing_to_unload, "No resources can be unloaded. Code: 0x80290215" },	//	TBSIMP_E_NOTHING_TO_UNLOAD
			{ com_errc::tbsimp_e_hash_table_full, "No new entries can be added to the hash table. Code: 0x80290216" },	//	TBSIMP_E_HASH_TABLE_FULL
			{ com_errc::tbsimp_e_too_many_tbs_contexts, "A new TBS context could not be created because there are too many open contexts. Code: 0x80290217" },	//	TBSIMP_E_TOO_MANY_TBS_CONTEXTS
			{ com_errc::tbsimp_e_too_many_resources, "A new virtual resource could not be created because there are too many open virtual resources. Code: 0x80290218" },	//	TBSIMP_E_TOO_MANY_RESOURCES
			{ com_errc::tbsimp_e_ppi_not_supported, "The physical presence interface is not supported. Code: 0x80290219" },	//	TBSIMP_E_PPI_NOT_SUPPORTED
			{ com_errc::tbsimp_e_tpm_incompatible, "TBS is not compatible with the version of TPM found on the system. Code: 0x8029021A" },	//	TBSIMP_E_TPM_INCOMPATIBLE
			{ com_errc::tbsimp_e_no_event_log, "No TCG event log is available. Code: 0x8029021B" },	//	TBSIMP_E_NO_EVENT_LOG
			{ com_errc::tpm_e_ppi_acpi_failure, "A general error was detected when attempting to acquire the BIOS's response to a Physical Presence command. Code: 0x80290300" },	//	TPM_E_PPI_ACPI_FAILURE
			{ com_errc::tpm_e_ppi_user_abort, "The user failed to confirm the TPM operation request. Code: 0x80290301" },	//	TPM_E_PPI_USER_ABORT
			{ com_errc::tpm_e_ppi_bios_failure, "The BIOS failure prevented the successful execution of the requested TPM operation (e.g. invalid TPM operation request, BIOS communication error with the TPM). Code: 0x80290302" },	//	TPM_E_PPI_BIOS_FAILURE
			{ com_errc::tpm_e_ppi_not_supported, "The BIOS does not support the physical presence interface. Code: 0x80290303" },	//	TPM_E_PPI_NOT_SUPPORTED
			{ com_errc::tpm_e_ppi_blocked_in_bios, "The Physical Presence command was blocked by current BIOS settings. The system owner may be able to reconfigure the BIOS settings to allow the command. Code: 0x80290304" },	//	TPM_E_PPI_BLOCKED_IN_BIOS
			{ com_errc::tpm_e_pcp_error_mask, "This is an error mask to convert Platform Crypto Provider errors to win errors. Code: 0x80290400" },	//	TPM_E_PCP_ERROR_MASK
			{ com_errc::tpm_e_pcp_device_not_ready, "The Platform Crypto Device is currently not ready. It needs to be fully provisioned to be operational. Code: 0x80290401" },	//	TPM_E_PCP_DEVICE_NOT_READY
			{ com_errc::tpm_e_pcp_invalid_handle, "The handle provided to the Platform Crypto Provider is invalid. Code: 0x80290402" },	//	TPM_E_PCP_INVALID_HANDLE
			{ com_errc::tpm_e_pcp_invalid_parameter, "A parameter provided to the Platform Crypto Provider is invalid. Code: 0x80290403" },	//	TPM_E_PCP_INVALID_PARAMETER
			{ com_errc::tpm_e_pcp_flag_not_supported, "A provided flag to the Platform Crypto Provider is not supported. Code: 0x80290404" },	//	TPM_E_PCP_FLAG_NOT_SUPPORTED
			{ com_errc::tpm_e_pcp_not_supported, "The requested operation is not supported by this Platform Crypto Provider. Code: 0x80290405" },	//	TPM_E_PCP_NOT_SUPPORTED
			{ com_errc::tpm_e_pcp_buffer_too_small, "The buffer is too small to contain all data. No information has been written to the buffer. Code: 0x80290406" },	//	TPM_E_PCP_BUFFER_TOO_SMALL
			{ com_errc::tpm_e_pcp_internal_error, "An unexpected internal error has occurred in the Platform Crypto Provider. Code: 0x80290407" },	//	TPM_E_PCP_INTERNAL_ERROR
			{ com_errc::tpm_e_pcp_authentication_failed, "The authorization to use a provider object has failed. Code: 0x80290408" },	//	TPM_E_PCP_AUTHENTICATION_FAILED
			{ com_errc::tpm_e_pcp_authentication_ignored, "The Platform Crypto Device has ignored the authorization for the provider object, to mitigate against a dictionary attack. Code: 0x80290409" },	//	TPM_E_PCP_AUTHENTICATION_IGNORED
			{ com_errc::tpm_e_pcp_policy_not_found, "The referenced policy was not found. Code: 0x8029040A" },	//	TPM_E_PCP_POLICY_NOT_FOUND
			{ com_errc::tpm_e_pcp_profile_not_found, "The referenced profile was not found. Code: 0x8029040B" },	//	TPM_E_PCP_PROFILE_NOT_FOUND
			{ com_errc::tpm_e_pcp_validation_failed, "The validation was not succesful. Code: 0x8029040C" },	//	TPM_E_PCP_VALIDATION_FAILED
			{ com_errc::pla_e_dcs_not_found, "Data Collector Set was not found. Code: 0x80300002" },	//	PLA_E_DCS_NOT_FOUND
			{ com_errc::pla_e_dcs_in_use, "The Data Collector Set or one of its dependencies is already in use. Code: 0x803000AA" },	//	PLA_E_DCS_IN_USE
			{ com_errc::pla_e_too_many_folders, "Unable to start Data Collector Set because there are too many folders. Code: 0x80300045" },	//	PLA_E_TOO_MANY_FOLDERS
			{ com_errc::pla_e_no_min_disk, "Not enough free disk space to start Data Collector Set. Code: 0x80300070" },	//	PLA_E_NO_MIN_DISK
			{ com_errc::pla_e_dcs_already_exists, "Data Collector Set already exists. Code: 0x803000B7" },	//	PLA_E_DCS_ALREADY_EXISTS
			{ com_errc::pla_s_property_ignored, "Property value will be ignored. Code: 0x00300100" },	//	PLA_S_PROPERTY_IGNORED
			{ com_errc::pla_e_property_conflict, "Property value conflict. Code: 0x80300101" },	//	PLA_E_PROPERTY_CONFLICT
			{ com_errc::pla_e_dcs_singleton_required, "The current configuration for this Data Collector Set requires that it contain exactly one Data Collector. Code: 0x80300102" },	//	PLA_E_DCS_SINGLETON_REQUIRED
			{ com_errc::pla_e_credentials_required, "A user account is required in order to commit the current Data Collector Set properties. Code: 0x80300103" },	//	PLA_E_CREDENTIALS_REQUIRED
			{ com_errc::pla_e_dcs_not_running, "Data Collector Set is not running. Code: 0x80300104" },	//	PLA_E_DCS_NOT_RUNNING
			{ com_errc::pla_e_conflict_incl_excl_api, "A conflict was detected in the list of include/exclude APIs. Do not specify the same API in both the include list and the exclude list. Code: 0x80300105" },	//	PLA_E_CONFLICT_INCL_EXCL_API
			{ com_errc::pla_e_network_exe_not_valid, "The executable path you have specified refers to a network share or UNC path. Code: 0x80300106" },	//	PLA_E_NETWORK_EXE_NOT_VALID
			{ com_errc::pla_e_exe_already_configured, "The executable path you have specified is already configured for API tracing. Code: 0x80300107" },	//	PLA_E_EXE_ALREADY_CONFIGURED
			{ com_errc::pla_e_exe_path_not_valid, "The executable path you have specified does not exist. Verify that the specified path is correct. Code: 0x80300108" },	//	PLA_E_EXE_PATH_NOT_VALID
			{ com_errc::pla_e_dc_already_exists, "Data Collector already exists. Code: 0x80300109" },	//	PLA_E_DC_ALREADY_EXISTS
			{ com_errc::pla_e_dcs_start_wait_timeout, "The wait for the Data Collector Set start notification has timed out. Code: 0x8030010A" },	//	PLA_E_DCS_START_WAIT_TIMEOUT
			{ com_errc::pla_e_dc_start_wait_timeout, "The wait for the Data Collector to start has timed out. Code: 0x8030010B" },	//	PLA_E_DC_START_WAIT_TIMEOUT
			{ com_errc::pla_e_report_wait_timeout, "The wait for the report generation tool to finish has timed out. Code: 0x8030010C" },	//	PLA_E_REPORT_WAIT_TIMEOUT
			{ com_errc::pla_e_no_duplicates, "Duplicate items are not allowed. Code: 0x8030010D" },	//	PLA_E_NO_DUPLICATES
			{ com_errc::pla_e_exe_full_path_required, "When specifying the executable that you want to trace, you must specify a full path to the executable and not just a filename. Code: 0x8030010E" },	//	PLA_E_EXE_FULL_PATH_REQUIRED
			{ com_errc::pla_e_invalid_session_name, "The session name provided is invalid. Code: 0x8030010F" },	//	PLA_E_INVALID_SESSION_NAME
			{ com_errc::pla_e_pla_channel_not_enabled, "The Event Log channel Microsoft-Windows-Diagnosis-PLA/Operational must be enabled to perform this operation. Code: 0x80300110" },	//	PLA_E_PLA_CHANNEL_NOT_ENABLED
			{ com_errc::pla_e_tasksched_channel_not_enabled, "The Event Log channel Microsoft-Windows-TaskScheduler must be enabled to perform this operation. Code: 0x80300111" },	//	PLA_E_TASKSCHED_CHANNEL_NOT_ENABLED
			{ com_errc::pla_e_rules_manager_failed, "The execution of the Rules Manager failed. Code: 0x80300112" },	//	PLA_E_RULES_MANAGER_FAILED
			{ com_errc::pla_e_cabapi_failure, "An error occurred while attempting to compress or extract the data. Code: 0x80300113" },	//	PLA_E_CABAPI_FAILURE
			{ com_errc::fve_e_locked_volume, "This drive is locked by BitLocker Drive Encryption. You must unlock this drive from Control Panel. Code: 0x80310000" },	//	FVE_E_LOCKED_VOLUME
			{ com_errc::fve_e_not_encrypted, "This drive is not encrypted. Code: 0x80310001" },	//	FVE_E_NOT_ENCRYPTED
			{ com_errc::fve_e_no_tpm_bios, "The BIOS did not correctly communicate with the Trusted Platform Module (TPM). Contact the computer manufacturer for BIOS upgrade instructions. Code: 0x80310002" },	//	FVE_E_NO_TPM_BIOS
			{ com_errc::fve_e_no_mbr_metric, "The BIOS did not correctly communicate with the master boot record (MBR). Contact the computer manufacturer for BIOS upgrade instructions. Code: 0x80310003" },	//	FVE_E_NO_MBR_METRIC
			{ com_errc::fve_e_no_bootsector_metric, "A required TPM measurement is missing. If there is a bootable CD or DVD in your computer, remove it, restart the computer, and turn on BitLocker again. If the problem persists, ensure the master boot record is up to date. Code: 0x80310004" },	//	FVE_E_NO_BOOTSECTOR_METRIC
			{ com_errc::fve_e_no_bootmgr_metric, "The boot sector of this drive is not compatible with BitLocker Drive Encryption. Use the Bootrec.exe tool in the Windows Recovery Environment to update or repair the boot manager (BOOTMGR). Code: 0x80310005" },	//	FVE_E_NO_BOOTMGR_METRIC
			{ com_errc::fve_e_wrong_bootmgr, "The boot manager of this operating system is not compatible with BitLocker Drive Encryption. Use the Bootrec.exe tool in the Windows Recovery Environment to update or repair the boot manager (BOOTMGR). Code: 0x80310006" },	//	FVE_E_WRONG_BOOTMGR
			{ com_errc::fve_e_secure_key_required, "At least one secure key protector is required for this operation to be performed. Code: 0x80310007" },	//	FVE_E_SECURE_KEY_REQUIRED
			{ com_errc::fve_e_not_activated, "BitLocker Drive Encryption is not enabled on this drive. Turn on BitLocker. Code: 0x80310008" },	//	FVE_E_NOT_ACTIVATED
			{ com_errc::fve_e_action_not_allowed, "BitLocker Drive Encryption cannot perform the requested action. This condition may occur when two requests are issued at the same time. Wait a few moments and then try the action again. Code: 0x80310009" },	//	FVE_E_ACTION_NOT_ALLOWED
			{ com_errc::fve_e_ad_schema_not_installed, "The Active Directory Domain Services forest does not contain the required attributes and classes to host BitLocker Drive Encryption or Trusted Platform Module information. Contact your domain administrator to verify that any required BitLocker Active Directory schema extensions have been installed. Code: 0x8031000A" },	//	FVE_E_AD_SCHEMA_NOT_INSTALLED
			{ com_errc::fve_e_ad_invalid_datatype, "The type of the data obtained from Active Directory was not expected. The BitLocker recovery information may be missing or corrupted. Code: 0x8031000B" },	//	FVE_E_AD_INVALID_DATATYPE
			{ com_errc::fve_e_ad_invalid_datasize, "The size of the data obtained from Active Directory was not expected. The BitLocker recovery information may be missing or corrupted. Code: 0x8031000C" },	//	FVE_E_AD_INVALID_DATASIZE
			{ com_errc::fve_e_ad_no_values, "The attribute read from Active Directory does not contain any values. The BitLocker recovery information may be missing or corrupted. Code: 0x8031000D" },	//	FVE_E_AD_NO_VALUES
			{ com_errc::fve_e_ad_attr_not_set, "The attribute was not set. Verify that you are logged on with a domain account that has the ability to write information to Active Directory objects. Code: 0x8031000E" },	//	FVE_E_AD_ATTR_NOT_SET
			{ com_errc::fve_e_ad_guid_not_found, "The specified attribute cannot be found in Active Directory Domain Services. Contact your domain administrator to verify that any required BitLocker Active Directory schema extensions have been installed. Code: 0x8031000F" },	//	FVE_E_AD_GUID_NOT_FOUND
			{ com_errc::fve_e_bad_information, "The BitLocker metadata for the encrypted drive is not valid. You can attempt to repair the drive to restore access. Code: 0x80310010" },	//	FVE_E_BAD_INFORMATION
			{ com_errc::fve_e_too_small, "The drive cannot be encrypted because it does not have enough free space. Delete any unnecessary data on the drive to create additional free space and then try again. Code: 0x80310011" },	//	FVE_E_TOO_SMALL
			{ com_errc::fve_e_system_volume, "The drive cannot be encrypted because it contains system boot information. Create a separate partition for use as the system drive that contains the boot information and a second partition for use as the operating system drive and then encrypt the operating system drive. Code: 0x80310012" },	//	FVE_E_SYSTEM_VOLUME
			{ com_errc::fve_e_failed_wrong_fs, "The drive cannot be encrypted because the file system is not supported. Code: 0x80310013" },	//	FVE_E_FAILED_WRONG_FS
			{ com_errc::fve_e_bad_partition_size, "The file system size is larger than the partition size in the partition table. This drive may be corrupt or may have been tampered with. To use it with BitLocker, you must reformat the partition. Code: 0x80310014" },	//	FVE_E_BAD_PARTITION_SIZE
			{ com_errc::fve_e_not_supported, "This drive cannot be encrypted. Code: 0x80310015" },	//	FVE_E_NOT_SUPPORTED
			{ com_errc::fve_e_bad_data, "The data is not valid. Code: 0x80310016" },	//	FVE_E_BAD_DATA
			{ com_errc::fve_e_volume_not_bound, "The data drive specified is not set to automatically unlock on the current computer and cannot be unlocked automatically. Code: 0x80310017" },	//	FVE_E_VOLUME_NOT_BOUND
			{ com_errc::fve_e_tpm_not_owned, "You must initialize the Trusted Platform Module (TPM) before you can use BitLocker Drive Encryption. Code: 0x80310018" },	//	FVE_E_TPM_NOT_OWNED
			{ com_errc::fve_e_not_data_volume, "The operation attempted cannot be performed on an operating system drive. Code: 0x80310019" },	//	FVE_E_NOT_DATA_VOLUME
			{ com_errc::fve_e_ad_insufficient_buffer, "The buffer supplied to a function was insufficient to contain the returned data. Increase the buffer size before running the function again. Code: 0x8031001A" },	//	FVE_E_AD_INSUFFICIENT_BUFFER
			{ com_errc::fve_e_conv_read, "A read operation failed while converting the drive. The drive was not converted. Please re-enable BitLocker. Code: 0x8031001B" },	//	FVE_E_CONV_READ
			{ com_errc::fve_e_conv_write, "A write operation failed while converting the drive. The drive was not converted. Please re-enable BitLocker. Code: 0x8031001C" },	//	FVE_E_CONV_WRITE
			{ com_errc::fve_e_key_required, "One or more BitLocker key protectors are required. You cannot delete the last key on this drive. Code: 0x8031001D" },	//	FVE_E_KEY_REQUIRED
			{ com_errc::fve_e_clustering_not_supported, "Cluster configurations are not supported by BitLocker Drive Encryption. Code: 0x8031001E" },	//	FVE_E_CLUSTERING_NOT_SUPPORTED
			{ com_errc::fve_e_volume_bound_already, "The drive specified is already configured to be automatically unlocked on the current computer. Code: 0x8031001F" },	//	FVE_E_VOLUME_BOUND_ALREADY
			{ com_errc::fve_e_os_not_protected, "The operating system drive is not protected by BitLocker Drive Encryption. Code: 0x80310020" },	//	FVE_E_OS_NOT_PROTECTED
			{ com_errc::fve_e_protection_disabled, "BitLocker Drive Encryption has been suspended on this drive. All BitLocker key protectors configured for this drive are effectively disabled, and the drive will be automatically unlocked using an unencrypted (clear) key. Code: 0x80310021" },	//	FVE_E_PROTECTION_DISABLED
			{ com_errc::fve_e_recovery_key_required, "The drive you are attempting to lock does not have any key protectors available for encryption because BitLocker protection is currently suspended. Re-enable BitLocker to lock this drive. Code: 0x80310022" },	//	FVE_E_RECOVERY_KEY_REQUIRED
			{ com_errc::fve_e_foreign_volume, "BitLocker cannot use the Trusted Platform Module (TPM) to protect a data drive. TPM protection can only be used with the operating system drive. Code: 0x80310023" },	//	FVE_E_FOREIGN_VOLUME
			{ com_errc::fve_e_overlapped_update, "The BitLocker metadata for the encrypted drive cannot be updated because it was locked for updating by another process. Please try this process again. Code: 0x80310024" },	//	FVE_E_OVERLAPPED_UPDATE
			{ com_errc::fve_e_tpm_srk_auth_not_zero, "The authorization data for the storage root key (SRK) of the Trusted Platform Module (TPM) is not zero and is therefore incompatible with BitLocker. Please initialize the TPM before attempting to use it with BitLocker. Code: 0x80310025" },	//	FVE_E_TPM_SRK_AUTH_NOT_ZERO
			{ com_errc::fve_e_failed_sector_size, "The drive encryption algorithm cannot be used on this sector size. Code: 0x80310026" },	//	FVE_E_FAILED_SECTOR_SIZE
			{ com_errc::fve_e_failed_authentication, "The drive cannot be unlocked with the key provided. Confirm that you have provided the correct key and try again. Code: 0x80310027" },	//	FVE_E_FAILED_AUTHENTICATION
			{ com_errc::fve_e_not_os_volume, "The drive specified is not the operating system drive. Code: 0x80310028" },	//	FVE_E_NOT_OS_VOLUME
			{ com_errc::fve_e_autounlock_enabled, "BitLocker Drive Encryption cannot be turned off on the operating system drive until the auto unlock feature has been disabled for the fixed data drives and removable data drives associated with this computer. Code: 0x80310029" },	//	FVE_E_AUTOUNLOCK_ENABLED
			{ com_errc::fve_e_wrong_bootsector, "The system partition boot sector does not perform Trusted Platform Module (TPM) measurements. Use the Bootrec.exe tool in the Windows Recovery Environment to update or repair the boot sector. Code: 0x8031002A" },	//	FVE_E_WRONG_BOOTSECTOR
			{ com_errc::fve_e_wrong_system_fs, "BitLocker Drive Encryption operating system drives must be formatted with the NTFS file system in order to be encrypted. Convert the drive to NTFS, and then turn on BitLocker. Code: 0x8031002B" },	//	FVE_E_WRONG_SYSTEM_FS
			{ com_errc::fve_e_policy_password_required, "Group Policy settings require that a recovery password be specified before encrypting the drive. Code: 0x8031002C" },	//	FVE_E_POLICY_PASSWORD_REQUIRED
			{ com_errc::fve_e_cannot_set_fvek_encrypted, "The drive encryption algorithm and key cannot be set on a previously encrypted drive. To encrypt this drive with BitLocker Drive Encryption, remove the previous encryption and then turn on BitLocker. Code: 0x8031002D" },	//	FVE_E_CANNOT_SET_FVEK_ENCRYPTED
			{ com_errc::fve_e_cannot_encrypt_no_key, "BitLocker Drive Encryption cannot encrypt the specified drive because an encryption key is not available. Add a key protector to encrypt this drive. Code: 0x8031002E" },	//	FVE_E_CANNOT_ENCRYPT_NO_KEY
			{ com_errc::fve_e_bootable_cddvd, "BitLocker Drive Encryption detected bootable media (CD or DVD) in the computer. Remove the media and restart the computer before configuring BitLocker. Code: 0x80310030" },	//	FVE_E_BOOTABLE_CDDVD
			{ com_errc::fve_e_protector_exists, "This key protector cannot be added. Only one key protector of this type is allowed for this drive. Code: 0x80310031" },	//	FVE_E_PROTECTOR_EXISTS
			{ com_errc::fve_e_relative_path, "The recovery password file was not found because a relative path was specified. Recovery passwords must be saved to a fully qualified path. Environment variables configured on the computer can be used in the path. Code: 0x80310032" },	//	FVE_E_RELATIVE_PATH
			{ com_errc::fve_e_protector_not_found, "The specified key protector was not found on the drive. Try another key protector. Code: 0x80310033" },	//	FVE_E_PROTECTOR_NOT_FOUND
			{ com_errc::fve_e_invalid_key_format, "The recovery key provided is corrupt and cannot be used to access the drive. An alternative recovery method, such as recovery password, a data recovery agent, or a backup version of the recovery key must be used to recover access to the drive. Code: 0x80310034" },	//	FVE_E_INVALID_KEY_FORMAT
			{ com_errc::fve_e_invalid_password_format, "The format of the recovery password provided is invalid. BitLocker recovery passwords are 48 digits. Verify that the recovery password is in the correct format and then try again. Code: 0x80310035" },	//	FVE_E_INVALID_PASSWORD_FORMAT
			{ com_errc::fve_e_fips_rng_check_failed, "The random number generator check test failed. Code: 0x80310036" },	//	FVE_E_FIPS_RNG_CHECK_FAILED
			{ com_errc::fve_e_fips_prevents_recovery_password, "The Group Policy setting requiring FIPS compliance prevents a local recovery password from being generated or used by BitLocker Drive Encryption. When operating in FIPS-compliant mode, BitLocker recovery options can be either a recovery key stored on a USB drive or recovery through a data recovery agent. Code: 0x80310037" },	//	FVE_E_FIPS_PREVENTS_RECOVERY_PASSWORD
			{ com_errc::fve_e_fips_prevents_external_key_export, "The Group Policy setting requiring FIPS compliance prevents the recovery password from being saved to Active Directory. When operating in FIPS-compliant mode, BitLocker recovery options can be either a recovery key stored on a USB drive or recovery through a data recovery agent. Check your Group Policy settings configuration. Code: 0x80310038" },	//	FVE_E_FIPS_PREVENTS_EXTERNAL_KEY_EXPORT
			{ com_errc::fve_e_not_decrypted, "The drive must be fully decrypted to complete this operation. Code: 0x80310039" },	//	FVE_E_NOT_DECRYPTED
			{ com_errc::fve_e_invalid_protector_type, "The key protector specified cannot be used for this operation. Code: 0x8031003A" },	//	FVE_E_INVALID_PROTECTOR_TYPE
			{ com_errc::fve_e_no_protectors_to_test, "No key protectors exist on the drive to perform the hardware test. Code: 0x8031003B" },	//	FVE_E_NO_PROTECTORS_TO_TEST
			{ com_errc::fve_e_keyfile_not_found, "The BitLocker startup key or recovery password cannot be found on the USB device. Verify that you have the correct USB device, that the USB device is plugged into the computer on an active USB port, restart the computer, and then try again. If the problem persists, contact the computer manufacturer for BIOS upgrade instructions. Code: 0x8031003C" },	//	FVE_E_KEYFILE_NOT_FOUND
			{ com_errc::fve_e_keyfile_invalid, "The BitLocker startup key or recovery password file provided is corrupt or invalid. Verify that you have the correct startup key or recovery password file and try again. Code: 0x8031003D" },	//	FVE_E_KEYFILE_INVALID
			{ com_errc::fve_e_keyfile_no_vmk, "The BitLocker encryption key cannot be obtained from the startup key or recovery password. Verify that you have the correct startup key or recovery password and try again. Code: 0x8031003E" },	//	FVE_E_KEYFILE_NO_VMK
			{ com_errc::fve_e_tpm_disabled, "The Trusted Platform Module (TPM) is disabled. The TPM must be enabled, initialized, and have valid ownership before it can be used with BitLocker Drive Encryption. Code: 0x8031003F" },	//	FVE_E_TPM_DISABLED
			{ com_errc::fve_e_not_allowed_in_safe_mode, "The BitLocker configuration of the specified drive cannot be managed because this computer is currently operating in Safe Mode. While in Safe Mode, BitLocker Drive Encryption can only be used for recovery purposes. Code: 0x80310040" },	//	FVE_E_NOT_ALLOWED_IN_SAFE_MODE
			{ com_errc::fve_e_tpm_invalid_pcr, "The Trusted Platform Module (TPM) was unable to unlock the drive. Either the system boot information changed after choosing BitLocker settings or the PIN did not match. If the problem persists after several tries, there may be a hardware or firmware problem. Code: 0x80310041" },	//	FVE_E_TPM_INVALID_PCR
			{ com_errc::fve_e_tpm_no_vmk, "The BitLocker encryption key cannot be obtained from the Trusted Platform Module (TPM). Code: 0x80310042" },	//	FVE_E_TPM_NO_VMK
			{ com_errc::fve_e_pin_invalid, "The BitLocker encryption key cannot be obtained from the Trusted Platform Module (TPM) and PIN. Code: 0x80310043" },	//	FVE_E_PIN_INVALID
			{ com_errc::fve_e_auth_invalid_application, "A boot application has changed since BitLocker Drive Encryption was enabled. Code: 0x80310044" },	//	FVE_E_AUTH_INVALID_APPLICATION
			{ com_errc::fve_e_auth_invalid_config, "The Boot Configuration Data (BCD) settings have changed since BitLocker Drive Encryption was enabled. Code: 0x80310045" },	//	FVE_E_AUTH_INVALID_CONFIG
			{ com_errc::fve_e_fips_disable_protection_not_allowed, "The Group Policy setting requiring FIPS compliance prohibits the use of unencrypted keys, which prevents BitLocker from being suspended on this drive. Please contact your domain administrator for more information. Code: 0x80310046" },	//	FVE_E_FIPS_DISABLE_PROTECTION_NOT_ALLOWED
			{ com_errc::fve_e_fs_not_extended, "This drive cannot be encrypted by BitLocker Drive Encryption because the file system does not extend to the end of the drive. Repartition this drive and then try again. Code: 0x80310047" },	//	FVE_E_FS_NOT_EXTENDED
			{ com_errc::fve_e_firmware_type_not_supported, "BitLocker Drive Encryption cannot be enabled on the operating system drive. Contact the computer manufacturer for BIOS upgrade instructions. Code: 0x80310048" },	//	FVE_E_FIRMWARE_TYPE_NOT_SUPPORTED
			{ com_errc::fve_e_no_license, "This version of Windows does not include BitLocker Drive Encryption. To use BitLocker Drive Encryption, please upgrade the operating system. Code: 0x80310049" },	//	FVE_E_NO_LICENSE
			{ com_errc::fve_e_not_on_stack, "BitLocker Drive Encryption cannot be used because critical BitLocker system files are missing or corrupted. Use Windows Startup Repair to restore these files to your computer. Code: 0x8031004A" },	//	FVE_E_NOT_ON_STACK
			{ com_errc::fve_e_fs_mounted, "The drive cannot be locked when the drive is in use. Code: 0x8031004B" },	//	FVE_E_FS_MOUNTED
			{ com_errc::fve_e_token_not_impersonated, "The access token associated with the current thread is not an impersonated token. Code: 0x8031004C" },	//	FVE_E_TOKEN_NOT_IMPERSONATED
			{ com_errc::fve_e_dry_run_failed, "The BitLocker encryption key cannot be obtained. Verify that the Trusted Platform Module (TPM) is enabled and ownership has been taken. If this computer does not have a TPM, verify that the USB drive is inserted and available. Code: 0x8031004D" },	//	FVE_E_DRY_RUN_FAILED
			{ com_errc::fve_e_reboot_required, "You must restart your computer before continuing with BitLocker Drive Encryption. Code: 0x8031004E" },	//	FVE_E_REBOOT_REQUIRED
			{ com_errc::fve_e_debugger_enabled, "Drive encryption cannot occur while boot debugging is enabled. Use the bcdedit command-line tool to turn off boot debugging. Code: 0x8031004F" },	//	FVE_E_DEBUGGER_ENABLED
			{ com_errc::fve_e_raw_access, "No action was taken as BitLocker Drive Encryption is in raw access mode. Code: 0x80310050" },	//	FVE_E_RAW_ACCESS
			{ com_errc::fve_e_raw_blocked, "BitLocker Drive Encryption cannot enter raw access mode for this drive because the drive is currently in use. Code: 0x80310051" },	//	FVE_E_RAW_BLOCKED
			{ com_errc::fve_e_bcd_applications_path_incorrect, "The path specified in the Boot Configuration Data (BCD) for a BitLocker Drive Encryption integrity-protected application is incorrect. Please verify and correct your BCD settings and try again. Code: 0x80310052" },	//	FVE_E_BCD_APPLICATIONS_PATH_INCORRECT
			{ com_errc::fve_e_not_allowed_in_version, "BitLocker Drive Encryption can only be used for limited provisioning or recovery purposes when the computer is running in pre-installation or recovery environments. Code: 0x80310053" },	//	FVE_E_NOT_ALLOWED_IN_VERSION
			{ com_errc::fve_e_no_autounlock_master_key, "The auto-unlock master key was not available from the operating system drive. Code: 0x80310054" },	//	FVE_E_NO_AUTOUNLOCK_MASTER_KEY
			{ com_errc::fve_e_mor_failed, "The system firmware failed to enable clearing of system memory when the computer was restarted. Code: 0x80310055" },	//	FVE_E_MOR_FAILED
			{ com_errc::fve_e_hidden_volume, "The hidden drive cannot be encrypted. Code: 0x80310056" },	//	FVE_E_HIDDEN_VOLUME
			{ com_errc::fve_e_transient_state, "BitLocker encryption keys were ignored because the drive was in a transient state. Code: 0x80310057" },	//	FVE_E_TRANSIENT_STATE
			{ com_errc::fve_e_pubkey_not_allowed, "Public key based protectors are not allowed on this drive. Code: 0x80310058" },	//	FVE_E_PUBKEY_NOT_ALLOWED
			{ com_errc::fve_e_volume_handle_open, "BitLocker Drive Encryption is already performing an operation on this drive. Please complete all operations before continuing. Code: 0x80310059" },	//	FVE_E_VOLUME_HANDLE_OPEN
			{ com_errc::fve_e_no_feature_license, "This version of Windows does not support this feature of BitLocker Drive Encryption. To use this feature, upgrade the operating system. Code: 0x8031005A" },	//	FVE_E_NO_FEATURE_LICENSE
			{ com_errc::fve_e_invalid_startup_options, "The Group Policy settings for BitLocker startup options are in conflict and cannot be applied. Contact your system administrator for more information. Code: 0x8031005B" },	//	FVE_E_INVALID_STARTUP_OPTIONS
			{ com_errc::fve_e_policy_recovery_password_not_allowed, "Group Policy settings do not permit the creation of a recovery password. Code: 0x8031005C" },	//	FVE_E_POLICY_RECOVERY_PASSWORD_NOT_ALLOWED
			{ com_errc::fve_e_policy_recovery_password_required, "Group Policy settings require the creation of a recovery password. Code: 0x8031005D" },	//	FVE_E_POLICY_RECOVERY_PASSWORD_REQUIRED
			{ com_errc::fve_e_policy_recovery_key_not_allowed, "Group Policy settings do not permit the creation of a recovery key. Code: 0x8031005E" },	//	FVE_E_POLICY_RECOVERY_KEY_NOT_ALLOWED
			{ com_errc::fve_e_policy_recovery_key_required, "Group Policy settings require the creation of a recovery key. Code: 0x8031005F" },	//	FVE_E_POLICY_RECOVERY_KEY_REQUIRED
			{ com_errc::fve_e_policy_startup_pin_not_allowed, "Group Policy settings do not permit the use of a PIN at startup. Please choose a different BitLocker startup option. Code: 0x80310060" },	//	FVE_E_POLICY_STARTUP_PIN_NOT_ALLOWED
			{ com_errc::fve_e_policy_startup_pin_required, "Group Policy settings require the use of a PIN at startup. Please choose this BitLocker startup option. Code: 0x80310061" },	//	FVE_E_POLICY_STARTUP_PIN_REQUIRED
			{ com_errc::fve_e_policy_startup_key_not_allowed, "Group Policy settings do not permit the use of a startup key. Please choose a different BitLocker startup option. Code: 0x80310062" },	//	FVE_E_POLICY_STARTUP_KEY_NOT_ALLOWED
			{ com_errc::fve_e_policy_startup_key_required, "Group Policy settings require the use of a startup key. Please choose this BitLocker startup option. Code: 0x80310063" },	//	FVE_E_POLICY_STARTUP_KEY_REQUIRED
			{ com_errc::fve_e_policy_startup_pin_key_not_allowed, "Group Policy settings do not permit the use of a startup key and PIN. Please choose a different BitLocker startup option. Code: 0x80310064" },	//	FVE_E_POLICY_STARTUP_PIN_KEY_NOT_ALLOWED
			{ com_errc::fve_e_policy_startup_pin_key_required, "Group Policy settings require the use of a startup key and PIN. Please choose this BitLocker startup option. Code: 0x80310065" },	//	FVE_E_POLICY_STARTUP_PIN_KEY_REQUIRED
			{ com_errc::fve_e_policy_startup_tpm_not_allowed, "Group policy does not permit the use of TPM-only at startup. Please choose a different BitLocker startup option. Code: 0x80310066" },	//	FVE_E_POLICY_STARTUP_TPM_NOT_ALLOWED
			{ com_errc::fve_e_policy_startup_tpm_required, "Group Policy settings require the use of TPM-only at startup. Please choose this BitLocker startup option. Code: 0x80310067" },	//	FVE_E_POLICY_STARTUP_TPM_REQUIRED
			{ com_errc::fve_e_policy_invalid_pin_length, "The PIN provided does not meet minimum or maximum length requirements. Code: 0x80310068" },	//	FVE_E_POLICY_INVALID_PIN_LENGTH
			{ com_errc::fve_e_key_protector_not_supported, "The key protector is not supported by the version of BitLocker Drive Encryption currently on the drive. Upgrade the drive to add the key protector. Code: 0x80310069" },	//	FVE_E_KEY_PROTECTOR_NOT_SUPPORTED
			{ com_errc::fve_e_policy_passphrase_not_allowed, "Group Policy settings do not permit the creation of a password. Code: 0x8031006A" },	//	FVE_E_POLICY_PASSPHRASE_NOT_ALLOWED
			{ com_errc::fve_e_policy_passphrase_required, "Group Policy settings require the creation of a password. Code: 0x8031006B" },	//	FVE_E_POLICY_PASSPHRASE_REQUIRED
			{ com_errc::fve_e_fips_prevents_passphrase, "The Group Policy setting requiring FIPS compliance prevents passwords from being generated or used. Please contact your system administrator for more information. Code: 0x8031006C" },	//	FVE_E_FIPS_PREVENTS_PASSPHRASE
			{ com_errc::fve_e_os_volume_passphrase_not_allowed, "A password cannot be added to the operating system drive. Code: 0x8031006D" },	//	FVE_E_OS_VOLUME_PASSPHRASE_NOT_ALLOWED
			{ com_errc::fve_e_invalid_bitlocker_oid, "The BitLocker object identifier (OID) on the drive appears to be invalid or corrupt. Use manage-BDE to reset the OID on this drive. Code: 0x8031006E" },	//	FVE_E_INVALID_BITLOCKER_OID
			{ com_errc::fve_e_volume_too_small, "The drive is too small to be protected using BitLocker Drive Encryption. Code: 0x8031006F" },	//	FVE_E_VOLUME_TOO_SMALL
			{ com_errc::fve_e_dv_not_supported_on_fs, "The selected discovery drive type is incompatible with the file system on the drive. BitLocker To Go discovery drives must be created on FAT formatted drives. Code: 0x80310070" },	//	FVE_E_DV_NOT_SUPPORTED_ON_FS
			{ com_errc::fve_e_dv_not_allowed_by_gp, "The selected discovery drive type is not allowed by the computer's Group Policy settings. Verify that Group Policy settings allow the creation of discovery drives for use with BitLocker To Go. Code: 0x80310071" },	//	FVE_E_DV_NOT_ALLOWED_BY_GP
			{ com_errc::fve_e_policy_user_certificate_not_allowed, "Group Policy settings do not permit user certificates such as smart cards to be used with BitLocker Drive Encryption. Code: 0x80310072" },	//	FVE_E_POLICY_USER_CERTIFICATE_NOT_ALLOWED
			{ com_errc::fve_e_policy_user_certificate_required, "Group Policy settings require that you have a valid user certificate, such as a smart card, to be used with BitLocker Drive Encryption. Code: 0x80310073" },	//	FVE_E_POLICY_USER_CERTIFICATE_REQUIRED
			{ com_errc::fve_e_policy_user_cert_must_be_hw, "Group Policy settings requires that you use a smart card-based key protector with BitLocker Drive Encryption. Code: 0x80310074" },	//	FVE_E_POLICY_USER_CERT_MUST_BE_HW
			{ com_errc::fve_e_policy_user_configure_fdv_autounlock_not_allowed, "Group Policy settings do not permit BitLocker-protected fixed data drives to be automatically unlocked. Code: 0x80310075" },	//	FVE_E_POLICY_USER_CONFIGURE_FDV_AUTOUNLOCK_NOT_ALLOWED
			{ com_errc::fve_e_policy_user_configure_rdv_autounlock_not_allowed, "Group Policy settings do not permit BitLocker-protected removable data drives to be automatically unlocked. Code: 0x80310076" },	//	FVE_E_POLICY_USER_CONFIGURE_RDV_AUTOUNLOCK_NOT_ALLOWED
			{ com_errc::fve_e_policy_user_configure_rdv_not_allowed, "Group Policy settings do not permit you to configure BitLocker Drive Encryption on removable data drives. Code: 0x80310077" },	//	FVE_E_POLICY_USER_CONFIGURE_RDV_NOT_ALLOWED
			{ com_errc::fve_e_policy_user_enable_rdv_not_allowed, "Group Policy settings do not permit you to turn on BitLocker Drive Encryption on removable data drives. Please contact your system administrator if you need to turn on BitLocker. Code: 0x80310078" },	//	FVE_E_POLICY_USER_ENABLE_RDV_NOT_ALLOWED
			{ com_errc::fve_e_policy_user_disable_rdv_not_allowed, "Group Policy settings do not permit turning off BitLocker Drive Encryption on removable data drives. Please contact your system administrator if you need to turn off BitLocker. Code: 0x80310079" },	//	FVE_E_POLICY_USER_DISABLE_RDV_NOT_ALLOWED
			{ com_errc::fve_e_policy_invalid_passphrase_length, "Your password does not meet minimum password length requirements. By default, passwords must be at least 8 characters in length. Check with your system administrator for the password length requirement in your organization. Code: 0x80310080" },	//	FVE_E_POLICY_INVALID_PASSPHRASE_LENGTH
			{ com_errc::fve_e_policy_passphrase_too_simple, "Your password does not meet the complexity requirements set by your system administrator. Try adding upper and lowercase characters, numbers, and symbols. Code: 0x80310081" },	//	FVE_E_POLICY_PASSPHRASE_TOO_SIMPLE
			{ com_errc::fve_e_recovery_partition, "This drive cannot be encrypted because it is reserved for Windows System Recovery Options. Code: 0x80310082" },	//	FVE_E_RECOVERY_PARTITION
			{ com_errc::fve_e_policy_conflict_fdv_rk_off_auk_on, "BitLocker Drive Encryption cannot be applied to this drive because of conflicting Group Policy settings. BitLocker cannot be configured to automatically unlock fixed data drives when user recovery options are disabled. If you want BitLocker-protected fixed data drives to be automatically unlocked after key validation has occurred, please ask your system administrator to resolve the settings conflict before enabling BitLocker. Code: 0x80310083" },	//	FVE_E_POLICY_CONFLICT_FDV_RK_OFF_AUK_ON
			{ com_errc::fve_e_policy_conflict_rdv_rk_off_auk_on, "BitLocker Drive Encryption cannot be applied to this drive because of conflicting Group Policy settings. BitLocker cannot be configured to automatically unlock removable data drives when user recovery option are disabled. If you want BitLocker-protected removable data drives to be automatically unlocked after key validation has occurred, please ask your system administrator to resolve the settings conflict before enabling BitLocker. Code: 0x80310084" },	//	FVE_E_POLICY_CONFLICT_RDV_RK_OFF_AUK_ON
			{ com_errc::fve_e_non_bitlocker_oid, "The Enhanced Key Usage (EKU) attribute of the specified certificate does not permit it to be used for BitLocker Drive Encryption. BitLocker does not require that a certificate have an EKU attribute, but if one is configured it must be set to an object identifier (OID) that matches the OID configured for BitLocker. Code: 0x80310085" },	//	FVE_E_NON_BITLOCKER_OID
			{ com_errc::fve_e_policy_prohibits_selfsigned, "BitLocker Drive Encryption cannot be applied to this drive as currently configured because of Group Policy settings. The certificate you provided for drive encryption is self-signed. Current Group Policy settings do not permit the use of self-signed certificates. Obtain a new certificate from your certification authority before attempting to enable BitLocker. Code: 0x80310086" },	//	FVE_E_POLICY_PROHIBITS_SELFSIGNED
			{ com_errc::fve_e_policy_conflict_ro_and_startup_key_required, "BitLocker Encryption cannot be applied to this drive because of conflicting Group Policy settings. When write access to drives not protected by BitLocker is denied, the use of a USB startup key cannot be required. Please have your system administrator resolve these policy conflicts before attempting to enable BitLocker. Code: 0x80310087" },	//	FVE_E_POLICY_CONFLICT_RO_AND_STARTUP_KEY_REQUIRED
			{ com_errc::fve_e_conv_recovery_failed, "BitLocker Drive Encryption failed to recover from an abruptly terminated conversion. This could be due to either all conversion logs being corrupted or the media being write-protected. Code: 0x80310088" },	//	FVE_E_CONV_RECOVERY_FAILED
			{ com_errc::fve_e_virtualized_space_too_big, "The requested virtualization size is too big. Code: 0x80310089" },	//	FVE_E_VIRTUALIZED_SPACE_TOO_BIG
			{ com_errc::fve_e_policy_conflict_osv_rp_off_adb_on, "BitLocker Drive Encryption cannot be applied to this drive because there are conflicting Group Policy settings for recovery options on operating system drives. Storing recovery information to Active Directory Domain Services cannot be required when the generation of recovery passwords is not permitted. Please have your system administrator resolve these policy conflicts before attempting to enable BitLocker. Code: 0x80310090" },	//	FVE_E_POLICY_CONFLICT_OSV_RP_OFF_ADB_ON
			{ com_errc::fve_e_policy_conflict_fdv_rp_off_adb_on, "BitLocker Drive Encryption cannot be applied to this drive because there are conflicting Group Policy settings for recovery options on fixed data drives. Storing recovery information to Active Directory Domain Services cannot be required when the generation of recovery passwords is not permitted. Please have your system administrator resolve these policy conflicts before attempting to enable BitLocker. Code: 0x80310091" },	//	FVE_E_POLICY_CONFLICT_FDV_RP_OFF_ADB_ON
			{ com_errc::fve_e_policy_conflict_rdv_rp_off_adb_on, "BitLocker Drive Encryption cannot be applied to this drive because there are conflicting Group Policy settings for recovery options on removable data drives. Storing recovery information to Active Directory Domain Services cannot be required when the generation of recovery passwords is not permitted. Please have your system administrator resolve these policy conflicts before attempting to enable BitLocker. Code: 0x80310092" },	//	FVE_E_POLICY_CONFLICT_RDV_RP_OFF_ADB_ON
			{ com_errc::fve_e_non_bitlocker_ku, "The Key Usage (KU) attribute of the specified certificate does not permit it to be used for BitLocker Drive Encryption. BitLocker does not require that a certificate have a KU attribute, but if one is configured it must be set to either Key Encipherment or Key Agreement. Code: 0x80310093" },	//	FVE_E_NON_BITLOCKER_KU
			{ com_errc::fve_e_privatekey_auth_failed, "The private key associated with the specified certificate cannot be authorized. The private key authorization was either not provided or the provided authorization was invalid. Code: 0x80310094" },	//	FVE_E_PRIVATEKEY_AUTH_FAILED
			{ com_errc::fve_e_removal_of_dra_failed, "Removal of the data recovery agent certificate must be done using the Certificates snap-in. Code: 0x80310095" },	//	FVE_E_REMOVAL_OF_DRA_FAILED
			{ com_errc::fve_e_operation_not_supported_on_vista_volume, "This drive was encrypted using the version of BitLocker Drive Encryption included with Windows Vista and Windows Server 2008 which does not support organizational identifiers. To specify organizational identifiers for this drive upgrade the drive encryption to the latest version using the \"manage-bde -upgrade\" command. Code: 0x80310096" },	//	FVE_E_OPERATION_NOT_SUPPORTED_ON_VISTA_VOLUME
			{ com_errc::fve_e_cant_lock_autounlock_enabled_volume, "The drive cannot be locked because it is automatically unlocked on this computer.  Remove the automatic unlock protector to lock this drive. Code: 0x80310097" },	//	FVE_E_CANT_LOCK_AUTOUNLOCK_ENABLED_VOLUME
			{ com_errc::fve_e_fips_hash_kdf_not_allowed, "The default BitLocker Key Derivation Function SP800-56A for ECC smart cards is not supported by your smart card. The Group Policy setting requiring FIPS-compliance prevents BitLocker from using any other key derivation function for encryption. You have to use a FIPS compliant smart card in FIPS restricted environments. Code: 0x80310098" },	//	FVE_E_FIPS_HASH_KDF_NOT_ALLOWED
			{ com_errc::fve_e_enh_pin_invalid, "The BitLocker encryption key could not be obtained from the Trusted Platform Module (TPM) and enhanced PIN. Try using a PIN containing only numerals. Code: 0x80310099" },	//	FVE_E_ENH_PIN_INVALID
			{ com_errc::fve_e_invalid_pin_chars, "The requested TPM PIN contains invalid characters. Code: 0x8031009A" },	//	FVE_E_INVALID_PIN_CHARS
			{ com_errc::fve_e_invalid_datum_type, "The management information stored on the drive contained an unknown type. If you are using an old version of Windows, try accessing the drive from the latest version. Code: 0x8031009B" },	//	FVE_E_INVALID_DATUM_TYPE
			{ com_errc::fve_e_efi_only, "The feature is only supported on EFI systems. Code: 0x8031009C" },	//	FVE_E_EFI_ONLY
			{ com_errc::fve_e_multiple_nkp_certs, "More than one Network Key Protector certificate has been found on the system. Code: 0x8031009D" },	//	FVE_E_MULTIPLE_NKP_CERTS
			{ com_errc::fve_e_removal_of_nkp_failed, "Removal of the Network Key Protector certificate must be done using the Certificates snap-in. Code: 0x8031009E" },	//	FVE_E_REMOVAL_OF_NKP_FAILED
			{ com_errc::fve_e_invalid_nkp_cert, "An invalid certificate has been found in the Network Key Protector certificate store. Code: 0x8031009F" },	//	FVE_E_INVALID_NKP_CERT
			{ com_errc::fve_e_no_existing_pin, "This drive isn't protected with a PIN. Code: 0x803100A0" },	//	FVE_E_NO_EXISTING_PIN
			{ com_errc::fve_e_protector_change_pin_mismatch, "Please enter the correct current PIN. Code: 0x803100A1" },	//	FVE_E_PROTECTOR_CHANGE_PIN_MISMATCH
			{ com_errc::fve_e_pin_protector_change_by_std_user_disallowed, "You must be logged on with an administrator account to change the PIN. Click the link to reset the PIN as an administrator. Code: 0x803100A2" },	//	FVE_E_PIN_PROTECTOR_CHANGE_BY_STD_USER_DISALLOWED
			{ com_errc::fve_e_protector_change_max_pin_change_attempts_reached, "BitLocker has disabled PIN changes after too many failed requests. Click the link to reset the PIN as an administrator. Code: 0x803100A3" },	//	FVE_E_PROTECTOR_CHANGE_MAX_PIN_CHANGE_ATTEMPTS_REACHED
			{ com_errc::fve_e_policy_passphrase_requires_ascii, "Your system administrator requires that passwords contain only printable ASCII characters. This includes unaccented letters (A-Z, a-z), numbers (0-9), space, arithmetic signs, common punctuation, separators, and the following symbols: # $ & @ ^ _ ~ . Code: 0x803100A4" },	//	FVE_E_POLICY_PASSPHRASE_REQUIRES_ASCII
			{ com_errc::fve_e_full_encryption_not_allowed_on_tp_storage, "BitLocker Drive Encryption only supports Used Space Only encryption on thin provisioned storage. Code: 0x803100A5" },	//	FVE_E_FULL_ENCRYPTION_NOT_ALLOWED_ON_TP_STORAGE
			{ com_errc::fve_e_wipe_not_allowed_on_tp_storage, "BitLocker Drive Encryption does not support wiping free space on thin provisioned storage. Code: 0x803100A6" },	//	FVE_E_WIPE_NOT_ALLOWED_ON_TP_STORAGE
			{ com_errc::fve_e_key_length_not_supported_by_edrive, "The required authentication key length is not supported by the drive. Code: 0x803100A7" },	//	FVE_E_KEY_LENGTH_NOT_SUPPORTED_BY_EDRIVE
			{ com_errc::fve_e_no_existing_passphrase, "This drive isn't protected with a password. Code: 0x803100A8" },	//	FVE_E_NO_EXISTING_PASSPHRASE
			{ com_errc::fve_e_protector_change_passphrase_mismatch, "Please enter the correct current password. Code: 0x803100A9" },	//	FVE_E_PROTECTOR_CHANGE_PASSPHRASE_MISMATCH
			{ com_errc::fve_e_passphrase_too_long, "The password cannot exceed 256 characters. Code: 0x803100AA" },	//	FVE_E_PASSPHRASE_TOO_LONG
			{ com_errc::fve_e_no_passphrase_with_tpm, "A password key protector cannot be added because a TPM protector exists on the drive. Code: 0x803100AB" },	//	FVE_E_NO_PASSPHRASE_WITH_TPM
			{ com_errc::fve_e_no_tpm_with_passphrase, "A TPM key protector cannot be added because a password protector exists on the drive. Code: 0x803100AC" },	//	FVE_E_NO_TPM_WITH_PASSPHRASE
			{ com_errc::fve_e_not_allowed_on_csv_stack, "This command can only be performed from the coordinator node for the specified CSV volume. Code: 0x803100AD" },	//	FVE_E_NOT_ALLOWED_ON_CSV_STACK
			{ com_errc::fve_e_not_allowed_on_cluster, "This command cannot be performed on a volume when it is part of a cluster. Code: 0x803100AE" },	//	FVE_E_NOT_ALLOWED_ON_CLUSTER
			{ com_errc::fve_e_edrive_no_failover_to_sw, "BitLocker did not revert to using BitLocker software encryption due to group policy configuration. Code: 0x803100AF" },	//	FVE_E_EDRIVE_NO_FAILOVER_TO_SW
			{ com_errc::fve_e_edrive_band_in_use, "The drive cannot be managed by BitLocker because the drive's hardware encryption feature is already in use. Code: 0x803100B0" },	//	FVE_E_EDRIVE_BAND_IN_USE
			{ com_errc::fve_e_edrive_disallowed_by_gp, "Group Policy settings do not allow the use of hardware-based encryption. Code: 0x803100B1" },	//	FVE_E_EDRIVE_DISALLOWED_BY_GP
			{ com_errc::fve_e_edrive_incompatible_volume, "The drive specified does not support hardware-based encryption. Code: 0x803100B2" },	//	FVE_E_EDRIVE_INCOMPATIBLE_VOLUME
			{ com_errc::fve_e_not_allowed_to_upgrade_while_converting, "BitLocker cannot be upgraded during disk encryption or decryption. Code: 0x803100B3" },	//	FVE_E_NOT_ALLOWED_TO_UPGRADE_WHILE_CONVERTING
			{ com_errc::fve_e_edrive_dv_not_supported, "Discovery Volumes are not supported for volumes using hardware encryption. Code: 0x803100B4" },	//	FVE_E_EDRIVE_DV_NOT_SUPPORTED
			{ com_errc::fve_e_no_preboot_keyboard_detected, "No pre-boot keyboard detected. The user may not be able to provide required input to unlock the volume. Code: 0x803100B5" },	//	FVE_E_NO_PREBOOT_KEYBOARD_DETECTED
			{ com_errc::fve_e_no_preboot_keyboard_or_winre_detected, "No pre-boot keyboard or Windows Recovery Environment detected. The user may not be able to provide required input to unlock the volume. Code: 0x803100B6" },	//	FVE_E_NO_PREBOOT_KEYBOARD_OR_WINRE_DETECTED
			{ com_errc::fve_e_policy_requires_startup_pin_on_touch_device, "Group Policy settings require the creation of a startup PIN, but a pre-boot keyboard is not available on this device. The user may not be able to provide required input to unlock the volume. Code: 0x803100B7" },	//	FVE_E_POLICY_REQUIRES_STARTUP_PIN_ON_TOUCH_DEVICE
			{ com_errc::fve_e_policy_requires_recovery_password_on_touch_device, "Group Policy settings require the creation of a recovery password, but neither a pre-boot keyboard nor Windows Recovery Environment is available on this device. The user may not be able to provide required input to unlock the volume. Code: 0x803100B8" },	//	FVE_E_POLICY_REQUIRES_RECOVERY_PASSWORD_ON_TOUCH_DEVICE
			{ com_errc::fve_e_wipe_cancel_not_applicable, "Wipe of free space is not currently taking place. Code: 0x803100B9" },	//	FVE_E_WIPE_CANCEL_NOT_APPLICABLE
			{ com_errc::fve_e_secureboot_disabled, "BitLocker cannot use Secure Boot for platform integrity because Secure Boot has been disabled. Code: 0x803100BA" },	//	FVE_E_SECUREBOOT_DISABLED
			{ com_errc::fve_e_secureboot_configuration_invalid, "BitLocker cannot use Secure Boot for platform integrity because the Secure Boot configuration does not meet the requirements for BitLocker. Code: 0x803100BB" },	//	FVE_E_SECUREBOOT_CONFIGURATION_INVALID
			{ com_errc::fve_e_edrive_dry_run_failed, "Your computer doesn't support BitLocker hardware-based encryption. Check with your computer manufacturer for firmware updates. Code: 0x803100BC" },	//	FVE_E_EDRIVE_DRY_RUN_FAILED
			{ com_errc::fve_e_shadow_copy_present, "BitLocker cannot be enabled on the volume because it contains a Volume Shadow Copy. Remove all Volume Shadow Copies before encrypting the volume. Code: 0x803100BD" },	//	FVE_E_SHADOW_COPY_PRESENT
			{ com_errc::fve_e_policy_invalid_enhanced_bcd_settings, "BitLocker Drive Encryption cannot be applied to this drive because the Group Policy setting for Enhanced Boot Configuration Data contains invalid data. Please have your system administrator resolve this invalid configuration before attempting to enable BitLocker. Code: 0x803100BE" },	//	FVE_E_POLICY_INVALID_ENHANCED_BCD_SETTINGS
			{ com_errc::fve_e_edrive_incompatible_firmware, "This PC's firmware is not capable of supporting hardware encryption. Code: 0x803100BF" },	//	FVE_E_EDRIVE_INCOMPATIBLE_FIRMWARE
			{ com_errc::fve_e_protector_change_max_passphrase_change_attempts_reached, "BitLocker has disabled password changes after too many failed requests. Click the link to reset the password as an administrator. Code: 0x803100C0" },	//	FVE_E_PROTECTOR_CHANGE_MAX_PASSPHRASE_CHANGE_ATTEMPTS_REACHED
			{ com_errc::fve_e_passphrase_protector_change_by_std_user_disallowed, "You must be logged on with an administrator account to change the password. Click the link to reset the password as an administrator. Code: 0x803100C1" },	//	FVE_E_PASSPHRASE_PROTECTOR_CHANGE_BY_STD_USER_DISALLOWED
			{ com_errc::fve_e_liveid_account_suspended, "BitLocker cannot save the recovery password because the specified Microsoft account is Suspended. Code: 0x803100C2" },	//	FVE_E_LIVEID_ACCOUNT_SUSPENDED
			{ com_errc::fve_e_liveid_account_blocked, "BitLocker cannot save the recovery password because the specified Microsoft account is Blocked. Code: 0x803100C3" },	//	FVE_E_LIVEID_ACCOUNT_BLOCKED
			{ com_errc::fve_e_not_provisioned_on_all_volumes, "This PC is not provisioned to support device encryption. Please enable BitLocker on all volumes to comply with device encryption policy. Code: 0x803100C4" },	//	FVE_E_NOT_PROVISIONED_ON_ALL_VOLUMES
			{ com_errc::fve_e_de_fixed_data_not_supported, "This PC cannot support device encryption because unencrypted fixed data volumes are present. Code: 0x803100C5" },	//	FVE_E_DE_FIXED_DATA_NOT_SUPPORTED
			{ com_errc::fve_e_de_hardware_not_compliant, "This PC does not meet the hardware requirements to support device encryption. Code: 0x803100C6" },	//	FVE_E_DE_HARDWARE_NOT_COMPLIANT
			{ com_errc::fve_e_de_winre_not_configured, "This PC cannot support device encryption because WinRE is not properly configured. Code: 0x803100C7" },	//	FVE_E_DE_WINRE_NOT_CONFIGURED
			{ com_errc::fve_e_de_protection_suspended, "Protection is enabled on the volume but has been suspended. This is likely to have happened due to an update being applied to your system. Please try again after a reboot. Code: 0x803100C8" },	//	FVE_E_DE_PROTECTION_SUSPENDED
			{ com_errc::fve_e_de_os_volume_not_protected, "This PC is not provisioned to support device encryption. Code: 0x803100C9" },	//	FVE_E_DE_OS_VOLUME_NOT_PROTECTED
			{ com_errc::fve_e_de_device_lockedout, "Device Lock has been triggered due to too many incorrect password attempts. Code: 0x803100CA" },	//	FVE_E_DE_DEVICE_LOCKEDOUT
			{ com_errc::fve_e_de_protection_not_yet_enabled, "Protection has not been enabled on the volume. Enabling protection requires a connected account. If you already have a connected account and are seeing this error, please refer to the event log for more information. Code: 0x803100CB" },	//	FVE_E_DE_PROTECTION_NOT_YET_ENABLED
			{ com_errc::fve_e_invalid_pin_chars_detailed, "Your PIN can only contain numbers from 0 to 9. Code: 0x803100CC" },	//	FVE_E_INVALID_PIN_CHARS_DETAILED
			{ com_errc::fve_e_device_lockout_counter_unavailable, "BitLocker cannot use hardware replay protection because no counter is available on your PC. Code: 0x803100CD" },	//	FVE_E_DEVICE_LOCKOUT_COUNTER_UNAVAILABLE
			{ com_errc::fve_e_devicelockout_counter_mismatch, "Device Lockout state validation failed due to counter mismatch. Code: 0x803100CE" },	//	FVE_E_DEVICELOCKOUT_COUNTER_MISMATCH
			{ com_errc::fve_e_buffer_too_large, "The input buffer is too large. Code: 0x803100CF" },	//	FVE_E_BUFFER_TOO_LARGE
			{ com_errc::fve_e_no_such_capability_on_target, "The target of an invocation does not support requested capability. Code: 0x803100D0" },	//	FVE_E_NO_SUCH_CAPABILITY_ON_TARGET
			{ com_errc::fve_e_de_prevented_for_os, "Device encryption is currently blocked by this PC's configuration. Code: 0x803100D1" },	//	FVE_E_DE_PREVENTED_FOR_OS
			{ com_errc::fve_e_de_volume_opted_out, "This drive has been opted out of device encryption. Code: 0x803100D2" },	//	FVE_E_DE_VOLUME_OPTED_OUT
			{ com_errc::fve_e_de_volume_not_supported, "Device encryption isn't available for this drive. Code: 0x803100D3" },	//	FVE_E_DE_VOLUME_NOT_SUPPORTED
			{ com_errc::fve_e_eow_not_supported_in_version, "The encrypt on write mode for BitLocker is not supported in this version of Windows. You can turn on BitLocker without using the encrypt on write mode. Code: 0x803100D4" },	//	FVE_E_EOW_NOT_SUPPORTED_IN_VERSION
			{ com_errc::fve_e_adbackup_not_enabled, "Group policy prevents you from backing up your recovery password to Active Directory for this drive type. For more info, contact your system administrator. Code: 0x803100D5" },	//	FVE_E_ADBACKUP_NOT_ENABLED
			{ com_errc::fve_e_volume_extend_prevents_eow_decrypt, "Device encryption can't be turned off while this drive is being encrypted. Please try again later. Code: 0x803100D6" },	//	FVE_E_VOLUME_EXTEND_PREVENTS_EOW_DECRYPT
			{ com_errc::fve_e_not_de_volume, "This action isn't supported because this drive isn't automatically managed with device encryption. Code: 0x803100D7" },	//	FVE_E_NOT_DE_VOLUME
			{ com_errc::fve_e_protection_cannot_be_disabled, "BitLocker can't be suspended on this drive until the next restart. Code: 0x803100D8" },	//	FVE_E_PROTECTION_CANNOT_BE_DISABLED
			{ com_errc::fwp_e_callout_not_found, "The callout does not exist. Code: 0x80320001" },	//	FWP_E_CALLOUT_NOT_FOUND
			{ com_errc::fwp_e_condition_not_found, "The filter condition does not exist. Code: 0x80320002" },	//	FWP_E_CONDITION_NOT_FOUND
			{ com_errc::fwp_e_filter_not_found, "The filter does not exist. Code: 0x80320003" },	//	FWP_E_FILTER_NOT_FOUND
			{ com_errc::fwp_e_layer_not_found, "The layer does not exist. Code: 0x80320004" },	//	FWP_E_LAYER_NOT_FOUND
			{ com_errc::fwp_e_provider_not_found, "The provider does not exist. Code: 0x80320005" },	//	FWP_E_PROVIDER_NOT_FOUND
			{ com_errc::fwp_e_provider_context_not_found, "The provider context does not exist. Code: 0x80320006" },	//	FWP_E_PROVIDER_CONTEXT_NOT_FOUND
			{ com_errc::fwp_e_sublayer_not_found, "The sublayer does not exist. Code: 0x80320007" },	//	FWP_E_SUBLAYER_NOT_FOUND
			{ com_errc::fwp_e_not_found, "The object does not exist. Code: 0x80320008" },	//	FWP_E_NOT_FOUND
			{ com_errc::fwp_e_already_exists, "An object with that GUID or LUID already exists. Code: 0x80320009" },	//	FWP_E_ALREADY_EXISTS
			{ com_errc::fwp_e_in_use, "The object is referenced by other objects so cannot be deleted. Code: 0x8032000A" },	//	FWP_E_IN_USE
			{ com_errc::fwp_e_dynamic_session_in_progress, "The call is not allowed from within a dynamic session. Code: 0x8032000B" },	//	FWP_E_DYNAMIC_SESSION_IN_PROGRESS
			{ com_errc::fwp_e_wrong_session, "The call was made from the wrong session so cannot be completed. Code: 0x8032000C" },	//	FWP_E_WRONG_SESSION
			{ com_errc::fwp_e_no_txn_in_progress, "The call must be made from within an explicit transaction. Code: 0x8032000D" },	//	FWP_E_NO_TXN_IN_PROGRESS
			{ com_errc::fwp_e_txn_in_progress, "The call is not allowed from within an explicit transaction. Code: 0x8032000E" },	//	FWP_E_TXN_IN_PROGRESS
			{ com_errc::fwp_e_txn_aborted, "The explicit transaction has been forcibly cancelled. Code: 0x8032000F" },	//	FWP_E_TXN_ABORTED
			{ com_errc::fwp_e_session_aborted, "The session has been cancelled. Code: 0x80320010" },	//	FWP_E_SESSION_ABORTED
			{ com_errc::fwp_e_incompatible_txn, "The call is not allowed from within a read-only transaction. Code: 0x80320011" },	//	FWP_E_INCOMPATIBLE_TXN
			{ com_errc::fwp_e_timeout, "The call timed out while waiting to acquire the transaction lock. Code: 0x80320012" },	//	FWP_E_TIMEOUT
			{ com_errc::fwp_e_net_events_disabled, "Collection of network diagnostic events is disabled. Code: 0x80320013" },	//	FWP_E_NET_EVENTS_DISABLED
			{ com_errc::fwp_e_incompatible_layer, "The operation is not supported by the specified layer. Code: 0x80320014" },	//	FWP_E_INCOMPATIBLE_LAYER
			{ com_errc::fwp_e_km_clients_only, "The call is allowed for kernel-mode callers only. Code: 0x80320015" },	//	FWP_E_KM_CLIENTS_ONLY
			{ com_errc::fwp_e_lifetime_mismatch, "The call tried to associate two objects with incompatible lifetimes. Code: 0x80320016" },	//	FWP_E_LIFETIME_MISMATCH
			{ com_errc::fwp_e_builtin_object, "The object is built in so cannot be deleted. Code: 0x80320017" },	//	FWP_E_BUILTIN_OBJECT
			{ com_errc::fwp_e_too_many_callouts, "The maximum number of callouts has been reached. Code: 0x80320018" },	//	FWP_E_TOO_MANY_CALLOUTS
			{ com_errc::fwp_e_notification_dropped, "A notification could not be delivered because a message queue is at its maximum capacity. Code: 0x80320019" },	//	FWP_E_NOTIFICATION_DROPPED
			{ com_errc::fwp_e_traffic_mismatch, "The traffic parameters do not match those for the security association context. Code: 0x8032001A" },	//	FWP_E_TRAFFIC_MISMATCH
			{ com_errc::fwp_e_incompatible_sa_state, "The call is not allowed for the current security association state. Code: 0x8032001B" },	//	FWP_E_INCOMPATIBLE_SA_STATE
			{ com_errc::fwp_e_null_pointer, "A required pointer is null. Code: 0x8032001C" },	//	FWP_E_NULL_POINTER
			{ com_errc::fwp_e_invalid_enumerator, "An enumerator is not valid. Code: 0x8032001D" },	//	FWP_E_INVALID_ENUMERATOR
			{ com_errc::fwp_e_invalid_flags, "The flags field contains an invalid value. Code: 0x8032001E" },	//	FWP_E_INVALID_FLAGS
			{ com_errc::fwp_e_invalid_net_mask, "A network mask is not valid. Code: 0x8032001F" },	//	FWP_E_INVALID_NET_MASK
			{ com_errc::fwp_e_invalid_range, "An FWP_RANGE is not valid. Code: 0x80320020" },	//	FWP_E_INVALID_RANGE
			{ com_errc::fwp_e_invalid_interval, "The time interval is not valid. Code: 0x80320021" },	//	FWP_E_INVALID_INTERVAL
			{ com_errc::fwp_e_zero_length_array, "An array that must contain at least one element is zero length. Code: 0x80320022" },	//	FWP_E_ZERO_LENGTH_ARRAY
			{ com_errc::fwp_e_null_display_name, "The displayData.name field cannot be null. Code: 0x80320023" },	//	FWP_E_NULL_DISPLAY_NAME
			{ com_errc::fwp_e_invalid_action_type, "The action type is not one of the allowed action types for a filter. Code: 0x80320024" },	//	FWP_E_INVALID_ACTION_TYPE
			{ com_errc::fwp_e_invalid_weight, "The filter weight is not valid. Code: 0x80320025" },	//	FWP_E_INVALID_WEIGHT
			{ com_errc::fwp_e_match_type_mismatch, "A filter condition contains a match type that is not compatible with the operands. Code: 0x80320026" },	//	FWP_E_MATCH_TYPE_MISMATCH
			{ com_errc::fwp_e_type_mismatch, "An FWP_VALUE or FWPM_CONDITION_VALUE is of the wrong type. Code: 0x80320027" },	//	FWP_E_TYPE_MISMATCH
			{ com_errc::fwp_e_out_of_bounds, "An integer value is outside the allowed range. Code: 0x80320028" },	//	FWP_E_OUT_OF_BOUNDS
			{ com_errc::fwp_e_reserved, "A reserved field is non-zero. Code: 0x80320029" },	//	FWP_E_RESERVED
			{ com_errc::fwp_e_duplicate_condition, "A filter cannot contain multiple conditions operating on a single field. Code: 0x8032002A" },	//	FWP_E_DUPLICATE_CONDITION
			{ com_errc::fwp_e_duplicate_keymod, "A policy cannot contain the same keying module more than once. Code: 0x8032002B" },	//	FWP_E_DUPLICATE_KEYMOD
			{ com_errc::fwp_e_action_incompatible_with_layer, "The action type is not compatible with the layer. Code: 0x8032002C" },	//	FWP_E_ACTION_INCOMPATIBLE_WITH_LAYER
			{ com_errc::fwp_e_action_incompatible_with_sublayer, "The action type is not compatible with the sublayer. Code: 0x8032002D" },	//	FWP_E_ACTION_INCOMPATIBLE_WITH_SUBLAYER
			{ com_errc::fwp_e_context_incompatible_with_layer, "The raw context or the provider context is not compatible with the layer. Code: 0x8032002E" },	//	FWP_E_CONTEXT_INCOMPATIBLE_WITH_LAYER
			{ com_errc::fwp_e_context_incompatible_with_callout, "The raw context or the provider context is not compatible with the callout. Code: 0x8032002F" },	//	FWP_E_CONTEXT_INCOMPATIBLE_WITH_CALLOUT
			{ com_errc::fwp_e_incompatible_auth_method, "The authentication method is not compatible with the policy type. Code: 0x80320030" },	//	FWP_E_INCOMPATIBLE_AUTH_METHOD
			{ com_errc::fwp_e_incompatible_dh_group, "The Diffie-Hellman group is not compatible with the policy type. Code: 0x80320031" },	//	FWP_E_INCOMPATIBLE_DH_GROUP
			{ com_errc::fwp_e_em_not_supported, "An IKE policy cannot contain an Extended Mode policy. Code: 0x80320032" },	//	FWP_E_EM_NOT_SUPPORTED
			{ com_errc::fwp_e_never_match, "The enumeration template or subscription will never match any objects. Code: 0x80320033" },	//	FWP_E_NEVER_MATCH
			{ com_errc::fwp_e_provider_context_mismatch, "The provider context is of the wrong type. Code: 0x80320034" },	//	FWP_E_PROVIDER_CONTEXT_MISMATCH
			{ com_errc::fwp_e_invalid_parameter, "The parameter is incorrect. Code: 0x80320035" },	//	FWP_E_INVALID_PARAMETER
			{ com_errc::fwp_e_too_many_sublayers, "The maximum number of sublayers has been reached. Code: 0x80320036" },	//	FWP_E_TOO_MANY_SUBLAYERS
			{ com_errc::fwp_e_callout_notification_failed, "The notification function for a callout returned an error. Code: 0x80320037" },	//	FWP_E_CALLOUT_NOTIFICATION_FAILED
			{ com_errc::fwp_e_invalid_auth_transform, "The IPsec authentication transform is not valid. Code: 0x80320038" },	//	FWP_E_INVALID_AUTH_TRANSFORM
			{ com_errc::fwp_e_invalid_cipher_transform, "The IPsec cipher transform is not valid. Code: 0x80320039" },	//	FWP_E_INVALID_CIPHER_TRANSFORM
			{ com_errc::fwp_e_incompatible_cipher_transform, "The IPsec cipher transform is not compatible with the policy. Code: 0x8032003A" },	//	FWP_E_INCOMPATIBLE_CIPHER_TRANSFORM
			{ com_errc::fwp_e_invalid_transform_combination, "The combination of IPsec transform types is not valid. Code: 0x8032003B" },	//	FWP_E_INVALID_TRANSFORM_COMBINATION
			{ com_errc::fwp_e_duplicate_auth_method, "A policy cannot contain the same auth method more than once. Code: 0x8032003C" },	//	FWP_E_DUPLICATE_AUTH_METHOD
			{ com_errc::fwp_e_invalid_tunnel_endpoint, "A tunnel endpoint configuration is invalid. Code: 0x8032003D" },	//	FWP_E_INVALID_TUNNEL_ENDPOINT
			{ com_errc::fwp_e_l2_driver_not_ready, "The WFP MAC Layers are not ready. Code: 0x8032003E" },	//	FWP_E_L2_DRIVER_NOT_READY
			{ com_errc::fwp_e_key_dictator_already_registered, "A key manager capable of key dictation is already registered. Code: 0x8032003F" },	//	FWP_E_KEY_DICTATOR_ALREADY_REGISTERED
			{ com_errc::fwp_e_key_dictation_invalid_keying_material, "A key manager dictated invalid keys. Code: 0x80320040" },	//	FWP_E_KEY_DICTATION_INVALID_KEYING_MATERIAL
			{ com_errc::fwp_e_connections_disabled, "The BFE IPsec Connection Tracking is disabled. Code: 0x80320041" },	//	FWP_E_CONNECTIONS_DISABLED
			{ com_errc::fwp_e_invalid_dns_name, "The DNS name is invalid. Code: 0x80320042" },	//	FWP_E_INVALID_DNS_NAME
			{ com_errc::fwp_e_still_on, "The engine option is still enabled due to other configuration settings. Code: 0x80320043" },	//	FWP_E_STILL_ON
			{ com_errc::fwp_e_ikeext_not_running, "The IKEEXT service is not running.  This service only runs when there is IPsec policy applied to the machine. Code: 0x80320044" },	//	FWP_E_IKEEXT_NOT_RUNNING
			{ com_errc::fwp_e_drop_noicmp, "The packet should be dropped, no ICMP should be sent. Code: 0x80320104" },	//	FWP_E_DROP_NOICMP
			{ com_errc::ws_s_async, "The function call is completing asynchronously. Code: 0x003D0000" },	//	WS_S_ASYNC
			{ com_errc::ws_s_end, "There are no more messages available on the channel. Code: 0x003D0001" },	//	WS_S_END
			{ com_errc::ws_e_invalid_format, "The input data was not in the expected format or did not have the expected value. Code: 0x803D0000" },	//	WS_E_INVALID_FORMAT
			{ com_errc::ws_e_object_faulted, "The operation could not be completed because the object is in a faulted state due to a previous error. Code: 0x803D0001" },	//	WS_E_OBJECT_FAULTED
			{ com_errc::ws_e_numeric_overflow, "The operation could not be completed because it would lead to numeric overflow. Code: 0x803D0002" },	//	WS_E_NUMERIC_OVERFLOW
			{ com_errc::ws_e_invalid_operation, "The operation is not allowed due to the current state of the object. Code: 0x803D0003" },	//	WS_E_INVALID_OPERATION
			{ com_errc::ws_e_operation_aborted, "The operation was aborted. Code: 0x803D0004" },	//	WS_E_OPERATION_ABORTED
			{ com_errc::ws_e_endpoint_access_denied, "Access was denied by the remote endpoint. Code: 0x803D0005" },	//	WS_E_ENDPOINT_ACCESS_DENIED
			{ com_errc::ws_e_operation_timed_out, "The operation did not complete within the time allotted. Code: 0x803D0006" },	//	WS_E_OPERATION_TIMED_OUT
			{ com_errc::ws_e_operation_abandoned, "The operation was abandoned. Code: 0x803D0007" },	//	WS_E_OPERATION_ABANDONED
			{ com_errc::ws_e_quota_exceeded, "A quota was exceeded. Code: 0x803D0008" },	//	WS_E_QUOTA_EXCEEDED
			{ com_errc::ws_e_no_translation_available, "The information was not available in the specified language. Code: 0x803D0009" },	//	WS_E_NO_TRANSLATION_AVAILABLE
			{ com_errc::ws_e_security_verification_failure, "Security verification was not successful for the received data. Code: 0x803D000A" },	//	WS_E_SECURITY_VERIFICATION_FAILURE
			{ com_errc::ws_e_address_in_use, "The address is already being used. Code: 0x803D000B" },	//	WS_E_ADDRESS_IN_USE
			{ com_errc::ws_e_address_not_available, "The address is not valid for this context. Code: 0x803D000C" },	//	WS_E_ADDRESS_NOT_AVAILABLE
			{ com_errc::ws_e_endpoint_not_found, "The remote endpoint does not exist or could not be located. Code: 0x803D000D" },	//	WS_E_ENDPOINT_NOT_FOUND
			{ com_errc::ws_e_endpoint_not_available, "The remote endpoint is not currently in service at this location. Code: 0x803D000E" },	//	WS_E_ENDPOINT_NOT_AVAILABLE
			{ com_errc::ws_e_endpoint_failure, "The remote endpoint could not process the request. Code: 0x803D000F" },	//	WS_E_ENDPOINT_FAILURE
			{ com_errc::ws_e_endpoint_unreachable, "The remote endpoint was not reachable. Code: 0x803D0010" },	//	WS_E_ENDPOINT_UNREACHABLE
			{ com_errc::ws_e_endpoint_action_not_supported, "The operation was not supported by the remote endpoint. Code: 0x803D0011" },	//	WS_E_ENDPOINT_ACTION_NOT_SUPPORTED
			{ com_errc::ws_e_endpoint_too_busy, "The remote endpoint is unable to process the request due to being overloaded. Code: 0x803D0012" },	//	WS_E_ENDPOINT_TOO_BUSY
			{ com_errc::ws_e_endpoint_fault_received, "A message containing a fault was received from the remote endpoint. Code: 0x803D0013" },	//	WS_E_ENDPOINT_FAULT_RECEIVED
			{ com_errc::ws_e_endpoint_disconnected, "The connection with the remote endpoint was terminated. Code: 0x803D0014" },	//	WS_E_ENDPOINT_DISCONNECTED
			{ com_errc::ws_e_proxy_failure, "The HTTP proxy server could not process the request. Code: 0x803D0015" },	//	WS_E_PROXY_FAILURE
			{ com_errc::ws_e_proxy_access_denied, "Access was denied by the HTTP proxy server. Code: 0x803D0016" },	//	WS_E_PROXY_ACCESS_DENIED
			{ com_errc::ws_e_not_supported, "The requested feature is not available on this platform. Code: 0x803D0017" },	//	WS_E_NOT_SUPPORTED
			{ com_errc::ws_e_proxy_requires_basic_auth, "The HTTP proxy server requires HTTP authentication scheme 'basic'. Code: 0x803D0018" },	//	WS_E_PROXY_REQUIRES_BASIC_AUTH
			{ com_errc::ws_e_proxy_requires_digest_auth, "The HTTP proxy server requires HTTP authentication scheme 'digest'. Code: 0x803D0019" },	//	WS_E_PROXY_REQUIRES_DIGEST_AUTH
			{ com_errc::ws_e_proxy_requires_ntlm_auth, "The HTTP proxy server requires HTTP authentication scheme 'NTLM'. Code: 0x803D001A" },	//	WS_E_PROXY_REQUIRES_NTLM_AUTH
			{ com_errc::ws_e_proxy_requires_negotiate_auth, "The HTTP proxy server requires HTTP authentication scheme 'negotiate'. Code: 0x803D001B" },	//	WS_E_PROXY_REQUIRES_NEGOTIATE_AUTH
			{ com_errc::ws_e_server_requires_basic_auth, "The remote endpoint requires HTTP authentication scheme 'basic'. Code: 0x803D001C" },	//	WS_E_SERVER_REQUIRES_BASIC_AUTH
			{ com_errc::ws_e_server_requires_digest_auth, "The remote endpoint requires HTTP authentication scheme 'digest'. Code: 0x803D001D" },	//	WS_E_SERVER_REQUIRES_DIGEST_AUTH
			{ com_errc::ws_e_server_requires_ntlm_auth, "The remote endpoint requires HTTP authentication scheme 'NTLM'. Code: 0x803D001E" },	//	WS_E_SERVER_REQUIRES_NTLM_AUTH
			{ com_errc::ws_e_server_requires_negotiate_auth, "The remote endpoint requires HTTP authentication scheme 'negotiate'. Code: 0x803D001F" },	//	WS_E_SERVER_REQUIRES_NEGOTIATE_AUTH
			{ com_errc::ws_e_invalid_endpoint_url, "The endpoint address URL is invalid. Code: 0x803D0020" },	//	WS_E_INVALID_ENDPOINT_URL
			{ com_errc::ws_e_other, "Unrecognized error occurred in the Windows Web Services framework. Code: 0x803D0021" },	//	WS_E_OTHER
			{ com_errc::ws_e_security_token_expired, "A security token was rejected by the server because it has expired. Code: 0x803D0022" },	//	WS_E_SECURITY_TOKEN_EXPIRED
			{ com_errc::ws_e_security_system_failure, "A security operation failed in the Windows Web Services framework. Code: 0x803D0023" },	//	WS_E_SECURITY_SYSTEM_FAILURE
			{ com_errc::wpn_e_channel_closed, "The notification channel has already been closed. Code: 0x803E0100" },	//	WPN_E_CHANNEL_CLOSED
			{ com_errc::wpn_e_channel_request_not_complete, "The notification channel request did not complete successfully. Code: 0x803E0101" },	//	WPN_E_CHANNEL_REQUEST_NOT_COMPLETE
			{ com_errc::wpn_e_invalid_app, "The application identifier provided is invalid. Code: 0x803E0102" },	//	WPN_E_INVALID_APP
			{ com_errc::wpn_e_outstanding_channel_request, "A notification channel request for the provided application identifier is in progress. Code: 0x803E0103" },	//	WPN_E_OUTSTANDING_CHANNEL_REQUEST
			{ com_errc::wpn_e_duplicate_channel, "The channel identifier is already tied to another application endpoint. Code: 0x803E0104" },	//	WPN_E_DUPLICATE_CHANNEL
			{ com_errc::wpn_e_platform_unavailable, "The notification platform is unavailable. Code: 0x803E0105" },	//	WPN_E_PLATFORM_UNAVAILABLE
			{ com_errc::wpn_e_notification_posted, "The notification has already been posted. Code: 0x803E0106" },	//	WPN_E_NOTIFICATION_POSTED
			{ com_errc::wpn_e_notification_hidden, "The notification has already been hidden. Code: 0x803E0107" },	//	WPN_E_NOTIFICATION_HIDDEN
			{ com_errc::wpn_e_notification_not_posted, "The notification cannot be hidden until it has been shown. Code: 0x803E0108" },	//	WPN_E_NOTIFICATION_NOT_POSTED
			{ com_errc::wpn_e_cloud_disabled, "Cloud notifications have been turned off. Code: 0x803E0109" },	//	WPN_E_CLOUD_DISABLED
			{ com_errc::wpn_e_cloud_incapable, "The application does not have the cloud notification capability. Code: 0x803E0110" },	//	WPN_E_CLOUD_INCAPABLE
			{ com_errc::wpn_e_cloud_auth_unavailable, "The notification platform is unable to retrieve the authentication credentials required to connect to the cloud notification service. Code: 0x803E011A" },	//	WPN_E_CLOUD_AUTH_UNAVAILABLE
			{ com_errc::wpn_e_cloud_service_unavailable, "The notification platform is unable to connect to the cloud notification service. Code: 0x803E011B" },	//	WPN_E_CLOUD_SERVICE_UNAVAILABLE
			{ com_errc::wpn_e_failed_lock_screen_update_intialization, "The notification platform is unable to initialize a callback for lock screen updates. Code: 0x803E011C" },	//	WPN_E_FAILED_LOCK_SCREEN_UPDATE_INTIALIZATION
			{ com_errc::wpn_e_notification_disabled, "Settings prevent the notification from being delivered. Code: 0x803E0111" },	//	WPN_E_NOTIFICATION_DISABLED
			{ com_errc::wpn_e_notification_incapable, "Application capabilities prevent the notification from being delivered. Code: 0x803E0112" },	//	WPN_E_NOTIFICATION_INCAPABLE
			{ com_errc::wpn_e_internet_incapable, "The application does not have the internet access capability. Code: 0x803E0113" },	//	WPN_E_INTERNET_INCAPABLE
			{ com_errc::wpn_e_notification_type_disabled, "Settings prevent the notification type from being delivered. Code: 0x803E0114" },	//	WPN_E_NOTIFICATION_TYPE_DISABLED
			{ com_errc::wpn_e_notification_size, "The size of the notification content is too large. Code: 0x803E0115" },	//	WPN_E_NOTIFICATION_SIZE
			{ com_errc::wpn_e_tag_size, "The size of the notification tag is too large. Code: 0x803E0116" },	//	WPN_E_TAG_SIZE
			{ com_errc::wpn_e_access_denied, "The notification platform doesn't have appropriate privilege on resources. Code: 0x803E0117" },	//	WPN_E_ACCESS_DENIED
			{ com_errc::wpn_e_duplicate_registration, "The notification platform found application is already registered. Code: 0x803E0118" },	//	WPN_E_DUPLICATE_REGISTRATION
			{ com_errc::wpn_e_push_notification_incapable, "The application background task does not have the push notification capability. Code: 0x803E0119" },	//	WPN_E_PUSH_NOTIFICATION_INCAPABLE
			{ com_errc::wpn_e_dev_id_size, "The size of the developer id for scheduled notification is too large. Code: 0x803E0120" },	//	WPN_E_DEV_ID_SIZE
			{ com_errc::wpn_e_tag_alphanumeric, "The notification tag is not alphanumeric. Code: 0x803E012A" },	//	WPN_E_TAG_ALPHANUMERIC
			{ com_errc::wpn_e_invalid_http_status_code, "The notification platform has received invalid HTTP status code other than 2xx for polling. Code: 0x803E012B" },	//	WPN_E_INVALID_HTTP_STATUS_CODE
			{ com_errc::wpn_e_out_of_session, "The notification platform has run out of presentation layer sessions. Code: 0x803E0200" },	//	WPN_E_OUT_OF_SESSION
			{ com_errc::wpn_e_power_save, "The notification platform rejects image download request due to system in power save mode. Code: 0x803E0201" },	//	WPN_E_POWER_SAVE
			{ com_errc::wpn_e_image_not_found_in_cache, "The notification platform doesn't have the requested image in its cache. Code: 0x803E0202" },	//	WPN_E_IMAGE_NOT_FOUND_IN_CACHE
			{ com_errc::wpn_e_all_url_not_completed, "The notification platform cannot complete all of requested image. Code: 0x803E0203" },	//	WPN_E_ALL_URL_NOT_COMPLETED
			{ com_errc::wpn_e_invalid_cloud_image, "A cloud image downloaded from the notification platform is invalid. Code: 0x803E0204" },	//	WPN_E_INVALID_CLOUD_IMAGE
			{ com_errc::wpn_e_notification_id_matched, "Notification Id provided as filter is matched with what the notification platform maintains. Code: 0x803E0205" },	//	WPN_E_NOTIFICATION_ID_MATCHED
			{ com_errc::wpn_e_callback_already_registered, "Notification callback interface is already registered. Code: 0x803E0206" },	//	WPN_E_CALLBACK_ALREADY_REGISTERED
			{ com_errc::wpn_e_toast_notification_dropped, "Toast Notification was dropped without being displayed to the user. Code: 0x803E0207" },	//	WPN_E_TOAST_NOTIFICATION_DROPPED
			{ com_errc::wpn_e_storage_locked, "The notification platform does not have the proper privileges to complete the request. Code: 0x803E0208" },	//	WPN_E_STORAGE_LOCKED
			{ com_errc::e_mbn_context_not_activated, "Context is not activated. Code: 0x80548201" },	//	E_MBN_CONTEXT_NOT_ACTIVATED
			{ com_errc::e_mbn_bad_sim, "Bad SIM is inserted. Code: 0x80548202" },	//	E_MBN_BAD_SIM
			{ com_errc::e_mbn_data_class_not_available, "Requested data class is not avaialable. Code: 0x80548203" },	//	E_MBN_DATA_CLASS_NOT_AVAILABLE
			{ com_errc::e_mbn_invalid_access_string, "Access point name (APN) or Access string is incorrect. Code: 0x80548204" },	//	E_MBN_INVALID_ACCESS_STRING
			{ com_errc::e_mbn_max_activated_contexts, "Max activated contexts have reached. Code: 0x80548205" },	//	E_MBN_MAX_ACTIVATED_CONTEXTS
			{ com_errc::e_mbn_packet_svc_detached, "Device is in packet detach state. Code: 0x80548206" },	//	E_MBN_PACKET_SVC_DETACHED
			{ com_errc::e_mbn_provider_not_visible, "Provider is not visible. Code: 0x80548207" },	//	E_MBN_PROVIDER_NOT_VISIBLE
			{ com_errc::e_mbn_radio_power_off, "Radio is powered off. Code: 0x80548208" },	//	E_MBN_RADIO_POWER_OFF
			{ com_errc::e_mbn_service_not_activated, "MBN subscription is not activated. Code: 0x80548209" },	//	E_MBN_SERVICE_NOT_ACTIVATED
			{ com_errc::e_mbn_sim_not_inserted, "SIM is not inserted. Code: 0x8054820A" },	//	E_MBN_SIM_NOT_INSERTED
			{ com_errc::e_mbn_voice_call_in_progress, "Voice call in progress. Code: 0x8054820B" },	//	E_MBN_VOICE_CALL_IN_PROGRESS
			{ com_errc::e_mbn_invalid_cache, "Visible provider cache is invalid. Code: 0x8054820C" },	//	E_MBN_INVALID_CACHE
			{ com_errc::e_mbn_not_registered, "Device is not registered. Code: 0x8054820D" },	//	E_MBN_NOT_REGISTERED
			{ com_errc::e_mbn_providers_not_found, "Providers not found. Code: 0x8054820E" },	//	E_MBN_PROVIDERS_NOT_FOUND
			{ com_errc::e_mbn_pin_not_supported, "Pin is not supported. Code: 0x8054820F" },	//	E_MBN_PIN_NOT_SUPPORTED
			{ com_errc::e_mbn_pin_required, "Pin is required. Code: 0x80548210" },	//	E_MBN_PIN_REQUIRED
			{ com_errc::e_mbn_pin_disabled, "PIN is disabled. Code: 0x80548211" },	//	E_MBN_PIN_DISABLED
			{ com_errc::e_mbn_failure, "Generic Failure. Code: 0x80548212" },	//	E_MBN_FAILURE
			{ com_errc::e_mbn_invalid_profile, "Profile is invalid. Code: 0x80548218" },	//	E_MBN_INVALID_PROFILE
			{ com_errc::e_mbn_default_profile_exist, "Default profile exist. Code: 0x80548219" },	//	E_MBN_DEFAULT_PROFILE_EXIST
			{ com_errc::e_mbn_sms_encoding_not_supported, "SMS encoding is not supported. Code: 0x80548220" },	//	E_MBN_SMS_ENCODING_NOT_SUPPORTED
			{ com_errc::e_mbn_sms_filter_not_supported, "SMS filter is not supported. Code: 0x80548221" },	//	E_MBN_SMS_FILTER_NOT_SUPPORTED
			{ com_errc::e_mbn_sms_invalid_memory_index, "Invalid SMS memory index is used. Code: 0x80548222" },	//	E_MBN_SMS_INVALID_MEMORY_INDEX
			{ com_errc::e_mbn_sms_lang_not_supported, "SMS language is not supported. Code: 0x80548223" },	//	E_MBN_SMS_LANG_NOT_SUPPORTED
			{ com_errc::e_mbn_sms_memory_failure, "SMS memory failure occurred. Code: 0x80548224" },	//	E_MBN_SMS_MEMORY_FAILURE
			{ com_errc::e_mbn_sms_network_timeout, "SMS network timeout happened. Code: 0x80548225" },	//	E_MBN_SMS_NETWORK_TIMEOUT
			{ com_errc::e_mbn_sms_unknown_smsc_address, "Unknown SMSC address is used. Code: 0x80548226" },	//	E_MBN_SMS_UNKNOWN_SMSC_ADDRESS
			{ com_errc::e_mbn_sms_format_not_supported, "SMS format is not supported. Code: 0x80548227" },	//	E_MBN_SMS_FORMAT_NOT_SUPPORTED
			{ com_errc::e_mbn_sms_operation_not_allowed, "SMS operation is not allowed. Code: 0x80548228" },	//	E_MBN_SMS_OPERATION_NOT_ALLOWED
			{ com_errc::e_mbn_sms_memory_full, "Device SMS memory is full. Code: 0x80548229" },	//	E_MBN_SMS_MEMORY_FULL
			{ com_errc::peer_e_ipv6_not_installed, "The IPv6 protocol is not installed. Code: 0x80630001" },	//	PEER_E_IPV6_NOT_INSTALLED
			{ com_errc::peer_e_not_initialized, "The compoment has not been initialized. Code: 0x80630002" },	//	PEER_E_NOT_INITIALIZED
			{ com_errc::peer_e_cannot_start_service, "The required service canot be started. Code: 0x80630003" },	//	PEER_E_CANNOT_START_SERVICE
			{ com_errc::peer_e_not_licensed, "The P2P protocol is not licensed to run on this OS. Code: 0x80630004" },	//	PEER_E_NOT_LICENSED
			{ com_errc::peer_e_invalid_graph, "The graph handle is invalid. Code: 0x80630010" },	//	PEER_E_INVALID_GRAPH
			{ com_errc::peer_e_dbname_changed, "The graph database name has changed. Code: 0x80630011" },	//	PEER_E_DBNAME_CHANGED
			{ com_errc::peer_e_duplicate_graph, "A graph with the same ID already exists. Code: 0x80630012" },	//	PEER_E_DUPLICATE_GRAPH
			{ com_errc::peer_e_graph_not_ready, "The graph is not ready. Code: 0x80630013" },	//	PEER_E_GRAPH_NOT_READY
			{ com_errc::peer_e_graph_shutting_down, "The graph is shutting down. Code: 0x80630014" },	//	PEER_E_GRAPH_SHUTTING_DOWN
			{ com_errc::peer_e_graph_in_use, "The graph is still in use. Code: 0x80630015" },	//	PEER_E_GRAPH_IN_USE
			{ com_errc::peer_e_invalid_database, "The graph database is corrupt. Code: 0x80630016" },	//	PEER_E_INVALID_DATABASE
			{ com_errc::peer_e_too_many_attributes, "Too many attributes have been used. Code: 0x80630017" },	//	PEER_E_TOO_MANY_ATTRIBUTES
			{ com_errc::peer_e_connection_not_found, "The connection can not be found. Code: 0x80630103" },	//	PEER_E_CONNECTION_NOT_FOUND
			{ com_errc::peer_e_connect_self, "The peer attempted to connect to itself. Code: 0x80630106" },	//	PEER_E_CONNECT_SELF
			{ com_errc::peer_e_already_listening, "The peer is already listening for connections. Code: 0x80630107" },	//	PEER_E_ALREADY_LISTENING
			{ com_errc::peer_e_node_not_found, "The node was not found. Code: 0x80630108" },	//	PEER_E_NODE_NOT_FOUND
			{ com_errc::peer_e_connection_failed, "The Connection attempt failed. Code: 0x80630109" },	//	PEER_E_CONNECTION_FAILED
			{ com_errc::peer_e_connection_not_authenticated, "The peer connection could not be authenticated. Code: 0x8063010A" },	//	PEER_E_CONNECTION_NOT_AUTHENTICATED
			{ com_errc::peer_e_connection_refused, "The connection was refused. Code: 0x8063010B" },	//	PEER_E_CONNECTION_REFUSED
			{ com_errc::peer_e_classifier_too_long, "The peer name classifier is too long. Code: 0x80630201" },	//	PEER_E_CLASSIFIER_TOO_LONG
			{ com_errc::peer_e_too_many_identities, "The maximum number of identities have been created. Code: 0x80630202" },	//	PEER_E_TOO_MANY_IDENTITIES
			{ com_errc::peer_e_no_key_access, "Unable to access a key. Code: 0x80630203" },	//	PEER_E_NO_KEY_ACCESS
			{ com_errc::peer_e_groups_exist, "The group already exists. Code: 0x80630204" },	//	PEER_E_GROUPS_EXIST
			{ com_errc::peer_e_record_not_found, "The requested record could not be found. Code: 0x80630301" },	//	PEER_E_RECORD_NOT_FOUND
			{ com_errc::peer_e_database_accessdenied, "Access to the database was denied. Code: 0x80630302" },	//	PEER_E_DATABASE_ACCESSDENIED
			{ com_errc::peer_e_dbinitialization_failed, "The Database could not be initialized. Code: 0x80630303" },	//	PEER_E_DBINITIALIZATION_FAILED
			{ com_errc::peer_e_max_record_size_exceeded, "The record is too big. Code: 0x80630304" },	//	PEER_E_MAX_RECORD_SIZE_EXCEEDED
			{ com_errc::peer_e_database_already_present, "The database already exists. Code: 0x80630305" },	//	PEER_E_DATABASE_ALREADY_PRESENT
			{ com_errc::peer_e_database_not_present, "The database could not be found. Code: 0x80630306" },	//	PEER_E_DATABASE_NOT_PRESENT
			{ com_errc::peer_e_identity_not_found, "The identity could not be found. Code: 0x80630401" },	//	PEER_E_IDENTITY_NOT_FOUND
			{ com_errc::peer_e_event_handle_not_found, "The event handle could not be found. Code: 0x80630501" },	//	PEER_E_EVENT_HANDLE_NOT_FOUND
			{ com_errc::peer_e_invalid_search, "Invalid search. Code: 0x80630601" },	//	PEER_E_INVALID_SEARCH
			{ com_errc::peer_e_invalid_attributes, "The search atributes are invalid. Code: 0x80630602" },	//	PEER_E_INVALID_ATTRIBUTES
			{ com_errc::peer_e_invitation_not_trusted, "The invitiation is not trusted. Code: 0x80630701" },	//	PEER_E_INVITATION_NOT_TRUSTED
			{ com_errc::peer_e_chain_too_long, "The certchain is too long. Code: 0x80630703" },	//	PEER_E_CHAIN_TOO_LONG
			{ com_errc::peer_e_invalid_time_period, "The time period is invalid. Code: 0x80630705" },	//	PEER_E_INVALID_TIME_PERIOD
			{ com_errc::peer_e_circular_chain_detected, "A circular cert chain was detected. Code: 0x80630706" },	//	PEER_E_CIRCULAR_CHAIN_DETECTED
			{ com_errc::peer_e_cert_store_corrupted, "The certstore is corrupted. Code: 0x80630801" },	//	PEER_E_CERT_STORE_CORRUPTED
			{ com_errc::peer_e_no_cloud, "The specified PNRP cloud deos not exist. Code: 0x80631001" },	//	PEER_E_NO_CLOUD
			{ com_errc::peer_e_cloud_name_ambiguous, "The cloud name is ambiguous. Code: 0x80631005" },	//	PEER_E_CLOUD_NAME_AMBIGUOUS
			{ com_errc::peer_e_invalid_record, "The record is invlaid. Code: 0x80632010" },	//	PEER_E_INVALID_RECORD
			{ com_errc::peer_e_not_authorized, "Not authorized. Code: 0x80632020" },	//	PEER_E_NOT_AUTHORIZED
			{ com_errc::peer_e_password_does_not_meet_policy, "The password does not meet policy requirements. Code: 0x80632021" },	//	PEER_E_PASSWORD_DOES_NOT_MEET_POLICY
			{ com_errc::peer_e_deferred_validation, "The record validation has been defered. Code: 0x80632030" },	//	PEER_E_DEFERRED_VALIDATION
			{ com_errc::peer_e_invalid_group_properties, "The group properies are invalid. Code: 0x80632040" },	//	PEER_E_INVALID_GROUP_PROPERTIES
			{ com_errc::peer_e_invalid_peer_name, "The peername is invalid. Code: 0x80632050" },	//	PEER_E_INVALID_PEER_NAME
			{ com_errc::peer_e_invalid_classifier, "The classifier is invalid. Code: 0x80632060" },	//	PEER_E_INVALID_CLASSIFIER
			{ com_errc::peer_e_invalid_friendly_name, "The friendly name is invalid. Code: 0x80632070" },	//	PEER_E_INVALID_FRIENDLY_NAME
			{ com_errc::peer_e_invalid_role_property, "Invalid role property. Code: 0x80632071" },	//	PEER_E_INVALID_ROLE_PROPERTY
			{ com_errc::peer_e_invalid_classifier_property, "Invalid classifer property. Code: 0x80632072" },	//	PEER_E_INVALID_CLASSIFIER_PROPERTY
			{ com_errc::peer_e_invalid_record_expiration, "Invlaid record expiration. Code: 0x80632080" },	//	PEER_E_INVALID_RECORD_EXPIRATION
			{ com_errc::peer_e_invalid_credential_info, "Invlaid credential info. Code: 0x80632081" },	//	PEER_E_INVALID_CREDENTIAL_INFO
			{ com_errc::peer_e_invalid_credential, "Invalid credential. Code: 0x80632082" },	//	PEER_E_INVALID_CREDENTIAL
			{ com_errc::peer_e_invalid_record_size, "Invalid record size. Code: 0x80632083" },	//	PEER_E_INVALID_RECORD_SIZE
			{ com_errc::peer_e_unsupported_version, "Unsupported version. Code: 0x80632090" },	//	PEER_E_UNSUPPORTED_VERSION
			{ com_errc::peer_e_group_not_ready, "The group is not ready. Code: 0x80632091" },	//	PEER_E_GROUP_NOT_READY
			{ com_errc::peer_e_group_in_use, "The group is still in use. Code: 0x80632092" },	//	PEER_E_GROUP_IN_USE
			{ com_errc::peer_e_invalid_group, "The group is invalid. Code: 0x80632093" },	//	PEER_E_INVALID_GROUP
			{ com_errc::peer_e_no_members_found, "No members were found. Code: 0x80632094" },	//	PEER_E_NO_MEMBERS_FOUND
			{ com_errc::peer_e_no_member_connections, "There are no member connections. Code: 0x80632095" },	//	PEER_E_NO_MEMBER_CONNECTIONS
			{ com_errc::peer_e_unable_to_listen, "Unable to listen. Code: 0x80632096" },	//	PEER_E_UNABLE_TO_LISTEN
			{ com_errc::peer_e_identity_deleted, "The identity does not exist. Code: 0x806320A0" },	//	PEER_E_IDENTITY_DELETED
			{ com_errc::peer_e_service_not_available, "The service is not availible. Code: 0x806320A1" },	//	PEER_E_SERVICE_NOT_AVAILABLE
			{ com_errc::peer_e_contact_not_found, "THe contact could not be found. Code: 0x80636001" },	//	PEER_E_CONTACT_NOT_FOUND
			{ com_errc::peer_s_graph_data_created, "The graph data was created. Code: 0x00630001" },	//	PEER_S_GRAPH_DATA_CREATED
			{ com_errc::peer_s_no_event_data, "There is not more event data. Code: 0x00630002" },	//	PEER_S_NO_EVENT_DATA
			{ com_errc::peer_s_already_connected, "The graph is already connect. Code: 0x00632000" },	//	PEER_S_ALREADY_CONNECTED
			{ com_errc::peer_s_subscription_exists, "The subscription already exists. Code: 0x00636000" },	//	PEER_S_SUBSCRIPTION_EXISTS
			{ com_errc::peer_s_no_connectivity, "No connectivity. Code: 0x00630005" },	//	PEER_S_NO_CONNECTIVITY
			{ com_errc::peer_s_already_a_member, "Already a member. Code: 0x00630006" },	//	PEER_S_ALREADY_A_MEMBER
			{ com_errc::peer_e_cannot_convert_peer_name, "The peername could not be converted to a DNS pnrp name. Code: 0x80634001" },	//	PEER_E_CANNOT_CONVERT_PEER_NAME
			{ com_errc::peer_e_invalid_peer_host_name, "Invalid peer host name. Code: 0x80634002" },	//	PEER_E_INVALID_PEER_HOST_NAME
			{ com_errc::peer_e_no_more, "No more data could be found. Code: 0x80634003" },	//	PEER_E_NO_MORE
			{ com_errc::peer_e_pnrp_duplicate_peer_name, "The existing peer name is already registered. Code: 0x80634005" },	//	PEER_E_PNRP_DUPLICATE_PEER_NAME
			{ com_errc::peer_e_invite_cancelled, "The app invite request was cancelled by the user. Code: 0x80637000" },	//	PEER_E_INVITE_CANCELLED
			{ com_errc::peer_e_invite_response_not_available, "No response of the invite was received. Code: 0x80637001" },	//	PEER_E_INVITE_RESPONSE_NOT_AVAILABLE
			{ com_errc::peer_e_not_signed_in, "User is not signed into serverless presence. Code: 0x80637003" },	//	PEER_E_NOT_SIGNED_IN
			{ com_errc::peer_e_privacy_declined, "The user declined the privacy policy prompt. Code: 0x80637004" },	//	PEER_E_PRIVACY_DECLINED
			{ com_errc::peer_e_timeout, "A timeout occurred. Code: 0x80637005" },	//	PEER_E_TIMEOUT
			{ com_errc::peer_e_invalid_address, "The address is invalid. Code: 0x80637007" },	//	PEER_E_INVALID_ADDRESS
			{ com_errc::peer_e_fw_exception_disabled, "A required firewall exception is disabled. Code: 0x80637008" },	//	PEER_E_FW_EXCEPTION_DISABLED
			{ com_errc::peer_e_fw_blocked_by_policy, "The service is blocked by a firewall policy. Code: 0x80637009" },	//	PEER_E_FW_BLOCKED_BY_POLICY
			{ com_errc::peer_e_fw_blocked_by_shields_up, "Firewall exceptions are disabled. Code: 0x8063700A" },	//	PEER_E_FW_BLOCKED_BY_SHIELDS_UP
			{ com_errc::peer_e_fw_declined, "The user declined to enable the firewall exceptions. Code: 0x8063700B" },	//	PEER_E_FW_DECLINED
			{ com_errc::ui_e_create_failed, "The object could not be created. Code: 0x802A0001" },	//	UI_E_CREATE_FAILED
			{ com_errc::ui_e_shutdown_called, "Shutdown was already called on this object or the object that owns it. Code: 0x802A0002" },	//	UI_E_SHUTDOWN_CALLED
			{ com_errc::ui_e_illegal_reentrancy, "This method cannot be called during this type of callback. Code: 0x802A0003" },	//	UI_E_ILLEGAL_REENTRANCY
			{ com_errc::ui_e_object_sealed, "This object has been sealed, so this change is no longer allowed. Code: 0x802A0004" },	//	UI_E_OBJECT_SEALED
			{ com_errc::ui_e_value_not_set, "The requested value was never set. Code: 0x802A0005" },	//	UI_E_VALUE_NOT_SET
			{ com_errc::ui_e_value_not_determined, "The requested value cannot be determined. Code: 0x802A0006" },	//	UI_E_VALUE_NOT_DETERMINED
			{ com_errc::ui_e_invalid_output, "A callback returned an invalid output parameter. Code: 0x802A0007" },	//	UI_E_INVALID_OUTPUT
			{ com_errc::ui_e_boolean_expected, "A callback returned a success code other than S_OK or S_FALSE. Code: 0x802A0008" },	//	UI_E_BOOLEAN_EXPECTED
			{ com_errc::ui_e_different_owner, "A parameter that should be owned by this object is owned by a different object. Code: 0x802A0009" },	//	UI_E_DIFFERENT_OWNER
			{ com_errc::ui_e_ambiguous_match, "More than one item matched the search criteria. Code: 0x802A000A" },	//	UI_E_AMBIGUOUS_MATCH
			{ com_errc::ui_e_fp_overflow, "A floating-point overflow occurred. Code: 0x802A000B" },	//	UI_E_FP_OVERFLOW
			{ com_errc::ui_e_wrong_thread, "This method can only be called from the thread that created the object. Code: 0x802A000C" },	//	UI_E_WRONG_THREAD
			{ com_errc::ui_e_storyboard_active, "The storyboard is currently in the schedule. Code: 0x802A0101" },	//	UI_E_STORYBOARD_ACTIVE
			{ com_errc::ui_e_storyboard_not_playing, "The storyboard is not playing. Code: 0x802A0102" },	//	UI_E_STORYBOARD_NOT_PLAYING
			{ com_errc::ui_e_start_keyframe_after_end, "The start keyframe might occur after the end keyframe. Code: 0x802A0103" },	//	UI_E_START_KEYFRAME_AFTER_END
			{ com_errc::ui_e_end_keyframe_not_determined, "It might not be possible to determine the end keyframe time when the start keyframe is reached. Code: 0x802A0104" },	//	UI_E_END_KEYFRAME_NOT_DETERMINED
			{ com_errc::ui_e_loops_overlap, "Two repeated portions of a storyboard might overlap. Code: 0x802A0105" },	//	UI_E_LOOPS_OVERLAP
			{ com_errc::ui_e_transition_already_used, "The transition has already been added to a storyboard. Code: 0x802A0106" },	//	UI_E_TRANSITION_ALREADY_USED
			{ com_errc::ui_e_transition_not_in_storyboard, "The transition has not been added to a storyboard. Code: 0x802A0107" },	//	UI_E_TRANSITION_NOT_IN_STORYBOARD
			{ com_errc::ui_e_transition_eclipsed, "The transition might eclipse the beginning of another transition in the storyboard. Code: 0x802A0108" },	//	UI_E_TRANSITION_ECLIPSED
			{ com_errc::ui_e_time_before_last_update, "The given time is earlier than the time passed to the last update. Code: 0x802A0109" },	//	UI_E_TIME_BEFORE_LAST_UPDATE
			{ com_errc::ui_e_timer_client_already_connected, "This client is already connected to a timer. Code: 0x802A010A" },	//	UI_E_TIMER_CLIENT_ALREADY_CONNECTED
			{ com_errc::ui_e_invalid_dimension, "The passed dimension is invalid or does not match the object's dimension. Code: 0x802A010B" },	//	UI_E_INVALID_DIMENSION
			{ com_errc::ui_e_primitive_out_of_bounds, "The added primitive begins at or beyond the duration of the interpolator. Code: 0x802A010C" },	//	UI_E_PRIMITIVE_OUT_OF_BOUNDS
			{ com_errc::ui_e_window_closed, "The operation cannot be completed because the window is being closed. Code: 0x802A0201" },	//	UI_E_WINDOW_CLOSED
			{ com_errc::e_bluetooth_att_invalid_handle, "The attribute handle given was not valid on this server. Code: 0x80650001" },	//	E_BLUETOOTH_ATT_INVALID_HANDLE
			{ com_errc::e_bluetooth_att_read_not_permitted, "The attribute cannot be read. Code: 0x80650002" },	//	E_BLUETOOTH_ATT_READ_NOT_PERMITTED
			{ com_errc::e_bluetooth_att_write_not_permitted, "The attribute cannot be written. Code: 0x80650003" },	//	E_BLUETOOTH_ATT_WRITE_NOT_PERMITTED
			{ com_errc::e_bluetooth_att_invalid_pdu, "The attribute PDU was invalid. Code: 0x80650004" },	//	E_BLUETOOTH_ATT_INVALID_PDU
			{ com_errc::e_bluetooth_att_insufficient_authentication, "The attribute requires authentication before it can be read or written. Code: 0x80650005" },	//	E_BLUETOOTH_ATT_INSUFFICIENT_AUTHENTICATION
			{ com_errc::e_bluetooth_att_request_not_supported, "Attribute server does not support the request received from the client. Code: 0x80650006" },	//	E_BLUETOOTH_ATT_REQUEST_NOT_SUPPORTED
			{ com_errc::e_bluetooth_att_invalid_offset, "Offset specified was past the end of the attribute. Code: 0x80650007" },	//	E_BLUETOOTH_ATT_INVALID_OFFSET
			{ com_errc::e_bluetooth_att_insufficient_authorization, "The attribute requires authorization before it can be read or written. Code: 0x80650008" },	//	E_BLUETOOTH_ATT_INSUFFICIENT_AUTHORIZATION
			{ com_errc::e_bluetooth_att_prepare_queue_full, "Too many prepare writes have been queued. Code: 0x80650009" },	//	E_BLUETOOTH_ATT_PREPARE_QUEUE_FULL
			{ com_errc::e_bluetooth_att_attribute_not_found, "No attribute found within the given attribute handle range. Code: 0x8065000A" },	//	E_BLUETOOTH_ATT_ATTRIBUTE_NOT_FOUND
			{ com_errc::e_bluetooth_att_attribute_not_long, "The attribute cannot be read or written using the Read Blob Request. Code: 0x8065000B" },	//	E_BLUETOOTH_ATT_ATTRIBUTE_NOT_LONG
			{ com_errc::e_bluetooth_att_insufficient_encryption_key_size, "The Encryption Key Size used for encrypting this link is insufficient. Code: 0x8065000C" },	//	E_BLUETOOTH_ATT_INSUFFICIENT_ENCRYPTION_KEY_SIZE
			{ com_errc::e_bluetooth_att_invalid_attribute_value_length, "The attribute value length is invalid for the operation. Code: 0x8065000D" },	//	E_BLUETOOTH_ATT_INVALID_ATTRIBUTE_VALUE_LENGTH
			{ com_errc::e_bluetooth_att_unlikely, "The attribute request that was requested has encountered an error that was unlikely, and therefore could not be completed as requested. Code: 0x8065000E" },	//	E_BLUETOOTH_ATT_UNLIKELY
			{ com_errc::e_bluetooth_att_insufficient_encryption, "The attribute requires encryption before it can be read or written. Code: 0x8065000F" },	//	E_BLUETOOTH_ATT_INSUFFICIENT_ENCRYPTION
			{ com_errc::e_bluetooth_att_unsupported_group_type, "The attribute type is not a supported grouping attribute as defined by a higher layer specification. Code: 0x80650010" },	//	E_BLUETOOTH_ATT_UNSUPPORTED_GROUP_TYPE
			{ com_errc::e_bluetooth_att_insufficient_resources, "Insufficient Resources to complete the request. Code: 0x80650011" },	//	E_BLUETOOTH_ATT_INSUFFICIENT_RESOURCES
			{ com_errc::e_bluetooth_att_unknown_error, "An error that lies in the reserved range has been received. Code: 0x80651000" },	//	E_BLUETOOTH_ATT_UNKNOWN_ERROR
			{ com_errc::e_audio_engine_node_not_found, "PortCls could not find an audio engine node exposed by a miniport driver claiming support for IMiniportAudioEngineNode. Code: 0x80660001" },	//	E_AUDIO_ENGINE_NODE_NOT_FOUND
			{ com_errc::e_hdaudio_empty_connection_list, "HD Audio widget encountered an unexpected empty connection list. Code: 0x80660002" },	//	E_HDAUDIO_EMPTY_CONNECTION_LIST
			{ com_errc::e_hdaudio_connection_list_not_supported, "HD Audio widget does not support the connection list parameter. Code: 0x80660003" },	//	E_HDAUDIO_CONNECTION_LIST_NOT_SUPPORTED
			{ com_errc::e_hdaudio_no_logical_devices_created, "No HD Audio subdevices were successfully created. Code: 0x80660004" },	//	E_HDAUDIO_NO_LOGICAL_DEVICES_CREATED
			{ com_errc::e_hdaudio_null_linked_list_entry, "An unexpected NULL pointer was encountered in a linked list. Code: 0x80660005" },	//	E_HDAUDIO_NULL_LINKED_LIST_ENTRY
			{ com_errc::error_spaces_pool_was_deleted, "The storage pool was deleted by the driver. The object cache should be updated. Code: 0x00E70001" },	//	ERROR_SPACES_POOL_WAS_DELETED
			{ com_errc::error_spaces_resiliency_type_invalid, "The specified resiliency type is not valid. Code: 0x80E70003" },	//	ERROR_SPACES_RESILIENCY_TYPE_INVALID
			{ com_errc::error_spaces_drive_sector_size_invalid, "The physical disk's sector size is not supported by the storage pool. Code: 0x80E70004" },	//	ERROR_SPACES_DRIVE_SECTOR_SIZE_INVALID
			{ com_errc::error_spaces_drive_redundancy_invalid, "The requested redundancy is outside of the supported range of values. Code: 0x80E70006" },	//	ERROR_SPACES_DRIVE_REDUNDANCY_INVALID
			{ com_errc::error_spaces_number_of_data_copies_invalid, "The number of data copies requested is outside of the supported range of values. Code: 0x80E70007" },	//	ERROR_SPACES_NUMBER_OF_DATA_COPIES_INVALID
			{ com_errc::error_spaces_parity_layout_invalid, "The value for ParityLayout is outside of the supported range of values. Code: 0x80E70008" },	//	ERROR_SPACES_PARITY_LAYOUT_INVALID
			{ com_errc::error_spaces_interleave_length_invalid, "The value for interleave length is outside of the supported range of values. Code: 0x80E70009" },	//	ERROR_SPACES_INTERLEAVE_LENGTH_INVALID
			{ com_errc::error_spaces_number_of_columns_invalid, "The number of columns specified is outside of the supported range of values. Code: 0x80E7000A" },	//	ERROR_SPACES_NUMBER_OF_COLUMNS_INVALID
			{ com_errc::error_spaces_not_enough_drives, "There were not enough physical disks to complete the requested operation. Code: 0x80E7000B" },	//	ERROR_SPACES_NOT_ENOUGH_DRIVES
			{ com_errc::error_volsnap_bootfile_not_valid, "The bootfile is too small to support persistent snapshots. Code: 0x80820001" },	//	ERROR_VOLSNAP_BOOTFILE_NOT_VALID
			{ com_errc::error_tiering_not_supported_on_volume, "The specified volume does not support storage tiers. Code: 0x80830001" },	//	ERROR_TIERING_NOT_SUPPORTED_ON_VOLUME
			{ com_errc::error_tiering_volume_dismount_in_progress, "The Storage Tiers Management service detected that the specified volume is in the process of being dismounted. Code: 0x80830002" },	//	ERROR_TIERING_VOLUME_DISMOUNT_IN_PROGRESS
			{ com_errc::error_tiering_storage_tier_not_found, "The specified storage tier could not be found on the volume. Confirm that the storage tier name is valid. Code: 0x80830003" },	//	ERROR_TIERING_STORAGE_TIER_NOT_FOUND
			{ com_errc::error_tiering_invalid_file_id, "The file identifier specified is not valid on the volume. Code: 0x80830004" },	//	ERROR_TIERING_INVALID_FILE_ID
			{ com_errc::error_tiering_wrong_cluster_node, "Storage tier operations must be called on the clustering node that owns the metadata volume. Code: 0x80830005" },	//	ERROR_TIERING_WRONG_CLUSTER_NODE
			{ com_errc::error_tiering_already_processing, "The Storage Tiers Management service is already optimizing the storage tiers on the specified volume. Code: 0x80830006" },	//	ERROR_TIERING_ALREADY_PROCESSING
			{ com_errc::error_tiering_cannot_pin_object, "The requested object type cannot be assigned to a storage tier. Code: 0x80830007" },	//	ERROR_TIERING_CANNOT_PIN_OBJECT
			{ com_errc::dxgi_status_occluded, "The Present operation was invisible to the user. Code: 0x087A0001" },	//	DXGI_STATUS_OCCLUDED
			{ com_errc::dxgi_status_clipped, "The Present operation was partially invisible to the user. Code: 0x087A0002" },	//	DXGI_STATUS_CLIPPED
			{ com_errc::dxgi_status_no_redirection, "The driver is requesting that the DXGI runtime not use shared resources to communicate with the Desktop Window Manager. Code: 0x087A0004" },	//	DXGI_STATUS_NO_REDIRECTION
			{ com_errc::dxgi_status_no_desktop_access, "The Present operation was not visible because the Windows session has switched to another desktop (for example, ctrl-alt-del). Code: 0x087A0005" },	//	DXGI_STATUS_NO_DESKTOP_ACCESS
			{ com_errc::dxgi_status_graphics_vidpn_source_in_use, "The Present operation was not visible because the target monitor was being used for some other purpose. Code: 0x087A0006" },	//	DXGI_STATUS_GRAPHICS_VIDPN_SOURCE_IN_USE
			{ com_errc::dxgi_status_mode_changed, "The Present operation was not visible because the display mode changed. DXGI will have re-attempted the presentation. Code: 0x087A0007" },	//	DXGI_STATUS_MODE_CHANGED
			{ com_errc::dxgi_status_mode_change_in_progress, "The Present operation was not visible because another Direct3D device was attempting to take fullscreen mode at the time. Code: 0x087A0008" },	//	DXGI_STATUS_MODE_CHANGE_IN_PROGRESS
			{ com_errc::dxgi_error_invalid_call, "The application made a call that is invalid. Either the parameters of the call or the state of some object was incorrect. Enable the D3D debug layer in order to see details via debug messages. Code: 0x887A0001" },	//	DXGI_ERROR_INVALID_CALL
			{ com_errc::dxgi_error_not_found, "The object was not found. If calling IDXGIFactory::EnumAdaptes, there is no adapter with the specified ordinal. Code: 0x887A0002" },	//	DXGI_ERROR_NOT_FOUND
			{ com_errc::dxgi_error_more_data, "The caller did not supply a sufficiently large buffer. Code: 0x887A0003" },	//	DXGI_ERROR_MORE_DATA
			{ com_errc::dxgi_error_unsupported, "The specified device interface or feature level is not supported on this system. Code: 0x887A0004" },	//	DXGI_ERROR_UNSUPPORTED
			{ com_errc::dxgi_error_device_removed, "The GPU device instance has been suspended. Use GetDeviceRemovedReason to determine the appropriate action. Code: 0x887A0005" },	//	DXGI_ERROR_DEVICE_REMOVED
			{ com_errc::dxgi_error_device_hung, "The GPU will not respond to more commands, most likely because of an invalid command passed by the calling application. Code: 0x887A0006" },	//	DXGI_ERROR_DEVICE_HUNG
			{ com_errc::dxgi_error_device_reset, "The GPU will not respond to more commands, most likely because some other application submitted invalid commands. The calling application should re-create the device and continue. Code: 0x887A0007" },	//	DXGI_ERROR_DEVICE_RESET
			{ com_errc::dxgi_error_was_still_drawing, "The GPU was busy at the moment when the call was made, and the call was neither executed nor scheduled. Code: 0x887A000A" },	//	DXGI_ERROR_WAS_STILL_DRAWING
			{ com_errc::dxgi_error_frame_statistics_disjoint, "An event (such as power cycle) interrupted the gathering of presentation statistics. Any previous statistics should be considered invalid. Code: 0x887A000B" },	//	DXGI_ERROR_FRAME_STATISTICS_DISJOINT
			{ com_errc::dxgi_error_graphics_vidpn_source_in_use, "Fullscreen mode could not be achieved because the specified output was already in use. Code: 0x887A000C" },	//	DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE
			{ com_errc::dxgi_error_driver_internal_error, "An internal issue prevented the driver from carrying out the specified operation. The driver's state is probably suspect, and the application should not continue. Code: 0x887A0020" },	//	DXGI_ERROR_DRIVER_INTERNAL_ERROR
			{ com_errc::dxgi_error_nonexclusive, "A global counter resource was in use, and the specified counter cannot be used by this Direct3D device at this time. Code: 0x887A0021" },	//	DXGI_ERROR_NONEXCLUSIVE
			{ com_errc::dxgi_error_not_currently_available, "A resource is not available at the time of the call, but may become available later. Code: 0x887A0022" },	//	DXGI_ERROR_NOT_CURRENTLY_AVAILABLE
			{ com_errc::dxgi_error_remote_client_disconnected, "The application's remote device has been removed due to session disconnect or network disconnect. The application should call IDXGIFactory1::IsCurrent to find out when the remote device becomes available again. Code: 0x887A0023" },	//	DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED
			{ com_errc::dxgi_error_remote_outofmemory, "The device has been removed during a remote session because the remote computer ran out of memory. Code: 0x887A0024" },	//	DXGI_ERROR_REMOTE_OUTOFMEMORY
			{ com_errc::dxgi_error_access_lost, "The keyed mutex was abandoned. Code: 0x887A0026" },	//	DXGI_ERROR_ACCESS_LOST
			{ com_errc::dxgi_error_wait_timeout, "The timeout value has elapsed and the resource is not yet available. Code: 0x887A0027" },	//	DXGI_ERROR_WAIT_TIMEOUT
			{ com_errc::dxgi_error_session_disconnected, "The output duplication has been turned off because the Windows session ended or was disconnected. This happens when a remote user disconnects, or when \"switch user\" is used locally. Code: 0x887A0028" },	//	DXGI_ERROR_SESSION_DISCONNECTED
			{ com_errc::dxgi_error_restrict_to_output_stale, "The DXGI outuput (monitor) to which the swapchain content was restricted, has been disconnected or changed. Code: 0x887A0029" },	//	DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE
			{ com_errc::dxgi_error_cannot_protect_content, "DXGI is unable to provide content protection on the swapchain. This is typically caused by an older driver, or by the application using a swapchain that is incompatible with content protection. Code: 0x887A002A" },	//	DXGI_ERROR_CANNOT_PROTECT_CONTENT
			{ com_errc::dxgi_error_access_denied, "The application is trying to use a resource to which it does not have the required access privileges. This is most commonly caused by writing to a shared resource with read-only access. Code: 0x887A002B" },	//	DXGI_ERROR_ACCESS_DENIED
			{ com_errc::dxgi_error_name_already_exists, "The application is trying to create a shared handle using a name that is already associated with some other resource. Code: 0x887A002C" },	//	DXGI_ERROR_NAME_ALREADY_EXISTS
			{ com_errc::dxgi_error_sdk_component_missing, "The application requested an operation that depends on an SDK component that is missing or mismatched. Code: 0x887A002D" },	//	DXGI_ERROR_SDK_COMPONENT_MISSING
			{ com_errc::dxgi_status_unoccluded, "The swapchain has become unoccluded. Code: 0x087A0009" },	//	DXGI_STATUS_UNOCCLUDED
			{ com_errc::dxgi_status_dda_was_still_drawing, "The adapter did not have access to the required resources to complete the Desktop Duplication Present() call, the Present() call needs to be made again. Code: 0x087A000A" },	//	DXGI_STATUS_DDA_WAS_STILL_DRAWING
			{ com_errc::dxgi_error_mode_change_in_progress, "An on-going mode change prevented completion of the call. The call may succeed if attempted later. Code: 0x887A0025" },	//	DXGI_ERROR_MODE_CHANGE_IN_PROGRESS
			{ com_errc::dxgi_ddi_err_wasstilldrawing, "The GPU was busy when the operation was requested. Code: 0x887B0001" },	//	DXGI_DDI_ERR_WASSTILLDRAWING
			{ com_errc::dxgi_ddi_err_unsupported, "The driver has rejected the creation of this resource. Code: 0x887B0002" },	//	DXGI_DDI_ERR_UNSUPPORTED
			{ com_errc::dxgi_ddi_err_nonexclusive, "The GPU counter was in use by another process or d3d device when application requested access to it. Code: 0x887B0003" },	//	DXGI_DDI_ERR_NONEXCLUSIVE
			{ com_errc::d3d10_error_too_many_unique_state_objects, "The application has exceeded the maximum number of unique state objects per Direct3D device. The limit is 4096 for feature levels up to 11.1. Code: 0x88790001" },	//	D3D10_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS
			{ com_errc::d3d10_error_file_not_found, "The specified file was not found. Code: 0x88790002" },	//	D3D10_ERROR_FILE_NOT_FOUND
			{ com_errc::d3d11_error_too_many_unique_state_objects, "The application has exceeded the maximum number of unique state objects per Direct3D device. The limit is 4096 for feature levels up to 11.1. Code: 0x887C0001" },	//	D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS
			{ com_errc::d3d11_error_file_not_found, "The specified file was not found. Code: 0x887C0002" },	//	D3D11_ERROR_FILE_NOT_FOUND
			{ com_errc::d3d11_error_too_many_unique_view_objects, "The application has exceeded the maximum number of unique view objects per Direct3D device. The limit is 2^20 for feature levels up to 11.1. Code: 0x887C0003" },	//	D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS
			{ com_errc::d3d11_error_deferred_context_map_without_initial_discard, "The application's first call per command list to Map on a deferred context did not use D3D11_MAP_WRITE_DISCARD. Code: 0x887C0004" },	//	D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD
			{ com_errc::d2derr_wrong_state, "The object was not in the correct state to process the method. Code: 0x88990001" },	//	D2DERR_WRONG_STATE
			{ com_errc::d2derr_not_initialized, "The object has not yet been initialized. Code: 0x88990002" },	//	D2DERR_NOT_INITIALIZED
			{ com_errc::d2derr_unsupported_operation, "The requested operation is not supported. Code: 0x88990003" },	//	D2DERR_UNSUPPORTED_OPERATION
			{ com_errc::d2derr_scanner_failed, "The geometry scanner failed to process the data. Code: 0x88990004" },	//	D2DERR_SCANNER_FAILED
			{ com_errc::d2derr_screen_access_denied, "Direct2D could not access the screen. Code: 0x88990005" },	//	D2DERR_SCREEN_ACCESS_DENIED
			{ com_errc::d2derr_display_state_invalid, "A valid display state could not be determined. Code: 0x88990006" },	//	D2DERR_DISPLAY_STATE_INVALID
			{ com_errc::d2derr_zero_vector, "The supplied vector is zero. Code: 0x88990007" },	//	D2DERR_ZERO_VECTOR
			{ com_errc::d2derr_internal_error, "An internal error (Direct2D bug) occurred. On checked builds, we would assert. The application should close this instance of Direct2D and should consider restarting its process. Code: 0x88990008" },	//	D2DERR_INTERNAL_ERROR
			{ com_errc::d2derr_display_format_not_supported, "The display format Direct2D needs to render is not supported by the hardware device. Code: 0x88990009" },	//	D2DERR_DISPLAY_FORMAT_NOT_SUPPORTED
			{ com_errc::d2derr_invalid_call, "A call to this method is invalid. Code: 0x8899000A" },	//	D2DERR_INVALID_CALL
			{ com_errc::d2derr_no_hardware_device, "No hardware rendering device is available for this operation. Code: 0x8899000B" },	//	D2DERR_NO_HARDWARE_DEVICE
			{ com_errc::d2derr_recreate_target, "There has been a presentation error that may be recoverable. The caller needs to recreate, rerender the entire frame, and reattempt present. Code: 0x8899000C" },	//	D2DERR_RECREATE_TARGET
			{ com_errc::d2derr_too_many_shader_elements, "Shader construction failed because it was too complex. Code: 0x8899000D" },	//	D2DERR_TOO_MANY_SHADER_ELEMENTS
			{ com_errc::d2derr_shader_compile_failed, "Shader compilation failed. Code: 0x8899000E" },	//	D2DERR_SHADER_COMPILE_FAILED
			{ com_errc::d2derr_max_texture_size_exceeded, "Requested DirectX surface size exceeded maximum texture size. Code: 0x8899000F" },	//	D2DERR_MAX_TEXTURE_SIZE_EXCEEDED
			{ com_errc::d2derr_unsupported_version, "The requested Direct2D version is not supported. Code: 0x88990010" },	//	D2DERR_UNSUPPORTED_VERSION
			{ com_errc::d2derr_bad_number, "Invalid number. Code: 0x88990011" },	//	D2DERR_BAD_NUMBER
			{ com_errc::d2derr_wrong_factory, "Objects used together must be created from the same factory instance. Code: 0x88990012" },	//	D2DERR_WRONG_FACTORY
			{ com_errc::d2derr_layer_already_in_use, "A layer resource can only be in use once at any point in time. Code: 0x88990013" },	//	D2DERR_LAYER_ALREADY_IN_USE
			{ com_errc::d2derr_pop_call_did_not_match_push, "The pop call did not match the corresponding push call. Code: 0x88990014" },	//	D2DERR_POP_CALL_DID_NOT_MATCH_PUSH
			{ com_errc::d2derr_wrong_resource_domain, "The resource was realized on the wrong render target. Code: 0x88990015" },	//	D2DERR_WRONG_RESOURCE_DOMAIN
			{ com_errc::d2derr_push_pop_unbalanced, "The push and pop calls were unbalanced. Code: 0x88990016" },	//	D2DERR_PUSH_POP_UNBALANCED
			{ com_errc::d2derr_render_target_has_layer_or_cliprect, "Attempt to copy from a render target while a layer or clip rect is applied. Code: 0x88990017" },	//	D2DERR_RENDER_TARGET_HAS_LAYER_OR_CLIPRECT
			{ com_errc::d2derr_incompatible_brush_types, "The brush types are incompatible for the call. Code: 0x88990018" },	//	D2DERR_INCOMPATIBLE_BRUSH_TYPES
			{ com_errc::d2derr_win32_error, "An unknown win32 failure occurred. Code: 0x88990019" },	//	D2DERR_WIN32_ERROR
			{ com_errc::d2derr_target_not_gdi_compatible, "The render target is not compatible with GDI. Code: 0x8899001A" },	//	D2DERR_TARGET_NOT_GDI_COMPATIBLE
			{ com_errc::d2derr_text_effect_is_wrong_type, "A text client drawing effect object is of the wrong type. Code: 0x8899001B" },	//	D2DERR_TEXT_EFFECT_IS_WRONG_TYPE
			{ com_errc::d2derr_text_renderer_not_released, "The application is holding a reference to the IDWriteTextRenderer interface after the corresponding DrawText or DrawTextLayout call has returned. The IDWriteTextRenderer instance will be invalid. Code: 0x8899001C" },	//	D2DERR_TEXT_RENDERER_NOT_RELEASED
			{ com_errc::d2derr_exceeds_max_bitmap_size, "The requested size is larger than the guaranteed supported texture size at the Direct3D device's current feature level. Code: 0x8899001D" },	//	D2DERR_EXCEEDS_MAX_BITMAP_SIZE
			{ com_errc::d2derr_invalid_graph_configuration, "There was a configuration error in the graph. Code: 0x8899001E" },	//	D2DERR_INVALID_GRAPH_CONFIGURATION
			{ com_errc::d2derr_invalid_internal_graph_configuration, "There was a internal configuration error in the graph. Code: 0x8899001F" },	//	D2DERR_INVALID_INTERNAL_GRAPH_CONFIGURATION
			{ com_errc::d2derr_cyclic_graph, "There was a cycle in the graph. Code: 0x88990020" },	//	D2DERR_CYCLIC_GRAPH
			{ com_errc::d2derr_bitmap_cannot_draw, "Cannot draw with a bitmap that has the D2D1_BITMAP_OPTIONS_CANNOT_DRAW option. Code: 0x88990021" },	//	D2DERR_BITMAP_CANNOT_DRAW
			{ com_errc::d2derr_outstanding_bitmap_references, "The operation cannot complete while there are outstanding references to the target bitmap. Code: 0x88990022" },	//	D2DERR_OUTSTANDING_BITMAP_REFERENCES
			{ com_errc::d2derr_original_target_not_bound, "The operation failed because the original target is not currently bound as a target. Code: 0x88990023" },	//	D2DERR_ORIGINAL_TARGET_NOT_BOUND
			{ com_errc::d2derr_invalid_target, "Cannot set the image as a target because it is either an effect or is a bitmap that does not have the D2D1_BITMAP_OPTIONS_TARGET flag set. Code: 0x88990024" },	//	D2DERR_INVALID_TARGET
			{ com_errc::d2derr_bitmap_bound_as_target, "Cannot draw with a bitmap that is currently bound as the target bitmap. Code: 0x88990025" },	//	D2DERR_BITMAP_BOUND_AS_TARGET
			{ com_errc::d2derr_insufficient_device_capabilities, "D3D Device does not have sufficient capabilities to perform the requested action. Code: 0x88990026" },	//	D2DERR_INSUFFICIENT_DEVICE_CAPABILITIES
			{ com_errc::d2derr_intermediate_too_large, "The graph could not be rendered with the context's current tiling settings. Code: 0x88990027" },	//	D2DERR_INTERMEDIATE_TOO_LARGE
			{ com_errc::d2derr_effect_is_not_registered, "The CLSID provided to Unregister did not correspond to a registered effect. Code: 0x88990028" },	//	D2DERR_EFFECT_IS_NOT_REGISTERED
			{ com_errc::d2derr_invalid_property, "The specified property does not exist. Code: 0x88990029" },	//	D2DERR_INVALID_PROPERTY
			{ com_errc::d2derr_no_subproperties, "The specified sub-property does not exist. Code: 0x8899002A" },	//	D2DERR_NO_SUBPROPERTIES
			{ com_errc::d2derr_print_job_closed, "AddPage or Close called after print job is already closed. Code: 0x8899002B" },	//	D2DERR_PRINT_JOB_CLOSED
			{ com_errc::d2derr_print_format_not_supported, "Error during print control creation. Indicates that none of the package target types (representing printer formats) are supported by Direct2D print control. Code: 0x8899002C" },	//	D2DERR_PRINT_FORMAT_NOT_SUPPORTED
			{ com_errc::d2derr_too_many_transform_inputs, "An effect attempted to use a transform with too many inputs. Code: 0x8899002D" },	//	D2DERR_TOO_MANY_TRANSFORM_INPUTS
			{ com_errc::dwrite_e_fileformat, "Indicates an error in an input file such as a font file. Code: 0x88985000" },	//	DWRITE_E_FILEFORMAT
			{ com_errc::dwrite_e_unexpected, "Indicates an error originating in DirectWrite code, which is not expected to occur but is safe to recover from. Code: 0x88985001" },	//	DWRITE_E_UNEXPECTED
			{ com_errc::dwrite_e_nofont, "Indicates the specified font does not exist. Code: 0x88985002" },	//	DWRITE_E_NOFONT
			{ com_errc::dwrite_e_filenotfound, "A font file could not be opened because the file, directory, network location, drive, or other storage location does not exist or is unavailable. Code: 0x88985003" },	//	DWRITE_E_FILENOTFOUND
			{ com_errc::dwrite_e_fileaccess, "A font file exists but could not be opened due to access denied, sharing violation, or similar error. Code: 0x88985004" },	//	DWRITE_E_FILEACCESS
			{ com_errc::dwrite_e_fontcollectionobsolete, "A font collection is obsolete due to changes in the system. Code: 0x88985005" },	//	DWRITE_E_FONTCOLLECTIONOBSOLETE
			{ com_errc::dwrite_e_alreadyregistered, "The given interface is already registered. Code: 0x88985006" },	//	DWRITE_E_ALREADYREGISTERED
			{ com_errc::dwrite_e_cacheformat, "The font cache contains invalid data. Code: 0x88985007" },	//	DWRITE_E_CACHEFORMAT
			{ com_errc::dwrite_e_cacheversion, "A font cache file corresponds to a different version of DirectWrite. Code: 0x88985008" },	//	DWRITE_E_CACHEVERSION
			{ com_errc::dwrite_e_unsupportedoperation, "The operation is not supported for this type of font. Code: 0x88985009" },	//	DWRITE_E_UNSUPPORTEDOPERATION
			{ com_errc::dwrite_e_textrendererincompatible, "The version of the text renderer interface is not compatible. Code: 0x8898500A" },	//	DWRITE_E_TEXTRENDERERINCOMPATIBLE
			{ com_errc::dwrite_e_flowdirectionconflicts, "The flow direction conflicts with the reading direction. They must be perpendicular to each other. Code: 0x8898500B" },	//	DWRITE_E_FLOWDIRECTIONCONFLICTS
			{ com_errc::dwrite_e_nocolor, "The font or glyph run does not contain any colored glyphs. Code: 0x8898500C" },	//	DWRITE_E_NOCOLOR
			{ com_errc::wincodec_err_wrongstate, "The codec is in the wrong state. Code: 0x88982F04" },	//	WINCODEC_ERR_WRONGSTATE
			{ com_errc::wincodec_err_valueoutofrange, "The value is out of range. Code: 0x88982F05" },	//	WINCODEC_ERR_VALUEOUTOFRANGE
			{ com_errc::wincodec_err_unknownimageformat, "The image format is unknown. Code: 0x88982F07" },	//	WINCODEC_ERR_UNKNOWNIMAGEFORMAT
			{ com_errc::wincodec_err_unsupportedversion, "The SDK version is unsupported. Code: 0x88982F0B" },	//	WINCODEC_ERR_UNSUPPORTEDVERSION
			{ com_errc::wincodec_err_notinitialized, "The component is not initialized. Code: 0x88982F0C" },	//	WINCODEC_ERR_NOTINITIALIZED
			{ com_errc::wincodec_err_alreadylocked, "There is already an outstanding read or write lock. Code: 0x88982F0D" },	//	WINCODEC_ERR_ALREADYLOCKED
			{ com_errc::wincodec_err_propertynotfound, "The specified bitmap property cannot be found. Code: 0x88982F40" },	//	WINCODEC_ERR_PROPERTYNOTFOUND
			{ com_errc::wincodec_err_propertynotsupported, "The bitmap codec does not support the bitmap property. Code: 0x88982F41" },	//	WINCODEC_ERR_PROPERTYNOTSUPPORTED
			{ com_errc::wincodec_err_propertysize, "The bitmap property size is invalid. Code: 0x88982F42" },	//	WINCODEC_ERR_PROPERTYSIZE
			{ com_errc::wincodec_err_codecpresent, "An unknown error has occurred. Code: 0x88982F43" },	//	WINCODEC_ERR_CODECPRESENT
			{ com_errc::wincodec_err_codecnothumbnail, "The bitmap codec does not support a thumbnail. Code: 0x88982F44" },	//	WINCODEC_ERR_CODECNOTHUMBNAIL
			{ com_errc::wincodec_err_paletteunavailable, "The bitmap palette is unavailable. Code: 0x88982F45" },	//	WINCODEC_ERR_PALETTEUNAVAILABLE
			{ com_errc::wincodec_err_codectoomanyscanlines, "Too many scanlines were requested. Code: 0x88982F46" },	//	WINCODEC_ERR_CODECTOOMANYSCANLINES
			{ com_errc::wincodec_err_internalerror, "An internal error occurred. Code: 0x88982F48" },	//	WINCODEC_ERR_INTERNALERROR
			{ com_errc::wincodec_err_sourcerectdoesnotmatchdimensions, "The bitmap bounds do not match the bitmap dimensions. Code: 0x88982F49" },	//	WINCODEC_ERR_SOURCERECTDOESNOTMATCHDIMENSIONS
			{ com_errc::wincodec_err_componentnotfound, "The component cannot be found. Code: 0x88982F50" },	//	WINCODEC_ERR_COMPONENTNOTFOUND
			{ com_errc::wincodec_err_imagesizeoutofrange, "The bitmap size is outside the valid range. Code: 0x88982F51" },	//	WINCODEC_ERR_IMAGESIZEOUTOFRANGE
			{ com_errc::wincodec_err_toomuchmetadata, "There is too much metadata to be written to the bitmap. Code: 0x88982F52" },	//	WINCODEC_ERR_TOOMUCHMETADATA
			{ com_errc::wincodec_err_badimage, "The image is unrecognized. Code: 0x88982F60" },	//	WINCODEC_ERR_BADIMAGE
			{ com_errc::wincodec_err_badheader, "The image header is unrecognized. Code: 0x88982F61" },	//	WINCODEC_ERR_BADHEADER
			{ com_errc::wincodec_err_framemissing, "The bitmap frame is missing. Code: 0x88982F62" },	//	WINCODEC_ERR_FRAMEMISSING
			{ com_errc::wincodec_err_badmetadataheader, "The image metadata header is unrecognized. Code: 0x88982F63" },	//	WINCODEC_ERR_BADMETADATAHEADER
			{ com_errc::wincodec_err_badstreamdata, "The stream data is unrecognized. Code: 0x88982F70" },	//	WINCODEC_ERR_BADSTREAMDATA
			{ com_errc::wincodec_err_streamwrite, "Failed to write to the stream. Code: 0x88982F71" },	//	WINCODEC_ERR_STREAMWRITE
			{ com_errc::wincodec_err_streamread, "Failed to read from the stream. Code: 0x88982F72" },	//	WINCODEC_ERR_STREAMREAD
			{ com_errc::wincodec_err_streamnotavailable, "The stream is not available. Code: 0x88982F73" },	//	WINCODEC_ERR_STREAMNOTAVAILABLE
			{ com_errc::wincodec_err_unsupportedpixelformat, "The bitmap pixel format is unsupported. Code: 0x88982F80" },	//	WINCODEC_ERR_UNSUPPORTEDPIXELFORMAT
			{ com_errc::wincodec_err_unsupportedoperation, "The operation is unsupported. Code: 0x88982F81" },	//	WINCODEC_ERR_UNSUPPORTEDOPERATION
			{ com_errc::wincodec_err_invalidregistration, "The component registration is invalid. Code: 0x88982F8A" },	//	WINCODEC_ERR_INVALIDREGISTRATION
			{ com_errc::wincodec_err_componentinitializefailure, "The component initialization has failed. Code: 0x88982F8B" },	//	WINCODEC_ERR_COMPONENTINITIALIZEFAILURE
			{ com_errc::wincodec_err_insufficientbuffer, "The buffer allocated is insufficient. Code: 0x88982F8C" },	//	WINCODEC_ERR_INSUFFICIENTBUFFER
			{ com_errc::wincodec_err_duplicatemetadatapresent, "Duplicate metadata is present. Code: 0x88982F8D" },	//	WINCODEC_ERR_DUPLICATEMETADATAPRESENT
			{ com_errc::wincodec_err_propertyunexpectedtype, "The bitmap property type is unexpected. Code: 0x88982F8E" },	//	WINCODEC_ERR_PROPERTYUNEXPECTEDTYPE
			{ com_errc::wincodec_err_unexpectedsize, "The size is unexpected. Code: 0x88982F8F" },	//	WINCODEC_ERR_UNEXPECTEDSIZE
			{ com_errc::wincodec_err_invalidqueryrequest, "The property query is invalid. Code: 0x88982F90" },	//	WINCODEC_ERR_INVALIDQUERYREQUEST
			{ com_errc::wincodec_err_unexpectedmetadatatype, "The metadata type is unexpected. Code: 0x88982F91" },	//	WINCODEC_ERR_UNEXPECTEDMETADATATYPE
			{ com_errc::wincodec_err_requestonlyvalidatmetadataroot, "The specified bitmap property is only valid at root level. Code: 0x88982F92" },	//	WINCODEC_ERR_REQUESTONLYVALIDATMETADATAROOT
			{ com_errc::wincodec_err_invalidquerycharacter, "The query string contains an invalid character. Code: 0x88982F93" },	//	WINCODEC_ERR_INVALIDQUERYCHARACTER
			{ com_errc::wincodec_err_win32error, "Windows Codecs received an error from the Win32 system. Code: 0x88982F94" },	//	WINCODEC_ERR_WIN32ERROR
			{ com_errc::wincodec_err_invalidprogressivelevel, "The requested level of detail is not present. Code: 0x88982F95" },	//	WINCODEC_ERR_INVALIDPROGRESSIVELEVEL
			{ com_errc::milerr_objectbusy, "MILERR_OBJECTBUSY. Code: 0x88980001" },	//	MILERR_OBJECTBUSY
			{ com_errc::milerr_insufficientbuffer, "MILERR_INSUFFICIENTBUFFER. Code: 0x88980002" },	//	MILERR_INSUFFICIENTBUFFER
			{ com_errc::milerr_win32error, "MILERR_WIN32ERROR. Code: 0x88980003" },	//	MILERR_WIN32ERROR
			{ com_errc::milerr_scanner_failed, "MILERR_SCANNER_FAILED. Code: 0x88980004" },	//	MILERR_SCANNER_FAILED
			{ com_errc::milerr_screenaccessdenied, "MILERR_SCREENACCESSDENIED. Code: 0x88980005" },	//	MILERR_SCREENACCESSDENIED
			{ com_errc::milerr_displaystateinvalid, "MILERR_DISPLAYSTATEINVALID. Code: 0x88980006" },	//	MILERR_DISPLAYSTATEINVALID
			{ com_errc::milerr_noninvertiblematrix, "MILERR_NONINVERTIBLEMATRIX. Code: 0x88980007" },	//	MILERR_NONINVERTIBLEMATRIX
			{ com_errc::milerr_zerovector, "MILERR_ZEROVECTOR. Code: 0x88980008" },	//	MILERR_ZEROVECTOR
			{ com_errc::milerr_terminated, "MILERR_TERMINATED. Code: 0x88980009" },	//	MILERR_TERMINATED
			{ com_errc::milerr_badnumber, "MILERR_BADNUMBER. Code: 0x8898000A" },	//	MILERR_BADNUMBER
			{ com_errc::milerr_internalerror, "An internal error (MIL bug) occurred. On checked builds, an assert would be raised. Code: 0x88980080" },	//	MILERR_INTERNALERROR
			{ com_errc::milerr_displayformatnotsupported, "The display format we need to render is not supported by the hardware device. Code: 0x88980084" },	//	MILERR_DISPLAYFORMATNOTSUPPORTED
			{ com_errc::milerr_invalidcall, "A call to this method is invalid. Code: 0x88980085" },	//	MILERR_INVALIDCALL
			{ com_errc::milerr_alreadylocked, "Lock attempted on an already locked object. Code: 0x88980086" },	//	MILERR_ALREADYLOCKED
			{ com_errc::milerr_notlocked, "Unlock attempted on an unlocked object. Code: 0x88980087" },	//	MILERR_NOTLOCKED
			{ com_errc::milerr_devicecannotrendertext, "No algorithm avaliable to render text with this device. Code: 0x88980088" },	//	MILERR_DEVICECANNOTRENDERTEXT
			{ com_errc::milerr_glyphbitmapmissed, "Some glyph bitmaps, required for glyph run rendering, are not contained in glyph cache. Code: 0x88980089" },	//	MILERR_GLYPHBITMAPMISSED
			{ com_errc::milerr_malformedglyphcache, "Some glyph bitmaps in glyph cache are unexpectedly big. Code: 0x8898008A" },	//	MILERR_MALFORMEDGLYPHCACHE
			{ com_errc::milerr_generic_ignore, "Marker error for known Win32 errors that are currently being ignored by the compositor. This is to avoid returning S_OK when an error has occurred, but still unwind the stack in the correct location. Code: 0x8898008B" },	//	MILERR_GENERIC_IGNORE
			{ com_errc::milerr_malformed_guideline_data, "Guideline coordinates are not sorted properly or contain NaNs. Code: 0x8898008C" },	//	MILERR_MALFORMED_GUIDELINE_DATA
			{ com_errc::milerr_no_hardware_device, "No HW rendering device is available for this operation. Code: 0x8898008D" },	//	MILERR_NO_HARDWARE_DEVICE
			{ com_errc::milerr_need_recreate_and_present, "There has been a presentation error that may be recoverable. The caller needs to recreate, rerender the entire frame, and reattempt present. There are two known case for this: 1) D3D Driver Internal error 2) D3D E_FAIL 2a) Unknown root cause b) When resizing too quickly for DWM and D3D stay in sync. Code: 0x8898008E" },	//	MILERR_NEED_RECREATE_AND_PRESENT
			{ com_errc::milerr_already_initialized, "The object has already been initialized. Code: 0x8898008F" },	//	MILERR_ALREADY_INITIALIZED
			{ com_errc::milerr_mismatched_size, "The size of the object does not match the expected size. Code: 0x88980090" },	//	MILERR_MISMATCHED_SIZE
			{ com_errc::milerr_no_redirection_surface_available, "No Redirection surface available. Code: 0x88980091" },	//	MILERR_NO_REDIRECTION_SURFACE_AVAILABLE
			{ com_errc::milerr_remoting_not_supported, "Remoting of this content is not supported. Code: 0x88980092" },	//	MILERR_REMOTING_NOT_SUPPORTED
			{ com_errc::milerr_queued_present_not_supported, "Queued Presents are not supported. Code: 0x88980093" },	//	MILERR_QUEUED_PRESENT_NOT_SUPPORTED
			{ com_errc::milerr_not_queuing_presents, "Queued Presents are not being used. Code: 0x88980094" },	//	MILERR_NOT_QUEUING_PRESENTS
			{ com_errc::milerr_no_redirection_surface_retry_later, "No redirection surface was available. Caller should retry the call. Code: 0x88980095" },	//	MILERR_NO_REDIRECTION_SURFACE_RETRY_LATER
			{ com_errc::milerr_toomanyshaderelemnts, "Shader construction failed because it was too complex. Code: 0x88980096" },	//	MILERR_TOOMANYSHADERELEMNTS
			{ com_errc::milerr_mrow_readlock_failed, "MROW attempt to get a read lock failed. Code: 0x88980097" },	//	MILERR_MROW_READLOCK_FAILED
			{ com_errc::milerr_mrow_update_failed, "MROW attempt to update the data failed because another update was outstanding. Code: 0x88980098" },	//	MILERR_MROW_UPDATE_FAILED
			{ com_errc::milerr_shader_compile_failed, "Shader compilation failed. Code: 0x88980099" },	//	MILERR_SHADER_COMPILE_FAILED
			{ com_errc::milerr_max_texture_size_exceeded, "Requested DX redirection surface size exceeded maximum texture size. Code: 0x8898009A" },	//	MILERR_MAX_TEXTURE_SIZE_EXCEEDED
			{ com_errc::milerr_qpc_time_went_backward, "QueryPerformanceCounter returned a time in the past. Code: 0x8898009B" },	//	MILERR_QPC_TIME_WENT_BACKWARD
			{ com_errc::milerr_dxgi_enumeration_out_of_sync, "Primary Display device returned an invalid refresh rate. Code: 0x8898009D" },	//	MILERR_DXGI_ENUMERATION_OUT_OF_SYNC
			{ com_errc::milerr_adapter_not_found, "DWM can not find the adapter specified by the LUID. Code: 0x8898009E" },	//	MILERR_ADAPTER_NOT_FOUND
			{ com_errc::milerr_colorspace_not_supported, "The requested bitmap color space is not supported. Code: 0x8898009F" },	//	MILERR_COLORSPACE_NOT_SUPPORTED
			{ com_errc::milerr_prefilter_not_supported, "The requested bitmap pre-filtering state is not supported. Code: 0x889800A0" },	//	MILERR_PREFILTER_NOT_SUPPORTED
			{ com_errc::milerr_displayid_access_denied, "Access is denied to the requested bitmap for the specified display id. Code: 0x889800A1" },	//	MILERR_DISPLAYID_ACCESS_DENIED
			{ com_errc::uceerr_invalidpacketheader, "UCEERR_INVALIDPACKETHEADER. Code: 0x88980400" },	//	UCEERR_INVALIDPACKETHEADER
			{ com_errc::uceerr_unknownpacket, "UCEERR_UNKNOWNPACKET. Code: 0x88980401" },	//	UCEERR_UNKNOWNPACKET
			{ com_errc::uceerr_illegalpacket, "UCEERR_ILLEGALPACKET. Code: 0x88980402" },	//	UCEERR_ILLEGALPACKET
			{ com_errc::uceerr_malformedpacket, "UCEERR_MALFORMEDPACKET. Code: 0x88980403" },	//	UCEERR_MALFORMEDPACKET
			{ com_errc::uceerr_illegalhandle, "UCEERR_ILLEGALHANDLE. Code: 0x88980404" },	//	UCEERR_ILLEGALHANDLE
			{ com_errc::uceerr_handlelookupfailed, "UCEERR_HANDLELOOKUPFAILED. Code: 0x88980405" },	//	UCEERR_HANDLELOOKUPFAILED
			{ com_errc::uceerr_renderthreadfailure, "UCEERR_RENDERTHREADFAILURE. Code: 0x88980406" },	//	UCEERR_RENDERTHREADFAILURE
			{ com_errc::uceerr_ctxstackfrsttargetnull, "UCEERR_CTXSTACKFRSTTARGETNULL. Code: 0x88980407" },	//	UCEERR_CTXSTACKFRSTTARGETNULL
			{ com_errc::uceerr_connectionidlookupfailed, "UCEERR_CONNECTIONIDLOOKUPFAILED. Code: 0x88980408" },	//	UCEERR_CONNECTIONIDLOOKUPFAILED
			{ com_errc::uceerr_blocksfull, "UCEERR_BLOCKSFULL. Code: 0x88980409" },	//	UCEERR_BLOCKSFULL
			{ com_errc::uceerr_memoryfailure, "UCEERR_MEMORYFAILURE. Code: 0x8898040A" },	//	UCEERR_MEMORYFAILURE
			{ com_errc::uceerr_packetrecordoutofrange, "UCEERR_PACKETRECORDOUTOFRANGE. Code: 0x8898040B" },	//	UCEERR_PACKETRECORDOUTOFRANGE
			{ com_errc::uceerr_illegalrecordtype, "UCEERR_ILLEGALRECORDTYPE. Code: 0x8898040C" },	//	UCEERR_ILLEGALRECORDTYPE
			{ com_errc::uceerr_outofhandles, "UCEERR_OUTOFHANDLES. Code: 0x8898040D" },	//	UCEERR_OUTOFHANDLES
			{ com_errc::uceerr_unchangable_update_attempted, "UCEERR_UNCHANGABLE_UPDATE_ATTEMPTED. Code: 0x8898040E" },	//	UCEERR_UNCHANGABLE_UPDATE_ATTEMPTED
			{ com_errc::uceerr_no_multiple_worker_threads, "UCEERR_NO_MULTIPLE_WORKER_THREADS. Code: 0x8898040F" },	//	UCEERR_NO_MULTIPLE_WORKER_THREADS
			{ com_errc::uceerr_remotingnotsupported, "UCEERR_REMOTINGNOTSUPPORTED. Code: 0x88980410" },	//	UCEERR_REMOTINGNOTSUPPORTED
			{ com_errc::uceerr_missingendcommand, "UCEERR_MISSINGENDCOMMAND. Code: 0x88980411" },	//	UCEERR_MISSINGENDCOMMAND
			{ com_errc::uceerr_missingbegincommand, "UCEERR_MISSINGBEGINCOMMAND. Code: 0x88980412" },	//	UCEERR_MISSINGBEGINCOMMAND
			{ com_errc::uceerr_channelsynctimedout, "UCEERR_CHANNELSYNCTIMEDOUT. Code: 0x88980413" },	//	UCEERR_CHANNELSYNCTIMEDOUT
			{ com_errc::uceerr_channelsyncabandoned, "UCEERR_CHANNELSYNCABANDONED. Code: 0x88980414" },	//	UCEERR_CHANNELSYNCABANDONED
			{ com_errc::uceerr_unsupportedtransportversion, "UCEERR_UNSUPPORTEDTRANSPORTVERSION. Code: 0x88980415" },	//	UCEERR_UNSUPPORTEDTRANSPORTVERSION
			{ com_errc::uceerr_transportunavailable, "UCEERR_TRANSPORTUNAVAILABLE. Code: 0x88980416" },	//	UCEERR_TRANSPORTUNAVAILABLE
			{ com_errc::uceerr_feedback_unsupported, "UCEERR_FEEDBACK_UNSUPPORTED. Code: 0x88980417" },	//	UCEERR_FEEDBACK_UNSUPPORTED
			{ com_errc::uceerr_commandtransportdenied, "UCEERR_COMMANDTRANSPORTDENIED. Code: 0x88980418" },	//	UCEERR_COMMANDTRANSPORTDENIED
			{ com_errc::uceerr_graphicsstreamunavailable, "UCEERR_GRAPHICSSTREAMUNAVAILABLE. Code: 0x88980419" },	//	UCEERR_GRAPHICSSTREAMUNAVAILABLE
			{ com_errc::uceerr_graphicsstreamalreadyopen, "UCEERR_GRAPHICSSTREAMALREADYOPEN. Code: 0x88980420" },	//	UCEERR_GRAPHICSSTREAMALREADYOPEN
			{ com_errc::uceerr_transportdisconnected, "UCEERR_TRANSPORTDISCONNECTED. Code: 0x88980421" },	//	UCEERR_TRANSPORTDISCONNECTED
			{ com_errc::uceerr_transportoverloaded, "UCEERR_TRANSPORTOVERLOADED. Code: 0x88980422" },	//	UCEERR_TRANSPORTOVERLOADED
			{ com_errc::uceerr_partition_zombied, "UCEERR_PARTITION_ZOMBIED. Code: 0x88980423" },	//	UCEERR_PARTITION_ZOMBIED
			{ com_errc::milaverr_noclock, "MILAVERR_NOCLOCK. Code: 0x88980500" },	//	MILAVERR_NOCLOCK
			{ com_errc::milaverr_nomediatype, "MILAVERR_NOMEDIATYPE. Code: 0x88980501" },	//	MILAVERR_NOMEDIATYPE
			{ com_errc::milaverr_novideomixer, "MILAVERR_NOVIDEOMIXER. Code: 0x88980502" },	//	MILAVERR_NOVIDEOMIXER
			{ com_errc::milaverr_novideopresenter, "MILAVERR_NOVIDEOPRESENTER. Code: 0x88980503" },	//	MILAVERR_NOVIDEOPRESENTER
			{ com_errc::milaverr_noreadyframes, "MILAVERR_NOREADYFRAMES. Code: 0x88980504" },	//	MILAVERR_NOREADYFRAMES
			{ com_errc::milaverr_modulenotloaded, "MILAVERR_MODULENOTLOADED. Code: 0x88980505" },	//	MILAVERR_MODULENOTLOADED
			{ com_errc::milaverr_wmpfactorynotregistered, "MILAVERR_WMPFACTORYNOTREGISTERED. Code: 0x88980506" },	//	MILAVERR_WMPFACTORYNOTREGISTERED
			{ com_errc::milaverr_invalidwmpversion, "MILAVERR_INVALIDWMPVERSION. Code: 0x88980507" },	//	MILAVERR_INVALIDWMPVERSION
			{ com_errc::milaverr_insufficientvideoresources, "MILAVERR_INSUFFICIENTVIDEORESOURCES. Code: 0x88980508" },	//	MILAVERR_INSUFFICIENTVIDEORESOURCES
			{ com_errc::milaverr_videoaccelerationnotavailable, "MILAVERR_VIDEOACCELERATIONNOTAVAILABLE. Code: 0x88980509" },	//	MILAVERR_VIDEOACCELERATIONNOTAVAILABLE
			{ com_errc::milaverr_requestedtexturetoobig, "MILAVERR_REQUESTEDTEXTURETOOBIG. Code: 0x8898050A" },	//	MILAVERR_REQUESTEDTEXTURETOOBIG
			{ com_errc::milaverr_seekfailed, "MILAVERR_SEEKFAILED. Code: 0x8898050B" },	//	MILAVERR_SEEKFAILED
			{ com_errc::milaverr_unexpectedwmpfailure, "MILAVERR_UNEXPECTEDWMPFAILURE. Code: 0x8898050C" },	//	MILAVERR_UNEXPECTEDWMPFAILURE
			{ com_errc::milaverr_mediaplayerclosed, "MILAVERR_MEDIAPLAYERCLOSED. Code: 0x8898050D" },	//	MILAVERR_MEDIAPLAYERCLOSED
			{ com_errc::milaverr_unknownhardwareerror, "MILAVERR_UNKNOWNHARDWAREERROR. Code: 0x8898050E" },	//	MILAVERR_UNKNOWNHARDWAREERROR
			{ com_errc::mileffectserr_unknownproperty, "MILEFFECTSERR_UNKNOWNPROPERTY. Code: 0x8898060E" },	//	MILEFFECTSERR_UNKNOWNPROPERTY
			{ com_errc::mileffectserr_effectnotpartofgroup, "MILEFFECTSERR_EFFECTNOTPARTOFGROUP. Code: 0x8898060F" },	//	MILEFFECTSERR_EFFECTNOTPARTOFGROUP
			{ com_errc::mileffectserr_noinputsourceattached, "MILEFFECTSERR_NOINPUTSOURCEATTACHED. Code: 0x88980610" },	//	MILEFFECTSERR_NOINPUTSOURCEATTACHED
			{ com_errc::mileffectserr_connectornotconnected, "MILEFFECTSERR_CONNECTORNOTCONNECTED. Code: 0x88980611" },	//	MILEFFECTSERR_CONNECTORNOTCONNECTED
			{ com_errc::mileffectserr_connectornotassociatedwitheffect, "MILEFFECTSERR_CONNECTORNOTASSOCIATEDWITHEFFECT. Code: 0x88980612" },	//	MILEFFECTSERR_CONNECTORNOTASSOCIATEDWITHEFFECT
			{ com_errc::mileffectserr_reserved, "MILEFFECTSERR_RESERVED. Code: 0x88980613" },	//	MILEFFECTSERR_RESERVED
			{ com_errc::mileffectserr_cycledetected, "MILEFFECTSERR_CYCLEDETECTED. Code: 0x88980614" },	//	MILEFFECTSERR_CYCLEDETECTED
			{ com_errc::mileffectserr_effectinmorethanonegraph, "MILEFFECTSERR_EFFECTINMORETHANONEGRAPH. Code: 0x88980615" },	//	MILEFFECTSERR_EFFECTINMORETHANONEGRAPH
			{ com_errc::mileffectserr_effectalreadyinagraph, "MILEFFECTSERR_EFFECTALREADYINAGRAPH. Code: 0x88980616" },	//	MILEFFECTSERR_EFFECTALREADYINAGRAPH
			{ com_errc::mileffectserr_effecthasnochildren, "MILEFFECTSERR_EFFECTHASNOCHILDREN. Code: 0x88980617" },	//	MILEFFECTSERR_EFFECTHASNOCHILDREN
			{ com_errc::mileffectserr_alreadyattachedtolistener, "MILEFFECTSERR_ALREADYATTACHEDTOLISTENER. Code: 0x88980618" },	//	MILEFFECTSERR_ALREADYATTACHEDTOLISTENER
			{ com_errc::mileffectserr_notaffinetransform, "MILEFFECTSERR_NOTAFFINETRANSFORM. Code: 0x88980619" },	//	MILEFFECTSERR_NOTAFFINETRANSFORM
			{ com_errc::mileffectserr_emptybounds, "MILEFFECTSERR_EMPTYBOUNDS. Code: 0x8898061A" },	//	MILEFFECTSERR_EMPTYBOUNDS
			{ com_errc::mileffectserr_outputsizetoolarge, "MILEFFECTSERR_OUTPUTSIZETOOLARGE. Code: 0x8898061B" },	//	MILEFFECTSERR_OUTPUTSIZETOOLARGE
			{ com_errc::dwmerr_state_transition_failed, "DWMERR_STATE_TRANSITION_FAILED. Code: 0x88980700" },	//	DWMERR_STATE_TRANSITION_FAILED
			{ com_errc::dwmerr_theme_failed, "DWMERR_THEME_FAILED. Code: 0x88980701" },	//	DWMERR_THEME_FAILED
			{ com_errc::dwmerr_catastrophic_failure, "DWMERR_CATASTROPHIC_FAILURE. Code: 0x88980702" },	//	DWMERR_CATASTROPHIC_FAILURE
			{ com_errc::dcomposition_error_window_already_composed, "DCOMPOSITION_ERROR_WINDOW_ALREADY_COMPOSED. Code: 0x88980800" },	//	DCOMPOSITION_ERROR_WINDOW_ALREADY_COMPOSED
			{ com_errc::dcomposition_error_surface_being_rendered, "DCOMPOSITION_ERROR_SURFACE_BEING_RENDERED. Code: 0x88980801" },	//	DCOMPOSITION_ERROR_SURFACE_BEING_RENDERED
			{ com_errc::dcomposition_error_surface_not_being_rendered, "DCOMPOSITION_ERROR_SURFACE_NOT_BEING_RENDERED. Code: 0x88980802" },	//	DCOMPOSITION_ERROR_SURFACE_NOT_BEING_RENDERED
			{ com_errc::onl_e_invalid_authentication_target, "Authentication target is invalid or not configured correctly. Code: 0x80860001" },	//	ONL_E_INVALID_AUTHENTICATION_TARGET
			{ com_errc::onl_e_access_denied_by_tou, "Your application cannot get the Online Id properties due to the Terms of Use accepted by the user. Code: 0x80860002" },	//	ONL_E_ACCESS_DENIED_BY_TOU
			{ com_errc::onl_e_invalid_application, "The application requesting authentication tokens is either disabled or incorrectly configured. Code: 0x80860003" },	//	ONL_E_INVALID_APPLICATION
			{ com_errc::onl_e_password_update_required, "Online Id password must be updated before signin. Code: 0x80860004" },	//	ONL_E_PASSWORD_UPDATE_REQUIRED
			{ com_errc::onl_e_account_update_required, "Online Id account properties must be updated before signin. Code: 0x80860005" },	//	ONL_E_ACCOUNT_UPDATE_REQUIRED
			{ com_errc::onl_e_forcesignin, "To help protect your Online Id account you must signin again. Code: 0x80860006" },	//	ONL_E_FORCESIGNIN
			{ com_errc::onl_e_account_locked, "Online Id account was locked because there have been too many attempts to sign in. Code: 0x80860007" },	//	ONL_E_ACCOUNT_LOCKED
			{ com_errc::onl_e_parental_consent_required, "Online Id account requires parental consent before proceeding. Code: 0x80860008" },	//	ONL_E_PARENTAL_CONSENT_REQUIRED
			{ com_errc::onl_e_email_verification_required, "Online Id signin name is not yet verified. Email verification is required before signin. Code: 0x80860009" },	//	ONL_E_EMAIL_VERIFICATION_REQUIRED
			{ com_errc::onl_e_account_suspended_comproimise, "We have noticed some unusual activity in your Online Id account. Your action is needed to make sure no one else is using your account. Code: 0x8086000A" },	//	ONL_E_ACCOUNT_SUSPENDED_COMPROIMISE
			{ com_errc::onl_e_account_suspended_abuse, "We detected some suspicious activity with your Online Id account. To help protect you, we've temporarily blocked your account. Code: 0x8086000B" },	//	ONL_E_ACCOUNT_SUSPENDED_ABUSE
			{ com_errc::onl_e_action_required, "User interaction is required for authentication. Code: 0x8086000C" },	//	ONL_E_ACTION_REQUIRED
			{ com_errc::onl_connection_count_limit, "User has reached the maximum device associations per user limit. Code: 0x8086000D" },	//	ONL_CONNECTION_COUNT_LIMIT
			{ com_errc::onl_e_connected_account_can_not_signout, "Cannot sign out from the application since the user account is connected. Code: 0x8086000E" },	//	ONL_E_CONNECTED_ACCOUNT_CAN_NOT_SIGNOUT
			{ com_errc::onl_e_user_authentication_required, "User authentication is required for this operation. Code: 0x8086000F" },	//	ONL_E_USER_AUTHENTICATION_REQUIRED
			{ com_errc::onl_e_request_throttled, "We want to make sure this is you. User interaction is required for authentication. Code: 0x80860010" },	//	ONL_E_REQUEST_THROTTLED
			{ com_errc::fa_e_max_persisted_items_reached, "The maximum number of items for the access list has been reached. An item must be removed before another item is added. Code: 0x80270220" },	//	FA_E_MAX_PERSISTED_ITEMS_REACHED
			{ com_errc::fa_e_homegroup_not_available, "Cannot access Homegroup. Homegroup may not be set up or may have encountered an error. Code: 0x80270222" },	//	FA_E_HOMEGROUP_NOT_AVAILABLE
			{ com_errc::e_monitor_resolution_too_low, "This app can't start because the screen resolution is below 1024x768. Choose a higher screen resolution and then try again. Code: 0x80270250" },	//	E_MONITOR_RESOLUTION_TOO_LOW
			{ com_errc::e_elevated_activation_not_supported, "This app can't be activated from an elevated context. Code: 0x80270251" },	//	E_ELEVATED_ACTIVATION_NOT_SUPPORTED
			{ com_errc::e_uac_disabled, "This app can't be activated when UAC is disabled. Code: 0x80270252" },	//	E_UAC_DISABLED
			{ com_errc::e_full_admin_not_supported, "This app can't be activated by the Built-in Administrator. Code: 0x80270253" },	//	E_FULL_ADMIN_NOT_SUPPORTED
			{ com_errc::e_application_not_registered, "This app does not support the contract specified or is not installed. Code: 0x80270254" },	//	E_APPLICATION_NOT_REGISTERED
			{ com_errc::e_multiple_extensions_for_application, "This app has mulitple extensions registered to support the specified contract. Activation by AppUserModelId is ambiguous. Code: 0x80270255" },	//	E_MULTIPLE_EXTENSIONS_FOR_APPLICATION
			{ com_errc::e_multiple_packages_for_family, "This app's package family has more than one package installed. This is not supported. Code: 0x80270256" },	//	E_MULTIPLE_PACKAGES_FOR_FAMILY
			{ com_errc::e_application_manager_not_running, "The app manager is required to activate applications, but is not running. Code: 0x80270257" },	//	E_APPLICATION_MANAGER_NOT_RUNNING
			{ com_errc::s_store_launched_for_remediation, "The Store was launched instead of the specified app because the app's package was in an invalid state. Code: 0x00270258" },	//	S_STORE_LAUNCHED_FOR_REMEDIATION
			{ com_errc::s_application_activation_error_handled_by_dialog, "This app failed to launch, but the error was handled with a dialog. Code: 0x00270259" },	//	S_APPLICATION_ACTIVATION_ERROR_HANDLED_BY_DIALOG
			{ com_errc::e_application_activation_timed_out, "The app didn't start in the required time. Code: 0x8027025A" },	//	E_APPLICATION_ACTIVATION_TIMED_OUT
			{ com_errc::e_application_activation_exec_failure, "The app didn't start. Code: 0x8027025B" },	//	E_APPLICATION_ACTIVATION_EXEC_FAILURE
			{ com_errc::e_application_temporary_license_error, "This app failed to launch because of an issue with its license. Please try again in a moment. Code: 0x8027025C" },	//	E_APPLICATION_TEMPORARY_LICENSE_ERROR
			{ com_errc::e_application_trial_license_expired, "This app failed to launch because its trial license has expired. Code: 0x8027025D" },	//	E_APPLICATION_TRIAL_LICENSE_EXPIRED
			{ com_errc::e_skydrive_root_target_file_system_not_supported, "Please choose a folder on a drive that's formatted with the NTFS file system. Code: 0x80270260" },	//	E_SKYDRIVE_ROOT_TARGET_FILE_SYSTEM_NOT_SUPPORTED
			{ com_errc::e_skydrive_root_target_overlap, "This location is already being used. Please choose a different location. Code: 0x80270261" },	//	E_SKYDRIVE_ROOT_TARGET_OVERLAP
			{ com_errc::e_skydrive_root_target_cannot_index, "This location cannot be indexed. Please choose a different location. Code: 0x80270262" },	//	E_SKYDRIVE_ROOT_TARGET_CANNOT_INDEX
			{ com_errc::e_skydrive_file_not_uploaded, "Sorry, the action couldn't be completed because the file hasn't finished uploading. Try again later. Code: 0x80270263" },	//	E_SKYDRIVE_FILE_NOT_UPLOADED
			{ com_errc::e_skydrive_update_availability_fail, "Sorry, the action couldn't be completed. Code: 0x80270264" },	//	E_SKYDRIVE_UPDATE_AVAILABILITY_FAIL
			{ com_errc::e_skydrive_root_target_volume_root_not_supported, "This content can only be moved to a folder. To move the content to this drive, please choose or create a folder. Code: 0x80270265" },	//	E_SKYDRIVE_ROOT_TARGET_VOLUME_ROOT_NOT_SUPPORTED
			{ com_errc::e_syncengine_file_size_over_limit, "The file size is larger than supported by the sync engine. Code: 0x8802B001" },	//	E_SYNCENGINE_FILE_SIZE_OVER_LIMIT
			{ com_errc::e_syncengine_file_size_exceeds_remaining_quota, "The file cannot be uploaded because it doesn't fit in the user's available service provided storage space. Code: 0x8802B002" },	//	E_SYNCENGINE_FILE_SIZE_EXCEEDS_REMAINING_QUOTA
			{ com_errc::e_syncengine_unsupported_file_name, "The file name contains invalid characters. Code: 0x8802B003" },	//	E_SYNCENGINE_UNSUPPORTED_FILE_NAME
			{ com_errc::e_syncengine_folder_item_count_limit_exceeded, "The maximum file count has been reached for this folder in the sync engine. Code: 0x8802B004" },	//	E_SYNCENGINE_FOLDER_ITEM_COUNT_LIMIT_EXCEEDED
			{ com_errc::e_syncengine_file_sync_partner_error, "The file sync has been delegated to another program and has run into an issue. Code: 0x8802B005" },	//	E_SYNCENGINE_FILE_SYNC_PARTNER_ERROR
			{ com_errc::e_syncengine_sync_paused_by_service, "Sync has been delayed due to a throttling request from the service. Code: 0x8802B006" },	//	E_SYNCENGINE_SYNC_PAUSED_BY_SERVICE
			{ com_errc::e_syncengine_file_identifier_unknown, "We can't seem to find that file. Please try again later. Code: 0x8802C002" },	//	E_SYNCENGINE_FILE_IDENTIFIER_UNKNOWN
			{ com_errc::e_syncengine_service_authentication_failed, "The account you're signed in with doesn't have permission to open this file. Code: 0x8802C003" },	//	E_SYNCENGINE_SERVICE_AUTHENTICATION_FAILED
			{ com_errc::e_syncengine_unknown_service_error, "There was a problem connecting to the service. Please try again later. Code: 0x8802C004" },	//	E_SYNCENGINE_UNKNOWN_SERVICE_ERROR
			{ com_errc::e_syncengine_service_returned_unexpected_size, "Sorry, there was a problem downloading the file. Code: 0x8802C005" },	//	E_SYNCENGINE_SERVICE_RETURNED_UNEXPECTED_SIZE
			{ com_errc::e_syncengine_request_blocked_by_service, "We're having trouble downloading the file right now. Please try again later. Code: 0x8802C006" },	//	E_SYNCENGINE_REQUEST_BLOCKED_BY_SERVICE
			{ com_errc::e_syncengine_request_blocked_due_to_client_error, "We're having trouble downloading the file right now. Please try again later. Code: 0x8802C007" },	//	E_SYNCENGINE_REQUEST_BLOCKED_DUE_TO_CLIENT_ERROR
			{ com_errc::e_syncengine_folder_inaccessible, "The sync engine does not have permissions to access a local folder under the sync root. Code: 0x8802D001" },	//	E_SYNCENGINE_FOLDER_INACCESSIBLE
			{ com_errc::e_syncengine_unsupported_folder_name, "The folder name contains invalid characters. Code: 0x8802D002" },	//	E_SYNCENGINE_UNSUPPORTED_FOLDER_NAME
			{ com_errc::e_syncengine_unsupported_market, "The sync engine is not allowed to run in your current market. Code: 0x8802D003" },	//	E_SYNCENGINE_UNSUPPORTED_MARKET
			{ com_errc::e_syncengine_path_length_limit_exceeded, "All files and folders can't be uploaded because a path of a file or folder is too long. Code: 0x8802D004" },	//	E_SYNCENGINE_PATH_LENGTH_LIMIT_EXCEEDED
			{ com_errc::e_syncengine_remote_path_length_limit_exceeded, "All file and folders cannot be synchronized because a path of a file or folder would exceed the local path limit. Code: 0x8802D005" },	//	E_SYNCENGINE_REMOTE_PATH_LENGTH_LIMIT_EXCEEDED
			{ com_errc::e_syncengine_client_update_needed, "Updates are needed in order to use the sync engine. Code: 0x8802D006" },	//	E_SYNCENGINE_CLIENT_UPDATE_NEEDED
			{ com_errc::e_syncengine_proxy_authentication_required, "The sync engine needs to authenticate with a proxy server. Code: 0x8802D007" },	//	E_SYNCENGINE_PROXY_AUTHENTICATION_REQUIRED
			{ com_errc::e_syncengine_storage_service_provisioning_failed, "There was a problem setting up the storage services for the account. Code: 0x8802D008" },	//	E_SYNCENGINE_STORAGE_SERVICE_PROVISIONING_FAILED
			{ com_errc::e_syncengine_unsupported_reparse_point, "Files can't be uploaded because there's an unsupported reparse point. Code: 0x8802D009" },	//	E_SYNCENGINE_UNSUPPORTED_REPARSE_POINT
			{ com_errc::e_syncengine_storage_service_blocked, "The service has blocked your account from accessing the storage service. Code: 0x8802D00A" },	//	E_SYNCENGINE_STORAGE_SERVICE_BLOCKED
			{ com_errc::e_syncengine_folder_in_redirection, "The action can't be performed right now because this folder is being moved. Please try again later. Code: 0x8802D00B" },	//	E_SYNCENGINE_FOLDER_IN_REDIRECTION
			{ com_errc::eas_e_policy_not_managed_by_os, "Windows cannot evaluate this EAS policy since this is not managed by the operating system. Code: 0x80550001" },	//	EAS_E_POLICY_NOT_MANAGED_BY_OS
			{ com_errc::eas_e_policy_compliant_with_actions, "The system can be made compliant to this EAS policy if certain actions are performed by the user. Code: 0x80550002" },	//	EAS_E_POLICY_COMPLIANT_WITH_ACTIONS
			{ com_errc::eas_e_requested_policy_not_enforceable, "The EAS policy being evaluated cannot be enforced by the system. Code: 0x80550003" },	//	EAS_E_REQUESTED_POLICY_NOT_ENFORCEABLE
			{ com_errc::eas_e_current_user_has_blank_password, "EAS password policies for the user cannot be evaluated as the user has a blank password. Code: 0x80550004" },	//	EAS_E_CURRENT_USER_HAS_BLANK_PASSWORD
			{ com_errc::eas_e_requested_policy_password_expiration_incompatible, "EAS password expiration policy cannot be satisfied as the password expiration interval is less than the minimum password interval of the system. Code: 0x80550005" },	//	EAS_E_REQUESTED_POLICY_PASSWORD_EXPIRATION_INCOMPATIBLE
			{ com_errc::eas_e_user_cannot_change_password, "The user is not allowed to change her password. Code: 0x80550006" },	//	EAS_E_USER_CANNOT_CHANGE_PASSWORD
			{ com_errc::eas_e_admins_have_blank_password, "EAS password policies cannot be evaluated as one or more admins have blank passwords. Code: 0x80550007" },	//	EAS_E_ADMINS_HAVE_BLANK_PASSWORD
			{ com_errc::eas_e_admins_cannot_change_password, "One or more admins are not allowed to change their password. Code: 0x80550008" },	//	EAS_E_ADMINS_CANNOT_CHANGE_PASSWORD
			{ com_errc::eas_e_local_controlled_users_cannot_change_password, "There are other standard users present who are not allowed to change their password. Code: 0x80550009" },	//	EAS_E_LOCAL_CONTROLLED_USERS_CANNOT_CHANGE_PASSWORD
			{ com_errc::eas_e_password_policy_not_enforceable_for_connected_admins, "The EAS password policy cannot be enforced by the connected account provider of at least one administrator. Code: 0x8055000A" },	//	EAS_E_PASSWORD_POLICY_NOT_ENFORCEABLE_FOR_CONNECTED_ADMINS
			{ com_errc::eas_e_connected_admins_need_to_change_password, "There is at least one administrator whose connected account password needs to be changed for EAS password policy compliance. Code: 0x8055000B" },	//	EAS_E_CONNECTED_ADMINS_NEED_TO_CHANGE_PASSWORD
			{ com_errc::eas_e_password_policy_not_enforceable_for_current_connected_user, "The EAS password policy cannot be enforced by the connected account provider of the current user. Code: 0x8055000C" },	//	EAS_E_PASSWORD_POLICY_NOT_ENFORCEABLE_FOR_CURRENT_CONNECTED_USER
			{ com_errc::eas_e_current_connected_user_need_to_change_password, "The connected account password of the current user needs to be changed for EAS password policy compliance. Code: 0x8055000D" },	//	EAS_E_CURRENT_CONNECTED_USER_NEED_TO_CHANGE_PASSWORD
			{ com_errc::web_e_unsupported_format, "Unsupported format. Code: 0x83750001" },	//	WEB_E_UNSUPPORTED_FORMAT
			{ com_errc::web_e_invalid_xml, "Invalid XML. Code: 0x83750002" },	//	WEB_E_INVALID_XML
			{ com_errc::web_e_missing_required_element, "Missing required element. Code: 0x83750003" },	//	WEB_E_MISSING_REQUIRED_ELEMENT
			{ com_errc::web_e_missing_required_attribute, "Missing required attribute. Code: 0x83750004" },	//	WEB_E_MISSING_REQUIRED_ATTRIBUTE
			{ com_errc::web_e_unexpected_content, "Unexpected content. Code: 0x83750005" },	//	WEB_E_UNEXPECTED_CONTENT
			{ com_errc::web_e_resource_too_large, "Resource too large. Code: 0x83750006" },	//	WEB_E_RESOURCE_TOO_LARGE
			{ com_errc::web_e_invalid_json_string, "Invalid JSON string. Code: 0x83750007" },	//	WEB_E_INVALID_JSON_STRING
			{ com_errc::web_e_invalid_json_number, "Invalid JSON number. Code: 0x83750008" },	//	WEB_E_INVALID_JSON_NUMBER
			{ com_errc::web_e_json_value_not_found, "JSON value not found. Code: 0x83750009" },	//	WEB_E_JSON_VALUE_NOT_FOUND
			{ com_errc::http_e_status_unexpected, "Unexpected HTTP status code. Code: 0x80190001" },	//	HTTP_E_STATUS_UNEXPECTED
			{ com_errc::http_e_status_unexpected_redirection, "Unexpected redirection status code (3xx). Code: 0x80190003" },	//	HTTP_E_STATUS_UNEXPECTED_REDIRECTION
			{ com_errc::http_e_status_unexpected_client_error, "Unexpected client error status code (4xx). Code: 0x80190004" },	//	HTTP_E_STATUS_UNEXPECTED_CLIENT_ERROR
			{ com_errc::http_e_status_unexpected_server_error, "Unexpected server error status code (5xx). Code: 0x80190005" },	//	HTTP_E_STATUS_UNEXPECTED_SERVER_ERROR
			{ com_errc::http_e_status_ambiguous, "Multiple choices (300). Code: 0x8019012C" },	//	HTTP_E_STATUS_AMBIGUOUS
			{ com_errc::http_e_status_moved, "Moved permanently (301). Code: 0x8019012D" },	//	HTTP_E_STATUS_MOVED
			{ com_errc::http_e_status_redirect, "Found (302). Code: 0x8019012E" },	//	HTTP_E_STATUS_REDIRECT
			{ com_errc::http_e_status_redirect_method, "See Other (303). Code: 0x8019012F" },	//	HTTP_E_STATUS_REDIRECT_METHOD
			{ com_errc::http_e_status_not_modified, "Not modified (304). Code: 0x80190130" },	//	HTTP_E_STATUS_NOT_MODIFIED
			{ com_errc::http_e_status_use_proxy, "Use proxy (305). Code: 0x80190131" },	//	HTTP_E_STATUS_USE_PROXY
			{ com_errc::http_e_status_redirect_keep_verb, "Temporary redirect (307). Code: 0x80190133" },	//	HTTP_E_STATUS_REDIRECT_KEEP_VERB
			{ com_errc::http_e_status_bad_request, "Bad request (400). Code: 0x80190190" },	//	HTTP_E_STATUS_BAD_REQUEST
			{ com_errc::http_e_status_denied, "Unauthorized (401). Code: 0x80190191" },	//	HTTP_E_STATUS_DENIED
			{ com_errc::http_e_status_payment_req, "Payment required (402). Code: 0x80190192" },	//	HTTP_E_STATUS_PAYMENT_REQ
			{ com_errc::http_e_status_forbidden, "Forbidden (403). Code: 0x80190193" },	//	HTTP_E_STATUS_FORBIDDEN
			{ com_errc::http_e_status_not_found, "Not found (404). Code: 0x80190194" },	//	HTTP_E_STATUS_NOT_FOUND
			{ com_errc::http_e_status_bad_method, "Method not allowed (405). Code: 0x80190195" },	//	HTTP_E_STATUS_BAD_METHOD
			{ com_errc::http_e_status_none_acceptable, "Not acceptable (406). Code: 0x80190196" },	//	HTTP_E_STATUS_NONE_ACCEPTABLE
			{ com_errc::http_e_status_proxy_auth_req, "Proxy authentication required (407). Code: 0x80190197" },	//	HTTP_E_STATUS_PROXY_AUTH_REQ
			{ com_errc::http_e_status_request_timeout, "Request timeout (408). Code: 0x80190198" },	//	HTTP_E_STATUS_REQUEST_TIMEOUT
			{ com_errc::http_e_status_conflict, "Conflict (409). Code: 0x80190199" },	//	HTTP_E_STATUS_CONFLICT
			{ com_errc::http_e_status_gone, "Gone (410). Code: 0x8019019A" },	//	HTTP_E_STATUS_GONE
			{ com_errc::http_e_status_length_required, "Length required (411). Code: 0x8019019B" },	//	HTTP_E_STATUS_LENGTH_REQUIRED
			{ com_errc::http_e_status_precond_failed, "Precondition failed (412). Code: 0x8019019C" },	//	HTTP_E_STATUS_PRECOND_FAILED
			{ com_errc::http_e_status_request_too_large, "Request entity too large (413). Code: 0x8019019D" },	//	HTTP_E_STATUS_REQUEST_TOO_LARGE
			{ com_errc::http_e_status_uri_too_long, "Request-URI too long (414). Code: 0x8019019E" },	//	HTTP_E_STATUS_URI_TOO_LONG
			{ com_errc::http_e_status_unsupported_media, "Unsupported media type (415). Code: 0x8019019F" },	//	HTTP_E_STATUS_UNSUPPORTED_MEDIA
			{ com_errc::http_e_status_range_not_satisfiable, "Requested range not satisfiable (416). Code: 0x801901A0" },	//	HTTP_E_STATUS_RANGE_NOT_SATISFIABLE
			{ com_errc::http_e_status_expectation_failed, "Expectation failed (417). Code: 0x801901A1" },	//	HTTP_E_STATUS_EXPECTATION_FAILED
			{ com_errc::http_e_status_server_error, "Internal server error (500). Code: 0x801901F4" },	//	HTTP_E_STATUS_SERVER_ERROR
			{ com_errc::http_e_status_not_supported, "Not implemented (501). Code: 0x801901F5" },	//	HTTP_E_STATUS_NOT_SUPPORTED
			{ com_errc::http_e_status_bad_gateway, "Bad gateway (502). Code: 0x801901F6" },	//	HTTP_E_STATUS_BAD_GATEWAY
			{ com_errc::http_e_status_service_unavail, "Service unavailable (503). Code: 0x801901F7" },	//	HTTP_E_STATUS_SERVICE_UNAVAIL
			{ com_errc::http_e_status_gateway_timeout, "Gateway timeout (504). Code: 0x801901F8" },	//	HTTP_E_STATUS_GATEWAY_TIMEOUT
			{ com_errc::http_e_status_version_not_sup, "Version not supported (505). Code: 0x801901F9" },	//	HTTP_E_STATUS_VERSION_NOT_SUP
			{ com_errc::e_invalid_protocol_operation, "Invalid operation performed by the protocol. Code: 0x83760001" },	//	E_INVALID_PROTOCOL_OPERATION
			{ com_errc::e_invalid_protocol_format, "Invalid data format for the specific protocol operation. Code: 0x83760002" },	//	E_INVALID_PROTOCOL_FORMAT
			{ com_errc::e_protocol_extensions_not_supported, "Protocol extensions are not supported. Code: 0x83760003" },	//	E_PROTOCOL_EXTENSIONS_NOT_SUPPORTED
			{ com_errc::e_subprotocol_not_supported, "Subrotocol is not supported. Code: 0x83760004" },	//	E_SUBPROTOCOL_NOT_SUPPORTED
			{ com_errc::e_protocol_version_not_supported, "Incorrect protocol version. Code: 0x83760005" },	//	E_PROTOCOL_VERSION_NOT_SUPPORTED
			{ com_errc::input_e_out_of_order, "Input data cannot be processed in the non-chronological order. Code: 0x80400000" },	//	INPUT_E_OUT_OF_ORDER
			{ com_errc::input_e_reentrancy, "Requested operation cannot be performed inside the callback or event handler. Code: 0x80400001" },	//	INPUT_E_REENTRANCY
			{ com_errc::input_e_multimodal, "Input cannot be processed because there is ongoing interaction with another pointer type. Code: 0x80400002" },	//	INPUT_E_MULTIMODAL
			{ com_errc::input_e_packet, "One or more fields in the input packet are invalid. Code: 0x80400003" },	//	INPUT_E_PACKET
			{ com_errc::input_e_frame, "Packets in the frame are inconsistent. Either pointer ids are not unique or there is a discrepancy in timestamps, frame ids, pointer types or source devices. Code: 0x80400004" },	//	INPUT_E_FRAME
			{ com_errc::input_e_history, "The history of frames is inconsistent. Pointer ids, types, source devices don't match, or frame ids are not unique, or timestamps are out of order. Code: 0x80400005" },	//	INPUT_E_HISTORY
			{ com_errc::input_e_device_info, "Failed to retrieve information about the input device. Code: 0x80400006" },	//	INPUT_E_DEVICE_INFO
			{ com_errc::input_e_transform, "Coordinate system transformation failed to transform the data. Code: 0x80400007" },	//	INPUT_E_TRANSFORM
			{ com_errc::input_e_device_property, "The property is not supported or not reported correctly by the input device. Code: 0x80400008" },	//	INPUT_E_DEVICE_PROPERTY
			{ com_errc::inet_e_invalid_url, "The URL is invalid. Code: 0x800C0002" },	//	INET_E_INVALID_URL
			{ com_errc::inet_e_no_session, "No Internet session has been established. Code: 0x800C0003" },	//	INET_E_NO_SESSION
			{ com_errc::inet_e_cannot_connect, "Unable to connect to the target server. Code: 0x800C0004" },	//	INET_E_CANNOT_CONNECT
			{ com_errc::inet_e_resource_not_found, "The system cannot locate the resource specified. Code: 0x800C0005" },	//	INET_E_RESOURCE_NOT_FOUND
			{ com_errc::inet_e_object_not_found, "The system cannot locate the object specified. Code: 0x800C0006" },	//	INET_E_OBJECT_NOT_FOUND
			{ com_errc::inet_e_data_not_available, "No data is available for the requested resource. Code: 0x800C0007" },	//	INET_E_DATA_NOT_AVAILABLE
			{ com_errc::inet_e_download_failure, "The download of the specified resource has failed. Code: 0x800C0008" },	//	INET_E_DOWNLOAD_FAILURE
			{ com_errc::inet_e_authentication_required, "Authentication is required to access this resource. Code: 0x800C0009" },	//	INET_E_AUTHENTICATION_REQUIRED
			{ com_errc::inet_e_no_valid_media, "The server could not recognize the provided mime type. Code: 0x800C000A" },	//	INET_E_NO_VALID_MEDIA
			{ com_errc::inet_e_connection_timeout, "The operation was timed out. Code: 0x800C000B" },	//	INET_E_CONNECTION_TIMEOUT
			{ com_errc::inet_e_invalid_request, "The server did not understand the request, or the request was invalid. Code: 0x800C000C" },	//	INET_E_INVALID_REQUEST
			{ com_errc::inet_e_unknown_protocol, "The specified protocol is unknown. Code: 0x800C000D" },	//	INET_E_UNKNOWN_PROTOCOL
			{ com_errc::inet_e_security_problem, "A security problem occurred. Code: 0x800C000E" },	//	INET_E_SECURITY_PROBLEM
			{ com_errc::inet_e_cannot_load_data, "The system could not load the persisted data. Code: 0x800C000F" },	//	INET_E_CANNOT_LOAD_DATA
			{ com_errc::inet_e_cannot_instantiate_object, "Unable to instantiate the object. Code: 0x800C0010" },	//	INET_E_CANNOT_INSTANTIATE_OBJECT
			{ com_errc::inet_e_invalid_certificate, "Security certificate required to access this resource is invalid. Code: 0x800C0019" },	//	INET_E_INVALID_CERTIFICATE
			{ com_errc::inet_e_redirect_failed, "A redirection problem occurred. Code: 0x800C0014" },	//	INET_E_REDIRECT_FAILED
			{ com_errc::inet_e_redirect_to_dir, "The requested resource is a directory, not a file. Code: 0x800C0015" },	//	INET_E_REDIRECT_TO_DIR
			{ com_errc::error_dbg_create_process_failure_lockdown, "Could not create new process from ARM architecture device. Code: 0x80B00001" },	//	ERROR_DBG_CREATE_PROCESS_FAILURE_LOCKDOWN
			{ com_errc::error_dbg_attach_process_failure_lockdown, "Could not attach to the application process from ARM architecture device. Code: 0x80B00002" },	//	ERROR_DBG_ATTACH_PROCESS_FAILURE_LOCKDOWN
			{ com_errc::error_dbg_connect_server_failure_lockdown, "Could not connect to dbgsrv server from ARM architecture device. Code: 0x80B00003" },	//	ERROR_DBG_CONNECT_SERVER_FAILURE_LOCKDOWN
			{ com_errc::error_dbg_start_server_failure_lockdown, "Could not start dbgsrv server from ARM architecture device. Code: 0x80B00004" },	//	ERROR_DBG_START_SERVER_FAILURE_LOCKDOWN
			{ com_errc::error_io_preempted, "The operation was preempted by a higher priority operation. It must be resumed later. Code: 0x89010001" },	//	ERROR_IO_PREEMPTED
			{ com_errc::jscript_e_cantexecute, "Function could not execute because it was deleted or garbage collected. Code: 0x89020001" },	//	JSCRIPT_E_CANTEXECUTE
			{ com_errc::wep_e_not_provisioned_on_all_volumes, "One or more fixed volumes are not provisioned with the 3rd party encryption providers to support device encryption. Enable encryption with the 3rd party provider to comply with policy. Code: 0x88010001" },	//	WEP_E_NOT_PROVISIONED_ON_ALL_VOLUMES
			{ com_errc::wep_e_fixed_data_not_supported, "This computer is not fully encrypted. There are fixed volumes present which are not supported for encryption. Code: 0x88010002" },	//	WEP_E_FIXED_DATA_NOT_SUPPORTED
			{ com_errc::wep_e_hardware_not_compliant, "This computer does not meet the hardware requirements to support device encryption with the installed 3rd party provider. Code: 0x88010003" },	//	WEP_E_HARDWARE_NOT_COMPLIANT
			{ com_errc::wep_e_lock_not_configured, "This computer cannot support device encryption because the requisites for the device lock feature are not configured. Code: 0x88010004" },	//	WEP_E_LOCK_NOT_CONFIGURED
			{ com_errc::wep_e_protection_suspended, "Protection is enabled on this volume but is not in the active state. Code: 0x88010005" },	//	WEP_E_PROTECTION_SUSPENDED
			{ com_errc::wep_e_no_license, "The 3rd party provider has been installed, but cannot activate encryption beacuse a license has not been activated. Code: 0x88010006" },	//	WEP_E_NO_LICENSE
			{ com_errc::wep_e_os_not_protected, "The operating system drive is not protected by 3rd party drive encryption. Code: 0x88010007" },	//	WEP_E_OS_NOT_PROTECTED
			{ com_errc::wep_e_unexpected_fail, "Unexpected failure was encountered while calling into the 3rd Party drive encryption plugin. Code: 0x88010008" },	//	WEP_E_UNEXPECTED_FAIL
			{ com_errc::wep_e_buffer_too_large, "The input buffer size for the lockout metadata used by the 3rd party drive encryption is too large. Code: 0x88010009" },	//	WEP_E_BUFFER_TOO_LARGE
			{ com_errc::error_svhdx_error_stored, "The proper error code with sense data was stored on server side. Code: 0xC05C0000" },	//	ERROR_SVHDX_ERROR_STORED
			{ com_errc::error_svhdx_error_not_available, "The requested error data is not available on the server. Code: 0xC05CFF00" },	//	ERROR_SVHDX_ERROR_NOT_AVAILABLE
			{ com_errc::error_svhdx_unit_attention_available, "Unit Attention data is available for the initiator to query. Code: 0xC05CFF01" },	//	ERROR_SVHDX_UNIT_ATTENTION_AVAILABLE
			{ com_errc::error_svhdx_unit_attention_capacity_data_changed, "The data capacity of the device has changed, resulting in a Unit Attention condition. Code: 0xC05CFF02" },	//	ERROR_SVHDX_UNIT_ATTENTION_CAPACITY_DATA_CHANGED
			{ com_errc::error_svhdx_unit_attention_reservations_preempted, "A previous operation resulted in this initiator's reservations being preempted, resulting in a Unit Attention condition. Code: 0xC05CFF03" },	//	ERROR_SVHDX_UNIT_ATTENTION_RESERVATIONS_PREEMPTED
			{ com_errc::error_svhdx_unit_attention_reservations_released, "A previous operation resulted in this initiator's reservations being released, resulting in a Unit Attention condition. Code: 0xC05CFF04" },	//	ERROR_SVHDX_UNIT_ATTENTION_RESERVATIONS_RELEASED
			{ com_errc::error_svhdx_unit_attention_registrations_preempted, "A previous operation resulted in this initiator's registrations being preempted, resulting in a Unit Attention condition. Code: 0xC05CFF05" },	//	ERROR_SVHDX_UNIT_ATTENTION_REGISTRATIONS_PREEMPTED
			{ com_errc::error_svhdx_unit_attention_operating_definition_changed, "The data storage format of the device has changed, resulting in a Unit Attention condition. Code: 0xC05CFF06" },	//	ERROR_SVHDX_UNIT_ATTENTION_OPERATING_DEFINITION_CHANGED
			{ com_errc::error_svhdx_reservation_conflict, "The current initiator is not allowed to perform the SCSI command because of a reservation conflict. Code: 0xC05CFF07" },	//	ERROR_SVHDX_RESERVATION_CONFLICT
			{ com_errc::error_svhdx_wrong_file_type, "Multiple virtual machines sharing a virtual hard disk is supported only on Fixed or Dynamic VHDX format virtual hard disks. Code: 0xC05CFF08" },	//	ERROR_SVHDX_WRONG_FILE_TYPE
			{ com_errc::error_svhdx_version_mismatch, "The server version does not match the requested version. Code: 0xC05CFF09" },	//	ERROR_SVHDX_VERSION_MISMATCH
			{ com_errc::error_vhd_shared, "The requested operation cannot be performed on the virtual disk as it is currently used in shared mode. Code: 0xC05CFF0A" },	//	ERROR_VHD_SHARED
			{ com_errc::wininet_e_out_of_handles, "No more Internet handles can be allocated. Code: 0x80072EE1" },	//	WININET_E_OUT_OF_HANDLES
			{ com_errc::wininet_e_timeout, "The operation timed out. Code: 0x80072EE2" },	//	WININET_E_TIMEOUT
			{ com_errc::wininet_e_extended_error, "The server returned extended information. Code: 0x80072EE3" },	//	WININET_E_EXTENDED_ERROR
			{ com_errc::wininet_e_internal_error, "An internal error occurred in the Microsoft Internet extensions. Code: 0x80072EE4" },	//	WININET_E_INTERNAL_ERROR
			{ com_errc::wininet_e_invalid_url, "The URL is invalid. Code: 0x80072EE5" },	//	WININET_E_INVALID_URL
			{ com_errc::wininet_e_unrecognized_scheme, "The URL does not use a recognized protocol. Code: 0x80072EE6" },	//	WININET_E_UNRECOGNIZED_SCHEME
			{ com_errc::wininet_e_name_not_resolved, "The server name or address could not be resolved. Code: 0x80072EE7" },	//	WININET_E_NAME_NOT_RESOLVED
			{ com_errc::wininet_e_protocol_not_found, "A protocol with the required capabilities was not found. Code: 0x80072EE8" },	//	WININET_E_PROTOCOL_NOT_FOUND
			{ com_errc::wininet_e_invalid_option, "The option is invalid. Code: 0x80072EE9" },	//	WININET_E_INVALID_OPTION
			{ com_errc::wininet_e_bad_option_length, "The length is incorrect for the option type. Code: 0x80072EEA" },	//	WININET_E_BAD_OPTION_LENGTH
			{ com_errc::wininet_e_option_not_settable, "The option value cannot be set. Code: 0x80072EEB" },	//	WININET_E_OPTION_NOT_SETTABLE
			{ com_errc::wininet_e_shutdown, "Microsoft Internet Extension support has been shut down. Code: 0x80072EEC" },	//	WININET_E_SHUTDOWN
			{ com_errc::wininet_e_incorrect_user_name, "The user name was not allowed. Code: 0x80072EED" },	//	WININET_E_INCORRECT_USER_NAME
			{ com_errc::wininet_e_incorrect_password, "The password was not allowed. Code: 0x80072EEE" },	//	WININET_E_INCORRECT_PASSWORD
			{ com_errc::wininet_e_login_failure, "The login request was denied. Code: 0x80072EEF" },	//	WININET_E_LOGIN_FAILURE
			{ com_errc::wininet_e_invalid_operation, "The requested operation is invalid. Code: 0x80072EF0" },	//	WININET_E_INVALID_OPERATION
			{ com_errc::wininet_e_operation_cancelled, "The operation has been canceled. Code: 0x80072EF1" },	//	WININET_E_OPERATION_CANCELLED
			{ com_errc::wininet_e_incorrect_handle_type, "The supplied handle is the wrong type for the requested operation. Code: 0x80072EF2" },	//	WININET_E_INCORRECT_HANDLE_TYPE
			{ com_errc::wininet_e_incorrect_handle_state, "The handle is in the wrong state for the requested operation. Code: 0x80072EF3" },	//	WININET_E_INCORRECT_HANDLE_STATE
			{ com_errc::wininet_e_not_proxy_request, "The request cannot be made on a Proxy session. Code: 0x80072EF4" },	//	WININET_E_NOT_PROXY_REQUEST
			{ com_errc::wininet_e_registry_value_not_found, "The registry value could not be found. Code: 0x80072EF5" },	//	WININET_E_REGISTRY_VALUE_NOT_FOUND
			{ com_errc::wininet_e_bad_registry_parameter, "The registry parameter is incorrect. Code: 0x80072EF6" },	//	WININET_E_BAD_REGISTRY_PARAMETER
			{ com_errc::wininet_e_no_direct_access, "Direct Internet access is not available. Code: 0x80072EF7" },	//	WININET_E_NO_DIRECT_ACCESS
			{ com_errc::wininet_e_no_context, "No context value was supplied. Code: 0x80072EF8" },	//	WININET_E_NO_CONTEXT
			{ com_errc::wininet_e_no_callback, "No status callback was supplied. Code: 0x80072EF9" },	//	WININET_E_NO_CALLBACK
			{ com_errc::wininet_e_request_pending, "There are outstanding requests. Code: 0x80072EFA" },	//	WININET_E_REQUEST_PENDING
			{ com_errc::wininet_e_incorrect_format, "The information format is incorrect. Code: 0x80072EFB" },	//	WININET_E_INCORRECT_FORMAT
			{ com_errc::wininet_e_item_not_found, "The requested item could not be found. Code: 0x80072EFC" },	//	WININET_E_ITEM_NOT_FOUND
			{ com_errc::wininet_e_cannot_connect, "A connection with the server could not be established. Code: 0x80072EFD" },	//	WININET_E_CANNOT_CONNECT
			{ com_errc::wininet_e_connection_aborted, "The connection with the server was terminated abnormally. Code: 0x80072EFE" },	//	WININET_E_CONNECTION_ABORTED
			{ com_errc::wininet_e_connection_reset, "The connection with the server was reset. Code: 0x80072EFF" },	//	WININET_E_CONNECTION_RESET
			{ com_errc::wininet_e_force_retry, "The action must be retried. Code: 0x80072F00" },	//	WININET_E_FORCE_RETRY
			{ com_errc::wininet_e_invalid_proxy_request, "The proxy request is invalid. Code: 0x80072F01" },	//	WININET_E_INVALID_PROXY_REQUEST
			{ com_errc::wininet_e_need_ui, "User interaction is required to complete the operation. Code: 0x80072F02" },	//	WININET_E_NEED_UI
			{ com_errc::wininet_e_handle_exists, "The handle already exists. Code: 0x80072F04" },	//	WININET_E_HANDLE_EXISTS
			{ com_errc::wininet_e_sec_cert_date_invalid, "The date in the certificate is invalid or has expired. Code: 0x80072F05" },	//	WININET_E_SEC_CERT_DATE_INVALID
			{ com_errc::wininet_e_sec_cert_cn_invalid, "The host name in the certificate is invalid or does not match. Code: 0x80072F06" },	//	WININET_E_SEC_CERT_CN_INVALID
			{ com_errc::wininet_e_http_to_https_on_redir, "A redirect request will change a non-secure to a secure connection. Code: 0x80072F07" },	//	WININET_E_HTTP_TO_HTTPS_ON_REDIR
			{ com_errc::wininet_e_https_to_http_on_redir, "A redirect request will change a secure to a non-secure connection. Code: 0x80072F08" },	//	WININET_E_HTTPS_TO_HTTP_ON_REDIR
			{ com_errc::wininet_e_mixed_security, "Mixed secure and non-secure connections. Code: 0x80072F09" },	//	WININET_E_MIXED_SECURITY
			{ com_errc::wininet_e_chg_post_is_non_secure, "Changing to non-secure post. Code: 0x80072F0A" },	//	WININET_E_CHG_POST_IS_NON_SECURE
			{ com_errc::wininet_e_post_is_non_secure, "Data is being posted on a non-secure connection. Code: 0x80072F0B" },	//	WININET_E_POST_IS_NON_SECURE
			{ com_errc::wininet_e_client_auth_cert_needed, "A certificate is required to complete client authentication. Code: 0x80072F0C" },	//	WININET_E_CLIENT_AUTH_CERT_NEEDED
			{ com_errc::wininet_e_invalid_ca, "The certificate authority is invalid or incorrect. Code: 0x80072F0D" },	//	WININET_E_INVALID_CA
			{ com_errc::wininet_e_client_auth_not_setup, "Client authentication has not been correctly installed. Code: 0x80072F0E" },	//	WININET_E_CLIENT_AUTH_NOT_SETUP
			{ com_errc::wininet_e_async_thread_failed, "An error has occurred in a Wininet asynchronous thread. You may need to restart. Code: 0x80072F0F" },	//	WININET_E_ASYNC_THREAD_FAILED
			{ com_errc::wininet_e_redirect_scheme_change, "The protocol scheme has changed during a redirect operaiton. Code: 0x80072F10" },	//	WININET_E_REDIRECT_SCHEME_CHANGE
			{ com_errc::wininet_e_dialog_pending, "There are operations awaiting retry. Code: 0x80072F11" },	//	WININET_E_DIALOG_PENDING
			{ com_errc::wininet_e_retry_dialog, "The operation must be retried. Code: 0x80072F12" },	//	WININET_E_RETRY_DIALOG
			{ com_errc::wininet_e_no_new_containers, "There are no new cache containers. Code: 0x80072F13" },	//	WININET_E_NO_NEW_CONTAINERS
			{ com_errc::wininet_e_https_http_submit_redir, "A security zone check indicates the operation must be retried. Code: 0x80072F14" },	//	WININET_E_HTTPS_HTTP_SUBMIT_REDIR
			{ com_errc::wininet_e_sec_cert_errors, "The SSL certificate contains errors. Code: 0x80072F17" },	//	WININET_E_SEC_CERT_ERRORS
			{ com_errc::wininet_e_sec_cert_rev_failed, "It was not possible to connect to the revocation server or a definitive response could not be obtained. Code: 0x80072F19" },	//	WININET_E_SEC_CERT_REV_FAILED
			{ com_errc::wininet_e_header_not_found, "The requested header was not found. Code: 0x80072F76" },	//	WININET_E_HEADER_NOT_FOUND
			{ com_errc::wininet_e_downlevel_server, "The server does not support the requested protocol level. Code: 0x80072F77" },	//	WININET_E_DOWNLEVEL_SERVER
			{ com_errc::wininet_e_invalid_server_response, "The server returned an invalid or unrecognized response. Code: 0x80072F78" },	//	WININET_E_INVALID_SERVER_RESPONSE
			{ com_errc::wininet_e_invalid_header, "The supplied HTTP header is invalid. Code: 0x80072F79" },	//	WININET_E_INVALID_HEADER
			{ com_errc::wininet_e_invalid_query_request, "The request for a HTTP header is invalid. Code: 0x80072F7A" },	//	WININET_E_INVALID_QUERY_REQUEST
			{ com_errc::wininet_e_header_already_exists, "The HTTP header already exists. Code: 0x80072F7B" },	//	WININET_E_HEADER_ALREADY_EXISTS
			{ com_errc::wininet_e_redirect_failed, "The HTTP redirect request failed. Code: 0x80072F7C" },	//	WININET_E_REDIRECT_FAILED
			{ com_errc::wininet_e_security_channel_error, "An error occurred in the secure channel support. Code: 0x80072F7D" },	//	WININET_E_SECURITY_CHANNEL_ERROR
			{ com_errc::wininet_e_unable_to_cache_file, "The file could not be written to the cache. Code: 0x80072F7E" },	//	WININET_E_UNABLE_TO_CACHE_FILE
			{ com_errc::wininet_e_tcpip_not_installed, "The TCP/IP protocol is not installed properly. Code: 0x80072F7F" },	//	WININET_E_TCPIP_NOT_INSTALLED
			{ com_errc::wininet_e_disconnected, "The computer is disconnected from the network. Code: 0x80072F83" },	//	WININET_E_DISCONNECTED
			{ com_errc::wininet_e_server_unreachable, "The server is unreachable. Code: 0x80072F84" },	//	WININET_E_SERVER_UNREACHABLE
			{ com_errc::wininet_e_proxy_server_unreachable, "The proxy server is unreachable. Code: 0x80072F85" },	//	WININET_E_PROXY_SERVER_UNREACHABLE
			{ com_errc::wininet_e_bad_auto_proxy_script, "The proxy auto-configuration script is in error. Code: 0x80072F86" },	//	WININET_E_BAD_AUTO_PROXY_SCRIPT
			{ com_errc::wininet_e_unable_to_download_script, "Could not download the proxy auto-configuration script file. Code: 0x80072F87" },	//	WININET_E_UNABLE_TO_DOWNLOAD_SCRIPT
			{ com_errc::wininet_e_sec_invalid_cert, "The supplied certificate is invalid. Code: 0x80072F89" },	//	WININET_E_SEC_INVALID_CERT
			{ com_errc::wininet_e_sec_cert_revoked, "The supplied certificate has been revoked. Code: 0x80072F8A" },	//	WININET_E_SEC_CERT_REVOKED
			{ com_errc::wininet_e_failed_duetosecuritycheck, "The Dialup failed because file sharing was turned on and a failure was requested if security check was needed. Code: 0x80072F8B" },	//	WININET_E_FAILED_DUETOSECURITYCHECK
			{ com_errc::wininet_e_not_initialized, "Initialization of the WinINet API has not occurred. Code: 0x80072F8C" },	//	WININET_E_NOT_INITIALIZED
			{ com_errc::wininet_e_login_failure_display_entity_body, "Login failed and the client should display the entity body to the user. Code: 0x80072F8E" },	//	WININET_E_LOGIN_FAILURE_DISPLAY_ENTITY_BODY
			{ com_errc::wininet_e_decoding_failed, "Content decoding has failed. Code: 0x80072F8F" },	//	WININET_E_DECODING_FAILED
			{ com_errc::wininet_e_not_redirected, "The HTTP request was not redirected. Code: 0x80072F80" },	//	WININET_E_NOT_REDIRECTED
			{ com_errc::wininet_e_cookie_needs_confirmation, "A cookie from the server must be confirmed by the user. Code: 0x80072F81" },	//	WININET_E_COOKIE_NEEDS_CONFIRMATION
			{ com_errc::wininet_e_cookie_declined, "A cookie from the server has been declined acceptance. Code: 0x80072F82" },	//	WININET_E_COOKIE_DECLINED
			{ com_errc::wininet_e_redirect_needs_confirmation, "The HTTP redirect request must be confirmed by the user. Code: 0x80072F88" },	//	WININET_E_REDIRECT_NEEDS_CONFIRMATION
		};
	}

	class com_error_category final: public std::error_category
	{
	public:

		virtual const char* name() const _NOEXCEPT override
		{
			return "com";
		}

			virtual std::string message(int error_val) const override
		{
			const auto code = static_cast<com_errc>(error_val);
			const auto table_iter = std::find_if(std::cbegin(code_to_text_table), std::cend(code_to_text_table), [code](const table_item_t& item) { return item.first == code; });

			if (table_iter != std::cend(code_to_text_table))
				return table_iter->second;
			else
				return std::string{ "Code: (" } +string_utils::to_hex(error_val) + ')';
		}
	};

	const std::error_category& get_com_error_category()
	{
		static com_error_category category;
		return category;
	}
}
