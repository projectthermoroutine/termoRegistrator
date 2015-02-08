#pragma once

#define AV_SERVER_PORT_NAME L"\\SAvMFDFilterServerPort"

#define MAX_PACKET_SIZE  1024UL * 1024UL  // 1 MB
#define DEFAULT_PACKET_SIZE 65536UL  // bytes
#define REPLY_MESSAGE_MAX_SIZE 16UL // bytes
#define MIN_PACKET_SIZE 16UL // bytes

#ifdef __cplusplus
extern "C" {
#endif

typedef ULONG32 MESSAGE_SIZE_T;
typedef ULONG64 MESSAGE_ID;
typedef ULONG64 PROCESS_ID;
typedef ULONG64 THREAD_ID;
typedef ULONG32 STRING_LEN_T;
typedef ULONG32 MESSAGE_TYPE;
typedef ULONG32 AVSCAN_RESULT_T;
typedef ULONG64 SCAN_ID;
typedef ULONG64 ACCESS_TOKEN_T;
typedef ULONG32 PROCESS_PRIORITY_CLASS_T;
typedef ULONG32 THREAD_PRIORITY_T;

typedef enum _AV_MESSAGE
{
	AvMessageAcknowledge,
	AvMessageScanRequest,
	AvMessageScanResult,
	AvMessageRegisterFriendlyProccess,
	AvMessageUnregisterFriendlyProccess,
	AvMessageNotifyMinifilterUnloading,
	AvMessageProcessFile
} AV_MESSAGE;

typedef enum _AVSCAN_RESULT
{
	AvScanResultUndetermined,
	AvScanResultInfected,
	AvScanResultClean
} AVSCAN_RESULT;

typedef struct _WIDE_STRING
{
	STRING_LEN_T length; // in WCHARS
	WCHAR * str;	// Attention! This string is without null terminating character at the end.
					// Use designated field in the structure to get a correct string length.
} WIDE_STRING;

typedef struct _MESSAGE_ACKNOWLEDGE
{
	MESSAGE_ID message_id;
} MESSAGE_ACKNOWLEDGE;

typedef struct _MESSAGE_SCAN_REQUEST
{
	MESSAGE_ID message_id;
	SCAN_ID scan_id;
	ACCESS_TOKEN_T access_token;
	PROCESS_ID process_id;
	THREAD_ID thread_id;
	PROCESS_PRIORITY_CLASS_T process_priority_class;
	THREAD_PRIORITY_T thread_priority;
	WIDE_STRING file_name;
} MESSAGE_SCAN_REQUEST;

typedef struct _MESSAGE_SCAN_RESULT
{
	MESSAGE_ID message_id;
	SCAN_ID scan_id;
	AVSCAN_RESULT_T result;
} MESSAGE_SCAN_RESULT;

typedef struct _MESSAGE_REGISTER_FRIENDLY_PROCESS
{
	MESSAGE_ID message_id;
	PROCESS_ID process_id;
} MESSAGE_REGISTER_FRIENDLY_PROCESS;

typedef struct _MESSAGE_UNREGISTER_FRIENDLY_PROCESS
{
	MESSAGE_ID message_id;
	PROCESS_ID process_id;
} MESSAGE_UNREGISTER_FRIENDLY_PROCESS;

typedef struct _MESSAGE_NOTIFY_MINIFILTER_UNLOADING
{
	MESSAGE_ID message_id;
} MESSAGE_NOTIFY_MINIFILTER_UNLOADING;

typedef struct _MESSAGE_PROCESS_FILE
{
	MESSAGE_ID message_id;
	ACCESS_TOKEN_T access_token;
	PROCESS_ID process_id;
	THREAD_ID thread_id;
	PROCESS_PRIORITY_CLASS_T process_priority_class;
	THREAD_PRIORITY_T thread_priority;
	WIDE_STRING file_name;
} MESSAGE_PROCESS_FILE;

typedef void * (*MEMORY_ALLOC_FUNC)(ULONG);
typedef void (*MEMORY_FREE_FUNC)(void *);

typedef struct _MEMORY_MANAGEMENT_FUNCS
{
	MEMORY_ALLOC_FUNC alloc_func; // Please, check calling convention of passed function
	MEMORY_FREE_FUNC free_func; // Please, check calling convention of passed function
} MEMORY_MANAGEMENT_FUNCS;

BOOLEAN is_message_well_formed(const void *);
MESSAGE_SIZE_T get_message_size(const void *);
MESSAGE_TYPE get_message_type(const void *);
MESSAGE_ID get_message_id(const void *);

void * pack_message_acknowledge(const MESSAGE_ACKNOWLEDGE *, const MEMORY_MANAGEMENT_FUNCS *);
MESSAGE_ACKNOWLEDGE * unpack_message_acknowledge(const void *, const MEMORY_MANAGEMENT_FUNCS *);
void free_unpacked_acknowledge(MESSAGE_ACKNOWLEDGE *, MEMORY_FREE_FUNC);

void * pack_message_scan_request(const MESSAGE_SCAN_REQUEST *, const MEMORY_MANAGEMENT_FUNCS *);
MESSAGE_SCAN_REQUEST * unpack_message_scan_request(const void *, const MEMORY_MANAGEMENT_FUNCS *);
void free_unpacked_scan_request(MESSAGE_SCAN_REQUEST *, MEMORY_FREE_FUNC);

void * pack_message_scan_result(const MESSAGE_SCAN_RESULT *, const MEMORY_MANAGEMENT_FUNCS *);
MESSAGE_SCAN_RESULT * unpack_message_scan_result(const void *, const MEMORY_MANAGEMENT_FUNCS *);
void free_unpacked_scan_result(MESSAGE_SCAN_RESULT *, MEMORY_FREE_FUNC);

void * pack_message_register_friendly_process(const MESSAGE_REGISTER_FRIENDLY_PROCESS *, const MEMORY_MANAGEMENT_FUNCS *);
MESSAGE_REGISTER_FRIENDLY_PROCESS * unpack_message_register_friendly_process(const void *, const MEMORY_MANAGEMENT_FUNCS *);
void free_unpacked_register_friendly_process(MESSAGE_REGISTER_FRIENDLY_PROCESS *, MEMORY_FREE_FUNC);

void * pack_message_unregister_friendly_process(const MESSAGE_UNREGISTER_FRIENDLY_PROCESS *, const MEMORY_MANAGEMENT_FUNCS *);
MESSAGE_UNREGISTER_FRIENDLY_PROCESS * unpack_message_unregister_friendly_process(const void *, const MEMORY_MANAGEMENT_FUNCS *);
void free_unpacked_unregister_friendly_process(MESSAGE_UNREGISTER_FRIENDLY_PROCESS *, MEMORY_FREE_FUNC);

void * pack_message_notify_minifilter_unloading(const MESSAGE_NOTIFY_MINIFILTER_UNLOADING *, const MEMORY_MANAGEMENT_FUNCS *);
MESSAGE_NOTIFY_MINIFILTER_UNLOADING * unpack_message_notify_minifilter_unloading(const void *, const MEMORY_MANAGEMENT_FUNCS *);
void free_unpacked_notify_minifilter_unloading(MESSAGE_NOTIFY_MINIFILTER_UNLOADING *, MEMORY_FREE_FUNC);

void * pack_message_process_file(const MESSAGE_PROCESS_FILE *, const MEMORY_MANAGEMENT_FUNCS *);
MESSAGE_PROCESS_FILE * unpack_message_process_file(const void *, const MEMORY_MANAGEMENT_FUNCS *);
void free_unpacked_process_file(MESSAGE_PROCESS_FILE *, MEMORY_FREE_FUNC);

#ifdef __cplusplus
}
#endif