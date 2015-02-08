#ifdef _KERNEL_MODE
#include <ntddk.h>
#define assert(x) ASSERT(x)
#else
#include <Windows.h>
#include <assert.h>
#endif

#include "minifilter_channel.h"

#define PACK_VALUE(p, t, v)  *((t*)p) = v; p = ((t*)p) + 1;
#define UNPACK_VALUE(p, t, v)  v = *((t*)p); p = ((t*)p) + 1;

static const MESSAGE_SIZE_T BASIC_MESSAGE_SIZE = sizeof(MESSAGE_SIZE_T)+sizeof(MESSAGE_TYPE)+sizeof(MESSAGE_ID);

typedef struct _basic_message_header
{
	MESSAGE_SIZE_T size;
	MESSAGE_TYPE type;
	MESSAGE_ID id;
} basic_message_header;

static void simple_free(void * buffer, MEMORY_FREE_FUNC free_func);
static void free_string(const WIDE_STRING * str, MEMORY_FREE_FUNC free_func);
static const WCHAR * copy_string(WCHAR * dest, const WCHAR * src, STRING_LEN_T len);

static const void * unpack_basic_message_header(
	const void * buffer,
	basic_message_header * header,
	MESSAGE_TYPE expected_type);

static void * pack_basic_message(
	MESSAGE_TYPE message_type,
	MESSAGE_ID message_id,
	const MEMORY_MANAGEMENT_FUNCS * mem_funcs);

#define UNPACK_BASIC_MESSAGE(buffer, mem_funcs, message_type, type_of_result) \
	assert(buffer != 0); \
	assert(mem_funcs != 0); \
	assert(mem_funcs->alloc_func != 0); \
	if (buffer == 0 || mem_funcs == 0 || mem_funcs->alloc_func == 0) \
	{ \
		return 0; \
	} \
	basic_message_header header; \
	const void * ptr = unpack_basic_message_header(buffer, &header, message_type); \
	if (ptr == 0) \
	{ \
		return 0; \
	} \
	type_of_result * message = mem_funcs->alloc_func(sizeof(type_of_result)); \
	if (message == 0) \
	{ \
		return 0; \
	} \
	message->message_id = header.id; \
	return message;

BOOLEAN is_message_well_formed(const void * buffer)
{
	assert(buffer != 0);

	if (buffer == 0)
	{
		return FALSE;
	}

	MESSAGE_SIZE_T size = *(const MESSAGE_SIZE_T*)buffer;
	return (size >= BASIC_MESSAGE_SIZE) ? TRUE : FALSE;
}

MESSAGE_SIZE_T get_message_size(const void * buffer)
{
	assert(buffer != 0);

	if (buffer == 0)
	{
		return 0;
	}

	return *(const MESSAGE_SIZE_T*)buffer;
}

MESSAGE_TYPE get_message_type(const void * buffer)
{
	assert(is_message_well_formed(buffer));
	
	const void * ptr = ((const MESSAGE_SIZE_T *)buffer) + 1;
	return *(const MESSAGE_TYPE *)ptr;
}

MESSAGE_ID get_message_id(const void * buffer)
{
	assert(is_message_well_formed(buffer));

	const void * ptr = ((const MESSAGE_TYPE *)(((const MESSAGE_SIZE_T *)buffer) + 1)) + 1;
	return *(const MESSAGE_ID *)ptr;
}

void * pack_message_acknowledge(
	const MESSAGE_ACKNOWLEDGE * message,
	const MEMORY_MANAGEMENT_FUNCS * mem_funcs)
{
	assert(message != 0);
	if (message == 0)
	{
		return 0;
	}

	return pack_basic_message(AvMessageAcknowledge, message->message_id, mem_funcs);
}

MESSAGE_ACKNOWLEDGE * unpack_message_acknowledge(
	const void * buffer,
	const MEMORY_MANAGEMENT_FUNCS * mem_funcs)
{
	UNPACK_BASIC_MESSAGE(buffer, mem_funcs, AvMessageAcknowledge, MESSAGE_ACKNOWLEDGE);
}

void free_unpacked_acknowledge(MESSAGE_ACKNOWLEDGE * message, MEMORY_FREE_FUNC free_func)
{
	simple_free(message, free_func);
}

void * pack_message_scan_request(
	const MESSAGE_SCAN_REQUEST * message,
	const MEMORY_MANAGEMENT_FUNCS * mem_funcs)
{
	assert(message != 0);
	assert(mem_funcs != 0);
	assert(mem_funcs->alloc_func != 0);

	if (message == 0 || mem_funcs == 0 || mem_funcs->alloc_func == 0)
	{
		return 0;
	}

	MESSAGE_SIZE_T size = BASIC_MESSAGE_SIZE + sizeof(message->scan_id) +
		sizeof(message->access_token)+ sizeof(message->process_id) + sizeof(message->thread_id) +
		sizeof(message->process_priority_class) + sizeof(message->thread_priority) + sizeof(message->file_name.length);

	if (message->file_name.length != 0)
	{
		assert(message->file_name.str != 0);
		if (message->file_name.str == 0)
		{
			return 0;
		}

		size += message->file_name.length * sizeof(WCHAR);
	}
	else
	{
		assert(message->file_name.str == 0);
		if (message->file_name.str != 0)
		{
			return 0;
		}
	}

	void * buffer = mem_funcs->alloc_func(size);
	if (buffer == 0)
	{
		return 0;
	}

	void * ptr = buffer;
	PACK_VALUE(ptr, MESSAGE_SIZE_T, size);
	PACK_VALUE(ptr, MESSAGE_TYPE, AvMessageScanRequest);
	PACK_VALUE(ptr, MESSAGE_ID, message->message_id);
	PACK_VALUE(ptr, SCAN_ID, message->scan_id);
	PACK_VALUE(ptr, ACCESS_TOKEN_T, message->access_token);
	PACK_VALUE(ptr, PROCESS_ID, message->process_id);
	PACK_VALUE(ptr, THREAD_ID, message->thread_id);
	PACK_VALUE(ptr, PROCESS_PRIORITY_CLASS_T, message->process_priority_class);
	PACK_VALUE(ptr, THREAD_PRIORITY_T, message->thread_priority);
	PACK_VALUE(ptr, STRING_LEN_T, message->file_name.length);
	copy_string(ptr, message->file_name.str, message->file_name.length);

	return buffer;
}

MESSAGE_SCAN_REQUEST * unpack_message_scan_request(
	const void * buffer,
	const MEMORY_MANAGEMENT_FUNCS * mem_funcs)
{
	assert(buffer != 0);
	assert(mem_funcs != 0);
	assert(mem_funcs->alloc_func != 0);
	assert(mem_funcs->free_func != 0);

	if (buffer == 0 || mem_funcs == 0 || mem_funcs->alloc_func == 0 || mem_funcs->free_func == 0)
	{
		return 0;
	}

	basic_message_header header;
	const void * ptr = unpack_basic_message_header(buffer, &header, AvMessageScanRequest);
	if (ptr == 0)
	{
		return 0;
	}

	MESSAGE_SIZE_T min_expected_size = BASIC_MESSAGE_SIZE + sizeof(SCAN_ID) +
		sizeof(ACCESS_TOKEN_T) + sizeof(PROCESS_ID) +
		sizeof(THREAD_ID) + sizeof(PROCESS_PRIORITY_CLASS_T) +
		sizeof(THREAD_PRIORITY_T) + sizeof(STRING_LEN_T);

	if (header.size < min_expected_size)
	{
		return 0;
	}

	SCAN_ID scan_id;
	UNPACK_VALUE(ptr, SCAN_ID, scan_id);

	ACCESS_TOKEN_T access_token;
	UNPACK_VALUE(ptr, ACCESS_TOKEN_T, access_token);

	PROCESS_ID process_id;
	UNPACK_VALUE(ptr, PROCESS_ID, process_id);

	THREAD_ID thread_id;
	UNPACK_VALUE(ptr, THREAD_ID, thread_id);

	PROCESS_PRIORITY_CLASS_T process_priority_class;
	UNPACK_VALUE(ptr, PROCESS_PRIORITY_CLASS_T, process_priority_class);

	THREAD_PRIORITY_T thread_priority;
	UNPACK_VALUE(ptr, THREAD_PRIORITY_T, thread_priority);

	STRING_LEN_T str_len;
	UNPACK_VALUE(ptr, STRING_LEN_T, str_len);

	const ULONG string_len_in_bytes = str_len * sizeof(WCHAR);
	min_expected_size += string_len_in_bytes;
	if (header.size < min_expected_size)
	{
		return 0;
	}

	MESSAGE_SCAN_REQUEST * message = mem_funcs->alloc_func(sizeof(MESSAGE_SCAN_REQUEST));
	if (message == 0)
	{
		return 0;
	}

	WCHAR * str_buffer = 0;
	if (str_len != 0)
	{
		str_buffer = mem_funcs->alloc_func(string_len_in_bytes);
		if (str_buffer == 0)
		{
			mem_funcs->free_func(message);
			return 0;
		}

		copy_string(str_buffer, ptr, str_len);
	}
	
	message->message_id = header.id;
	message->scan_id = scan_id;
	message->access_token = access_token;
	message->process_id = process_id;
	message->thread_id = thread_id;
	message->process_priority_class = process_priority_class;
	message->thread_priority = thread_priority;
	message->file_name.length = str_len;
	message->file_name.str = str_buffer;

	return message;
}

void free_unpacked_scan_request(MESSAGE_SCAN_REQUEST * message, MEMORY_FREE_FUNC free_func)
{
	assert(message != 0);
	assert(free_func != 0);

	if (message == 0)
	{
		return;
	}

	free_string(&message->file_name, free_func);
	simple_free(message, free_func);
}

void * pack_message_scan_result(const MESSAGE_SCAN_RESULT * message, const MEMORY_MANAGEMENT_FUNCS * mem_funcs)
{
	assert(message != 0);
	assert(mem_funcs != 0);
	assert(mem_funcs->alloc_func != 0);

	if (message == 0 || mem_funcs == 0 || mem_funcs->alloc_func == 0)
	{
		return 0;
	}

	MESSAGE_SIZE_T size = BASIC_MESSAGE_SIZE + sizeof(message->scan_id) + sizeof(message->result);

	void * buffer = mem_funcs->alloc_func(size);
	if (buffer == 0)
	{
		return 0;
	}

	void * ptr = buffer;
	PACK_VALUE(ptr, MESSAGE_SIZE_T, size);
	PACK_VALUE(ptr, MESSAGE_TYPE, AvMessageScanResult);
	PACK_VALUE(ptr, MESSAGE_ID, message->message_id);
	PACK_VALUE(ptr, SCAN_ID, message->scan_id);
	PACK_VALUE(ptr, AVSCAN_RESULT_T, message->result);

	return buffer;
}

MESSAGE_SCAN_RESULT * unpack_message_scan_result(
	const void * buffer,
	const MEMORY_MANAGEMENT_FUNCS * mem_funcs)
{
	assert(buffer != 0);
	assert(mem_funcs != 0);
	assert(mem_funcs->alloc_func != 0);

	if (buffer == 0 || mem_funcs == 0 || mem_funcs->alloc_func == 0)
	{
		return 0;
	}

	basic_message_header header;
	const void * ptr = unpack_basic_message_header(buffer, &header, AvMessageScanResult);
	if (ptr == 0)
	{
		return 0;
	}

	MESSAGE_SIZE_T min_expected_size = BASIC_MESSAGE_SIZE + sizeof(SCAN_ID) + sizeof(AVSCAN_RESULT_T);
	if (header.size < min_expected_size)
	{
		return 0;
	}

	SCAN_ID scan_id;
	UNPACK_VALUE(ptr, SCAN_ID, scan_id);

	AVSCAN_RESULT_T scan_result;
	UNPACK_VALUE(ptr, AVSCAN_RESULT_T, scan_result);

	MESSAGE_SCAN_RESULT * message = mem_funcs->alloc_func(sizeof(MESSAGE_SCAN_RESULT));
	if (message == 0)
	{
		return 0;
	}

	message->message_id = header.id;
	message->scan_id = scan_id;
	message->result = scan_result;

	return message;
}

void free_unpacked_scan_result(MESSAGE_SCAN_RESULT * message, MEMORY_FREE_FUNC free_func)
{
	simple_free(message, free_func);
}

void * pack_message_register_friendly_process(
	const MESSAGE_REGISTER_FRIENDLY_PROCESS * message, const MEMORY_MANAGEMENT_FUNCS * mem_funcs)
{
	assert(message != 0);
	assert(mem_funcs != 0);
	assert(mem_funcs->alloc_func != 0);

	if (message == 0 || mem_funcs == 0 || mem_funcs->alloc_func == 0)
	{
		return 0;
	}

	MESSAGE_SIZE_T size = BASIC_MESSAGE_SIZE + sizeof(message->process_id);

	void * buffer = mem_funcs->alloc_func(size);
	if (buffer == 0)
	{
		return 0;
	}

	void * ptr = buffer;
	PACK_VALUE(ptr, MESSAGE_SIZE_T, size);
	PACK_VALUE(ptr, MESSAGE_TYPE, AvMessageRegisterFriendlyProccess);
	PACK_VALUE(ptr, MESSAGE_ID, message->message_id);
	PACK_VALUE(ptr, PROCESS_ID, message->process_id);

	return buffer;
}

MESSAGE_REGISTER_FRIENDLY_PROCESS * unpack_message_register_friendly_process(
	const void * buffer,
	const MEMORY_MANAGEMENT_FUNCS * mem_funcs)
{
	assert(buffer != 0);
	assert(mem_funcs != 0);
	assert(mem_funcs->alloc_func != 0);

	if (buffer == 0 || mem_funcs == 0 || mem_funcs->alloc_func == 0)
	{
		return 0;
	}

	basic_message_header header;
	const void * ptr = unpack_basic_message_header(buffer, &header, AvMessageRegisterFriendlyProccess);
	if (ptr == 0)
	{
		return 0;
	}

	MESSAGE_SIZE_T min_expected_size = BASIC_MESSAGE_SIZE + sizeof(PROCESS_ID);
	if (header.size < min_expected_size)
	{
		return 0;
	}

	PROCESS_ID process_id;
	UNPACK_VALUE(ptr, PROCESS_ID, process_id);

	MESSAGE_REGISTER_FRIENDLY_PROCESS * message = mem_funcs->alloc_func(sizeof(MESSAGE_REGISTER_FRIENDLY_PROCESS));
	if (message == 0)
	{
		return 0;
	}

	message->message_id = header.id;
	message->process_id = process_id;

	return message;
}

void free_unpacked_register_friendly_process(MESSAGE_REGISTER_FRIENDLY_PROCESS * message, MEMORY_FREE_FUNC free_func)
{
	simple_free(message, free_func);
}

void * pack_message_unregister_friendly_process(
	const MESSAGE_UNREGISTER_FRIENDLY_PROCESS * message,
	const MEMORY_MANAGEMENT_FUNCS * mem_funcs)
{
	assert(message != 0);
	assert(mem_funcs != 0);
	assert(mem_funcs->alloc_func != 0);

	if (message == 0 || mem_funcs == 0 || mem_funcs->alloc_func == 0)
	{
		return 0;
	}

	MESSAGE_SIZE_T size = BASIC_MESSAGE_SIZE + sizeof(message->process_id);

	void * buffer = mem_funcs->alloc_func(size);
	if (buffer == 0)
	{
		return 0;
	}

	void * ptr = buffer;
	PACK_VALUE(ptr, MESSAGE_SIZE_T, size);
	PACK_VALUE(ptr, MESSAGE_TYPE, AvMessageUnregisterFriendlyProccess);
	PACK_VALUE(ptr, MESSAGE_ID, message->message_id);
	PACK_VALUE(ptr, PROCESS_ID, message->process_id);

	return buffer;
}

MESSAGE_UNREGISTER_FRIENDLY_PROCESS * unpack_message_unregister_friendly_process(
	const void * buffer,
	const MEMORY_MANAGEMENT_FUNCS * mem_funcs)
{
	assert(buffer != 0);
	assert(mem_funcs != 0);
	assert(mem_funcs->alloc_func != 0);

	if (buffer == 0 || mem_funcs == 0 || mem_funcs->alloc_func == 0)
	{
		return 0;
	}

	basic_message_header header;
	const void * ptr = unpack_basic_message_header(buffer, &header, AvMessageUnregisterFriendlyProccess);
	if (ptr == 0)
	{
		return 0;
	}

	MESSAGE_SIZE_T min_expected_size = BASIC_MESSAGE_SIZE + sizeof(PROCESS_ID);
	if (header.size < min_expected_size)
	{
		return 0;
	}

	PROCESS_ID process_id;
	UNPACK_VALUE(ptr, PROCESS_ID, process_id);

	MESSAGE_UNREGISTER_FRIENDLY_PROCESS * message = mem_funcs->alloc_func(sizeof(MESSAGE_UNREGISTER_FRIENDLY_PROCESS));
	if (message == 0)
	{
		return 0;
	}

	message->message_id = header.id;
	message->process_id = process_id;

	return message;
}

void free_unpacked_unregister_friendly_process(MESSAGE_UNREGISTER_FRIENDLY_PROCESS * message, MEMORY_FREE_FUNC free_func)
{
	simple_free(message, free_func);
}

void * pack_message_notify_minifilter_unloading(const MESSAGE_NOTIFY_MINIFILTER_UNLOADING * message, const MEMORY_MANAGEMENT_FUNCS * mem_funcs)
{
	assert(message != 0);
	if (message == 0)
	{
		return 0;
	}

	return pack_basic_message(AvMessageNotifyMinifilterUnloading, message->message_id, mem_funcs);
}

MESSAGE_NOTIFY_MINIFILTER_UNLOADING * unpack_message_notify_minifilter_unloading(const void * buffer, const MEMORY_MANAGEMENT_FUNCS * mem_funcs)
{
	UNPACK_BASIC_MESSAGE(buffer, mem_funcs, AvMessageNotifyMinifilterUnloading, MESSAGE_NOTIFY_MINIFILTER_UNLOADING);
}

void free_unpacked_notify_minifilter_unloading(MESSAGE_NOTIFY_MINIFILTER_UNLOADING * message, MEMORY_FREE_FUNC free_func)
{
	simple_free(message, free_func);
}

void * pack_message_process_file(const MESSAGE_PROCESS_FILE * message, const MEMORY_MANAGEMENT_FUNCS * mem_funcs)
{
	assert(message != 0);
	assert(mem_funcs != 0);
	assert(mem_funcs->alloc_func != 0);

	if (message == 0 || mem_funcs == 0 || mem_funcs->alloc_func == 0)
	{
		return 0;
	}

	MESSAGE_SIZE_T size = BASIC_MESSAGE_SIZE + sizeof(message->access_token) +
		sizeof(message->process_id) + sizeof(message->thread_id) +
		sizeof(message->thread_priority) + sizeof(message->process_priority_class) +
		sizeof(message->file_name.length);

	if (message->file_name.length != 0)
	{
		assert(message->file_name.str != 0);
		if (message->file_name.str == 0)
		{
			return 0;
		}

		size += message->file_name.length * sizeof(WCHAR);
	}
	else
	{
		assert(message->file_name.str == 0);
		if (message->file_name.str != 0)
		{
			return 0;
		}
	}

	void * buffer = mem_funcs->alloc_func(size);
	if (buffer == 0)
	{
		return 0;
	}

	void * ptr = buffer;
	PACK_VALUE(ptr, MESSAGE_SIZE_T, size);
	PACK_VALUE(ptr, MESSAGE_TYPE, AvMessageProcessFile);
	PACK_VALUE(ptr, MESSAGE_ID, message->message_id);
	PACK_VALUE(ptr, ACCESS_TOKEN_T, message->access_token);
	PACK_VALUE(ptr, PROCESS_ID, message->process_id);
	PACK_VALUE(ptr, THREAD_ID, message->thread_id);
	PACK_VALUE(ptr, PROCESS_PRIORITY_CLASS_T, message->process_priority_class);
	PACK_VALUE(ptr, THREAD_PRIORITY_T, message->thread_priority);
	PACK_VALUE(ptr, STRING_LEN_T, message->file_name.length);

	copy_string(ptr, message->file_name.str, message->file_name.length);

	return buffer;
}

MESSAGE_PROCESS_FILE * unpack_message_process_file(const void * buffer, const MEMORY_MANAGEMENT_FUNCS * mem_funcs)
{
	assert(buffer != 0);
	assert(mem_funcs != 0);
	assert(mem_funcs->alloc_func != 0);
	assert(mem_funcs->free_func != 0);

	if (buffer == 0 || mem_funcs == 0 || mem_funcs->alloc_func == 0 || mem_funcs->free_func == 0)
	{
		return 0;
	}

	basic_message_header header;
	const void * ptr = unpack_basic_message_header(buffer, &header, AvMessageProcessFile);
	if (ptr == 0)
	{
		return 0;
	}

	MESSAGE_SIZE_T min_expected_size = BASIC_MESSAGE_SIZE + sizeof(ACCESS_TOKEN_T) +
		sizeof(PROCESS_ID) + sizeof(THREAD_ID) + 
		sizeof(PROCESS_PRIORITY_CLASS_T) + sizeof(THREAD_PRIORITY_T) +
		sizeof(STRING_LEN_T);

	if (header.size < min_expected_size)
	{
		return 0;
	}

	ACCESS_TOKEN_T access_token;
	UNPACK_VALUE(ptr, ACCESS_TOKEN_T, access_token);

	PROCESS_ID process_id;
	UNPACK_VALUE(ptr, PROCESS_ID, process_id);

	THREAD_ID thread_id;
	UNPACK_VALUE(ptr, THREAD_ID, thread_id);

	PROCESS_PRIORITY_CLASS_T process_priority_class;
	UNPACK_VALUE(ptr, PROCESS_PRIORITY_CLASS_T, process_priority_class);

	THREAD_PRIORITY_T thread_priority;
	UNPACK_VALUE(ptr, THREAD_PRIORITY_T, thread_priority);

	STRING_LEN_T str_len;
	UNPACK_VALUE(ptr, STRING_LEN_T, str_len);

	const ULONG string_len_in_bytes = str_len * sizeof(WCHAR);
	min_expected_size += string_len_in_bytes;
	if (header.size < min_expected_size)
	{
		return 0;
	}

	MESSAGE_PROCESS_FILE * message = mem_funcs->alloc_func(sizeof(MESSAGE_PROCESS_FILE));
	if (message == 0)
	{
		return 0;
	}

	WCHAR * str_buffer = 0;
	if (str_len != 0)
	{
		str_buffer = mem_funcs->alloc_func(string_len_in_bytes);
		if (str_buffer == 0)
		{
			mem_funcs->free_func(message);
			return 0;
		}

		copy_string(str_buffer, ptr, str_len);
	}

	message->message_id = header.id;
	message->access_token = access_token;
	message->process_id = process_id;
	message->thread_id = thread_id;
	message->process_priority_class = process_priority_class;
	message->thread_priority = thread_priority;
	message->file_name.length = str_len;
	message->file_name.str = str_buffer;

	return message;
}

void free_unpacked_process_file(MESSAGE_PROCESS_FILE * message, MEMORY_FREE_FUNC free_func)
{
	assert(message != 0);
	assert(free_func != 0);

	if (message == 0)
	{
		return;
	}

	free_string(&message->file_name, free_func);
	simple_free(message, free_func);
}

void simple_free(void * buffer, MEMORY_FREE_FUNC free_func)
{
	assert(free_func != 0);
	if (free_func == 0)
	{
		return;
	}

	if (buffer != 0)
	{
		free_func(buffer);
	}
}

void free_string(const WIDE_STRING * str, MEMORY_FREE_FUNC free_func)
{
	assert(str != 0);
	if (str == 0)
	{
		return;
	}

	if (str->str == 0)
	{
		assert(str->length == 0);
		return;
	}

	assert(str->length != 0);
	simple_free(str->str, free_func);
}

const void * unpack_basic_message_header(
	const void * buffer,
	basic_message_header * header,
	MESSAGE_TYPE expected_type)
{
	assert(buffer != 0);
	assert(header != 0);
	
	const void * ptr = buffer;
	
	MESSAGE_SIZE_T size;
	UNPACK_VALUE(ptr, MESSAGE_SIZE_T, size);

	if (size < BASIC_MESSAGE_SIZE)
	{
		return 0;
	}

	MESSAGE_TYPE type;
	UNPACK_VALUE(ptr, MESSAGE_TYPE, type);

	assert(type == expected_type);
	if (type != expected_type)
	{
		return 0;
	}

	MESSAGE_ID id;
	UNPACK_VALUE(ptr, MESSAGE_ID, id);

	header->size = size;
	header->type = type;
	header->id = id;

	return ptr;
}

const WCHAR * copy_string(WCHAR * dest, const WCHAR * src, STRING_LEN_T len)
{
	assert(dest != 0);
	assert(src != 0);
	assert(dest != src);

	const WCHAR * const dest_end = dest + len;
	for (; dest != dest_end; ++dest, ++src)
	{
		*dest = *src;
	}

	return src;
}

void * pack_basic_message(
	MESSAGE_TYPE message_type,
	MESSAGE_ID message_id,
	const MEMORY_MANAGEMENT_FUNCS * mem_funcs)
{
	assert(mem_funcs != 0);
	assert(mem_funcs->alloc_func != 0);

	if (mem_funcs == 0 || mem_funcs->alloc_func == 0)
	{
		return 0;
	}

	const MESSAGE_SIZE_T size = BASIC_MESSAGE_SIZE;
	void * buffer = mem_funcs->alloc_func(size);
	if (buffer == 0)
	{
		return 0;
	}

	void * ptr = buffer;
	PACK_VALUE(ptr, MESSAGE_SIZE_T, size);
	PACK_VALUE(ptr, MESSAGE_TYPE, message_type);
	PACK_VALUE(ptr, MESSAGE_ID, message_id);

	return buffer;
}